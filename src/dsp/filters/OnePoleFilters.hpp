#pragma once
#include <cmath>
#include <stdio.h>
#include <cstdint>

#define _1_FACT_2 0.5
#define _1_FACT_3 0.1666666667
#define _1_FACT_4 0.04166666667
#define _1_FACT_5 0.008333333333
#define _2M_PI 2.0 * M_PI

template<typename T>
T fastexp(T x) {
    T xx = x * x;
    T x3 = x * xx;
    T x4 = xx * xx;
    T x5 = x4 * x;
    x = 1 + x + (xx * _1_FACT_2) + (x3 * _1_FACT_3) + (x4 * _1_FACT_4);
    return x + (x5 * _1_FACT_5);
}

class OnePoleLPFilter {
public:
    OnePoleLPFilter(double cutoffFreq = 22049.0, double initSampleRate = 44100.0);
    double process();
    void clear();
    void setCutoffFreq(double cutoffFreq);
    void setSampleRate(double sampleRate);
    double getMaxCutoffFreq() const;
    double input = 0.0;
    double output = 0.0;
private:
    double _sampleRate = 44100.0;
    double _1_sampleRate = 1.0 / _sampleRate;
    double _cutoffFreq = 0.0;
    double _maxCutoffFreq = _sampleRate / 2.0;
    double _a = 0.0;
    double _b = 0.0;
    double _z = 0.0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////

class OnePoleHPFilter {
public:
    OnePoleHPFilter(double initCutoffFreq = 10.0, double initSampleRate = 44100.0);
    double process();
    void clear();
    void setCutoffFreq(double cutoffFreq);
    void setSampleRate(double sampleRate);
    double input = 0.0;
    double output = 0.0;
private:
    double _sampleRate = 0.0;
    double _1_sampleRate = 0.0;
    double _cutoffFreq = 0.0;
    double _maxCutoffFreq = _sampleRate / 2.0 - 1.0;
    double _y0 = 0.0;
    double _y1 = 0.0;
    double _x0 = 0.0;
    double _x1 = 0.0;
    double _a0 = 0.0;
    double _a1 = 0.0;
    double _b1 = 0.0;
};

class DCBlocker {
public:
    DCBlocker();
    DCBlocker(double cutoffFreq);
    double process(double input);
    void clear();
    void setCutoffFreq(double cutoffFreq);
    void setSampleRate(double sampleRate);
    double getMaxCutoffFreq() const;
    double output;
private:
    double _sampleRate;
    double _cutoffFreq;
    double _maxCutoffFreq;
    double _b;
    double _z;
};
