#ifndef WAVETABLE_RESOSQUARE_H
#define WAVETABLE_RESOSQUARE_H
#define WAVETABLE_RESOSQUARE_NUM 8

#include "resoSquare_1.h"
#include "resoSquare_2.h"
#include "resoSquare_3.h"
#include "resoSquare_4.h"
#include "resoSquare_5.h"
#include "resoSquare_6.h"
#include "resoSquare_7.h"
#include "resoSquare_8.h"

static float* wavetable_resoSquare[WAVETABLE_RESOSQUARE_NUM] = {
    resoSquare_1_waveTable,
    resoSquare_2_waveTable,
    resoSquare_3_waveTable,
    resoSquare_4_waveTable,
    resoSquare_5_waveTable,
    resoSquare_6_waveTable,
    resoSquare_7_waveTable,
    resoSquare_8_waveTable
};

static long wavetable_resoSquare_lengths[WAVETABLE_RESOSQUARE_NUM] = {
    resoSquare_1_tableLength,
    resoSquare_2_tableLength,
    resoSquare_3_tableLength,
    resoSquare_4_tableLength,
    resoSquare_5_tableLength,
    resoSquare_6_tableLength,
    resoSquare_7_tableLength,
    resoSquare_8_tableLength
};

#endif // WAVETABLE_RESOSQUARE_H
