#ifndef TFORM_EDITOR_MAIN_MENU_HPP
#define TFORM_EDITOR_MAIN_MENU_HPP
#include "MenuBase.hpp"
#include "TFormTextField.hpp"
#include "../TerrorformWaveBank.hpp"

struct TFormEditMainMenu : TFormMenu {
    TFormEditorButton* loadButton;
    TFormEditorButton* viewButton;
    TFormEditorButton* clearButton;
    TFormEditorButton* cloneButton;
    TFormEditorButton* moveButton;
    TFormEditorButton* backButton;
    TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>* grid;

    TFormEditorButtonStyleSet emptySlotStyle;
    TFormEditorButtonStyleSet filledSlotStyle;
    TerrorformWaveBank bank;
    TFormTextField* nameField;
    
    std::vector<bool> slotFilled;
    std::shared_ptr<int> selectedBank;
    bool selectedBankIsFilled;

    std::function<void(int, TerrorformWaveBank&)> onGetBankCallback;
    std::function<void(int, const std::string&)> onRenameBankCallback;

    TFormEditMainMenu();
    void step() override;
    void draw(const DrawArgs& args) override;
    void setSlotFilledFlag(int slot, bool isFilled);
    void setBankData(TerrorformWaveBank& bank);
    void onShow(const event::Show& e) override;
};

#endif
