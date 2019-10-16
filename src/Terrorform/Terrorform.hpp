//
// Terrorform.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 8/2/2019
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
//

#ifndef DSJ_CELL_HPP
#define DSJ_CELL_HPP
#define DSJ_CELL_NUM_USER_BANKS 64
#define DSJ_CELL_MAX_USER_TABLE_WAVES 64
#define DSJ_CELL_DEFAULT_USER_TABLE_WAVES 8
#define DSJ_CELL_MAX_USER_WAVE_LENGTH 256
#define DSJ_CELL_MAX_TABLE_SIZE DSJ_CELL_MAX_USER_TABLE_WAVES * DSJ_CELL_MAX_USER_WAVE_LENGTH

#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "../Common/SIMD/QuadOsc.hpp"
#include "../Common/SIMD/VecLPG.hpp"
#include "../Common/FreqLUT.hpp"
#include "../../dep/dr_wav.h"
#include "TerrorformWavetableROM.hpp"
#include "TerrorformWaveTableEditor.hpp"
#include "Degrader.hpp"
#include "osdialog.h"
#include <cstdio>
#include <fstream>
#include <iostream>

using namespace std;

struct Terrorform : Module {

    enum InputIds {
        VOCT_1_INPUT,
        VOCT_2_INPUT,
        BANK_INPUT_1,
        BANK_INPUT_2,
        WAVE_INPUT_1,
        WAVE_INPUT_2,
        SHAPE_TYPE_INPUT_1,
        SHAPE_TYPE_INPUT_2,
        SHAPE_DEPTH_INPUT_1,
        SHAPE_DEPTH_INPUT_2,
        DEGRADE_TYPE_INPUT_1,
        DEGRADE_TYPE_INPUT_2,
        DEGRADE_DEPTH_INPUT_1,
        DEGRADE_DEPTH_INPUT_2,
        FM_A_VCA_INPUT,
        FM_A1_INPUT,
        FM_A2_INPUT,
        FM_B_VCA_INPUT,
        FM_B1_INPUT,
        FM_B2_INPUT,
        SYNC_1_INPUT,
        SYNC_2_INPUT,
        DECAY_1_INPUT,
        DECAY_2_INPUT,
        VELOCITY_1_INPUT,
        VELOCITY_2_INPUT,
        TRIGGER_1_INPUT,
        TRIGGER_2_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        PHASOR_OUTPUT,
        END_OF_CYCLE_OUTPUT,
        PRE_DEGRADE_OUTPUT,
        SUB_OUTPUT,
        MAIN_OUTPUT,
        ENVELOPE_OUTPUT,
        NUM_OUTPUTS
    };

    enum ParamIds {
        OCTAVE_PARAM,
        COARSE_PARAM,
        FINE_PARAM,
        BANK_PARAM,
        WAVE_PARAM,
        SHAPE_TYPE_PARAM,
        SHAPE_DEPTH_PARAM,
        DEGRADE_TYPE_PARAM,
        DEGRADE_DEPTH_PARAM,
        PERC_DECAY_PARAM,
        PERC_VELOCITY_PARAM,
        PERC_SWITCH_PARAM,

        VOCT_1_CV_PARAM,
        VOCT_2_CV_PARAM,
        BANK_CV_1_PARAM,
        BANK_CV_2_PARAM,
        WAVE_CV_1_PARAM,
        WAVE_CV_2_PARAM,
        SHAPE_TYPE_CV_1_PARAM,
        SHAPE_TYPE_CV_2_PARAM,
        SHAPE_DEPTH_CV_1_PARAM,
        SHAPE_DEPTH_CV_2_PARAM,
        DEGRADE_TYPE_CV_1_PARAM,
        DEGRADE_TYPE_CV_2_PARAM,
        DEGRADE_DEPTH_CV_1_PARAM,
        DEGRADE_DEPTH_CV_2_PARAM,
        PERC_DECAY_CV_1_PARAM,
        PERC_DECAY_CV_2_PARAM,
        PERC_VELOCITY_CV_1_PARAM,
        PERC_VELOCITY_CV_2_PARAM,
        FM_A_VCA_ATTEN_PARAM,
        FM_A1_ATTEN_PARAM,
        FM_A2_ATTEN_PARAM,
        FM_B_VCA_ATTEN_PARAM,
        FM_B1_ATTEN_PARAM,
        FM_B2_ATTEN_PARAM,

        USER_BANK_SWITCH_PARAM,
        LOAD_TABLE_SWITCH_PARAM,

        TRIGGER_1_SWITCH_PARAM,
        TRIGGER_2_SWITCH_PARAM,
        WEAK_SYNC_1_SWITCH_PARAM,
        WEAK_SYNC_2_SWITCH_PARAM,
        NUM_PARAMS
    };

    enum LightIds {
        PERCUSSION_LIGHT,
        USER_BANK_LIGHT,
        LOAD_TABLE_LIGHT,
        TRIGGER_1_LIGHT,
        TRIGGER_2_LIGHT,
        WEAK_SYNC_1_LIGHT,
        WEAK_SYNC_2_LIGHT,
        NUM_LIGHTS
    };

    int panelStyle = 0;
    int displayStyle = 0;

    static const int kMaxNumGroups = 1;
    ScanningQuadOsc osc[kMaxNumGroups];

    float** userWaveTableData[DSJ_CELL_NUM_USER_BANKS];
    int32_t userWaveTableSizes[DSJ_CELL_NUM_USER_BANKS];
    long userWaveTableLengths[DSJ_CELL_NUM_USER_BANKS];
    bool userWaveTableFilled[DSJ_CELL_NUM_USER_BANKS];

    int numUserWaveTables = 0;

    bool userWavesButtonState, prevUserWavesButtonState;
    bool readFromUserWaves;

    __m128 __mainOutput[kMaxNumGroups];
    __m128 __preDegradeOutput[kMaxNumGroups];
    __m128 __phasorOutput[kMaxNumGroups];
    __m128 __eocOutput[kMaxNumGroups];

    VecDegrader degrader[4];
    FreqLUT freqLUT;

    int rootBank;
    int rootShapeType;
    int rootDegradeType;
    int bank;
    int shapeType;
    int degradeType;

    float rootPitch;
    float* freqs;
    __m128 __freq;
    float pitchCV1;
    float pitchCV2;

    float rootWave;
    float* waves;
    float numWavesInTable;
    __m128 __wave;
    __m128 __numWavesInTable;
    float waveCV;

    float rootShapeDepth;
    float* shapes;
    __m128 __shape;
    float shapeDepthCV;

    float rootDegradeDepth;
    float* degrades;
    __m128 __degrade;
    float degradeDepthCV;

    bool percMode;
    int modBusTarget = 0;

    bool sync1IsMono, sync2IsMono;
    float* sync1;
    float* sync2;
    __m128 __sync1, __sync2, __prevSync1, __prevSync2;
    __m128 __sync1Pls, __sync2Pls;
    unsigned long syncChoice = 0;

    // Percussion
    bool trig1State, trig2State;
    float trig1, trig2;
    __m128 __trig1, __trig2;
    dsp::PulseGenerator trig1LightPulse, trig2LightPulse;
    float trigLightDurationTime = 0.125f;
    float trigLightDurationSamples = trigLightDurationTime * 44100.f;
    VecLPG lpg[kMaxNumGroups];
    __m128 __decay;

    bool fmA1IsMono, fmA2IsMono;
    bool fmB1IsMono, fmB2IsMono;
    bool fmAVCAIsMono, fmBVCAIsMono;
    bool fmAVCAIsConnected, fmBVCAIsConnected;
    float fmA1Level, fmA2Level, fmB1Level, fmB2Level, fmAVCACV, fmBVCACV;
    float* fmA1;
    float* fmA2;
    float* fmB1;
    float* fmB2;
    float* fmAVCA;
    float* fmBVCA;
    __m128 __fmA, __fmB, __fmSum;
    __m128 __fmAVCA, __fmBVCA;
    __m128 __fmA1Level, __fmA2Level, __fmB1Level, __fmB2Level;
    __m128 __fmAVCACV, __fmBVCACV;

    __m128 __zeros, __ones, __negOnes, __twos, __negTwos, __fives, __negFives, __tens;

    int counter = 512;

    Terrorform();
    ~Terrorform();

    void process(const ProcessArgs &args) override;
    void onSampleRateChange() override;
    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
};

struct TerrorformPanelStyleItem : MenuItem {
    Terrorform* module;
    int panelStyle;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct TerrorformDisplayStyleItem : MenuItem {
    Terrorform* module;
    int displayStyle;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct TerrorformWidget : ModuleWidget {
    int errorDisplayTime = 30 * 5;
    int elapsedErrorDisplayTime = 0;
    int displayBlankTime = 2;

    enum DisplayModes {
        DISPLAY_PARAMS,
        DISPLAY_LOAD_ERROR,
        NUM_DISPLAY_MODES
    };
    DisplayModes displayMode = DISPLAY_PARAMS;

    enum TerrorformDisplayLayers {
        CELL_DISPLAY_BACK = 0,
        CELL_DISPLAY_BLUR_1,
        CELL_DISPLAY_BLUR_2,
        CELL_DISPLAY_FRONT,
        NUM_DISPLAY_LAYERS
    };

    enum TerrorformDisplayColourModes {
        CELL_RED_LED_COLOUR = 0,
        CELL_RED_LED_COLOUR_HOVER,
        CELL_YELLOW_COLOUR,
        CELL_YELLOW_COLOUR_HOVER,
        CELL_GREEN_COLOUR,
        CELL_GREEN_COLOUR_HOVER,
        CELL_VFD_COLOUR,
        CELL_VFD_COLOUR_HOVER,
        CELL_WHITE_COLOUR,
        CELL_WHITE_COLOUR_HOVER,
        NUM_CELL_COLOURS
    };

    enum TerrorformPanels {
        TRRFORM_DARK_PANEL_NORMAL = 0,
        TRRFORM_LIGHT_PANEL_NORMAL,
        TRRFORM_DARK_PANEL_EDITOR,
        TRRFORM_LIGHT_PANEL_EDITOR,
        NUM_TRRFORM_PANELS
    };

    const unsigned char cellDisplayColours[NUM_CELL_COLOURS][NUM_DISPLAY_LAYERS][4] {
        {{0x3F,0x00,0x00,0xFF}, {0xFF,0x5F,0x5F,0xBF}, {0xFF,0x2F,0x2F,0xBF}, {0xFF,0x00,0x00,0xFF}},
        {{0x6F,0x00,0x00,0xFF}, {0xFF,0x7F,0xFF,0xEF}, {0xFF,0x5F,0xFF,0xEF}, {0xFF,0x4F,0x6F,0xFF}},

        {{0x44,0x15,0x00,0xFF}, {0xFF,0xD5,0x3F,0xBF}, {0xFF,0xD5,0x1F,0xBF}, {0xFF,0xB0,0x00,0xFF}},
        {{0x64,0x35,0x00,0xFF}, {0xFF,0xE5,0x3F,0xBF}, {0xFF,0xE5,0x1F,0xBF}, {0xFF,0xD0,0x10,0xFF}},

        {{0x00,0x3F,0x00,0xFF}, {0x5F,0xFF,0x5F,0xBF}, {0x2F,0xFF,0x2F,0xBF}, {0x00,0xFF,0x1F,0xFF}},
        {{0x00,0x6F,0x00,0xFF}, {0x7F,0xFF,0x7F,0xBF}, {0x5F,0xFF,0x5F,0xBF}, {0x2F,0xFF,0x5F,0xFF}},

        {{0x00,0x31,0x30,0xFF}, {0x72,0xFF,0xEE,0xBF}, {0x2A,0xFF,0xF9,0xBF}, {0x00,0xFF,0xEF,0xFF}},
        {{0x00,0x61,0x60,0xFF}, {0x92,0xFF,0xFE,0xBF}, {0x5A,0xFF,0xFF,0xBF}, {0x4F,0xFF,0xFF,0xFF}},

        {{0x30,0x30,0x30,0xFF}, {0xAF,0xAF,0xAF,0xBF}, {0xCF,0xCF,0xCF,0xBF}, {0xEE,0xEE,0xEE,0xFF}},
        {{0x50,0x50,0x50,0xFF}, {0xCF,0xCF,0xCF,0xBF}, {0xEF,0xEF,0xEF,0xBF}, {0xFF,0xFF,0xFF,0xFF}}
    };

    // Knobs
    Vec octavePos = Vec(93, 49);
    Vec coarsePos = Vec(150, 49);
    Vec finePos = Vec(207, 49);
    Vec bankPos = Vec(121, 92);
    Vec wavePos = Vec(179, 92);
    Vec shapeTypePos = Vec(70, 138);
    Vec shapeDepthPos = Vec(230, 138);
    Vec degradeTypePos = Vec(79, 182);
    Vec degradeDepthPos = Vec(221, 182);
    Vec percDecayPos = Vec(125, 200);
    Vec percVelocityPos = Vec(175, 200);

    RoganMedBlue* octaveKnob;
    RoganMedBlue* coarseKnob;
    RoganMedBlue* fineKnob;
    RoganMedPurple* bankKnob;
    RoganMedPurple* waveKnob;
    RoganMedRed* shapeTypeKnob;
    RoganMedRed* shapeDepthKnob;
    RoganMedGreen* degradeTypeKnob;
    RoganMedGreen* degradeDepthKnob;
    RoganMedMustard* decayKnob;
    RoganMedMustard* velocityKnob;

    // Attenuators
    Vec vOct1CVPos = Vec(53, 43);
    Vec vOct2CVPos = Vec(247, 43);
    Vec bankCV1Pos = Vec(59, 76);
    Vec bankCV2Pos = Vec(67, 104);
    Vec waveCV1Pos = Vec(241, 76);
    Vec waveCV2Pos = Vec(232, 104);
    Vec shapeTypeCV1Pos = Vec(42, 122);
    Vec shapeTypeCV2Pos = Vec(47, 172);
    Vec shapeDepthCV1Pos = Vec(258, 122);
    Vec shapeDepthCV2Pos = Vec(253, 172);
    Vec degradeTypeCV1Pos = Vec(37, 198);
    Vec degradeTypeCV2Pos = Vec(75, 224);
    Vec degradeDepthCV1Pos = Vec(263, 198);
    Vec degradeDepthCV2Pos = Vec(225, 224);
    Vec percDecayCV1Pos = Vec(99, 215);
    Vec percDecayCV2Pos = Vec(123, 234);
    Vec percVelocityCV1Pos = Vec(178, 234);
    Vec percVelocityCV2Pos = Vec(201, 215);

    RoganSmallBlue* vOct1CV;
    RoganSmallBlue* vOct2CV;
    RoganSmallPurple* bankCV1;
    RoganSmallPurple* bankCV2;
    RoganSmallPurple* waveCV1;
    RoganSmallPurple* waveCV2;
    RoganSmallRed* shapeTypeCV1;
    RoganSmallRed* shapeTypeCV2;
    RoganSmallRed* shapeDepthCV1;
    RoganSmallRed* shapeDepthCV2;
    RoganSmallGreen* degradeTypeCV1;
    RoganSmallGreen* degradeTypeCV2;
    RoganSmallGreen* degradeDepthCV1;
    RoganSmallGreen* degradeDepthCV2;
    RoganSmallMustard* percDecayCV1;
    RoganSmallMustard* percDecayCV2;
    RoganSmallMustard* percVelocityCV1;
    RoganSmallMustard* percVelocityCV2;

    // Switches
    LightLEDButton2* lfoButton;
    LightLEDButton2* loadButton;
    LightLEDButton3* percButton;

    MediumLight<RedLight>* lfoButtonLight;
    LargeLight<RedLight>* percButtonLight;

    Vec percSwitchPos = Vec(150, 219);
    Vec trigSwitch1Pos = Vec(118, 282);
    Vec trigSwitch2Pos = Vec(182, 282);
    Vec userBankSwitchPos = Vec(143.3, 77.3);
    Vec loadTableSwitchPos = Vec(143.3, 102.3);
    Vec weakSyncSwitch1Pos = Vec(87, 299);
    Vec weakSyncSwitch2Pos = Vec(213, 299);

    Vec vcaAPos = Vec(44, 278);
    Vec fmA1Pos = Vec(44, 303);
    Vec fmA2Pos = Vec(44, 328);

    Vec vcaBPos = Vec(256, 278);
    Vec fmB1Pos = Vec(256, 303);
    Vec fmB2Pos = Vec(256, 328);

    // Jacks
    float col1X = 15.0;
    float col2X = 285.0;
    Vec vOct1InputPos = Vec(col1X, 40);
    Vec vOct2InputPos = Vec(col2X, 40);

    Vec bankInput1Pos = Vec(col1X, 75);
    Vec bankInput2Pos = Vec(col1X, 101);

    Vec waveInput1Pos = Vec(col2X, 75);
    Vec waveInput2Pos = Vec(col2X, 101);

    Vec shapeTypeInput1Pos = Vec(col1X, 133);
    Vec shapeTypeInput2Pos = Vec(col1X, 171);

    Vec shapeDepthInput1Pos = Vec(col2X, 133);
    Vec shapeDepthInput2Pos = Vec(col2X, 171);

    Vec degradeTypeInput1Pos = Vec(col1X, 227);
    Vec degradeTypeInput2Pos = Vec(39, 249);

    Vec degradeDepthInput1Pos = Vec(col2X, 227);
    Vec degradeDepthInput2Pos = Vec(261, 249);

    Vec vcaAInputPos = Vec(col1X, 271);
    Vec fmA1InputPos = Vec(col1X, 307);
    Vec fmA2InputPos = Vec(col1X, 339);
    Vec vcaBInputPos = Vec(col2X, 271);
    Vec fmB1InputPos = Vec(col2X, 307);
    Vec fmB2InputPos = Vec(col2X, 339);

    Vec syncInput1Pos = Vec(87, 264);
    Vec syncInput2Pos = Vec(213, 264);

    Vec decayInput1Pos = Vec(93, 240);
    Vec decayInput2Pos = Vec(111, 258);
    Vec velocityInput1Pos = Vec(207, 240);
    Vec velocityInput2Pos = Vec(189, 258);
    Vec triggerInput1Pos = Vec(135, 254);
    Vec triggerInput2Pos = Vec(165, 254);

    // Outputs
    Vec phasorOutPos = Vec(70, 330);
    Vec eocOutPos = Vec(102, 330);
    Vec envOutPos = Vec(134, 330);
    Vec preDegradePos = Vec(166, 330);
    Vec subOutPos = Vec(198, 330);
    Vec mainOutPos = Vec(230, 330);

    // Text
    Vec bankTextPos = Vec(97.46, 130.358);
    Vec shapeTextPos = Vec(97.46, 143.211);
    Vec degradeTextPos = Vec(97.46, 156.065);

    Vec waveTextPos = Vec(202.897, 130.358);
    Vec shapeDepthTextPos = Vec(202.897, 143.211);
    Vec degradeDepthTextPos = Vec(202.897, 156.065);

    Vec syncTextPos = Vec(149.829, 300);

    widget::Widget* panels[NUM_TRRFORM_PANELS];
    bool inEditorMode = false;
    DynamicText* bankBackText;
    DynamicText* shapeBackText;
    DynamicText* degradeBackText;
    DynamicText* syncBackText;

    DynamicText* bankText;
    DynamicText* shapeText;
    DynamicText* degradeText;
    DynamicText* waveText;
    DynamicText* shapeDepthText;
    DynamicText* degradeDepthText;
    DynamicText* syncText;

    DynamicText* bankBlurText;
    DynamicText* bankBlurText2;
    DynamicText* waveBlurText;
    DynamicText* waveBlurText2;
    DynamicText* shapeBlurText;
    DynamicText* shapeBlurText2;
    DynamicText* shapeDepthBlurText;
    DynamicText* shapeDepthBlurText2;
    DynamicText* degradeBlurText;
    DynamicText* degradeBlurText2;
    DynamicText* degradeDepthBlurText;
    DynamicText* degradeDepthBlurText2;
    DynamicText* syncBlurText;
    DynamicText* syncBlurText2;

    std::shared_ptr<std::string> bankStr;
    std::shared_ptr<std::string> shapeTypeStr;
    std::shared_ptr<std::string> degradeTypeStr;

    std::shared_ptr<std::string> waveStr;
    std::shared_ptr<std::string> shapeDepthStr;
    std::shared_ptr<std::string> degradeDepthStr;

    std::shared_ptr<std::string> syncStr;

    int wavePercent;
    int shapeDepthPercent;
    int degradeDepthPercent;

    DynamicMenu* bankMenu;
    DynamicMenu* shapeMenu;
    DynamicMenu* degradeMenu;
    DynamicMenu* syncMenu;

    unsigned long bankChoice = 0;
    unsigned long menuBankChoice = 0;
    bool bankMenuIsOpen = false;

    TFormEditor* editor;
    float *newTable;

    TerrorformDisplayColourModes displayStyle = CELL_RED_LED_COLOUR;
    TerrorformDisplayColourModes prevDisplayStyle = CELL_RED_LED_COLOUR;

    std::vector<std::string> bankNames = {
        "OPAL", "BASIC", "TEE_EKS", "SIN_HARM", "ADD_SIN", "AM_HARM", "SWEEPHARM", "XFADE",
        "ADD_SAW", "ADD_SQR", "ADD_BANK1", "ADD_BANK2", "ADD_BANK3", "ADD_BANK4", "OBOE",
        "SAXOPHONE", "REED", "CELLO_1", "CELLO_2", "VIOLIN", "PIANO", "THEREMIN", "PLUCK",
        "OVERTONE1", "OVERTONE2", "SYMMETRY", "CHIP_1", "CHIP_2", "BITCRUSH1", "BITCRUSH2",
        "GRIT", "VOICE_1", "VOICE_2", "VOICE_3", "VOICE_4", "VOICE_5", "VOICE_6", "PWM",
        "BI_PULSE", "SAW_GAP1", "SAW_GAP2", "VIDEOGAME", "FOLD_SINE", "FM1", "FM2", "FM3", "FM4",
        "FM5", "FM6", "2_OPFM1", "2_OPFM2", "2_OP_RAND", "VOX_MACH", "LINEAR_1", "PLAITS_2",
        "PLAITS_3", "PLAITS_4"
    };

    std::vector<std::string> bankMenuItems = {
        "Opal", "Basic", "Tee Eks", "Sine Harmonics", "Additive Sine", "AM Harmonics", "Sweep Harmonics", "X Fade",
        "Additive Saw", "Additive Square", "Additive Bank 1", "Additive Bank 2", "Additive Bank 3", "Additive Bank 4",
        "Oboe", "Saxophone", "Reed", "Cello 1", "Cello 2",
        "Violin", "Piano", "Theremin", "Pluck", "Overtone 1", "Overtone 2", "Symmetry",
        "Chip 1", "Chip 2", "Bit Crush 1", "Bit Crush 2", "Grit", "Voice 1", "Voice 2",
        "Voice 3", "Voice 4", "Voice 5", "Voice 6", "PWM", "Bi Pulse", "Saw Gap 1",
        "Saw Gap 2", "Video Game", "Folding Sine", "FM1", "FM2", "FM3", "FM4", "FM5", "FM6",
        "Two OP FM1", "Two OP FM2", "Two OP Random", "Vox Machine", "Linear 1", "Plaits 2",
        "Plaits 3", "Plaits 4"
    };

    std::vector<std::string> shapeNames = {
        "BEND", "TILT", "LEAN", "TWIST", "WRAP", "MIRROR", "REFLECT",
        "PULSE", "STEP_4", "STEP_8", "STEP_16", "VAR_STEP"
    };

    std::vector<std::string> shapeMenuItems = {
        "Bend", "Tilt", "Lean", "Twist", "Wrap", "Mirror", "Reflect",
        "Pulse", "Step 4", "Step 8", "Step 16", "Variable Step"
    };

    std::vector<std::string> degradeNames = {
        "BITCRUSH", "QUANTIZE", "AND_INT", "XOR_INT", "AND_FLOAT", "OVERDRIVE",
        "RING_MOD", "GRAIN", "SINE", "FOLD", "LOCK_SUB", /*"WRAP",*/ "MIRROR"
    };

    std::vector<std::string> degradeMenuItems = {
        "Bit Crush", "Quantize", "Bitwise AND Int", "Bitwise XOR Int", "Bitwise AND Float", "Overdrive",
        "Ring Mod", "Grain", "Sine", "Fold", "Locking Sub", /*"Wrap",*/ "Mirror"
    };

    std::vector<std::string> syncNames = {
        "HARD", "+FIFTH", "OCTAVE+1", "OCTAVE-1", "RISE_1", "RISE_2",
        "FALL_1", "FALL_2", "PULL_1", "PULL_2", "PUSH_1", "PUSH_2",
        "HOLD", "ONE_SHOT", "LOCKSHOT", "REVERSE!"
    };

    std::vector<std::string> syncMenuItems = {
        "Hard", "Fifth", "+1 Oct", "-1 Oct", "Rise 1", "Rise 2",
        "Fall 1", "Fall 2", "Pull 1", "Pull 2", "Push 1", "Push 2",
        "Hold", "One Shot", "Lock Shot", "Reverse"
    };

    std::vector<std::string> modBusNames = {
        "PITCH", "WAVE_BANK", "WAVE_POS", "SHAPE_TYPE", "SHAPE_DEPTH", "DEGRADE_TYPE",
        "DEGRADEDEPTH", "FM_IN", "FM_DEPTH", "SYNC_IN", "PERC_TRIG", "PERC_DECAY"
    };

    std::vector<std::string> modBusMenuItems = {
        "Pitch", "Wave Bank", "Wave Position", "Shape Type", "Shape Depth", "Degrade Type",
        "Degrade Depth", "FM In", "FM Depth", "Sync In", "Percussion Trig", "Percussion Decay"
    };

    TerrorformWidget(Terrorform *module);
    void appendContextMenu(Menu *menu) override;
    void step() override;
    void onDisplayParams();
    void onDisplayLoadError();
    void changeDisplayStyle();
    void exportWavetables();
    void importWavetables();
};

#endif
