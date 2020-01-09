#ifndef TFORM_EDITOR_MAIN_MENU_HPP
#define TFORM_EDITOR_MAIN_MENU_HPP
#include "MenuBase.hpp"

struct TFormEditMainMenu : TFormMenu {
    TFormEditorButton* loadButton;
    TFormEditorButton* viewButton;
    TFormEditorButton* clearButton;
    TFormEditorButton* cloneButton;
    TFormEditorButton* moveButton;
    TFormEditorButton* backButton;
    TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>* grid;

    // TFormEditorButtonStyle emptySlotButtonStyles[NUM_BUTTON_MODES];
    // TFormEditorButtonStyle filledSlotButtonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonStyleSet emptySlotStyle;
    TFormEditorButtonStyleSet filledSlotStyle;
    
    std::vector<bool> slotFilled;
    std::shared_ptr<int> selectedBank;
    bool selectedBankIsFilled;

    TFormEditMainMenu();
    void step() override;
    void draw(const DrawArgs& args) override;
    void setSlotFilledFlag(int slot, bool isFilled);
};

#endif
