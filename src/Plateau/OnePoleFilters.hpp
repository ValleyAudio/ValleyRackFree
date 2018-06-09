#pragma once
#include <cmath>
#include <stdio.h>

class OnePoleLPFilter {
public:
    OnePoleLPFilter();
    OnePoleLPFilter(double cutoffFreq);
    double process();
    void clear();
    void setCutoffFreq(double cutoffFreq);
    void setSampleRate(double sampleRate);
    double getMaxCutoffFreq() const;
    double input;
    double output;
private:
    double _sampleRate;
    double _cutoffFreq;
    double _maxCutoffFreq;
    double _a;
    double _b;
    double _z;
};

class OnePoleHPFilter {
public:
    OnePoleHPFilter();
    OnePoleHPFilter(double cutoffFreq);
    double process();
    void clear();
    void setCutoffFreq(double cutoffFreq);
    void setSampleRate(double sampleRate);
    double input;
    double output;
private:
    double _sampleRate;
    double _cutoffFreq;
    double _y0;
    double _y1;
    double _x0;
    double _x1;
    double _a0;
    double _a1;
    double _b1;
};

class DCBlocker {
public:
    static int i;
    int id;
    DCBlocker();
    DCBlocker(double cutoffFreq);
    double process();
    void clear();
    void setCutoffFreq(double cutoffFreq);
    void setSampleRate(double sampleRate);
    double getMaxCutoffFreq() const;
    double input;
    double output;
private:
    double _sampleRate;
    double _cutoffFreq;
    double _maxCutoffFreq;
    double _a;
    double _b;
    double _z;
};
