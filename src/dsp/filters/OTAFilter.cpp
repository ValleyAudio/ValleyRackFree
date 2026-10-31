//
//  OTAFilter.cpp
//
//  Created by Dale Johnson on 13/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "OTAFilter.hpp"

void calcGTable(float sampleRate) {
    float f = 0.f;
    float wd = 0.f;
    float T = 1.f / sampleRate;
    float T_2 = T / 2.f;
    float wa = 0.f;
    float g = 0.f;

    for(auto i = 0; i < G_TABLESIZE; ++i) {
        f = 440.f * powf(2.f, ((i - 500000.f) / 100000.f));
        wd = 2.f * M_PI * f;
        wa = (2.f / T) * tanf(wd * T_2);
        g = wa * T_2;
        kGTable[i] = g;
    }
}

TPTOnePoleStage::TPTOnePoleStage() {
    G = 0.f;
    s = 0.f;
    z = 0.f;
    v = 0.f;
    out = 0.f;
    setSampleRate(44100.f);
    tanhf_recip = 1.f / tanhDriveSignal(1.f, 1.f);
    doNlp = false;
}

void TPTOnePoleStage::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
}

float TPTOnePoleStage::getSampleRate() const {
    return sampleRate;
}

void TPTOnePoleStage::enableNLP(bool enable) {
    doNlp = enable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

OTAFilter::OTAFilter() {
    G = 0.f;
    sigma = 0.f;
    gamma = 0.f;
    k = 0.f;
    u = 0.f;
    h = 1.f;
    S1 = 0.f;
    S2 = 0.f;
    S3 = 0.f;
    S4 = 0.f;
    lp1 = 0.f;
    lp2 = 0.f;
    lp3 = 0.f;
    tanhf_recip = 1.f / tanhDriveSignal(1.f, 1.f);
    doNlp = false;
    pitch = 0.f;
    prevPitch = -1.f;
}

void OTAFilter::setSampleRate(float sampleRate) {
    stage1.setSampleRate(sampleRate);
    stage2.setSampleRate(sampleRate);
    stage3.setSampleRate(sampleRate);
    stage4.setSampleRate(sampleRate);
    setCutoff(pitch);
}

void OTAFilter::setCutoff(float newPitch) {
    pitch = clip(newPitch, 0.f, 10.0f);
    if(pitch == prevPitch) {
        return;
    }
    prevPitch = pitch;
    cutoff = pitch * 100000.f;
    long pos = (long)cutoff;
    float frac = cutoff - (float)pos;

    g = linterp(kGTable[pos], kGTable[pos + 1], frac);
    h = 1.f + g;
    h_recip = 1.f / h;
    G = g * h_recip;
    stage1.G = G;
    stage2.G = G;
    stage3.G = G;
    stage4.G = G;
    gamma = G * G * G * G;
}

void OTAFilter::setQ(float Q) {
  k = 4.f * clip(Q, 0.f, 10.f) / 10.f;
}

void OTAFilter::enableNLP(bool enable) {
    doNlp = enable;
    stage1.enableNLP(doNlp);
    stage2.enableNLP(doNlp);
    stage3.enableNLP(doNlp);
    stage4.enableNLP(doNlp);
}
