//
// DexterRoutingMatrix.hpp
// Author: Dale Johnson
//
// Copyright (c) 2020 Dale Johnson, Valley Audio
// Licensed under GPLv3
//


#pragma once

enum DexterRoutingMatrixDestination {
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

struct DexterRoutingMatrixRow {
    DexterRoutingMatrixRow();
    float _sourceValue;
    float _depth;
    DexterRoutingMatrixDestination _destination;
};

class DexterRoutingMatrix {
public:
    DexterRoutingMatrix();
    void process();
    float getDestinationValue(DexterRoutingMatrixDestination dest) const;
    void setRowSourceValue(int row, float sourceValue);
    void setRowDepth(int row, float depth);
    void setRowDestination(int row, DexterRoutingMatrixDestination dest);
private:
    static const int kNumMatrixRows = 4;
    DexterRoutingMatrixRow _rows[kNumMatrixRows];
    float _destValues[NUM_DESTS];
    float kMatrixDestScaling[NUM_DESTS];
};
