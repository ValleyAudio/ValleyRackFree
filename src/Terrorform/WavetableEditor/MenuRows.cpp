#include "MenuRows.hpp"

TFormMenuRow::TFormMenuRow() {
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
}

TFormBankEditMainRow::TFormBankEditMainRow() {
    box.size = Vec(238, 40);
    loadButton = createNewMenuButton("Load", NULL, buttonOffset, 21, buttonWidth, buttonHeight);
    addChild(loadButton);
    copyButton = createNewMenuButton("Copy", NULL, buttonWidth + buttonOffset * 2, 21, buttonWidth, buttonHeight);
    addChild(copyButton);
    pasteButton = createNewMenuButton("Paste", NULL, buttonWidth * 2 + buttonOffset * 3, 21, buttonWidth, buttonHeight);
    addChild(pasteButton);
    clearButton = createNewMenuButton("Clear", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    addChild(clearButton);
    purgeButton = createNewMenuButton("Purge", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(purgeButton);
    viewButton = createNewMenuButton("View", NULL, buttonWidth * 3 + buttonOffset * 4, 3, buttonWidth, buttonHeight);
    addChild(viewButton);
    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    addChild(backButton);
    selectedBank = 0;
    selectedBankIsFilled = false;
}

void TFormBankEditMainRow::draw(const DrawArgs& args) {
    std::string strSelectedBank = "Bank: " + std::to_string(selectedBank + 1);
    std::string strFilled = "Filled";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strSelectedBank.c_str(), NULL);

    if(selectedBankIsFilled) {
        nvgFillColor(args.vg, nvgRGB(0xFF, 0x00, 0x00));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);
        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(args.vg, 55, 5, strFilled.c_str(), NULL);
    }

    Widget::draw(args);
}

TFormClearRow::TFormClearRow() {
    box.size = Vec(238, 40);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 21, buttonWidth, buttonHeight);
    addChild(yesButton);
    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 21, buttonWidth, buttonHeight);
    addChild(noButton);
}

void TFormClearRow::draw(const DrawArgs& args) {
    std::string strQuestion = "Do you really want to clear bank " + std::to_string(selectedBank + 1) + "?";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgText(args.vg, box.size.x / 2, 5, strQuestion.c_str(), NULL);
    Widget::draw(args);
}

TFormBankEditPurgeRow::TFormBankEditPurgeRow() {
    box.size = Vec(238, 40);
    yesButton = createNewMenuButton("Yes", NULL, box.size.x / 2 - buttonWidth - 1.5, 21, buttonWidth, buttonHeight);
    addChild(yesButton);
    noButton = createNewMenuButton("No", NULL, box.size.x / 2 + 1.5, 21, buttonWidth, buttonHeight);
    addChild(noButton);
}

void TFormBankEditPurgeRow::draw(const DrawArgs& args) {
    std::string strQuestion = "Do you really want to purge ALL banks?";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgText(args.vg, box.size.x / 2, 5, strQuestion.c_str(), NULL);
    Widget::draw(args);
}

TFormBankEditCopyRow::TFormBankEditCopyRow() {

}

void TFormBankEditCopyRow::draw(const DrawArgs& args) {
    Widget::draw(args);
};

TFormWaveViewPane::TFormWaveViewPane() {
    box.size = Vec(238, 195);

    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    addChild(backButton);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(10,8));
    waveDisplay->box.size.x = box.size.x - 20.f;
    waveDisplay->box.size.y = box.size.y - 40.f;
    addChild(waveDisplay);

    waveLineColor = nvgRGB(0xCF, 0x00, 0x00);
    waveFillColor = nvgRGBA(0xCF, 0x00, 0x00, 0x4F);

    for (int i = 0; i < TFORM_MAX_NUM_WAVES; ++i) {
        for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
            waveData[i][j] = 0.f;
        }
    }
}

void TFormWaveViewPane::draw(const DrawArgs& args) {
    std::string strText = "Viewing wave " + std::to_string(selectedWave) + " 64 of bank " + std::to_string(selectedBank);
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strText.c_str(), NULL);

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
        nvgLineTo(args.vg, startX + (float) i * xScale, -waveData[selectedWave][i] * yScale + yOffset);
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
        nvgLineTo(args.vg, startX + (float) i * xScale, -waveData[selectedWave][i] * yScale + yOffset);
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
    nvgMoveTo(args.vg, 0, box.pos.y + 20);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 20);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);

    Widget::draw(args);
}

void TFormWaveViewPane::step() {
    for (int i = 0; i < TFORM_MAX_NUM_WAVES; ++i) {
        memcpy(&waveDisplay->waveData[i], &waveData[i], sizeof(float) * TFORM_MAX_WAVELENGTH);
    }
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormWaveViewPane::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}
