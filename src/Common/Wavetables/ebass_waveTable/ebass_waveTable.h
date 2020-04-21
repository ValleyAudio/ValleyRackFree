#ifndef WAVETABLE_EBASS_H
#define WAVETABLE_EBASS_H
#define WAVETABLE_EBASS_NUM 8

#include "ebass_1.h"
#include "ebass_2.h"
#include "ebass_3.h"
#include "ebass_4.h"
#include "ebass_5.h"
#include "ebass_6.h"
#include "ebass_7.h"
#include "ebass_8.h"

static float* wavetable_ebass[WAVETABLE_EBASS_NUM] = {
    ebass_1_waveTable,
    ebass_2_waveTable,
    ebass_3_waveTable,
    ebass_4_waveTable,
    ebass_5_waveTable,
    ebass_6_waveTable,
    ebass_7_waveTable,
    ebass_8_waveTable
};

static long wavetable_ebass_lengths[WAVETABLE_EBASS_NUM] = {
    ebass_1_tableLength,
    ebass_2_tableLength,
    ebass_3_tableLength,
    ebass_4_tableLength,
    ebass_5_tableLength,
    ebass_6_tableLength,
    ebass_7_tableLength,
    ebass_8_tableLength
};

#endif // WAVETABLE_EBASS_H
