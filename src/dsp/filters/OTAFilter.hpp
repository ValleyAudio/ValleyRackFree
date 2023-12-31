//
//  OTAFilter.hpp
//
//  Created by Dale Johnson on 13/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#ifndef OTA_FILTER_HPP
#define OTA_FILTER_HPP

#include <cmath>
#define G_TABLESIZE 1100000
#define TANH_TABLESIZE 8192
#include "../../utilities/Utilities.hpp"
#include "../shaping/NonLinear.hpp"
#include <iostream>

static float kGTable[G_TABLESIZE];

void calcGTable(float sampleRate);

class TPTOnePoleStage {
public:
  TPTOnePoleStage();
  inline float process(float in) {
    v = (tanhDriveSignal(in, 1.f) - z) * G;
    out = tanhDriveSignal(v + z, 1.f);
    z = out + v;
    return out;
  }

  inline void calcG(float g) {
      G = g / (1.f + g);
  }

  inline void setG(float g) {
      G = g;
  }

  void setSampleRate(float newSampleRate);
  float getSampleRate() const;
  float getZ() const;
  void enableNLP(bool enable);
  float G;
  float z;

protected:
  float s;
  float sampleRate;

  float out;
  float v;
  float tanhf_recip;
  bool doNlp;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class OTAFilter {
public:
  float out;
  OTAFilter();

  inline float process(float in) {
      G2 = G * G;
      G3 = G2 * G;
      sigma = G3 * stage1.z;
      sigma += G2 * stage2.z;
      sigma += G * stage3.z;
      sigma += stage4.z;
      sigma *= h_recip;

      //u = (in * 0.5f - k * lookUpTanhf(sigma) * tanhf_recip) / (1.f + k * gamma);
      u = (in * 0.5f - k * tanhDriveSignal(sigma, 1.f) * tanhf_recip) / (1.f + k * gamma);
      lp1 = stage1.process(u);
      lp2 = stage2.process(lp1);
      lp3 = stage3.process(lp2);
      lp4 = stage4.process(lp3);
      out = lp4 * fourPole + lp2 * (1.f - fourPole);
      return out;
  }

  void setSampleRate(float sampleRate);
  void setCutoff(float newPitch);
  void setQ(float Q);
  void enableNLP(bool enable);
  inline void set4Pole(float isFourPole) {
      fourPole = isFourPole;
  }
protected:
  TPTOnePoleStage stage1;
  TPTOnePoleStage stage2;
  TPTOnePoleStage stage3;
  TPTOnePoleStage stage4;
  float k;

  float pitch, prevPitch, cutoff, g , h, h_recip;
  float G, G2, G3;
  float sigma, gamma;
  float S1, S2, S3, S4;
  float u;
  float lp1, lp2, lp3, lp4;

  float tanhf_recip;
  bool doNlp;
  float fourPole;
};

#endif /* OTAFilter_hpp */
