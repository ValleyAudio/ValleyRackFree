#ifndef WAVETABLE_PLAITS3_H
#define WAVETABLE_PLAITS3_H
#define WAVETABLE_PLAITS3_NUM 8

#include "plaits3_1.h"
#include "plaits3_2.h"
#include "plaits3_3.h"
#include "plaits3_4.h"
#include "plaits3_5.h"
#include "plaits3_6.h"
#include "plaits3_7.h"
#include "plaits3_8.h"

static float* wavetable_plaits3[WAVETABLE_PLAITS3_NUM] = {
    plaits3_1_waveTable,
    plaits3_2_waveTable,
    plaits3_3_waveTable,
    plaits3_4_waveTable,
    plaits3_5_waveTable,
    plaits3_6_waveTable,
    plaits3_7_waveTable,
    plaits3_8_waveTable
};

static long wavetable_plaits3_lengths[WAVETABLE_PLAITS3_NUM] = {
    plaits3_1_tableLength,
    plaits3_2_tableLength,
    plaits3_3_tableLength,
    plaits3_4_tableLength,
    plaits3_5_tableLength,
    plaits3_6_tableLength,
    plaits3_7_tableLength,
    plaits3_8_tableLength
};

#endif // WAVETABLE_PLAITS3_H
