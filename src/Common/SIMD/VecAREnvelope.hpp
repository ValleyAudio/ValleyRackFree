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
    __m128 zeros, ones, high;
    __m128 env;
    __m128 prevTrigger;
    __m128 rising, falling;
    __m128 riseRate, fallRate;
    __m128 risingEdge;
    __m128 riseFinished;
    __m128 targetValue;
    __m128 risingEpsilon, fallingEpsilon;

    VecAREnvelope() {
        zeros = _mm_set1_ps(0.f);
        ones = _mm_set1_ps(1.f);
        high = _mm_high_ps();
        env = zeros;
        prevTrigger = zeros;
        rising = zeros;
        falling = high;
        risingEdge = zeros;
        riseFinished = zeros;
        targetValue = zeros;

        riseRate = _mm_set1_ps(0.99f);
        fallRate = _mm_set1_ps(0.99f);
        risingEpsilon = _mm_set1_ps(0.002f);
        fallingEpsilon = _mm_set1_ps(0.000031f);
    }

    __m128 process(const __m128& trigger) {
        risingEdge = _mm_cmpgt_ps(trigger, prevTrigger);
        targetValue = _mm_switch_ps(targetValue, trigger, risingEdge);
        s.setStartValue(_mm_switch_ps(s.seg, _mm_sub_ps(targetValue, s.seg), _mm_and_ps(risingEdge, falling)));

        s.epsilon = _mm_switch_ps(risingEpsilon, fallingEpsilon, falling);
        rising = _mm_switch_ps(rising, high, risingEdge);
        falling = _mm_switch_ps(falling, zeros, risingEdge);
        riseFinished = _mm_and_ps(s.hasFinished(), rising);
        rising = _mm_switch_ps(rising, zeros, riseFinished);
        falling = _mm_switch_ps(falling, high, riseFinished);
        s.setStartValue(_mm_switch_ps(s.seg, targetValue, riseFinished));

        s.rate = _mm_switch_ps(fallRate, riseRate, rising);
        s.process();

        env = _mm_switch_ps(s.output, _mm_sub_ps(targetValue, s.seg), rising);

        prevTrigger = trigger;
        return env;
    }

    __m128 hasFinished() const {
        return _mm_cmplt_ps(env, s.epsilon);
    }
};
