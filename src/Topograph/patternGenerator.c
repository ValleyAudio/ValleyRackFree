//
// patternGenerator.cc
//
// Topograph
// A port of "Mutable Instruments Grids" for VCV Rack
// Author: Dale Johnson (valley.audio.soft@gmail.com)
// Date: 4/12/2017
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

uint8_t readDrumMap(uint8_t step, uint8_t instrument, uint8_t x, uint8_t y) {
    uint8_t i = (int)floor(x * 3.0 / 127.0);
    uint8_t j = (int)floor(y * 3.0 / 127.0);
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
    uint8_t r = (( a * x + b * (maxValue - x) ) * y +
                 (c * x + d * (maxValue - x)) * ( maxValue - y )) / maxValue / maxValue;
    return r;
}

uint8_t getDrums(uint8_t step, t_drumSettings* settings, uint8_t randomness) {
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
        uint8_t level = readDrumMap(step, i, settings->mapX, settings->mapY);

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
    return state;
}

uint8_t U8Mix(uint8_t a, uint8_t b, uint8_t balance) {
    return (a * (255 - balance) + b * balance) / 255;
}
