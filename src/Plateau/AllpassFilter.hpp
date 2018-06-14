#pragma once
#include "InterpDelay.hpp"

class AllpassFilter {
public:
    AllpassFilter();
    AllpassFilter(long maxDelay, long initDelay, double gain);
    double process();
    void clear();

    double input;
    double gain;
    double output;
    InterpDelay<double> delay;
private:
    double _inSum;
    double _outSum;
};
