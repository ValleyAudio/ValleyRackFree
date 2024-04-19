#include "VecDiodeRingMod.hpp"

VecDiode::VecDiode() {
    den = _mm_set1_ps(1.f);

    float initPrecision = 0.01f;
    float precision = initPrecision;
    float direction = -1.f;

    float x = 0.f;
    float xMax = 0.999999f;
    float xMin = 0.999998f;
    for(auto i = 0; i < DSJ_VEC_DIODE_TABLE_N; ++i) {
        float setupVB = (float)i / (float)(DSJ_VEC_DIODE_TABLE_N - 1) * 0.75f;
        for(auto j = 0; j < DSJ_VEC_DIODE_TABLE_N; ++j) {
            float hB = 4.f;
            precision = initPrecision;
            direction = -1.f;
            float setupVL = rescale((float)j, 0.f, (float)(DSJ_VEC_DIODE_TABLE_N - 1), setupVB, 1.f);
            setupVL = clamp(setupVL, setupVB + 0.0001f, 1.f);
            while(true) {
                x = hB * calcLin(1.f, setupVB, setupVL);
                if(x >= xMin && x <= xMax) {
                    break;
                }
                hB += direction * precision;
                if(x < xMin && direction == -1.f) {
                    direction = 1.f;
                    precision *= 0.1f;
                }
                else if(x > xMax && direction == 1.f) {
                    direction = -1.f;
                    precision *= 0.1f;
                }
            }
            makeupGainTable[i][j] = hB;
        }
    }
}

__m128 VecDiode::process(__m128 x) {
    __m128 out = _mm_and_ps(vecCalcNLP(x, vB, vLScaled),_mm_cmpgt_ps(x, _mm_set1_ps(vB)));
    out = _mm_switch_ps(out, vecCalcLin(x, vB, vLScaled), _mm_cmpgt_ps(x, _mm_set1_ps(vLScaled)));
    return _mm_mul_ps(out, _mm_set1_ps(makeupGain));
}

void VecDiode::setV(float newVB, float newVL) {
    vB = 0.75f * newVB;
    vL = clamp(newVL, 0.0f, 1.f);
    vLScaled = rescale(vL, 0.0f, 1.f, vB, 1.f);
    vLScaled = clamp(vLScaled, vB + 0.001f, 1.f);
    den = _mm_set1_ps(1.f / (2.f * (vLScaled - vB)));
    calcMakeupGain();
}

float VecDiode::calcNLP(float x, float vB, float vL) {
    float num = x - vB;
    num *= num;
    return num / (2 * (vL - vB));
}

float VecDiode::calcLin(float x, float vB, float vL) {
    float num = vL - vB;
    num *= num;
    return x - vL + num / (2 * (vL - vB));
}

__m128 VecDiode::vecCalcNLP(__m128 x, float vB, float vL) {
    __m128 num = _mm_sub_ps(x, _mm_set1_ps(vB));
    num = _mm_mul_ps(num, num);
    return _mm_mul_ps(num, den);
}

__m128 VecDiode::vecCalcLin(__m128 x, float vB, float vL) {
    float num = vL - vB;
    num *= num;
    return _mm_add_ps(_mm_sub_ps(x, _mm_set1_ps(vL)), _mm_mul_ps(_mm_set1_ps(num), den));
}

float VecDiode::subCalcLin(float vB, float vL) {
    float num = vL - vB;
    num *= num;
    return vL + num / (2 * (vL - vB));
}

void VecDiode::calcMakeupGain() {
    float vBF = rescale(vB, 0.001f, 0.75f, 0.f, (float)DSJ_VEC_DIODE_TABLE_N - 1);
    float vLF = rescale(vLScaled, vB, 1.f, 0.f, (float)DSJ_VEC_DIODE_TABLE_N - 1);
    int vBI_1 = clamp((int)vBF, 0, DSJ_VEC_DIODE_TABLE_N - 1);
    int vLI_1 = clamp((int)vLF, 0, DSJ_VEC_DIODE_TABLE_N - 1);
    int vBI_2 = clamp(vBI_1 + 1, 0, DSJ_VEC_DIODE_TABLE_N - 1);
    int vLI_2 = clamp(vLI_1 + 1, 0, DSJ_VEC_DIODE_TABLE_N - 1);

    vBF -= (float)vBI_1;
    vLF -= (float)vLI_1;

    float lutA = makeupGainTable[vBI_1][vLI_1];
    float lutB = makeupGainTable[vBI_1][vLI_2];
    float lutC = makeupGainTable[vBI_2][vLI_1];
    float lutD = makeupGainTable[vBI_2][vLI_2];

    makeupGain = linterp(linterp(lutA, lutB, vBF),
                         linterp(lutC, lutD, vBF), vLF);
}

__m128 VecDiodeRingMod::process(__m128 x, __m128 y, float vB, float vL) {
    d.setV(vB, vL);
    __m128 b = _mm_mul_ps(x, _mm_set1_ps(0.5f));
    __m128 a = _mm_add_ps(y, b);
    b = _mm_sub_ps(y, b);
    __m128 negA = _mm_mul_ps(_mm_set1_ps(-1.f), a);
    __m128 negB = _mm_mul_ps(_mm_set1_ps(-1.f), b);
    return _mm_sub_ps(_mm_add_ps(d.process(a), d.process(negA)),
                      _mm_add_ps(d.process(b), d.process(negB)));
}
