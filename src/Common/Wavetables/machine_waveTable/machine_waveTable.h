#ifndef WAVETABLE_MACHINE_H
#define WAVETABLE_MACHINE_H
#define WAVETABLE_MACHINE_NUM 8

#include "machine_1.h"
#include "machine_2.h"
#include "machine_3.h"
#include "machine_4.h"
#include "machine_5.h"
#include "machine_6.h"
#include "machine_7.h"
#include "machine_8.h"

static float* wavetable_machine[WAVETABLE_MACHINE_NUM] = {
    machine_1_waveTable,
    machine_2_waveTable,
    machine_3_waveTable,
    machine_4_waveTable,
    machine_5_waveTable,
    machine_6_waveTable,
    machine_7_waveTable,
    machine_8_waveTable
};

static long wavetable_machine_lengths[WAVETABLE_MACHINE_NUM] = {
    machine_1_tableLength,
    machine_2_tableLength,
    machine_3_tableLength,
    machine_4_tableLength,
    machine_5_tableLength,
    machine_6_tableLength,
    machine_7_tableLength,
    machine_8_tableLength
};

#endif // WAVETABLE_MACHINE_H
