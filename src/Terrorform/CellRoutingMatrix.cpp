#include "CellRoutingMatrix.hpp"

CellRoutingMatrixRow::CellRoutingMatrixRow() {
    _sourceValue = 0.f;
    _depth = 0.f;
    _destination = PITCH_DEST;
}

CellRoutingMatrix::CellRoutingMatrix() {
    for(auto i = 0; i < NUM_DESTS; ++i) {
        _destValues[i] = 0.f;
    }

    kMatrixDestScaling[PITCH_DEST] = 1.f;
    kMatrixDestScaling[WAVE_BANK_DEST] = 1.f;
    kMatrixDestScaling[WAVE_POS_DEST] = 1.f;
    kMatrixDestScaling[SHAPE_TYPE_DEST] = 1.f;
    kMatrixDestScaling[SHAPE_DEPTH_DEST] = 1.f;
    kMatrixDestScaling[DEGRADE_TYPE_DEST] = 1.f;
    kMatrixDestScaling[DEGRADE_DEPTH_DEST] = 1.f;
    kMatrixDestScaling[EXT_FM_DEST] = 1.f;
    kMatrixDestScaling[EXT_FM_VCA_DEST] = 1.f;
    kMatrixDestScaling[EXT_SYNC_DEST] = 1.f;
    kMatrixDestScaling[PERC_TRIGGER_DEST] = 1.f;
    kMatrixDestScaling[PERC_DECAY_DEST] = 1.f;

}

void CellRoutingMatrix::process() {
    for(auto i = 0; i < NUM_DESTS; ++i) {
        _destValues[i] = 0.f;
    }

    for(auto i = 0; i < kNumMatrixRows; ++i) {
        _destValues[_rows[i]._destination] += _rows[i]._sourceValue * _rows[i]._depth *
                                              kMatrixDestScaling[_rows[i]._destination];
    }
}

float CellRoutingMatrix::getDestinationValue(CellRoutingMatrixDestination dest) const {
    return _destValues[dest];
}

void CellRoutingMatrix::setRowSourceValue(int row, float sourceValue) {
    _rows[row]._sourceValue = sourceValue;
}

void CellRoutingMatrix::setRowDepth(int row, float depth) {
    _rows[row]._depth = depth;
}

void CellRoutingMatrix::setRowDestination(int row, CellRoutingMatrixDestination dest) {
    _rows[row]._destination = dest;
}
