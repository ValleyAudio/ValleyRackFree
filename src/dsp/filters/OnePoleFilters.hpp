#pragma once
#include <cmath>
#include <stdio.h>
#include <cstdint>

#define VALLEY_2M_PI 2.0 * M_PI

class OnePoleLPFilter {
public:
    OnePoleLPFilter(double cutoffFreq = 22049.0, double initSampleRate = 44100.0);
    double process();
    void clear();
    void setCutoffFreq(double newCutoffFreq);
    void setSampleRate(double newSampleRate);
    double getMaxCutoffFreq() const;
    double input = 0.0;
    double output = 0.0;
private:
    double sampleRate = 44100.0;
    double sampleTime = 1.0 / sampleRate;
    double cutoffFreq = 0.0;
    double maxCutoffFreq = sampleRate / 2.0;
    double a = 0.0;
    double b = 0.0;
    double z = 0.0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////

class OnePoleHPFilter {
public:
    OnePoleHPFilter(double initCutoffFreq = 10.0, double initSampleRate = 44100.0);
    double process();
    void clear();
    void setCutoffFreq(double newCutoffFreq);
    void setSampleRate(double newSampleRate);
    double input = 0.0;
    double output = 0.0;
private:
    double sampleRate = 0.0;
    double sampleTime = 0.0;
    double cutoffFreq = 0.0;
    double maxCutoffFreq = sampleRate / 2.0 - 1.0;
    double y0 = 0.0;
    double y1 = 0.0;
    double x0 = 0.0;
    double x1 = 0.0;
    double a0 = 0.0;
    double a1 = 0.0;
    double b1 = 0.0;
};

class DCBlocker {
public:
    DCBlocker();
    DCBlocker(double initCutoffFreq);
    double process(double input);
    void clear();
    void setCutoffFreq(double newCutoffFreq);
    void setSampleRate(double newSampleRate);
    double getMaxCutoffFreq() const;
    double output;
private:
    double sampleRate;
    double cutoffFreq;
    double maxCutoffFreq;
    double b;
    double z;
};
