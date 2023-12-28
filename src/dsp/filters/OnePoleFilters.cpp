#include "OnePoleFilters.hpp"
#include <cassert>

OnePoleLPFilter::OnePoleLPFilter(double cutoffFreq, double initSampleRate) {
    setSampleRate(initSampleRate);
    setCutoffFreq(cutoffFreq);
}

double OnePoleLPFilter::process() {
    z =  a * input + z * b;
    output = z;
    return output;
}

void OnePoleLPFilter::clear() {
    input = 0.0;
    z = 0.0;
    output = 0.0;
}

void OnePoleLPFilter::setSampleRate(double newSampleRate) {
    assert(sampleRate > 0.0);

    sampleRate = newSampleRate;
    sampleTime = 1.0 / sampleRate;
    maxCutoffFreq = sampleRate / 2.0 - 1.0;
    setCutoffFreq(cutoffFreq);
}

void OnePoleLPFilter::setCutoffFreq(double newCutoffFreq) {
    if (cutoffFreq == newCutoffFreq) {
        return;
    }

    assert(cutoffFreq > 0.0);
    assert(cutoffFreq <= maxCutoffFreq);

    cutoffFreq = newCutoffFreq;
    b = expf(-VALLEY_2M_PI * cutoffFreq * sampleTime);
    a = 1.0 - b;
}

double OnePoleLPFilter::getMaxCutoffFreq() const {
    return maxCutoffFreq;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

OnePoleHPFilter::OnePoleHPFilter(double initCutoffFreq, double initSampleRate) {
    setSampleRate(initSampleRate);
    setCutoffFreq(initCutoffFreq);
    clear();
}

double OnePoleHPFilter::process() {
    x0 = input;
    y0 = a0 * x0 + a1 * x1 + b1 * y1;
    y1 = y0;
    x1 = x0;
    output = y0;
    return y0;
}

void OnePoleHPFilter::clear() {
    input = 0.0;
    output = 0.0;
    x0 = 0.0;
    x1 = 0.0;
    y0 = 0.0;
    y1 = 0.0;
}

void OnePoleHPFilter::setCutoffFreq(double newCutoffFreq) {
    if (cutoffFreq == newCutoffFreq) {
        return;
    }

    assert(newCutoffFreq > 0.0);
    assert(newCutoffFreq <= maxCutoffFreq);

    cutoffFreq = newCutoffFreq;
    b1 = expf(-VALLEY_2M_PI * cutoffFreq * sampleTime);
    a0 = (1.0 + b1) / 2.0;
    a1 = -a0;
}

void OnePoleHPFilter::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    sampleTime = 1.0 / sampleRate;
    maxCutoffFreq = sampleRate / 2.0 - 1.0;
    setCutoffFreq(cutoffFreq);
    clear();
}

DCBlocker::DCBlocker() {
    setSampleRate(44100.0);
    setCutoffFreq(20.f);
    clear();
}

DCBlocker::DCBlocker(double cutoffFreq) {
    setSampleRate(44100.0);
    setCutoffFreq(cutoffFreq);
    clear();
}

double DCBlocker::process(double input) {
    output = input - z + b * output;
    z = input;
    return output;
}

void DCBlocker::clear() {
    z = 0.0;
    output = 0.0;
}

void DCBlocker::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    maxCutoffFreq = sampleRate / 2.0;
    setCutoffFreq(cutoffFreq);
}

void DCBlocker::setCutoffFreq(double newCutoffFreq) {
    cutoffFreq = newCutoffFreq;
    b = 0.999;
}

double DCBlocker::getMaxCutoffFreq() const {
    return maxCutoffFreq;
}
