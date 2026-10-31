#pragma once
#include "../../simd/SIMDUtilities.hpp"
#include "../filters/VecOnePoleFilters.hpp"

struct VecDirectOsc {
    VecDirectOsc() {
        a = _mm_set1_ps(0.f);
        saw = _mm_set1_ps(0.f);
        frequency = _mm_set1_ps(440.f);
        phasor = _mm_set1_ps(0.f);
        prevPhasor = _mm_set1_ps(0.f);
        phaseShift = _mm_set1_ps(0.f);

        flyBack = _mm_set1_ps(0.f);
        flyForward = _mm_set1_ps(0.f);
        offset = _mm_set1_ps(0.f);

        subScale = _mm_set1_ps(1.f);
        subLimit = _mm_set1_ps(1);
        subOffsetDegree = _mm_set1_ps(0.f);
        subPulseWidth = _mm_set1_ps(0.5f);
        subWidthOffset = _mm_mul_ps(_mm_sub_ps(subPulseWidth, _mm_set1_ps(0.5f)), _mm_set1_ps(2.f));

        sawHpf.setCutoffFreq(20.f);
        pulseHpf.setCutoffFreq(20.f);
        subSawHpf.setCutoffFreq(20.f);
        subPulseHpf.setCutoffFreq(20.f);

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

        flyBack = _mm_cmpge_ps(_mm_sub_ps(prevPhasor, phasor), _mm_set1_ps(0.5f));
        flyForward = _mm_cmpgt_ps(_mm_sub_ps(phasor, prevPhasor), _mm_set1_ps(0.5f));

        saw = _mm_sub_ps(_mm_mul_ps(phasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        saw = _mm_sub_ps(saw, _mm_polyblep_ps(phasor, _mm_abs_ps(stepSize)));

        // Calculate pulse wave
        __m128 revPhasor = _mm_add_ps(phasor, pwm);
        revPhasor = _mm_sub_ps(revPhasor, _mm_and_ps(_mm_set1_ps(1.f), _mm_cmpge_ps(revPhasor, _mm_set1_ps(1.f))));
        revPhasor = _mm_sub_ps(_mm_set1_ps(1.f), revPhasor);
        pulse = _mm_sub_ps(_mm_mul_ps(revPhasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        pulse = _mm_add_ps(saw, _mm_sub_ps(pulse, _mm_polyblep_ps(revPhasor, _mm_abs_ps(stepSize))));
        pulse = _mm_add_ps(pulse, _mm_mul_ps(_mm_sub_ps(pwm, _mm_set1_ps(0.5f)), _mm_set1_ps(2.f)));

        // Derive sub wave
        offset = _mm_add_ps(offset, _mm_and_ps(_mm_set1_ps(1.f), flyBack));
        offset = _mm_sub_ps(offset, _mm_and_ps(_mm_set1_ps(1.f), flyForward));
        offset = _mm_sub_ps(offset, _mm_and_ps(subLimit, _mm_cmpeq_ps(offset, subLimit)));
        offset = _mm_add_ps(offset, _mm_and_ps(subLimit, _mm_cmplt_ps(offset, _mm_set1_ps(0.f))));

        __m128 subPhasor = _mm_add_ps(_mm_mul_ps(phasor, subScale), _mm_mul_ps(offset, subOffsetDegree));
        __m128i xInt = _mm_cvttps_epi32(subPhasor);
        subPhasor = _mm_sub_ps(subPhasor, _mm_cvtepi32_ps(xInt));
        subSaw = _mm_sub_ps(_mm_mul_ps(subPhasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        subSaw = _mm_sub_ps(subSaw, _mm_polyblep_ps(subPhasor, subStepSize));

        subPhasor = _mm_add_ps(subPhasor, subPulseWidth);
        xInt = _mm_cvttps_epi32(subPhasor);
        subPhasor = _mm_sub_ps(subPhasor, _mm_cvtepi32_ps(xInt));
        subPhasor = _mm_sub_ps(_mm_set1_ps(1.f), subPhasor);
        subPulse = _mm_sub_ps(_mm_mul_ps(subPhasor, _mm_set1_ps(2.f)), _mm_set1_ps(1.f));
        subPulse = _mm_sub_ps(subPulse, _mm_polyblep_ps(subPhasor, subStepSize));
        subPulse = _mm_add_ps(subPulse, subSaw);
        subPulse = _mm_add_ps(subPulse, subWidthOffset);

        prevPhasor = phasor;
        a = _mm_add_ps(a, stepSize);

        saw = sawHpf.process(saw);
        pulse = pulseHpf.process(pulse);
        subSaw = subSawHpf.process(subSaw);
        subPulse = subPulseHpf.process(subPulse);
    }

    void setFrequency(const __m128& newFrequency) {
        frequency = newFrequency;
        stepSize = _mm_mul_ps(frequency, sampleTime);
        subStepSize = _mm_mul_ps(_mm_abs_ps(stepSize), subScale);
    }

    void setSampleRate(float newSampleRate) {
        sampleTime = _mm_set1_ps(1.f / newSampleRate);
        setFrequency(frequency);
        sawHpf.setSampleRate(newSampleRate);
        pulseHpf.setSampleRate(newSampleRate);
        subSawHpf.setSampleRate(newSampleRate);
        subPulseHpf.setSampleRate(newSampleRate);
    }

    /** Sets the pulse width of the sub pulse wave. */
    void setSubWidth(const __m128& newSubPulseWidth) {
        subPulseWidth = newSubPulseWidth;
        subWidthOffset = _mm_mul_ps(_mm_sub_ps(subPulseWidth, _mm_set1_ps(0.5f)), _mm_set1_ps(2.f));
    }

    /**
     * Set sub wave coefficients. The sub wave is derived using segmented pieces of the main phasor
     * which are rescaled and offset to build a new phasor at a different frequency.
     */
    void setSubOctave(int octave) {
        switch(octave) {
            case SUB_2_OCTAVE_HARMONIC:
                subScale = _mm_set1_ps(0.25f);
                subLimit = _mm_set1_ps(4);
                subOffsetDegree = _mm_set1_ps(0.25f);
                break;
            case SUB_OCTAVE_HARMONIC:
                subScale = _mm_set1_ps(0.5f);
                subLimit = _mm_set1_ps(2);
                subOffsetDegree = _mm_set1_ps(0.5f);
                break;
            case SUB_FIFTH_HARMONIC:
                subScale = _mm_set1_ps(0.75f);
                subLimit = _mm_set1_ps(4);
                subOffsetDegree = _mm_set1_ps(0.75);
                break;
            case ZEROETH_HARMONIC:
                subScale = _mm_set1_ps(1.f);
                subLimit = _mm_set1_ps(1);
                subOffsetDegree = _mm_set1_ps(0.f);
                break;
            case FIFTH_HARMONIC:
                subScale = _mm_set1_ps(1.5f);
                subLimit = _mm_set1_ps(2);
                subOffsetDegree = _mm_set1_ps(0.5f);
                break;
            case OCTAVE_HARMONIC:
                subScale = _mm_set1_ps(2.f);
                subLimit = _mm_set1_ps(1);
                subOffsetDegree = _mm_set1_ps(0.f);
                break;
            case DOUBLE_OCTAVE_HARMONIC:
                subScale = _mm_set1_ps(4.f);
                subLimit = _mm_set1_ps(1);
                subOffsetDegree = _mm_set1_ps(0.f);
                break;
            default:
                subScale = _mm_set1_ps(1.f);
                subLimit = _mm_set1_ps(1);
                subOffsetDegree = _mm_set1_ps(0.f);
        }
        subStepSize = _mm_mul_ps(stepSize, subScale);
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
    __m128 pwm;
    __m128 saw, pulse;
    __m128 subSaw, subPulse;
    VecOnePoleHPFilter sawHpf, pulseHpf, subSawHpf, subPulseHpf;
    __m128 flyBack, flyForward, offset, subLimit, subScale, subOffsetDegree;
    __m128 subPulseWidth, subWidthOffset;
};
