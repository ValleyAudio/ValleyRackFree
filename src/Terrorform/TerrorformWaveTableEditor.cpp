#include "TerrorformWaveTableEditor.hpp"

TFormEditorButtonStyle::TFormEditorButtonStyle() {
    textColor = nvgRGB(0x00, 0x00, 0x00);
    bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
}

TFormEditorButtonStyle::TFormEditorButtonStyle(const NVGcolor& newTextColor,
                                               const NVGcolor& newBgColor,
                                               const NVGcolor& newBorderColor) {
    textColor = newTextColor;
    bgColor = newBgColor;
    borderColor = newBorderColor;
}

TFormEditorButton::TFormEditorButton() {
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    mode = IDLE_MODE;
    modeOffset = IDLE_MODE;
    buttonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF),
                                                     nvgRGB(0x17, 0x17, 0x17),
                                                     nvgRGB(0x7F, 0x7F, 0x7F));
    buttonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                      nvgRGB(0x32, 0x32, 0x7F),
                                                      nvgRGB(0x7F, 0x7F, 0x7F));
    buttonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00),
                                                          nvgRGB(0xDF, 0x00, 0x00),
                                                          nvgRGB(0x7F, 0x7F, 0x7F));
    buttonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                nvgRGB(0xDF, 0x00, 0x00),
                                                                nvgRGB(0x7F, 0x7F, 0x7F));
    fontSize = 12;
    enabled = true;
}

void TFormEditorButton::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, buttonStyles[mode + modeOffset].bgColor);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgFill(args.vg);

    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, buttonStyles[mode + modeOffset].borderColor);
    nvgStrokeWidth(args.vg, 1.0);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgStroke(args.vg);

    if (font->handle >= 0) {
        nvgFillColor(args.vg, buttonStyles[mode + modeOffset].textColor);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, fontSize);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(args.vg, box.size.x / 2, box.size.y / 2, text.c_str(), NULL);
    }
}

void TFormEditorButton::onEnter(const event::Enter& e) {
    mode = IDLE_MODE;
    if(enabled) {
        mode = HOVER_MODE;
    }
}

void TFormEditorButton::onLeave(const event::Leave& e) {
    mode = IDLE_MODE;
}

void TFormEditorButton::onDragEnd(const event::DragEnd& e) {
    if(onClick != NULL && enabled) {
        onClick();
    }
}

void TFormEditorButton::setHighlight(bool highlight) {
    modeOffset = IDLE_MODE;
    if(highlight) {
        modeOffset = HIGHLIGHT_MODE;
    }
}

TFormEditorButton* createNewMenuButton(const std::string& text,
                                       const std::function<void()>& onClickCallback,
                                       int x, int y, int width, int height) {
    TFormEditorButton* newButton = createWidget<TFormEditorButton>(Vec(x, y));
    newButton->text = text;
    newButton->box.size.x = width;
    newButton->box.size.y = height;
    newButton->onClick = onClickCallback;
    return newButton;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorGrid::TFormEditorGrid() {
    color = nvgRGB(0xAF, 0xAF, 0xAF);
}

void TFormEditorGrid::draw(const DrawArgs& args) {
    auto drawVSeparator = [=](float x, float height) {
        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, x, 0);
        nvgLineTo(args.vg, x, height);
        nvgStrokeWidth(args.vg, 1.0);
        nvgStrokeColor(args.vg, color);
        nvgStroke(args.vg);
    };

    auto drawHSeparator = [=](float y) {
        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, 0, y);
        nvgLineTo(args.vg, box.size.x, y);
        nvgStrokeWidth(args.vg, 1.0);
        nvgStrokeColor(args.vg, color);
        nvgStroke(args.vg);
    };

    float gridHeight = box.size.y;
    for(int i = 1; i < TRRFORM_EDITOR_COLS; ++i) {
        drawVSeparator(i * (box.size.x / TRRFORM_EDITOR_COLS), gridHeight);
    }

    for(int i = 0; i < TRRFORM_EDITOR_ROWS; ++i) {
        drawHSeparator(i * gridHeight / TRRFORM_EDITOR_ROWS);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorNumberItem::TFormEditorNumberItem(unsigned long itemNumber) {
    _itemNumber = itemNumber;
}

void TFormEditorNumberItem::onAction(const event::Action &e) {
    *choice = _itemNumber;
}

void TFormEditorNumberItem::step() {
    if(choice) {
        if(*choice == _itemNumber) {
            rightText = (*choice == _itemNumber) ? "✔" : "";
        }
    }
    MenuItem::step();
}

TFormEditorNumberChoice::TFormEditorNumberChoice() {
    range = 0;
    choice = std::make_shared<unsigned long>(0);
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    boxOnEnterColor = nvgRGB(0xEF, 0xEF, 0xEF);
    boxOnLeaveColor = nvgRGB(0xAF, 0xAF, 0xAF);
    boxColor = boxOnLeaveColor;

    arrowOnEnterColor = nvgRGB(0xFF, 0x00, 0x00);
    arrowOnLeaveColor = nvgRGB(0xAF, 0xAF, 0xAF);
    arrowColor = arrowOnLeaveColor;

    textColor = nvgRGB(0xEF, 0xEF, 0xEF);
}

void TFormEditorNumberChoice::onAction(const event::Action &e) {
    Menu* menu = createMenu();
    menu->box.pos = getAbsoluteOffset(Vec(0, box.size.y)).round();
	menu->box.size.x = box.size.x;

    for(unsigned long i = 0; i < range; ++i) {
        TFormEditorNumberItem *item = new TFormEditorNumberItem(i);
        item->_itemNumber = i;
        item->choice = choice;
        item->text = std::to_string(i + 1);
        menu->addChild(item);
    }
}

void TFormEditorNumberChoice::onEnter(const event::Enter &e) {
    boxColor = boxOnEnterColor;
    arrowColor = arrowOnEnterColor;
}

void TFormEditorNumberChoice::onLeave(const event::Leave &e) {
    boxColor = boxOnLeaveColor;
    arrowColor = arrowOnLeaveColor;
}

void TFormEditorNumberChoice::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, boxColor);
    nvgStrokeWidth(args.vg, 1.f);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgStroke(args.vg);
    nvgClosePath(args.vg);

    nvgFontSize(args.vg, 12);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0);
    Vec textPos = Vec(3, box.size.y / 2).round();
    nvgFillColor(args.vg, textColor);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    if(choice) {
        nvgText(args.vg, textPos.x, textPos.y, std::to_string(*choice + 1).c_str(), NULL);
    }
    else {
        nvgText(args.vg, textPos.x, textPos.y, "--", NULL);
    }

    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, arrowColor);
    nvgMoveTo(args.vg, box.size.x - 2, 4);
    nvgLineTo(args.vg, box.size.x - 6, box.size.y - 4);
    nvgLineTo(args.vg, box.size.x - 10, 4);
    nvgLineTo(args.vg, box.size.x - 2, 4);
    nvgFill(args.vg);
    nvgClosePath(args.vg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Main menu
////////////////////////////////////////////////////////////////////////////////////////////////////

TFormBankEditMainRow::TFormBankEditMainRow() {
    box.size = Vec(238, 40);
    loadButton = createNewMenuButton("Load", NULL, buttonOffset, 21, buttonWidth, buttonHeight);
    addChild(loadButton);
    copyButton = createNewMenuButton("Copy", NULL, buttonWidth + buttonOffset * 2, 21, buttonWidth, buttonHeight);
    addChild(copyButton);
    pasteButton = createNewMenuButton("Paste", NULL, buttonWidth * 2 + buttonOffset * 3, 21, buttonWidth, buttonHeight);
    addChild(pasteButton);
    clearButton = createNewMenuButton("Clear", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    addChild(clearButton);
    purgeButton = createNewMenuButton("Purge", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(purgeButton);
    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    addChild(backButton);
    selectedBank = 0;
    selectedBankIsFilled = false;

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormBankEditMainRow::draw(const DrawArgs& args) {
    std::string strSelectedBank = "Bank: " + std::to_string(selectedBank + 1);
    std::string strFilled = "Filled";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strSelectedBank.c_str(), NULL);

    if(selectedBankIsFilled) {
        nvgFillColor(args.vg, nvgRGB(0xFF, 0x00, 0x00));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);
        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(args.vg, 55, 5, strFilled.c_str(), NULL);
    }

    Widget::draw(args);
}

TFormBankEditLoadRow::TFormBankEditLoadRow() {
    box.size = Vec(238, 40);
    startWave = createWidget<TFormEditorNumberChoice>(Vec(45, 20));
    startWave->range = 64;
    startWave->box.size.x = 28;
    startWave->box.size.y = 15;
    addChild(startWave);

    endWave = createWidget<TFormEditorNumberChoice>(Vec(108, 20));
    endWave->range = 64;
    endWave->box.size.x = 28;
    endWave->box.size.y = 15;
    addChild(endWave);

    auto triggerIngest = [=]() {
        ingestNewTable(*startWave->choice, *endWave->choice);
    };

    cancelButton = createNewMenuButton("Cancel", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    addChild(cancelButton);
    confirmButton = createNewMenuButton("Okay", triggerIngest, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(confirmButton);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormBankEditLoadRow::draw(const DrawArgs& args) {
    std::string strDetectedWaves = "Found " + std::to_string(detectedWaves) + " waves";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);

    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strDetectedWaves.c_str(), NULL);

    nvgText(args.vg, 5, 21, "Start:", NULL);
    nvgText(args.vg, 80, 21, "End:", NULL);
    Widget::draw(args);
}

TFormBankEditClearRow::TFormBankEditClearRow() {
    box.size = Vec(238, 40);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 21, buttonWidth, buttonHeight);
    addChild(yesButton);
    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 21, buttonWidth, buttonHeight);
    addChild(noButton);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormBankEditClearRow::draw(const DrawArgs& args) {
    std::string strQuestion = "Do you really want to clear bank " + std::to_string(selectedBank + 1) + "?";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgText(args.vg, box.size.x / 2, 5, strQuestion.c_str(), NULL);
    Widget::draw(args);
}

TFormBankEditPurgeRow::TFormBankEditPurgeRow() {
    box.size = Vec(238, 40);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 21, buttonWidth, buttonHeight);
    addChild(yesButton);
    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 21, buttonWidth, buttonHeight);
    addChild(noButton);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormBankEditPurgeRow::draw(const DrawArgs& args) {
    std::string strQuestion = "Do you really want to purge ALL banks?";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgText(args.vg, box.size.x / 2, 5, strQuestion.c_str(), NULL);
    Widget::draw(args);
}

TFormBankEditCopyRow::TFormBankEditCopyRow() {

}

void TFormBankEditCopyRow::draw(const DrawArgs& args) {
    Widget::draw(args);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Edit menu
////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorBankEditMenu::TFormEditorBankEditMenu() {
    box.size = Vec(238, 195);

    emptySlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x32, 0x32, 0x7F), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x6F, 0x6F, 0xEF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x6F, 0x6F, 0xEF), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x9F, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0xBF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    selectedBank = 0;

    // Slots grid
    Vec offset = Vec(0, 40);
    grid = createWidget<TFormEditorGrid>(box.pos.plus(offset));
    grid->box.size.x = box.size.x;
    grid->box.size.y = box.size.y - offset.y;

    int id = 0;
    Vec slotButtonPos;
    float slotButtonWidth = grid->box.size.x / TRRFORM_EDITOR_COLS;
    float slotButtonHeight = grid->box.size.y / TRRFORM_EDITOR_ROWS;
    for(int row = 0; row < TRRFORM_EDITOR_ROWS; ++row) {
        for(int col = 0; col < TRRFORM_EDITOR_COLS; ++col) {
            auto selectBank = [=]() {
                selectedBank = id;
            };

            slotButtonPos = Vec(col * slotButtonWidth, row * slotButtonHeight + offset.y);
            slotButton[row][col] = createWidget<TFormEditorButton>(slotButtonPos);
            slotButton[row][col]->box.size.x = slotButtonWidth;
            slotButton[row][col]->box.size.y = slotButtonHeight;
            slotButton[row][col]->text = std::to_string(id + 1);
            slotButton[row][col]->onClick = selectBank;
            for(int m = 0; m < NUM_BUTTON_MODES; ++m) {
                slotButton[row][col]->buttonStyles[m] = emptySlotButtonStyles[m];
            }
            slotButton[row][col]->mode = IDLE_MODE;
            addChild(slotButton[row][col]);
            ++id;
        }
    }
    addChild(grid);

    for(int i = 0; i < TRRFORM_EDITOR_SLOTS; ++i) {
        slotFilled[i] = false;
    }

    // Button Rows

    mainButtonRow = createWidget<TFormBankEditMainRow>(Vec(0, 0));
    mainButtonRow->loadButton->onClick = [=]() {
        int detectedWaves = -1;
        if(onLoadWAVCallback) {
            detectedWaves = onLoadWAVCallback();
            if (detectedWaves > 0) {
                *loadButtonRow->endWave->choice = detectedWaves - 1;
                loadButtonRow->detectedWaves = detectedWaves;
                changeState(LOAD_WAVE_STATE);
            }
        }
    };
    mainButtonRow->clearButton->onClick = [=]() {
        changeState(CLEAR_BANK_STATE);
    };
    mainButtonRow->purgeButton->onClick = [=]() {
        changeState(PURGE_STATE);
    };
    addChild(mainButtonRow);

    loadButtonRow = createWidget<TFormBankEditLoadRow>(Vec(0, 0));
    loadButtonRow->ingestNewTable = [=](int startWave, int endWave) {
        onIngestTableCallback(selectedBank, startWave, endWave);
        slotFilled[selectedBank] = true;
        changeState(SELECT_BANK_STATE);
    };
    loadButtonRow->cancelButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(loadButtonRow);

    clearButtonRow = createWidget<TFormBankEditClearRow>(Vec(0, 0));
    clearButtonRow->yesButton->onClick = [=]() {
        onClearBankCallback(selectedBank);
        changeState(SELECT_BANK_STATE);
    };
    clearButtonRow->noButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(clearButtonRow);

    purgeButtonRow = createWidget<TFormBankEditPurgeRow>(Vec(0, 0));
    purgeButtonRow->yesButton->onClick = [=]() {
        for (int i = 0; i < TRRFORM_EDITOR_SLOTS; ++i) {
            onClearBankCallback(i);
        }
        changeState(SELECT_BANK_STATE);
    };
    purgeButtonRow->noButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(purgeButtonRow);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    changeState(SELECT_BANK_STATE);
}

void TFormEditorBankEditMenu::step() {
    int row = 0;
    int col = 0;

    for(auto i = 0; i < TRRFORM_EDITOR_SLOTS; ++i) {
        row = i / TRRFORM_EDITOR_ROWS;
        col = i % TRRFORM_EDITOR_COLS;
        slotButton[row][col]->setHighlight(i == selectedBank);
    }
    mainButtonRow->selectedBankIsFilled = slotFilled[selectedBank];
    mainButtonRow->selectedBank = selectedBank;
    Widget::step();
}

void TFormEditorBankEditMenu::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TRRFORM_EDITOR_SLOTS) {
        int row = slot / TRRFORM_EDITOR_ROWS;
        int col = slot % TRRFORM_EDITOR_COLS;
        slotFilled[slot] = isFilled;
        for(int m = 0; m < NUM_BUTTON_MODES; ++m) {
            if(isFilled) {
                slotButton[row][col]->buttonStyles[m] = filledSlotButtonStyles[m];
            }
            else {
                slotButton[row][col]->buttonStyles[m] = emptySlotButtonStyles[m];
            }
        }
    }
}

void TFormEditorBankEditMenu::changeState(const State newState) {
    mainButtonRow->visible = false;
    loadButtonRow->visible = false;
    clearButtonRow->visible = false;
    purgeButtonRow->visible = false;
    switch (newState) {
        case SELECT_BANK_STATE: mainButtonRow->visible = true; break;
        case LOAD_WAVE_STATE: loadButtonRow->visible = true; break;
        case CLEAR_BANK_STATE: clearButtonRow->visible = true; break;
        case PURGE_STATE: purgeButtonRow->visible = true; break;
    }
}

TFormEditorMainMenu::TFormEditorMainMenu() {
    Vec buttonSize = Vec(92, 42);
    float spacing = 8;
    float yOffset = 20.f;
    float buttonFontSize = 18;
    box.size.x = buttonSize.x * 2 + spacing;
    box.size.y = buttonSize.y * 2 + spacing + 20.f;

    editButton = createWidget<TFormEditorButton>(Vec(0, yOffset));
    exitButton = createWidget<TFormEditorButton>(Vec(0, buttonSize.y + spacing + yOffset));
    importButton = createWidget<TFormEditorButton>(Vec(buttonSize.x + spacing, yOffset));
    exportButton = createWidget<TFormEditorButton>(Vec(buttonSize.x + spacing, buttonSize.y + spacing + yOffset));
    title = createWidget<PlainText>(Vec(box.size.x / 2.f, 0));

    title->box.size = buttonSize;
    editButton->box.size = buttonSize;
    exitButton->box.size = buttonSize;
    importButton->box.size = buttonSize;
    exportButton->box.size = buttonSize;

    title->text = "Main Menu";
    editButton->text = "Edit";
    exitButton->text = "Exit";
    importButton->text = "Import";
    exportButton->text = "Export";

    editButton->fontSize = buttonFontSize;
    exitButton->fontSize = buttonFontSize;
    importButton->fontSize = buttonFontSize;
    exportButton->fontSize = buttonFontSize;

    addChild(editButton);
    addChild(exitButton);
    addChild(importButton);
    addChild(exportButton);
    addChild(title);

    //header = "User Wavetables";
}


////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditor::TFormEditor() {
    box.size = Vec(238, 195);

    auto gotoMainMenu = [=] {
        mainMenu->visible = true;
        editMenu->visible = false;
    };

    auto gotoEditMenu = [=] {
        mainMenu->visible = false;
        editMenu->visible = true;
    };

    auto showWaveDisplay = [=] {
        mainMenu->visible = false;
        editMenu->visible = false;
        waveDisplay->visible = true;
    };

    // Menu buttons
    mainMenu = new TFormEditorMainMenu;
    mainMenu->box.pos.x = box.getCenter().x - mainMenu->box.getCenter().x;
    mainMenu->box.pos.y = box.getCenter().y - mainMenu->box.getCenter().y - 10.f;
    mainMenu->editButton->onClick = gotoEditMenu;
    addChild(mainMenu);

    editMenu = createWidget<TFormEditorBankEditMenu>(Vec(0,0));
    editMenu->mainButtonRow->backButton->onClick = gotoMainMenu;
    addChild(editMenu);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(20,20));
    waveDisplay->box.size.x = box.size.x;
    waveDisplay->box.size.y = box.size.y;
    addChild(waveDisplay);

    //gotoMainMenu();
    showWaveDisplay();
}

void TFormEditor::addOnExitCallback(const std::function<void()>& onExitCallback) {
    mainMenu->exitButton->onClick = onExitCallback;
}

void TFormEditor::addLoadWAVCallback(const std::function<int()>& onLoadWAVCallback) {
    editMenu->onLoadWAVCallback = onLoadWAVCallback;
}

void TFormEditor::addIngestTableCallback(const std::function<void(int, int, int)>& onIngestTableCallback) {
    editMenu->onIngestTableCallback = onIngestTableCallback;
}

void TFormEditor::addClearBankCallback(const std::function<void(int)>&  onClearBankCallback) {
    editMenu->onClearBankCallback = onClearBankCallback;
}

void TFormEditor::addCloneBankCallback(const std::function<void(int)>& onCloneBankCallback) {
    editMenu->onCloneBankCallback = onCloneBankCallback;
}

void TFormEditor::addImportCallback(const std::function<void()>& onImportWaveTableCallback) {
    mainMenu->importButton->onClick = onImportWaveTableCallback;
}

void TFormEditor::addExportCallback(const std::function<void()>& onExportWaveTableCallback) {
    mainMenu->exportButton->onClick = onExportWaveTableCallback;
}

void TFormEditor::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TRRFORM_EDITOR_SLOTS) {
        editMenu->setSlotFilledFlag(slot, isFilled);
    }
}
