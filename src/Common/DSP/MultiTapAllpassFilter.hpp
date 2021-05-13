/**
 * @file MultiTapAllpassFilter.hpp
 * @author Dale Johnson
 * @date ...uhhh
 */

#pragma once
#include "MultiTapInterpDelay.hpp"

template<typename T = float, uint64_t numTaps = 1>
class MultiTapAllpassFilter {
public:
    /** Construct a new MultiTapAllpassFilter, either from default or developer specified
    arguments */
    MultiTapAllpassFilter(uint64_t maxDelay = 2, uint64_t initDelay = 0, T initGain = T(0)) :
        delay(maxDelay, initDelay)
    {
        gain = initGain;
    }

    /** Construct a new MultiTapAllpassFilter, either from default or developer specified
    arguments */
    MultiTapAllpassFilter(uint64_t maxDelay,
                          const std::array<T, numTaps>& initTimes,
                          T initGain = T(0)) :
        delay(maxDelay, initTimes)
    {
        gain = initGain;
    }

    /** Single tick process. The all-pass filtering uses only the 0th or first tap only.*/
    T inline process() {
        _inSum = input + delay.output[0] * gain;
        output = delay.output[0] + _inSum * gain * T(-1);
        delay.input = _inSum;
        delay.process();
        return output;
    }

    /** Block process. The all-pass filtering uses only the 0th or first tap only.
     * @param inputBuffer The input audio buffer to be all-pass filtered. The size of this array
     * equals 1 x blockSize
     * @param outputBuffer The output audio buffer. The size of this array equals 1 x blockSize.
     */
    void blockProcess(const T* inputBuffer, T* outputBuffer, const uint64_t blockSize) {
        for (uint64_t i = 0; i < blockSize; ++i) {
            _inSum = inputBuffer[i] + delay.output[0] * gain;
            output = delay.output[0] + _inSum * gain * T(-1);
            delay.input = _inSum;
            delay.process();
            outputBuffer[i] = output;
        }
    }

    /** Clears the I/O and internal delay of this all-pass filter.*/
    void clear() {
        input = 0;
        output = 0;
        _inSum = 0;
        _outSum = 0;
        delay.clear();
    }

    T input = T(0);
    T output = T(0);
    T gain = T(0);

    MultiTapInterpDelay<T, numTaps> delay;

private:
    T _inSum = T(0);
    T _outSum = T(0);
};

