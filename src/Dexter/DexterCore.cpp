//
// Osc4Core_SIMD.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 18/3/2018
//

#include "DexterCore.hpp"

DexterCore::DexterCore() {
    _algorithm = -1;
    setWavebank(0, 0);
    setWavebank(1, 0);
    setWavebank(2, 0);
    setWavebank(3, 0);
    setWavePosition(0, 0.f);
    setWavePosition(1, 0.f);
    setWavePosition(2, 0.f);
    setWavePosition(3, 0.f);
    __ones = _mm_set1_ps(1.f);
    __zeros = _mm_set1_ps(0.f);
    __five = _mm_set1_ps(5.f);
    __outputLevels[0] = _mm_set1_ps(5.f);
    __outputLevels[1] = _mm_set1_ps(3.3f);
    __outputLevels[2] = _mm_set1_ps(2.5f);
    __outputLevels[3] = _mm_set1_ps(2.f);
    __opLevel[0] = __zeros;
    __opLevel[1] = __zeros;
    __opLevel[2] = __zeros;
    __opLevel[3] = __zeros;
    __opOut[0] = __zeros;
    __opOut[1] = __zeros;
    __opOut[2] = __zeros;
    __opOut[3] = __zeros;
    __opAuxOut[0] = __zeros;
    __opAuxOut[1] = __zeros;
    __opAuxOut[2] = __zeros;
    __opAuxOut[3] = __zeros;
    __op1Eoc = __zeros;
    __op2Eoc = __zeros;
    __op3Eoc = __zeros;
    __op4Eoc = __zeros;
    __opExtFM[0] = __zeros;
    __opExtFM[1] = __zeros;
    __opExtFM[2] = __zeros;
    __opExtFM[3] = __zeros;
    __opExtSync[0] = __zeros;
    __opExtSync[1] = __zeros;
    __opExtSync[2] = __zeros;
    __opExtSync[3] = __zeros;
    __opSyncEnable[0] = __zeros;
    __opSyncEnable[1] = __zeros;
    __opSyncEnable[2] = __zeros;
    __opSyncEnable[3] = __zeros;
    setAlgorithm(0);
}

void DexterCore::process() {
    calcOpLevels();

    __opAuxOut[0] = _op[0].getOutput();
    __opAuxOut[1] = _op[1].getOutput();
    __opAuxOut[2] = _op[2].getOutput();
    __opAuxOut[3] = _op[3].getOutput();

    __opOut[0] = _mm_mul_ps(__opAuxOut[0], __opLevel[0]);
    __opOut[1] = _mm_mul_ps(__opAuxOut[1], __opLevel[1]);
    __opOut[2] = _mm_mul_ps(__opAuxOut[2], __opLevel[2]);
    __opOut[3] = _mm_mul_ps(__opAuxOut[3], __opLevel[3]);

    switch (_algorithm) {
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

    _op[0].__inputPhase = _mm_add_ps(_op[0].__inputPhase, __opExtFM[0]);
    _op[1].__inputPhase = _mm_add_ps(_op[1].__inputPhase, __opExtFM[1]);
    _op[2].__inputPhase = _mm_add_ps(_op[2].__inputPhase, __opExtFM[2]);
    _op[3].__inputPhase = _mm_add_ps(_op[3].__inputPhase, __opExtFM[3]);

    // Sync
    for (size_t i = 0; i < kNumOperators; ++i) {
        __opSyncIn[i] = __zeros;
    }

    for (size_t i = 0; i < kNumOperators; ++i) {
        __opSyncSignal[i] = _op[i].getEOCPulse();
        __opSyncIn[0] = _mm_add_ps(__opSyncIn[0], _mm_mul_ps(__opSyncSignal[i], __matrix[i][0]));
        __opSyncIn[1] = _mm_add_ps(__opSyncIn[1], _mm_mul_ps(__opSyncSignal[i], __matrix[i][1]));
        __opSyncIn[2] = _mm_add_ps(__opSyncIn[2], _mm_mul_ps(__opSyncSignal[i], __matrix[i][2]));
        __opSyncIn[3] = _mm_add_ps(__opSyncIn[3], _mm_mul_ps(__opSyncSignal[i], __matrix[i][3]));
    }

    if (_opSyncSource == NEIGHBOUR_SYNC_SOURCE) {
        __opSyncIn[0] = __opSyncSignal[1];
        __opSyncIn[1] = __opSyncSignal[2];
        __opSyncIn[2] = __opSyncSignal[3];
        __opSyncIn[3] = __zeros;
    }

    for (size_t i = 0; i < kNumOperators; ++i) {
        __opSyncIn[i] = _mm_mul_ps(__opSyncIn[i], __opSyncEnable[i]);
        __opSyncIn[i] = _mm_add_ps(__opSyncIn[i], __opExtSync[i]);
        if (_weakSync[i]) {
            __opSyncIn[i] = _mm_and_ps(__opSyncIn[i], _mm_cmplt_ps(_op[i].getPhasor(), _mm_set1_ps(0.25f)));
        }
        _op[i].sync(__opSyncIn[i]);
    }

    _op[3].tick();
    _op[2].tick();
    _op[1].tick();
    _op[0].tick();
}

void DexterCore::resetPhase() {
    _op[0].resetPhase();
    _op[1].resetPhase();
    _op[2].resetPhase();
    _op[3].resetPhase();
}

__m128 DexterCore::getMainOutput() const {
    return _mm_mul_ps(__mainCol, __aOutLevel);
}

__m128 DexterCore::getBOutput() const {
    return _mm_mul_ps(__bCol, __bOutLevel);
}

__m128 DexterCore::getOpOutput(size_t opNum) const {
    if (_opPreFade[opNum]) {
        return _mm_mul_ps(__opAuxOut[opNum], __five);
    }
    return _mm_mul_ps(__opOut[opNum], __five);
}

void DexterCore::setAlgorithm(int newAlgorithm) {
    if (newAlgorithm != _algorithm) {
        _algorithm = newAlgorithm;
        clearMatrix();
        switch (_algorithm) {
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
    __op3FeedbackDepth = _mm_set1_ps(feedback);
}

void DexterCore::externalFM(size_t opNum, float extFM) {
    __opExtFM[opNum] = _mm_set1_ps(extFM);
}

void DexterCore::externalSync(size_t opNum, float extSync) {
    if (extSync > 0 && !extSyncing) {
        extSyncing = true;
        __opExtSync[opNum] = __ones;
    }
    else {
        __opExtSync[opNum] = __zeros;
    }
    if (extSync <= 0) {
        extSyncing = false;
    }

}

void DexterCore::setFrequency(size_t opNum, float frequency) {
    _op[opNum].setFrequency(frequency);
}

void DexterCore::_mm_setFrequency(size_t opNum, const __m128& frequency) {
    _op[opNum].setFrequency(frequency);
}

void DexterCore::setWavebank(size_t opNum, int bankNum) {
    float* wavebank;
    int32_t numWaves;
    int32_t tableSize;

    wavebank = dexterWavetables[bankNum];
    numWaves = wavetable_sizes[bankNum];
    tableSize = wavetable_lengths[bankNum];
    _op[opNum].setWavebank(wavebank, numWaves, tableSize);
}

void DexterCore::setWavePosition(size_t opNum, float position) {
    float scanPosition = position * ((float)_op[opNum].getNumwaves() - 1.f);
    _op[opNum].setScanPosition(scanPosition);
}

void DexterCore::setShape(size_t opNum, float shape) {
    _op[opNum].setShape(shape);
}

void DexterCore::setLevel(size_t opNum, float level) {
    _inLevels[opNum] = level;
}

void DexterCore::setOpPreFade(size_t opNum, bool opPreFade) {
    _opPreFade[opNum] = opPreFade;
}

void DexterCore::setBrightness(float brightness) {
    _brightness = brightness;
}

void DexterCore::setSyncMode(size_t opNum, int syncMode) {
    _op[opNum].setSyncMode(syncMode);
}

void DexterCore::setSyncSource(OpSyncSource opSyncSource) {
    _opSyncSource = opSyncSource;
}

void DexterCore::enableSync(size_t opNum, bool enableSync) {
    _op[opNum].enableSync(enableSync);
}

void DexterCore::enableIntSync(size_t opNum, bool enableSync) {
    if (enableSync) {
        _op[opNum].enableSync(enableSync);
        __opSyncEnable[opNum] = __ones;
    }
    else {
        __opSyncEnable[opNum] = __zeros;
    }
}

void DexterCore::enableWeakSync(size_t opNum, bool weakEnable) {
    // TODO: Handle weak sync in this class rather than in QuadOsc
    //_op[opNum].enableWeakSync(weakEnable);
    _weakSync[opNum] = weakEnable;
}

void DexterCore::setShapeMode(size_t opNum, int shapeMode) {
    _op[opNum].setShapeMethod(shapeMode);
}

void DexterCore::setPMPostShape(size_t opNum, bool PMPostShape) {
    _op[opNum].setPMPostShape(PMPostShape);
}

void DexterCore::setSampleRate(float sampleRate) {
    _op[0].setSampleRate(sampleRate);
    _op[1].setSampleRate(sampleRate);
    _op[2].setSampleRate(sampleRate);
    _op[3].setSampleRate(sampleRate);
}

void DexterCore::calcOpLevels() {
    int _opMask = 1;
    for (size_t i = 0; i < kNumOperators; ++i) {
        _opLevels[i] = _inLevels[i];
        if (_opMask & _brightnessMask) {
            _opLevels[i] += _brightness;
        }
        if (_opLevels[i] < 0.f) {
            _opLevels[i] = 0.f;
        }
        else if(_opLevels[i] > 1.f) {
            _opLevels[i] = 1.f;
        }
        __opLevel[i] = _mm_set1_ps(_opLevels[i]);
        _opMask <<= 1;
    }
}

void DexterCore::clearMatrix() {
    for (size_t i = 0; i < MatrixRows::NUM_ROWS; ++i) {
        for (size_t j = 0; j < MatrixColumns::NUM_COLS; ++j) {
            __matrix[i][j] = __zeros;
        }
    }
}

void DexterCore::setMatrixAlgo0() {
    //
    // [3]-->[2]-->[1]-->[0]--> A & B
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo1() {
    //
    // [3]-->+-->[1]-->[0]--> A & B
    // [2]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_1_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo2() {
    //
    // [2]-->[1]-->+-->[0]--> out
    //       [3]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_3_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo3() {
    //
    // [3]-->[2]-->+-->[0]--> out
    //       [1]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_0_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo4() {
    //
    //       |-->[2]-->|
    // [3]-->|         +-->[0]--> out
    //       |-->[1]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_0_COL] = __ones;
    __matrix[OP_3_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo5() {
    //
    //             |-->[1]-->|
    // [3]-->[2]-->|         +--> out
    //             |-->[0]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __matrix[OP_2_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[1];
    __bOutLevel = __outputLevels[1];
    _brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo6() {
    //
    // [3]-->[2]-->[1]-->+--> outputs
    //             [0]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[1];
    __bOutLevel = __outputLevels[1];
    _brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo7() {
    //
    // [3]-->|
    // [2]-->+-->[0]--> out
    // [1]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_0_COL] = __ones;
    __matrix[OP_3_ROW][OP_0_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0E;
}

void DexterCore::setMatrixAlgo8() {
    //
    // [3]-->[2]-->|
    //             +--> out
    // [1]-->[0]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[1];
    __bOutLevel = __outputLevels[1];
    _brightnessMask = 0x0A;
}

void DexterCore::setMatrixAlgo9() {
    //
    //       |-->[2]-->|
    // [3]-->|-->[1]-->+--> out
    //       |-->[0]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][OP_0_COL] = __ones;
    __matrix[OP_3_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[2];
    __bOutLevel = __outputLevels[2];
    _brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo10() {
    //
    // [3]-->[2]-->|
    //       [1]-->+--> out
    //       [0]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[2];
    __bOutLevel = __outputLevels[2];
    _brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo11() {
    // All going to all outputs
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_3_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][B_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[3];
    __bOutLevel = __outputLevels[3];
    _brightnessMask = 0x00;
}

void DexterCore::setMatrixAlgo12() {
    //
    // [1]-->[0]--> A
    //
    // [3]-->[2]--> B
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0A;
}

void DexterCore::setMatrixAlgo13() {
    //
    // [2]-->[1]-->[0]--> A
    //
    // [3]--> B
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][B_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x06;
}

void DexterCore::setMatrixAlgo14() {
    //
    // [3]-->[2]-->[1]--> A
    //
    // [0]--> B
    //
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo15() {
    //
    // [3]--> A
    //
    // [2]-->[1]-->[0]--> B
    //
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][MAIN_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo16() {
    //
    // [0]--> A
    //
    // [3]-->[2]-->[1]--> B
    //
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __matrix[OP_2_ROW][OP_1_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x0C;
}

void DexterCore::setMatrixAlgo17() {
    //
    // [1]-->[0]--> A
    //
    // [2]-->+--> B
    // [3]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][OP_0_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][B_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[1];
    _brightnessMask = 0x02;
}

void DexterCore::setMatrixAlgo18() {
    //
    // [3]-->[2]--> A
    //
    // [0]-->+--> B
    // [1]-->|
    //
    __matrix[OP_2_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[1];
    _brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo19() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [3]-->[2]--> B
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][OP_2_COL] = __ones;
    __aOutLevel = __outputLevels[1];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x08;
}

void DexterCore::setMatrixAlgo20() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [2]-->+--> B
    // [3]-->|
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][B_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[1];
    __bOutLevel = __outputLevels[1];
    _brightnessMask = 0x00;
}

void DexterCore::setMatrixAlgo21() {
    //
    // [0]-->|
    // [1]-->+--> A
    // [2]-->|
    //
    // [3]--> B
    //
    __matrix[OP_0_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_1_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_2_ROW][MAIN_OUT_COL] = __ones;
    __matrix[OP_3_ROW][B_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[2];
    __bOutLevel = __outputLevels[0];
    _brightnessMask = 0x00;
}

void DexterCore::setMatrixAlgo22() {
    //
    // [3]-->+--> A
    //
    // [0]-->|
    // [1]-->+--> B
    // [2]-->|
    //
    __matrix[OP_0_ROW][B_OUT_COL] = __ones;
    __matrix[OP_1_ROW][B_OUT_COL] = __ones;
    __matrix[OP_2_ROW][B_OUT_COL] = __ones;
    __matrix[OP_3_ROW][MAIN_OUT_COL] = __ones;
    __aOutLevel = __outputLevels[0];
    __bOutLevel = __outputLevels[2];
    _brightnessMask = 0x00;
}

void DexterCore::doAlgorithm0() {
    //
    // [3]-->[2]-->[1]-->[0]--> A & B
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(__opOut[3], _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(__opOut[1], _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm1() {
    //
    // [3]-->+-->[1]-->[0]--> A & B
    // [2]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(_mm_add_ps(__opOut[3], __opOut[2]),
                                     _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(__opOut[1], _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm2() {
    //
    // [2]-->[1]-->+-->[0]--> out
    //       [3]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(_mm_add_ps(__opOut[2], __opOut[1]),
                                     _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm3() {
    //
    // [3]-->[2]-->+-->[0]--> out
    //       [1]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(_mm_add_ps(__opOut[2], __opOut[1]),
                                     _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm4() {
    //
    //       |-->[2]-->|
    // [3]-->|         +-->[0]--> out
    //       |-->[1]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(_mm_add_ps(__opOut[2], __opOut[1]),
                                     _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm5() {
    //
    //             |-->[1]-->|
    // [3]-->[2]-->|         +--> out
    //             |-->[0]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    auto op0and1Sum = _mm_add_ps(__opOut[0], __opOut[1]);
    __mainCol = op0and1Sum;
    __bCol = op0and1Sum;
}

void DexterCore::doAlgorithm6() {
    //
    // [3]-->[2]-->[1]-->+--> outputs
    //             [0]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    auto op0and1Sum = _mm_add_ps(__opOut[0], __opOut[1]);
    __mainCol = op0and1Sum;
    __bCol = op0and1Sum;
}

void DexterCore::doAlgorithm7() {
    //
    // [3]-->|
    // [2]-->+-->[0]--> out
    // [1]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));

    auto op123Sum = _mm_add_ps(_mm_mul_ps(__opOut[3], _mm_set1_ps(2.5f)),
                               _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f)));
    op123Sum = _mm_add_ps(op123Sum, _mm_mul_ps(__opOut[1], _mm_set1_ps(2.5f)));
    _op[0].__inputPhase = _mm_mul_ps(op123Sum, _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm8() {
    //
    // [3]-->[2]-->|
    //             +--> out
    // [1]-->[0]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(__opOut[1], _mm_set1_ps(2.5f));
    __mainCol = _mm_add_ps(__opOut[2], __opOut[0]);
    __bCol = __mainCol;
}

void DexterCore::doAlgorithm9() {
    //
    //       |-->[2]-->|
    // [3]-->|-->[1]-->+--> out
    //       |-->[0]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __mainCol = _mm_add_ps(_mm_add_ps(__opOut[2], __opOut[1]), __opOut[0]);
    __bCol = __mainCol;
}

void DexterCore::doAlgorithm10() {
    //
    // [3]-->[2]-->|
    //       [1]-->+--> out
    //       [0]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __mainCol = _mm_add_ps(_mm_add_ps(__opOut[2], __opOut[1]), __opOut[0]);
    __bCol = __mainCol;
}

void DexterCore::doAlgorithm11() {
    // All going to all outputs
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __mainCol = _mm_add_ps(__opOut[3], __opOut[2]);
    __mainCol = _mm_add_ps(__mainCol, __opOut[1]);
    __mainCol = _mm_add_ps(__mainCol, __opOut[0]);
    __bCol = __mainCol;
}

void DexterCore::doAlgorithm12() {
    //
    // [1]-->[0]--> A
    //
    // [3]-->[2]--> B
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(__opOut[1], _mm_set1_ps(2.5f));
    __mainCol = __opOut[2];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm13() {
    //
    // [2]-->[1]-->[0]--> A
    //
    // [3]--> B
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    __bCol = __opOut[3];

    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    _op[0].__inputPhase = _mm_mul_ps(__opOut[1], _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];
}

void DexterCore::doAlgorithm14() {
    //
    // [3]-->[2]-->[1]--> A
    //
    // [0]--> B
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    __mainCol = __opOut[2];
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm15() {
    //
    // [3]--> A
    //
    // [2]-->[1]-->[0]--> B
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __mainCol = __opOut[3];

    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    __bCol = __opOut[0];
}

void DexterCore::doAlgorithm16() {
    //
    // [0]--> A
    //
    // [3]-->[2]-->[1]--> B
    //
    __mainCol = __opOut[0];

    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[1].__inputPhase = _mm_mul_ps(__opOut[2], _mm_set1_ps(2.5f));
    __bCol = __opOut[1];
}

void DexterCore::doAlgorithm17() {
    //
    // [1]-->[0]--> A
    //
    // [2]-->+--> B
    // [3]-->|
    //
    _op[0].__inputPhase = _mm_mul_ps(__opOut[1], _mm_set1_ps(2.5f));
    __mainCol = __opOut[0];

    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __bCol = _mm_add_ps(op3Out, __opOut[2]);
}

void DexterCore::doAlgorithm18() {
    //
    // [3]-->[2]--> A
    //
    // [0]-->+--> B
    // [1]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __mainCol = __opOut[2];

    __bCol = _mm_add_ps(__opOut[1], __opOut[0]);
}

void DexterCore::doAlgorithm19() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [3]-->[2]--> B
    //
    __mainCol = _mm_add_ps(__opOut[1], __opOut[0]);

    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    _op[2].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __bCol = __opOut[2];
}

void DexterCore::doAlgorithm20() {
    //
    // [0]-->+--> A
    // [1]-->|
    //
    // [2]-->+--> B
    // [3]-->|
    //
    __mainCol = _mm_add_ps(__opOut[1], __opOut[0]);

    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __bCol = _mm_add_ps(__opOut[3], __opOut[2]);
}

void DexterCore::doAlgorithm21() {
    //
    // [0]-->|
    // [1]-->+--> A
    // [2]-->|
    //
    // [3]--> B
    //
    __mainCol = _mm_add_ps(__opOut[2], _mm_add_ps(__opOut[1], __opOut[0]));

    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __bCol = __opOut[3];
}

void DexterCore::doAlgorithm22() {
    //
    // [3]-->+--> A
    //
    // [0]-->|
    // [1]-->+--> B
    // [2]-->|
    //
    auto op3Out = _mm_mul_ps(_op[3].getOutput(), __op3FeedbackDepth);
    _op[3].__inputPhase = _mm_mul_ps(op3Out, _mm_set1_ps(2.5f));
    __mainCol = __opOut[3];

    __bCol = _mm_add_ps(__opOut[2], _mm_add_ps(__opOut[1], __opOut[0]));
}

