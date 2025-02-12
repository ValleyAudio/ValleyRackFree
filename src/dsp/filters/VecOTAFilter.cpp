//
//  VecOTAFilter.cpp
//
//  Created by Dale Johnson on 13/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "VecOTAFilter.hpp"

VecTPTOnePoleStage::VecTPTOnePoleStage() {
    G = _mm_set1_ps(0.f);
    z = _mm_set1_ps(0.f);
}

__m128 VecTPTOnePoleStage::process(const __m128& x) {
    __m128 v = _mm_mul_ps(_mm_sub_ps(vecDriveSignal(x, _mm_set1_ps(1.f)), z), G);
    __m128 out = vecDriveSignal(_mm_add_ps(v, z), _mm_set1_ps(1.f));
    z = _mm_add_ps(out, v);
    return out;
}

void VecTPTOnePoleStage::calcG(const __m128& g) {
    G = _mm_div_ps(g, _mm_add_ps(_mm_set1_ps(1.f), g));
}

void VecTPTOnePoleStage::setG(const __m128& g) {
    G = g;
}

void VecTPTOnePoleStage::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
}

float VecTPTOnePoleStage::getSampleRate() const {
    return sampleRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VecOTAFilter::VecOTAFilter() {
    k = _mm_set1_ps(0.f);

    pitch = _mm_set1_ps(0.f);
    cutoff = _mm_set1_ps(0.f);

    g = _mm_set1_ps(0.f);
    h = _mm_set1_ps(1.f);
    hRecip = _mm_set1_ps(1.f);

    G = _mm_set1_ps(0.f);
    G2 = _mm_set1_ps(0.f);
    G3 = _mm_set1_ps(0.f);
    sigma = _mm_set1_ps(0.f);
    gamma = _mm_set1_ps(0.f);
    u = _mm_set1_ps(0.f);

    lp1Result = _mm_set1_ps(0.f);
    lp2Result = _mm_set1_ps(0.f);
    lp3Result = _mm_set1_ps(0.f);
    lp4Result = _mm_set1_ps(0.f);

    pole0Coeff = _mm_set1_ps(0.f);
    pole1Coeff = _mm_set1_ps(0.f);
    pole2Coeff = _mm_set1_ps(0.f);
    pole3Coeff = _mm_set1_ps(0.f);
    pole4Coeff = _mm_set1_ps(0.f);

    tanhRecip = 1.f / tanhDriveSignal(1.f, 1.f);

    setSampleRate(44100.f);
    _mode = -1;
    setMode(LP4_MODE);
}

__m128 VecOTAFilter::process(const __m128& in) {
    sigma = _mm_mul_ps(G3, stage1.z);
    sigma = _mm_add_ps(sigma, _mm_mul_ps(G2, stage2.z));
    sigma = _mm_add_ps(sigma, _mm_mul_ps(G, stage3.z));
    sigma = _mm_mul_ps(_mm_add_ps(sigma, stage4.z), hRecip);

    u = _mm_mul_ps(in, _mm_set1_ps(0.5f));
    u = _mm_sub_ps(u, _mm_mul_ps(_mm_mul_ps(k, vecDriveSignal(sigma, _mm_set1_ps(1.f))), _mm_set1_ps(tanhRecip)));
    u = _mm_div_ps(u, _mm_add_ps(_mm_set1_ps(1.f), _mm_mul_ps(k, gamma)));
    lp1Result = stage1.process(u);
    lp2Result = stage2.process(lp1Result);
    lp3Result = stage3.process(lp2Result);
    lp4Result = stage4.process(lp3Result);
    out = _mm_mul_ps(lp1Result, pole1Coeff);
    out = _mm_add_ps(out, _mm_mul_ps(lp2Result, pole2Coeff));
    out = _mm_add_ps(out, _mm_mul_ps(lp3Result, pole3Coeff));
    out = _mm_add_ps(out, _mm_mul_ps(lp4Result, pole4Coeff));
    return out;
}

void VecOTAFilter::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    stage1.setSampleRate(sampleRate);
    stage2.setSampleRate(sampleRate);
    stage3.setSampleRate(sampleRate);
    stage4.setSampleRate(sampleRate);
    setCutoff(pitch);
}

void VecOTAFilter::setCutoff(const __m128& newPitch) {
    pitch = _mm_clamp_ps(newPitch, _mm_set1_ps(0.f), _mm_set1_ps(10.f));
    pitch = _mm_add_ps(pitch, _mm_set1_ps(-(5.f + (2.f / 12.f))));
    g = _mm_mul_ps(_mm_set1_ps(0.0314f), valley::_mm_exp_ps(_mm_mul_ps(_mm_set1_ps(0.6937f), pitch)));
    g = _mm_add_ps(g, _mm_mul_ps(_mm_set1_ps(0.0000024f), valley::_mm_exp_ps(_mm_mul_ps(_mm_set1_ps(2.4544f), pitch))));

    hRecip = _mm_div_ps(_mm_set1_ps(1.f), _mm_add_ps(g, _mm_set1_ps(1.f)));
    G = _mm_mul_ps(g, hRecip);

    stage1.G = G;
    stage2.G = G;
    stage3.G = G;
    stage4.G = G;
    G2 = _mm_mul_ps(G, G);
    G3 = _mm_mul_ps(G2, G);
    gamma = _mm_mul_ps(G3, G);
}

void VecOTAFilter::setQ(const __m128& Q) {
  //_k = 4.f * clip(Q, 0.f, 10.f) / 10.f;
  k = _mm_mul_ps(_mm_set1_ps(0.4f), _mm_clamp_ps(Q, _mm_set1_ps(0.f), _mm_set1_ps(10.f)));
}

void VecOTAFilter::setMode(int mode) {
    if(_mode == mode) {
        return;
    }

    _mode = mode;
    pole0Coeff = _mm_set1_ps(0.f);
    pole1Coeff = _mm_set1_ps(0.f);
    pole2Coeff = _mm_set1_ps(0.f);
    pole3Coeff = _mm_set1_ps(0.f);
    pole4Coeff = _mm_set1_ps(0.f);

    if (mode == LP2_MODE) {
        pole2Coeff = _mm_set1_ps(1.f);

    }
    else if (mode == LP4_MODE) {
        pole4Coeff = _mm_set1_ps(1.f);

    }
    else if (mode == BP2_MODE) {
        pole1Coeff = _mm_set1_ps(2.f);
        pole2Coeff = _mm_set1_ps(-2.f);

    }
    else if (mode == BP4_MODE) {
        pole2Coeff = _mm_set1_ps(4.f);
        pole3Coeff = _mm_set1_ps(-8.f);
        pole4Coeff = _mm_set1_ps(4.f);
    }
    else {
        pole4Coeff = _mm_set1_ps(1.f);
    }
}

