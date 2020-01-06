#ifndef TFORM_EDITOR_CLEAR_MENU_HPP
#define TFORM_EDITOR_CLEAR_MENU_HPP
#include "MenuBase.hpp"

struct TFormClearMenu : TFormMenu {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    PlainText* questionText;
    PlainText* clearedText;
    int counter;

    std::shared_ptr<int> selectedBank;

    std::function<void(int)> onClearBankCallback;

    TFormClearMenu();
    void step() override;
};

#endif
