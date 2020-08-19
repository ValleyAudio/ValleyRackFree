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
    __m128 risingEdge, fallingEdge;
    __m128 riseFinished;
    __m128 targetValue;
    __m128 inputEpsilon, risingEpsilon, fallingEpsilon;
    bool inOneShotMode;

    VecAREnvelope() {
        zeros = _mm_set1_ps(0.f);
        ones = _mm_set1_ps(1.f);
        high = _mm_high_ps();
        env = zeros;
        prevTrigger = zeros;
        rising = zeros;
        falling = high;
        risingEdge = zeros;
        fallingEdge = zeros;
        riseFinished = zeros;
        targetValue = zeros;

        riseRate = _mm_set1_ps(0.99f);
        fallRate = _mm_set1_ps(0.99f);
        inputEpsilon = _mm_set1_ps(0.0001f);
        risingEpsilon = _mm_set1_ps(0.002f);
        fallingEpsilon = _mm_set1_ps(0.000031f);
        inOneShotMode = false;
    }

    __m128 process(const __m128& trigger) {
        // Generate edge triggers that set start values when trigger changes
        risingEdge = _mm_and_ps(_mm_cmpgt_ps(trigger, inputEpsilon),
                                _mm_cmple_ps(prevTrigger, inputEpsilon));
        if (inOneShotMode) {
            rising = _mm_switch_ps(rising, risingEdge, risingEdge);
            fallingEdge = _mm_and_ps(rising, _mm_andnot_ps(risingEdge, s.hasFinished()));
            rising = _mm_switch_ps(rising, zeros, fallingEdge);
        }
        else {
            fallingEdge = _mm_and_ps(_mm_cmple_ps(trigger, inputEpsilon),
                                     _mm_cmpgt_ps(prevTrigger, inputEpsilon));
            rising = _mm_cmpgt_ps(trigger, zeros);
        }

        falling = _mm_andnot_ps(rising, _mm_high_ps());
        targetValue = _mm_switch_ps(targetValue, trigger, risingEdge);

        // Now set a start value when an edge is detected
        s.setStartValue(_mm_switch_ps(s.seg, _mm_sub_ps(targetValue, s.seg), risingEdge));
        s.setStartValue(_mm_switch_ps(s.seg, _mm_sub_ps(targetValue, s.seg), fallingEdge));

        s.epsilon = _mm_switch_ps(s.epsilon, risingEpsilon, risingEdge);
        s.epsilon = _mm_switch_ps(s.epsilon, fallingEpsilon, fallingEdge);

        s.rate = _mm_switch_ps(fallRate, riseRate, rising);
        s.process();
        env = _mm_switch_ps(s.seg, _mm_sub_ps(targetValue, s.seg), rising);

        prevTrigger = trigger;
        return env;
    }

    void setTimeScale(const __m128& tScale) {
        s.setTimeScale(tScale);
    }

    __m128 hasFinished() const {
        return _mm_cmplt_ps(env, s.epsilon);
    }
};
