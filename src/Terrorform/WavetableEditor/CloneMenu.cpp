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

    startWaveFieldLabel = createWidget<PlainText>(Vec(5, 22));
    startWaveFieldLabel->box.size.x = buttonWidth;
    startWaveFieldLabel->color = nvgRGB(0xEF, 0xEF, 0xEF);
    startWaveFieldLabel->size = 12;
    startWaveFieldLabel->horzAlignment = NVG_ALIGN_LEFT;
    startWaveFieldLabel->text = "Start:";
    addChild(startWaveFieldLabel);

    endWaveFieldLabel = createWidget<PlainText>(Vec(65, 22));
    endWaveFieldLabel->box.size.x = buttonWidth;
    endWaveFieldLabel->color = nvgRGB(0xEF, 0xEF, 0xEF);
    endWaveFieldLabel->size = 12;
    endWaveFieldLabel->horzAlignment = NVG_ALIGN_LEFT;
    endWaveFieldLabel->text = "End:";
    addChild(endWaveFieldLabel);

    startWaveField = createWidget<TFormNumberField>(Vec(40, 21));
    startWaveField->box.size.x = 20;
    startWaveField->box.size.y = buttonHeight;
    startWaveField->setValue(1);
    startWaveField->onChangeCallback = [=]() {
        startWaveField->maximum = endWaveField->value;
        endWaveField->minimum = startWaveField->value;
        updateWaveDisplay();
    };
    addChild(startWaveField);

    endWaveField = createWidget<TFormNumberField>(Vec(89, 21));
    endWaveField->box.size.x = 20;
    endWaveField->box.size.y = buttonHeight;
    endWaveField->setValue(TFORM_MAX_NUM_WAVES);
    endWaveField->onChangeCallback = [=]() {
        startWaveField->maximum = endWaveField->value;
        endWaveField->minimum = startWaveField->value;
        updateWaveDisplay();
    };
    addChild(endWaveField);

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
        waveData[i].resize(TFORM_WAVELENGTH_CAP);
        for (int j = 0; j < TFORM_WAVELENGTH_CAP; ++j) {
            waveData[i][j] = 0.f;
        }
    }

    onView = [=] {
        startWaveField->setValue(1);
        endWaveField->setValue(bank.data.size());
        endWaveField->maximum = bank.data.size();
        updateWaveDisplay();
    };
}

void TFormCloneMenuSourcePage::step() {
    // for (int i = startWaveField->value - 1; i < endWaveField->value; ++i) {
    //     memcpy(&waveDisplay->waveData[j], bank.data[i].data(), sizeof(float) * TFORM_MAX_WAVELENGTH);
    //     ++j;
    // }
    // waveDisplay->numWaves = endWaveField->value - (startWaveField->value - 1);
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormCloneMenuSourcePage::draw(const DrawArgs& args) {
    std::shared_ptr<Font> font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    if(sourceBank && font) {
        std::string strCloningFrom = "Cloning from bank " + std::to_string(*sourceBank + 1);
        nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(args.vg, 5, 5, strCloningFrom.c_str(), NULL);
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

void TFormCloneMenuSourcePage::updateWaveDisplay() {
    if (bank.data.size() == 0) {
        return;
    }

    numWaves = endWaveField->value - (startWaveField->value - 1);
    numSamplesToCopy = numWaves * TFORM_WAVELENGTH_CAP;
    waveDisplay->waveData.clear();
    waveDisplay->waveData.assign(numSamplesToCopy, 0.f);

    int k = 0;
    int index = 0;
    for (int i = startWaveField->value - 1; i < endWaveField->value; ++i) {
        for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
            index = k * TFORM_WAVELENGTH_CAP + j;
            waveDisplay->waveData[index] =  bank.data[i][j];
        }
        ++k;
    }

    waveDisplay->setWaveCycleSize(TFORM_WAVELENGTH_CAP);
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
    filledText->visible = false;
    addChild(filledText);

    onView = [=]() {
        int row = *sourceBank / TFORM_EDITOR_ROWS;
        int col = *sourceBank % TFORM_EDITOR_COLS;
        grid->slotButton[row][col]->respondToMouse = false;
        grid->slotButton[row][col]->applyStyle(filledSlotStyle);
        *destBank = (*sourceBank + 1) % TFORM_EDITOR_SLOTS;
    };
}

void TFormCloneMenuDestPage::step() {
    int row = 0;
    int col = 0;
    if(sourceBank && destBank) {
        for(auto i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
            row = i / TFORM_EDITOR_ROWS;
            col = i % TFORM_EDITOR_COLS;
            grid->slotButton[row][col]->setHighlight((i == *sourceBank) || (i == *destBank));
        }
        sourceDestText->text = "Bank " + std::to_string(*sourceBank + 1) + " -> Bank " + std::to_string(*destBank + 1);
        if((*slotFilled)[*destBank]) {
            filledText->visible = true;
        }
        else {
            filledText->visible = false;
        }
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
        if ((*slotFilled)[slot] != isFilled) {
            (*slotFilled)[slot] = isFilled;
            grid->slotButton[row][col]->setFilled(isFilled);
            grid->slotButton[row][col]->applyStyle(isFilled ? filledSlotStyle : emptySlotStyle);
        }
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
            onCloneBankCallback(*sourceBank, *destBank, sourcePage->startWaveField->value - 1, sourcePage->endWaveField->value - 1);
            exit();
        }
    };

    destPage->destBank = destBank;
    addChild(destPage);

    overwriteMenu = createWidget<TFormQuestionMenu>(Vec(0, 0));
    overwriteMenu->taskCallback = [=]() {
        onCloneBankCallback(*sourceBank, *destBank, sourcePage->startWaveField->value - 1, sourcePage->endWaveField->value - 1);
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
