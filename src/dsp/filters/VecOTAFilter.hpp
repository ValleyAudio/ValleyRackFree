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

    __m128 process(const __m128& x);

    void calcG(const __m128& g);
    void setG(const __m128& g);

    void setSampleRate(float newSampleRate);
    float getSampleRate() const;
    float getZ() const;
    __m128 G;
    __m128 z;

protected:
    float sampleRate = 44100.f;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class VecOTAFilter {
public:
    VecOTAFilter();

    __m128 process(const __m128& in);

    void setSampleRate(float sampleRate);
    void setCutoff(const __m128& pitch);
    void setQ(const __m128& Q);
    void setMode(int mode);

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

    __m128 pitch, cutoff, g , h, hRecip;
    __m128 G, G2, G3;
    __m128 sigma, gamma, u;
    __m128 pole1Coeff, pole2Coeff, pole3Coeff, pole4Coeff;
    __m128 lp1Result, lp2Result, lp3Result, lp4Result;

    int _mode = -1;

    float tanhRecip = 1.f;
    float sampleRate = 44100.f;
};

#endif /* OTAFilter_hpp */
