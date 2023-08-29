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
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __twos = _mm_set1_ps(2.f);
        __threes = _mm_set1_ps(3.f);
        __halfs = _mm_set1_ps(0.5f);
        __negOnes = _mm_set1_ps(-1.f);
        __pi = _mm_set1_ps(3.1415926f);
        __posEpsilon = _mm_set1_ps(0.00001f);
        __negEpsilon = _mm_set1_ps(-0.00001f);
        __prev = __zeros;
        __trig = __zeros;
        reset();
        setWave(_mm_set1_ps(0.25f));
    }

    __m128 process(const __m128& x,
                   const __m128& phasor,
                   const __m128& eoc,
                   const __m128& stepSize,
                   const __m128& direction) {

        __counter = _mm_add_ps(__counter, _mm_and_ps(__ones, eoc));
        __counter = _mm_switch_ps(__counter, __zeros, _mm_cmpgt_ps(__counter, __ones));
        __stepSize = _mm_mul_ps(stepSize, direction);

        // Saw wave
        __a = _mm_mul_ps(phasor, __halfs);
        __a = _mm_add_ps(__a, _mm_mul_ps(__counter, __halfs));
        __b = _mm_add_ps(__a, __halfs);
        __b = _mm_sub_ps(__b, _mm_and_ps(__ones, _mm_cmpge_ps(__b, __ones)));
        __saw = _mm_sub_ps(_mm_mul_ps(__b, __twos), __ones);
        __saw = _mm_sub_ps(__saw, _mm_polyblep_ps(__b, __stepSize));

        // Square wave derived from Saw wave
        __y = _mm_switch_ps(__negOnes, __ones, _mm_cmplt_ps(__a, __halfs));
        __y = _mm_add_ps(__y, _mm_polyblep_ps(__a, __stepSize));
        __square = _mm_sub_ps(__y, _mm_polyblep_ps(__b, __stepSize));

        // Sine wave
        __c = _mm_mul_ps(_mm_sub_ps(__a, __halfs), _mm_set1_ps(2.005f)); // Correct inaccuracy
        __sine = _mm_mul_ps(valley::_mm_sine_ps(_mm_mul_ps(__c, __pi)), __negOnes);

        // Glitch wave
        __trig = _mm_switch_ps(__trig,
                               _mm_sub_ps(__ones, __trig),
                               _mm_and_ps(_mm_cmpgt_ps(x, __posEpsilon),
                                          _mm_cmple_ps(__prev, __negEpsilon)));
        __prev = x;
        __glitch = _mm_sub_ps(_mm_mul_ps(__trig, __twos), __ones);

        __output = _mm_linterp_ps(__sine, __saw, __wave1);
        __output = _mm_linterp_ps(__output, __square, __wave2);
        __output = _mm_linterp_ps(__output, __glitch, __wave3);
        return __output;
    }

    void setWave(const __m128& param) {
        __wave1 = _mm_min_ps(_mm_mul_ps(param, __threes), __ones);
        __wave2 = _mm_clamp_ps(_mm_sub_ps(_mm_mul_ps(param, __threes), __ones), __zeros, __ones);
        __wave3 = _mm_clamp_ps(_mm_sub_ps(_mm_mul_ps(param, __threes), __twos), __zeros, __ones);
    }

    void setSampleRate(float sampleRate) {
        __filter.setSampleRate(sampleRate);
    }

    void reset() {
        __a = __zeros;
        __b = __zeros;
        __y = __zeros;
        __counter = __zeros;
    }

    __m128 __output;

private:
    __m128 __zeros, __ones, __twos, __threes, __halfs, __negOnes, __pi;
    __m128 __posEpsilon, __negEpsilon;
    __m128 __a, __b, __c, __y;
    __m128 __counter, __stepSize, __trig, __prev;
    __m128 __square, __saw, __sine, __glitch;
    __m128 __wave1, __wave2, __wave3;
    VecOnePoleLPFilter __filter;
};
