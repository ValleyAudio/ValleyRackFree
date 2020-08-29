#ifndef TFORM_EDITOR_VIEW_PANE_HPP
#define TFORM_EDITOR_VIEW_PANE_HPP
#include "MenuBase.hpp"
#include "TFormTextField.hpp"
#include "../TerrorformWaveBank.hpp"

struct TFormWaveViewPane : TFormMenu {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* backButton;
    PlainText* selectedBankText;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    std::shared_ptr<int> selectedBank;
    int selectedWave = 0;

    TerrorformWaveBank bank;
    TFormTextField* nameField;

    std::vector<std::vector<float>> waveData;

    std::function<void(int, TerrorformWaveBank&)> onGetBankCallback;
    std::function<void(int, const std::string&)> onRenameBankCallback;

    TFormWaveViewPane();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;
    void onShow(const event::Show& e) override;
};

#endif
