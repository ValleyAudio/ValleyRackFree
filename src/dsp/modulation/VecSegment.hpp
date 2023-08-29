//
//  VecSegment.hpp
//
//  Created by Dale Johnson on 27/4/2020.
//  Copyright © 2020 Dale Johnson. All rights reserved.
//

#pragma once
#include "../../simd/SIMDUtilities.hpp"

struct VecSegment {
    __m128 seg, rate, output;
    __m128 ones, zeros, epsilon;

    VecSegment() {
        zeros = _mm_set1_ps(0.f);
        ones = _mm_set1_ps(1.f);
        epsilon = _mm_set1_ps(0.000031f);
        seg = zeros;
        output = zeros;
        rate = ones;
        __1_timeScale = ones;
    }

    __m128 process() {
        output = seg;
        seg = _mm_mul_ps(seg, valley::_mm_power_ps(rate, __1_timeScale));
        return output;
    }

    void setStartValue(const __m128& x) {
        seg = x;
    }

    void setTimeScale(const __m128& __tScale) {
        __1_timeScale = _mm_div_ps(ones, __tScale);
    }

    __m128 hasFinished() {
        return _mm_cmplt_ps(seg, epsilon);
    }
private:
    __m128 __1_timeScale;
};
