//
//  QuadOsc.cpp
//  QuadOsc - A syncronous, SIMD optimised oscillator.
//
//  Created by Dale Johnson on 02/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "Shaper.hpp"

////////////////////////////////////////////////////////////////////////////////

Shaper::Shaper() {
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

    for(auto& z : _z)
        z = std::rand();

    for(auto& w : _w)
        w = std::rand();

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
        case SINEWRAP_MODE: sineWrap(a, f); break; // Dexter stops here
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

void Shaper::setShapeMode(int mode) {
    _shapeMode = (Modes)mode;
}

void Shaper::bend(const __m128& a, const __m128& f) {
    __x = _mm_mul_ps(_mm_sub_ps(__ones, f), __half);
    __mask = _mm_cmplt_ps(a, __x);
    __denom = _mm_add_ps(_mm_and_ps(__mask, __x),
              _mm_andnot_ps(__mask, _mm_sub_ps(__ones, __x)));
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

