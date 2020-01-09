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

    // emptySlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // emptySlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0x2F, 0xAF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // emptySlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // emptySlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    //
    // sourceSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // sourceSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0x2F, 0xAF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // sourceSlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // sourceSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    //
    // destSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // destSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0x2F, 0xAF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // destSlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // destSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    //
    // filledSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x9F, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // filledSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0xBF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // filledSlotButtonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    // filledSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotStyle.idleColors =           TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                                   0x9F, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);
    filledSlotStyle.hoverColors =          TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                                   0x9F, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);
    filledSlotStyle.highlightIdleColors =  TFormEditorButtonColors(0x00, 0x00, 0x00, 0xFF,
                                                                   0xFF, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);
    filledSlotStyle.highlightHoverColors = TFormEditorButtonColors(0x00, 0x00, 0x00, 0x00,
                                                                   0xFF, 0x00, 0x00, 0x00,
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

        int row = sourceBank;
        int col = sourceBank;
        grid->slotButton[row][col]->respondToMouse = false;
        grid->slotButton[row][col]->applyStyle(filledSlotStyle);
        grid->slotButton[row][col]->setHighlight(true);
    };
}

void TFormMoveMenuDestPage::step() {
    int row = 0;
    int col = 0;
    for(auto i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
        row = i / TFORM_EDITOR_ROWS;
        col = i % TFORM_EDITOR_COLS;
        grid->slotButton[row][col]->setHighlight(i == destBank);
        // grid->slotButton[row][col]->highlight = i == sourceBank || i == destBank;
        // grid->slotButton[row][col]->respondToMouse = i != sourceBank;
    }
    sourceDestText->text = "Bank " + std::to_string(sourceBank + 1) + " -> Bank " + std::to_string(destBank + 1);
    Widget::step();
}

void TFormMoveMenuDestPage::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        int row = slot / TFORM_EDITOR_ROWS;
        int col = slot % TFORM_EDITOR_COLS;
        (*slotFilled)[slot] = isFilled;
        grid->slotButton[row][col]->applyStyle(isFilled ? filledSlotStyle : emptySlotStyle);
        // for(int m = 0; m < NUM_BUTTON_MODES; ++m) {
        //     if(isFilled) {
        //         grid->slotButton[row][col]->buttonStyles[m] = filledSlotButtonStyles[m];
        //     }
        //     else {
        //         grid->slotButton[row][col]->buttonStyles[m] = emptySlotButtonStyles[m];
        //     }
        // }
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
        counter = 35;
        moveDoneText->text = "Bank " + std::to_string(*sourceBank + 1) + " moved";
        moveDoneText->show();
    };
    addChild(destPage);

    onView = [=]() {
        destPage->show();
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
