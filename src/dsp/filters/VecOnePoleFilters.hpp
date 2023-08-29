#pragma once
#include <cmath>
#include "../../simd/SIMDUtilities.hpp"
#define _2M_PI 2.0 * M_PI

struct VecOnePoleLPFilter {
    VecOnePoleLPFilter();
    VecOnePoleLPFilter(float cutoffFreq);
    __m128 process(const __m128& input);
    void clear();
    void setCutoff(const __m128& cutoff);
    void setCutoffFreq(float cutoffFreq);
    void setCutoffFreq(const __m128& cutoffFreq);
    void setCutoffFreqAlt(float cutoffFreq);
    void setCutoffFreqAlt(const __m128& cutoffFreq);
    void setSampleRate(float sampleRate);
    float getMaxCutoffFreq() const;

    float _sampleRate;
    float _1_sampleRate;
    float _cutoffFreq;
    float _maxCutoffFreq;
    __m128 _fc;
    __m128 _a;
    __m128 _b;
    __m128 _z;
};

///////////////////////////////////////////////////////////////////////////////

struct VecOnePoleHPFilter {
    VecOnePoleHPFilter();
    VecOnePoleHPFilter(float cutoffFreq);
    __m128 process(const __m128& input);
    void clear();
    void setCutoffFreq(float cutoffFreq);
    void setCutoffFreq(const __m128& cutoffFreq);
    void setSampleRate(float sampleRate);
    float getMaxCutoffFreq() const;
    void setBypass(bool bypass);

    float _sampleRate;
    float _1_sampleRate;
    float _cutoffFreq;
    float _maxCutoffFreq;
    bool _bypassed;
    __m128 _fc;
    __m128 _a;
    __m128 _b;
    __m128 _z;
};
