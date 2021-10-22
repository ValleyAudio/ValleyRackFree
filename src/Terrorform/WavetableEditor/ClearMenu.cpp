#include "ClearMenu.hpp"

TFormClearMenu::TFormClearMenu() {
    box.size = Vec(238, 195);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 100, buttonWidth, buttonHeight);
    yesButton->onClick = [=]() {
        onClearBankCallback(*selectedBank);
        noButton->visible = false;
        yesButton->visible = false;
        questionText->visible = false;
        clearedText->visible = true;
        counter = static_cast<int>(1.f / APP->window->getLastFrameDuration());
    };
    addChild(yesButton);

    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 100, buttonWidth, buttonHeight);
    noButton->onClick = [=]() {
        exit();
    };
    addChild(noButton);

    questionText = createWidget<PlainText>(Vec(119, 84));
    questionText->box.size.x = 238;
    questionText->size = 12;
    questionText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    addChild(questionText);

    clearedText = createWidget<PlainText>(Vec(119, 97.5));
    clearedText->box.size.x = 238;
    clearedText->size = 16;
    clearedText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    clearedText->vertAlignment = NVG_ALIGN_MIDDLE;
    clearedText->visible = false;
    addChild(clearedText);

    onView = [=]() {
        noButton->visible = true;
        yesButton->visible = true;
        questionText->visible = true;
        clearedText->visible = false;
        questionText->text = "Do you really want to delete bank " + std::to_string(*selectedBank + 1) + "?";
    };
}

void TFormClearMenu::step() {
    clearedText->text = "Bank " + std::to_string(*selectedBank + 1) + " cleared";
    if(counter == 0 && clearedText->visible) {
        clearedText->visible = false;
        exit();
    }
    else {
        --counter;
    }
}
