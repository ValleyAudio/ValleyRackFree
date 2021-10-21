#include "Feline.hpp"

Feline::Feline() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Feline::CUTOFF_PARAM, 0.f, 10.f, 10.f, "Cutoff Frequency");
    configParam(Feline::RESONANCE_PARAM, 0.f, 10.f, 0.f, "Resonance");
    configParam(Feline::SPACING_PARAM, -1.f, 1.f, 0.f, "Cutoff Spacing");
    configParam(Feline::POLES_PARAM, 0.f, 1.f, 0.f, "Poles");
    configParam(Feline::TYPE_PARAM, 0.f, 1.f, 0.f, "Type");
    configParam(Feline::SPACING_POLARITY_PARAM, 0.f, 1.f, 0.f, "Spacing Target");
    configParam(Feline::DRIVE_PARAM, 0.f, 1.f, 0.f, "Input Drive");

    long p = 0;
    long i = 0;
    std::vector<std::string> cvNames = {"Cutoff", "Resonance", "Drive", "Spacing", "Spacing Target"};
    for(auto row = 0; row < 2; ++row) {
        for(auto col = 0; col < 5; ++col) {
            p = Feline::CV1_1_PARAM + row * 5 + col;
            i = Feline::CV1_1_INPUT + row * 5 + col;
            configParam(p, -1.f, 1.f, 0.f, cvNames[col] + " CV " + std::to_string(row + 1));
        }
    }

    panelStyle = 0;
    calcGTable(APP->engine->getSampleRate());
    filter.setSampleRate(APP->engine->getSampleRate());
    filter.setCutoff(_mm_set1_ps(10.f));
    filter.setQ(_mm_set1_ps(0.f));
    prevLeftCutoff = 10.f;
    prevRightCutoff = 10.f;
}

void Feline::step() {
    cutoff = params[CUTOFF_PARAM].getValue();
    cutoff += inputs[CV1_1_INPUT].getVoltage() * params[CV1_1_PARAM].getValue();
    cutoff += inputs[CV2_1_INPUT].getVoltage() * params[CV2_1_PARAM].getValue();

    resonance = params[RESONANCE_PARAM].getValue();
    resonance += inputs[CV1_2_INPUT].getVoltage() * params[CV1_2_PARAM].getValue();
    resonance += inputs[CV2_2_INPUT].getVoltage() * params[CV2_2_PARAM].getValue();

    spacing = params[SPACING_PARAM].getValue();
    spacing += inputs[CV1_4_INPUT].getVoltage() * params[CV1_4_PARAM].getValue() * 0.1f;
    spacing += inputs[CV2_4_INPUT].getVoltage() * params[CV2_4_PARAM].getValue() * 0.1f;
    spacing = clamp(spacing, -1.f, 1.f);

    leftCutoff = linterp(0.f, -spacing, params[SPACING_POLARITY_PARAM].getValue());
    leftCutoff += cutoff;
    rightCutoff = cutoff + spacing;

    filter.setMode(params[POLES_PARAM].getValue() + params[TYPE_PARAM].getValue() * 2);

    if(leftCutoff != prevLeftCutoff || rightCutoff != prevRightCutoff) {
        prevLeftCutoff = leftCutoff;
        prevRightCutoff = rightCutoff;
        filter.setCutoff(_mm_set_ps(0.f, 0.f, rightCutoff, leftCutoff));
    }

    filter.setQ(_mm_set1_ps(resonance));

    drive = inputs[CV1_3_INPUT].getVoltage() * 0.1f * params[CV1_3_PARAM].getValue();
    drive += inputs[CV2_3_INPUT].getVoltage() * 0.1f * params[CV2_3_PARAM].getValue();
    drive += params[DRIVE_PARAM].getValue();
    drive = clamp(drive, 0.f, 1.f);
    drive *= drive;
    drive = drive * 9.25f + 0.75f;

    input[0] = inputs[LEFT_INPUT].getVoltage();
    input[1] = inputs[RIGHT_INPUT].getVoltage();
    if(inputs[LEFT_INPUT].isConnected() == false && inputs[RIGHT_INPUT].isConnected() == true) {
        input[0] = inputs[RIGHT_INPUT].getVoltage();
    }
    else if(inputs[LEFT_INPUT].isConnected() == true && inputs[RIGHT_INPUT].isConnected() == false) {
        input[1] = inputs[LEFT_INPUT].getVoltage();
    }

    input[0] *= drive;
    input[1] *= drive;
    __input = _mm_set_ps(0.f, 0.f, input[1], input[0]);

    __output = filter.process(_mm_mul_ps(__input, _mm_set1_ps(0.5f)));
    _mm_storeu_ps(output, __output);
    outputs[LEFT_OUTPUT].setVoltage(output[0] * 5.f);
    outputs[RIGHT_OUTPUT].setVoltage(output[1] * 5.f);
    outputs[SUM_OUTPUT].setVoltage((output[0] + output[1]) * 2.5f);
}

void Feline::onSampleRateChange() {
    calcGTable(APP->engine->getSampleRate());
    filter.setSampleRate(APP->engine->getSampleRate());
}

json_t* Feline::dataToJson()  {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
    return rootJ;
}

void Feline::dataFromJson(json_t *rootJ) {
    json_t *panelStyleJ = json_object_get(rootJ, "panelStyle");
    panelStyle = json_integer_value(panelStyleJ);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void FelinePanelStyleItem::onAction(const event::Action &e) {
    module->panelStyle = panelStyle;
}

void FelinePanelStyleItem::step() {
    rightText = (module->panelStyle == panelStyle) ? "✔" : "";
    MenuItem::step();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

FelineWidget::FelineWidget(Feline* module) {
    setModule(module);

    darkPanel = new SvgPanel;
    darkPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/FelinePanelDark.svg")));
    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/FelinePanelLight.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }
    setPanel(darkPanel);

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Make jacks
    addInput(createInput<PJ301MDarkSmall>(leftInputPos, module, Feline::LEFT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(rightInputPos, module, Feline::RIGHT_INPUT));

    addOutput(createOutput<PJ301MDarkSmallOut>(leftOutputPos, module, Feline::LEFT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(rightOutputPos, module, Feline::RIGHT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(sumOutputPos, module, Feline::SUM_OUTPUT));

    // Make knobs
    addParam(createParam<OrangeSlider>(cutoffSliderPos, module, Feline::CUTOFF_PARAM));
    addParam(createParam<OrangeSlider>(resonanceSliderPos, module, Feline::RESONANCE_PARAM));
    addParam(createParam<BlueSlider>(spacingSliderPos, module, Feline::SPACING_PARAM));
    addParam(createParam<BlueSlider>(spacingPolSliderPos, module, Feline::SPACING_POLARITY_PARAM));
    addParam(createParam<CKSS>(polesPos, module, Feline::POLES_PARAM));
    addParam(createParam<CKSS>(typePos, module, Feline::TYPE_PARAM));
    addParam(createParam<RoganMedWhite>(driveKnobPos, module, Feline::DRIVE_PARAM));

    long p = 0;
    long i = 0;
    for(auto row = 0; row < 2; ++row) {
        for(auto col = 0; col < 5; ++col) {
            p = Feline::CV1_1_PARAM + row * 5 + col;
            i = Feline::CV1_1_INPUT + row * 5 + col;
            if(col < 2) {
                addParam(createParam<RoganSmallOrange>(Vec(cvKnobX[col], cvKnobY[row]), module, p));
            }
            else if(col > 2) {
                addParam(createParam<RoganSmallBlue>(Vec(cvKnobX[col], cvKnobY[row]), module, p));
            }
            else {
                addParam(createParam<RoganSmallWhite>(Vec(cvKnobX[col], cvKnobY[row]), module, p));
            }
            addInput(createInput<PJ301MDarkSmall>(Vec(cvJackX[col], cvJackY[row]), module, i));
        }
    }
}

void FelineWidget::appendContextMenu(Menu *menu) {
    Feline *module = dynamic_cast<Feline*>(this->module);
    assert(module);

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel style"));
    menu->addChild(construct<FelinePanelStyleItem>(&MenuItem::text, "Dark", &FelinePanelStyleItem::module,
                                                    module, &FelinePanelStyleItem::panelStyle, 0));
    menu->addChild(construct<FelinePanelStyleItem>(&MenuItem::text, "Light", &FelinePanelStyleItem::module,
                                                      module, &FelinePanelStyleItem::panelStyle, 1));
}

void FelineWidget::step() {
    if(module) {
        if(dynamic_cast<Feline*>(module)->panelStyle == 1) {
            darkPanel->visible = false;
            lightPanel->visible = true;
        }
        else {
            darkPanel->visible = true;
            lightPanel->visible = false;
        }
    }
    Widget::step();
}

Model *modelFeline = createModel<Feline, FelineWidget>("Feline");
