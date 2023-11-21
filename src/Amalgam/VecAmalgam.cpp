//
//  VecAmalgam.cpp
//  BitCombiner
//
//  Created by Dale Johnson on 26/11/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#include "VecAmalgam.hpp"

VecAmalgam::VecAmalgam() {
    zerosVec = _mm_set1_ps(0.f);
    onesVec = _mm_set1_ps(1.f);
    negOnesVec = _mm_set1_ps(-1.f);
    halfsVec = _mm_set1_ps(0.5f);
    ffTargetVec = zerosVec;
    highVec = _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF));

    xVec = zerosVec;
    yVec = zerosVec;
    zVec = zerosVec;
    xFoldedVec = zerosVec;
    yFoldedVec = zerosVec;

    step_ = 1.f;
    xDSVec = zerosVec;
    yDSVec = zerosVec;
    zDSVec = zerosVec;

    p[RING_MOD_1_MODE] = &VecAmalgam::ringMod1;
    p[RING_MOD_2_MODE] = &VecAmalgam::ringMod2;
    p[RING_MOD_3_MODE] = &VecAmalgam::ringMod3;
    p[DIODE_RING_MOD_MODE] = &VecAmalgam::diodeRingMod;
    p[MIN_MAX_MODE] = &VecAmalgam::minMax;
    p[SIGN_SWITCH_1_MODE] = &VecAmalgam::signSwitch1;
    p[SIGN_SWITCH_2_MODE] = &VecAmalgam::signSwitch2;
    p[X_FADE_MODE] = &VecAmalgam::xFade;
    p[FLIP_FLOP_MODE] = &VecAmalgam::flipFlop;
    p[ALPHA_PWM_MODE] = &VecAmalgam::alphaPWM;
    p[BIT_AND_MODE] = &VecAmalgam::bitAND;
    p[BIT_XOR_MODE] = &VecAmalgam::bitXOR;
    p[BIT_INTERLEAVE_MODE] = &VecAmalgam::bitInterleave;
    p[BIT_HACK_MODE] = &VecAmalgam::bitHack;
    p[BIT_AND_FLOAT_MODE] = &VecAmalgam::bitANDFloat;
    p[BIT_INTERLEAVE_FLOAT_MODE] = &VecAmalgam::bitInterleaveFloat;
    p[BIT_HACK_FLOAT_MODE] = &VecAmalgam::bitHackFloat;

    _mode = RING_MOD_1_MODE;

    std::srand(std::time(NULL));
    for(auto i = 0; i < 4; ++i) {
        z_[i] = std::rand();
        w_[i] = std::rand();
        k_[i] = 0;
    }
    setSampleRate(44100.f);
}

void VecAmalgam::setMode(int mode) {
    _mode = mode;
    if(_mode < 0) {
        _mode = 0;
    }
    else if(_mode >= NUM_MODES) {
        _mode = NUM_MODES - 1;
    }
}

void VecAmalgam::setSampleRate(float sampleRate) {
    engineSampleRate_ = sampleRate;
    quarterNyquist_ = engineSampleRate_ / 32.f;
}

void VecAmalgam::calcStepSize() {
    internalSampleRate_ = quarterNyquist_ * powf(2.f, (updateRate_ * 5.f));
    stepSize_ = internalSampleRate_ / engineSampleRate_;
}

__m128 VecAmalgam::ringMod1(const __m128& x, const __m128& y, float paramA, float paramB) {
    zVec = _mm_or_ps(_mm_cmpgt_ps(y, _mm_set1_ps(paramB * 1.066f)),
                    _mm_cmplt_ps(y, _mm_set1_ps(paramB * -1.066f)));
    zVec = _mm_and_ps(y, zVec);
    return _mm_mul_ps(_mm_add_ps(_mm_mul_ps(x, _mm_set1_ps(1.f - 0.5f * paramA)),
                                               _mm_set1_ps(paramA * 0.5f)), zVec);
}

__m128 VecAmalgam::ringMod2(const __m128& x, const __m128& y, float paramA, float paramB) {
    xFoldedVec = _mm_add_ps(_mm_mul_ps(x, halfsVec), halfsVec);
    yFoldedVec = _mm_add_ps(_mm_mul_ps(y, halfsVec), halfsVec);
    xFoldedVec = _mm_mirror_ps(xFoldedVec, _mm_set1_ps(paramA + 0.00001f));
    yFoldedVec = _mm_mirror_ps(yFoldedVec, _mm_set1_ps(paramB + 0.00001f));
    xFoldedVec = _mm_mul_ps(_mm_add_ps(xFoldedVec, _mm_set1_ps(-0.5f)),_mm_set1_ps(2.f));
    yFoldedVec = _mm_mul_ps(_mm_add_ps(yFoldedVec, _mm_set1_ps(-0.5f)),_mm_set1_ps(2.f));

    return _mm_mul_ps(xFoldedVec, yFoldedVec);
}

__m128 VecAmalgam::ringMod3(const __m128& x, const __m128& y, float paramA, float paramB) {
    xFoldedVec = _mm_add_ps(_mm_mul_ps(x, halfsVec), halfsVec);
    yFoldedVec = _mm_add_ps(_mm_mul_ps(y, halfsVec), halfsVec);
    xFoldedVec = _mm_mirror_ps(xFoldedVec, _mm_set1_ps(paramA + 0.00001f));
    yFoldedVec = _mm_mirror_ps(yFoldedVec, _mm_set1_ps(paramA + 0.00001f));
    xFoldedVec = _mm_mul_ps(_mm_add_ps(xFoldedVec, _mm_set1_ps(-0.5f)),_mm_set1_ps(2.f));
    yFoldedVec = _mm_mul_ps(_mm_add_ps(yFoldedVec, _mm_set1_ps(-0.5f)),_mm_set1_ps(-2.f));

    xLogicVec = _mm_cmpgt_ps(xFoldedVec, zerosVec);
    yLogicVec = _mm_cmpgt_ps(yFoldedVec, zerosVec);
    zLogicVec = _mm_xor_ps(xLogicVec, yLogicVec);
    zVec = _mm_and_ps(zLogicVec, onesVec);
    zVec = _mm_add_ps(_mm_mul_ps(zVec, _mm_set1_ps(-2.f)), onesVec);

    return _mm_linterp_ps(_mm_mul_ps(xFoldedVec, yFoldedVec), zVec, _mm_set1_ps(paramB));
}

__m128 VecAmalgam::diodeRingMod(const __m128& x, const __m128& y, float paramA, float paramB) {
    return diodeRingModCore.process(x, y, paramA, paramB);
}

__m128 VecAmalgam::minMax(const __m128& x, const __m128& y, float paramA, float paramB) {
    __m128 select = _mm_cmpgt_ps(x, y);
    __m128 max = _mm_switch_ps(y, x, select);
    __m128 min = _mm_switch_ps(x, y, select);
    __m128 delta = _mm_sub_ps(max, min);
    zVec = _mm_linterp_ps(min, max, _mm_set1_ps(paramA));
    return _mm_linterp_ps(zVec, _mm_mul_ps(zVec, delta), _mm_set1_ps(paramB));
}

__m128 VecAmalgam::signSwitch1(const __m128& x, const __m128& y, float paramA, float paramB) {
    __m128 midPoint = _mm_set1_ps(paramA * 2.f - 1.f);
    __m128 thresh = _mm_set1_ps(paramB);
    return _mm_add_ps(_mm_and_ps(x, _mm_cmpge_ps(x, _mm_add_ps(midPoint, thresh))),
                      _mm_and_ps(y, _mm_cmplt_ps(y,_mm_sub_ps(midPoint, thresh))));
}

__m128 VecAmalgam::signSwitch2(const __m128& x, const __m128& y, float paramA, float paramB) {
    __m128 midPoint = _mm_set1_ps(paramA * 2.f - 1.f);
    __m128 thresh = _mm_set1_ps(paramB);
    zVec = _mm_switch_ps(zerosVec, x, _mm_cmpgt_ps(x, _mm_add_ps(midPoint, thresh)));
    return _mm_switch_ps(zVec, y, _mm_cmplt_ps(y, _mm_sub_ps(midPoint, thresh)));
}

__m128 VecAmalgam::xFade(const __m128& x, const __m128& y, float paramA, float paramB) {
    __m128 xScaled = _mm_add_ps(_mm_mul_ps(x, halfsVec), halfsVec);
    xScaled = _mm_mul_ps(xScaled, _mm_set1_ps(paramB));
    return _mm_linterp_ps(x,y, _mm_clamp_ps(_mm_add_ps(_mm_set1_ps(paramA), xScaled), zerosVec, onesVec));
}

__m128 VecAmalgam::flipFlop(const __m128& x, const __m128& y, float paramA, float paramB) {
    __m128 thresh = _mm_set1_ps(paramB);
    for(auto i = 0; i < 4; ++i) {
        k_[i] = (float)mwcRand(z_[i], w_[i]) / (float)UINT32_MAX;
    }
    chanceXVec = _mm_loadu_ps(k_);
    chanceXVec = _mm_and_ps(_mm_cmpgt_ps(chanceXVec, _mm_set1_ps(paramA)), highVec);

    for(auto i = 0; i < 4; ++i) {
        k_[i] = (float)mwcRand(z_[i], w_[i]) / (float)UINT32_MAX;
    }
    chanceYVec = _mm_loadu_ps(k_);
    chanceYVec = _mm_and_ps(_mm_cmpgt_ps(chanceYVec, _mm_set1_ps(1.f - paramA)), highVec);

    xREdgeVec = _mm_and_ps(_mm_cmpgt_ps(x, thresh), _mm_cmple_ps(xPrevVec, thresh));
    yREdgeVec = _mm_and_ps(_mm_cmpgt_ps(y, thresh), _mm_cmple_ps(yPrevVec, thresh));
    xREdgeVec = _mm_and_ps(xREdgeVec, chanceXVec);
    yREdgeVec = _mm_and_ps(yREdgeVec, chanceYVec);

    ffTargetVec = _mm_switch_ps(ffTargetVec, zerosVec, xREdgeVec);
    ffTargetVec = _mm_switch_ps(ffTargetVec, highVec, yREdgeVec);
    xPrevVec = x;
    yPrevVec = y;
    return _mm_switch_ps(x, y, ffTargetVec);
}

__m128 VecAmalgam::alphaPWM(const __m128& x, const __m128& y, float paramA, float paramB) {
    zVec = _mm_mul_ps(_mm_abs_ps(x), halfsVec);
    zVec = _mm_mul_ps(zVec, _mm_add_ps(_mm_mul_ps(_mm_set1_ps(paramA), _mm_set1_ps(16.f)), onesVec));
    __m128i xInt = _mm_cvttps_epi32(zVec);
    __m128 xIntF = _mm_cvtepi32_ps(xInt);
    zVec = _mm_sub_ps(zVec, xIntF);
    return _mm_mul_ps(_mm_switch_ps(x, _mm_set1_ps(0.f), _mm_cmpgt_ps(zVec, _mm_set1_ps(1.f - paramB))), y);
}

__m128 VecAmalgam::bitAND(const __m128& x, const __m128& y, float paramA, float paramB) {
    paramA = 1.f - paramA * 0.8f;
    paramA *= paramA;
    downSample(_mm_varStep_ps(x, _mm_set1_ps(1.f - paramA)),_mm_varStep_ps(y, _mm_set1_ps(1.f - paramA)));
    a32Vec = _mm_cvttps_epi32(_mm_mul_ps(xDSVec, _mm_set1_ps(0x7FFFFFFF)));
    b32Vec = _mm_cvttps_epi32(_mm_mul_ps(yDSVec, _mm_set1_ps(0x7FFFFFFF)));
    c32Vec = _mm_and_si128(a32Vec, b32Vec);
    return _mm_div_ps(_mm_cvtepi32_ps(c32Vec), _mm_set1_ps(0x7FFFFFFF));
}

__m128 VecAmalgam::bitXOR(const __m128& x, const __m128& y, float paramA, float paramB) {
    paramA = 1.f - paramA * 0.8f;
    paramA *= paramA;
    downSample(_mm_varStep_ps(x, _mm_set1_ps(1.f - paramA)),_mm_varStep_ps(y, _mm_set1_ps(1.f - paramA)));
    a32Vec = _mm_cvttps_epi32(_mm_mul_ps(xDSVec, _mm_set1_ps(0x7FFFFFFF)));
    b32Vec = _mm_cvttps_epi32(_mm_mul_ps(yDSVec, _mm_set1_ps(0x7FFFFFFF)));
    c32Vec = _mm_xor_si128(a32Vec, b32Vec);
    return _mm_div_ps(_mm_cvtepi32_ps(c32Vec), _mm_set1_ps(0x7FFFFFFF));
}

__m128 VecAmalgam::bitInterleave(const __m128& x, const __m128& y, float paramA, float paramB) {
    paramA = 1.f - paramA * 0.8f;
    paramA *= paramA;
    downSample(_mm_varStep_ps(x, _mm_set1_ps(1.f - paramA)),_mm_varStep_ps(y, _mm_set1_ps(1.f - paramA)));
    a32Vec = _mm_cvttps_epi32(_mm_mul_ps(xDSVec, _mm_set1_ps(0x7FFFFFFF)));
    b32Vec = _mm_cvttps_epi32(_mm_mul_ps(yDSVec, _mm_set1_ps(0x7FFFFFFF)));
    c32Vec = _mm_xor_si128(a32Vec, b32Vec);
    c32Vec = _mm_or_si128(_mm_and_si128(a32Vec, _mm_set1_epi32(0x55555555)), _mm_and_si128(b32Vec, _mm_set1_epi32(0xAAAAAAAA)));
    return _mm_div_ps(_mm_cvtepi32_ps(c32Vec), _mm_set1_ps(0x7FFFFFFF));
}

__m128 VecAmalgam::bitHack(const __m128& x, const __m128& y, float paramA, float paramB) {
    chance32Vec = highVec;
    int random = 0;
    for(auto i = 0; i < 4; ++i) {
        random = mwcRand(z_[i], w_[i]);
        k32_[i] = (float)random / (float)UINT32_MAX > (0.5f - paramA * paramA * 0.5f) ? random : 0xFFFFFFFF;
    }
    chance32Vec = _mm_castsi128_ps(_mm_set_epi32(k32_[3], k32_[2], k32_[1], k32_[0]));

    downSample(x,y);
    a32Vec = _mm_cvttps_epi32(_mm_mul_ps(xDSVec, _mm_set1_ps(0x7FFFFFFF)));
    b32Vec = _mm_cvttps_epi32(_mm_mul_ps(yDSVec, _mm_set1_ps(0x7FFFFFFF)));
    c32Vec = _mm_and_si128(_mm_or_si128(c32Vec, _mm_and_si128(a32Vec, b32Vec)), _mm_castps_si128(chance32Vec)); // Make 1 if a == 1 AND b == 1
    c32Vec = _mm_and_si128(c32Vec, ~_mm_and_si128(_mm_and_si128(~a32Vec, ~b32Vec), _mm_castps_si128(chance32Vec))); // Make 0 if a == 0 AND b == 0
    zVec = _mm_div_ps(_mm_cvtepi32_ps(c32Vec), _mm_set1_ps(0x7FFFFFFF));
    zDSVec = _mm_switch_ps(zDSVec, zVec, sampleVec);
    return zDSVec;
}

__m128 VecAmalgam::bitANDFloat(const __m128& x, const __m128& y, float paramA, float paramB) {
    chance32Vec = highVec;
    uint32_t random = 0;
    for(auto i = 0; i < 4; ++i) {
        random = mwcRand(z_[i], w_[i]);
        k32_[i] = (float)random / (float)UINT32_MAX > (0.5f - paramA * paramA * 0.5f) ? random : 0xFFFFFFFF;
    }
    chance32Vec = _mm_castsi128_ps(_mm_set_epi32(k32_[3], k32_[2], k32_[1], k32_[0]));

    downSample(x,y);
    zVec = _mm_and_ps(_mm_and_ps(xDSVec,yDSVec), chance32Vec);
    zDSVec = _mm_switch_ps(zDSVec, zVec, sampleVec);
    return zDSVec;
}

__m128 VecAmalgam::bitInterleaveFloat(const __m128& x, const __m128& y, float paramA, float paramB) {
    paramA = 1.f - paramA * 0.8f;
    paramA *= paramA;
    downSample(_mm_varStep_ps(x, _mm_set1_ps(1.f - paramA)),_mm_varStep_ps(y, _mm_set1_ps(1.f - paramA)));
    return _mm_or_ps(_mm_and_ps(xDSVec, _mm_castsi128_ps(_mm_set1_epi32(0x55555555))),
                     _mm_and_ps(yDSVec, _mm_castsi128_ps(_mm_set1_epi32(0xAAAAAAAA))));
}

__m128 VecAmalgam::bitHackFloat(const __m128& x, const __m128& y, float paramA, float paramB) {
    chance32Vec = highVec;
    uint32_t random = 0;
    for(auto i = 0; i < 4; ++i) {
        random = mwcRand(z_[i], w_[i]);
        k32_[i] = (float)random / (float)UINT32_MAX > (0.5f - paramA * paramA * 0.5f) ? random : 0xFFFFFFFF;
    }
    chance32Vec = _mm_castsi128_ps(_mm_set_epi32(k32_[3], k32_[2], k32_[1], k32_[0]));

    downSample(x,y);
    zVec = _mm_and_ps(_mm_or_ps(zVec, _mm_and_ps(xDSVec,yDSVec)), chance32Vec);
    zVec = _mm_andnot_ps(_mm_and_ps(_mm_and_ps(_mm_xor_ps(xDSVec, highVec), _mm_xor_ps(yDSVec, highVec)),chance32Vec), zVec);
    zDSVec = _mm_switch_ps(zDSVec, zVec, sampleVec);
    return zDSVec;
}
