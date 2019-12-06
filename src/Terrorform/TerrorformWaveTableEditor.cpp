#include "TerrorformWaveTableEditor.hpp"

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
    grid = createWidget<TFormEditorGrid<TRRFORM_EDITOR_ROWS, TRRFORM_EDITOR_COLS>>(box.pos.plus(offset));
    grid->box.size.x = box.size.x;
    grid->box.size.y = box.size.y - offset.y;

    int id = 0;
    for(int row = 0; row < TRRFORM_EDITOR_ROWS; ++row) {
        for(int col = 0; col < TRRFORM_EDITOR_COLS; ++col) {
            auto selectBank = [=]() {
                selectedBank = id;
            };
            grid->slotButton[row][col]->onClick = selectBank;
            grid->slotButton[row][col]->text = std::to_string(id + 1);
            ++id;
        }
    }
    addChild(grid);

    for(int i = 0; i < TRRFORM_EDITOR_SLOTS; ++i) {
        slotFilled[i] = false;
    }

    // Main button row
    mainButtonRow = createWidget<TFormBankEditMainRow>(Vec(0, 0));
    mainButtonRow->loadButton->onClick = [=]() {
        //int detectedWaves = -1;
        std::shared_ptr<std::vector<std::vector<float>>> detectedWaves;
        if(onLoadWAVCallback) {
            detectedWaves = onLoadWAVCallback();
            if (detectedWaves->size() > 0) {
                *loadButtonRow->endWave->choice = detectedWaves->size() - 1;
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
    mainButtonRow->viewButton->onClick = [=]() {
        onViewBankCallback(selectedBank, viewPane->waveData);
        changeState(VIEW_BANK_STATE);
    };
    addChild(mainButtonRow);

    // Load row
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

    // Clear row
    clearButtonRow = createWidget<TFormClearRow>(Vec(0, 0));
    clearButtonRow->yesButton->onClick = [=]() {
        onClearBankCallback(selectedBank);
        changeState(SELECT_BANK_STATE);
    };
    clearButtonRow->noButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(clearButtonRow);

    // Purge row
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

    // View row
    viewPane = createWidget<TFormWaveViewPane>(Vec(0, 0));
    viewPane->backButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(viewPane);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    changeState(SELECT_BANK_STATE);
}

void TFormEditorBankEditMenu::step() {
    int row = 0;
    int col = 0;

    for(auto i = 0; i < TRRFORM_EDITOR_SLOTS; ++i) {
        row = i / TRRFORM_EDITOR_ROWS;
        col = i % TRRFORM_EDITOR_COLS;
        grid->slotButton[row][col]->setHighlight(i == selectedBank);
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
                grid->slotButton[row][col]->buttonStyles[m] = filledSlotButtonStyles[m];
            }
            else {
                grid->slotButton[row][col]->buttonStyles[m] = emptySlotButtonStyles[m];
            }
        }
    }
}

void TFormEditorBankEditMenu::changeState(const State newState) {
    mainButtonRow->visible = false;
    loadButtonRow->visible = false;
    clearButtonRow->visible = false;
    purgeButtonRow->visible = false;
    viewPane->visible = false;
    grid->visible = false;
    switch (newState) {
        case SELECT_BANK_STATE:
            mainButtonRow->visible = true;
            grid->visible = true;
            break;
        case LOAD_WAVE_STATE:
            loadButtonRow->visible = true;
            break;
        case CLEAR_BANK_STATE:
            clearButtonRow->visible = true;
            break;
        case PURGE_STATE:
            purgeButtonRow->visible = true;
            break;
        case VIEW_BANK_STATE:
            viewPane->visible = true;
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Main menu
////////////////////////////////////////////////////////////////////////////////////////////////////

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

    title->text = "User Wavetable Manager";
    editButton->text = "Edit";
    exitButton->text = "Exit";
    importButton->text = "Load ROM";
    exportButton->text = "Save ROM";

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

    // Menu buttons
    mainMenu = new TFormEditorMainMenu;
    mainMenu->box.pos.x = box.getCenter().x - mainMenu->box.getCenter().x;
    mainMenu->box.pos.y = box.getCenter().y - mainMenu->box.getCenter().y - 10.f;
    mainMenu->editButton->onClick = gotoEditMenu;
    addChild(mainMenu);

    editMenu = createWidget<TFormEditorBankEditMenu>(Vec(0,0));
    editMenu->mainButtonRow->backButton->onClick = gotoMainMenu;
    addChild(editMenu);

    gotoMainMenu();
    //showWaveDisplay();
}

void TFormEditor::addOnExitCallback(const std::function<void()>& onExitCallback) {
    mainMenu->exitButton->onClick = onExitCallback;
}

void TFormEditor::addLoadWAVCallback(const std::function<std::shared_ptr<std::vector<std::vector<float>>>()>& onLoadWAVCallback) {
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

void TFormEditor::addViewBankCallback(const std::function<void(int, float[TFORM_MAX_NUM_WAVES][TFORM_MAX_WAVELENGTH])>& onViewBankCallback) {
    editMenu->onViewBankCallback = onViewBankCallback;
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
