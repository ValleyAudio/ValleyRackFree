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

static unsigned char* dexterWavetables[NUM_DEXTER_WAVETABLES] = {
    src_OPAL_bin, src_BASIC_bin, src_TEE_EKS_bin, src_SINE_HARM_bin,
    src_ADD_SINE_bin, src_AM_HARM_bin, src_SWEEPHARM_bin, src_ADD_SAW_bin,
    src_ADD_SQR_bin, src_ADD_BANK1_bin, src_OBOE_bin, src_ALTOSAX_bin,
    src_CELLO_1_bin, src_CELLO_2_bin, src_VIOLIN_bin, src_PIANO_bin, src_THEREMIN_bin,
    src_PLUCK_bin, src_OVERTONE1_bin, src_OVERTONE2_bin, src_SYMMETRY_bin, src_CHIP_1_bin,
    src_CHIP_2_bin, src_BITCRUSH1_bin, src_BITCRUSH2_bin, src_VOICE_1_bin, src_VOICE_2_bin,
    src_VOICE_3_bin, src_VOICE_4_bin, src_VOICE_5_bin, src_PWM_bin, src_BI_PULSE_bin,
    src_SAW_GAP1_bin, src_SAW_GAP2_bin, src_VIDEOGAME_bin
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
