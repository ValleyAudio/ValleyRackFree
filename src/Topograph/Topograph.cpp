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

// TODO : Add ACC output options

#include "../Valley.hpp"
#include "dsp/digital.hpp"
#include "../Common/Metronome.hpp"
#include "../Common/Oneshot.hpp"
#include "patternGenerator.h"

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

    SchmittTrigger clockTrig;
    SchmittTrigger resetTrig;
    SchmittTrigger resetButtonTrig;
    SchmittTrigger runButtonTrig;
    Metronome metro;
    bool running = false;
    bool extClock = false;
    bool advStep = false;
    long seqStep = 0;

    float tempoParam = 40.0;
    float mapX = 0.0;
    float mapY = 0.0;
    float chaos = 0.0;
    float BDFill = 0.0;
    float SNFill = 0.0;
    float HHFill = 0.0;

    t_drumSettings drumSettings;
    uint8_t state = 0;

    // LED Triggers
    Oneshot BDLed;
    Oneshot SNLed;
    Oneshot HHLed;
    Oneshot resetLed;
    Oneshot runningLed;

    // Drum Triggers
    Oneshot BDTrig;
    Oneshot SNTrig;
    Oneshot HHTrig;
    Oneshot BDAccTrig;
    Oneshot SNAccTrig;
    Oneshot HHAccTrig;

	Topograph() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS){
        metro = Metronome(120, engineGetSampleRate(), 32.0);
        drumSettings.mapX = 0;
        drumSettings.mapY = 0;
        drumSettings.density[0] = 64;
        drumSettings.density[1] = 64;
        drumSettings.density[2] = 64;
        drumSettings.perturbation[0] = 0;
        drumSettings.perturbation[1] = 0;
        drumSettings.perturbation[2] = 0;

        srand(time(NULL));
        BDLed = Oneshot(0.1, engineGetSampleRate());
        SNLed = Oneshot(0.1, engineGetSampleRate());
        HHLed = Oneshot(0.1, engineGetSampleRate());
        resetLed = Oneshot(0.1, engineGetSampleRate());
        BDTrig = Oneshot(0.001, engineGetSampleRate());
        SNTrig = Oneshot(0.001, engineGetSampleRate());
        HHTrig = Oneshot(0.001, engineGetSampleRate());
        BDAccTrig = Oneshot(0.001, engineGetSampleRate());
        SNAccTrig = Oneshot(0.001, engineGetSampleRate());
        HHAccTrig = Oneshot(0.001, engineGetSampleRate());
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
        metro.reset();
        resetLed.trigger();
        seqStep = 0;
    }

    tempoParam = params[TEMPO_PARAM].value;
    metro.setTempo(rescalef(tempoParam, 0.01, 1.0, 40.0, 240.0));
    if(tempoParam < 0.01) {
        extClock = true;
    }
    else {
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

        drumSettings.mapX = (uint8_t)(mapX * 255.0);
        drumSettings.mapY = (uint8_t)(mapY * 255.0);
        drumSettings.density[0] = (uint8_t)(BDFill * 255.0);
        drumSettings.density[1] = (uint8_t)(SNFill * 255.0);
        drumSettings.density[2] = (uint8_t)(HHFill * 255.0);
        chaos = (uint8_t)(chaos * 127.0);
    }

    if(advStep) {
        state = getDrums(seqStep, &drumSettings, chaos);

        // Trigger Out
        if((state & 1) == 1) {
            BDTrig.trigger();
            BDLed.trigger();
        }

        if((state & 2) == 2) {
            SNTrig.trigger();
            SNLed.trigger();
        }

        if((state & 4) == 4) {
            HHLed.trigger();
            HHTrig.trigger();
        }

        // Accent Out
        if((state & 8) == 8) {
            BDAccTrig.trigger();
        }
        if((state & 16) == 16) {
            SNAccTrig.trigger();
        }

        if((state & 32) == 32) {
            HHAccTrig.trigger();
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
    BDLed.process();
    SNLed.process();
    HHLed.process();
    resetLed.process();

    if(BDLed.getState() == 1) {
        lights[BD_LIGHT].value = 1.0;
    }
    else {
        lights[BD_LIGHT].value = 0.0;
    }

    if(SNLed.getState() == 1) {
        lights[SN_LIGHT].value = 1.0;
    }
    else {
        lights[SN_LIGHT].value = 0.0;
    }

    if(HHLed.getState() == 1) {
        lights[HH_LIGHT].value = 1.0;
    }
    else {
        lights[HH_LIGHT].value = 0.0;
    }

    if(resetLed.getState() == 1) {
        lights[RESET_LIGHT].value = 1.0;
    }
    else {
        lights[RESET_LIGHT].value = 0.0;
    }
}

void Topograph::updateOutputs() {
    BDTrig.process();
    SNTrig.process();
    HHTrig.process();
    BDAccTrig.process();
    SNAccTrig.process();
    HHAccTrig.process();

    if(BDTrig.getState()) {
        outputs[BD_OUTPUT].value = 10;
    }
    else {
        outputs[BD_OUTPUT].value = 0;
    }

    if(SNTrig.getState()) {
        outputs[SN_OUTPUT].value = 10;
    }
    else {
        outputs[SN_OUTPUT].value = 0;
    }

    if(HHTrig.getState()) {
        outputs[HH_OUTPUT].value = 10;
    }
    else {
        outputs[HH_OUTPUT].value = 0;
    }

    if(BDAccTrig.getState()) {
        outputs[BD_ACC_OUTPUT].value = 10;
    }
    else {
        outputs[BD_ACC_OUTPUT].value = 0;
    }

    if(SNAccTrig.getState()) {
        outputs[SN_ACC_OUTPUT].value = 10;
    }
    else {
        outputs[SN_ACC_OUTPUT].value = 0;
    }

    if(HHAccTrig.getState()) {
        outputs[HH_ACC_OUTPUT].value = 10;
    }
    else {
        outputs[HH_ACC_OUTPUT].value = 0;
    }
}

void Topograph::onSampleRateChange() {
    metro.setSampleRate(engineGetSampleRate());
    BDLed.setSampleRate(engineGetSampleRate());
    SNLed.setSampleRate(engineGetSampleRate());
    HHLed.setSampleRate(engineGetSampleRate());
    resetLed.setSampleRate(engineGetSampleRate());
}

TopographWidget::TopographWidget() {
    Topograph *module = new Topograph();
    setModule(module);
    box.size = Vec(16 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/TopographPanel.svg")));
        addChild(panel);
    }

    // Custom Knobs
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

    addChild(createScrew<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createScrew<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createScrew<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createScrew<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(createParam<LEDButton>(Vec(60, 114.5), module, Topograph::RESET_BUTTON_PARAM, 0.0, 1.0, 0.0));
    addChild(createLight<MediumLight<RedLight>>(Vec(64.4, 119), module, Topograph::RESET_LIGHT));
    addParam(createParam<LEDButton>(Vec(102, 114.5), module, Topograph::RUN_BUTTON_PARAM, 0.0, 1.0, 0.0));
    addChild(createLight<MediumLight<RedLight>>(Vec(106.4, 119), module, Topograph::RUNNING_LIGHT));

    addParam(createParam<Rogan1PSBlue>(Vec(49, 40.15), module, Topograph::TEMPO_PARAM, 0.0, 1.0, 0.4));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 166.15), module, Topograph::MAPX_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 226.15), module, Topograph::MAPY_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 286.15), module, Topograph::CHAOS_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Rogan1PSBrightRed>(Vec(121, 40.15), module, Topograph::BD_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(createParam<Rogan1PSOrange>(Vec(157, 103.15), module, Topograph::SN_DENS_PARAM, 0.0, 1.0, 0.5));
    addParam(createParam<Rogan1PSYellow>(Vec(193, 166.15), module, Topograph::HH_DENS_PARAM, 0.0, 1.0, 0.5));

    addInput(createInput<PJ301MPort>(Vec(15.5, 48.5), module, Topograph::CLOCK_INPUT));
    addInput(createInput<PJ301MPort>(Vec(15.5, 111.5), module, Topograph::RESET_INPUT));
    addInput(createInput<PJ301MPort>(Vec(15.5, 174.5), module, Topograph::MAPX_CV));
    addInput(createInput<PJ301MPort>(Vec(15.5, 234.5), module, Topograph::MAPY_CV));
    addInput(createInput<PJ301MPort>(Vec(15.5, 294.5), module, Topograph::CHAOS_CV));
    addInput(createInput<PJ301MPort>(Vec(129.5, 234.5), module, Topograph::BD_FILL_CV));
    addInput(createInput<PJ301MPort>(Vec(165.5, 234.5), module, Topograph::SN_FILL_CV));
    addInput(createInput<PJ301MPort>(Vec(201.5, 234.5), module, Topograph::HH_FILL_CV));

    addOutput(createOutput<PJ3410Port>(Vec(126.7, 270.736), module, Topograph::BD_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(162.7, 270.736), module, Topograph::SN_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(198.7, 270.736), module, Topograph::HH_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(126.7, 306.736), module, Topograph::BD_ACC_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(162.7, 306.736), module, Topograph::SN_ACC_OUTPUT));
    addOutput(createOutput<PJ3410Port>(Vec(198.7, 306.736), module, Topograph::HH_ACC_OUTPUT));

    addChild(createLight<SmallLight<RedLight>>(Vec(138.824, 218), module, Topograph::BD_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(174.824, 218), module, Topograph::SN_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(210.824, 218), module, Topograph::HH_LIGHT));
}
