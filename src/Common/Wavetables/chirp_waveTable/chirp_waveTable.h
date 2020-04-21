#ifndef WAVETABLE_CHIRP_H
#define WAVETABLE_CHIRP_H
#define WAVETABLE_CHIRP_NUM 8

#include "chirp_1.h"
#include "chirp_2.h"
#include "chirp_3.h"
#include "chirp_4.h"
#include "chirp_5.h"
#include "chirp_6.h"
#include "chirp_7.h"
#include "chirp_8.h"

static float* wavetable_chirp[WAVETABLE_CHIRP_NUM] = {
    chirp_1_waveTable,
    chirp_2_waveTable,
    chirp_3_waveTable,
    chirp_4_waveTable,
    chirp_5_waveTable,
    chirp_6_waveTable,
    chirp_7_waveTable,
    chirp_8_waveTable
};

static long wavetable_chirp_lengths[WAVETABLE_CHIRP_NUM] = {
    chirp_1_tableLength,
    chirp_2_tableLength,
    chirp_3_tableLength,
    chirp_4_tableLength,
    chirp_5_tableLength,
    chirp_6_tableLength,
    chirp_7_tableLength,
    chirp_8_tableLength
};

#endif // WAVETABLE_CHIRP_H
