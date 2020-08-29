#ifndef WAVETABLE_GMTRY_2_H
#define WAVETABLE_GMTRY_2_H
#define WAVETABLE_GMTRY_2_NUM 8

#include "gmtry_2_1.h"
#include "gmtry_2_2.h"
#include "gmtry_2_3.h"
#include "gmtry_2_4.h"
#include "gmtry_2_5.h"
#include "gmtry_2_6.h"
#include "gmtry_2_7.h"
#include "gmtry_2_8.h"

static float* wavetable_gmtry_2[WAVETABLE_GMTRY_2_NUM] = {
    gmtry_2_1_waveTable,
    gmtry_2_2_waveTable,
    gmtry_2_3_waveTable,
    gmtry_2_4_waveTable,
    gmtry_2_5_waveTable,
    gmtry_2_6_waveTable,
    gmtry_2_7_waveTable,
    gmtry_2_8_waveTable
};

static long wavetable_gmtry_2_lengths[WAVETABLE_GMTRY_2_NUM] = {
    gmtry_2_1_tableLength,
    gmtry_2_2_tableLength,
    gmtry_2_3_tableLength,
    gmtry_2_4_tableLength,
    gmtry_2_5_tableLength,
    gmtry_2_6_tableLength,
    gmtry_2_7_tableLength,
    gmtry_2_8_tableLength
};

#endif // WAVETABLE_GMTRY_2_H
