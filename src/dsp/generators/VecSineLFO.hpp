#pragma once

#include "../../simd/SIMDUtilities.hpp"

struct VecSineLFO {
    VecSineLFO() {
        pi = _mm_set1_ps(3.1415926f);
        output = _mm_set1_ps(0.f);
        phasor = _mm_set1_ps(0.f);
        phaseOffset = _mm_set1_ps(0.f);
        x = _mm_set1_ps(0.f);

        setSampleRate(44100.f);
        setFrequency(_mm_set1_ps(1.f));
    }

    __m128 process() {
        x = _mm_add_ps(phasor, phaseOffset);
        x = _mm_sub_ps(x, _mm_and_ps(_mm_set1_ps(1.f),
                                     _mm_cmpge_ps(x, _mm_set1_ps(1.f))));
        x = _mm_mul_ps(x, _mm_set1_ps(2.f));
        x = _mm_sub_ps(x, _mm_set1_ps(1.f));
        x = _mm_mul_ps(x, pi);
        output = valley::_mm_sine_ps(x);

        phasor = _mm_add_ps(phasor, stepSize);
        phasor = _mm_sub_ps(phasor,
                            _mm_and_ps(_mm_set1_ps(1.f),
                                       _mm_cmpge_ps(phasor, _mm_set1_ps(1.f))));

        return output;
    }

    void setFrequency(const __m128& newFrequency) {
        frequency = newFrequency;
        stepSize = _mm_mul_ps(frequency, sampleTime);
    }

    void setSampleRate(float sampleRate) {
        sampleTime = _mm_set1_ps(1.f / sampleRate);
        setFrequency(frequency);
    }

    __m128 output;
    __m128 phasor, phaseOffset, x;
    __m128 frequency, sampleTime, stepSize;
    __m128 pi;
};
