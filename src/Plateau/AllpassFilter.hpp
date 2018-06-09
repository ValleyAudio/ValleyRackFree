#pragma once
#include "InterpDelay.hpp"

class AllpassFilter {
public:
    AllpassFilter();
    AllpassFilter(long maxDelay, long initDelay, float gain);
    float process();
    void clear();

    float input;
    float gain;
    float output;
    InterpDelay delay;
private:
    float _inSum;
    float _outSum;
};
