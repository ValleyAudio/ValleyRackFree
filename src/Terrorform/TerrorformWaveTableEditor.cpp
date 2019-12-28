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
        if (onLoadWAVCallback) {
            std::shared_ptr<std::vector<std::vector<float>>> detectedWaves = onLoadWAVCallback();
            if (detectedWaves->size() > 0) {
                *loadButtonRow->endWave->choice = detectedWaves->size() - 1;
                loadButtonRow->detectedWaves = detectedWaves;
                mainButtonRow->hide();
                loadButtonRow->view();
            }
        }
    };

    mainButtonRow->clearButton->onClick = [=]() {
        mainButtonRow->hide();
        clearButtonRow->view();
    };

    mainButtonRow->purgeButton->onClick = [=]() {
        mainButtonRow->hide();
        purgeButtonRow->view();
    };

    mainButtonRow->viewButton->onClick = [=]() {
        onGetBankCallback(*selectedBank, viewPane->waveData);
        mainButtonRow->hide();
        viewPane->view();
    };

    mainButtonRow->cloneButton->onClick = [=]() {
        onGetBankCallback(*selectedBank, cloneMenu->waveData);
        mainButtonRow->hide();
        cloneMenu->view();
    };

    addChild(mainButtonRow);

    // Load row
    loadButtonRow = createWidget<TFormBankEditLoadRow>(Vec(0, 0));
    loadButtonRow->selectedBank = selectedBank;
    loadButtonRow->slotFilled = slotFilled;
    loadButtonRow->onHide = [=]() {
        mainButtonRow->view();
    };
    loadButtonRow->visible = false;
    addChild(loadButtonRow);

    // Clear row
    clearButtonRow = createWidget<TFormClearRow>(Vec(0, 0));
    clearButtonRow->selectedBank = selectedBank;
    clearButtonRow->onHide = [=]() {
        mainButtonRow->view();
    };
    clearButtonRow->visible = false;
    addChild(clearButtonRow);

    // Purge row
    purgeButtonRow = createWidget<TFormPurgeRow>(Vec(0, 0));
    purgeButtonRow->onHide = [=]() {
        mainButtonRow->view();
    };
    purgeButtonRow->visible = false;
    addChild(purgeButtonRow);

    // View row
    viewPane = createWidget<TFormWaveViewPane>(Vec(0, 0));
    viewPane->selectedBank = selectedBank;
    viewPane->onHide = [=]() {
        mainButtonRow->view();
    };
    viewPane->visible = false;
    addChild(viewPane);

    // Clone menu
    cloneMenu = createWidget<TFormCloneRow>(Vec(0, 0));
    cloneMenu->slotFilled = slotFilled;
    cloneMenu->sourceBank = selectedBank;
    cloneMenu->onHide = [=]() {
        mainButtonRow->view();
    };
    cloneMenu->visible = false;
    addChild(cloneMenu);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormEditorBankEditMenu::setSlotFilledFlag(int slot, bool isFilled) {
    mainButtonRow->setSlotFilledFlag(slot, isFilled);
    cloneMenu->setSlotFilledFlag(slot, isFilled);
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
    editMenu->loadButtonRow->onIngestTableCallback = onIngestTableCallback;
}

void TFormEditor::addClearBankCallback(const std::function<void(int)>&  onClearBankCallback) {
    editMenu->clearButtonRow->onClearBankCallback = onClearBankCallback;
}

void TFormEditor::addCloneBankCallback(const std::function<void(int, int)>& onCloneBankCallback) {
    editMenu->cloneMenu->onCloneBankCallback = onCloneBankCallback;
}

void TFormEditor::addGetBankCallback(const std::function<void(int, std::vector<std::vector<float>>&)>& onGetBankCallback) {
    editMenu->onGetBankCallback = onGetBankCallback;
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
