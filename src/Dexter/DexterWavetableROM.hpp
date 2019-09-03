#pragma once

#include "../Common/Wavetables/Wavetables.hpp"
#define NUM_DEXTER_WAVETABLES 35

static float** wavetables[NUM_DEXTER_WAVETABLES] = {
    wavetable_opal, wavetable_basic, wavetable_TeeEks, wavetable_sineHarmonics,
    wavetable_additiveSine, wavetable_amHarmonic, wavetable_sweepHarmonic, wavetable_additiveSaw,
    wavetable_additiveSquare, wavetable_additiveBank, wavetable_oboe, wavetable_altosax,
    wavetable_cello1, wavetable_cello2, wavetable_violin, wavetable_piano, wavetable_theremin,
    wavetable_pluck, wavetable_overtone1, wavetable_overtone2, wavetable_symetric, wavetable_chip1,
    wavetable_chip2, wavetable_bitcrush1, wavetable_bitcrush2, wavetable_voice1, wavetable_voice2,
    wavetable_voice3, wavetable_voice4, wavetable_voice5, wavetable_pwm, wavetable_biPulse,
    wavetable_sawGap, wavetable_sawGap2, wavetable_vgame
};

static long* wavetable_lengths[NUM_DEXTER_WAVETABLES] = {
    wavetable_opal_lengths, wavetable_basic_lengths, wavetable_TeeEks_lengths, wavetable_sineHarmonics_lengths,
    wavetable_additiveSine_lengths, wavetable_amHarmonic_lengths, wavetable_sweepHarmonic_lengths, wavetable_additiveSaw_lengths,
    wavetable_additiveSquare_lengths, wavetable_additiveBank_lengths, wavetable_oboe_lengths, wavetable_altosax_lengths,
    wavetable_cello1_lengths, wavetable_cello2_lengths, wavetable_violin_lengths, wavetable_piano_lengths, wavetable_theremin_lengths,
    wavetable_pluck_lengths, wavetable_overtone1_lengths, wavetable_overtone2_lengths, wavetable_symetric_lengths, wavetable_chip1_lengths,
    wavetable_chip2_lengths, wavetable_bitcrush1_lengths, wavetable_bitcrush2_lengths, wavetable_voice1_lengths, wavetable_voice2_lengths,
    wavetable_voice3_lengths, wavetable_voice4_lengths, wavetable_voice5_lengths, wavetable_pwm_lengths, wavetable_biPulse_lengths,
    wavetable_sawGap_lengths, wavetable_sawGap2_lengths, wavetable_vgame_lengths
};

static int32_t wavetable_sizes[NUM_DEXTER_WAVETABLES] = {
    WAVETABLE_OPAL_NUM, WAVETABLE_BASIC_NUM, WAVETABLE_TEEEKS_NUM, WAVETABLE_SINEHARMONICS_NUM,
    WAVETABLE_ADDITIVESINE_NUM, WAVETABLE_AMHARMONIC_NUM, WAVETABLE_SWEEPHARMONIC_NUM, WAVETABLE_ADDITIVESAW_NUM,
    WAVETABLE_ADDITIVESQUARE_NUM, WAVETABLE_ADDITIVEBANK_NUM, WAVETABLE_OBOE_NUM, WAVETABLE_ALTOSAX_NUM,
    WAVETABLE_CELLO1_NUM, WAVETABLE_CELLO2_NUM, WAVETABLE_VIOLIN_NUM, WAVETABLE_PIANO_NUM, WAVETABLE_THEREMIN_NUM,
    WAVETABLE_PLUCK_NUM, WAVETABLE_OVERTONE1_NUM, WAVETABLE_OVERTONE2_NUM, WAVETABLE_SYMETRIC_NUM, WAVETABLE_CHIP1_NUM,
    WAVETABLE_CHIP2_NUM, WAVETABLE_BITCRUSH1_NUM, WAVETABLE_BITCRUSH2_NUM, WAVETABLE_VOICE1_NUM, WAVETABLE_VOICE2_NUM,
    WAVETABLE_VOICE3_NUM, WAVETABLE_VOICE4_NUM, WAVETABLE_VOICE5_NUM, WAVETABLE_PWM_NUM, WAVETABLE_BIPULSE_NUM,
    WAVETABLE_SAWGAP_NUM, WAVETABLE_SAWGAP2_NUM, WAVETABLE_VGAME_NUM
};
