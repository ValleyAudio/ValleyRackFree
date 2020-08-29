#ifndef TFORM_EDITOR_LOAD_MENU_HPP
#define TFORM_EDITOR_LOAD_MENU_HPP
#include "TFormTextField.hpp"
#include "MenuBase.hpp"

struct TFormLoadMenu : TFormMenu {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* cancelButton;
    TFormEditorButton* confirmButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    std::shared_ptr<std::vector<std::vector<float>>> detectedWaves;
    std::shared_ptr<int> selectedBank;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedWave;

    PlainText* nameFieldLabel;
    PlainText* startWaveFieldLabel;
    PlainText* endWaveFieldLabel;
    TFormTextField* nameField;
    TFormNumberField* startWaveField;
    TFormNumberField* endWaveField;

    std::function<void(int, int)> ingestNewTable;
    std::function<void(int, int, int, const std::string&)> onIngestTableCallback;

    TFormLoadMenu();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
};

#endif
