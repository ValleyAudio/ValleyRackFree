#ifndef TFORM_EDITOR_CONFIRMATION_MENU_HPP
#define TFORM_EDITOR_CONFIRMATION_MENU_HPP
#include "MenuBase.hpp"

struct TFormQuestionMenu : TFormMenu {
    TFormEditorButton* yesButton;
    TFormEditorButton* noButton;
    PlainText* questionText;
    int counter;

    std::function<void()> taskCallback;

    TFormQuestionMenu();
};

#endif
