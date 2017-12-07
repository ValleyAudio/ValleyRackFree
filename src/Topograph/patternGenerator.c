//
// patternGenerator.cc
//
// Topograph
// A port of "Mutable Instruments Grids" for VCV Rack
// Author: Dale Johnson (valley.audio.soft@gmail.com)
// Date: 4/12/2017
//
// Modifications to the drum map reader code provided by Henri David
// https://github.com/hdavid/Grids/
//
// Copyright 2011 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "patternGenerator.h"

uint8_t readDrumMap(uint8_t step, uint8_t instrument, uint8_t x, uint8_t y,
                    bool henriMode) {
    uint8_t r = 0;
    if(henriMode) { // Gives a different feel to the groove
        uint8_t i = (int)floor(x * 3.0 / 255.0);
        uint8_t j = (int)floor(y * 3.0 / 255.0);
        const uint8_t* a_map = drum_map[i][j];
        const uint8_t* b_map = drum_map[i + 1][j];
        const uint8_t* c_map = drum_map[i][j + 1];
        const uint8_t* d_map = drum_map[i + 1][j + 1];
        uint8_t offset = (instrument * kStepsPerPattern) + step;
        uint8_t a = a_map[offset];
        uint8_t b = b_map[offset];
        uint8_t c = c_map[offset];
        uint8_t d = d_map[offset];
        uint8_t maxValue = 127;
        r = (( a * x + b * (maxValue - x) ) * y + (c * x + d * (maxValue - x)) *
             ( maxValue - y )) / maxValue / maxValue;
    }
    else { // Original M.I. code that is closer to the AVR behaviour
        uint8_t i = x >> 6;
        uint8_t j = y >> 6;

        const uint8_t* a_map = drum_map[i][j];
        const uint8_t* b_map = drum_map[i + 1][j];
        const uint8_t* c_map = drum_map[i][j + 1];
        const uint8_t* d_map = drum_map[i + 1][j + 1];
        uint8_t offset = (instrument * kStepsPerPattern) + step;
        uint8_t a = *(a_map + offset);
        uint8_t b = *(b_map + offset);
        uint8_t c = *(c_map + offset);
        uint8_t d = *(d_map + offset);
        r = U8Mix(U8Mix(a, b, x << 2), U8Mix(c, d, x << 2), y << 2);
    }
    return r;
}

uint8_t getDrums(uint8_t step, t_drumSettings* settings, uint8_t randomness,
                 bool henriMode, bool mask16thNotes) {
    // At the beginning of a pattern, decide on perturbation levels.
    if (step == 0) {
        for (uint8_t i = 0; i < kNumParts; ++i) {
            uint8_t r = randomness >> 2;
            uint8_t rand2 = (uint8_t)(abs(rand()) % 256);
            settings->perturbation[i] = (rand2 * r) >> 8;
        }
    }
    uint8_t state = 0;
    uint8_t instrument_mask = 1;
    uint8_t accent_bits = 0;
    for (uint8_t i = 0; i < kNumParts; ++i) {
        uint8_t level = readDrumMap(step, i, settings->mapX, settings->mapY, henriMode);

        if (level < (255 - settings->perturbation[i])) {
            level += settings->perturbation[i];
        }
        else {
            level = 255;
        }

        uint8_t threshold = 255 - settings->density[i];
        if (level > threshold) {
            if (level > 192) {
                accent_bits |= instrument_mask;
            }
            state |= instrument_mask;
        }
        instrument_mask <<= 1;
    }
    state |= accent_bits << 3;
    if((step % 2) && mask16thNotes) {
        state = 0;
    }
    return state;
}

uint8_t U8Mix(uint8_t a, uint8_t b, uint8_t balance) {
    return (a * (255 - balance) + b * balance) / 255;
}

void evaluateEuclidean(t_drumState* drumState, t_drumSettings* settings) {
    if (drumState->step & 1) {
        return;
    }
    uint8_t instrument_mask = 1;
    uint8_t reset_bits = 0;
    for (uint8_t i = 0; i < kNumParts; ++i)  {
        uint8_t length = (drumState->euclideanLength[i] >> 3) + 1;
        uint8_t density = settings->density[i] >> 3;
        uint32_t address = (length - 1) * (uint8_t)32 + density;
        while(drumState->euclideanStep[i] >= length) {
            drumState->euclideanStep[i] -= length;
        }
        uint32_t step_mask = 1L << (uint32_t)(drumState->euclideanStep[i]);
        uint32_t pattern_bits = *(lut_res_euclidean + address);
        if (pattern_bits & step_mask) {
          drumState->triggerState |= instrument_mask;
        }
        if (drumState->euclideanStep[i] == 0) {
          reset_bits |= instrument_mask;
        }
        instrument_mask <<= 1;
    }
    drumState->triggerState |= reset_bits << 3;
}
