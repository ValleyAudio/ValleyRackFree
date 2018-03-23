//
// FreqLUT.cpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 18/3/2018
//
// Copyright 2018 Dale Johnson. Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met: 1. Redistributions of
// source code must retain the above copyright notice, this list of conditions and the following
// disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this
// list of conditions and the following disclaimer in the documentation and/or other materials
// provided with the distribution. 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this software without
// specific prior written permission.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.

#include "FreqLUT.hpp"

FreqLUT::FreqLUT() {
    _resolution = 100000.f;
    _pitch = 0;
    _pos = 0;
    _inputLow = -5.f;
    _inputHigh = 6.f;
    makeLUT();
}

float FreqLUT::getFrequency(float pitch) {
    _pitch = pitch * _resolution + (5.f * _resolution);
    if(_pitch > _lut.size() - 2) {
        _pitch = _lut.size() - 2;
    }
    if(_pitch < 0) {
        _pitch = 0;
    }
    _pos = (long)_pitch;
    _frac = _pitch - (float)_pos;
    return _lut[_pos] + _frac * (_lut[_pos + 1] - _lut[_pos]);
}

void FreqLUT::makeLUT() {
    float startPitch = _inputLow * _resolution;
    float endPitch = _inputHigh * _resolution;

    _lut.clear();
    for(float i = startPitch; i <= endPitch; i += 1.f) {
        _lut.push_back(261.6255f * powf(2.f, i / _resolution));
    }
}
