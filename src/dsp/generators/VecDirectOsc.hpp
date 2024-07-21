#pragma once
#include "../../simd/SIMDUtilities.hpp"
#include "../filters/VecOnePoleFilters.hpp"

struct VecDirectOsc {
    VecDirectOsc() {
        a = _mm_set1_ps(0.f);
        __saw = _mm_set1_ps(0.f);
        frequency = _mm_set1_ps(440.f);
        phasor = _mm_set1_ps(0.f);
        prevPhasor = _mm_set1_ps(0.f);
        phaseShift = _mm_set1_ps(0.f);

        __flyBack = _mm_set1_ps(0.f);
        __flyForward = _mm_set1_ps(0.f);
        __offset = _mm_set1_ps(0.f);

        __subScale = _mm_set1_ps(1.f);
        __subLimit = _mm_set1_ps(1);
        __subOffsetDegree = _mm_set1_ps(0.f);
        __subWidth = _mm_set1_ps(0.5f);
        __subWidthOffset = _mm_mul_ps(_mm_sub_ps(__subWidth, _mm_set1_ps(0.5f)), _mm_set1_ps(2.f));

        __sawHPF.setCutoffFreq(20.f);
        __pulseHPF.setCutoffFreq(20.f);
        __subSawHPF.setCutoffFreq(20.f);
        __subPulseHPF.setCutoffFreq(20.f);

        setSampleRate(44100.f);
        setSubOctave(0);
    }

    void process() {
        // Wrap the phasor
        a = _mm_sub_ps(a, _mm_and_ps(_mm_set1_ps(1.f), _mm_cmpge_ps(a, _mm_set1_ps(1.f))));
        a = _mm_add_ps(a, _mm_and_ps(_mm_set1_ps(1.f), _mm_cmplt_ps(a, _mm_set1_ps(0.f))));
        phasor = _mm_add_ps(a, phaseShift);
        phasor = _mm_sub_ps(phasor, _mm_and_ps(_mm_set1_ps(1.f), _mm_cmpge_ps(phasor, _mm_set1_ps(1.f))));
        phasor = _mm_add_ps(phasor, _mm_and_ps(_mm_set1_ps(1.f), _mm_cmplt_ps(phasor, _mm_set1_ps(0.f))));

        __flyBack = _mm_cmpge_ps(_mm_sub_ps(prevPhasor, phasor), _mm_set1_ps(0.5f));
        __flyForward = _mm_cmpgt_ps(_mm_sub_ps(phasor, prevPhasor), _mm_set1_ps(0.5f));

        __saw = _mm_sub_ps(_mm_mul_ps(phasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        __saw = _mm_sub_ps(__saw, _mm_polyblep_ps(phasor, _mm_abs_ps(stepSize)));

        // Calculate pulse wave
        __m128 revPhasor = _mm_add_ps(phasor, __pwm);
        revPhasor = _mm_sub_ps(revPhasor, _mm_and_ps(_mm_set1_ps(1.f), _mm_cmpge_ps(revPhasor, _mm_set1_ps(1.f))));
        revPhasor = _mm_sub_ps(_mm_set1_ps(1.f), revPhasor);
        __pulse = _mm_sub_ps(_mm_mul_ps(revPhasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        __pulse = _mm_add_ps(__saw, _mm_sub_ps(__pulse, _mm_polyblep_ps(revPhasor, _mm_abs_ps(stepSize))));
        __pulse = _mm_add_ps(__pulse, _mm_mul_ps(_mm_sub_ps(__pwm, _mm_set1_ps(0.5f)), _mm_set1_ps(2.f)));

        // Derive sub wave
        __offset = _mm_add_ps(__offset, _mm_and_ps(_mm_set1_ps(1.f), __flyBack));
        __offset = _mm_sub_ps(__offset, _mm_and_ps(_mm_set1_ps(1.f), __flyForward));
        __offset = _mm_sub_ps(__offset, _mm_and_ps(__subLimit, _mm_cmpeq_ps(__offset, __subLimit)));
        __offset = _mm_add_ps(__offset, _mm_and_ps(__subLimit, _mm_cmplt_ps(__offset, _mm_set1_ps(0.f))));

        __m128 subPhasor = _mm_add_ps(_mm_mul_ps(phasor, __subScale), _mm_mul_ps(__offset, __subOffsetDegree));
        __m128i xInt = _mm_cvttps_epi32(subPhasor);
        subPhasor = _mm_sub_ps(subPhasor, _mm_cvtepi32_ps(xInt));
        __subSaw = _mm_sub_ps(_mm_mul_ps(subPhasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        __subSaw = _mm_sub_ps(__subSaw, _mm_polyblep_ps(subPhasor, subStepSize));

        subPhasor = _mm_add_ps(subPhasor, __subWidth);
        xInt = _mm_cvttps_epi32(subPhasor);
        subPhasor = _mm_sub_ps(subPhasor, _mm_cvtepi32_ps(xInt));
        subPhasor = _mm_sub_ps(_mm_set1_ps(1.f), subPhasor);
        __subPulse = _mm_sub_ps(_mm_mul_ps(subPhasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        __subPulse = _mm_sub_ps(__subPulse, _mm_polyblep_ps(subPhasor, subStepSize));
        __subPulse = _mm_add_ps(__subPulse, __subSaw);
        __subPulse = _mm_add_ps(__subPulse, __subWidthOffset);

        prevPhasor = phasor;
        a = _mm_add_ps(a, stepSize);

        __saw = __sawHPF.process(__saw);
        __pulse = __pulseHPF.process(__pulse);
        __subSaw = __subSawHPF.process(__subSaw);
        __subPulse = __subPulseHPF.process(__subPulse);
    }

    void setFrequency(const __m128& newFrequency) {
        frequency = newFrequency;
        stepSize = _mm_mul_ps(frequency, sampleTime);
        subStepSize = _mm_mul_ps(_mm_abs_ps(stepSize), __subScale);
    }

    void setSampleRate(float newSampleRate) {
        sampleTime = _mm_set1_ps(1.f / newSampleRate);
        setFrequency(frequency);
        __sawHPF.setSampleRate(newSampleRate);
        __pulseHPF.setSampleRate(newSampleRate);
        __subSawHPF.setSampleRate(newSampleRate);
        __subPulseHPF.setSampleRate(newSampleRate);
    }

    /** Sets the pulse width of the sub pulse wave. */
    void setSubWidth(const __m128& subWidth) {
        __subWidth = subWidth;
        __subWidthOffset = _mm_mul_ps(_mm_sub_ps(__subWidth, _mm_set1_ps(0.5f)), _mm_set1_ps(2.f));
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
        subStepSize = _mm_mul_ps(stepSize, __subScale);
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

    __m128 phasor, a, prevPhasor, phaseShift;
    __m128 frequency, sampleTime, stepSize, subStepSize;
    __m128 __pwm;
    __m128 __saw, __pulse;
    __m128 __subSaw, __subPulse;
    VecOnePoleHPFilter __sawHPF, __pulseHPF, __subSawHPF, __subPulseHPF;
    __m128 __flyBack, __flyForward, __offset, __subLimit, __subScale, __subOffsetDegree;
    __m128 __subWidth, __subWidthOffset;
};
