/**
 * @filename Noise.hpp
 * @author Dale Johnson
 *
 * Copyright (c) 2020 Dale Johnson, Valley Audio
 * Licensed under GPLv3
 */

#pragma once
#include <random>
#include <cmath>
#include <ctime>

class WhiteNoise {
public:
#ifdef ARCH_WIN
    WhiteNoise() :rand(time(0)),
                  uniform(-1.0, 1.0) {
        value = 0.f;
    }
#else
    WhiteNoise() : rand(seed()),
              uniform(-1.0, 1.0) {
        value = 0.f;
    }
#endif

    inline float process() {
        value = uniform(rand);
        return value;
    }

    float getValue() const {
        return value;
    }
private:
    std::random_device seed;
    std::minstd_rand rand;
    std::uniform_real_distribution<float> uniform;
    float value;
};

class PinkNoise {
public:
    PinkNoise() {
        setSampleRate(44100.f);
    }

    inline double process() {
        white = whiteGen.process();
        b[0] = b[0] + (a[0] * ((white * 48.69991228070175) - b[0]));
        b[1] = b[1] + (a[1] * ((white * 11.23890718562874) - b[1]));
        b[2] = b[2] + (a[2] * ((white * 4.96296774193548) - b[2]));
        b[3] = b[3] + (a[3] * ((white * 2.32573483146067) - b[3]));
        b[4] = b[4] + (a[4] * ((white * 1.18433822222222) - b[4]));
        b[5] = -0.7616 * b[5] - white * 0.0168980;
        pink = (b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + white * 0.5362);
        b[6] = white * 0.115926;
        pink *= 0.15;
        return pink;
    }

    void setSampleRate(double newSampleRate) {
        sampleRate = newSampleRate;
        for(auto& i : b) {
            i = 0.0;
        }
        white = 0.0;
        pink = 0.0;
        calcAValues();
    }

    double getValue() const {
        return pink;
    }

private:
    WhiteNoise whiteGen;

    double b[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double a[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double alpha[5] = {M_PI * 2.0 * 8.00135734209627,
                       M_PI * 2.0 * 46.88548507044182,
                       M_PI * 2.0 * 217.61558695916962,
                       M_PI * 2.0 * 939.80665948455472,
                       M_PI * 2.0 * 3276.10128392439381};
    double sampleRate = 0.0;
    double white = 0.0;
    double pink = 0.0;

    void calcAValues() {
        for(auto i = 0; i < 5; ++i) {
            a[i] = sin(alpha[i] / sampleRate);
        }
    }
};
