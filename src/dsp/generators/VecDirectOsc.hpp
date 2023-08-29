#pragma once
#include "../../simd/SIMDUtilities.hpp"
#include "../filters/VecOnePoleFilters.hpp"

struct VecDirectOsc {
    VecDirectOsc() {
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __twos = _mm_set1_ps(2.f);
        __halfs = _mm_set1_ps(0.5f);
        __threeQuarters = _mm_set1_ps(0.75f);

        __a = __zeros;
        __saw = __zeros;
        __frequency = _mm_set1_ps(440.f);
        __phasor = __zeros;
        __prevPhasor = __zeros;
        __revPhasor = __zeros;
        __phaseShift = __zeros;

        __flyBack = __zeros;
        __flyForward = __zeros;
        __offset = __zeros;

        __subScale = _mm_set1_ps(1.f);
        __subLimit = _mm_set1_ps(1);
        __subOffsetDegree = _mm_set1_ps(0.f);
        __subWidth = __halfs;
        __subWidthOffset = _mm_mul_ps(_mm_sub_ps(__subWidth, __halfs), __twos);

        __sawHPF.setCutoffFreq(20.f);
        __pulseHPF.setCutoffFreq(20.f);
        __subSawHPF.setCutoffFreq(20.f);
        __subPulseHPF.setCutoffFreq(20.f);

        setSampleRate(44100.f);
        setSubOctave(0);
    }

    void process() {
        // Wrap the phasor
        __a = _mm_sub_ps(__a, _mm_and_ps(__ones, _mm_cmpge_ps(__a, __ones)));
        __a = _mm_add_ps(__a, _mm_and_ps(__ones, _mm_cmplt_ps(__a, __zeros)));
        __phasor = _mm_add_ps(__a, __phaseShift);
        __phasor = _mm_sub_ps(__phasor, _mm_and_ps(__ones, _mm_cmpge_ps(__phasor, __ones)));
        __phasor = _mm_add_ps(__phasor, _mm_and_ps(__ones, _mm_cmplt_ps(__phasor, __zeros)));

        __flyBack = _mm_cmpge_ps(_mm_sub_ps(__prevPhasor, __phasor), __halfs);
        __flyForward = _mm_cmpgt_ps(_mm_sub_ps(__phasor, __prevPhasor), __halfs);

        __saw = _mm_sub_ps(_mm_mul_ps(__phasor, __twos), __ones);
        __saw = _mm_sub_ps(__saw, _mm_polyblep_ps(__phasor, _mm_abs_ps(__stepSize)));

        // Calculate pulse wave
        __revPhasor = _mm_add_ps(__phasor, __pwm);
        __revPhasor = _mm_sub_ps(__revPhasor, _mm_and_ps(__ones, _mm_cmpge_ps(__revPhasor, __ones)));
        __revPhasor = _mm_sub_ps(__ones, __revPhasor);
        __pulse = _mm_sub_ps(_mm_mul_ps(__revPhasor, __twos), __ones);
        __pulse = _mm_add_ps(__saw, _mm_sub_ps(__pulse, _mm_polyblep_ps(__revPhasor, _mm_abs_ps(__stepSize))));
        __pulse = _mm_add_ps(__pulse, _mm_mul_ps(_mm_sub_ps(__pwm, __halfs), __twos));

        // Derive sub wave
        __offset = _mm_add_ps(__offset, _mm_and_ps(__ones, __flyBack));
        __offset = _mm_sub_ps(__offset, _mm_and_ps(__ones, __flyForward));
        __offset = _mm_sub_ps(__offset, _mm_and_ps(__subLimit, _mm_cmpeq_ps(__offset, __subLimit)));
        __offset = _mm_add_ps(__offset, _mm_and_ps(__subLimit, _mm_cmplt_ps(__offset, __zeros)));

        __subPhasor = _mm_add_ps(_mm_mul_ps(__phasor, __subScale), _mm_mul_ps(__offset, __subOffsetDegree));
        __xInt = _mm_cvttps_epi32(__subPhasor);
        __subPhasor = _mm_sub_ps(__subPhasor, _mm_cvtepi32_ps(__xInt));
        __subSaw = _mm_sub_ps(_mm_mul_ps(__subPhasor, __twos), __ones);
        __subSaw = _mm_sub_ps(__subSaw, _mm_polyblep_ps(__subPhasor, __subStepSize));

        __subPhasor = _mm_add_ps(__subPhasor, __subWidth);
        __xInt = _mm_cvttps_epi32(__subPhasor);
        __subPhasor = _mm_sub_ps(__subPhasor, _mm_cvtepi32_ps(__xInt));
        __subPhasor = _mm_sub_ps(__ones, __subPhasor);
        __subPulse = _mm_sub_ps(_mm_mul_ps(__subPhasor, __twos), __ones);
        __subPulse = _mm_sub_ps(__subPulse, _mm_polyblep_ps(__subPhasor, __subStepSize));
        __subPulse = _mm_add_ps(__subPulse, __subSaw);
        __subPulse = _mm_add_ps(__subPulse, __subWidthOffset);

        __prevPhasor = __phasor;
        __a = _mm_add_ps(__a, __stepSize);

        __saw = __sawHPF.process(__saw);
        __pulse = __pulseHPF.process(__pulse);
        __subSaw = __subSawHPF.process(__subSaw);
        __subPulse = __subPulseHPF.process(__subPulse);
    }

    void setFrequency(const __m128& frequency) {
        __frequency = frequency;
        __stepSize = _mm_mul_ps(__frequency, __1_sampleRate);
        __subStepSize = _mm_mul_ps(_mm_abs_ps(__stepSize), __subScale);
    }

    void setSampleRate(float sampleRate) {
        __1_sampleRate = _mm_set1_ps(1.f / sampleRate);
        setFrequency(__frequency);
        __sawHPF.setSampleRate(sampleRate);
        __pulseHPF.setSampleRate(sampleRate);
        __subSawHPF.setSampleRate(sampleRate);
        __subPulseHPF.setSampleRate(sampleRate);
    }

    /** Sets the pulse width of the sub pulse wave. */
    void setSubWidth(const __m128& subWidth) {
        __subWidth = subWidth;
        __subWidthOffset = _mm_mul_ps(_mm_sub_ps(__subWidth, __halfs), __twos);
    }

    /**
     * Set sub wave coefficients. The sub wave is derived using segmented pieces of the main phasor
     * which are rescaled and offset to build a new phasor at a different frequency.
     */
    void setSubOctave(int octave) {
        switch(octave) {
            case SUB_2_OCTAVE_HARMONIC:
                __subScale = _mm_set1_ps(0.25f);
                __subLimit = _mm_set1_ps(4);
                __subOffsetDegree = _mm_set1_ps(0.25f);
                break;
            case SUB_OCTAVE_HARMONIC:
                __subScale = _mm_set1_ps(0.5f);
                __subLimit = _mm_set1_ps(2);
                __subOffsetDegree = _mm_set1_ps(0.5f);
                break;
            case SUB_FIFTH_HARMONIC:
                __subScale = _mm_set1_ps(0.75f);
                __subLimit = _mm_set1_ps(4);
                __subOffsetDegree = _mm_set1_ps(0.75);
                break;
            case ZEROETH_HARMONIC:
                __subScale = _mm_set1_ps(1.f);
                __subLimit = _mm_set1_ps(1);
                __subOffsetDegree = _mm_set1_ps(0.f);
                break;
            case FIFTH_HARMONIC:
                __subScale = _mm_set1_ps(1.5f);
                __subLimit = _mm_set1_ps(2);
                __subOffsetDegree = _mm_set1_ps(0.5f);
                break;
            case OCTAVE_HARMONIC:
                __subScale = _mm_set1_ps(2.f);
                __subLimit = _mm_set1_ps(1);
                __subOffsetDegree = _mm_set1_ps(0.f);
                break;
            case DOUBLE_OCTAVE_HARMONIC:
                __subScale = _mm_set1_ps(4.f);
                __subLimit = _mm_set1_ps(1);
                __subOffsetDegree = _mm_set1_ps(0.f);
                break;
            default:
                __subScale = _mm_set1_ps(1.f);
                __subLimit = _mm_set1_ps(1);
                __subOffsetDegree = _mm_set1_ps(0.f);
        }
        __subStepSize = _mm_mul_ps(__stepSize, __subScale);
    }

    enum Harmonic {
        SUB_2_OCTAVE_HARMONIC = 0,
        SUB_OCTAVE_HARMONIC,
        SUB_FIFTH_HARMONIC,
        ZEROETH_HARMONIC,
        FIFTH_HARMONIC,
        OCTAVE_HARMONIC,
        DOUBLE_OCTAVE_HARMONIC,
        NUM_HARMONICS
    };

    __m128 __phasor, __a, __revPhasor, __prevPhasor, __phaseShift;
    __m128 __subPhasor;
    __m128 __frequency, __1_sampleRate, __stepSize, __subStepSize;
    __m128 __pwm;
    __m128 __saw, __pulse;
    __m128 __subSaw, __subPulse;
    VecOnePoleHPFilter __sawHPF, __pulseHPF, __subSawHPF, __subPulseHPF;
    __m128 __flyBack, __flyForward, __offset, __subLimit, __subScale, __subOffsetDegree;
    __m128 __subWidth, __subWidthOffset;

    __m128 __zeros, __ones, __twos, __halfs, __threeQuarters;
    __m128i __xInt;
    float delta[4];
    float gamma[4];
    float chi[4];
};
