#include "LinearEnvelope.hpp"

LinearEnvelope::LinearEnvelope() {
    _value = 0.f;
    _t = 0.f;
    _running = false;
    _justFinished = false;
    _time = 1.f;
    _sampleRate = 44100.0;
    setStartEndPoints(0.f, 1.f);
}

float LinearEnvelope::process() {
    if(_running) {
        _t += _deltaT;
        _value = _t * _m + _start;
    }
    else {
        _justFinished = false;
    }
    if(_t >= 1.f && _running) {
        _t = 0.f;
        _running = false;
        _justFinished = true;
    }
    return _value;
}

void LinearEnvelope::trigger() {
    _t = 0.f;
    _running = true;
    _justFinished = false;
}

void LinearEnvelope::setStartEndPoints(float startPoint, float endPoint) {
    _start = startPoint;
    _end = endPoint;
    calculateDeltaV();
}

void LinearEnvelope::setTime(float time) {
    _time = time;
    calculateDeltaV();
}

void LinearEnvelope::setSampleRate(float sampleRate) {
    _sampleRate = sampleRate;
    calculateDeltaV();
}

void LinearEnvelope::calculateDeltaV() {
    _m = _end - _start;
    _deltaT = 1.f / (_time * _sampleRate);
}
