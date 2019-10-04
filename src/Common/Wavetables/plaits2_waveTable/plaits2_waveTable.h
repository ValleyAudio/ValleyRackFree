#ifndef WAVETABLE_PLAITS2_H
#define WAVETABLE_PLAITS2_H
#define WAVETABLE_PLAITS2_NUM 8

#include "plaits2_1.h"
#include "plaits2_2.h"
#include "plaits2_3.h"
#include "plaits2_4.h"
#include "plaits2_5.h"
#include "plaits2_6.h"
#include "plaits2_7.h"
#include "plaits2_8.h"

static float* wavetable_plaits2[WAVETABLE_PLAITS2_NUM] = {
    plaits2_1_waveTable,
    plaits2_2_waveTable,
    plaits2_3_waveTable,
    plaits2_4_waveTable,
    plaits2_5_waveTable,
    plaits2_6_waveTable,
    plaits2_7_waveTable,
    plaits2_8_waveTable
};

static long wavetable_plaits2_lengths[WAVETABLE_PLAITS2_NUM] = {
    plaits2_1_tableLength,
    plaits2_2_tableLength,
    plaits2_3_tableLength,
    plaits2_4_tableLength,
    plaits2_5_tableLength,
    plaits2_6_tableLength,
    plaits2_7_tableLength,
    plaits2_8_tableLength
};

#endif // WAVETABLE_PLAITS2_H
