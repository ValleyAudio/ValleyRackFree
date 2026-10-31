//
//  QuadOsc.hpp
//  QuadOsc - A syncronous, SIMD optimised oscillator.
//
//  Created by Dale Johnson on 02/02/2018.
//  Copyright © 2018 Dale Johnson. All rights reserved.
//

#pragma once

#include "valley_sse_include.h"
#include <cmath>
#include <cstdint>
#include <ctime>
#include <vector>
#include "../../simd/SIMDUtilities.hpp"
#include "../../utilities/Utilities.hpp"
#include "../shaping/Shaper.hpp"

class ScanningQuadOsc {
public:
    __m128 inputPhase;

    enum SyncModes {
        HARD_SYNC = 0,
        FIFTH_SYNC,
        OCTAVE_SYNC,
        SUB_OCTAVE_SYNC,
        RISE_A_SYNC,
        RISE_B_SYNC,
        FALL_A_SYNC,
        FALL_B_SYNC,
        PULL_A_SYNC,
        PULL_B_SYNC,
        PUSH_A_SYNC,
        PUSH_B_SYNC,
        HOLD_SYNC,
        ONE_SHOT_SYNC,
        LOCK_SHOT_SYNC,
        REVERSE_SYNC,
        NUM_SYNC_MODES
    };

    ScanningQuadOsc();
    ScanningQuadOsc(const ScanningQuadOsc&) = delete;
    ScanningQuadOsc& operator=(const ScanningQuadOsc&) = delete;

    void tick();
    void resetPhase();
    void sync(const __m128& syncSource);

    float getOutput(int channel) const;
    const __m128& getOutput() const;
    const __m128& getPhasor() const;
    const __m128& getStepSize() const;
    const __m128& getShapedPhasor() const;
    const __m128& getEOCPulse() const;
    const __m128& getDirection() const;

    void setFrequency(float newFrequency);
    void setFrequency(float f0, float f1, float f2, float f3);
    void setFrequency(const __m128& newFrequency);

    void setShape(float newShape);
    void setShape(const __m128& vNewShape);
    void setShapeMode(int shapeMode);
    void setPhaseModPostPhasorShaping(bool phaseModShouldBePostPhasorShaping);

    void setSyncMode(int newSyncMode);
    void enableSync(bool enableSync);

    void setSampleRate(float sampleRate);

    void setWavebank(float* newWavebank,
                     int32_t newWavebankNumWaves,
                     int32_t tableSize);
    void setScanPosition(float position);
    void mm_setScanPosition(const __m128& position);
    int32_t getNumwaves() const;

private:
    float* wavebank;
    int32_t numWaves;
    __m128i vNumWaves, vNumWaves_1;

    int32_t lowBank[4] = {0, 0, 0, 0};
    int32_t highBank[4] = {0, 0, 0, 0};
    float lowSample2[4] = {0.f, 0.f, 0.f, 0.f};
    float highSample2[4] = {0.f, 0.f, 0.f, 0.f};
    __m128 vFade, vResult1, vResult2;
    __m128i vLowBank, vHighBank;

    float lowSample[4] = {0.f, 0.f, 0.f, 0.f};
    float highSample[4] = {0.f, 0.f, 0.f, 0.f};
    int32_t tableSize;
    float shape;
    bool doPhaseModPostPhasorShaping;
    int32_t syncMode;
    bool syncIsEnabled;

    __m128 vFrequency, vSamplerate, vNyquist;

    __m128 vReadPhase, vNegMask, vSyncSource, vSyncState, vSyncing;
    __m128 vShifts;
    __m128i vShiftsI;

    __m128 vShape;
    __m128 vA, vB, vStepSize, vTableSize, vTableSize_1;
    __m128 vPhasorDirection;
    __m128 vMoreThanMask, vLessThanMask;
    __m128 vMask, vSub, vEndOfCycle, vSyncOut;
    __m128i vAInt;
    __m128i vBInt;

    __m128 vLowerValue, vUpperValue, vInterpFraction, vOutput, vOutputLevel;
    Shaper shaper;
    void calcStepSize();
    void onChangeSyncMode();
    void hardSync(const __m128& syncSource);
    void softSync(const __m128& syncSource);
    void reverseSync(const __m128& syncSource);
    void octaveSync(const __m128& syncSource);
    void fifthSync(const __m128& syncSource);
    void subOctaveSync(const __m128& syncSource);
    void riseASync(const __m128& syncSource);
    void riseBSync(const __m128& syncSource);
    void fallASync(const __m128& syncSource);
    void fallBSync(const __m128& syncSource);
    void pullASync(const __m128& syncSource);
    void pullBSync(const __m128& syncSource);
    void pushASync(const __m128& syncSource);
    void pushBSync(const __m128& syncSource);
    void holdSync(const __m128& syncSource);
    void oneShot(const __m128& syncSource);
    void lockShot(const __m128& syncSource);
};

