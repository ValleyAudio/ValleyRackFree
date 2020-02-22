#pragma once
#include "SIMDUtilities.hpp"
#include "VecOnePoleFilters.hpp"

struct VecLPG {
    __m128 __env;
    __m128 __decay;
    __m128 __sampleRate;
    __m128 __zeros, __ones;
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
        __env = __zeros;
        __maxCutoff = _mm_set1_ps(22050.f);
        __cutoff = __maxCutoff;
        setDecay(__ones);
        mode = BYPASS_MODE;
    }

    __m128 process(const __m128& x, const __m128& trigger) {
        __env = _mm_switch_ps(__env, trigger, _mm_cmpgt_ps(trigger, __zeros));
        __env = _mm_mul_ps(__env, __decay);
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

    inline void clear() {
        __env = __zeros;
    }

    void setDecay(const __m128& decay) {
        __decay = _mm_sub_ps(__ones, decay);
        __decay = _mm_mul_ps(__decay, __decay);
        __decay = _mm_mul_ps(__decay, __decay);
        __decay = _mm_mul_ps(__decay, __decay);
        __decay = _mm_mul_ps(__decay, _mm_set1_ps(0.001995));
        __decay = _mm_add_ps(__decay, _mm_set1_ps(0.000005));
        __decay = _mm_sub_ps(__ones, __decay);
    }

    void setSampleRate(float newSampleRate) {
        _lpf1.setSampleRate(newSampleRate);
        _lpf2.setSampleRate(newSampleRate);
    }
};
