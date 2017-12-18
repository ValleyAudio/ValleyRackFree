//
// Topograph.cpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 5/12/2017
//
// Topograph, a port of "Mutable Instruments Grids" for VCV Rack
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
#include "dsp/digital.hpp"
#include "../Common/Metronome.hpp"
#include "../Common/Oneshot.hpp"
#include "TopographPatternGenerator.hpp"

struct Topograph : Module {
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
    bool initExtReset = true;
    bool running = false;
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

    int panelStyle;

    Topograph() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        metro = Metronome(120, engineGetSampleRate(), 24.0, 0.0);
        numTicks = ticks_granularity[2];
        srand(time(NULL));
        BDLed = Oneshot(0.1, engineGetSampleRate());
        SNLed = Oneshot(0.1, engineGetSampleRate());
        HHLed = Oneshot(0.1, engineGetSampleRate());
        resetLed = Oneshot(0.1, engineGetSampleRate());
        clockTrig.setThresholds(0.25, 0.75);
        resetTrig.setThresholds(0.25, 0.75);
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
        json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
        return rootJ;
    }

    void fromJson(json_t *rootJ) override {
        json_t *sequencerModeJ = json_object_get(rootJ, "sequencerMode");
        if (sequencerModeJ) {
            sequencerMode = (Topograph::SequencerMode) json_integer_value(sequencerModeJ);
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
			triggerOutputMode = (Topograph::TriggerOutputMode) json_integer_value(triggerOutputModeJ);
		}

        json_t *accOutputModeJ = json_object_get(rootJ, "accOutputMode");
		if (accOutputModeJ) {
			accOutputMode = (Topograph::AccOutputMode) json_integer_value(accOutputModeJ);
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
			extClockResolution = (Topograph::ExtClockResolution) json_integer_value(extClockResolutionJ);
            grids.reset();
		}

        json_t *chaosKnobModeJ = json_object_get(rootJ, "chaosKnobMode");
		if (chaosKnobModeJ) {
			chaosKnobMode = (Topograph::ChaosKnobMode) json_integer_value(chaosKnobModeJ);
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

void Topograph::step() {
    if (runButtonTrig.process(params[RUN_BUTTON_PARAM].value)) {
        running = !running;
        lights[RUNNING_LIGHT].value = running ? 1.0 : 0.0;
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
    tempo = rescalef(tempoParam, 0.01, 1.0, 40.0, 240.0);
    swing = clampf(params[SWING_PARAM].value + inputs[SWING_CV].value / 10.0, 0.0, 0.9);
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

        mapX = params[MAPX_PARAM].value + (inputs[MAPX_CV].value / 10.0);
        mapX = clampf(mapX, 0.0, 1.0);
        mapY = params[MAPY_PARAM].value + (inputs[MAPY_CV].value / 10.0);
        mapY = clampf(mapY, 0.0, 1.0);
        BDFill = params[BD_DENS_PARAM].value + (inputs[BD_FILL_CV].value / 10.0);
        BDFill = clampf(BDFill, 0.0, 1.0);
        SNFill = params[SN_DENS_PARAM].value + (inputs[SN_FILL_CV].value / 10.0);
        SNFill = clampf(SNFill, 0.0, 1.0);
        HHFill = params[HH_DENS_PARAM].value + (inputs[HH_FILL_CV].value / 10.0);
        HHFill = clampf(HHFill, 0.0, 1.0);
        chaos = params[CHAOS_PARAM].value + (inputs[CHAOS_CV].value / 10.0);
        chaos = clampf(chaos, 0.0, 1.0);

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

void Topograph::updateUI() {

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

void Topograph::updateOutputs() {
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

void Topograph::onSampleRateChange() {
    metro.setSampleRate(engineGetSampleRate());
    for(int i = 0; i < 3; ++i) {
        drumLED[i].setSampleRate(engineGetSampleRate());
    }
    resetLed.setSampleRate(engineGetSampleRate());
    for(int i = 0; i < 6; ++i) {
        drumTriggers[i].setSampleRate(engineGetSampleRate());
    }
}

struct DynamicPanel : FramebufferWidget {
    int* mode;
    int oldMode;
    std::vector<std::shared_ptr<SVG>> panels;
    SVGWidget* panel;

    DynamicPanel() {
        oldMode = -1;
        panel = new SVGWidget();
        addChild(panel);
        addPanel(SVG::load(assetPlugin(plugin, "res/TopographPanel.svg")));
        addPanel(SVG::load(assetPlugin(plugin, "res/TopographPanelWhite.svg")));
    }

    void addPanel(std::shared_ptr<SVG> svg) {
        panels.push_back(svg);
        if(!panel->svg) {
            panel->setSVG(svg);
            box.size = panel->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);
        }
    }

    void step() override {
        if(mode && *mode != oldMode) {
            panel->setSVG(panels[*mode]);
            oldMode = *mode;
            dirty = true;
        }
    }
};

TopographWidget::TopographWidget() {
    Topograph *module = new Topograph();
    setModule(module);
    box.size = Vec(16 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        DynamicPanel *panel = new DynamicPanel();
        panel->box.size = box.size;
        panel->mode = &module->panelStyle;
        addChild(panel);
    }

    // Custom control graphics
    struct Rogan1PSBrightRed : Rogan {
        Rogan1PSBrightRed() {
            setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSBrightRed.svg")));
        }
    };

    struct Rogan1PSOrange : Rogan {
        Rogan1PSOrange() {
            setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSOrange.svg")));
        }
    };

    struct Rogan1PSYellow : Rogan {
        Rogan1PSYellow() {
            setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSYellow.svg")));
        }
    };

    struct LightLEDButton : SVGSwitch, MomentarySwitch {
        LightLEDButton() {
            addFrame(SVG::load(assetPlugin(plugin, "res/LightLEDButton.svg")));
        }
    };

    addChild(createScrew<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createScrew<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createScrew<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createScrew<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(createParam<LightLEDButton>(Vec(60, 114.5), module, Topograph::RESET_BUTTON_PARAM, 0.0, 1.0, 0.0));
    addChild(createLight<MediumLight<RedLight>>(Vec(64.4, 119), module, Topograph::RESET_LIGHT));
    addParam(createParam<LightLEDButton>(Vec(102, 114.5), module, Topograph::RUN_BUTTON_PARAM, 0.0, 1.0, 0.0));
    addChild(createLight<MediumLight<RedLight>>(Vec(106.4, 119), module, Topograph::RUNNING_LIGHT));

    addParam(createParam<Rogan1PSBlue>(Vec(49, 40.15), module, Topograph::TEMPO_PARAM, 0.0, 1.0, 0.4));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 166.15), module, Topograph::MAPX_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 226.15), module, Topograph::MAPY_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 286.15), module, Topograph::CHAOS_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Rogan1PSBrightRed>(Vec(121, 40.15), module, Topograph::BD_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(createParam<Rogan1PSOrange>(Vec(157, 103.15), module, Topograph::SN_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(createParam<Rogan1PSYellow>(Vec(193, 166.15), module, Topograph::HH_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(createParam<Rogan1PSWhite>(Vec(193, 40.15), module, Topograph::SWING_PARAM, 0.0, 0.9, 0.0));

    addInput(createInput<PJ301MPort>(Vec(15.5, 48.5), module, Topograph::CLOCK_INPUT));
    addInput(createInput<PJ301MPort>(Vec(15.5, 111.5), module, Topograph::RESET_INPUT));
    addInput(createInput<PJ301MPort>(Vec(15.5, 174.5), module, Topograph::MAPX_CV));
    addInput(createInput<PJ301MPort>(Vec(15.5, 234.5), module, Topograph::MAPY_CV));
    addInput(createInput<PJ301MPort>(Vec(15.5, 294.5), module, Topograph::CHAOS_CV));
    addInput(createInput<PJ301MPort>(Vec(129.5, 234.5), module, Topograph::BD_FILL_CV));
    addInput(createInput<PJ301MPort>(Vec(165.5, 234.5), module, Topograph::SN_FILL_CV));
    addInput(createInput<PJ301MPort>(Vec(201.5, 234.5), module, Topograph::HH_FILL_CV));
    addInput(createInput<PJ301MPort>(Vec(165.5, 48.5), module, Topograph::SWING_CV));

    addOutput(createOutput<PJ3410Port>(Vec(126.7, 270.736), module, Topograph::BD_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(162.7, 270.736), module, Topograph::SN_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(198.7, 270.736), module, Topograph::HH_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(126.7, 306.736), module, Topograph::BD_ACC_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(162.7, 306.736), module, Topograph::SN_ACC_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(198.7, 306.736), module, Topograph::HH_ACC_OUTPUT));

    addChild(createLight<SmallLight<RedLight>>(Vec(138.6, 218), module, Topograph::BD_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(174.6, 218), module, Topograph::SN_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(210.6, 218), module, Topograph::HH_LIGHT));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Context Menu ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

struct TopographPanelStyleItem : MenuItem {
    Topograph* topograph;
    int panelStyle;
    void onAction(EventAction &e) override {
        topograph->panelStyle = panelStyle;
    }
    void step() override {
        rightText = (topograph->panelStyle == panelStyle) ? "✔" : "";
    }
};

struct TopographSequencerModeItem : MenuItem {
    Topograph* topograph;
    Topograph::SequencerMode sequencerMode;
    void onAction(EventAction &e) override {
        topograph->sequencerMode = sequencerMode;
        switch(sequencerMode) {
            case Topograph::HENRI:
                topograph->grids.setPatternMode(PATTERN_HENRI);
                break;
            case Topograph::OLIVIER:
                topograph->grids.setPatternMode(PATTERN_OLIVIER);
                break;
            case Topograph::EUCLIDEAN:
                topograph->grids.setPatternMode(PATTERN_EUCLIDEAN);
                break;
        }
    }
    void step() override {
        rightText = (topograph->sequencerMode == sequencerMode) ? "✔" : "";
    }
};

struct TopographTriggerOutputModeItem : MenuItem {
    Topograph* topograph;
    Topograph::TriggerOutputMode triggerOutputMode;
    void onAction(EventAction &e) override {
        topograph->triggerOutputMode = triggerOutputMode;
    }
    void step() override {
        rightText = (topograph->triggerOutputMode == triggerOutputMode) ? "✔" : "";
    }
};

struct TopographAccOutputModeItem : MenuItem {
    Topograph* topograph;
    Topograph::AccOutputMode accOutputMode;
    void onAction(EventAction &e) override {
        topograph->accOutputMode = accOutputMode;
        switch(accOutputMode) {
            case Topograph::INDIVIDUAL_ACCENTS:
                topograph->grids.setAccentAltMode(false);
                break;
            case Topograph::ACC_CLK_RST:
                topograph->grids.setAccentAltMode(true);
        }
    }
    void step() override {
        rightText = (topograph->accOutputMode == accOutputMode) ? "✔" : "";
    }
};

struct TopographClockResolutionItem : MenuItem {
    Topograph* topograph;
    Topograph::ExtClockResolution extClockResolution;
    void onAction(EventAction &e) override {
        topograph->extClockResolution = extClockResolution;
        topograph->grids.reset();
    }
    void step() override {
        rightText = (topograph->extClockResolution == extClockResolution) ? "✔" : "";
    }
};

Menu* TopographWidget::createContextMenu() {
    Menu* menu = ModuleWidget::createContextMenu();
    Topograph *topograph = dynamic_cast<Topograph*>(module);
    assert(topograph);

    // Panel style
    MenuLabel *panelStyleSpacerLabel = new MenuLabel();
    menu->addChild(panelStyleSpacerLabel);
    MenuLabel *panelStyleLabel = new MenuLabel();
    panelStyleLabel->text = "Panel style";
    menu->addChild(panelStyleLabel);

    TopographPanelStyleItem *darkPanelStyleItem = new TopographPanelStyleItem();
    darkPanelStyleItem->text = "Dark";
    darkPanelStyleItem->topograph = topograph;
    darkPanelStyleItem->panelStyle = 0;
    menu->addChild(darkPanelStyleItem);

    TopographPanelStyleItem *lightPanelStyleItem = new TopographPanelStyleItem();
    lightPanelStyleItem->text = "Light";
    lightPanelStyleItem->topograph = topograph;
    lightPanelStyleItem->panelStyle = 1;
    menu->addChild(lightPanelStyleItem);

    // Sequencer Modes
    MenuLabel *sequencerModeSpacerLabel = new MenuLabel();
    menu->addChild(sequencerModeSpacerLabel);
    MenuLabel *sequencerModeLabel = new MenuLabel();
    sequencerModeLabel->text = "Sequencer Mode";
    menu->addChild(sequencerModeLabel);

    TopographSequencerModeItem *henriSeqModeItem = new TopographSequencerModeItem();
    henriSeqModeItem->text = "Henri";
    henriSeqModeItem->topograph = topograph;
    henriSeqModeItem->sequencerMode = Topograph::HENRI;
    menu->addChild(henriSeqModeItem);

    TopographSequencerModeItem *olivierSeqModeItem = new TopographSequencerModeItem();
    olivierSeqModeItem->text = "Olivier";
    olivierSeqModeItem->topograph = topograph;
    olivierSeqModeItem->sequencerMode = Topograph::OLIVIER;
    menu->addChild(olivierSeqModeItem);

    TopographSequencerModeItem *euclideanSeqModeItem = new TopographSequencerModeItem();
    euclideanSeqModeItem->text = "Euclidean";
    euclideanSeqModeItem->topograph = topograph;
    euclideanSeqModeItem->sequencerMode = Topograph::EUCLIDEAN;
    menu->addChild(euclideanSeqModeItem);

    // Trigger Output Modes
    MenuLabel *triggerOutputModeSpacerLabel = new MenuLabel();
    menu->addChild(triggerOutputModeSpacerLabel);
    MenuLabel *triggerOutputModeLabel = new MenuLabel();
    triggerOutputModeLabel->text = "Trigger Output Mode";
    menu->addChild(triggerOutputModeLabel);

    TopographTriggerOutputModeItem *triggerPulseItem = new TopographTriggerOutputModeItem();
    triggerPulseItem->text = "1ms Pulse";
    triggerPulseItem->topograph = topograph;
    triggerPulseItem->triggerOutputMode = Topograph::PULSE;
    menu->addChild(triggerPulseItem);

    TopographTriggerOutputModeItem *triggerGateItem = new TopographTriggerOutputModeItem();
    triggerGateItem->text = "Gate";
    triggerGateItem->topograph = topograph;
    triggerGateItem->triggerOutputMode = Topograph::GATE;
    menu->addChild(triggerGateItem);

    // Acc Output Modes
    MenuLabel *accModeSpacerLabel = new MenuLabel();
    menu->addChild(accModeSpacerLabel);
    MenuLabel *accOutputModeLabel = new MenuLabel();
    accOutputModeLabel->text = "Accent Output Mode";
    menu->addChild(accOutputModeLabel);

    TopographAccOutputModeItem *individualAccItem = new TopographAccOutputModeItem();
    individualAccItem->text = "Individual accents";
    individualAccItem->topograph = topograph;
    individualAccItem->accOutputMode = Topograph::INDIVIDUAL_ACCENTS;
    menu->addChild(individualAccItem);

    TopographAccOutputModeItem *accClkRstItem = new TopographAccOutputModeItem();
    accClkRstItem->text = "Accent / Clock / Reset";
    accClkRstItem->topograph = topograph;
    accClkRstItem->accOutputMode = Topograph::ACC_CLK_RST;
    menu->addChild(accClkRstItem);

    // External clock resolution
    MenuLabel *extClockResSpacerLabel = new MenuLabel();
    menu->addChild(extClockResSpacerLabel);
    MenuLabel *extClockResLabel = new MenuLabel();
    extClockResLabel->text = "Ext. Clock Resolution";
    menu->addChild(extClockResLabel);

    TopographClockResolutionItem *fourPPQNItem = new TopographClockResolutionItem();
    fourPPQNItem->text = "4 PPQN";
    fourPPQNItem->topograph = topograph;
    fourPPQNItem->extClockResolution = Topograph::EXTCLOCK_RES_4_PPQN;
    menu->addChild(fourPPQNItem);

    TopographClockResolutionItem *eightPPQNItem = new TopographClockResolutionItem();
    eightPPQNItem->text = "8 PPQN";
    eightPPQNItem->topograph = topograph;
    eightPPQNItem->extClockResolution = Topograph::EXTCLOCK_RES_8_PPQN;
    menu->addChild(eightPPQNItem);

    TopographClockResolutionItem *twentyFourPPQNItem = new TopographClockResolutionItem();
    twentyFourPPQNItem->text = "24 PPQN";
    twentyFourPPQNItem->topograph = topograph;
    twentyFourPPQNItem->extClockResolution = Topograph::EXTCLOCK_RES_24_PPQN;
    menu->addChild(twentyFourPPQNItem);

    return menu;
}
