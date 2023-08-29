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
    __m128 __inputPhase;

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

    void setWavetable(float* wavetable, long size);
    void setFrequency(float frequency);
    void setFrequency(float f0, float f1, float f2, float f3);
    void setFrequency(const __m128& frequency);

    void setShape(float shape);
    void setShape(const __m128& shape);
    void setShapeMethod(int shapeMethod);
    void setPMPostShape(bool PMPostShape);

    void setSyncMode(int syncMode);
    void enableSync(bool enableSync);
    void enableWeakSync(bool weakSync);

    void setSampleRate(float sampleRate);

    // void setWavebank(float** wavebank, int32_t numWaves, int32_t tableSize);
    void setWavebank(float* wavebank, int32_t numWaves, int32_t tableSize);
    void setScanPosition(float position);
    void mm_setScanPosition(const __m128& position);
    int32_t getNumwaves() const;
private:
    float* _wavebank;
    int32_t _numWaves;
    __m128i __numWaves, __numWaves_1;

    int32_t _lowBank[4] = {0, 0, 0, 0};
    int32_t _highBank[4] = {0, 0, 0, 0};
    float _lowSample2[4] = {0.f, 0.f, 0.f, 0.f};
    float _highSample2[4] = {0.f, 0.f, 0.f, 0.f};
    __m128 __fade, __result1, __result2;
    __m128i __lowBank, __highBank;
    float* _wavetable;
    float _frequency;

    int32_t _aPos[4] = {0, 0, 0, 0};
    int32_t _bPos[4] = {0, 0, 0, 0};
    float _lowSample[4] = {0.f, 0.f, 0.f, 0.f};
    float _highSample[4] = {0.f, 0.f, 0.f, 0.f};
    float _output[4] = {0.f, 0.f, 0.f, 0.f};
    int32_t _tabSize;
    float _shape;
    bool _PMPostShape;
    int32_t _syncMode;
    bool _sync, _weakSync;

    __m128 __frequency, __samplerate, __nyquist;

    __m128 __readPhase, __negMask, __syncSource, __syncState, __syncing;
    __m128 __shifts;
    __m128i __shiftsI;

    __m128 __shape;
    __m128 __a, __aPrev, __b, __stepSize, __tabSize, __tabSize_1;
    __m128 __ones, __zeros, __twos, __fours, __minus, __1_5, __half, __quarter;
    __m128 __dir;
    __m128 __mtMask, __ltMask;
    __m128 __mask, __sub, __eoc, __syncOut;
    __m128i __aInt;
    __m128i __bInt;

    __m128 __lowSamp, __highSamp, __frac, __output, __outputLevel;
    Shaper _shaper;
    void calcStepSize();
    __m128 (*shapeMethod)(const __m128&, const __m128&);
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

