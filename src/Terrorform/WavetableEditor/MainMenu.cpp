#include "MainMenu.hpp"

TFormEditMainMenu::TFormEditMainMenu() {
    box.size = Vec(238, 195);
    loadButton = createNewMenuButton("Load", NULL, buttonOffset, 21, buttonWidth, buttonHeight);
    loadButton->setFilled(true);
    addChild(loadButton);
    clearButton = createNewMenuButton("Delete", NULL, buttonWidth + buttonOffset * 2, 21, buttonWidth, buttonHeight);
    clearButton->setFilled(true);
    addChild(clearButton);
    viewButton = createNewMenuButton("View", NULL, buttonWidth * 2 + buttonOffset * 3, 21, buttonWidth, buttonHeight);
    viewButton->setFilled(true);
    addChild(viewButton);
    cloneButton = createNewMenuButton("Clone", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    cloneButton->setFilled(true);
    addChild(cloneButton);
    moveButton = createNewMenuButton("Move", NULL, buttonWidth * 4 + buttonOffset * 5, 21, buttonWidth, buttonHeight);
    moveButton->setFilled(true);
    addChild(moveButton);
    backButton = createNewMenuButton("Back", NULL, buttonWidth * 4 + buttonOffset * 5, 3, buttonWidth, buttonHeight);
    backButton->setFilled(true);
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

    // Slots grid

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
            auto selectBank = [=]() {
                *selectedBank = id;
                if (onGetBankCallback) {
                    onGetBankCallback(*selectedBank, bank);
                    nameField->text = bank.name;
                }
            };
            grid->slotButton[row][col]->onClick = selectBank;
            grid->slotButton[row][col]->text = std::to_string(id + 1);
            ++id;
        }
    }
    addChild(grid);

    slotFilled = std::vector<bool>(TFORM_EDITOR_SLOTS, false);
}

void TFormEditMainMenu::step() {
    selectedBankIsFilled = slotFilled[*selectedBank];
    viewButton->setEnable(selectedBankIsFilled);
    clearButton->setEnable(selectedBankIsFilled);
    cloneButton->setEnable(selectedBankIsFilled);
    moveButton->setEnable(selectedBankIsFilled);
    nameField->visible = selectedBankIsFilled;

    int row = 0;
    int col = 0;
    for(int i = 0; i < TFORM_EDITOR_SLOTS; ++i) {
        row = i / TFORM_EDITOR_ROWS;
        col = i % TFORM_EDITOR_COLS;
        grid->slotButton[row][col]->setHighlight(i == *selectedBank);
    }
    Widget::step();
}

void TFormEditMainMenu::draw(const DrawArgs& args) {
    std::shared_ptr<Font> font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    std::string strSelectedBank = "Bank: ";
    if (font) {
        nvgFillColor(args.vg, nvgRGB(0xEF, 0xEF, 0xEF));
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);
        nvgFontSize(args.vg, 12);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(args.vg, 5, 5, strSelectedBank.c_str(), NULL);

        std::string bankNum = std::to_string(*selectedBank + 1);
        nvgTextAlign(args.vg, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
        nvgText(args.vg, 3 + buttonWidth, 5, bankNum.c_str(), NULL);
    }

    Widget::draw(args);
}

void TFormEditMainMenu::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        int row = slot / TFORM_EDITOR_ROWS;
        int col = slot % TFORM_EDITOR_COLS;
        if (slotFilled[slot] != isFilled) {
            slotFilled[slot] = isFilled;
            grid->slotButton[row][col]->setFilled(isFilled);
            grid->slotButton[row][col]->applyStyle(isFilled ? filledSlotStyle : emptySlotStyle);
        }
    }
}

void TFormEditMainMenu::onShow(const event::Show& e) {
    if (onGetBankCallback) {
        onGetBankCallback(*selectedBank, bank);
        nameField->text = bank.name;
    }
    recurseEvent(&Widget::onShow, e);
}
