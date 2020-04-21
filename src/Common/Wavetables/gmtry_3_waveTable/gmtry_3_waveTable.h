#ifndef WAVETABLE_GMTRY_3_H
#define WAVETABLE_GMTRY_3_H
#define WAVETABLE_GMTRY_3_NUM 8

#include "gmtry_3_1.h"
#include "gmtry_3_2.h"
#include "gmtry_3_3.h"
#include "gmtry_3_4.h"
#include "gmtry_3_5.h"
#include "gmtry_3_6.h"
#include "gmtry_3_7.h"
#include "gmtry_3_8.h"

static float* wavetable_gmtry_3[WAVETABLE_GMTRY_3_NUM] = {
    gmtry_3_1_waveTable,
    gmtry_3_2_waveTable,
    gmtry_3_3_waveTable,
    gmtry_3_4_waveTable,
    gmtry_3_5_waveTable,
    gmtry_3_6_waveTable,
    gmtry_3_7_waveTable,
    gmtry_3_8_waveTable
};

static long wavetable_gmtry_3_lengths[WAVETABLE_GMTRY_3_NUM] = {
    gmtry_3_1_tableLength,
    gmtry_3_2_tableLength,
    gmtry_3_3_tableLength,
    gmtry_3_4_tableLength,
    gmtry_3_5_tableLength,
    gmtry_3_6_tableLength,
    gmtry_3_7_tableLength,
    gmtry_3_8_tableLength
};

#endif // WAVETABLE_GMTRY_3_H
