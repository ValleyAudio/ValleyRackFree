#pragma once
#include "../Valley.hpp"
#include "../ValleyComponents.hpp"

struct TFormEditorWaveDisplay : OpaqueWidget {
    NVGcolor offColor;
    NVGcolor onColor;
    NVGcolor bgColor;
    int selectedWave;
    float waveData[TFORM_MAX_NUM_WAVES][TFORM_MAX_WAVELENGTH];

    TFormEditorWaveDisplay();
    void draw(const DrawArgs& args) override;
};
