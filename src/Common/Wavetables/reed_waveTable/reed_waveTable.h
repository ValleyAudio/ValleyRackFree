#ifndef WAVETABLE_REED_H
#define WAVETABLE_REED_H
#define WAVETABLE_REED_NUM 8

#include "reed_1.h"
#include "reed_2.h"
#include "reed_3.h"
#include "reed_4.h"
#include "reed_5.h"
#include "reed_6.h"
#include "reed_7.h"
#include "reed_8.h"

static float* wavetable_reed[WAVETABLE_REED_NUM] = {
    reed_1_waveTable,
    reed_2_waveTable,
    reed_3_waveTable,
    reed_4_waveTable,
    reed_5_waveTable,
    reed_6_waveTable,
    reed_7_waveTable,
    reed_8_waveTable
};

static long wavetable_reed_lengths[WAVETABLE_REED_NUM] = {
    reed_1_tableLength,
    reed_2_tableLength,
    reed_3_tableLength,
    reed_4_tableLength,
    reed_5_tableLength,
    reed_6_tableLength,
    reed_7_tableLength,
    reed_8_tableLength
};

#endif // WAVETABLE_REED_H
