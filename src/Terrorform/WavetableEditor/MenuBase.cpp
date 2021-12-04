#include "MenuBase.hpp"

TFormMenu::TFormMenu() {
}

void TFormMenu::view() {
    visible = true;
    if(onView) {
        onView();
    }
}

void TFormMenu::hide() {
    visible = false;
    if(onHide) {
        onHide();
    }
}

void TFormMenu::exit() {
    visible = false;
    if(onExit) {
        onExit();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
