//
// RoutingMatrix.hpp
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

#ifndef DEXTER_ROUTING_MATRIX_HPP
#define DEXTER_ROUTING_MATRIX_HPP

enum RoutingMatrixDestination {
    PITCH_DEST = 0,
    RATIO_DEST,
    WAVE_POS_DEST,
    WAVE_BANK_DEST,
    SHAPE_DEST,
    LEVEL_DEST,
    EXT_FM_DEST,
    EXT_SYNC_DEST,
    SHAPE_MODE_DEST,
    POST_SHAPE_DEST,
    SYNC_MODE_DEST,
    SYNC_ENABLE_DEST,
    WEAK_SYNC_DEST,
    NUM_DESTS
};

struct RoutingMatrixRow {
    RoutingMatrixRow();
    float _sourceValue;
    float _depth;
    RoutingMatrixDestination _destination;
};

const int kNumMatrixRows = 4;

class RoutingMatrix {
public:
    RoutingMatrix();
    void process();
    float getDestinationValue(RoutingMatrixDestination dest) const;
    void setRowSourceValue(int row, float sourceValue);
    void setRowDepth(int row, float depth);
    void setRowDestination(int row, RoutingMatrixDestination dest);
private:
    RoutingMatrixRow _rows[kNumMatrixRows];
    float _destValues[NUM_DESTS];
    float kMatrixDestScaling[NUM_DESTS];
};

#endif
