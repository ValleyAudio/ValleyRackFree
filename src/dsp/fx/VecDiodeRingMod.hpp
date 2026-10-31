#pragma once

#include "../../Valley.hpp"
#include "../../utilities/Utilities.hpp"
#include "../../simd/SIMDUtilities.hpp"
#define DSJ_VEC_DIODE_TABLE_N 512
using namespace std;

class VecDiode {
public:
    VecDiode();
    __m128 process(__m128 x);
    void setV(float vB, float vL);
private:
    __m128 den;

    float vB, vL, vLScaled;
    float makeupGain = 1.f;
    float makeupGainTable[512][512];
    float calcNLP(float x, float vB, float vL);
    float calcLin(float x, float vB, float vL);
    __m128 vecCalcNLP(__m128 x, float vB, float vL);
    __m128 vecCalcLin(__m128 x, float vB, float vL);
    float subCalcLin(float vB, float vL);
    void calcMakeupGain();
};

class VecDiodeRingMod {
public:
    __m128 process(__m128 x, __m128 y, float vB, float vL);
private:
    VecDiode d;
};
