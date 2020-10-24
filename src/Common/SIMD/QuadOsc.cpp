//
//  QuadOsc.cpp
//  QuadOsc - A syncronous, SIMD optimised oscillator.
//
//  Created by Dale Johnson on 02/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "QuadOsc.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

Shaper::Shaper() {
    _shapeMode = 0;

    __aScale = _mm_set1_ps(0.f);
    __x = _mm_set1_ps(0.f);
    __f = _mm_set1_ps(0.f);
    __xx = _mm_set1_ps(0.f);
    __ff = _mm_set1_ps(0.f);
    __k = _mm_set1_ps(0.f);
    __mask = _mm_set1_ps(0.f);
    __midMask = _mm_set1_ps(0.f);
    __highMask = _mm_set1_ps(0.f);
    __m = _mm_set1_ps(0.f);
    __c = _mm_set1_ps(0.f);
    __denom = _mm_set1_ps(1.f);
    __output = _mm_set1_ps(0.f);
    __aInt = _mm_set1_epi32(0);
    __xInt = _mm_set1_epi32(0);
    __yInt = _mm_set1_epi32(0);
    __aIntF = _mm_set1_ps(0.f);
    __xIntF = _mm_set1_ps(0.f);
    __yIntF = _mm_set1_ps(0.f);

    __third = _mm_set1_ps(0.333333f);
    __twoThird = _mm_set1_ps(0.666666f);
    __half = _mm_set1_ps(0.5f);
    __minusHalf = _mm_set1_ps(-0.5f);
    __minus = _mm_set1_ps(-1.f);
    __ones = _mm_set1_ps(1.f);
    __zeros = _mm_set1_ps(0.f);
    __twos = _mm_set1_ps(2.f);
    __threes = _mm_set1_ps(3.f);
    __fours = _mm_set1_ps(4.f);
    __eights = _mm_set1_ps(8.f);
    __nines = _mm_set1_ps(9.f);
    __sixteens = _mm_set1_ps(16.f);
    __hundredth = _mm_set1_ps(0.01f);

    __fourth = _mm_set1_ps(1.f / 4.f);
    __eighth = _mm_set1_ps(1.f / 8.f);
    __sixteenth = _mm_set1_ps(1.f / 16.f);

    std::srand(std::time(NULL));
    for(auto i = 0; i < 4; ++i) {
        _z[i] = std::rand();
        _w[i] = std::rand();
    }
    random = (float*)aligned_alloc_16(sizeof(float) * 4);
    __filter1.setSampleRate(44100);
    __filter1.setCutoffFreq(40.f);
    __filter2.setSampleRate(44100);
    __filter2.setCutoffFreq(40.f);
}

__m128 Shaper::process(const __m128& a, const __m128& f) {
    switch((Modes)_shapeMode) {
        case BEND_MODE: bend(a, f); break;
        case TILT_MODE: tilt(a, f); break;
        case LEAN_MODE: lean(a, f); break;
        case TWIST_MODE: twist(a, f); break;
        case WRAP_MODE: wrap(a, f); break;
        case MIRROR_MODE: mirror(a, f); break;
        case HARMONICS_MODE: harmonics(a, f); break;
        case WARBLE_MODE: warble(a, f); break;
        case REFLECT_MODE: reflect(a, f); break;
        case PULSE_MODE: pulse(a, f); break;
        case STEP4_MODE: step4(a, f); break;
        case STEP8_MODE: step8(a, f); break;
        case STEP16_MODE: step16(a, f); break;
        case VARSTEP_MODE: varStep(a, f); ;break;
        case SINEWRAP_MODE: sineWrap(a, f); break; // Appended here as not to break Dexter
        case BUZZ_X2_MODE: buzzX2(a, f); break;
        case BUZZ_X4_MODE: buzzX4(a, f); break;
        case BUZZ_X8_MODE: buzzX8(a, f); break;
        case WRINKLE_X2_MODE: wrinkleX2(a, f); break;
        case WRINKLE_X4_MODE: wrinkleX4(a, f); break;
        case WRINKLE_X8_MODE: wrinkleX8(a, f); break;
        case SINE_DOWN_X2_MODE: sineDownX2(a, f); break;
        case SINE_DOWN_X4_MODE: sineDownX4(a, f); break;
        case SINE_DOWN_X8_MODE: sineDownX8(a, f); break;
        case SINE_UP_X2_MODE: sineUpX2(a, f); break;
        case SINE_UP_X4_MODE: sineUpX4(a, f); break;
        case SINE_UP_X8_MODE: sineUpX8(a, f); break;
        default: bend(a, f);
    }
    return __output;
}

Shaper::~Shaper() {
    aligned_free_16(random);
}

void Shaper::setShapeMode(int mode) {
    _shapeMode = mode;
}

void Shaper::bend(const __m128& a, const __m128& f) {
    __x = _mm_mul_ps(_mm_sub_ps(__ones, f), __half);
    __mask = _mm_cmplt_ps(a, __x);
    __denom = _mm_add_ps(_mm_and_ps(__mask, __x), _mm_andnot_ps(__mask, _mm_sub_ps(__ones, __x)));
    __m = _mm_div_ps(__half, __denom);
    __c = _mm_sub_ps(__half, _mm_mul_ps(__m, __x));
    __output = _mm_add_ps(_mm_mul_ps(__m, a), _mm_andnot_ps(__mask, __c));
}

void Shaper::tilt(const __m128& a, const __m128& f) {
    __x = _mm_mul_ps(f, __threes);
    __x = _mm_abs_ps(__x);
    __x = _mm_add_ps(__x, __ones);
    __x = _mm_mul_ps(a, __x);
    __mask = _mm_cmplt_ps(f, __zeros);
    __output = _mm_add_ps(__x, _mm_and_ps(__mask, _mm_mul_ps(f, __threes)));
}

void Shaper::lean(const __m128& a, const __m128& f) {
    __x = _mm_mul_ps(a, a);
    __x = _mm_mul_ps(__x, __x);
    __output = _mm_linterp_ps(a, __x, f);
}

void Shaper::lean2(const __m128& a, const __m128& f) {
    __x = _mm_sub_ps(__ones, a);
    __x = _mm_mul_ps(__x, __x);
    __x = _mm_mul_ps(__x, __x);
    __x = _mm_sub_ps(__ones, __x);
    __output = _mm_linterp_ps(a, __x, _mm_mul_ps(f, __minus));
}

void Shaper::twist(const __m128& a, const __m128& f) {
    __f = _mm_add_ps(_mm_mul_ps(f, _mm_set1_ps(1.98f)), __ones);
    __midMask = _mm_and_ps(_mm_cmpgt_ps(a , __third), _mm_cmple_ps(a , __twoThird));
    __highMask = _mm_cmpgt_ps(a, __twoThird);

    __k = _mm_add_ps(_mm_mul_ps(__f, __minusHalf), _mm_set1_ps(1.5f));
    __m128 x1 = _mm_mul_ps(a, __k);
    __m128 x2 = _mm_add_ps(_mm_mul_ps(a, __f), _mm_mul_ps(_mm_sub_ps(__f, __ones), __minusHalf));
    __output = x1;
    __output = _mm_switch_ps(__output, x2, __midMask);
    __output = _mm_switch_ps(__output, _mm_add_ps(x1, _mm_sub_ps(__ones, __k)), __highMask);
}

void Shaper::wrap(const __m128& a, const __m128& f) {
    __x = _mm_mul_ps(a, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), __eights), __ones));
    __xInt = _mm_cvttps_epi32(__x);
    __xIntF = _mm_cvtepi32_ps(__xInt);
    __output = _mm_sub_ps(__x, __xIntF);
}

void Shaper::mirror(const __m128& a, const __m128& f) {
    // Make switching phasor
    __x = _mm_sub_ps(_mm_mul_ps(a, __twos), __ones);
    __x = _mm_mul_ps(__x, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), __nines), __ones));
    __x = _mm_mul_ps(_mm_add_ps(__x, __ones), __half);
    __x = _mm_abs_ps(__x);
    __x = _mm_mul_ps(__x, __half);
    __xInt = _mm_cvttps_epi32(__x);
    __xIntF = _mm_cvtepi32_ps(__xInt);
    __x = _mm_sub_ps(__x, __xIntF);

    __y = _mm_sub_ps(_mm_mul_ps(a, __twos), __ones);
    __y = _mm_mul_ps(__y, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), __nines), __ones));
    __y = _mm_mul_ps(_mm_add_ps(__y, __ones), __half);
    __y = _mm_abs_ps(__y);
    __yInt = _mm_cvttps_epi32(__y);
    __yIntF = _mm_cvtepi32_ps(__yInt);
    __z = _mm_sub_ps(__y, __yIntF);

    __output = _mm_switch_ps(__z, _mm_sub_ps(__ones, __z), _mm_cmpgt_ps(__x, __half));
}

void Shaper::reflect(const __m128& a, const __m128& f) {
    __output = _mm_switch_ps(a, _mm_sub_ps(__ones, a), _mm_cmplt_ps(a, f));
}

void Shaper::pulse(const __m128& a, const __m128& f) {
    __x = _mm_mul_ps(a, __half);
    __x = _mm_mul_ps(__x, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), __eights), __ones));
    __xInt = _mm_cvttps_epi32(__x);
    __xIntF = _mm_cvtepi32_ps(__xInt);
    __x = _mm_sub_ps(__x, __xIntF);
    __output = _mm_switch_ps(a, __ones, _mm_cmpgt_ps(__x, __half));
}

void Shaper::step4(const __m128& a, const __m128& f) {
    __aScale = _mm_mul_ps(a, __fours);
    __aInt = _mm_cvttps_epi32(__aScale);
    __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_mul_ps(__aIntF, __fourth);
    __output = _mm_linterp_ps(a, __aIntF, _mm_abs_ps(f));
}

void Shaper::step8(const __m128& a, const __m128& f) {
    __aScale = _mm_mul_ps(a, __eights);
    __aInt = _mm_cvttps_epi32(__aScale);
    __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_mul_ps(__aIntF, __eighth);
    __output = _mm_linterp_ps(a, __aIntF, _mm_abs_ps(f));
}

void Shaper::step16(const __m128& a, const __m128& f) {
    __aScale = _mm_mul_ps(a, __sixteens);
    __aInt = _mm_cvttps_epi32(__aScale);
    __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_mul_ps(__aIntF, __sixteenth);
    __output = _mm_linterp_ps(a, __aIntF, _mm_abs_ps(f));
}

void Shaper::varStep(const __m128& a, const __m128& f) {
    __m128 absF = _mm_abs_ps(f);
    __ff = _mm_sub_ps(_mm_set1_ps(128.f), _mm_mul_ps(absF, _mm_set1_ps(128.f)));
    __aScale = _mm_mul_ps(a, __ff);
    __aInt = _mm_cvttps_epi32(__aScale);
    __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_div_ps(__aIntF, __ff);
    __ff = _mm_mul_ps(absF, _mm_set1_ps(100.f));
    __ff = _mm_clamp_ps(__ff, __zeros, __ones);
    __output = _mm_linterp_ps(a, __aIntF, __ff);
}

void Shaper::sineWrap(const __m128& a, const __m128& f) {
    __y = _mm_max_ps(f, _mm_set1_ps(0.0625f));
    __x = _mm_sub_ps(_mm_mul_ps(a, __twos), __ones);
    __x = _mm_circle_ps(_mm_mul_ps(__x, _mm_mul_ps(__y, __eights)));
    __output = valley::_mm_sine_ps(_mm_mul_ps(__x, _mm_set1_ps(M_PI)));
    __output = _mm_add_ps(_mm_mul_ps(__output, __half), __half);
    __output = _mm_linterp_ps(a, __output, _mm_min_ps(_mm_mul_ps(f, __eights), __ones));
}

void Shaper::warble(const __m128& a, const __m128& f) {
    for (int i = 0; i < 4; ++i) {
        random[i] = (float) mwcRand(_z[i], _w[i]) / (float)UINT32_MAX;
        random[i] = random[i] * 4.f - 2.f;
    }

    __noise = _mm_load_ps(random);
    __noise = __filter1.process(__noise);
    __noise = __filter2.process(__noise);
    __y = _mm_add_ps(a, _mm_mul_ps(__noise, f));
    __y = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__y, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__y, __ones), __half);

}

void Shaper::harmonics(const __m128& a, const __m128& f) {
    __ff = _mm_sub_ps(_mm_max_ps(f, _mm_set1_ps(0.0625f)), _mm_set1_ps(0.0625f));
    __ff = _mm_mul_ps(__ff, _mm_set1_ps(6.4f));
    __m = _mm_min_ps(_mm_mul_ps(f, __sixteens), __ones);

    __aInt = _mm_cvttps_epi32(_mm_add_ps(__ff, __ones));
    __aIntF = _mm_cvtepi32_ps(__aInt);

    // First
    __x = _mm_mul_ps(a, __aIntF);
    __y = _mm_sub_ps(_mm_mul_ps(__x, __twos), __ones);
    __y = _mm_circle_ps(__y);
    __b = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));

    // Next
    __x = _mm_mul_ps(a, _mm_add_ps(__aIntF, __ones));
    __y = _mm_sub_ps(_mm_mul_ps(__x, __twos), __ones);
    __y = _mm_circle_ps(__y);
    __c = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));

    __output = _mm_linterp_ps(__b, __c, _mm_wrap_1_ps(__ff));
    __output = _mm_add_ps(_mm_mul_ps(__output, __half), __half);
    __output = _mm_linterp_ps(a, __output, __m);
}

void Shaper::buzzX2(const __m128&a, const __m128& f) {
    __x = _mm_circle_ps(_mm_mul_ps(a, __twos));
    __output = _mm_add_ps(a, _mm_mul_ps(__x, f));
}

void Shaper::buzzX4(const __m128&a, const __m128& f) {
    __x = _mm_circle_ps(_mm_mul_ps(a, __fours));
    __output = _mm_add_ps(a, _mm_mul_ps(__x, f));
}

void Shaper::buzzX8(const __m128&a, const __m128& f) {
    __x = _mm_circle_ps(_mm_mul_ps(a, __eights));
    __output = _mm_add_ps(a, _mm_mul_ps(__x, f));
}

void Shaper::wrinkleX2(const __m128&a, const __m128& f) {
    __x = _mm_circle_ps(_mm_mul_ps(a, __twos));
    __x = valley::_mm_sine_ps(_mm_mul_ps(__x, _mm_set1_ps(M_PI)));
    __output = _mm_add_ps(a, _mm_mul_ps(__x, f));
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

void Shaper::wrinkleX4(const __m128&a, const __m128& f) {
    __x = _mm_circle_ps(_mm_mul_ps(a, __fours));
    __x = valley::_mm_sine_ps(_mm_mul_ps(__x, _mm_set1_ps(M_PI)));
    __output = _mm_add_ps(a, _mm_mul_ps(__x, f));
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

void Shaper::wrinkleX8(const __m128&a, const __m128& f) {
    __x = _mm_circle_ps(_mm_mul_ps(a, __eights));
    __x = valley::_mm_sine_ps(_mm_mul_ps(__x, _mm_set1_ps(M_PI)));
    __output = _mm_add_ps(a, _mm_mul_ps(__x, f));
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

void Shaper::sineDownX2(const __m128&a, const __m128& f) {
    __x = _mm_mul_ps(_mm_sub_ps(__ones, a), f);
    __y = _mm_circle_ps(_mm_mul_ps(a, __twos));
    __y = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));
    __y = _mm_mul_ps(__x, __y);
    __output = _mm_add_ps(a, __y);
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

void Shaper::sineDownX4(const __m128&a, const __m128& f) {
    __x = _mm_mul_ps(_mm_sub_ps(__ones, a), f);
    __y = _mm_circle_ps(_mm_mul_ps(a, __fours));
    __y = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));
    __y = _mm_mul_ps(__x, __y);
    __output = _mm_add_ps(a, __y);
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}
void Shaper::sineDownX8(const __m128&a, const __m128& f) {
    __x = _mm_mul_ps(_mm_sub_ps(__ones, a), f);
    __y = _mm_circle_ps(_mm_mul_ps(a, __eights));
    __y = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));
    __y = _mm_mul_ps(__x, __y);
    __output = _mm_add_ps(a, __y);
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

void Shaper::sineUpX2(const __m128&a, const __m128& f) {
    __x = _mm_mul_ps(a, f);
    __y = _mm_circle_ps(_mm_mul_ps(a, __twos));
    __y = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));
    __y = _mm_mul_ps(__x, __y);
    __output = _mm_add_ps(a, __y);
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

void Shaper::sineUpX4(const __m128&a, const __m128& f) {
    __x = _mm_mul_ps(a, f);
    __y = _mm_circle_ps(_mm_mul_ps(a, __fours));
    __y = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));
    __y = _mm_mul_ps(__x, __y);
    __output = _mm_add_ps(a, __y);
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

void Shaper::sineUpX8(const __m128&a, const __m128& f) {
    __x = _mm_mul_ps(a, f);
    __y = _mm_circle_ps(_mm_mul_ps(a, __eights));
    __y = valley::_mm_sine_ps(_mm_mul_ps(__y, _mm_set1_ps(M_PI)));
    __y = _mm_mul_ps(__x, __y);
    __output = _mm_add_ps(a, __y);
    __output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(__output, __twos), __ones));
    __output = _mm_mul_ps(_mm_add_ps(__output, __ones), __half);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

QuadOsc::QuadOsc() {
    _aPos = (int32_t*) aligned_alloc_16(4 * sizeof(int32_t));
    _bPos = (int32_t*) aligned_alloc_16(4 * sizeof(int32_t));
    _lowSample = (float*) aligned_alloc_16(4 * sizeof(float));
    _highSample = (float*) aligned_alloc_16(4 * sizeof(float));
    _output = (float*) aligned_alloc_16(4 * sizeof(float));
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

    _shape = 0.0f;
    __shape = _mm_set1_ps(0.f);
    __a = __zeros;
    __b = __zeros;
    __readPhase = __zeros;
    __dir = __ones;
    setFrequency(1.f);
    setSyncMode(0);
    setShapeMethod(0);
    _shaper.setShapeMode(0);
}

QuadOsc::~QuadOsc() {
    aligned_free_16(_aPos);
    aligned_free_16(_bPos);
    aligned_free_16(_lowSample);
    aligned_free_16(_highSample);
    aligned_free_16(_output);
}

void QuadOsc::tick() {
    // Phase modulate
    __readPhase = _mm_add_ps(__a, __inputPhase);
    __negMask = _mm_cmplt_ps(__readPhase, __zeros);

    __shifts = __readPhase;
    __shiftsI = _mm_cvttps_epi32(__shifts);
    __shifts = _mm_cvtepi32_ps(__shiftsI);
    __shifts = _mm_sub_ps(__shifts, _mm_and_ps(__ones, __negMask));

    __readPhase = _mm_sub_ps(__readPhase, __shifts);
    __mask = _mm_cmpeq_ps(__readPhase, __ones);
    __readPhase = _mm_sub_ps(__readPhase, _mm_and_ps(_mm_and_ps(__mask, __negMask), __ones));

    // Shape
    __readPhase = shapeMethod(__readPhase, _mm_set1_ps(_shape));
    __readPhase = _mm_clamp_ps(__readPhase, __zeros, __ones);
    __readPhase = _mm_mul_ps(__readPhase, __tabSize);

    // Prepare read positions
    __b = _mm_add_ps(__readPhase, __ones);
    __mask = _mm_cmpge_ps(__b, __tabSize);
    __sub = _mm_and_ps(__tabSize, __mask);
    __b = _mm_sub_ps(__b, __sub);
    __aInt = _mm_cvttps_epi32(__readPhase);
    __bInt = _mm_cvttps_epi32(__b);
    _mm_store_si128((__m128i*)_aPos, __aInt);
    _mm_store_si128((__m128i*)_bPos, __bInt);

    // Do linear interpolation
    for(auto j = 0; j < 4; ++j) {
        _lowSample[j] = _wavetable[_aPos[j]];
        _highSample[j] = _wavetable[_bPos[j]];
    }
    __lowSamp = _mm_load_ps(_lowSample);
    __highSamp = _mm_load_ps(_highSample);
    __frac = _mm_sub_ps(__readPhase, _mm_cvtepi32_ps(__aInt));
    __output = _mm_linterp_ps(__lowSamp, __highSamp, __frac);
    __output = _mm_mul_ps(__output, __outputLevel);

    // Advance wave read position
    __a = _mm_add_ps(__a, __stepSize);
    __mask = _mm_cmpge_ps(__a, __ones);
    __sub = _mm_and_ps(__ones, __mask);
    __a = _mm_sub_ps(__a, __sub);
}

void QuadOsc::resetPhase() {
    __a = __half;
}

void QuadOsc::sync(const __m128& syncSource) {
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

float QuadOsc::getOutput(int channel) const {
    _mm_store_ps(_output, __output);
    return _output[channel];
}

const __m128& QuadOsc::getOutput() const {
    return __output;
}

const __m128& QuadOsc::getPhasor() const {
    return __a;
}

const __m128& QuadOsc::getStepSize() const {
    return __stepSize;
}

const __m128& QuadOsc::getShapedPhasor() const {
    return __readPhase;
}

const __m128& QuadOsc::getEOCPulse() const {
    return __eoc;
}

const __m128& QuadOsc::getDirection() const {
    return __dir;
}

void QuadOsc::setWavetable(float* wavetable, long size) {
    _wavetable = wavetable;
    __tabSize = _mm_set1_ps((float)size);
    __tabSize_1 = _mm_set1_ps((float)size - 1.f);
    calcStepSize();
}

void QuadOsc::setFrequency(float frequency) {
    __frequency = _mm_set1_ps(frequency);
    __frequency = _mm_switch_ps(__frequency, __nyquist, _mm_cmpgt_ps(__frequency, __nyquist));
    calcStepSize();
}

void QuadOsc::setFrequency(float f0, float f1, float f2, float f3) {
    __frequency = _mm_set_ps(f0, f1, f2, f3);
    __frequency = _mm_switch_ps(__frequency, __nyquist, _mm_cmpgt_ps(__frequency, __nyquist));
    calcStepSize();
}

void QuadOsc::setFrequency(const __m128& frequency) {
    __frequency = frequency;
    __frequency = _mm_switch_ps(__frequency, __nyquist, _mm_cmpgt_ps(__frequency, __nyquist));
    calcStepSize();
}

void QuadOsc::setShape(float shape) {
    _shape = shape;
    __shape = _mm_set1_ps(_shape);
}

void QuadOsc::setShape(const __m128& shape) {
    __shape = shape;
}

void QuadOsc::setShapeMethod(int shapeMethod) {
    _shaper.setShapeMode(shapeMethod);
}

void QuadOsc::setPMPostShape(bool PMPostShape) {
    _PMPostShape = PMPostShape;
}

void QuadOsc::setSyncMode(int syncMode) {
    if(_syncMode != syncMode) {
        _syncMode = syncMode;
        __outputLevel = __ones;
        onChangeSyncMode();
    }
}

void QuadOsc::enableSync(bool enableSync) {
    _sync = enableSync;
}

void QuadOsc::enableWeakSync(bool weakSync) {
    _weakSync = weakSync;
}

void QuadOsc::setSampleRate(float sampleRate) {
    __samplerate = _mm_set1_ps(sampleRate);
    __nyquist = _mm_div_ps(__samplerate, __twos);
    calcStepSize();
}

void QuadOsc::calcStepSize() {
    __stepSize = _mm_div_ps(__frequency, __samplerate);
}

void QuadOsc::onChangeSyncMode() {
    __dir = __ones;
}

void QuadOsc::hardSync(const __m128& syncSource) {
    __sub = _mm_and_ps(__a, syncSource);
    __a = _mm_sub_ps(__a, __sub);
}

void QuadOsc::softSync(const __m128& syncSource) {
    __mask = _mm_and_ps(syncSource, _mm_cmplt_ps(__a, __quarter));
    __sub = _mm_and_ps(__a, __mask);
    __a = _mm_sub_ps(__a, __sub);
}

void QuadOsc::reverseSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __minus, syncSource));
}

void QuadOsc::octaveSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __twos, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __twos);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void QuadOsc::fifthSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __1_5, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __1_5);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void QuadOsc::subOctaveSync(const __m128& syncSource) {
    __dir = _mm_mul_ps(__dir, _mm_switch_ps(__ones, __half, syncSource));
    __mask = _mm_cmplt_ps(__dir, __half);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void QuadOsc::riseASync(const __m128& syncSource) {
    __dir = _mm_add_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __twos);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void QuadOsc::riseBSync(const __m128& syncSource) {
    __dir = _mm_add_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmpgt_ps(__dir, __fours);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void QuadOsc::fallASync(const __m128& syncSource) {
    __dir = _mm_sub_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmplt_ps(__dir, __ones);
    __dir = _mm_switch_ps(__dir, __twos, __mask);
}

void QuadOsc::fallBSync(const __m128& syncSource) {
    __dir = _mm_sub_ps(__dir, _mm_switch_ps(__zeros, __half, syncSource));
    __mask = _mm_cmplt_ps(__dir, __ones);
    __dir = _mm_switch_ps(__dir, __fours, __mask);
}

void QuadOsc::pullASync(const __m128& syncSource) {
    __sub = _mm_and_ps(__half, syncSource);
    __a = _mm_sub_ps(__a, __sub);
}

void QuadOsc::pullBSync(const __m128& syncSource) {
    __sub = _mm_and_ps(__quarter, syncSource);
    __a = _mm_sub_ps(__a, __sub);
}

void QuadOsc::pushASync(const __m128& syncSource) {
    __sub = _mm_and_ps(__quarter, syncSource);
    __a = _mm_add_ps(__a, __sub);
}

void QuadOsc::pushBSync(const __m128& syncSource) {
    __sub = _mm_and_ps(__half, syncSource);
    __a = _mm_add_ps(__a, __sub);
}

void QuadOsc::holdSync(const __m128& syncSource) {
    __dir = _mm_sub_ps(__dir, _mm_switch_ps(__zeros, __ones, syncSource));
    __mask = _mm_cmplt_ps(__dir, __zeros);
    __dir = _mm_switch_ps(__dir, __ones, __mask);
}

void QuadOsc::oneShot(const __m128& syncSource) {
    __outputLevel = _mm_switch_ps(__outputLevel, __zeros, __mtMask);
    __outputLevel = _mm_switch_ps(__outputLevel, __ones, syncSource);
    __dir = _mm_switch_ps(__dir, __zeros, __mtMask);
    __dir = _mm_switch_ps(__dir, __ones, syncSource);
    __a = _mm_switch_ps(__a, __zeros, syncSource);
    __a = _mm_switch_ps(__zeros, __a, _mm_cmpeq_ps(__dir, __ones));
}

void QuadOsc::lockShot(const __m128& syncSource) {
    __outputLevel = _mm_switch_ps(__outputLevel, __zeros, __mtMask);
    __outputLevel = _mm_switch_ps(__outputLevel, __ones, syncSource);
    __dir = _mm_switch_ps(__dir, __zeros, __mtMask);
    __dir = _mm_switch_ps(__dir, __ones, syncSource);
    __a = _mm_switch_ps(__zeros, __a, _mm_cmpeq_ps(__dir, __ones));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ScanningQuadOsc::ScanningQuadOsc() {
    _lowBank = (int32_t*)aligned_alloc_16(4 * sizeof(int32_t));
    _highBank = (int32_t*)aligned_alloc_16(4 * sizeof(int32_t));
    _lowSample2 = (float*)aligned_alloc_16(4 * sizeof(float));
    _highSample2 = (float*)aligned_alloc_16(4 * sizeof(float));
    __fade = _mm_set1_ps(0.f);
    __result1 = _mm_set1_ps(0.f);
    __result2 = _mm_set1_ps(0.f);
    __lowBank = _mm_set1_epi32(0);
    __highBank = _mm_set1_epi32(0);
    __numWaves = _mm_set1_epi32(0);
    __numWaves_1 = __numWaves;
}

ScanningQuadOsc::~ScanningQuadOsc() {
    aligned_free_16(_lowBank);
    aligned_free_16(_highBank);
    aligned_free_16(_lowSample2);
    aligned_free_16(_highSample2);
}

void ScanningQuadOsc::tick() {
    // Phase modulate
    if(_PMPostShape) {
        // Shape
        //__readPhase = shapeMethod(__a, _mm_set1_ps(_shape));
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
        __mask = _mm_cmpeq_ps(__readPhase, __ones);
        __readPhase = _mm_sub_ps(__readPhase, _mm_and_ps(_mm_and_ps(__mask, __negMask), __ones));

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
        __mask = _mm_cmpeq_ps(__readPhase, __ones);
        __readPhase = _mm_sub_ps(__readPhase, _mm_and_ps(_mm_and_ps(__mask, __negMask), __ones));

        // Shape
        //__readPhase = shapeMethod(__readPhase, _mm_set1_ps(_shape));
        __readPhase = _shaper.process(__readPhase, __shape);
        __readPhase = _mm_clamp_ps(__readPhase, __zeros, __ones);
    }

    __scaledReadPhase = _mm_mul_ps(__readPhase, __tabSize_1);

    // Prepare read positions
    __b = _mm_add_ps(__scaledReadPhase, __ones);
    __mask = _mm_cmpge_ps(__b, __tabSize);
    __sub = _mm_and_ps(__tabSize, __mask);
    __b = _mm_sub_ps(__b, __sub);
    __aInt = _mm_cvttps_epi32(__scaledReadPhase);
    __bInt = _mm_cvttps_epi32(__b);
    _mm_store_si128((__m128i*)_aPos, __aInt);
    _mm_store_si128((__m128i*)_bPos, __bInt);
    __frac = _mm_sub_ps(__scaledReadPhase, _mm_cvtepi32_ps(__aInt));

    // Do linear interpolation
    for(auto j = 0; j < 4; ++j) {
        // _lowSample[j] = _wavebank[_lowBank[j]][_aPos[j]];
        // _lowSample2[j] = _wavebank[_highBank[j]][_aPos[j]];
        // _highSample[j] = _wavebank[_lowBank[j]][_bPos[j]];
        // _highSample2[j] = _wavebank[_highBank[j]][_bPos[j]];
        _lowSample[j] = _wavebank[_lowBank[j] * _tabSize + _aPos[j]];
        _lowSample2[j] = _wavebank[_highBank[j] * _tabSize +_aPos[j]];
        _highSample[j] = _wavebank[_lowBank[j] * _tabSize +_bPos[j]];
        _highSample2[j] = _wavebank[_highBank[j] * _tabSize +_bPos[j]];
    }

    __lowSamp = _mm_load_ps(_lowSample);
    __highSamp = _mm_load_ps(_highSample);
    __result1 = _mm_linterp_ps(__lowSamp, __highSamp, __frac);
    __lowSamp = _mm_load_ps(_lowSample2);
    __highSamp = _mm_load_ps(_highSample2);
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

// void ScanningQuadOsc::setWavebank(float** wavebank, int32_t numWaves, int32_t tableSize) {
//     _wavebank = wavebank;
//     _numWaves = numWaves;
//     __numWaves = _mm_set1_epi32(numWaves);
//     __numWaves_1 = _mm_sub_epi32(__numWaves, _mm_set1_epi32(1));
//     __tabSize = _mm_set1_ps((float)tableSize);
//     __tabSize_1 = _mm_sub_ps(__tabSize, __ones);
// }

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
