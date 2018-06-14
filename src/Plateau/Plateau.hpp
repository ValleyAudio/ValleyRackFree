#ifndef DSJ_PLATEAU_HPP
#define DSJ_PLATEAU_HPP

#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "Dattorro.hpp"

struct Plateau : Module {

    enum InputIds {
        LEFT_INPUT,
        RIGHT_INPUT,

        DRY_CV_INPUT,
        WET_CV_INPUT,
        PRE_DELAY_CV_INPUT,
        INPUT_LOW_DAMP_CV_INPUT,
        INPUT_HIGH_DAMP_CV_INPUT,

        SIZE_CV_INPUT,
        DIFFUSION_CV_INPUT,
        DECAY_CV_INPUT,
        REVERB_HIGH_DAMP_CV_INPUT,
        REVERB_LOW_DAMP_CV_INPUT,

        MOD_SPEED_CV_INPUT,
        MOD_SHAPE_CV_INPUT,
        MOD_DEPTH_CV_INPUT,

        HOLD_CV_INPUT,
        CLEAR_CV_INPUT,

        NUM_INPUTS
    };

    enum OutputIds {
        LEFT_OUTPUT,
        RIGHT_OUTPUT,
        NUM_OUTPUTS
    };

    enum ParamIds {
        DRY_PARAM,
        WET_PARAM,
        PRE_DELAY_PARAM,
        INPUT_LOW_DAMP_PARAM,
        INPUT_HIGH_DAMP_PARAM,

        SIZE_PARAM,
        DIFFUSION_PARAM,
        DECAY_PARAM,
        REVERB_HIGH_DAMP_PARAM,
        REVERB_LOW_DAMP_PARAM,

        MOD_SPEED_PARAM,
        MOD_SHAPE_PARAM,
        MOD_DEPTH_PARAM,

        FREEZE_PARAM,
        CLEAR_PARAM,
        FREEZE_TOGGLE_PARAM,
        CLEAR_TOGGLE_PARAM,

        DRY_CV_PARAM,
        WET_CV_PARAM,
        INPUT_LOW_DAMP_CV_PARAM,
        INPUT_HIGH_DAMP_CV_PARAM,

        SIZE_CV_PARAM,
        DIFFUSION_CV_PARAM,
        DECAY_CV_PARAM,
        REVERB_HIGH_DAMP_CV_PARAM,
        REVERB_LOW_DAMP_CV_PARAM,

        MOD_SPEED_CV_PARAM,
        MOD_SHAPE_CV_PARAM,
        MOD_DEPTH_CV_PARAM,

        NUM_PARAMS
    };

    enum LightIds {
        FREEZE_LIGHT,
        CLEAR_LIGHT,
        NUM_LIGHTS
    };

    Plateau();
    void step() override;
    void onSampleRateChange() override;
    json_t *toJson() override;
    void fromJson(json_t *rootJ) override;

    // Control positions
    Vec dryPos = Vec(53.1, 56.1);
    Vec wetPos = Vec(95.1, 56.1);
    Vec preDelayPos = Vec(80.106, 26.106);
    Vec inputLowDampPos = Vec(53.1, 113.1);
    Vec inputHighDampPos = Vec(95.1, 113.1);

    Vec sizePos = Vec(32.1, 170.1);
    Vec diffPos = Vec(74.1, 181.1);
    Vec decayPos = Vec(116.1, 170.1);
    Vec reverbLowDampPos = Vec(53.1, 238.1);
    Vec reverbHighDampPos = Vec(95.1, 238.1);

    Vec modRatePos = Vec(32.1, 296.1);
    Vec modShapePos = Vec(74.1, 310.1);
    Vec modDepthPos = Vec(116.1, 296.1);

    Vec holdButtonPos = Vec(7.875, 244.85);
    Vec clearButtonPos = Vec(157.875, 244.85);

    // Attenuverter positions
    Vec dryAttenPos = Vec(29.53, 80.1);
    Vec wetAttenPos = Vec(131.01, 80.1);
    Vec inputLowDampAttenPos = Vec(29.53, 126.1);
    Vec inputHighDampAttenPos = Vec(131.01, 126.1);

    Vec sizeAttenPos = Vec(5.1, 164.1);
    Vec diffAttenPos = Vec(65.11, 158.51);
    Vec decayAttenPos = Vec(155.1, 164.1);
    Vec reverbLowDampAttenPos = Vec(29.1, 229.37);
    Vec reverbHighDampAttenPos = Vec(131.1, 229.37);

    Vec modRateAttenPos = Vec(5.1, 306.1);
    Vec modShapeAttenPos = Vec(65.1, 286.2);
    Vec modDepthAttenPos = Vec(155.1, 306.1);

    // Jack positions
    Vec leftInputPos = Vec(4.395, 37.39);
    Vec rightInputPos = Vec(31.395, 37.39);
    Vec leftOutputPos = Vec(127.395, 37.39);
    Vec rightOutputPos = Vec(154.395, 37.39);

    Vec dryCVPos = Vec(4.395, 94.41);
    Vec wetCVPos = Vec(154.395, 94.41);
    Vec inputLowDampCVPos = Vec(4.395, 130.395);
    Vec inputHighDampCVPos = Vec(154.395, 130.395);

    Vec sizeCVPos = Vec(4.395, 190.395);
    Vec diffCVPos = Vec(94.395, 157.794);
    Vec decayCVPos = Vec(154.395, 190.395);
    Vec reverbLowDampCVPos = Vec(4.395, 217.383);
    Vec reverbHighDampCVPos = Vec(154.395, 217.383);

    Vec modRateCVPos = Vec(4.395, 331.395);
    Vec modShapeCVPos = Vec(94.395, 285.51);
    Vec modDepthCVPos = Vec(154.395, 331.395);

    Vec holdCVPos = Vec(4.395, 265.42);
    Vec clearCVPos = Vec(154.395, 265.42);

    // CV scaling
    const float dryMin = 0.f;
    const float dryMax = 1.f;
    const float wetMin = 0.f;
    const float wetMax = 1.f;
    const float sizeMin = 0.01f;
    const float sizeMax = 2.5f;
    const float diffMin = 0.f;
    const float diffMax = 1.f;
    const float decayMin = 0.1f;
    const float decayMax = 0.9999f;
    const float reverbLowDampMin = 0.f;
    const float reverbLowDampMax = 10.f;
    const float reverbHighDampMin = 0.f;
    const float reverbHIghDampMax = 10.f;
    const float modSpeedMin = 0.f;
    const float modSpeedMax = 1.f;
    const float modDepthMin = 1.f;
    const float modDepthMax = 16.f;
    const float modShapeMin = 0.001f;
    const float modShapeMax = 0.999f;

    float size = 1.f;
    float diffusion;
    float decay = 0.f;
    float inputDampLow = 0.f;
    float inputDampHigh = 10.f;
    float reverbDampLow = 0.f;
    float reverbDampHigh = 10.f;
    float modSpeed = 0.1f;
    float modShape = 0.5f;
    float modDepth = 0.0f;
    bool freezeButtonState = false;
    int freeze = 0;
    bool frozen = false;
    bool clearButtonState = false;
    int clear = 0;
    bool cleared = false;
    Dattorro reverb;

    int panelStyle = 0;
};

struct PlateauWidget : ModuleWidget {
    PlateauWidget(Plateau *module);
};

#endif
