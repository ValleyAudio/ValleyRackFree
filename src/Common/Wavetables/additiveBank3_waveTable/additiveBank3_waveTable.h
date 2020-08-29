#ifndef WAVETABLE_ADDITIVEBANK3_H
#define WAVETABLE_ADDITIVEBANK3_H
#define WAVETABLE_ADDITIVEBANK3_NUM 8

#include "additiveBank3_1.h"
#include "additiveBank3_2.h"
#include "additiveBank3_3.h"
#include "additiveBank3_4.h"
#include "additiveBank3_5.h"
#include "additiveBank3_6.h"
#include "additiveBank3_7.h"
#include "additiveBank3_8.h"

static float* wavetable_additiveBank3[WAVETABLE_ADDITIVEBANK3_NUM] = {
    additiveBank3_1_waveTable,
    additiveBank3_2_waveTable,
    additiveBank3_3_waveTable,
    additiveBank3_4_waveTable,
    additiveBank3_5_waveTable,
    additiveBank3_6_waveTable,
    additiveBank3_7_waveTable,
    additiveBank3_8_waveTable
};

static long wavetable_additiveBank3_lengths[WAVETABLE_ADDITIVEBANK3_NUM] = {
    additiveBank3_1_tableLength,
    additiveBank3_2_tableLength,
    additiveBank3_3_tableLength,
    additiveBank3_4_tableLength,
    additiveBank3_5_tableLength,
    additiveBank3_6_tableLength,
    additiveBank3_7_tableLength,
    additiveBank3_8_tableLength
};

#endif // WAVETABLE_ADDITIVEBANK3_H
