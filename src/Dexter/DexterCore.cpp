//
// Osc4Core_SIMD.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 18/3/2018
//

#include "DexterCore.hpp"

DexterCore::DexterCore() {
    algorithm = -1;
    setWavebank(0, 0);
    setWavebank(1, 0);
    setWavebank(2, 0);
    setWavebank(3, 0);
    setWavePosition(0, 0.f);
    setWavePosition(1, 0.f);
    setWavePosition(2, 0.f);
    setWavePosition(3, 0.f);
    onesVec = _mm_set1_ps(1.f);
    zerosVec = _mm_set1_ps(0.f);
    fiveVec = _mm_set1_ps(5.f);
    outputLevelsVec[0] = _mm_set1_ps(5.f);
    outputLevelsVec[1] = _mm_set1_ps(3.3f);
    outputLevelsVec[2] = _mm_set1_ps(2.5f);
    outputLevelsVec[3] = _mm_set1_ps(2.f);
    opLevelVec[0] = zerosVec;
    opLevelVec[1] = zerosVec;
    opLevelVec[2] = zerosVec;
    opLevelVec[3] = zerosVec;
    opOutVec[0] = zerosVec;
    opOutVec[1] = zerosVec;
    opOutVec[2] = zerosVec;
    opOutVec[3] = zerosVec;
    opAuxOutVec[0] = zerosVec;
    opAuxOutVec[1] = zerosVec;
    opAuxOutVec[2] = zerosVec;
    opAuxOutVec[3] = zerosVec;
    op1EocVec = zerosVec;
    op2EocVec = zerosVec;
    op3EocVec = zerosVec;
    op4EocVec = zerosVec;
    opExtFMVec[0] = zerosVec;
    opExtFMVec[1] = zerosVec;
    opExtFMVec[2] = zerosVec;
    opExtFMVec[3] = zerosVec;
    opExtSyncVec[0] = zerosVec;
    opExtSyncVec[1] = zerosVec;
    opExtSyncVec[2] = zerosVec;
    opExtSyncVec[3] = zerosVec;
    opSyncEnableVec[0] = zerosVec;
    opSyncEnableVec[1] = zerosVec;
    opSyncEnableVec[2] = zerosVec;
    opSyncEnableVec[3] = zerosVec;
    setAlgorithm(0);
}

void DexterCore::process() {
    calcOpLevels();

    opAuxOutVec[0] = op[0].getOutput();
    opAuxOutVec[1] = op[1].getOutput();
    opAuxOutVec[2] = op[2].getOutput();
    opAuxOutVec[3] = op[3].getOutput();

    opOutVec[0] = _mm_mul_ps(opAuxOutVec[0], opLevelVec[0]);
    opOutVec[1] = _mm_mul_ps(opAuxOutVec[1], opLevelVec[1]);
    opOutVec[2] = _mm_mul_ps(opAuxOutVec[2], opLevelVec[2]);
    opOutVec[3] = _mm_mul_ps(opAuxOutVec[3], opLevelVec[3]);

    switch (algorithm) {
        case 0: doAlgorithm0(); break;
        case 1: doAlgorithm1(); break;
        case 2: doAlgorithm2(); break;
        case 3: doAlgorithm3(); break;
        case 4: doAlgorithm4(); break;
        case 5: doAlgorithm5(); break;
        case 6: doAlgorithm6(); break;
        case 7: doAlgorithm7(); break;
        case 8: doAlgorithm8(); break;
        case 9: doAlgorithm9(); break;
        case 10: doAlgorithm10(); break;
        case 11: doAlgorithm11(); break;
        case 12: doAlgorithm12(); break;
        case 13: doAlgorithm13(); break;
        case 14: doAlgorithm14(); break;
        case 15: doAlgorithm15(); break;
        case 16: doAlgorithm16(); break;
        case 17: doAlgorithm17(); break;
        case 18: doAlgorithm18(); break;
        case 19: doAlgorithm19(); break;
        case 20: doAlgorithm20(); break;
        case 21: doAlgorithm21(); break;
        case 22: doAlgorithm22();
    }

    op[0].__inputPhase = _mm_add_ps(op[0].__inputPhase, opExtFMVec[0]);
    op[1].__inputPhase = _mm_add_ps(op[1].__inputPhase, opExtFMVec[1]);
    op[2].__inputPhase = _mm_add_ps(op[2].__inputPhase, opExtFMVec[2]);
    op[3].__inputPhase = _mm_add_ps(op[3].__inputPhase, opExtFMVec[3]);

    // Sync
    for (size_t i = 0; i < kNumOperators; ++i) {
        opSyncIn[i] = zerosVec;
    }

    for (size_t i = 0; i < kNumOperators; ++i) {
        opSyncSignalVec[i] = op[i].getEOCPulse();
        opSyncIn[0] = _mm_add_ps(opSyncIn[0], _mm_mul_ps(opSyncSignalVec[i], matrixVec[i][0]));
        opSyncIn[1] = _mm_add_ps(opSyncIn[1], _mm_mul_ps(opSyncSignalVec[i], matrixVec[i][1]));
        opSyncIn[2] = _mm_add_ps(opSyncIn[2], _mm_mul_ps(opSyncSignalVec[i], matrixVec[i][2]));
        opSyncIn[3] = _mm_add_ps(opSyncIn[3], _mm_mul_ps(opSyncSignalVec[i], matrixVec[i][3]));
    }

    if (opSyncSource == NEIGHBOUR_SYNC_SOURCE) {
        opSyncIn[0] = opSyncSignalVec[1];
        opSyncIn[1] = opSyncSignalVec[2];
        opSyncIn[2] = opSyncSignalVec[3];
        opSyncIn[3] = zerosVec;
    }

    for (size_t i = 0; i < kNumOperators; ++i) {
        opSyncIn[i] = _mm_mul_ps(opSyncIn[i], opSyncEnableVec[i]);
        opSyncIn[i] = _mm_add_ps(opSyncIn[i], opExtSyncVec[i]);
        if (weakSync[i]) {
            opSyncIn[i] = _mm_and_ps(opSyncIn[i], _mm_cmplt_ps(op[i].getPhasor(), _mm_set1_ps(0.25f)));
        }
        op[i].sync(opSyncIn[i]);
    }

    op[3].tick();
    op[2].tick();
    op[1].tick();
    op[0].tick();
}

void DexterCore::resetPhase() {
    op[0].resetPhase();
    op[1].resetPhase();
    op[2].resetPhase();
    op[3].resetPhase();
}

__m128 DexterCore::getMainOutput() const {
    return _mm_mul_ps(mainColVec, aOutLevelVec);
}

__m128 DexterCore::getBOutput() const {
    return _mm_mul_ps(bColVec, bOutLevelVec);
}

__m128 DexterCore::getOpOutput(size_t opNum) const {
    if (outputOpPreFade[opNum]) {
        return _mm_mul_ps(opAuxOutVec[opNum], fiveVec);
    }
    return _mm_mul_ps(opOutVec[opNum], fiveVec);
}

void DexterCore::setAlgorithm(int newAlgorithm) {
    if (newAlgorithm != algorithm) {
        algorithm = newAlgorithm;
        clearMatrix();
        switch (algorithm) {
            case 0: setMatrixAlgo0(); break;
            case 1: setMatrixAlgo1(); break;
            case 2: setMatrixAlgo2(); break;
            case 3: setMatrixAlgo3(); break;
            case 4: setMatrixAlgo4(); break;
            case 5: setMatrixAlgo5(); break;
            case 6: setMatrixAlgo6(); break;
            case 7: setMatrixAlgo7(); break;
            case 8: setMatrixAlgo8(); break;
            case 9: setMatrixAlgo9(); break;
            case 10: setMatrixAlgo10(); break;
            case 11: setMatrixAlgo11(); break;
            case 12: setMatrixAlgo12(); break;
            case 13: setMatrixAlgo13(); break;
            case 14: setMatrixAlgo14(); break;
            case 15: setMatrixAlgo15(); break;
            case 16: setMatrixAlgo16(); break;
            case 17: setMatrixAlgo17(); break;
            case 18: setMatrixAlgo18(); break;
            case 19: setMatrixAlgo19(); break;
            case 20: setMatrixAlgo20(); break;
            case 21: setMatrixAlgo21(); break;
            case 22: setMatrixAlgo22(); break;
            default: setMatrixAlgo22();
        }
    }
}

void DexterCore::setFeedback(float feedback) {
    op3FeedbackDepthVec = _mm_set1_ps(feedback);
}

void DexterCore::externalFM(size_t opNum, float extFM) {
    opExtFMVec[opNum] = _mm_set1_ps(extFM);
}

void DexterCore::externalSync(size_t opNum, float extSync) {
    if (extSync > 0 && !extSyncing) {
        extSyncing = true;
        opExtSyncVec[opNum] = onesVec;
    }
    else {
        opExtSyncVec[opNum] = zerosVec;
    }
    if (extSync <= 0) {
        extSyncing = false;
    }

}

void DexterCore::setFrequency(size_t opNum, float frequency) {
    op[opNum].setFrequency(frequency);
}

void DexterCore::_mm_setFrequency(size_t opNum, const __m128& frequency) {
    op[opNum].setFrequency(frequency);
}

void DexterCore::setWavebank(size_t opNum, int bankNum) {
    float* wavebank;
    int32_t numWaves;
    int32_t tableSize;

    wavebank = dexterWavetables[bankNum];
    numWaves = wavetable_sizes[bankNum];
    tableSize = wavetable_lengths[bankNum];
    op[opNum].setWavebank(wavebank, numWaves, tableSize);
}

void DexterCore::setWavePosition(size_t opNum, float position) {
    float scanPosition = position * ((float)op[opNum].getNumwaves() - 1.f);
    op[opNum].setScanPosition(scanPosition);
}

void DexterCore::setShape(size_t opNum, float shape) {
    op[opNum].setShape(shape);
}

void DexterCore::setLevel(size_t opNum, float level) {
    inLevels[opNum] = level;
}

void DexterCore::setOpPreFade(size_t opNum, bool opPreFade) {
    outputOpPreFade[opNum] = opPreFade;
}

void DexterCore::setBrightness(float newBrightness) {
    brightness = newBrightness;
}

void DexterCore::setSyncMode(size_t opNum, int syncMode) {
    op[opNum].setSyncMode(syncMode);
}

void DexterCore::setSyncSource(OpSyncSource newOpSyncSource) {
    opSyncSource = newOpSyncSource;
}

void DexterCore::enableSync(size_t opNum, bool enableSync) {
    op[opNum].enableSync(enableSync);
}

void DexterCore::enableIntSync(size_t opNum, bool enableSync) {
    if (enableSync) {
        op[opNum].enableSync(enableSync);
        opSyncEnableVec[opNum] = onesVec;
    }
    else {
        opSyncEnableVec[opNum] = zerosVec;
    }
}

void DexterCore::enableWeakSync(size_t opNum, bool weakEnable) {
    // TODO: Handle weak sync in this class rather than in QuadOsc
    //op[opNum].enableWeakSync(weakEnable);
    weakSync[opNum] = weakEnable;
}

void DexterCore::setShapeMode(size_t opNum, int shapeMode) {
    op[opNum].setShapeMethod(shapeMode);
}

void DexterCore::setPMPostShape(size_t opNum, bool PMPostShape) {
    op[opNum].setPMPostShape(PMPostShape);
}

void DexterCore::setSampleRate(float sampleRate) {
    op[0].setSampleRate(sampleRate);
    op[1].setSampleRate(sampleRate);
    op[2].setSampleRate(sampleRate);
    op[3].setSampleRate(sampleRate);
}

void DexterCore::calcOpLevels() {
    int _opMask = 1;
    for (size_t i = 0; i < kNumOperators; ++i) {
        opLevels[i] = inLevels[i];
        if (_opMask & brightnessMask) {
            opLevels[i] += brightness;
        }
        if (opLevels[i] < 0.f) {
            opLevels[i] = 0.f;
        }
        else if(opLevels[i] > 1.f) {
            opLevels[i] = 1.f;
        }
        opLevelVec[i] = _mm_set1_ps(opLevels[i]);
        _opMask <<= 1;
    }
}

void DexterCore::clearMatrix() {
    for (size_t i = 0; i < MatrixRows::NUM_ROWS; ++i) {
        for (size_t j = 0; j < MatrixColumns::NUM_COLS; ++j) {
            matrixVec[i][j] = zerosVec;
        }
    }
}

void DexterCore::setMatrixAlgo0() {
    //
    // [3]-->[2]-->[1]-->[0]--> A & B
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo1() {
    //
    // [3]-->+-->[1]-->[0]--> A & B
    // [2]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_1_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo2() {
    //
    // [2]-->[1]-->+-->[0]--> out
    //       [3]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo3() {
    //
    // [3]-->[2]-->+-->[0]--> out
    //       [1]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo4() {
    //
    //       |-->[2]-->|
    // [3]-->|         +-->[0]--> out
    //       |-->[1]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo5() {
    //
    //             |-->[1]-->|
    // [3]-->[2]-->|         +--> out
    //             |-->[0]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[1];
    bOutLevelVec = outputLevelsVec[1];
    brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo6() {
    //
    // [3]-->[2]-->[1]-->+--> outputs
    //             [0]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[1];
    bOutLevelVec = outputLevelsVec[1];
    brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo7() {
    //
    // [3]-->|
    // [2]-->+-->[0]--> out
    // [1]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_0_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo8() {
    //
    // [3]-->[2]-->|
    //             +--> out
    // [1]-->[0]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[1];
    bOutLevelVec = outputLevelsVec[1];
    brightnessMask = 0x0A;
}

void DexterCore::setMatrixAlgo9() {
    //
    //       |-->[2]-->|
    // [3]-->|-->[1]-->+--> out
    //       |-->[0]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[2];
    bOutLevelVec = outputLevelsVec[2];
    brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo10() {
    //
    // [3]-->[2]-->|
    //       [1]-->+--> out
    //       [0]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[2];
    bOutLevelVec = outputLevelsVec[2];
    brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo11() {
    // All going to all outputs
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][B_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[3];
    bOutLevelVec = outputLevelsVec[3];
    brightnessMask = 0x00;
}

void DexterCore::setMatrixAlgo12() {
    //
    // [1]-->[0]--> A
    //
    // [3]-->[2]--> B
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0A;
}

void DexterCore::setMatrixAlgo13() {
    //
    // [2]-->[1]-->[0]--> A
    //
    // [3]--> B
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][B_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x06;
}

void DexterCore::setMatrixAlgo14() {
    //
    // [3]-->[2]-->[1]--> A
    //
    // [0]--> B
    //
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo15() {
    //
    // [3]--> A
    //
    // [2]-->[1]-->[0]--> B
    //
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][MAIN_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo16() {
    //
    // [0]--> A
    //
    // [3]-->[2]-->[1]--> B
    //
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][OP_1_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo17() {
    //
    // [1]-->[0]--> A
    //
    // [2]-->+--> B
    // [3]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][OP_0_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][B_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[1];
    brightnessMask = 0x02;
}

void DexterCore::setMatrixAlgo18() {
    //
    // [3]-->[2]--> A
    //
    // [0]-->+--> B
    // [1]-->|
    //
    matrixVec[OP_2_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[1];
    brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo19() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [3]-->[2]--> B
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][OP_2_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[1];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo20() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [2]-->+--> B
    // [3]-->|
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][B_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[1];
    bOutLevelVec = outputLevelsVec[1];
    brightnessMask = 0x00;
}

void DexterCore::setMatrixAlgo21() {
    //
    // [0]-->|
    // [1]-->+--> A
    // [2]-->|
    //
    // [3]--> B
    //
    matrixVec[OP_0_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][MAIN_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][B_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[2];
    bOutLevelVec = outputLevelsVec[0];
    brightnessMask = 0x00;
}

void DexterCore::setMatrixAlgo22() {
    //
    // [3]-->+--> A
    //
    // [0]-->|
    // [1]-->+--> B
    // [2]-->|
    //
    matrixVec[OP_0_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_1_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_2_ROW][B_OUT_COL] = onesVec;
    matrixVec[OP_3_ROW][MAIN_OUT_COL] = onesVec;
    aOutLevelVec = outputLevelsVec[0];
    bOutLevelVec = outputLevelsVec[2];
    brightnessMask = 0x00;
}

void DexterCore::doAlgorithm0() {
    //
    // [3]-->[2]-->[1]-->[0]--> A & B
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm1() {
    //
    // [3]-->+-->[1]-->[0]--> A & B
    // [2]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(_mm_add_ps(opOutVec[3], opOutVec[2]),
                                     _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm2() {
    //
    // [2]-->[1]-->+-->[0]--> out
    //       [3]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(_mm_add_ps(opOutVec[2], opOutVec[1]),
                                     _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm3() {
    //
    // [3]-->[2]-->+-->[0]--> out
    //       [1]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(_mm_add_ps(opOutVec[2], opOutVec[1]),
                                     _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm4() {
    //
    //       |-->[2]-->|
    // [3]-->|         +-->[0]--> out
    //       |-->[1]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(_mm_add_ps(opOutVec[2], opOutVec[1]),
                                     _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm5() {
    //
    //             |-->[1]-->|
    // [3]-->[2]-->|         +--> out
    //             |-->[0]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    auto op0and1Sum = _mm_add_ps(opOutVec[0], opOutVec[1]);
    mainColVec = op0and1Sum;
    bColVec = op0and1Sum;
}

void DexterCore::doAlgorithm6() {
    //
    // [3]-->[2]-->[1]-->+--> outputs
    //             [0]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    auto op0and1Sum = _mm_add_ps(opOutVec[0], opOutVec[1]);
    mainColVec = op0and1Sum;
    bColVec = op0and1Sum;
}

void DexterCore::doAlgorithm7() {
    //
    // [3]-->|
    // [2]-->+-->[0]--> out
    // [1]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));

    auto op123Sum = _mm_add_ps(_mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f)),
                               _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f)));
    op123Sum = _mm_add_ps(op123Sum, _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f)));
    op[0].__inputPhase = _mm_mul_ps(op123Sum, _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm8() {
    //
    // [3]-->[2]-->|
    //             +--> out
    // [1]-->[0]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f));
    mainColVec = _mm_add_ps(opOutVec[2], opOutVec[0]);
    bColVec = mainColVec;
}

void DexterCore::doAlgorithm9() {
    //
    //       |-->[2]-->|
    // [3]-->|-->[1]-->+--> out
    //       |-->[0]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    mainColVec = _mm_add_ps(_mm_add_ps(opOutVec[2], opOutVec[1]), opOutVec[0]);
    bColVec = mainColVec;
}

void DexterCore::doAlgorithm10() {
    //
    // [3]-->[2]-->|
    //       [1]-->+--> out
    //       [0]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    mainColVec = _mm_add_ps(_mm_add_ps(opOutVec[2], opOutVec[1]), opOutVec[0]);
    bColVec = mainColVec;
}

void DexterCore::doAlgorithm11() {
    // All going to all outputs
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    mainColVec = _mm_add_ps(opOutVec[3], opOutVec[2]);
    mainColVec = _mm_add_ps(mainColVec, opOutVec[1]);
    mainColVec = _mm_add_ps(mainColVec, opOutVec[0]);
    bColVec = mainColVec;
}

void DexterCore::doAlgorithm12() {
    //
    // [1]-->[0]--> A
    //
    // [3]-->[2]--> B
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
    bColVec = opOutVec[2];
}

void DexterCore::doAlgorithm13() {
    //
    // [2]-->[1]-->[0]--> A
    //
    // [3]--> B
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    bColVec = opOutVec[3];

    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];
}

void DexterCore::doAlgorithm14() {
    //
    // [3]-->[2]-->[1]--> A
    //
    // [0]--> B
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    mainColVec = opOutVec[1];
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm15() {
    //
    // [3]--> A
    //
    // [2]-->[1]-->[0]--> B
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    mainColVec = opOutVec[3];

    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    op[0].__inputPhase = _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f));
    bColVec = opOutVec[0];
}

void DexterCore::doAlgorithm16() {
    //
    // [0]--> A
    //
    // [3]-->[2]-->[1]--> B
    //
    mainColVec = opOutVec[0];

    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    op[1].__inputPhase = _mm_mul_ps(opOutVec[2], _mm_set1_ps(2.5f));
    bColVec = opOutVec[1];
}

void DexterCore::doAlgorithm17() {
    //
    // [1]-->[0]--> A
    //
    // [2]-->+--> B
    // [3]-->|
    //
    op[0].__inputPhase = _mm_mul_ps(opOutVec[1], _mm_set1_ps(2.5f));
    mainColVec = opOutVec[0];

    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    bColVec = _mm_add_ps(opOutVec[3], opOutVec[2]);
}

void DexterCore::doAlgorithm18() {
    //
    // [3]-->[2]--> A
    //
    // [0]-->+--> B
    // [1]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    mainColVec = opOutVec[2];

    bColVec = _mm_add_ps(opOutVec[1], opOutVec[0]);
}

void DexterCore::doAlgorithm19() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [3]-->[2]--> B
    //
    mainColVec = _mm_add_ps(opOutVec[1], opOutVec[0]);

    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    op[2].__inputPhase = _mm_mul_ps(opOutVec[3], _mm_set1_ps(2.5f));
    bColVec = opOutVec[2];
}

void DexterCore::doAlgorithm20() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [2]-->+--> B
    // [3]-->|
    //
    mainColVec = _mm_add_ps(opOutVec[1], opOutVec[0]);

    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    bColVec = _mm_add_ps(opOutVec[3], opOutVec[2]);
}

void DexterCore::doAlgorithm21() {
    //
    // [0]-->|
    // [1]-->+--> A
    // [2]-->|
    //
    // [3]--> B
    //
    mainColVec = _mm_add_ps(opOutVec[2], _mm_add_ps(opOutVec[1], opOutVec[0]));

    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    bColVec = opOutVec[3];
}

void DexterCore::doAlgorithm22() {
    //
    // [3]-->+--> A
    //
    // [0]-->|
    // [1]-->+--> B
    // [2]-->|
    //
    auto op3Out = _mm_mul_ps(op[3].getOutput(), op3FeedbackDepthVec);
    op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    mainColVec = opOutVec[3];

    bColVec = _mm_add_ps(opOutVec[2], _mm_add_ps(opOutVec[1], opOutVec[0]));
}

