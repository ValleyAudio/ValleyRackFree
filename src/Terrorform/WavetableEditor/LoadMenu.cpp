#include "LoadMenu.hpp"

TFormLoadMenu::TFormLoadMenu() {
    box.size = Vec(238, 195);

    ingestNewTable = [=](int startWave, int endWave) {
        onIngestTableCallback(*selectedBank, startWave, endWave, nameField->getText());
        hide();
    };

    auto triggerIngest = [=]() {
        ingestNewTable(startWaveField->value - 1, endWaveField->value - 1);
    };

    cancelButton = createNewMenuButton("Cancel", NULL, buttonWidth * 4 + buttonOffset * 5, 3, buttonWidth, buttonHeight);
    cancelButton->onClick = [=]() {
        hide();
    };
    addChild(cancelButton);
    confirmButton = createNewMenuButton("Okay", triggerIngest, buttonWidth * 4 + buttonOffset * 5, 21, buttonWidth, buttonHeight);
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

    startWaveField = createWidget<TFormNumberField>(Vec(40, 21));
    startWaveField->box.size.x = 20;
    startWaveField->box.size.y = buttonHeight;
    startWaveField->setValue(1);
    startWaveField->onChangeCallback = [=]() {
        startWaveField->maximum = endWaveField->value;
        endWaveField->minimum = startWaveField->value;
    };
    addChild(startWaveField);

    endWaveField = createWidget<TFormNumberField>(Vec(89, 21));
    endWaveField->box.size.x = 20;
    endWaveField->box.size.y = buttonHeight;
    endWaveField->setValue(64);
    endWaveField->onChangeCallback = [=]() {
        startWaveField->maximum = endWaveField->value;
        endWaveField->minimum = startWaveField->value;
    };
    addChild(endWaveField);

    onView = [=]() {
        startWaveField->setValue(1);
        nameField->text = "EMPTY_" + std::to_string(*selectedBank);
    };
}

void TFormLoadMenu::draw(const DrawArgs& args) {
    std::string strDetectedWaves = "Found " + std::to_string(detectedWaves->size()) + " waves";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);

    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    //nvgText(args.vg, 5, 5, strDetectedWaves.c_str(), NULL);

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
    if(detectedWaves) {
        for (unsigned long i = 0; i < (*detectedWaves).size(); ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[i][j] = 0.f;
            }
        }

        unsigned long k = 0;
        //for (unsigned long i = *startWaveChoice->choice; i <= *endWaveChoice->choice; ++i) {
        for (unsigned long i = startWaveField->value - 1; i < endWaveField->value; ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[k][j] = (*detectedWaves)[i][j];
            }
            ++k;
        }
    }

    waveDisplay->numWaves = endWaveField->value - (startWaveField->value - 1);
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormLoadMenu::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}
