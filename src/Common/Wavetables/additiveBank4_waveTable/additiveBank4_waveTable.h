#ifndef WAVETABLE_ADDITIVEBANK4_H
#define WAVETABLE_ADDITIVEBANK4_H
#define WAVETABLE_ADDITIVEBANK4_NUM 8

#include "additiveBank4_1.h"
#include "additiveBank4_2.h"
#include "additiveBank4_3.h"
#include "additiveBank4_4.h"
#include "additiveBank4_5.h"
#include "additiveBank4_6.h"
#include "additiveBank4_7.h"
#include "additiveBank4_8.h"

static float* wavetable_additiveBank4[WAVETABLE_ADDITIVEBANK4_NUM] = {
    additiveBank4_1_waveTable,
    additiveBank4_2_waveTable,
    additiveBank4_3_waveTable,
    additiveBank4_4_waveTable,
    additiveBank4_5_waveTable,
    additiveBank4_6_waveTable,
    additiveBank4_7_waveTable,
    additiveBank4_8_waveTable
};

static long wavetable_additiveBank4_lengths[WAVETABLE_ADDITIVEBANK4_NUM] = {
    additiveBank4_1_tableLength,
    additiveBank4_2_tableLength,
    additiveBank4_3_tableLength,
    additiveBank4_4_tableLength,
    additiveBank4_5_tableLength,
    additiveBank4_6_tableLength,
    additiveBank4_7_tableLength,
    additiveBank4_8_tableLength
};

#endif // WAVETABLE_ADDITIVEBANK4_H
