/**
 * @file AllpassFilter.hpp
 * @author Dale Johnson
 * @date ...uhhh
 */

#pragma once
#include "InterpDelay.hpp"
#include <cassert>

template<class T>
class AllpassFilter {
public:
    AllpassFilter() {
        clear();
        gain = 0;
    }

    AllpassFilter(long maxDelay, long initDelay = 0, T gain = 0) {
        clear();
        delay = InterpDelay<T>(maxDelay, initDelay);
        this->gain = gain;
    }

    T inline process() {
        inSum = input + delay.output * gain;
        output = delay.output + inSum * gain * -1;
        delay.input = inSum;
        delay.process();
        return output;
    }

    void clear() {
        input = 0;
        output = 0;
        inSum = 0;
        _outSum = 0;
        delay.clear();
    }

    void setGain(const T newGain) {
        assert(newGain >= -1.0);
        assert(newGain <= 1.0);

        gain = newGain;
    }

    T input;
    T output;
    InterpDelay<T> delay;

private:
    T gain;
    T inSum;
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
        delay1 = InterpDelay<T>(maxDelay, delayTime1);
        delay2 = InterpDelay<T>(maxDelay, delayTime2);
        gain1 = 0;
        gain2 = 0;
        decay1 = 0;
        decay2 = 0;
        clear();
    }

    T inline process() {
        inSum1 = input + delay1.output * gain1;
        inSum2 = inSum1 + delay2.output * gain2;
        delay2.input = inSum2;
        delay1.input = delay2.output * decay2 + inSum2 * -gain2;
        output = delay1.output * decay1 + inSum1 * -gain1;
        delay1.process();
        delay2.process();
        return output;
    }

    void clear() {
        input = 0;
        output = 0;
        inSum1 = 0;
        inSum2 = 0;
        delay1.clear();
        delay2.clear();
    }

    T input;
    T gain1, gain2;;
    T output;
    T decay1, decay2;
    InterpDelay<T> delay1, delay2;
private:
    T inSum1, inSum2;
};
