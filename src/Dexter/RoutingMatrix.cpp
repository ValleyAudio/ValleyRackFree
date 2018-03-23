//
// RoutingMatrix.cpp
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

#include "RoutingMatrix.hpp"

RoutingMatrixRow::RoutingMatrixRow() {
    _sourceValue = 0.f;
    _depth = 0.f;
    _destination = PITCH_DEST;
}

RoutingMatrix::RoutingMatrix() {
    for(auto i = 0; i < NUM_DESTS; ++i) {
        _destValues[i] = 0.f;
    }

    kMatrixDestScaling[PITCH_DEST] = 5.f;
    kMatrixDestScaling[RATIO_DEST] = 27.f;
    kMatrixDestScaling[WAVE_POS_DEST] = 1.f;
    kMatrixDestScaling[WAVE_BANK_DEST] = 34.1f;
    kMatrixDestScaling[SHAPE_DEST] = 1.f;
    kMatrixDestScaling[LEVEL_DEST] = 1.f;
    kMatrixDestScaling[EXT_FM_DEST] = 1.f;
    kMatrixDestScaling[EXT_SYNC_DEST] = 1.f;
    kMatrixDestScaling[SHAPE_MODE_DEST] = 11.1f;
    kMatrixDestScaling[POST_SHAPE_DEST] = 1.f;
    kMatrixDestScaling[SYNC_MODE_DEST] = 14.1f;
    kMatrixDestScaling[SYNC_ENABLE_DEST] = 1.f;
    kMatrixDestScaling[WEAK_SYNC_DEST] = 1.f;

}

void RoutingMatrix::process() {
    for(auto i = 0; i < NUM_DESTS; ++i) {
        _destValues[i] = 0.f;
    }

    for(auto i = 0; i < kNumMatrixRows; ++i) {
        _destValues[_rows[i]._destination] += _rows[i]._sourceValue * _rows[i]._depth *
                                              kMatrixDestScaling[_rows[i]._destination];
    }
}

float RoutingMatrix::getDestinationValue(RoutingMatrixDestination dest) const {
    return _destValues[dest];
}

void RoutingMatrix::setRowSourceValue(int row, float sourceValue) {
    _rows[row]._sourceValue = sourceValue;
}

void RoutingMatrix::setRowDepth(int row, float depth) {
    _rows[row]._depth = depth;
}

void RoutingMatrix::setRowDestination(int row, RoutingMatrixDestination dest) {
    _rows[row]._destination = dest;
}
