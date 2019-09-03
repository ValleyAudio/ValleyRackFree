#ifndef WAVETABLE_LINEAR1_H
#define WAVETABLE_LINEAR1_H
#define WAVETABLE_LINEAR1_NUM 8

#include "linear1_1.h"
#include "linear1_2.h"
#include "linear1_3.h"
#include "linear1_4.h"
#include "linear1_5.h"
#include "linear1_6.h"
#include "linear1_7.h"
#include "linear1_8.h"

static float* wavetable_linear1[WAVETABLE_LINEAR1_NUM] = {
    linear1_1_waveTable,
    linear1_2_waveTable,
    linear1_3_waveTable,
    linear1_4_waveTable,
    linear1_5_waveTable,
    linear1_6_waveTable,
    linear1_7_waveTable,
    linear1_8_waveTable
};

static long wavetable_linear1_lengths[WAVETABLE_LINEAR1_NUM] = {
    linear1_1_tableLength,
    linear1_2_tableLength,
    linear1_3_tableLength,
    linear1_4_tableLength,
    linear1_5_tableLength,
    linear1_6_tableLength,
    linear1_7_tableLength,
    linear1_8_tableLength
};

#endif // WAVETABLE_LINEAR1_H
