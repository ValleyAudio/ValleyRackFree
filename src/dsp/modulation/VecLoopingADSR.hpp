//
// VecLoopingADSR.hpp
// Author: Dale Johnson
//
// Copyright (c) 2020 Dale Johnson, Valley Audio
// Licensed under GPLv3
//
// Based on the ADSR module from the VCV Fundamental plugin.
// It adds a looping mode and timescale parameter.
//

#pragma once

#include "rack.hpp"
#include "simd/functions.hpp"

using namespace rack::simd;

class VecLoopingADSR {
public:
    float_4 attack = float_4::zero();
    float_4 decay = float_4::zero();
    float_4 sustain = float_4::zero();
    float_4 release = float_4::zero();
    float_4 env = float_4::zero();
    float sampleTime = 1.f / 44100.f;

    bool looping = false;

    VecLoopingADSR() = default;

    void process(const float_4& gate, const float_4& trigger);
    void setTimeScale(float timeScale);
    void setSampleRate(float newSampleRate);
    void setADSR(const float_4& attack, const float_4& decay,
                 const float_4& sustain, const float_4& release);

private:
    float MIN_TIME = 1e-3f;
    float MAX_TIME = 10.f;
    float lamdaBase = MAX_TIME / MIN_TIME;
    float minTimeScaled = MAX_TIME;
    float maxTimeScaled = MIN_TIME;
    float EPSILON = 0.01f;

    float_4 attacking = float_4::zero();
    float_4 sustaining = float_4::zero();

    float_4 attackLambda = float_4::zero();
    float_4 decayLambda = float_4::zero();
    float_4 releaseLambda = float_4::zero();
    float_4 gated = float_4::zero();
    float_4 prevGated = float_4::zero();
    float_4 triggered = float_4::zero();
    float_4 prevTriggered = float_4::zero();
    float_4 latched = float_4::zero();
};
