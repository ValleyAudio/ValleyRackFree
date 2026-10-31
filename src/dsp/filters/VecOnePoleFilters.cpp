#include "VecOnePoleFilters.hpp"

VecOnePoleLPFilter::VecOnePoleLPFilter() {
    cutoffFreq = 0.f;
    setSampleRate(44100.f);
    setCutoffFreq(sampleRate / 2.f);
    clear();
}

VecOnePoleLPFilter::VecOnePoleLPFilter(float cutoffFreq) {
    setSampleRate(44100.f);
    setCutoffFreq(cutoffFreq);
    clear();
}

__m128 VecOnePoleLPFilter::process(const __m128& input) {
    z =  _mm_add_ps(_mm_mul_ps(a, input), _mm_mul_ps(z, b));
    return z;
}

void VecOnePoleLPFilter::clear() {
    z = _mm_set1_ps(0.f);
}

void VecOnePoleLPFilter::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    sampleTime = 1.f / sampleRate;
    maxCutoffFreq = sampleRate / 2.f - 2.f;
    setCutoffFreq(cutoffFreq);
}

void VecOnePoleLPFilter::setCutoff(const __m128& newCutoff) {
    b = newCutoff;
    a = _mm_sub_ps(_mm_set1_ps(1.f), b);
}

void VecOnePoleLPFilter::setCutoffFreq(float newCutoffFreq) {
    cutoffFreq = newCutoffFreq > maxCutoffFreq ? maxCutoffFreq : newCutoffFreq;
    b = _mm_set1_ps(expf(-_2M_PI * cutoffFreq * sampleTime));
    a = _mm_sub_ps(_mm_set1_ps(1.f), b);
}

void VecOnePoleLPFilter::setCutoffFreq(const __m128& newCutoffFreq) {
    fc = _mm_clamp_ps(newCutoffFreq, _mm_set1_ps(1.f), _mm_set1_ps(maxCutoffFreq));
    fc = _mm_mul_ps(fc, _mm_set1_ps(-_2M_PI * sampleTime));
    b = valley::_mm_exp_ps(fc);
    a = _mm_sub_ps(_mm_set1_ps(1.f), b);
}

void VecOnePoleLPFilter::setCutoffFreqAlt(float newCutoffFreq) {
    cutoffFreq = newCutoffFreq > maxCutoffFreq ? maxCutoffFreq : newCutoffFreq;
    a = _mm_set1_ps(sin(_2M_PI * cutoffFreq * sampleTime));
    b = _mm_sub_ps(_mm_set1_ps(1.f), a);
}

void VecOnePoleLPFilter::setCutoffFreqAlt(const __m128& newCutoffFreq) {
    fc = _mm_clamp_ps(newCutoffFreq, _mm_set1_ps(1.f), _mm_set1_ps(maxCutoffFreq));
    fc = _mm_mul_ps(fc, _mm_set1_ps(sin(_2M_PI * sampleTime)));
    a = valley::_mm_sine_ps(fc);
    b = _mm_sub_ps(_mm_set1_ps(1.f), a);

}

float VecOnePoleLPFilter::getMaxCutoffFreq() const {
    return maxCutoffFreq;
}

///////////////////////////////////////////////////////////////////////////////

VecOnePoleHPFilter::VecOnePoleHPFilter() {
    cutoffFreq = 0.f;
    setSampleRate(44100.f);
    setCutoffFreq(sampleRate / 2.f);
    clear();
    _bypassed = false;
}

VecOnePoleHPFilter::VecOnePoleHPFilter(float newCutoffFreq) {
    setSampleRate(44100.f);
    setCutoffFreq(newCutoffFreq);
    clear();
    _bypassed = false;
}

__m128 VecOnePoleHPFilter::process(const __m128& input) {
    z =  _mm_add_ps(_mm_mul_ps(a, input), _mm_mul_ps(z, b));
    return _bypassed ? input : _mm_sub_ps(input, z);
}

void VecOnePoleHPFilter::clear() {
    z = _mm_set1_ps(0.f);
}

void VecOnePoleHPFilter::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    sampleTime = 1.f / sampleRate;
    maxCutoffFreq = sampleRate / 2.f - 2.f;
    setCutoffFreq(cutoffFreq);
}

void VecOnePoleHPFilter::setCutoffFreq(float newCutoffFreq) {
    cutoffFreq = newCutoffFreq > maxCutoffFreq ? maxCutoffFreq : newCutoffFreq;
    b = _mm_set1_ps(expf(-_2M_PI * cutoffFreq * sampleTime));
    a = _mm_sub_ps(_mm_set1_ps(1.f), b);
}

void VecOnePoleHPFilter::setCutoffFreq(const __m128& newCutoffFreq) {
    fc = _mm_clamp_ps(newCutoffFreq, _mm_set1_ps(1.f), _mm_set1_ps(maxCutoffFreq));
    fc = _mm_mul_ps(fc, _mm_set1_ps(-_2M_PI * sampleTime));
    b = valley::_mm_exp_ps(fc);
    a = _mm_sub_ps(_mm_set1_ps(1.f), b);
}

float VecOnePoleHPFilter::getMaxCutoffFreq() const {
    return maxCutoffFreq;
}

void VecOnePoleHPFilter::setBypass(bool bypass) {
    _bypassed = bypass;
}
