//
//  SIMDUtilities.hpp
//  SIMDUtilities - A set of functions and tools for SIMD operations
//
//  Created by Dale Johnson on 17/07/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#pragma once
#include <pmmintrin.h>
#define VALLEY_1F2 0.5
#define VALLEY_1F3 0.1666666667
#define VALLEY_1F4 0.04166666667
#define VALLEY_1F5 0.008333333333
#define VALLEY_1F6 0.001388888889
#define VALLEY_1F7 0.0001984126984
#define VALLEY_1F8 0.0000248015873
#define VALLEY_1F9 0.000002755731922
#define VALLEY_1F10 0.000000275573192
#define VALLEY_1F11 0.000000025052108
#define VALLEY_1F12 0.000000002087676
#define VALLEY_1F13 0.000000001605904

#ifdef _WIN32
    inline void* aligned_alloc_16(size_t __size) {
        return _aligned_malloc(__size, 16);
    }

    inline void aligned_free_16(void* ptr) {
        _aligned_free(ptr);
    }
#elif __APPLE__
    inline void* aligned_alloc_16(size_t __size) {
        return malloc(__size);
    }

    inline void aligned_free_16(void* ptr) {
        free(ptr);
    }
#elif __linux__
    inline void* aligned_alloc_16(size_t __size) {
        return aligned_alloc(16, __size);
    }

    inline void aligned_free_16(void* ptr) {
        free(ptr);
    }
#endif

inline __m128 _mm_linterp_ps(const __m128& a, const __m128& b, const __m128& frac) {
    return _mm_add_ps(a, _mm_mul_ps(frac, _mm_sub_ps(b, a)));
}

inline __m128d _mm_linterp_pd(const __m128d& a, const __m128d& b, const __m128d& frac) {
    return _mm_add_pd(a, _mm_mul_pd(frac, _mm_sub_pd(b, a)));
}

inline __m128 _mm_abs_ps(const __m128& a) {
    __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
    return _mm_and_ps(a,mask);
}

inline __m128 _mm_switch_ps(const __m128& a, const __m128& b, const __m128& cond) {
    return _mm_or_ps(_mm_andnot_ps(cond, a), _mm_and_ps(cond, b));
}

inline __m128d _mm_switch_pd(const __m128d& a, const __m128d& b, const __m128d& cond) {
    return _mm_or_pd(_mm_andnot_pd(cond, a), _mm_and_pd(cond, b));
}

inline __m128i _mm_switch_int32(const __m128i& a, const __m128i& b, const __m128i& cond) {
    return _mm_or_si128(_mm_andnot_si128(cond, a), _mm_and_si128(cond, b));
}

inline __m128 _mm_clamp_ps(const __m128& a, const __m128& min, const __m128& max) {
    return _mm_min_ps(_mm_max_ps(a, min), max);
}

inline __m128d _mm_clamp_pd(const __m128d& a, const __m128d& min, const __m128d& max) {
    return _mm_min_pd(_mm_max_pd(a, min), max);
}

inline __m128i _mm_clamp_int32(const __m128i& a, const __m128i& min, const __m128i& max) {
    return _mm_switch_int32(_mm_switch_int32(a, min, _mm_cmplt_epi32(a, min)), max, _mm_cmpgt_epi32(a, max));
}

namespace valley {
    inline __m128 _mm_sine_ps(__m128 x) {
        __m128 x2 = _mm_mul_ps(x, x);
        __m128 x3 = _mm_mul_ps(x2, x);
        __m128 x5 = _mm_mul_ps(x2, x3);
        __m128 x7 = _mm_mul_ps(x2, x5);
        __m128 x9 = _mm_mul_ps(x2, x7);
        __m128 x11 = _mm_mul_ps(x2, x9);
        __m128 out = _mm_sub_ps(x, _mm_mul_ps(x3, _mm_set1_ps((float)VALLEY_1F3)));
        out = _mm_add_ps(out, _mm_mul_ps(x5, _mm_set1_ps((float)VALLEY_1F5)));
        out = _mm_sub_ps(out, _mm_mul_ps(x7, _mm_set1_ps((float)VALLEY_1F7)));
        out = _mm_add_ps(out, _mm_mul_ps(x9, _mm_set1_ps((float)VALLEY_1F9)));
        return _mm_sub_ps(out, _mm_mul_ps(x11, _mm_set1_ps((float)VALLEY_1F11)));
    }
}
