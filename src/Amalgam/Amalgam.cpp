#include "Amalgam.hpp"

Amalgam::Amalgam() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Amalgam::X_GAIN, 0.f, 4.f, 1.f, "X Gain");
    configParam(Amalgam::Y_GAIN, 0.f, 4.f, 1.f, "Y Gain");
    configParam(Amalgam::A_PARAM, 0.0f, 1.f, 0.f, "Parameter A");
    configParam(Amalgam::B_PARAM, 0.0f, 1.f, 0.f, "Parameter B");
    configParam(Amalgam::TYPE_PARAM, 0.0f, VecAmalgam::NUM_MODES - 0.9f, 0.f, "Combination Type");
    configParam(Amalgam::TYPE_CV1_PARAM, -1.0f, 1.f, 0.0f, "Combination Type CV 1 Depth");
    configParam(Amalgam::TYPE_CV2_PARAM, -1.0f, 1.f, 0.0f, "Combination Type CV 2 Depth");
    configParam(Amalgam::X_GAIN_CV_PARAM, -1.0f, 1.f, 0.0f, "X Gain CV Depth");
    configParam(Amalgam::Y_GAIN_CV_PARAM, -1.0f, 1.f, 0.0f, "Y Gain CV Depth");
    configParam(Amalgam::PARAM_A_CV1_PARAM, -1.0f, 1.f, 0.0f, "Param. A CV 1 Depth");
    configParam(Amalgam::PARAM_A_CV2_PARAM, -1.0f, 1.f, 0.0f, "Param. A CV 1 Depth");
    configParam(Amalgam::PARAM_B_CV1_PARAM, -1.0f, 1.f, 0.0f, "Param. B CV 1 Depth");
    configParam(Amalgam::PARAM_B_CV2_PARAM, -1.0f, 1.f, 0.0f, "Param. B CV 1 Depth");
    configParam(Amalgam::DC_COUPLE_PARAM, 0.f, 1.f, 0.f, "DC Couple");

    amalgamType = 0;
    __zeros = _mm_set1_ps(0.f);
    __fives = _mm_set1_ps(5.f);
    __halfs = _mm_set1_ps(0.5f);

    __x = __zeros;
    __y = __zeros;
    __z = __zeros;
    __xyAnd = __zeros;
    __xyXor = __zeros;

    xGain = 0.f;
    yGain = 0.f;

    float newSampleRate = APP->engine->getSampleRate();
    vecAmalgam.setSampleRate(newSampleRate);
    xyAndDCFilter.setSampleRate(newSampleRate);
    xyXorDCFilter.setSampleRate(newSampleRate);
    zOutDCFilter.setSampleRate(newSampleRate);
    zPls1DCFilter.setSampleRate(newSampleRate);
    zPls2DCFilter.setSampleRate(newSampleRate);
    zAndDCFilter.setSampleRate(newSampleRate);
    zXorDCFilter.setSampleRate(newSampleRate);
    xyAndDCFilter.setCutoffFreq(1.f);
    xyXorDCFilter.setCutoffFreq(1.f);
    zOutDCFilter.setCutoffFreq(1.f);
    zPls1DCFilter.setCutoffFreq(1.f);
    zPls2DCFilter.setCutoffFreq(1.f);
    zAndDCFilter.setCutoffFreq(1.f);
    zXorDCFilter.setCutoffFreq(1.f);
}

void Amalgam::process(const ProcessArgs &args) {
    amalgamType = inputs[TYPE_CV1_INPUT].getVoltage() * params[TYPE_CV1_PARAM].getValue();
    amalgamType += inputs[TYPE_CV2_INPUT].getVoltage() * params[TYPE_CV2_PARAM].getValue();
    amalgamType = rescale(amalgamType, 0.f, 10.f, 0.f, (float)VecAmalgam::NUM_MODES);
    amalgamType += params[TYPE_PARAM].getValue();
    iAmalgamType = (int)clamp(amalgamType, 0.f, (float)VecAmalgam::NUM_MODES - 1);
    vecAmalgam.setMode(iAmalgamType);

    paramA = params[A_PARAM].getValue();
    paramA += inputs[PARAM_A_CV1_INPUT].getVoltage() * 0.1f * params[PARAM_A_CV1_PARAM].getValue();
    paramA += inputs[PARAM_A_CV2_INPUT].getVoltage() * 0.1f * params[PARAM_A_CV2_PARAM].getValue();
    paramA = clamp(paramA, 0.f, 1.f);

    paramB = params[B_PARAM].getValue();
    paramB += inputs[PARAM_B_CV1_INPUT].getVoltage() * 0.1f * params[PARAM_B_CV1_PARAM].getValue();
    paramB += inputs[PARAM_B_CV2_INPUT].getVoltage() * 0.1f * params[PARAM_B_CV2_PARAM].getValue();
    paramB = clamp(paramB, 0.f, 1.f);

    /*dcCoupleButtonState = params[DC_COUPLE_PARAM].getValue() > 0.5f ? true : false;
    if(dcCoupleButtonState && !prevDcCoupleButtonState) {
        dcCoupled = dcCoupled ? false : true;
    }
    prevDcCoupleButtonState = dcCoupleButtonState;*/
    dcCoupled = params[DC_COUPLE_PARAM].getValue() > 0.5f ? true : false;
    lights[DC_COUPLE_LIGHT].value = dcCoupled ? 10.f : 0.f;

    xyAndDCFilter.setBypass(dcCoupled);
    xyXorDCFilter.setBypass(dcCoupled);
    zOutDCFilter.setBypass(dcCoupled);
    zPls1DCFilter.setBypass(dcCoupled);
    zPls2DCFilter.setBypass(dcCoupled);

    xIn[0] = inputs[X_LEFT_INPUT].getVoltage();
    xIn[1] = inputs[X_RIGHT_INPUT].getVoltage();
    yIn[0] = inputs[Y_LEFT_INPUT].getVoltage();
    yIn[1] = inputs[Y_RIGHT_INPUT].getVoltage();

    // Normal route left and right inputs if used in mono
    if(inputs[X_LEFT_INPUT].isConnected() && !inputs[X_RIGHT_INPUT].isConnected()) {
        xIn[1] = xIn[0];
    }
    else if(!inputs[X_LEFT_INPUT].isConnected() && inputs[X_RIGHT_INPUT].isConnected()) {
        xIn[0] = xIn[1];
    }

    if(inputs[Y_LEFT_INPUT].isConnected() && !inputs[Y_RIGHT_INPUT].isConnected()) {
        yIn[1] = yIn[0];
    }
    else if(!inputs[Y_LEFT_INPUT].isConnected() && inputs[Y_RIGHT_INPUT].isConnected()) {
        yIn[0] = yIn[1];
    }

    xGain = inputs[X_GAIN_CV_INPUT].getVoltage() * params[X_GAIN_CV_PARAM].getValue() * 0.4f;
    xGain += params[X_GAIN].getValue();
    xGain = clamp(xGain, 0.f, 4.f);

    yGain = inputs[Y_GAIN_CV_INPUT].getVoltage() * params[Y_GAIN_CV_PARAM].getValue() * 0.4;
    yGain += params[Y_GAIN].getValue();
    yGain = clamp(yGain, 0.f, 4.f);

    __x = _mm_set_ps(xIn[0], xIn[0], xIn[1], xIn[1]);
    __y = _mm_set_ps(yIn[0], yIn[0], yIn[1], yIn[1]);
    __x = _mm_mul_ps(__x, _mm_set1_ps(0.15f));
    __y = _mm_mul_ps(__y, _mm_set1_ps(0.15f));
    __x = _mm_mul_ps(vecDriveSignal(__x, _mm_set1_ps(xGain)), _mm_set1_ps(1.333333f));
    __y = _mm_mul_ps(vecDriveSignal(__y, _mm_set1_ps(yGain)), _mm_set1_ps(1.333333f));

    // AND and XOR
    __xyAnd = _mm_switch_ps(__zeros, __fives, _mm_and_ps(_mm_cmpgt_ps(__x, __zeros), _mm_cmpgt_ps(__y, __zeros)));
    __xyXor = _mm_switch_ps(__zeros, __fives, _mm_xor_ps(_mm_cmpgt_ps(__x, __zeros), _mm_cmpgt_ps(__y, __zeros)));
    __xyAnd = xyAndDCFilter.process(__xyAnd);
    __xyXor = xyXorDCFilter.process(__xyXor);

    __z = vecAmalgam.process(__x, __y, paramA, paramB);

    __zPls1Mask = _mm_cmpgt_ps(__z, __zeros);
    __zPls1 = _mm_and_ps(__fives, _mm_and_ps(_mm_cmpgt_ps(__z, __zeros), _mm_cmple_ps(__z, __halfs)));
    __zPls2 = _mm_and_ps(__fives, _mm_or_ps(_mm_and_ps(_mm_cmpgt_ps(__z, __zeros), _mm_cmple_ps(__z, _mm_set1_ps(0.25f))),
                                            _mm_and_ps(_mm_cmpgt_ps(__z, __halfs), _mm_cmple_ps(__z, _mm_set1_ps(0.99f)))));

    __zPls1 = zPls1DCFilter.process(__zPls1);
    __zPls2 = zPls2DCFilter.process(__zPls2);

    _mm_storeu_ps(zOut, __z);
    _mm_storeu_ps(zPls1, __zPls1);
    _mm_storeu_ps(zPls2, __zPls2);
    _mm_storeu_ps(andOut, __xyAnd);
    _mm_storeu_ps(xorOut, __xyXor);

    zAnd = ((zOut[0] > 0.f) & (zOut[2] > 0.f)) ? 5.f : 0.f;
    zXor = ((zOut[0] > 0.f) ^ (zOut[2] > 0.f)) ? 5.f : 0.f;
    zAndDCFilter.input = zAnd;
    zXorDCFilter.input = zXor;
    zAndDCFilter.process();
    zXorDCFilter.process();
    zAnd = dcCoupled ? zAnd : zAndDCFilter.output;
    zXor = dcCoupled ? zXor : zXorDCFilter.output;

    __z = zOutDCFilter.process(__z);
    _mm_storeu_ps(zOut, __z);

    outputs[X_Y_LEFT_AND_OUTPUT].setVoltage(andOut[2]);
    outputs[X_Y_RIGHT_AND_OUTPUT].setVoltage(andOut[0]);
    outputs[X_Y_LEFT_XOR_OUTPUT].setVoltage(xorOut[2]);
    outputs[X_Y_RIGHT_XOR_OUTPUT].setVoltage(xorOut[0]);
    outputs[Z_LEFT_OUTPUT].setVoltage(tanhDriveSignal(zOut[0], 0.66f) * 5.5f);
    outputs[Z_RIGHT_OUTPUT].setVoltage(tanhDriveSignal(zOut[2], 0.66f) * 5.5f);
    outputs[Z_AND_OUTPUT].setVoltage(zAnd);
    outputs[Z_XOR_OUTPUT].setVoltage(zXor);
    outputs[Z_LEFT_PULSE_1_OUTPUT].setVoltage(zPls1[2]);
    outputs[Z_RIGHT_PULSE_1_OUTPUT].setVoltage(zPls1[0]);
    outputs[Z_LEFT_PULSE_2_OUTPUT].setVoltage(zPls2[2]);
    outputs[Z_RIGHT_PULSE_2_OUTPUT].setVoltage(zPls2[0]);
}

void Amalgam::onSampleRateChange() {
    float newSampleRate = APP->engine->getSampleRate();
    vecAmalgam.setSampleRate(newSampleRate);
    xyAndDCFilter.setSampleRate(newSampleRate);
    xyXorDCFilter.setSampleRate(newSampleRate);
    zOutDCFilter.setSampleRate(newSampleRate);
    zPls1DCFilter.setSampleRate(newSampleRate);
    zPls2DCFilter.setSampleRate(newSampleRate);
}

json_t* Amalgam::dataToJson()  {
    json_t *rootJ = json_object();
    int dcCoupledI = dcCoupled ? 1 : 0;
    json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
    json_object_set_new(rootJ, "dcCoupled", json_integer(dcCoupledI));
    return rootJ;
}

void Amalgam::dataFromJson(json_t *rootJ) {
    json_t *panelStyleJ = json_object_get(rootJ, "panelStyle");
    json_t *dcCoupledJ = json_object_get(rootJ, "dcCoupled");
    panelStyle = json_integer_value(panelStyleJ);
    dcCoupled = json_integer_value(dcCoupledJ) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void AmalgamPanelStyleItem::onAction(const event::Action &e) {
    module->panelStyle = panelStyle;
}

void AmalgamPanelStyleItem::step() {
    rightText = (module->panelStyle == panelStyle) ? "✔" : "";
    MenuItem::step();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AmalgamWidget::AmalgamWidget(Amalgam* module) {
    setModule(module);

    darkPanel = new SvgPanel;
    darkPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AmalgamPanelDark.svg")));
    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AmalgamPanelLight.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }
    setPanel(darkPanel);

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Make jacks
    addInput(createInput<PJ301MDarkSmall>(xLeftInputPos, module, Amalgam::X_LEFT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(xRightInputPos, module, Amalgam::X_RIGHT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(yLeftInputPos, module, Amalgam::Y_LEFT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(yRightInputPos, module, Amalgam::Y_RIGHT_INPUT));

    addInput(createInput<PJ301MDarkSmall>(xDriveCVInputPos, module, Amalgam::X_GAIN_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(yDriveCVInputPos, module, Amalgam::Y_GAIN_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(typeCV1InputPos, module, Amalgam::TYPE_CV1_INPUT));
    addInput(createInput<PJ301MDarkSmall>(typeCV2InputPos, module, Amalgam::TYPE_CV2_INPUT));
    addInput(createInput<PJ301MDarkSmall>(paramACV1InputPos, module, Amalgam::PARAM_A_CV1_INPUT));
    addInput(createInput<PJ301MDarkSmall>(paramACV2InputPos, module, Amalgam::PARAM_A_CV2_INPUT));
    addInput(createInput<PJ301MDarkSmall>(paramBCV1InputPos, module, Amalgam::PARAM_B_CV1_INPUT));
    addInput(createInput<PJ301MDarkSmall>(paramBCV2InputPos, module, Amalgam::PARAM_B_CV2_INPUT));


    addOutput(createOutput<PJ301MDarkSmallOut>(leftANDOutputPos, module, Amalgam::X_Y_LEFT_AND_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(leftXOROutputPos, module, Amalgam::X_Y_LEFT_XOR_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(rightANDOutputPos, module, Amalgam::X_Y_RIGHT_AND_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(rightXOROutputPos, module, Amalgam::X_Y_RIGHT_XOR_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zLeftOutputPos, module, Amalgam::Z_LEFT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zRightOutputPos, module, Amalgam::Z_RIGHT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zANDOutputPos, module, Amalgam::Z_AND_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zXOROutputPos, module, Amalgam::Z_XOR_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zLeftPulseOutputPos, module, Amalgam::Z_LEFT_PULSE_1_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zRightPulseOutputPos, module, Amalgam::Z_RIGHT_PULSE_1_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zLeftPulse2OutputPos, module, Amalgam::Z_LEFT_PULSE_2_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(zRightPulse2OutputPos, module, Amalgam::Z_RIGHT_PULSE_2_OUTPUT));

    // Knobs

    addParam(createParam<RoganSmallWhite>(xDriveKnobPos, module, Amalgam::X_GAIN));
    addParam(createParam<RoganSmallWhite>(yDriveKnobPos, module, Amalgam::Y_GAIN));
    addParam(createParam<RoganMedGreen>(paramAKnobPos, module, Amalgam::A_PARAM));
    addParam(createParam<RoganMedBlue>(paramBKnobPos, module, Amalgam::B_PARAM));
    addParam(createParam<RoganMedWhite>(typeKnobPos, module, Amalgam::TYPE_PARAM));
    addParam(createParam<RoganSmallWhite>(modeCV1KnobPos, module, Amalgam::TYPE_CV1_PARAM));
    addParam(createParam<RoganSmallWhite>(modeCV2KnobPos, module, Amalgam::TYPE_CV2_PARAM));
    addParam(createParam<RoganSmallWhite>(xDriveCVKnobPos, module, Amalgam::X_GAIN_CV_PARAM));
    addParam(createParam<RoganSmallWhite>(yDriveCVKnobPos, module, Amalgam::Y_GAIN_CV_PARAM));
    addParam(createParam<RoganSmallGreen>(paramACV1KnobPos, module, Amalgam::PARAM_A_CV1_PARAM));
    addParam(createParam<RoganSmallGreen>(paramACV2KnobPos, module, Amalgam::PARAM_A_CV2_PARAM));
    addParam(createParam<RoganSmallBlue>(paramBCV1KnobPos, module, Amalgam::PARAM_B_CV1_PARAM));
    addParam(createParam<RoganSmallBlue>(paramBCV2KnobPos, module, Amalgam::PARAM_B_CV2_PARAM));

    modeDisplay = new DigitalDisplay;
    modeDisplay->box.pos = modeDisplayPos;
    modeDisplay->size = 13;
    modeDisplay->setText("NVRGONNA");
    addChild(modeDisplay);

    paramADisplay = new DigitalDisplay;
    paramADisplay->box.pos = paramADisplayPos;
    paramADisplay->size = 12;
    paramADisplay->setText("GIVE YOU");
    addChild(paramADisplay);

    paramBDisplay = new DigitalDisplay;
    paramBDisplay->box.pos = paramBDisplayPos;
    paramBDisplay->size = 12;
    paramBDisplay->setText("UP");
    addChild(paramBDisplay);

    dcCoupleButton = createParam<LightLEDButton>(DCCoupleLightPos, module, Amalgam::DC_COUPLE_PARAM);
    dcCoupleButton->momentary = false;
    addParam(dcCoupleButton);
    addChild(createLight<MediumLight<RedLight>>(DCCoupleLightPos.plus(Vec(2.5f, 2.5f)), module, Amalgam::DC_COUPLE_LIGHT));
}

void AmalgamWidget::appendContextMenu(Menu *menu) {
    Amalgam *module = dynamic_cast<Amalgam*>(this->module);
    assert(module);

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel style"));
    menu->addChild(construct<AmalgamPanelStyleItem>(&MenuItem::text, "Dark", &AmalgamPanelStyleItem::module,
                                                    module, &AmalgamPanelStyleItem::panelStyle, 0));
    menu->addChild(construct<AmalgamPanelStyleItem>(&MenuItem::text, "Light", &AmalgamPanelStyleItem::module,
                                                      module, &AmalgamPanelStyleItem::panelStyle, 1));
}

void AmalgamWidget::step() {
    if(!module) {
        darkPanel->visible = true;
        return;
    }
    Amalgam* m = reinterpret_cast<Amalgam*>(module);
    modeDisplay->text = modeNames[m->iAmalgamType];
    paramADisplay->text = paramANames[m->iAmalgamType];
    paramBDisplay->text = paramBNames[m->iAmalgamType];

    if (m->panelStyle) {
        darkPanel->visible = false;
        lightPanel->visible = true;
    }
    else {
        darkPanel->visible = true;
        lightPanel->visible = false;
    }
    Widget::step();
}

Model *modelAmalgam = createModel<Amalgam, AmalgamWidget>("Amalgam");
