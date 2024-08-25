#include "LinearEnvelope.hpp"

LinearEnvelope::LinearEnvelope() {
    setSampleRate(44100.f);
    setStartEndPoints(0.f, 1.f);
}

float LinearEnvelope::process() {
    if(isRunning) {
        t += deltaT;
        currentValue = t * gradient + startValue;
    }
    else {
        justFinished = false;
    }
    if(t >= 1.f && isRunning) {
        t = 0.f;
        isRunning = false;
        justFinished = true;
    }
    return currentValue;
}

void LinearEnvelope::trigger() {
    t = 0.f;
    isRunning = true;
    justFinished = false;
}

void LinearEnvelope::setStartEndPoints(float startPoint, float endPoint) {
    startValue = startPoint;
    endValue = endPoint;
    calculateDeltaV();
}

void LinearEnvelope::setTime(float newTime) {
    time = newTime;
    calculateDeltaV();
}

void LinearEnvelope::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    calculateDeltaV();
}

void LinearEnvelope::calculateDeltaV() {
    gradient = endValue - startValue;
    deltaT = 1.f / (time * sampleRate);
}
