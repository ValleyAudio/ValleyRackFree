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
    __m128 __env;
    __m128 __attack, __decay;
    __m128 __cutoff, __maxCutoff;
    __m128 __filterSwitch;
    __m128 __vca, __filter, __output;
    VecOnePoleLPFilter _lpf1, _lpf2;

    enum Modes {
        BYPASS_MODE = 0,
        VCA_MODE,
        FILTER_MODE,
        BOTH_MODE
    };
    Modes mode;

    VecLPG() {
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __halfs = _mm_set1_ps(0.5f);
        __quarters = _mm_set1_ps(0.25f);
        __scale = _mm_set1_ps(0.75f);
        __longOffset = _mm_set1_ps(0.25f);
        __offset = __zeros;
        __env = __zeros;
        __maxCutoff = _mm_set1_ps(22050.f);
        __cutoff = __maxCutoff;
        setDecay(__ones, false);
        mode = BYPASS_MODE;
    }

    __m128 process(const __m128& x, const __m128& trigger) {
        __env = __envelope.process(trigger);
        __vca = _mm_mul_ps(x, __env);
        __cutoff = _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(__env, __env), __env), __maxCutoff);
        _lpf1.setCutoffFreqAlt(_mm_mul_ps(__cutoff, _mm_set1_ps(0.5f)));
        _lpf2._a = _lpf1._a;
        _lpf2._b = _lpf1._b;
        __filter = _lpf2.process(_lpf1.process(x));

        __output = x;
        switch (mode) {
            case BYPASS_MODE : break;
            case VCA_MODE : __output = __vca; break;
            case FILTER_MODE : __output = __filter; break;
            case BOTH_MODE : __output = _mm_mul_ps(__env, __filter); break;
        }

        return __output;
    }

    void clear() {
        __env = __zeros;
    }

    void setAttack(const __m128& attack, bool longScale) {
        __offset = longScale ? __longOffset : __zeros;
        __m128 x = _mm_mul_ps(_mm_clamp_ps(attack, __zeros, __ones), _mm_add_ps(__scale, __offset));
        x = _mm_sub_ps(_mm_set1_ps(0.62f), _mm_mul_ps(_mm_set1_ps(0.62f), x));
        __attack = _mm_mul_ps(x, x);
        __attack = _mm_mul_ps(__attack, x);
        __attack = _mm_mul_ps(__attack, x);
        __attack = _mm_mul_ps(__attack, x);
        __attack = _mm_mul_ps(__attack, x);
        __attack = _mm_sub_ps(_mm_set1_ps(0.99999f), __attack);
        __envelope.riseRate = __attack;
    }

    void setDecay(const __m128& decay, bool longScale) {
        __offset = longScale ? __longOffset : __zeros;
        __m128 x = _mm_mul_ps(_mm_clamp_ps(decay, __zeros, __ones), _mm_add_ps(__scale, __offset));
        x = _mm_sub_ps(_mm_set1_ps(0.4f), _mm_mul_ps(_mm_set1_ps(0.4f), x));
        __decay = _mm_mul_ps(x, x);
        __decay = _mm_mul_ps(__decay, x);
        __decay = _mm_mul_ps(__decay, x);
        __decay = _mm_mul_ps(__decay, x);
        __decay = _mm_mul_ps(__decay, x);
        __decay = _mm_sub_ps(_mm_set1_ps(0.999995f), __decay);
        __envelope.fallRate = __decay;
    }

    void setSampleRate(float newSampleRate) {
        _lpf1.setSampleRate(newSampleRate);
        _lpf2.setSampleRate(newSampleRate);
        __envelope.setTimeScale(_mm_div_ps(_mm_set1_ps(newSampleRate), _mm_set1_ps(44100.f)));
    }

    void setTriggerMode(bool triggerMode) {
        __envelope.inOneShotMode = triggerMode;
    }

private:
    __m128 __zeros, __ones, __halfs, __quarters;
    __m128 __scale, __offset, __longOffset;
    VecAREnvelope __envelope;
};
