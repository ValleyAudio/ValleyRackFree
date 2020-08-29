#ifndef WAVETABLE_XFADE_H
#define WAVETABLE_XFADE_H
#define WAVETABLE_XFADE_NUM 8

#include "xfade_1.h"
#include "xfade_2.h"
#include "xfade_3.h"
#include "xfade_4.h"
#include "xfade_5.h"
#include "xfade_6.h"
#include "xfade_7.h"
#include "xfade_8.h"

static float* wavetable_xfade[WAVETABLE_XFADE_NUM] = {
    xfade_1_waveTable,
    xfade_2_waveTable,
    xfade_3_waveTable,
    xfade_4_waveTable,
    xfade_5_waveTable,
    xfade_6_waveTable,
    xfade_7_waveTable,
    xfade_8_waveTable
};

static long wavetable_xfade_lengths[WAVETABLE_XFADE_NUM] = {
    xfade_1_tableLength,
    xfade_2_tableLength,
    xfade_3_tableLength,
    xfade_4_tableLength,
    xfade_5_tableLength,
    xfade_6_tableLength,
    xfade_7_tableLength,
    xfade_8_tableLength
};

#endif // WAVETABLE_XFADE_H
