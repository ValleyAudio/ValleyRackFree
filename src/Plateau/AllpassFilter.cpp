#include "AllpassFilter.hpp"

AllpassFilter::AllpassFilter() {
    clear();
    gain = 0.f;
}

AllpassFilter::AllpassFilter(long maxDelay, long initDelay, double gain) {
    clear();
    delay = InterpDelay<double>(maxDelay, initDelay);
    this->gain = gain;
}

double AllpassFilter::process() {
    _inSum = input + delay.output * gain;
    output = delay.output + _inSum * gain * -1.f;
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
