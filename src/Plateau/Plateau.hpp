//
// Plateu.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 24/6/2018
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
// Plateau is based on the venerable Dattorro (1997) reverb algorithm.
// Reference: Dattorro, J. (1997). Effect design part 1: Reverberator and other filters, J. Audio
// Eng. Soc, 45(9), 660-684.

#ifndef DSJ_PLATEAU_HPP
#define DSJ_PLATEAU_HPP

#include "../Valley.hpp"
#include "../gui/ValleyComponents.hpp"
#include "Dattorro.hpp"
#include "../dsp/shaping/NonLinear.hpp"
#include "../dsp/modulation/LinearEnvelope.hpp"
#include <vector>

struct Plateau : Module {

    enum InputIds {
        LEFT_INPUT,
        RIGHT_INPUT,

        DRY_CV_INPUT,
        WET_CV_INPUT,
        PRE_DELAY_CV_INPUT,
        INPUT_LOW_DAMP_CV_INPUT,
        INPUT_HIGH_DAMP_CV_INPUT,

        SIZE_CV_INPUT,
        DIFFUSION_CV_INPUT,
        DECAY_CV_INPUT,
        REVERB_HIGH_DAMP_CV_INPUT,
        REVERB_LOW_DAMP_CV_INPUT,

        MOD_SPEED_CV_INPUT,
        MOD_SHAPE_CV_INPUT,
        MOD_DEPTH_CV_INPUT,

        FREEZE_CV_INPUT,
        CLEAR_CV_INPUT,

        NUM_INPUTS
    };

    enum OutputIds {
        LEFT_OUTPUT,
        RIGHT_OUTPUT,
        NUM_OUTPUTS
    };

    enum ParamIds {
        DRY_PARAM,
        WET_PARAM,
        PRE_DELAY_PARAM,
        INPUT_LOW_DAMP_PARAM,
        INPUT_HIGH_DAMP_PARAM,

        SIZE_PARAM,
        DIFFUSION_PARAM,
        DECAY_PARAM,
        REVERB_HIGH_DAMP_PARAM,
        REVERB_LOW_DAMP_PARAM,

        MOD_SPEED_PARAM,
        MOD_SHAPE_PARAM,
        MOD_DEPTH_PARAM,

        FREEZE_PARAM,
        CLEAR_PARAM,
        FREEZE_TOGGLE_PARAM,
        CLEAR_TOGGLE_PARAM,

        DRY_CV_PARAM,
        WET_CV_PARAM,
        INPUT_LOW_DAMP_CV_PARAM,
        INPUT_HIGH_DAMP_CV_PARAM,

        SIZE_CV_PARAM,
        DIFFUSION_CV_PARAM,
        DECAY_CV_PARAM,
        REVERB_HIGH_DAMP_CV_PARAM,
        REVERB_LOW_DAMP_CV_PARAM,

        MOD_SPEED_CV_PARAM,
        MOD_SHAPE_CV_PARAM,
        MOD_DEPTH_CV_PARAM,

        TUNED_MODE_PARAM,
        DIFFUSE_INPUT_PARAM,

        NUM_PARAMS
    };

    enum LightIds {
        FREEZE_LIGHT,
        CLEAR_LIGHT,
        FREEZE_TOGGLE_LIGHT,
        TUNED_MODE_LIGHT,
        DIFFUSE_INPUT_LIGHT,
        NUM_LIGHTS
    };

    // CV scaling
    const float dryMin = 0.f;
    const float dryMax = 1.f;
    const float wetMin = 0.f;
    const float wetMax = 1.f;
    const float preDelayNormSens = 0.1f;
    const float preDelayLowSens = 0.05f;
    const float sizeMin = 0.0025f;
    const float sizeMax = 4.0f;
    const float diffMin = 0.f;
    const float diffMax = 1.f;
    const float decayMin = 0.1f;
    const float decayMax = 0.9999f;
    const float reverbLowDampMin = 0.f;
    const float reverbLowDampMax = 10.f;
    const float reverbHighDampMin = 0.f;
    const float reverbHIghDampMax = 10.f;
    const float modSpeedMin = 0.f;
    const float modSpeedMax = 1.f;
    const float modDepthMin = 0.f;
    const float modDepthMax = 16.f;
    const float modShapeMin = 0.001f;
    const float modShapeMax = 0.999f;

    static constexpr float minus20dBGain = 0.1f;
    static constexpr float minus18dBGain = 0.12589254f;
    static constexpr float zerodBGain = 1.f;

    static constexpr float saturatorPreGain = 0.111f;
    static constexpr float saturatorDrive = 0.95f;
    static constexpr float saturatorPostGain = 9.999f;

    float wet = 0.5f;
    float dry = 1.f;
    float preDelay = 0.f;
    float preDelayCVSens = preDelayNormSens;
    float size = 1.f;
    float diffusion = 1.f;
    float decay = 0.f;
    float inputSensitivity = minus18dBGain;
    float inputDampLow = 0.f;
    float inputDampHigh = 10.f;
    float reverbDampLow = 0.f;
    float reverbDampHigh = 10.f;
    float modSpeed = 0.1f;
    float modShape = 0.5f;
    float modDepth = 0.f;

    bool freezeButtonState = false;
    bool freezeToggle = false;
    bool freezeToggleButtonState = false;
    bool freeze = false;
    bool frozen = false;
    bool tunedButtonState = false;
    bool diffuseButtonState = false;
    int preDelayCVSensState = 0;
    int inputSensitivityState = 0;
    bool softDriveOutput = 0;

    bool clear = false;
    bool cleared = true;
    bool fadeOut = false;
    bool fadeIn = false;

    float leftInput = 0.f;
    float rightInput = 0.f;
    float leftOutput = 0.f;
    float rightOutput = 0.f;

    Dattorro reverb;
    LinearEnvelope envelope;

    int panelStyle = 0;
    int tuned = 0;
    int diffuseInput = 1;

    Plateau();

    void process(const ProcessArgs &args) override;

    void getParameters();
    void setLights();

    void onSampleRateChange() override;
    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
    void onReset(const ResetEvent& e) override;
};

struct PlateauPanelStyleItem : MenuItem {
    Plateau* module;
    int panelStyle;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct PlateauPreDelayCVSensItem : MenuItem {
    Plateau* module;
    int preDelayCVSensState;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct PlateauInputSensItem : MenuItem {
    Plateau* module;
    int inputSensitivityState;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct PlateauOutputSaturationItem : MenuItem {
    Plateau* module;
    bool softDriveOutput = false;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct PlateauWidget : ModuleWidget {
    PlateauWidget(Plateau *module);
    void appendContextMenu(Menu *menu) override;
    void step() override;

    // Control positions
    Vec dryPos = Vec(52.1, 61.6);
    Vec wetPos = Vec(102.6, 61.6);
    Vec preDelayPos = Vec(80.106, 26.106);
    Vec inputLowDampPos = Vec(53.1, 113.1);
    Vec inputHighDampPos = Vec(95.1, 113.1);

    Vec sizePos = Vec(32.1, 170.1);
    Vec diffPos = Vec(74.1, 181.1);
    Vec decayPos = Vec(116.1, 170.1);
    Vec reverbLowDampPos = Vec(53.1, 238.1);
    Vec reverbHighDampPos = Vec(95.1, 238.1);

    Vec modRatePos = Vec(32.1, 296.1);
    Vec modShapePos = Vec(74.1, 310.1);
    Vec modDepthPos = Vec(116.1, 296.1);

    Vec holdButtonPos = Vec(7.875, 244.85);
    Vec clearButtonPos = Vec(157.875, 244.85);

    // Attenuverter positions
    Vec dryAttenPos = Vec(28.53, 72.6);
    Vec wetAttenPos = Vec(132.01, 72.6);
    Vec inputLowDampAttenPos = Vec(29.53, 111.59);
    Vec inputHighDampAttenPos = Vec(131.01, 111.59);

    Vec sizeAttenPos = Vec(5.1, 164.1);
    Vec diffAttenPos = Vec(65.11, 158.51);
    Vec decayAttenPos = Vec(155.1, 164.1);
    Vec reverbLowDampAttenPos = Vec(29.1, 229.37);
    Vec reverbHighDampAttenPos = Vec(131.1, 229.37);

    Vec modRateAttenPos = Vec(5.1, 306.1);
    Vec modShapeAttenPos = Vec(65.1, 286.2);
    Vec modDepthAttenPos = Vec(155.1, 306.1);

    // Jack positions
    Vec leftInputPos = Vec(4.395, 28.385);
    Vec rightInputPos = Vec(31.395, 28.385);
    Vec leftOutputPos = Vec(127.395, 28.385);
    Vec rightOutputPos = Vec(154.395, 28.385);

    Vec dryCVPos = Vec(4.395, 78.397);
    Vec wetCVPos = Vec(154.395, 78.397);
    Vec preDelayCVPos = Vec(79.106, 67.0);
    Vec inputLowDampCVPos = Vec(4.395, 103.926);
    Vec inputHighDampCVPos = Vec(154.395, 103.926);

    Vec sizeCVPos = Vec(4.395, 190.395);
    Vec diffCVPos = Vec(94.395, 157.794);
    Vec decayCVPos = Vec(154.395, 190.395);
    Vec reverbLowDampCVPos = Vec(4.395, 217.383);
    Vec reverbHighDampCVPos = Vec(154.395, 217.383);

    Vec modRateCVPos = Vec(4.395, 331.395);
    Vec modShapeCVPos = Vec(94.395, 285.51);
    Vec modDepthCVPos = Vec(154.395, 331.395);

    Vec holdCVPos = Vec(4.395, 265.42);
    Vec clearCVPos = Vec(154.395, 265.42);

    // The widgets
    SvgPanel* darkPanel;
    SvgPanel* lightPanel;

    LightLEDButton* freezeToggleButton;
    LightLEDButton* tunedModeButton;
    LightLEDButton* diffuseInputButton;
};

#endif
