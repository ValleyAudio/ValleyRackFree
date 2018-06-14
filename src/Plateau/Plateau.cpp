#include "Plateau.hpp"

Plateau::Plateau() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    reverb.setSampleRate(engineGetSampleRate());
}

void Plateau::step() {
    // Freeze
    if(inputs[HOLD_CV_INPUT].value > 0.5f || params[FREEZE_PARAM].value > 0.5f) {
        freeze = 1;
    }
    else if(inputs[HOLD_CV_INPUT].value < 0.5f && params[FREEZE_PARAM].value < 0.5f) {
        freeze = 0;
    }

    if(freeze && !frozen) {
        frozen = true;
        reverb.freeze();
        lights[FREEZE_LIGHT].value = 10.f;
    }
    else if(!freeze && frozen){
        frozen = false;
        reverb.unFreeze();
        lights[FREEZE_LIGHT].value = 0.f;
    }

    // Clear
    if(inputs[CLEAR_CV_INPUT].value > 0.5f || params[CLEAR_PARAM].value > 0.5f) {
        clear = 1;
    }
    else if(inputs[CLEAR_CV_INPUT].value < 0.5f && params[CLEAR_PARAM].value < 0.5f) {
        clear = 0;
    }

    if(clear && !cleared) {
        cleared = true;
        reverb.clear();
        lights[CLEAR_LIGHT].value = 10.f;
    }
    else if(!clear && cleared){
        cleared = false;
        lights[CLEAR_LIGHT].value = 0.f;
    }

    size = inputs[SIZE_CV_INPUT].value * params[SIZE_CV_PARAM].value * 0.1f;
    size += params[SIZE_PARAM].value;
    size *= size;
    size = rescale(size, 0.f, 1.f, sizeMin, sizeMax);
    size = clamp(size, sizeMin, sizeMax);
    reverb.setTimeScale(size);

    diffusion = inputs[DIFFUSION_CV_INPUT].value * params[DIFFUSION_CV_PARAM].value;
    diffusion += params[DIFFUSION_PARAM].value;
    diffusion = clamp(diffusion, 0.f, 10.f);
    reverb.plateDiffusion1 = rescale(diffusion, 0.f, 10.f, 0.f, -0.7f);
    reverb.plateDiffusion2 = rescale(diffusion, 0.f, 10.f, 0.f, 0.5f);

    decay = rescale(inputs[DECAY_CV_INPUT].value * params[DECAY_CV_PARAM].value, 0.f, 10.f, 0.1f, 0.999f);
    decay += params[DECAY_PARAM].value;
    decay = clamp(decay, 0.1f, 0.999f);
    decay = 1.f - decay;
    decay = 1.f - decay * decay;

    inputDampLow = inputs[INPUT_LOW_DAMP_CV_INPUT].value * params[INPUT_LOW_DAMP_CV_PARAM].value;
    inputDampLow += params[INPUT_LOW_DAMP_PARAM].value;
    inputDampLow = clamp(inputDampLow, 0.f, 10.f);
    inputDampLow = 10.f - inputDampLow;

    inputDampHigh = inputs[INPUT_HIGH_DAMP_CV_INPUT].value * params[INPUT_HIGH_DAMP_CV_PARAM].value;
    inputDampHigh += params[INPUT_HIGH_DAMP_PARAM].value;
    inputDampHigh = clamp(inputDampHigh, 0.f, 10.f);

    reverbDampLow = inputs[REVERB_LOW_DAMP_CV_INPUT].value * params[REVERB_LOW_DAMP_CV_PARAM].value;
    reverbDampLow += params[REVERB_LOW_DAMP_PARAM].value;
    reverbDampLow = clamp(reverbDampLow, 0.f, 10.f);
    reverbDampLow = 10.f - reverbDampLow;

    reverbDampHigh = inputs[REVERB_HIGH_DAMP_CV_INPUT].value * params[REVERB_HIGH_DAMP_CV_PARAM].value;
    reverbDampHigh += params[REVERB_HIGH_DAMP_PARAM].value;
    reverbDampHigh = clamp(reverbDampHigh, 0.f, 10.f);

    reverb.decay = decay;
    reverb.inputLowCut = 440.f * powf(2.f, inputDampLow - 5.f);
    reverb.inputHighCut = 440.f * powf(2.f, inputDampHigh - 5.f);
    reverb.reverbLowCut = 440.f * powf(2.f, reverbDampLow - 5.f);
    reverb.reverbHighCut = 440.f * powf(2.f, reverbDampHigh - 5.f);

    modSpeed = inputs[MOD_SPEED_CV_INPUT].value * params[MOD_SPEED_CV_PARAM].value * 0.1f;
    modSpeed += params[MOD_SPEED_PARAM].value;
    modSpeed = clamp(modSpeed, modSpeedMin, modSpeedMax);
    modSpeed *= modSpeed;
    modSpeed = modSpeed * 99.f + 1.f;

    modShape = inputs[MOD_SHAPE_CV_INPUT].value * params[MOD_DEPTH_CV_PARAM].value * 0.1f;
    modShape += params[MOD_SHAPE_PARAM].value;
    modShape = rescale(modShape, 0.f, 1.f, modShapeMin, modShapeMax);
    modShape = clamp(modShape, modShapeMin, modShapeMax);

    modDepth = inputs[MOD_DEPTH_CV_INPUT].value * params[MOD_DEPTH_CV_PARAM].value;
    modDepth = rescale(modDepth, 0.f, 10.f, modDepthMin, modDepthMax);
    modDepth += params[MOD_DEPTH_PARAM].value;
    modDepth = clamp(modDepth, modDepthMin, modDepthMax);

    reverb.modSpeed = modSpeed;
    reverb.modDepth = modDepth;
    reverb.setModShape(modShape);

    reverb.process(inputs[LEFT_INPUT].value / 10.f, inputs[RIGHT_INPUT].value / 10.f);
    outputs[LEFT_OUTPUT].value = inputs[LEFT_INPUT].value * params[DRY_PARAM].value;
    outputs[RIGHT_OUTPUT].value = inputs[LEFT_INPUT].value * params[DRY_PARAM].value;
    outputs[LEFT_OUTPUT].value += reverb.leftOut * params[WET_PARAM].value * 10.f;
    outputs[RIGHT_OUTPUT].value += reverb.rightOut * params[WET_PARAM].value * 10.f;
}

void Plateau::onSampleRateChange() {
    reverb.setSampleRate(engineGetSampleRate());
}

json_t* Plateau::toJson()  {
    json_t *rootJ = json_object();
    return rootJ;
}

void Plateau::fromJson(json_t *rootJ) {
    json_object_set_new(rootJ, "frozen", json_integer(freeze));
}

PlateauWidget::PlateauWidget(Plateau* module) : ModuleWidget(module) {
    {
        DynamicPanelWidget *panel = new DynamicPanelWidget();
        panel->addPanel(SVG::load(assetPlugin(plugin, "res/PlateauDark.svg")));
        box.size = panel->box.size;
        panel->mode = &module->panelStyle;
        addChild(panel);
    }

    // Make jacks
    addInput(Port::create<PJ301MDarkSmall>(module->leftInputPos, Port::INPUT, module, Plateau::LEFT_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->rightInputPos, Port::INPUT, module, Plateau::RIGHT_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->dryCVPos, Port::INPUT, module, Plateau::DRY_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->wetCVPos, Port::INPUT, module, Plateau::WET_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->inputLowDampCVPos, Port::INPUT, module, Plateau::INPUT_LOW_DAMP_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->inputHighDampCVPos, Port::INPUT, module, Plateau::INPUT_HIGH_DAMP_CV_INPUT));

    addInput(Port::create<PJ301MDarkSmall>(module->sizeCVPos, Port::INPUT, module, Plateau::SIZE_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->diffCVPos, Port::INPUT, module, Plateau::DIFFUSION_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->decayCVPos, Port::INPUT, module, Plateau::DECAY_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->reverbLowDampCVPos, Port::INPUT, module, Plateau::REVERB_LOW_DAMP_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->reverbHighDampCVPos, Port::INPUT, module, Plateau::REVERB_HIGH_DAMP_CV_INPUT));

    addInput(Port::create<PJ301MDarkSmall>(module->modRateCVPos, Port::INPUT, module, Plateau::MOD_SPEED_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->modShapeCVPos, Port::INPUT, module, Plateau::MOD_SHAPE_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->modDepthCVPos, Port::INPUT, module, Plateau::MOD_DEPTH_CV_INPUT));

    addInput(Port::create<PJ301MDarkSmall>(module->holdCVPos, Port::INPUT, module, Plateau::HOLD_CV_INPUT));
    addInput(Port::create<PJ301MDarkSmall>(module->clearCVPos, Port::INPUT, module, Plateau::CLEAR_CV_INPUT));

    addOutput(Port::create<PJ301MDarkSmallOut>(module->leftOutputPos, Port::OUTPUT, module, Plateau::LEFT_OUTPUT));
    addOutput(Port::create<PJ301MDarkSmallOut>(module->rightOutputPos, Port::OUTPUT, module, Plateau::RIGHT_OUTPUT));

    // Make knobs
    addParam(ParamWidget::create<RoganMedWhite>(module->dryPos, module, Plateau::DRY_PARAM, 0.0f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganMedWhite>(module->wetPos, module, Plateau::WET_PARAM, 0.0f, 1.f, 0.5f));
    addParam(ParamWidget::create<RoganSmallWhite>(module->preDelayPos, module, Plateau::PRE_DELAY_PARAM, 0.f, 10.f, 0.f));
    addParam(ParamWidget::create<RoganMedGreen>(module->inputLowDampPos, module, Plateau::INPUT_LOW_DAMP_PARAM, 0.f, 10.f, 10.f));
    addParam(ParamWidget::create<RoganMedGreen>(module->inputHighDampPos, module, Plateau::INPUT_HIGH_DAMP_PARAM, 0.f, 10.f, 10.f));

    addParam(ParamWidget::create<RoganMedBlue>(module->sizePos, module, Plateau::SIZE_PARAM, 0.f, 1.f, 0.5f));
    addParam(ParamWidget::create<RoganMedBlue>(module->diffPos, module, Plateau::DIFFUSION_PARAM, 0.f, 10.f, 10.f));
    addParam(ParamWidget::create<RoganMedBlue>(module->decayPos, module, Plateau::DECAY_PARAM, 0.1f, 0.9999f, 0.5f));
    addParam(ParamWidget::create<RoganMedGreen>(module->reverbLowDampPos, module, Plateau::REVERB_LOW_DAMP_PARAM, 0.0f, 10.f, 10.f));
    addParam(ParamWidget::create<RoganMedGreen>(module->reverbHighDampPos, module, Plateau::REVERB_HIGH_DAMP_PARAM, 0.0f, 10.f, 10.f));

    addParam(ParamWidget::create<RoganMedRed>(module->modRatePos, module, Plateau::MOD_SPEED_PARAM, 0.f, 1.f, 0.f));
    addParam(ParamWidget::create<RoganMedRed>(module->modDepthPos, module, Plateau::MOD_DEPTH_PARAM, 1.f, 16.f, 0.5f));
    addParam(ParamWidget::create<RoganMedRed>(module->modShapePos, module, Plateau::MOD_SHAPE_PARAM, 0.f, 1.f, 0.5f));

    // Make Attenuverters
    addParam(ParamWidget::create<RoganSmallWhite>(module->dryAttenPos, module, Plateau::DRY_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallWhite>(module->wetAttenPos, module, Plateau::WET_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallGreen>(module->inputLowDampAttenPos, module, Plateau::INPUT_LOW_DAMP_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallGreen>(module->inputHighDampAttenPos, module, Plateau::INPUT_HIGH_DAMP_CV_PARAM, -1.f, 1.f, 1.f));

    addParam(ParamWidget::create<RoganSmallBlue>(module->sizeAttenPos, module, Plateau::SIZE_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallBlue>(module->diffAttenPos, module, Plateau::DIFFUSION_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallBlue>(module->decayAttenPos, module, Plateau::DECAY_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallGreen>(module->reverbLowDampAttenPos, module, Plateau::REVERB_LOW_DAMP_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallGreen>(module->reverbHighDampAttenPos, module, Plateau::REVERB_HIGH_DAMP_CV_PARAM, -1.f, 1.f, 1.f));

    addParam(ParamWidget::create<RoganSmallRed>(module->modRateAttenPos, module, Plateau::MOD_SPEED_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallRed>(module->modShapeAttenPos, module, Plateau::MOD_DEPTH_CV_PARAM, -1.f, 1.f, 1.f));
    addParam(ParamWidget::create<RoganSmallRed>(module->modDepthAttenPos, module, Plateau::MOD_SHAPE_CV_PARAM, -1.f, 1.f, 1.f));


    addParam(ParamWidget::create<LightLEDButton>(Vec(7.875, 244.85), module, Plateau::FREEZE_PARAM, 0.f, 10.f, 0.f));
    addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(10.375, 247.35), module, Plateau::FREEZE_LIGHT));

    addParam(ParamWidget::create<LightLEDButton>(Vec(157.875, 244.85), module, Plateau::CLEAR_PARAM, 0.f, 10.f, 0.f));
    addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(160.375, 247.35), module, Plateau::CLEAR_LIGHT));
}


Model *modelPlateau = Model::create<Plateau, PlateauWidget>("Valley", "Plateau", "Plateau",
                                                                  REVERB_TAG);
