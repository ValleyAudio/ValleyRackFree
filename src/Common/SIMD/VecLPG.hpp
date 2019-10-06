#pragma once
#include "SIMDUtilities.hpp"
#include "VecOnePoleFilters.hpp"

struct VecLPG {
    VecLPG() {
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __y = __zeros;
        __maxCutoff = _mm_set1_ps(22050.f);
        __cutoff = __maxCutoff;
        setDecay(__ones);
        enableFilter(false);
    }

    __m128 process(const __m128& x, const __m128& trigger) {
        __y = _mm_switch_ps(__y, trigger, _mm_cmpgt_ps(trigger, __zeros));
        __y = _mm_mul_ps(__y, __decay);
        __cutoff = _mm_switch_ps(__maxCutoff, _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(__y, __y), __y),
                                 __maxCutoff), __filterSwitch);
        _lpf1.setCutoffFreq(__cutoff);
        _lpf2._a = _lpf1._a;
        _lpf2._b = _lpf1._b;
        return _lpf2.process(_lpf1.process(x));;
    }

    __m128 getEnvelope() const {
        return __y;
    }

    void enableFilter(bool enable) {
        if(enable) {
            __filterSwitch = _mm_high_ps();
        }
        else {
            __filterSwitch = __zeros;
        }
    }

    void clear() {
        __y = __zeros;
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

    __m128 __y;
    __m128 __decay;
    __m128 __sampleRate;
    __m128 __zeros, __ones;
    __m128 __cutoff, __maxCutoff;
    __m128 __filterSwitch;
    VecOnePoleLPFilter _lpf1, _lpf2;
};
