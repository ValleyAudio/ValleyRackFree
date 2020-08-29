#include "ViewPane.hpp"

TFormWaveViewPane::TFormWaveViewPane() {
    box.size = Vec(238, 195);

    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    backButton->setFilled(true);
    backButton->onClick = [=] {
        exit();
    };
    addChild(backButton);

    nameField = createWidget<TFormTextField>(Vec(buttonWidth + buttonOffset * 2, 3));
    nameField->box.size.x = buttonWidth * 2 + buttonOffset;
    nameField->box.size.y = buttonHeight;
    nameField->onDeselectCallback = [=](const std::string s) {
        if (onRenameBankCallback) {
            onRenameBankCallback(*selectedBank, s);
        }
    };
    addChild(nameField);

    waveDisplay = createWidget<TFormEditorWaveDisplay>(Vec(1.5f, 27.f));
    waveDisplay->box.size.x = box.size.x - 3.f;
    waveDisplay->box.size.y = box.size.y - 27.f;
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

    selectedBankText = createWidget<PlainText>(Vec(5, 21));
    selectedBankText->box.size.x = 238;
    selectedBankText->color = nvgRGB(0xEF, 0xEF, 0xEF);
    selectedBankText->horzAlignment = NVG_ALIGN_LEFT;
    selectedBankText->size = 12;
    addChild(selectedBankText);
}

void TFormWaveViewPane::draw(const DrawArgs& args) {
    std::string strSelectedBank = "Bank: ";
    nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);
    nvgFontSize(args.vg, 12);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgText(args.vg, 5, 5, strSelectedBank.c_str(), NULL);

    std::string bankNum = std::to_string(*selectedBank + 1);
    nvgTextAlign(args.vg, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
    nvgText(args.vg, 3 + buttonWidth, 5, bankNum.c_str(), NULL);

    // Horizontal bar
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 0, box.pos.y + 40);
    nvgLineTo(args.vg, box.size.x, box.pos.y + 40);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStrokeColor(args.vg, nvgRGB(0xAF, 0xAF, 0xAF));
    nvgStroke(args.vg);

    Widget::draw(args);
}

void TFormWaveViewPane::step() {
    selectedBankText->text = "Viewing wave " + std::to_string(selectedWave + 1) +
                             " of " + std::to_string(bank.data.size());
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

void TFormWaveViewPane::onShow(const event::Show& e) {
    if (onGetBankCallback) {
        onGetBankCallback(*selectedBank, bank);
        nameField->text = bank.name;
    }
    recurseEvent(&Widget::onShow, e);
}