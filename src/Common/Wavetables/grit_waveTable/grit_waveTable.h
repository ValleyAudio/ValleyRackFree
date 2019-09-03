#ifndef WAVETABLE_GRIT_H
#define WAVETABLE_GRIT_H
#define WAVETABLE_GRIT_NUM 8

#include "grit_1.h"
#include "grit_2.h"
#include "grit_3.h"
#include "grit_4.h"
#include "grit_5.h"
#include "grit_6.h"
#include "grit_7.h"
#include "grit_8.h"

static float* wavetable_grit[WAVETABLE_GRIT_NUM] = {
    grit_1_waveTable,
    grit_2_waveTable,
    grit_3_waveTable,
    grit_4_waveTable,
    grit_5_waveTable,
    grit_6_waveTable,
    grit_7_waveTable,
    grit_8_waveTable
};

static long wavetable_grit_lengths[WAVETABLE_GRIT_NUM] = {
    grit_1_tableLength,
    grit_2_tableLength,
    grit_3_tableLength,
    grit_4_tableLength,
    grit_5_tableLength,
    grit_6_tableLength,
    grit_7_tableLength,
    grit_8_tableLength
};

#endif // WAVETABLE_GRIT_H
