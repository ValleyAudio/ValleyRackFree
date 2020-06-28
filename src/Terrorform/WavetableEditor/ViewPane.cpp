#include "ViewPane.hpp"

TFormWaveViewPane::TFormWaveViewPane() {
    box.size = Vec(238, 195);

    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    backButton->setFilled(true);
    backButton->onClick = [=] {
        exit();
    };
    addChild(backButton);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(1.5,8));
    waveDisplay->box.size.x = box.size.x - 3.f;
    waveDisplay->box.size.y = box.size.y - 8.f;
    addChild(waveDisplay);

    waveLineColor = nvgRGB(0xCF, 0x00, 0x00);
    waveFillColor = nvgRGBA(0xCF, 0x00, 0x00, 0x4F);

    waveData.resize(TFORM_MAX_NUM_WAVES);
    for (int i = 0; i < TFORM_MAX_NUM_WAVES; ++i) {
        waveData[i].resize(TFORM_MAX_WAVELENGTH);
        for (int j = 0; j < TFORM_MAX_WAVELENGTH; ++j) {
            waveData[i][j] = 0.f;
        }
    }

    selectedBankText = createWidget<PlainText>(Vec(5, 5));
    selectedBankText->box.size.x = 238;
    selectedBankText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    selectedBankText->horzAlignment = NVG_ALIGN_LEFT;
    selectedBankText->size = 12;
    addChild(selectedBankText);
}

void TFormWaveViewPane::draw(const DrawArgs& args) {
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
    selectedBankText->text = "Viewing wave " + std::to_string(selectedWave + 1) + " in bank " + std::to_string(*selectedBank + 1);
    waveDisplay->numWaves = bank.data.size();
    for (int i = 0; i < bank.data.size(); ++i) {
        memcpy(&waveDisplay->waveData[i], bank.data[i].data(), sizeof(float) * TFORM_MAX_WAVELENGTH);
    }
    selectedWave = waveDisplay->selectedWave;
    Widget::step();
}

void TFormWaveViewPane::onDragMove(const event::DragMove& e) {
    waveDisplay->moveSliderPos(e.mouseDelta.y);
}
