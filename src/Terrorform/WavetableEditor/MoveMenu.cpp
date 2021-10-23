#include "MoveMenu.hpp"

TFormMoveMenuDestPage::TFormMoveMenuDestPage() {
    destBank = 0;
    box.size = Vec(238, 195);

    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    addChild(backButton);

    okayButton = createNewMenuButton("Okay", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    okayButton->onClick = [=]() {
        if ((*slotFilled)[destBank]) {
            instructionText->visible = false;
            sourceDestText->visible = false;
            filledText->visible = false;
            overwriteQuestion->text = "Overwrite bank " + std::to_string(destBank) + "?";
            overwriteQuestion->visible = true;
            backButton->visible = false;
            okayButton->visible = false;
            yesButton->visible = true;
            noButton->visible = true;
        }
        else {
            onMoveBankCallback(sourceBank, destBank);
            exit();
        }
    };
    addChild(okayButton);

    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 21, buttonWidth, buttonHeight);
    yesButton->onClick = [=]() {
        onMoveBankCallback(sourceBank, destBank);
        exit();
    };
    yesButton->visible = false;
    addChild(yesButton);

    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 21, buttonWidth, buttonHeight);
    noButton->visible = false;
    noButton->onClick = [=]() {
        instructionText->visible = true;
        sourceDestText->visible = true;
        filledText->visible = true;
        overwriteQuestion->visible = false;
        backButton->visible = true;
        okayButton->visible = true;
        yesButton->visible = false;
        noButton->visible = false;
    };
    addChild(noButton);

    filledSlotStyle.idleColors =           TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                                   0x9F, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);
    filledSlotStyle.hoverColors =          TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                                   0x9F, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);
    filledSlotStyle.highlightIdleColors =  TFormEditorButtonColors(0x00, 0x00, 0x00, 0xFF,
                                                                   0xFF, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);
    filledSlotStyle.highlightHoverColors = TFormEditorButtonColors(0x00, 0x00, 0x00, 0xFF,
                                                                   0xFF, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);

    Vec offset = Vec(0, 40);
    grid = createWidget<TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>>(box.pos.plus(offset));
    grid->box.size.x = box.size.x;
    grid->box.size.y = box.size.y - offset.y;

    int id = 0;
    for (int row = 0; row < TFORM_EDITOR_ROWS; ++row) {
        for (int col = 0; col < TFORM_EDITOR_COLS; ++col) {
            grid->slotButton[row][col]->onClick = [=]() {
                destBank = id;
            };
            grid->slotButton[row][col]->text = std::to_string(id + 1);
            grid->slotButton[row][col]->applyStyle(emptySlotStyle);
            ++id;
        }
    }
    addChild(grid);

    instructionText = createWidget<PlainText>(Vec(5, 5));
    instructionText->box.size.x = 233;
    instructionText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    instructionText->size = 12;
    instructionText->horzAlignment = NVG_ALIGN_LEFT;
    instructionText->text = "Select destination bank";
    addChild(instructionText);

    sourceDestText = createWidget<PlainText>(Vec(5, 21));
    sourceDestText->box.size.x = 233;
    sourceDestText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    sourceDestText->size = 12;
    sourceDestText->horzAlignment = NVG_ALIGN_LEFT;
    addChild(sourceDestText);

    filledText = createWidget<PlainText>(Vec(5, 21));
    filledText->box.size.x = 233;
    filledText->color = nvgRGB(0xFF, 0x00, 0x00);
    filledText->size = 12;
    filledText->horzAlignment = NVG_ALIGN_LEFT;
    filledText->text = "                   (Filled)";
    filledText->hide();
    addChild(filledText);

    overwriteQuestion = createWidget<PlainText>(Vec(119, 5));
    overwriteQuestion->box.size.x = 238;
    overwriteQuestion->color = nvgRGB(0xEF, 0xEF, 0xEF);
    overwriteQuestion->size = 12;
    overwriteQuestion->visible = false;
    addChild(overwriteQuestion);

    onView = [=] {
        yesButton->visible = false;
        noButton->visible = false;
        filledText->visible = false;
        overwriteQuestion->visible = false;

        for (int row = 0; row < TFORM_EDITOR_ROWS; ++row) {
            for (int col = 0; col < TFORM_EDITOR_COLS; ++col) {
                grid->slotButton[row][col]->respondToMouse = true;
            }
        }

        int row = sourceBank / TFORM_EDITOR_ROWS;
        int col = sourceBank % TFORM_EDITOR_COLS;
        grid->slotButton[row][col]->respondToMouse = false;
        grid->slotButton[row][col]->applyStyle(filledSlotStyle);
        grid->slotButton[row][col]->setHighlight(true);

        destBank = (sourceBank + 1) % TFORM_EDITOR_SLOTS;
    };
}

void TFormMoveMenuDestPage::step() {
    int row = 0;
    int col = 0;
    for(auto i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
        row = i / TFORM_EDITOR_ROWS;
        col = i % TFORM_EDITOR_COLS;
        grid->slotButton[row][col]->setHighlight(i == sourceBank || i == destBank);

    }
    if ((*slotFilled)[destBank]) {
        filledText->show();
    }
    else {
        filledText->hide();
    }
    sourceDestText->text = "Bank " + std::to_string(sourceBank + 1) + " -> Bank " + std::to_string(destBank + 1);
    Widget::step();
}

void TFormMoveMenuDestPage::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        int row = slot / TFORM_EDITOR_ROWS;
        int col = slot % TFORM_EDITOR_COLS;
        if ((*slotFilled)[slot] != isFilled) {
            (*slotFilled)[slot] = isFilled;
            grid->slotButton[row][col]->setFilled(isFilled);
            grid->slotButton[row][col]->applyStyle(isFilled ? filledSlotStyle : emptySlotStyle);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormMoveMenu::TFormMoveMenu() {
    box.size = Vec(238, 195);

    slotFilled = std::make_shared<std::vector<bool>>(TFORM_EDITOR_SLOTS, false);
    moveDoneText = createWidget<PlainText>(Vec(119, 97.5));
    moveDoneText->box.size.x = 238;
    moveDoneText->size = 16;
    moveDoneText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    moveDoneText->vertAlignment = NVG_ALIGN_MIDDLE;
    moveDoneText->hide();
    addChild(moveDoneText);

    destPage = createWidget<TFormMoveMenuDestPage>(Vec(0,0));
    destPage->slotFilled = slotFilled;
    destPage->visible = false;
    destPage->backButton->onClick = [=]() {
        exit();
    };
    destPage->onExit = [=]() {
        float frameRate = 1.f / APP->window->getLastFrameDuration();
        counter = static_cast<int>(frameRate);
        moveDoneText->text = "Bank " + std::to_string(*sourceBank + 1) + " moved";
        moveDoneText->show();
    };
    addChild(destPage);

    onView = [=]() {
        destPage->view();
    };
}

void TFormMoveMenu::step() {
    destPage->sourceBank = *sourceBank;

    if (counter == 0 && moveDoneText->visible) {
        moveDoneText->visible = false;
        exit();
    }
    else {
        --counter;
    }

    Widget::step();
}

void TFormMoveMenu::draw(const DrawArgs& args) {
    if (destPage->visible) {
        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, 0, box.pos.y + 40);
        nvgLineTo(args.vg, box.size.x, box.pos.y + 40);
        nvgStrokeWidth(args.vg, 1.0);
        nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
        nvgStroke(args.vg);
    }

    Widget::draw(args);
}

void TFormMoveMenu::setSlotFilledFlag(int slot, bool isFilled) {
    destPage->setSlotFilledFlag(slot, isFilled);
}

void TFormMoveMenu::addMoveBankCallback(const std::function<void(int, int)>& onMoveBankCallback) {
    destPage->onMoveBankCallback = onMoveBankCallback;
}
