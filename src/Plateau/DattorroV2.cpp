
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
        leftApf2.input = leftDelay1.output;
        leftDelay2.input = leftApf2.process();
        leftDelay2.process();

        tankRightSum = leftDelay2.output * decay;
        tankLeftSum = rightDelay2.output * decay;
    }
}
