#pragma once

#include "../Common/Wavetables/Wavetables.hpp"
#define NUM_TERRORFORM_WAVETABLES 64

BINARY(ADD_BANK1_bin);
BINARY(ADD_BANK2_bin);
BINARY(ADD_BANK3_bin);
BINARY(ADD_BANK4_bin);
BINARY(ADD_SAW_bin);
BINARY(ADD_SINE_bin);
BINARY(ADD_SQR_bin);
BINARY(ALTOSAX_bin);
BINARY(AM_HARM_bin);
BINARY(BASIC_bin);
BINARY(BI_PULSE_bin);
BINARY(BITCRUSH1_bin);
BINARY(BITCRUSH2_bin);
BINARY(CELLO_1_bin);
BINARY(CELLO_2_bin);
BINARY(CHIP_1_bin);
BINARY(CHIP_2_bin);
BINARY(CHIRP_bin);
BINARY(DISTORT_bin);
BINARY(EBASS_bin);
BINARY(FM1_bin);
BINARY(FM2_bin);
BINARY(FM3_bin);
BINARY(FM4_bin);
BINARY(FOLD_SINE_bin);
BINARY(GMTRY_1_bin);
BINARY(GMTRY_2_bin);
BINARY(GMTRY_3_bin);
BINARY(GRIT_bin);
BINARY(LINEAR_bin);
BINARY(OBOE_bin);
BINARY(OPAL_bin);
BINARY(OVERTONE1_bin);
BINARY(OVERTONE2_bin);
BINARY(PIANO_bin);
BINARY(PLAITS_1_bin);
BINARY(PLAITS_2_bin);
BINARY(PLAITS_3_bin);
BINARY(PLUCK_bin);
BINARY(PWM_bin);
BINARY(REED_bin);
BINARY(RESO_SAW_bin);
BINARY(RESO_SQR_bin);
BINARY(SAW_GAP1_bin);
BINARY(SAW_GAP2_bin);
BINARY(SAW_PHASE_bin);
BINARY(SINE_HARM_bin);
BINARY(SWEEPHARM_bin);
BINARY(SYMMETRY_bin);
BINARY(TEE_EKS_bin);
BINARY(THEREMIN_bin);
BINARY(TWO_OP_RAND_bin);
BINARY(TWO_OPFM1_bin);
BINARY(TWO_OPFM2_bin);
BINARY(VIDEOGAME_bin);
BINARY(VIOLIN_bin);
BINARY(VOICE_1_bin);
BINARY(VOICE_2_bin);
BINARY(VOICE_3_bin);
BINARY(VOICE_4_bin);
BINARY(VOICE_5_bin);
BINARY(VOICE_6_bin);
BINARY(VOX_MACH_bin);
BINARY(XFADE_bin);

// static float** wavetables[NUM_TERRORFORM_WAVETABLES] = {
//     wavetable_basic, wavetable_opal, wavetable_TeeEks, wavetable_sawPhase,
//     wavetable_sawGap, wavetable_sawGap2, wavetable_pwm, wavetable_biPulse,
//     wavetable_resoSaw, wavetable_resoSquare, wavetable_sineHarmonics, wavetable_amHarmonic,
//     wavetable_sweepHarmonic, wavetable_xfade, wavetable_chirp, wavetable_additiveSine,
//     wavetable_additiveSaw, wavetable_additiveSquare, wavetable_additiveBank, wavetable_additiveBank2,
//     wavetable_additiveBank3, wavetable_additiveBank4, wavetable_foldSine, wavetable_twoOpFMBank,
//     wavetable_twoOpFM1, wavetable_twoOpFM2, wavetable_fm1, wavetable_fm2,
//     wavetable_fm3, wavetable_fm4, wavetable_oboe, wavetable_altosax,
//     wavetable_reed, wavetable_cello1, wavetable_cello2, wavetable_violin,
//     wavetable_piano, wavetable_ebass, wavetable_pluck, wavetable_theremin, wavetable_overtone1,
//     wavetable_overtone2, wavetable_voice1, wavetable_voice2, wavetable_voice3, wavetable_voice4,
//     wavetable_voice5, wavetable_voice6, wavetable_machine, wavetable_symetric, wavetable_chip1, wavetable_chip2,
//     wavetable_bitcrush1, wavetable_bitcrush2, wavetable_vgame, wavetable_linear1,
//     wavetable_distort, wavetable_grit, wavetable_gmtry_1, wavetable_gmtry_2,
//     wavetable_gmtry_3, wavetable_plaits2, wavetable_plaits3, wavetable_plaits4
// };

unsigned char* wavetables[NUM_TERRORFORM_WAVETABLES] = {
    BASIC_bin, OPAL_bin, TEE_EKS_bin, SAW_PHASE_bin,
    SAW_GAP1_bin, SAW_GAP2_bin, PWM_bin, BI_PULSE_bin,
    RESO_SAW_bin, RESO_SQR_bin, SINE_HARM_bin, AM_HARM_bin,
    SWEEPHARM_bin, XFADE_bin, CHIRP_bin, ADD_SINE_bin,
    ADD_SAW_bin, ADD_SQR_bin, ADD_BANK1_bin, ADD_BANK2_bin,
    ADD_BANK3_bin, ADD_BANK4_bin, FOLD_SINE_bin, TWO_OP_RAND_bin,
    TWO_OPFM1_bin, TWO_OPFM2_bin, FM1_bin, FM2_bin,
    FM3_bin, FM4_bin, OBOE_bin, ALTOSAX_bin,
    REED_bin, CELLO_1_bin, CELLO_2_bin, VIOLIN_bin,
    PIANO_bin, EBASS_bin, PLUCK_bin, THEREMIN_bin, OVERTONE1_bin,
    OVERTONE2_bin, VOICE_1_bin, VOICE_2_bin, VOICE_3_bin, VOICE_4_bin,
    VOICE_5_bin, VOICE_6_bin, VOX_MACH_bin, SYMMETRY_bin, CHIP_1_bin, CHIP_2_bin,
    BITCRUSH1_bin, BITCRUSH2_bin, VIDEOGAME_bin, LINEAR_bin,
    DISTORT_bin, GRIT_bin, GMTRY_1_bin, GMTRY_2_bin,
    GMTRY_3_bin, PLAITS_1_bin, PLAITS_2_bin, PLAITS_3_bin
};

static long* wavetable_lengths[NUM_TERRORFORM_WAVETABLES] = {
    wavetable_basic_lengths, wavetable_opal_lengths, wavetable_TeeEks_lengths, wavetable_sawPhase_lengths,
    wavetable_sawGap_lengths, wavetable_sawGap2_lengths, wavetable_pwm_lengths, wavetable_biPulse_lengths,
    wavetable_resoSaw_lengths, wavetable_resoSquare_lengths, wavetable_sineHarmonics_lengths, wavetable_amHarmonic_lengths,
    wavetable_sweepHarmonic_lengths, wavetable_xfade_lengths, wavetable_chirp_lengths, wavetable_additiveSine_lengths,
    wavetable_additiveSaw_lengths, wavetable_additiveSquare_lengths, wavetable_additiveBank_lengths, wavetable_additiveBank2_lengths,
    wavetable_additiveBank3_lengths, wavetable_additiveBank4_lengths, wavetable_foldSine_lengths, wavetable_twoOpFMBank_lengths,
    wavetable_twoOpFM1_lengths, wavetable_twoOpFM2_lengths, wavetable_fm1_lengths, wavetable_fm2_lengths,
    wavetable_fm3_lengths, wavetable_fm4_lengths, wavetable_oboe_lengths, wavetable_altosax_lengths,
    wavetable_reed_lengths, wavetable_cello1_lengths, wavetable_cello2_lengths, wavetable_violin_lengths,
    wavetable_piano_lengths, wavetable_ebass_lengths, wavetable_pluck_lengths, wavetable_theremin_lengths,
    wavetable_overtone1_lengths, wavetable_overtone2_lengths, wavetable_voice1_lengths,
    wavetable_voice2_lengths, wavetable_voice3_lengths, wavetable_voice4_lengths, wavetable_voice5_lengths,
    wavetable_voice6_lengths, wavetable_machine_lengths, wavetable_symetric_lengths, wavetable_chip1_lengths,
    wavetable_chip2_lengths, wavetable_bitcrush1_lengths, wavetable_bitcrush2_lengths, wavetable_vgame_lengths,
    wavetable_linear1_lengths, wavetable_distort_lengths, wavetable_grit_lengths, wavetable_gmtry_1_lengths,
    wavetable_gmtry_2_lengths, wavetable_gmtry_3_lengths, wavetable_plaits2_lengths, wavetable_plaits3_lengths, wavetable_plaits4_lengths
};

static int32_t wavetable_sizes[NUM_TERRORFORM_WAVETABLES] = {
    WAVETABLE_BASIC_NUM, WAVETABLE_OPAL_NUM, WAVETABLE_TEEEKS_NUM, WAVETABLE_SAWPHASE_NUM,
    WAVETABLE_SAWGAP_NUM, WAVETABLE_SAWGAP2_NUM, WAVETABLE_PWM_NUM, WAVETABLE_BIPULSE_NUM,
    WAVETABLE_RESOSAW_NUM, WAVETABLE_RESOSQUARE_NUM, WAVETABLE_SINEHARMONICS_NUM, WAVETABLE_AMHARMONIC_NUM,
    WAVETABLE_SWEEPHARMONIC_NUM, WAVETABLE_XFADE_NUM, WAVETABLE_CHIRP_NUM, WAVETABLE_ADDITIVESINE_NUM,
    WAVETABLE_ADDITIVESAW_NUM, WAVETABLE_ADDITIVESQUARE_NUM, WAVETABLE_ADDITIVEBANK_NUM, WAVETABLE_ADDITIVEBANK2_NUM,
    WAVETABLE_ADDITIVEBANK3_NUM, WAVETABLE_ADDITIVEBANK4_NUM, WAVETABLE_FOLDSINE_NUM, WAVETABLE_TWOOPFMBANK_NUM,
    WAVETABLE_TWOOPFM1_NUM, WAVETABLE_TWOOPFM2_NUM, WAVETABLE_FM1_NUM, WAVETABLE_FM2_NUM,
    WAVETABLE_FM3_NUM, WAVETABLE_FM4_NUM, WAVETABLE_OBOE_NUM, WAVETABLE_ALTOSAX_NUM,
    WAVETABLE_REED_NUM, WAVETABLE_CELLO1_NUM, WAVETABLE_CELLO2_NUM, WAVETABLE_VIOLIN_NUM,
    WAVETABLE_PIANO_NUM, WAVETABLE_EBASS_NUM, WAVETABLE_PLUCK_NUM, WAVETABLE_THEREMIN_NUM,
    WAVETABLE_OVERTONE1_NUM, WAVETABLE_OVERTONE2_NUM, WAVETABLE_VOICE1_NUM, WAVETABLE_VOICE2_NUM,
    WAVETABLE_VOICE3_NUM, WAVETABLE_VOICE4_NUM, WAVETABLE_VOICE5_NUM, WAVETABLE_VOICE6_NUM,
    WAVETABLE_MACHINE_NUM, WAVETABLE_SYMETRIC_NUM, WAVETABLE_CHIP1_NUM, WAVETABLE_CHIP2_NUM, WAVETABLE_BITCRUSH1_NUM,
    WAVETABLE_BITCRUSH2_NUM, WAVETABLE_VGAME_NUM, WAVETABLE_LINEAR1_NUM, WAVETABLE_DISTORT_NUM,
    WAVETABLE_GRIT_NUM, WAVETABLE_GMTRY_1_NUM, WAVETABLE_GMTRY_2_NUM, WAVETABLE_GMTRY_3_NUM,
    WAVETABLE_PLAITS2_NUM, WAVETABLE_PLAITS3_NUM, WAVETABLE_PLAITS4_NUM
};
