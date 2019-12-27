#pragma once
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"

struct TFormEditorWaveDisplay : TransparentWidget {
    NVGcolor offColor;
    NVGcolor onColor;
    NVGcolor bgColor;
    float dX, dY, dZ, mX, mY;
    Vec pW, triPos;

    int numWaves;
    float waveSliderPos;
    float normSliderPos;
    int selectedWave;

    float waveData[TFORM_MAX_NUM_WAVES][TFORM_MAX_WAVELENGTH];

    TFormEditorWaveDisplay();
    void draw(const DrawArgs& args) override;
    void drawWaveLine(int w, const NVGcolor& color, const DrawArgs& args);
    void drawWaveFilled(int w, const NVGcolor& color, const DrawArgs& args);
    void drawWaveBox(int w, const DrawArgs& args);
    Vec dimetricProject(float x, float y, float z) const;
    Vec scalePoint(const Vec& p) const;
    void moveSliderPos(float sliderDelta);
};
