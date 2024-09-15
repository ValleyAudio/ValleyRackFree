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
    calcTickIncrement();
}

Metronome::Metronome(float initTempo,
                     float initSampleRate,
                     float initDivision,
                     float initPhase)
    : tempo(initTempo),
      sampleRate(initSampleRate),
      division(initDivision),
      phase(initPhase)
{
    phasedElapsedTickTime = phase;
    prevPhasedElapsedTickTime = phasedElapsedTickTime;
    calcTickIncrement();
}

void Metronome::process() {
    prevPhasedElapsedTickTime = phasedElapsedTickTime;
    phasedElapsedTickTime = elapsedTickTime + phase;

    if (phasedElapsedTickTime >= 1.0) {
        phasedElapsedTickTime -= 1.0;
    }

    ticked = prevPhasedElapsedTickTime > phasedElapsedTickTime;

    // Wrap real timer
    elapsedTickTime += tickIncrement;
    if(elapsedTickTime >= 1.0) {
        elapsedTickTime -= 1.0;
    }
}

void Metronome::reset() {
    phasedElapsedTickTime = phase;
    prevPhasedElapsedTickTime = phasedElapsedTickTime;
    elapsedTickTime = 1.f;
    ticked = true;
}

void Metronome::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    calcTickIncrement();
}

void Metronome::setTempo(float newTempo) {
    tempo = newTempo;
    calcTickIncrement();
}

void Metronome::setDivision(float newDivision) {
    division = newDivision;
    calcTickIncrement();
}

void Metronome::setPhase(float newPhase) {
    phase = newPhase;
}

bool Metronome::hasTicked() const {
    return ticked;
}

float Metronome::getElapsedTickTime() const {
    return elapsedTickTime;
}

void Metronome::calcTickIncrement() {
    beatInterval = 60.f / (tempo * division);
    tickIncrement = 1.f / (beatInterval * sampleRate);
}

