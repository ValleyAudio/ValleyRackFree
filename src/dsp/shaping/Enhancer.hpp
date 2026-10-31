#pragma once
#include "valley_sse_include.h"
#include <cmath>
#include <cstdint>
#include "../filters/VecOnePoleFilters.hpp"
#include "../shaping/VecNonLinear.hpp"
#include "../../utilities/Utilities.hpp"

class VecEnhancer {
public:
    enum VecEnhancerModes {
        BITCRUSH_MODE = 0,
        QUANTIZE_MODE,
        BIT_AND_MODE,
        BIT_XOR_MODE,
        BIT_AND_FLOAT_MODE,
        OVERDRIVE_MODE,
        MULTIPLY_MODE,
        SHARPEN_MODE,
        CHEBYSHEV_MODE,
        FOLD_MODE,
        MIRROR_MODE,
        NUM_MODES
    };

    __m128 output;

    VecEnhancer() {
        posEpsilon = _mm_set1_ps(0.00001f);
        negEpsilon = _mm_set1_ps(-0.00001f);

        mode = BITCRUSH_MODE;
        counter = _mm_set1_ps(0.f);
        x = _mm_set1_ps(0.f);
        y = _mm_set1_ps(0.f);
        z = _mm_set1_ps(0.f);
        xInt = _mm_set1_epi32(0);
        yInt = _mm_set1_epi32(0);
        zInt = _mm_set1_epi32(0);
        invIntMaxF = _mm_div_ps(_mm_set1_ps(1.f),
                                  _mm_set1_ps((float)0x7FFFFFFF));

        doSample = _mm_set1_ps(0.f);
        updateRate = _mm_set1_ps(1.f);

        output = _mm_set1_ps(0.f);

        std::srand(std::time(NULL));
        for(auto i = 0; i < 4; ++i) {
            randZ[i] = std::rand();
            randW[i] = std::rand();
        }

        setSampleRate(44100.f);

        p[BITCRUSH_MODE] = &VecEnhancer::bitCrush;
        p[QUANTIZE_MODE] = &VecEnhancer::quantize;
        p[BIT_AND_MODE] = &VecEnhancer::bitAND;
        p[BIT_XOR_MODE] = &VecEnhancer::bitXOR;
        p[BIT_AND_FLOAT_MODE] = &VecEnhancer::bitANDFloat;
        p[OVERDRIVE_MODE] = &VecEnhancer::overdrive;
        p[MULTIPLY_MODE] = &VecEnhancer::multiply;
        p[SHARPEN_MODE] = &VecEnhancer::sharpen;
        p[FOLD_MODE] = &VecEnhancer::fold;
        p[CHEBYSHEV_MODE] = &VecEnhancer::chebyshev;
        p[MIRROR_MODE] = &VecEnhancer::mirror;
    }

    inline __m128 process(const __m128& x, const __m128& param) {
        updateRate = _mm_sub_ps(_mm_set1_ps(1.f), param);
        updateRate = _mm_mul_ps(updateRate, updateRate);
        updateRate = _mm_mul_ps(updateRate, updateRate);
        updateRate = _mm_mul_ps(updateRate, _mm_set1_ps(updateRateScale));
        updateRate = _mm_add_ps(updateRate, _mm_set1_ps(updateRateOffset));
        output = (this->*p[mode])(x, param);
        return output;
    }

    void setMode(int newMode) {
        if (mode == newMode) {
            return;
        }

        mode = newMode;
        if(mode < 0) {
            mode = 0;
        }
        else if(mode >= NUM_MODES) {
            mode = NUM_MODES - 1;
        }
    }

    // Reset internal flags and registers
    void reset() {
        counter = _mm_set1_ps(0.f);
        a = _mm_set1_ps(0.f);
        b = _mm_set1_ps(0.f);
        x = _mm_set1_ps(0.f);
        y = _mm_set1_ps(0.f);
        z = _mm_set1_ps(0.f);
    }

    void setSampleRate(float newSampleRate) {
        filter.setSampleRate(newSampleRate);
        updateRateOffset = 0.03f * (44100.f / newSampleRate);
        updateRateScale = 1.f - updateRateOffset;
    }

    void insertAuxSignals(const __m128& auxPhasor, const __m128& auxPhasorStepSize,
                          const __m128& auxEoc, const __m128& auxDirection) {
        phasor = auxPhasor;
        phasorStepSize = auxPhasorStepSize;
        eoc = auxEoc;
        direction = auxDirection;
        phasorStepSize = _mm_mul_ps(auxPhasorStepSize, auxDirection);
    }

private:

    __m128 (VecEnhancer::*p[NUM_MODES])(const __m128& x, const __m128& param);

    __m128 bitCrush(const __m128& x, const __m128& param) {
        counter = _mm_add_ps(counter, updateRate);
        doSample = _mm_and_ps(_mm_cmpge_ps(counter, _mm_set1_ps(1.f)),
                              _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF)));
        counter = _mm_switch_ps(counter,
                                _mm_sub_ps(counter, _mm_set1_ps(1.f)),
                                doSample);
        y = _mm_switch_ps(y, x, doSample);
        return y;
    }

    __m128 quantize(const __m128& x, const __m128& param) {
        a = _mm_sub_ps(_mm_set1_ps(1.f), _mm_mul_ps(param, _mm_set1_ps(0.8f)));
        a = _mm_mul_ps(_mm_mul_ps(a, a), _mm_set1_ps(64.f));
        y = _mm_mul_ps(x, a);
        yInt = _mm_cvttps_epi32(y);
        y = _mm_cvtepi32_ps(yInt);
        y = _mm_div_ps(y, a);
        return _mm_linterp_ps(x, y, _mm_clamp_ps(_mm_mul_ps(param,
                                                            _mm_set1_ps(64.f)),
                                                 _mm_set1_ps(0.f),
                                                 _mm_set1_ps(1.f)));
    }

    __m128 bitANDFloat(const __m128& x, const __m128& param) {
        return _mm_linterp_ps(x, _mm_and_ps(x,phasor), param);
    }

    __m128 bitAND(const __m128& x, const __m128& param) {
        xInt = _mm_cvttps_epi32(_mm_mul_ps(x, _mm_set1_ps(0x7FFFFFFF)));
        yInt = _mm_cvttps_epi32(_mm_mul_ps(phasor, _mm_set1_ps(0x7FFFFFFF)));
        yInt = _mm_and_si128(xInt, yInt);
        y = _mm_mul_ps(_mm_cvtepi32_ps(yInt), invIntMaxF);
        return _mm_linterp_ps(x, y, param);
    }

    __m128 bitXOR(const __m128& x, const __m128& param) {
        xInt = _mm_cvttps_epi32(_mm_mul_ps(x, _mm_set1_ps(0x7FFFFFFF)));
        yInt = _mm_cvttps_epi32(_mm_mul_ps(phasor, _mm_set1_ps(0x7FFFFFFF)));
        yInt = _mm_xor_si128(xInt, yInt);
        a = _mm_mul_ps(_mm_cvtepi32_ps(yInt), invIntMaxF);
        return _mm_linterp_ps(x, a, param);
    }

    __m128 overdrive(const __m128& x, const __m128& param) {
        a = _mm_mul_ps(a, _mm_set1_ps(200.f));
        a = _mm_add_ps(param, _mm_set1_ps(1.f));
        return vecDriveSignal(x, a);
    }

    __m128 multiply(const __m128& x, const __m128& param) {
        a = _mm_mul_ps(param, _mm_set1_ps(16.f));
        y = _mm_mul_ps(x, valley::_mm_cosine_ps(_mm_mul_ps(_mm_circle_ps(_mm_mul_ps(phasor, a)),
                         _mm_set1_ps(M_PI))));
        return _mm_linterp_ps(x, y, _mm_clamp_ps(a, _mm_set1_ps(0.f),
                                                    _mm_set1_ps(1.f)));
    }

    __m128 sharpen(const __m128& x, const __m128& param) {
        y = _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(x, x), x), x);
        y = _mm_switch_ps(y, _mm_mul_ps(y, _mm_set1_ps(-1.f)),
                             _mm_cmplt_ps(x, _mm_set1_ps(0.f)));
        return _mm_linterp_ps(x, y, param);
    }

    __m128 fold(const __m128& x, const __m128& param) {
        y = _mm_add_ps(_mm_mul_ps(x, _mm_set1_ps(0.5f)), _mm_set1_ps(0.5f));
        y = _mm_mirror_ps(y, _mm_add_ps(param, _mm_set1_ps(0.00001f)));
        y = _mm_mul_ps(_mm_add_ps(y, _mm_set1_ps(-0.5f)), _mm_set1_ps(2.f));
        return y;
    }

    // Fold the input signal x with the sine function
    __m128 chebyshev(const __m128& x, const __m128& param) {
        a = _mm_mul_ps(param, _mm_set1_ps(8.f));
        y = valley::_mm_sine_ps(_mm_mul_ps(_mm_circle_ps(_mm_mul_ps(x, a)),
                                           _mm_set1_ps(M_PI)));
        return _mm_linterp_ps(x, y, _mm_clamp_ps(a, _mm_set1_ps(0.f),
                                                    _mm_set1_ps(1.f)));
    }

    /** Similar to folding, but the wave arrives wraps round to negative. Uses cross fadeing to tame
        the buzzing.
    */
    __m128 mirror(const __m128& x, const __m128& param) {
        a = _mm_mul_ps(param, _mm_set1_ps(4.f));
        aInt = _mm_cvttps_epi32(_mm_add_ps(a, _mm_set1_ps(1.f)));
        a = _mm_wrap_1_ps(a);
        b = _mm_cvtepi32_ps(aInt);
        b = _mm_sub_ps(b, _mm_set1_ps(0.000001f));
        return _mm_linterp_ps(_mm_circle_ps(_mm_mul_ps(x, b)),
                              _mm_circle_ps(_mm_mul_ps(x, _mm_add_ps(b, _mm_set1_ps(1.f)))),
                              a);
    }

    /** Saw sub-oscillator that is derived from and external phasor using piecewise transform
        to create a sub-octave phasor, which is then anti-aliased using a PolyBLEP
    */
    __m128 subSaw(const __m128& x, const __m128& param) {
        filter.setCutoffFreq(_mm_mul_ps(_mm_mul_ps(param, param),
                                        _mm_set1_ps(22050.f)));
        counter = _mm_add_ps(counter, _mm_and_ps(_mm_set1_ps(1.f), eoc));
        counter = _mm_switch_ps(counter,
                                _mm_set1_ps(0.f),
                                _mm_cmpgt_ps(counter, _mm_set1_ps(1.f)));

        a = _mm_mul_ps(phasor, _mm_set1_ps(0.5f));
        a = _mm_add_ps(a, _mm_mul_ps(counter, _mm_set1_ps(0.5f)));
        y = _mm_mul_ps(a, _mm_set1_ps(2.f));
        y = _mm_sub_ps(y, _mm_set1_ps(1.f));
        y = _mm_sub_ps(y, _mm_polyblep_ps(a, phasorStepSize));

        return _mm_add_ps(x, _mm_mul_ps(y, param));
    }

    /** Similar to above except a square wave is further derived from the sub-octave phasor and
        anti-aliased using two PolyBLEPs
    */
    __m128 subSquare(const __m128& x, const __m128& param) {
        filter.setCutoffFreq(_mm_mul_ps(_mm_mul_ps(param, param),
                                        _mm_set1_ps(22050.f)));
        counter = _mm_add_ps(counter, _mm_and_ps(_mm_set1_ps(1.f), eoc));
        counter = _mm_switch_ps(counter,
                                _mm_set1_ps(0.f),
                                _mm_cmpgt_ps(counter, _mm_set1_ps(1.f)));

        a = _mm_mul_ps(phasor, _mm_set1_ps(0.5f));
        a = _mm_add_ps(a, _mm_mul_ps(counter, _mm_set1_ps(0.5f)));
        b = _mm_add_ps(a, _mm_set1_ps(0.5f));
        b = _mm_sub_ps(b, _mm_and_ps(_mm_set1_ps(1.f),
                                     _mm_cmpge_ps(b, _mm_set1_ps(1.f))));
        y = _mm_switch_ps(_mm_set1_ps(-1.f),
                          _mm_set1_ps(1.f),
                          _mm_cmplt_ps(a, _mm_set1_ps(0.5f)));
        y = _mm_add_ps(y, _mm_polyblep_ps(a, phasorStepSize));
        y = _mm_sub_ps(y, _mm_polyblep_ps(b, phasorStepSize));

        return _mm_add_ps(x, _mm_mul_ps(y, param));
    }

    /** Using a similar principle to the previous sub-octave methods, make a sub-octave phasor, then
        shape it using a Chebyshev shaper
    */
    __m128 subChebyshev(const __m128& x, const __m128& param) {
        counter = _mm_add_ps(counter, eoc);
        counter = _mm_sub_ps(counter,
                             _mm_and_ps(_mm_set1_ps(2.f),
                             _mm_cmpeq_ps(counter, _mm_set1_ps(2.f))));

        a = _mm_switch_ps(phasor,
                          _mm_sub_ps(_mm_set1_ps(1.f), phasor),
                          _mm_cmpeq_ps(counter, _mm_set1_ps(1.f)));
        y = _mm_mul_ps(a, _mm_set1_ps(2.f));
        y = _mm_sub_ps(y, _mm_set1_ps(1.f));

        b = _mm_mul_ps(param, _mm_set1_ps(8.f));
        y = valley::_mm_sine_ps(_mm_mul_ps(_mm_circle_ps(_mm_mul_ps(y, b)),
                                           _mm_set1_ps(M_PI)));
        return _mm_add_ps(x, _mm_mul_ps(y, _mm_clamp_ps(b,
                                                        _mm_set1_ps(0.f),
                                                        _mm_set1_ps(1.f))));
    }

    int mode = -1;
    float updateRateScale, updateRateOffset;
    __m128 updateRate, counter, doSample;
    __m128 posEpsilon, negEpsilon;
    __m128 a, b, x, y, z;
    __m128i xInt, yInt, zInt, aInt;
    __m128 invIntMaxF;
    __m128 phasor, phasorStepSize, eoc, direction;

    VecOnePoleLPFilter filter;

    uint32_t randZ[4] = {0};
    uint32_t randW[4] = {0};
};
