#include "AllpassFilter.hpp"

AllpassFilter::AllpassFilter() {
    clear();
    gain = 0.f;
}

AllpassFilter::AllpassFilter(long maxDelay, long initDelay, float gain) {
    clear();
    delay = InterpDelay(maxDelay, initDelay);
    this->gain = gain;
}

float AllpassFilter::process() {
    _inSum = input + delay.output * gain;
    output = delay.output + _inSum * -gain;
    delay.input = _inSum;
    delay.process();
    return output;
}

void AllpassFilter::clear() {
    input = 0.f;
    output = 0.f;
    _inSum = 0.f;
    _outSum = 0.f;
    delay.clear();
}
