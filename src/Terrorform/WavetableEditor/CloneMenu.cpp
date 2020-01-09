#include "CloneMenu.hpp"

TFormCloneMenuSourcePage::TFormCloneMenuSourcePage() {
    box.size = Vec(238, 195);
    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    backButton->onClick = [=]() {
        exit();
    };
    addChild(backButton);

    nextButton = createNewMenuButton("Next", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(nextButton);

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

    onView = [=] {
        *startWave->choice = 0;
        *endWave->choice = waveData.size() - 1;
    };
}

void TFormCloneMenuSourcePage::step() {
    int j = 0;
    for (unsigned long i = *startWave->choice; i <= *endWave->choice; ++i) {
        memcpy(&waveDisplay->waveData[j], waveData[i].data(), sizeof(float) * TFORM_MAX_WAVELENGTH);
        ++j;
    }
    waveDisplay->numWaves = *endWave->choice - *startWave->choice + 1;
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormCloneMenuSourcePage::draw(const DrawArgs& args) {
    if(sourceBank) {
        std::string strCloningFrom = "Cloning from bank " + std::to_string(*sourceBank + 1);
        nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(args.vg, 5, 5, strCloningFrom.c_str(), NULL);
        nvgText(args.vg, 5, 21, "Start:", NULL);
        nvgText(args.vg, 80, 21, "End:", NULL);
    }
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 0, box.pos.y + 40);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 40);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);
    Widget::draw(args);
}

void TFormCloneMenuSourcePage::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormCloneMenuDestPage::TFormCloneMenuDestPage() {
    box.size = Vec(238, 195);

    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    addChild(backButton);

    okayButton = createNewMenuButton("Okay", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(okayButton);

    filledSlotStyle.idleColors =           TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                                   0x9F, 0x00, 0x00, 0xFF,
                                                                   0x00, 0x00, 0x00, 0x00);
    filledSlotStyle.hoverColors =          TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                                   0xBF, 0x00, 0x00, 0xFF,
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
                *destBank = id;
                if((*slotFilled)[id]) {
                    filledText->visible = true;
                }
                else {
                    filledText->visible = false;
                }
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

    onView = [=]() {
        int row = *sourceBank;
        int col = *sourceBank;
        grid->slotButton[row][col]->respondToMouse = false;
        grid->slotButton[row][col]->applyStyle(filledSlotStyle);
        grid->slotButton[row][col]->setHighlight(true);
    };
}

void TFormCloneMenuDestPage::step() {
    int row = 0;
    int col = 0;
    if(sourceBank && destBank) {
        for(auto i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
            row = i / TFORM_EDITOR_ROWS;
            col = i % TFORM_EDITOR_COLS;
            grid->slotButton[row][col]->setHighlight(i == *destBank);
            //grid->slotButton[row][col]->highlight = i == *sourceBank || i == *destBank;
            //grid->slotButton[row][col]->respondToMouse = i != *sourceBank;
        }
        sourceDestText->text = "Bank " + std::to_string(*sourceBank + 1) + " -> Bank " + std::to_string(*destBank + 1);
    }
    Widget::step();
}

void TFormCloneMenuDestPage::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 0, box.pos.y + 40);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 40);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);
    Widget::draw(args);
}

void TFormCloneMenuDestPage::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        int row = slot / TFORM_EDITOR_ROWS;
        int col = slot % TFORM_EDITOR_COLS;
        (*slotFilled)[slot] = isFilled;
        grid->slotButton[row][col]->applyStyle(isFilled ? filledSlotStyle : emptySlotStyle);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormCloneMenu::TFormCloneMenu() {
    box.size = Vec(238, 195);

    slotFilled = std::make_shared<std::vector<bool>>(TFORM_EDITOR_SLOTS, false);
    destBank = std::make_shared<int>(0);

    cloneDoneText = createWidget<PlainText>(Vec(119, 97.5));
    cloneDoneText->box.size.x = 238;
    cloneDoneText->size = 16;
    cloneDoneText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    cloneDoneText->vertAlignment = NVG_ALIGN_MIDDLE;
    cloneDoneText->visible = false;
    addChild(cloneDoneText);

    sourcePage = createWidget<TFormCloneMenuSourcePage>(Vec(0,0));
    sourcePage->onExit = [=]() {
        exit();
    };

    sourcePage->nextButton->onClick = [=]() {
        sourcePage->hide();
        destPage->view();
    };

    sourcePage->sourceBank = sourceBank;
    addChild(sourcePage);

    destPage = createWidget<TFormCloneMenuDestPage>(Vec(0,0));
    destPage->slotFilled = slotFilled;
    destPage->visible = false;
    destPage->backButton->onClick = [=]() {
        destPage->hide();
        sourcePage->view();
    };

    destPage->okayButton->onClick = [=]() {
        destPage->hide();
        if ((*slotFilled)[*destBank]) {
            overwriteMenu->questionText->text = "Overwrite bank " + std::to_string(*destBank + 1) + "?";
            overwriteMenu->view();
        }
        else {
            onCloneBankCallback(*sourceBank, *destBank);
            exit();
        }
    };

    destPage->destBank = destBank;
    addChild(destPage);

    overwriteMenu = createWidget<TFormQuestionMenu>(Vec(0, 0));
    overwriteMenu->taskCallback = [=]() {
        onCloneBankCallback(*sourceBank, *destBank);
    };

    overwriteMenu->onExit = [=]() {
        exit();
    };

    overwriteMenu->visible = false;
    addChild(overwriteMenu);

    onView = [=]() {
        sourcePage->sourceBank = sourceBank;
        destPage->sourceBank = sourceBank;
        destPage->destBank = destBank;
        sourcePage->view();
        destPage->hide();
    };
}

void TFormCloneMenu::setSlotFilledFlag(int slot, bool isFilled) {
    destPage->setSlotFilledFlag(slot, isFilled);
}
