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
    output = _mm_set1_ps(0.f);

    std::srand(std::time(NULL));
    for(auto& i : z)
        i = std::rand();

    for(auto& i : w)
        i = std::rand();

    filter1.setSampleRate(44100.f);
    filter1.setCutoffFreq(40.f);
    filter2.setSampleRate(44100.f);
    filter2.setCutoffFreq(40.f);
}

__m128 Shaper::process(const __m128& a, const __m128& f) {
    switch((Modes)shapeMode) {
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
    return output;
}

void Shaper::setShapeMode(int newShapeMode) {
    shapeMode = (Modes)newShapeMode;
}

void Shaper::bend(const __m128& a, const __m128& f) {
    auto x = _mm_mul_ps(_mm_sub_ps(_mm_set1_ps(1.f), f), _mm_set1_ps(0.5f));
    auto __mask = _mm_cmplt_ps(a, x);
    auto __denom = _mm_add_ps(_mm_and_ps(__mask, x),
              _mm_andnot_ps(__mask, _mm_sub_ps(_mm_set1_ps(1.f), x)));
    auto __m = _mm_div_ps(_mm_set1_ps(0.5f), __denom);
    auto __c = _mm_sub_ps(_mm_set1_ps(0.5f), _mm_mul_ps(__m, x));
    output = _mm_add_ps(_mm_mul_ps(__m, a), _mm_andnot_ps(__mask, __c));
}

void Shaper::tilt(const __m128& a, const __m128& f) {
    auto x = _mm_mul_ps(f, _mm_set1_ps(3.f));
    x = _mm_abs_ps(x);
    x = _mm_add_ps(x, _mm_set1_ps(1.f));
    x = _mm_mul_ps(a, x);
    auto __mask = _mm_cmplt_ps(f, _mm_set1_ps(0.f));
    output = _mm_add_ps(x, _mm_and_ps(__mask, _mm_mul_ps(f, _mm_set1_ps(3.f))));
}

void Shaper::lean(const __m128& a, const __m128& f) {
    auto x = _mm_mul_ps(a, a);
    x = _mm_mul_ps(x, x);
    output = _mm_linterp_ps(a, x, f);
}

void Shaper::lean2(const __m128& a, const __m128& f) {
    auto x = _mm_sub_ps(_mm_set1_ps(1.f), a);
    x = _mm_mul_ps(x, x);
    x = _mm_mul_ps(x, x);
    x = _mm_sub_ps(_mm_set1_ps(1.f), x);
    output = _mm_linterp_ps(a, x, _mm_mul_ps(f, _mm_set1_ps(-1.f)));
}

void Shaper::twist(const __m128& a, const __m128& f) {
    auto fScaled = _mm_add_ps(_mm_mul_ps(f, _mm_set1_ps(1.98f)), _mm_set1_ps(1.f));
    auto __midMask = _mm_and_ps(_mm_cmpgt_ps(a , _mm_set1_ps(1.f / 3.f)), _mm_cmple_ps(a , _mm_set1_ps(2.f / 3.f)));
    auto __highMask = _mm_cmpgt_ps(a, _mm_set1_ps(2.f / 3.f));

    auto __k = _mm_add_ps(_mm_mul_ps(fScaled, _mm_set1_ps(-0.5f)), _mm_set1_ps(1.5f));
    __m128 x1 = _mm_mul_ps(a, __k);
    __m128 x2 = _mm_add_ps(_mm_mul_ps(a, fScaled), _mm_mul_ps(_mm_sub_ps(fScaled, _mm_set1_ps(1.f)), _mm_set1_ps(-0.5f)));
    output = x1;
    output = _mm_switch_ps(output, x2, __midMask);
    output = _mm_switch_ps(output, _mm_add_ps(x1, _mm_sub_ps(_mm_set1_ps(1.f), __k)), __highMask);
}

void Shaper::wrap(const __m128& a, const __m128& f) {
    auto x = _mm_mul_ps(a, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), _mm_set1_ps(8.f)), _mm_set1_ps(1.f)));
    __m128i __xInt = _mm_cvttps_epi32(x);
    __m128 __xIntF = _mm_cvtepi32_ps(__xInt);
    output = _mm_sub_ps(x, __xIntF);
}

void Shaper::mirror(const __m128& a, const __m128& f) {
    // Make switching phasor
    auto x = _mm_sub_ps(_mm_mul_ps(a, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
    x = _mm_mul_ps(x, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), _mm_set1_ps(9.f)), _mm_set1_ps(1.f)));
    x = _mm_mul_ps(_mm_add_ps(x, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
    x = _mm_abs_ps(x);
    x = _mm_mul_ps(x, _mm_set1_ps(0.5f));
    __m128i __xInt = _mm_cvttps_epi32(x);
    __m128 __xIntF = _mm_cvtepi32_ps(__xInt);
    x = _mm_sub_ps(x, __xIntF);

    auto y = _mm_sub_ps(_mm_mul_ps(a, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
    y = _mm_mul_ps(y, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), _mm_set1_ps(9.f)), _mm_set1_ps(1.f)));
    y = _mm_mul_ps(_mm_add_ps(y, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
    y = _mm_abs_ps(y);
    __m128i __yInt = _mm_cvttps_epi32(y);
    __m128 __yIntF = _mm_cvtepi32_ps(__yInt);
    auto z = _mm_sub_ps(y, __yIntF);

    output = _mm_switch_ps(z, _mm_sub_ps(_mm_set1_ps(1.f), z), _mm_cmpgt_ps(x, _mm_set1_ps(0.5f)));
}

void Shaper::reflect(const __m128& a, const __m128& f) {
    output = _mm_switch_ps(a, _mm_sub_ps(_mm_set1_ps(1.f), a), _mm_cmplt_ps(a, f));
}

void Shaper::pulse(const __m128& a, const __m128& f) {
    auto x = _mm_mul_ps(a, _mm_set1_ps(0.5f));
    x = _mm_mul_ps(x, _mm_add_ps(_mm_mul_ps(_mm_abs_ps(f), _mm_set1_ps(8.f)), _mm_set1_ps(1.f)));
    __m128i __xInt = _mm_cvttps_epi32(x);
    __m128 __xIntF = _mm_cvtepi32_ps(__xInt);
    x = _mm_sub_ps(x, __xIntF);
    output = _mm_switch_ps(a, _mm_set1_ps(1.f), _mm_cmpgt_ps(x, _mm_set1_ps(0.5f)));
}

void Shaper::step4(const __m128& a, const __m128& f) {
    auto __aScale = _mm_mul_ps(a, _mm_set1_ps(4.f));
    __m128i __aInt = _mm_cvttps_epi32(__aScale);
    __m128 __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_mul_ps(__aIntF, _mm_set1_ps(1.f / 4.f));
    output = _mm_linterp_ps(a, __aIntF, _mm_abs_ps(f));
}

void Shaper::step8(const __m128& a, const __m128& f) {
    auto __aScale = _mm_mul_ps(a, _mm_set1_ps(8.f));
    __m128i __aInt = _mm_cvttps_epi32(__aScale);
    __m128 __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_mul_ps(__aIntF, _mm_set1_ps(1.f / 8.f));
    output = _mm_linterp_ps(a, __aIntF, _mm_abs_ps(f));
}

void Shaper::step16(const __m128& a, const __m128& f) {
    auto __aScale = _mm_mul_ps(a, _mm_set1_ps(16.f));
    __m128i __aInt = _mm_cvttps_epi32(__aScale);
    __m128 __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_mul_ps(__aIntF, _mm_set1_ps(1.f / 16.f));
    output = _mm_linterp_ps(a, __aIntF, _mm_abs_ps(f));
}

void Shaper::varStep(const __m128& a, const __m128& f) {
    __m128 absF = _mm_abs_ps(f);
    auto __ff = _mm_sub_ps(_mm_set1_ps(128.f), _mm_mul_ps(absF, _mm_set1_ps(128.f)));
    auto __aScale = _mm_mul_ps(a, __ff);
    __m128i __aInt = _mm_cvttps_epi32(__aScale);
    __m128 __aIntF = _mm_cvtepi32_ps(__aInt);
    __aIntF = _mm_div_ps(__aIntF, __ff);
    __ff = _mm_mul_ps(absF, _mm_set1_ps(100.f));
    __ff = _mm_clamp_ps(__ff, _mm_set1_ps(0.f), _mm_set1_ps(1.f));
    output = _mm_linterp_ps(a, __aIntF, __ff);
}

void Shaper::sineWrap(const __m128& a, const __m128& f) {
    auto x = _mm_sub_ps(_mm_mul_ps(a, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
    auto y = _mm_max_ps(f, _mm_set1_ps(0.0625f));
    x = _mm_circle_ps(_mm_mul_ps(x, _mm_mul_ps(y, _mm_set1_ps(8.f))));
    output = valley::_mm_sine_ps(_mm_mul_ps(x, _mm_set1_ps(M_PI)));
    output = _mm_add_ps(_mm_mul_ps(output, _mm_set1_ps(0.5f)), _mm_set1_ps(0.5f));
    output = _mm_linterp_ps(a, output, _mm_min_ps(_mm_mul_ps(f, _mm_set1_ps(8.f)), _mm_set1_ps(1.f)));
}

void Shaper::warble(const __m128& a, const __m128& f) {
    for (int i = 0; i < 4; ++i) {
        random[i] = (float) mwcRand(z[i], w[i]) / (float)UINT32_MAX;
        random[i] = random[i] * 4.f - 2.f;
    }

    noise = _mm_load_ps(random);
    noise = filter1.process(noise);
    noise = filter2.process(noise);
    auto y = _mm_add_ps(a, _mm_mul_ps(noise, f));
    y = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(y, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(y, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));

}

void Shaper::harmonics(const __m128& a, const __m128& f) {
    auto __ff = _mm_sub_ps(_mm_max_ps(f, _mm_set1_ps(0.0625f)), _mm_set1_ps(0.0625f));
    __ff = _mm_mul_ps(__ff, _mm_set1_ps(6.4f));
    auto __m = _mm_min_ps(_mm_mul_ps(f, _mm_set1_ps(16.f)), _mm_set1_ps(1.f));

    __m128i __aInt = _mm_cvttps_epi32(_mm_add_ps(__ff, _mm_set1_ps(1.f)));
    __m128 __aIntF = _mm_cvtepi32_ps(__aInt);

    // First
    auto x = _mm_mul_ps(a, __aIntF);
    auto y = _mm_sub_ps(_mm_mul_ps(x, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
    y = _mm_circle_ps(y);
    auto __b = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));

    // Next
    x = _mm_mul_ps(a, _mm_add_ps(__aIntF, _mm_set1_ps(1.f)));
    y = _mm_sub_ps(_mm_mul_ps(x, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
    y = _mm_circle_ps(y);
    auto __c = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));

    output = _mm_linterp_ps(__b, __c, _mm_wrap_1_ps(__ff));
    output = _mm_add_ps(_mm_mul_ps(output, _mm_set1_ps(0.5f)), _mm_set1_ps(0.5f));
    output = _mm_linterp_ps(a, output, __m);
}

void Shaper::buzzX2(const __m128&a, const __m128& f) {
    auto x = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(2.f)));
    output = _mm_add_ps(a, _mm_mul_ps(x, f));
}

void Shaper::buzzX4(const __m128&a, const __m128& f) {
    auto x = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(4.f)));
    output = _mm_add_ps(a, _mm_mul_ps(x, f));
}

void Shaper::buzzX8(const __m128&a, const __m128& f) {
    auto x = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(8.f)));
    output = _mm_add_ps(a, _mm_mul_ps(x, f));
}

void Shaper::wrinkleX2(const __m128&a, const __m128& f) {
    auto x = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(2.f)));
    x = valley::_mm_sine_ps(_mm_mul_ps(x, _mm_set1_ps(M_PI)));
    output = _mm_add_ps(a, _mm_mul_ps(x, f));
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

void Shaper::wrinkleX4(const __m128&a, const __m128& f) {
    auto x = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(4.f)));
    x = valley::_mm_sine_ps(_mm_mul_ps(x, _mm_set1_ps(M_PI)));
    output = _mm_add_ps(a, _mm_mul_ps(x, f));
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

void Shaper::wrinkleX8(const __m128&a, const __m128& f) {
    auto x = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(8.f)));
    x = valley::_mm_sine_ps(_mm_mul_ps(x, _mm_set1_ps(M_PI)));
    output = _mm_add_ps(a, _mm_mul_ps(x, f));
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

void Shaper::sineDownX2(const __m128&a, const __m128& f) {
    auto x = _mm_mul_ps(_mm_sub_ps(_mm_set1_ps(1.f), a), f);
    auto y = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(2.f)));
    y = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));
    y = _mm_mul_ps(x, y);
    output = _mm_add_ps(a, y);
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

void Shaper::sineDownX4(const __m128&a, const __m128& f) {
    auto x = _mm_mul_ps(_mm_sub_ps(_mm_set1_ps(1.f), a), f);
    auto y = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(4.f)));
    y = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));
    y = _mm_mul_ps(x, y);
    output = _mm_add_ps(a, y);
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}
void Shaper::sineDownX8(const __m128&a, const __m128& f) {
    auto x = _mm_mul_ps(_mm_sub_ps(_mm_set1_ps(1.f), a), f);
    auto y = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(8.f)));
    y = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));
    y = _mm_mul_ps(x, y);
    output = _mm_add_ps(a, y);
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

void Shaper::sineUpX2(const __m128&a, const __m128& f) {
    auto x = _mm_mul_ps(a, f);
    auto y = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(2.f)));
    y = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));
    y = _mm_mul_ps(x, y);
    output = _mm_add_ps(a, y);
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

void Shaper::sineUpX4(const __m128&a, const __m128& f) {
    auto x = _mm_mul_ps(a, f);
    auto y = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(4.f)));
    y = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));
    y = _mm_mul_ps(x, y);
    output = _mm_add_ps(a, y);
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

void Shaper::sineUpX8(const __m128&a, const __m128& f) {
    auto x = _mm_mul_ps(a, f);
    auto y = _mm_circle_ps(_mm_mul_ps(a, _mm_set1_ps(8.f)));
    y = valley::_mm_sine_ps(_mm_mul_ps(y, _mm_set1_ps(M_PI)));
    y = _mm_mul_ps(x, y);
    output = _mm_add_ps(a, y);
    output = _mm_circle_ps(_mm_sub_ps(_mm_mul_ps(output, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));
    output = _mm_mul_ps(_mm_add_ps(output, _mm_set1_ps(1.f)), _mm_set1_ps(0.5f));
}

