/**
 * @file DattorroV2.cpp
 * @author Dale Johnson
 */

#include "DattorroV2.hpp"
#include <_types/_uint64_t.h>

DattorroV2::DattorroV2(double initSampleRate, uint64_t initBlockSize) :
    inputChainBuffer(initBlockSize, 0.0)
{
    // Initial delay times
    const uint64_t kInApf1Time = 141;
    const uint64_t kInApf2Time = 107;
    const uint64_t kInApf3Time = 379;
    const uint64_t kInApf4Time = 277;

    const uint64_t kLeftApf1Time = 672;
    const uint64_t kLeftApf2Time = 1800;
    const uint64_t kLeftDelay1Time = 4453;
    const uint64_t kLeftDelay2Time = 3720;

    const uint64_t kRightApf1Time = 908;
    const uint64_t kRightApf2Time = 2656;
    const uint64_t kRightDelay1Time = 4217;
    const uint64_t kRightDelay2Time = 3163;

    // Delay taps
    const std::array<double, 4> leftDelay1Taps = {4451, 266, 2974, 1990};
    const std::array<double, 3> leftDelay2Taps = {3720, 1996, 1066};
    const std::array<double, 4> rightDelay1Taps = {4217, 1990, 266, 2974};
    const std::array<double, 3> rightDelay2Taps = {3163, 1066, 1996};
    const std::array<double, 3> leftApf2Taps = {1800, 1913, 187};
    const std::array<double, 3> rightApf2Taps = {2656, 187, 1913};

    // Construct!
    blockSize = initBlockSize;

    inApf1 = AllpassFilter<double>(kInApf1Time, kInApf1Time, 0.7071);
    inApf2 = AllpassFilter<double>(kInApf2Time, kInApf2Time, 0.7071);
    inApf3 = AllpassFilter<double>(kInApf3Time, kInApf3Time, 0.7071);
    inApf4 = AllpassFilter<double>(kInApf4Time, kInApf4Time, 0.7071);

    leftApf1 = AllpassFilter<double>(kLeftApf1Time, kLeftApf1Time, 0.7071);
    leftApf2 = MultiTapAllpassFilter<double, 3>(kLeftApf2Time, kLeftApf2Time, 0.7071);
    leftDelay1 = MultiTapInterpDelay<double, 4>(kLeftDelay1Time, kLeftDelay1Time);
    leftDelay2 = MultiTapInterpDelay<double, 3>(kLeftDelay2Time, kLeftDelay2Time);
    rightApf1 = AllpassFilter<double>(kRightApf1Time, kRightApf1Time, 0.7071);
    rightApf2 = MultiTapAllpassFilter<double, 3>(kRightApf2Time, kRightApf2Time, 0.7071);
    rightDelay1 = MultiTapInterpDelay<double, 4>(kRightDelay1Time, kRightDelay1Time);
    rightDelay2 = MultiTapInterpDelay<double, 3>(kRightDelay2Time, kRightDelay2Time);

    // Apply taps
    leftDelay1.setDelayTimes(leftDelay1Taps);
    leftDelay2.setDelayTimes(leftDelay2Taps);
    rightDelay1.setDelayTimes(rightDelay1Taps);
    rightDelay2.setDelayTimes(rightDelay2Taps);
    leftApf2.delay.setDelayTimes(leftApf2Taps);
    rightApf2.delay.setDelayTimes(rightApf2Taps);
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

        leftApf1.input = tankLeftSum;
        leftDelay1.input = leftApf1.process();
        leftDelay1.process();
        leftApf2.input = leftDelay1.output[0];
        leftDelay2.input = leftApf2.process();
        leftDelay2.process();

        rightApf1.input = tankLeftSum;
        rightDelay1.input = rightApf1.process();
        rightDelay1.process();
        rightApf2.input = rightDelay1.output[0];
        rightDelay2.input = rightApf2.process();
        rightDelay2.process();

        tankRightSum = leftDelay2.output[0] * decay;
        tankLeftSum = rightDelay2.output[0] * decay;

        leftOutputSum = leftApf1.output;
        leftOutputSum -= leftApf2.delay.output[1];
        leftOutputSum += leftDelay1.output[1];
        leftOutputSum += leftDelay1.output[2];
        leftOutputSum += leftDelay2.output[1];
        leftOutputSum -= rightApf2.delay.output[1];
        leftOutputSum -= rightDelay1.output[1];
        leftOutputSum -= rightDelay2.output[1];

        rightOutputSum = rightApf1.output;
        rightOutputSum -= rightApf2.delay.output[2];
        rightOutputSum += rightDelay1.output[2];
        rightOutputSum += rightDelay1.output[3];
        rightOutputSum += rightDelay2.output[2];
        rightOutputSum -= leftApf2.delay.output[2];
        rightOutputSum -= leftDelay1.output[3];
        rightOutputSum -= leftDelay2.output[2];

        leftOutBuffer[i] = leftOutputSum;
        rightOutBuffer[i] = rightOutputSum;
    }
}

