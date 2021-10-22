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
    TFormEditorChoice* cycleSizeMenu;

    PlainText* nameFieldLabel;
    PlainText* startWaveFieldLabel;
    PlainText* endWaveFieldLabel;
    PlainText* cycleSizeLabel;
    TFormTextField* nameField;
    TFormNumberField* startWaveField;
    TFormNumberField* endWaveField;

    std::shared_ptr<std::vector<float>> detectedWaves;
    std::shared_ptr<int> selectedBank;
    int maxWaves;
    int selectedWave;

    int downSampleRatio;
    int maxDownSampleRatio;

    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    bool waveDisplayNeedsUpdate;

    std::function<void(int, int)> ingestNewTable;
    std::function<void(int, int, int, int, const std::string&)> onIngestTableCallback;

    TFormLoadMenu();
    void draw(const DrawArgs& args) override;
    void step() override;
    void onDragMove(const event::DragMove& e) override;

private:
    int numWaves = 0;
    int waveLength = TFORM_MAX_WAVELENGTH;
    int startOffset = 0;
    size_t numSamplesToCopy = TFORM_MAX_WAVELENGTH;

    void calcWaveLength();
    void selectWaves();
    void updateWaveDisplay();
};

#endif
