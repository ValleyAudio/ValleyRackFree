//
//  VecOTAFilter.cpp
//
//  Created by Dale Johnson on 13/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "VecOTAFilter.hpp"

VecTPTOnePoleStage::VecTPTOnePoleStage() {
    _zeros = _mm_set1_ps(0.f);
    _ones = _mm_set1_ps(1.f);
    _G = _zeros;
    _z = _zeros;
    _v = _zeros;
    _out = _zeros;
}

void VecTPTOnePoleStage::setSampleRate(float sampleRate) {
    _sampleRate = sampleRate;
}

float VecTPTOnePoleStage::getSampleRate() const {
    return _sampleRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VecOTAFilter::VecOTAFilter() {
    ones = _mm_set1_ps(1.f);
    zeros = _mm_set1_ps(0.f);
    k = zeros;

    pitch = zeros;
    cutoff = zeros;

    g = zeros;
    h = ones;
    hRecip = ones;

    G = zeros;
    G2 = zeros;
    G3 = zeros;
    sigma = zeros;
    gamma = zeros;
    u = zeros;

    lp1Result = zeros;
    lp2Result = zeros;
    lp3Result = zeros;
    lp4Result = zeros;

    pole0Coeff = zeros;
    pole1Coeff = zeros;
    pole2Coeff = zeros;
    pole3Coeff = zeros;
    pole4Coeff = zeros;

    _1_tanhf = 1.f / tanhDriveSignal(1.f, 1.f);

    for (auto& x : _kGTable)
        x = 0.f;

    for (auto& x : _kHTable)
        x = 0.f;

    setSampleRate(44100.f);
    _mode = -1;
    setMode(LP4_MODE);
}

void VecOTAFilter::setSampleRate(float sampleRate) {
    _sampleRate = sampleRate;
    calcInternalGTable();
    _stage1.setSampleRate(sampleRate);
    _stage2.setSampleRate(sampleRate);
    _stage3.setSampleRate(sampleRate);
    _stage4.setSampleRate(sampleRate);
    setCutoff(pitch);
}

void VecOTAFilter::setCutoff(const __m128& newPitch) {
    pitch = _mm_clamp_ps(pitch, zeros, _mm_set1_ps(10.f));
    cutoff = _mm_mul_ps(pitch, _mm_set1_ps(100000.f));
    __m128i cutoffI = _mm_cvttps_epi32(cutoff);
    _mm_storeu_si128((__m128i*)_pos, cutoffI);
    __m128 frac = _mm_sub_ps(cutoff, _mm_cvtepi32_ps(cutoffI));

    for (auto& p : _pos)
        p = (p < 0 ? 0 : p) > (G_TABLE_SIZE - 2) ? (G_TABLE_SIZE - 2) : p;

    float lowG[4] = {0.f, 0.f, 0.f, 0.f};
    float highG[4] = {0.f, 0.f, 0.f, 0.f};
    float lowH[4] = {1.f, 1.f, 1.f, 1.f};
    float highH[4] = {1.f, 1.f, 1.f, 1.f};

    for(auto i = 0; i < 4; ++i) {
        lowG[i] = _kGTable[_pos[i]];
        highG[i] = _kGTable[_pos[i] + 1];
        lowH[i] = _kHTable[_pos[i]];
        highH[i] = _kHTable[_pos[i] + 1];
    }

    __m128 vLowG = _mm_loadu_ps(lowG);
    __m128 vHighG = _mm_loadu_ps(highG);
    g = _mm_linterp_ps(vLowG, vHighG, frac);

    __m128 vLowH = _mm_loadu_ps(lowH);
    __m128 vHighH = _mm_loadu_ps(highH);
    hRecip = _mm_linterp_ps(vLowH, vHighH, frac);

    /*long pos = (long)_cutoff;
    float frac = _cutoff - (float)pos;
    _g = linterp(kGTable[pos], kGTable[pos + 1], frac);*/

    /*h = _mm_add_ps(ones, g);
    hRecip = _mm_div_ps(ones, h);*/
    G = _mm_mul_ps(g, hRecip);

    _stage1._G = G;
    _stage2._G = G;
    _stage3._G = G;
    _stage4._G = G;
    G2 = _mm_mul_ps(G, G);
    G3 = _mm_mul_ps(G2, G);
    gamma = _mm_mul_ps(G3, G);
}

void VecOTAFilter::setQ(const __m128& Q) {
  //_k = 4.f * clip(Q, 0.f, 10.f) / 10.f;
  k = _mm_mul_ps(_mm_set1_ps(0.4f), _mm_clamp_ps(Q, _mm_set1_ps(0.f), _mm_set1_ps(10.f)));
}

void VecOTAFilter::calcInternalGTable() {
    float f = 0.f;
    float wd = 0.f;
    float T = 1.f / _sampleRate;
    float T_2 = T / 2.f;
    float wa = 0.f;
    float g = 0.f;
    float h = 0.f;

    for(auto i = 0; i < G_TABLE_SIZE; ++i) {
        f = 440.f * powf(2.f, ((i - 500000.f) / 100000.f));
        wd = 2.f * M_PI * f;
        wa = (2.f / T) * tanf(wd * T_2);
        g = wa * T_2;
        _kGTable[i] = g;
        h = g + 1.f;
        h = 1.f / h;
        _kHTable[i] = h;
    }
}
