#include "TerrorformWaveTableEditor.hpp"
#include "osdialog.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Edit menu
////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorBankEditMenu::TFormEditorBankEditMenu() {
    box.size = Vec(238, 195);

    selectedBank = std::make_shared<int>(0);
    slotFilled = std::vector<bool>(TFORM_EDITOR_SLOTS, false);

    // Main button row
    mainMenu = createWidget<TFormEditMainMenu>(Vec(0, 0));
    mainMenu->selectedBank = selectedBank;
    mainMenu->loadButton->onClick = [=]() {
        if (onLoadWAVCallback) {
            if (dir.empty()) {
                dir = asset::user("");
            }

            auto wavPathSelected = [=](char* path) {
                if (path) {
                    dir = extractDirectoryFromFilePath(std::string(path));
                }
                std::shared_ptr<std::vector<float>> detectedWaves = onLoadWAVCallback(path);
                if (detectedWaves->size() > 0) {
                    loadMenu->detectedWaves = detectedWaves;
                    mainMenu->hide();
                    loadMenu->view();
                }
            };

#ifdef USING_CARDINAL_NOT_RACK
            async_dialog_filebrowser(false, dir.c_str(), "Load sample", [=](char* path) {
                wavPathSelected(path);
            });
#else
            const char FILE_FILTERS[] = "WAV File (.wav):wav";
            std::string filename;

            osdialog_filters* filters = osdialog_filters_parse(FILE_FILTERS);
            DEFER({
                osdialog_filters_free(filters);
            });

            char* path = osdialog_file(OSDIALOG_OPEN, dir.c_str(), filename.c_str(), filters);
            wavPathSelected(path);
#endif
        }
    };

    mainMenu->clearButton->onClick = [=]() {
        mainMenu->hide();
        clearMenu->view();
    };

    mainMenu->viewButton->onClick = [=]() {
        onGetBankCallback(*selectedBank, viewPane->bank);
        mainMenu->hide();
        viewPane->show();
    };

    mainMenu->cloneButton->onClick = [=]() {
        onGetBankCallback(*selectedBank, cloneMenu->bank);
        onGetBankCallback(*selectedBank, cloneMenu->sourcePage->bank);
        mainMenu->hide();
        cloneMenu->view();
    };

    mainMenu->moveButton->onClick = [=]() {
        mainMenu->hide();
        moveMenu->view();
    };

    addChild(mainMenu);

    // Load row
    loadMenu = createWidget<TFormLoadMenu>(Vec(0, 0));
    loadMenu->selectedBank = selectedBank;
    loadMenu->onHide = [=]() {
        mainMenu->show();
    };
    loadMenu->visible = false;
    addChild(loadMenu);

    // Clear row
    clearMenu = createWidget<TFormClearMenu>(Vec(0, 0));
    clearMenu->selectedBank = selectedBank;
    clearMenu->onExit = [=]() {
        mainMenu->show();
    };
    clearMenu->hide();
    addChild(clearMenu);

    // View row
    viewPane = createWidget<TFormWaveViewPane>(Vec(0, 0));
    viewPane->selectedBank = selectedBank;
    viewPane->onExit = [=]() {
        mainMenu->show();
    };
    viewPane->hide();
    addChild(viewPane);

    // Clone menu
    cloneMenu = createWidget<TFormCloneMenu>(Vec(0, 0));
    cloneMenu->sourceBank = selectedBank;
    cloneMenu->onExit = [=]() {
        mainMenu->show();
    };
    cloneMenu->visible = false;
    addChild(cloneMenu);

    // Mode menu
    moveMenu = createWidget<TFormMoveMenu>(Vec(0, 0));
    moveMenu->sourceBank = selectedBank;
    moveMenu->onExit = [=]() {
        mainMenu->show();
    };
    moveMenu->visible = false;
    addChild(moveMenu);
}

void TFormEditorBankEditMenu::setSlotFilledFlag(int slot, bool isFilled) {
    mainMenu->setSlotFilledFlag(slot, isFilled);
    cloneMenu->setSlotFilledFlag(slot, isFilled);
    moveMenu->setSlotFilledFlag(slot, isFilled);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Main menu
////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditorDefragMenu::TFormEditorDefragMenu() {
    box.size = Vec(238, 195);
    defragDoneMessage = createWidget<PlainText>(box.getCenter());
    defragDoneMessage->vertAlignment = NVG_ALIGN_MIDDLE;
    defragDoneMessage->box.size = box.size;
    defragDoneMessage->text = "Defragmentation Complete";
    addChild(defragDoneMessage);
    viewCounter = 0;
}

void TFormEditorDefragMenu::step() {
    if (visible) {
        --viewCounter;
    }
    if(viewCounter <= 0) {
        viewCounter = (int) std::ceil(APP->window->getLastFrameDuration());
        hide();
    }
    Widget::step();
}

TFormPurgeMenu::TFormPurgeMenu() {
    box.size = Vec(238, 195);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 100, buttonWidth, buttonHeight);
    yesButton->onClick = [=]() {
        for (int i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
            onClearBankCallback(i);
        }
        noButton->hide();
        yesButton->hide();
        questionText->hide();
        clearedText->show();
        float frameRate = 1.f / APP->window->getLastFrameDuration();
        counter = static_cast<int>(std::ceil(frameRate));
    };
    addChild(yesButton);

    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 100, buttonWidth, buttonHeight);
    noButton->onClick = [=]() {
        hide();
    };
    addChild(noButton);

    questionText = createWidget<PlainText>(Vec(119, 84));
    questionText->box.size.x = 238;
    questionText->size = 12;
    questionText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    questionText->text = "Really delete ALL banks?";
    addChild(questionText);

    clearedText = createWidget<PlainText>(Vec(119, 97.5));
    clearedText->box.size.x = 238;
    clearedText->size = 16;
    clearedText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    clearedText->vertAlignment = NVG_ALIGN_MIDDLE;
    clearedText->text = "Deleted ALL banks";
    clearedText->hide();
    addChild(clearedText);

    onView = [=]() {
        noButton->show();
        yesButton->show();
        questionText->show();
        clearedText->hide();
    };
}

void TFormPurgeMenu::step() {
    if (counter == 0 && clearedText->visible) {
        clearedText->hide();
        hide();
    }
    if (counter) {
        --counter;
    }
}

TFormEditorMainMenu::TFormEditorMainMenu() {
    box.size = Vec(238, 195);
    Vec buttonSize = Vec(92, 32);
    float gap = 8.f;
    float ySpacing = buttonSize.y + gap;
    float xSpacing = buttonSize.x + gap;
    float xOffset = 23.f;
    float yOffset = 41.f;
    float buttonFontSize = 16;

    title = createWidget<PlainText>(Vec(box.getCenter().x, 20.f));
    editButton = createWidget<TFormEditorButton>(Vec(xOffset, yOffset));
    defragButton = createWidget<TFormEditorButton>(Vec(xSpacing + xOffset, yOffset));
    importButton = createWidget<TFormEditorButton>(Vec(xOffset, ySpacing + yOffset));
    exportButton = createWidget<TFormEditorButton>(Vec(xSpacing + xOffset, ySpacing + yOffset));
    exitButton = createWidget<TFormEditorButton>(Vec(xOffset, ySpacing * 2 + yOffset));
    purgeButton = createWidget<TFormEditorButton>(Vec(xSpacing + xOffset, ySpacing * 2 + yOffset));

    title->box.size = box.size;
    editButton->box.size = buttonSize;
    defragButton->box.size = buttonSize;
    importButton->box.size = buttonSize;
    exportButton->box.size = buttonSize;
    exitButton->box.size = buttonSize;
    purgeButton->box.size = buttonSize;

    title->text = "User Wavetable Manager";
    editButton->text = "Edit";
    defragButton->text = "Defrag";
    importButton->text = "Load ROM";
    exportButton->text = "Save ROM";
    exitButton->text = "Exit";
    purgeButton->text = "Delete All";

    editButton->style.fontSize = buttonFontSize;
    defragButton->style.fontSize = buttonFontSize;
    importButton->style.fontSize = buttonFontSize;
    exportButton->style.fontSize = buttonFontSize;
    exitButton->style.fontSize = buttonFontSize;
    purgeButton->style.fontSize = buttonFontSize;

    editButton->setFilled(true);
    defragButton->setFilled(true);
    importButton->setFilled(true);
    exportButton->setFilled(true);
    exitButton->setFilled(true);
    purgeButton->setFilled(true);

    defragButton->onClick = [=]() {
        onDefragmentCallback();
        title->hide();
        editButton->hide();
        defragButton->hide();
        importButton->hide();
        exportButton->hide();
        exitButton->hide();
        purgeButton->hide();
        defragMenu->view();
        float frameRate = 1.f / APP->window->getLastFrameDuration();
        defragMenu->viewCounter = static_cast<int>(std::ceil(frameRate));
    };

    purgeButton->onClick = [=]() {
        title->hide();
        editButton->hide();
        defragButton->hide();
        importButton->hide();
        exportButton->hide();
        exitButton->hide();
        purgeButton->hide();
        purgeMenu->view();
    };

    defragMenu = createWidget<TFormEditorDefragMenu>(Vec(0, 0));
    defragMenu->onHide = [=]() {
        title->show();
        editButton->show();
        defragButton->show();
        importButton->show();
        exportButton->show();
        exitButton->show();
        purgeButton->show();
    };
    defragMenu->visible = false;

    purgeMenu = createWidget<TFormPurgeMenu>(Vec(0, 0));
    purgeMenu->onHide = [=]() {
        title->show();
        editButton->show();
        defragButton->show();
        importButton->show();
        exportButton->show();
        exitButton->show();
        purgeButton->show();
    };
    purgeMenu->visible = false;

    addChild(title);
    addChild(editButton);
    addChild(defragButton);
    addChild(importButton);
    addChild(exportButton);
    addChild(exitButton);
    addChild(purgeButton);
    addChild(defragMenu);
    addChild(purgeMenu);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormEditor::TFormEditor() {
    box.size = Vec(238, 195);

    auto gotoMainMenu = [=] {
        mainMenu->show();
        editMenu->hide();
    };

    auto gotoEditMenu = [=] {
        mainMenu->hide();
        editMenu->show();
    };

    // Menu buttons
    mainMenu = new TFormEditorMainMenu;
    mainMenu->box.pos.x = box.getCenter().x - mainMenu->box.getCenter().x;
    mainMenu->box.pos.y = box.getCenter().y - mainMenu->box.getCenter().y;
    mainMenu->editButton->onClick = gotoEditMenu;
    addChild(mainMenu);

    editMenu = createWidget<TFormEditorBankEditMenu>(Vec(0,0));
    editMenu->mainMenu->backButton->onClick = gotoMainMenu;
    addChild(editMenu);

    gotoMainMenu();
}

void TFormEditor::addOnExitCallback(const std::function<void()>& onExitCallback) {
    mainMenu->exitButton->onClick = onExitCallback;
}

void TFormEditor::addLoadWAVCallback(const std::function<std::shared_ptr<std::vector<float>>(char* path)>& onLoadWAVCallback) {
    editMenu->onLoadWAVCallback = onLoadWAVCallback;
}

void TFormEditor::addIngestTableCallback(const std::function<void(int, int, int, int, const std::string&)>& onIngestTableCallback) {
    editMenu->loadMenu->onIngestTableCallback = onIngestTableCallback;
}

void TFormEditor::addClearBankCallback(const std::function<void(int)>&  onClearBankCallback) {
    editMenu->clearMenu->onClearBankCallback = onClearBankCallback;
    mainMenu->purgeMenu->onClearBankCallback = onClearBankCallback;
}

void TFormEditor::addCloneBankCallback(const std::function<void(int, int, int, int)>& onCloneBankCallback) {
    editMenu->cloneMenu->onCloneBankCallback = onCloneBankCallback;
}

void TFormEditor::addMoveBankCallback(const std::function<void(int, int)>& onMoveBankCallback) {
    editMenu->moveMenu->addMoveBankCallback(onMoveBankCallback);
}

void TFormEditor::addGetBankCallback(const std::function<void(int, TerrorformWaveBank&)>& onGetBankCallback) {
    editMenu->onGetBankCallback = onGetBankCallback;
    editMenu->mainMenu->onGetBankCallback = onGetBankCallback;
    editMenu->viewPane->onGetBankCallback = onGetBankCallback;
}

void TFormEditor::addRenameBankCallback(const std::function<void(int, const std::string&)>& onRenameBankCallback) {
    editMenu->mainMenu->onRenameBankCallback = onRenameBankCallback;
    editMenu->viewPane->onRenameBankCallback = onRenameBankCallback;
}

void TFormEditor::addImportCallback(const std::function<void()>& onImportWaveTableCallback) {
    mainMenu->importButton->onClick = onImportWaveTableCallback;
}

void TFormEditor::addExportCallback(const std::function<void()>& onExportWaveTableCallback) {
    mainMenu->exportButton->onClick = onExportWaveTableCallback;
}

void TFormEditor::addDefragmentCallback(const std::function<void()>& onDefragmentCallback) {
    mainMenu->onDefragmentCallback = onDefragmentCallback;
}

void TFormEditor::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        editMenu->setSlotFilledFlag(slot, isFilled);
    }
}
