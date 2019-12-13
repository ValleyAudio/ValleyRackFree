#include "WaveLoader.hpp"

TFormBankEditLoadRow::TFormBankEditLoadRow() {
    box.size = Vec(238, 195);
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

    auto triggerIngest = [=]() {
        ingestNewTable(*startWave->choice, *endWave->choice);
    };

    cancelButton = createNewMenuButton("Cancel", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    addChild(cancelButton);
    confirmButton = createNewMenuButton("Okay", triggerIngest, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(confirmButton);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(10,27));
    waveDisplay->box.size.x = box.size.x - 20.f;
    waveDisplay->box.size.y = box.size.y - 55.f;
    addChild(waveDisplay);
    waveSliderPos = 0.f;
    selectedWave = 0;

    waveLineColor = nvgRGB(0xAF, 0xAF, 0xAF);
    waveFillColor = nvgRGBA(0xAF, 0xAF, 0xAF, 0x6F);

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

void TFormBankEditLoadRow::draw(const DrawArgs& args) {
    std::string strDetectedWaves = "Found " + std::to_string(detectedWaves->size()) + " waves";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);

    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strDetectedWaves.c_str(), NULL);
    nvgText(args.vg, 5, 21, "Start:", NULL);
    nvgText(args.vg, 80, 21, "End:", NULL);

    float startX = 3;
    float endX = box.size.x - 3;
    float xScale = (endX - startX) / 255.f;
    float yOffset =  box.size.y - 18;
    float yScale = 15.f;

    float boxWidth = box.size.x - 6;
    float boxHeight = 30.f;
    float boxX = 3;
    float boxY = box.size.y - boxHeight - 3.f;

    // Waveform fill
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, startX, yOffset);
    for (int i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
        nvgLineTo(args.vg, startX + (float) i * xScale, -waveDisplay->waveData[selectedWave][i] * yScale + yOffset);
    }
    nvgLineTo(args.vg, endX, yOffset);
    nvgFillColor(args.vg, waveFillColor);
    nvgFill(args.vg);
    nvgClosePath(args.vg);

    // Waveform stroke
    nvgBeginPath(args.vg);
    nvgLineCap(args.vg, NVG_ROUND);
    nvgLineJoin(args.vg, NVG_ROUND);
    nvgMoveTo(args.vg, startX, yOffset);
    for (int i = 0; i < TFORM_MAX_WAVELENGTH; ++i) {
        nvgLineTo(args.vg, startX + (float) i * xScale, -waveDisplay->waveData[selectedWave][i] * yScale + yOffset);
    }
    nvgLineTo(args.vg, endX, yOffset);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, waveLineColor);
    nvgStroke(args.vg);
    nvgLineCap(args.vg, NVG_BUTT);
    nvgLineJoin(args.vg, NVG_MITER);

    // Waveform box outline
    nvgBeginPath(args.vg);
    nvgRect(args.vg, boxX, boxY, boxWidth, boxHeight);
    nvgStrokeColor(args.vg, nvgRGBA(0xAF, 0xAF, 0xAF, 0x6F));
    nvgStroke(args.vg);

    // Horizontal bar
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 0, box.pos.y + 40);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 40);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);

    Widget::draw(args);
}

void TFormBankEditLoadRow::step() {
    if(detectedWaves) {
        for (unsigned long i = 0; i < (*detectedWaves).size(); ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[i][j] = 0.f;
            }
        }

        unsigned long k = 0;
        for (unsigned long i = *startWave->choice; i <= *endWave->choice; ++i) {
            for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
                waveDisplay->waveData[k][j] = (*detectedWaves)[i][j];
            }
            ++k;
        }
    }

    waveDisplay->numWaves = *endWave->choice - *startWave->choice + 1;
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormBankEditLoadRow::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}
