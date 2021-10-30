#include "Interzone.hpp"

Interzone::Interzone() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Interzone::GLIDE_PARAM, 0.f, 1.f, 0.f, "Glide");
    configParam(Interzone::PITCH_MOD_PARAM, 0.f, 1.f, 0.f, "Pitch Mod Depth");
    configParam(Interzone::PW_PARAM, 0.5f, 0.f, 0.5f, "Pulse Width");
    configParam(Interzone::PW_MOD_PARAM, 0.f, 0.5f, 0.f, "PWM Depth");
    configParam(Interzone::OCTAVE_PARAM, -2.f, 2.f, 0.f, "Octave");

    configParam(Interzone::COARSE_PARAM, 0.f, 2.f, 1.f, "Coarse Tune");
    configParam(Interzone::FINE_PARAM, -0.0833333f, 0.0833333f, 0.f, "Fine Tune");
    configParam(Interzone::PITCH_MOD_ENV_POL_PARAM, 0.f, 1.f, 1.f, "Pitch Mod Envelope Polarity");
    configParam(Interzone::PITCH_MOD_SOURCE_PARAM, 0.0f, 1.f, 0.f, "Pitch Mod Source");
    configParam(Interzone::PW_MOD_ENV_POL_PARAM, 0.f, 1.f, 0.f, "Pitch Mod Envelope Depth");
    configParam(Interzone::COARSE_MODE_PARAM, 0.f, 1.f, 0.f, "Coarse Tune Quantisation");
    configParam(Interzone::PW_MOD_SOURCE_PARAM, 0.0f, 2.f, 1.f, "PWM Source");
    configParam(Interzone::SUB_OCTAVE_PARAM, 0.f, 6.f, 3.f, "Sub Wave Octave");

    configParam(Interzone::SAW_LEVEL_PARAM, 0.f, 1.f, 0.8f, "Saw Wave Level");
    configParam(Interzone::PULSE_LEVEL_PARAM, 0.f, 1.f, 0.f, "Pulse Wave Level");
    configParam(Interzone::SUB_LEVEL_PARAM, 0.f, 1.f, 0.f, "Sub Wave Level");
    configParam(Interzone::SUB_WAVE_PARAM, 0.0f, 2.f, 1.f, "Sub Wave Type");
    configParam(Interzone::NOISE_TYPE_PARAM, 0.0f, 1.f, 0.0f, "Noise Type");
    configParam(Interzone::NOISE_LEVEL_PARAM, 0.f, 1.f, 0.f, "Noise Level");
    configParam(Interzone::EXT_LEVEL_PARAM, 0.f, 1.f, 0.f, "External Input Level");

    configParam(Interzone::FILTER_CUTOFF_PARAM, 0.f, 10.0f, 10.f, "Filter Cutoff", "V");
    configParam(Interzone::FILTER_Q_PARAM, 0.f, 10.f, 0.f, "Filter Resonance");
    configParam(Interzone::FILTER_HPF_PARAM, 0.f, 10.0f, 0.f, "High Pass Filter Cutoff");
    configParam(Interzone::FILTER_POLES_PARAM, 0.f, 1.f, 1.f, "Filter Poles");
    configParam(Interzone::FILTER_ENV_PARAM, 0.f, 1.f, 0.f, "Filter Envelope Depth");
    configParam(Interzone::FILTER_MOD_PARAM, 0.f, 1.f, 0.f, "Filter LFO Mod Depth");
    configParam(Interzone::FILTER_VOCT_PARAM, 0.f, 1.f, 0.f, "Filter V/Oct Tracking");
    configParam(Interzone::FILTER_ENV_POL_PARAM, 0.f, 1.f, 1.f, "Filter Envelope Polarity");
    configParam(Interzone::FILTER_CV_1_PARAM, -1.0f, 1.f, 0.f, "Filter Cutoff CV 1 Depth");
    configParam(Interzone::FILTER_CV_2_PARAM, -1.0f, 1.f, 0.f, "Filter Cutoff CV 2 Depth");

    configParam(Interzone::ENV_ATTACK_PARAM, 0.f, 1.f, 0.f, "Attack Time");
    configParam(Interzone::ENV_DECAY_PARAM, 0.f, 1.f, 0.f, "Decay Time");
    configParam(Interzone::ENV_SUSTAIN_PARAM, 0.f, 1.f, 1.f, "Sustain Level");
    configParam(Interzone::ENV_RELEASE_PARAM, 0.f, 1.f, 0.f, "Release Time");
    configParam(Interzone::ENV_LENGTH_PARAM, 0.0f, 1.f, 0.f, "Envelope Time Scale");
    configParam(Interzone::ENV_CYCLE_PARAM, 0.0f, 1.f, 0.f, "Envelope Cycle Enable");
    configParam(Interzone::ENV_MANUAL_PARAM, 0.f, 1.f, 0.f, "Envelope Manual Gate");

    configParam(Interzone::LFO_RATE_PARAM, 0.f, 11.f, 0.f, "LFO Rate");
    configParam(Interzone::LFO_FINE_PARAM, -0.5f, 0.5f, 0.f, "LFO Fine Tune");
    configParam(Interzone::LFO_SLEW_PARAM, 0.f, 1.f, 0.f, "LFO Slew");
    configParam(Interzone::LFO_WAVE_PARAM, 0.f, 6.f, 0.f, "LFO Wave");
    configParam(Interzone::VCA_SOURCE_PARAM, 0.0f, 1.f, 0.0f, "VCA Source");
    configParam(Interzone::VCA_LEVEL_CV_PARAM, -1.f, 1.f, 0.f, "VCA Level CV Depth");

    float initSampleRate = APP->engine->getSampleRate();
    calcGTable(initSampleRate);
    lfo.setSampleRate(initSampleRate);
    lfoSlew.setSampleRate(initSampleRate);
    lfoSlew.setCutoffFreq(14000.f);
    pink.setSampleRate(initSampleRate);

    for (int i = 0; i < kMaxNumVoiceGroups; ++i) {
        vOsc[i].setSampleRate(initSampleRate);
        vFilter[i].setSampleRate(initSampleRate);
        vHighpass[i].setSampleRate(initSampleRate);
        vEnv[i].setSampleRate(initSampleRate);
        vGateSlew[i].setSampleRate(initSampleRate);
        vGateSlew[i].setCutoffFreq(90.f);
        vGlide[i].setSampleRate(initSampleRate);
    }

    __zero = _mm_set1_ps(0.f);
    __one = _mm_set1_ps(1.f);
    __two = _mm_set1_ps(2.f);
    __negTwo = _mm_set1_ps(-2.f);
    __five = _mm_set1_ps(5.f);
    __ten = _mm_set1_ps(10.f);
    __negTen = _mm_set1_ps(-10.f);
    __half = _mm_set1_ps(0.5f);
    __quarter = _mm_set1_ps(0.25f);

    cvDivider.setDivision(16);
}

void Interzone::process(const ProcessArgs &args) {
    numActiveVoices = std::max(inputs[VOCT_INPUT_1].getChannels(),
                               inputs[VOCT_INPUT_2].getChannels());
    numActiveVoices = numActiveVoices < 1 ? 1 : numActiveVoices;
    numActiveVoiceGroups = (int) std::ceil((float) numActiveVoices / (float)kNumVoicesPerGroup);
    numActiveVoiceGroups = numActiveVoiceGroups < 1 ? 1 : numActiveVoiceGroups;

    outputs[SAW_OUTPUT].setChannels(numActiveVoices);
    outputs[PULSE_OUTPUT].setChannels(numActiveVoices);
    outputs[SUB_OUTPUT].setChannels(numActiveVoices);
    outputs[MIX_OUTPUT].setChannels(numActiveVoices);
    outputs[FILTER_OUTPUT].setChannels(numActiveVoices);
    outputs[VCA_OUTPUT].setChannels(numActiveVoices);
    outputs[ENV_POSITIVE_OUTPUT].setChannels(numActiveVoices);
    outputs[ENV_NEGATIVE_OUTPUT].setChannels(numActiveVoices);

    //// Param extraction
    if (cvDivider.process()) {
        lfo.setFrequency(0.1f * powf(2.f, params[LFO_RATE_PARAM].getValue() + params[LFO_FINE_PARAM].getValue() + inputs[LFO_RATE_INPUT].getVoltage()));
        lfoSlew.setCutoffFreq(1760.f * pow(2.f, (params[LFO_SLEW_PARAM].getValue() * 2.f) * -6.f));

        glideParam = 330.f * pow(2.f, (params[GLIDE_PARAM].getValue() * 2.f) * -7.f);

        pitchParam = params[COARSE_MODE_PARAM].getValue() > 0.5f ? semitone(params[COARSE_PARAM].getValue() + 0.04f) : params[COARSE_PARAM].getValue();
        pitchParam -= 1.f;
        pitchParam += (int)params[OCTAVE_PARAM].getValue() + params[FINE_PARAM].getValue();
        vPitchParam = _mm_set1_ps(pitchParam);

        vPulseWidth = _mm_set1_ps(params[PW_PARAM].getValue());
        vPwmDepth = _mm_set1_ps(params[PW_MOD_PARAM].getValue());
        vPwmEnvPol = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(params[PW_MOD_ENV_POL_PARAM].getValue()), __negTwo), __one);
        vPwmSource = _mm_set1_ps(params[PW_MOD_SOURCE_PARAM].getValue());
        subOctave = static_cast<int>(params[SUB_OCTAVE_PARAM].getValue());

        vSawLevel = _mm_set1_ps(params[SAW_LEVEL_PARAM].getValue());
        vPulseLevel = _mm_set1_ps(params[PULSE_LEVEL_PARAM].getValue());
        vSubLevel = _mm_set1_ps(params[SUB_LEVEL_PARAM].getValue());
        vNoiseLevel = _mm_set1_ps(params[NOISE_LEVEL_PARAM].getValue());
        vExtInLevel = _mm_set1_ps(params[EXT_LEVEL_PARAM].getValue());

        vFilterCV1Depth = _mm_set1_ps(params[FILTER_CV_1_PARAM].getValue());
        vFilterCV2Depth = _mm_set1_ps(params[FILTER_CV_2_PARAM].getValue());
        vFilterLFODepth = _mm_set1_ps(params[FILTER_MOD_PARAM].getValue());
        vFilterLFODepth = _mm_mul_ps(_mm_mul_ps(vFilterLFODepth, vFilterLFODepth), __five);
        vFilterEnvParam = _mm_set1_ps(params[FILTER_ENV_PARAM].getValue() * 10.f);
        vFilterEnvPol = _mm_sub_ps(_mm_mul_ps(_mm_set1_ps(params[FILTER_ENV_POL_PARAM].getValue()), __two), __one);
        vFilterCutoffParam = _mm_set1_ps(params[FILTER_CUTOFF_PARAM].getValue());
        vFilterQParam = _mm_set1_ps(params[FILTER_Q_PARAM].getValue());
        filterMode = static_cast<int>(params[FILTER_POLES_PARAM].getValue());
        vFilterKeyTrack = _mm_set1_ps(params[FILTER_VOCT_PARAM].getValue());

        vHpfCutoff = _mm_set1_ps(440.f * powf(2.0, params[FILTER_HPF_PARAM].getValue() - 5.f));
        vVCACVParam = _mm_set1_ps(params[VCA_LEVEL_CV_PARAM].getValue() * 0.1f);

        vAttack = params[ENV_ATTACK_PARAM].getValue();
        vDecay = params[ENV_DECAY_PARAM].getValue();
        vSustain = params[ENV_SUSTAIN_PARAM].getValue();
        vRelease = params[ENV_RELEASE_PARAM].getValue();
        for (int i = 0; i < kMaxNumVoiceGroups; ++i) {
            vGlide[i].setCutoffFreq(glideParam);
            vEnv[i].setADSR(vAttack, vDecay, vSustain, vRelease);
            vEnv[i].looping = params[ENV_CYCLE_PARAM].getValue() > 0.5f;
            vEnv[i].setTimeScale(params[ENV_LENGTH_PARAM].getValue() > 0.5f ? 10.f : 1.f);
            vHighpass[i].setCutoffFreq(vHpfCutoff);
        }
    }

    lfo.sync(inputs[LFO_SYNC_INPUT].getVoltage());
    lfo.trigger(inputs[LFO_TRIG_INPUT].getVoltage());
    lfo.process();

    lfoSlew.input = lfo.out[(int)params[LFO_WAVE_PARAM].getValue()];
    lfoSlew.process();
    lfoValue = params[LFO_SLEW_PARAM].getValue() > 0.0001f ? lfoSlew.output : lfoSlew.input;
    vLfoValue = _mm_set1_ps(lfoValue);
    lights[LFO_LIGHT].value = lfoValue;

    pink.process();
    noise = params[NOISE_TYPE_PARAM].getValue() > 0.5f ? pink.getValue() : lfo.out[DLFO::NOISE_WAVE];
    vNoise = _mm_set1_ps(noise);

    // CV Input conditioning
    vManualGate.v = _mm_set_ps(0.f, 0.f, 0.f, params[ENV_MANUAL_PARAM].getValue());
    gateLevel = (inputs[GATE_INPUT].getVoltageSum() + params[ENV_MANUAL_PARAM].getValue()) > 0.5f ? 1.f : 0.f;
    lights[ENV_LIGHT].value = clamp(gateLevel, 0.f, 1.f);

    vPitchModEnvPol = _mm_set1_ps(params[PITCH_MOD_ENV_POL_PARAM].getValue() * 2.f - 1.f);
    vPitchModSource = params[PITCH_MOD_SOURCE_PARAM].getValue() > 0.5f ? __zero : _mm_high_ps();
    vPitchModParam = _mm_set1_ps(params[PITCH_MOD_PARAM].getValue());
    vPitchModParam = _mm_mul_ps(vPitchModParam, vPitchModParam);
    vSubWidth = _mm_set1_ps(params[SUB_WAVE_PARAM].getValue() < 1.f ? 0.75f : 0.5f);

    //// Vectorised main synth process

    // Tick the CV
    int startChan = 0;
    for (int i = 0; i < numActiveVoiceGroups; ++i) {
        startChan = i * kNumVoicesPerGroup;
        vPitch = inputs[VOCT_INPUT_1].getPolyVoltageSimd<float_4>(startChan).v;
        vPitch = _mm_add_ps(vPitch, inputs[VOCT_INPUT_2].getPolyVoltageSimd<float_4>(startChan).v);

        vGate = inputs[GATE_INPUT].getPolyVoltageSimd<float_4>(startChan);
        vGate += i == 0 ? vManualGate : simd::float_4::zero();
        vTrigger = inputs[TRIG_INPUT].getPolyVoltageSimd<float_4>(startChan);
        vGateSlew[i].process(vGate.v);

        vEnv[i].process(vGate, vTrigger);
        vVCACVInput = inputs[VCA_LEVEL_CV_INPUT].getPolyVoltageSimd<float_4>(startChan).v;
        vOutputLevel[i] = params[VCA_SOURCE_PARAM].getValue() > 0.5f ? vGateSlew[i]._z : vEnv[i].env.v;
        vOutputLevel[i] = _mm_add_ps(vOutputLevel[i], _mm_mul_ps(vVCACVInput, vVCACVParam));
        vOutputLevel[i] = _mm_clamp_ps(vOutputLevel[i], __zero, __one);
        outputs[ENV_POSITIVE_OUTPUT].setVoltageSimd(vEnv[i].env * 10.f, startChan);
        outputs[ENV_NEGATIVE_OUTPUT].setVoltageSimd(vEnv[i].env * -10.f, startChan);

        vPitch = vGlide[i].process(vPitch);
        vFilterCV1In = inputs[FILTER_CUTOFF_INPUT_1].getPolyVoltageSimd<float_4>(startChan).v;
        vFilterCV2In = inputs[FILTER_CUTOFF_INPUT_2].getPolyVoltageSimd<float_4>(startChan).v;
        vFilterCV1In = _mm_mul_ps(vFilterCV1In, vFilterCV1Depth);
        vFilterCV2In = _mm_mul_ps(vFilterCV2In, vFilterCV2Depth);
        vFilterCutoff = _mm_add_ps(vFilterCutoffParam, _mm_add_ps(vFilterCV1In, vFilterCV2In));
        vFilterCutoff = _mm_add_ps(vFilterCutoff, _mm_mul_ps(vPitch, vFilterKeyTrack));
        vFilterCutoff = _mm_add_ps(vFilterCutoff, _mm_mul_ps(vLfoValue, vFilterLFODepth));
        vFilterCutoff = _mm_add_ps(vFilterCutoff, _mm_mul_ps(_mm_mul_ps(vEnv[i].env.v, vFilterEnvPol), vFilterEnvParam));
        vFilterQ = _mm_add_ps(vFilterQParam, inputs[FILTER_RES_INPUT].getPolyVoltageSimd<float_4>(startChan).v);

        vOscPitchMod = _mm_switch_ps(_mm_mul_ps(vEnv[i].env.v, vPitchModEnvPol), vLfoValue, vPitchModSource);
        vOscPitchMod = _mm_mul_ps(vOscPitchMod, vPitchModParam);
        vPitch = _mm_add_ps(vPitch, vPitchParam);
        rackSimd_vPitch = _mm_add_ps(vPitch, vOscPitchMod);
        rackSimd_vFreq = dsp::FREQ_C4 * dsp::approxExp2_taylor5(rackSimd_vPitch + 30.f) / 1073741824.f;
        vFreq = rackSimd_vFreq.v;
        vOsc[i].setFrequency(vFreq);
        vOsc[i].setSubOctave(subOctave);

        vExternalPwm = inputs[PW_MOD_INPUT].getPolyVoltageSimd<float_4>(startChan).v;
        vExternalPwm = _mm_mul_ps(vExternalPwm, _mm_set1_ps(-0.1f));
        vLfoPwm = _mm_set1_ps(lfoValue * -0.5f - 0.5f);
        vEnvPwm = _mm_mul_ps(vEnv[i].env.v, vPwmEnvPol);

        vPwm = _mm_switch_ps(vExternalPwm, vLfoPwm, _mm_cmpeq_ps(vPwmSource, _mm_set1_ps(LFO_PWM)));
        vPwm = _mm_switch_ps(vPwm, vEnvPwm, _mm_cmpeq_ps(vPwmSource, _mm_set1_ps(ENVELOPE_PWM)));
        vPwm = _mm_mul_ps(vPwm, vPwmDepth);
        vPwm = _mm_add_ps(vPwm, vPulseWidth);
        vOsc[i].__pwm = _mm_clamp_ps(vPwm, __zero, __half);
        vOsc[i].setSubWidth(vSubWidth);

        vFilter[i].setCutoff(vFilterCutoff);
        vFilter[i].setQ(vFilterQ);
        vFilter[i].setMode(filterMode);
    }

    // Tick the synth
    for (int i = 0; i < numActiveVoiceGroups; ++i) {
        startChan = i * kNumVoicesPerGroup;
        vOsc[i].process();
        vSubWave = params[SUB_WAVE_PARAM].getValue() > 1.f ? vOsc[i].__subSaw : vOsc[i].__subPulse;
        vExtInput = inputs[EXT_INPUT].getPolyVoltageSimd<float_4>(startChan).v;
        vMix = _mm_mul_ps(vOsc[i].__saw, vSawLevel);
        vMix = _mm_add_ps(vMix, _mm_mul_ps(vOsc[i].__pulse, vPulseLevel));
        vMix = _mm_add_ps(vMix, _mm_mul_ps(vSubWave, vSubLevel));
        vMix = _mm_add_ps(vMix, _mm_mul_ps(vNoise, vNoiseLevel));
        vMix = _mm_add_ps(vMix, _mm_mul_ps(vExtInput, vExtInLevel));
        vFilterInput = _mm_mul_ps(vMix, __two);

        vFilter[i].process(_mm_add_ps(vFilterInput, _mm_mul_ps(vNoise, _mm_set1_ps(8e-5f))));
        vFilterOutput = vHighpass[i].process(_mm_mul_ps(vFilter[i].out, __five));
        vOutput = _mm_mul_ps(vFilterOutput, vOutputLevel[i]);
        vOutput = _mm_clamp_ps(vOutput, __negTen, __ten);

        _mm_store_ps(outputs[SAW_OUTPUT].getVoltages(startChan), _mm_mul_ps(vOsc[i].__saw, __five));
        _mm_store_ps(outputs[PULSE_OUTPUT].getVoltages(startChan), _mm_mul_ps(vOsc[i].__pulse, __five));
        _mm_store_ps(outputs[SUB_OUTPUT].getVoltages(startChan), _mm_mul_ps(vSubWave, __five));
        _mm_store_ps(outputs[MIX_OUTPUT].getVoltages(startChan), vMix);
        _mm_store_ps(outputs[FILTER_OUTPUT].getVoltages(startChan), vFilterOutput);
        _mm_store_ps(outputs[VCA_OUTPUT].getVoltages(startChan), vOutput);
    }

    outputs[LFO_SINE_OUTPUT].setVoltage(lfo.out[DLFO::SINE_WAVE] * 5.f);
    outputs[LFO_TRI_OUTPUT].setVoltage(lfo.out[DLFO::TRI_WAVE] * 5.f);
    outputs[LFO_SAW_UP_OUTPUT].setVoltage(lfo.out[DLFO::SAW_UP_WAVE] * 5.f);
    outputs[LFO_SAW_DOWN_OUTPUT].setVoltage(lfo.out[DLFO::SAW_DOWN_WAVE] * 5.f);
    outputs[LFO_PULSE_OUTPUT].setVoltage(lfo.out[DLFO::SQUARE_WAVE] * 5.f);
    outputs[LFO_SH_OUTPUT].setVoltage(lfo.out[DLFO::SH_WAVE] * 5.f);
    outputs[LFO_NOISE_OUTPUT].setVoltage(noise * 5.f);
}

void Interzone::onSampleRateChange() {
    float newSampleRate = APP->engine->getSampleRate();
    calcGTable(newSampleRate);
    lfo.setSampleRate(newSampleRate);
    lfoSlew.setSampleRate(newSampleRate);
    pink.setSampleRate(newSampleRate);

    for (int i = 0; i < kMaxNumVoiceGroups; ++i) {
        vOsc[i].setSampleRate(newSampleRate);
        vFilter[i].setSampleRate(newSampleRate);
        vHighpass[i].setSampleRate(newSampleRate);
        vEnv[i].setSampleRate(newSampleRate);
        vGateSlew[i].setSampleRate(newSampleRate);
        vGlide[i].setSampleRate(newSampleRate);
    }
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

    darkPanel = new SvgPanel;
    darkPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/InterzonePanelDark.svg")));
    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/InterzonePanelLight.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }
    setPanel(darkPanel);

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Params

    addParam(createParam<OrangeSlider>(VCOGlideSliderPos, module, Interzone::GLIDE_PARAM));
    addParam(createParam<OrangeSlider>(VCOModSliderPos, module, Interzone::PITCH_MOD_PARAM));
    addParam(createParam<OrangeSlider>(VCOWidthSliderPos, module, Interzone::PW_PARAM));
    addParam(createParam<OrangeSlider>(VCOPWMSliderPos, module, Interzone::PW_MOD_PARAM));

    RoganMedOrange* VCOOctaveKnob = createParam<RoganMedOrange>(VCOOctavePos, module, Interzone::OCTAVE_PARAM);
    if (module) {
        VCOOctaveKnob->smooth = false;
        VCOOctaveKnob->getParamQuantity()->snapEnabled = true;
        VCOOctaveKnob->minAngle = octaveMinAngle;
        VCOOctaveKnob->maxAngle = octaveMaxAngle;
    }
    addParam(VCOOctaveKnob);

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

    RoganMedOrange* LFOWaveKnob = createParam<RoganMedOrange>(LFOWavePos, module, Interzone::LFO_WAVE_PARAM);
    if (module) {
        LFOWaveKnob->smooth = false;
        LFOWaveKnob->getParamQuantity()->snapEnabled = true;
        LFOWaveKnob->minAngle = lfoWaveMinAngle;
        LFOWaveKnob->maxAngle = lfoWaveMaxAngle;
    }
    addParam(LFOWaveKnob);

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

Model *modelInterzone = createModel<Interzone, InterzoneWidget>("Interzone");
