#ifndef WAVETABLE_TWOOPFMBANK_H
#define WAVETABLE_TWOOPFMBANK_H
#define WAVETABLE_TWOOPFMBANK_NUM 8

#include "twoOpFMBank_1.h"
#include "twoOpFMBank_2.h"
#include "twoOpFMBank_3.h"
#include "twoOpFMBank_4.h"
#include "twoOpFMBank_5.h"
#include "twoOpFMBank_6.h"
#include "twoOpFMBank_7.h"
#include "twoOpFMBank_8.h"

static float* wavetable_twoOpFMBank[WAVETABLE_TWOOPFMBANK_NUM] = {
    twoOpFMBank_1_waveTable,
    twoOpFMBank_2_waveTable,
    twoOpFMBank_3_waveTable,
    twoOpFMBank_4_waveTable,
    twoOpFMBank_5_waveTable,
    twoOpFMBank_6_waveTable,
    twoOpFMBank_7_waveTable,
    twoOpFMBank_8_waveTable
};

static long wavetable_twoOpFMBank_lengths[WAVETABLE_TWOOPFMBANK_NUM] = {
    twoOpFMBank_1_tableLength,
    twoOpFMBank_2_tableLength,
    twoOpFMBank_3_tableLength,
    twoOpFMBank_4_tableLength,
    twoOpFMBank_5_tableLength,
    twoOpFMBank_6_tableLength,
    twoOpFMBank_7_tableLength,
    twoOpFMBank_8_tableLength
};

#endif // WAVETABLE_TWOOPFMBANK_H
