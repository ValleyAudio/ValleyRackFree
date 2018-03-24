//
// UGraph.cpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 5/12/2017
//
// UGraph, a port of "Mutable Instruments Grids" for VCV Rack
// Original author: Olivier Gillet (ol.gillet@gmail.com)
// https://github.com/pichenettes/eurorack/tree/master/grids
// Copyright 2012 Olivier Gillet.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "dsp/digital.hpp"
#include "../Common/Metronome.hpp"
#include "../Common/Oneshot.hpp"
#include "../Topograph/TopographPatternGenerator.hpp"

struct UGraph : Module {
    enum ParamIds {
        RESET_BUTTON_PARAM,
        RUN_BUTTON_PARAM,
        TEMPO_PARAM,
        MAPX_PARAM,
        MAPY_PARAM,
        CHAOS_PARAM,
        BD_DENS_PARAM,
        SN_DENS_PARAM,
        HH_DENS_PARAM,
        SWING_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        CLOCK_INPUT,
        RESET_INPUT,
        MAPX_CV,
        MAPY_CV,
        CHAOS_CV,
        BD_FILL_CV,
        SN_FILL_CV,
        HH_FILL_CV,
        SWING_CV,
        RUN_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        BD_OUTPUT,
        SN_OUTPUT,
        HH_OUTPUT,
        BD_ACC_OUTPUT,
        SN_ACC_OUTPUT,
        HH_ACC_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        RUNNING_LIGHT,
        RESET_LIGHT,
        BD_LIGHT,
        SN_LIGHT,
        HH_LIGHT,
        NUM_LIGHTS
    };

    Metronome metro;
    PatternGenerator grids;
    uint8_t numTicks;
    SchmittTrigger clockTrig;
    SchmittTrigger resetTrig;
    SchmittTrigger resetButtonTrig;
    SchmittTrigger runButtonTrig;
    SchmittTrigger runInputTrig;
    bool initExtReset = true;
    int running = 0;
    bool extClock = false;
    bool advStep = false;
    long seqStep = 0;
    float swing = 0.5;
    float swingHighTempo = 0.0;
    float swingLowTempo = 0.0;
    long elapsedTicks = 0;

    float tempoParam = 0.0;
    float tempo = 120.0;
    float mapX = 0.0;
    float mapY = 0.0;
    float chaos = 0.0;
    float BDFill = 0.0;
    float SNFill = 0.0;
    float HHFill = 0.0;

    uint8_t state = 0;

    // LED Triggers
    Oneshot drumLED[3];
    const LightIds drumLEDIds[3] = {BD_LIGHT, SN_LIGHT, HH_LIGHT};
    Oneshot BDLed;
    Oneshot SNLed;
    Oneshot HHLed;
    Oneshot resetLed;
    Oneshot runningLed;

    // Drum Triggers
    Oneshot drumTriggers[6];
    bool gateState[6];
    const OutputIds outIDs[6] = {BD_OUTPUT, SN_OUTPUT, HH_OUTPUT,
                                 BD_ACC_OUTPUT, SN_ACC_OUTPUT, HH_ACC_OUTPUT};

    enum SequencerMode {
        HENRI,
        OLIVIER,
        EUCLIDEAN
    };
    SequencerMode sequencerMode = HENRI;

    enum TriggerOutputMode {
        PULSE,
        GATE
    };
    TriggerOutputMode triggerOutputMode = PULSE;

    enum AccOutputMode {
        INDIVIDUAL_ACCENTS,
        ACC_CLK_RST
    };
    AccOutputMode accOutputMode = INDIVIDUAL_ACCENTS;

    enum ExtClockResolution {
        EXTCLOCK_RES_4_PPQN,
        EXTCLOCK_RES_8_PPQN,
        EXTCLOCK_RES_24_PPQN,
    };
    ExtClockResolution extClockResolution = EXTCLOCK_RES_24_PPQN;

    enum ChaosKnobMode {
        CHAOS,
        SWING
    };
    ChaosKnobMode chaosKnobMode = CHAOS;

    enum RunMode {
        TOGGLE,
        MOMENTARY
    };
    RunMode runMode = TOGGLE;

    int panelStyle;
    std::string clockBPM;
    std::string mapXText = "Map X";
    std::string mapYText = "Map Y";
    std::string chaosText = "Chaos";
    int textVisible = 1;

    UGraph() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        metro = Metronome(120, engineGetSampleRate(), 24.0, 0.0);
        numTicks = ticks_granularity[2];
        srand(time(NULL));
        BDLed = Oneshot(0.1, engineGetSampleRate());
        SNLed = Oneshot(0.1, engineGetSampleRate());
        HHLed = Oneshot(0.1, engineGetSampleRate());
        resetLed = Oneshot(0.1, engineGetSampleRate());
        //clockTrig.setThresholds(0.25, 0.75);
        //resetTrig.setThresholds(0.25, 0.75);
        //runInputTrig.setThresholds(0.25, 0.75);
        for(int i = 0; i < 6; ++i) {
            drumTriggers[i] = Oneshot(0.001, engineGetSampleRate());
            gateState[i] = false;
        }
        for(int i = 0; i < 3; ++i) {
            drumLED[i] = Oneshot(0.1, engineGetSampleRate());
        }
        panelStyle = 0;
    }

    json_t *toJson() override {
        json_t *rootJ = json_object();
        json_object_set_new(rootJ, "sequencerMode", json_integer(sequencerMode));
        json_object_set_new(rootJ, "triggerOutputMode", json_integer(triggerOutputMode));
        json_object_set_new(rootJ, "accOutputMode", json_integer(accOutputMode));
        json_object_set_new(rootJ, "extClockResolution", json_integer(extClockResolution));
        json_object_set_new(rootJ, "chaosKnobMode", json_integer(chaosKnobMode));
        json_object_set_new(rootJ, "runMode", json_integer(runMode));
        json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
        return rootJ;
    }

    void fromJson(json_t *rootJ) override {
        json_t *sequencerModeJ = json_object_get(rootJ, "sequencerMode");
        if (sequencerModeJ) {
            sequencerMode = (UGraph::SequencerMode) json_integer_value(sequencerModeJ);
            switch(sequencerMode) {
                case HENRI:
                    grids.setPatternMode(PATTERN_HENRI);
                    break;
                case OLIVIER:
                    grids.setPatternMode(PATTERN_OLIVIER);
                    break;
                case EUCLIDEAN:
                    grids.setPatternMode(PATTERN_EUCLIDEAN);
                    break;
            }
		}

        json_t *triggerOutputModeJ = json_object_get(rootJ, "triggerOutputMode");
		if (triggerOutputModeJ) {
			triggerOutputMode = (UGraph::TriggerOutputMode) json_integer_value(triggerOutputModeJ);
		}

        json_t *accOutputModeJ = json_object_get(rootJ, "accOutputMode");
		if (accOutputModeJ) {
			accOutputMode = (UGraph::AccOutputMode) json_integer_value(accOutputModeJ);
            switch(accOutputMode) {
                case INDIVIDUAL_ACCENTS:
                    grids.setAccentAltMode(false);
                    break;
                case ACC_CLK_RST:
                    grids.setAccentAltMode(true);
            }
		}

        json_t *extClockResolutionJ = json_object_get(rootJ, "extClockResolution");
		if (extClockResolutionJ) {
			extClockResolution = (UGraph::ExtClockResolution) json_integer_value(extClockResolutionJ);
            grids.reset();
		}

        json_t *chaosKnobModeJ = json_object_get(rootJ, "chaosKnobMode");
		if (chaosKnobModeJ) {
			chaosKnobMode = (UGraph::ChaosKnobMode) json_integer_value(chaosKnobModeJ);
		}

        json_t *runModeJ = json_object_get(rootJ, "runMode");
		if (runModeJ) {
			runMode = (UGraph::RunMode) json_integer_value(runModeJ);
		}

        json_t *panelStyleJ = json_object_get(rootJ, "panelStyle");
        if (panelStyleJ) {
            panelStyle = (int)json_integer_value(panelStyleJ);
        }
	}

    void step() override;
    void onSampleRateChange() override;
    void updateUI();
    void updateOutputs();
};

void UGraph::step() {
    if(runMode == TOGGLE) {
        if (runButtonTrig.process(params[RUN_BUTTON_PARAM].value) ||
            runInputTrig.process(inputs[RUN_INPUT].value)) {
            if(runMode == TOGGLE){
                running = !running;
                lights[RUNNING_LIGHT].value = running ? 1.0 : 0.0;
            }
        }
    }
    else {
        running = params[RUN_BUTTON_PARAM].value + inputs[RUN_INPUT].value;
        lights[RUNNING_LIGHT].value = running ? 1.0 : 0.0;
        if(running == 0) {
            metro.reset();
        }
    }

    if(resetButtonTrig.process(params[RESET_BUTTON_PARAM].value) ||
        resetTrig.process(inputs[RESET_INPUT].value)) {
        grids.reset();
        metro.reset();
        resetLed.trigger();
        seqStep = 0;
        elapsedTicks = 0;
    }

    // Clock, tempo and swing
    tempoParam = params[TEMPO_PARAM].value;
    tempo = rescale(tempoParam, 0.01f, 1.f, 40.f, 240.f);
    char clockBPMChar[16];
    sprintf(clockBPMChar, "%.1f", tempo);
    clockBPM = clockBPMChar;
    swing = clamp(params[SWING_PARAM].value + inputs[SWING_CV].value / 10.f, 0.f, 0.9f);
    swingHighTempo = tempo / (1 - swing);
    swingLowTempo = tempo / (1 + swing);
    if(elapsedTicks < 6) {
        metro.setTempo(swingLowTempo);
    }
    else {
        metro.setTempo(swingHighTempo);
    }

    // External clock select
    if(tempoParam < 0.01) {
        clockBPM = "Ext.";
        if(initExtReset) {
            grids.reset();
            initExtReset = false;
        }
        numTicks = ticks_granularity[extClockResolution];
        extClock = true;
    }
    else {
        initExtReset = true;
        numTicks = ticks_granularity[2];
        extClock = false;
        metro.process();
    }

    mapX = params[MAPX_PARAM].value + (inputs[MAPX_CV].value / 10.f);
    mapX = clamp(mapX, 0.f, 1.f);
    mapY = params[MAPY_PARAM].value + (inputs[MAPY_CV].value / 10.f);
    mapY = clamp(mapY, 0.f, 1.f);
    BDFill = params[BD_DENS_PARAM].value + (inputs[BD_FILL_CV].value / 10.f);
    BDFill = clamp(BDFill, 0.f, 1.f);
    SNFill = params[SN_DENS_PARAM].value + (inputs[SN_FILL_CV].value / 10.f);
    SNFill = clamp(SNFill, 0.f, 1.f);
    HHFill = params[HH_DENS_PARAM].value + (inputs[HH_FILL_CV].value / 10.f);
    HHFill = clamp(HHFill, 0.f, 1.f);
    chaos = params[CHAOS_PARAM].value + (inputs[CHAOS_CV].value / 10.f);
    chaos = clamp(chaos, 0.f, 1.f);
    if(grids.getPatternMode() == PATTERN_EUCLIDEAN) {
        mapXText = std::to_string(((uint8_t)(mapX * 255.0) >> 3) + 1);
        mapYText = std::to_string(((uint8_t)(mapY * 255.0) >> 3) + 1);
        chaosText = std::to_string(((uint8_t)(chaos * 255.0) >> 3) + 1);
    }
    else {
        mapXText = "Map X";
        mapYText = "Map Y";
        chaosText = "Chaos";
    }

    if(running) {
        if(extClock) {
            if(clockTrig.process(inputs[CLOCK_INPUT].value)) {
                advStep = true;
            }
        }
        else if(metro.hasTicked()){
            advStep = true;
            elapsedTicks++;
            elapsedTicks %= 12;
        }
        else {
            advStep = false;
        }

        grids.setMapX((uint8_t)(mapX * 255.0));
        grids.setMapY((uint8_t)(mapY * 255.0));
        grids.setBDDensity((uint8_t)(BDFill * 255.0));
        grids.setSDDensity((uint8_t)(SNFill * 255.0));
        grids.setHHDensity((uint8_t)(HHFill * 255.0));
        grids.setRandomness((uint8_t)(chaos * 255.0));

        grids.setEuclideanLength(0, (uint8_t)(mapX * 255.0));
        grids.setEuclideanLength(1, (uint8_t)(mapY * 255.0));
        grids.setEuclideanLength(2, (uint8_t)(chaos * 255.0));
    }

    if(advStep) {
        grids.tick(numTicks);
        for(int i = 0; i < 6; ++i) {
            if(grids.getDrumState(i)) {
                drumTriggers[i].trigger();
                gateState[i] = true;
                if(i < 3) {
                    drumLED[i].trigger();
                }
            }
        }
        seqStep++;
        if(seqStep >= 32) {
            seqStep = 0;
        }
        advStep = false;
    }
    updateOutputs();
    updateUI();
}

void UGraph::updateUI() {
    resetLed.process();
    for(int i = 0; i < 3; ++i) {
        drumLED[i].process();
        if(drumLED[i].getState() == 1) {
            lights[drumLEDIds[i]].value = 1.0;
        }
        else {
            lights[drumLEDIds[i]].value = 0.0;
        }
    }


    if(resetLed.getState() == 1) {
        lights[RESET_LIGHT].value = 1.0;
    }
    else {
        lights[RESET_LIGHT].value = 0.0;
    }
}

void UGraph::updateOutputs() {
    if(triggerOutputMode == PULSE) {
        for(int i = 0; i < 6; ++i) {
            drumTriggers[i].process();
            if(drumTriggers[i].getState()) {
                outputs[outIDs[i]].value = 10;
            }
            else {
                outputs[outIDs[i]].value = 0;
            }
        }
    }
    else {
        for(int i = 0; i < 6; ++i) {
            if(metro.getElapsedTickTime() < 0.5 && gateState[i]) {
                outputs[outIDs[i]].value = 10;
            }
            else {
                outputs[outIDs[i]].value = 0;
                gateState[i] = false;
            }
        }
    }
}

void UGraph::onSampleRateChange() {
    metro.setSampleRate(engineGetSampleRate());
    for(int i = 0; i < 3; ++i) {
        drumLED[i].setSampleRate(engineGetSampleRate());
    }
    resetLed.setSampleRate(engineGetSampleRate());
    for(int i = 0; i < 6; ++i) {
        drumTriggers[i].setSampleRate(engineGetSampleRate());
    }
}

// The widget

struct PanelBorder : TransparentWidget {
	void draw(NVGcontext *vg) override {
		NVGcolor borderColor = nvgRGBAf(0.5, 0.5, 0.5, 0.5);
		nvgBeginPath(vg);
		nvgRect(vg, 0.5, 0.5, box.size.x - 1.0, box.size.y - 1.0);
		nvgStrokeColor(vg, borderColor);
		nvgStrokeWidth(vg, 1.0);
		nvgStroke(vg);
	}
};

struct UGraphDynamicText : TransparentWidget {
    std::string oldText;
    std::string* pText;
    std::shared_ptr<Font> font;
    int size;
    NVGcolor drawColour;
    int* visibility;
    DynamicViewMode viewMode;

    enum Colour {
        COLOUR_WHITE,
        COLOUR_BLACK
    };
    int* colourHandle;

    UGraphDynamicText() {
        font = Font::load(assetPlugin(plugin, "res/din1451alt.ttf"));
        size = 16;
        visibility = nullptr;
        pText = nullptr;
        viewMode = ACTIVE_HIGH_VIEW;
    }

    void draw(NVGcontext* vg) {
        nvgFontSize(vg, size);
        nvgFontFaceId(vg, font->handle);
        nvgTextLetterSpacing(vg, 0.f);
        Vec textPos = Vec(0.f, 0.f);
        if(colourHandle != nullptr) {
            switch(*colourHandle) {
                case COLOUR_BLACK : drawColour = nvgRGB(0x00,0x00,0x00); break;
                case COLOUR_WHITE : drawColour = nvgRGB(0xFF,0xFF,0xFF); break;
                default : drawColour = nvgRGB(0x00,0x00,0x00);
            }
        }
        else {
            drawColour = nvgRGB(0x00,0x00,0x00);
        }

        nvgFillColor(vg, drawColour);
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
        if(pText != nullptr) {
            nvgText(vg, textPos.x, textPos.y, pText->c_str(), NULL);
        }
    }

    void step() {
        if(visibility != nullptr) {
            if(*visibility) {
                visible = true;
            }
            else {
                visible = false;
            }
            if(viewMode == ACTIVE_LOW_VIEW) {
                visible = !visible;
            }
        }
        else {
            visible = true;
        }
    }
};

UGraphDynamicText* createUGraphDynamicText(const Vec& pos, int size, int* colourHandle, std::string* pText,
                                           int* visibilityHandle, DynamicViewMode viewMode) {
    UGraphDynamicText* dynText = new UGraphDynamicText();
    dynText->size = size;
    dynText->colourHandle = colourHandle;
    dynText->pText = pText;
    dynText->box.pos = pos;
    dynText->box.size = Vec(82,14);
    dynText->visibility = visibilityHandle;
    dynText->viewMode = viewMode;
    return dynText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Context Menu ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

struct UGraphWidget : ModuleWidget {
    UGraphWidget(UGraph *module);
    void appendContextMenu(Menu* menu) override;
};

UGraphWidget::UGraphWidget(UGraph *module) : ModuleWidget(module){
    {
        DynamicPanelWidget *panel = new DynamicPanelWidget();
        panel->addPanel(SVG::load(assetPlugin(plugin, "res/UGraphPanel.svg")));
        panel->addPanel(SVG::load(assetPlugin(plugin, "res/TopographPanelWhite.svg")));
        box.size = panel->box.size;
        panel->mode = &module->panelStyle;
        addChild(panel);
    }
    addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addChild(createUGraphDynamicText(Vec(54, 66.75), 14, &module->panelStyle, &module->clockBPM, nullptr, ACTIVE_HIGH_VIEW));
    addChild(createUGraphDynamicText(Vec(53, 147), 14, &module->panelStyle, &module->mapXText, nullptr, ACTIVE_HIGH_VIEW));
    addChild(createUGraphDynamicText(Vec(89, 147), 14, &module->panelStyle, &module->mapYText, nullptr, ACTIVE_HIGH_VIEW));
    addChild(createUGraphDynamicText(Vec(125, 147), 14, &module->panelStyle, &module->chaosText, nullptr, ACTIVE_HIGH_VIEW));

    addParam(ParamWidget::create<RoganMedBlue>(Vec(36.5, 30.15), module, UGraph::TEMPO_PARAM, 0.0, 1.0, 0.406));
    addParam(ParamWidget::create<RoganSmallWhite>(Vec(43.5, 125), module, UGraph::MAPX_PARAM, 0.0, 1.0, 0.0));
    addParam(ParamWidget::create<RoganSmallWhite>(Vec(79.5, 125), module, UGraph::MAPY_PARAM, 0.0, 1.0, 0.0));
    addParam(ParamWidget::create<RoganSmallWhite>(Vec(115.5, 125), module, UGraph::CHAOS_PARAM, 0.0, 1.0, 0.0));
    addParam(ParamWidget::create<RoganSmallBrightRed>(Vec(43.5, 195.65), module, UGraph::BD_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(ParamWidget::create<RoganSmallOrange>(Vec(79.5, 195.65), module, UGraph::SN_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(ParamWidget::create<RoganSmallYellow>(Vec(115.5, 195.65), module, UGraph::HH_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(ParamWidget::create<RoganMedWhite>(Vec(108.5, 30.15), module, UGraph::SWING_PARAM, 0.0, 0.9, 0.0));

    addInput(Port::create<PJ301MPort>(Vec(5, 35.5), Port::INPUT, module, UGraph::CLOCK_INPUT));
    addInput(Port::create<PJ301MPort>(Vec(23.0, 83.5), Port::INPUT, module, UGraph::RESET_INPUT));
    addInput(Port::create<PJ301MPort>(Vec(41, 162), Port::INPUT, module, UGraph::MAPX_CV));
    addInput(Port::create<PJ301MPort>(Vec(77, 162), Port::INPUT, module, UGraph::MAPY_CV));
    addInput(Port::create<PJ301MPort>(Vec(113, 162), Port::INPUT, module, UGraph::CHAOS_CV));
    addInput(Port::create<PJ301MPort>(Vec(40.5, 241.5), Port::INPUT, module, UGraph::BD_FILL_CV));
    addInput(Port::create<PJ301MPort>(Vec(76.5, 241.5), Port::INPUT, module, UGraph::SN_FILL_CV));
    addInput(Port::create<PJ301MPort>(Vec(112.5, 241.5), Port::INPUT, module, UGraph::HH_FILL_CV));
    addInput(Port::create<PJ301MPort>(Vec(77, 35.5), Port::INPUT, module, UGraph::SWING_CV));
    addInput(Port::create<PJ301MPort>(Vec(95, 83.5), Port::INPUT, module, UGraph::RUN_INPUT));

    addOutput(Port::create<PJ3410Port>(Vec(38.2, 277.736), Port::OUTPUT, module, UGraph::BD_OUTPUT));
    addOutput(Port::create<PJ3410Port>(Vec(74.2, 277.736), Port::OUTPUT, module, UGraph::SN_OUTPUT));
    addOutput(Port::create<PJ3410Port>(Vec(110.2, 277.736), Port::OUTPUT, module, UGraph::HH_OUTPUT));
    addOutput(Port::create<PJ3410Port>(Vec(38.2, 313.736), Port::OUTPUT, module, UGraph::BD_ACC_OUTPUT));
    addOutput(Port::create<PJ3410Port>(Vec(74.2, 313.736), Port::OUTPUT, module, UGraph::SN_ACC_OUTPUT));
    addOutput(Port::create<PJ3410Port>(Vec(110.2, 313.736), Port::OUTPUT, module, UGraph::HH_ACC_OUTPUT));

    addChild(ModuleLightWidget::create<SmallLight<RedLight>>(Vec(49.6, 225), module, UGraph::BD_LIGHT));
    addChild(ModuleLightWidget::create<SmallLight<RedLight>>(Vec(85.6, 225), module, UGraph::SN_LIGHT));
    addChild(ModuleLightWidget::create<SmallLight<RedLight>>(Vec(121.6, 225), module, UGraph::HH_LIGHT));

    addParam(ParamWidget::create<LightLEDButton>(Vec(58, 88), module, UGraph::RESET_BUTTON_PARAM, 0.0, 1.0, 0.0));
    addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(60.4, 90.5), module, UGraph::RESET_LIGHT));
    addParam(ParamWidget::create<LightLEDButton>(Vec(130, 88), module, UGraph::RUN_BUTTON_PARAM, 0.0, 1.0, 0.0));
    addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(132.4, 90.5), module, UGraph::RUNNING_LIGHT));
}

struct UGraphPanelStyleItem : MenuItem {
    UGraph* module;
    int panelStyle;
    void onAction(EventAction &e) override {
        module->panelStyle = panelStyle;
    }
    void step() override {
        rightText = (module->panelStyle == panelStyle) ? "✔" : "";
        MenuItem::step();
    }
};

struct UGraphSequencerModeItem : MenuItem {
    UGraph* module;
    UGraph::SequencerMode sequencerMode;
    void onAction(EventAction &e) override {
        module->sequencerMode = sequencerMode;
        switch(sequencerMode) {
            case UGraph::HENRI:
                module->grids.setPatternMode(PATTERN_HENRI);
                break;
            case UGraph::OLIVIER:
                module->grids.setPatternMode(PATTERN_OLIVIER);
                break;
            case UGraph::EUCLIDEAN:
                module->grids.setPatternMode(PATTERN_EUCLIDEAN);
                break;
        }
    }
    void step() override {
        rightText = (module->sequencerMode == sequencerMode) ? "✔" : "";
        MenuItem::step();
    }
};

struct UGraphTriggerOutputModeItem : MenuItem {
    UGraph* module;
    UGraph::TriggerOutputMode triggerOutputMode;
    void onAction(EventAction &e) override {
        module->triggerOutputMode = triggerOutputMode;
    }
    void step() override {
        rightText = (module->triggerOutputMode == triggerOutputMode) ? "✔" : "";
        MenuItem::step();
    }
};

struct UGraphAccOutputModeItem : MenuItem {
    UGraph* module;
    UGraph::AccOutputMode accOutputMode;
    void onAction(EventAction &e) override {
        module->accOutputMode = accOutputMode;
        switch(accOutputMode) {
            case UGraph::INDIVIDUAL_ACCENTS:
                module->grids.setAccentAltMode(false);
                break;
            case UGraph::ACC_CLK_RST:
                module->grids.setAccentAltMode(true);
        }
    }
    void step() override {
        rightText = (module->accOutputMode == accOutputMode) ? "✔" : "";
        MenuItem::step();
    }
};

struct UGraphClockResolutionItem : MenuItem {
    UGraph* module;
    UGraph::ExtClockResolution extClockResolution;
    void onAction(EventAction &e) override {
        module->extClockResolution = extClockResolution;
        module->grids.reset();
    }
    void step() override {
        rightText = (module->extClockResolution == extClockResolution) ? "✔" : "";
        MenuItem::step();
    }
};

struct UGraphRunModeItem : MenuItem {
    UGraph* module;
    UGraph::RunMode runMode;
    void onAction(EventAction &e) override {
        module->runMode = runMode;
    }
    void step() override {
        rightText = (module->runMode == runMode) ? "✔" : "";
        MenuItem::step();
    }
};

void UGraphWidget::appendContextMenu(Menu *menu) {
    UGraph *module = dynamic_cast<UGraph*>(this->module);
    assert(module);

    // Panel style
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel style"));
    menu->addChild(construct<UGraphPanelStyleItem>(&MenuItem::text, "Dark", &UGraphPanelStyleItem::module,
                                                      module, &UGraphPanelStyleItem::panelStyle, 0));
    menu->addChild(construct<UGraphPanelStyleItem>(&MenuItem::text, "Light", &UGraphPanelStyleItem::module,
                                                      module, &UGraphPanelStyleItem::panelStyle, 1));

    // Sequencer Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Sequencer Mode"));
    menu->addChild(construct<UGraphSequencerModeItem>(&MenuItem::text, "Henri", &UGraphSequencerModeItem::module,
                                                         module, &UGraphSequencerModeItem::sequencerMode, UGraph::HENRI));
    menu->addChild(construct<UGraphSequencerModeItem>(&MenuItem::text, "Olivier", &UGraphSequencerModeItem::module,
                                                         module, &UGraphSequencerModeItem::sequencerMode, UGraph::OLIVIER));
    menu->addChild(construct<UGraphSequencerModeItem>(&MenuItem::text, "Euclidean", &UGraphSequencerModeItem::module,
                                                         module, &UGraphSequencerModeItem::sequencerMode, UGraph::EUCLIDEAN));

    // Trigger Output Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Trigger Output Mode"));
    menu->addChild(construct<UGraphTriggerOutputModeItem>(&MenuItem::text, "1ms Pulse", &UGraphTriggerOutputModeItem::module,
                                                             module, &UGraphTriggerOutputModeItem::triggerOutputMode, UGraph::PULSE));
    menu->addChild(construct<UGraphTriggerOutputModeItem>(&MenuItem::text, "Gate", &UGraphTriggerOutputModeItem::module,
                                                             module, &UGraphTriggerOutputModeItem::triggerOutputMode, UGraph::GATE));

    // Acc Output Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Accent Output Mode"));
    menu->addChild(construct<UGraphAccOutputModeItem>(&MenuItem::text, "Individual accents", &UGraphAccOutputModeItem::module,
                                                         module, &UGraphAccOutputModeItem::accOutputMode, UGraph::INDIVIDUAL_ACCENTS));
    menu->addChild(construct<UGraphAccOutputModeItem>(&MenuItem::text, "Accent / Clock / Reset", &UGraphAccOutputModeItem::module,
                                                         module, &UGraphAccOutputModeItem::accOutputMode, UGraph::ACC_CLK_RST));

    // External clock resolution
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Ext. Clock Resolution"));
    menu->addChild(construct<UGraphClockResolutionItem>(&MenuItem::text, "4 PPQN", &UGraphClockResolutionItem::module,
                                                           module, &UGraphClockResolutionItem::extClockResolution, UGraph::EXTCLOCK_RES_4_PPQN));
    menu->addChild(construct<UGraphClockResolutionItem>(&MenuItem::text, "8 PPQN", &UGraphClockResolutionItem::module,
                                                           module, &UGraphClockResolutionItem::extClockResolution, UGraph::EXTCLOCK_RES_8_PPQN));
    menu->addChild(construct<UGraphClockResolutionItem>(&MenuItem::text, "24 PPQN", &UGraphClockResolutionItem::module,
                                                           module, &UGraphClockResolutionItem::extClockResolution, UGraph::EXTCLOCK_RES_24_PPQN));

    // Acc Output Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Run Mode"));
    menu->addChild(construct<UGraphRunModeItem>(&MenuItem::text, "Toggle", &UGraphRunModeItem::module,
                                                   module, &UGraphRunModeItem::runMode, UGraph::RunMode::TOGGLE));
    menu->addChild(construct<UGraphRunModeItem>(&MenuItem::text, "Momentary", &UGraphRunModeItem::module,
                                                   module, &UGraphRunModeItem::runMode, UGraph::RunMode::MOMENTARY));
}

Model *modelUGraph = Model::create<UGraph, UGraphWidget>("Valley", "uGraph", "uGraph", SEQUENCER_TAG);
