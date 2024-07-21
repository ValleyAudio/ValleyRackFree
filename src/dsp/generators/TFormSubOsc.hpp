#pragma once
#include "valley_sse_include.h"
#include <cmath>
#include <cstdint>
#include "../../simd/SIMDUtilities.hpp"
#include "../../utilities/Utilities.hpp"
#include "../filters/VecOnePoleFilters.hpp"

class TFormSubOsc {
public:
    TFormSubOsc() {
        pi = _mm_set1_ps(3.1415926f);
        posEpsilon = _mm_set1_ps(0.00001f);
        negEpsilon = _mm_set1_ps(-0.00001f);
        prev = _mm_set1_ps(0.f);
        trig = _mm_set1_ps(0.f);
        reset();
        setWave(_mm_set1_ps(0.25f));
    }

    __m128 process(const __m128& x,
                   const __m128& phasor,
                   const __m128& eoc,
                   const __m128& stepSize,
                   const __m128& direction) {

        counter = _mm_add_ps(counter, _mm_and_ps(_mm_set1_ps(1.f), eoc));
        counter = _mm_switch_ps(counter, _mm_set1_ps(0.f), _mm_cmpgt_ps(counter, _mm_set1_ps(1.f)));
        scaledStepSize = _mm_mul_ps(stepSize, direction);

        // Saw wave
        a = _mm_mul_ps(phasor, _mm_set1_ps(0.5f));
        a = _mm_add_ps(a, _mm_mul_ps(counter, _mm_set1_ps(0.5f)));
        b = _mm_add_ps(a, _mm_set1_ps(0.5f));
        b = _mm_sub_ps(b, _mm_and_ps(_mm_set1_ps(1.f), _mm_cmpge_ps(b, _mm_set1_ps(1.f))));
        __m128 saw = _mm_sub_ps(_mm_mul_ps(b, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        saw = _mm_sub_ps(saw, _mm_polyblep_ps(b, scaledStepSize));

        // Square wave derived from Saw wave
        y = _mm_switch_ps(_mm_set1_ps(-1.f), _mm_set1_ps(1.f), _mm_cmplt_ps(a, _mm_set1_ps(0.5f)));
        y = _mm_add_ps(y, _mm_polyblep_ps(a, scaledStepSize));
        __m128 square = _mm_sub_ps(y, _mm_polyblep_ps(b, scaledStepSize));

        // Sine wave
        c = _mm_mul_ps(_mm_sub_ps(a, _mm_set1_ps(0.5f)), _mm_set1_ps(2.005f)); // Correct inaccuracy
        __m128 sine = _mm_mul_ps(valley::_mm_sine_ps(_mm_mul_ps(c, pi)), _mm_set1_ps(-1.f));

        // Glitch wave
        trig = _mm_switch_ps(trig, _mm_sub_ps(_mm_set1_ps(1.f), trig),
                                   _mm_and_ps(_mm_cmpgt_ps(x, posEpsilon),
                                              _mm_cmple_ps(prev, negEpsilon)));
        prev = x;
        __m128 __glitch = _mm_sub_ps(_mm_mul_ps(trig, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));

        output = _mm_linterp_ps(sine, saw, wave1);
        output = _mm_linterp_ps(output, square, wave2);
        output = _mm_linterp_ps(output, __glitch, wave3);
        return output;
    }

    void setWave(const __m128& param) {
        wave1 = _mm_min_ps(_mm_mul_ps(param, _mm_set1_ps(3.f)),
                             _mm_set1_ps(1.f));
        wave2 = _mm_clamp_ps(_mm_sub_ps(_mm_mul_ps(param, _mm_set1_ps(3.f)), _mm_set1_ps(1.f)),
                               _mm_set1_ps(0.f), _mm_set1_ps(1.f));
        wave3 = _mm_clamp_ps(_mm_sub_ps(_mm_mul_ps(param, _mm_set1_ps(3.f)), _mm_set1_ps(2.f)),
                               _mm_set1_ps(0.f), _mm_set1_ps(1.f));
    }

    void reset() {
        a = _mm_set1_ps(0.f);
        b = _mm_set1_ps(0.f);
        y = _mm_set1_ps(0.f);
        counter = _mm_set1_ps(0.f);
    }

    __m128 output;

private:
    __m128 pi;
    __m128 posEpsilon, negEpsilon;
    __m128 a, b, c, y;
    __m128 counter, scaledStepSize, trig, prev;
    __m128 wave1, wave2, wave3;
};
