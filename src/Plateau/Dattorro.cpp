#include "Dattorro.hpp"

Dattorro::Dattorro() {
    _dattorroScaleFactor = _sampleRate / _dattorroSampleRate;

    _preDelay.delayTime = 0;
    _earlyRefs = MultiTapInterpDelay<float, 8>(44100);
    _earlyRefs.setDelaySamples(0, _sampleRate * 0.0199);
    _earlyRefs.setDelaySamples(1, _sampleRate * 0.0219);
    _earlyRefs.setDelaySamples(2, _sampleRate * 0.0354);
    _earlyRefs.setDelaySamples(3, _sampleRate * 0.0389);
    _earlyRefs.setDelaySamples(4, _sampleRate * 0.0414);
    _earlyRefs.setDelaySamples(5, _sampleRate * 0.0692);
    _earlyRefs.setDelaySamples(6, _sampleRate * 0.0717);
    _earlyRefs.setDelaySamples(7, _sampleRate * 0.0885);

    _inputLpf = OnePoleLPFilter(22000.0);
    _inputHpf = OnePoleHPFilter(0.0);

    _inApf1 = AllpassFilter(dattorroScale(40 * _kInApf1Time), dattorroScale(_kInApf1Time), inputDiffusion1);
    _inApf2 = AllpassFilter(dattorroScale(40 * _kInApf2Time), dattorroScale(_kInApf2Time), inputDiffusion1);
    _inApf3 = AllpassFilter(dattorroScale(40 * _kInApf3Time), dattorroScale(_kInApf3Time), inputDiffusion2);
    _inApf4 = AllpassFilter(dattorroScale(40 * _kInApf4Time), dattorroScale(_kInApf4Time), inputDiffusion2);

    _leftApf1 = AllpassFilter(dattorroScale(40 * _kLeftApf1Time), dattorroScale(_kLeftApf1Time), -plateDiffusion1);
    _leftDelay1 = InterpDelay<double>(dattorroScale(40 * _kLeftDelay1Time), dattorroScale(_kLeftDelay1Time));
    _leftFilter = OnePoleLPFilter(reverbHighCut);
    _leftHpf = OnePoleHPFilter(reverbLowCut);
    _leftApf2 = AllpassFilter(dattorroScale(40 * _kLeftApf2Time), dattorroScale(_kLeftApf2Time), plateDiffusion2);
    _leftDelay2 = InterpDelay<double>(dattorroScale(40 * _kLeftDelay2Time), dattorroScale(_kLeftDelay2Time));

    _rightApf1 = AllpassFilter(dattorroScale(40 * _kRightApf1Time), dattorroScale(_kRightApf1Time), -plateDiffusion1);
    _rightDelay1 = InterpDelay<double>(dattorroScale(40 * _kRightDelay1Time), dattorroScale(_kRightDelay1Time));
    _rightFilter = OnePoleLPFilter(reverbHighCut);
    _rightHpf = OnePoleHPFilter(reverbLowCut);
    _rightApf2 = AllpassFilter(dattorroScale(40 * _kRightApf2Time), dattorroScale(_kRightApf2Time), plateDiffusion2);
    _rightDelay2 = InterpDelay<double>(dattorroScale(40 * _kRightDelay2Time), dattorroScale(_kRightDelay2Time));

    _leftApf1Time = dattorroScale(_kLeftApf1Time);
    _leftApf2Time = dattorroScale(_kLeftApf2Time);
    _rightApf1Time = dattorroScale(_kRightApf1Time);
    _rightApf2Time = dattorroScale(_kRightApf2Time);

    for(auto i = 0; i < 7; ++i) {
        _scaledLeftTaps[i] = dattorroScale(_kLeftTaps[i]);
        _scaledRightTaps[i] = dattorroScale(_kRightTaps[i]);
    }

    _leftInputDCBlock.setCutoffFreq(20.0);
    _rightInputDCBlock.setCutoffFreq(20.0);
    _leftOutDCBlock.setCutoffFreq(20.0);
    _rightOutDCBlock.setCutoffFreq(20.0);

    _lfo1.setFrequency(_lfo1Freq);
    _lfo2.setFrequency(_lfo2Freq);
    _lfo3.setFrequency(_lfo3Freq);
    _lfo4.setFrequency(_lfo4Freq);

    _lfo2.phase = 0.25;
    _lfo3.phase = 0.5;
    _lfo4.phase = 0.75;

    _lfo1.setRevPoint(0.5);
    _lfo2.setRevPoint(0.5);
    _lfo3.setRevPoint(0.5);
    _lfo4.setRevPoint(0.5);

    _lfoDepth = dattorroScale(_kLfoExcursion);
}

void Dattorro::process(double leftInput, double rightInput) {
    if(!_freeze) {
        _decay = decay;
        _leftFilter.setCutoffFreq(reverbHighCut);
        _leftHpf.setCutoffFreq(reverbLowCut);
        _rightFilter.setCutoffFreq(reverbHighCut);
        _rightHpf.setCutoffFreq(reverbLowCut);
    }

    _lfo1.setFrequency(_lfo1Freq * modSpeed);
    _lfo2.setFrequency(_lfo2Freq * modSpeed);
    _lfo3.setFrequency(_lfo3Freq * modSpeed);
    _lfo4.setFrequency(_lfo4Freq * modSpeed);

    _leftApf1.gain = plateDiffusion1;
    _leftApf2.gain = plateDiffusion2;
    _rightApf1.gain = plateDiffusion1;
    _rightApf2.gain = plateDiffusion2;

    _leftApf1.delay.delayTime = _lfo1.process() * _lfoDepth * modDepth + _leftApf1Time;
    _leftApf2.delay.delayTime = _lfo2.process() * _lfoDepth * modDepth + _leftApf2Time;
    _rightApf1.delay.delayTime = _lfo3.process() * _lfoDepth * modDepth + _rightApf1Time;
    _rightApf2.delay.delayTime = _lfo4.process() * _lfoDepth * modDepth + _rightApf2Time;

    _leftInputDCBlock.input = leftInput;
    _rightInputDCBlock.input = rightInput;
    _preDelay.input = _leftInputDCBlock.process() + _rightInputDCBlock.process();

    //_earlyRefs.process(_preDelay.process());
    _earlyRefSum = _earlyRefs[0] * 0.7 + _earlyRefs[1] * 0.6 + _earlyRefs[2] * 0.4 + _earlyRefs[3] * 0.3;
    _earlyRefSum += _earlyRefs[4] * 0.3 + _earlyRefs[5] * 0.1 + _earlyRefs[6] * 0.1 + _earlyRefs[7] * 0.07;

    _inputLpf.setCutoffFreq(inputHighCut);
    _inputHpf.setCutoffFreq(inputLowCut);
    _inputLpf.input = _preDelay.process();
    _inputHpf.input = _inputLpf.process();
    _inApf1.input = _inputHpf.process();
    _inApf2.input = _inApf1.process();
    _inApf3.input = _inApf2.process();
    _inApf4.input = _inApf3.process();
    _leftSum += _inApf4.process();
    _rightSum += _inApf4.output;

    _leftApf1.input = _leftSum;//+ _inApf4.output * 0.25f;
    _leftDelay1.input = _leftApf1.process();
    _leftFilter.input = _leftDelay1.process();
    _leftHpf.input = _leftFilter.process();
    _leftApf2.input = _leftHpf.process() * _decay;// + _inApf4.output * 0.25f;
    _leftDelay2.input = _leftApf2.process();
    _leftDelay2.process();

    _rightApf1.input = _rightSum;// + _inApf4.output * 0.25f;
    _rightDelay1.input = _rightApf1.process();
    _rightFilter.input = _rightDelay1.process();
    _rightHpf.input = _rightFilter.process();
    _rightApf2.input = _rightHpf.process() * _decay;// + _inApf4.output * 0.25f;
    _rightDelay2.input = _rightApf2.process();
    _rightDelay2.process();

    _rightSum = _leftDelay2.output * _decay;
    _leftSum = _rightDelay2.output * _decay;

    //_leftOutDCBlock.input = _leftApf1.output + _leftFilter.output + _leftApf2.output;
    //_rightOutDCBlock.input = _rightApf1.output + _rightFilter.output + _rightApf2.output;
    _leftOutDCBlock.input = _leftApf1.output;
    _leftOutDCBlock.input += _leftDelay1.tap(_scaledLeftTaps[0]) * 0.6;
    _leftOutDCBlock.input += _leftDelay1.tap(_scaledLeftTaps[1]) * 0.6;
    _leftOutDCBlock.input -= _leftApf2.delay.tap(_scaledLeftTaps[2]) * 0.6;
    _leftOutDCBlock.input += _leftDelay2.tap(_scaledLeftTaps[3]) * 0.6;
    _leftOutDCBlock.input -= _rightDelay1.tap(_scaledLeftTaps[4]) * 0.6;
    _leftOutDCBlock.input -= _rightApf2.delay.tap(_scaledLeftTaps[5]) * 0.6;
    _leftOutDCBlock.input -= _rightDelay2.tap(_scaledLeftTaps[6]) * 0.6;

    _rightOutDCBlock.input = _rightApf1.output;
    _rightOutDCBlock.input += _rightDelay1.tap(_scaledRightTaps[0]) * 0.6;
    _rightOutDCBlock.input += _rightDelay1.tap(_scaledRightTaps[1]) * 0.6;
    _rightOutDCBlock.input -= _rightApf2.delay.tap(_scaledRightTaps[2]) * 0.6;
    _rightOutDCBlock.input += _rightDelay2.tap(_scaledRightTaps[3]) * 0.6;
    _rightOutDCBlock.input -= _leftDelay1.tap(_scaledRightTaps[4]) * 0.6;
    _rightOutDCBlock.input -= _leftApf2.delay.tap(_scaledRightTaps[5]) * 0.6;
    _rightOutDCBlock.input -= _leftDelay2.tap(_scaledRightTaps[6]) * 0.6;
    leftOut = _leftOutDCBlock.process();
    rightOut = _rightOutDCBlock.process();
}

void Dattorro::clear() {
    _preDelay.clear();
    _earlyRefs.clear();
    _inputLpf.clear();
    _inputHpf.clear();
    _inApf1.clear();
    _inApf2.clear();
    _inApf3.clear();
    _inApf4.clear();

    _leftApf1.clear();
    _leftDelay1.clear();
    _leftFilter.clear();
    _leftHpf.clear();
    _leftApf2.clear();
    _leftDelay2.clear();

    _rightApf1.clear();
    _rightDelay1.clear();
    _rightFilter.clear();
    _rightHpf.clear();
    _rightApf2.clear();
    _rightDelay2.clear();

    _leftInputDCBlock.clear();
    _rightInputDCBlock.clear();
    _leftOutDCBlock.clear();
    _rightOutDCBlock.clear();

    _earlyRefSum = 0.0f;
    _leftSum = 0.0f;
    _rightSum = 0.0f;
    leftOut = 0.0f;
    rightOut = 0.0f;
}

void Dattorro::setTimeScale(double timeScale) {
    _timeScale = timeScale;
    if(_timeScale < 0.01) {
        _timeScale = 0.01;
    }

    _leftDelay1.delayTime = dattorroScale(_kLeftDelay1Time * _timeScale);
    _leftDelay2.delayTime = dattorroScale(_kLeftDelay2Time * _timeScale);
    _rightDelay1.delayTime = dattorroScale(_kRightDelay1Time * _timeScale);
    _rightDelay2.delayTime = dattorroScale(_kRightDelay2Time * _timeScale);
    _leftApf1Time = dattorroScale(_kLeftApf1Time * _timeScale);
    _leftApf2Time = dattorroScale(_kLeftApf2Time * _timeScale);
    _rightApf1Time = dattorroScale(_kRightApf1Time * _timeScale);
    _rightApf2Time = dattorroScale(_kRightApf2Time * _timeScale);
}

void Dattorro::setModShape(double shape) {
    _lfo1.setRevPoint(shape);
    _lfo2.setRevPoint(shape);
    _lfo3.setRevPoint(shape);
    _lfo4.setRevPoint(shape);
}

void Dattorro::setSampleRate(double sampleRate) {
    _sampleRate = sampleRate;
    _dattorroScaleFactor = _sampleRate / _dattorroSampleRate;

    _earlyRefs.setDelaySamples(0, _sampleRate * 0.0199);
    _earlyRefs.setDelaySamples(1, _sampleRate * 0.0219);
    _earlyRefs.setDelaySamples(2, _sampleRate * 0.0354);
    _earlyRefs.setDelaySamples(3, _sampleRate * 0.0389);
    _earlyRefs.setDelaySamples(4, _sampleRate * 0.0414);
    _earlyRefs.setDelaySamples(5, _sampleRate * 0.0692);
    _earlyRefs.setDelaySamples(6, _sampleRate * 0.0717);
    _earlyRefs.setDelaySamples(7, _sampleRate * 0.0885);

    _leftDelay1.delayTime = dattorroScale(_kLeftDelay1Time * _timeScale);
    _leftDelay2.delayTime = dattorroScale(_kLeftDelay2Time * _timeScale);
    _rightDelay1.delayTime = dattorroScale(_kRightDelay1Time * _timeScale);
    _rightDelay2.delayTime = dattorroScale(_kRightDelay2Time * _timeScale);
    _leftApf1Time = dattorroScale(_kLeftApf1Time * _timeScale);
    _leftApf2Time = dattorroScale(_kLeftApf2Time * _timeScale);
    _rightApf1Time = dattorroScale(_kRightApf1Time * _timeScale);
    _rightApf2Time = dattorroScale(_kRightApf2Time * _timeScale);

    for(auto i = 0; i < 7; ++i) {
        _scaledLeftTaps[i] = dattorroScale(_kLeftTaps[i]);
        _scaledRightTaps[i] = dattorroScale(_kRightTaps[i]);
    }

    _lfoDepth = dattorroScale(_kLfoExcursion);
    _lfo1.setSamplerate(sampleRate);
    _lfo2.setSamplerate(sampleRate);
    _lfo3.setSamplerate(sampleRate);
    _lfo4.setSamplerate(sampleRate);
    _inputHpf.setSampleRate(sampleRate);
    _inputLpf.setSampleRate(sampleRate);
    _leftFilter.setSampleRate(sampleRate);
    _leftHpf.setSampleRate(sampleRate);
    _rightFilter.setSampleRate(sampleRate);
    _rightHpf.setSampleRate(sampleRate);

    _leftInputDCBlock.setSampleRate(sampleRate);
    _rightInputDCBlock.setSampleRate(sampleRate);
    _leftOutDCBlock.setSampleRate(sampleRate);
    _rightOutDCBlock.setSampleRate(sampleRate);

    clear();
}

void Dattorro::freeze() {
    _freeze = true;
    _decay = 1.0;
    _leftFilter.setCutoffFreq(_leftFilter.getMaxCutoffFreq());
    _leftHpf.setCutoffFreq(0.0);
    _rightFilter.setCutoffFreq(_leftFilter.getMaxCutoffFreq());
    _rightHpf.setCutoffFreq(0.0);
}

void Dattorro::unFreeze() {
    _freeze = false;
    _decay = decay;
    _leftFilter.setCutoffFreq(reverbHighCut);
    _leftHpf.setCutoffFreq(reverbLowCut);
    _rightFilter.setCutoffFreq(reverbHighCut);
    _rightHpf.setCutoffFreq(reverbLowCut);
}

double Dattorro::dattorroScale(double delayTime) {
    return delayTime * _dattorroScaleFactor;
}
