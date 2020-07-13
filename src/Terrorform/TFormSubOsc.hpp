#pragma once
#include <pmmintrin.h>
#include <cmath>
#include <cstdint>
#include "../Common/SIMD/SIMDUtilities.hpp"
#include "../Common/SIMD/VecOnePoleFilters.hpp"
#include "../Common/Utilities.hpp"

class TFormSubOsc {
public:
    TFormSubOsc() {
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __twos = _mm_set1_ps(2.f);
        __halfs = _mm_set1_ps(0.5f);
        __negOnes = _mm_set1_ps(-1.f);
        __pi = _mm_set1_ps(3.1415926f);
        reset();
        setWave(_mm_set1_ps(0.25f));
    }

    __m128 process(const __m128& phasor,
                   const __m128& eoc,
                   const __m128& stepSize,
                   const __m128& direction) {

        __counter = _mm_add_ps(__counter, _mm_and_ps(__ones, eoc));
        __counter = _mm_switch_ps(__counter, __zeros, _mm_cmpgt_ps(__counter, __ones));
        __stepSize = _mm_mul_ps(stepSize, direction);

        __a = _mm_mul_ps(phasor, __halfs);
        __a = _mm_add_ps(__a, _mm_mul_ps(__counter, __halfs));
        __b = _mm_add_ps(__a, __halfs);
        __b = _mm_sub_ps(__b, _mm_and_ps(__ones, _mm_cmpge_ps(__b, __ones)));
        __saw = _mm_sub_ps(_mm_mul_ps(__b, __twos), __ones);
        __saw = _mm_sub_ps(__saw, _mm_polyblep_ps(__b, __stepSize));

        __y = _mm_switch_ps(__negOnes, __ones, _mm_cmplt_ps(__a, __halfs));
        __y = _mm_add_ps(__y, _mm_polyblep_ps(__a, __stepSize));
        __square = _mm_sub_ps(__y, _mm_polyblep_ps(__b, __stepSize));

        __c = _mm_mul_ps(_mm_sub_ps(__a, __halfs), _mm_set1_ps(2.005f)); // Correct inaccuracy
        __sine = _mm_mul_ps(valley::_mm_sine_ps(_mm_mul_ps(__c, __pi)), __negOnes);

        __output = _mm_linterp_ps(__sine, __saw, __wave1);
        return _mm_linterp_ps(__output, __square, __wave2);
    }

    void setWave(const __m128& param) {
        __wave1 = _mm_min_ps(_mm_mul_ps(param, __twos), __ones);
        __wave2 = _mm_clamp_ps(_mm_sub_ps(_mm_mul_ps(param, __twos), __ones), __zeros, __ones);
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

private:
    __m128 __zeros, __ones, __twos, __halfs, __negOnes, __pi;
    __m128 __a, __b, __c, __y;
    __m128 __counter, __stepSize;
    __m128 __square, __saw, __sine, __output;
    __m128 __wave1, __wave2;
    VecOnePoleLPFilter __filter;
};
