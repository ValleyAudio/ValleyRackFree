#include "OnePoleFilters.hpp"

OnePoleLPFilter::OnePoleLPFilter() {
    setSampleRate(44100.f);
    setCutoffFreq(_sampleRate / 2.f);
    clear();
}

OnePoleLPFilter::OnePoleLPFilter(double cutoffFreq) {
    setSampleRate(44100.f);
    setCutoffFreq(cutoffFreq);
    clear();
}

double OnePoleLPFilter::process() {
    _z =  _a * input + _z * _b;
    output = _z;
    return output;
}

void OnePoleLPFilter::clear() {
    input = 0.f;
    _z = 0.f;
    output = 0.f;
}

void OnePoleLPFilter::setSampleRate(double sampleRate) {
    _sampleRate = sampleRate;
    _maxCutoffFreq = sampleRate / 2.f - 1.f;
    setCutoffFreq(_cutoffFreq);
}

void OnePoleLPFilter::setCutoffFreq(double cutoffFreq) {
    _cutoffFreq = cutoffFreq;
    _b = expf(-2.f* M_PI * _cutoffFreq / _sampleRate);
    _a = 1.f - _b;
}

double OnePoleLPFilter::getMaxCutoffFreq() const {
    return _maxCutoffFreq;
}


OnePoleHPFilter::OnePoleHPFilter() {
    _sampleRate = 44100.f;
    setCutoffFreq(0.f);
    clear();
}

OnePoleHPFilter::OnePoleHPFilter(double cutoffFreq) {
    _sampleRate = 44100.f;
    setCutoffFreq(cutoffFreq);
    clear();
}

double OnePoleHPFilter::process() {
    _x0 = input;
    _y0 = _a0 * _x0 + _a1 * _x1 + _b1 * _y1;
    _y1 = _y0;
    _x1 = _x0;
    output = _y0;
    return _y0;
}

void OnePoleHPFilter::clear() {
    input = 0.f;
    output = 0.f;
    _x0 = 0.f;
    _x1 = 0.f;
    _y0 = 0.f;
    _y1 = 0.f;
}

void OnePoleHPFilter::setCutoffFreq(double cutoffFreq) {
    _cutoffFreq = cutoffFreq;
    _b1 = expf(-2.0 * M_PI * _cutoffFreq / _sampleRate);
    _a0 = (1.f + _b1) / 2.f;
    _a1 = -_a0;
}

void OnePoleHPFilter::setSampleRate(double sampleRate) {
    _sampleRate = sampleRate;
    setCutoffFreq(_cutoffFreq);
    clear();
}

int DCBlocker::i = 0;

DCBlocker::DCBlocker() {
    id = i;
    i++;
    setSampleRate(44100.f);
    setCutoffFreq(_sampleRate / 2.f);
    clear();
}

DCBlocker::DCBlocker(double cutoffFreq) {
    setSampleRate(44100.f);
    setCutoffFreq(cutoffFreq);
    clear();
}

double DCBlocker::process() {
    output = input - _z + _b * output;
    _z = input;
    return output;
}

void DCBlocker::clear() {
    input = 0.f;
    _z = 0.f;
    output = 0.f;
}

void DCBlocker::setSampleRate(double sampleRate) {
    _sampleRate = sampleRate;
    _maxCutoffFreq = sampleRate / 2.f;
    setCutoffFreq(_cutoffFreq);
}

void DCBlocker::setCutoffFreq(double cutoffFreq) {
    _cutoffFreq = cutoffFreq;
    _b = 0.995f;
}

double DCBlocker::getMaxCutoffFreq() const {
    return _maxCutoffFreq;
}
