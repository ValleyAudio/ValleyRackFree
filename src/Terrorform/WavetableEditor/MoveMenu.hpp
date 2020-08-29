#ifndef TFORM_EDITOR_MOVE_MENU_HPP
#define TFORM_EDITOR_MOVE_MENU_HPP
#include "MenuBase.hpp"

struct TFormMoveMenuDestPage : TFormMenu {
    TFormEditorButton* backButton;
    TFormEditorButton* okayButton;
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    PlainText* instructionText;
    PlainText* sourceDestText;
    PlainText* filledText;
    PlainText* overwriteQuestion;

    TFormEditorButtonStyleSet emptySlotStyle;
    TFormEditorButtonStyleSet filledSlotStyle;

    TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>* grid;
    std::shared_ptr<std::vector<bool>> slotFilled;
    int sourceBank;
    int destBank;

    std::function<void(int, int)> onMoveBankCallback;

    TFormMoveMenuDestPage();
    void step() override;
    void setSlotFilledFlag(int slot, bool isFilled);
};

struct TFormMoveMenu : TFormMenu {
    TFormMoveMenuDestPage* destPage;
    PlainText* moveDoneText;
    int counter;

    std::shared_ptr<std::vector<bool>> slotFilled;
    std::shared_ptr<int> sourceBank;

    TFormMoveMenu();
    void step() override;
    void draw(const DrawArgs& args) override;
    void setSlotFilledFlag(int slot, bool isFilled);
    void addMoveBankCallback(const std::function<void(int, int)>& onMoveBankCallback);
};

#endif
