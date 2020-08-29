#ifndef WAVETABLE_DISTORT_H
#define WAVETABLE_DISTORT_H
#define WAVETABLE_DISTORT_NUM 8

#include "distort_1.h"
#include "distort_2.h"
#include "distort_3.h"
#include "distort_4.h"
#include "distort_5.h"
#include "distort_6.h"
#include "distort_7.h"
#include "distort_8.h"

static float* wavetable_distort[WAVETABLE_DISTORT_NUM] = {
    distort_1_waveTable,
    distort_2_waveTable,
    distort_3_waveTable,
    distort_4_waveTable,
    distort_5_waveTable,
    distort_6_waveTable,
    distort_7_waveTable,
    distort_8_waveTable
};

static long wavetable_distort_lengths[WAVETABLE_DISTORT_NUM] = {
    distort_1_tableLength,
    distort_2_tableLength,
    distort_3_tableLength,
    distort_4_tableLength,
    distort_5_tableLength,
    distort_6_tableLength,
    distort_7_tableLength,
    distort_8_tableLength
};

#endif // WAVETABLE_DISTORT_H
