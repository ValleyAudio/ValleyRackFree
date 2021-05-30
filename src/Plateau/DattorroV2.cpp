/**
 * @file DattorroV2.cpp
 * @author Dale Johnson
 */

#include "DattorroV2.hpp"
#include <algorithm>
#include <array>

DattorroV2::DattorroV2(double initSampleRate, uint64_t initBlockSize) :
    decaySmoother(3.f, initSampleRate),
    sizeSmoother(3.f, initSampleRate),
    inputLpf(11050.0, initSampleRate),
    inputHpf(10.0)
{
    preDelayTrajectory.fill(1.0);
    sizeTrajectory.fill(1.0);
    inputChainBuffer.fill(1.0);

    double timeScale = initSampleRate / dattoroSampleRate;
    freezeXFadeStepSize = 1.0 / (0.1 / initSampleRate);
    lfoExcursion = timeScale * 16.0;

    auto scaleQuadTapTimes = [](std::array<double, 4>& x, double scaling) {
        for (auto& i : x) {
            i *= scaling;
        }
    };

    auto scaleTripleTapTimes = [](std::array<double, 3>& x, double scaling) {
        for (auto& i : x) {
            i *= scaling;
        }
    };

    leftDelay1Taps = kLeftDelay1Taps;
    leftDelay2Taps = kLeftDelay2Taps;
    rightDelay1Taps = kRightDelay1Taps;
    rightDelay2Taps = kRightDelay2Taps;
    leftApf2Taps = kLeftApf2Taps;
    rightApf2Taps = kRightApf2Taps;

    scaleQuadTapTimes(leftDelay1Taps, timeScale);
    scaleTripleTapTimes(leftDelay2Taps, timeScale);
    scaleQuadTapTimes(rightDelay1Taps, timeScale);
    scaleTripleTapTimes(rightDelay2Taps, timeScale);
    scaleTripleTapTimes(leftApf2Taps, timeScale);
    scaleTripleTapTimes(rightApf2Taps, timeScale);
    scaledLeftApf1Time = kLeftApf1Time * timeScale;
    scaledRightApf1Time = kRightApf1Time * timeScale;

    double maxLeftDelay1Time = *std::max_element(leftDelay1Taps.begin(), leftDelay1Taps.end());
    double maxLeftDelay2Time = *std::max_element(leftDelay2Taps.begin(), leftDelay2Taps.end());
    double maxRightDelay1Time = *std::max_element(rightDelay1Taps.begin(), rightDelay1Taps.end());
    double maxRightDelay2Time = *std::max_element(rightDelay2Taps.begin(), rightDelay2Taps.end());
    double maxLeftApf1Time = scaledLeftApf1Time;
    double maxRightApf1Time = scaledRightApf1Time;
    double maxLeftApf2Time = *std::max_element(leftApf2Taps.begin(), leftApf2Taps.end());
    double maxRightApf2Time = *std::max_element(rightApf2Taps.begin(), rightApf2Taps.end());

    maxLeftDelay1Time *= maxSize;
    maxLeftDelay2Time *= maxSize;
    maxRightDelay1Time *= maxSize;
    maxRightDelay2Time *= maxSize;
    maxLeftApf1Time *= maxSize;
    maxRightApf1Time *= maxSize;
    maxLeftApf2Time *= maxSize;
    maxRightApf2Time *= maxSize;

    maxLeftDelay1Time += lfoExcursion + 1;
    maxLeftDelay2Time += lfoExcursion + 1;
    maxRightDelay1Time += lfoExcursion + 1;
    maxRightDelay2Time += lfoExcursion + 1;
    maxLeftApf1Time += lfoExcursion + 1;
    maxRightApf1Time += lfoExcursion + 1;
    maxLeftApf2Time += lfoExcursion + 1;
    maxRightApf2Time += lfoExcursion + 1;

    // Construct!
    maxBlockSize = initBlockSize;
    uint64_t scaledInApf1Time = static_cast<uint64_t>(std::ceil(kInApf1Time * timeScale));
    uint64_t scaledInApf2Time = static_cast<uint64_t>(std::ceil(kInApf2Time * timeScale));
    uint64_t scaledInApf3Time = static_cast<uint64_t>(std::ceil(kInApf3Time * timeScale));
    uint64_t scaledInApf4Time = static_cast<uint64_t>(std::ceil(kInApf4Time * timeScale));

    preDelay = InterpDelay2<double>(static_cast<uint64_t>(std::ceil(0.5 * initSampleRate + 1.f)));
    inApf1 = AllpassFilter<double>(scaledInApf1Time, scaledInApf1Time, 0.75);
    inApf2 = AllpassFilter<double>(scaledInApf2Time, scaledInApf2Time, 0.625);
    inApf3 = AllpassFilter<double>(scaledInApf3Time, scaledInApf3Time, 0.7);
    inApf4 = AllpassFilter<double>(scaledInApf4Time, scaledInApf4Time, 0.5);

    uint64_t scaledLeftApf1Time = static_cast<uint64_t>(std::ceil(kLeftApf1Time * timeScale));
    uint64_t scaledRightApf1Time = static_cast<uint64_t>(std::ceil(kRightApf1Time * timeScale));
    leftApf1 = AllpassFilter<double>(maxLeftApf1Time, scaledLeftApf1Time, -plateDiffusion1);
    leftApf2 = MultiTapAllpassFilter<double, 3>(static_cast<uint64_t>(maxLeftApf2Time), leftApf2Taps, plateDiffusion2);
    leftDelay1 = MultiTapInterpDelay<double, 4>(static_cast<uint64_t>(maxLeftDelay1Time), leftDelay1Taps);
    leftDelay2 = MultiTapInterpDelay<double, 3>(static_cast<uint64_t>(maxLeftDelay2Time), leftDelay2Taps);
    rightApf1 = AllpassFilter<double>(maxRightApf1Time, scaledRightApf1Time, -plateDiffusion1);
    rightApf2 = MultiTapAllpassFilter<double, 3>(static_cast<uint64_t>(maxRightApf2Time), rightApf2Taps, plateDiffusion2);
    rightDelay1 = MultiTapInterpDelay<double, 4>(static_cast<uint64_t>(maxRightDelay1Time), rightDelay1Taps);
    rightDelay2 = MultiTapInterpDelay<double, 3>(static_cast<uint64_t>(maxRightDelay2Time), rightDelay2Taps);

    const double kLeftApf2LfoPhase = 0.25;
    const double kRightApf1LfoPhase = 0.5;
    const double kRightApf2LfoPhase = 0.75;

    const double allLfoRevPoint = 0.5;

    leftApf1Lfo = TriSawLFO(initSampleRate, kLeftApf1LfoFreq);
    leftApf2Lfo = TriSawLFO(initSampleRate, kLeftApf2LfoFreq);
    rightApf1Lfo = TriSawLFO(initSampleRate, kRightApf1LfoFreq);
    rightApf2Lfo = TriSawLFO(initSampleRate, kRightApf2LfoFreq);

    leftApf1Lfo.setRevPoint(allLfoRevPoint);
    leftApf2Lfo.setRevPoint(allLfoRevPoint);
    rightApf1Lfo.setRevPoint(allLfoRevPoint);
    rightApf2Lfo.setRevPoint(allLfoRevPoint);

    leftApf2Lfo.phase = kLeftApf2LfoPhase;
    rightApf1Lfo.phase = kRightApf1LfoPhase;
    rightApf2Lfo.phase = kRightApf2LfoPhase;
}

void DattorroV2::blockProcess(const double* leftInBuffer, const double* rightInBuffer,
                              double* leftOutBuffer, double* rightOutBuffer, uint64_t blockSize) {
    leftApf1.gain = -plateDiffusion1;
    leftApf2.gain = plateDiffusion2;
    rightApf1.gain = -plateDiffusion1;
    rightApf2.gain = plateDiffusion2;

    for (uint64_t i = 0; i < blockSize; ++i) {
        preDelay.setDelayTime(preDelayTrajectory[i]);
        preDelay.input = leftInBuffer[i] + rightInBuffer[i];
        preDelay.process();
        inputChainBuffer[i] = preDelay.output;
    }

    inputLpf.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    inputHpf.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    if (diffuseInput) {
        inApf1.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
        inApf2.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
        inApf3.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
        inApf4.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    }

    // Reverb tank
    for (uint64_t i = 0; i < blockSize; ++i) {
        double newDelayTime;
        tankLeftSum += inputChainBuffer[i];
        tankRightSum += inputChainBuffer[i];

        leftApf1Lfo.process();
        leftApf2Lfo.process();
        newDelayTime = scaledLeftApf1Time * sizeTrajectory[i] + leftApf1Lfo.getOutput() * lfoExcursion * modDepth;
        leftApf1.delay.setDelayTime(newDelayTime);
        newDelayTime = leftDelay1Taps[0] * sizeTrajectory[i];
        leftDelay1.setDelayTime(0, newDelayTime);
        newDelayTime = leftApf2Taps[0] * sizeTrajectory[i] + leftApf2Lfo.getOutput() * lfoExcursion * modDepth;
        leftApf2.delay.setDelayTime(0, newDelayTime);
        newDelayTime = leftDelay2Taps[0] * sizeTrajectory[i];
        leftDelay2.setDelayTime(0, newDelayTime);

        rightApf1Lfo.process();
        rightApf2Lfo.process();
        newDelayTime = scaledRightApf1Time * sizeTrajectory[i] + rightApf1Lfo.getOutput() * lfoExcursion * modDepth;
        rightApf1.delay.setDelayTime(newDelayTime);
        newDelayTime = rightDelay1Taps[0] * sizeTrajectory[i];
        rightDelay1.setDelayTime(0, newDelayTime);
        newDelayTime = rightApf2Taps[0] * sizeTrajectory[i] + rightApf2Lfo.getOutput() * lfoExcursion * modDepth;
        rightApf2.delay.setDelayTime(0, newDelayTime);
        newDelayTime = rightDelay2Taps[0] * sizeTrajectory[i];
        rightDelay2.setDelayTime(0, newDelayTime);

        // 'Tick' the left arm of the tank
        leftApf1.input = tankLeftSum;
        leftDelay1.input = leftApf1.process();
        leftDelay1.process();
        leftHpf.input = leftDelay1.output[0];
        leftHpf.process();
        leftLpf.input = leftHpf.output;
        leftApf2.input = leftLpf.process() * (1.0 - freezeXFade) + leftDelay1.output[0] * freezeXFade;
        leftDelay2.input = leftApf2.process();
        leftDelay2.process();

        // 'Tick' the right arm of the tank
        rightApf1.input = tankRightSum;
        rightDelay1.input = rightApf1.process();
        rightDelay1.process();
        rightHpf.input = rightDelay1.output[0];
        rightHpf.process();
        rightLpf.input = rightHpf.output;
        rightApf2.input = rightLpf.process() * (1.0 - freezeXFade) + rightDelay1.output[0] * freezeXFade;
        rightDelay2.input = rightApf2.process();
        rightDelay2.process();

        // Feedback to the beginning of the tank arms
        tankRightSum = leftDelay2.output[0] * decay;
        tankLeftSum = rightDelay2.output[0] * decay;

        // Sum outputs from pre-processed taps in each arm
        double leftOutputSum = leftApf1.output;
        leftOutputSum -= leftApf2.delay.output[1];
        leftOutputSum += leftDelay1.output[1];
        leftOutputSum += leftDelay1.output[2];
        leftOutputSum += leftDelay2.output[1];
        leftOutputSum -= rightApf2.delay.output[1];
        leftOutputSum -= rightDelay1.output[1];
        leftOutputSum -= rightDelay2.output[1];

        double rightOutputSum = rightApf1.output;
        rightOutputSum -= rightApf2.delay.output[2];
        rightOutputSum += rightDelay1.output[2];
        rightOutputSum += rightDelay1.output[3];
        rightOutputSum += rightDelay2.output[2];
        rightOutputSum -= leftApf2.delay.output[2];
        rightOutputSum -= leftDelay1.output[3];
        rightOutputSum -= leftDelay2.output[2];

        leftOutBuffer[i] = leftOutputSum;
        rightOutBuffer[i] = rightOutputSum;

        freezeXFade += freezeXFadeStepSize * freezeXFadeDir;
        freezeXFade = freezeXFade > 1.0 ? 1.0 : (freezeXFade < 0.0 ? 0.0 : freezeXFade);
    }
}

void DattorroV2::clear() {
    inputHpf.clear();
    inputLpf.clear();
    inApf1.clear();
    inApf2.clear();
    inApf3.clear();
    inApf4.clear();

    leftApf1.clear();
    leftDelay1.clear();
    leftHpf.clear();
    leftLpf.clear();
    leftApf2.clear();
    leftDelay2.clear();

    rightApf1.clear();
    rightDelay1.clear();
    rightHpf.clear();
    rightLpf.clear();
    rightApf2.clear();
    rightDelay2.clear();

    tankLeftSum = 0.0;
    tankRightSum = 0.0;

    std::fill(inputChainBuffer.begin(), inputChainBuffer.end(), 0.0);
}

void DattorroV2::freeze(bool freezing) {
    if (freezing && !frozen) {
        thawedDecay = decay;
        decay = 1.0;
        freezeXFadeDir = 1.0;
        frozen = true;
    }

    if (!freezing && frozen) {
        decay = thawedDecay;
        freezeXFadeDir = -1.0;
        frozen = false;
    }
}

void DattorroV2::setDecay(double newDecay) {
    if (!frozen) {
        decay = newDecay < 0.0 ? 0.0 : (newDecay > 1.0 ? 1.0 : newDecay);
    }
}

void DattorroV2::setSize(double newSize) {
    size = newSize < minSize ? minSize : (newSize > maxSize ? maxSize : newSize);
    sizeSmoother.input = size;
}

void DattorroV2::setSizeTrajectory(const std::array<double, DattorroV2_MaxBlockSize>& newSizeTrajectory) {
    sizeTrajectory = newSizeTrajectory;
}

void DattorroV2::setPreDelayTrajectory(const std::array<double, DattorroV2_MaxBlockSize>& newPreDelayTrajectory) {
    preDelayTrajectory = newPreDelayTrajectory;
}

void DattorroV2::setAbsorption(double inputLow, double inputHigh,
                               double tankLow, double tankHigh) {
    inputLpf.setCutoffFreq(inputHigh);
    inputHpf.setCutoffFreq(inputLow);
    leftLpf.setCutoffFreq(tankHigh);
    leftHpf.setCutoffFreq(tankLow);
    rightLpf.setCutoffFreq(tankHigh);
    rightHpf.setCutoffFreq(tankLow);
}

void DattorroV2::setModulation(double rate, double shape, double depth) {
    leftApf1Lfo.setFrequency(rate * kLeftApf1LfoFreq);
    leftApf2Lfo.setFrequency(rate * kLeftApf2LfoFreq);
    rightApf1Lfo.setFrequency(rate * kRightApf1LfoFreq);
    rightApf2Lfo.setFrequency(rate * kRightApf2LfoFreq);
    leftApf1Lfo.setRevPoint(shape);
    leftApf2Lfo.setRevPoint(shape);
    rightApf1Lfo.setRevPoint(shape);
    rightApf2Lfo.setRevPoint(shape);

    modDepth = depth;
}
