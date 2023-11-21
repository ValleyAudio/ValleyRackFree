//
//  VecAmalgam.hpp
//  BitCombiner
//
//  Created by Dale Johnson on 26/11/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#ifndef VecAmalgam_hpp
#define VecAmalgam_hpp
#include <ctime>
#include <cmath>
#include "../utilities/Utilities.hpp"
#include "../simd/SIMDUtilities.hpp"
#include "../dsp/filters/VecOnePoleFilters.hpp"
#include "../dsp/fx/VecDiodeRingMod.hpp"

class VecAmalgam {
public:
    enum {
        RING_MOD_1_MODE,
        RING_MOD_2_MODE,
        RING_MOD_3_MODE,
        DIODE_RING_MOD_MODE,
        MIN_MAX_MODE,
        SIGN_SWITCH_1_MODE,
        SIGN_SWITCH_2_MODE,
        X_FADE_MODE,
        FLIP_FLOP_MODE,
        ALPHA_PWM_MODE,
        BIT_AND_MODE,
        BIT_XOR_MODE,
        BIT_INTERLEAVE_MODE,
        BIT_HACK_MODE,

        BIT_AND_FLOAT_MODE,
        BIT_INTERLEAVE_FLOAT_MODE,
        BIT_HACK_FLOAT_MODE,
        NUM_MODES
    } Modes;

    VecAmalgam();

    inline __m128 process(const __m128& x, const __m128& y, float paramA, float paramB) {
        // Calculate sample clock for bit wise ops
        updateRate_ = 1.f - paramB;
        calcStepSize();
        sampleVec = zerosVec;
        step_ += stepSize_;
        if(step_ >= 1.f) {
            step_ -= 1.f;
            sampleVec = highVec;
        }

        xVec = x;
        yVec = y;
        return (this->*p[_mode])(xVec, yVec, paramA, paramB);
    }

    void setMode(int mode);
    void setSampleRate(float sampleRate);

private:
    int _mode;
    __m128 xVec, yVec, zVec;
    __m128 zerosVec, onesVec, negOnesVec, halfsVec, highVec;

    // Ring Mod
    __m128 xFoldedVec, yFoldedVec, xLogicVec, yLogicVec, zLogicVec;

    // Flip Flop
    __m128 ffTargetVec;
    __m128 xPrevVec, yPrevVec, xREdgeVec, yREdgeVec;
    __m128 chanceXVec, chanceYVec;
    uint32_t z_[4];
    uint32_t w_[4];
    float k_[4];

    VecDiodeRingMod diodeRingModCore;

    // 32 bit mode vars
    int k32_[4];
    __m128 chance32Vec;
    __m128i a32Vec, b32Vec, c32Vec;

    // Sample reduction
    float step_, stepSize_, engineSampleRate_, internalSampleRate_;
    float quarterNyquist_, updateRate_;
    __m128 sampleVec;
    __m128 xDSVec, yDSVec, zDSVec;

    void calcStepSize();

    __m128 (VecAmalgam::*p[NUM_MODES])(const __m128& x, const __m128& y, float paramA, float paramB);

    __m128 ringMod1(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 ringMod2(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 ringMod3(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 diodeRingMod(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 minMax(const __m128& x, const __m128& y,float paramA, float paramB);
    __m128 signSwitch1(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 signSwitch2(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 xFade(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 flipFlop(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 alphaPWM(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 bitAND(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 bitXOR(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 bitInterleave(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 bitHack(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 bitANDFloat(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 bitInterleaveFloat(const __m128& x, const __m128& y, float paramA, float paramB);
    __m128 bitHackFloat(const __m128& x, const __m128& y, float paramA, float paramB);

    inline void downSample(const __m128& x, const __m128& y) {
        xDSVec = _mm_switch_ps(xDSVec, x, sampleVec);
        yDSVec = _mm_switch_ps(yDSVec, y, sampleVec);
    }
};
#endif /* VecAmalgam_hpp */
