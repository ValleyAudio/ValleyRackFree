#ifndef WAVETABLE_RESOSAW_H
#define WAVETABLE_RESOSAW_H
#define WAVETABLE_RESOSAW_NUM 8

#include "resoSaw_1.h"
#include "resoSaw_2.h"
#include "resoSaw_3.h"
#include "resoSaw_4.h"
#include "resoSaw_5.h"
#include "resoSaw_6.h"
#include "resoSaw_7.h"
#include "resoSaw_8.h"

static float* wavetable_resoSaw[WAVETABLE_RESOSAW_NUM] = {
    resoSaw_1_waveTable,
    resoSaw_2_waveTable,
    resoSaw_3_waveTable,
    resoSaw_4_waveTable,
    resoSaw_5_waveTable,
    resoSaw_6_waveTable,
    resoSaw_7_waveTable,
    resoSaw_8_waveTable
};

static long wavetable_resoSaw_lengths[WAVETABLE_RESOSAW_NUM] = {
    resoSaw_1_tableLength,
    resoSaw_2_tableLength,
    resoSaw_3_tableLength,
    resoSaw_4_tableLength,
    resoSaw_5_tableLength,
    resoSaw_6_tableLength,
    resoSaw_7_tableLength,
    resoSaw_8_tableLength
};

#endif // WAVETABLE_RESOSAW_H
