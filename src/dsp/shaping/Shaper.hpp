//
//  Shaper.hpp
//  Shaper - Vectorised waveform shaper
//
//  Created by Dale Johnson on 02/02/2018.
//  Copyright © 2023 Dale Johnson. All rights reserved.
//

#pragma once

#include "valley_sse_include.h"

#include <cmath>
#include <cstdint>
#include <ctime>
#include <vector>
#include "../../simd/SIMDUtilities.hpp"
#include "../../utilities/Utilities.hpp"
#include "../filters/VecOnePoleFilters.hpp"

class Shaper {
public:
    enum Modes {
        BEND_MODE,
        TILT_MODE,
        LEAN_MODE,
        TWIST_MODE,
        WRAP_MODE,
        MIRROR_MODE,
        REFLECT_MODE,
        PULSE_MODE,
        STEP4_MODE,
        STEP8_MODE,
        STEP16_MODE,
        VARSTEP_MODE,
        WARBLE_MODE,
        SINEWRAP_MODE,
        HARMONICS_MODE,
        BUZZ_X2_MODE,
        BUZZ_X4_MODE,
        BUZZ_X8_MODE,
        WRINKLE_X2_MODE,
        WRINKLE_X4_MODE,
        WRINKLE_X8_MODE,
        SINE_DOWN_X2_MODE,
        SINE_DOWN_X4_MODE,
        SINE_DOWN_X8_MODE,
        SINE_UP_X2_MODE,
        SINE_UP_X4_MODE,
        SINE_UP_X8_MODE,
        NUM_MODES
    };

    Shaper();
    __m128 process(const __m128& a, const __m128& f);
    void setShapeMode(int newShapeMode);

private:
    Modes shapeMode = Modes::BEND_MODE;

    __m128 output;

    uint32_t z[4] = {0, 0, 0, 0};
    uint32_t w[4] = {0, 0, 0, 0};
    float random[4] = {0.f, 0.f, 0.f, 0.f};

    __m128 noise;
    VecOnePoleLPFilter filter1, filter2;

    // Shaping functions
    void bend(const __m128& a, const __m128& f);
    void tilt(const __m128& a, const __m128& f);
    void lean(const __m128& a, const __m128& f);
    void lean2(const __m128& a, const __m128& f);
    void twist(const __m128& a, const __m128& f);
    void wrap(const __m128& a, const __m128& f);
    void mirror(const __m128& a, const __m128& f);
    void reflect(const __m128& a, const __m128& f);
    void pulse(const __m128& a, const __m128& f);
    void step4(const __m128& a, const __m128& f);
    void step8(const __m128& a, const __m128& f);
    void step16(const __m128& a, const __m128& f);
    void varStep(const __m128& a, const __m128& f);
    void sineWrap(const __m128& a, const __m128& f);
    void harmonics(const __m128& a, const __m128& f);
    void buzzX2(const __m128& a, const __m128& f);
    void buzzX4(const __m128& a, const __m128& f);
    void buzzX8(const __m128& a, const __m128& f);
    void wrinkleX2(const __m128& a, const __m128& f);
    void wrinkleX4(const __m128& a, const __m128& f);
    void wrinkleX8(const __m128& a, const __m128& f);
    void sineDownX2(const __m128& a, const __m128& f);
    void sineDownX4(const __m128& a, const __m128& f);
    void sineDownX8(const __m128& a, const __m128& f);
    void sineUpX2(const __m128& a, const __m128& f);
    void sineUpX4(const __m128& a, const __m128& f);
    void sineUpX8(const __m128& a, const __m128& f);
    void warble(const __m128& a, const __m128& f);
};

