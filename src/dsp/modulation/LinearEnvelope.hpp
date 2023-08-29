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

    float _value;
    bool _justFinished;
private:
    void calculateDeltaV();

    float _t;
    float _time;
    float _m;
    float _deltaT;
    float _start, _end;
    float _sampleRate;
    bool _running;
};
