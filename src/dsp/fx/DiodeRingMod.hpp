/**
 * @file DiodeRingMod.hpp
 * @author Dale Johnson
 */

#pragma once
#include <iostream>
#include "../../Valley.hpp"
#include "../../utilities/Utilities.hpp"

using namespace std;

class Diode {
public:
    Diode();
    float process(float x);
    void setV(float vB, float vL);
private:
    static constexpr std::size_t diodeTableSize = 512;
    float vB, vL, vLScaled;
    float makeupGain = 1.f;
    float makeupGainTable[512][512];
    float calcNLP(float x, float vB, float vL);
    float calcLin(float x, float vB, float vL);
    void calcMakeupGain();
};

class DiodeRingMod {
public:
    float process(float x, float y, float vB, float vL);
private:
    float a;
    float b;
    Diode d;
};
