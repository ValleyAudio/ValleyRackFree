/**
 * @file DattorroV2.cpp
 * @author Dale Johnson
 */

#include "DattorroV2.hpp"
#include <algorithm>
#include <array>

DattorroV2::DattorroV2(double initSampleRate, uint64_t initBlockSize) :
    inputLpf(11050.0, initSampleRate),
    inputHpf(10.0),
    inputChainBuffer(initBlockSize, 0.0)
{
    double timeScale = dattoroSampleRate / initSampleRate;

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

    std::array<double, 4> leftDelay1Taps(kLeftDelay1Taps);
    std::array<double, 3> leftDelay2Taps(kLeftDelay2Taps);
    std::array<double, 4> rightDelay1Taps(kRightDelay1Taps);
    std::array<double, 3> rightDelay2Taps(kRightDelay2Taps);
    std::array<double, 3> leftApf2Taps(kLeftApf2Taps);
    std::array<double, 3> rightApf2Taps(kRightApf2Taps);
    scaleQuadTapTimes(leftDelay1Taps, timeScale);
    scaleTripleTapTimes(leftDelay2Taps, timeScale);
    scaleQuadTapTimes(rightDelay1Taps, timeScale);
    scaleTripleTapTimes(rightDelay2Taps, timeScale);
    scaleTripleTapTimes(leftApf2Taps, timeScale);
    scaleTripleTapTimes(rightApf2Taps, timeScale);

    double maxLeftDelay1Time = *std::max_element(leftDelay1Taps.begin(), leftDelay1Taps.end());
    double maxLeftDelay2Time = *std::max_element(leftDelay2Taps.begin(), leftDelay2Taps.end());
    double maxRightDelay1Time = *std::max_element(rightDelay1Taps.begin(), rightDelay1Taps.end());
    double maxRightDelay2Time = *std::max_element(rightDelay2Taps.begin(), rightDelay2Taps.end());
    double maxLeftApf1Time = kLeftApf1Time * timeScale + lfoExcursion + 1;
    double maxRightApf1Time = kRightApf1Time * timeScale + lfoExcursion + 1;
    double maxLeftApf2Time = *std::max_element(leftApf2Taps.begin(), leftApf2Taps.end());
    double maxRightApf2Time = *std::max_element(rightApf2Taps.begin(), rightApf2Taps.end());

    maxLeftDelay1Time += lfoExcursion + 1;
    maxLeftDelay2Time += lfoExcursion + 1;
    maxRightDelay1Time += lfoExcursion + 1;
    maxRightDelay2Time += lfoExcursion + 1;
    maxLeftApf2Time += lfoExcursion + 1;
    maxRightApf2Time += lfoExcursion + 1;

    // Construct!
    blockSize = initBlockSize;
    uint64_t scaledInApf1Time = static_cast<uint64_t>(std::ceil(kInApf1Time * timeScale));
    uint64_t scaledInApf2Time = static_cast<uint64_t>(std::ceil(kInApf2Time * timeScale));
    uint64_t scaledInApf3Time = static_cast<uint64_t>(std::ceil(kInApf3Time * timeScale));
    uint64_t scaledInApf4Time = static_cast<uint64_t>(std::ceil(kInApf4Time * timeScale));

    inApf1 = AllpassFilter<double>(scaledInApf1Time, scaledInApf1Time, 0.75);
    inApf2 = AllpassFilter<double>(scaledInApf2Time, scaledInApf2Time, 0.625);
    inApf3 = AllpassFilter<double>(scaledInApf3Time, scaledInApf3Time, 0.7);
    inApf4 = AllpassFilter<double>(scaledInApf4Time, scaledInApf4Time, 0.5);

    uint64_t scaledLeftApf1Time = static_cast<uint64_t>(std::ceil(kLeftApf1Time * timeScale));
    uint64_t scaledRightApf1Time = static_cast<uint64_t>(std::ceil(kRightApf1Time * timeScale));
    leftApf1 = AllpassFilter<double>(maxLeftApf1Time, scaledLeftApf1Time, 0.7071);
    leftApf2 = MultiTapAllpassFilter<double, 3>(static_cast<uint64_t>(maxLeftApf2Time), leftApf2Taps, 0.5);
    leftDelay1 = MultiTapInterpDelay<double, 4>(static_cast<uint64_t>(maxLeftDelay1Time), leftDelay1Taps);
    leftDelay2 = MultiTapInterpDelay<double, 3>(static_cast<uint64_t>(maxLeftDelay2Time), leftDelay2Taps);
    rightApf1 = AllpassFilter<double>(maxRightApf1Time, scaledRightApf1Time, 0.7071);
    rightApf2 = MultiTapAllpassFilter<double, 3>(static_cast<uint64_t>(maxRightApf2Time), rightApf2Taps, 0.5);
    rightDelay1 = MultiTapInterpDelay<double, 4>(static_cast<uint64_t>(maxRightDelay1Time), rightDelay1Taps);
    rightDelay2 = MultiTapInterpDelay<double, 3>(static_cast<uint64_t>(maxRightDelay2Time), rightDelay2Taps);

    const double kLeftApf1LfoFreq = 0.10;
    const double kLeftApf2LfoFreq = 0.15;
    const double kRightApf1LfoFreq = 0.12;
    const double kRightApf2LfoFreq = 0.18;

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
                              double* leftOutBuffer, double* rightOutBuffer) {
    for (uint64_t i = 0; i < blockSize; ++i) {
        inputChainBuffer[i] = leftInBuffer[i] + rightInBuffer[i];
    }

    inputLpf.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    inputHpf.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    inApf1.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    inApf2.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    inApf3.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);
    inApf4.blockProcess(inputChainBuffer.data(), inputChainBuffer.data(), blockSize);

    // Reverb tank
    for (uint64_t i = 0; i < blockSize; ++i) {
        tankLeftSum += inputChainBuffer[i];
        tankRightSum += inputChainBuffer[i];

        // 'Tick' the left arm of the tank
        leftApf1Lfo.process();
        leftApf2Lfo.process();
        leftApf1.delay.setDelayTime(static_cast<double>(kLeftApf1Time) + leftApf1Lfo.getOutput() * lfoExcursion);
        leftApf2.delay.setDelayTime(0, static_cast<double>(kLeftApf2Time) + leftApf2Lfo.getOutput() * lfoExcursion);

        leftApf1.input = tankLeftSum;
        leftDelay1.input = leftApf1.process();
        leftDelay1.process();
        leftHighpassFilter.input = leftDelay1.output[0];
        leftHighpassFilter.process();
        leftLowpassFilter.input = leftHighpassFilter.output;
        leftApf2.input = leftLowpassFilter.process() * (1.0 - freezeXFade) + leftDelay1.output[0] * freezeXFade;
        leftDelay2.input = leftApf2.process();
        leftDelay2.process();

        // 'Tick' the right arm of the tank
        rightApf1Lfo.process();
        rightApf2Lfo.process();
        rightApf1.delay.setDelayTime(static_cast<double>(kRightApf1Time) + rightApf1Lfo.getOutput() * lfoExcursion);
        rightApf2.delay.setDelayTime(0, static_cast<double>(kRightApf2Time) + rightApf2Lfo.getOutput() * lfoExcursion);

        rightApf1.input = tankLeftSum;
        rightDelay1.input = rightApf1.process();
        rightDelay1.process();
        rightHighpassFilter.input = rightDelay1.output[0];
        rightHighpassFilter.process();
        rightLowpassFilter.input = rightHighpassFilter.output;
        rightApf2.input = rightLowpassFilter.process() * (1.0 - freezeXFade) + rightDelay1.output[0] * freezeXFade;
        rightDelay2.input = rightApf2.process();
        rightDelay2.process();

        // Feedback to the beginning of the tank arms
        tankRightSum = leftDelay2.output[0] * decay;
        tankLeftSum = rightDelay2.output[0] * decay;

        // Sum outputs from pre-processed taps in each arm
        double leftOutputSum = leftApf1.output;
        leftOutputSum -= leftApf2.delay.output[1];
        leftOutputSum = leftApf2.delay.output[1];
        leftOutputSum += leftDelay1.output[1];
        leftOutputSum += leftDelay1.output[2];
        leftOutputSum += leftDelay2.output[1];
        leftOutputSum -= rightApf2.delay.output[1];
        leftOutputSum -= rightDelay1.output[1];
        leftOutputSum -= rightDelay2.output[1];

        double rightOutputSum = rightApf1.output;
        rightOutputSum -= rightApf2.delay.output[2];
        rightOutputSum = rightApf2.delay.output[2];
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
    leftHighpassFilter.clear();
    leftLowpassFilter.clear();
    leftApf2.clear();
    leftDelay2.clear();

    rightApf1.clear();
    rightDelay1.clear();
    rightHighpassFilter.clear();
    rightLowpassFilter.clear();
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
    decay = newDecay < 0.0 ? 0.0 : (newDecay > 1.0 ? 1.0 : newDecay);
}
