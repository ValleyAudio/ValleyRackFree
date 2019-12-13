#pragma once
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"

struct TFormEditorWaveDisplay : TransparentWidget {
    NVGcolor offColor;
    NVGcolor onColor;
    NVGcolor bgColor;
    int numWaves;
    float waveSliderPos;
    float normSliderPos;
    int selectedWave;

    float waveData[TFORM_MAX_NUM_WAVES][TFORM_MAX_WAVELENGTH];

    TFormEditorWaveDisplay();
    void draw(const DrawArgs& args) override;
    void moveSliderPos(float sliderDelta);
};
