//
// VecLoopingADSR.cpp
// Author: Dale Johnson
//
// Copyright (c) 2020 Dale Johnson, Valley Audio
// Licensed under GPLv3
//

#include "VecLoopingADSR.hpp"

using namespace rack;
using namespace rack::simd;

void VecLoopingADSR::process(const float_4& gate, const float_4& trigger) {

    // Gate and Looping
    prevGated = gated;
    gated = gate >= 1.f;
    if (looping) {
        gated = simd::ifelse(prevGated | latched, float_4::mask(), gated);
        attacking = simd::ifelse(sustaining, float_4::mask(), attacking);
    }

    // Retrigger
    prevTriggered = triggered;
    triggered = trigger >= 1.f;
    latched = simd::ifelse(~prevTriggered & triggered, float_4::mask(), latched);
    latched = simd::ifelse(gated, float_4::zero(), latched);
    attacking = simd::ifelse(triggered, float_4::mask(), attacking);

    // Get target and lambda for exponential decay
    const float attackTarget = 1.2f;
    float_4 target = simd::ifelse(gated, simd::ifelse(attacking, attackTarget, sustain), simd::ifelse(latched, attackTarget, 0.f));
    float_4 lambda = simd::ifelse(gated | latched, simd::ifelse(attacking, attackLambda, decayLambda), releaseLambda);

    // Adjust env
    env += (target - env) * lambda * sampleTime;

    // Turn off attacking state if envelope is HIGH
    attacking = simd::ifelse(env >= 1.f, float_4::zero(), attacking);
    latched = simd::ifelse(env >= 1.f, float_4::zero(), latched);

    // Turn on attacking state if gate is LOW
    attacking = simd::ifelse(gated, attacking, float_4::mask());

    sustaining = (sustain <= env) & (env < sustain + EPSILON);
}

void VecLoopingADSR::setTimeScale(float timeScale) {
    timeScale = clamp(timeScale, 0.f, 10.f);
    minTimeScaled = MIN_TIME * timeScale;
    maxTimeScaled = MAX_TIME * timeScale;
    lamdaBase = maxTimeScaled / minTimeScaled;
}

void VecLoopingADSR::setSampleRate(float newSampleRate) {
    sampleTime = 1.f / newSampleRate;
}

void VecLoopingADSR::setADSR(const float_4& attack, const float_4& decay,
                             const float_4& sustain, const float_4& release) {
    this->attack = simd::clamp(attack, 0.f, 1.f);
    this->decay = simd::clamp(decay, 0.f, 1.f);
    this->sustain = simd::clamp(sustain, 0.f, 1.f);
    this->release = simd::clamp(release, 0.f, 1.f);

    attackLambda = simd::pow(lamdaBase, -this->attack) / minTimeScaled;
    decayLambda = simd::pow(lamdaBase, -this->decay) / minTimeScaled;
    releaseLambda = simd::pow(lamdaBase, -this->release) / minTimeScaled;
}
