//
//  VecOTAFilter.hpp
//
//  Created by Dale Johnson on 13/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#ifndef VEC_OTA_FILTER_HPP
#define VEC_OTA_FILTER_HPP

#include <cmath>
#define G_TABLE_SIZE 1100000
#define TANH_TABLE_SIZE 8192
#include "../../simd/SIMDUtilities.hpp"
#include "../../utilities/Utilities.hpp"
#include "../shaping/VecNonLinear.hpp"
#include "../filters/OTAFilter.hpp"
#include <iostream>

class VecTPTOnePoleStage {
public:
  VecTPTOnePoleStage();
  inline __m128 process(const __m128& in) {
    __m128 v = _mm_mul_ps(_mm_sub_ps(vecDriveSignal(in, ones), z), G);
    __m128 out = vecDriveSignal(_mm_add_ps(v, z), ones);
    z = _mm_add_ps(out, v);
    return out;
  }

  inline void calcG(const __m128& g) {
      G = _mm_div_ps(g, _mm_add_ps(ones, g));
  }

  inline void setG(const __m128& g) {
      G = g;
  }

  void setSampleRate(float newSampleRate);
  float getSampleRate() const;
  float getZ() const;
  __m128 G;
  __m128 z;

protected:
  float sampleRate = 44100.f;

  __m128 ones, zeros;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class VecOTAFilter {
public:
  VecOTAFilter();

  inline __m128 process(const __m128& in) {
      sigma = _mm_mul_ps(G3, stage1.z);
      sigma = _mm_add_ps(sigma, _mm_mul_ps(G2, stage2.z));
      sigma = _mm_add_ps(sigma, _mm_mul_ps(G, stage3.z));
      sigma = _mm_mul_ps(_mm_add_ps(sigma, stage4.z), hRecip);

      u = _mm_mul_ps(in, _mm_set1_ps(0.5f));
      u = _mm_sub_ps(u, _mm_mul_ps(_mm_mul_ps(k, vecDriveSignal(sigma, ones)), _mm_set1_ps(tanhRecip)));
      u = _mm_div_ps(u, _mm_add_ps(ones, _mm_mul_ps(k, gamma)));
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

  void setSampleRate(float sampleRate);
  void setCutoff(const __m128& pitch);
  void setQ(const __m128& Q);

    inline void setMode(int mode) {
        if(_mode == mode) {
            return;
        }

        _mode = mode;
        pole0Coeff = zeros;
        pole1Coeff = zeros;
        pole2Coeff = zeros;
        pole3Coeff = zeros;
        pole4Coeff = zeros;

        switch(_mode) {
            case LP2_MODE:
                pole2Coeff = ones;
                break;
            case LP4_MODE:
                pole4Coeff = ones;
                break;
            case BP2_MODE:
                pole1Coeff = _mm_set1_ps(2.f);
                pole2Coeff = _mm_set1_ps(-2.f);
                break;
            case BP4_MODE:
                pole2Coeff = _mm_set1_ps(4.f);
                pole3Coeff = _mm_set1_ps(-8.f);
                pole4Coeff = _mm_set1_ps(4.f);
                break;
          default :
                pole4Coeff = ones;
        }
    }

    __m128 out;
    enum Modes {
      LP2_MODE = 0,
      LP4_MODE,
      BP2_MODE,
      BP4_MODE,
      HP2_MODE,
      HP4_MODE
    };

protected:
    VecTPTOnePoleStage stage1;
    VecTPTOnePoleStage stage2;
    VecTPTOnePoleStage stage3;
    VecTPTOnePoleStage stage4;
    __m128 k;

    __m128 ones, zeros;
    __m128 pitch, cutoff, g , h, hRecip;
    __m128 G, G2, G3;
    __m128 sigma, gamma, u;
    __m128 lp1Result, lp2Result, lp3Result, lp4Result;

    int _mode = -1;
    __m128 pole0Coeff, pole1Coeff, pole2Coeff, pole3Coeff, pole4Coeff;

    int32_t pos[4] = {0, 0, 0, 0};

    float tanhRecip = 1.f;

    float kGTable[G_TABLE_SIZE];
    float kHTable[G_TABLE_SIZE];
    float sampleRate = 44100.f;

    void calcInternalGTable();
};

#endif /* OTAFilter_hpp */
