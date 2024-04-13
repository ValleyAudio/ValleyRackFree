#include "DiodeRingMod.hpp"

Diode::Diode() {
    float initPrecision = 0.01f;
    float precision = initPrecision;
    float direction = -1.f;

    float x = 0.f;
    float xMax = 0.999999f;
    float xMin = 0.999998f;

    for (std::size_t i = 0; i < diodeTableSize; ++i) {
        float setupVB = (float)i / (float)(diodeTableSize - 1) * 0.75f;

        for (std::size_t j = 0; j < diodeTableSize; ++j) {
            float hB = 4.f;
            precision = initPrecision;
            direction = -1.f;
            float setupVL = rescale((float)j, 0.f, (float)(diodeTableSize - 1), setupVB, 1.f);
            setupVL = clamp(setupVL, setupVB + 0.0001f, 1.f);

            while (true) {
                x = hB * calcLin(1.f, setupVB, setupVL);

                if (x >= xMin && x <= xMax) {
                    break;
                }

                hB += direction * precision;

                if (x < xMin && direction == -1.f) {
                    direction = 1.f;
                    precision *= 0.1f;
                }
                else if (x > xMax && direction == 1.f) {
                    direction = -1.f;
                    precision *= 0.1f;
                }
            }
            makeupGainTable[i][j] = hB;
        }
    }
}

float Diode::process(float x) {
    float out = 0.f;
    if(x <= vB) {
        out = 0.f;
    }
    else if(x > vB && x <= vLScaled) {
        out = calcNLP(x, vB, vLScaled);
    }
    else {
        out = calcLin(x, vB, vLScaled);
    }
    return out * makeupGain;
}

void Diode::setV(float newVB, float newVL) {
    vB = 0.75f * newVB;
    vL = clamp(newVL, 0.0f, 1.f);
    vLScaled = rescale(vL, 0.0f, 1.f, vB, 1.f);
    vLScaled = clamp(vLScaled, vB + 0.001f, 1.f);
    calcMakeupGain();
}

float Diode::calcNLP(float x, float a, float b) {
    float num = x - a;
    num *= num;
    return num / (2.f * (b - a));
}

float Diode::calcLin(float x, float a, float b) {
    float num = b - a;
    num *= num;
    return x - b + num / (2.f * (b - a));
}

void Diode::calcMakeupGain() {
    float vBF = rescale(vB, 0.001f, 0.75f, 0.f, (float)diodeTableSize - 1);
    float vLF = rescale(vLScaled, vB, 1.f, 0.f, (float)diodeTableSize - 1);
    int vBI_1 = clamp((long)vBF, 0, diodeTableSize - 1);
    int vLI_1 = clamp((long)vLF, 0, diodeTableSize - 1);
    int vBI_2 = clamp(vBI_1 + 1, 0, diodeTableSize - 1);
    int vLI_2 = clamp(vLI_1 + 1, 0, diodeTableSize - 1);

    vBF -= (float)vBI_1;
    vLF -= (float)vLI_1;

    float lutA = makeupGainTable[vBI_1][vLI_1];
    float lutB = makeupGainTable[vBI_1][vLI_2];
    float lutC = makeupGainTable[vBI_2][vLI_1];
    float lutD = makeupGainTable[vBI_2][vLI_2];

    makeupGain = linterp(linterp(lutA, lutB, vBF), linterp(lutC, lutD, vBF), vLF);
}

float DiodeRingMod::process(float x, float y, float vB, float vL) {
    d.setV(vB, vL);
    b = x * 0.5f;
    a = y + b;
    b = y - b;
    return d.process(a) + d.process(-a) - d.process(b) + d.process(-b);
}
