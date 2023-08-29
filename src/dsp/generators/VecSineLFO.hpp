#pragma once

#include "../../simd/SIMDUtilities.hpp"

struct VecSineLFO {
    VecSineLFO() {
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __twos = _mm_set1_ps(2.f);
        __pi = _mm_set1_ps(3.1415926f);
        __output = __zeros;
        __phasor = __zeros;
        __phaseOffset = __zeros;
        __x = __zeros;

        setSampleRate(44100.f);
        setFrequency(_mm_set1_ps(1.f));
    }

    __m128 process() {
        __x = _mm_add_ps(__phasor, __phaseOffset);
        __x = _mm_sub_ps(__x, _mm_and_ps(__ones, _mm_cmpge_ps(__x, __ones)));
        __x = _mm_mul_ps(__x, __twos);
        __x = _mm_sub_ps(__x, __ones);
        __x = _mm_mul_ps(__x, __pi);
        __output = valley::_mm_sine_ps(__x);

        __phasor = _mm_add_ps(__phasor, __stepSize);
        __phasor = _mm_sub_ps(__phasor, _mm_and_ps(__ones, _mm_cmpge_ps(__phasor, __ones)));

        return __output;
    }

    void setFrequency(const __m128& freq) {
        __freq = freq;
        __stepSize = _mm_mul_ps(__freq, _1_sampleRate);
    }

    void setSampleRate(float sampleRate) {
        _1_sampleRate = _mm_set1_ps(1.f / sampleRate);
        setFrequency(__freq);
    }

    __m128 __output;
    __m128 __phasor, __phaseOffset, __x;
    __m128 __freq, _1_sampleRate, __stepSize;
    __m128 __zeros, __ones, __twos, __pi;
};
