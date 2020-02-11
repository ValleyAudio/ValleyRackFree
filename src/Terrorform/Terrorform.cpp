#include "Terrorform.hpp"

Terrorform::Terrorform() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Terrorform::OCTAVE_PARAM, -3.0, 3.0, 0.0, "Octave");
    configParam(Terrorform::COARSE_PARAM, -1.0, 1.0, 0.0, "Coarse Tune");
    configParam(Terrorform::FINE_PARAM, -0.041666, 0.041666, 0.0, "Fine Tune");
    configParam(Terrorform::VOCT_1_CV_PARAM, -1.0, 1.0, 0.0, "VOct 1 Atten.");
    configParam(Terrorform::VOCT_2_CV_PARAM, -1.0, 1.0, 0.0, "VOct 2 Atten.");

    configParam(Terrorform::BANK_PARAM, 0.0, NUM_TERRORFORM_WAVETABLES - 1.f, 0.0, "Bank");
    configParam(Terrorform::WAVE_PARAM, 0.0, 1.0, 0.0, "Wave");
    configParam(Terrorform::BANK_CV_1_PARAM, -1.0, 1.0, 0.0, "Bank CV Atten. 1");
    configParam(Terrorform::BANK_CV_2_PARAM, -1.0, 1.0, 0.0, "Bank CV Atten. 2");
    configParam(Terrorform::WAVE_CV_1_PARAM, -1.0, 1.0, 0.0, "Wave CV Atten. 1");
    configParam(Terrorform::WAVE_CV_2_PARAM, -1.0, 1.0, 0.0, "Wave CV Atten. 2");

    configParam(Terrorform::SHAPE_TYPE_PARAM, 0.0, 11.0, 0.0, "Shape Type");
    configParam(Terrorform::SHAPE_DEPTH_PARAM, 0.0, 1.0, 0.0, "Shape Depth");
    configParam(Terrorform::SHAPE_TYPE_CV_1_PARAM, -1.0, 1.0, 0.0, "Shape Type CV Atten. 1");
    configParam(Terrorform::SHAPE_TYPE_CV_2_PARAM, -1.0, 1.0, 0.0, "Shape Type CV Atten. 2");
    configParam(Terrorform::SHAPE_DEPTH_CV_1_PARAM, -1.0, 1.0, 0.0, "Shape Depth CV Atten. 1");
    configParam(Terrorform::SHAPE_DEPTH_CV_2_PARAM, -1.0, 1.0, 0.0, "Shape Depth CV Atten. 2");

    configParam(Terrorform::DEGRADE_TYPE_PARAM, 0.0, VecDegrader::VecDegraderModes::NUM_MODES - 1.f, 0.0, "Degrade Type");
    configParam(Terrorform::DEGRADE_DEPTH_PARAM, 0.0, 1.0, 0.0, "Degrade Depth");
    configParam(Terrorform::DEGRADE_TYPE_CV_1_PARAM, -1.0, 1.0, 0.0, "Degrade Type CV Atten. 1");
    configParam(Terrorform::DEGRADE_TYPE_CV_2_PARAM, -1.0, 1.0, 0.0, "Degrade Type CV Atten. 2");
    configParam(Terrorform::DEGRADE_DEPTH_CV_1_PARAM, -1.0, 1.0, 0.0, "Degrade Depth CV Atten. 1");
    configParam(Terrorform::DEGRADE_DEPTH_CV_2_PARAM, -1.0, 1.0, 0.0, "Degrade Depth CV Atten. 2");

    configParam(Terrorform::PERC_DECAY_PARAM, 0.0, 1.0, 0.5, "Perc Decay");
    configParam(Terrorform::PERC_VELOCITY_PARAM, 0.0, 1.0, 1.0, "Perc Velocity");
    configParam(Terrorform::PERC_SWITCH_PARAM, 0.0, 1.0, 0.0, "Perc Enable");
    configParam(Terrorform::PERC_DECAY_CV_1_PARAM, -1.0, 1.0, 0.0, "Perc Decay CV1 Atten.");
    configParam(Terrorform::PERC_DECAY_CV_2_PARAM, -1.0, 1.0, 0.0, "Perc Decay CV2 Atten.");
    configParam(Terrorform::PERC_VELOCITY_CV_1_PARAM, -1.0, 1.0, 0.0, "Perc Velocity CV1 Atten.");
    configParam(Terrorform::PERC_VELOCITY_CV_2_PARAM, -1.0, 1.0, 0.0, "Perc Velocity CV2 Atten.");
    configParam(Terrorform::TRIGGER_1_SWITCH_PARAM, 0.0, 1.0, 0.0, "Perc Trigger 1");
    configParam(Terrorform::TRIGGER_2_SWITCH_PARAM, 0.0, 1.0, 0.0, "Perc Trigger 2");

    configParam(Terrorform::FM_A1_ATTEN_PARAM, 0.0, 1.0, 0.0, "FM A1 Level");
    configParam(Terrorform::FM_A2_ATTEN_PARAM, 0.0, 1.0, 0.0, "FM A2 Level");
    configParam(Terrorform::FM_B1_ATTEN_PARAM, 0.0, 1.0, 0.0, "FM B1 Level");
    configParam(Terrorform::FM_B2_ATTEN_PARAM, 0.0, 1.0, 0.0, "FM B2 Level");
    configParam(Terrorform::FM_A_VCA_ATTEN_PARAM, 0.0, 1.0, 0.0, "FM A VCA Gain");
    configParam(Terrorform::FM_B_VCA_ATTEN_PARAM, 0.0, 1.0, 0.0, "FM B VCA Gain");

    configParam(Terrorform::WEAK_SYNC_1_SWITCH_PARAM, 0.0, 1.0, 0.0, "Weak Sync 1");
    configParam(Terrorform::WEAK_SYNC_2_SWITCH_PARAM, 0.0, 1.0, 0.0, "Weak Sync 2");

    configParam(Terrorform::USER_BANK_SWITCH_PARAM, 0.0, 1.0, 0.0, "User Waves Switch");

    for(int i = 0; i < kMaxNumGroups; ++i) {
        osc[i].setWavebank(wavetables[0], wavetable_sizes[0], wavetable_lengths[0][0]);
        osc[i].setScanPosition(0.f);
        osc[i].setSampleRate(APP->engine->getSampleRate());
        osc[i].setShape(0.0);
        osc[i].setShapeMethod(0);
        osc[i].enableSync(true);
    }
    bank = 0;
    shapeType = 0;

    rootWave = 0.f;
    numWavesInTable = 1.f;
    rootShapeDepth = 0.f;
    rootDegradeDepth = 0.f;

    __zeros = _mm_set1_ps(0.f);
    __ones = _mm_set1_ps(1.f);
    __negOnes = _mm_set1_ps(-1.f);
    __twos = _mm_set1_ps(2.f);
    __negTwos = _mm_set1_ps(-2.f);
    __fives = _mm_set1_ps(5.f);
    __negFives = _mm_set1_ps(-5.f);
    __tens = _mm_set1_ps(10.f);

    __sync1 = __zeros;
    __sync2 = __zeros;
    __sync1Pls = __zeros;
    __sync2Pls = __zeros;
    __percMode = __zeros;

    freqs = (float*)aligned_alloc_16(sizeof(float) * kMaxNumGroups * 4);
    waves = (float*)aligned_alloc_16(sizeof(float) * kMaxNumGroups * 4);
    shapes = (float*)aligned_alloc_16(sizeof(float) * kMaxNumGroups * 4);
    degrades = (float*)aligned_alloc_16(sizeof(float) * kMaxNumGroups * 4);

    percMode = false;

    // Fill user wavetables
    for(auto bank = 0; bank < TFORM_MAX_BANKS; ++bank) {
        userWaveTableData[bank] = new float*[TFORM_MAX_NUM_WAVES];
        userWaveTableFilled[bank] = false;
        userWaveTableSizes[bank] = 1;
        userWaveTableNames.push_back("USRWAVE_" + std::to_string(bank + 1));
        for(auto wave = 0; wave < TFORM_MAX_NUM_WAVES; ++wave) {
            userWaveTableData[bank][wave] = new float[TFORM_MAX_WAVELENGTH];
            for(auto i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
                userWaveTableData[bank][wave][i] = 0.f;
            }
        }
    }
    numUserWaveTables = 0;

    readFromUserWaves = false;
    userWavesButtonState = false;
    prevUserWavesButtonState = false;

    trigLightDurationSamples = trigLightDurationTime * APP->engine->getSampleRate();
}

Terrorform::~Terrorform() {
    aligned_free_16(freqs);
    aligned_free_16(waves);
    aligned_free_16(shapes);
    aligned_free_16(degrades);

    for(auto bank = 0; bank < TFORM_MAX_BANKS; ++bank) {
        for(auto wave = 0; wave < TFORM_MAX_NUM_WAVES; ++wave) {
            delete[] userWaveTableData[bank][wave];
        }
        delete[] userWaveTableData[bank];
    }
}

void Terrorform::process(const ProcessArgs &args) {
    ++counter;
    if(counter > 512) {
        rootBank = (int)params[BANK_PARAM].getValue();
        rootShapeType = (int)params[SHAPE_TYPE_PARAM].getValue();
        rootDegradeType = (int)params[DEGRADE_TYPE_PARAM].getValue();

        bank = rootBank + (int)(inputs[BANK_INPUT_1].getVoltage() * params[BANK_CV_1_PARAM].getValue() * 0.2f);
        shapeType = rootShapeType + (int)(inputs[SHAPE_TYPE_INPUT_1].getVoltage() * params[SHAPE_TYPE_CV_1_PARAM].getValue() * 0.2f);
        degradeType = rootDegradeType + (int)(inputs[DEGRADE_TYPE_INPUT_1].getVoltage() * params[DEGRADE_TYPE_CV_1_PARAM].getValue() * 0.2f);

        bank = clamp(bank, 0, NUM_TERRORFORM_WAVETABLES - 1);
        shapeType = clamp(shapeType, 0, 11);
        degradeType = clamp(degradeType, 0, VecDegrader::VecDegraderModes::NUM_MODES - 1);

        prevUserWavesButtonState = userWavesButtonState;
        userWavesButtonState = params[USER_BANK_SWITCH_PARAM].getValue() > 0.5f;
        if(prevUserWavesButtonState == false && userWavesButtonState == true) {
            readFromUserWaves = !readFromUserWaves;
        }

        for(auto c = 0; c < kMaxNumGroups; ++c) {
            if(readFromUserWaves) {
                lights[USER_BANK_LIGHT].value = 1.f;
                osc[c].setWavebank(userWaveTableData[0],
                                   userWaveTableSizes[0],
                                   TFORM_MAX_WAVELENGTH);
            }
            else {
                lights[USER_BANK_LIGHT].value = 0.f;
                osc[c].setWavebank(wavetables[bank],
                                   wavetable_sizes[bank],
                                   wavetable_lengths[bank][0]);
            }
            osc[c].setShapeMethod(shapeType);
            degrader[c].setMode(degradeType);
            osc[c].setSyncMode(syncChoice);
        }
        percMode = params[PERC_SWITCH_PARAM].getValue() > 0.5f ? true : false;
        lights[PERCUSSION_LIGHT].value = percMode ? 1.f : 0.f;

        numWavesInTable = osc[0].getNumwaves() - 1.f;
        __numWavesInTable = _mm_set1_ps(numWavesInTable);

        sync1IsMono = inputs[SYNC_1_INPUT].isMonophonic();
        sync2IsMono = inputs[SYNC_2_INPUT].isMonophonic();
        fmA1IsMono = inputs[FM_A1_INPUT].isMonophonic();
        fmA2IsMono = inputs[FM_A2_INPUT].isMonophonic();
        fmB1IsMono = inputs[FM_B1_INPUT].isMonophonic();
        fmB2IsMono = inputs[FM_B2_INPUT].isMonophonic();
        fmAVCAIsMono = inputs[FM_A_VCA_INPUT].isMonophonic();
        fmBVCAIsMono = inputs[FM_B_VCA_INPUT].isMonophonic();
        fmAVCAIsConnected = inputs[FM_A_VCA_INPUT].isConnected();
        fmBVCAIsConnected = inputs[FM_B_VCA_INPUT].isConnected();

        counter = 0;
    }

    trig1 = 0.f;
    if(params[TRIGGER_1_SWITCH_PARAM].getValue() > 0.5f && trig1State == false) {
        trig1State = true;
        trig1 = 1.f;
        trig1LightPulse.trigger(trigLightDurationSamples);
    }
    if(params[TRIGGER_1_SWITCH_PARAM].getValue() < 0.5f && trig1State == true) {
        trig1State = false;
    }

    trig2 = 0.f;
    if(params[TRIGGER_2_SWITCH_PARAM].getValue() > 0.5f && trig2State == false) {
        trig2State = true;
        trig2 = 1.f;
        trig2LightPulse.trigger(trigLightDurationSamples);
    }
    if(params[TRIGGER_2_SWITCH_PARAM].getValue() < 0.5f && trig2State == true) {
        trig2State = false;
    }

    lights[TRIGGER_1_LIGHT].value = trig1LightPulse.process(1.f) ? 10.f : 0.f;
    lights[TRIGGER_2_LIGHT].value = trig2LightPulse.process(1.f) ? 10.f : 0.f;

    rootPitch = (int)params[OCTAVE_PARAM].getValue();
    rootPitch += params[COARSE_PARAM].getValue();
    rootPitch += params[FINE_PARAM].getValue();
    rootWave = params[WAVE_PARAM].getValue();
    rootShapeDepth = params[SHAPE_DEPTH_PARAM].getValue();
    rootDegradeDepth = params[DEGRADE_DEPTH_PARAM].getValue();

    pitchCV1 = params[VOCT_1_CV_PARAM].getValue();
    pitchCV2 = params[VOCT_2_CV_PARAM].getValue();
    pitchCV1 *= pitchCV1;
    pitchCV2 *= pitchCV2;
    waveCV = params[WAVE_CV_1_PARAM].getValue();
    shapeDepthCV = params[SHAPE_DEPTH_CV_1_PARAM].getValue();
    degradeDepthCV = params[DEGRADE_DEPTH_CV_1_PARAM].getValue();

    for(auto i = 0; i < kMaxNumGroups * 4; ++i) {
        freqs[i] = freqLUT.getFrequency(inputs[VOCT_1_INPUT].getPolyVoltage(i) * pitchCV1 +
                                        inputs[VOCT_2_INPUT].getPolyVoltage(i) * pitchCV2 +
                                        rootPitch);
        waves[i] = rootWave + inputs[WAVE_INPUT_1].getPolyVoltage(i) * waveCV * 0.1f;
        waves[i] *= numWavesInTable;
        shapes[i] = rootShapeDepth + inputs[SHAPE_DEPTH_INPUT_1].getPolyVoltage(i) * shapeDepthCV * 0.1f;
        degrades[i] = rootDegradeDepth + inputs[DEGRADE_DEPTH_INPUT_1].getPolyVoltage(i) * degradeDepthCV * 0.1f;
    }

    sync1 = inputs[SYNC_1_INPUT].getVoltages();
    sync2 = inputs[SYNC_2_INPUT].getVoltages();
    fmA1 = inputs[FM_A1_INPUT].getVoltages();
    fmA2 = inputs[FM_A2_INPUT].getVoltages();
    fmB1 = inputs[FM_B1_INPUT].getVoltages();
    fmB2 = inputs[FM_B2_INPUT].getVoltages();
    fmAVCA = inputs[FM_A_VCA_INPUT].getVoltages();
    fmBVCA = inputs[FM_B_VCA_INPUT].getVoltages();

    fmA1Level = params[FM_A1_ATTEN_PARAM].getValue() * 0.2f;
    fmA2Level = params[FM_A2_ATTEN_PARAM].getValue() * 0.2f;
    fmB1Level = params[FM_B1_ATTEN_PARAM].getValue() * 0.2f;
    fmB2Level = params[FM_B2_ATTEN_PARAM].getValue() * 0.2f;
    fmAVCACV = params[FM_A_VCA_ATTEN_PARAM].getValue() * 0.1f;
    fmBVCACV = params[FM_B_VCA_ATTEN_PARAM].getValue() * 0.1f;
    __fmA1Level = _mm_set1_ps(fmA1Level);
    __fmA2Level = _mm_set1_ps(fmA2Level);
    __fmB1Level = _mm_set1_ps(fmB1Level);
    __fmB2Level = _mm_set1_ps(fmB2Level);
    __fmAVCACV = _mm_set1_ps(fmAVCACV);
    __fmBVCACV = _mm_set1_ps(fmBVCACV);

    __decay = _mm_set1_ps(params[PERC_DECAY_PARAM].getValue());

    // Tick the oscillator
    int g = 0;
    for(auto c = 0; c < kMaxNumGroups; ++c) {
        g = c * 4;
        __sync1 = sync1IsMono ? _mm_set1_ps(sync1[0]) : _mm_load_ps(sync1 + g);
        __sync2 = sync2IsMono ? _mm_set1_ps(sync2[0]) : _mm_load_ps(sync2 + g);
        __sync1Pls = _mm_and_ps(_mm_cmple_ps(__prevSync1, __zeros), _mm_cmpgt_ps(__sync1, __zeros));
        __sync1Pls = _mm_and_ps(__ones, __sync1Pls);
        __sync2Pls = _mm_and_ps(_mm_cmple_ps(__prevSync2, __zeros), _mm_cmpgt_ps(__sync2, __zeros));
        __sync2Pls = _mm_and_ps(__ones, __sync2Pls);
        __prevSync1 = __sync1;
        __prevSync2 = __sync2;

        __fmA = fmA1IsMono ? _mm_set1_ps(fmA1[0]) : _mm_load_ps(fmA1 + g);
        __fmA = _mm_mul_ps(__fmA, __fmA1Level);
        __fmA = _mm_add_ps(__fmA, _mm_mul_ps(fmA2IsMono ? _mm_set1_ps(fmA2[0]) : _mm_load_ps(fmA2 + g), __fmA2Level));
        __fmAVCA = fmAVCAIsMono ? _mm_set1_ps(fmAVCA[0]) : _mm_load_ps(fmAVCA + g);
        __fmAVCA = _mm_mul_ps(__fmAVCA, __fmAVCACV);
        __fmAVCA = fmAVCAIsConnected ? __fmAVCA : __ones;
        __fmA = _mm_mul_ps(__fmA, __fmAVCA);

        __fmB = fmB1IsMono ? _mm_set1_ps(fmB1[0]) : _mm_load_ps(fmB1 + g);
        __fmB = _mm_mul_ps(__fmB, __fmB1Level);
        __fmB = _mm_add_ps(__fmB, _mm_mul_ps(fmB2IsMono ? _mm_set1_ps(fmB2[0]) : _mm_load_ps(fmB2 + g), __fmB2Level));
        __fmBVCA = fmBVCAIsMono ? _mm_set1_ps(fmBVCA[0]) : _mm_load_ps(fmBVCA + g);
        __fmBVCA = _mm_mul_ps(__fmBVCA, __fmBVCACV);
        __fmBVCA = fmBVCAIsConnected ? __fmBVCA : __ones;
        __fmB = _mm_mul_ps(__fmB, __fmBVCA);

        __fmSum = _mm_add_ps(__fmA, __fmB);

        __freq = _mm_load_ps(freqs);
        __wave = _mm_load_ps(waves);
        __shape = _mm_load_ps(shapes);
        __degrade = _mm_load_ps(degrades);

        __wave = _mm_clamp_ps(__wave, __zeros, __numWavesInTable);
        __shape = _mm_clamp_ps(__shape, __zeros, __ones);
        __degrade = _mm_clamp_ps(__degrade, __zeros, __ones);

        lpg[c].setDecay(__decay);
        //lpg[c].process()

        osc[c].sync(_mm_add_ps(__sync1Pls, __sync2Pls));
        osc[c].setPhase(__fmSum);
        osc[c].setFrequency(__freq);
        osc[c].mm_setScanPosition(__wave);
        osc[c].setShape(__shape);

        osc[c].tick();

        __phasorOutput[c] = osc[c].getPhasor();
        __phasorOutput[c] = _mm_add_ps(_mm_mul_ps(__phasorOutput[c], __negTwos), __ones);
        degrader[c].insertAuxSignal(__phasorOutput[c]);
        __phasorOutput[c] = _mm_mul_ps(__phasorOutput[c], __negFives);
        __preDegradeOutput[c] = osc[c].getOutput();

        __mainOutput[c] = degrader[c].process(__preDegradeOutput[c], __degrade);
        __preDegradeOutput[c] = _mm_mul_ps(__preDegradeOutput[c], __fives);
        __mainOutput[c] = _mm_mul_ps(__mainOutput[c], __fives);

        __mainOutput[c] = _mm_switch_ps(__mainOutput[c], _mm_mul_ps(__mainOutput[c], lpg[c].__output), __percMode);

        _mm_store_ps(outputs[ENVELOPE_OUTPUT].getVoltages(g), lpg[c].getEnvelope());
        _mm_store_ps(outputs[PRE_DEGRADE_OUTPUT].getVoltages(g), __preDegradeOutput[c]);
        _mm_store_ps(outputs[PHASOR_OUTPUT].getVoltages(g), __phasorOutput[c]);
        _mm_store_ps(outputs[END_OF_CYCLE_OUTPUT].getVoltages(g), _mm_mul_ps(osc[c].getEOCPulse(), __fives));
        _mm_store_ps(outputs[MAIN_OUTPUT].getVoltages(g), lpg[c].process(__mainOutput[c], __sync1Pls));

        //_mm_store_ps(outputs[MAIN_OUTPUT].getVoltages(g), lpg[c].process(__sync1Pls));
    }
    outputs[MAIN_OUTPUT].setChannels(kMaxNumGroups * 4);
}

void Terrorform::onSampleRateChange() {
    for(auto i = 0; i < kMaxNumGroups; ++i) {
        osc[i].setSampleRate(APP->engine->getSampleRate());
        degrader[i].setSampleRate(APP->engine->getSampleRate());
    }
    trigLightDurationSamples = trigLightDurationTime * APP->engine->getSampleRate();
}

json_t* Terrorform::dataToJson()  {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
    json_object_set_new(rootJ, "displayStyle", json_integer(displayStyle));

    char str[25];
    json_t* userWavesJ = json_array();
    for (auto bank = 0; bank < TFORM_MAX_BANKS; ++bank) {
        json_t* userWaveJ = json_object();
        json_object_set_new(userWaveJ, "bank", json_integer(bank));
        if(userWaveTableFilled[bank] == false) {
            json_object_set_new(userWaveJ, "numWaves", json_integer(0));
            continue;
        }

        json_object_set_new(userWaveJ, "name", json_string(userWaveTableNames[bank].c_str()));
        json_t* tableJ = json_array();
        for (auto wave = 0; wave < userWaveTableSizes[bank]; ++wave) {
            json_t* waveJ = json_array();
            for (auto i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
                sprintf(str, "%e", userWaveTableData[bank][wave][i]);
                json_t* valueJ = json_string(str);
                json_array_append_new(waveJ, valueJ);
            }
            json_array_append_new(tableJ, waveJ);
        }
        json_object_set_new(userWaveJ, "waveTableData", tableJ);
        json_array_append_new(userWavesJ, userWaveJ);
    }
    json_object_set_new(rootJ, "userWaves", userWavesJ);
    return rootJ;
}

void Terrorform::dataFromJson(json_t *rootJ) {
    json_t *panelStyleJ = json_object_get(rootJ, "panelStyle");
    panelStyle = json_integer_value(panelStyleJ);
    json_t *displayStyleJ = json_object_get(rootJ, "displayStyle");
    displayStyle = json_integer_value(displayStyleJ);

    int destBank;
    int numWaves;
    clearUserWaveTables();

    json_t *userWavesJ = json_object_get(rootJ, "userWaves");
    numUserWaveTables = (int) json_array_size(userWavesJ);

    for (auto bank = 0; bank < numUserWaveTables; ++bank) {
        json_t* userWaveJ = json_array_get(userWavesJ, bank);
        json_t* destBankJ = json_object_get(userWaveJ, "bank");
        json_t* nameJ = json_object_get(userWaveJ, "name");
        json_t* tableJ = json_object_get(userWaveJ, "waveTableData");

        destBank = json_integer_value(destBankJ);
        numWaves = (int) json_array_size(tableJ);
        userWaveTableNames[bank] = std::string(json_string_value(nameJ));
        if (numWaves) {
            userWaveTableSizes[destBank] = numWaves;
            userWaveTableFilled[destBank] = true;
        }
        else {
            userWaveTableSizes[destBank] = 1;
        }

        for (auto wave = 0; wave < numWaves; ++wave) {
            json_t *waveJ = json_array_get(tableJ, wave);
            for(auto i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
                json_t *valueJ = json_array_get(waveJ, i);
                userWaveTableData[destBank][wave][i] = atof(json_string_value(valueJ));
            }
        }
    }
}

void Terrorform::clearBank(int bankNum) {
    for (int wave = 0; wave < TFORM_MAX_NUM_WAVES; ++wave) {
        for (int i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
            userWaveTableData[bankNum][wave][i] = 0.0;
        }
    }
    userWaveTableFilled[bankNum] = false;
    userWaveTableSizes[bankNum] = 1;
}

void Terrorform::clearUserWaveTables() {
    for (int i = 0; i < TFORM_MAX_BANKS; ++i) {
        clearBank(i);
    }
}

void Terrorform::cloneBank(int sourceBank, int destBank, int startWave, int endWave) {
    int k = startWave;
    float a = 0.f;
    for (int i = 0; i < TFORM_MAX_NUM_WAVES; ++i) {
        for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
            a = k <= endWave ? userWaveTableData[sourceBank][k][j] : 0.f;
            userWaveTableData[destBank][i][j] = a;
        }
        ++k;
    }
    userWaveTableFilled[destBank] = userWaveTableFilled[sourceBank];
    userWaveTableSizes[destBank] =  (endWave - startWave) + 1;
}

void Terrorform::moveBank(int sourceBank, int destBank) {
    for (int i = 0; i < TFORM_MAX_NUM_WAVES; ++i) {
        for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
            userWaveTableData[destBank][i][j] = userWaveTableData[sourceBank][i][j];
            userWaveTableData[sourceBank][i][j] = 0.f;
        }
    }
    userWaveTableSizes[destBank] = userWaveTableSizes[sourceBank];
    userWaveTableFilled[destBank] = userWaveTableFilled[sourceBank];
    userWaveTableSizes[sourceBank] = 1;
    userWaveTableFilled[sourceBank] = false;
}

void Terrorform::defragmentBanks() {
    for (int i = 0; i < TFORM_MAX_BANKS; ++i) {
        if (!userWaveTableFilled[i]) {
            for(int j = i; j < TFORM_MAX_BANKS; ++j) {
                if(userWaveTableFilled[j]) {
                    moveBank(j, i);
                    break;
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void TerrorformPanelStyleItem::onAction(const event::Action &e) {
    module->panelStyle = panelStyle;
}

void TerrorformPanelStyleItem::step() {
    rightText = (module->panelStyle == panelStyle) ? "✔" : "";
    MenuItem::step();
}

void TerrorformDisplayStyleItem::onAction(const event::Action &e) {
    module->displayStyle = displayStyle;
}

void TerrorformDisplayStyleItem::step() {
    rightText = (module->displayStyle == displayStyle) ? "✔" : "";
    MenuItem::step();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TerrorformWidget::TerrorformWidget(Terrorform* module) {
    const std::string panelFilenames[NUM_TRRFORM_PANELS] = {"res/Cell/CellDarkInky3.svg",
                                                            "res/Cell/CellLightInky3.svg",
                                                            "res/Cell/CellDarkLoader3.svg",
                                                            "res/Cell/CellDarkLoader3.svg"};
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, panelFilenames[TRRFORM_DARK_PANEL_NORMAL])));
    for(auto i = 1; i < NUM_TRRFORM_PANELS; ++i) {
        SvgPanel* newPanel = new SvgPanel;
        newPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, panelFilenames[i])));
        newPanel->visible = false;
        panels[i] = newPanel;
        addChild(panels[i]);
    }
    panels[0] = panel;

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Make jacks
    addInput(createInputCentered<PJ301MDarkSmall>(vOct1InputPos, module, Terrorform::VOCT_1_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(vOct2InputPos, module, Terrorform::VOCT_2_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(bankInput1Pos, module, Terrorform::BANK_INPUT_1));
    addInput(createInputCentered<PJ301MDarkSmall>(bankInput2Pos, module, Terrorform::BANK_INPUT_2));
    addInput(createInputCentered<PJ301MDarkSmall>(waveInput1Pos, module, Terrorform::WAVE_INPUT_1));
    addInput(createInputCentered<PJ301MDarkSmall>(waveInput2Pos, module, Terrorform::WAVE_INPUT_2));
    addInput(createInputCentered<PJ301MDarkSmall>(shapeTypeInput1Pos, module, Terrorform::SHAPE_TYPE_INPUT_1));
    addInput(createInputCentered<PJ301MDarkSmall>(shapeTypeInput2Pos, module, Terrorform::SHAPE_TYPE_INPUT_2));
    addInput(createInputCentered<PJ301MDarkSmall>(shapeDepthInput1Pos, module, Terrorform::SHAPE_DEPTH_INPUT_1));
    addInput(createInputCentered<PJ301MDarkSmall>(shapeDepthInput2Pos, module, Terrorform::SHAPE_DEPTH_INPUT_2));
    addInput(createInputCentered<PJ301MDarkSmall>(degradeTypeInput1Pos, module, Terrorform::DEGRADE_TYPE_INPUT_1));
    addInput(createInputCentered<PJ301MDarkSmall>(degradeTypeInput2Pos, module, Terrorform::DEGRADE_TYPE_INPUT_2));
    addInput(createInputCentered<PJ301MDarkSmall>(degradeDepthInput1Pos, module, Terrorform::DEGRADE_DEPTH_INPUT_1));
    addInput(createInputCentered<PJ301MDarkSmall>(degradeDepthInput2Pos, module, Terrorform::DEGRADE_DEPTH_INPUT_2));
    addInput(createInputCentered<PJ301MDarkSmall>(vcaAInputPos, module, Terrorform::FM_A_VCA_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(fmA1InputPos, module, Terrorform::FM_A1_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(fmA2InputPos, module, Terrorform::FM_A2_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(vcaBInputPos, module, Terrorform::FM_B_VCA_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(fmB1InputPos, module, Terrorform::FM_B1_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(fmB2InputPos, module, Terrorform::FM_B2_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(syncInput1Pos, module, Terrorform::SYNC_1_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(syncInput2Pos, module, Terrorform::SYNC_2_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(decayInput1Pos, module, Terrorform::DECAY_1_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(decayInput2Pos, module, Terrorform::DECAY_2_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(velocityInput1Pos, module, Terrorform::VELOCITY_1_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(velocityInput2Pos, module, Terrorform::VELOCITY_2_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(triggerInput1Pos, module, Terrorform::TRIGGER_1_INPUT));
    addInput(createInputCentered<PJ301MDarkSmall>(triggerInput2Pos, module, Terrorform::TRIGGER_2_INPUT));

    addOutput(createOutputCentered<PJ301MDarkSmallOut>(phasorOutPos, module, Terrorform::PHASOR_OUTPUT));
    addOutput(createOutputCentered<PJ301MDarkSmallOut>(eocOutPos, module, Terrorform::END_OF_CYCLE_OUTPUT));
    addOutput(createOutputCentered<PJ301MDarkSmallOut>(envOutPos, module, Terrorform::ENVELOPE_OUTPUT));
    addOutput(createOutputCentered<PJ301MDarkSmallOut>(preDegradePos, module, Terrorform::PRE_DEGRADE_OUTPUT));
    addOutput(createOutputCentered<PJ301MDarkSmallOut>(subOutPos, module, Terrorform::SUB_OUTPUT));
    addOutput(createOutputCentered<PJ301MDarkSmallOut>(mainOutPos, module, Terrorform::MAIN_OUTPUT));

    // Knobs
    octaveKnob = createParamCentered<RoganMedBlue>(octavePos, module, Terrorform::OCTAVE_PARAM);
    octaveKnob->smooth = false;
    octaveKnob->snap = true;
    addParam(octaveKnob);
    coarseKnob = createParamCentered<RoganMedBlue>(coarsePos, module, Terrorform::COARSE_PARAM);
    addParam(coarseKnob);
    fineKnob = createParamCentered<RoganMedBlue>(finePos, module, Terrorform::FINE_PARAM);
    addParam(fineKnob);

    bankKnob = createParamCentered<RoganMedPurple>(bankPos, module, Terrorform::BANK_PARAM);
    bankKnob->smooth = false;
    bankKnob->snap = true;
    addParam(bankKnob);
    waveKnob = createParamCentered<RoganMedPurple>(wavePos, module, Terrorform::WAVE_PARAM);
    addParam(waveKnob);

    shapeTypeKnob = createParamCentered<RoganMedRed>(shapeTypePos, module, Terrorform::SHAPE_TYPE_PARAM);
    shapeTypeKnob->smooth = false;
    shapeTypeKnob->snap = true;
    addParam(shapeTypeKnob);
    shapeDepthKnob = createParamCentered<RoganMedRed>(shapeDepthPos, module, Terrorform::SHAPE_DEPTH_PARAM);
    addParam(shapeDepthKnob);

    degradeTypeKnob = createParamCentered<RoganMedGreen>(degradeTypePos, module, Terrorform::DEGRADE_TYPE_PARAM);
    degradeTypeKnob->smooth = false;
    degradeTypeKnob->snap = true;
    addParam(degradeTypeKnob);
    degradeDepthKnob = createParamCentered<RoganMedGreen>(degradeDepthPos, module, Terrorform::DEGRADE_DEPTH_PARAM);
    addParam(degradeDepthKnob);

    decayKnob = createParamCentered<RoganMedMustard>(percDecayPos, module, Terrorform::PERC_DECAY_PARAM);
    addParam(decayKnob);
    velocityKnob = createParamCentered<RoganMedMustard>(percVelocityPos, module, Terrorform::PERC_VELOCITY_PARAM);
    addParam(velocityKnob);

    vOct1CV = createParamCentered<RoganSmallBlue>(vOct1CVPos, module, Terrorform::VOCT_1_CV_PARAM);
    vOct2CV = createParamCentered<RoganSmallBlue>(vOct2CVPos, module, Terrorform::VOCT_2_CV_PARAM);
    addParam(vOct1CV);
    addParam(vOct2CV);
    bankCV1 = createParamCentered<RoganSmallPurple>(bankCV1Pos, module, Terrorform::BANK_CV_1_PARAM);
    bankCV2 = createParamCentered<RoganSmallPurple>(bankCV2Pos, module, Terrorform::BANK_CV_2_PARAM);
    waveCV1 = createParamCentered<RoganSmallPurple>(waveCV1Pos, module, Terrorform::WAVE_CV_1_PARAM);
    waveCV2 = createParamCentered<RoganSmallPurple>(waveCV2Pos, module, Terrorform::WAVE_CV_2_PARAM);
    addParam(bankCV1);
    addParam(bankCV2);
    addParam(waveCV1);
    addParam(waveCV2);

    shapeTypeCV1 = createParamCentered<RoganSmallRed>(shapeTypeCV1Pos, module, Terrorform::SHAPE_TYPE_CV_1_PARAM);
    shapeTypeCV2 = createParamCentered<RoganSmallRed>(shapeTypeCV2Pos, module, Terrorform::SHAPE_TYPE_CV_2_PARAM);
    shapeDepthCV1 = createParamCentered<RoganSmallRed>(shapeDepthCV1Pos, module, Terrorform::SHAPE_DEPTH_CV_1_PARAM);
    shapeDepthCV2 = createParamCentered<RoganSmallRed>(shapeDepthCV2Pos, module, Terrorform::SHAPE_DEPTH_CV_2_PARAM);
    addParam(shapeTypeCV1);
    addParam(shapeTypeCV2);
    addParam(shapeDepthCV1);
    addParam(shapeDepthCV2);

    degradeTypeCV1 = createParamCentered<RoganSmallGreen>(degradeTypeCV1Pos, module, Terrorform::DEGRADE_TYPE_CV_1_PARAM);
    degradeTypeCV2 = createParamCentered<RoganSmallGreen>(degradeTypeCV2Pos, module, Terrorform::DEGRADE_TYPE_CV_2_PARAM);
    degradeDepthCV1 = createParamCentered<RoganSmallGreen>(degradeDepthCV1Pos, module, Terrorform::DEGRADE_DEPTH_CV_1_PARAM);
    degradeDepthCV2 = createParamCentered<RoganSmallGreen>(degradeDepthCV2Pos, module, Terrorform::DEGRADE_DEPTH_CV_2_PARAM);
    addParam(degradeTypeCV1);
    addParam(degradeTypeCV2);
    addParam(degradeDepthCV1);
    addParam(degradeDepthCV2);

    percDecayCV1 = createParamCentered<RoganSmallMustard>(percDecayCV1Pos, module, Terrorform::PERC_DECAY_CV_1_PARAM);
    percDecayCV2 = createParamCentered<RoganSmallMustard>(percDecayCV2Pos, module, Terrorform::PERC_DECAY_CV_2_PARAM);
    percVelocityCV1 = createParamCentered<RoganSmallMustard>(percVelocityCV1Pos, module, Terrorform::PERC_VELOCITY_CV_1_PARAM);
    percVelocityCV2 = createParamCentered<RoganSmallMustard>(percVelocityCV2Pos, module, Terrorform::PERC_VELOCITY_CV_2_PARAM);
    addParam(percDecayCV1);
    addParam(percDecayCV2);
    addParam(percVelocityCV1);
    addParam(percVelocityCV2);

    addParam(createParamCentered<RoganSmallWhite>(vcaAPos, module, Terrorform::FM_A_VCA_ATTEN_PARAM));
    addParam(createParamCentered<RoganSmallWhite>(vcaBPos, module, Terrorform::FM_B_VCA_ATTEN_PARAM));
    addParam(createParamCentered<RoganSmallWhite>(fmA1Pos, module, Terrorform::FM_A1_ATTEN_PARAM));
    addParam(createParamCentered<RoganSmallWhite>(fmA2Pos, module, Terrorform::FM_A2_ATTEN_PARAM));
    addParam(createParamCentered<RoganSmallWhite>(fmB1Pos, module, Terrorform::FM_B1_ATTEN_PARAM));
    addParam(createParamCentered<RoganSmallWhite>(fmB2Pos, module, Terrorform::FM_B2_ATTEN_PARAM));

    // Back text
    auto makeBackText = [=](const Vec& pos, int length, const NVGalign& align) {
        DynamicText* t = new DynamicText;
        t->size = 10;
        t->box.pos = pos;
        t->box.size = Vec(82, 14);
        t->visibility = nullptr;
        t->viewMode = ACTIVE_LOW_VIEW;
        t->horzAlignment = align;
        t->setFont(DynamicText::FontMode::FONT_MODE_7SEG);
        t->text = make_shared<std::string>(length, '~');
        t->customColor = nvgRGB(cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BACK][0],
                                cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BACK][1],
                                cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BACK][2]);
        return t;
    };

    auto setupFrontText = [=](DynamicText* t, DynamicText* tBlur1, DynamicText* tBlur2,
                             std::shared_ptr<std::string> text, const Vec& pos,
                             const NVGalign& align) {
        tBlur1->size = 10;
        tBlur1->box.pos = pos;
        tBlur1->box.size = Vec(82, 14);
        tBlur1->visibility = nullptr;
        tBlur1->viewMode = ACTIVE_LOW_VIEW;
        tBlur1->horzAlignment = align;
        tBlur1->setFont(DynamicText::FontMode::FONT_MODE_7SEG);
        tBlur1->text = text;
        tBlur1->customColor = nvgRGBA(cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_1][0],
                                      cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_1][1],
                                      cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_1][2],
                                      cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_1][3]);
        tBlur1->blur = 10.f;

        tBlur2->size = 10;
        tBlur2->box.pos = pos;
        tBlur2->box.size = Vec(82, 14);
        tBlur2->visibility = nullptr;
        tBlur2->viewMode = ACTIVE_LOW_VIEW;
        tBlur2->horzAlignment = align;
        tBlur2->setFont(DynamicText::FontMode::FONT_MODE_7SEG);
        tBlur2->text = text;
        tBlur2->customColor = nvgRGBA(cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_2][0],
                                      cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_2][1],
                                      cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_2][2],
                                      cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_BLUR_2][3]);
        tBlur2->blur = 8.f;

        t->size = 10;
        t->box.pos = pos;
        t->box.size = Vec(82, 14);
        t->visibility = nullptr;
        t->viewMode = ACTIVE_LOW_VIEW;
        t->horzAlignment = align;
        t->setFont(DynamicText::FontMode::FONT_MODE_7SEG);
        t->text = text;
        t->customColor = nvgRGB(cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_FRONT][0],
                                cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_FRONT][1],
                                cellDisplayColours[CELL_RED_LED_COLOUR][CELL_DISPLAY_FRONT][2]);
    };


    bool inBrowser = module == nullptr ? true : false;

    // Back Text
    bankBackText = makeBackText(bankTextPos, 13, NVG_ALIGN_LEFT);
    addChild(bankBackText);
    shapeBackText = makeBackText(shapeTextPos, 13, NVG_ALIGN_LEFT);
    addChild(shapeBackText);
    degradeBackText = makeBackText(degradeTextPos, 13, NVG_ALIGN_LEFT);
    addChild(degradeBackText);
    syncBackText = makeBackText(syncTextPos, 10, NVG_ALIGN_CENTER);
    addChild(syncBackText);

    // Wave Text
    bankStr = std::make_shared<std::string>(inBrowser ? "NEVER" : bankNames[0]);
    bankBlurText = new DynamicText;
    bankBlurText2 = new DynamicText;
    bankText = new DynamicText;
    setupFrontText(bankText, bankBlurText, bankBlurText2, bankStr, bankTextPos, NVG_ALIGN_LEFT);
    addChild(bankBlurText);
    addChild(bankBlurText2);
    addChild(bankText);

    waveStr = std::make_shared<std::string>("0");
    waveBlurText = new DynamicText;
    waveBlurText2 = new DynamicText;
    waveText = new DynamicText;
    setupFrontText(waveText, waveBlurText, waveBlurText2, waveStr, waveTextPos, NVG_ALIGN_RIGHT);
    addChild(waveBlurText);
    addChild(waveBlurText2);
    addChild(waveText);

    // Shape Text
    shapeTypeStr = make_shared<std::string>(inBrowser ? "GONNA" : shapeNames[0]);
    shapeBlurText = new DynamicText;
    shapeBlurText2 = new DynamicText;
    shapeText = new DynamicText;
    setupFrontText(shapeText, shapeBlurText, shapeBlurText2, shapeTypeStr, shapeTextPos, NVG_ALIGN_LEFT);
    addChild(shapeBlurText);
    addChild(shapeBlurText2);
    addChild(shapeText);

    shapeDepthStr = std::make_shared<std::string>("0");
    shapeDepthBlurText = new DynamicText;
    shapeDepthBlurText2 = new DynamicText;
    shapeDepthText = new DynamicText;
    setupFrontText(shapeDepthText, shapeDepthBlurText, shapeDepthBlurText2, shapeDepthStr, shapeDepthTextPos, NVG_ALIGN_RIGHT);
    addChild(shapeDepthBlurText);
    addChild(shapeDepthBlurText2);
    addChild(shapeDepthText);

    // Degrade Text

    degradeTypeStr = make_shared<std::string>(inBrowser ? "LET_YOU" : degradeNames[0]);
    degradeBlurText = new DynamicText;
    degradeBlurText2 = new DynamicText;
    degradeText = new DynamicText;
    setupFrontText(degradeText, degradeBlurText, degradeBlurText2, degradeTypeStr, degradeTextPos, NVG_ALIGN_LEFT);
    addChild(degradeBlurText);
    addChild(degradeBlurText2);
    addChild(degradeText);

    degradeDepthStr = std::make_shared<std::string>("0");
    degradeDepthBlurText = new DynamicText;
    degradeDepthBlurText2 = new DynamicText;
    degradeDepthText = new DynamicText;
    setupFrontText(degradeDepthText, degradeDepthBlurText, degradeDepthBlurText2, degradeDepthStr, degradeDepthTextPos, NVG_ALIGN_RIGHT);
    addChild(degradeDepthBlurText);
    addChild(degradeDepthBlurText2);
    addChild(degradeDepthText);

    // Sync Text
    syncStr = make_shared<std::string>(inBrowser ? "DOWN" : syncNames[0]);
    syncBlurText = new DynamicText;
    syncBlurText2 = new DynamicText;
    syncText = new DynamicText;
    setupFrontText(syncText, syncBlurText, syncBlurText2, syncStr, syncTextPos, NVG_ALIGN_CENTER);
    addChild(syncBlurText);
    addChild(syncBlurText2);
    addChild(syncText);

    if (module) {
        auto setOnHoverColour = [=](DynamicText* backText, DynamicText* frontText,
                                    DynamicText* blurText1, DynamicText* blurText2) {
            backText->customColor = nvgRGB(cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BACK][0],
                                           cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BACK][1],
                                           cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BACK][2]);
            frontText->customColor = nvgRGB(cellDisplayColours[displayStyle + 1][CELL_DISPLAY_FRONT][0],
                                            cellDisplayColours[displayStyle + 1][CELL_DISPLAY_FRONT][1],
                                            cellDisplayColours[displayStyle + 1][CELL_DISPLAY_FRONT][2]);
            blurText1->customColor = nvgRGBA(cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_1][0],
                                             cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_1][1],
                                             cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_1][2],
                                             cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_1][3]);
            blurText2->customColor = nvgRGBA(cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_2][0],
                                             cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_2][1],
                                             cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_2][2],
                                             cellDisplayColours[displayStyle + 1][CELL_DISPLAY_BLUR_2][3]);
        };

        auto setOnLeaveColour = [=](DynamicText* backText, DynamicText* frontText,
                                    DynamicText* blurText1, DynamicText* blurText2) {
            backText->customColor = nvgRGB(cellDisplayColours[displayStyle][CELL_DISPLAY_BACK][0],
                                           cellDisplayColours[displayStyle][CELL_DISPLAY_BACK][1],
                                           cellDisplayColours[displayStyle][CELL_DISPLAY_BACK][2]);
            frontText->customColor = nvgRGB(cellDisplayColours[displayStyle][CELL_DISPLAY_FRONT][0],
                                            cellDisplayColours[displayStyle][CELL_DISPLAY_FRONT][1],
                                            cellDisplayColours[displayStyle][CELL_DISPLAY_FRONT][2]);
            blurText1->customColor = nvgRGBA(cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][0],
                                             cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][1],
                                             cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][2],
                                             cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][3]);
            blurText2->customColor = nvgRGBA(cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][0],
                                             cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][1],
                                             cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][2],
                                             cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][3]);
        };

        bankMenu = createDynamicMenu(bankTextPos, bankText->box.size,
                                     bankMenuItems, true, false);
        bankMenu->onMouseEnter = [=]() {
            setOnHoverColour(bankBackText, bankText, bankBlurText, bankBlurText2);
        };

        bankMenu->onMouseLeave = [=]() {
            setOnLeaveColour(bankBackText, bankText, bankBlurText, bankBlurText2);
        };

        bankMenu->setChoice = [=](int i) {
            bankKnob->paramQuantity->setValue((float)i);
        };
        addChild(bankMenu);

        shapeMenu = createDynamicMenu(shapeTextPos, shapeText->box.size,
                                      shapeMenuItems, true, false);
        shapeMenu->onMouseEnter = [=]() {
            setOnHoverColour(shapeBackText, shapeText, shapeBlurText, shapeBlurText2);
        };

        shapeMenu->onMouseLeave = [=]() {
            setOnLeaveColour(shapeBackText, shapeText, shapeBlurText, shapeBlurText2);
        };

        shapeMenu->setChoice = [=](int i) {
            shapeTypeKnob->paramQuantity->setValue((float)i);
        };
        addChild(shapeMenu);

        degradeMenu = createDynamicMenu(degradeTextPos, degradeText->box.size,
                                        degradeMenuItems, true, false);
        degradeMenu->onMouseEnter = [=]() {
            setOnHoverColour(degradeBackText, degradeText, degradeBlurText, degradeBlurText2);
        };

        degradeMenu->onMouseLeave = [=]() {
            setOnLeaveColour(degradeBackText, degradeText, degradeBlurText, degradeBlurText2);
        };

        degradeMenu->setChoice = [=](int i) {
            degradeTypeKnob->paramQuantity->setValue((float)i);
        };
        addChild(degradeMenu);

        syncMenu = createDynamicMenu(syncTextPos.minus(Vec(syncText->box.size.x / 2.f, 0.f)), syncText->box.size,
                                     syncMenuItems, true, true);
        syncMenu->onMouseEnter = [=]() {
            setOnHoverColour(syncBackText, syncText, syncBlurText, syncBlurText2);
        };

        syncMenu->onMouseLeave = [=]() {
            setOnLeaveColour(syncBackText, syncText, syncBlurText, syncBlurText2);
        };

        syncMenu->setChoice = [=](int i) {
            module->syncChoice = i;
        };

        addChild(syncMenu);
    }

    // Switches
    percButton = createParamCentered<LightLEDButton3>(percSwitchPos, module, Terrorform::PERC_SWITCH_PARAM);
    percButton->momentary = false;
    addParam(percButton);

    percButtonLight = createLightCentered<LargeLight<RedLight>>(percSwitchPos, module, Terrorform::PERCUSSION_LIGHT);
    addChild(percButtonLight);

    addChild(createParamCentered<LightLEDButton3>(trigSwitch1Pos, module, Terrorform::TRIGGER_1_SWITCH_PARAM));
    addChild(createParamCentered<LightLEDButton3>(trigSwitch2Pos, module, Terrorform::TRIGGER_2_SWITCH_PARAM));
    addChild(createLightCentered<LargeLight<RedLight>>(trigSwitch1Pos, module, Terrorform::TRIGGER_1_LIGHT));
    addChild(createLightCentered<LargeLight<RedLight>>(trigSwitch2Pos, module, Terrorform::TRIGGER_2_LIGHT));
    addChild(createParamCentered<LightLEDButton>(weakSyncSwitch1Pos, module, Terrorform::WEAK_SYNC_1_SWITCH_PARAM));
    addChild(createParamCentered<LightLEDButton>(weakSyncSwitch2Pos, module, Terrorform::WEAK_SYNC_2_SWITCH_PARAM));
    addChild(createLightCentered<MediumLight<RedLight>>(weakSyncSwitch1Pos, module, Terrorform::WEAK_SYNC_1_LIGHT));
    addChild(createLightCentered<MediumLight<RedLight>>(weakSyncSwitch2Pos, module, Terrorform::WEAK_SYNC_2_LIGHT));

    lfoButton = createParam<LightLEDButton2>(userBankSwitchPos, module, Terrorform::USER_BANK_SWITCH_PARAM);
    lfoButton->momentary = true;
    addParam(lfoButton);

    lfoButtonLight = createLight<MediumLight<RedLight>>(userBankSwitchPos.plus(Vec(2.5f, 2.5f)), module, Terrorform::USER_BANK_LIGHT);
    addChild(lfoButtonLight);

    loadButton = createParam<LightLEDButton2>(loadTableSwitchPos, module, Terrorform::LOAD_TABLE_SWITCH_PARAM);
    loadButton->momentary = true;

    loadButton->onClick = [=]() {
        octaveKnob->visible = false;
        coarseKnob->visible = false;
        fineKnob->visible = false;
        bankKnob->visible = false;
        waveKnob->visible = false;
        shapeTypeKnob->visible = false;
        shapeDepthKnob->visible = false;
        degradeTypeKnob->visible = false;
        degradeDepthKnob->visible = false;
        decayKnob->visible = false;
        velocityKnob->visible = false;

        bankBackText->visible = false;
        shapeBackText->visible = false;
        degradeBackText->visible = false;

        bankText->visible = false;
        shapeText->visible = false;
        degradeText->visible = false;
        waveText->visible = false;
        shapeDepthText->visible = false;
        degradeDepthText->visible = false;

        bankBlurText->visible = false;
        bankBlurText2->visible = false;
        waveBlurText->visible = false;
        waveBlurText2->visible = false;
        shapeBlurText->visible = false;
        shapeBlurText2->visible = false;
        shapeDepthBlurText->visible = false;
        shapeDepthBlurText2->visible = false;
        degradeBlurText->visible = false;
        degradeBlurText2->visible = false;
        degradeDepthBlurText->visible = false;
        degradeDepthBlurText2->visible = false;

        vOct1CV->visible = false;
        vOct2CV->visible = false;
        bankCV1->visible = false;
        bankCV2->visible = false;
        waveCV1->visible = false;
        waveCV2->visible = false;
        shapeTypeCV1->visible = false;
        shapeTypeCV2->visible = false;
        shapeDepthCV1->visible = false;
        shapeDepthCV2->visible = false;
        degradeTypeCV1->visible = false;
        degradeTypeCV2->visible = false;
        degradeDepthCV1->visible = false;
        degradeDepthCV2->visible = false;
        percDecayCV1->visible = false;
        percDecayCV2->visible = false;
        percVelocityCV1->visible = false;
        percVelocityCV2->visible = false;

        lfoButton->visible = false;
        loadButton->visible = false;
        percButton->visible = false;

        percButtonLight->visible = false;
        lfoButtonLight->visible = false;

        editor->visible = true;
        inEditorMode = true;
    };
    addParam(loadButton);

    auto onExitEditor = [=]() {
        octaveKnob->visible = true;
        coarseKnob->visible = true;
        fineKnob->visible = true;
        bankKnob->visible = true;
        waveKnob->visible = true;
        shapeTypeKnob->visible = true;
        shapeDepthKnob->visible = true;
        degradeTypeKnob->visible = true;
        degradeDepthKnob->visible = true;
        decayKnob->visible = true;
        velocityKnob->visible = true;

        bankBackText->visible = true;
        shapeBackText->visible = true;
        degradeBackText->visible = true;

        bankText->visible = true;
        shapeText->visible = true;
        degradeText->visible = true;
        waveText->visible = true;
        shapeDepthText->visible = true;
        degradeDepthText->visible = true;

        bankBlurText->visible = true;
        bankBlurText2->visible = true;
        waveBlurText->visible = true;
        waveBlurText2->visible = true;
        shapeBlurText->visible = true;
        shapeBlurText2->visible = true;
        shapeDepthBlurText->visible = true;
        shapeDepthBlurText2->visible = true;
        degradeBlurText->visible = true;
        degradeBlurText2->visible = true;
        degradeDepthBlurText->visible = true;
        degradeDepthBlurText2->visible = true;

        vOct1CV->visible = true;
        vOct2CV->visible = true;
        bankCV1->visible = true;
        bankCV2->visible = true;
        waveCV1->visible = true;
        waveCV2->visible = true;
        shapeTypeCV1->visible = true;
        shapeTypeCV2->visible = true;
        shapeDepthCV1->visible = true;
        shapeDepthCV2->visible = true;
        degradeTypeCV1->visible = true;
        degradeTypeCV2->visible = true;
        degradeDepthCV1->visible = true;
        degradeDepthCV2->visible = true;
        percDecayCV1->visible = true;
        percDecayCV2->visible = true;
        percVelocityCV1->visible = true;
        percVelocityCV2->visible = true;

        lfoButton->visible = true;
        loadButton->visible = true;
        percButton->visible = true;

        percButtonLight->visible = true;
        lfoButtonLight->visible = true;

        editor->visible = false;
        inEditorMode = false;
    };

    auto loadWAVFile = [=]() -> std::shared_ptr<std::vector<std::vector<float>>> {
        const char FILE_FILTERS[] = "WAV File (.wav):wav";
        std::string dir = asset::user("");
        std::string filename;

        osdialog_filters* filters = osdialog_filters_parse(FILE_FILTERS);
        DEFER({
            osdialog_filters_free(filters);
        });

        drwav_uint64 numSamples = 0;
        unsigned int channels = 1;
        unsigned int sampleRate = 44100;
        newTable = NULL;

        char* path = osdialog_file(OSDIALOG_OPEN, dir.c_str(), filename.c_str(), filters);
        if (path) {
            newTable = drwav_open_file_and_read_f32(path, &channels, &sampleRate, &numSamples);
            DEFER({
                std::free(path);
            });
        }

        std::shared_ptr<std::vector<std::vector<float>>> waves = std::make_shared<std::vector<std::vector<float>>>();
        if (newTable == NULL) {
            return waves;
        }

        auto overflow = numSamples % TFORM_MAX_WAVELENGTH;
        numSamples -= overflow;
        numSamples = numSamples > TFORM_MAX_TABLE_SIZE ? TFORM_MAX_TABLE_SIZE : numSamples;

        int numBlocks = (int) numSamples / TFORM_MAX_WAVELENGTH;
        int readPos = 0;
        waves->resize(64);
        for (int i = 0; i < numBlocks; ++i) {
            (*waves)[i].assign(256, 0.f);
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                (*waves)[i][j] = newTable[readPos];
                ++readPos;
            }
        }
        return waves;
    };

    auto ingestNewTable = [=](int bank, int startWave, int endWave, const std::string name) {
        int numWaves = (endWave - startWave) + 1;
        int tableLength = numWaves * TFORM_MAX_WAVELENGTH;
        int readPos = 0;
        int startPos = startWave * TFORM_MAX_WAVELENGTH;
        int writePos = 0;
        int wave = 0;

        for (int i = 0; i < tableLength; ++i) {
            readPos = startPos + i;
            wave = i / TFORM_MAX_WAVELENGTH;
            writePos = i % TFORM_MAX_WAVELENGTH;
            module->userWaveTableData[bank][wave][writePos] = newTable[readPos];
        }
        module->userWaveTableSizes[bank] = numWaves;
        module->userWaveTableFilled[bank] = true;
        module->userWaveTableNames[bank] = name;
        module->numUserWaveTables++;
    };

    editor = createWidget<TFormEditor>(Vec(31, 30));
    editor->box.size.x = 238;
    editor->box.size.y = 195;
    editor->visible = false;
    editor->addOnExitCallback(onExitEditor);
    editor->addLoadWAVCallback(loadWAVFile);
    editor->addIngestTableCallback(ingestNewTable);
    editor->addExportCallback(std::bind(&TerrorformWidget::exportWavetables, this));
    editor->addImportCallback(std::bind(&TerrorformWidget::importWavetables, this));

    editor->addClearBankCallback([=](int bankNum){
        module->clearBank(bankNum);
    });

    editor->addGetBankCallback([=](int bank, std::vector<std::vector<float>>& data) {
        data.resize(module->userWaveTableSizes[bank]);
        for (int i = 0; i < data.size(); ++i) {
            data[i].resize(TFORM_MAX_WAVELENGTH);
            memcpy(data[i].data(), module->userWaveTableData[bank][i], sizeof(float) * TFORM_MAX_WAVELENGTH);
        }
    });

    editor->addCloneBankCallback([=](int sourceBank, int destBank, int startWave, int endWave) {
        module->cloneBank(sourceBank, destBank, startWave, endWave);
    });

    editor->addMoveBankCallback([=](int sourceBank, int destBank) {
        module->moveBank(sourceBank, destBank);
    });

    editor->addDefragmentCallback([=]() {
        module->defragmentBanks();
    });

    addChild(editor);
}

void TerrorformWidget::appendContextMenu(Menu *menu) {
    Terrorform *module = dynamic_cast<Terrorform*>(this->module);
    assert(module);

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel style"));
    menu->addChild(construct<TerrorformPanelStyleItem>(&MenuItem::text, "Dark", &TerrorformPanelStyleItem::module,
                                                    module, &TerrorformPanelStyleItem::panelStyle, 0));
    menu->addChild(construct<TerrorformPanelStyleItem>(&MenuItem::text, "Light", &TerrorformPanelStyleItem::module,
                                                      module, &TerrorformPanelStyleItem::panelStyle, 1));

    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Display style"));
    menu->addChild(construct<TerrorformDisplayStyleItem>(&MenuItem::text, "Red LED", &TerrorformDisplayStyleItem::module,
                                                    module, &TerrorformDisplayStyleItem::displayStyle, 0));
    menu->addChild(construct<TerrorformDisplayStyleItem>(&MenuItem::text, "Yellow LED", &TerrorformDisplayStyleItem::module,
                                                    module, &TerrorformDisplayStyleItem::displayStyle, 1));
    menu->addChild(construct<TerrorformDisplayStyleItem>(&MenuItem::text, "Green LED", &TerrorformDisplayStyleItem::module,
                                                    module, &TerrorformDisplayStyleItem::displayStyle, 2));
    menu->addChild(construct<TerrorformDisplayStyleItem>(&MenuItem::text, "Blue VFD", &TerrorformDisplayStyleItem::module,
                                                      module, &TerrorformDisplayStyleItem::displayStyle, 3));
    menu->addChild(construct<TerrorformDisplayStyleItem>(&MenuItem::text, "White LED", &TerrorformDisplayStyleItem::module,
                                                      module, &TerrorformDisplayStyleItem::displayStyle, 4));
 }

void TerrorformWidget::step() {
    if (module) {
        int panelStyleOffset = dynamic_cast<Terrorform*>(module)->panelStyle;
        for (auto i = 0; i < NUM_TRRFORM_PANELS; ++i) {
            panels[i]->visible = false;
        }
        if (inEditorMode) {
            panels[TRRFORM_DARK_PANEL_EDITOR + panelStyleOffset]->visible = true;
        }
        else {
            panels[TRRFORM_DARK_PANEL_NORMAL + panelStyleOffset]->visible = true;
        }

        displayStyle = (TerrorformDisplayColourModes)(dynamic_cast<Terrorform*>(module)->displayStyle * 2);
        if (displayStyle != prevDisplayStyle) {
            changeDisplayStyle();
        }
        prevDisplayStyle = displayStyle;

        switch (displayMode) {
            case DISPLAY_PARAMS:
                onDisplayParams();
                break;
            case DISPLAY_LOAD_ERROR:
                onDisplayLoadError();
                break;
        }

        for (auto i = 0; i < TFORM_MAX_BANKS; ++i) {
            editor->setSlotFilledFlag(i, dynamic_cast<Terrorform*>(module)->userWaveTableFilled[i]);
        }

        *syncStr = syncNames[(unsigned long)dynamic_cast<Terrorform*>(module)->syncChoice];
    }
    Widget::step();
}

void TerrorformWidget::onDisplayParams() {
    bankChoice = (int)(bankKnob->paramQuantity->getValue());
    *bankStr = bankNames[bankChoice];
    wavePercent = (int)(waveKnob->paramQuantity->getValue() * 100.f);
    *waveStr = std::to_string(wavePercent);

    *shapeTypeStr = shapeNames[(int)(shapeTypeKnob->paramQuantity->getValue())];
    shapeDepthPercent = (int)(shapeDepthKnob->paramQuantity->getValue() * 100.f);
    *shapeDepthStr = std::to_string(shapeDepthPercent);

    *degradeTypeStr = degradeNames[(int)(degradeTypeKnob->paramQuantity->getValue())];
    degradeDepthPercent = (int)(degradeDepthKnob->paramQuantity->getValue() * 100.f);
    *degradeDepthStr = std::to_string(degradeDepthPercent);

    bankMenu->visible = true;
    shapeMenu->visible = true;
    degradeMenu->visible = true;
}

void TerrorformWidget::onDisplayLoadError() {
    *bankStr = "CANT!OVRWRITE";
    *waveStr = "";

    *shapeTypeStr = "!!READ-ONLY!!";
    *shapeDepthStr = "";

    *degradeTypeStr = "!!WAVETABLE!!";
    *degradeDepthStr = "";
    bankMenu->visible = false;
    shapeMenu->visible = false;
    degradeMenu->visible = false;

    elapsedErrorDisplayTime++;
    if (elapsedErrorDisplayTime >= errorDisplayTime) {
        displayMode = DISPLAY_PARAMS;
    }
}

void TerrorformWidget::changeDisplayStyle() {
    auto setNewColour = [=](DynamicText* backText, DynamicText* frontText,
                            DynamicText* blurText1, DynamicText* blurText2) {
        if (backText) {
            backText->customColor = nvgRGB(cellDisplayColours[displayStyle][CELL_DISPLAY_BACK][0],
                                           cellDisplayColours[displayStyle][CELL_DISPLAY_BACK][1],
                                           cellDisplayColours[displayStyle][CELL_DISPLAY_BACK][2]);
        }

        frontText->customColor = nvgRGB(cellDisplayColours[displayStyle][CELL_DISPLAY_FRONT][0],
                                        cellDisplayColours[displayStyle][CELL_DISPLAY_FRONT][1],
                                        cellDisplayColours[displayStyle][CELL_DISPLAY_FRONT][2]);

        blurText1->customColor = nvgRGBA(cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][0],
                                         cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][1],
                                         cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][2],
                                         cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_1][3]);

        blurText2->customColor = nvgRGBA(cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][0],
                                         cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][1],
                                         cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][2],
                                         cellDisplayColours[displayStyle][CELL_DISPLAY_BLUR_2][3]);
    };
    setNewColour(bankBackText, bankText, bankBlurText, bankBlurText2);
    setNewColour(nullptr, waveText, waveBlurText, waveBlurText2);
    setNewColour(shapeBackText, shapeText, shapeBlurText, shapeBlurText2);
    setNewColour(nullptr, shapeDepthText, shapeDepthBlurText, shapeDepthBlurText2);
    setNewColour(degradeBackText, degradeText, degradeBlurText, degradeBlurText2);
    setNewColour(nullptr, degradeDepthText, degradeDepthBlurText, degradeDepthBlurText2);
    setNewColour(syncBackText, syncText, syncBlurText, syncBlurText2);
}

void TerrorformWidget::exportWavetables() {
    Terrorform* tform = dynamic_cast<Terrorform*>(module);
    std::fstream outFile;

    const char FILE_FILTERS[] = "Valley Wavetable (.vwt):vwt";
    std::string dir = asset::user("");
    std::string filename;
    std::string filepath;

    osdialog_filters* filters = osdialog_filters_parse(FILE_FILTERS);
    DEFER({
        osdialog_filters_free(filters);
    });

    char* path = osdialog_file(OSDIALOG_SAVE, dir.c_str(), filename.c_str(), filters);
    if (path) {
        filepath = std::string(path);
        DEFER({
            std::free(path);
        });
    }
    else {
        return;
    }

    // Mark empty banks in new table sizes array
    char userWaveTableSizes[TFORM_MAX_BANKS];
    memcpy(&userWaveTableSizes, &tform->userWaveTableSizes, sizeof(char) * TFORM_MAX_BANKS);
    for (int b = 0; b < TFORM_MAX_BANKS; ++b) {
        userWaveTableSizes[b] = tform->userWaveTableFilled[b] ? userWaveTableSizes[b] : 0;
    }

    outFile.open(filepath, std::ios::out | std::ios::binary);
    if (outFile.is_open()) {
        outFile.seekp(0);
        outFile.write("T401VWT", sizeof(char) * 7);
        outFile.write(&tform->numUserWaveTables, sizeof(char));
        outFile.write((char*) &userWaveTableSizes, sizeof(char) * TFORM_MAX_BANKS);
        for (int b = 0; b < TFORM_MAX_BANKS; ++b) {
            for (int w = 0; w < TFORM_MAX_NUM_WAVES; ++w) {
                for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                    outFile.write(reinterpret_cast<char*>(&tform->userWaveTableData[b][w][j]), sizeof(float));
                }
            }
        }
    }
    outFile.close();
}

void TerrorformWidget::importWavetables() {
    Terrorform* tform = dynamic_cast<Terrorform*>(module);
    std::fstream inFile;

    const char FILE_FILTERS[] = "Valley Wavetable (.vwt):vwt";
    std::string dir = asset::user("");
    std::string filename;
    std::string filepath;

    osdialog_filters* filters = osdialog_filters_parse(FILE_FILTERS);
    DEFER({
        osdialog_filters_free(filters);
    });

    char* path = osdialog_file(OSDIALOG_OPEN, dir.c_str(), filename.c_str(), filters);
    if (path) {
        filepath = std::string(path);
        DEFER({
            std::free(path);
        });
    }
    else {
        return;
    }

    char header[8];
    int pos = 0;
    inFile.open(filepath, std::ios::in | std::ios::binary);

    if (!inFile.is_open()) {
        printf("Error opening wavetable file\n");
        return;
    }

    inFile.read((char*) &header, sizeof(char) * 7);
    header[7] = '\0';
    if (std::string(header) != "T401VWT") {
        printf("Not a valid Valley Wavetable file\n");
        return;
    }
    pos += sizeof(char) * 7;

    inFile.seekg(pos);
    inFile.read(&tform->numUserWaveTables, sizeof(char));
    inFile.read((char*) &tform->userWaveTableSizes, sizeof(char) * TFORM_MAX_BANKS);
    pos += sizeof(char) * TFORM_MAX_BANKS;

    for (int b = 0; b < TFORM_MAX_BANKS; ++b) {
        tform->userWaveTableFilled[b] = true;
        if (tform->userWaveTableSizes[b] == 0) {
            tform->userWaveTableSizes[b] = 1;
            tform->userWaveTableFilled[b] = false;
        }
    }

    for (int b = 0; b < TFORM_MAX_BANKS; ++b) {
        for (int w = 0; w < TFORM_MAX_NUM_WAVES; ++w) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                inFile.read(reinterpret_cast<char*>(&tform->userWaveTableData[b][w][j]), sizeof(float));
            }
        }
    }

    inFile.close();
    printf("Wavetable import successful\n");
}

Model *modelTerrorform = createModel<Terrorform, TerrorformWidget>("Terrorform");
