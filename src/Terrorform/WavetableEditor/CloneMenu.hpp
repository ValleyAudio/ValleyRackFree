#ifndef TFORM_EDITOR_CLONE_MENU_HPP
#define TFORM_EDITOR_CLONE_MENU_HPP
#include "MenuBase.hpp"
#include "QuestionMenu.hpp"
#include "TFormTextField.hpp"
#include "../TerrorformWaveBank.hpp"

struct TFormCloneMenuSourcePage : TFormMenu {
    TFormEditorWaveDisplay* waveDisplay;
    TFormEditorButton* backButton;
    TFormEditorButton* nextButton;
    PlainText* startWaveFieldLabel;
    PlainText* endWaveFieldLabel;
    TFormNumberField* startWaveField;
    TFormNumberField* endWaveField;
    NVGcolor waveLineColor;
    NVGcolor waveFillColor;
    float waveSliderPos;
    int selectedWave;

    std::vector<std::vector<float>> waveData;
    std::shared_ptr<int> sourceBank;
    TerrorformWaveBank bank;

    TFormCloneMenuSourcePage();
    void step() override;
    void draw(const DrawArgs& args) override;
    void onDragMove(const event::DragMove& e) override;

private:
    int numWaves = 0;
    size_t numSamplesToCopy = 0;

    void updateWaveDisplay();
};

struct TFormCloneMenuDestPage : TFormMenu {
    TFormEditorButton* backButton;
    TFormEditorButton* okayButton;
    PlainText* instructionText;
    PlainText* sourceDestText;
    PlainText* filledText;

    TFormEditorButtonStyleSet emptySlotStyle;
    TFormEditorButtonStyleSet filledSlotStyle;

    TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>* grid;
    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> sourceBank;
    std::shared_ptr<int> destBank;

    TFormCloneMenuDestPage();
    void step() override;
    void draw(const DrawArgs& args) override;
    void setSlotFilledFlag(int slot, bool isFilled);
};

struct TFormCloneMenu : TFormMenu {
    TFormCloneMenuSourcePage* sourcePage;
    TFormCloneMenuDestPage* destPage;
    TFormQuestionMenu *overwriteMenu;
    PlainText* cloneDoneText;
    int counter;
    std::string bankName;

    TerrorformWaveBank bank;

    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> sourceBank;
    std::shared_ptr<int> destBank;
    std::function<void(int, int, int, int)> onCloneBankCallback;

    TFormCloneMenu();
    void setSlotFilledFlag(int slot, bool isFilled);
};

#endif
