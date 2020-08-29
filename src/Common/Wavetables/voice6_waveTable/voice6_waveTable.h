#ifndef WAVETABLE_VOICE6_H
#define WAVETABLE_VOICE6_H
#define WAVETABLE_VOICE6_NUM 8

#include "voice6_1.h"
#include "voice6_2.h"
#include "voice6_3.h"
#include "voice6_4.h"
#include "voice6_5.h"
#include "voice6_6.h"
#include "voice6_7.h"
#include "voice6_8.h"

static float* wavetable_voice6[WAVETABLE_VOICE6_NUM] = {
    voice6_1_waveTable,
    voice6_2_waveTable,
    voice6_3_waveTable,
    voice6_4_waveTable,
    voice6_5_waveTable,
    voice6_6_waveTable,
    voice6_7_waveTable,
    voice6_8_waveTable
};

static long wavetable_voice6_lengths[WAVETABLE_VOICE6_NUM] = {
    voice6_1_tableLength,
    voice6_2_tableLength,
    voice6_3_tableLength,
    voice6_4_tableLength,
    voice6_5_tableLength,
    voice6_6_tableLength,
    voice6_7_tableLength,
    voice6_8_tableLength
};

#endif // WAVETABLE_VOICE6_H
