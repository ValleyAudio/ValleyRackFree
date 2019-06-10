#pragma once

class DEnv {
public:
    DEnv() {
        _gated = false;
        _triggered = false;
        _idling = true;
        _attacking = false;
        _decaying = false;
        _sustaining = false;
        _releasing = false;
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
        /*if(gate >= 0.5f) {
            if(trig >= 0.5f && _prevTrigState < 0.5f && !_triggered) {
                _triggered = true;
            }
            _prevTrigState = trig;
            if(_idling || _releasing || _triggered) {
                _triggered = false;
                _attacking = true;
                _idling = false;
                _decaying = false;
                _sustaining = false;
                _releasing = false;
            }
        }
        else if(gate < 0.5f) {
            if(!_idling) {
                _attacking = false;
                _decaying = false;
                _sustaining = false;
                _releasing = true;
            }
        }*/

        if(trig >= 0.1f && _prevTrigState < 0.1f && !_triggered) {
            _triggered = true;
        }
        _prevTrigState = trig;

        _prevGateState = _gated;
        _gated = gate >= 0.1f ? true : false;

        if(_gated || _triggered) {
            if(_idling || _releasing) {
                _triggered = false;
                _attacking = true;
                _idling = false;
                _decaying = false;
                _sustaining = false;
                _releasing = false;
            }
        }

        if(!_gated && _prevGateState) {
            if(!_idling) {
                _attacking = false;
                _decaying = false;
                _sustaining = false;
                _releasing = true;
            }
        }

        // The envelope
        if(_idling) {
            value = 0.f;
        }
        if(_attacking) {
            value += powf(base, 1 - attackTime) / maxTime * (1.01f - value) * _sampleTime * timeScale;
            if(value > (1.f - 1e-4)) {
                value = 1.f;
                _attacking = false;
                if(_gated) {
                    _decaying = true;
                }
                else {
                    _releasing = true;
                }
            }
        }
        if(_decaying) {
            value += powf(base, 1 - decayTime) / maxTime * (sustain - value) * _sampleTime * timeScale;
            if(value < sustain + 1e-4) {
                value = sustain;
                _decaying = false;
                _sustaining = true;
            }
        }
        if(loop) {
            if(_sustaining || _releasing) {
                value = 0.f;
                _attacking = true;
                _sustaining = false;
                _releasing = false;
            }
        }

        if(_sustaining) {
            value = sustain;
        }
        if(_releasing) {
            value += powf(base, 1 - releaseTime) / maxTime * (0.0f - value) * _sampleTime * timeScale;
            if(value < 1e-4) {
                value = 0.f;
                _releasing = false;
                _idling = true;
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
    bool _idling, _attacking, _decaying, _sustaining, _releasing;
    bool _gated, _triggered;

};
