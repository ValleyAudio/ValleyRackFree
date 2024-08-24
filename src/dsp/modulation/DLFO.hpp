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
    float out[7];

    DLFO() {
        _step = 0.f;
        setSampleRate(44100.f);
        setFrequency(0.75f);
        _syncHigh = false;
        _triggerHigh = false;
        _shTriggered = false;
        out[SH_WAVE] = _noise.getValue();
    }

    inline void process() {
        out[TRI_WAVE] = (_step < 0.5f) ? _step : (1.f - _step);
        out[TRI_WAVE] = -(out[TRI_WAVE] * 4.f - 1.f);

        //_x = out[SAW_UP_WAVE] * M_PI;
        _x = out[TRI_WAVE] * 0.5f * M_PI;
        _xx = _x * _x;
        if(_x < 0) {
            out[SINE_WAVE] = a * _x + b * _xx;
        }
        else {
            out[SINE_WAVE] = a * _x - b * _xx;
        }

        out[SAW_UP_WAVE] = _step - 0.25f;
        if(out[SAW_UP_WAVE] < 0.f) {
            out[SAW_UP_WAVE] += 1.f;
        }

        out[SAW_UP_WAVE] = out[SAW_UP_WAVE] * 2.f - 1.f;
        out[SAW_DOWN_WAVE] = 1.f - out[SAW_UP_WAVE] - 1.f;

        out[SQUARE_WAVE] = (out[SAW_UP_WAVE] > 0.f) ? 1.f : -1.f;
        out[NOISE_WAVE] = _noise.process();
        if(out[SQUARE_WAVE] > 0.5f && _shTriggered == false) {
            _shTriggered = true;
            out[SH_WAVE] = _noise.getValue();
        }
        else if(out[SQUARE_WAVE] < 0.5f) {
            _shTriggered = false;
        }


        _step += _stepSize;
        _step -= (_step > 1.f) ? 1.f : 0.f;
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
        if(syncSignal > 0.1f && !_syncHigh) {
            _syncHigh = true;
            _step = 0.f;
        }
        if(syncSignal <= 0.1f && _syncHigh) {
            _syncHigh = false;
        }
    }

    inline void trigger(float triggerSignal) {
        if(triggerSignal > 0.1f && !_triggerHigh) {
            _triggerHigh = true;
            out[SH_WAVE] = _noise.getValue();
        }
        if(triggerSignal <= 0.1f && _triggerHigh) {
            _triggerHigh = false;
        }
    }
private:
    static constexpr float a = 1.27323954f;
    static constexpr float b = 0.405284735;
    float frequency, sampleRate, sampleTime;
    float _step, _stepSize;
    bool _syncHigh, _triggerHigh;
    float _x, _xx;
    WhiteNoise _noise;
    bool _shTriggered;

    void calcStepSize() {
        _stepSize = frequency * sampleTime;
    }
};
