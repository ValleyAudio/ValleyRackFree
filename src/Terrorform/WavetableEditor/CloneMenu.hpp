#ifndef TFORM_EDITOR_CLONE_MENU_HPP
#define TFORM_EDITOR_CLONE_MENU_HPP
#include "MenuRows.hpp"

struct TFormCloneMenuSourcePage : TFormMenuRow {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* backButton;
    TFormEditorButton* nextButton;
    TFormEditorNumberChoice* startWave;
    TFormEditorNumberChoice* endWave;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedWave;

    std::vector<std::vector<float>> waveData;
    int sourceBank;

    TFormCloneMenuSourcePage();
    void step() override;
    void draw(const DrawArgs& args) override;
    void onDragMove(const event::DragMove& e) override;
};

struct TFormCloneMenuDestPage : TFormMenuRow {
    TFormEditorButton* backButton;
    TFormEditorButton* okayButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    PlainText* instructionText;
    PlainText* sourceDestText;
    PlainText* filledText;
    PlainText* overwriteQuestion;

    TFormEditorButtonStyle emptySlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle sourceSlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle destSlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyle filledSlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>* grid;
    std::shared_ptr<std::vector<bool>> slotFilled;
    int sourceBank;
    int destBank;

    std::function<void(int, int)> onCloneBankCallback;

    TFormCloneMenuDestPage();
    void step() override;
    void setSlotFilledFlag(int slot, bool isFilled);
};

struct TFormCloneMenu : TFormMenuRow {
    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> sourceBank;

    TFormCloneMenuSourcePage* sourcePage;
    TFormCloneMenuDestPage* destPage;

    TFormCloneMenu();
    void step() override;
    void draw(const DrawArgs& args) override;
    void setSlotFilledFlag(int slot, bool isFilled);
    void addCloneBankCallback(const std::function<void(int, int)>& onCloneBankCallback);
};

#endif
