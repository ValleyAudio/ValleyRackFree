//
// Terrorform.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 8/2/2019
//
// Copyright 2098 Dale Johnson. Redistribution and use in source and binary forms, with or without
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

#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "../Common/SIMD/QuadOsc.hpp"
#include "../Common/SIMD/VecLPG.hpp"
#include "../Common/FreqLUT.hpp"
#include "../../dep/dr_wav.h"
#include "TerrorformWavetableROM.hpp"
#include "TerrorformWaveTableEditor.hpp"
#include "TerrorformWaveBank.hpp"
#include "Enhancer.hpp"
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
        ENHANCE_TYPE_INPUT_1,
        ENHANCE_TYPE_INPUT_2,
        ENHANCE_DEPTH_INPUT_1,
        ENHANCE_DEPTH_INPUT_2,
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
        SHAPED_PHASOR_OUTPUT,
        RAW_OUTPUT,
        ENHANCER_OUTPUT,
        ENVELOPE_OUTPUT,
        MAIN_OUTPUT,
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
        ENHANCE_TYPE_PARAM,
        ENHANCE_DEPTH_PARAM,
        LPG_ATTACK_PARAM,
        LPG_DECAY_PARAM,

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
        ENHANCE_TYPE_CV_1_PARAM,
        ENHANCE_TYPE_CV_2_PARAM,
        ENHANCE_DEPTH_CV_1_PARAM,
        ENHANCE_DEPTH_CV_2_PARAM,
        LPG_ATTACK_CV_1_PARAM,
        LPG_ATTACK_CV_2_PARAM,
        LPG_DECAY_CV_1_PARAM,
        LPG_DECAY_CV_2_PARAM,
        FM_A_VCA_ATTEN_PARAM,
        FM_A1_ATTEN_PARAM,
        FM_A2_ATTEN_PARAM,
        FM_B_VCA_ATTEN_PARAM,
        FM_B1_ATTEN_PARAM,
        FM_B2_ATTEN_PARAM,

        USER_BANK_SWITCH_PARAM,
        LOAD_TABLE_SWITCH_PARAM,
        LPG_SWITCH_PARAM,
        LPG_TRIGGER_PARAM,
        LPG_LONG_TIME_SWITCH_PARAM,
        LPG_VELOCITY_SWITCH_PARAM,
        LPG_TRIGGER_SWITCH_PARAM,
        WEAK_SYNC_1_SWITCH_PARAM,
        WEAK_SYNC_2_SWITCH_PARAM,
        TRUE_FM_SWITCH_PARAM,
        SWAP_SWITCH_PARAM,
        LFO_SWITCH_PARAM,
        ZERO_SWITCH_PARAM,
        NUM_PARAMS
    };

    enum LightIds {
        LPG_RED_LIGHT,
        LPG_GREEN_LIGHT,
        LPG_BLUE_LIGHT,
        LPG_LONG_TIME_LIGHT,
        LPG_VELO_LIGHT,
        LPG_TRIG_LIGHT,
        USER_BANK_LIGHT,
        LOAD_TABLE_LIGHT,
        WEAK_SYNC_1_LIGHT,
        WEAK_SYNC_2_LIGHT,
        TRUE_FM_LIGHT,
        SWAP_LIGHT,
        LFO_LIGHT,
        ZERO_LIGHT,
        NUM_LIGHTS
    };

    int panelStyle = 0;
    int displayStyle = 0;

    static const int kMaxNumGroups = 4;
    int numActiveChannels = 0;
    int numActiveGroups = 0;
    ScanningQuadOsc osc[kMaxNumGroups];

    // User wave table data
    float** userWaveTableData[TFORM_MAX_BANKS];
    char userWaveTableSizes[TFORM_MAX_BANKS];
    bool userWaveTableFilled[TFORM_MAX_BANKS];
    char numUserWaveTables = 0;
    std::vector<std::string> userWaveTableNames;

    bool userWavesButtonState, prevUserWavesButtonState;
    bool readFromUserWaves;

    // Outputs
    __m128 __mainOutput[kMaxNumGroups];
    __m128 __preEnhanceOutput[kMaxNumGroups];
    __m128 __phasorOutput[kMaxNumGroups];
    __m128 __shapedPhasorOutput[kMaxNumGroups];
    __m128 __eocOutput[kMaxNumGroups];

    VecEnhancer enhancer[kMaxNumGroups];
    FreqLUT freqLUT;

    float rootBank;
    float rootShapeType;
    float rootEnhanceType;

    int maxNumBanks;
    float bank;
    int bankI;
    float shapeType;
    int shapeTypeI;
    float enhanceType;
    int enhanceTypeI;

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
    float bankCV1, bankCV2;
    float waveCV1, waveCV2;

    float rootShapeDepth;
    float* shapes;
    __m128 __shape;
    float shapeDepthCV1;
    float shapeDepthCV2;

    float rootEnhanceDepth;
    float* enhances;
    __m128 __enhance;
    float enhanceDepthCV1;
    float enhanceDepthCV2;

    int modBusTarget = 0;

    // Sync
    bool sync1IsMono, sync2IsMono;
    float* sync1;
    float* sync2;
    __m128 __sync1, __sync2, __prevSync1, __prevSync2, __quarterPhase;
    __m128 __sync1Pls, __sync2Pls, __weakSync1Flag, __weakSync2Flag;
    unsigned long syncChoice = 0;
    int weakSync1Enable = 0;
    int weakSync2Enable = 0;
    bool weakSwitch1State = false;
    bool weakSwitch2State = false;
    bool prevWeakSwitch1State = false;
    bool prevWeakSwitch2State = false;

    // Percussion
    dsp::Timer lpgButtonTimer;
    int lpgMode = 0;
    bool lpgButtonPressed = false;
    bool lpgButtonHeldDown = false;
    bool lpgButtonPrevState = false;
    __m128 __lpgVCAMode;
    __m128 __lpgFilterMode;

    bool trig1ButtonState = false;
    bool trig2ButtonState = false;

    float trig1, trig2;
    __m128 __trig1, __trig2;
    VecLPG lpg[kMaxNumGroups];
    __m128 __attack, __decay;
    bool swapEnhancerAndLPG = false;

    // FM
    int fmMode = 0;
    float trueFMSwitchValue = 0.f;
    float prevTrueFMSwitchValue = 0.f;
    bool trueFMEnabled = false;
    bool lfoModeEnabled = false;
    bool zeroFreqEnabled = false;
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
    float* trigger1;
    float* trigger2;
    float* velocity1;
    float* velocity2;
    __m128 __fmA, __fmB, __fmSum;
    __m128 __fmAVCA, __fmBVCA;
    __m128 __fmA1Level, __fmA2Level, __fmB1Level, __fmB2Level;
    __m128 __fmAVCACV, __fmBVCACV;
    __m128 __trigger1 , __trigger2;

    __m128 __zeros, __ones, __negOnes, __twos, __negTwos, __fives, __negFives, __tens;
    __m128 __hundredths, __quarters;

    int counter = 512;
    int samplySampler = 0;

    bool romIsLoading = false;

    __m128 __a;
    float* a;

    Terrorform();
    ~Terrorform();

    void process(const ProcessArgs &args) override;
    void onSampleRateChange() override;
    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
    void clearBank(int bankNum);
    void clearUserWaveTables();
    void cloneBank(int sourceBank, int destBank, int startWave, int endWave);
    void moveBank(int sourceBank, int destBank);
    void defragmentBanks();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

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

struct TerrorformManagerPanel : OpaqueWidget {

};

struct TerrorformManagerItem : MenuItem {
    std::function<void()> openMenu;
    void onAction(const event::Action &e) override;
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
    Vec enhanceTypePos = Vec(79, 182);
    Vec enhanceDepthPos = Vec(221, 182);
    Vec lpgAttackPos = Vec(125, 200);
    Vec lpgDecayPos = Vec(175, 200);

    RoganMedBlue* octaveKnob;
    RoganMedBlue* coarseKnob;
    RoganMedBlue* fineKnob;
    RoganMedPurple* bankKnob;
    RoganMedPurple* waveKnob;
    RoganMedRed* shapeTypeKnob;
    RoganMedRed* shapeDepthKnob;
    RoganMedGreen* enhanceTypeKnob;
    RoganMedGreen* enhanceDepthKnob;
    RoganMedMustard* attackKnob;
    RoganMedMustard* decayKnob;

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
    Vec enhanceTypeCV1Pos = Vec(37, 198);
    Vec enhanceTypeCV2Pos = Vec(75, 224);
    Vec enhanceDepthCV1Pos = Vec(263, 198);
    Vec enhanceDepthCV2Pos = Vec(225, 224);
    Vec lpgAttackCV1Pos = Vec(99, 215);
    Vec lpgAttackCV2Pos = Vec(123, 234);
    Vec lpgDecayCV1Pos = Vec(178, 234);
    Vec lpgDecayCV2Pos = Vec(201, 215);

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
    RoganSmallGreen* enhanceTypeCV1;
    RoganSmallGreen* enhanceTypeCV2;
    RoganSmallGreen* enhanceDepthCV1;
    RoganSmallGreen* enhanceDepthCV2;
    RoganSmallMustard* lpgAttackCV1;
    RoganSmallMustard* lpgAttackCV2;
    RoganSmallMustard* lpgDecayCV1;
    RoganSmallMustard* lpgDecayCV2;

    // Switches
    LightLEDButton2* lfoButton;
    LightLEDButton2* zeroFreqButton;
    LightLEDButton2* userBankButton;

    LightLEDButton2* lpgButton;
    LightLEDButton2* longTimeButton;
    LightLEDButton2* velocityButton;
    LightLEDButton2* trigButton;


    MediumLight<RedLight>* lfoButtonLight;
    MediumLight<RedLight>* zeroFreqLight;
    MediumLight<RedLight>* userBankLight;
    MediumLight<RedGreenBlueLight>* lpgButtonLight;

    Vec lfoButtonPos = Vec(121, 55);
    Vec zeroFreqButtonPos = Vec(179, 55);
    Vec userBankSwitchPos = Vec(150, 98);

    Vec lpgModeSwitchPos = Vec(150, 184);
    Vec lpgLongTimeSwitchPos = Vec(150, 209);
    Vec lpgVeloSwitchPos = Vec(141, 234);
    Vec lpgTrigSwitchPos = Vec(159, 234);

    Vec weakSyncSwitch1Pos = Vec(110, 278);
    Vec weakSyncSwitch2Pos = Vec(190, 278);
    Vec trueFMButtonPos = Vec(81.64, 301);
    Vec swapButtonPos = Vec(218.34, 301);

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

    Vec enhanceTypeInput1Pos = Vec(col1X, 227);
    Vec enhanceTypeInput2Pos = Vec(39, 249);

    Vec enhanceDepthInput1Pos = Vec(col2X, 227);
    Vec enhanceDepthInput2Pos = Vec(261, 249);

    Vec vcaAInputPos = Vec(col1X, 271);
    Vec fmA1InputPos = Vec(col1X, 307);
    Vec fmA2InputPos = Vec(col1X, 339);
    Vec vcaBInputPos = Vec(col2X, 271);
    Vec fmB1InputPos = Vec(col2X, 307);
    Vec fmB2InputPos = Vec(col2X, 339);

    Vec syncInput1Pos = Vec(88, 264);
    Vec syncInput2Pos = Vec(212, 264);

    Vec attackInput1Pos = Vec(93, 240);
    Vec attackInput2Pos = Vec(112, 257);
    Vec decayInput1Pos = Vec(207, 240);
    Vec decayInput2Pos = Vec(188, 257);
    Vec triggerInput1Pos = Vec(136, 264);
    Vec triggerInput2Pos = Vec(164, 264);

    // Outputs
    Vec phasorOutPos =  Vec(67.9541, 331);
    Vec eocOutPos =     Vec(95.3541, 331);
    Vec shapedOutPos =  Vec(122.9541, 331);
    Vec rawOutPos =     Vec(150, 331);
    Vec enhancerOutPos = Vec(177.3541, 331);
    Vec envOutPos =     Vec(204.6541, 331);
    Vec mainOutPos =    Vec(231.9541, 331);

    // Text
    Vec bankTextPos = Vec(97.46, 130.358);
    Vec shapeTextPos = Vec(97.46, 143.211);
    Vec enhanceTextPos = Vec(97.46, 156.065);

    Vec waveTextPos = Vec(202.897, 130.358);
    Vec shapeDepthTextPos = Vec(202.897, 143.211);
    Vec enhanceDepthTextPos = Vec(202.897, 156.065);

    Vec syncTextPos = Vec(149.829, 302);

    widget::Widget* panels[NUM_TRRFORM_PANELS];
    bool inEditorMode = false;
    DynamicText* bankBackText;
    DynamicText* shapeBackText;
    DynamicText* enhanceBackText;
    DynamicText* syncBackText;

    DynamicText* bankText;
    DynamicText* shapeText;
    DynamicText* enhanceText;
    DynamicText* waveText;
    DynamicText* shapeDepthText;
    DynamicText* enhanceDepthText;
    DynamicText* syncText;

    DynamicText* bankBlurText;
    DynamicText* bankBlurText2;
    DynamicText* waveBlurText;
    DynamicText* waveBlurText2;
    DynamicText* shapeBlurText;
    DynamicText* shapeBlurText2;
    DynamicText* shapeDepthBlurText;
    DynamicText* shapeDepthBlurText2;
    DynamicText* enhanceBlurText;
    DynamicText* enhanceBlurText2;
    DynamicText* enhanceDepthBlurText;
    DynamicText* enhanceDepthBlurText2;
    DynamicText* syncBlurText;
    DynamicText* syncBlurText2;

    std::shared_ptr<std::string> bankStr;
    std::shared_ptr<std::string> shapeTypeStr;
    std::shared_ptr<std::string> enhanceTypeStr;

    std::shared_ptr<std::string> waveStr;
    std::shared_ptr<std::string> shapeDepthStr;
    std::shared_ptr<std::string> enhanceDepthStr;

    std::shared_ptr<std::string> syncStr;

    int wavePercent;
    int shapeDepthPercent;
    int enhanceDepthPercent;

    DynamicMenu* bankMenu;
    DynamicMenu* shapeMenu;
    DynamicMenu* enhanceMenu;
    DynamicMenu* syncMenu;
    bool readingUserWaves = false;

    unsigned long bankChoice = 0;
    unsigned long menuBankChoice = 0;
    bool bankMenuIsOpen = false;
    bool updateBankNames = false;

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
        "BI_PULSE", "SAW_GAP1", "SAW_GAP2", "SAW_PHASE", "VIDEOGAME", "FOLD_SINE", "FM1", "FM2", "FM3", "FM4",
        "FM5", "FM6", "2_OPFM1", "2_OPFM2", "2_OP_RAND", "VOX_MACH", "LINEAR_1", "PLAITS_2",
        "PLAITS_3", "PLAITS_4", "GMTRY_1", "GMTRY_2", "GMTRY_3", "RESO_SAW", "RESO_SQR", "CHIRP",
        "DISTRT_1", "EBASS"
    };

    std::vector<std::string> bankMenuItems = {
        "Opal", "Basic", "Tee Eks", "Sine Harmonics", "Additive Sine", "AM Harmonics", "Sweep Harmonics", "X Fade",
        "Additive Saw", "Additive Square", "Additive Bank 1", "Additive Bank 2", "Additive Bank 3", "Additive Bank 4",
        "Oboe", "Saxophone", "Reed", "Cello 1", "Cello 2", "Violin", "Piano", "Theremin", "Pluck",
        "Overtone 1", "Overtone 2", "Symmetry", "Chip 1", "Chip 2", "Bit Crush 1", "Bit Crush 2",
        "Grit", "Voice 1", "Voice 2", "Voice 3", "Voice 4", "Voice 5", "Voice 6", "PWM",
        "Bi Pulse", "Saw Gap 1", "Saw Gap 2", "Saw Phase", "Video Game", "Folding Sine", "FM1", "FM2", "FM3", "FM4",
        "FM5", "FM6", "Two OP FM1", "Two OP FM2", "Two OP Random", "Vox Machine", "Linear 1", "Plaits 2",
        "Plaits 3", "Plaits 4", "Geometry 1", "Geometry 2", "Geometry3", "Resonant Saw", "Resonant Square", "Chirp",
        "Distorted 1", "Electric Bass"
    };

    std::vector<std::string> shapeNames = {
        "BEND", "TILT", "LEAN", "TWIST", "WRAP", "MIRROR", "REFLECT",
        "PULSE", "STEP_4", "STEP_8", "STEP_16", "VAR_STEP"
    };

    std::vector<std::string> shapeMenuItems = {
        "Bend", "Tilt", "Lean", "Twist", "Wrap", "Mirror", "Reflect",
        "Pulse", "Step 4", "Step 8", "Step 16", "Variable Step"
    };

    std::vector<std::string> enhanceNames = {
        "BITCRUSH", "QUANTIZE", "AND_INT", "XOR_INT", "AND_FLOAT", "OVERDRIVE",
        "RING_MOD", "GRAIN", "SINE", "FOLD", "MIRROR", "SUB_SAW", "SUB_PULSE", "SUB_CHEBY", "SUB_GLTCH"
    };

    std::vector<std::string> enhanceMenuItems = {
        "Bit Crush", "Quantize", "Bitwise AND Int", "Bitwise XOR Int", "Bitwise AND Float", "Overdrive",
        "Ring Mod", "Grain", "Sine", "Fold", "Mirror", "Sub Saw", "Sub Pulse", "Sub Chebyshev", "Sub Glitch"
    };

    std::vector<std::string> syncNames = {
        "HARD", "+FIFTH", "OCTAVE+1", "OCTAVE-1", "RISE_1", "RISE_2",
        "FALL_1", "FALL_2", "PULL_1", "PULL_2", "PUSH_1", "PUSH_2",
        "HOLD", "ONE_SHOT", "LOCKSHOT", "RE-VERSE"
    };

    std::vector<std::string> syncMenuItems = {
        "Hard", "Fifth", "+1 Oct", "-1 Oct", "Rise 1", "Rise 2",
        "Fall 1", "Fall 2", "Pull 1", "Pull 2", "Push 1", "Push 2",
        "Hold", "One Shot", "Lock Shot", "Reverse"
    };

    TerrorformWidget(Terrorform *module);
    void appendContextMenu(Menu *menu) override;
    void step() override;
    void changeDisplayStyle();
    void exportWavetables();
    void importWavetables();
};

#endif
