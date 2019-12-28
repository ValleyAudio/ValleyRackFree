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

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormBankEditMainRow::TFormBankEditMainRow() {
    box.size = Vec(238, 195);
    loadButton = createNewMenuButton("Load", NULL, buttonOffset, 21, buttonWidth, buttonHeight);
    addChild(loadButton);
    viewButton = createNewMenuButton("View", NULL, buttonWidth + buttonOffset * 2, 21, buttonWidth, buttonHeight);
    addChild(viewButton);
    cloneButton = createNewMenuButton("Clone", NULL, buttonWidth * 2 + buttonOffset * 3, 21, buttonWidth, buttonHeight);
    addChild(cloneButton);
    clearButton = createNewMenuButton("Clear", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    addChild(clearButton);
    purgeButton = createNewMenuButton("Purge", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(purgeButton);
    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    addChild(backButton);

    // Slots grid
    emptySlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x32, 0x7F, 0x32), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0xCF, 0x2F), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0xCF, 0x2F), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x9F, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0xBF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    Vec offset = Vec(0, 40);
    grid = createWidget<TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>>(box.pos.plus(offset));
    grid->box.size.x = box.size.x;
    grid->box.size.y = box.size.y - offset.y;

    int id = 0;
    for (int row = 0; row < TFORM_EDITOR_ROWS; ++row) {
        for (int col = 0; col < TFORM_EDITOR_COLS; ++col) {
            auto selectBank = [=]() {
                *selectedBank = id;
            };
            grid->slotButton[row][col]->onClick = selectBank;
            grid->slotButton[row][col]->text = std::to_string(id + 1);
            ++id;
        }
    }
    addChild(grid);
}

void TFormBankEditMainRow::step() {
    int row = 0;
    int col = 0;

    for(auto i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
        row = i / TFORM_EDITOR_ROWS;
        col = i % TFORM_EDITOR_COLS;
        grid->slotButton[row][col]->setHighlight(i == *selectedBank);
    }

    selectedBankIsFilled = (*slotFilled)[*selectedBank];
}

void TFormBankEditMainRow::draw(const DrawArgs& args) {
    std::string strSelectedBank = "Bank: " + std::to_string(*selectedBank + 1);
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

void TFormBankEditMainRow::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        int row = slot / TFORM_EDITOR_ROWS;
        int col = slot % TFORM_EDITOR_COLS;
        (*slotFilled)[slot] = isFilled;
        for(int m = 0; m < NUM_BUTTON_MODES; ++m) {
            if(isFilled) {
                grid->slotButton[row][col]->buttonStyles[m] = filledSlotButtonStyles[m];
            }
            else {
                grid->slotButton[row][col]->buttonStyles[m] = emptySlotButtonStyles[m];
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormBankEditLoadRow::TFormBankEditLoadRow() {
    box.size = Vec(238, 195);
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

    ingestNewTable = [=](int startWave, int endWave) {
        onIngestTableCallback(*selectedBank, startWave, endWave);
        (*slotFilled)[*selectedBank] = true;
        hide();
    };

    auto triggerIngest = [=]() {
        ingestNewTable(*startWave->choice, *endWave->choice);
    };

    cancelButton = createNewMenuButton("Cancel", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    cancelButton->onClick = [=]() {
        hide();
    };
    addChild(cancelButton);
    confirmButton = createNewMenuButton("Okay", triggerIngest, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(confirmButton);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(1.5,27));
    waveDisplay->box.size.x = box.size.x - 3.f;
    waveDisplay->box.size.y = box.size.y - 27.f;
    addChild(waveDisplay);
    waveSliderPos = 0.f;
    selectedWave = 0;

    waveLineColor = nvgRGB(0xAF, 0xAF, 0xAF);
    waveFillColor = nvgRGBA(0xAF, 0xAF, 0xAF, 0x6F);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormBankEditLoadRow::draw(const DrawArgs& args) {
    std::string strDetectedWaves = "Found " + std::to_string(detectedWaves->size()) + " waves";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);

    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strDetectedWaves.c_str(), NULL);
    nvgText(args.vg, 5, 21, "Start:", NULL);
    nvgText(args.vg, 80, 21, "End:", NULL);

    // Horizontal bar
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 0, box.pos.y + 40);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 40);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);

    Widget::draw(args);
}

void TFormBankEditLoadRow::step() {
    if(detectedWaves) {
        for (unsigned long i = 0; i < (*detectedWaves).size(); ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[i][j] = 0.f;
            }
        }

        unsigned long k = 0;
        for (unsigned long i = *startWave->choice; i <= *endWave->choice; ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[k][j] = (*detectedWaves)[i][j];
            }
            ++k;
        }
    }

    waveDisplay->numWaves = *endWave->choice - *startWave->choice + 1;
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormBankEditLoadRow::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormClearRow::TFormClearRow() {
    box.size = Vec(238, 40);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 21, buttonWidth, buttonHeight);
    yesButton->onClick = [=]() {
        onClearBankCallback(*selectedBank);
    };
    addChild(yesButton);

    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 21, buttonWidth, buttonHeight);
    noButton->onClick = [=]() {
        hide();
    };
    addChild(noButton);
}

void TFormClearRow::draw(const DrawArgs& args) {
    std::string strQuestion = "Do you really want to clear bank " + std::to_string(*selectedBank + 1) + "?";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgText(args.vg, box.size.x / 2, 5, strQuestion.c_str(), NULL);
    Widget::draw(args);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormPurgeRow::TFormPurgeRow() {
    box.size = Vec(238, 40);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 21, buttonWidth, buttonHeight);
    yesButton->onClick = [=]() {
        for (int i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
            onClearBankCallback(i);
        }
        hide();
    };
    addChild(yesButton);

    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 21, buttonWidth, buttonHeight);
    noButton->onClick = [=]() {
        hide();
    };
    addChild(noButton);
}

void TFormPurgeRow::draw(const DrawArgs& args) {
    std::string strQuestion = "Do you really want to purge ALL banks?";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgText(args.vg, box.size.x / 2, 5, strQuestion.c_str(), NULL);
    Widget::draw(args);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormCloneRow::TFormCloneRow() {
    box.size = Vec(238, 195);
    sourceBank = 0;
    destBank = 0;

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

    selectSourceBackButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    selectSourceBackButton->onClick = [=]() {
        hide();
    };
    addChild(selectSourceBackButton);

    selectDestBackButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    selectDestBackButton->onClick = [=] {
        changeState(SELECT_SOURCE_STATE);
    };
    addChild(selectDestBackButton);

    nextButton = createNewMenuButton("Next", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    nextButton->onClick = [=]() {
        changeState(SELECT_DEST_STATE);
    };
    addChild(nextButton);

    emptySlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x32, 0x7F, 0x32), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0xCF, 0x2F), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0xCF, 0x2F), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    sourceSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    sourceSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x32, 0x7F, 0x32), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    sourceSlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0xCF, 0x2F), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    sourceSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0xCF, 0x2F), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    destSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    destSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x32, 0x32, 0x7F), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    destSlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x6F, 0x6F, 0xEF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    destSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x6F, 0x6F, 0xEF), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x9F, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0xBF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    Vec offset = Vec(0, 40);
    grid = createWidget<TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>>(box.pos.plus(offset));
    grid->box.size.x = box.size.x;
    grid->box.size.y = box.size.y - offset.y;

    int id = 0;
    for (int row = 0; row < TFORM_EDITOR_ROWS; ++row) {
        for (int col = 0; col < TFORM_EDITOR_COLS; ++col) {
            auto selectBank = [=]() {
                destBank = id;
            };
            grid->slotButton[row][col]->onClick = selectBank;
            grid->slotButton[row][col]->text = std::to_string(id + 1);
            ++id;
        }
    }

    grid->visible = false;
    addChild(grid);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(1.5,27));
    waveDisplay->box.size.x = box.size.x - 3.f;
    waveDisplay->box.size.y = box.size.y - 27.f;
    addChild(waveDisplay);

    waveSliderPos = 0.f;
    selectedWave = 0;

    waveLineColor = nvgRGB(0xAF, 0xAF, 0xAF);
    waveFillColor = nvgRGBA(0xAF, 0xAF, 0xAF, 0x6F);

    waveData.resize(TFORM_MAX_NUM_WAVES);
    for (int i = 0; i < TFORM_MAX_NUM_WAVES; ++i) {
        waveData[i].resize(TFORM_MAX_WAVELENGTH);
        for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
            waveData[i][j] = 0.f;
        }
    }

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    onView = [=] {
        changeState(SELECT_SOURCE_STATE);
    };
}

void TFormCloneRow::step() {
    int row = 0;
    int col = 0;

    for(auto i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
        row = i / TFORM_EDITOR_ROWS;
        col = i % TFORM_EDITOR_COLS;
        grid->slotButton[row][col]->setHighlight(i == *sourceBank || i == destBank);
    }

    int j = 0;
    for (unsigned long i = *startWave->choice; i <= *endWave->choice; ++i) {
        memcpy(&waveDisplay->waveData[j], waveData[i].data(), sizeof(float) * TFORM_MAX_WAVELENGTH);
        ++j;
    }
    waveDisplay->numWaves = *endWave->choice - *startWave->choice + 1;
    selectedWave = waveDisplay->selectedWave;
}

void TFormCloneRow::draw(const DrawArgs& args) {
    switch(state) {
        case SELECT_SOURCE_STATE: displaySourceSelection(args); break;
        case SELECT_DEST_STATE: displayDestinationSelection(args); break;
        case CONFIRM_OVERWRITE_STATE: break;
    }

    // Horizontal bar
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 0, box.pos.y + 40);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 40);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);

    Widget::draw(args);
}

void TFormCloneRow::displaySourceSelection(const DrawArgs& args) {
    std::string strCloningFrom = "Cloning from bank " + std::to_string(*sourceBank);
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);

    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strCloningFrom.c_str(), NULL);
    nvgText(args.vg, 5, 21, "Start:", NULL);
    nvgText(args.vg, 80, 21, "End:", NULL);
}

void TFormCloneRow::displayDestinationSelection(const DrawArgs& args) {

}

void TFormCloneRow::changeState(TFormCloneRowState newState) {
    state = newState;

    grid->visible = false;
    waveDisplay->visible = false;
    selectSourceBackButton->visible = false;
    selectDestBackButton->visible = false;
    nextButton->visible = false;
    // cancelButton->visible = false;
    // pasteButton->visible = false;
    startWave->visible = false;
    endWave->visible = false;
    switch(state) {
        case SELECT_SOURCE_STATE:
            waveDisplay->visible = true;
            selectSourceBackButton->visible = true;
            nextButton->visible = true;
            startWave->visible = true;
            endWave->visible = true;
            *startWave->choice = 0;
            *endWave->choice = waveData.size() - 1;
            break;
        case SELECT_DEST_STATE:
            selectDestBackButton->visible = true;
            grid->visible = true;
            break;
        case CONFIRM_OVERWRITE_STATE:
            break;
    }
}

void TFormCloneRow::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}

void TFormCloneRow::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        int row = slot / TFORM_EDITOR_ROWS;
        int col = slot % TFORM_EDITOR_COLS;
        (*slotFilled)[slot] = isFilled;
        for(int m = 0; m < NUM_BUTTON_MODES; ++m) {
            if(isFilled) {
                grid->slotButton[row][col]->buttonStyles[m] = filledSlotButtonStyles[m];
            }
            else {
                grid->slotButton[row][col]->buttonStyles[m] = emptySlotButtonStyles[m];
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormWaveViewPane::TFormWaveViewPane() {
    box.size = Vec(238, 195);

    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    backButton->onClick = [=] {
        hide();
    };
    addChild(backButton);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(1.5,8));
    waveDisplay->box.size.x = box.size.x - 3.f;
    waveDisplay->box.size.y = box.size.y - 8.f;
    addChild(waveDisplay);

    waveLineColor = nvgRGB(0xCF, 0x00, 0x00);
    waveFillColor = nvgRGBA(0xCF, 0x00, 0x00, 0x4F);

    waveData.resize(TFORM_MAX_NUM_WAVES);
    for (int i = 0; i < TFORM_MAX_NUM_WAVES; ++i) {
        waveData[i].resize(TFORM_MAX_WAVELENGTH);
        for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
            waveData[i][j] = 0.f;
        }
    }
}

void TFormWaveViewPane::draw(const DrawArgs& args) {
    std::string strText = "Viewing wave " + std::to_string(selectedWave) + " 64 of bank " + std::to_string(*selectedBank);
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strText.c_str(), NULL);

    // Horizontal bar
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 0, box.pos.y + 20);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 20);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);

    Widget::draw(args);
}

void TFormWaveViewPane::step() {
    waveDisplay->numWaves = waveData.size();
    for (int i = 0; i < waveData.size(); ++i) {
        memcpy(&waveDisplay->waveData[i], waveData[i].data(), sizeof(float) * TFORM_MAX_WAVELENGTH);
    }
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormWaveViewPane::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}
