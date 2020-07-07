#pragma once
#include <pmmintrin.h>
#include <cmath>
#include <cstdint>
#include "../Common/SIMD/SIMDUtilities.hpp"
#include "../Common/Utilities.hpp"

class TFormSubOsc {
public:
    TFormSubOsc() {
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __halfs = _mm_set1_ps(0.5f);
        __negOnes = _mm_set1_ps(-1.f);
        reset();
    }

    __m128 process(const __m128& phasor,
                   const __m128& eoc,
                   const __m128& stepSize) {
        __counter = _mm_add_ps(__counter, _mm_and_ps(__ones, eoc));
        __counter = _mm_switch_ps(__counter, __zeros, _mm_cmpgt_ps(__counter, __ones));
å
        __a = _mm_mul_ps(phasor, __halfs);
        __a = _mm_add_ps(__a, _mm_mul_ps(__counter, __halfs));
        __b = _mm_add_ps(__a, __halfs);
        __b = _mm_sub_ps(__b, _mm_and_ps(__ones, _mm_cmpge_ps(__b, __ones)));
        __y = _mm_switch_ps(__negOnes, __ones, _mm_cmplt_ps(__a, __halfs));
        __y = _mm_add_ps(__y, _mm_polyblep_ps(__a, stepSize));
        return _mm_sub_ps(__y, _mm_polyblep_ps(__b, stepSize));
    }

    void reset() {
        __a = __zeros;
        __b = __zeros;
        __y = __zeros;
        __counter = __zeros;
    }

private:
    __m128 __zeros, __ones, __halfs, __negOnes;
    __m128 __a, __b, __y;
    __m128 __counter;
}
