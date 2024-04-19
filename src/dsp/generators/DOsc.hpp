#pragma once
#include <iostream>
#include <cmath>
#include "../filters/OnePoleFilters.hpp"

class DOsc {
public:
    float saw, pulse, pulsePhase;
    float subSaw, subPulse;
    float pwm;

    enum Harmonic {
        SUB_2_OCTAVE_HARMONIC = 0,
        SUB_OCTAVE_HARMONIC,
        SUB_FIFTH_HARMONIC,
        ZEROETH_HARMONIC,
        FIFTH_HARMONIC,
        OCTAVE_HARMONIC,
        DOUBLE_OCTAVE_HARMONIC,
        NUM_HARMONICS
    };

    DOsc() {
        sampleRate = 44100.f;
        step = 0.f;
        setFrequency(100.f);
        setSubOctave(0);
        subOffsetLevel = 0;
        subOffsetDegree = 0;
        subWidth = 0.5f;
        saw = 0.f;
        subSaw = 0.f;
        pwm = 0.5f;
    }

    inline void process() {
        //// Fundamental Wave
        saw = (step * 2.f - 1.f) - PolyBLEP(step, stepSize);
        pulse = step + pwm;
        xInt = (int)pulse;
        pulse -= (float)xInt;
        pulse = 1.f - pulse;
        pulse = saw + (pulse * 2.f - 1.f) - PolyBLEP(pulse, stepSize);
        pulse = pulse + (pwm - 0.5f) * 2.f;
        saw = sawHPF.process(saw);
        pulse = pulseHPF.process(pulse);

        //// Derive Sub Wave
        sub1Step = (step * subScale + (float)subOffsetLevel * subOffsetDegree);// + 0.75f;
        xInt = (int)sub1Step;
        sub1Step -= (float)xInt;
        subSaw = (sub1Step * 2.f - 1.f) - PolyBLEP(sub1Step, subStepSize);

        subPulse = sub1Step + subWidth;
        xInt = (int)subPulse;
        subPulse -= (float)xInt;
        subPulse = 1.f - subPulse;
        subPulse = subSaw + (subPulse * 2.f - 1.f) - PolyBLEP(subPulse, subStepSize);
        subPulse = subPulse + (subWidth - 0.5f) * 2.f;
        subPulse = subHPF.process(subPulse);

        // Increment phasor
        step += stepSize;
        if(step > 1.f) {
            step -= 1.f;
            subOffsetLevel++;
            subOffsetLevel -= (subOffsetLevel >= subLimit) ? subLimit : 0;
        }
    }

    inline void setFrequency(float f) {
        freq = f;
        _calcStepSize();
    }

    void setSampleRate(float newSampleRate) {
        sampleRate = newSampleRate;
        _calcStepSize();
        sawHPF.setSampleRate(sampleRate);
        pulseHPF.setSampleRate(sampleRate);
        subHPF.setSampleRate(sampleRate);
    }

    void setSubWave(int subWave) {
        switch(subWave) {
            case 0:
                subWidth = 0.f;
                break;
            case 1:
                subWidth = 0.5;
                break;
            case 2:
                subWidth = 0.75f;
                break;
            default:
                subWidth = 0.f;
                break;
        }
    }

    void setSubOctave(int octave) {
        switch(octave) {
            case SUB_2_OCTAVE_HARMONIC:
                subScale = 0.25f;
                subLimit = 4;
                subOffsetDegree = 0.25f;
                break;
            case SUB_OCTAVE_HARMONIC:
                subScale = 0.5f;
                subLimit = 2;
                subOffsetDegree = 0.5f;
                break;
            case SUB_FIFTH_HARMONIC:
                subScale = 0.75f;
                subLimit = 4;
                subOffsetDegree = 0.75;
                break;
            case ZEROETH_HARMONIC:
                subScale = 1.f;
                subLimit = 1;
                subOffsetDegree = 0.f;
                break;
            /*case THIRD_HARMONIC:
                subScale = 1.333333f;
                subWidth = 0.5f;
                subLimit = 3;
                subOffsetDegree = 0.333333f;
                break;*/
            case FIFTH_HARMONIC:
                subScale = 1.5f;
                subLimit = 2;
                subOffsetDegree = 0.5f;
                break;
            case OCTAVE_HARMONIC:
                subScale = 2.f;
                subLimit = 1;
                subOffsetDegree = 0.f;
                break;
            case DOUBLE_OCTAVE_HARMONIC:
                subScale = 4.f;
                subLimit = 1;
                subOffsetDegree = 0.f;
                break;
            default:
                subScale = 0.5f;
                subLimit = 2;
        }
        subStepSize = stepSize * subScale;
    }
private:

    float freq, sampleRate;

    float stepSize, subStepSize, subWidth;
    float step, sub1Step;
    float subScale, subOffsetDegree;
    int subOffsetLevel, subLimit;
    int xInt;
    DCBlocker sawHPF, pulseHPF, subHPF;
    void _calcStepSize() {
        stepSize = freq / sampleRate;
        stepSize = stepSize > 0.5f ? 0.5f : stepSize;
        subStepSize = stepSize * subScale;
    }

    inline float PolyBLEP(float t, float dt) {
        if(t < dt) {
            t /= dt;
            return t + t - t * t - 1.f;
        }
        else if(t > (1.f - dt)) {
            t = (t - 1.f) / dt;
            return t * t + t + t + 1.f;
        }
        return 0.f;
    }
};
