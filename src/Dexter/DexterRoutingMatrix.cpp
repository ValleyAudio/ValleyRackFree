//
// DexterRoutingMatrix.cpp
// Author: Dale Johnson
//
// Copyright (c) 2020 Dale Johnson, Valley Audio
// Licensed under GPLv3
//

#include "DexterRoutingMatrix.hpp"


DexterRoutingMatrix::DexterRoutingMatrix() {
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

void DexterRoutingMatrix::process() {
    for(auto i = 0; i < NUM_DESTS; ++i) {
        _destValues[i] = 0.f;
    }

    for(auto i = 0; i < kNumMatrixRows; ++i) {
        auto rowDest = _rows[i]._destination;
        _destValues[rowDest] += _rows[i]._sourceValue * _rows[i]._depth *
                                kMatrixDestScaling[rowDest];
    }
}

float DexterRoutingMatrix::getDestinationValue(
        const DexterRoutingMatrix::Destination dest) const
{
    return _destValues[dest];
}

void DexterRoutingMatrix::setRowSourceValue(
        const int row,
        const float sourceValue)
{
    _rows[row]._sourceValue = sourceValue;
}

void DexterRoutingMatrix::setRowDepth(const int row, const float depth) {
    _rows[row]._depth = depth;
}

void DexterRoutingMatrix::setRowDestination(
        const int row,
        const DexterRoutingMatrix::Destination dest)
{
    _rows[row]._destination = dest;
}

DexterRoutingMatrix::DexterRoutingMatrixRow::DexterRoutingMatrixRow() {
    _sourceValue = 0.f;
    _depth = 0.f;
    _destination = PITCH_DEST;
}
