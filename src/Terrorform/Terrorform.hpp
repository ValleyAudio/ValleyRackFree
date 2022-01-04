//
// Terrorform.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 8/2/2019
//
// Copyright 2019 Dale Johnson. Redistribution and use in source and binary forms, with or without
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
#include "../DigitalDisplay.hpp"
#include "../Common/SIMD/QuadOsc.hpp"
#include "../Common/SIMD/VecLPG.hpp"
#include "../Common/FreqLUT.hpp"
#include "../dep/dr_wav.h"
#include "TerrorformWavetableROM.hpp"
#include "TerrorformWaveTableEditor.hpp"
#include "TerrorformWaveBank.hpp"
#include "Enhancer.hpp"
#include "TFormSubOsc.hpp"
#include "VecSineLFO.hpp"
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
        ATTACK_1_INPUT,
        ATTACK_2_INPUT,
        DECAY_1_INPUT,
        DECAY_2_INPUT,
        TRIGGER_1_INPUT,
        TRIGGER_2_INPUT,
        SKEW_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        PHASOR_OUTPUT,
        END_OF_CYCLE_OUTPUT,
        SHAPED_PHASOR_OUTPUT,
        RAW_OUTPUT,
        ENHANCER_OUTPUT,
        ENVELOPE_OUTPUT,
        SUB_OSC_OUTPUT,
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
        SKEW_PARAM,
        SUB_OSC_LEVEL_PARAM,
        SUB_OSC_WAVE_PARAM,

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

        LPG_MODE_SWITCH_PARAM,
        LPG_LONG_TIME_SWITCH_PARAM,
        LPG_VELOCITY_SWITCH_PARAM,
        LPG_TRIGGER_SWITCH_PARAM,

        USER_BANK_SWITCH_PARAM,
        WEAK_SYNC_1_SWITCH_PARAM,
        WEAK_SYNC_2_SWITCH_PARAM,
        TRUE_FM_SWITCH_PARAM,
        SWAP_SWITCH_PARAM,
        LFO_SWITCH_PARAM,
        ZERO_SWITCH_PARAM,
        POST_PM_SHAPE_PARAM,
        DISPLAY_CV_SWITCH_PARAM,
        NUM_PARAMS
    };

    enum LightIds {
        LPG_RED_LIGHT,
        LPG_GREEN_LIGHT,
        LPG_BLUE_LIGHT,

        LPG_LONG_TIME_LIGHT,
        LPG_VELOCITY_LIGHT,
        LPG_TRIGGER_LIGHT,

        USER_BANK_LIGHT,
        LOAD_TABLE_LIGHT,
        WEAK_SYNC_1_LIGHT,
        WEAK_SYNC_2_LIGHT,
        TRUE_FM_LIGHT,
        SWAP_LIGHT,
        LFO_LIGHT,
        ZERO_LIGHT,
        POST_PM_SHAPE_LIGHT,
        DISPLAY_CV_LIGHT,
        NUM_LIGHTS
    };

    int panelStyle = 0;
    int displayStyle = 0;

    static const int kMaxNumGroups = 4;
    int numActiveChannels = 0;
    int numActiveGroups = 0;
    bool spreadActive = false;
    float spread = 0.01f;
    float spreadPitches[kMaxNumGroups * 4] = {
        0, -1, -0.866666667, -0.733333333, -0.6, -0.466666667, -0.333333333, -0.2,
        0.066666667, 0.2, 0.333333333, 0.466666667, 0.6, 0.733333333, 0.866666667, 1
    };

    FreqLUT freqLUT;
    int numVoices = 0;
    ScanningQuadOsc osc[kMaxNumGroups];
    VecEnhancer enhancer[kMaxNumGroups];
    VecLPG lpg[kMaxNumGroups];
    TFormSubOsc subOsc[kMaxNumGroups];
    VecOnePoleHPFilter mainOutDCBlock[kMaxNumGroups];
    VecOnePoleHPFilter rawOutDCBlock[kMaxNumGroups];
    VecOnePoleHPFilter enhancerOutDCBlock[kMaxNumGroups];
    VecSineLFO unisonDrifters[kMaxNumGroups];
    float drifterPhases[kMaxNumGroups * 4] = {0, 0.0625, 0.125, 0.1875, 0.25, 0.3125, 0.375, 0.4375,
                                              0.5, 0.5625, 0.625, 0.6875, 0.75, 0.8125, 0.875, 0.9375};
    float drifterFreqs[kMaxNumGroups * 4] = {0.00625, 0.0125, 0.01875, 0.025, 0.03125, 0.0375, 0.04375,
                                             0.05, 0.05625, 0.0625, 0.06875, 0.075, 0.08125, 0.0875, 0.09375, 0.1};

    // User wave table data
    float* userWaveTableData[TFORM_MAX_BANKS];
    char userWaveTableSizes[TFORM_MAX_BANKS];
    int userWaveTableWavelengths[TFORM_MAX_BANKS];
    bool userWaveTableFilled[TFORM_MAX_BANKS];
    char numUserWaveTables = 0;
    std::vector<std::string> userWaveTableNames;
    bool readFromUserWaves;

    // Outputs
    __m128 __mainOutput[kMaxNumGroups];
    __m128 __preEnhanceOutput[kMaxNumGroups];
    __m128 __phasorOutput[kMaxNumGroups];
    __m128 __shapedPhasorOutput[kMaxNumGroups];
    __m128 __eocOutput[kMaxNumGroups];
    __m128 __subOscOut;
    __m128 __lpgInput;

    float rootBank;
    float rootShapeType;
    float rootEnhanceType;

    int maxNumBanks;
    float bank;
    float bankCV;
    int bankI = -1;
    bool wavebankChanged = true;

    float** romWaveTableData;
    char romWaveTableSize = 0;
    int romWaveTableLength;

    float shapeType;
    float shapeTypeCV;
    int shapeTypeI;

    float enhanceType;
    float enhanceTypeCV;
    int enhanceTypeI;

    float rootPitch;
    float* freqs;
    __m128 __freq;
    float pitchCV1;
    float pitchCV2;

    float rootWave;
    float* wavesCV;
    float* waves;
    float numWavesInTable;
    __m128 __wave;
    __m128 __numWavesInTable;
    float bankCV1, bankCV2;
    float waveCV1, waveCV2;

    float bankDisplay;
    float shapeDisplay;
    float enhanceDisplay;

    float waveAmountDisplay;
    float shapeAmountDisplay;
    float enhanceAmountDisplay;

    Shaper::Modes phasorShapeMap[Shaper::Modes::NUM_MODES] = {
        Shaper::Modes::BEND_MODE,
        Shaper::Modes::TILT_MODE,
        Shaper::Modes::LEAN_MODE,
        Shaper::Modes::TWIST_MODE,
        Shaper::Modes::WRAP_MODE,
        Shaper::Modes::SINEWRAP_MODE,
        Shaper::Modes::MIRROR_MODE,
        Shaper::Modes::HARMONICS_MODE,
        Shaper::Modes::WARBLE_MODE,
        Shaper::Modes::REFLECT_MODE,
        Shaper::Modes::PULSE_MODE,
        Shaper::Modes::STEP4_MODE,
        Shaper::Modes::STEP8_MODE,
        Shaper::Modes::STEP16_MODE,
        Shaper::Modes::VARSTEP_MODE,
        Shaper::Modes::BUZZ_X2_MODE,
        Shaper::Modes::BUZZ_X4_MODE,
        Shaper::Modes::BUZZ_X8_MODE,
        Shaper::Modes::WRINKLE_X2_MODE,
        Shaper::Modes::WRINKLE_X4_MODE,
        Shaper::Modes::WRINKLE_X8_MODE,
        Shaper::Modes::SINE_DOWN_X2_MODE,
        Shaper::Modes::SINE_DOWN_X4_MODE,
        Shaper::Modes::SINE_DOWN_X8_MODE,
        Shaper::Modes::SINE_UP_X2_MODE,
        Shaper::Modes::SINE_UP_X4_MODE,
        Shaper::Modes::SINE_UP_X8_MODE
    };

    float rootShapeDepth;
    float* shapes;
    float* shapesCV;
    __m128 __shape;
    float shapeDepthCV1;
    float shapeDepthCV2;

    float rootEnhanceDepth;
    float* enhances;
    float* enhancesCV;
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

    // Lowpass Gate
    bool gateInput1IsMono;
    bool gateInput2IsMono;
    dsp::Timer lpgButtonTimer;
    int lpgMode = 0;
    bool lpgButtonPressed = false;
    bool lpgButtonHeldDown = false;
    bool lpgButtonPrevState = false;
    bool swapEnhancerAndLPG = false;
    __m128 __lpgVCAMode;
    __m128 __lpgFilterMode;

    bool lpgLongTime = false;
    bool lpgVelocitySensitive = false;
    bool lpgTriggerMode = false;

    __m128 __lpgVelocitySensitiveFlag;

    float* attacks;
    float* decays;
    float attackParam, decayParam;
    float attackCV1Depth, attackCV2Depth, decayCV1Depth, decayCV2Depth;
    __m128 __attackParam, __attackSum;
    __m128 __decayParam, __decaySum;

    // FM
    int fmMode = 0;
    float trueFMSwitchValue = 0.f;
    float prevTrueFMSwitchValue = 0.f;
    bool trueFMEnabled = false;
    bool postPMShapeEnabled = false;
    bool lfoModeEnabled = false;
    bool zeroFreqEnabled = false;
    bool fmA1IsMono, fmA2IsMono;
    bool fmB1IsMono, fmB2IsMono;
    bool fmAVCAIsMono, fmBVCAIsMono;
    bool fmAVCAIsConnected, fmBVCAIsConnected;
    float fmA1Level, fmA2Level, fmB1Level, fmB2Level, fmAVCACV, fmBVCACV, skewParam, skewCV;
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
    float* skew;
    __m128 __fmA, __fmB, __fmSum;
    __m128 __fmAVCA, __fmBVCA;
    __m128 __fmA1Level, __fmA2Level, __fmB1Level, __fmB2Level;
    __m128 __fmAVCACV, __fmBVCACV;
    __m128 __trigger1 , __trigger2;

    __m128 __zeros, __ones, __negOnes, __twos, __negTwos, __fives, __negFives, __tens, __negTens;
    __m128 __quarters, __tenths, __hundredths;
    __m128 __fullLevel, __halfLevel;

    int counter = 512;

    bool romIsLoading = false;
    bool displayCV = false;
    bool minus12dB = false;

    Terrorform();
    ~Terrorform();

    void process(const ProcessArgs &args) override;
    void onSampleRateChange() override;
    void onReset() override; // For some stupid reason some buttons don't reset?!??!?!
    void manageVoices();
    void changeBank(int newBank);

    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
    void clearBank(int bankNum);
    void clearUserWaveTables();
    void cloneBank(int sourceBank, int destBank, int startWave, int endWave);
    void moveBank(int sourceBank, int destBank);
    void defragmentBanks();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct TerrorformManagerItem : MenuItem {
    std::function<void()> openMenu;
    void onAction(const event::Action &e) override;
};

struct TerrorformVoicingValueItem : MenuItem {
    Terrorform* module;
    int numVoices;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct TerrorformVoicingItem : MenuItem {
    Terrorform* module;
    Menu* createChildMenu() override;
};

struct TerrorformSpreadVoicesItem : MenuItem {
    Terrorform* module;
    void onAction(const event::Action &e) override;
};

struct TerrorformOutputLevelItem : MenuItem {
    Terrorform* module;
    void onAction(const event::Action &e) override;
};

struct TerrorformDisplayStyleItem : MenuItem {
    Terrorform* module;
    int displayStyle;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct TerrorformPanelStyleItem : MenuItem {
    Terrorform* module;
    int panelStyle;
    void onAction(const event::Action &e) override;
    void step() override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

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

    // Normal colours
    const DigitalDisplayColours redDisplayNormal = {
        {0x3F,0x00,0x00,0xFF}, {0xFF,0x5F,0x5F,0xBF},
        {0xFF,0x2F,0x2F,0xBF}, {0xFF,0x00,0x00,0xFF}
    };

    const DigitalDisplayColours yellowDisplayNormal = {
        {0x44,0x15,0x00,0xFF}, {0xFF,0xD5,0x3F,0xBF},
        {0xFF,0xD5,0x1F,0xBF}, {0xFF,0xB0,0x00,0xFF}
    };

    const DigitalDisplayColours greenDisplayNormal = {
        {0x00,0x3F,0x00,0xFF}, {0x5F,0xFF,0x5F,0xBF},
        {0x2F,0xFF,0x2F,0xBF}, {0x00,0xFF,0x1F,0xFF}
    };

    const DigitalDisplayColours blueDisplayNormal = {
        {0x00,0x31,0x30,0xFF}, {0x72,0xFF,0xEE,0xBF},
        {0x2A,0xFF,0xF9,0xBF}, {0x00,0xFF,0xEF,0xFF}
    };

    const DigitalDisplayColours whiteDisplayNormal = {
        {0x30,0x30,0x30,0xFF}, {0xAF,0xAF,0xAF,0xBF},
        {0xCF,0xCF,0xCF,0xBF}, {0xEE,0xEE,0xEE,0xFF}
    };

    // Hover Colours
    const DigitalDisplayColours redDisplayHover = {
        {0x6F,0x00,0x00,0xFF}, {0xFF,0x7F,0xFF,0xEF},
        {0xFF,0x5F,0xFF,0xEF}, {0xFF,0x4F,0x6F,0xFF}
    };

    const DigitalDisplayColours yellowDisplayHover = {
        {0x64,0x35,0x00,0xFF}, {0xFF,0xE5,0x3F,0xBF},
        {0xFF,0xE5,0x1F,0xBF}, {0xFF,0xD0,0x10,0xFF}
    };

    const DigitalDisplayColours greenDisplayHover = {
        {0x00,0x6F,0x00,0xFF}, {0x7F,0xFF,0x7F,0xBF},
        {0x5F,0xFF,0x5F,0xBF}, {0x2F,0xFF,0x5F,0xFF}
    };

    const DigitalDisplayColours blueDisplayHover = {
        {0x00,0x61,0x60,0xFF}, {0x92,0xFF,0xFE,0xBF},
        {0x5A,0xFF,0xFF,0xBF}, {0x4F,0xFF,0xFF,0xFF}
    };

    const DigitalDisplayColours whiteDisplayHover = {
        {0x50,0x50,0x50,0xFF}, {0xCF,0xCF,0xCF,0xBF},
        {0xEF,0xEF,0xEF,0xBF}, {0xFF,0xFF,0xFF,0xFF}
    };

    DigitalDisplayColours normalColours = redDisplayNormal;
    DigitalDisplayColours hoverColours = redDisplayHover;

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
    Vec lpgAttackPos = Vec(125, 196);
    Vec lpgDecayPos = Vec(175, 196);
    Vec skewPos = Vec(64, 243.265);
    Vec subOscLevelPos = Vec(236, 243.265);
    Vec subOscWavePos = Vec(260, 245.265);

    RoganMedBlue* octaveKnob;
    RoganMedBlue* coarseKnob;
    RoganMedBlue* fineKnob;
    RoganMedPurpleWithModeText* bankKnob;
    RoganMedPurple* waveKnob;
    RoganMedRedWithModeText* shapeTypeKnob;
    RoganMedRed* shapeDepthKnob;
    RoganMedGreenWithModeText* enhanceTypeKnob;
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
    Vec shapeTypeCV2Pos = Vec(46, 169);
    Vec shapeDepthCV1Pos = Vec(258, 122);
    Vec shapeDepthCV2Pos = Vec(253, 169);
    Vec enhanceTypeCV1Pos = Vec(33, 190);
    Vec enhanceTypeCV2Pos = Vec(71, 220);
    Vec enhanceDepthCV1Pos = Vec(267, 190);
    Vec enhanceDepthCV2Pos = Vec(229, 220);
    Vec lpgAttackCV1Pos = Vec(99, 211);
    Vec lpgAttackCV2Pos = Vec(122, 230);
    Vec lpgDecayCV1Pos = Vec(201, 211);
    Vec lpgDecayCV2Pos = Vec(178, 230);

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
    LightLEDButtonWithModeText* lfoButton;
    LightLEDButtonWithModeText* zeroFreqButton;
    LightLEDButtonWithModeText* userBankButton;
    LightLEDButtonWithModeText* displayCVButton;
    LightLEDButtonWithModeText* phasorShapingOrderButton;
    LightLEDButtonWithModeText* trueFMButton;
    LightLEDButtonWithModeText* swapButton;
    LightLEDButtonWithModeText* weakSync1Button;
    LightLEDButtonWithModeText* weakSync2Button;

    LightLEDButtonWithModeText* lpgButton;
    LightLEDButtonWithModeText* lpgLongTimeButton;
    LightLEDButtonWithModeText* lpgVelocityButton;
    LightLEDButtonWithModeText* lpgTrigButton;

    MediumLight<RedLight>* lfoButtonLight;
    MediumLight<RedLight>* zeroFreqLight;
    MediumLight<RedLight>* userBankLight;
    MediumLight<RedLight>* displayCVLight;
    MediumLight<RedGreenBlueLight>* lpgButtonLight;
    MediumLight<RedLight>* lpgLongTimeButtonLight;
    MediumLight<RedLight>* lpgVelocityButtonLight;
    MediumLight<RedLight>* lpgTrigButtonLight;

    Vec lfoButtonPos = Vec(121, 55);
    Vec zeroFreqButtonPos = Vec(179, 55);
    Vec userBankSwitchPos = Vec(150, 84);
    Vec displayCVSwitchPos = Vec(150, 107);

    Vec lpgModeSwitchPos = Vec(150, 176);
    Vec lpgLongTimeSwitchPos = Vec(150, 201);
    Vec lpgVelocitySwitchPos = Vec(141, 226);
    Vec lpgTrigSwitchPos = Vec(159, 226);

    Vec weakSyncSwitch1Pos = Vec(108, 280);
    Vec weakSyncSwitch2Pos = Vec(192, 280);
    Vec trueFMButtonPos = Vec(95.3443, 301);
    Vec postPMShapeButtonPos = Vec(76.7443, 301);
    Vec swapButtonPos = Vec(218.34, 293);

    Vec vcaAPos = Vec(44, 280);
    Vec fmA1Pos = Vec(44, 305);
    Vec fmA2Pos = Vec(44, 330);

    Vec vcaBPos = Vec(256, 280);
    Vec fmB1Pos = Vec(256, 305);
    Vec fmB2Pos = Vec(256, 330);

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
    Vec shapeTypeInput2Pos = Vec(col1X, 165);

    Vec shapeDepthInput1Pos = Vec(col2X, 133);
    Vec shapeDepthInput2Pos = Vec(col2X, 165);

    Vec enhanceTypeInput1Pos = Vec(col1X, 209);
    Vec enhanceTypeInput2Pos = Vec(col1X, 240);

    Vec enhanceDepthInput1Pos = Vec(col2X, 209);
    Vec enhanceDepthInput2Pos = Vec(col2X, 240);

    Vec vcaAInputPos = Vec(col1X, 273);
    Vec fmA1InputPos = Vec(col1X, 309);
    Vec fmA2InputPos = Vec(col1X, 341);
    Vec vcaBInputPos = Vec(col2X, 273);
    Vec fmB1InputPos = Vec(col2X, 309);
    Vec fmB2InputPos = Vec(col2X, 341);

    Vec syncInput1Pos = Vec(89, 261);
    Vec syncInput2Pos = Vec(211, 261);

    Vec attackInput1Pos = Vec(93, 236);
    Vec attackInput2Pos = Vec(112, 253);
    Vec decayInput1Pos = Vec(207, 236);
    Vec decayInput2Pos = Vec(188, 253);
    Vec triggerInput1Pos = Vec(128, 273);
    Vec triggerInput2Pos = Vec(172, 273);

    Vec skewInputPos = Vec(40, 245.265);

    // Outputs
    Vec phasorOutPos =  Vec(67.9541, 331);
    Vec eocOutPos =     Vec(95.3541, 331);
    Vec shapedOutPos =  Vec(122.9541, 331);
    Vec rawOutPos =     Vec(150, 331);
    Vec enhancerOutPos = Vec(177.3541, 331);
    Vec subOutPos =     Vec(204.6541, 331);
    Vec mainOutPos =    Vec(231.9541, 331);
    Vec envOutPos =     Vec(150, 255.5);

    // Text
    Vec bankTextPos = Vec(97.46, 126.358);
    Vec shapeTextPos = Vec(97.46, 139.211);
    Vec enhanceTextPos = Vec(97.46, 152.065);

    Vec waveTextPos = Vec(202.897, 126.358);
    Vec shapeDepthTextPos = Vec(202.897, 139.211);
    Vec enhanceDepthTextPos = Vec(202.897, 152.065);

    Vec syncTextPos = Vec(149.829, 303);

    SvgPanel* panels[NUM_TRRFORM_PANELS];
    SvgPanel* darkPanel;
    SvgPanel* lightPanel;
    bool inEditorMode = false;
    DigitalDisplay* bankText;
    DigitalDisplay* shapeText;
    DigitalDisplay* enhanceText;

    DigitalDisplay* waveText;
    DigitalDisplay* shapeDepthText;
    DigitalDisplay* enhanceDepthText;
    DigitalDisplay* syncText;

    PlainText* rightHandVOctText;
    Vec rightHandVOctTextPos = Vec(284.5, 51.5);
    bool spreadActive = false;

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
    bool prevReadUserWavesState = false;

    unsigned long bankKnobStep = 0;
    unsigned long shapeTypeKnobStep = 0;
    unsigned long enhanceTypeKnobStep = 0;
    std::string bankKnobTooltipText;
    std::string shapeTypeKnobTooltipText;
    std::string enhanceTypeKnobTooltipText;

    TFormEditor* editor;
    std::string dir;
    float *newTable;
    unsigned int numChannels;

    TerrorformDisplayColourModes displayStyle = CELL_RED_LED_COLOUR;
    TerrorformDisplayColourModes prevDisplayStyle = CELL_RED_LED_COLOUR;

    std::vector<std::string> bankNames = {
        "BASIC", "OPAL",  "TEE_EKS", "SAW_PHASE",
        "SAW_GAP1", "SAW_GAP2", "PWM", "BI_PULSE",
        "RESO_SAW", "RESO_SQR", "SINE_HARM", "AM_HARM",
        "SWEEPHARM", "XFADE", "CHIRP", "ADD_SINE",
        "ADD_SAW", "ADD_SQR", "ADD_BANK1", "ADD_BANK2",
        "ADD_BANK3", "ADD_BANK4", "FOLD_SINE", "2_OP_RAND",
        "2_OPFM1", "2_OPFM2", "FM1", "FM2",
        "FM3", "FM4", "OBOE", "ALTOSAX",
        "REED", "CELLO_1", "CELLO_2", "VIOLIN",
        "PIANO", "EBASS", "PLUCK", "THEREMIN",
        "OVERTONE1", "OVERTONE2", "VOICE_1", "VOICE_2", "VOICE_3", "VOICE_4",
        "VOICE_5", "VOICE_6", "VOX_MACH", "SYMMETRY", "CHIP_1", "CHIP_2",
        "BITCRUSH1", "BITCRUSH2", "VIDEOGAME", "LINEAR",
        "DISTORT", "GRIT", "GMTRY_1", "GMTRY_2",
        "GMTRY_3", "PLAITS_1", "PLAITS_2", "PLAITS_3"
    };

    std::vector<std::string> bankMenuItems = {
        "Basic", "Opal", "Tee Eks", "Saw Phase", "Saw Gap 1", "Saw Gap 2", "PWM", "Bi Pulse",
        "Resonant Saw", "Resonant Square", "Sine Harmonics", "Amp. Mod. Harmonics",
        "Sweep Harmonics", "X Fade", "Chirp", "Additive Sine", "Additive Saw", "Additive Square",
        "Additive Bank 1", "Additive Bank 2", "Additive Bank 3", "Additive Bank 4", "Folding Sine",
        "Two OP FM Random", "Two OP FM Bank 1", "Two OP FM Bank 2", "FM 1", "FM 2", "FM 3", "FM 4",
        "Oboe", "Alto Sax", "Reed", "Cello 1", "Cello 2", "Violin", "Piano", "Electric Bass", "Pluck",
        "Theremin", "Overtone 1", "Overtone 2", "Voice 1", "Voice 2", "Voice 3", "Voice 4", "Voice 5",
        "Voice 6", "Vox Machine", "Symmetry", "Chip 1", "Chip 2", "Bitcrush 1", "Bircrush 2", "Video Game", "Linear",
        "Distorted", "Grit", "Geometry 1", "Geometry 2", "Geometry 3", "Plaits 1", "Plaits 2", "Plaits 3"

    };

    std::vector<std::string> shapeNames = {
        "BEND", "TILT", "LEAN", "TWIST", "WRAP", "SINE_WRAP", "MIRROR", "HARMONICS", "WARBLE",
        "REFLECT", "PULSE", "STEP_4", "STEP_8", "STEP_16", "VAR_STEP", "BUZZ", "BUZZ_X2",
        "BUZZ_X4", "WRINKLE", "WRINKLE_X2", "WRINKLE_X4", "SINE_DOWN", "SINEDWNX2", "SINEDWNX4",
        "SINE_UP", "SINEUP_X2", "SINEUP_X4"
    };

    std::vector<std::string> shapeMenuItems = {
        "Bend", "Tilt", "Lean", "Twist", "Wrap", "Sine Wrap", "Mirror", "Harmonics", "Warble",
        "Reflect", "Pulse", "Step 4", "Step 8", "Step 16", "Variable Step", "Buzz", "Buzz x2",
        "Buzz x4", "Wrinkle", "Wrinkle x2", "Wrinkle x4", "Sine Down", "Sine Down x2",
        "Sine Down x4", "Sine Up", "Sine Up x2", "Sine Up x4"
    };

    std::vector<std::string> enhanceNames = {
        "BITCRUSH", "QUANTIZE", "AND_INT", "XOR_INT", "AND_FLOAT", "OVERDRIVE",
        "RING_MOD", "SHARPEN", "SINE", "FOLD", "MIRROR"
    };

    std::vector<std::string> enhanceMenuItems = {
        "Bit Crush", "Quantize", "Bitwise AND Int", "Bitwise XOR Int", "Bitwise AND Float", "Overdrive",
        "Ring Mod", "Sharpen", "Sine", "Fold", "Mirror"
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
    void exportWavetablesPathSelected(char* path);
    void importWavetables();
    void importWavetablesPathSelected(char* path);
};

#endif
