/**
 * @file DattorroV2.cpp
 * @author Dale Johnson
 */

#include "DattorroV2.hpp"
#include <_types/_uint64_t.h>

DattorroV2::DattorroV2(double initSampleRate, uint64_t initBlockSize) :
    inputChainBuffer(initBlockSize, 0.0)
{
    blockSize = initBlockSize;
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
        rightOutputSum -= rightApf2.delay.output[1];
        rightOutputSum += rightDelay1.output[1];
        rightOutputSum += rightDelay1.output[2];
        rightOutputSum += rightDelay2.output[1];
        rightOutputSum -= leftApf2.delay.output[1];
        rightOutputSum -= leftDelay1.output[1];
        rightOutputSum -= leftDelay2.output[1];

        leftOutBuffer[i] = leftOutputSum;
        rightOutBuffer[i] = rightOutputSum;
    }
}

