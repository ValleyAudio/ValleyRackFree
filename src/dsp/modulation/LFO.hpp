#ifndef DSJ_LFO_HPP
#define DSJ_LFO_HPP
#include <vector>
#include <cmath>
#include <cstdint>

class LFO {
public:
    double output = 0.0;
    double phase = 0.0;

    LFO() {
        frequency = 1.0;
        sampleRate = 44100.0;
        stepSize = frequency * (double)kTableLength / sampleRate;
        for(auto i = 0; i < kTableLength; ++i) {
            sineTable.push_back(sin(2.0 * M_PI * (double)i / (double)kTableLength));
        }
        phasor = 0.0;
    }

    double process() {
        plusPhase = phasor + phase * kTableLength;
        if(plusPhase < 0.0) {
            plusPhase += kTableLength;
        }
        else if(plusPhase >= kTableLength) {
            plusPhase -= kTableLength;
        }

        a = (long)plusPhase;
        frac = plusPhase - a;
        b = a + 1.0;
        b %= kTableLength;
        output = sineTable[a] * (1.0 - frac) + sineTable[b] * frac;

        phasor += stepSize;
        if(phasor >= kTableLength) {
            phasor -= kTableLength;
        }
        return output;
    }

    void setFrequency(double newFrequency) {
        frequency = newFrequency;
        calcStepSize();
    }
    void setSamplerate(double newSampleRate) {
        sampleRate = newSampleRate;
        calcStepSize();
    }
private:
    double frequency;
    double sampleRate;
    double stepSize;
    double phasor;
    double plusPhase;
    long a, b;
    double frac;
    const long kTableLength = 4096;
    std::vector<double> sineTable;

    void calcStepSize() {
        stepSize = frequency * (double)kTableLength / sampleRate;
    }
};

class TriSawLFO {
public:
    TriSawLFO(double sampleRate = 44100.0, double frequency = 1.0) {
        phase = 0.0;
        output = 0.0;
        this->sampleRate = sampleRate;
        step = 0.0;
        rising = true;
        setFrequency(frequency);
        setRevPoint(0.5);
    }

    double process() {
        if(step > 1.0) {
            step -= 1.0;
            rising = true;
        }

        if(step >= revPoint) {
            rising = false;
        }

        if(rising) {
            output = step * riseRate;
        }
        else {
            output = step * fallRate - fallRate;
        }

        step += stepSize;
        output *= 2.0;
        output -= 1.0;
        return output;
    }

    void setFrequency(double newFrequency) {
        if (newFrequency == frequency) {
            return;
        }
        frequency = newFrequency;
        calcStepSize();
    }

    void setRevPoint(double newRevPoint) {
        revPoint = newRevPoint;
        if(revPoint < 0.0001) {
            revPoint = 0.0001;
        }
        if(revPoint > 0.999) {
            revPoint = 0.999;
        }

        riseRate = 1.0 / revPoint;
        fallRate = -1.0 / (1.0 - revPoint);
    }

    void setSamplerate(double newSampleRate) {
        sampleRate = newSampleRate;
        calcStepSize();
    }

    double getOutput() const {
        return output;
    }

    double phase;

private:
    double output;
    double sampleRate;
    double frequency = 0.0;
    double revPoint;
    double riseRate;
    double fallRate;
    double step;
    double stepSize;
    bool rising;

    void calcStepSize() {
        stepSize = frequency / sampleRate;
    }
};

#endif
