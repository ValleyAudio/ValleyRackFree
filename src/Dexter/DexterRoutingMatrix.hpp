//
// DexterRoutingMatrix.hpp
// Author: Dale Johnson
//
// Copyright (c) 2020 Dale Johnson, Valley Audio
// Licensed under GPLv3
//

#pragma once

class DexterRoutingMatrix {
public:
    enum Destination {
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

    DexterRoutingMatrix();

    void process();

    float getDestinationValue(const Destination dest) const;

    void setRowSourceValue(const int row, const float sourceValue);
    void setRowDepth(const int row, const float depth);
    void setRowDestination(const int row, const Destination dest);

private:
    struct DexterRoutingMatrixRow {
        DexterRoutingMatrixRow();
        float _sourceValue;
        float _depth;
        Destination _destination;
    };

    static const int kNumMatrixRows = 4;
    DexterRoutingMatrixRow _rows[kNumMatrixRows];
    float _destValues[NUM_DESTS];
    float kMatrixDestScaling[NUM_DESTS];
};
