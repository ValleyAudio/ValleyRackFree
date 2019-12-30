#ifndef TFORM_EDITOR_CLEAR_MENU_HPP
#define TFORM_EDITOR_CLEAR_MENU_HPP
#include "MenuRows.hpp"

struct TFormClearMenu : TFormMenuRow {
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

struct TFormPurgeMenu : TFormMenuRow {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    PlainText* questionText;
    PlainText* clearedText;
    int counter;

    std::function<void(int)> onClearBankCallback;

    TFormPurgeMenu();
    void step() override;
};

#endif
