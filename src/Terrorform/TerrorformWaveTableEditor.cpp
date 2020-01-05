#include "TerrorformWaveTableEditor.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Edit menu
////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorBankEditMenu::TFormEditorBankEditMenu() {
    box.size = Vec(238, 195);

    selectedBank = std::make_shared<int>(0);

    // Main button row
    mainButtonRow = createWidget<TFormEditMainMenu>(Vec(0, 0));
    mainButtonRow->selectedBank = selectedBank;
    mainButtonRow->loadButton->onClick = [=]() {
        if (onLoadWAVCallback) {
            std::shared_ptr<std::vector<std::vector<float>>> detectedWaves = onLoadWAVCallback();
            if (detectedWaves->size() > 0) {
                *loadButtonRow->endWaveChoice->choice = detectedWaves->size() - 1;
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
        onGetBankCallback(*selectedBank, cloneMenu->sourcePage->waveData);
        mainButtonRow->hide();
        cloneMenu->view();
    };

    addChild(mainButtonRow);

    // Load row
    loadButtonRow = createWidget<TFormLoadMenu>(Vec(0, 0));
    loadButtonRow->selectedBank = selectedBank;
    loadButtonRow->onHide = [=]() {
        mainButtonRow->view();
    };
    loadButtonRow->visible = false;
    addChild(loadButtonRow);

    // Clear row
    clearButtonRow = createWidget<TFormClearMenu>(Vec(0, 0));
    clearButtonRow->selectedBank = selectedBank;
    clearButtonRow->onExit = [=]() {
        mainButtonRow->view();
    };
    clearButtonRow->hide();
    addChild(clearButtonRow);

    // Purge row
    purgeButtonRow = createWidget<TFormPurgeMenu>(Vec(0, 0));
    purgeButtonRow->onExit = [=]() {
        mainButtonRow->view();
    };
    purgeButtonRow->hide();
    addChild(purgeButtonRow);

    // View row
    viewPane = createWidget<TFormWaveViewPane>(Vec(0, 0));
    viewPane->selectedBank = selectedBank;
    viewPane->onExit = [=]() {
        mainButtonRow->view();
    };
    viewPane->hide();
    viewPane->visible = false;
    addChild(viewPane);

    // Clone menu
    cloneMenu = createWidget<TFormCloneMenu>(Vec(0, 0));
    cloneMenu->sourceBank = selectedBank;
    cloneMenu->onExit = [=]() {
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
    Vec buttonSize = Vec(82, 32);
    float gap = 8.f;
    float ySpacing = buttonSize.y + gap;
    float xSpacing = buttonSize.x + gap;
    float yOffset = 20.f;
    float buttonFontSize = 18;
    box.size.x = buttonSize.x * 2 + gap;
    box.size.y = buttonSize.y * 3 + (gap + yOffset) * 2;

    title = createWidget<PlainText>(Vec(box.getCenter().x, 0));
    defragDoneMessage = createWidget<PlainText>(box.getCenter());
    editButton = createWidget<TFormEditorButton>(Vec(0, yOffset));
    defragButton = createWidget<TFormEditorButton>(Vec(xSpacing, yOffset));
    importButton = createWidget<TFormEditorButton>(Vec(0, ySpacing + yOffset));
    exportButton = createWidget<TFormEditorButton>(Vec(xSpacing, ySpacing + yOffset));
    exitButton = createWidget<TFormEditorButton>(Vec(0, ySpacing * 2 + yOffset));
    purgeButton = createWidget<TFormEditorButton>(Vec(xSpacing, ySpacing * 2 + yOffset));

    title->box.size = box.size;
    defragDoneMessage->box.size = box.size;
    editButton->box.size = buttonSize;
    defragButton->box.size = buttonSize;
    importButton->box.size = buttonSize;
    exportButton->box.size = buttonSize;
    exitButton->box.size = buttonSize;
    purgeButton->box.size = buttonSize;

    title->text = "User Wavetable Manager";
    defragDoneMessage->text = "Defragmentation Complete";
    editButton->text = "Edit";
    defragButton->text = "Defrag";
    importButton->text = "Load ROM";
    exportButton->text = "Save ROM";
    exitButton->text = "Exit";
    purgeButton->text = "Purge";

    editButton->fontSize = buttonFontSize;
    defragButton->fontSize = buttonFontSize;
    importButton->fontSize = buttonFontSize;
    exportButton->fontSize = buttonFontSize;
    exitButton->fontSize = buttonFontSize;
    purgeButton->fontSize = buttonFontSize;

    purgeButton->onClick = [=]() {
        if(onClearBankCallback) {
            for (int i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
                onClearBankCallback(i);
            }
        }
    };

    defragDoneMessage->visible = false;

    addChild(title);
    addChild(defragDoneMessage);
    addChild(editButton);
    addChild(defragButton);
    addChild(importButton);
    addChild(exportButton);
    addChild(exitButton);
    addChild(purgeButton);
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

    editMenu = createWidget<TFormEditorBankEditMenu>(Vec(0,0));
    editMenu->mainButtonRow->backButton->onClick = gotoMainMenu;
    addChild(editMenu);

    gotoMainMenu();
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
    // editMenu->purgeButtonRow->onClearBankCallback = onClearBankCallback;
    mainMenu->onClearBankCallback = onClearBankCallback;
}

void TFormEditor::addCloneBankCallback(const std::function<void(int, int)>& onCloneBankCallback) {
    editMenu->cloneMenu->addCloneBankCallback(onCloneBankCallback);
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

void TFormEditor::addDefragmentCallback(const std::function<void()>& onDefragmentCallback) {
    mainMenu->defragButton->onClick = onDefragmentCallback;
}

void TFormEditor::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        editMenu->setSlotFilledFlag(slot, isFilled);
    }
}
