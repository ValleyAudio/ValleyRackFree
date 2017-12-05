//
// Metronome.cpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 5/12/17
//
// Copyright 2017 Dale Johnson
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "Metronome.hpp"

Metronome::Metronome() {
    _sampleRate = 44100.0;
    _tempo = 120.0;
    _division = 4.0;
    _elapsedTickTime = 1.0;
    _ticked = false;
    calcTickIncrement();
}

Metronome::Metronome(float initTempo, float sampleRate) {
    _sampleRate = sampleRate;
    _tempo = initTempo;
    _division = 4.0;
    _elapsedTickTime = 1.0;
    _ticked = false;
    calcTickIncrement();
}

Metronome::Metronome(float initTempo, float sampleRate, float division) {
    _sampleRate = sampleRate;
    _tempo = initTempo;
    _division = division;
    _elapsedTickTime = 0.0;
    _ticked = false;
    calcTickIncrement();
}

void Metronome::process() {
    if(_elapsedTickTime >= 1.0) {
        _ticked = true;
        _elapsedTickTime = 0.0;
    }
    else {
        _ticked = false;
        _elapsedTickTime += _tickIncrement;
    }
}

void Metronome::reset() {
    _elapsedTickTime = 1.0;
    _ticked = true;
}

void Metronome::setSampleRate(float sampleRate) {
    _sampleRate = sampleRate;
    calcTickIncrement();
}

void Metronome::setTempo(float tempo) {
    _tempo = tempo;
    calcTickIncrement();
}

void Metronome::setDivision(float division) {
    _division = division;
    calcTickIncrement();
}

bool Metronome::hasTicked() const {
    return _ticked;
}

void Metronome::calcTickIncrement() {
    _beatInterval = (60.0 * 4.0) / (_tempo * _division);
    _tickIncrement = 1.0 / (_beatInterval * _sampleRate);
}
