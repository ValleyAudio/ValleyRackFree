//
// Topograph.cpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 5/12/2017
//

#include "Topograph.hpp"

Topograph::Topograph() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Topograph::TEMPO_PARAM, 0.0, 1.0, 0.406, "Tempo", "BPM", 0.f, 202.020202, 37.979797);
    configParam(Topograph::MAPX_PARAM, 0.0, 1.0, 0.0, "Pattern Map X");
    configParam(Topograph::MAPY_PARAM, 0.0, 1.0, 0.0, "Pattern Map Y");
    configParam(Topograph::CHAOS_PARAM, 0.0, 1.0, 0.0, "Pattern Chaos");
    configParam(Topograph::BD_DENS_PARAM, 0.0, 1.0, 0.5, "Channel 1 Density");
    configParam(Topograph::SN_DENS_PARAM, 0.0, 1.0, 0.5, "Channel 2 Density");
    configParam(Topograph::HH_DENS_PARAM, 0.0, 1.0, 0.5, "Channel 3 Density");
    configParam(Topograph::SWING_PARAM, 0.0, 0.9, 0.0, "Swing");
    configParam(Topograph::RESET_BUTTON_PARAM, 0.0, 1.0, 0.0, "Reset");
    configParam(Topograph::RUN_BUTTON_PARAM, 0.0, 1.0, 0.0, "Run");

    metro = Metronome(120, APP->engine->getSampleRate(), 24.0, 0.0);
    numTicks = ticks_granularity[2];
    srand(time(NULL));
    BDLed = Oneshot(0.1, APP->engine->getSampleRate());
    SNLed = Oneshot(0.1, APP->engine->getSampleRate());
    HHLed = Oneshot(0.1, APP->engine->getSampleRate());
    resetLed = Oneshot(0.1, APP->engine->getSampleRate());

    for (int i = 0; i < 6; ++i) {
        drumTriggers[i] = Oneshot(0.001, APP->engine->getSampleRate());
        gateState[i] = false;
    }
    for (int i = 0; i < 3; ++i) {
        drumLED[i] = Oneshot(0.1, APP->engine->getSampleRate());
    }
    panelStyle = 0;
}

json_t* Topograph::dataToJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "sequencerMode", json_integer(sequencerMode));
    json_object_set_new(rootJ, "triggerOutputMode", json_integer(triggerOutputMode));
    json_object_set_new(rootJ, "accOutputMode", json_integer(accOutputMode));
    json_object_set_new(rootJ, "extClockResolution", json_integer(extClockResolution));
    json_object_set_new(rootJ, "chaosKnobMode", json_integer(chaosKnobMode));
    json_object_set_new(rootJ, "runMode", json_integer(runMode));
    json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
    json_object_set_new(rootJ, "running", json_integer(running));
    return rootJ;
}

void Topograph::dataFromJson(json_t* rootJ) {
    json_t *sequencerModeJ = json_object_get(rootJ, "sequencerMode");
    if (sequencerModeJ) {
        sequencerMode = (Topograph::SequencerMode) json_integer_value(sequencerModeJ);
        inEuclideanMode = 0;
        switch (sequencerMode) {
            case HENRI:
                grids.setPatternMode(PATTERN_HENRI);
                break;
            case ORIGINAL:
                grids.setPatternMode(PATTERN_ORIGINAL);
                break;
            case EUCLIDEAN:
                grids.setPatternMode(PATTERN_EUCLIDEAN);
                inEuclideanMode = 1;
                break;
        }
	}

    json_t* triggerOutputModeJ = json_object_get(rootJ, "triggerOutputMode");
	if (triggerOutputModeJ) {
		triggerOutputMode = (Topograph::TriggerOutputMode) json_integer_value(triggerOutputModeJ);
	}

    json_t* accOutputModeJ = json_object_get(rootJ, "accOutputMode");
	if (accOutputModeJ) {
		accOutputMode = (Topograph::AccOutputMode) json_integer_value(accOutputModeJ);
        switch (accOutputMode) {
            case INDIVIDUAL_ACCENTS:
                grids.setAccentAltMode(false);
                break;
            case ACC_CLK_RST:
                grids.setAccentAltMode(true);
        }
	}

    json_t* extClockResolutionJ = json_object_get(rootJ, "extClockResolution");
	if (extClockResolutionJ) {
		extClockResolution = (Topograph::ExtClockResolution) json_integer_value(extClockResolutionJ);
        grids.reset();
	}

    json_t* chaosKnobModeJ = json_object_get(rootJ, "chaosKnobMode");
	if (chaosKnobModeJ) {
		chaosKnobMode = (Topograph::ChaosKnobMode) json_integer_value(chaosKnobModeJ);
	}

    json_t* runModeJ = json_object_get(rootJ, "runMode");
	if (runModeJ) {
		runMode = (Topograph::RunMode) json_integer_value(runModeJ);
	}

    json_t* panelStyleJ = json_object_get(rootJ, "panelStyle");
    if (panelStyleJ) {
        panelStyle = (int)json_integer_value(panelStyleJ);
    }

    json_t* runningJ = json_object_get(rootJ, "running");
    if (runningJ) {
        running = (int)json_integer_value(runningJ);
    }
}

void Topograph::process(const ProcessArgs &args) {
    if (runMode == TOGGLE) {
        if (runButtonTrig.process(params[RUN_BUTTON_PARAM].getValue()) ||
            runInputTrig.process(inputs[RUN_INPUT].getVoltage())) {
            if (runMode == TOGGLE){
                running = !running;
            }
        }
    }
    else {
        running = params[RUN_BUTTON_PARAM].getValue() + inputs[RUN_INPUT].getVoltage();
        if (running == 0) {
            metro.reset();
        }
    }
    lights[RUNNING_LIGHT].value = running ? 1.0 : 0.0;

    if (resetButtonTrig.process(params[RESET_BUTTON_PARAM].getValue()) ||
        resetTrig.process(inputs[RESET_INPUT].getVoltage())) {
        grids.reset();
        metro.reset();
        resetLed.trigger();
        seqStep = 0;
        elapsedTicks = 0;
    }

    // Clock, tempo and swing
    tempoParam = params[TEMPO_PARAM].getValue();
    tempo = rescale(tempoParam, 0.01f, 1.f, 40.f, 240.f);
    swing = clamp(params[SWING_PARAM].getValue() + inputs[SWING_CV].getVoltage() / 10.f, 0.f, 0.9f);
    swingHighTempo = tempo / (1 - swing);
    swingLowTempo = tempo / (1 + swing);
    if (elapsedTicks < 6) {
        metro.setTempo(swingLowTempo);
    }
    else {
        metro.setTempo(swingHighTempo);
    }

    // External clock select
    if (tempoParam < 0.01) {
        if (initExtReset) {
            grids.reset();
            initExtReset = false;
        }
        numTicks = ticks_granularity[extClockResolution];
        inExternalClockMode = true;
    }
    else {
        initExtReset = true;
        numTicks = ticks_granularity[2];
        inExternalClockMode = false;
        metro.process();
    }

    mapX = params[MAPX_PARAM].getValue() + (inputs[MAPX_CV].getVoltage() / 10.f);
    mapX = clamp(mapX, 0.f, 1.f);
    mapY = params[MAPY_PARAM].getValue() + (inputs[MAPY_CV].getVoltage() / 10.f);
    mapY = clamp(mapY, 0.f, 1.f);
    BDFill = params[BD_DENS_PARAM].getValue() + (inputs[BD_FILL_CV].getVoltage() / 10.f);
    BDFill = clamp(BDFill, 0.f, 1.f);
    SNFill = params[SN_DENS_PARAM].getValue() + (inputs[SN_FILL_CV].getVoltage() / 10.f);
    SNFill = clamp(SNFill, 0.f, 1.f);
    HHFill = params[HH_DENS_PARAM].getValue() + (inputs[HH_FILL_CV].getVoltage() / 10.f);
    HHFill = clamp(HHFill, 0.f, 1.f);
    chaos = params[CHAOS_PARAM].getValue() + (inputs[CHAOS_CV].getVoltage() / 10.f);
    chaos = clamp(chaos, 0.f, 1.f);

    externalClockConnected = inputs[CLOCK_INPUT].isConnected();

    if (running) {
        if (inExternalClockMode) {
            if (clockTrig.process(inputs[CLOCK_INPUT].getVoltage())) {
                advStep = true;
            }
        }
        else if (metro.hasTicked()){
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

    if (advStep) {
        grids.tick(numTicks);
        for (int i = 0; i < 6; ++i) {
            if (grids.getDrumState(i)) {
                drumTriggers[i].trigger();
                gateState[i] = true;
                if (i < 3) {
                    drumLED[i].trigger();
                }
            }
        }
        seqStep++;
        if (seqStep >= 32) {
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
        if (drumLED[i].getState() == 1) {
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
                outputs[outIDs[i]].setVoltage(10);
            }
            else {
                outputs[outIDs[i]].setVoltage(0);
            }
        }
    }
    else if(inExternalClockMode && triggerOutputMode == GATE) {
        for(int i = 0; i < 6; ++i) {
            if(inputs[CLOCK_INPUT].getVoltage() > 0 && gateState[i]) {
                gateState[i] = false;
                outputs[outIDs[i]].setVoltage(10);
            }
            if(inputs[CLOCK_INPUT].getVoltage() <= 0) {
                outputs[outIDs[i]].setVoltage(0);
            }
        }
    }
    else {
        for(int i = 0; i < 6; ++i) {
            if(metro.getElapsedTickTime() < 0.5 && gateState[i]) {
                outputs[outIDs[i]].setVoltage(10);
            }
            else {
                outputs[outIDs[i]].setVoltage(0);
                gateState[i] = false;
            }
        }
    }
}

void Topograph::onSampleRateChange() {
    metro.setSampleRate(APP->engine->getSampleRate());
    for(int i = 0; i < 3; ++i) {
        drumLED[i].setSampleRate(APP->engine->getSampleRate());
    }
    resetLed.setSampleRate(APP->engine->getSampleRate());
    for(int i = 0; i < 6; ++i) {
        drumTriggers[i].setSampleRate(APP->engine->getSampleRate());
    }
}

void Topograph::onReset() {
    running = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Widget //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void TempoKnob::randomize() {
    if (randomizationAllowed) {
        //Knob::randomize();
    }
}

TopographWidget::TopographWidget(Topograph *module) {
    setModule(module);

    darkPanel = new SvgPanel;
    darkPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/TopographPanel.svg")));
    setPanel(darkPanel);
    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/TopographPanelWhite.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    std::string fontPath = "res/din1451alt.ttf";

    // Tempo text
    tempoText = new PlainText;
    tempoText->box.pos = Vec(69, 83);
    tempoText->size = 14;
    tempoText->fontPath = fontPath;
    tempoText->color = nvgRGB(0xFF, 0xFF, 0xFF);
    tempoText->text = "120";
    addChild(tempoText);

    // Map Text
    mapXText = new PlainText;
    mapXText->box.pos = Vec(27.1, 208.5);
    mapXText->size = 14;
    mapXText->fontPath = fontPath;
    mapXText->color = nvgRGB(0xFF, 0xFF, 0xFF);
    mapXText->text = "Map X";
    addChild(mapXText);

    mapYText = new PlainText;
    mapYText->box.pos = Vec(27.1, 268.5);
    mapYText->size = 14;
    mapYText->fontPath = fontPath;
    mapYText->color = nvgRGB(0xFF, 0xFF, 0xFF);
    mapYText->text = "Map Y";
    addChild(mapYText);

    // Chaos Text
    chaosText = new PlainText;
    chaosText->box.pos = Vec(27.1, 329);
    chaosText->size = 14;
    chaosText->fontPath = fontPath;
    chaosText->color = nvgRGB(0xFF, 0xFF, 0xFF);
    chaosText->text = "Chaos";
    addChild(chaosText);

    tempoKnob = createParam<TempoKnob>(Vec(49, 40.15), module, Topograph::TEMPO_PARAM);
    addParam(tempoKnob);
    addParam(createParam<Rogan1PSWhite>(Vec(49, 166.15), module, Topograph::MAPX_PARAM));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 226.15), module, Topograph::MAPY_PARAM));
    addParam(createParam<Rogan1PSWhite>(Vec(49, 286.15), module, Topograph::CHAOS_PARAM));
    addParam(createParam<Rogan1PSBrightRed>(Vec(121, 40.15), module, Topograph::BD_DENS_PARAM));
    addParam(createParam<Rogan1PSOrange>(Vec(157, 103.15), module, Topograph::SN_DENS_PARAM));
    addParam(createParam<Rogan1PSYellow>(Vec(193, 166.15), module, Topograph::HH_DENS_PARAM));
    addParam(createParam<Rogan1PSWhite>(Vec(193, 40.15), module, Topograph::SWING_PARAM));

    addInput(createInput<PJ301MDarkSmall>(Vec(17.0, 50.0), module, Topograph::CLOCK_INPUT));
    addInput(createInput<PJ301MDarkSmall>(Vec(17.0, 113.0), module, Topograph::RESET_INPUT));
    addInput(createInput<PJ301MDarkSmall>(Vec(17.0, 176.0), module, Topograph::MAPX_CV));
    addInput(createInput<PJ301MDarkSmall>(Vec(17.0, 236.0), module, Topograph::MAPY_CV));
    addInput(createInput<PJ301MDarkSmall>(Vec(17.0, 296.0), module, Topograph::CHAOS_CV));
    addInput(createInput<PJ301MDarkSmall>(Vec(131.0, 236.0), module, Topograph::BD_FILL_CV));
    addInput(createInput<PJ301MDarkSmall>(Vec(167.0, 236.0), module, Topograph::SN_FILL_CV));
    addInput(createInput<PJ301MDarkSmall>(Vec(203.0, 236.0), module, Topograph::HH_FILL_CV));
    addInput(createInput<PJ301MDarkSmall>(Vec(167.0, 50.0), module, Topograph::SWING_CV));
    addInput(createInput<PJ301MDarkSmall>(Vec(74.5, 113.0), module, Topograph::RUN_INPUT));

    addOutput(createOutput<PJ301MDarkSmallOut>(Vec(131.2, 272.536), module, Topograph::BD_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Vec(167.2, 272.536), module, Topograph::SN_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Vec(203.2, 272.536), module, Topograph::HH_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Vec(131.2, 308.536), module, Topograph::BD_ACC_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Vec(167.2, 308.536), module, Topograph::SN_ACC_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Vec(203.2, 308.536), module, Topograph::HH_ACC_OUTPUT));

    addChild(createLight<SmallLight<RedLight>>(Vec(138.6, 218), module, Topograph::BD_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(174.6, 218), module, Topograph::SN_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(210.6, 218), module, Topograph::HH_LIGHT));

    addParam(createParam<LightLEDButton>(Vec(55, 116), module, Topograph::RESET_BUTTON_PARAM));
    addChild(createLight<MediumLight<RedLight>>(Vec(57.5, 118.5), module, Topograph::RESET_LIGHT));
    addParam(createParam<LightLEDButton>(Vec(112, 116), module, Topograph::RUN_BUTTON_PARAM));
    addChild(createLight<MediumLight<RedLight>>(Vec(114.5, 118.5), module, Topograph::RUNNING_LIGHT));
}

struct TopographPanelStyleItem : MenuItem {
    Topograph* module;
    int panelStyle;
    NVGcolor textColour;
    void onAction(const event::Action &e) override {
        module->panelStyle = panelStyle;
    }
    void step() override {
        rightText = (module->panelStyle == panelStyle) ? "✔" : "";
        MenuItem::step();
    }
};

struct TopographSequencerModeItem : MenuItem {
    Topograph* module;
    Topograph::SequencerMode sequencerMode;
    void onAction(const event::Action &e) override {
        module->sequencerMode = sequencerMode;
        module->inEuclideanMode = 0;
        switch(sequencerMode) {
            case Topograph::HENRI:
                module->grids.setPatternMode(PATTERN_HENRI);
                break;
            case Topograph::ORIGINAL:
                module->grids.setPatternMode(PATTERN_ORIGINAL);
                break;
            case Topograph::EUCLIDEAN:
                module->grids.setPatternMode(PATTERN_EUCLIDEAN);
                module->inEuclideanMode = 1;
                break;
        }
    }
    void step() override {
        rightText = (module->sequencerMode == sequencerMode) ? "✔" : "";
        MenuItem::step();
    }
};

struct TopographTriggerOutputModeItem : MenuItem {
    Topograph* module;
    Topograph::TriggerOutputMode triggerOutputMode;
    void onAction(const event::Action &e) override {
        module->triggerOutputMode = triggerOutputMode;
    }
    void step() override {
        rightText = (module->triggerOutputMode == triggerOutputMode) ? "✔" : "";
        MenuItem::step();
    }
};

struct TopographAccOutputModeItem : MenuItem {
    Topograph* module;
    Topograph::AccOutputMode accOutputMode;
    void onAction(const event::Action &e) override {
        module->accOutputMode = accOutputMode;
        switch(accOutputMode) {
            case Topograph::INDIVIDUAL_ACCENTS:
                module->grids.setAccentAltMode(false);
                break;
            case Topograph::ACC_CLK_RST:
                module->grids.setAccentAltMode(true);
        }
    }
    void step() override {
        rightText = (module->accOutputMode == accOutputMode) ? "✔" : "";
        MenuItem::step();
    }
};

struct TopographClockResolutionItem : MenuItem {
    Topograph* module;
    Topograph::ExtClockResolution extClockResolution;
    void onAction(const event::Action &e) override {
        module->extClockResolution = extClockResolution;
        module->grids.reset();
    }
    void step() override {
        rightText = (module->extClockResolution == extClockResolution) ? "✔" : "";
        MenuItem::step();
    }
};

struct TopographRunModeItem : MenuItem {
    Topograph* module;
    Topograph::RunMode runMode;
    void onAction(const event::Action &e) override {
        module->runMode = runMode;
    }
    void step() override {
        rightText = (module->runMode == runMode) ? "✔" : "";
        MenuItem::step();
    }
};

void TopographWidget::appendContextMenu(Menu *menu) {
    Topograph *module = dynamic_cast<Topograph*>(this->module);
    assert(module);

    // Panel style
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel style"));

    TopographPanelStyleItem* darkPanelStyleItem = new TopographPanelStyleItem;
    darkPanelStyleItem->text = "Dark";
    darkPanelStyleItem->module = module;
    darkPanelStyleItem->panelStyle = 0;
    menu->addChild(darkPanelStyleItem);

    TopographPanelStyleItem* lightPanelStyleItem = new TopographPanelStyleItem;
    lightPanelStyleItem->text = "Light";
    lightPanelStyleItem->module = module;
    lightPanelStyleItem->panelStyle = 1;
    menu->addChild(lightPanelStyleItem);

    // Sequencer Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Sequencer Mode"));
    menu->addChild(construct<TopographSequencerModeItem>(&MenuItem::text, "Henri", &TopographSequencerModeItem::module,
                                                         module, &TopographSequencerModeItem::sequencerMode, Topograph::HENRI));
    menu->addChild(construct<TopographSequencerModeItem>(&MenuItem::text, "Original", &TopographSequencerModeItem::module,
                                                         module, &TopographSequencerModeItem::sequencerMode, Topograph::ORIGINAL));
    menu->addChild(construct<TopographSequencerModeItem>(&MenuItem::text, "Euclidean", &TopographSequencerModeItem::module,
                                                         module, &TopographSequencerModeItem::sequencerMode, Topograph::EUCLIDEAN));

    // Trigger Output Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Trigger Output Mode"));
    menu->addChild(construct<TopographTriggerOutputModeItem>(&MenuItem::text, "1ms Pulse", &TopographTriggerOutputModeItem::module,
                                                             module, &TopographTriggerOutputModeItem::triggerOutputMode, Topograph::PULSE));
    menu->addChild(construct<TopographTriggerOutputModeItem>(&MenuItem::text, "Gate", &TopographTriggerOutputModeItem::module,
                                                             module, &TopographTriggerOutputModeItem::triggerOutputMode, Topograph::GATE));

    // Acc Output Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Accent Output Mode"));
    menu->addChild(construct<TopographAccOutputModeItem>(&MenuItem::text, "Individual accents", &TopographAccOutputModeItem::module,
                                                         module, &TopographAccOutputModeItem::accOutputMode, Topograph::INDIVIDUAL_ACCENTS));
    menu->addChild(construct<TopographAccOutputModeItem>(&MenuItem::text, "Accent / Clock / Reset", &TopographAccOutputModeItem::module,
                                                         module, &TopographAccOutputModeItem::accOutputMode, Topograph::ACC_CLK_RST));

    // External clock resolution
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Ext. Clock Resolution"));
    menu->addChild(construct<TopographClockResolutionItem>(&MenuItem::text, "4 PPQN", &TopographClockResolutionItem::module,
                                                           module, &TopographClockResolutionItem::extClockResolution, Topograph::EXTCLOCK_RES_4_PPQN));
    menu->addChild(construct<TopographClockResolutionItem>(&MenuItem::text, "8 PPQN", &TopographClockResolutionItem::module,
                                                           module, &TopographClockResolutionItem::extClockResolution, Topograph::EXTCLOCK_RES_8_PPQN));
    menu->addChild(construct<TopographClockResolutionItem>(&MenuItem::text, "24 PPQN", &TopographClockResolutionItem::module,
                                                           module, &TopographClockResolutionItem::extClockResolution, Topograph::EXTCLOCK_RES_24_PPQN));

    // Acc Output Modes
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Run Mode"));
    menu->addChild(construct<TopographRunModeItem>(&MenuItem::text, "Toggle", &TopographRunModeItem::module,
                                                   module, &TopographRunModeItem::runMode, Topograph::RunMode::TOGGLE));
    menu->addChild(construct<TopographRunModeItem>(&MenuItem::text, "Momentary", &TopographRunModeItem::module,
                                                   module, &TopographRunModeItem::runMode, Topograph::RunMode::MOMENTARY));
}

void TopographWidget::step() {
    if (!module) {
        Widget::step();
        return;
    }
    Topograph* tgraph = dynamic_cast<Topograph*>(module);

    if (!isInExtClockMode && tgraph->externalClockConnected) {
        isInExtClockMode = true;
        tempoKnob->randomizationAllowed = false;
        //tempoKnob->paramQuantity->setValue(0.f);
        APP->engine->setParamValue(module, Topograph::TEMPO_PARAM, 0.f);
    }
    else if (isInExtClockMode && !tgraph->externalClockConnected) {
        isInExtClockMode = false;
        tempoKnob->randomizationAllowed = true;
    }

    // Panel text
    auto floatToTempoText = [](float a){
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << a;
        if(a >= 40.0) {
            return stream.str();
        }
        std::string out = "Ext.";
        return out;
    };

    auto floatToEuclideanText = [](float a) -> std::string {
        return std::to_string(((uint8_t)(a * 255.0) >> 3) + 1);
    };

    tempoText->text = floatToTempoText(tgraph->tempo);

    if (tgraph->panelStyle == 1) {
        darkPanel->visible = false;
        lightPanel->visible = true;
        tempoText->color = lightPanelTextColour;
        mapXText->color = lightPanelTextColour;
        mapYText->color = lightPanelTextColour;
        chaosText->color = lightPanelTextColour;
    }
    else {
        darkPanel->visible = true;
        lightPanel->visible = false;
        tempoText->color = darkPanelTextColour;
        mapXText->color = darkPanelTextColour;
        mapYText->color = darkPanelTextColour;
        chaosText->color = darkPanelTextColour;
    }

    if (tgraph->inEuclideanMode) {
        mapXText->text = "Len: " + floatToEuclideanText(tgraph->mapX);
        mapYText->text = "Len: " + floatToEuclideanText(tgraph->mapY);
        chaosText->text = "Len: " + floatToEuclideanText(tgraph->chaos);
    }
    else {
        mapXText->text = "Map X";
        mapYText->text = "Map Y";
        chaosText->text = "Chaos";
    }

    Widget::step();
}

Model *modelTopograph = createModel<Topograph, TopographWidget>("Topograph");
