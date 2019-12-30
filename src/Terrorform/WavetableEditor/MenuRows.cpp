#include "MenuRows.hpp"

TFormMenuRow::TFormMenuRow() {
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormMenuRow::view() {
    visible = true;
    if(onView) {
        onView();
    }
}

void TFormMenuRow::hide() {
    visible = false;
    if(onHide) {
        onHide();
    }
}

void TFormMenuRow::exit() {
    visible = false;
    if(onExit) {
        onExit();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
