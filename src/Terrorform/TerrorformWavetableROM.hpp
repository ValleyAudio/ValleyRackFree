#pragma once

#include "../Common/Wavetables/Wavetables.hpp"
#define NUM_TERRORFORM_WAVETABLES 66

static float** wavetables[NUM_TERRORFORM_WAVETABLES] = {
    wavetable_opal, wavetable_basic, wavetable_TeeEks, wavetable_sineHarmonics,
    wavetable_additiveSine, wavetable_amHarmonic, wavetable_sweepHarmonic, wavetable_xfade,
    wavetable_additiveSaw, wavetable_additiveSquare, wavetable_additiveBank, wavetable_additiveBank2,
    wavetable_additiveBank3, wavetable_additiveBank4, wavetable_oboe, wavetable_altosax,
    wavetable_reed, wavetable_cello1, wavetable_cello2, wavetable_violin,
    wavetable_piano, wavetable_theremin, wavetable_pluck, wavetable_overtone1,
    wavetable_overtone2, wavetable_symetric, wavetable_chip1, wavetable_chip2,
    wavetable_bitcrush1, wavetable_bitcrush2, wavetable_grit, wavetable_voice1,
    wavetable_voice2, wavetable_voice3, wavetable_voice4, wavetable_voice5,
    wavetable_voice6, wavetable_pwm, wavetable_biPulse, wavetable_sawGap,
    wavetable_sawGap2, wavetable_sawPhase, wavetable_vgame, wavetable_foldSine,
    wavetable_fm1, wavetable_fm2, wavetable_fm3, wavetable_fm4,
    wavetable_fm5, wavetable_fm6, wavetable_twoOpFM1, wavetable_twoOpFM2,
    wavetable_twoOpFMBank, wavetable_machine, wavetable_linear1, wavetable_plaits2,
    wavetable_plaits3, wavetable_plaits4, wavetable_gmtry_1, wavetable_gmtry_2,
    wavetable_gmtry_3, wavetable_resoSaw, wavetable_resoSquare, wavetable_chirp,
    wavetable_distort, wavetable_ebass
};

static long* wavetable_lengths[NUM_TERRORFORM_WAVETABLES] = {
    wavetable_opal_lengths, wavetable_basic_lengths, wavetable_TeeEks_lengths, wavetable_sineHarmonics_lengths,
    wavetable_additiveSine_lengths, wavetable_amHarmonic_lengths, wavetable_sweepHarmonic_lengths, wavetable_xfade_lengths,
    wavetable_additiveSaw_lengths, wavetable_additiveSquare_lengths, wavetable_additiveBank_lengths, wavetable_additiveBank2_lengths,
    wavetable_additiveBank3_lengths, wavetable_additiveBank4_lengths, wavetable_oboe_lengths, wavetable_altosax_lengths,
    wavetable_reed_lengths, wavetable_cello1_lengths, wavetable_cello2_lengths, wavetable_violin_lengths,
    wavetable_piano_lengths, wavetable_theremin_lengths, wavetable_pluck_lengths, wavetable_overtone1_lengths,
    wavetable_overtone2_lengths, wavetable_symetric_lengths, wavetable_chip1_lengths, wavetable_chip2_lengths,
    wavetable_bitcrush1_lengths, wavetable_bitcrush2_lengths, wavetable_grit_lengths, wavetable_voice1_lengths,
    wavetable_voice2_lengths, wavetable_voice3_lengths, wavetable_voice4_lengths, wavetable_voice5_lengths,
    wavetable_voice6_lengths, wavetable_pwm_lengths, wavetable_biPulse_lengths, wavetable_sawGap_lengths,
    wavetable_sawGap2_lengths, wavetable_sawPhase_lengths, wavetable_vgame_lengths, wavetable_foldSine_lengths,
    wavetable_fm1_lengths, wavetable_fm2_lengths, wavetable_fm3_lengths, wavetable_fm4_lengths,
    wavetable_fm5_lengths, wavetable_fm6_lengths, wavetable_twoOpFM1_lengths, wavetable_twoOpFM2_lengths,
    wavetable_twoOpFMBank_lengths, wavetable_machine_lengths, wavetable_linear1_lengths, wavetable_plaits2_lengths,
    wavetable_plaits3_lengths, wavetable_plaits4_lengths, wavetable_gmtry_1_lengths, wavetable_gmtry_2_lengths,
    wavetable_gmtry_3_lengths, wavetable_resoSaw_lengths, wavetable_resoSquare_lengths, wavetable_chirp_lengths,
    wavetable_distort_lengths, wavetable_ebass_lengths
};

static int32_t wavetable_sizes[NUM_TERRORFORM_WAVETABLES] = {
    WAVETABLE_OPAL_NUM, WAVETABLE_BASIC_NUM, WAVETABLE_TEEEKS_NUM, WAVETABLE_SINEHARMONICS_NUM,
    WAVETABLE_ADDITIVESINE_NUM, WAVETABLE_AMHARMONIC_NUM, WAVETABLE_SWEEPHARMONIC_NUM, WAVETABLE_XFADE_NUM,
    WAVETABLE_ADDITIVESAW_NUM, WAVETABLE_ADDITIVESQUARE_NUM, WAVETABLE_ADDITIVEBANK_NUM, WAVETABLE_ADDITIVEBANK2_NUM,
    WAVETABLE_ADDITIVEBANK3_NUM, WAVETABLE_ADDITIVEBANK4_NUM, WAVETABLE_OBOE_NUM, WAVETABLE_ALTOSAX_NUM,
    WAVETABLE_REED_NUM, WAVETABLE_CELLO1_NUM, WAVETABLE_CELLO2_NUM, WAVETABLE_VIOLIN_NUM,
    WAVETABLE_PIANO_NUM, WAVETABLE_THEREMIN_NUM, WAVETABLE_PLUCK_NUM, WAVETABLE_OVERTONE1_NUM,
    WAVETABLE_OVERTONE2_NUM, WAVETABLE_SYMETRIC_NUM, WAVETABLE_CHIP1_NUM, WAVETABLE_CHIP2_NUM,
    WAVETABLE_BITCRUSH1_NUM, WAVETABLE_BITCRUSH2_NUM, WAVETABLE_GRIT_NUM, WAVETABLE_VOICE1_NUM,
    WAVETABLE_VOICE2_NUM, WAVETABLE_VOICE3_NUM, WAVETABLE_VOICE4_NUM, WAVETABLE_VOICE5_NUM,
    WAVETABLE_VOICE6_NUM, WAVETABLE_PWM_NUM, WAVETABLE_BIPULSE_NUM, WAVETABLE_SAWGAP_NUM,
    WAVETABLE_SAWGAP2_NUM, WAVETABLE_SAWPHASE_NUM, WAVETABLE_VGAME_NUM, WAVETABLE_FOLDSINE_NUM,
    WAVETABLE_FM1_NUM, WAVETABLE_FM2_NUM, WAVETABLE_FM3_NUM, WAVETABLE_FM4_NUM,
    WAVETABLE_FM5_NUM, WAVETABLE_FM6_NUM, WAVETABLE_TWOOPFM1_NUM, WAVETABLE_TWOOPFM2_NUM,
    WAVETABLE_TWOOPFMBANK_NUM, WAVETABLE_MACHINE_NUM, WAVETABLE_LINEAR1_NUM, WAVETABLE_PLAITS2_NUM,
    WAVETABLE_PLAITS3_NUM, WAVETABLE_PLAITS4_NUM, WAVETABLE_GMTRY_1_NUM, WAVETABLE_GMTRY_2_NUM,
    WAVETABLE_GMTRY_3_NUM, WAVETABLE_RESOSAW_NUM, WAVETABLE_RESOSQUARE_NUM, WAVETABLE_CHIRP_NUM,
    WAVETABLE_DISTORT_NUM, WAVETABLE_EBASS_NUM
};
