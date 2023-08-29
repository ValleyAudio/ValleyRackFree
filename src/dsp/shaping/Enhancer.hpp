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
        BITCRUSH_MODE,
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
        __zeros = _mm_set1_ps(0.f);
        __ones = _mm_set1_ps(1.f);
        __twos = _mm_set1_ps(2.f);
        __fours = _mm_set1_ps(4.f);
        __eights = _mm_set1_ps(8.f);
        __sixteens = _mm_set1_ps(16.f);
        __sixtyFour = _mm_set1_ps(64.f);
        _twoHundred = _mm_set1_ps(200.f);

        __negOnes = _mm_set1_ps(-1.f);
        __halfs = _mm_set1_ps(0.5f);
        __quarters = _mm_set1_ps(0.25f);
        __eighths = _mm_set1_ps(0.125f);

        __posEpsilon = _mm_set1_ps(0.00001f);
        __negEpsilon = _mm_set1_ps(-0.00001f);

        __high = _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF));
        __flipFlop = __zeros;
        __trig = __zeros;
        __prev = __zeros;

        _mode = BITCRUSH_MODE;
        __counter = __zeros;
        __x = __zeros;
        __y = __zeros;
        __z = __zeros;
        __xInt = _mm_set1_epi32(0);
        __yInt = _mm_set1_epi32(0);
        __zInt = _mm_set1_epi32(0);
        __intMaxF = _mm_set1_ps(0x7FFFFFFF);
        __invIntMaxF = _mm_div_ps(__ones, __intMaxF);

        __doSample = __zeros;
        __updateRate = __ones;

        output = __zeros;

        std::srand(std::time(NULL));
        for(auto i = 0; i < 4; ++i) {
            _randZ[i] = std::rand();
            _randW[i] = std::rand();
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
        __updateRate = _mm_sub_ps(__ones, param);
        __updateRate = _mm_mul_ps(__updateRate, __updateRate);
        __updateRate = _mm_mul_ps(__updateRate, __updateRate);
        __updateRate = _mm_mul_ps(__updateRate, _mm_set1_ps(updateRateScale));
        __updateRate = _mm_add_ps(__updateRate, _mm_set1_ps(updateRateOffset));
        output = (this->*p[_mode])(x, param);
        return output;
    }

    void setMode(int mode) {
        if (mode == _mode) {
            return;
        }

        _mode = mode;
        if(_mode < 0) {
            _mode = 0;
        }
        else if(_mode >= NUM_MODES) {
            _mode = NUM_MODES - 1;
        }
    }

    // Reset internal flags and registers
    void reset() {
        __counter = __zeros;
        __a = __zeros;
        __b = __zeros;
        __x = __zeros;
        __y = __zeros;
        __z = __zeros;
        __flipFlop = __zeros;
        __trig = __zeros;
        __prev = __zeros;
    }

    void setSampleRate(float sampleRate) {
        __filter.setSampleRate(sampleRate);
        updateRateOffset = 0.03f * (44100.f / sampleRate);
        updateRateScale = 1.f - updateRateOffset;
    }

    void insertAuxSignals(const __m128& phasor, const __m128& phasorStepSize,
                          const __m128& eoc, const __m128& direction) {
        __phasor = phasor;
        __phasorStepSize = phasorStepSize;
        __eoc = eoc;
        __direction = direction;
        __phasorStepSize = _mm_mul_ps(__phasorStepSize, __direction);
    }

private:

    __m128 (VecEnhancer::*p[NUM_MODES])(const __m128& x, const __m128& param);

    __m128 bitCrush(const __m128& x, const __m128& param) {
        __counter = _mm_add_ps(__counter, __updateRate);
        __doSample = _mm_and_ps(_mm_cmpge_ps(__counter, __ones), __high);
        __counter = _mm_switch_ps(__counter, _mm_sub_ps(__counter, __ones), __doSample);
        __y = _mm_switch_ps(__y, x, __doSample);
        return __y;
    }

    __m128 quantize(const __m128& x, const __m128& param) {
        __a = _mm_sub_ps(__ones, _mm_mul_ps(param, _mm_set1_ps(0.8f)));
        __a = _mm_mul_ps(_mm_mul_ps(__a, __a), __sixtyFour);
        __y = _mm_mul_ps(x, __a);
        __yInt = _mm_cvttps_epi32(__y);
        __y = _mm_cvtepi32_ps(__yInt);
        __y = _mm_div_ps(__y, __a);
        return _mm_linterp_ps(x, __y, _mm_clamp_ps(_mm_mul_ps(param, __sixtyFour), __zeros, __ones));
    }

    __m128 bitANDFloat(const __m128& x, const __m128& param) {
        return _mm_linterp_ps(x, _mm_and_ps(x,__phasor), param);
    }

    __m128 bitAND(const __m128& x, const __m128& param) {
        __xInt = _mm_cvttps_epi32(_mm_mul_ps(x, __intMaxF));
        __yInt = _mm_cvttps_epi32(_mm_mul_ps(__phasor, __intMaxF));
        __yInt = _mm_and_si128(__xInt, __yInt);
        __y = _mm_mul_ps(_mm_cvtepi32_ps(__yInt), __invIntMaxF);
        return _mm_linterp_ps(x, __y, param);
    }

    __m128 bitXOR(const __m128& x, const __m128& param) {
        __xInt = _mm_cvttps_epi32(_mm_mul_ps(x, __intMaxF));
        __yInt = _mm_cvttps_epi32(_mm_mul_ps(__phasor, __intMaxF));
        __yInt = _mm_xor_si128(__xInt, __yInt);
        __a = _mm_mul_ps(_mm_cvtepi32_ps(__yInt), __invIntMaxF);
        return _mm_linterp_ps(x, __a, param);
    }

    __m128 overdrive(const __m128& x, const __m128& param) {
        __a = _mm_mul_ps(__a, _twoHundred);
        __a = _mm_add_ps(param, __ones);
        return vecDriveSignal(x, __a);
    }

    __m128 multiply(const __m128& x, const __m128& param) {
        __a = _mm_mul_ps(param, __sixteens);
        __y = _mm_mul_ps(x, valley::_mm_cosine_ps(_mm_mul_ps(_mm_circle_ps(_mm_mul_ps(__phasor, __a)),
                         _mm_set1_ps(M_PI))));
        return _mm_linterp_ps(x, __y, _mm_clamp_ps(__a, __zeros, __ones));
    }

    __m128 sharpen(const __m128& x, const __m128& param) {
        __y = _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(x, x), x), x);
        __y = _mm_switch_ps(__y, _mm_mul_ps(__y, __negOnes), _mm_cmplt_ps(x, __zeros));
        return _mm_linterp_ps(x, __y, param);
    }

    __m128 fold(const __m128& x, const __m128& param) {
        __y = _mm_add_ps(_mm_mul_ps(x, __halfs), __halfs);
        __y = _mm_mirror_ps(__y, _mm_add_ps(param, _mm_set1_ps(0.00001f)));
        __y = _mm_mul_ps(_mm_add_ps(__y, _mm_set1_ps(-0.5f)), __twos);
        return __y;
    }

    // Fold the input signal x with the sine function
    __m128 chebyshev(const __m128& x, const __m128& param) {
        __a = _mm_mul_ps(param, _mm_set1_ps(8.f));
        __y = valley::_mm_sine_ps(_mm_mul_ps(_mm_circle_ps(_mm_mul_ps(x, __a)), _mm_set1_ps(M_PI)));
        return _mm_linterp_ps(x, __y, _mm_clamp_ps(__a, __zeros, __ones));
    }

    /** Similar to folding, but the wave arrives wraps round to negative. Uses cross fadeing to tame
        the buzzing.
    */
    __m128 mirror(const __m128& x, const __m128& param) {
        __a = _mm_mul_ps(param, __fours);
        __aInt = _mm_cvttps_epi32(_mm_add_ps(__a, __ones));
        __a = _mm_wrap_1_ps(__a);
        __b = _mm_cvtepi32_ps(__aInt);
        __b = _mm_sub_ps(__b, _mm_set1_ps(0.000001f));
        return _mm_linterp_ps(_mm_circle_ps(_mm_mul_ps(x, __b)),
                              _mm_circle_ps(_mm_mul_ps(x, _mm_add_ps(__b, __ones))),
                              __a);
    }

    /** Saw sub-oscillator that is derived from and external phasor using piecewise transform
        to create a sub-octave phasor, which is then anti-aliased using a PolyBLEP
    */
    __m128 subSaw(const __m128& x, const __m128& param) {
        __filter.setCutoffFreq(_mm_mul_ps(_mm_mul_ps(param, param), _mm_set1_ps(22050.f)));
        __counter = _mm_add_ps(__counter, _mm_and_ps(__ones, __eoc));
        __counter = _mm_switch_ps(__counter, __zeros, _mm_cmpgt_ps(__counter, __ones));

        __a = _mm_mul_ps(__phasor, __halfs);
        __a = _mm_add_ps(__a, _mm_mul_ps(__counter, __halfs));
        __y = _mm_mul_ps(__a, __twos);
        __y = _mm_sub_ps(__y, __ones);
        __y = _mm_sub_ps(__y, _mm_polyblep_ps(__a, __phasorStepSize));

        return _mm_add_ps(x, _mm_mul_ps(__y, param));
    }

    /** Similar to above except a square wave is further derived from the sub-octave phasor and
        anti-aliased using two PolyBLEPs
    */
    __m128 subSquare(const __m128& x, const __m128& param) {
        __filter.setCutoffFreq(_mm_mul_ps(_mm_mul_ps(param, param), _mm_set1_ps(22050.f)));
        __counter = _mm_add_ps(__counter, _mm_and_ps(__ones, __eoc));
        __counter = _mm_switch_ps(__counter, __zeros, _mm_cmpgt_ps(__counter, __ones));

        __a = _mm_mul_ps(__phasor, __halfs);
        __a = _mm_add_ps(__a, _mm_mul_ps(__counter, __halfs));
        __b = _mm_add_ps(__a, __halfs);
        __b = _mm_sub_ps(__b, _mm_and_ps(__ones, _mm_cmpge_ps(__b, __ones)));
        __y = _mm_switch_ps(__negOnes, __ones, _mm_cmplt_ps(__a, __halfs));
        __y = _mm_add_ps(__y, _mm_polyblep_ps(__a, __phasorStepSize));
        __y = _mm_sub_ps(__y, _mm_polyblep_ps(__b, __phasorStepSize));

        return _mm_add_ps(x, _mm_mul_ps(__y, param));
    }

    /** Using a similar principle to the previous sub-octave methods, make a sub-octave phasor, then
        shape it using a Chebyshev shaper
    */
    __m128 subChebyshev(const __m128& x, const __m128& param) {
        __counter = _mm_add_ps(__counter, __eoc);
        __counter = _mm_sub_ps(__counter, _mm_and_ps(__twos, _mm_cmpeq_ps(__counter, __twos)));

        __a = _mm_switch_ps(__phasor, _mm_sub_ps(__ones, __phasor), _mm_cmpeq_ps(__counter, __ones));
        __y = _mm_mul_ps(__a, __twos);
        __y = _mm_sub_ps(__y, __ones);

        __b = _mm_mul_ps(param, _mm_set1_ps(8.f));
        __y = valley::_mm_sine_ps(_mm_mul_ps(_mm_circle_ps(_mm_mul_ps(__y, __b)), _mm_set1_ps(M_PI)));
        return _mm_add_ps(x, _mm_mul_ps(__y, _mm_clamp_ps(__b, __zeros, __ones)));
    }

    int _mode;
    float updateRateScale, updateRateOffset;
    __m128 __updateRate, __counter, __doSample;
    __m128 __zeros, __ones, __twos, __fours, __high, __negOnes, __eights, __sixteens, __sixtyFour, _twoHundred;
    __m128 __halfs, __quarters, __eighths;
    __m128 __posEpsilon, __negEpsilon;
    __m128 __a, __b, __x, __y, __z;
    __m128i __xInt, __yInt, __zInt, __aInt;
    __m128 __intMaxF, __invIntMaxF;
    __m128 __phasor, __phasorStepSize, __eoc, __direction;

    __m128 __flipFlop, __prev, __trig;
    VecOnePoleLPFilter __filter;

    uint32_t _randZ[4];
    uint32_t _randW[4];
};
