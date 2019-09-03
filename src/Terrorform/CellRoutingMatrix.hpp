#pragma once

enum CellRoutingMatrixDestination {
    PITCH_DEST = 0,
    WAVE_BANK_DEST,
    WAVE_POS_DEST,
    SHAPE_TYPE_DEST,
    SHAPE_DEPTH_DEST,
    DEGRADE_TYPE_DEST,
    DEGRADE_DEPTH_DEST,
    EXT_FM_DEST,
    EXT_FM_VCA_DEST,
    EXT_SYNC_DEST,
    PERC_TRIGGER_DEST,
    PERC_DECAY_DEST,
    NUM_DESTS
};

struct CellRoutingMatrixRow {
    CellRoutingMatrixRow();
    float _sourceValue;
    float _depth;
    CellRoutingMatrixDestination _destination;
};

class CellRoutingMatrix {
public:
    CellRoutingMatrix();
    void process();
    float getDestinationValue(CellRoutingMatrixDestination dest) const;
    void setRowSourceValue(int row, float sourceValue);
    void setRowDepth(int row, float depth);
    void setRowDestination(int row, CellRoutingMatrixDestination dest);
private:
    static const int kNumMatrixRows = 3;
    CellRoutingMatrixRow _rows[kNumMatrixRows];
    float _destValues[NUM_DESTS];
    float kMatrixDestScaling[NUM_DESTS];
};
