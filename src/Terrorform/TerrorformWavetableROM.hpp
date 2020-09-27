#pragma once

#define NUM_TERRORFORM_WAVETABLES 64

BINARY(src_ADD_BANK1_bin);
BINARY(src_ADD_BANK2_bin);
BINARY(src_ADD_BANK3_bin);
BINARY(src_ADD_BANK4_bin);
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
BINARY(src_CHIRP_bin);
BINARY(src_DISTORT_bin);
BINARY(src_EBASS_bin);
BINARY(src_FM1_bin);
BINARY(src_FM2_bin);
BINARY(src_FM3_bin);
BINARY(src_FM4_bin);
BINARY(src_FOLD_SINE_bin);
BINARY(src_GMTRY_1_bin);
BINARY(src_GMTRY_2_bin);
BINARY(src_GMTRY_3_bin);
BINARY(src_GRIT_bin);
BINARY(src_LINEAR_bin);
BINARY(src_OBOE_bin);
BINARY(src_OPAL_bin);
BINARY(src_OVERTONE1_bin);
BINARY(src_OVERTONE2_bin);
BINARY(src_PIANO_bin);
BINARY(src_PLAITS_1_bin);
BINARY(src_PLAITS_2_bin);
BINARY(src_PLAITS_3_bin);
BINARY(src_PLUCK_bin);
BINARY(src_PWM_bin);
BINARY(src_REED_bin);
BINARY(src_RESO_SAW_bin);
BINARY(src_RESO_SQR_bin);
BINARY(src_SAW_GAP1_bin);
BINARY(src_SAW_GAP2_bin);
BINARY(src_SAW_PHASE_bin);
BINARY(src_SINE_HARM_bin);
BINARY(src_SWEEPHARM_bin);
BINARY(src_SYMMETRY_bin);
BINARY(src_TEE_EKS_bin);
BINARY(src_THEREMIN_bin);
BINARY(src_TWO_OP_RAND_bin);
BINARY(src_TWO_OPFM1_bin);
BINARY(src_TWO_OPFM2_bin);
BINARY(src_VIDEOGAME_bin);
BINARY(src_VIOLIN_bin);
BINARY(src_VOICE_1_bin);
BINARY(src_VOICE_2_bin);
BINARY(src_VOICE_3_bin);
BINARY(src_VOICE_4_bin);
BINARY(src_VOICE_5_bin);
BINARY(src_VOICE_6_bin);
BINARY(src_VOX_MACH_bin);
BINARY(src_XFADE_bin);

static unsigned char* wavetables[NUM_TERRORFORM_WAVETABLES] = {
    src_BASIC_bin, src_OPAL_bin, src_TEE_EKS_bin, src_SAW_PHASE_bin,
    src_SAW_GAP1_bin, src_SAW_GAP2_bin, src_PWM_bin, src_BI_PULSE_bin,
    src_RESO_SAW_bin, src_RESO_SQR_bin, src_SINE_HARM_bin, src_AM_HARM_bin,
    src_SWEEPHARM_bin, src_XFADE_bin, src_CHIRP_bin, src_ADD_SINE_bin,
    src_ADD_SAW_bin, src_ADD_SQR_bin, src_ADD_BANK1_bin, src_ADD_BANK2_bin,
    src_ADD_BANK3_bin, src_ADD_BANK4_bin, src_FOLD_SINE_bin, src_TWO_OP_RAND_bin,
    src_TWO_OPFM1_bin, src_TWO_OPFM2_bin, src_FM1_bin, src_FM2_bin,
    src_FM3_bin, src_FM4_bin, src_OBOE_bin, src_ALTOSAX_bin,
    src_REED_bin, src_CELLO_1_bin, src_CELLO_2_bin, src_VIOLIN_bin,
    src_PIANO_bin, src_EBASS_bin, src_PLUCK_bin, src_THEREMIN_bin, src_OVERTONE1_bin,
    src_OVERTONE2_bin, src_VOICE_1_bin, src_VOICE_2_bin, src_VOICE_3_bin, src_VOICE_4_bin,
    src_VOICE_5_bin, src_VOICE_6_bin, src_VOX_MACH_bin, src_SYMMETRY_bin, src_CHIP_1_bin, src_CHIP_2_bin,
    src_BITCRUSH1_bin, src_BITCRUSH2_bin, src_VIDEOGAME_bin, src_LINEAR_bin,
    src_DISTORT_bin, src_GRIT_bin, src_GMTRY_1_bin, src_GMTRY_2_bin,
    src_GMTRY_3_bin, src_PLAITS_1_bin, src_PLAITS_2_bin, src_PLAITS_3_bin
};

// Yeah I know, it's magical
static int32_t wavetable_lengths[NUM_TERRORFORM_WAVETABLES] = {
    1024, 4096, 8192, 256, 1024, 1024, 1024, 1024,
    512, 512, 8192, 8192, 2048, 1024, 512, 8192,
    8192, 8192, 8192, 1024, 1024, 1024, 1024, 1024,
    1024, 1024, 1024, 1024, 1024, 1024, 600, 600,
    1024, 600, 600, 600, 600, 1024, 600, 600, 600,
    600, 600, 600, 600, 600, 600, 512, 1024,
    600, 600, 600, 600, 600, 600, 600, 600,
    1024, 512, 512, 1024, 512, 512, 512
};

// Can you feel the magic?
static int32_t wavetable_sizes[NUM_TERRORFORM_WAVETABLES] = {
    256, 8, 8, 256, 64, 128, 64, 64,
    8, 8, 16, 64, 124, 8, 8, 16,
    16, 16, 8, 8, 8, 8, 30, 8,
    128, 128, 128, 128, 128, 128, 13, 8,
    8, 8, 8, 14, 30, 8, 9, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 14, 8, 8, 8, 8, 8, 8,
    8, 8, 64, 8, 8, 8, 8, 8
};
