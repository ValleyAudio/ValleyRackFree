#include "Plateau.hpp"

    Plateau::Plateau() : reverb(192000, 16, sizeMax) {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Plateau::DRY_PARAM, 0.0f, 1.f, 1.f, "Dry level", "%", 0.f,
                100.f);
    configParam(Plateau::WET_PARAM, 0.0f, 1.f, 0.5f, "Wet level", "%", 0.f,
                100.f);
    configParam(Plateau::PRE_DELAY_PARAM, 0.f, 0.500f, 0.f, "Pre-delay", "ms",
                0.f, 1000.f);
    configParam(Plateau::INPUT_LOW_DAMP_PARAM, 0.f, 10.f, 10.f,
                "Input low cut", "Hz", 2.f, 13.75f);
    configParam(Plateau::INPUT_HIGH_DAMP_PARAM, 0.f, 10.f, 10.f,
                "Input high cut", "Hz", 2.f, 13.75f);
    configParam(Plateau::SIZE_PARAM, 0.f, 1.f, 0.5f, "Size", "%", 0.f, 100.f);
    configParam(Plateau::DIFFUSION_PARAM, 0.f, 10.f, 10.f, "Diffusion", "%",
                0.f, 10.f);
    configParam(Plateau::DECAY_PARAM, 0.1f, 0.9999f, 0.54995f, "Decay", "%",
                0.f, 111.12345816f, -11.11234582f);
    configParam(Plateau::REVERB_LOW_DAMP_PARAM, 0.f, 10.f, 10.f,
                "Reverb low cut", "Hz", 2.f, 13.75f);
    configParam(Plateau::REVERB_HIGH_DAMP_PARAM, 0.f, 10.f, 10.f,
                "Reverb high cut", "Hz", 2.f, 13.75f);
    configParam(Plateau::MOD_SPEED_PARAM, 0.f, 1.f, 0.f, "Modulation rate", "%",
                0.f, 100.f);
    configParam(Plateau::MOD_DEPTH_PARAM, 0.f, 16.f, 0.5f, "Modulation depth",
                "%", 0.f, 6.25f);
    configParam(Plateau::MOD_SHAPE_PARAM, 0.f, 1.f, 0.5f, "Modulation shape",
                "%", 0.f, 200.f, -100.f);

    configParam(Plateau::DRY_CV_PARAM, -1.f, 1.f, 0.f, "Dry CV depth", "x");
    configParam(Plateau::WET_CV_PARAM, -1.f, 1.f, 0.f, "Wet CV depth", "x");
    configParam(Plateau::INPUT_LOW_DAMP_CV_PARAM, -1.f, 1.f, 0.f,
                "Input low cut CV", "x");
    configParam(Plateau::INPUT_HIGH_DAMP_CV_PARAM, -1.f, 1.f, 0.f,
                "Input high cut CV", "x");
    configParam(Plateau::SIZE_CV_PARAM, -1.f, 1.f, 0.f, "Size CV", "x");
    configParam(Plateau::DIFFUSION_CV_PARAM, -1.f, 1.f, 0.f, "Diffusion CV",
                "x");
    configParam(Plateau::DECAY_CV_PARAM, -1.f, 1.f, 0.f, "Decay CV", "x");
    configParam(Plateau::REVERB_LOW_DAMP_CV_PARAM, -1.f, 1.f, 0.f,
                "Reverb low cut CV", "x");
    configParam(Plateau::REVERB_HIGH_DAMP_CV_PARAM, -1.f, 1.f, 0.f,
                "Reverb high cut CV", "x");
    configParam(Plateau::MOD_SPEED_CV_PARAM, -1.f, 1.f, 0.f, "Mod speed CV",
                "x");
    configParam(Plateau::MOD_SHAPE_CV_PARAM, -1.f, 1.f, 0.f, "Mod shape CV",
                "x");
    configParam(Plateau::MOD_DEPTH_CV_PARAM, -1.f, 1.f, 0.f, "Mod depth CV",
                "x");
    configParam(Plateau::FREEZE_PARAM, 0.f, 1.f, 0.f, "Hold");
    configSwitch(Plateau::FREEZE_TOGGLE_PARAM, 0.f, 1.f, 0.f, "Hold toggle",
                 {"Off", "On"});
    configSwitch(Plateau::CLEAR_PARAM, 0.f, 1.f, 0.f, "Clear",
                 {"Off", "On"});
    configSwitch(Plateau::TUNED_MODE_PARAM, 0.f, 1.f, 0.f, "Tuned mode",
                 {"Off", "On"});
    configSwitch(Plateau::DIFFUSE_INPUT_PARAM, 0.f, 1.f, 1.f, "Diffuse input",
                 {"Off", "On"});

    configInput(Plateau::LEFT_INPUT, "Left");
    configInput(Plateau::RIGHT_INPUT, "Right");
    configInput(Plateau::DRY_CV_INPUT, "Dry level CV");
    configInput(Plateau::WET_CV_INPUT, "Wet level CV");
    configInput(Plateau::PRE_DELAY_CV_INPUT, "Pre delay CV");
    configInput(Plateau::INPUT_LOW_DAMP_CV_INPUT, "Input low cut CV");
    configInput(Plateau::INPUT_HIGH_DAMP_CV_INPUT, "Input high cut CV");
    configInput(Plateau::SIZE_CV_INPUT, "Size CV");
    configInput(Plateau::DIFFUSION_CV_INPUT, "Diffusion CV");
    configInput(Plateau::DECAY_CV_INPUT, "Decay CV");
    configInput(Plateau::REVERB_HIGH_DAMP_CV_INPUT, "Reverb high cut CV");
    configInput(Plateau::REVERB_LOW_DAMP_CV_INPUT, "Reverb low cut CV");
    configInput(Plateau::MOD_SPEED_CV_INPUT, "Mod. speed CV");
    configInput(Plateau::MOD_SHAPE_CV_INPUT, "Mod. shape CV");
    configInput(Plateau::MOD_DEPTH_CV_INPUT, "Mod. depth CV");
    configInput(Plateau::FREEZE_CV_INPUT, "Hold");
    configInput(Plateau::CLEAR_CV_INPUT, "Clear");

    configOutput(Plateau::LEFT_OUTPUT, "Left");
    configOutput(Plateau::RIGHT_OUTPUT, "Right");

    configBypass(Plateau::LEFT_INPUT, Plateau::LEFT_OUTPUT);
    configBypass(Plateau::RIGHT_INPUT, Plateau::RIGHT_OUTPUT);

    reverb.setSampleRate(APP->engine->getSampleRate());
    envelope.setSampleRate(APP->engine->getSampleRate());
    envelope.setTime(0.004f);
    envelope._value = 1.f;
}

void Plateau::process(const ProcessArgs &args) {
    getParameters();
    setLights();

    leftInput = clamp(leftInput, -10.f, 10.f);
    rightInput = clamp(rightInput, -10.f, 10.f);

    reverb.setTimeScale(size);
    reverb.setPreDelay(clamp(preDelay, 0.f, 1.f));
    reverb.setInputFilterLowCutoffPitch(inputDampLow);
    reverb.setInputFilterHighCutoffPitch(inputDampHigh);
    reverb.enableInputDiffusion(diffuseInput > 0.5f);
    reverb.setDecay(decay);
    reverb.setTankDiffusion(diffusion);
    reverb.setTankFilterLowCutFrequency(reverbDampLow);
    reverb.setTankFilterHighCutFrequency(reverbDampHigh);
    reverb.setTankModSpeed(modSpeed);
    reverb.setTankModDepth(modDepth);
    reverb.setTankModShape(modShape);

    reverb.process(leftInput * minus20dBGain * inputSensitivity * envelope._value,
                   rightInput * minus20dBGain * inputSensitivity * envelope._value);

    leftOutput = leftInput * dry + reverb.getLeftOutput() * wet *
                 envelope._value;
    rightOutput = rightInput * dry + reverb.getRightOutput() * wet *
                  envelope._value;

    if (softDriveOutput) {
        leftOutput = tanhDriveSignal(leftOutput * saturatorPreGain,
                                     saturatorDrive) * saturatorPostGain;
        rightOutput = tanhDriveSignal(rightOutput * saturatorPreGain,
                                      saturatorDrive) * saturatorPostGain;
        outputs[LEFT_OUTPUT].setVoltage(leftOutput);
        outputs[RIGHT_OUTPUT].setVoltage(rightOutput);
    }
    else {
        outputs[LEFT_OUTPUT].setVoltage(clamp(leftOutput, -10.f, 10.f));
        outputs[RIGHT_OUTPUT].setVoltage(clamp(rightOutput, -10.f, 10.f));
    }
}

void Plateau::getParameters() {
    freezeToggle = params[FREEZE_TOGGLE_PARAM].getValue() > 0.f;
    tuned = params[TUNED_MODE_PARAM].getValue() > 0.5f ? 1 : 0;
    diffuseInput = params[DIFFUSE_INPUT_PARAM].getValue();

    //Freeze
    if((params[FREEZE_PARAM].getValue() > 0.5f ||
        inputs[FREEZE_CV_INPUT].getVoltage() > 0.5f) && !freezeButtonState) {
        freeze = freezeToggle ? !freeze : true;
        freezeButtonState = true;
    }
    else if(params[FREEZE_PARAM].getValue() <= 0.5f 
            && inputs[FREEZE_CV_INPUT].getVoltage() <= 0.5f
            && freezeButtonState) {
        freeze = freezeToggle ? freeze : false;
        freezeButtonState = false;
    }

    if(freeze && !frozen) {
        frozen = true;
        reverb.freeze(frozen);
    }
    else if(!freeze && frozen){
        frozen = false;
        reverb.freeze(frozen);
    }

    // Clear
    if((params[CLEAR_PARAM].getValue() > 0.5f ||
        inputs[CLEAR_CV_INPUT].getVoltage() > 0.5f) && !clear && cleared) {
        cleared = false;
        clear = true;
    }
    else if((params[CLEAR_PARAM].getValue() <= 0.5f
             && inputs[CLEAR_CV_INPUT].getVoltage() <= 0.5f) && cleared) {
        clear = false;
    }

    if(clear) {
        if(!cleared && !fadeOut && !fadeIn) {
            fadeOut = true;
            envelope.setStartEndPoints(1.f, 0.f);
            envelope.trigger();
        }
        if(fadeOut && envelope._justFinished) {
            reverb.clear();
            fadeOut = false;
            fadeIn = true;
            envelope.setStartEndPoints(0.f, 1.f);
            envelope.trigger();
        }
        if(fadeIn && envelope._justFinished) {
            fadeIn = false;
            cleared = true;
            envelope._value = 1.f;
        }
    }
    envelope.process();

    // CV
    switch(preDelayCVSensState) {
        case 0: preDelayCVSens = preDelayNormSens; break;
        case 1: preDelayCVSens = preDelayLowSens;
    }
    preDelay = params[PRE_DELAY_PARAM].getValue();
    preDelay += 0.5f * (powf(2.f,inputs[PRE_DELAY_CV_INPUT].getVoltage() *
                             preDelayCVSens) - 1.f);

    size = inputs[SIZE_CV_INPUT].getVoltage() *
           params[SIZE_CV_PARAM].getValue() * 0.1f;
    size += params[SIZE_PARAM].getValue();

    if(tuned) {
        size = sizeMin * powf(2.f, size * 5.f);
        size = clamp(size, sizeMin, 2.5f);
    }
    else {
        size *= size;
        size = rescale(size, 0.f, 1.f, 0.01f, sizeMax);
        size = clamp(size, 0.01f, sizeMax);
    }

    diffusion = inputs[DIFFUSION_CV_INPUT].getVoltage() *
                params[DIFFUSION_CV_PARAM].getValue();
    diffusion += params[DIFFUSION_PARAM].getValue();
    diffusion = clamp(diffusion, 0.f, 10.f);


    decay = rescale(inputs[DECAY_CV_INPUT].getVoltage() *
                    params[DECAY_CV_PARAM].getValue(), 0.f, 10.f, 0.1f, 0.999f);
    decay += params[DECAY_PARAM].getValue();
    decay = clamp(decay, 0.1f, decayMax);
    decay = 1.f - decay;
    decay = 1.f - decay * decay;

    inputDampLow = inputs[INPUT_LOW_DAMP_CV_INPUT].getVoltage() *
                   params[INPUT_LOW_DAMP_CV_PARAM].getValue();
    inputDampLow += (params[INPUT_LOW_DAMP_PARAM].getValue() + 5.f);
    inputDampLow = clamp(inputDampLow, 0.f, 10.f);
    inputDampLow = 10.f - inputDampLow;

    inputDampHigh = inputs[INPUT_HIGH_DAMP_CV_INPUT].getVoltage() *
                    params[INPUT_HIGH_DAMP_CV_PARAM].getValue();
    inputDampHigh += (params[INPUT_HIGH_DAMP_PARAM].getValue() + 5.f);
    inputDampHigh = clamp(inputDampHigh, 0.f, 10.f);

    reverbDampLow = inputs[REVERB_LOW_DAMP_CV_INPUT].getVoltage() *
                    params[REVERB_LOW_DAMP_CV_PARAM].getValue();
    reverbDampLow += (params[REVERB_LOW_DAMP_PARAM].getValue() + 5.f);
    reverbDampLow = clamp(reverbDampLow, 0.f, 10.f);
    reverbDampLow = 10.f - reverbDampLow;

    reverbDampHigh = inputs[REVERB_HIGH_DAMP_CV_INPUT].getVoltage() *
                     params[REVERB_HIGH_DAMP_CV_PARAM].getValue();
    reverbDampHigh += (params[REVERB_HIGH_DAMP_PARAM].getValue() + 5.f);
    reverbDampHigh = clamp(reverbDampHigh, 0.f, 10.f);

    modSpeed = inputs[MOD_SPEED_CV_INPUT].getVoltage() *
               params[MOD_SPEED_CV_PARAM].getValue() * 0.1f;
    modSpeed += params[MOD_SPEED_PARAM].getValue();
    modSpeed = clamp(modSpeed, modSpeedMin, modSpeedMax);
    modSpeed *= modSpeed;
    modSpeed = modSpeed * 99.f + 1.f;

    modShape = inputs[MOD_SHAPE_CV_INPUT].getVoltage() *
               params[MOD_SHAPE_CV_PARAM].getValue() * 0.1f;
    modShape += params[MOD_SHAPE_PARAM].getValue();
    modShape = rescale(modShape, 0.f, 1.f, modShapeMin, modShapeMax);
    modShape = clamp(modShape, modShapeMin, modShapeMax);

    modDepth = inputs[MOD_DEPTH_CV_INPUT].getVoltage() *
               params[MOD_DEPTH_CV_PARAM].getValue();
    modDepth = rescale(modDepth, 0.f, 10.f, modDepthMin, modDepthMax);
    modDepth += params[MOD_DEPTH_PARAM].getValue();
    modDepth = clamp(modDepth, modDepthMin, modDepthMax);

    leftInput = inputs[LEFT_INPUT].getVoltageSum();
    rightInput = inputs[RIGHT_INPUT].getVoltageSum();

    if(!inputs[LEFT_INPUT].isConnected() &&
        inputs[RIGHT_INPUT].isConnected()) {
        leftInput = rightInput;
    }
    else if(inputs[LEFT_INPUT].isConnected() &&
            !inputs[RIGHT_INPUT].isConnected()) {
        rightInput = leftInput;
    }

    inputSensitivity = inputSensitivityState ? minus18dBGain : zerodBGain;

    dry = inputs[DRY_CV_INPUT].getVoltage() * params[DRY_CV_PARAM].getValue();
    dry += params[DRY_PARAM].getValue();
    dry = clamp(dry, 0.f, 1.f);

    wet = inputs[WET_CV_INPUT].getVoltage() * params[WET_CV_PARAM].getValue();
    wet += params[WET_PARAM].getValue();
    wet = clamp(wet, 0.f, 1.f) * 10.f;
}

void Plateau::setLights() {
    lights[FREEZE_TOGGLE_LIGHT].value = freezeToggle ? 10.f : 0.f;
    lights[FREEZE_LIGHT].value = freeze ? 10.f : 0.f;
    lights[TUNED_MODE_LIGHT].value = tuned ? 10.f : 0.f;
    lights[DIFFUSE_INPUT_LIGHT].value = diffuseInput > 0.5f ? 10.f : 0.f;
    lights[CLEAR_LIGHT].value = clear ? 10.f : 0.f;
}

void Plateau::onSampleRateChange() {
    reverb.setSampleRate(APP->engine->getSampleRate());
    envelope.setSampleRate(APP->engine->getSampleRate());
}

json_t* Plateau::dataToJson()  {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "frozen", json_boolean(freeze));
    json_object_set_new(rootJ, "freezeToggle", json_boolean(freezeToggle));
    json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
    json_object_set_new(rootJ, "tuned", json_integer((int)tuned));
    json_object_set_new(rootJ, "diffuseInput", json_integer((int)diffuseInput));
    json_object_set_new(rootJ, "preDelayCVSens", json_integer((int)preDelayCVSensState));
    json_object_set_new(rootJ, "inputSensitivity", json_integer((int)inputSensitivityState));
    json_object_set_new(rootJ, "outputSaturation", json_integer((int)clamp(softDriveOutput, 0, 1)));
    return rootJ;
}

void Plateau::dataFromJson(json_t *rootJ) {
    json_t *frozenJ = json_object_get(rootJ, "frozen");
    freeze = json_boolean_value(frozenJ);

    json_t *freezeToggleJ = json_object_get(rootJ, "freezeToggle");
    freezeToggle = json_boolean_value(freezeToggleJ);

    json_t *panelStyleJ = json_object_get(rootJ, "panelStyle");
    panelStyle = json_integer_value(panelStyleJ);

    json_t *tunedJ = json_object_get(rootJ, "tuned");
    tuned = json_integer_value(tunedJ);

    json_t *diffuseInputJ = json_object_get(rootJ, "diffuseInput");
    diffuseInput = json_integer_value(diffuseInputJ);

    json_t *preDelayCVSensJ = json_object_get(rootJ, "preDelayCVSens");
    preDelayCVSensState = json_integer_value(preDelayCVSensJ);

    json_t *inputSensitivityJ = json_object_get(rootJ, "inputSensitivity");
    inputSensitivityState = json_integer_value(inputSensitivityJ);

    json_t *outputSaturationJ = json_object_get(rootJ, "outputSaturation");
    softDriveOutput = (bool)json_integer_value(outputSaturationJ);
}

void Plateau::onReset(const ResetEvent& e) {
    params[FREEZE_TOGGLE_PARAM].setValue(0.f);
    params[TUNED_MODE_PARAM].setValue(0.f);
    params[DIFFUSE_INPUT_PARAM].setValue(1.f);
    Module::onReset(e);
}

////////////////////////////////////////////////////////////////////////////////

void PlateauPanelStyleItem::onAction(const event::Action &e) {
    module->panelStyle = panelStyle;
}

void PlateauPanelStyleItem::step() {
    rightText = (module->panelStyle == panelStyle) ? "✔" : "";
    MenuItem::step();
}

void PlateauPreDelayCVSensItem::onAction(const event::Action &e) {
    module->preDelayCVSensState = preDelayCVSensState;
}

void PlateauPreDelayCVSensItem::step() {
    rightText = (module->preDelayCVSensState == preDelayCVSensState) ? "✔" : "";
    MenuItem::step();
}

void PlateauInputSensItem::onAction(const event::Action &e) {
    module->inputSensitivityState = inputSensitivityState;
}

void PlateauInputSensItem::step() {
    rightText = (module->inputSensitivityState == inputSensitivityState) ? "✔" : "";
    MenuItem::step();
}

void PlateauOutputSaturationItem::onAction(const event::Action &e) {
    module->softDriveOutput = softDriveOutput;
}

void PlateauOutputSaturationItem::step() {
    rightText = (module->softDriveOutput == softDriveOutput) ? "✔" : "";
    MenuItem::step();
}

////////////////////////////////////////////////////////////////////////////////

PlateauWidget::PlateauWidget(Plateau* module) {
    setModule(module);

    darkPanel = new SvgPanel;
    darkPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance,
                             "res/PlateauPanelDark.svg")));
    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance,
                                  "res/PlateauPanelLight.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }
    setPanel(darkPanel);

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Make jacks
    addInput(createInput<PJ301MDarkSmall>(leftInputPos, module,
                                          Plateau::LEFT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(rightInputPos, module,
                                          Plateau::RIGHT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(dryCVPos, module,
                                          Plateau::DRY_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(wetCVPos, module,
                                          Plateau::WET_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(preDelayCVPos, module,
                                          Plateau::PRE_DELAY_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(inputLowDampCVPos, module,
                                          Plateau::INPUT_LOW_DAMP_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(inputHighDampCVPos, module,
                                          Plateau::INPUT_HIGH_DAMP_CV_INPUT));

    addInput(createInput<PJ301MDarkSmall>(sizeCVPos, module,
                                          Plateau::SIZE_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(diffCVPos, module,
                                          Plateau::DIFFUSION_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(decayCVPos, module,
                                          Plateau::DECAY_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(reverbLowDampCVPos, module,
                                          Plateau::REVERB_LOW_DAMP_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(reverbHighDampCVPos, module,
                                          Plateau::REVERB_HIGH_DAMP_CV_INPUT));

    addInput(createInput<PJ301MDarkSmall>(modRateCVPos, module,
                                          Plateau::MOD_SPEED_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(modShapeCVPos, module,
                                          Plateau::MOD_SHAPE_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(modDepthCVPos, module,
                                          Plateau::MOD_DEPTH_CV_INPUT));

    addInput(createInput<PJ301MDarkSmall>(holdCVPos, module,
                                          Plateau::FREEZE_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(clearCVPos, module,
                                          Plateau::CLEAR_CV_INPUT));

    addOutput(createOutput<PJ301MDarkSmallOut>(leftOutputPos, module,
                                          Plateau::LEFT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(rightOutputPos, module,
                                          Plateau::RIGHT_OUTPUT));

    // Make knobs

    float minAngle = -0.77f * M_PI;
    float maxAngle = 0.77f * M_PI;
    RoganMedSmallWhite* knobDryParam = createParam<RoganMedSmallWhite>(dryPos, module, Plateau::DRY_PARAM);
    knobDryParam->minAngle = minAngle;
    knobDryParam->maxAngle = maxAngle;
    addParam(knobDryParam);

    RoganMedSmallWhite* knobWetParam = createParam<RoganMedSmallWhite>(wetPos, module, Plateau::WET_PARAM);
    knobWetParam->minAngle = minAngle;
    knobWetParam->maxAngle = maxAngle;
    addParam(knobWetParam);

    RoganSmallWhite* knobPreDelayParam = createParam<RoganSmallWhite>(preDelayPos, module, Plateau::PRE_DELAY_PARAM);
    knobPreDelayParam->minAngle = minAngle;
    knobPreDelayParam->maxAngle = maxAngle;
    addParam(knobPreDelayParam);

    RoganMedGreen* knobInputLowDamp = createParam<RoganMedGreen>(inputLowDampPos, module, Plateau::INPUT_LOW_DAMP_PARAM);
    knobInputLowDamp->minAngle = minAngle;
    knobInputLowDamp->maxAngle = maxAngle;
    addParam(knobInputLowDamp);

    RoganMedGreen* knobInputHighDamp = createParam<RoganMedGreen>(inputHighDampPos, module, Plateau::INPUT_HIGH_DAMP_PARAM);
    knobInputHighDamp->minAngle = minAngle;
    knobInputHighDamp->maxAngle = maxAngle;
    addParam(knobInputHighDamp);

    RoganMedBlue* knobSize = createParam<RoganMedBlue>(sizePos, module, Plateau::SIZE_PARAM);
    knobSize->minAngle = minAngle;
    knobSize->maxAngle = maxAngle;
    addParam(knobSize);

    RoganMedBlue* knobDiffusion = createParam<RoganMedBlue>(diffPos, module, Plateau::DIFFUSION_PARAM);
    knobDiffusion->minAngle = minAngle;
    knobDiffusion->maxAngle = maxAngle;
    addParam(knobDiffusion);

    RoganMedBlue* knobDecay = createParam<RoganMedBlue>(decayPos, module, Plateau::DECAY_PARAM);
    knobDecay->minAngle = minAngle;
    knobDecay->maxAngle = maxAngle;
    addParam(knobDecay);

    RoganMedGreen* knobReverbLowDamp = createParam<RoganMedGreen>(reverbLowDampPos, module, Plateau::REVERB_LOW_DAMP_PARAM);
    knobReverbLowDamp->minAngle = minAngle;
    knobReverbLowDamp->maxAngle = maxAngle;
    addParam(knobReverbLowDamp);

    RoganMedGreen* knobReverbHighDamp = createParam<RoganMedGreen>(reverbHighDampPos, module, Plateau::REVERB_HIGH_DAMP_PARAM);
    knobReverbHighDamp->minAngle = minAngle;
    knobReverbHighDamp->maxAngle = maxAngle;
    addParam(knobReverbHighDamp);

    RoganMedRed* knobModRate = createParam<RoganMedRed>(modRatePos, module, Plateau::MOD_SPEED_PARAM);
    knobModRate->minAngle = minAngle;
    knobModRate->maxAngle = maxAngle;
    addParam(knobModRate);

    RoganMedRed* knobModDepth = createParam<RoganMedRed>(modDepthPos, module, Plateau::MOD_DEPTH_PARAM);
    knobModDepth->minAngle = minAngle;
    knobModDepth->maxAngle = maxAngle;
    addParam(knobModDepth);

    RoganMedRed* knobModShape = createParam<RoganMedRed>(modShapePos, module, Plateau::MOD_SHAPE_PARAM);
    knobModShape->minAngle = minAngle;
    knobModShape->maxAngle = maxAngle;
    addParam(knobModShape);

    // Make Attenuverters
    addParam(createParam<RoganSmallWhite>(dryAttenPos, module, Plateau::DRY_CV_PARAM));
    addParam(createParam<RoganSmallWhite>(wetAttenPos, module, Plateau::WET_CV_PARAM));
    addParam(createParam<RoganSmallGreen>(inputLowDampAttenPos, module, Plateau::INPUT_LOW_DAMP_CV_PARAM));
    addParam(createParam<RoganSmallGreen>(inputHighDampAttenPos, module, Plateau::INPUT_HIGH_DAMP_CV_PARAM));

    addParam(createParam<RoganSmallBlue>(sizeAttenPos, module, Plateau::SIZE_CV_PARAM));
    addParam(createParam<RoganSmallBlue>(diffAttenPos, module, Plateau::DIFFUSION_CV_PARAM));
    addParam(createParam<RoganSmallBlue>(decayAttenPos, module, Plateau::DECAY_CV_PARAM));
    addParam(createParam<RoganSmallGreen>(reverbLowDampAttenPos, module, Plateau::REVERB_LOW_DAMP_CV_PARAM));
    addParam(createParam<RoganSmallGreen>(reverbHighDampAttenPos, module, Plateau::REVERB_HIGH_DAMP_CV_PARAM));

    addParam(createParam<RoganSmallRed>(modRateAttenPos, module, Plateau::MOD_SPEED_CV_PARAM));
    addParam(createParam<RoganSmallRed>(modShapeAttenPos, module, Plateau::MOD_SHAPE_CV_PARAM));
    addParam(createParam<RoganSmallRed>(modDepthAttenPos, module, Plateau::MOD_DEPTH_CV_PARAM));

    // Make buttons
    addParam(createParam<LightLEDButton>(Vec(7.875, 244.85), module, Plateau::FREEZE_PARAM));
    addChild(createLight<MediumLight<RedLight>>(Vec(10.375, 247.35), module, Plateau::FREEZE_LIGHT));

    addParam(createParam<LightLEDButton>(Vec(157.875, 244.85), module, Plateau::CLEAR_PARAM));
    addChild(createLight<MediumLight<RedLight>>(Vec(160.375, 247.35), module, Plateau::CLEAR_LIGHT));

    freezeToggleButton = createParam<LightLEDButton>(Vec(31.375, 256.35), module, Plateau::FREEZE_TOGGLE_PARAM);
    freezeToggleButton->momentary = false;
    addParam(freezeToggleButton);
    addChild(createLight<MediumLight<RedLight>>(Vec(33.875, 258.85), module, Plateau::FREEZE_TOGGLE_LIGHT));

    tunedModeButton = createParam<LightLEDButton>(Vec(13.875, 127.35), module, Plateau::TUNED_MODE_PARAM);
    tunedModeButton->momentary = false;
    addParam(tunedModeButton);
    addChild(createLight<MediumLight<RedLight>>(Vec(16.375, 129.85), module, Plateau::TUNED_MODE_LIGHT));

    diffuseInputButton = createParam<LightLEDButton>(Vec(151.875, 127.35), module, Plateau::DIFFUSE_INPUT_PARAM);
    diffuseInputButton->momentary = false;
    addParam(diffuseInputButton);
    addChild(createLight<MediumLight<RedLight>>(Vec(154.375, 129.85), module, Plateau::DIFFUSE_INPUT_LIGHT));
}

void PlateauWidget::appendContextMenu(Menu *menu) {
    Plateau *module = dynamic_cast<Plateau*>(this->module);
    assert(module);

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel style"));
    menu->addChild(construct<PlateauPanelStyleItem>(&MenuItem::text, "Dark", &PlateauPanelStyleItem::module,
                                                    module, &PlateauPanelStyleItem::panelStyle, 0));
    menu->addChild(construct<PlateauPanelStyleItem>(&MenuItem::text, "Light", &PlateauPanelStyleItem::module,
                                                    module, &PlateauPanelStyleItem::panelStyle, 1));

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Predelay CV Sensitivity"));
    menu->addChild(construct<PlateauPreDelayCVSensItem>(&MenuItem::text, "Normal (1x)", &PlateauPreDelayCVSensItem::module,
                                                        module, &PlateauPreDelayCVSensItem::preDelayCVSensState, 0));
    menu->addChild(construct<PlateauPreDelayCVSensItem>(&MenuItem::text, "Low (0.5x)", &PlateauPreDelayCVSensItem::module,
                                                        module, &PlateauPreDelayCVSensItem::preDelayCVSensState, 1));

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Input Sensitivity"));
    menu->addChild(construct<PlateauInputSensItem>(&MenuItem::text, "0 dB", &PlateauInputSensItem::module,
                                                        module, &PlateauInputSensItem::inputSensitivityState, 0));
    menu->addChild(construct<PlateauInputSensItem>(&MenuItem::text, "-18 dB", &PlateauInputSensItem::module,
                                                        module, &PlateauInputSensItem::inputSensitivityState, 1));

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Output Saturation"));
    menu->addChild(construct<PlateauOutputSaturationItem>(&MenuItem::text, "Off", &PlateauOutputSaturationItem::module,
                                                          module, &PlateauOutputSaturationItem::softDriveOutput, false));
    menu->addChild(construct<PlateauOutputSaturationItem>(&MenuItem::text, "On", &PlateauOutputSaturationItem::module,
                                                          module, &PlateauOutputSaturationItem::softDriveOutput, true));
}

void PlateauWidget::step() {
    if(module) {
        if(dynamic_cast<Plateau*>(module)->panelStyle == 1) {
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

Model *modelPlateau = createModel<Plateau, PlateauWidget>("Plateau");
