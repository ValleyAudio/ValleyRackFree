//
// Dexter.cpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 18/3/2018
//

#include "Dexter.hpp"

Dexter::Dexter() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configParam(Dexter::OCTAVE_PARAM, 0.0, 6.0, 3.0, "Voice A Octave");
    configParam(Dexter::COARSE_PARAM, -1.0, 1.0, 0.0, "Voice A Coarse");
    configParam(Dexter::FINE_PARAM, -0.05, 0.05, 0.0, "Voice A Fine");
    configParam(Dexter::CHORD_PARAM, 0.0, NUM_CHORDS - 1, 0.0, "Voice A Chord");
    configParam(Dexter::INVERT_PARAM, 0.0, 20.0, 10.0, "Voice A Chord Invert");
    configParam(Dexter::DETUNE_PARAM, 0.0, 1.0, 0.0, "Voice A Chord Detune");
    configParam(Dexter::B_OCTAVE_PARAM, 0.0, 6.0, 3.0, "Voice B Octave");
    configParam(Dexter::B_COARSE_PARAM, -1.0, 1.0, 0.0, "Voice B Coarse");
    configParam(Dexter::B_FINE_PARAM, -0.05, 0.05, 0.0, "Voice B Fine");

    configParam(Dexter::ALGORITHM_PARAM, 0, 22, 0.0, "Algorithm", "", 0, 1, 1);
    configParam(Dexter::BRIGHTNESS_PARAM, -1.0, 1.0, 0.0, "Brightness");
    configParam(Dexter::SHAPE_PARAM, 0.0, 1.0, 0.0, "Shape");
    configParam(Dexter::FEEDBACK_PARAM, 0.0, 0.25, 0.0, "Feedback");

    configParam(Dexter::CHORD_DEPTH_PARAM, -1.0, 1.0, 0.0, "Chord");
    configParam(Dexter::INVERT_DEPTH_PARAM, -1.0, 1.0, 0.0, "Chord Inversion");
    configParam(Dexter::DETUNE_DEPTH_PARAM, -1.0, 1.0, 0.0, "Chord Detune");
    configParam(Dexter::ALGO_DEPTH_PARAM, -1.0, 1.0, 0.0, "Algorithm Mod Depth");
    configParam(Dexter::FB_DEPTH_PARAM, -1.0, 1.0, 0.0, "Feedback Mod Depth");
    configParam(Dexter::BRIGHT_DEPTH_PARAM, -1.0, 1.0, 0.0, "Brightness Mod Depth");
    configParam(Dexter::SHAPE_DEPTH_PARAM, -1.0, 1.0, 0.0, "Shape Mod Depth");

    configParam(Dexter::MASTER_LFO_BUTTON, 0.0, 1.0, 0.0, "Global LFO Enable");
    configParam(Dexter::RESET_PHASE_BUTTON, 0.0, 1.0, 0.0, "Reset Operator Phases");
    configParam(Dexter::FULL_INVERSION_BUTTON, 0.0, 1.0, 0.0, "Full Chord Inversion Enable");

    for(int op = 0; op < kNumOperators; ++op) {
        std::string opStr = "Op. " + std::to_string(op + 1) + " ";
        configParam(Dexter::OP_1_MULT_PARAM + Dexter::NUM_PARAM_GROUPS * op, 0, 26, 3, opStr + "Multiplier");
        configParam(Dexter::OP_1_COARSE_PARAM + Dexter::NUM_PARAM_GROUPS * op, -1.0, 1.0, 0.0, opStr + "Coarse");
        configParam(Dexter::OP_1_FINE_PARAM + Dexter::NUM_PARAM_GROUPS * op, -0.25, 0.25, 0.0, opStr + "Fine");
        configParam(Dexter::OP_1_WAVE_PARAM + Dexter::NUM_PARAM_GROUPS * op, 0.0, 1.0, 0.0, opStr + "Wave");
        configParam(Dexter::OP_1_SHAPE_PARAM + Dexter::NUM_PARAM_GROUPS * op, 0.0, 1.0, 0.0, opStr + "Phase Shape");
        if(op == 0) {
            configParam(Dexter::OP_1_LEVEL_PARAM + Dexter::NUM_PARAM_GROUPS * op, 0.0, 1.0, 1.0, opStr + "Level");
        }
        else {
            configParam(Dexter::OP_1_LEVEL_PARAM + Dexter::NUM_PARAM_GROUPS * op, 0.0, 1.0, 0.0, opStr + "Level");
        }
        configParam(opParams[op][Dexter::OP_PRE_PARAM], 0.0, 1.0, 0.0, opStr + "Indiv. Out Pre-fade");
        configParam(opParams[op][Dexter::OP_SETTINGS_PARAM], 0.0, 1.0, 0.0, opStr + "Settings");
        configParam(opParams[op][Dexter::OP_POST_SHAPE_PARAM], 0.0, 1.0, 0.0, opStr + "Post FM Phase Shape");
        configParam(opParams[op][Dexter::OP_WEAK_PARAM], 0.0, 1.0, 0.0, opStr + "Weak Sync");
        configParam(opParams[op][Dexter::OP_LFO_PARAM], 0.0, 1.0, 0.0, opStr + "LFO Mode");
        configParam(opParams[op][Dexter::OP_SYNC_PARAM], 0.0, 1.0, 0.0, opStr + "Sync");

        configParam(Dexter::OP_1_WAVE_MENU_PARAM + Dexter::NUM_PARAM_GROUPS * op,
                    0.0, 1.0, 0.0, opStr + "Wave Menu");

        configParam(OP_1_MOD_1_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Assignable Mod 1 Depth");
        configParam(OP_1_MOD_2_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Assignable Mod 2 Depth");
        configParam(OP_1_MOD_3_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Assignable Mod 3 Depth");
        configParam(OP_1_MOD_4_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Assignable Mod 4 Depth");

        configParam(OP_1_PITCH_CV1_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Pitch CV 1 Depth");
        configParam(OP_1_WAVE_CV1_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Wave CV 1 Depth");
        configParam(OP_1_SHAPE_CV1_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Phase Shape CV 1 Depth");
        configParam(OP_1_LEVEL_CV1_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Level CV 1 Depth");

        configParam(OP_1_PITCH_CV2_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Pitch CV 2 Depth");
        configParam(OP_1_WAVE_CV2_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Wave CV 2 Depth");
        configParam(OP_1_SHAPE_CV2_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Phase Shape CV 2 Depth");
        configParam(OP_1_LEVEL_CV2_PARAM + 12 * op, -1.0, 1.0, 0.0, opStr + "Level CV 2 Depth");

        configParam(OP_1_BANK_PARAM + NUM_PARAM_GROUPS * op, 0.0, NUM_DEXTER_WAVETABLES - 1.f, 0.0, opStr + "Wave Bank");
    }

    resetPhaseState = false;
    octave = 0;
    aPitch = 0.f;
    for(auto i = 0; i < kNumOperators; ++i) {
        opPitch[i] = 0.f;
        opLowFreq[i][0] = 440.f;
        opLowFreq[i][1] = 440.f;
        opLowFreq[i][2] = 440.f;
        opLowFreq[i][3] = 440.f;
        opHighFreq[i][0] = 440.f;
        opHighFreq[i][1] = 440.f;
        opHighFreq[i][2] = 440.f;
        opHighFreq[i][3] = 440.f;
        opLevel[i] = 0.f;
        opWave[i] = 0.f;
        opWaveBank[i] = 0.f;
        opButtonSync[i] = 0.f;
        opButtonWeakSync[i] = 0.f;
        opButtonPostShape[i] = 0.f;
        opButtonLFO[i] = 0.f;
        opButtonPreFade[i] = 0.f;
        opSync[i] = false;
        opWeakSync[i] = false;
        opPostShape[i] = false;
        opPreFade[i] = false;

        opMultipleKnob[i] = 0;
        opSettingsMenu[i] = 0;
        opWaveMenuVis[i] = 0;
        opModAMenuVis[i] = 0;
        opModBMenuVis[i] = 0;
        opMiscMenuVis[i] = 0;
    }
    brightness = 0.f;
    feedback = 0.f;
    allShape = 0.f;
    int NaN = 0xFFFFFFFF;
    __0000 = _mm_castsi128_ps(_mm_set1_epi32(0));
    __0001 = _mm_castsi128_ps(_mm_set_epi32(0,   0,   0,   NaN));
    __0011 = _mm_castsi128_ps(_mm_set_epi32(0,   0,   NaN, NaN));
    __0111 = _mm_castsi128_ps(_mm_set_epi32(0,   NaN, NaN, NaN));
    __1111 = _mm_castsi128_ps(_mm_set_epi32(NaN, NaN, NaN, NaN));

    __0010 = _mm_castsi128_ps(_mm_set_epi32(0,   0,   NaN, 0));
    __0110 = _mm_castsi128_ps(_mm_set_epi32(0,   NaN, NaN, 0));
    __1110 = _mm_castsi128_ps(_mm_set_epi32(NaN, NaN, NaN, 0));

    __bOutMask = __0001;
    __lowChordMask = __0010;
    __highChordMask = __0000;

    __lowChordMaskTable[0] = __0010;
    __lowChordMaskTable[1] = __0110;
    __lowChordMaskTable[2] = __1110;
    __lowChordMaskTable[3] = __1110;
    __lowChordMaskTable[4] = __1110;
    __lowChordMaskTable[5] = __1110;
    __lowChordMaskTable[6] = __1110;

    __highChordMaskTable[0] = __0000;
    __highChordMaskTable[1] = __0000;
    __highChordMaskTable[2] = __0000;
    __highChordMaskTable[3] = __0001;
    __highChordMaskTable[4] = __0011;
    __highChordMaskTable[5] = __0111;
    __highChordMaskTable[6] = __1111;

    __lowLeftGainTable[0] = _mm_set_ps(0.f,    0.f,    1.f, 0.f);
    __lowLeftGainTable[1] = _mm_set_ps(0.f,    0.f,    1.f, 0.f);
    __lowLeftGainTable[2] = _mm_set_ps(0.f,    0.5f,   1.f, 0.f);
    __lowLeftGainTable[3] = _mm_set_ps(0.333f, 0.666f, 1.f, 0.f);
    __lowLeftGainTable[4] = _mm_set_ps(0.5f,   0.75f,  1.f, 0.f);
    __lowLeftGainTable[5] = _mm_set_ps(0.6f,   0.8f,   1.f, 0.f);
    __lowLeftGainTable[6] = _mm_set_ps(0.666f, 0.833f, 1.f, 0.f);

    __lowRightGainTable[0] = _mm_set_ps(0.f,    0.f,    1.f,    0.f);
    __lowRightGainTable[1] = _mm_set_ps(0.f,    1.f,    0.f,    0.f);
    __lowRightGainTable[2] = _mm_set_ps(1.f,    0.5f,   0.f,    0.f);
    __lowRightGainTable[3] = _mm_set_ps(0.666f, 0.333f, 0.f,    0.f);
    __lowRightGainTable[4] = _mm_set_ps(0.5f,   0.25f,  0.f,    0.f);
    __lowRightGainTable[5] = _mm_set_ps(0.4f,   0.2f,   0.f,    0.f);
    __lowRightGainTable[6] = _mm_set_ps(0.333f, 0.166f, 0.f,    0.f);

    __highLeftGainTable[0] = _mm_set_ps(0.f,    0.f,    0.f,    0.f);
    __highLeftGainTable[1] = _mm_set_ps(0.f,    0.f,    0.f,    0.f);
    __highLeftGainTable[2] = _mm_set_ps(0.f,    0.f,    0.f,    0.f);
    __highLeftGainTable[3] = _mm_set_ps(0.f,    0.f,    0.f,    0.f);
    __highLeftGainTable[4] = _mm_set_ps(0.f,    0.f,    0.f,    0.25f);
    __highLeftGainTable[5] = _mm_set_ps(0.f,    0.f,    0.2f,   0.4f);
    __highLeftGainTable[6] = _mm_set_ps(0.f,    0.166f, 0.333f, 0.5f);

    __highRightGainTable[0] = _mm_set_ps(0.f,    0.f,    0.f,    0.f);
    __highRightGainTable[1] = _mm_set_ps(0.f,    0.f,    0.f,    0.f);
    __highRightGainTable[2] = _mm_set_ps(0.f,    0.f,    0.f,    0.f);
    __highRightGainTable[3] = _mm_set_ps(0.f,    0.f,    0.f,    1.f);
    __highRightGainTable[4] = _mm_set_ps(0.f,    0.f,    1.f,    0.75f);
    __highRightGainTable[5] = _mm_set_ps(0.f,    1.f,    0.8f,   0.6f);
    __highRightGainTable[6] = _mm_set_ps(1.f,    0.833f, 0.666f, 0.5f);

    chordKnob = 0;
    currentChord = 0;
    syncMode = 0;
    algo = 0;
    chordNotes = getChord(0, 0, 0, 0);
    pOpFreqs = (float*)aligned_alloc_16(sizeof(float) * 8);

    for(auto i = 0; i < 8; ++i) {
        pOpFreqs[i] = 0.f;
    }

    pLeftOut = (float*)aligned_alloc_16(sizeof(float) * 4);
    pRightOut = (float*)aligned_alloc_16(sizeof(float) * 4);
    pOP1 = (float*)aligned_alloc_16(sizeof(float) * 4);
    pOP2 = (float*)aligned_alloc_16(sizeof(float) * 4);
    pOP3 = (float*)aligned_alloc_16(sizeof(float) * 4);
    pOP4 = (float*)aligned_alloc_16(sizeof(float) * 4);
    pBOut = (float*)aligned_alloc_16(sizeof(float) * 4);
    for(auto i = 0; i < 4; ++i) {
        pLeftOut[i] = 0.f;
        pRightOut[i] = 0.f;
        pOP1[i] = 0.f;
        pOP2[i] = 0.f;
        pOP3[i] = 0.f;
        pOP4[i] = 0.f;
        pBOut[i] = 0.f;
    }
    __zeros = _mm_set1_ps(0.f);
    __leftOut = __zeros;
    __rightOut = __zeros;

    coreA.setSampleRate(APP->engine->getSampleRate());
    coreB.setSampleRate(APP->engine->getSampleRate());
}

Dexter::~Dexter() {
    aligned_free_16(pLeftOut);
    aligned_free_16(pRightOut);
    aligned_free_16(pOP1);
    aligned_free_16(pOP2);
    aligned_free_16(pOP3);
    aligned_free_16(pOP4);
    aligned_free_16(pBOut);
    aligned_free_16(pOpFreqs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// DSP ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dexter::step() {
    if(params[RESET_PHASE_BUTTON].getValue()) {
        if(!resetPhaseState) {
            resetPhaseState = true;
            coreA.resetPhase();
            coreB.resetPhase();
        }
        lights[RESET_PHASE_LIGHT].value = 1.f;
    } else {
        resetPhaseState = false;
        lights[RESET_PHASE_LIGHT].value = 0.f;
    }

    if(masterLFOButtonTrig.process(params[MASTER_LFO_BUTTON].getValue())) {
        masterLFO = 1.f - masterLFO;
    }
    lights[MASTER_LFO_LIGHT].value = masterLFO ? 1.f : 0.f;

    if(fullInversionButtonTrig.process(params[FULL_INVERSION_BUTTON].getValue())) {
        buttonFullInversion = buttonFullInversion ? 0 : 1;
    }
    lights[FULL_INVERSION_LIGHT].value = buttonFullInversion ? 1.f : 0.f;

    // Menu system
    for(auto op = 0; op < kNumOperators; ++op) {
        if(opSettBtnTrig[op].process(params[opParams[op][OP_SETTINGS_PARAM]].getValue())) {
            opSettingsMenu[op] = !opSettingsMenu[op];
        }
        lights[opLights[op][OP_SETTINGS]].value = opSettingsMenu[op] ? 1.f : 0.f;

        if(opSyncBtnTrig[op].process(params[opParams[op][OP_SYNC_PARAM]].getValue())) {
            opButtonSync[op] = 1.f - opButtonSync[op];
        }
        lights[opLights[op][OP_SYNC]].value = opButtonSync[op] ? 1.f : 0.f;

        if(opPostShapeBtnTrig[op].process(params[opParams[op][OP_POST_SHAPE_PARAM]].getValue())) {
            opButtonPostShape[op] = 1.f - opButtonPostShape[op];
        }
        lights[opLights[op][OP_POST_SHAPE]].value = opButtonPostShape[op] ? 1.f : 0.f;

        if(opWeakBtnTrig[op].process(params[opParams[op][OP_WEAK_PARAM]].getValue())) {
            opButtonWeakSync[op] = 1.f - opButtonWeakSync[op];
        }
        lights[opLights[op][OP_WEAK]].value = opButtonWeakSync[op] ? 1.f : 0.f;

        if(opLFOBtnTrig[op].process(params[opParams[op][OP_LFO_PARAM]].getValue())) {
            opButtonLFO[op] = 1.f - opButtonLFO[op];
        }
        lights[opLights[op][OP_LFO]].value = (opButtonLFO[op] + masterLFO) ? 1.f : 0.f;

        if(opPreFadeBtnTrig[op].process(params[opParams[op][OP_PRE_PARAM]].getValue())) {
            opButtonPreFade[op] = 1.f - opButtonPreFade[op];
        }
        lights[opLights[op][OP_PRE_LIGHT]].value = opButtonPreFade[op] ? 1.f : 0.f;
    }

    for(auto op = 0; op < kNumOperators; ++op) {
        if(opWaveMenuBtnTrig[op].process(params[opParams[op][OP_WAVE_MENU_PARAM]].getValue())) {
            opMenuPage[op] = 0;
        }
        else if (opModAMenuBtnTrig[op].process(params[opParams[op][OP_MODA_MENU_PARAM]].getValue())) {
            opMenuPage[op] = 1;
        }
        else if (opModBMenuBtnTrig[op].process(params[opParams[op][OP_MODB_MENU_PARAM]].getValue())) {
            opMenuPage[op] = 2;
        }

        opWaveMenuVis[op] = 0;
        opModAMenuVis[op] = 0;
        opModBMenuVis[op] = 0;
        opMiscMenuVis[op] = 0;
        if(opSettingsMenu[op]) {
            switch(opMenuPage[op]) {
                case 0:
                    opWaveMenuVis[op] = 1;
                    break;
                case 1:
                    opModAMenuVis[op] = 1;
                    break;
                case 2:
                    opModBMenuVis[op] = 1;
            }
        }
        lights[opLights[op][OP_WAVE_LIGHT]].value = opWaveMenuVis[op] ? 1.f : 0.f;
        lights[opLights[op][OP_MODA_LIGHT]].value = opModAMenuVis[op] ? 1.f : 0.f;
        lights[opLights[op][OP_MODB_LIGHT]].value = opModBMenuVis[op] ? 1.f : 0.f;
    }

    brightness = params[BRIGHTNESS_PARAM].getValue();
    brightness += inputs[BRIGHTNESS_INPUT].getVoltage() * 0.1f * params[BRIGHT_DEPTH_PARAM].getValue();
    brightness = clamp(brightness, -1.f, 1.f);
    allShape = params[SHAPE_PARAM].getValue() + inputs[SHAPE_INPUT].getVoltage() * 0.1f * params[SHAPE_DEPTH_PARAM].getValue();

    octaveAKnob = (int)params[OCTAVE_PARAM].getValue();
    aPitch = (float)(octaveAKnob - 3) + params[COARSE_PARAM].getValue() + params[FINE_PARAM].getValue();
    aPitch += inputs[A_VOCT_INPUT].getVoltage();
    chordKnob = (int)params[CHORD_PARAM].getValue();
    chordParam = (float)chordKnob + (inputs[CHORD_INPUT].getVoltage() * 0.1f * params[CHORD_DEPTH_PARAM].getValue() * (float)(NUM_CHORDS - 1));
    chordParam = clamp(chordParam, 0.f, (float)NUM_CHORDS - 1.f);
    chordDetuneParam = params[DETUNE_PARAM].getValue() + inputs[DETUNE_INPUT].getVoltage() * 0.1f * params[DETUNE_DEPTH_PARAM].getValue();
    chordDetuneParam = clamp(chordDetuneParam, 0.f, 1.f) * -0.1f;
    invDepthKnob = (int)params[INVERT_PARAM].getValue();
    invDepthParam = invDepthKnob + (int)(inputs[INVERT_INPUT].getVoltage() * params[INVERT_DEPTH_PARAM].getValue()) - 10;
    invDepthParam = clamp(invDepthParam, -10, 10);
    if((Chords)chordParam != currentChord || invDepthParam != invDepth || chordDetuneParam != chordDetune
       || buttonFullInversion != fullInversion) {
        fullInversion = buttonFullInversion;
        chordDetune = chordDetuneParam;
        makeChord(chordParam, invDepthParam);
    }

    octaveBKnob = (int)params[B_OCTAVE_PARAM].getValue();
    bPitch = (float)(octaveBKnob - 3) + params[B_COARSE_PARAM].getValue() + params[B_FINE_PARAM].getValue();
    bPitch += inputs[B_VOCT_INPUT].getVoltage();

    coreA.setSyncSource(opSyncSource);
    coreB.setSyncSource(opSyncSource);

    for(auto op = 0; op < kNumOperators; ++op) {
        modMatrix[op].setRowDestination(0, (DexterRoutingMatrixDestination) opMod1Assign[op]);
        modMatrix[op].setRowDestination(1, (DexterRoutingMatrixDestination) opMod2Assign[op]);
        modMatrix[op].setRowDestination(2, (DexterRoutingMatrixDestination) opMod3Assign[op]);
        modMatrix[op].setRowDestination(3, (DexterRoutingMatrixDestination) opMod4Assign[op]);

        modMatrix[op].setRowSourceValue(0, inputs[opCVInputs[op][OP_MOD_CV_1]].getVoltage() * 0.1f);
        modMatrix[op].setRowSourceValue(1, inputs[opCVInputs[op][OP_MOD_CV_2]].getVoltage() * 0.1f);
        modMatrix[op].setRowSourceValue(2, inputs[opCVInputs[op][OP_MOD_CV_3]].getVoltage() * 0.1f);
        modMatrix[op].setRowSourceValue(3, inputs[opCVInputs[op][OP_MOD_CV_4]].getVoltage() * 0.1f);

        modMatrix[op].setRowDepth(0, params[opCVAtten[op][OP_MOD_CV_1]].getValue());
        modMatrix[op].setRowDepth(1, params[opCVAtten[op][OP_MOD_CV_2]].getValue());
        modMatrix[op].setRowDepth(2, params[opCVAtten[op][OP_MOD_CV_3]].getValue());
        modMatrix[op].setRowDepth(3, params[opCVAtten[op][OP_MOD_CV_4]].getValue());

        modMatrix[op].process();

        opPitch[op] = params[opParams[op][OP_COARSE_PARAM]].getValue();
        opPitch[op] += params[opParams[op][OP_FINE_PARAM]].getValue();
        opPitch[op] += inputs[opCVInputs[op][OP_PITCH_CV_1]].getVoltage() * params[opCVAtten[op][OP_PITCH_CV_1]].getValue();
        opPitch[op] += inputs[opCVInputs[op][OP_PITCH_CV_2]].getVoltage() * params[opCVAtten[op][OP_PITCH_CV_2]].getValue();
        opPitch[op] += modMatrix[op].getDestinationValue(PITCH_DEST);
        opMultipleKnob[op] = (int)params[opParams[op][OP_MULT_PARAM]].getValue();
        multiple = opMultipleKnob[op] + (int)modMatrix[op].getDestinationValue(RATIO_DEST);

        pOpFreqs[0] = freqLUT.getFrequency((opPitch[op]) + bPitch) * multiples[clamp(multiple, 0, 26)];
        if(pOpFreqs[0] >= 22049.f) {
            pOpFreqs[0] = 22049.f;
        }
        __lowChordMask = __lowChordMaskTable[chordNotes.size() - 1];
        __highChordMask = __highChordMaskTable[chordNotes.size() - 1];
        __lowLeftGain = __lowLeftGainTable[chordNotes.size() - 1];
        __lowRightGain = __lowRightGainTable[chordNotes.size() - 1];
        __highLeftGain = __highLeftGainTable[chordNotes.size() - 1];
        __highRightGain = __highRightGainTable[chordNotes.size() - 1];

        for(unsigned long i = 0; i < chordNotes.size(); ++i) {
            pOpFreqs[i + 1] = freqLUT.getFrequency((opPitch[op]) + aPitch + chordNotes[i]) * multiples[clamp(multiple, 0, 26)];
            if(pOpFreqs[i + 1] >= 22049.f) {
                pOpFreqs[i + 1] = 22049.f;
            }
            if(opButtonLFO[op] > 0.f || masterLFO > 0.f) {
                pOpFreqs[i + 1] /= 100.f;
            }
        }

        __opLowFreq[op] = _mm_load_ps(pOpFreqs);
        __opHighFreq[op] = _mm_load_ps(pOpFreqs + 4);

        opWaveBankKnob[op] = (int)params[opParams[op][OP_BANK_PARAM]].getValue();
        opWaveBank[op] = opWaveBankKnob[op] + modMatrix[op].getDestinationValue(WAVE_BANK_DEST);
        opWaveBank[op] = clamp(opWaveBank[op], 0, NUM_DEXTER_WAVETABLES - 1);

        opWave[op] = params[opParams[op][OP_WAVE_PARAM]].getValue();
        opWave[op] += inputs[opCVInputs[op][OP_WAVE_CV_1]].getVoltage() * 0.1f * params[opCVAtten[op][OP_WAVE_CV_1]].getValue();
        opWave[op] += inputs[opCVInputs[op][OP_WAVE_CV_2]].getVoltage() * 0.1f * params[opCVAtten[op][OP_WAVE_CV_2]].getValue();
        opWave[op] += modMatrix[op].getDestinationValue(WAVE_POS_DEST);
        opWave[op] = clamp(opWave[op], 0.f, 1.f);

        opShape[op] = params[opParams[op][OP_SHAPE_PARAM]].getValue() + allShape;
        opShape[op] += inputs[opCVInputs[op][OP_SHAPE_CV_1]].getVoltage() * 0.1f * params[opCVAtten[op][OP_SHAPE_CV_1]].getValue();
        opShape[op] += inputs[opCVInputs[op][OP_SHAPE_CV_2]].getVoltage() * 0.1f * params[opCVAtten[op][OP_SHAPE_CV_2]].getValue();
        opShape[op] += modMatrix[op].getDestinationValue(SHAPE_DEST);
        opShape[op] = clamp(opShape[op], -1.f, 1.f);

        opLevel[op] = params[opParams[op][OP_LEVEL_PARAM]].getValue();
        opLevel[op] += inputs[opCVInputs[op][OP_LEVEL_CV_1]].getVoltage() * 0.1f * params[opCVAtten[op][OP_LEVEL_CV_1]].getValue();
        opLevel[op] += inputs[opCVInputs[op][OP_LEVEL_CV_2]].getVoltage() * 0.1f * params[opCVAtten[op][OP_LEVEL_CV_2]].getValue();
        opLevel[op] += modMatrix[op].getDestinationValue(LEVEL_DEST);
        opLevel[op] = clamp(opLevel[op], 0.f, 1.0f);

        coreA._mm_setFrequency(op, __opLowFreq[op]);
        coreB._mm_setFrequency(op, __opHighFreq[op]);
        coreA.setWavebank(op, opWaveBank[op]);
        coreB.setWavebank(op, opWaveBank[op]);
        coreA.setWavePosition(op, opWave[op]);
        coreB.setWavePosition(op, opWave[op]);
        opShapeMode[op] = opMenuShapeMode[op] + modMatrix[op].getDestinationValue(SHAPE_MODE_DEST);
        opShapeMode[op] = clamp(opShapeMode[op], 0, kNumShapeModes - 1);
        coreA.setShapeMode(op, opShapeMode[op]);
        coreB.setShapeMode(op, opShapeMode[op]);
        coreA.setShape(op, opShape[op]);
        coreB.setShape(op, opShape[op]);
        coreA.setPMPostShape(op, opPostShape[op]);
        coreB.setPMPostShape(op, opPostShape[op]);
        if(opButtonPostShape[op] + modMatrix[op].getDestinationValue(POST_SHAPE_DEST) > 0.5f) {
            opPostShape[op] = true;
        }
        else {
            opPostShape[op] = false;
        }
        coreA.enableSync(op, opSync[op]);
        coreB.enableSync(op, opSync[op]);

        coreA.externalFM(op, modMatrix[op].getDestinationValue(EXT_FM_DEST));
        coreB.externalFM(op, modMatrix[op].getDestinationValue(EXT_FM_DEST));
        coreA.externalSync(op, modMatrix[op].getDestinationValue(EXT_SYNC_DEST));
        coreB.externalSync(op, modMatrix[op].getDestinationValue(EXT_SYNC_DEST));

        opSyncMode[op] = opMenuSyncMode[op] + modMatrix[op].getDestinationValue(SYNC_MODE_DEST);
        opSyncMode[op] = clamp(opSyncMode[op], 0, kNumSyncModes - 1);
        coreA.setSyncMode(op, opSyncMode[op]);
        coreB.setSyncMode(op, opSyncMode[op]);

        // Enable sync if any external mod jack is connected, regardless of the sync button state
        bool modJackState = inputs[opCVInputs[op][OP_MOD_CV_1]].isConnected() | inputs[opCVInputs[op][OP_MOD_CV_2]].isConnected()
                          | inputs[opCVInputs[op][OP_MOD_CV_3]].isConnected() | inputs[opCVInputs[op][OP_MOD_CV_4]].isConnected();
        coreA.enableSync(op, modJackState);
        coreB.enableSync(op, modJackState);

        // Internal sync enable
        if((opButtonSync[op] + modMatrix[op].getDestinationValue(SYNC_ENABLE_DEST) > 0.5f)) {
            opSync[op] = true;
        }
        else {
            opSync[op] = false;
        }
        coreA.enableIntSync(op, opSync[op]);
        coreB.enableIntSync(op, opSync[op]);

        if(opButtonWeakSync[op] + modMatrix[op].getDestinationValue(WEAK_SYNC_DEST) > 0.f) {
            opWeakSync[op] = true;
        }
        else {
            opWeakSync[op] = false;
        }
        coreA.enableWeakSync(op, opWeakSync[op]);
        coreB.enableWeakSync(op, opWeakSync[op]);
        if(opButtonPreFade[op] > 0.f) {
            coreA.setOpPreFade(op, true);
        }
        else {
            coreA.setOpPreFade(op, false);
        }
        coreA.setLevel(op, opLevel[op]);
        coreB.setLevel(op, opLevel[op]);
    }
    algo = round_int(params[ALGORITHM_PARAM].getValue() + inputs[ALGO_INPUT].getVoltage() * 0.1f * params[ALGO_DEPTH_PARAM].getValue() * (float)(kNumAlgorithms - 1.f));
    algo = clamp(algo, 0, kNumAlgorithms - 1);
    coreA.setAlgorithm(algo);
    coreB.setAlgorithm(algo);
    feedback = params[FEEDBACK_PARAM].getValue();
    feedback += inputs[FEEDBACK_INPUT].getVoltage() * 0.025 * params[FB_DEPTH_PARAM].getValue();
    feedback = clamp(feedback, 0.f, 0.25f);
    coreA.setFeedback(feedback);
    coreB.setFeedback(feedback);
    coreA.setBrightness(brightness);
    coreB.setBrightness(brightness);

    coreA.process();
    if(chordNotes.size() > 3) {
        coreB.process();
    }

    _mm_store_ps(pBOut, _mm_and_ps(coreA.getBOutput(), __bOutMask));
    outputs[B_OUTPUT].setVoltage(pBOut[0]);

    if(outputs[A_RIGHT_OUTPUT].isConnected()) {
        __leftOut = __zeros;
        __rightOut = __zeros;
        __leftOut = _mm_mul_ps(coreA.getMainOutput(), __lowLeftGain);
        __leftOut = _mm_add_ps(__leftOut, _mm_mul_ps(coreB.getMainOutput(), __highLeftGain));
        __rightOut = _mm_mul_ps(coreA.getMainOutput(), __lowRightGain);
        __rightOut = _mm_add_ps(__rightOut, _mm_mul_ps(coreB.getMainOutput(), __highRightGain));
        _mm_store_ps(pLeftOut, __leftOut);
        _mm_store_ps(pRightOut, __rightOut);
    }
    else {
        _mm_store_ps(pLeftOut, _mm_add_ps(_mm_and_ps(coreA.getMainOutput(), __lowChordMask),
                                          _mm_and_ps(coreB.getMainOutput(), __highChordMask)));
        _mm_store_ps(pRightOut, __zeros);
    }

    if(indivBOutputs) {
        _mm_store_ps(pOP1, _mm_and_ps(coreA.getOpOutput(0), __0001));
        _mm_store_ps(pOP2, _mm_and_ps(coreA.getOpOutput(1), __0001));
        _mm_store_ps(pOP3, _mm_and_ps(coreA.getOpOutput(2), __0001));
        _mm_store_ps(pOP4, _mm_and_ps(coreA.getOpOutput(3), __0001));
    }
    else {
        _mm_store_ps(pOP1, _mm_add_ps(_mm_and_ps(coreA.getOpOutput(0), __lowChordMask),
                                      _mm_and_ps(coreB.getOpOutput(0), __highChordMask)));
        _mm_store_ps(pOP2, _mm_add_ps(_mm_and_ps(coreA.getOpOutput(1), __lowChordMask),
                                      _mm_and_ps(coreB.getOpOutput(1), __highChordMask)));
        _mm_store_ps(pOP3, _mm_add_ps(_mm_and_ps(coreA.getOpOutput(2), __lowChordMask),
                                      _mm_and_ps(coreB.getOpOutput(2), __highChordMask)));
        _mm_store_ps(pOP4, _mm_add_ps(_mm_and_ps(coreA.getOpOutput(3), __lowChordMask),
                                      _mm_and_ps(coreB.getOpOutput(3), __highChordMask)));
    }

    outputs[A_LEFT_OUTPUT].setVoltage(0);
    outputs[A_RIGHT_OUTPUT].setVoltage(0);
    outputs[OP_1_OUTPUT].setVoltage(0);
    outputs[OP_2_OUTPUT].setVoltage(0);
    outputs[OP_3_OUTPUT].setVoltage(0);
    outputs[OP_4_OUTPUT].setVoltage(0);
    for(auto i = 0; i < 4; ++i) {
        outputs[A_LEFT_OUTPUT].value += pLeftOut[i];
        outputs[A_RIGHT_OUTPUT].value += pRightOut[i];
        outputs[OP_1_OUTPUT].value += pOP1[i];
        outputs[OP_2_OUTPUT].value += pOP2[i];
        outputs[OP_3_OUTPUT].value += pOP3[i];
        outputs[OP_4_OUTPUT].value += pOP4[i];
    }
    outputs[A_LEFT_OUTPUT].value *= _outputLevels[chordNotes.size() - 1];
    outputs[A_RIGHT_OUTPUT].value *= _outputLevels[chordNotes.size() - 1];
    outputs[OP_1_OUTPUT].value *= _outputLevels[chordNotes.size() - 1];
    outputs[OP_2_OUTPUT].value *= _outputLevels[chordNotes.size() - 1];
    outputs[OP_3_OUTPUT].value *= _outputLevels[chordNotes.size() - 1];
    outputs[OP_4_OUTPUT].value *= _outputLevels[chordNotes.size() - 1];
}

void Dexter::makeChord(float chord, float invert) {
    currentChord = (Chords)chord;
    invDepth = invert;
    chordNotes = getChord(currentChord, chordDetune, invDepth, fullInversion);
}

void Dexter::onSampleRateChange() {
    for(auto i = 0; i < kMaxChordSize; ++i) {
        coreA.setSampleRate(APP->engine->getSampleRate());
        coreB.setSampleRate(APP->engine->getSampleRate());
    }
}

void Dexter::onReset() {
    for(auto i = 0; i < kNumOperators; ++i) {
        opButtonSync[i] = 0;
        opButtonPostShape[i] = 0;
        opButtonWeakSync[i] = 0;
        opButtonLFO[i] = 0;
        opButtonPreFade[i] = 0;

        opMenuSyncMode[i] = 0;
        opMenuShapeMode[i] = 0;
        opMod1Assign[i] = 0;
        opMod2Assign[i] = 0;
        opMod3Assign[i] = 0;
        opMod4Assign[i] = 0;
    }
    masterLFO = 0.f;
}

json_t *Dexter::dataToJson() {
    json_t *rootJ = json_object();

    json_object_set_new(rootJ, "op1syncEnable", json_integer(opButtonSync[0]));
    json_object_set_new(rootJ, "op1weakSyncEnable", json_integer(opButtonWeakSync[0]));
    json_object_set_new(rootJ, "op1PostShape", json_integer(opButtonPostShape[0]));
    json_object_set_new(rootJ, "op1PreFade", json_integer(opButtonPreFade[0]));
    json_object_set_new(rootJ, "op1LFO", json_integer(opButtonLFO[0]));
    json_object_set_new(rootJ, "op1syncMode", json_integer(opMenuSyncMode[0]));
    json_object_set_new(rootJ, "op1shapeMode", json_integer(opMenuShapeMode[0]));
    json_object_set_new(rootJ, "op1Mod1Dest", json_integer(opMod1Assign[0]));
    json_object_set_new(rootJ, "op1Mod2Dest", json_integer(opMod2Assign[0]));
    json_object_set_new(rootJ, "op1Mod3Dest", json_integer(opMod3Assign[0]));
    json_object_set_new(rootJ, "op1Mod4Dest", json_integer(opMod4Assign[0]));

    json_object_set_new(rootJ, "op2syncEnable", json_integer(opButtonSync[1]));
    json_object_set_new(rootJ, "op2weakSyncEnable", json_integer(opButtonWeakSync[1]));
    json_object_set_new(rootJ, "op2PostShape", json_integer(opButtonPostShape[1]));
    json_object_set_new(rootJ, "op2PreFade", json_integer(opButtonPreFade[1]));
    json_object_set_new(rootJ, "op2LFO", json_integer(opButtonLFO[1]));
    json_object_set_new(rootJ, "op2syncMode", json_integer(opMenuSyncMode[1]));
    json_object_set_new(rootJ, "op2shapeMode", json_integer(opMenuShapeMode[1]));
    json_object_set_new(rootJ, "op2Mod1Dest", json_integer(opMod1Assign[1]));
    json_object_set_new(rootJ, "op2Mod2Dest", json_integer(opMod2Assign[1]));
    json_object_set_new(rootJ, "op2Mod3Dest", json_integer(opMod3Assign[1]));
    json_object_set_new(rootJ, "op2Mod4Dest", json_integer(opMod4Assign[1]));

    json_object_set_new(rootJ, "op3syncEnable", json_integer(opButtonSync[2]));
    json_object_set_new(rootJ, "op3weakSyncEnable", json_integer(opButtonWeakSync[2]));
    json_object_set_new(rootJ, "op3PostShape", json_integer(opButtonPostShape[2]));
    json_object_set_new(rootJ, "op3PreFade", json_integer(opButtonPreFade[2]));
    json_object_set_new(rootJ, "op3LFO", json_integer(opButtonLFO[2]));
    json_object_set_new(rootJ, "op3syncMode", json_integer(opMenuSyncMode[2]));
    json_object_set_new(rootJ, "op3shapeMode", json_integer(opMenuShapeMode[2]));
    json_object_set_new(rootJ, "op3Mod1Dest", json_integer(opMod1Assign[2]));
    json_object_set_new(rootJ, "op3Mod2Dest", json_integer(opMod2Assign[2]));
    json_object_set_new(rootJ, "op3Mod3Dest", json_integer(opMod3Assign[2]));
    json_object_set_new(rootJ, "op3Mod4Dest", json_integer(opMod4Assign[2]));

    json_object_set_new(rootJ, "op4syncEnable", json_integer(opButtonSync[3]));
    json_object_set_new(rootJ, "op4weakSyncEnable", json_integer(opButtonWeakSync[3]));
    json_object_set_new(rootJ, "op4PostShape", json_integer(opButtonPostShape[3]));
    json_object_set_new(rootJ, "op4PreFade", json_integer(opButtonPreFade[3]));
    json_object_set_new(rootJ, "op4LFO", json_integer(opButtonLFO[3]));
    json_object_set_new(rootJ, "op4syncMode", json_integer(opMenuSyncMode[3]));
    json_object_set_new(rootJ, "op4shapeMode", json_integer(opMenuShapeMode[3]));
    json_object_set_new(rootJ, "op4Mod1Dest", json_integer(opMod1Assign[3]));
    json_object_set_new(rootJ, "op4Mod2Dest", json_integer(opMod2Assign[3]));
    json_object_set_new(rootJ, "op4Mod3Dest", json_integer(opMod3Assign[3]));
    json_object_set_new(rootJ, "op4Mod4Dest", json_integer(opMod4Assign[3]));

    json_object_set_new(rootJ, "panelStyle", json_integer(panelStyle));
    json_object_set_new(rootJ, "opSyncSource", json_integer((int)opSyncSource));
    json_object_set_new(rootJ, "masterLFO", json_integer(masterLFO));
    json_object_set_new(rootJ, "fullInversion", json_integer(buttonFullInversion));
    json_object_set_new(rootJ, "indivOutputSource", json_integer(indivBOutputs));

    return rootJ;
}

void Dexter::dataFromJson(json_t *rootJ) {
    json_t *j_opSyncEnable = json_object_get(rootJ, "op1syncEnable");
    json_t *j_opWeakSync = json_object_get(rootJ, "op1weakSyncEnable");
    json_t *j_opPostShape = json_object_get(rootJ, "op1PostShape");
    json_t *j_opPreFade = json_object_get(rootJ, "op1PreFade");
    json_t *j_opLFO = json_object_get(rootJ, "op1LFO");
    json_t *j_opSyncMode = json_object_get(rootJ, "op1syncMode");
    json_t *j_opShapeMode = json_object_get(rootJ, "op1shapeMode");
    json_t *j_opMod1Assign = json_object_get(rootJ, "op1Mod1Dest");
    json_t *j_opMod2Assign = json_object_get(rootJ, "op2Mod1Dest");
    json_t *j_opMod3Assign = json_object_get(rootJ, "op3Mod1Dest");
    json_t *j_opMod4Assign = json_object_get(rootJ, "op4Mod1Dest");

    opButtonSync[0] = json_integer_value(j_opSyncEnable);
    opButtonWeakSync[0] = json_integer_value(j_opWeakSync);
    opButtonPostShape[0] = json_integer_value(j_opPostShape);
    opButtonPreFade[0] = json_integer_value(j_opPreFade);
    opButtonLFO[0] = json_integer_value(j_opLFO);
    opMenuSyncMode[0] = json_integer_value(j_opSyncMode);
    opMenuShapeMode[0] = json_integer_value(j_opShapeMode);
    opMod1Assign[0] = json_integer_value(j_opMod1Assign);
    opMod2Assign[0] = json_integer_value(j_opMod2Assign);
    opMod3Assign[0] = json_integer_value(j_opMod3Assign);
    opMod4Assign[0] = json_integer_value(j_opMod4Assign);

    j_opSyncEnable = json_object_get(rootJ, "op2syncEnable");
    j_opWeakSync = json_object_get(rootJ, "op2weakSyncEnable");
    j_opPostShape = json_object_get(rootJ, "op2PostShape");
    j_opPreFade = json_object_get(rootJ, "op2PreFade");
    j_opLFO = json_object_get(rootJ, "op2LFO");
    j_opSyncMode = json_object_get(rootJ, "op2syncMode");
    j_opShapeMode = json_object_get(rootJ, "op2shapeMode");
    j_opMod1Assign = json_object_get(rootJ, "op2Mod1Dest");
    j_opMod2Assign = json_object_get(rootJ, "op2Mod2Dest");
    j_opMod3Assign = json_object_get(rootJ, "op2Mod3Dest");
    j_opMod4Assign = json_object_get(rootJ, "op2Mod4Dest");
    opButtonSync[1] = json_integer_value(j_opSyncEnable);
    opButtonWeakSync[1] = json_integer_value(j_opWeakSync);
    opButtonPostShape[1] = json_integer_value(j_opPostShape);
    opButtonPreFade[1] = json_integer_value(j_opPreFade);
    opButtonLFO[1] = json_integer_value(j_opLFO);
    opMenuSyncMode[1] = json_integer_value(j_opSyncMode);
    opMenuShapeMode[1] = json_integer_value(j_opShapeMode);
    opMod1Assign[1] = json_integer_value(j_opMod1Assign);
    opMod2Assign[1] = json_integer_value(j_opMod2Assign);
    opMod3Assign[1] = json_integer_value(j_opMod3Assign);
    opMod4Assign[1] = json_integer_value(j_opMod4Assign);

    j_opSyncEnable = json_object_get(rootJ, "op3syncEnable");
    j_opWeakSync = json_object_get(rootJ, "op3weakSyncEnable");
    j_opPostShape = json_object_get(rootJ, "op3PostShape");
    j_opPreFade = json_object_get(rootJ, "op3PreFade");
    j_opLFO = json_object_get(rootJ, "op3LFO");
    j_opSyncMode = json_object_get(rootJ, "op3syncMode");
    j_opShapeMode = json_object_get(rootJ, "op3shapeMode");
    j_opMod1Assign = json_object_get(rootJ, "op3Mod1Dest");
    j_opMod2Assign = json_object_get(rootJ, "op3Mod2Dest");
    j_opMod3Assign = json_object_get(rootJ, "op3Mod3Dest");
    j_opMod4Assign = json_object_get(rootJ, "op3Mod4Dest");
    opButtonSync[2] = json_integer_value(j_opSyncEnable);
    opButtonWeakSync[2] = json_integer_value(j_opWeakSync);
    opButtonPostShape[2] = json_integer_value(j_opPostShape);
    opButtonPreFade[2] = json_integer_value(j_opPreFade);
    opButtonLFO[2] = json_integer_value(j_opLFO);
    opMenuSyncMode[2] = json_integer_value(j_opSyncMode);
    opMenuShapeMode[2] = json_integer_value(j_opShapeMode);
    opMod1Assign[2] = json_integer_value(j_opMod1Assign);
    opMod2Assign[2] = json_integer_value(j_opMod2Assign);
    opMod3Assign[2] = json_integer_value(j_opMod3Assign);
    opMod4Assign[2] = json_integer_value(j_opMod4Assign);

    j_opSyncEnable = json_object_get(rootJ, "op4syncEnable");
    j_opWeakSync = json_object_get(rootJ, "op4weakSyncEnable");
    j_opPostShape = json_object_get(rootJ, "op4PostShape");
    j_opPreFade = json_object_get(rootJ, "op4PreFade");
    j_opLFO = json_object_get(rootJ, "op4LFO");
    j_opSyncMode = json_object_get(rootJ, "op4syncMode");
    j_opShapeMode = json_object_get(rootJ, "op4shapeMode");
    j_opMod1Assign = json_object_get(rootJ, "op4Mod1Dest");
    j_opMod2Assign = json_object_get(rootJ, "op4Mod2Dest");
    j_opMod3Assign = json_object_get(rootJ, "op4Mod3Dest");
    j_opMod4Assign = json_object_get(rootJ, "op4Mod4Dest");
    opButtonSync[3] = json_integer_value(j_opSyncEnable);
    opButtonWeakSync[3] = json_integer_value(j_opWeakSync);
    opButtonPostShape[3] = json_integer_value(j_opPostShape);
    opButtonPreFade[3] = json_integer_value(j_opPreFade);
    opButtonLFO[3] = json_integer_value(j_opLFO);
    opMenuSyncMode[3] = json_integer_value(j_opSyncMode);
    opMenuShapeMode[3] = json_integer_value(j_opShapeMode);
    opMod1Assign[3] = json_integer_value(j_opMod1Assign);
    opMod2Assign[3] = json_integer_value(j_opMod2Assign);
    opMod3Assign[3] = json_integer_value(j_opMod3Assign);
    opMod4Assign[3] = json_integer_value(j_opMod4Assign);

    json_t *j_panelStyle = json_object_get(rootJ, "panelStyle");
    panelStyle = json_integer_value(j_panelStyle);

    json_t *j_opSyncToParent = json_object_get(rootJ, "opSyncSource");
    opSyncSource = (FourVoiceOPCore::OpSyncSource)json_integer_value(j_opSyncToParent);

    json_t *j_masterLFO = json_object_get(rootJ, "masterLFO");
    masterLFO = json_integer_value(j_masterLFO);

    json_t *j_fullInversion = json_object_get(rootJ, "fullInversion");
    buttonFullInversion = json_integer_value(j_fullInversion);

    json_t *j_indivOutputSource = json_object_get(rootJ, "indivOutputSource");
    indivBOutputs = json_integer_value(j_indivOutputSource);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// UI ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

AlgoGraphic::AlgoGraphic() {
    sw = new SvgWidget();
    addChild(sw);
    value = 0;
    style = 0;
    styleOffset = 0;
    std::string algoGraphicFile;
    for(auto i = 0; i < 2; ++i) {
        for(auto j = 0; j < kNumAlgorithms; ++j) {
            algoGraphicFile = "res/algo" + std::to_string(j);
            if(i) {
                algoGraphicFile += "Dark";
            }
            algoGraphicFile += ".svg";
            addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, algoGraphicFile)));
        }
    }
}

void AlgoGraphic::addFrame(std::shared_ptr<Svg> svg) {
    frames.push_back(svg);
    // If this is our first frame, automatically set Svg and size
    if (!sw->svg) {
        sw->setSvg(svg);
        box.size = sw->box.size;
    }
}

void AlgoGraphic::step() {
    if(style == 0) {
        styleOffset = 0;
    }
    else {
        styleOffset = kNumAlgorithms;
    }
    int index = clamp(value + styleOffset, 0, frames.size() - 1);
    sw->setSvg(frames[index]);
    dirty = true;
    FramebufferWidget::step();
}


void OpSyncModeItem::onAction(const event::Action& e) {
    if (pSyncMode)
    *pSyncMode = syncMode;
}

void OpSyncModeChoice::onAction(const event::Action& e) {
    if (!pSyncMode) {
        return;
    }

    ui::Menu* menu = createMenu();
    for (size_t i = 0; i < syncModeLabels.size(); ++i) {
        OpSyncModeItem* item = new OpSyncModeItem;
        item->pSyncMode = pSyncMode;
        item->syncMode = i;
        item->text = syncModeLabels[i];
        item->rightText = CHECKMARK(item->syncMode == *pSyncMode);
        menu->addChild(item);
    }
}

void OpSyncModeChoice::step() {
    text = pSyncMode ? syncModeLabels[*pSyncMode] : syncModeLabels[0];
}


void OpShapeModeItem::onAction(const event::Action& e) {
    if (pShapeMode)
    *pShapeMode = shapeMode;
}

void OpShapeModeChoice::onAction(const event::Action& e) {
    if (!pShapeMode) {
        return;
    }

    ui::Menu* menu = createMenu();
    for (size_t i = 0; i < shapeModeLabels.size(); ++i) {
        OpShapeModeItem* item = new OpShapeModeItem;
        item->pShapeMode = pShapeMode;
        item->shapeMode = i;
        item->text = shapeModeLabels[i];
        item->rightText = CHECKMARK(item->shapeMode == *pShapeMode);
        menu->addChild(item);
    }
}

void OpShapeModeChoice::step() {
    text = pShapeMode ? shapeModeLabels[*pShapeMode] : shapeModeLabels[0];
}


void OpModModeItem::onAction(const event::Action& e) {
    if (pModMode)
    *pModMode = modMode;
}

void OpModModeChoice::onAction(const event::Action& e) {
    if (!pModMode) {
        return;
    }

    ui::Menu* menu = createMenu();
    for (size_t i = 0; i < modModeLabels.size(); ++i) {
        OpModModeItem* item = new OpModModeItem;
        item->pModMode = pModMode;
        item->modMode = i;
        item->text = modModeLabels[i];
        item->rightText = CHECKMARK(item->modMode == *pModMode);
        menu->addChild(item);
    }
}

void OpModModeChoice::step() {
    text = pModMode ? modModeLabels[*pModMode] : modModeLabels[0];
}

// The widget

DexterWidget::DexterWidget(Dexter *module) {
    setModule(module);

    darkPanel = new SvgPanel;
    darkPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DexterPanelDark.svg")));
    if(module) {
        lightPanel = new SvgPanel;
        lightPanel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DexterPanelLight.svg")));
        lightPanel->visible = false;
        addChild(lightPanel);
    }
    setPanel(darkPanel);

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    algo = new AlgoGraphic;
    algo->box.pos = Vec(138.75, 88.245);
    addChild(algo);

    RoganMedBlue* OctaveAKnob = createParam<RoganMedBlue>(OctaveAKnobPos, module, Dexter::OCTAVE_PARAM);
    if (module) {
        OctaveAKnob->smooth = false;
        OctaveAKnob->getParamQuantity()->snapEnabled = true;
    }
    addParam(OctaveAKnob);

    addParam(createParam<RoganMedBlue>(CoarseKnobPos, module, Dexter::COARSE_PARAM));
    addParam(createParam<RoganMedBlue>(FineKnobPos, module, Dexter::FINE_PARAM));

    RoganMedSmallBlue* ChordKnob = createParam<RoganMedSmallBlue>(ChordKnobPos, module, Dexter::CHORD_PARAM);
    if (module) {
        ChordKnob->smooth = false;
        ChordKnob->getParamQuantity()->snapEnabled = true;
    }
    addParam(ChordKnob);

    RoganMedSmallBlue* InvertKnob = createParam<RoganMedSmallBlue>(InvertKnobPos, module, Dexter::INVERT_PARAM);
    if (module) {
        InvertKnob->smooth = false;
        InvertKnob->getParamQuantity()->snapEnabled = true;
    }
    addParam(InvertKnob);

    addParam(createParam<RoganMedSmallBlue>(DetuneKnobPos, module, Dexter::DETUNE_PARAM));

    RoganMedBlue* Octave2Knob = createParam<RoganMedBlue>(Octave2KnobPos, module, Dexter::B_OCTAVE_PARAM);
    if (module) {
        Octave2Knob->smooth = false;
        Octave2Knob->getParamQuantity()->snapEnabled = true;
    }
    addParam(Octave2Knob);

    addParam(createParam<RoganMedBlue>(Coarse2KnobPos, module, Dexter::B_COARSE_PARAM));
    addParam(createParam<RoganMedBlue>(Fine2KnobPos, module, Dexter::B_FINE_PARAM));

    RoganMedBlue* AlgoKnob = createParam<RoganMedBlue>(AlgoKnobPos, module, Dexter::ALGORITHM_PARAM);
    if (module) {
        AlgoKnob->smooth = false;
        AlgoKnob->getParamQuantity()->snapEnabled = true;
    }
    addParam(AlgoKnob);

    addParam(createParam<RoganMedGreen>(BrightKnobPos, module, Dexter::BRIGHTNESS_PARAM));
    addParam(createParam<RoganMedRed>(ShapeKnobPos, module, Dexter::SHAPE_PARAM));
    addParam(createParam<RoganMedOrange>(FeedbackKnobPos, module, Dexter::FEEDBACK_PARAM));

    addParam(createParam<RoganSmallBlue>(ChordDepthKnobPos, module, Dexter::CHORD_DEPTH_PARAM));
    addParam(createParam<RoganSmallBlue>(InvertDepthKnobPos, module, Dexter::INVERT_DEPTH_PARAM));
    addParam(createParam<RoganSmallBlue>(DetuneDepthKnobPos, module, Dexter::DETUNE_DEPTH_PARAM));
    addParam(createParam<RoganSmallBlue>(AlgoDepthKnobPos, module, Dexter::ALGO_DEPTH_PARAM));
    addParam(createParam<RoganSmallOrange>(FBDepthKnobPos, module, Dexter::FB_DEPTH_PARAM));
    addParam(createParam<RoganSmallGreen>(BrightDepthKnobPos, module, Dexter::BRIGHT_DEPTH_PARAM));
    addParam(createParam<RoganSmallRed>(ShapeDepthKnobPos, module, Dexter::SHAPE_DEPTH_PARAM));

    addParam(createParam<LightLEDButton>(MasterLFOButtonPos, module, Dexter::MASTER_LFO_BUTTON));
    addParam(createParam<LightLEDButton>(ResetPhaseButtonPos, module, Dexter::RESET_PHASE_BUTTON));
    addParam(createParam<LightLEDButton>(FullInversionButtonPos, module, Dexter::FULL_INVERSION_BUTTON));

    addChild(createLight<MediumLight<RedLight>>(MasterLFOButtonLEDPos, module, Dexter::MASTER_LFO_LIGHT));
    addChild(createLight<MediumLight<RedLight>>(ResetPhaseButtonLEDPos, module, Dexter::RESET_PHASE_LIGHT));
    addChild(createLight<MediumLight<RedLight>>(FullInversionButtonLEDPos, module, Dexter::FULL_INVERSION_LIGHT));

    addOutput(createOutput<PJ301MDarkSmallOut>(AOutLeftJack, module, Dexter::A_LEFT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(AOutRightJack, module, Dexter::A_RIGHT_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Op1OutJack, module, Dexter::OP_1_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Op2OutJack, module, Dexter::OP_2_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Op3OutJack, module, Dexter::OP_3_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(Op4OutJack, module, Dexter::OP_4_OUTPUT));
    addOutput(createOutput<PJ301MDarkSmallOut>(BOutJack, module, Dexter::B_OUTPUT));
    addInput(createInput<PJ301MDarkSmall>(VOct1CVJack, module, Dexter::A_VOCT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(VOct2CVJack, module, Dexter::B_VOCT_INPUT));

    addInput(createInput<PJ301MDarkSmall>(ChordCVJack, module, Dexter::CHORD_INPUT));
    addInput(createInput<PJ301MDarkSmall>(InvertCVJack, module, Dexter::INVERT_INPUT));
    addInput(createInput<PJ301MDarkSmall>(DetuneCVJack, module, Dexter::DETUNE_INPUT));
    addInput(createInput<PJ301MDarkSmall>(AlgorithmCVJack, module, Dexter::ALGO_INPUT));
    addInput(createInput<PJ301MDarkSmall>(FeedbackCVJack, module, Dexter::FEEDBACK_INPUT));
    addInput(createInput<PJ301MDarkSmall>(BrightCVJack, module, Dexter::BRIGHTNESS_INPUT));
    addInput(createInput<PJ301MDarkSmall>(ShapeCVJack, module, Dexter::SHAPE_INPUT));

    std::string fontPath = "res/din1451alt.ttf";
    chordText = new PlainText;
    chordText->size = 12;
    chordText->fontPath = fontPath;
    chordText->box.pos = Vec(24.91, 124.468);
    chordText->box.size = Vec(82, 14);
    chordText->color = nvgRGB(0xFF, 0xFF, 0xFF);
    addChild(chordText);

    octaveAText = new PlainText;
    octaveAText->size = 14;
    octaveAText->fontPath = fontPath;
    octaveAText->box.pos = Vec(35.61, 68.076);
    octaveAText->box.size = Vec(82, 14);
    octaveAText->color = nvgRGB(0xFF, 0xFF, 0xFF);
    octaveAText->text = "0";
    addChild(octaveAText);

    octaveBText = new PlainText;
    octaveBText->size = 14;
    octaveBText->fontPath = fontPath;
    octaveBText->box.pos = Vec(35.61, 186.076);
    octaveBText->box.size = Vec(82, 14);
    octaveBText->color = nvgRGB(0xFF, 0xFF, 0xFF);
    octaveBText->text = "0";
    addChild(octaveBText);

    // Operator Controls
    float operatorSpacing = 125.5275;
    float offset = 0.0;
    for(auto op = 0; op < kNumOperators; ++op) {
        offset = operatorSpacing * op;

        OpMultKnob[op] = new RoganMedBlue;
        OpMultKnob[op] = createParam<RoganMedBlue>(Vec(OpMultKnobRootX + offset, OpRow1Y), module,
                                                       Dexter::OP_1_MULT_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        OpMultKnob[op]->snap = true;
        addChild(OpMultKnob[op]);

        OpCoarseKnob[op] = new RoganMedBlue;
        OpCoarseKnob[op] = createParam<RoganMedBlue>(Vec(OpCoarseKnobRootX + offset, OpRow1Y), module,
                                                         Dexter::OP_1_COARSE_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        addChild(OpCoarseKnob[op]);

        OpFineKnob[op] = new RoganMedBlue;
        OpFineKnob[op] = createParam<RoganMedBlue>(Vec(OpFineKnobRootX + offset, OpRow1Y), module,
                                                         Dexter::OP_1_FINE_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        addChild(OpFineKnob[op]);

        OpWaveKnob[op] = new RoganMedPurple;
        OpWaveKnob[op] = createParam<RoganMedPurple>(Vec(OpWaveKnobRootX + offset, OpRow2Y), module,
                                                         Dexter::OP_1_WAVE_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        addChild(OpWaveKnob[op]);

        OpShapeKnob[op] = new RoganMedRed;
        OpShapeKnob[op] = createParam<RoganMedRed>(Vec(OpShapeKnobRootX + offset, OpRow2Y), module,
                                                         Dexter::OP_1_SHAPE_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        addChild(OpShapeKnob[op]);

        OpLevelKnob[op] = new RoganMedGreen;
        OpLevelKnob[op] = createParam<RoganMedGreen>(Vec(OpLevelKnobRootX + offset, OpRow2Y), module,
                                                         Dexter::OP_1_LEVEL_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        addChild(OpLevelKnob[op]);

        multText[op] = new PlainText;
        multText[op]->box.pos = Vec(OpMainTextX[0] + offset + 19, OpMainTextY[0] - 2.718);
        multText[op]->size = 14;
        multText[op]->fontPath = fontPath;
        multText[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        multText[op]->text = "1";
        addChild(multText[op]);

        // Buttons
        addParam(createParam<LightLEDButton>(Vec(OpPreButtonPosX + offset, OpPreButtonPosY), module, Dexter::OP_1_PRE_PARAM + Dexter::NUM_PARAM_GROUPS * op));
        addParam(createParam<LightLEDButton>(Vec(OpSettingsButtonRootX + offset, OpSettingsButtonRootY), module, Dexter::OP_1_SETTINGS_PARAM + Dexter::NUM_PARAM_GROUPS * op));
        addParam(createParam<LightLEDButton>(Vec(OpPercButtonRootX + offset, OpPercButtonRootY), module, Dexter::OP_1_POST_SHAPE_PARAM + Dexter::NUM_PARAM_GROUPS * op));
        addParam(createParam<LightLEDButton>(Vec(OpIsolateButtonRootX + offset, OpIsolateButtonRootY), module, Dexter::OP_1_WEAK_PARAM + Dexter::NUM_PARAM_GROUPS * op));
        addParam(createParam<LightLEDButton>(Vec(OpLFOButtonRootX + offset, OpLFOButtonRootY), module, Dexter::OP_1_LFO_PARAM + Dexter::NUM_PARAM_GROUPS * op));
        addParam(createParam<LightLEDButton>(Vec(OpSyncButtonRootX + offset, OpSyncButtonRootY), module, Dexter::OP_1_SYNC_PARAM + Dexter::NUM_PARAM_GROUPS * op));
        addChild(createLight<MediumLight<RedLight>>(Vec(OpPreButtonPosX + offset + ledOffset, OpPreButtonPosY + ledOffset),
                                                    module, Dexter::OP_1_PRE_LIGHT + Dexter::NUM_LIGHT_GROUPS * op));
        addChild(createLight<MediumLight<RedLight>>(Vec(OpSettingsButtonRootX + offset + ledOffset, OpSettingsButtonRootY + ledOffset),
                                                    module, Dexter::OP_1_SETTINGS_LIGHT + Dexter::NUM_LIGHT_GROUPS * op));
        addChild(createLight<MediumLight<RedLight>>(Vec(OpPercButtonRootX + offset + ledOffset, OpPercButtonRootY + ledOffset),
                                                    module, Dexter::OP_1_POST_SHAPE_LIGHT + Dexter::NUM_LIGHT_GROUPS * op));
        addChild(createLight<MediumLight<RedLight>>(Vec(OpIsolateButtonRootX + offset + ledOffset, OpIsolateButtonRootY + ledOffset),
                                                    module, Dexter::OP_1_WEAK_LIGHT + Dexter::NUM_LIGHT_GROUPS * op));
        addChild(createLight<MediumLight<RedLight>>(Vec(OpLFOButtonRootX + offset + ledOffset, OpLFOButtonRootY + ledOffset),
                                                    module, Dexter::OP_1_LFO_LIGHT + Dexter::NUM_LIGHT_GROUPS * op));
        addChild(createLight<MediumLight<RedLight>>(Vec(OpSyncButtonRootX + offset + ledOffset, OpSyncButtonRootY + ledOffset),
                                                    module, Dexter::OP_1_SYNC_LIGHT + Dexter::NUM_LIGHT_GROUPS * op));

        // Text for all the main controls, hidden when the settings menu opens
        for(auto i = 0; i < 6; ++i){
            mainText[op][i] = new PlainText;
            mainText[op][i]->box.pos = Vec(OpMainTextX[i] + offset, OpMainTextY[i] - 2.718);
            mainText[op][i]->size = 14;
            mainText[op][i]->fontPath = fontPath;
            mainText[op][i]->color = nvgRGB(0xFF, 0xFF, 0xFF);
            mainText[op][i]->text = OpMainText[i];
            addChild(mainText[op][i]);
        }

        // Settings Menu
        opWaveButton[op] = createParam<LightLEDButtonNonDyn>(Vec(OpWaveButtonX + offset, 110.375), module,
                                                             Dexter::OP_1_WAVE_MENU_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        opWaveButton[op]->visible = false;
        addChild(opWaveButton[op]);

        opModAButton[op] = createParam<LightLEDButtonNonDyn>(Vec(OpModAButtonX + offset, 110.375), module,
                                                             Dexter::OP_1_MODA_MENU_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        opModAButton[op]->visible = false;
        addChild(opModAButton[op]);

        opModBButton[op] = createParam<LightLEDButtonNonDyn>(Vec(OpModBButtonX + offset, 110.375), module,
                                                             Dexter::OP_1_MODB_MENU_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        opModBButton[op]->visible = false;
        addChild(opModBButton[op]);

        opWaveButtonLight[op] = createLight<MediumLight<RedLight>>(Vec(OpWaveButtonX + offset + ledOffset,
                                                                   110.375 + ledOffset), module,
                                                                   Dexter::OP_1_WAVE_MENU_LIGHT + Dexter::NUM_LIGHT_GROUPS * op);
        opWaveButtonLight[op]->visible = false;
        addChild(opWaveButtonLight[op]);

        opModAButtonLight[op] = createLight<MediumLight<RedLight>>(Vec(OpModAButtonX + offset + ledOffset,
                                                                   110.375 + ledOffset), module,
                                                                   Dexter::OP_1_MODA_MENU_LIGHT + Dexter::NUM_LIGHT_GROUPS * op);
        opModAButtonLight[op]->visible = false;
        addChild(opModAButtonLight[op]);

        opModBButtonLight[op] = createLight<MediumLight<RedLight>>(Vec(OpModBButtonX + offset + ledOffset,
                                                                   110.375 + ledOffset), module,
                                                                   Dexter::OP_1_MODB_MENU_LIGHT + Dexter::NUM_LIGHT_GROUPS * op);
        opModBButtonLight[op]->visible = false;
        addChild(opModBButtonLight[op]);

        waveTableTabTopLabel[op] = new PlainText;
        waveTableTabTopLabel[op]->box.pos = Vec(203.2045 + offset, 125);
        waveTableTabTopLabel[op]->size = 12;
        waveTableTabTopLabel[op]->fontPath = fontPath;
        waveTableTabTopLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        waveTableTabTopLabel[op]->text = "Wave";
        waveTableTabTopLabel[op]->visible = false;
        addChild(waveTableTabTopLabel[op]);

        waveTableTabBottomLabel[op] = new PlainText;
        waveTableTabBottomLabel[op]->box.pos = Vec(203.2045 + offset, 134);
        waveTableTabBottomLabel[op]->size = 12;
        waveTableTabBottomLabel[op]->fontPath = fontPath;
        waveTableTabBottomLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        waveTableTabBottomLabel[op]->text = "Table";
        waveTableTabBottomLabel[op]->visible = false;
        addChild(waveTableTabBottomLabel[op]);

        mod1And2TabTopLabel[op] = new PlainText;
        mod1And2TabTopLabel[op]->box.pos = Vec(234.6105 + offset, 125);
        mod1And2TabTopLabel[op]->size = 12;
        mod1And2TabTopLabel[op]->fontPath = fontPath;
        mod1And2TabTopLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod1And2TabTopLabel[op]->text = "Mod";
        mod1And2TabTopLabel[op]->visible = false;
        addChild(mod1And2TabTopLabel[op]);

        mod1And2TabBottomLabel[op] = new PlainText;
        mod1And2TabBottomLabel[op]->box.pos = Vec(234.6105 + offset, 134);
        mod1And2TabBottomLabel[op]->size = 12;
        mod1And2TabBottomLabel[op]->fontPath = fontPath;
        mod1And2TabBottomLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod1And2TabBottomLabel[op]->text = "1 & 2";
        mod1And2TabBottomLabel[op]->visible = false;
        addChild(mod1And2TabBottomLabel[op]);

        mod3And4TabTopLabel[op] = new PlainText;
        mod3And4TabTopLabel[op]->box.pos = Vec(266.0295 + offset, 125);
        mod3And4TabTopLabel[op]->size = 12;
        mod3And4TabTopLabel[op]->fontPath = fontPath;
        mod3And4TabTopLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod3And4TabTopLabel[op]->text = "Mod";
        mod3And4TabTopLabel[op]->visible = false;
        addChild(mod3And4TabTopLabel[op]);

        mod3And4TabBottomLabel[op] = new PlainText;
        mod3And4TabBottomLabel[op]->box.pos = Vec(266.0295 + offset, 134);
        mod3And4TabBottomLabel[op]->size = 12;
        mod3And4TabBottomLabel[op]->fontPath = fontPath;
        mod3And4TabBottomLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod3And4TabBottomLabel[op]->text = "3 & 4";
        mod3And4TabBottomLabel[op]->visible = false;
        addChild(mod3And4TabBottomLabel[op]);

        waveBankKnob[op] = createParam<RoganMedBlue>(Vec(OpTableKnobRootX + offset, OpRow1Y), module,
                                                         Dexter::OP_1_BANK_PARAM + Dexter::NUM_PARAM_GROUPS * op);
        waveBankKnob[op]->visible = false;
        waveBankKnob[op]->snap = true;
        addChild(waveBankKnob[op]);

        syncModeChoice[op] = new OpSyncModeChoice;
        syncModeChoice[op]->box.pos = Vec(238.819 + offset, 55.25);
        syncModeChoice[op]->box.size.x = 67.806;
        syncModeChoice[op]->pSyncMode = &module->opMenuSyncMode[op];
        syncModeChoice[op]->visible = false;
        addChild(syncModeChoice[op]);

        shapeModeChoice[op] = new OpShapeModeChoice;
        shapeModeChoice[op]->box.pos = Vec(238.819 + offset, 87.125);
        shapeModeChoice[op]->box.size.x = 67.806;
        shapeModeChoice[op]->pShapeMode = &module->opMenuShapeMode[op];
        shapeModeChoice[op]->visible = false;
        addChild(shapeModeChoice[op]);

        tableLabel[op] = new PlainText;
        tableLabel[op]->box.pos = Vec(211.062 + offset, 81.468 - 2.718);
        tableLabel[op]->size = 14;
        tableLabel[op]->fontPath = fontPath;
        tableLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        tableLabel[op]->text = "Table";
        tableLabel[op]->visible = false;
        addChild(tableLabel[op]);

        syncModeLabel[op] = new PlainText;
        syncModeLabel[op]->box.pos = Vec(273.222 + offset, 45.058 - 4.0);
        syncModeLabel[op]->size = 14;
        syncModeLabel[op]->fontPath = fontPath;
        syncModeLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        syncModeLabel[op]->text = "Sync Mode";
        syncModeLabel[op]->visible = false;
        addChild(syncModeLabel[op]);

        shapeModeLabel[op] = new PlainText;
        shapeModeLabel[op]->box.pos = Vec(273.222 + offset, 77.933 - 4.0);
        shapeModeLabel[op]->size = 14;
        shapeModeLabel[op]->fontPath = fontPath;
        shapeModeLabel[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        shapeModeLabel[op]->text = "Shape Mode";
        shapeModeLabel[op]->visible = false;
        addChild(shapeModeLabel[op]);

        tableText[op] = new PlainText;
        tableText[op]->box.pos = Vec(211.062 + offset, 91.468);
        tableText[op]->box.size = Vec(82, 14);
        tableText[op]->size = 12;
        tableText[op]->fontPath = fontPath;
        tableText[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        tableText[op]->text = waveTableNames[0];
        tableText[op]->visible = false;
        addChild(tableText[op]);

        // Mod Menus
        mod1ModeChoice[op] = new OpModModeChoice;
        mod1ModeChoice[op]->box.pos = Vec(226.f + offset, 55.25f);
        mod1ModeChoice[op]->box.size.x = 80.f;
        mod1ModeChoice[op]->pModMode = &module->opMod1Assign[op];
        mod1ModeChoice[op]->visible = false;
        addChild(mod1ModeChoice[op]);

        mod2ModeChoice[op] = new OpModModeChoice;
        mod2ModeChoice[op]->box.pos = Vec(226.f + offset,  87.125f);
        mod2ModeChoice[op]->box.size.x = 80.f;
        mod2ModeChoice[op]->pModMode = &module->opMod2Assign[op];
        mod2ModeChoice[op]->visible = false;
        addChild(mod2ModeChoice[op]);

        mod3ModeChoice[op] = new OpModModeChoice;
        mod3ModeChoice[op]->box.pos = Vec(226.f + offset, 55.25f);
        mod3ModeChoice[op]->box.size.x = 80.f;
        mod3ModeChoice[op]->pModMode = &module->opMod3Assign[op];
        mod3ModeChoice[op]->visible = false;
        addChild(mod3ModeChoice[op]);

        mod4ModeChoice[op] = new OpModModeChoice;
        mod4ModeChoice[op]->box.pos = Vec(226.f + offset, 87.125f);
        mod4ModeChoice[op]->box.size.x = 80.f;
        mod4ModeChoice[op]->pModMode = &module->opMod4Assign[op];
        mod4ModeChoice[op]->visible = false;
        addChild(mod4ModeChoice[op]);

        mod1Label[op] = new PlainText;
        mod1Label[op]->box.pos = Vec(206.f + offset, 61.058f - 4.f);
        mod1Label[op]->size = 14;
        mod1Label[op]->fontPath = fontPath;
        mod1Label[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod1Label[op]->text = "Mod 1";
        mod1Label[op]->visible = false;
        addChild(mod1Label[op]);

        mod2Label[op] = new PlainText;
        mod2Label[op]->box.pos = Vec(206.f + offset, 92.933f - 4.f);
        mod2Label[op]->size = 14;
        mod2Label[op]->fontPath = fontPath;
        mod2Label[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod2Label[op]->text = "Mod 2";
        mod2Label[op]->visible = false;
        addChild(mod2Label[op]);

        mod3Label[op] = new PlainText;
        mod3Label[op]->box.pos = Vec(206.f + offset, 61.058f - 4.f);
        mod3Label[op]->size = 14;
        mod3Label[op]->fontPath = fontPath;
        mod3Label[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod3Label[op]->text = "Mod 3";
        mod3Label[op]->visible = false;
        addChild(mod3Label[op]);

        mod4Label[op] = new PlainText;
        mod4Label[op]->box.pos = Vec(206.f + offset, 92.933f - 4.f);
        mod4Label[op]->size = 14;
        mod4Label[op]->fontPath = fontPath;
        mod4Label[op]->color = nvgRGB(0xFF, 0xFF, 0xFF);
        mod4Label[op]->text = "Mod 4";
        mod4Label[op]->visible = false;
        addChild(mod4Label[op]);

        addParam(createParam<RoganSmallMustard>(Vec(OpMod1KnobRootX + offset, OpModRowY), module, Dexter::OP_1_MOD_1_PARAM + 12 * op));
        addParam(createParam<RoganSmallMustard>(Vec(OpMod2KnobRootX + offset, OpModRowY), module, Dexter::OP_1_MOD_2_PARAM + 12 * op));
        addParam(createParam<RoganSmallMustard>(Vec(OpMod3KnobRootX + offset, OpModRowY), module, Dexter::OP_1_MOD_3_PARAM + 12 * op));
        addParam(createParam<RoganSmallMustard>(Vec(OpMod4KnobRootX + offset, OpModRowY), module, Dexter::OP_1_MOD_4_PARAM + 12 * op));

        addParam(createParam<RoganSmallBlue>(Vec(OpPitch1KnobRootX + offset, OpCV1RowY), module, Dexter::OP_1_PITCH_CV1_PARAM + 12 * op));
        addParam(createParam<RoganSmallPurple>(Vec(OpWave1KnobRootX + offset, OpCV1RowY), module, Dexter::OP_1_WAVE_CV1_PARAM + 12 * op));
        addParam(createParam<RoganSmallRed>(Vec(OpShape1KnobRootX + offset, OpCV1RowY), module, Dexter::OP_1_SHAPE_CV1_PARAM + 12 * op));
        addParam(createParam<RoganSmallGreen>(Vec(OpLevel1KnobRootX + offset, OpCV1RowY), module, Dexter::OP_1_LEVEL_CV1_PARAM + 12 * op));

        addParam(createParam<RoganSmallBlue>(Vec(OpPitch2KnobRootX + offset, OpCV2RowY), module, Dexter::OP_1_PITCH_CV2_PARAM + 12 * op));
        addParam(createParam<RoganSmallPurple>(Vec(OpWave2KnobRootX + offset, OpCV2RowY), module, Dexter::OP_1_WAVE_CV2_PARAM + 12 * op));
        addParam(createParam<RoganSmallRed>(Vec(OpShape2KnobRootX + offset, OpCV2RowY), module, Dexter::OP_1_SHAPE_CV2_PARAM + 12 * op));
        addParam(createParam<RoganSmallGreen>(Vec(OpLevel2KnobRootX + offset, OpCV2RowY), module, Dexter::OP_1_LEVEL_CV2_PARAM + 12 * op));

        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV1JackRootX + offset, OpModJackRowY), module, Dexter::OP_1_MOD_1_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV2JackRootX + offset, OpModJackRowY), module, Dexter::OP_1_MOD_2_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV3JackRootX + offset, OpModJackRowY), module, Dexter::OP_1_MOD_3_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV4JackRootX + offset, OpModJackRowY), module, Dexter::OP_1_MOD_4_INPUT + 12 * op));

        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV1JackRootX + offset, OpCV1JackRootY), module, Dexter::OP_1_PITCH_1_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV2JackRootX + offset, OpCV1JackRootY), module, Dexter::OP_1_WAVE_1_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV3JackRootX + offset, OpCV1JackRootY), module, Dexter::OP_1_SHAPE_1_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV4JackRootX + offset, OpCV1JackRootY), module, Dexter::OP_1_LEVEL_1_INPUT + 12 * op));

        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV1JackRootX + offset, OpCV2JackRootY), module, Dexter::OP_1_PITCH_2_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV2JackRootX + offset, OpCV2JackRootY), module, Dexter::OP_1_WAVE_2_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV3JackRootX + offset, OpCV2JackRootY), module, Dexter::OP_1_SHAPE_2_INPUT + 12 * op));
        addInput(createInput<PJ301MDarkSmall>(Vec(OpCV4JackRootX + offset, OpCV2JackRootY), module, Dexter::OP_1_LEVEL_2_INPUT + 12 * op));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Context Menu ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DexterPanelStyleItem : MenuItem {
    Dexter* module;
    DexterWidget* widget;
    int panelStyle;
    void onAction(const event::Action &e) override {
        module->panelStyle = panelStyle;
        widget->panelChanged = true;
    }
    void step() override {
        rightText = (module->panelStyle == panelStyle) ? "✔" : "";
        MenuItem::step();
    }
};

struct DexterOpSyncSourceItem : MenuItem {
    Dexter* module;
    FourVoiceOPCore::OpSyncSource opSyncSource = FourVoiceOPCore::PARENT_SYNC_SOURCE;
    void onAction(const event::Action &e) override {
        module->opSyncSource = opSyncSource;
    }
    void step() override {
        rightText = (module->opSyncSource == opSyncSource) ? "✔" : "";
        MenuItem::step();
    }
};

struct DexterOpOuputSource : MenuItem {
    Dexter* module;
    unsigned long indivBOutputs = 0;
    void onAction(const event::Action &e) override {
        module->indivBOutputs = indivBOutputs;
    }
    void step() override {
        rightText = (module->indivBOutputs == indivBOutputs) ? "✔" : "";
        MenuItem::step();
    }
};

void DexterWidget::appendContextMenu(Menu *menu) {
    Dexter *module = dynamic_cast<Dexter*>(this->module);
    assert(module);

    // Panel style
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Panel Style"));
    menu->addChild(construct<DexterPanelStyleItem>(&MenuItem::text, "Dark",
                                                   &DexterPanelStyleItem::module, module,
                                                   &DexterPanelStyleItem::widget, this,
                                                   &DexterPanelStyleItem::panelStyle, 0));
    menu->addChild(construct<DexterPanelStyleItem>(&MenuItem::text, "Light",
                                                   &DexterPanelStyleItem::module, module,
                                                   &DexterPanelStyleItem::widget, this,
                                                   &DexterPanelStyleItem::panelStyle, 1));

    // Operator Sync Source
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Operator Sync Source"));
    menu->addChild(construct<DexterOpSyncSourceItem>(&MenuItem::text, "Parent",
                                                     &DexterOpSyncSourceItem::module, module,
                                                     &DexterOpSyncSourceItem::opSyncSource, FourVoiceOPCore::PARENT_SYNC_SOURCE));
    menu->addChild(construct<DexterOpSyncSourceItem>(&MenuItem::text, "Neighbour",
                                                     &DexterOpSyncSourceItem::module, module,
                                                     &DexterOpSyncSourceItem::opSyncSource, FourVoiceOPCore::NEIGHBOUR_SYNC_SOURCE));

    // Indiv output source
    menu->addChild(construct<MenuLabel>());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Operator Outputs Source"));
    menu->addChild(construct<DexterOpOuputSource>(&MenuItem::text, "Voice A",
                                                  &DexterOpOuputSource::module, module,
                                                  &DexterOpOuputSource::indivBOutputs, 0));
    menu->addChild(construct<DexterOpOuputSource>(&MenuItem::text, "Voice B",
                                                  &DexterOpOuputSource::module, module,
                                                  &DexterOpOuputSource::indivBOutputs, 1));
}

void DexterWidget::step() {
    if(!module) {
        algo->value = 0;
        return;
    }

    Dexter* dexter = reinterpret_cast<Dexter*>(module);

    if (panelChanged) {
        panelChanged = false;
        darkPanel->visible = false;
        lightPanel->visible = false;
        NVGcolor newTextColour;

        if (dexter->panelStyle == 0) {
            darkPanel->visible = true;
            newTextColour = darkPanelTextColour;
            algo->style = 0;
        }
        else {
            lightPanel->visible = true;
            newTextColour = lightPanelTextColour;
            algo->style = 1;
        }

        for (int i = 0; i < kNumOperators; ++i) {
            for (int j = 0; j < 6; ++j) {
                mainText[i][j]->color = newTextColour;
            }
            multText[i]->color = newTextColour;

            waveTableTabTopLabel[i]->color = newTextColour;
            waveTableTabBottomLabel[i]->color = newTextColour;
            mod1And2TabTopLabel[i]->color = newTextColour;
            mod1And2TabBottomLabel[i]->color = newTextColour;
            mod3And4TabTopLabel[i]->color = newTextColour;
            mod3And4TabBottomLabel[i]->color = newTextColour;

            tableText[i]->color = newTextColour;

            tableLabel[i]->color = newTextColour;
            syncModeLabel[i]->color = newTextColour;
            shapeModeLabel[i]->color = newTextColour;

            mod1Label[i]->color = newTextColour;
            mod2Label[i]->color = newTextColour;
            mod3Label[i]->color = newTextColour;
            mod4Label[i]->color = newTextColour;
        }
    }

    algo->value = dexter->algo;
    chordText->text = chordNames[(int)dexter->chordKnob];
    octaveAText->text = octaveTextItems[(int)dexter->octaveAKnob];
    octaveBText->text = octaveTextItems[(int)dexter->octaveBKnob];

    for (int i = 0; i < kNumOperators; ++i) {
        for (int j = 0; j < 6; ++j) {
            mainText[i][j]->visible = !dexter->opSettingsMenu[i];
        }
        multText[i]->text = multiplesText[(int)dexter->opMultipleKnob[i]];
        multText[i]->visible = !dexter->opSettingsMenu[i];

        OpMultKnob[i]->visible = !dexter->opSettingsMenu[i];
        OpCoarseKnob[i]->visible = !dexter->opSettingsMenu[i];
        OpFineKnob[i]->visible = !dexter->opSettingsMenu[i];
        OpWaveKnob[i]->visible = !dexter->opSettingsMenu[i];
        OpShapeKnob[i]->visible = !dexter->opSettingsMenu[i];
        OpLevelKnob[i]->visible = !dexter->opSettingsMenu[i];

        opWaveButton[i]->visible = dexter->opSettingsMenu[i];
        opModAButton[i]->visible = dexter->opSettingsMenu[i];
        opModBButton[i]->visible = dexter->opSettingsMenu[i];
        opWaveButtonLight[i]->visible = dexter->opSettingsMenu[i];
        opModAButtonLight[i]->visible = dexter->opSettingsMenu[i];
        opModBButtonLight[i]->visible = dexter->opSettingsMenu[i];

        waveTableTabTopLabel[i]->visible = dexter->opSettingsMenu[i];
        waveTableTabBottomLabel[i]->visible = dexter->opSettingsMenu[i];
        mod1And2TabTopLabel[i]->visible = dexter->opSettingsMenu[i];
        mod1And2TabBottomLabel[i]->visible = dexter->opSettingsMenu[i];
        mod3And4TabTopLabel[i]->visible = dexter->opSettingsMenu[i];
        mod3And4TabBottomLabel[i]->visible = dexter->opSettingsMenu[i];

        tableText[i]->visible = dexter->opWaveMenuVis[i];
        tableText[i]->text = waveTableNames[(int)dexter->opWaveBankKnob[i]];
        waveBankKnob[i]->visible = dexter->opWaveMenuVis[i];

        syncModeChoice[i]->visible = dexter->opWaveMenuVis[i];
        shapeModeChoice[i]->visible = dexter->opWaveMenuVis[i];
        tableLabel[i]->visible = dexter->opWaveMenuVis[i];
        syncModeLabel[i]->visible = dexter->opWaveMenuVis[i];
        shapeModeLabel[i]->visible = dexter->opWaveMenuVis[i];

        mod1ModeChoice[i]->visible = dexter->opModAMenuVis[i];
        mod2ModeChoice[i]->visible = dexter->opModAMenuVis[i];
        mod3ModeChoice[i]->visible = dexter->opModBMenuVis[i];
        mod4ModeChoice[i]->visible = dexter->opModBMenuVis[i];
        mod1Label[i]->visible = dexter->opModAMenuVis[i];
        mod2Label[i]->visible = dexter->opModAMenuVis[i];
        mod3Label[i]->visible = dexter->opModBMenuVis[i];
        mod4Label[i]->visible = dexter->opModBMenuVis[i];
    }

    Widget::step();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Model *modelDexter = createModel<Dexter, DexterWidget>("Dexter");
