#pragma once
#include <cmath>
#include "../../simd/SIMDUtilities.hpp"
#define _2M_PI 2.0 * M_PI

struct VecOnePoleLPFilter {
    VecOnePoleLPFilter();
    VecOnePoleLPFilter(float cutoffFreq);
    __m128 process(const __m128& input);
    void clear();
    void setCutoff(const __m128& newCutoff);
    void setCutoffFreq(float newCutoffFreq);
    void setCutoffFreq(const __m128& newCutoffFreq);
    void setCutoffFreqAlt(float newCutoffFreq);
    void setCutoffFreqAlt(const __m128& newCutoffFreq);
    void setSampleRate(float newSampleRate);
    float getMaxCutoffFreq() const;

    float sampleRate;
    float sampleTime;
    float cutoffFreq;
    float maxCutoffFreq;
    __m128 fc;
    __m128 a;
    __m128 b;
    __m128 z;
};

///////////////////////////////////////////////////////////////////////////////

struct VecOnePoleHPFilter {
    VecOnePoleHPFilter();
    VecOnePoleHPFilter(float cutoffFreq);
    __m128 process(const __m128& input);
    void clear();
    void setCutoffFreq(float newCutoffFreq);
    void setCutoffFreq(const __m128& newCutoffFreq);
    void setSampleRate(float newSampleRate);
    float getMaxCutoffFreq() const;
    void setBypass(bool bypass);

    float sampleRate;
    float sampleTime;
    float cutoffFreq;
    float maxCutoffFreq;
    bool _bypassed;
    __m128 fc;
    __m128 a;
    __m128 b;
    __m128 z;
};
