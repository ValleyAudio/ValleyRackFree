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
                                                          nvgRGB(0xFF, 0x00, 0x00),
                                                          nvgRGB(0x7F, 0x7F, 0x7F));
    buttonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                nvgRGB(0xFF, 0x00, 0x00),
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

    /*auto drawSlotFilledMarker = [=](int row, int col) {
        float x = col * box.size.x / TRRFORM_EDITOR_COLS;
        float y = row * gridHeight / TRRFORM_EDITOR_ROWS;
        nvgBeginPath(args.vg);
        nvgStrokeColor(args.vg, nvgRGB(0xFF, 0x00, 0x00));
        nvgStrokeWidth(args.vg, 2.0);
        nvgMoveTo(args.vg, x + 2, y + 5);
        nvgLineTo(args.vg, x + 5, y + 8);
        nvgLineTo(args.vg, x + 11, y + 2);
        nvgStroke(args.vg);
    };

    int row = 0;
    int col = 0;
    for(int i = 0; i < TRRFORM_EDITOR_SLOTS; ++i) {
        row = i / TRRFORM_EDITOR_ROWS;
        col = i % TRRFORM_EDITOR_COLS;
        if(slotFilled[i]) {
            drawSlotFilledMarker(row, col);
        }
    }*/
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

TFormEditorMainMenu::TFormEditorMainMenu() {
    Vec buttonSize = Vec(92, 42);
    float spacing = 8;
    float buttonFontSize = 18;
    editButton = createWidget<TFormEditorButton>(Vec(0, 0));
    exitButton = createWidget<TFormEditorButton>(Vec(0, buttonSize.y + spacing));
    importButton = createWidget<TFormEditorButton>(Vec(buttonSize.x + spacing, 0));
    exportButton = createWidget<TFormEditorButton>(Vec(buttonSize.x + spacing, buttonSize.y + spacing));

    editButton->box.size = buttonSize;
    exitButton->box.size = buttonSize;
    importButton->box.size = buttonSize;
    exportButton->box.size = buttonSize;

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

    box.size.x = buttonSize.x * 2 + spacing;
    box.size.y = buttonSize.y * 2 + spacing;

    //header = "User Wavetables";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Edit menu
////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorEditMenu::TFormEditorEditMenu() {
    box.size = Vec(238, 195);
    float menuButtonWidth = 44;
    float menuButtonHeight = 15;

    emptySlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF),
                                                              nvgRGB(0x17, 0x17, 0x17),
                                                              nvgRGBA(0x00, 0x00, 0x00, 0x00));

    emptySlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                               nvgRGB(0x32, 0x32, 0x7F),
                                                               nvgRGBA(0x00, 0x00, 0x00, 0x00));

    emptySlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                    nvgRGB(0x6F, 0x6F, 0xEF),
                                                                    nvgRGBA(0x00, 0x00, 0x00, 0x00));

    emptySlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                         nvgRGB(0x6F, 0x6F, 0xEF),
                                                                         nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF),
                                                               nvgRGB(0xAF, 0x00, 0x00),
                                                               nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                nvgRGB(0xAF, 0x00, 0x00),
                                                                nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                    nvgRGB(0xFF, 0x00, 0x00),
                                                                    nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                          nvgRGB(0xFF, 0x00, 0x00),
                                                                          nvgRGBA(0x00, 0x00, 0x00, 0x00));

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

    // Menu buttons
    auto loadWAVFile = [=]() {
        detectedWaves = -1;
        if(onLoadWAVCallback == NULL) {
            return;
        }
        detectedWaves = onLoadWAVCallback();
        if(detectedWaves > 0) {
            loadButton->visible = false;
            cloneButton->visible = false;
            clearButton->visible = false;
            purgeButton->visible = false;
            backButton->visible = false;

            cancelButton->visible = true;
            confirmButton->visible = true;
            startWave->visible = true;
            endWave->visible = true;
            *endWave->choice = detectedWaves - 1;

            state = LOAD_WAVE_STATE;
        }
    };

    loadButton = createWidget<TFormEditorButton>(Vec(3, 21));
    loadButton->text = "Load";
    loadButton->box.size.x = menuButtonWidth;
    loadButton->box.size.y = menuButtonHeight;
    loadButton->onClick = loadWAVFile;
    addChild(loadButton);

    cloneButton = createWidget<TFormEditorButton>(Vec(menuButtonWidth + 6, 21));
    cloneButton->text = "Clone";
    cloneButton->box.size.x = menuButtonWidth;
    cloneButton->box.size.y = menuButtonHeight;
    addChild(cloneButton);

    // Clear button
    auto clearBank = [=]() {
        loadButton->visible = false;
        cloneButton->visible = false;
        clearButton->visible = false;
        purgeButton->visible = false;
        backButton->visible = false;

        clearYesButton->visible = true;
        clearNoButton->visible = true;
        state = CLEAR_BANK_STATE;
    };

    auto clearYes = [=]() {

    };

    auto clearNo = [=]() {
        loadButton->visible = true;
        cloneButton->visible = true;
        clearButton->visible = true;
        purgeButton->visible = true;
        backButton->visible = true;

        clearYesButton->visible = false;
        clearNoButton->visible = false;
        state = SELECT_BANK_STATE;
    };

    clearButton = createWidget<TFormEditorButton>(Vec(menuButtonWidth * 2 + 9, 21));
    clearButton->text = "Clear";
    clearButton->box.size.x = menuButtonWidth;
    clearButton->box.size.y = menuButtonHeight;
    clearButton->onClick = clearBank;
    addChild(clearButton);

    clearYesButton = createWidget<TFormEditorButton>(Vec(box.size.x / 2 - menuButtonWidth - 1.5, 21));
    clearYesButton->text = "Yes";
    clearYesButton->box.size.x = menuButtonWidth;
    clearYesButton->box.size.y = menuButtonHeight;
    clearYesButton->onClick = clearYes;
    clearYesButton->visible = false;
    addChild(clearYesButton);

    clearNoButton = createWidget<TFormEditorButton>(Vec(box.size.x / 2 + 1.5, 21));
    clearNoButton->text = "No";
    clearNoButton->box.size.x = menuButtonWidth;
    clearNoButton->box.size.y = menuButtonHeight;
    clearNoButton->onClick = clearNo;
    clearNoButton->visible = false;
    addChild(clearNoButton);

    //Purge button
    purgeButton = createWidget<TFormEditorButton>(Vec(menuButtonWidth * 3 + 12, 21));
    purgeButton->text = "Purge";
    purgeButton->box.size.x = menuButtonWidth;
    purgeButton->box.size.y = menuButtonHeight;
    addChild(purgeButton);

    backButton = createWidget<TFormEditorButton>(Vec(box.size.x - menuButtonWidth - 3, 21));
    backButton->text = "Back";
    backButton->box.size.x = menuButtonWidth;
    backButton->box.size.y = menuButtonHeight;
    addChild(backButton);

    // Table load buttons
    auto cancelLoadWAV = [=]() {
        loadButton->visible = true;
        cloneButton->visible = true;
        clearButton->visible = true;
        purgeButton->visible = true;
        backButton->visible = true;

        cancelButton->visible = false;
        confirmButton->visible = false;
        startWave->visible = false;
        endWave->visible = false;

        state = SELECT_BANK_STATE;
    };

    cancelButton = createWidget<TFormEditorButton>(Vec(menuButtonWidth  * 3 + 12, 21));
    cancelButton->text = "Cancel";
    cancelButton->box.size.x = menuButtonWidth;
    cancelButton->box.size.y = menuButtonHeight;
    cancelButton->onClick = cancelLoadWAV;
    cancelButton->visible = false;
    addChild(cancelButton);

    auto ingestNewTable = [=]() {
        int row = selectedBank / TRRFORM_EDITOR_ROWS;
        int col = selectedBank % TRRFORM_EDITOR_COLS;
        onIngestTableCallback(selectedBank, *startWave->choice, *endWave->choice);
        slotFilled[selectedBank] = true;
        loadButton->visible = true;
        cloneButton->visible = true;
        clearButton->visible = true;
        purgeButton->visible = true;
        backButton->visible = true;

        cancelButton->visible = false;
        confirmButton->visible = false;
        startWave->visible = false;
        endWave->visible = false;

        state = SELECT_BANK_STATE;
    };

    confirmButton = createWidget<TFormEditorButton>(Vec(box.size.x - menuButtonWidth - 3, 21));
    confirmButton->text = "Okay";
    confirmButton->box.size.x = menuButtonWidth;
    confirmButton->box.size.y = menuButtonHeight;
    confirmButton->onClick = ingestNewTable;
    confirmButton->visible = false;
    addChild(confirmButton);

    startWave = createWidget<TFormEditorNumberChoice>(Vec(45, 20));
    startWave->range = 64;
    startWave->box.size.x = 28;
    startWave->box.size.y = 15;
    startWave->visible = false;
    addChild(startWave);

    endWave = createWidget<TFormEditorNumberChoice>(Vec(108, 20));
    endWave->range = 64;
    endWave->box.size.x = 28;
    endWave->box.size.y = 15;
    endWave->visible = false;
    addChild(endWave);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    state = SELECT_BANK_STATE;
}

void TFormEditorEditMenu::step() {
    int row = 0;
    int col = 0;
    for(auto i = 0; i < TRRFORM_EDITOR_SLOTS; ++i) {
        row = i / TRRFORM_EDITOR_ROWS;
        col = i % TRRFORM_EDITOR_COLS;
        slotButton[row][col]->setHighlight(i == selectedBank);
    }
    Widget::step();
}

void TFormEditorEditMenu::draw(const DrawArgs& args) {
    if(state == SELECT_BANK_STATE) {
        std::string strSelectedBank = "Selected bank: " + std::to_string(selectedBank + 1);
        nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);
        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(args.vg, 5, 5, strSelectedBank.c_str(), NULL);
    }
    if(state == LOAD_WAVE_STATE) {
        strDetectedWaves = "Found " + std::to_string(detectedWaves) + " waves";
        nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(args.vg, 5, 5, strDetectedWaves.c_str(), NULL);

        nvgText(args.vg, 5, 21, "Start:", NULL);
        nvgText(args.vg, 80, 21, "End:", NULL);
    }
    if(state == CLEAR_BANK_STATE) {
        std::string strQuestion = "Do you really want to clear bank " + std::to_string(selectedBank + 1) + "?";
        nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);
        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
        nvgText(args.vg, box.size.x / 2, 5, strQuestion.c_str(), NULL);
    }

    Widget::draw(args);
}

void TFormEditorEditMenu::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TRRFORM_EDITOR_SLOTS) {
        int row = slot / TRRFORM_EDITOR_ROWS;
        int col = slot % TRRFORM_EDITOR_COLS;

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

    // Menu buttons
    mainMenu = new TFormEditorMainMenu;
    mainMenu->box.pos.x = box.getCenter().x - mainMenu->box.getCenter().x;
    mainMenu->box.pos.y = box.getCenter().y - mainMenu->box.getCenter().y;
    mainMenu->editButton->onClick = gotoEditMenu;
    addChild(mainMenu);

    editMenu = createWidget<TFormEditorEditMenu>(Vec(0,0));
    editMenu->backButton->onClick = gotoMainMenu;
    addChild(editMenu);

    //gotoMainMenu();
    gotoEditMenu();
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
