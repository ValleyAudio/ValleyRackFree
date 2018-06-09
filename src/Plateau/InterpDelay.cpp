#include "InterpDelay.hpp"

InterpDelay::InterpDelay() {
    input = 0.0;
    output = 0.0;
    delayTime = 0.0;
    _length = 2;
    _buffer.assign(_length, 0.0);
    _readPos = 1.0;
    _phasedPos = 0.0;
    _lowerReadPos = 0;
    _upperReadPos = 0;
    _writePos = 0;
}

InterpDelay::InterpDelay(long maxLength, long initDelay) {
    input = 0.0;
    output = 0.0;
    delayTime = initDelay;
    _length = maxLength;
    _buffer.assign(_length, 0.0);
    _readPos = maxLength - 1.0;
    _phasedPos = 0.0;
    _lowerReadPos = 0;
    _upperReadPos = 0;
    _writePos = 0;
}

double InterpDelay::process() {
    _time = delayTime;
    if(_time < 0.0) {
        _time = 0.0;
    }
    else if(_time >= _length) {
        _time = _length - 1;
    }
    _buffer[_writePos] = input;
    _phasedPos = (double)_writePos - _time;
    if(_phasedPos < 0.0) {
        _phasedPos += (double)_length;
    }
    _ratio = _phasedPos - (long)_phasedPos;
    _lowerReadPos = (long)_phasedPos % _length;
    _upperReadPos = (_lowerReadPos + 1) % _length;
    output = _buffer[_lowerReadPos] * (1.0 - _ratio) + _buffer[_upperReadPos] *_ratio;

    _writePos++;
    if(_writePos >= _length) {
        _writePos -= _length;
    }

    return output;
}

void InterpDelay::clear() {
    _buffer.assign(_length, 0.0);
    input = 0.0;
    output = 0.0;
}

void InterpDelay::setMaxDelaySamples(const long maxDelaySamples) {
    _length = maxDelaySamples;
    _buffer.assign(_length, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
