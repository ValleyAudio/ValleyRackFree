//
//  VecLPG.hpp
//
//  Created by Dale Johnson on 6/12/2019.
//  Copyright © 2019 Dale Johnson. All rights reserved.
//

#pragma once
#include "../../simd/SIMDUtilities.hpp"
#include "../modulation/VecAREnvelope.hpp"
#include "../filters/VecOnePoleFilters.hpp"

/** A vectorised lowpass gate with simple AR envelope generator.
*/
class VecLPG {
public:
    __m128 env;
    __m128 attack, decay;
    __m128 cutoff, maxCutoff;
    __m128 filterSwitch;
    __m128 vca, filter, output;
    VecOnePoleLPFilter lpf1, lpf2;

    enum Modes {
        BYPASS_MODE = 0,
        VCA_MODE,
        FILTER_MODE,
        BOTH_MODE
    };
    Modes mode;

    VecLPG() {
        zeros = _mm_set1_ps(0.f);
        ones = _mm_set1_ps(1.f);
        halfs = _mm_set1_ps(0.5f);
        quarters = _mm_set1_ps(0.25f);
        scale = _mm_set1_ps(0.75f);
        longOffset = _mm_set1_ps(0.25f);
        offset = zeros;
        env = zeros;
        maxCutoff = _mm_set1_ps(22050.f);
        cutoff = maxCutoff;
        setDecay(ones, false);
        mode = BYPASS_MODE;
    }

    __m128 process(const __m128& x, const __m128& trigger) {
        env = envelope.process(trigger);
        vca = _mm_mul_ps(x, env);
        cutoff = _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(env, env), env), maxCutoff);
        lpf1.setCutoffFreqAlt(_mm_mul_ps(cutoff, _mm_set1_ps(0.5f)));
        lpf2._a = lpf1._a;
        lpf2._b = lpf1._b;
        filter = lpf2.process(lpf1.process(x));

        output = x;
        switch (mode) {
            case BYPASS_MODE : break;
            case VCA_MODE : output = vca; break;
            case FILTER_MODE : output = filter; break;
            case BOTH_MODE : output = _mm_mul_ps(env, filter); break;
        }

        return output;
    }

    void clear() {
        env = zeros;
    }

    void setAttack(const __m128& newAttack, bool longScale) {
        offset = longScale ? longOffset : zeros;
        __m128 x = _mm_mul_ps(_mm_clamp_ps(newAttack, zeros, ones), _mm_add_ps(scale, offset));
        x = _mm_sub_ps(_mm_set1_ps(0.62f), _mm_mul_ps(_mm_set1_ps(0.62f), x));
        attack = _mm_mul_ps(x, x);
        attack = _mm_mul_ps(attack, x);
        attack = _mm_mul_ps(attack, x);
        attack = _mm_mul_ps(attack, x);
        attack = _mm_mul_ps(attack, x);
        attack = _mm_sub_ps(_mm_set1_ps(0.99999f), attack);
        envelope.riseRate = attack;
    }

    void setDecay(const __m128& newDecay, bool longScale) {
        offset = longScale ? longOffset : zeros;
        __m128 x = _mm_mul_ps(_mm_clamp_ps(newDecay, zeros, ones), _mm_add_ps(scale, offset));
        x = _mm_sub_ps(_mm_set1_ps(0.4f), _mm_mul_ps(_mm_set1_ps(0.4f), x));
        decay = _mm_mul_ps(x, x);
        decay = _mm_mul_ps(decay, x);
        decay = _mm_mul_ps(decay, x);
        decay = _mm_mul_ps(decay, x);
        decay = _mm_mul_ps(decay, x);
        decay = _mm_sub_ps(_mm_set1_ps(0.999995f), decay);
        envelope.fallRate = decay;
    }

    void setSampleRate(float newSampleRate) {
        lpf1.setSampleRate(newSampleRate);
        lpf2.setSampleRate(newSampleRate);
        envelope.setTimeScale(_mm_div_ps(_mm_set1_ps(newSampleRate), _mm_set1_ps(44100.f)));
    }

    void setTriggerMode(bool triggerMode) {
        envelope.inOneShotMode = triggerMode;
    }

private:
    __m128 zeros, ones, halfs, quarters;
    __m128 scale, offset, longOffset;
    VecAREnvelope envelope;
};
