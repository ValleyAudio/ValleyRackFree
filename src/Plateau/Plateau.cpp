#include "Plateau.hpp"

Plateau::Plateau() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Plateau::DRY_PARAM, 0.0f, 1.f, 1.f, "Dry Level");
    configParam(Plateau::WET_PARAM, 0.0f, 1.f, 0.5f, "Wet Level");
    configParam(Plateau::PRE_DELAY_PARAM, 0.f, 0.500f, 0.f, "Pre-delay");
    configParam(Plateau::INPUT_LOW_DAMP_PARAM, 0.f, 10.f, 10.f, "Input Low Cut");
    configParam(Plateau::INPUT_HIGH_DAMP_PARAM, 0.f, 10.f, 10.f, "Input High Cut");
    configParam(Plateau::SIZE_PARAM, 0.f, 1.f, 0.5f, "Size");
    configParam(Plateau::DIFFUSION_PARAM, 0.f, 10.f, 10.f, "Diffusion");
    configParam(Plateau::DECAY_PARAM, 0.1f, 0.9999f, 0.54995f, "Decay");
    configParam(Plateau::REVERB_LOW_DAMP_PARAM, 0.0f, 10.f, 10.f, "Reverb Low Cut");
    configParam(Plateau::REVERB_HIGH_DAMP_PARAM, 0.0f, 10.f, 10.f, "Reverb High Cut");
    configParam(Plateau::MOD_SPEED_PARAM, 0.f, 1.f, 0.f, "Modulation Rate");
    configParam(Plateau::MOD_DEPTH_PARAM, 0.f, 16.f, 0.5f, "Modulation Depth");
    configParam(Plateau::MOD_SHAPE_PARAM, 0.f, 1.f, 0.5f, "Modulation Shape");

    configParam(Plateau::DRY_CV_PARAM, -1.f, 1.f, 0.f, "Dry CV Depth");
    configParam(Plateau::WET_CV_PARAM, -1.f, 1.f, 0.f, "Wet CV Depth");
    configParam(Plateau::INPUT_LOW_DAMP_CV_PARAM, -1.f, 1.f, 0.f, "Input Low Cut CV");
    configParam(Plateau::INPUT_HIGH_DAMP_CV_PARAM, -1.f, 1.f, 0.f, "Input High Cut CV");
    configParam(Plateau::SIZE_CV_PARAM, -1.f, 1.f, 0.f, "Size CV");
    configParam(Plateau::DIFFUSION_CV_PARAM, -1.f, 1.f, 0.f, "Diffusion CV");
    configParam(Plateau::DECAY_CV_PARAM, -1.f, 1.f, 0.f, "Decay CV");
    configParam(Plateau::REVERB_LOW_DAMP_CV_PARAM, -1.f, 1.f, 0.f, "Reverb Low Cut CV");
    configParam(Plateau::REVERB_HIGH_DAMP_CV_PARAM, -1.f, 1.f, 0.f, "Reverb High Cut CV");
    configParam(Plateau::MOD_SPEED_CV_PARAM, -1.f, 1.f, 0.f, "Mod Speed CV");
    configParam(Plateau::MOD_SHAPE_CV_PARAM, -1.f, 1.f, 0.f, "Mod Shape CV");
    configParam(Plateau::MOD_DEPTH_CV_PARAM, -1.f, 1.f, 0.f, "Mod Depth CV");
    configParam(Plateau::FREEZE_PARAM, 0.f, 1.f, 0.f, "Freeze");
    configParam(Plateau::FREEZE_TOGGLE_PARAM, 0.f, 1.f, 0.f, "Freeze Toggle");
    configParam(Plateau::CLEAR_PARAM, 0.f, 1.f, 0.f, "Clear");
    configParam(Plateau::TUNED_MODE_PARAM, 0.f, 1.f, 0.f, "Tuned Mode");
    configParam(Plateau::DIFFUSE_INPUT_PARAM, 0.f, 1.f, 1.f, "Diffuse Input");

    configBypass(Plateau::LEFT_INPUT, Plateau::LEFT_OUTPUT);
    configBypass(Plateau::RIGHT_INPUT, Plateau::RIGHT_OUTPUT);

    reverb.setSampleRate(APP->engine->getSampleRate());
    envelope.setSampleRate(APP->engine->getSampleRate());
    envelope.setTime(0.004f);
    envelope._value = 1.f;

    wet = 0.5f;
    dry = 1.f;
    preDelay = 0.f;
    preDelayCVSens = preDelayNormSens;
    size = 1.f;
    diffusion = 1.f;
    decay = 0.f;
    inputDampLow = 0.f;
    inputDampHigh = 10.f;
    reverbDampLow = 0.f;
    reverbDampHigh = 10.f;
    modSpeed = 0.1f;
    modShape = 0.5f;
    modDepth = 0.0f;

    freezeButtonState = false;
    freezeToggle = false;
    freezeToggleButtonState = false;
    freeze = false;
    frozen = false;
    tunedButtonState = false;
    diffuseButtonState = false;
    preDelayCVSensState = 0;
    inputSensitivityState = 0;
    outputSaturationState = 0;

    clear = false;
    cleared = true;
    fadeOut = false;
    fadeIn = false;
    tuned = 0;
    diffuseInput = 1;

    leftInput = 0.f;
    rightInput = 0.f;
}

void Plateau::process(const ProcessArgs &args) {
    //Freeze
    freezeToggle = params[FREEZE_TOGGLE_PARAM].getValue() > 0.5f ? true : false;
    lights[FREEZE_TOGGLE_LIGHT].value = freezeToggle ? 10.f : 0.f;

    if((params[FREEZE_PARAM].getValue() > 0.5f || inputs[FREEZE_CV_INPUT].getVoltage() > 0.5f)
    && !freezeButtonState) {
        freeze = freezeToggle ? !freeze : true;
        freezeButtonState = true;
    }
    if(params[FREEZE_PARAM].getValue() <= 0.5f && inputs[FREEZE_CV_INPUT].getVoltage() <= 0.5f
    && freezeButtonState) {
        freeze = freezeToggle ? freeze : false;
        freezeButtonState = false;
    }

    if(freeze && !frozen) {
        frozen = true;
        reverb.freeze();
    }
    else if(!freeze && frozen){
        frozen = false;
        reverb.unFreeze();
    }
    lights[FREEZE_LIGHT].value = freeze ? 10.f : 0.f;

    tuned = params[TUNED_MODE_PARAM].getValue() > 0.5f ? 1 : 0;
    lights[TUNED_MODE_LIGHT].value = tuned ? 10.f : 0.f;

    diffuseInput = params[DIFFUSE_INPUT_PARAM].getValue();
    lights[DIFFUSE_INPUT_LIGHT].value = diffuseInput ? 10.f : 0.f;

    // Clear
    if((params[CLEAR_PARAM].getValue() > 0.5f || inputs[CLEAR_CV_INPUT].getVoltage() > 0.5f) && !clear && cleared) {
        cleared = false;
        clear = true;
        //clear = 1;
    }
    else if((params[CLEAR_PARAM].getValue() < 0.5f && inputs[CLEAR_CV_INPUT].getVoltage() < 0.5f) && cleared) {
        clear = false;
    }

    if(clear) {
        if(!cleared && !fadeOut && !fadeIn) {
            fadeOut = true;
            envelope.setStartEndPoints(1.f, 0.f);
            envelope.trigger();
            lights[CLEAR_LIGHT].value = 10.f;
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
            lights[CLEAR_LIGHT].value = 0.f;
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
    preDelay += 0.5f * (powf(2.f, inputs[PRE_DELAY_CV_INPUT].getVoltage() * preDelayCVSens) - 1.f);
    reverb.setPreDelay(clamp(preDelay, 0.f, 1.f));

    size = inputs[SIZE_CV_INPUT].getVoltage() * params[SIZE_CV_PARAM].getValue() * 0.1f;
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
    reverb.setTimeScale(size);

    diffusion = inputs[DIFFUSION_CV_INPUT].getVoltage() * params[DIFFUSION_CV_PARAM].getValue();
    diffusion += params[DIFFUSION_PARAM].getValue();
    diffusion = clamp(diffusion, 0.f, 10.f);
    reverb.plateDiffusion1 = rescale(diffusion, 0.f, 10.f, 0.f, 0.7f);
    reverb.plateDiffusion2 = rescale(diffusion, 0.f, 10.f, 0.f, 0.5f);

    decay = rescale(inputs[DECAY_CV_INPUT].getVoltage() * params[DECAY_CV_PARAM].getValue(), 0.f, 10.f, 0.1f, 0.999f);
    decay += params[DECAY_PARAM].getValue();
    decay = clamp(decay, 0.1f, decayMax);
    decay = 1.f - decay;
    decay = 1.f - decay * decay;

    inputDampLow = inputs[INPUT_LOW_DAMP_CV_INPUT].getVoltage() * params[INPUT_LOW_DAMP_CV_PARAM].getValue();
    inputDampLow += params[INPUT_LOW_DAMP_PARAM].value;
    inputDampLow = clamp(inputDampLow, 0.f, 10.f);
    inputDampLow = 10.f - inputDampLow;

    inputDampHigh = inputs[INPUT_HIGH_DAMP_CV_INPUT].getVoltage() * params[INPUT_HIGH_DAMP_CV_PARAM].getValue();
    inputDampHigh += params[INPUT_HIGH_DAMP_PARAM].getValue();
    inputDampHigh = clamp(inputDampHigh, 0.f, 10.f);

    reverbDampLow = inputs[REVERB_LOW_DAMP_CV_INPUT].getVoltage() * params[REVERB_LOW_DAMP_CV_PARAM].getValue();
    reverbDampLow += params[REVERB_LOW_DAMP_PARAM].getValue();
    reverbDampLow = clamp(reverbDampLow, 0.f, 10.f);
    reverbDampLow = 10.f - reverbDampLow;

    reverbDampHigh = inputs[REVERB_HIGH_DAMP_CV_INPUT].getVoltage() * params[REVERB_HIGH_DAMP_CV_PARAM].getValue();
    reverbDampHigh += params[REVERB_HIGH_DAMP_PARAM].getValue();
    reverbDampHigh = clamp(reverbDampHigh, 0.f, 10.f);

    reverb.diffuseInput = (double)diffuseInput;

    reverb.decay = decay;
    reverb.inputLowCut = 440.f * powf(2.f, inputDampLow - 5.f);
    reverb.inputHighCut = 440.f * powf(2.f, inputDampHigh - 5.f);
    reverb.reverbLowCut = 440.f * powf(2.f, reverbDampLow - 5.f);
    reverb.reverbHighCut = 440.f * powf(2.f, reverbDampHigh - 5.f);

    modSpeed = inputs[MOD_SPEED_CV_INPUT].getVoltage() * params[MOD_SPEED_CV_PARAM].getValue() * 0.1f;
    modSpeed += params[MOD_SPEED_PARAM].getValue();
    modSpeed = clamp(modSpeed, modSpeedMin, modSpeedMax);
    modSpeed *= modSpeed;
    modSpeed = modSpeed * 99.f + 1.f;

    modShape = inputs[MOD_SHAPE_CV_INPUT].getVoltage() * params[MOD_SHAPE_CV_PARAM].getValue() * 0.1f;
    modShape += params[MOD_SHAPE_PARAM].getValue();
    modShape = rescale(modShape, 0.f, 1.f, modShapeMin, modShapeMax);
    modShape = clamp(modShape, modShapeMin, modShapeMax);

    modDepth = inputs[MOD_DEPTH_CV_INPUT].getVoltage() * params[MOD_DEPTH_CV_PARAM].getValue();
    modDepth = rescale(modDepth, 0.f, 10.f, modDepthMin, modDepthMax);
    modDepth += params[MOD_DEPTH_PARAM].getValue();
    modDepth = clamp(modDepth, modDepthMin, modDepthMax);

    reverb.modSpeed = modSpeed;
    reverb.modDepth = modDepth;
    reverb.setModShape(modShape);

    leftInput = inputs[LEFT_INPUT].getVoltageSum();
    rightInput = inputs[RIGHT_INPUT].getVoltageSum();
    if(inputs[LEFT_INPUT].isConnected() == false && inputs[RIGHT_INPUT].isConnected() == true) {
        leftInput = inputs[RIGHT_INPUT].getVoltageSum();
    }
    else if(inputs[LEFT_INPUT].isConnected() == true && inputs[RIGHT_INPUT].isConnected() == false) {
        rightInput = inputs[LEFT_INPUT].getVoltageSum();
    }
    leftInput = clamp(leftInput, -10.f, 10.f);
    rightInput = clamp(rightInput, -10.f, 10.f);

    inputSensitivity = inputSensitivityState ? 0.125893f : 1.f;
    reverb.process(leftInput * 0.1f * inputSensitivity * envelope._value,
                   rightInput * 0.1f * inputSensitivity * envelope._value);

    dry = inputs[DRY_CV_INPUT].getVoltage() * params[DRY_CV_PARAM].getValue();
    dry += params[DRY_PARAM].getValue();
    dry = clamp(dry, 0.f, 1.f);

    wet = inputs[WET_CV_INPUT].getVoltage() * params[WET_CV_PARAM].getValue();
    wet += params[WET_PARAM].getValue();
    wet = clamp(wet, 0.f, 1.f) * 10.f;

    leftOutput = leftInput * dry + reverb.leftOut * wet * envelope._value;
    rightOutput = rightInput * dry + reverb.rightOut * wet * envelope._value;

    if(outputSaturationState) {
        outputs[LEFT_OUTPUT].setVoltage(tanhDriveSignal(leftOutput * 0.111f, 0.95f) * 9.999f);
        outputs[RIGHT_OUTPUT].setVoltage(tanhDriveSignal(rightOutput * 0.111f, 0.95f) * 9.999f);
    }
    else {
        outputs[LEFT_OUTPUT].setVoltage(clamp(leftOutput, -10.f, 10.f));
        outputs[RIGHT_OUTPUT].setVoltage(clamp(rightOutput, -10.f, 10.f));
    }
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
    json_object_set_new(rootJ, "outputSaturation", json_integer((int)outputSaturationState));
    json_object_set_new(rootJ, "dspMode", json_integer((int)dspModeState));
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
    outputSaturationState = json_integer_value(outputSaturationJ);

    json_t *dspModeJ = json_object_get(rootJ, "dspMode");
    dspModeState = json_integer_value(dspModeJ);
}

void Plateau::onReset(const ResetEvent& e) {
    params[FREEZE_TOGGLE_PARAM].setValue(0.f);
    params[TUNED_MODE_PARAM].setValue(0.f);
    params[DIFFUSE_INPUT_PARAM].setValue(1.f);
    Module::onReset(e);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

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
    module->outputSaturationState = outputSaturationState;
}

void PlateauOutputSaturationItem::step() {
    rightText = (module->outputSaturationState == outputSaturationState) ? "✔" : "";
    MenuItem::step();
}

void PlateauDSPModeItem::onAction(const event::Action &e) {
    module->dspModeState = dspModeState;
}

void PlateauDSPModeItem::step() {
    rightText = (module->dspModeState == dspModeState) ? "✔" : "";
    MenuItem::step();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PlateauWidget::PlateauWidget(Plateau* module) {
    setModule(module);

    darkPanel = new SvgPanel;
    darkPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PlateauPanelDark.svg")));
    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PlateauPanelLight.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }
    setPanel(darkPanel);

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Make jacks
    addInput(createInput<PJ301MDarkSmall>(leftInputPos, module, Plateau::LEFT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(rightInputPos, module, Plateau::RIGHT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(dryCVPos, module, Plateau::DRY_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(wetCVPos, module, Plateau::WET_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(preDelayCVPos, module, Plateau::PRE_DELAY_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(inputLowDampCVPos, module, Plateau::INPUT_LOW_DAMP_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(inputHighDampCVPos, module, Plateau::INPUT_HIGH_DAMP_CV_INPUT));

    addInput(createInput<PJ301MDarkSmall>(sizeCVPos, module, Plateau::SIZE_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(diffCVPos, module, Plateau::DIFFUSION_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(decayCVPos, module, Plateau::DECAY_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(reverbLowDampCVPos, module, Plateau::REVERB_LOW_DAMP_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(reverbHighDampCVPos, module, Plateau::REVERB_HIGH_DAMP_CV_INPUT));

    addInput(createInput<PJ301MDarkSmall>(modRateCVPos, module, Plateau::MOD_SPEED_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(modShapeCVPos, module, Plateau::MOD_SHAPE_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(modDepthCVPos, module, Plateau::MOD_DEPTH_CV_INPUT));

    addInput(createInput<PJ301MDarkSmall>(holdCVPos, module, Plateau::FREEZE_CV_INPUT));
    addInput(createInput<PJ301MDarkSmall>(clearCVPos, module, Plateau::CLEAR_CV_INPUT));

    addOutput(createOutput<PJ301MDarkSmallOut>(leftOutputPos, module, Plateau::LEFT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(rightOutputPos, module, Plateau::RIGHT_OUTPUT));

    // Make knobs

    float minAngle = -0.77f * M_PI;
    float maxAngle = 0.77f * M_PI;
    addParam(createValleyKnob<RoganMedSmallWhite>(dryPos, module, Plateau::DRY_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedSmallWhite>(wetPos, module, Plateau::WET_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganSmallWhite>(preDelayPos, module, Plateau::PRE_DELAY_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedGreen>(inputLowDampPos, module, Plateau::INPUT_LOW_DAMP_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedGreen>(inputHighDampPos, module, Plateau::INPUT_HIGH_DAMP_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));

    addParam(createValleyKnob<RoganMedBlue>(sizePos, module, Plateau::SIZE_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedBlue>(diffPos, module, Plateau::DIFFUSION_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedBlue>(decayPos, module, Plateau::DECAY_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedGreen>(reverbLowDampPos, module, Plateau::REVERB_LOW_DAMP_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedGreen>(reverbHighDampPos, module, Plateau::REVERB_HIGH_DAMP_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));

    addParam(createValleyKnob<RoganMedRed>(modRatePos, module, Plateau::MOD_SPEED_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedRed>(modDepthPos, module, Plateau::MOD_DEPTH_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));
    addParam(createValleyKnob<RoganMedRed>(modShapePos, module, Plateau::MOD_SHAPE_PARAM, minAngle, maxAngle, DynamicKnobMotion::SMOOTH_MOTION));

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
                                                          module, &PlateauOutputSaturationItem::outputSaturationState, 0));
    menu->addChild(construct<PlateauOutputSaturationItem>(&MenuItem::text, "On", &PlateauOutputSaturationItem::module,
                                                          module, &PlateauOutputSaturationItem::outputSaturationState, 1));

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "DSP Mode"));
    menu->addChild(construct<PlateauDSPModeItem>(&MenuItem::text, "Real-time", &PlateauDSPModeItem::module,
                                                          module, &PlateauDSPModeItem::dspModeState, 0));
    menu->addChild(construct<PlateauDSPModeItem>(&MenuItem::text, "Efficient (Short Latency)", &PlateauDSPModeItem::module,
                                                          module, &PlateauDSPModeItem::dspModeState, 1));
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
