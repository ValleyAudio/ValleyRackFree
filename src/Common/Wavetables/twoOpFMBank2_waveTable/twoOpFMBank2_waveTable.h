#ifndef WAVETABLE_TWOOPFMBANK2_H
#define WAVETABLE_TWOOPFMBANK2_H
#define WAVETABLE_TWOOPFMBANK2_NUM 8

#include "twoOpFMBank2_1.h"
#include "twoOpFMBank2_2.h"
#include "twoOpFMBank2_3.h"
#include "twoOpFMBank2_4.h"
#include "twoOpFMBank2_5.h"
#include "twoOpFMBank2_6.h"
#include "twoOpFMBank2_7.h"
#include "twoOpFMBank2_8.h"

static float* wavetable_twoOpFMBank2[WAVETABLE_TWOOPFMBANK2_NUM] = {
    twoOpFMBank2_1_waveTable,
    twoOpFMBank2_2_waveTable,
    twoOpFMBank2_3_waveTable,
    twoOpFMBank2_4_waveTable,
    twoOpFMBank2_5_waveTable,
    twoOpFMBank2_6_waveTable,
    twoOpFMBank2_7_waveTable,
    twoOpFMBank2_8_waveTable
};

static long wavetable_twoOpFMBank2_lengths[WAVETABLE_TWOOPFMBANK2_NUM] = {
    twoOpFMBank2_1_tableLength,
    twoOpFMBank2_2_tableLength,
    twoOpFMBank2_3_tableLength,
    twoOpFMBank2_4_tableLength,
    twoOpFMBank2_5_tableLength,
    twoOpFMBank2_6_tableLength,
    twoOpFMBank2_7_tableLength,
    twoOpFMBank2_8_tableLength
};

#endif // WAVETABLE_TWOOPFMBANK2_H
