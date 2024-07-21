//
//  QuadOsc.cpp
//  QuadOsc - A syncronous, SIMD optimised oscillator.
//
//  Created by Dale Johnson on 02/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "QuadOsc.hpp"

ScanningQuadOsc::ScanningQuadOsc() {
    tableSize = 0;

    vSamplerate = _mm_set1_ps(44100.f);
    vNyquist = _mm_set1_ps(22050.f);
    inputPhase = _mm_set1_ps(0.f);
    vEndOfCycle = _mm_set1_ps(0.f);
    vSyncOut = _mm_set1_ps(0.f);
    vSyncing = _mm_set1_ps(0.f);
    vSyncSource = _mm_set1_ps(0.f);
    vSyncState = _mm_set1_ps(0.f);
    vOutputLevel = _mm_set1_ps(1.f);
    doPhaseModPostPhasorShaping = false;
    _weakSync = false;
    _sync = false;

    vNegMask = _mm_set1_ps(0.f);
    vShifts = _mm_set1_ps(0.f);
    vShiftsI = _mm_set1_epi32(0);
    vTableSize = _mm_set1_ps(0.f);
    vTableSize_1 = _mm_set1_ps(0.f);
    vAInt = _mm_set1_epi32(0);
    vBInt = _mm_set1_epi32(0);

    shape = 0.0f;
    vShape = _mm_set1_ps(0.f);
    vA = _mm_set1_ps(0.f);
    vB = _mm_set1_ps(0.f);
    vReadPhase = _mm_set1_ps(0.f);
    vPhasorDirection = _mm_set1_ps(1.f);
    _syncMode = 0;
    setFrequency(1.f);
    setShapeMode(0);
    shaper.setShapeMode(0);

    vFade = _mm_set1_ps(0.f);
    vResult1 = _mm_set1_ps(0.f);
    vResult2 = _mm_set1_ps(0.f);
    vLowBank = _mm_set1_epi32(0);
    vHighBank = _mm_set1_epi32(0);
    vNumWaves = _mm_set1_epi32(0);
    vNumWaves_1 = vNumWaves;
}

void ScanningQuadOsc::tick() {
    // Phase modulate
    __m128 vReadPhaseMask = _mm_set1_ps(0.f);
    if(doPhaseModPostPhasorShaping) {
        // Shape
        vReadPhase = shaper.process(vA, vShape);
        vReadPhase = _mm_clamp_ps(vReadPhase, _mm_set1_ps(0.f), _mm_set1_ps(1.f));

        // Phase Mod
        vReadPhase = _mm_add_ps(vReadPhase, inputPhase);

        vNegMask = _mm_cmplt_ps(vReadPhase, _mm_set1_ps(0.f));
        vShifts = vReadPhase;
        vShiftsI = _mm_cvttps_epi32(vShifts);
        vShifts = _mm_cvtepi32_ps(vShiftsI);
        vShifts = _mm_sub_ps(vShifts, _mm_and_ps(_mm_set1_ps(1.f), vNegMask));
        vReadPhase = _mm_sub_ps(vReadPhase, vShifts);
        vReadPhaseMask = _mm_cmpeq_ps(vReadPhase, _mm_set1_ps(1.f));
        vReadPhase = _mm_sub_ps(vReadPhase, _mm_and_ps(_mm_and_ps(vReadPhaseMask, vNegMask), _mm_set1_ps(1.f)));

        vReadPhase = _mm_clamp_ps(vReadPhase, _mm_set1_ps(0.f), _mm_set1_ps(1.f));
    }
    else {
        // Phase Mod
        vReadPhase = _mm_add_ps(vA, inputPhase);

        vNegMask = _mm_cmplt_ps(vReadPhase, _mm_set1_ps(0.f));
        vShifts = vReadPhase;
        vShiftsI = _mm_cvttps_epi32(vShifts);
        vShifts = _mm_cvtepi32_ps(vShiftsI);
        vShifts = _mm_sub_ps(vShifts, _mm_and_ps(_mm_set1_ps(1.f), vNegMask));
        vReadPhase = _mm_sub_ps(vReadPhase, vShifts);
        vReadPhaseMask = _mm_cmpeq_ps(vReadPhase, _mm_set1_ps(1.f));
        vReadPhase = _mm_sub_ps(vReadPhase, _mm_and_ps(_mm_and_ps(vReadPhaseMask, vNegMask), _mm_set1_ps(1.f)));

        // Shape
        vReadPhase = shaper.process(vReadPhase, vShape);
        vReadPhase = _mm_clamp_ps(vReadPhase, _mm_set1_ps(0.f), _mm_set1_ps(1.f));
    }

    __m128 scaledReadPhase = _mm_mul_ps(vReadPhase, vTableSize_1);

    // Prepare read positions
    vB = _mm_add_ps(scaledReadPhase, _mm_set1_ps(1.f));
    __m128 wrapMask = _mm_cmpge_ps(vB, vTableSize);
    vB = _mm_sub_ps(vB, _mm_and_ps(vTableSize, wrapMask));
    vAInt = _mm_cvttps_epi32(scaledReadPhase);
    vBInt = _mm_cvttps_epi32(vB);

    int32_t aPos[4] = {0, 0, 0, 0};
    int32_t bPos[4] = {0, 0, 0, 0};
    _mm_storeu_si128((__m128i*)aPos, vAInt);
    _mm_storeu_si128((__m128i*)bPos, vBInt);
    vInterpFraction = _mm_sub_ps(scaledReadPhase, _mm_cvtepi32_ps(vAInt));

    // Do linear interpolation
    for(auto j = 0; j < 4; ++j) {
        lowSample[j] = wavebank[lowBank[j] * tableSize + aPos[j]];
        lowSample2[j] = wavebank[highBank[j] * tableSize + aPos[j]];
        highSample[j] = wavebank[lowBank[j] * tableSize + bPos[j]];
        highSample2[j] = wavebank[highBank[j] * tableSize + bPos[j]];
    }

    vLowerValue = _mm_loadu_ps(lowSample);
    vUpperValue = _mm_loadu_ps(highSample);
    vResult1 = _mm_linterp_ps(vLowerValue, vUpperValue, vInterpFraction);
    vLowerValue = _mm_loadu_ps(lowSample2);
    vUpperValue = _mm_loadu_ps(highSample2);
    vResult2 = _mm_linterp_ps(vLowerValue, vUpperValue, vInterpFraction);
    vOutput = _mm_linterp_ps(vResult1, vResult2, vFade);
    vOutput = _mm_mul_ps(vOutput, vOutputLevel);

    // Advance wave read position
    vA = _mm_add_ps(vA, _mm_mul_ps(vStepSize, vPhasorDirection));

    // Wrap wave position
    vMoreThanMask = _mm_cmpge_ps(vA, _mm_set1_ps(1.f));
    vLessThanMask = _mm_cmplt_ps(vA, _mm_set1_ps(0.f));
    vA = _mm_add_ps(vA, _mm_and_ps(_mm_set1_ps(1.f), vLessThanMask));
    vA = _mm_sub_ps(vA, _mm_and_ps(_mm_set1_ps(1.f), vMoreThanMask));
    vEndOfCycle = _mm_or_ps(_mm_and_ps(_mm_set1_ps(1.f), vLessThanMask), _mm_and_ps(_mm_set1_ps(1.f), vMoreThanMask));
}

void ScanningQuadOsc::resetPhase() {
    vA = _mm_set1_ps(0.5f);
}

void ScanningQuadOsc::sync(const __m128& syncSource) {
    vSyncing = vSyncState;
    vSyncState = _mm_cmpgt_ps(syncSource, _mm_set1_ps(0.f));
    vSyncSource = _mm_and_ps(vSyncState, _mm_andnot_ps(vSyncing, vSyncState));

    if(!_sync) {
        vPhasorDirection = _mm_set1_ps(1.f);
        vOutputLevel = _mm_set1_ps(1.f);
    }
    else {
        switch((SyncModes) _syncMode) {
            case HARD_SYNC: hardSync(vSyncSource);
                break;
            case FIFTH_SYNC: fifthSync(vSyncSource);
                break;
            case OCTAVE_SYNC: octaveSync(vSyncSource);
                break;
            case SUB_OCTAVE_SYNC: subOctaveSync(vSyncSource);
                break;
            case RISE_A_SYNC: riseASync(vSyncSource);
                break;
            case RISE_B_SYNC: riseBSync(vSyncSource);
                break;
            case FALL_A_SYNC: fallASync(vSyncSource);
                break;
            case FALL_B_SYNC: fallBSync(vSyncSource);
                break;
            case PULL_A_SYNC: pullASync(vSyncSource);
                break;
            case PULL_B_SYNC: pullBSync(vSyncSource);
                break;
            case PUSH_A_SYNC: pushASync(vSyncSource);
                break;
            case PUSH_B_SYNC: pushBSync(vSyncSource);
                break;
            case HOLD_SYNC: holdSync(vSyncSource);
                break;
            case ONE_SHOT_SYNC: oneShot(vSyncSource);
                break;
            case LOCK_SHOT_SYNC: lockShot(vSyncSource);
                break;
            case REVERSE_SYNC: reverseSync(vSyncSource);
                break;
            default: hardSync(vSyncSource);
        }
    }
}

float ScanningQuadOsc::getOutput(int channel) const {
    float tempOut[4] = {0.f, 0.f, 0.f, 0.f};
    _mm_storeu_ps(tempOut, vOutput);
    return tempOut[channel];
}

const __m128& ScanningQuadOsc::getOutput() const {
    return vOutput;
}

const __m128& ScanningQuadOsc::getPhasor() const {
    return vA;
}

const __m128& ScanningQuadOsc::getStepSize() const {
    return vStepSize;
}

const __m128& ScanningQuadOsc::getShapedPhasor() const {
    return vReadPhase;
}

const __m128& ScanningQuadOsc::getEOCPulse() const {
    return vEndOfCycle;
}

const __m128& ScanningQuadOsc::getDirection() const {
    return vPhasorDirection;
}

void ScanningQuadOsc::setFrequency(float newFrequency) {
    vFrequency = _mm_set1_ps(newFrequency);
    vFrequency = _mm_switch_ps(vFrequency, vNyquist, _mm_cmpgt_ps(vFrequency, vNyquist));
    calcStepSize();
}

void ScanningQuadOsc::setFrequency(float f0, float f1, float f2, float f3) {
    vFrequency = _mm_set_ps(f0, f1, f2, f3);
    vFrequency = _mm_switch_ps(vFrequency, vNyquist, _mm_cmpgt_ps(vFrequency, vNyquist));
    calcStepSize();
}

void ScanningQuadOsc::setFrequency(const __m128& newFrequency) {
    vFrequency = newFrequency;
    vFrequency = _mm_switch_ps(vFrequency, vNyquist, _mm_cmpgt_ps(vFrequency, vNyquist));
    calcStepSize();
}

void ScanningQuadOsc::setShape(float newShape) {
    shape = newShape;
    vShape = _mm_set1_ps(shape);
}

void ScanningQuadOsc::setShape(const __m128& vNewShape) {
    vShape = vNewShape;
}

void ScanningQuadOsc::setShapeMode(int shapeMode) {
    shaper.setShapeMode(shapeMode);
}

void ScanningQuadOsc::setPhaseModPostPhasorShaping(bool phaseModShouldBePostPhasorShaping) {
    doPhaseModPostPhasorShaping = phaseModShouldBePostPhasorShaping;
}

void ScanningQuadOsc::setSyncMode(int syncMode) {
    if(_syncMode != syncMode) {
        _syncMode = syncMode;
        vOutputLevel = _mm_set1_ps(1.f);
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
    vSamplerate = _mm_set1_ps(sampleRate);
    vNyquist = _mm_div_ps(vSamplerate, _mm_set1_ps(2.f));
    calcStepSize();
}

void ScanningQuadOsc::calcStepSize() {
    vStepSize = _mm_div_ps(vFrequency, vSamplerate);
}

void ScanningQuadOsc::setWavebank(float* newWavebank,
                                  int32_t newWavebankNumWaves,
                                  int32_t newTableSize) {
    wavebank = newWavebank;
    numWaves = newWavebankNumWaves;
    vNumWaves = _mm_set1_epi32(numWaves);
    vNumWaves_1 = _mm_sub_epi32(vNumWaves, _mm_set1_epi32(1));
    tableSize = newTableSize;
    vTableSize = _mm_set1_ps((float)tableSize);
    vTableSize_1 = _mm_sub_ps(vTableSize, _mm_set1_ps(1.f));
}

void ScanningQuadOsc::setScanPosition(float position) {
    mm_setScanPosition(_mm_set1_ps(position));
}

void ScanningQuadOsc::mm_setScanPosition(const __m128& position) {
    vLowBank = _mm_cvttps_epi32(position);
    vHighBank = _mm_add_epi32(vLowBank, _mm_set1_epi32(1));
    vHighBank = _mm_clamp_int32(vHighBank, _mm_set1_epi32(0), vNumWaves_1);
    _mm_store_si128((__m128i*)lowBank, vLowBank);
    _mm_store_si128((__m128i*)highBank, vHighBank);
    vFade = _mm_sub_ps(position, _mm_cvtepi32_ps(vLowBank));
}

int32_t ScanningQuadOsc::getNumwaves() const {
    return numWaves;
}

void ScanningQuadOsc::onChangeSyncMode() {
    vPhasorDirection = _mm_set1_ps(1.f);
}

void ScanningQuadOsc::hardSync(const __m128& syncSource) {
    vSub = _mm_and_ps(vA, syncSource);
    vA = _mm_sub_ps(vA, vSub);
}

void ScanningQuadOsc::softSync(const __m128& syncSource) {
    vMask = _mm_and_ps(syncSource, _mm_cmplt_ps(vA, _mm_set1_ps(0.25f)));
    vSub = _mm_and_ps(vA, vMask);
    vA = _mm_sub_ps(vA, vSub);
}

void ScanningQuadOsc::reverseSync(const __m128& syncSource) {
    vPhasorDirection = _mm_mul_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(1.f), _mm_set1_ps(-1.f), syncSource));
}

void ScanningQuadOsc::octaveSync(const __m128& syncSource) {
    vPhasorDirection = _mm_mul_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(1.f), _mm_set1_ps(2.f), syncSource));
    vMask = _mm_cmpgt_ps(vPhasorDirection, _mm_set1_ps(2.f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), vMask);
}

void ScanningQuadOsc::fifthSync(const __m128& syncSource) {
    vPhasorDirection = _mm_mul_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(1.f), _mm_set1_ps(1.5f), syncSource));
    vMask = _mm_cmpgt_ps(vPhasorDirection, _mm_set1_ps(1.5f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), vMask);
}

void ScanningQuadOsc::subOctaveSync(const __m128& syncSource) {
    vPhasorDirection = _mm_mul_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(1.f), _mm_set1_ps(0.5f), syncSource));
    vMask = _mm_cmplt_ps(vPhasorDirection, _mm_set1_ps(0.5f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), vMask);
}

void ScanningQuadOsc::riseASync(const __m128& syncSource) {
    vPhasorDirection = _mm_add_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(0.f), _mm_set1_ps(0.5f), syncSource));
    vMask = _mm_cmpgt_ps(vPhasorDirection, _mm_set1_ps(2.f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), vMask);
}

void ScanningQuadOsc::riseBSync(const __m128& syncSource) {
    vPhasorDirection = _mm_add_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(0.f), _mm_set1_ps(0.5f), syncSource));
    vMask = _mm_cmpgt_ps(vPhasorDirection, _mm_set1_ps(4.f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), vMask);
}

void ScanningQuadOsc::fallASync(const __m128& syncSource) {
    vPhasorDirection = _mm_sub_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(0.f), _mm_set1_ps(0.5f), syncSource));
    vMask = _mm_cmplt_ps(vPhasorDirection, _mm_set1_ps(1.f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(2.f), vMask);
}

void ScanningQuadOsc::fallBSync(const __m128& syncSource) {
    vPhasorDirection = _mm_sub_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(0.f), _mm_set1_ps(0.5f), syncSource));
    vMask = _mm_cmplt_ps(vPhasorDirection, _mm_set1_ps(1.f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(4.f), vMask);
}

void ScanningQuadOsc::pullASync(const __m128& syncSource) {
    vSub = _mm_and_ps(_mm_set1_ps(0.5f), syncSource);
    vA = _mm_sub_ps(vA, vSub);
}

void ScanningQuadOsc::pullBSync(const __m128& syncSource) {
    vSub = _mm_and_ps(_mm_set1_ps(0.25f), syncSource);
    vA = _mm_sub_ps(vA, vSub);
}

void ScanningQuadOsc::pushASync(const __m128& syncSource) {
    vSub = _mm_and_ps(_mm_set1_ps(0.25f), syncSource);
    vA = _mm_add_ps(vA, vSub);
}

void ScanningQuadOsc::pushBSync(const __m128& syncSource) {
    vSub = _mm_and_ps(_mm_set1_ps(0.5f), syncSource);
    vA = _mm_add_ps(vA, vSub);
}

void ScanningQuadOsc::holdSync(const __m128& syncSource) {
    vPhasorDirection = _mm_sub_ps(vPhasorDirection, _mm_switch_ps(_mm_set1_ps(0.f), _mm_set1_ps(1.f), syncSource));
    vMask = _mm_cmplt_ps(vPhasorDirection, _mm_set1_ps(0.f));
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), vMask);
}

void ScanningQuadOsc::oneShot(const __m128& syncSource) {
    vOutputLevel = _mm_switch_ps(vOutputLevel, _mm_set1_ps(0.f), vMoreThanMask);
    vOutputLevel = _mm_switch_ps(vOutputLevel, _mm_set1_ps(1.f), syncSource);
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(0.f), vMoreThanMask);
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), syncSource);
    vA = _mm_switch_ps(vA, _mm_set1_ps(0.f), syncSource);
    vA = _mm_switch_ps(_mm_set1_ps(0.f), vA, _mm_cmpeq_ps(vPhasorDirection, _mm_set1_ps(1.f)));
}

void ScanningQuadOsc::lockShot(const __m128& syncSource) {
    vOutputLevel = _mm_switch_ps(vOutputLevel, _mm_set1_ps(0.f), vMoreThanMask);
    vOutputLevel = _mm_switch_ps(vOutputLevel, _mm_set1_ps(1.f), syncSource);
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(0.f), vMoreThanMask);
    vPhasorDirection = _mm_switch_ps(vPhasorDirection, _mm_set1_ps(1.f), syncSource);
    vA = _mm_switch_ps(_mm_set1_ps(0.f), vA, _mm_cmpeq_ps(vPhasorDirection, _mm_set1_ps(1.f)));
}

