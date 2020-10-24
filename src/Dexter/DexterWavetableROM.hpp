//
// DexterWavetableROM.hpp
// Author: Dale Johnson
//
// Copyright (c) 2020 Dale Johnson, Valley Audio
// Licensed under GPLv3
//

#pragma once

#include <common.hpp>
#define NUM_DEXTER_WAVETABLES 35

BINARY(src_ADD_BANK1_bin);
BINARY(src_ADD_SAW_bin);
BINARY(src_ADD_SINE_bin);
BINARY(src_ADD_SQR_bin);
BINARY(src_ALTOSAX_bin);
BINARY(src_AM_HARM_bin);
BINARY(src_BASIC_bin);
BINARY(src_BI_PULSE_bin);
BINARY(src_BITCRUSH1_bin);
BINARY(src_BITCRUSH2_bin);
BINARY(src_CELLO_1_bin);
BINARY(src_CELLO_2_bin);
BINARY(src_CHIP_1_bin);
BINARY(src_CHIP_2_bin);
BINARY(src_OBOE_bin);
BINARY(src_OPAL_bin);
BINARY(src_OVERTONE1_bin);
BINARY(src_OVERTONE2_bin);
BINARY(src_PIANO_bin);
BINARY(src_PLUCK_bin);
BINARY(src_PWM_bin);
BINARY(src_SAW_GAP1_bin);
BINARY(src_SAW_GAP2_bin);
BINARY(src_SINE_HARM_bin);
BINARY(src_SWEEPHARM_bin);
BINARY(src_SYMMETRY_bin);
BINARY(src_TEE_EKS_bin);
BINARY(src_THEREMIN_bin);
BINARY(src_VIDEOGAME_bin);
BINARY(src_VIOLIN_bin);
BINARY(src_VOICE_1_bin);
BINARY(src_VOICE_2_bin);
BINARY(src_VOICE_3_bin);
BINARY(src_VOICE_4_bin);
BINARY(src_VOICE_5_bin);

static float* dexterWavetables[NUM_DEXTER_WAVETABLES] = {
    (float*)BINARY_START(src_OPAL_bin), (float*)BINARY_START(src_BASIC_bin), (float*)BINARY_START(src_TEE_EKS_bin), (float*)BINARY_START(src_SINE_HARM_bin),
    (float*)BINARY_START(src_ADD_SINE_bin), (float*)BINARY_START(src_AM_HARM_bin), (float*)BINARY_START(src_SWEEPHARM_bin), (float*)BINARY_START(src_ADD_SAW_bin),
    (float*)BINARY_START(src_ADD_SQR_bin), (float*)BINARY_START(src_ADD_BANK1_bin), (float*)BINARY_START(src_OBOE_bin), (float*)BINARY_START(src_ALTOSAX_bin),
    (float*)BINARY_START(src_CELLO_1_bin), (float*)BINARY_START(src_CELLO_2_bin), (float*)BINARY_START(src_VIOLIN_bin), (float*)BINARY_START(src_PIANO_bin), (float*)BINARY_START(src_THEREMIN_bin),
    (float*)BINARY_START(src_PLUCK_bin), (float*)BINARY_START(src_OVERTONE1_bin), (float*)BINARY_START(src_OVERTONE2_bin), (float*)BINARY_START(src_SYMMETRY_bin), (float*)BINARY_START(src_CHIP_1_bin),
    (float*)BINARY_START(src_CHIP_2_bin), (float*)BINARY_START(src_BITCRUSH1_bin), (float*)BINARY_START(src_BITCRUSH2_bin), (float*)BINARY_START(src_VOICE_1_bin), (float*)BINARY_START(src_VOICE_2_bin),
    (float*)BINARY_START(src_VOICE_3_bin), (float*)BINARY_START(src_VOICE_4_bin), (float*)BINARY_START(src_VOICE_5_bin), (float*)BINARY_START(src_PWM_bin), (float*)BINARY_START(src_BI_PULSE_bin),
    (float*)BINARY_START(src_SAW_GAP1_bin), (float*)BINARY_START(src_SAW_GAP2_bin), (float*)BINARY_START(src_VIDEOGAME_bin)
};

static int32_t wavetable_lengths[NUM_DEXTER_WAVETABLES] = {
    4096, 1024, 8192, 8192, 8192, 8192, 2048, 8192,
    8192, 8192, 600, 600, 600, 600, 600, 600,
    600, 600, 600, 600, 600, 600, 600, 600,
    600, 600, 600, 600, 600, 600, 1024, 1024,
    1024, 1024, 600
};

static int32_t wavetable_sizes[NUM_DEXTER_WAVETABLES] = {
    8, 256, 8, 16, 16, 64, 124, 16,
    16, 8, 13, 8, 8, 8, 14, 30,
    8, 9, 8, 8, 14, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 64, 64,
    64, 128, 8
};
