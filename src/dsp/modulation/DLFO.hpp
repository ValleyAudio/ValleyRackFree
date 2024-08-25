#pragma once
#include <cmath>
#include "../generators/Noise.hpp"

class DLFO {
public:
    enum Waves {
        SINE_WAVE = 0,
        TRI_WAVE,
        SAW_UP_WAVE,
        SAW_DOWN_WAVE,
        SQUARE_WAVE,
        SH_WAVE,
        NOISE_WAVE,
        NUM_WAVES
    };

    float out[NUM_WAVES];

    DLFO() {
        step = 0.f;
        setSampleRate(44100.f);
        setFrequency(0.75f);
        isSyncing = false;
        isTriggered = false;
        shIsTriggered = false;
        out[SH_WAVE] = whiteNoiseGenerator.getValue();
    }

    inline void process() {
        out[TRI_WAVE] = (step < 0.5f) ? step : (1.f - step);
        out[TRI_WAVE] = -(out[TRI_WAVE] * 4.f - 1.f);

        //x = out[SAW_UP_WAVE] * M_PI;
        x = out[TRI_WAVE] * 0.5f * M_PI;
        auto xx = x * x;
        if(x < 0) {
            out[SINE_WAVE] = a * x + b * xx;
        }
        else {
            out[SINE_WAVE] = a * x - b * xx;
        }

        out[SAW_UP_WAVE] = step - 0.25f;
        if(out[SAW_UP_WAVE] < 0.f) {
            out[SAW_UP_WAVE] += 1.f;
        }

        out[SAW_UP_WAVE] = out[SAW_UP_WAVE] * 2.f - 1.f;
        out[SAW_DOWN_WAVE] = 1.f - out[SAW_UP_WAVE] - 1.f;

        out[SQUARE_WAVE] = (out[SAW_UP_WAVE] > 0.f) ? 1.f : -1.f;
        out[NOISE_WAVE] = whiteNoiseGenerator.process();
        if(out[SQUARE_WAVE] > 0.5f && shIsTriggered == false) {
            shIsTriggered = true;
            out[SH_WAVE] = whiteNoiseGenerator.getValue();
        }
        else if(out[SQUARE_WAVE] < 0.5f) {
            shIsTriggered = false;
        }


        step += stepSize;
        step -= (step > 1.f) ? 1.f : 0.f;
    }

    inline void setFrequency(float freq) {
        frequency = freq;
        calcStepSize();
    }

    inline void setSampleRate(float newSampleRate) {
        sampleRate = newSampleRate;
        sampleTime = 1.f / sampleRate;
        calcStepSize();
    }

    inline void sync(float syncSignal) {
        if(syncSignal > 0.1f && !isSyncing) {
            isSyncing = true;
            step = 0.f;
        }
        if(syncSignal <= 0.1f && isSyncing) {
            isSyncing = false;
        }
    }

    inline void trigger(float triggerSignal) {
        if(triggerSignal > 0.1f && !isTriggered) {
            isTriggered = true;
            out[SH_WAVE] = whiteNoiseGenerator.getValue();
        }
        if(triggerSignal <= 0.1f && isTriggered) {
            isTriggered = false;
        }
    }
private:
    static constexpr float a = 1.27323954f;
    static constexpr float b = 0.405284735;
    float frequency, sampleRate, sampleTime;
    float step, stepSize;
    bool isSyncing, isTriggered;
    float x;
    WhiteNoise whiteNoiseGenerator;
    bool shIsTriggered;

    void calcStepSize() {
        stepSize = frequency * sampleTime;
    }
};
