#ifndef TFORM_EDITOR_VIEW_PANE_HPP
#define TFORM_EDITOR_VIEW_PANE_HPP
#include "MenuBase.hpp"

struct TFormWaveViewPane : TFormMenu {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* backButton;
    PlainText* selectedBankText;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    std::shared_ptr<int> selectedBank;
    int selectedWave = 0;

    std::vector<std::vector<float>> waveData;

    TFormWaveViewPane();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
};

#endif
