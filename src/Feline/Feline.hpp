//
// Feline.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 11/2/2019
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

#ifndef DSJ_FELINE_HPP
#define DSJ_FELINE_HPP

#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "../Common/SIMD/VecOTAFilter.hpp"
#include "../Common/SIMD/VecOnePoleFilters.hpp"
#include <vector>

struct Feline : Module {

    enum InputIds {
        LEFT_INPUT,
        RIGHT_INPUT,
        CV1_1_INPUT, CV1_2_INPUT, CV1_3_INPUT, CV1_4_INPUT, CV1_5_INPUT,
        CV2_1_INPUT, CV2_2_INPUT, CV2_3_INPUT, CV2_4_INPUT, CV2_5_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        LEFT_OUTPUT,
        RIGHT_OUTPUT,
        SUM_OUTPUT,
        NUM_OUTPUTS
    };

    enum ParamIds {
        CUTOFF_PARAM,
        RESONANCE_PARAM,
        POLES_PARAM,
        TYPE_PARAM,
        SPACING_PARAM,
        SPACING_POLARITY_PARAM,
        DRIVE_PARAM,
        CV1_1_PARAM, CV1_2_PARAM, CV1_3_PARAM, CV1_4_PARAM, CV1_5_PARAM,
        CV2_1_PARAM, CV2_2_PARAM, CV2_3_PARAM, CV2_4_PARAM, CV2_5_PARAM,
        NUM_PARAMS
    };

    enum LightIds {
        NUM_LIGHTS
    };

    __m128 __input;
    __m128 __output;

    float input[2];
    float output[4];
    float hpfFreq;
    float drive;
    float spacing, cutoff, resonance;
    float leftCutoff, rightCutoff;
    float prevLeftCutoff, prevRightCutoff;

    int panelStyle;
    VecOTAFilter filter;
    __m128 __filterOut;
    __m128 __state[4];

    Feline();
    void step() override;
    void onSampleRateChange() override;
    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
};

struct FelinePanelStyleItem : MenuItem {
    Feline* module;
    int panelStyle;
    void onAction(const event::Action &e) override;
    void step() override;
};

struct FelineWidget : ModuleWidget {
    FelineWidget(Feline *module);
    void appendContextMenu(Menu *menu) override;
    void step() override;

    Vec leftInputPos = Vec(4.7f, 316.7f);
    Vec rightInputPos = Vec(27.2f, 316.7f);
    Vec sumOutputPos = Vec(49.7f, 316.7f);
    Vec leftOutputPos = Vec(72.2f, 316.7f);
    Vec rightOutputPos = Vec(94.7f, 316.7f);

    Vec cutoffSliderPos = Vec(9.f, 31.8f);
    Vec resonanceSliderPos = Vec(36.f, 31.8f);
    Vec spacingSliderPos = Vec(63.f, 31.8f);
    Vec spacingPolSliderPos = Vec(89.f, 31.8f);

    Vec polesPos = Vec(12.495f, 157.662f);
    Vec typePos = Vec(93.505f, 157.662f);
    Vec driveKnobPos = Vec(44.1f, 146.1f);
    float cvKnobY[2] = {199.1f, 252.1f};
    float cvKnobX[5] = {5.1f, 27.6f, 50.1f, 72.6f, 95.1f};
    float cvJackY[2] = {224.7f, 277.7f};
    float cvJackX[5] = {4.7f, 27.2f, 49.7f, 72.2f, 94.7f};

    SvgPanel* darkPanel;
    SvgPanel* lightPanel;
};

#endif
