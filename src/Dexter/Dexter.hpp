//
// Dexter.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 18/3/2018
//
// Copyright 2018 Dale Johnson. Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met: 1. Redistributions of
// source code must retain the above copyright notice, this list of conditions and the following
// disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this
// list of conditions and the following disclaimer in the documentation and/or other materials
// provided with the distribution. 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this software without
// specific prior written permission.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.

#ifndef VALLEY_DEXTER_HPP
#define VALLEY_DEXTER_HPP

#include "../Valley.hpp"
#include "../gui/ValleyComponents.hpp"
#include "../gui/ValleyChoiceMenu.hpp"
#include "Chords.hpp"
#include "DexterCore.hpp"
#include "DexterRoutingMatrix.hpp"

struct Dexter : Module {
    static constexpr unsigned long kNumShapeModes = 12;
    static constexpr unsigned long kNumSyncModes = 15;
    static constexpr unsigned long kNumModDests = 11;
    static constexpr unsigned long kNumCVInputs = 12;

    enum ParamIds {
        CHORD_PARAM, INVERT_PARAM, DETUNE_PARAM,
        OCTAVE_PARAM, COARSE_PARAM, FINE_PARAM, B_OCTAVE_PARAM, B_COARSE_PARAM, B_FINE_PARAM,
        ALGORITHM_PARAM, BRIGHTNESS_PARAM, SHAPE_PARAM, FEEDBACK_PARAM,

        CHORD_DEPTH_PARAM, INVERT_DEPTH_PARAM, DETUNE_DEPTH_PARAM, ALGO_DEPTH_PARAM, FB_DEPTH_PARAM,
        BRIGHT_DEPTH_PARAM, SHAPE_DEPTH_PARAM, VOCT_1_DEPTH_PARAM, VOCT_2_DEPTH_PARAM,

        MASTER_LFO_BUTTON, RESET_PHASE_BUTTON,

        OP_1_MULT_PARAM, OP_1_COARSE_PARAM, OP_1_FINE_PARAM, OP_1_WAVE_PARAM, OP_1_SHAPE_PARAM, OP_1_LEVEL_PARAM,
        OP_1_SETTINGS_PARAM, OP_1_POST_SHAPE_PARAM, OP_1_LFO_PARAM, OP_1_WEAK_PARAM, OP_1_SYNC_PARAM,
        OP_1_WAVE_MENU_PARAM, OP_1_MODA_MENU_PARAM, OP_1_MODB_MENU_PARAM, OP_1_MISC_MENU_PARAM, OP_1_BANK_PARAM,
        OP_1_PRE_PARAM,
        OP_2_MULT_PARAM, OP_2_COARSE_PARAM, OP_2_FINE_PARAM, OP_2_WAVE_PARAM, OP_2_SHAPE_PARAM, OP_2_LEVEL_PARAM,
        OP_2_SETTINGS_PARAM, OP_2_POST_SHAPE_PARAM, OP_2_LFO_PARAM, OP_2_WEAK_PARAM, OP_2_SYNC_PARAM,
        OP_2_WAVE_MENU_PARAM, OP_2_MODA_MENU_PARAM, OP_2_MODB_MENU_PARAM, OP_2_MISC_MENU_PARAM, OP_2_BANK_PARAM,
        OP_2_PRE_PARAM,
        OP_3_MULT_PARAM, OP_3_COARSE_PARAM, OP_3_FINE_PARAM, OP_3_WAVE_PARAM, OP_3_SHAPE_PARAM, OP_3_LEVEL_PARAM,
        OP_3_SETTINGS_PARAM, OP_3_POST_SHAPE_PARAM, OP_3_LFO_PARAM, OP_3_WEAK_PARAM, OP_3_SYNC_PARAM,
        OP_3_WAVE_MENU_PARAM, OP_3_MODA_MENU_PARAM, OP_3_MODB_MENU_PARAM, OP_3_MISC_MENU_PARAM, OP_3_BANK_PARAM,
        OP_3_PRE_PARAM,
        OP_4_MULT_PARAM, OP_4_COARSE_PARAM, OP_4_FINE_PARAM, OP_4_WAVE_PARAM, OP_4_SHAPE_PARAM, OP_4_LEVEL_PARAM,
        OP_4_SETTINGS_PARAM, OP_4_POST_SHAPE_PARAM, OP_4_LFO_PARAM, OP_4_WEAK_PARAM, OP_4_SYNC_PARAM,
        OP_4_WAVE_MENU_PARAM, OP_4_MODA_MENU_PARAM, OP_4_MODB_MENU_PARAM, OP_4_MISC_MENU_PARAM, OP_4_BANK_PARAM,
        OP_4_PRE_PARAM,

        OP_1_MOD_1_PARAM, OP_1_MOD_2_PARAM, OP_1_MOD_3_PARAM, OP_1_MOD_4_PARAM,
        OP_1_PITCH_CV1_PARAM, OP_1_WAVE_CV1_PARAM, OP_1_SHAPE_CV1_PARAM, OP_1_LEVEL_CV1_PARAM,
        OP_1_PITCH_CV2_PARAM, OP_1_WAVE_CV2_PARAM, OP_1_SHAPE_CV2_PARAM, OP_1_LEVEL_CV2_PARAM,
        OP_2_MOD_1_PARAM, OP_2_MOD_2_PARAM, OP_2_MOD_3_PARAM, OP_2_MOD_4_PARAM,
        OP_2_PITCH_CV1_PARAM, OP_2_WAVE_CV1_PARAM, OP_2_SHAPE_CV1_PARAM, OP_2_LEVEL_CV1_PARAM,
        OP_2_PITCH_CV2_PARAM, OP_2_WAVE_CV2_PARAM, OP_2_SHAPE_CV2_PARAM, OP_2_LEVEL_CV2_PARAM,
        OP_3_MOD_1_PARAM, OP_3_MOD_2_PARAM, OP_3_MOD_3_PARAM, OP_3_MOD_4_PARAM,
        OP_3_PITCH_CV1_PARAM, OP_3_WAVE_CV1_PARAM, OP_3_SHAPE_CV1_PARAM, OP_3_LEVEL_CV1_PARAM,
        OP_3_PITCH_CV2_PARAM, OP_3_WAVE_CV2_PARAM, OP_3_SHAPE_CV2_PARAM, OP_3_LEVEL_CV2_PARAM,
        OP_4_MOD_1_PARAM, OP_4_MOD_2_PARAM, OP_4_MOD_3_PARAM, OP_4_MOD_4_PARAM,
        OP_4_PITCH_CV1_PARAM, OP_4_WAVE_CV1_PARAM, OP_4_SHAPE_CV1_PARAM, OP_4_LEVEL_CV1_PARAM,
        OP_4_PITCH_CV2_PARAM, OP_4_WAVE_CV2_PARAM, OP_4_SHAPE_CV2_PARAM, OP_4_LEVEL_CV2_PARAM,

        FULL_INVERSION_BUTTON,
        NUM_PARAMS
    };

    enum OpParamGroup {
        OP_MULT_PARAM,
        OP_COARSE_PARAM,
        OP_FINE_PARAM,
        OP_WAVE_PARAM,
        OP_SHAPE_PARAM,
        OP_LEVEL_PARAM,
        OP_SETTINGS_PARAM,
        OP_POST_SHAPE_PARAM,
        OP_LFO_PARAM,
        OP_WEAK_PARAM,
        OP_SYNC_PARAM,
        OP_WAVE_MENU_PARAM,
        OP_MODA_MENU_PARAM,
        OP_MODB_MENU_PARAM,
        OP_MISC_MENU_PARAM,
        OP_BANK_PARAM,
        OP_PRE_PARAM,
        NUM_PARAM_GROUPS
    };

    enum InputIds {
        CHORD_INPUT,
        INVERT_INPUT,
        DETUNE_INPUT,
        ALGO_INPUT,
        FEEDBACK_INPUT,
        BRIGHTNESS_INPUT,
        SHAPE_INPUT,
        A_VOCT_INPUT,
        B_VOCT_INPUT,

        OP_1_MOD_1_INPUT,   OP_1_MOD_2_INPUT,   OP_1_MOD_3_INPUT,   OP_1_MOD_4_INPUT,
        OP_1_PITCH_1_INPUT, OP_1_WAVE_1_INPUT,  OP_1_SHAPE_1_INPUT,  OP_1_LEVEL_1_INPUT,
        OP_1_PITCH_2_INPUT, OP_1_WAVE_2_INPUT,  OP_1_SHAPE_2_INPUT,  OP_1_LEVEL_2_INPUT,
        OP_2_MOD_1_INPUT,   OP_2_MOD_2_INPUT,   OP_2_MOD_3_INPUT,   OP_2_MOD_4_INPUT,
        OP_2_PITCH_1_INPUT, OP_2_WAVE_1_INPUT,  OP_2_SHAPE_1_INPUT,  OP_2_LEVEL_1_INPUT,
        OP_2_PITCH_2_INPUT, OP_2_WAVE_2_INPUT,  OP_2_SHAPE_2_INPUT,  OP_2_LEVEL_2_INPUT,
        OP_3_MOD_1_INPUT,   OP_3_MOD_2_INPUT,   OP_3_MOD_3_INPUT,   OP_3_MOD_4_INPUT,
        OP_3_PITCH_1_INPUT, OP_3_WAVE_1_INPUT,  OP_3_SHAPE_1_INPUT,  OP_3_LEVEL_1_INPUT,
        OP_3_PITCH_2_INPUT, OP_3_WAVE_2_INPUT,  OP_3_SHAPE_2_INPUT,  OP_3_LEVEL_2_INPUT,
        OP_4_MOD_1_INPUT,   OP_4_MOD_2_INPUT,   OP_4_MOD_3_INPUT,   OP_4_MOD_4_INPUT,
        OP_4_PITCH_1_INPUT, OP_4_WAVE_1_INPUT,  OP_4_SHAPE_1_INPUT,  OP_4_LEVEL_1_INPUT,
        OP_4_PITCH_2_INPUT, OP_4_WAVE_2_INPUT,  OP_4_SHAPE_2_INPUT,  OP_4_LEVEL_2_INPUT,
        NUM_INPUTS
    };

    enum OpCVGroup {
        OP_MOD_CV_1,
        OP_MOD_CV_2,
        OP_MOD_CV_3,
        OP_MOD_CV_4,
        OP_PITCH_CV_1,
        OP_WAVE_CV_1,
        OP_SHAPE_CV_1,
        OP_LEVEL_CV_1,
        OP_PITCH_CV_2,
        OP_WAVE_CV_2,
        OP_SHAPE_CV_2,
        OP_LEVEL_CV_2,
        NUM_CV_GROUPS
    };

    enum OutputIds {
        A_LEFT_OUTPUT,
        A_RIGHT_OUTPUT,
        OP_1_OUTPUT,
        OP_2_OUTPUT,
        OP_3_OUTPUT,
        OP_4_OUTPUT,
        B_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        MASTER_LFO_LIGHT, RESET_PHASE_LIGHT,
        OP_1_SETTINGS_LIGHT, OP_1_POST_SHAPE_LIGHT, OP_1_LFO_LIGHT, OP_1_WEAK_LIGHT, OP_1_SYNC_LIGHT,
        OP_1_WAVE_MENU_LIGHT, OP_1_MODA_MENU_LIGHT, OP_1_MODB_MENU_LIGHT, OP_1_MISC_MENU_LIGHT, OP_1_PRE_LIGHT,
        OP_2_SETTINGS_LIGHT, OP_2_POST_SHAPE_LIGHT, OP_2_LFO_LIGHT, OP_2_WEAK_LIGHT, OP_2_SYNC_LIGHT,
        OP_2_WAVE_MENU_LIGHT, OP_2_MODA_MENU_LIGHT, OP_2_MODB_MENU_LIGHT, OP_2_MISC_MENU_LIGHT, OP_2_PRE_LIGHT,
        OP_3_SETTINGS_LIGHT, OP_3_POST_SHAPE_LIGHT, OP_3_LFO_LIGHT, OP_3_WEAK_LIGHT, OP_3_SYNC_LIGHT,
        OP_3_WAVE_MENU_LIGHT, OP_3_MODA_MENU_LIGHT, OP_3_MODB_MENU_LIGHT, OP_3_MISC_MENU_LIGHT, OP_3_PRE_LIGHT,
        OP_4_SETTINGS_LIGHT, OP_4_POST_SHAPE_LIGHT, OP_4_LFO_LIGHT, OP_4_WEAK_LIGHT, OP_4_SYNC_LIGHT,
        OP_4_WAVE_MENU_LIGHT, OP_4_MODA_MENU_LIGHT, OP_4_MODB_MENU_LIGHT, OP_4_MISC_MENU_LIGHT, OP_4_PRE_LIGHT,
        FULL_INVERSION_LIGHT,
        NUM_LIGHTS
    };

    enum OpLightGroup {
        OP_SETTINGS,
        OP_POST_SHAPE,
        OP_LFO,
        OP_WEAK,
        OP_SYNC,
        OP_WAVE_LIGHT,
        OP_MODA_LIGHT,
        OP_MODB_LIGHT,
        OP_MISC_LIGHT,
        OP_PRE_LIGHT,
        NUM_LIGHT_GROUPS
    };

    ParamIds opParams[DexterCore::kNumOperators][NUM_PARAM_GROUPS] = {
        {OP_1_MULT_PARAM, OP_1_COARSE_PARAM, OP_1_FINE_PARAM, OP_1_WAVE_PARAM, OP_1_SHAPE_PARAM, OP_1_LEVEL_PARAM,
         OP_1_SETTINGS_PARAM, OP_1_POST_SHAPE_PARAM, OP_1_LFO_PARAM, OP_1_WEAK_PARAM, OP_1_SYNC_PARAM,
         OP_1_WAVE_MENU_PARAM, OP_1_MODA_MENU_PARAM, OP_1_MODB_MENU_PARAM, OP_1_MISC_MENU_PARAM, OP_1_BANK_PARAM,
         OP_1_PRE_PARAM},
        {OP_2_MULT_PARAM, OP_2_COARSE_PARAM, OP_2_FINE_PARAM, OP_2_WAVE_PARAM, OP_2_SHAPE_PARAM, OP_2_LEVEL_PARAM,
         OP_2_SETTINGS_PARAM, OP_2_POST_SHAPE_PARAM, OP_2_LFO_PARAM, OP_2_WEAK_PARAM, OP_2_SYNC_PARAM,
         OP_2_WAVE_MENU_PARAM, OP_2_MODA_MENU_PARAM, OP_2_MODB_MENU_PARAM, OP_2_MISC_MENU_PARAM, OP_2_BANK_PARAM,
         OP_2_PRE_PARAM},
        {OP_3_MULT_PARAM, OP_3_COARSE_PARAM, OP_3_FINE_PARAM, OP_3_WAVE_PARAM, OP_3_SHAPE_PARAM, OP_3_LEVEL_PARAM,
         OP_3_SETTINGS_PARAM, OP_3_POST_SHAPE_PARAM, OP_3_LFO_PARAM, OP_3_WEAK_PARAM, OP_3_SYNC_PARAM,
         OP_3_WAVE_MENU_PARAM, OP_3_MODA_MENU_PARAM, OP_3_MODB_MENU_PARAM, OP_3_MISC_MENU_PARAM, OP_3_BANK_PARAM,
         OP_3_PRE_PARAM},
        {OP_4_MULT_PARAM, OP_4_COARSE_PARAM, OP_4_FINE_PARAM, OP_4_WAVE_PARAM, OP_4_SHAPE_PARAM, OP_4_LEVEL_PARAM,
         OP_4_SETTINGS_PARAM, OP_4_POST_SHAPE_PARAM, OP_4_LFO_PARAM, OP_4_WEAK_PARAM, OP_4_SYNC_PARAM,
         OP_4_WAVE_MENU_PARAM, OP_4_MODA_MENU_PARAM, OP_4_MODB_MENU_PARAM, OP_4_MISC_MENU_PARAM, OP_4_BANK_PARAM,
         OP_4_PRE_PARAM}
    };

    LightIds opLights[DexterCore::kNumOperators][NUM_LIGHT_GROUPS] {
        {OP_1_SETTINGS_LIGHT, OP_1_POST_SHAPE_LIGHT, OP_1_LFO_LIGHT, OP_1_WEAK_LIGHT, OP_1_SYNC_LIGHT,
         OP_1_WAVE_MENU_LIGHT, OP_1_MODA_MENU_LIGHT, OP_1_MODB_MENU_LIGHT, OP_1_MISC_MENU_LIGHT, OP_1_PRE_LIGHT},
        {OP_2_SETTINGS_LIGHT, OP_2_POST_SHAPE_LIGHT, OP_2_LFO_LIGHT, OP_2_WEAK_LIGHT, OP_2_SYNC_LIGHT,
         OP_2_WAVE_MENU_LIGHT, OP_2_MODA_MENU_LIGHT, OP_2_MODB_MENU_LIGHT, OP_2_MISC_MENU_LIGHT, OP_2_PRE_LIGHT},
        {OP_3_SETTINGS_LIGHT, OP_3_POST_SHAPE_LIGHT, OP_3_LFO_LIGHT, OP_3_WEAK_LIGHT, OP_3_SYNC_LIGHT,
         OP_3_WAVE_MENU_LIGHT, OP_3_MODA_MENU_LIGHT, OP_3_MODB_MENU_LIGHT, OP_3_MISC_MENU_LIGHT, OP_3_PRE_LIGHT},
        {OP_4_SETTINGS_LIGHT, OP_4_POST_SHAPE_LIGHT, OP_4_LFO_LIGHT, OP_4_WEAK_LIGHT, OP_4_SYNC_LIGHT,
         OP_4_WAVE_MENU_LIGHT, OP_4_MODA_MENU_LIGHT, OP_4_MODB_MENU_LIGHT, OP_4_MISC_MENU_LIGHT, OP_4_PRE_LIGHT}
    };

    InputIds opCVInputs[DexterCore::kNumOperators][NUM_CV_GROUPS] = {
        {OP_1_MOD_1_INPUT,   OP_1_MOD_2_INPUT,   OP_1_MOD_3_INPUT,   OP_1_MOD_4_INPUT,
         OP_1_PITCH_1_INPUT, OP_1_WAVE_1_INPUT,  OP_1_SHAPE_1_INPUT,  OP_1_LEVEL_1_INPUT,
         OP_1_PITCH_2_INPUT, OP_1_WAVE_2_INPUT,  OP_1_SHAPE_2_INPUT,  OP_1_LEVEL_2_INPUT},
        {OP_2_MOD_1_INPUT,   OP_2_MOD_2_INPUT,   OP_2_MOD_3_INPUT,   OP_2_MOD_4_INPUT,
         OP_2_PITCH_1_INPUT, OP_2_WAVE_1_INPUT,  OP_2_SHAPE_1_INPUT,  OP_2_LEVEL_1_INPUT,
         OP_2_PITCH_2_INPUT, OP_2_WAVE_2_INPUT,  OP_2_SHAPE_2_INPUT,  OP_2_LEVEL_2_INPUT},
        {OP_3_MOD_1_INPUT,   OP_3_MOD_2_INPUT,   OP_3_MOD_3_INPUT,   OP_3_MOD_4_INPUT,
         OP_3_PITCH_1_INPUT, OP_3_WAVE_1_INPUT,  OP_3_SHAPE_1_INPUT,  OP_3_LEVEL_1_INPUT,
         OP_3_PITCH_2_INPUT, OP_3_WAVE_2_INPUT,  OP_3_SHAPE_2_INPUT,  OP_3_LEVEL_2_INPUT},
        {OP_4_MOD_1_INPUT,   OP_4_MOD_2_INPUT,   OP_4_MOD_3_INPUT,   OP_4_MOD_4_INPUT,
         OP_4_PITCH_1_INPUT, OP_4_WAVE_1_INPUT,  OP_4_SHAPE_1_INPUT,  OP_4_LEVEL_1_INPUT,
         OP_4_PITCH_2_INPUT, OP_4_WAVE_2_INPUT,  OP_4_SHAPE_2_INPUT,  OP_4_LEVEL_2_INPUT}
    };

    ParamIds opCVAtten[DexterCore::kNumOperators][kNumCVInputs] = {
        {OP_1_MOD_1_PARAM, OP_1_MOD_2_PARAM, OP_1_MOD_3_PARAM, OP_1_MOD_4_PARAM,
         OP_1_PITCH_CV1_PARAM, OP_1_WAVE_CV1_PARAM, OP_1_SHAPE_CV1_PARAM, OP_1_LEVEL_CV1_PARAM,
         OP_1_PITCH_CV2_PARAM, OP_1_WAVE_CV2_PARAM, OP_1_SHAPE_CV2_PARAM, OP_1_LEVEL_CV2_PARAM,},
        {OP_2_MOD_1_PARAM, OP_2_MOD_2_PARAM, OP_2_MOD_3_PARAM, OP_2_MOD_4_PARAM,
         OP_2_PITCH_CV1_PARAM, OP_2_WAVE_CV1_PARAM, OP_2_SHAPE_CV1_PARAM, OP_2_LEVEL_CV1_PARAM,
         OP_2_PITCH_CV2_PARAM, OP_2_WAVE_CV2_PARAM, OP_2_SHAPE_CV2_PARAM, OP_2_LEVEL_CV2_PARAM,},
        {OP_3_MOD_1_PARAM, OP_3_MOD_2_PARAM, OP_3_MOD_3_PARAM, OP_3_MOD_4_PARAM,
         OP_3_PITCH_CV1_PARAM, OP_3_WAVE_CV1_PARAM, OP_3_SHAPE_CV1_PARAM, OP_3_LEVEL_CV1_PARAM,
         OP_3_PITCH_CV2_PARAM, OP_3_WAVE_CV2_PARAM, OP_3_SHAPE_CV2_PARAM, OP_3_LEVEL_CV2_PARAM,},
        {OP_4_MOD_1_PARAM, OP_4_MOD_2_PARAM, OP_4_MOD_3_PARAM, OP_4_MOD_4_PARAM,
         OP_4_PITCH_CV1_PARAM, OP_4_WAVE_CV1_PARAM, OP_4_SHAPE_CV1_PARAM, OP_4_LEVEL_CV1_PARAM,
         OP_4_PITCH_CV2_PARAM, OP_4_WAVE_CV2_PARAM, OP_4_SHAPE_CV2_PARAM, OP_4_LEVEL_CV2_PARAM,}
    };

    DexterCore::OpSyncSource opSyncSource = DexterCore::PARENT_SYNC_SOURCE;

    DexterCore coreA, coreB;
    DexterRoutingMatrix modMatrix[DexterCore::kNumOperators];

    float results[4] = {0.f, 0.f, 0.f, 0.f};
    __m128 leftOutVec, rightOutVec;
    __m128 zerosVec;
    float* pLeftOut;
    float* pRightOut;
    float* pOP1;
    float* pOP2;
    float* pOP3;
    float* pOP4;
    float* pBOut;

    bool resetPhaseState = false;
    float masterLFO = 0.f;
    dsp::SchmittTrigger resetPhaseButtonTrig;
    dsp::SchmittTrigger masterLFOButtonTrig;
    int octave;
    float aPitch = 0.f;
    float bPitch = 0.f;

    int multiple = 0;
    float multiples[27] = {0.125f, 0.25f, 0.5f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f,
                           9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f,
                           17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f};

    __m128 bOutMaskVec;
    __m128 v0000, v0001, v0011, v0111, v1111, v0010, v0110, v1110;
    __m128 lowLeftGainVec, lowRightGainVec;
    __m128 highLeftGainVec, highRightGainVec;
    __m128 lowLeftGainTableVec[7];
    __m128 lowRightGainTableVec[7];
    __m128 highLeftGainTableVec[7];
    __m128 highRightGainTableVec[7];

    std::vector<float> chordNotes;
    int currentChord;
    float chordParam;
    float chordDetune;
    float chordDetuneParam;

    int buttonFullInversion = 0;
    int fullInversion = 0;
    dsp::SchmittTrigger fullInversionButtonTrig;
    int chordKnob;
    int invDepth;
    int invDepthParam;
    int invertKnob;

    __m128 lowChordMaskTableVec[7];
    __m128 highChordMaskTableVec[7];
    __m128 lowChordMaskVec, highChordMaskVec;
    float _outputLevels[7] = {1.f, 0.5f, 0.333333f, 0.25f, 0.2f, 0.166666f, 0.143f};
    std::string syncRoutingText[2] = {"Parent", "Neighbour"};
    unsigned long syncRouting = 0;
    unsigned long indivBOutputs = 0;

    int octaveAKnob = 0;
    int octaveBKnob = 0;
    float opPitch[DexterCore::kNumOperators];
    float opLowFreq[DexterCore::kNumOperators][4];
    float opHighFreq[DexterCore::kNumOperators][4];

    float pOpPitches[8] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
    float pOpFreqs[8] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
    float pOpMutiples[8] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};

    __m128 opLowPitchVec[DexterCore::kNumOperators];
    __m128 opHighPitchVec[DexterCore::kNumOperators];
    __m128 opLowFreqVec[DexterCore::kNumOperators];
    __m128 opHighFreqVec[DexterCore::kNumOperators];

    float opLevel[DexterCore::kNumOperators];
    float opWave[DexterCore::kNumOperators];
    int opWaveBank[DexterCore::kNumOperators];
    float opShape[DexterCore::kNumOperators];
    float opButtonSync[DexterCore::kNumOperators];
    float opButtonWeakSync[DexterCore::kNumOperators];
    float opButtonPostShape[DexterCore::kNumOperators];
    float opButtonLFO[DexterCore::kNumOperators];
    float opButtonPreFade[DexterCore::kNumOperators];
    bool opSync[DexterCore::kNumOperators];
    bool opWeakSync[DexterCore::kNumOperators];
    bool opPostShape[DexterCore::kNumOperators];
    bool opPreFade[DexterCore::kNumOperators];

    // Knob variables
    float brightnessKnob = 0.f;
    float brightnessDepthKnob = 0.f;
    float feedbackKnob = 0.f;
    float feedbackDepthKnob = 0.f;
    float allShapeKnob = 0.f;
    float allShapeDepthKnob = 0.f;
    float coreACoarseKnob = 0.f;
    float coreAFineKnob = 0.f;
    float coreBCoarseKnob = 0.f;
    float coreBFineKnob = 0.f;

    float chordDepthKnob = 0.f;

    float detuneKnob = 0.f;
    float detuneDepthKnob = 0.f;
    
    float invertDepthKnob = 0.f;

    float brightness;
    float feedback;
    float allShape;

    float algoKnob = 0.f;
    float algoDepthKnob = 0.f;

    float opCoarseKnob[DexterCore::kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    float opFineKnob[DexterCore::kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    float opPitchCV1Knob[DexterCore::kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    float opPitchCV2Knob[DexterCore::kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    int opMultipleKnob[DexterCore::kNumOperators] = {0, 0, 0, 0};
    int opWaveBankKnob[DexterCore::kNumOperators] = {0, 0, 0, 0};
    float opWaveKnob[DexterCore::kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    float opShapeKnob[DexterCore::kNumOperators] = {0.f, 0.f, 0.f, 0.f};
    float opLevelKnob[DexterCore::kNumOperators] = {0.f, 0.f, 0.f, 0.f};

    float opCVAttenKnob[DexterCore::kNumOperators][kNumCVInputs];

    // Core control variables
    int syncMode = 0;
    int shapeMode = 0;
    int algo = 0;

    unsigned long opSyncMode[DexterCore::kNumOperators] = {0, 0, 0, 0};
    unsigned long opShapeMode[DexterCore::kNumOperators] = {0, 0, 0, 0};
    unsigned long opMenuSyncMode[DexterCore::kNumOperators] = {0, 0, 0, 0};
    unsigned long opMenuShapeMode[DexterCore::kNumOperators] = {0, 0, 0, 0};

    unsigned long opMod1Assign[DexterCore::kNumOperators] = {0, 0, 0, 0};
    unsigned long opMod2Assign[DexterCore::kNumOperators] = {0, 0, 0, 0};
    unsigned long opMod3Assign[DexterCore::kNumOperators] = {0, 0, 0, 0};
    unsigned long opMod4Assign[DexterCore::kNumOperators] = {0, 0, 0, 0};

    int opSettingsMenu[DexterCore::kNumOperators] = {0, 0, 0, 0};
    int opWaveMenuVis[DexterCore::kNumOperators] = {0, 0, 0, 0};
    int opModAMenuVis[DexterCore::kNumOperators] = {0, 0, 0, 0};
    int opModBMenuVis[DexterCore::kNumOperators] = {0, 0, 0, 0};
    int opMiscMenuVis[DexterCore::kNumOperators] = {0, 0, 0, 0};
    unsigned long opMenuPage[DexterCore::kNumOperators] = {0, 0, 0, 0};

    dsp::SchmittTrigger opSettBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opSyncBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opWeakBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opLFOBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opPreFadeBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opPostShapeBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opWaveMenuBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opModAMenuBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opModBMenuBtnTrig[DexterCore::kNumOperators];
    dsp::SchmittTrigger opMiscMenuBtnTrig[DexterCore::kNumOperators];

    float sampleRate = 44100.f;
    int panelStyle = 0;

    static constexpr float paramUpdateRateHz = 30.f;
    int paramCounterMax = 128;
    int paramCounter = 0;

    Dexter();
    ~Dexter();
    void step() override;

    void getParameters();
    void setLowPriorityCoreSettings();
    void setTimeCriticalCoreSettings();

    void makeChord(float chord, float invert);
    void onSampleRateChange() override;
    void onReset() override;
    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct AlgoGraphic : FramebufferWidget {
    std::vector<std::shared_ptr<Svg>> frames;
    SvgWidget *sw;
    int value;
    int style;
    int styleOffset;

    AlgoGraphic();
    void addFrame(std::shared_ptr<Svg> svg);
    void step() override;
};

struct OpSyncModeItem : ui::MenuItem {
    unsigned long* pSyncMode;
    unsigned long syncMode;
    void onAction(const event::Action& e) override;
};

struct OpSyncModeChoice : ValleyChoiceMenu {
    unsigned long* pSyncMode;
    std::vector<std::string> syncModeLabels = {"Hard", "5th", "+1 Oct", "-1 Oct", "Rise 1", "Rise 2",
                                               "Fall 1", "Fall 2", "Pull 1", "Pull 2", "Push 1", "Push 2",
                                               "Hold", "One Shot", "Lock Shot"};

    void onAction(const event::Action& e) override;
    void step() override;
};

struct OpShapeModeItem : ui::MenuItem {
    unsigned long* pShapeMode;
    unsigned long shapeMode;
    void onAction(const event::Action& e) override;
};

struct OpShapeModeChoice : ValleyChoiceMenu {
    unsigned long* pShapeMode;
    std::vector<std::string> shapeModeLabels = {"Bend", "Tilt", "Lean", "Twist", "Wrap", "Mirror",
                                                "Reflect", "Pulse", "Step 4", "Step 8", "Step 16",
                                                "Var Step"};

    void onAction(const event::Action& e) override;
    void step() override;
};

struct OpModModeItem : ui::MenuItem {
    unsigned long* pModMode;
    unsigned long modMode;
    void onAction(const event::Action& e) override;
};

struct OpModModeChoice : ValleyChoiceMenu {
    unsigned long* pModMode;
    std::vector<std::string> modModeLabels = {"Pitch", "Multiple", "Wave Pos.", "Wave Bank", "Shape",
                                              "Level", "Ext FM", "Ext Sync", "Shape Mode", "Post Shape",
                                              "Sync Mode", "Sync En.", "Weak Sync"};

    void onAction(const event::Action& e) override;
    void step() override;
};

struct DexterWidget : ModuleWidget {
    static constexpr unsigned long kNumMultiples = 27;

    std::string waveTableNames[NUM_DEXTER_WAVETABLES] = {
        "Opal", "Basic", "TeeEks", "SinHarm", "AddSin", "AMHarm", "SwpHarm",
        "AddSaw", "AddSqr", "AddBank", "Oboe", "Sax", "Cello1", "Cello2",
        "Violin", "Piano", "Thrmin", "Pluck", "OvrTne1", "OvrTne2", "Sym",
        "Chip1", "Chip2", "BitCrsh1", "BitCrsh2", "Voice1", "Voice2", "Voice3",
        "Voice4", "Voice5", "PWM", "BiPls", "SawGap1", "SawGap2", "VGame"
    };

    // Control positions
    const float ledOffset = 2.5;
    Vec ChordKnobPos = Vec(11.41, 84.625);
    Vec InvertKnobPos = Vec(54.71, 84.625);
    Vec DetuneKnobPos = Vec(97.41, 84.625);
    Vec OctaveAKnobPos = Vec(8.41, 35.12);
    Vec CoarseKnobPos = Vec(51.71, 35.12);
    Vec FineKnobPos = Vec(94.41, 35.12);
    Vec AlgoKnobPos = Vec(143.71, 35.12);
    Vec BrightKnobPos = Vec(51.71, 205.625);
    Vec ShapeKnobPos = Vec(8.41, 205.625);
    Vec FeedbackKnobPos = Vec(94.41, 205.625);

    Vec Octave2KnobPos = Vec(8.41, 153.12);
    Vec Coarse2KnobPos = Vec(51.71, 153.12);
    Vec Fine2KnobPos = Vec(94.41, 153.12);

    Vec ChordDepthKnobPos = Vec(5.5, 258.509);
    Vec InvertDepthKnobPos = Vec(31.7, 258.509);
    Vec DetuneDepthKnobPos = Vec(58, 258.509);
    Vec AlgoDepthKnobPos = Vec(85.2, 258.509);
    Vec FBDepthKnobPos = Vec(110.55, 258.509);
    Vec BrightDepthKnobPos = Vec(136.85, 258.509);
    Vec ShapeDepthKnobPos = Vec(163.15, 258.509);

    Vec MasterLFOButtonPos = Vec(138.125, 166);
    Vec ResetPhaseButtonPos = Vec(138.125, 184.462);
    Vec FullInversionButtonPos = Vec(53.71, 127);
    Vec MasterLFOButtonLEDPos = Vec(140.45, 168.35);
    Vec ResetPhaseButtonLEDPos = Vec(140.45, 186.742);
    Vec FullInversionButtonLEDPos = Vec(56.035, 129.28);

    Vec ChordCVJack = Vec(5.0, 280.735);
    Vec InvertCVJack = Vec(31.25, 280.735);
    Vec DetuneCVJack = Vec(57.5, 280.735);
    Vec AlgorithmCVJack = Vec(83.75, 280.735);
    Vec FeedbackCVJack = Vec(110.0, 280.735);
    Vec BrightCVJack = Vec(136.25, 280.735);
    Vec ShapeCVJack = Vec(162.5, 280.735);

    Vec AOutLeftJack = Vec(5.0, 319.951);
    Vec AOutRightJack = Vec(31.25, 319.951);
    Vec Op1OutJack = Vec(57.5, 319.951);
    Vec Op2OutJack = Vec(83.75, 319.951);
    Vec Op3OutJack = Vec(110.0, 319.951);
    Vec Op4OutJack = Vec(136.25, 319.951);
    Vec BOutJack = Vec(162.5, 319.951);

    Vec VOct1CVJack = Vec(136.25, 210.519);
    Vec VOct2CVJack = Vec(162.5, 210.519);

    float OpMultKnobRootX = 192.922;          const float OpRow1Y = 44.12;
    const float OpTableKnobRootX = 194.922;
    const float OpCoarseKnobRootX = 234.172;
    const float OpFineKnobRootX = 275.422;
    const float OpWaveKnobRootX = 192.922;          const float OpRow2Y = 96.625;
    const float OpShapeKnobRootX = 234.172;
    const float OpLevelKnobRootX = 275.422;
    const float OpMod1KnobRootX = 196.338;          const float OpModRowY = 185.401;
    const float OpMod2KnobRootX = 226.288;
    const float OpMod3KnobRootX = 256.537;
    const float OpMod4KnobRootX = 286.35;
    const float OpPitch1KnobRootX = 196.338;        const float OpCV1RowY = 247.649;
    const float OpPitch2KnobRootX = 196.338;        const float OpCV2RowY = 296.699;
    const float OpWave1KnobRootX = 226.288;
    const float OpWave2KnobRootX = 226.288;
    const float OpShape1KnobRootX = 256.537;
    const float OpShape2KnobRootX = 256.537;
    const float OpLevel1KnobRootX = 286.35;
    const float OpLevel2KnobRootX = 286.35;

    const float OpSettingsButtonRootX = 191.25;     const float OpSettingsButtonRootY = 27.7;
    const float OpWaveButtonX = 195.992;
    const float OpModAButtonX = 227.398;
    const float OpModBButtonX = 258.804;
    const float OpMiscButtonX = 290.21;
    const float OpPreButtonPosX = 198.65;          const float OpPreButtonPosY = 146.998;
    const float OpPercButtonRootX = 220.912;        const float OpPercButtonRootY = 146.998;
    const float OpIsolateButtonRootX = 220.912;     const float OpIsolateButtonRootY = 165.748;
    const float OpLFOButtonRootX = 267.561;         const float OpLFOButtonRootY = 146.998;
    const float OpSyncButtonRootX = 267.561;        const float OpSyncButtonRootY = 165.748;

    const float OpSyncJackRootX = 195.238;          const float OpSyncJackRootY = 147.75;
    const float OpCV1JackRootX = 195.238;           const float OpModJackRowY = 209.001;
    const float OpCV2JackRootX = 225.188;
    const float OpCV3JackRootX = 255.25;
    const float OpCV4JackRootX = 285.25;            const float OpCV1JackRootY = 271.25;
                                                    const float OpCV2JackRootY = 320.0;

    std::string OpMainText[6] = {"Mul:", "Coarse", "Fine", "Wave", "Shape", "Level"};
    std::string multiplesText[kNumMultiples] = {"1/8", "1/4", "1/2", "1", "2", "3", "4", "5", "6",
                                                "7", "8", "9", "10", "11", "12", "13", "14", "15", "16",
                                                "17", "18", "19", "20", "21", "22", "23", "24"};
    std::vector<std::string> octaveTextItems = {"-3", "-2", "-1", "0", "+1", "+2", "+3"};

    float OpMainTextX[6] = {202.062, 250.312, 291.562, 209.062, 250.312, 291.562};
    float OpMainTextY[6] = {81.468, 81.468, 81.468, 133.093, 133.093, 133.093};
    Vec syncChoiceRootPos = Vec(0.0, 0.0);

    PlainText* chordText;
    PlainText* octaveAText;
    PlainText* octaveBText;
    PlainText* mainText[DexterCore::kNumOperators][6];
    PlainText* multText[DexterCore::kNumOperators];

    PlainText* waveTableTabTopLabel[DexterCore::kNumOperators];
    PlainText* waveTableTabBottomLabel[DexterCore::kNumOperators];
    PlainText* mod1And2TabTopLabel[DexterCore::kNumOperators];
    PlainText* mod1And2TabBottomLabel[DexterCore::kNumOperators];
    PlainText* mod3And4TabTopLabel[DexterCore::kNumOperators];
    PlainText* mod3And4TabBottomLabel[DexterCore::kNumOperators];

    RoganMedBlue* OpMultKnob[DexterCore::kNumOperators];
    RoganMedBlue* OpCoarseKnob[DexterCore::kNumOperators];
    RoganMedBlue* OpFineKnob[DexterCore::kNumOperators];
    RoganMedPurple* OpWaveKnob[DexterCore::kNumOperators];
    RoganMedRed* OpShapeKnob[DexterCore::kNumOperators];
    RoganMedGreen* OpLevelKnob[DexterCore::kNumOperators];

    RoganMedBlue* waveBankKnob[DexterCore::kNumOperators];

    OpSyncModeChoice* syncModeChoice[DexterCore::kNumOperators];
    OpShapeModeChoice* shapeModeChoice[DexterCore::kNumOperators];
    OpModModeChoice* mod1ModeChoice[DexterCore::kNumOperators];
    OpModModeChoice* mod2ModeChoice[DexterCore::kNumOperators];
    OpModModeChoice* mod3ModeChoice[DexterCore::kNumOperators];
    OpModModeChoice* mod4ModeChoice[DexterCore::kNumOperators];

    PlainText* tableLabel[DexterCore::kNumOperators];
    PlainText* syncModeLabel[DexterCore::kNumOperators];
    PlainText* shapeModeLabel[DexterCore::kNumOperators];

    PlainText* tableText[DexterCore::kNumOperators];

    PlainText* mod1Label[DexterCore::kNumOperators];
    PlainText* mod2Label[DexterCore::kNumOperators];
    PlainText* mod3Label[DexterCore::kNumOperators];
    PlainText* mod4Label[DexterCore::kNumOperators];

    LightLEDButtonNonDyn* opWaveButton[DexterCore::kNumOperators];
    LightLEDButtonNonDyn* opModAButton[DexterCore::kNumOperators];
    LightLEDButtonNonDyn* opModBButton[DexterCore::kNumOperators];

    MediumLight<RedLight>* opWaveButtonLight[DexterCore::kNumOperators];
    MediumLight<RedLight>* opModAButtonLight[DexterCore::kNumOperators];
    MediumLight<RedLight>* opModBButtonLight[DexterCore::kNumOperators];

    bool panelChanged = true;
    NVGcolor darkPanelTextColour = nvgRGB(0xFF, 0xFF, 0xFF);
    NVGcolor lightPanelTextColour = nvgRGB(0x00, 0x00, 0x00);

    SvgPanel* darkPanel;
    SvgPanel* lightPanel;
    AlgoGraphic* algo;

    DexterWidget(Dexter *module);
    void appendContextMenu(Menu *menu) override;
    void step() override;
};

#endif
