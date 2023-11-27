//
// Osc4Core_SIMD.hpp
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

#pragma once

#include <cmath>
#include <string>
#include "../dsp/generators/QuadOsc.hpp"
#include "DexterWavetableROM.hpp"

class DexterCore {
public:
    enum OpSyncSource {
        PARENT_SYNC_SOURCE,
        NEIGHBOUR_SYNC_SOURCE
    };

    static constexpr size_t kNumOperators = 4;
    static constexpr size_t kNumAlgorithms = 23;

    DexterCore();
    DexterCore(const DexterCore& copy) = delete;
    void process();
    void resetPhase();
    void externalFM(size_t opNum, float extFM);
    void externalSync(size_t opNum, float extSync);

    __m128 getMainOutput() const;
    __m128 getBOutput() const;
    __m128 getOpOutput(size_t opNum) const;
    void setAlgorithm(int newAlgorithm);
    void setFeedback(float feedback);
    void setFrequency(size_t opNum, float frequency);
    void _mm_setFrequency(size_t opNum, const __m128& frequency);
    void setWavebank(size_t opNum, int bankNum);
    void setWavePosition(size_t opNum, float position);
    void setShape(size_t opNum, float shape);
    void setLevel(size_t opNum, float level);
    void setOpPreFade(size_t opNum, bool opPreFade);
    void setBrightness(float newBrightness);
    void setSyncMode(size_t opNum, int syncMode);
    void setSyncSource(OpSyncSource newOpSyncSource);
    void enableSync(size_t opNum, bool enableSync);
    void enableIntSync(size_t opNum, bool enableIntSync);
    void enableWeakSync(size_t opNum, bool weakEnable);
    void setShapeMode(size_t opNum, int shapeMode);
    void setPMPostShape(size_t opNum, bool PMPostShape);
    void setSampleRate(float sampleRate);

private:
    ScanningQuadOsc op[4];

    enum MatrixRows{
         OP_0_ROW,
         OP_1_ROW,
         OP_2_ROW,
         OP_3_ROW,
         NUM_ROWS
     };

    enum MatrixColumns{
         OP_0_COL,
         OP_1_COL,
         OP_2_COL,
         OP_3_COL,
         MAIN_OUT_COL,
         B_OUT_COL,
         NUM_COLS
    };

    float inLevels[kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    float opLevels[kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    bool outputOpPreFade[kNumOperators] = {false, false, false, false};
    __m128 opLevelVec[kNumOperators];
    __m128 opOutVec[kNumOperators];
    __m128 opAuxOutVec[kNumOperators];
    __m128 op1EocVec, op2EocVec, op3EocVec, op4EocVec;
    __m128 opExtFMVec[kNumOperators];
    __m128 opExtSyncVec[kNumOperators];
    __m128 opSyncEnableVec[kNumOperators];

    __m128 op3FeedbackDepthVec;
    __m128 matrixVec[NUM_ROWS][NUM_COLS]; // Row = Source, Col = Dest
    __m128 mainColVec, bColVec;
    OpSyncSource opSyncSource;
    __m128 opSyncSignalVec[kNumOperators];
    __m128 opSyncIn[kNumOperators];
    __m128 onesVec, zerosVec, fiveVec;
    __m128 outputLevelsVec[kNumOperators];
    __m128 aOutLevelVec, bOutLevelVec;
    bool weakSync[kNumOperators] = {false, false, false, false};
    bool extSyncing = false;

    float brightness = 0.f;
    int brightnessMask = 0;
    int algorithm = -1;

    void calcOpLevels();
    void clearMatrix();

    // Combined output algorithms
    void setMatrixAlgo0();
    void setMatrixAlgo1();
    void setMatrixAlgo2();
    void setMatrixAlgo3();
    void setMatrixAlgo4();
    void setMatrixAlgo5();
    void setMatrixAlgo6();
    void setMatrixAlgo7();
    void setMatrixAlgo8();
    void setMatrixAlgo9();
    void setMatrixAlgo10();
    void setMatrixAlgo11();

    // Seperate output algorithms
    void setMatrixAlgo12();
    void setMatrixAlgo13();
    void setMatrixAlgo14();
    void setMatrixAlgo15();
    void setMatrixAlgo16();
    void setMatrixAlgo17();
    void setMatrixAlgo18();
    void setMatrixAlgo19();
    void setMatrixAlgo20();
    void setMatrixAlgo21();
    void setMatrixAlgo22();

    // Optimised combined output algorithms
    void doAlgorithm0();
    void doAlgorithm1();
    void doAlgorithm2();
    void doAlgorithm3();
    void doAlgorithm4();
    void doAlgorithm5();
    void doAlgorithm6();
    void doAlgorithm7();
    void doAlgorithm8();
    void doAlgorithm9();
    void doAlgorithm10();
    void doAlgorithm11();

    // Optimised seperate output algorithms
    void doAlgorithm12();
    void doAlgorithm13();
    void doAlgorithm14();
    void doAlgorithm15();
    void doAlgorithm16();
    void doAlgorithm17();
    void doAlgorithm18();
    void doAlgorithm19();
    void doAlgorithm20();
    void doAlgorithm21();
    void doAlgorithm22();
};

