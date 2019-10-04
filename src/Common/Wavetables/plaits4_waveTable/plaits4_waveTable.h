#ifndef WAVETABLE_PLAITS4_H
#define WAVETABLE_PLAITS4_H
#define WAVETABLE_PLAITS4_NUM 8

#include "plaits4_1.h"
#include "plaits4_2.h"
#include "plaits4_3.h"
#include "plaits4_4.h"
#include "plaits4_5.h"
#include "plaits4_6.h"
#include "plaits4_7.h"
#include "plaits4_8.h"

static float* wavetable_plaits4[WAVETABLE_PLAITS4_NUM] = {
    plaits4_1_waveTable,
    plaits4_2_waveTable,
    plaits4_3_waveTable,
    plaits4_4_waveTable,
    plaits4_5_waveTable,
    plaits4_6_waveTable,
    plaits4_7_waveTable,
    plaits4_8_waveTable
};

static long wavetable_plaits4_lengths[WAVETABLE_PLAITS4_NUM] = {
    plaits4_1_tableLength,
    plaits4_2_tableLength,
    plaits4_3_tableLength,
    plaits4_4_tableLength,
    plaits4_5_tableLength,
    plaits4_6_tableLength,
    plaits4_7_tableLength,
    plaits4_8_tableLength
};

#endif // WAVETABLE_PLAITS4_H
