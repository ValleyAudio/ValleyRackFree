#ifndef WAVETABLE_ADDITIVEBANK2_H
#define WAVETABLE_ADDITIVEBANK2_H
#define WAVETABLE_ADDITIVEBANK2_NUM 8

#include "additiveBank2_1.h"
#include "additiveBank2_2.h"
#include "additiveBank2_3.h"
#include "additiveBank2_4.h"
#include "additiveBank2_5.h"
#include "additiveBank2_6.h"
#include "additiveBank2_7.h"
#include "additiveBank2_8.h"

static float* wavetable_additiveBank2[WAVETABLE_ADDITIVEBANK2_NUM] = {
    additiveBank2_1_waveTable,
    additiveBank2_2_waveTable,
    additiveBank2_3_waveTable,
    additiveBank2_4_waveTable,
    additiveBank2_5_waveTable,
    additiveBank2_6_waveTable,
    additiveBank2_7_waveTable,
    additiveBank2_8_waveTable
};

static long wavetable_additiveBank2_lengths[WAVETABLE_ADDITIVEBANK2_NUM] = {
    additiveBank2_1_tableLength,
    additiveBank2_2_tableLength,
    additiveBank2_3_tableLength,
    additiveBank2_4_tableLength,
    additiveBank2_5_tableLength,
    additiveBank2_6_tableLength,
    additiveBank2_7_tableLength,
    additiveBank2_8_tableLength
};

#endif // WAVETABLE_ADDITIVEBANK2_H
