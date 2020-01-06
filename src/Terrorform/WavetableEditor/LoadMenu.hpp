#ifndef TFORM_EDITOR_LOAD_MENU_HPP
#define TFORM_EDITOR_LOAD_MENU_HPP
#include "MenuBase.hpp"

struct TFormLoadMenu : TFormMenu {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* cancelButton;
    TFormEditorButton* confirmButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    TFormEditorNumberChoice* startWaveChoice;
    TFormEditorNumberChoice* endWaveChoice;
    std::shared_ptr<std::vector<std::vector<float>>> detectedWaves;
    std::shared_ptr<int> selectedBank;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedWave;

    std::function<void(int, int)> ingestNewTable;
    std::function<void(int, int, int)> onIngestTableCallback;

    TFormLoadMenu();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
};

#endif
