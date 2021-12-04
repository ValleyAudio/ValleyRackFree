#include "LoadMenu.hpp"

TFormLoadMenu::TFormLoadMenu() {
    box.size = Vec(238, 195);

    ingestNewTable = [=](int startWave, int endWave) {
        onIngestTableCallback(*selectedBank, startWave, endWave, downSampleRatio, nameField->getText());
        hide();
    };

    auto triggerIngest = [=]() {
        ingestNewTable(startWaveField->value - 1, endWaveField->value - 1);
    };

    confirmButton = createNewMenuButton("Okay", triggerIngest, buttonWidth * 4 + buttonOffset * 5, 21, buttonWidth, buttonHeight);
    addChild(confirmButton);

    cancelButton = createNewMenuButton("Cancel", NULL, buttonWidth * 4 + buttonOffset * 5, 3, buttonWidth, buttonHeight);
    cancelButton->onClick = [=]() {
        hide();
    };
    addChild(cancelButton);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(1.5, 27.f));
    waveDisplay->box.size.x = box.size.x - 3.f;
    waveDisplay->box.size.y = box.size.y - 27.f;
    addChild(waveDisplay);
    waveSliderPos = 0.f;
    selectedWave = 0;
    maxDownSampleRatio = 1;
    downSampleRatio = maxDownSampleRatio;

    waveLineColor = nvgRGB(0xAF, 0xAF, 0xAF);
    waveFillColor = nvgRGBA(0xAF, 0xAF, 0xAF, 0x6F);

    nameFieldLabel = createWidget<PlainText>(Vec(5, 5));
    nameFieldLabel->box.size.x = buttonWidth;
    nameFieldLabel->color = nvgRGB(0xEF, 0xEF, 0xEF);
    nameFieldLabel->size = 12;
    nameFieldLabel->horzAlignment = NVG_ALIGN_LEFT;
    nameFieldLabel->text = "Name:";
    addChild(nameFieldLabel);

    nameField = createWidget<TFormTextField>(Vec(40, 3));
    nameField->box.size.x = 69;
    nameField->box.size.y = buttonHeight;
    addChild(nameField);

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

    cycleSizeLabel = createWidget<PlainText>(Vec(113, 22));
    cycleSizeLabel->box.size.x = buttonWidth;
    cycleSizeLabel->color = nvgRGB(0xEF, 0xEF, 0xEF);
    cycleSizeLabel->size = 12;
    cycleSizeLabel->horzAlignment = NVG_ALIGN_LEFT;
    cycleSizeLabel->text = "Cycle:";
    addChild(cycleSizeLabel);

    startWaveField = createWidget<TFormNumberField>(Vec(40, 21));
    startWaveField->box.size.x = 20;
    startWaveField->box.size.y = buttonHeight;
    startWaveField->setValue(1);
    startWaveField->onChangeCallback = [=]() {
        startWaveField->maximum = endWaveField->value;
        endWaveField->minimum = startWaveField->value;
        selectWaves();
    };
    addChild(startWaveField);

    endWaveField = createWidget<TFormNumberField>(Vec(89, 21));
    endWaveField->box.size.x = 20;
    endWaveField->box.size.y = buttonHeight;
    endWaveField->setValue(TFORM_MAX_NUM_WAVES);
    endWaveField->onChangeCallback = [=]() {
        startWaveField->maximum = endWaveField->value;
        endWaveField->minimum = startWaveField->value;
        selectWaves();
    };
    addChild(endWaveField);

    cycleSizeMenu = createWidget<TFormEditorChoice>(Vec(148, 21));
    cycleSizeMenu->box.size.x = 40;
    cycleSizeMenu->box.size.y = buttonHeight;
    cycleSizeMenu->items.push_back("256");
    cycleSizeMenu->items.push_back("512");
    cycleSizeMenu->items.push_back("1024");
    cycleSizeMenu->items.push_back("2048");
    cycleSizeMenu->maxItems = 4;
    cycleSizeMenu->onChangeCallback = [=]() {
        calcWaveLength();
    };
    addChild(cycleSizeMenu);

    maxWaves = 0;
    waveDisplayNeedsUpdate = true;

    onView = [=]() {
        maxWaves = detectedWaves->size() / TFORM_WAVELENGTH_CAP;
        maxWaves = maxWaves > TFORM_MAX_NUM_WAVES ? TFORM_MAX_NUM_WAVES : maxWaves;
        startWaveField->setMaximum(maxWaves);
        endWaveField->setMaximum(maxWaves);
        endWaveField->setValue(maxWaves);
        *cycleSizeMenu->choice = 0;

        maxDownSampleRatio = detectedWaves->size() / 256;
        maxDownSampleRatio = maxDownSampleRatio > 8 ? 8 : maxDownSampleRatio;

        if (maxDownSampleRatio >= 8) {
            cycleSizeMenu->maxItems = 4;
        }
        else if (maxDownSampleRatio >= 4) {
            cycleSizeMenu->maxItems = 3;
        }
        else if (maxDownSampleRatio >= 2) {
            cycleSizeMenu->maxItems = 2;
        }
        else {
            cycleSizeMenu->maxItems = 1;
        }

        startWaveField->setValue(1);
        nameField->text = "Bank_" + std::to_string(*selectedBank + 1);
        calcWaveLength();
        selectWaves();
        updateWaveDisplay();
    };
}

void TFormLoadMenu::draw(const DrawArgs& args) {
    std::string strDetectedWaves = "Found " + std::to_string(maxWaves) + " waves";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));

    std::shared_ptr<Font> font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    if (font) {
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
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

void TFormLoadMenu::step() {
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormLoadMenu::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}

void TFormLoadMenu::calcWaveLength() {
    waveLength = TFORM_WAVELENGTH_CAP;
    switch (cycleSizeMenu->getChoice()) {
        case 0 :
            downSampleRatio = 1;
            waveLength = 256;
            break;
        case 1 :
            downSampleRatio = 2;
            waveLength = 512;
            break;
        case 2 :
            downSampleRatio = 4;
            waveLength = 1024;
            break;
        case 3 :
            downSampleRatio = 8;
            waveLength = 2048;
            break;
    }

    maxWaves = detectedWaves->size() / waveLength;
    maxWaves = maxWaves > TFORM_MAX_NUM_WAVES ? TFORM_MAX_NUM_WAVES : maxWaves;
    startWaveField->minimum = 1;
    startWaveField->maximum = maxWaves;
    startWaveField->setValue(1);

    endWaveField->minimum = 1;
    endWaveField->maximum = maxWaves;
    endWaveField->setValue(maxWaves);

    selectWaves();
}

void TFormLoadMenu::selectWaves() {
    if (detectedWaves == nullptr) {
        return;
    }

    numWaves = endWaveField->value - (startWaveField->value - 1);
    startOffset = (startWaveField->value - 1) * waveLength;
    numSamplesToCopy = numWaves * waveLength;

    updateWaveDisplay();
}

void TFormLoadMenu::updateWaveDisplay() {
    if (detectedWaves == nullptr) {
        return;
    }

    waveDisplay->waveData.clear();
    waveDisplay->waveData.assign(numSamplesToCopy, 0.f);

    for (size_t i = 0; i < numSamplesToCopy; ++i) {
        waveDisplay->waveData[i] = (*detectedWaves)[i + startOffset];
    }
    waveDisplay->setWaveCycleSize(waveLength);
}
