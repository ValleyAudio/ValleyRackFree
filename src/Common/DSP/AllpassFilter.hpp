/**
 * @file AllpassFilter.hpp
 * @author Dale Johnson
 * @date ...uhhh
 */

#pragma once
#include "InterpDelay2.hpp"

template<class T>
class AllpassFilter {
public:
    AllpassFilter() {
        clear();
        gain = 0;
    }

    AllpassFilter(long maxDelay, long initDelay, T gain) {
        clear();
        delay = InterpDelay2<T>(maxDelay, initDelay);
        this->gain = gain;
    }

    T inline process() {
        _inSum = input + delay.output * gain;
        output = delay.output + _inSum * gain * -1;
        delay.input = _inSum;
        delay.process();
        return output;
    }

    void blockProcess(const T* inputBuffer, T* outputBuffer, const uint64_t blockSize) {
        for (uint64_t i = 0; i < blockSize; ++i) {
            _inSum = inputBuffer[i] + delay.output * gain;
            output = delay.output + _inSum * gain * -1;
            delay.input = _inSum;
            delay.process();
            outputBuffer[i] = output;
        }
    }

    void clear() {
        input = 0;
        output = 0;
        _inSum = 0;
        _outSum = 0;
        delay.clear();
    }

    T input;
    T gain;
    T output;
    InterpDelay2<T> delay;
private:
    T _inSum;
    T _outSum;
};

template<class T>
class NestedAllPassType1 {
public:
    NestedAllPassType1() {
        gain1 = 0;
        gain2 = 0;
        decay1 = 0;
        decay2 = 0;
        clear();
    }

    NestedAllPassType1(long maxDelay, long delayTime1, long delayTime2) {
        delay1 = InterpDelay2<T>(maxDelay, delayTime1);
        delay2 = InterpDelay2<T>(maxDelay, delayTime2);
        gain1 = 0;
        gain2 = 0;
        decay1 = 0;
        decay2 = 0;
        clear();
    }

    T inline process() {
        _inSum1 = input + delay1.output * gain1;
        _inSum2 = _inSum1 + delay2.output * gain2;
        delay2.input = _inSum2;
        delay1.input = delay2.output * decay2 + _inSum2 * -gain2;
        output = delay1.output * decay1 + _inSum1 * -gain1;
        delay1.process();
        delay2.process();
        return output;
    }

    void clear() {
        input = 0;
        output = 0;
        _inSum1 = 0;
        _inSum2 = 0;
        delay1.clear();
        delay2.clear();
    }

    T input;
    T gain1, gain2;;
    T output;
    T decay1, decay2;
    InterpDelay2<T> delay1, delay2;
private:
    T _inSum1, _inSum2;
};
