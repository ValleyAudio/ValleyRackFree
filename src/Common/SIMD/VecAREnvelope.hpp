//
//  VecSegment.hpp
//
//  Created by Dale Johnson on 27/4/2020.
//  Copyright © 2020 Dale Johnson. All rights reserved.
//

#pragma once
#include "VecSegment.hpp"

struct VecAREnvelope {
    VecSegment s;
    __m128 zeros, ones;
    __m128 env;
    __m128 prevTrigger;
    __m128 rising, falling;
    __m128 high;
    __m128 riseRate, fallRate;
    __m128 risingEdge;
    __m128 riseFinished;

    VecAREnvelope() {
        zeros = _mm_set1_ps(0.f);
        ones = _mm_set1_ps(1.f);
        env = zeros;
        prevTrigger = zeros;
        rising = zeros;
        high = _mm_high_ps();

        riseRate = _mm_set1_ps(0.8f);
        fallRate = _mm_set1_ps(0.99f);
    }

    __m128 process(const __m128& trigger) {
        risingEdge = _mm_cmpgt_ps(trigger, prevTrigger);
        s.setStartValue(_mm_switch_ps(s.seg, _mm_sub_ps(ones, s.seg), risingEdge));

        rising = _mm_switch_ps(rising, high, risingEdge);
        riseFinished = _mm_and_ps(s.hasFinished(), rising);
        s.setStartValue(_mm_switch_ps(s.seg, ones, riseFinished));

        rising = _mm_switch_ps(rising, zeros, riseFinished);
        s.rate = _mm_switch_ps(fallRate, riseRate, rising);
        s.process();

        env = _mm_switch_ps(s.output, _mm_sub_ps(ones, s.seg), rising);

        prevTrigger = trigger;
        return env;
    }

    __m128 hasFinished() const {
        return _mm_cmplt_ps(env, s.epsilon);
    }
};
