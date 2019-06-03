#include "Interzone.hpp"

Interzone::Interzone() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Interzone::GLIDE_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::PITCH_MOD_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::PW_PARAM, 0.5f, 0.f, 0.5f);
    configParam(Interzone::PW_MOD_PARAM, 0.f, 0.5f, 0.f);

    configParam(Interzone::COARSE_PARAM, 0.f, 2.f, 1.f);
    configParam(Interzone::FINE_PARAM, -0.0833333f, 0.0833333f, 0.f);
    configParam(Interzone::PITCH_MOD_ENV_POL_PARAM, 0.f, 1.f, 1.f);
    configParam(Interzone::PITCH_MOD_SOURCE_PARAM, 0.0f, 1.f, 0.f);
    configParam(Interzone::PW_MOD_ENV_POL_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::COARSE_MODE_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::PW_MOD_SOURCE_PARAM, 0.0f, 2.f, 1.f);
    configParam(Interzone::SUB_OCTAVE_PARAM, 0.f, 6.f, 3.f);

    configParam(Interzone::SAW_LEVEL_PARAM, 0.f, 1.f, 0.8f);
    configParam(Interzone::PULSE_LEVEL_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::SUB_LEVEL_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::SUB_WAVE_PARAM, 0.0f, 2.f, 1.f);
    configParam(Interzone::NOISE_TYPE_PARAM, 0.0f, 1.f, 0.0f);
    configParam(Interzone::NOISE_LEVEL_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::EXT_LEVEL_PARAM, 0.f, 1.f, 0.f);

    configParam(Interzone::FILTER_CUTOFF_PARAM, 0.f, 10.0f, 10.f);
    configParam(Interzone::FILTER_Q_PARAM, 0.f, 10.f, 0.f);
    configParam(Interzone::FILTER_HPF_PARAM, 0.f, 10.0f, 0.f);
    configParam(Interzone::FILTER_POLES_PARAM, 0.f, 1.f, 1.f);
    configParam(Interzone::FILTER_ENV_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::FILTER_MOD_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::FILTER_VOCT_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::FILTER_ENV_POL_PARAM, 0.f, 1.f, 1.f);
    configParam(Interzone::FILTER_CV_1_PARAM, -1.0f, 1.f, 0.f);
    configParam(Interzone::FILTER_CV_2_PARAM, -1.0f, 1.f, 0.f);

    configParam(Interzone::ENV_ATTACK_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::ENV_DECAY_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::ENV_SUSTAIN_PARAM, 0.f, 1.f, 1.f);
    configParam(Interzone::ENV_RELEASE_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::ENV_LENGTH_PARAM, 0.0f, 1.f, 0.f);
    configParam(Interzone::ENV_CYCLE_PARAM, 0.0f, 1.f, 0.f);
    configParam(Interzone::ENV_MANUAL_PARAM, 0.f, 1.f, 0.f);

    configParam(Interzone::LFO_RATE_PARAM, 0.f, 11.f, 0.f);
    configParam(Interzone::LFO_FINE_PARAM, -0.5f, 0.5f, 0.f);
    configParam(Interzone::LFO_SLEW_PARAM, 0.f, 1.f, 0.f);
    configParam(Interzone::VCA_SOURCE_PARAM, 0.0f, 1.f, 0.0f);
    configParam(Interzone::VCA_LEVEL_CV_PARAM, -1.f, 1.f, 0.f);

    calcGTable(APP->engine->getSampleRate());
    filter.setSampleRate(APP->engine->getSampleRate());
    filter.setCutoff(5.f);
    filter.setNLP(true);
    highpass.setSampleRate(APP->engine->getSampleRate());

    lfoSlew.setSampleRate(APP->engine->getSampleRate());
    lfoSlew.setCutoffFreq(14000.f);
    osc.setSampleRate(APP->engine->getSampleRate());
    glide.setSampleRate(APP->engine->getSampleRate());

    lfo.setSampleRate(APP->engine->getSampleRate());
    lfoSlew.setSampleRate(APP->engine->getSampleRate());
    lfoSlew.setCutoffFreq(14000.f);

    gateSlew.setSampleRate(APP->engine->getSampleRate());
    gateSlew.setCutoffFreq(90.f);

    env.setSampleRate(APP->engine->getSampleRate());
    sampleAndHold = 0.f;
}

void Interzone::process(const ProcessArgs &args) {
    lfo.setFrequency(0.1f * powf(2.f, params[LFO_RATE_PARAM].getValue() + params[LFO_FINE_PARAM].getValue() + inputs[LFO_RATE_INPUT].getVoltage()));
    lfo.sync(inputs[LFO_SYNC_INPUT].getVoltage());
    lfo.trigger(inputs[LFO_TRIG_INPUT].getVoltage());
    lfo.process();
    pink.process();
    noise = params[NOISE_TYPE_PARAM].getValue() > 0.5f ? pink.getValue() : lfo.out[DLFO::NOISE_WAVE];

    outputs[LFO_SINE_OUTPUT].setVoltage(lfo.out[DLFO::SINE_WAVE] * 5.f);
    outputs[LFO_TRI_OUTPUT].setVoltage(lfo.out[DLFO::TRI_WAVE] * 5.f);
    outputs[LFO_SAW_UP_OUTPUT].setVoltage(lfo.out[DLFO::SAW_UP_WAVE] * 5.f);
    outputs[LFO_SAW_DOWN_OUTPUT].setVoltage(lfo.out[DLFO::SAW_DOWN_WAVE] * 5.f);
    outputs[LFO_PULSE_OUTPUT].setVoltage(lfo.out[DLFO::SQUARE_WAVE] * 5.f);
    outputs[LFO_SH_OUTPUT].setVoltage(lfo.out[DLFO::SH_WAVE] * 5.f);
    outputs[LFO_NOISE_OUTPUT].setVoltage(noise * 5.f);

    lfoSlew.setCutoffFreq(1760.f * pow(2.f, (params[LFO_SLEW_PARAM].getValue() * 2.f) * -6.f));
    lfoSlew.input = lfo.out[(int)params[LFO_WAVE_PARAM].getValue()];
    lfoSlew.process();
    lfoValue = params[LFO_SLEW_PARAM].getValue() > 0.001f ? lfoSlew.output : lfoSlew.input;
    lights[LFO_LIGHT].value = lfoValue;

    // CV Input conditioning
    gateLevel = (inputs[GATE_INPUT].getVoltage() + params[ENV_MANUAL_PARAM].getValue()) > 0.5f ? 1.f : 0.f;
    lights[ENV_LIGHT].value = gateLevel;
    env.attackTime = params[ENV_ATTACK_PARAM].getValue();
    env.decayTime = params[ENV_DECAY_PARAM].getValue();
    env.sustain = params[ENV_SUSTAIN_PARAM].getValue();
    env.releaseTime = params[ENV_RELEASE_PARAM].getValue();
    env.loop = params[ENV_CYCLE_PARAM].getValue() > 0.5f ? true : false;
    env.timeScale = params[ENV_LENGTH_PARAM].getValue() > 0.5f ? 0.1f : 1.f;
    env.process(gateLevel, inputs[TRIG_INPUT].getVoltage());

    pitch = params[COARSE_MODE_PARAM].getValue() > 0.5f ? semitone(params[COARSE_PARAM].getValue() + 0.04) : params[COARSE_PARAM].getValue();
    pitch -= 1.f;
    pitch += (int)params[OCTAVE_PARAM].getValue() + params[FINE_PARAM].getValue();
    pitch += inputs[VOCT_INPUT_1].getVoltage() + inputs[VOCT_INPUT_2].getVoltage();
    glide.setCutoffFreq(330.f * pow(2.f, (params[GLIDE_PARAM].getValue() * 2.f) * -7.f));
    glide.input = pitch;
    pitch = glide.process();

    oscPitchMod = params[PITCH_MOD_SOURCE_PARAM].getValue() > 0.5f ? (params[PITCH_MOD_ENV_POL_PARAM].getValue() * 2.f - 1.f) * env.value : lfoValue;
    osc.setFrequency(261.626f * powf(2.f, pitch + oscPitchMod * params[PITCH_MOD_PARAM].getValue() * params[PITCH_MOD_PARAM].getValue()));

    switch((int)params[PW_MOD_SOURCE_PARAM].getValue()) {
        case 0: pwm = inputs[PW_MOD_INPUT].getVoltage() * -0.1f; break;
        case 1: pwm = (lfoValue * -0.5f - 0.5f); break;
        case 2: pwm = -(params[PW_MOD_ENV_POL_PARAM].getValue() * 2.f - 1.f) * env.value;
    }

    pwm *= params[PW_MOD_PARAM].getValue();
    pwm += params[PW_PARAM].getValue();
    osc._pwm = clamp(pwm, 0.0f, 0.5f);
    osc.setSubOctave((int)params[SUB_OCTAVE_PARAM].getValue());

    filterCutoff = env.value * (params[FILTER_ENV_POL_PARAM].getValue() * 2.f - 1.f) * params[FILTER_ENV_PARAM].getValue() * 10.0f;
    filterCutoff += lfoValue * params[FILTER_MOD_PARAM].getValue() * params[FILTER_MOD_PARAM].getValue() * 5.f;
    filterCutoff += pitch * params[FILTER_VOCT_PARAM].getValue();
    filterCutoff += inputs[FILTER_CUTOFF_INPUT_1].getVoltage() * params[FILTER_CV_1_PARAM].getValue();
    filterCutoff += inputs[FILTER_CUTOFF_INPUT_2].getVoltage() * params[FILTER_CV_2_PARAM].getValue();
    filterCutoff += params[FILTER_CUTOFF_PARAM].getValue();
    filter.setCutoff(filterCutoff);
    filter.setQ(params[FILTER_Q_PARAM].getValue() + inputs[FILTER_RES_INPUT].getVoltage());
    filter.set4Pole(params[FILTER_POLES_PARAM].getValue());

    outputs[ENV_POSITIVE_OUTPUT].setVoltage(env.value * 5.f);
    outputs[ENV_NEGATIVE_OUTPUT].setVoltage(env.value * -5.f);

    // Main synth process
    osc.process();
    outputs[SAW_OUTPUT].setVoltage(osc._saw * 5.f);
    outputs[PULSE_OUTPUT].setVoltage(osc._pulse * 5.f);
    subWave = params[SUB_WAVE_PARAM].getValue() > 1.f ? osc._subSaw : osc._subPulse;
    params[SUB_WAVE_PARAM].getValue() < 1.f ? osc.setSubWave(2) : osc.setSubWave(1);
    outputs[SUB_OUTPUT].setVoltage(subWave * 5.f);

    mix = osc._saw * params[SAW_LEVEL_PARAM].getValue();
    mix += osc._pulse * params[PULSE_LEVEL_PARAM].getValue();
    mix += subWave * params[SUB_LEVEL_PARAM].getValue();
    mix += noise * params[NOISE_LEVEL_PARAM].getValue();
    mix += inputs[EXT_INPUT].getVoltage() * params[EXT_LEVEL_PARAM].getValue();
    outputs[MIX_OUTPUT].setVoltage(mix);
    mix *= 2.f;

    highpass.setCutoffFreq(440.f * powf(2.0, params[FILTER_HPF_PARAM].getValue() - 5.f));
    highpass.input = filter.process(mix + lfo.out[DLFO::NOISE_WAVE] * 8e-5f) * 5.f;
    output = highpass.process();
    outputs[FILTER_OUTPUT].setVoltage(highpass.output);

    gateSlew.input = gateLevel;
    gateSlew.process();
    outputLevel = params[VCA_SOURCE_PARAM].getValue() > 0.5f ? gateSlew.output : env.value;
    outputLevel += inputs[VCA_LEVEL_CV_INPUT].getVoltage() * params[VCA_LEVEL_CV_PARAM].getValue() * 0.1f;
    outputLevel = clamp(outputLevel, -1.f, 1.f);
    outputs[VCA_OUTPUT].setVoltage(output * outputLevel);
}

void Interzone::onSampleRateChange() {
    calcGTable(APP->engine->getSampleRate());
    osc.setSampleRate(APP->engine->getSampleRate());
    filter.setSampleRate(APP->engine->getSampleRate());
    highpass.setSampleRate(APP->engine->getSampleRate());
    lfo.setSampleRate(APP->engine->getSampleRate());
    lfoSlew.setSampleRate(APP->engine->getSampleRate());
    gateSlew.setSampleRate(APP->engine->getSampleRate());
    env.setSampleRate(APP->engine->getSampleRate());
    glide.setSampleRate(APP->engine->getSampleRate());
    pink.setSampleRate(APP->engine->getSampleRate());
}

json_t* Interzone::dataToJson()  {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
    return rootJ;
}

void Interzone::dataFromJson(json_t *rootJ) {
    json_t *panelStyleJ = json_object_get(rootJ, "panelStyle");
    panelStyle = json_integer_value(panelStyleJ);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void InterzonePanelStyleItem::onAction(const event::Action &e) {
    module->panelStyle = panelStyle;
}

void InterzonePanelStyleItem::step() {
    rightText = (module->panelStyle == panelStyle) ? "✔" : "";
    MenuItem::step();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

InterzoneWidget::InterzoneWidget(Interzone* module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/InterzonePanelDark.svg")));

    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/InterzonePanelLight.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Params

    addParam(createParam<OrangeSlider>(VCOGlideSliderPos, module, Interzone::GLIDE_PARAM));
    addParam(createParam<OrangeSlider>(VCOModSliderPos, module, Interzone::PITCH_MOD_PARAM));
    addParam(createParam<OrangeSlider>(VCOWidthSliderPos, module, Interzone::PW_PARAM));
    addParam(createParam<OrangeSlider>(VCOPWMSliderPos, module, Interzone::PW_MOD_PARAM));
    addParam(createValleyKnob<RoganMedOrange>(VCOOctavePos, module, Interzone::OCTAVE_PARAM, -2.f,
                                              2.f, 0.f, octaveMinAngle, octaveMaxAngle,
                                              DynamicKnobMotion::SNAP_MOTION));

    addParam(createParam<RoganSmallOrange>(VCOCoarsePos, module, Interzone::COARSE_PARAM));
    addParam(createParam<RoganSmallOrange>(VCOFinePos, module, Interzone::FINE_PARAM));
    addParam(createParam<CKSS>(VCOModEnvPolPos, module, Interzone::PITCH_MOD_ENV_POL_PARAM));
    addParam(createParam<CKSS>(VCOModSourcePos, module, Interzone::PITCH_MOD_SOURCE_PARAM));
    addParam(createParam<CKSS>(VCOPWMEnvPolPos, module, Interzone::PW_MOD_ENV_POL_PARAM));
    addParam(createParam<CKSS>(VCOCoarseModePos, module, Interzone::COARSE_MODE_PARAM));
    addParam(createParam<CKSSThree>(VCOPWMSourcePos, module, Interzone::PW_MOD_SOURCE_PARAM));
    addParam(createParam<YellowStepSlider>(VCOSubOctPos, module, Interzone::SUB_OCTAVE_PARAM));

    addParam(createParam<GreenSlider>(MixerSawLevelPos, module, Interzone::SAW_LEVEL_PARAM));
    addParam(createParam<GreenSlider>(MixerPulseLevelPos, module, Interzone::PULSE_LEVEL_PARAM));
    addParam(createParam<GreenSlider>(MixerSubLevelPos, module, Interzone::SUB_LEVEL_PARAM));
    addParam(createParam<CKSSThree>(MixerSubWavePos, module, Interzone::SUB_WAVE_PARAM));
    addParam(createParam<CKSS>(MixerNoiseTypePos, module, Interzone::NOISE_TYPE_PARAM));
    addParam(createParam<GreenSlider>(MixerNoiseLevelPos, module, Interzone::NOISE_LEVEL_PARAM));
    addParam(createParam<GreenSlider>(MixerExtInLevelPos, module, Interzone::EXT_LEVEL_PARAM));

    addParam(createParam<BlueSlider>(FilterCutoffPos, module, Interzone::FILTER_CUTOFF_PARAM));
    addParam(createParam<BlueSlider>(FilterResPos, module, Interzone::FILTER_Q_PARAM));
    addParam(createParam<BlueSlider>(FilterHPFPos, module, Interzone::FILTER_HPF_PARAM));
    addParam(createParam<CKSS>(FilterPolesPos, module, Interzone::FILTER_POLES_PARAM));
    addParam(createParam<OrangeSlider>(FilterEnvPos, module, Interzone::FILTER_ENV_PARAM));
    addParam(createParam<OrangeSlider>(FilterLFOPos, module, Interzone::FILTER_MOD_PARAM));
    addParam(createParam<OrangeSlider>(FilterVOctPos, module, Interzone::FILTER_VOCT_PARAM));
    addParam(createParam<CKSS>(FilterEnvPolPos, module, Interzone::FILTER_ENV_POL_PARAM));
    addParam(createParam<RoganSmallBlue>(FilterCV1Pos, module, Interzone::FILTER_CV_1_PARAM));
    addParam(createParam<RoganSmallBlue>(FilterCV2Pos, module, Interzone::FILTER_CV_2_PARAM));

    addParam(createParam<RedSlider>(EnvAttackPos, module, Interzone::ENV_ATTACK_PARAM));
    addParam(createParam<RedSlider>(EnvDecayPos, module, Interzone::ENV_DECAY_PARAM));
    addParam(createParam<RedSlider>(EnvSustainPos, module, Interzone::ENV_SUSTAIN_PARAM));
    addParam(createParam<RedSlider>(EnvReleasePos, module, Interzone::ENV_RELEASE_PARAM));
    addParam(createParam<CKSS>(EnvLengthPos, module, Interzone::ENV_LENGTH_PARAM));
    addParam(createParam<CKSS>(EnvCyclePos, module, Interzone::ENV_CYCLE_PARAM));
    addParam(createParam<LightLEDButton>(EnvManualPos, module, Interzone::ENV_MANUAL_PARAM));
    addChild(createLight<MediumLight<RedLight>>(EnvManualPos.plus(Vec(2.5f, 2.5f)), module, Interzone::ENV_LIGHT));

    addParam(createParam<GreenSlider>(LFORatePos, module, Interzone::LFO_RATE_PARAM));
    addParam(createParam<RoganSmallOrange>(LFOFinePos, module, Interzone::LFO_FINE_PARAM));
    addParam(createParam<RoganSmallOrange>(LFOSlewPos, module, Interzone::LFO_SLEW_PARAM));
    addParam(createValleyKnob<RoganMedOrange>(LFOWavePos, module, Interzone::LFO_WAVE_PARAM,
                                              0.f, 6.f, 0.f, lfoWaveMinAngle, lfoWaveMaxAngle,
                                              DynamicKnobMotion::SNAP_MOTION));

    addParam(createParam<CKSS>(VCASourcePos, module, Interzone::VCA_SOURCE_PARAM));
    addParam(createParam<RoganSmallOrange>(VCALevelPos, module, Interzone::VCA_LEVEL_CV_PARAM));

    // Lights
    addChild(createLight<SmallLight<RedLight>>(Vec(249.244, 155.875), module, Interzone::LFO_LIGHT));

    // IO
    addInput(createInput<PJ301MDarkSmall>(VOctIn1Pos, module, Interzone::VOCT_INPUT_1));
    addInput(createInput<PJ301MDarkSmall>(VOctIn2Pos, module, Interzone::VOCT_INPUT_2));
    addInput(createInput<PJ301MDarkSmall>(PWMInPos, module, Interzone::PW_MOD_INPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(SawOutPos, module, Interzone::SAW_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(PulseOutPos, module, Interzone::PULSE_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(SubOutPos, module, Interzone::SUB_OUTPUT));

    addInput(createInput<PJ301MDarkSmall>(MixerExtInPos, module, Interzone::EXT_INPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(MixerOutPos, module, Interzone::MIX_OUTPUT));

    addInput(createInput<PJ301MDarkSmall>(LFORateInPos, module, Interzone::LFO_RATE_INPUT));
    addInput(createInput<PJ301MDarkSmall>(LFOTrigInPos, module, Interzone::LFO_TRIG_INPUT));
    addInput(createInput<PJ301MDarkSmall>(LFOSyncInPos, module, Interzone::LFO_SYNC_INPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(LFOSineOutPos, module, Interzone::LFO_SINE_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(LFOTriOutPos, module, Interzone::LFO_TRI_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(LFOSawUpPos, module, Interzone::LFO_SAW_UP_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(LFOSawDownPos, module, Interzone::LFO_SAW_DOWN_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(LFOPulseOutPos, module, Interzone::LFO_PULSE_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(LFOSHOutPos, module, Interzone::LFO_SH_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(LFONoiseOutPos, module, Interzone::LFO_NOISE_OUTPUT));

    addInput(createInput<PJ301MDarkSmall>(FilterCutoffIn1Pos, module, Interzone::FILTER_CUTOFF_INPUT_1));
    addInput(createInput<PJ301MDarkSmall>(FilterCutoffIn2Pos, module, Interzone::FILTER_CUTOFF_INPUT_2));
    addInput(createInput<PJ301MDarkSmall>(FilterResInPos, module, Interzone::FILTER_RES_INPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(FilterOutPos, module, Interzone::FILTER_OUTPUT));

    addInput(createInput<PJ301MDarkSmall>(EnvGateInPos, module, Interzone::GATE_INPUT));
    addInput(createInput<PJ301MDarkSmall>(EnvTrigInPos, module, Interzone::TRIG_INPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(EnvPositiveOutPos, module, Interzone::ENV_POSITIVE_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(EnvNegativeOutPos, module, Interzone::ENV_NEGATIVE_OUTPUT));

    addOutput(createOutput<PJ301MDarkSmallOut>(VCAOutPos, module, Interzone::VCA_OUTPUT));
    addInput(createInput<PJ301MDarkSmall>(VCALevelCVPos, module, Interzone::VCA_LEVEL_CV_INPUT));
}

void InterzoneWidget::appendContextMenu(Menu *menu) {
    Interzone *module = dynamic_cast<Interzone*>(this->module);
    assert(module);

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel style"));
    menu->addChild(construct<InterzonePanelStyleItem>(&MenuItem::text, "Dark", &InterzonePanelStyleItem::module,
                                                    module, &InterzonePanelStyleItem::panelStyle, 0));
    menu->addChild(construct<InterzonePanelStyleItem>(&MenuItem::text, "Light", &InterzonePanelStyleItem::module,
                                                      module, &InterzonePanelStyleItem::panelStyle, 1));
}

void InterzoneWidget::step() {
    if(module) {
        if(dynamic_cast<Interzone*>(module)->panelStyle == 1) {
            panel->visible = false;
            lightPanel->visible = true;
        }
        else {
            panel->visible = true;
            lightPanel->visible = false;
        }
    }
    Widget::step();
}

Model *modelInterzone = createModel<Interzone, InterzoneWidget>("Interzone");
