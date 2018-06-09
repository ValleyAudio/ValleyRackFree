#pragma once
#include <vector>
#include <cmath>
#include <pmmintrin.h>
#define MAX_DELAY_TAP_GROUPS 512
#define MAX_DELAY_LENGTH 65536

class InterpDelay
{
public:
    InterpDelay();
    InterpDelay(long maxLength, long initDelay);
    double process();
    void clear();
    void setMaxDelaySamples(long maxDelaySamples);
    double input;
    double delayTime;
    double output;
private:
    std::vector<double> _buffer;
    long _length;
    double _time;
    double _readPos;
    double _phasedPos;
    long _upperReadPos;
    long _lowerReadPos;
    double _ratio;
    long _writePos;
};

template<class T, const int kNumTaps>
class MultiTapInterpDelay {
public:
    MultiTapInterpDelay() {
        _length = 2;
        _buffer.assign(_length, 0);
        for(auto i = 0; i < kNumTaps; ++i) {
            _output[i] = 0;
            _time[i] = 0;
        }
        _readPos = 1.0;
        _phasedPos = 0.0;
        _lower = 0;
        _upper = 0;
        _writePos = 0;
    }

    MultiTapInterpDelay(long maxLength) {
        _length = maxLength;
        _buffer.assign(_length, 0);
        for(auto i = 0; i < kNumTaps; ++i) {
            _output[i] = 0;
            _time[i] = 0;
        }
        _readPos = 1.0;
        _phasedPos = 0.0;
        _lower = 0;
        _upper = 0;
        _writePos = 0;
    }

    void process(T input) {
        _buffer[_writePos] = input;
        for(auto i = 0; i < kNumTaps; ++i) {
            _phasedPos = (T)_writePos - _time[i];
            if(_phasedPos < 0) {
                _phasedPos += (T)_length;
            }
            _frac = _phasedPos - (long)_phasedPos;
            _lower = (long)_phasedPos;
            _upper = _lower + 1;
            if(_upper >= _length) {
                _upper -= _length;
            }
            _output[i] = _buffer[_lower] + _frac * (_buffer[_upper] - _buffer[_lower]);
        }
        _writePos++;
        if(_writePos >= _length) {
            _writePos -= _length;
        }
    }

    T operator[](const int i) const {
        return _output[i];
    }

    void setDelaySamples(int tap, T dSamples) {
        _time[tap] = dSamples;
    }

    void clear() {
        _buffer.assign(_length, 0);
        for(auto i = 0; i < kNumTaps; ++i) {
            _output[i] = 0;
        }
    }
private:
    std::vector<T> _buffer;
    long _length;
    T _output[kNumTaps];
    T _time[kNumTaps];
    long _readPos;
    T _phasedPos;
    long _upper;
    long _lower;
    T _frac;
    long _writePos;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T, const int kNumTapGroups>
class VecMultiTapDelay {
public:
    VecMultiTapDelay() {
        for(auto i = 0; i < MAX_DELAY_LENGTH; ++i) {
            _buffer[i] = 0.0;
        }
        for(auto i = 0; i < kNumTapGroups; ++i) {
            _time[i] = _mm_set1_epi16(0);
            _readPos[i] = _mm_set1_epi16(MAX_DELAY_LENGTH - 1);
        }
        for(auto i = 0; i < kNumTapGroups * 8; ++i) {
            _output[i] = 0.0;
        }
        _phasedPos = _mm_set1_epi16(0);
        _writePos = 0;
    }

    void process(T input) {
        _buffer[_writePos] = input;
        long j = 0;
        for(auto i = 0; i < kNumTapGroups; ++i) {
            j = i * 8;
            _phasedPos = _mm_sub_epi16(_readPos[i], _time[i]);
            _mm_storeu_si128((__m128i*)_sReadPhasePos, _phasedPos);
            _output[j] = _buffer[_sReadPhasePos[0]];
            _output[j + 1] = _buffer[_sReadPhasePos[1]];
            _output[j + 2] = _buffer[_sReadPhasePos[2]];
            _output[j + 3] = _buffer[_sReadPhasePos[3]];
            _output[j + 4] = _buffer[_sReadPhasePos[4]];
            _output[j + 5] = _buffer[_sReadPhasePos[5]];
            _output[j + 6] = _buffer[_sReadPhasePos[6]];
            _output[j + 7] = _buffer[_sReadPhasePos[7]];
            _readPos[i] = _mm_add_epi16(_readPos[i], _ones);
        }
        _writePos++;
        if(_writePos >= MAX_DELAY_LENGTH) {
            _writePos -= MAX_DELAY_LENGTH;
        }
    }

    void clear() {
        for(auto i = 0; i < MAX_DELAY_LENGTH; ++i) {
            _buffer[i] = 0.0;
        }
        for(auto i = 0; i < kNumTapGroups * 8; ++i) {
            _output[i] = 0.0;
        }
    }

    T getTapOutput(long tap) const {
        return _output[tap];
    }

    const T operator[](long tap) const {
        return _output[tap];
    }

    void setDelayTime(long tap, short delayTime) {
        long group = tap / 8;
        long subTap = tap % 8;
        if(group > kNumTapGroups) {
            group = kNumTapGroups;
        }
        else if(group < 0) {
            group = 0;
        }
        short newTime[8];
        _mm_storeu_si128((__m128i*)newTime, _time[group]);
        newTime[subTap] = delayTime;
        _time[group] = _mm_loadu_si128((__m128i*)newTime);
    }
private:
    T _buffer[MAX_DELAY_LENGTH];
    __m128i _time[kNumTapGroups];
    __m128i _readPos[kNumTapGroups];
    T _output[kNumTapGroups * 8];
    long _numTaps;
    __m128i _phasedPos;
    __m128i _ones;
    unsigned short _sReadPhasePos[8];
    unsigned short _writePos;
};
