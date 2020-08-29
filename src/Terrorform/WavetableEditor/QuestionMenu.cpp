#include "QuestionMenu.hpp"

TFormQuestionMenu::TFormQuestionMenu() {
    box.size = Vec(238, 195);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 100, buttonWidth, buttonHeight);
    yesButton->onClick = [=]() {
        taskCallback();
        exit();
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
}
