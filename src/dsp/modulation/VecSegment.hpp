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
    __m128 epsilon;

    VecSegment() {
        epsilon = _mm_set1_ps(0.000031f);
        seg = _mm_set1_ps(0.f);
        output = _mm_set1_ps(0.f);
        rate = _mm_set1_ps(1.f);
        reciprocalTimeScale = _mm_set1_ps(1.f);
    }

    __m128 process() {
        output = seg;
        seg = _mm_mul_ps(seg, valley::_mm_power_ps(rate, reciprocalTimeScale));
        return output;
    }

    void setStartValue(const __m128& x) {
        seg = x;
    }

    void setTimeScale(const __m128& newTimeScale) {
        reciprocalTimeScale = _mm_div_ps(_mm_set1_ps(1.f), newTimeScale);
    }

    __m128 hasFinished() {
        return _mm_cmplt_ps(seg, epsilon);
    }
private:
    __m128 reciprocalTimeScale;
};
