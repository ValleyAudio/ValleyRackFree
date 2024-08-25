//
// LinearEnvelope.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 24/6/2019
//

#pragma once
#include <cstdio>

class LinearEnvelope {
public:
    LinearEnvelope();
    float process();
    void trigger();
    void setStartEndPoints(float startPoint, float endPoint);
    void setTime(float time);
    void setSampleRate(float sampleRate);

    float currentValue;
    bool justFinished;
private:
    void calculateDeltaV();

    float t = 0.f;
    float time = 1.f;
    float gradient = 0.f;
    float deltaT = 0.f;
    float startValue = 0.f;
    float endValue = 0.f;
    float sampleRate = 44100.f;
    bool isRunning = false;
};
