#include "LoadMenu.hpp"

TFormLoadMenu::TFormLoadMenu() {
    box.size = Vec(238, 195);
    startWaveChoice = createWidget<TFormEditorNumberChoice>(Vec(45, 20));
    startWaveChoice->range = 64;
    startWaveChoice->box.size.x = 28;
    startWaveChoice->box.size.y = 15;
    addChild(startWaveChoice);

    endWaveChoice = createWidget<TFormEditorNumberChoice>(Vec(108, 20));
    endWaveChoice->range = 64;
    endWaveChoice->box.size.x = 28;
    endWaveChoice->box.size.y = 15;
    addChild(endWaveChoice);

    ingestNewTable = [=](int startWave, int endWave) {
        onIngestTableCallback(*selectedBank, startWave, endWave);
        hide();
    };

    auto triggerIngest = [=]() {
        ingestNewTable(*startWaveChoice->choice, *endWaveChoice->choice);
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

void TFormLoadMenu::draw(const DrawArgs& args) {
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

void TFormLoadMenu::step() {
    if(detectedWaves) {
        for (unsigned long i = 0; i < (*detectedWaves).size(); ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[i][j] = 0.f;
            }
        }

        unsigned long k = 0;
        for (unsigned long i = *startWaveChoice->choice; i <= *endWaveChoice->choice; ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[k][j] = (*detectedWaves)[i][j];
            }
            ++k;
        }
    }

    waveDisplay->numWaves = *endWaveChoice->choice - *startWaveChoice->choice + 1;
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormLoadMenu::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}
