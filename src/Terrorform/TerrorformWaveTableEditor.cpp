#include "TerrorformWaveTableEditor.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Edit menu
////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorBankEditMenu::TFormEditorBankEditMenu() {
    box.size = Vec(238, 195);

    slotFilled = std::make_shared<std::vector<bool>>(TFORM_EDITOR_SLOTS, false);
    selectedBank = std::make_shared<int>(0);

    // Main button row
    mainButtonRow = createWidget<TFormBankEditMainRow>(Vec(0, 0));
    mainButtonRow->slotFilled = slotFilled;
    mainButtonRow->selectedBank = selectedBank;

    mainButtonRow->loadButton->onClick = [=]() {
        //int detectedWaves = -1;
        std::shared_ptr<std::vector<std::vector<float>>> detectedWaves;
        if (onLoadWAVCallback) {
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
        onViewBankCallback(*selectedBank, viewPane->waveData);
        changeState(VIEW_BANK_STATE);
    };

    mainButtonRow->cloneButton->onClick = [=]() {
        changeState(CLONE_BANK_STATE);
    };

    addChild(mainButtonRow);

    // Load row
    loadButtonRow = createWidget<TFormBankEditLoadRow>(Vec(0, 0));
    loadButtonRow->ingestNewTable = [=](int startWave, int endWave) {
        onIngestTableCallback(*selectedBank, startWave, endWave);
        (*slotFilled)[*selectedBank] = true;
        changeState(SELECT_BANK_STATE);
    };
    loadButtonRow->cancelButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(loadButtonRow);

    // Clear row
    clearButtonRow = createWidget<TFormClearRow>(Vec(0, 0));
    clearButtonRow->selectedBank = selectedBank;
    clearButtonRow->yesButton->onClick = [=]() {
        onClearBankCallback(*selectedBank);
        changeState(SELECT_BANK_STATE);
    };
    clearButtonRow->noButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(clearButtonRow);

    // Purge row
    purgeButtonRow = createWidget<TFormPurgeRow>(Vec(0, 0));
    purgeButtonRow->yesButton->onClick = [=]() {
        for (int i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
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
    viewPane->selectedBank = selectedBank;
    viewPane->backButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(viewPane);

    // Clone menu
    cloneMenu = createWidget<TFormCloneRow>(Vec(0, 0));
    cloneMenu->slotFilled = slotFilled;
    cloneMenu->sourceBank = selectedBank;
    cloneMenu->backButton->onClick = [=]() {
        changeState(SELECT_BANK_STATE);
    };
    addChild(cloneMenu);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    changeState(SELECT_BANK_STATE);
}

void TFormEditorBankEditMenu::setSlotFilledFlag(int slot, bool isFilled) {
    mainButtonRow->setSlotFilledFlag(slot, isFilled);
    cloneMenu->setSlotFilledFlag(slot, isFilled);
}

void TFormEditorBankEditMenu::changeState(const State newState) {
    mainButtonRow->visible = false;
    loadButtonRow->visible = false;
    clearButtonRow->visible = false;
    purgeButtonRow->visible = false;
    viewPane->visible = false;
    cloneMenu->visible = false;
    switch (newState) {
        case SELECT_BANK_STATE:
            mainButtonRow->visible = true;
            break;
        case LOAD_WAVE_STATE:
            loadButtonRow->visible = true;
            break;
        case VIEW_BANK_STATE:
            viewPane->visible = true;
            break;
        case CLONE_BANK_STATE:
            cloneMenu->visible = true;
            break;
        case CLEAR_BANK_STATE:
            clearButtonRow->visible = true;
            break;
        case PURGE_STATE:
            purgeButtonRow->visible = true;
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

void TFormEditor::addCloneBankCallback(const std::function<void(int, int)>& onCloneBankCallback) {
    editMenu->cloneMenu->onCloneBankCallback = onCloneBankCallback;
}

void TFormEditor::addViewBankCallback(const std::function<void(int, std::vector<std::vector<float>>&)>& onViewBankCallback) {
    editMenu->onViewBankCallback = onViewBankCallback;
}

void TFormEditor::addImportCallback(const std::function<void()>& onImportWaveTableCallback) {
    mainMenu->importButton->onClick = onImportWaveTableCallback;
}

void TFormEditor::addExportCallback(const std::function<void()>& onExportWaveTableCallback) {
    mainMenu->exportButton->onClick = onExportWaveTableCallback;
}

void TFormEditor::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        editMenu->setSlotFilledFlag(slot, isFilled);
    }
}
