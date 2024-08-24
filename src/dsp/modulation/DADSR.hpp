#pragma once
#include <cmath>

class DEnv {
public:
    enum class State {
        Idling,
        Attacking,
        Decaying,
        Sustaining,
        Releasing
    };

    DEnv() {
        gated = false;
        isTriggered = false;
        loop = false;
        value = 0.f;

        attackTime = 0.f;
        decayTime = 0.f;
        sustain = 1.f;
        releaseTime = 0.f;

        base = 20000.f;
        maxTime = 10.f;
        timeScale = 1.f;
        _prevGateState = false;
        _prevTrigState = 0.f;
        setSampleRate(44100.f);
    }
    void process(float gate, float trig) {
        if(trig >= 0.1f && _prevTrigState < 0.1f && !isTriggered) {
            isTriggered = true;
        }
        _prevTrigState = trig;

        _prevGateState = gated;
        gated = gate >= 0.1f ? true : false;

        if(gated || isTriggered) {
            if(state == State::Idling ||
               state == State::Releasing) {
                isTriggered = false;
                state = State::Attacking;
            }
        }

        if(!gated && _prevGateState) {
            if(state != State::Idling) {
                state = State::Releasing;
            }
        }

        // The envelope
        if(state == State::Idling) {
            value = 0.f;
        }

        if(state == State::Attacking) {
            value += std::powf(base, 1 - attackTime) / maxTime * (1.01f - value) * _sampleTime * timeScale;
            if(value > (1.f - 1e-4)) {
                value = 1.f;
                if(gated) {
                    state = State::Decaying;
                }
                else {
                    state = State::Releasing;
                }
            }
        }

        if(state == State::Decaying) {
            value += std::powf(base, 1 - decayTime) / maxTime * (sustain - value) * _sampleTime * timeScale;
            if(value < sustain + 1e-4) {
                value = sustain;
                state = State::Sustaining;
            }
        }

        if(loop) {
            if(state == State::Sustaining ||
               state == State::Releasing) {
                value = 0.f;
                state = State::Attacking;
            }
        }

        if(state == State::Sustaining) {
            value = sustain;
        }

        if(state == State::Releasing) {
            value += std::powf(base, 1 - releaseTime) / maxTime * (0.0f - value) * _sampleTime * timeScale;
            if(value < 1e-4) {
                value = 0.f;
                state = State::Idling;
            }
        }
    }

    void setSampleRate(float sampleRate) {
        _sampleRate = sampleRate;
        _sampleTime = 1.f / _sampleRate;
    }

    float attackTime;
    float decayTime;
    float sustain;
    float releaseTime;
    float value;
    float timeScale;
    bool loop;
private:
    float _sampleRate, _sampleTime;
    float base, maxTime;
    bool _prevGateState;
    float _prevTrigState;
    State state = State::Idling;
    bool _idling, _attacking, _decaying, _sustaining, _releasing;
    bool gated, isTriggered;

};
