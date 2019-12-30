#include "MainMenu.hpp"

TFormEditMainMenu::TFormEditMainMenu() {
    box.size = Vec(238, 195);
    loadButton = createNewMenuButton("Load", NULL, buttonOffset, 21, buttonWidth, buttonHeight);
    addChild(loadButton);
    viewButton = createNewMenuButton("View", NULL, buttonWidth + buttonOffset * 2, 21, buttonWidth, buttonHeight);
    addChild(viewButton);
    cloneButton = createNewMenuButton("Clone", NULL, buttonWidth * 2 + buttonOffset * 3, 21, buttonWidth, buttonHeight);
    addChild(cloneButton);
    clearButton = createNewMenuButton("Clear", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    addChild(clearButton);
    purgeButton = createNewMenuButton("Purge", NULL, box.size.x - buttonWidth - 3, 21, buttonWidth, buttonHeight);
    addChild(purgeButton);
    backButton = createNewMenuButton("Back", NULL, box.size.x - buttonWidth - 3, 3, buttonWidth, buttonHeight);
    addChild(backButton);

    // Slots grid
    emptySlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF), nvgRGB(0x17, 0x17, 0x17), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x2F, 0x2F, 0xAF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_MODE] =  TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    emptySlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x5F, 0x5F, 0xFF), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    filledSlotButtonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0x9F, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF), nvgRGB(0xBF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));
    filledSlotButtonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00), nvgRGB(0xFF, 0x00, 0x00), nvgRGBA(0x00, 0x00, 0x00, 0x00));

    Vec offset = Vec(0, 40);
    grid = createWidget<TFormEditorGrid<TFORM_EDITOR_ROWS, TFORM_EDITOR_COLS>>(box.pos.plus(offset));
    grid->box.size.x = box.size.x;
    grid->box.size.y = box.size.y - offset.y;

    int id = 0;
    for (int row = 0; row < TFORM_EDITOR_ROWS; ++row) {
        for (int col = 0; col < TFORM_EDITOR_COLS; ++col) {
            auto selectBank = [=]() {
                *selectedBank = id;
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
    std::string strSelectedBank = "Bank: " + std::to_string(*selectedBank + 1);
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

void TFormEditMainMenu::setSlotFilledFlag(int slot, bool isFilled) {
    if(slot >= 0 && slot < TFORM_EDITOR_SLOTS) {
        int row = slot / TFORM_EDITOR_ROWS;
        int col = slot % TFORM_EDITOR_COLS;
        slotFilled[slot] = isFilled;
        for(int m = 0; m < NUM_BUTTON_MODES; ++m) {
            if(isFilled) {
                grid->slotButton[row][col]->buttonStyles[m] = filledSlotButtonStyles[m];
            }
            else {
                grid->slotButton[row][col]->buttonStyles[m] = emptySlotButtonStyles[m];
            }
        }
    }
}
