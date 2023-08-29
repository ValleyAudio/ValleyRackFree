//
//  QuadOsc.cpp
//  QuadOsc - A syncronous, SIMD optimised oscillator.
//
//  Created by Dale Johnson on 02/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "QuadOsc.hpp"

ScanningQuadOsc::ScanningQuadOsc() {
    _tabSize = 0;

    __samplerate = _mm_set1_ps(44100.f);
    __nyquist = _mm_set1_ps(22050.f);
    __ones = _mm_set1_ps(1.f);
    __zeros = _mm_set1_ps(0.f);
    __twos = _mm_set1_ps(2.f);
    __1_5 = _mm_set1_ps(1.5f);
    __half = _mm_set1_ps(0.5f);
    __minus = _mm_set1_ps(-1.f);
    __fours = _mm_set1_ps(4.f);
    __quarter = _mm_set1_ps(0.25f);
    __inputPhase = __zeros;
    __eoc = __zeros;
    __syncOut = __zeros;
    __syncing = __zeros;
    __syncSource = __zeros;
    __syncState = __zeros;
    __outputLevel = __ones;
    _PMPostShape = false;
    _weakSync = false;
    _sync = false;

    __negMask = __zeros;
    __shifts = __zeros;
    __shiftsI = _mm_set1_epi32(0);
    __tabSize = __zeros;
    __tabSize_1 = __zeros;
    __aInt = _mm_set1_epi32(0);
    __bInt = _mm_set1_epi32(0);

    _shape = 0.0f;
    __shape = __zeros;
    __a = __zeros;
    __b = __zeros;
    __readPhase = __zeros;
    __dir = __ones;
    _syncMode = 0;
    setFrequency(1.f);
    setShapeMethod(0);
    _shaper.setShapeMode(0);

    __fade = _mm_set1_ps(0.f);
    __result1 = _mm_set1_ps(0.f);
    __result2 = _mm_set1_ps(0.f);
    __lowBank = _mm_set1_epi32(0);
    __highBank = _mm_set1_epi32(0);
    __numWaves = _mm_set1_epi32(0);
    __numWaves_1 = __numWaves;
}

void ScanningQuadOsc::tick() {
    // Phase modulate
    __m128 __readPhaseMask = __zeros;
    if(_PMPostShape) {
        // Shape
        __readPhase = _shaper.process(__a, __shape);
        __readPhase = _mm_clamp_ps(__readPhase, __zeros, __ones);

        // Phase Mod
        __readPhase = _mm_add_ps(__readPhase, __inputPhase);

        __negMask = _mm_cmplt_ps(__readPhase, __zeros);
        __shifts = __readPhase;
        __shiftsI = _mm_cvttps_epi32(__shifts);
        __shifts = _mm_cvtepi32_ps(__shiftsI);
        __shifts = _mm_sub_ps(__shifts, _mm_and_ps(__ones, __negMask));
        __readPhase = _mm_sub_ps(__readPhase, __shifts);
        __readPhaseMask = _mm_cmpeq_ps(__readPhase, __ones);
        __readPhase = _mm_sub_ps(__readPhase, _mm_and_ps(_mm_and_ps(__readPhaseMask, __negMask), __ones));

        __readPhase = _mm_clamp_ps(__readPhase, __zeros, __ones);
    }
    else {
        // Phase Mod
        __readPhase = _mm_add_ps(__a, __inputPhase);

        __negMask = _mm_cmplt_ps(__readPhase, __zeros);
        __shifts = __readPhase;
        __shiftsI = _mm_cvttps_epi32(__shifts);
        __shifts = _mm_cvtepi32_ps(__shiftsI);
        __shifts = _mm_sub_ps(__shifts, _mm_and_ps(__ones, __negMask));
        __readPhase = _mm_sub_ps(__readPhase, __shifts);
        __readPhaseMask = _mm_cmpeq_ps(__readPhase, __ones);
        __readPhase = _mm_sub_ps(__readPhase, _mm_and_ps(_mm_and_ps(__readPhaseMask, __negMask), __ones));

        // Shape
        __readPhase = _shaper.process(__readPhase, __shape);
        __readPhase = _mm_clamp_ps(__readPhase, __zeros, __ones);
    }

    __m128 __scaledReadPhase = _mm_mul_ps(__readPhase, __tabSize_1);

    // Prepare read positions
    __b = _mm_add_ps(__scaledReadPhase, __ones);
    __m128 __wrapMask = _mm_cmpge_ps(__b, __tabSize);
    __b = _mm_sub_ps(__b, _mm_and_ps(__tabSize, __wrapMask));
    __aInt = _mm_cvttps_epi32(__scaledReadPhase);
    __bInt = _mm_cvttps_epi32(__b);
    _mm_storeu_si128((__m128i*)_aPos, __aInt);
    _mm_storeu_si128((__m128i*)_bPos, __bInt);
    __frac = _mm_sub_ps(__scaledReadPhase, _mm_cvtepi32_ps(__aInt));

    // Do linear interpolation
    for(auto j = 0; j < 4; ++j) {
        _lowSample[j] = _wavebank[_lowBank[j] * _tabSize + _aPos[j]];
        _lowSample2[j] = _wavebank[_highBank[j] * _tabSize +_aPos[j]];
        _highSample[j] = _wavebank[_lowBank[j] * _tabSize +_bPos[j]];
        _highSample2[j] = _wavebank[_highBank[j] * _tabSize +_bPos[j]];
    }

    __lowSamp = _mm_loadu_ps(_lowSample);
    __highSamp = _mm_loadu_ps(_highSample);
    __result1 = _mm_linterp_ps(__lowSamp, __highSamp, __frac);
    __lowSamp = _mm_loadu_ps(_lowSample2);
    __highSamp = _mm_loadu_ps(_highSample2);
    __result2 = _mm_linterp_ps(__lowSamp, __highSamp, __frac);
    __output = _mm_linterp_ps(__result1, __result2, __fade);
    __output = _mm_mul_ps(__output, __outputLevel);

    // Advance wave read position
    __a = _mm_add_ps(__a, _mm_mul_ps(__stepSize, __dir));

    // Wrap wave position
    __mtMask = _mm_cmpge_ps(__a, __ones);
    __ltMask = _mm_cmplt_ps(__a, __zeros);
    __a = _mm_add_ps(__a, _mm_and_ps(__ones, __ltMask));
    __a = _mm_sub_ps(__a, _mm_and_ps(__ones, __mtMask));
    __eoc = _mm_or_ps(_mm_and_ps(__ones, __ltMask), _mm_and_ps(__ones, __mtMask));
}

void ScanningQuadOsc::resetPhase() {
    __a = __half;
}

void ScanningQuadOsc::sync(const __m128& syncSource) {
    __syncing = __syncState;
    __syncState = _mm_cmpgt_ps(syncSource, __zeros);
    __syncSource = _mm_and_ps(__syncState, _mm_andnot_ps(__syncing, __syncState));

    if(!_sync) {
        __dir = __ones;
        __outputLevel = __ones;
    }
    else {
        switch((SyncModes) _syncMode) {
            case HARD_SYNC: hardSync(__syncSource);
                break;
            case FIFTH_SYNC: fifthSync(__syncSource);
                break;
            case OCTAVE_SYNC: octaveSync(__syncSource);
                break;
            case SUB_OCTAVE_SYNC: subOctaveSync(__syncSource);
                break;
            case RISE_A_SYNC: riseASync(__syncSource);
                break;
            case RISE_B_SYNC: riseBSync(__syncSource);
                break;
            case FALL_A_SYNC: fallASync(__syncSource);
                break;
            case FALL_B_SYNC: fallBSync(__syncSource);
                break;
            case PULL_A_SYNC: pullASync(__syncSource);
                break;
            case PULL_B_SYNC: pullBSync(__syncSource);
                break;
            case PUSH_A_SYNC: pushASync(__syncSource);
                break;
            case PUSH_B_SYNC: pushBSync(__syncSource);
                break;
            case HOLD_SYNC: holdSync(__syncSource);
                break;
            case ONE_SHOT_SYNC: oneShot(__syncSource);
                break;
            case LOCK_SHOT_SYNC: lockShot(__syncSource);
                break;
            case REVERSE_SYNC: reverseSync(__syncSource);
                break;
            default: hardSync(__syncSource);
        }
    }
}

float ScanningQuadOsc::getOutput(int channel) const {
    float tempOut[4] = {0.f, 0.f, 0.f, 0.f};
    _mm_storeu_ps(tempOut, __output);
    return tempOut[channel];
}

const __m128& ScanningQuadOsc::getOutput() const {
    return __output;
}

const __m128& ScanningQuadOsc::getPhasor() const {
    return __a;
}

const __m128& ScanningQuadOsc::getStepSize() const {
    return __stepSize;
}

const __m128& ScanningQuadOsc::getShapedPhasor() const {
    return __readPhase;
}

const __m128& ScanningQuadOsc::getEOCPulse() const {
    return __eoc;
}

const __m128& ScanningQuadOsc::getDirection() const {
    return __dir;
}

void ScanningQuadOsc::setWavetable(float* wavetable, long size) {
    _wavetable = wavetable;
    __tabSize = _mm_set1_ps((float)size);
    __tabSize_1 = _mm_set1_ps((float)size - 1.f);
    calcStepSize();
}

void ScanningQuadOsc::setFrequency(float frequency) {
    __frequency = _mm_set1_ps(frequency);
    __frequency = _mm_switch_ps(__frequency, __nyquist, _mm_cmpgt_ps(__frequency, __nyquist));
    calcStepSize();
}

void ScanningQuadOsc::setFrequency(float f0, float f1, float f2, float f3) {
    __frequency = _mm_set_ps(f0, f1, f2, f3);
    __frequency = _mm_switch_ps(__frequency, __nyquist, _mm_cmpgt_ps(__frequency, __nyquist));
    calcStepSize();
}

void ScanningQuadOsc::setFrequency(const __m128& frequency) {
    __frequency = frequency;
    __frequency = _mm_switch_ps(__frequency, __nyquist, _mm_cmpgt_ps(__frequency, __nyquist));
    calcStepSize();
}

void ScanningQuadOsc::setShape(float shape) {
    _shape = shape;
    __shape = _mm_set1_ps(_shape);
}

void ScanningQuadOsc::setShape(const __m128& shape) {
    __shape = shape;
}

void ScanningQuadOsc::setShapeMethod(int shapeMethod) {
    _shaper.setShapeMode(shapeMethod);
}

void ScanningQuadOsc::setPMPostShape(bool PMPostShape) {
    _PMPostShape = PMPostShape;
}

void ScanningQuadOsc::setSyncMode(int syncMode) {
    if(_syncMode != syncMode) {
        _syncMode = syncMode;
        __outputLevel = __ones;
        onChangeSyncMode();
    }
}

void ScanningQuadOsc::enableSync(bool enableSync) {
    _sync = enableSync;
}

void ScanningQuadOsc::enableWeakSync(bool weakSync) {
    _weakSync = weakSync;
}

void ScanningQuadOsc::setSampleRate(float sampleRate) {
    __samplerate = _mm_set1_ps(sampleRate);
    __nyquist = _mm_div_ps(__samplerate, __twos);
    calcStepSize();
}

void ScanningQuadOsc::calcStepSize() {
    __stepSize = _mm_div_ps(__frequency, __samplerate);
}

void ScanningQuadOsc::setWavebank(float* wavebank, int32_t numWaves, int32_t tableSize) {
    _wavebank = wavebank;
    _numWaves = numWaves;
    __numWaves = _mm_set1_epi32(numWaves);
    __numWaves_1 = _mm_sub_epi32(__numWaves, _mm_set1_epi32(1));
    _tabSize = tableSize;
    __tabSize = _mm_set1_ps((float)tableSize);
    __tabSize_1 = _mm_sub_ps(__tabSize, __ones);
}

void ScanningQuadOsc::setScanPosition(float position) {
    mm_setScanPosition(_mm_set1_ps(position));
}

void ScanningQuadOsc::mm_setScanPosition(const __m128& position) {
    __lowBank = _mm_cvttps_epi32(position);
    __highBank = _mm_add_epi32(__lowBank, _mm_set1_epi32(1));
    __highBank = _mm_clamp_int32(__highBank, _mm_set1_epi32(0), __numWaves_1);
    _mm_store_si128((__m128i*)_lowBank, __lowBank);
    _mm_store_si128((__m128i*)_highBank, __highBank);
    __fade = _mm_sub_ps(position, _mm_cvtepi32_ps(__lowBank));
}

int32_t ScanningQuadOsc::getNumwaves() const {
    return _numWaves;
}

void ScanningQuadOsc::onChangeSyncMode() {
    __dir = __ones;
}

void ScanningQuadOsc::hardSync(const __m128& syncSource) {
    __sub = _mm_and_ps(__a, syncSource);
    __a = _mm_sub_ps(__a, __sub);
}

void ScanningQuadOsc::softSync(const __m128& syncSource) {
    __mask = _mm_and_ps(syncSource, _mm_cmplt_ps(__a, __quarter));
    __sub = _mm_and_ps(__a, __mask);
    __a = _mm_sub_ps(__a, __sub);
}

void ScanningQuadOsc::reverseSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __minus, syncSource));
}

void ScanningQuadOsc::octaveSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __twos, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __twos);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void ScanningQuadOsc::fifthSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __1_5, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __1_5);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void ScanningQuadOsc::subOctaveSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __half, syncSource));
    __mask = _mm_cmplt_ps(__dir, __half);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void ScanningQuadOsc::riseASync(const __m128& syncSource) {
    __dir = _mm_add_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __twos);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void ScanningQuadOsc::riseBSync(const __m128& syncSource) {
    __dir = _mm_add_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __fours);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void ScanningQuadOsc::fallASync(const __m128& syncSource) {
    __dir = _mm_sub_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmplt_ps(__dir, __ones);
    __dir = _mm_switch_ps(__dir, __twos, __mask);
}

void ScanningQuadOsc::fallBSync(const __m128& syncSource) {
    __dir = _mm_sub_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmplt_ps(__dir, __ones);
    __dir = _mm_switch_ps(__dir, __fours, __mask);
}

void ScanningQuadOsc::pullASync(const __m128& syncSource) {
    __sub = _mm_and_ps(__half, syncSource);
    __a = _mm_sub_ps(__a, __sub);
}

void ScanningQuadOsc::pullBSync(const __m128& syncSource) {
    __sub = _mm_and_ps(__quarter, syncSource);
    __a = _mm_sub_ps(__a, __sub);
}

void ScanningQuadOsc::pushASync(const __m128& syncSource) {
    __sub = _mm_and_ps(__quarter, syncSource);
    __a = _mm_add_ps(__a, __sub);
}

void ScanningQuadOsc::pushBSync(const __m128& syncSource) {
    __sub = _mm_and_ps(__half, syncSource);
    __a = _mm_add_ps(__a, __sub);
}

void ScanningQuadOsc::holdSync(const __m128& syncSource) {
    __dir = _mm_sub_ps(__dir, _mm_switch_ps(__zeros, __ones, syncSource));
    __mask = _mm_cmplt_ps(__dir, __zeros);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void ScanningQuadOsc::oneShot(const __m128& syncSource) {
    __outputLevel = _mm_switch_ps(__outputLevel, __zeros, __mtMask);
    __outputLevel = _mm_switch_ps(__outputLevel, __ones, syncSource);
    __dir = _mm_switch_ps(__dir, __zeros, __mtMask);
    __dir = _mm_switch_ps(__dir, __ones, syncSource);
    __a = _mm_switch_ps(__a, __zeros, syncSource);
    __a = _mm_switch_ps(__zeros, __a, _mm_cmpeq_ps(__dir, __ones));
}

void ScanningQuadOsc::lockShot(const __m128& syncSource) {
    __outputLevel = _mm_switch_ps(__outputLevel, __zeros, __mtMask);
    __outputLevel = _mm_switch_ps(__outputLevel, __ones, syncSource);
    __dir = _mm_switch_ps(__dir, __zeros, __mtMask);
    __dir = _mm_switch_ps(__dir, __ones, syncSource);
    __a = _mm_switch_ps(__zeros, __a, _mm_cmpeq_ps(__dir, __ones));
}

