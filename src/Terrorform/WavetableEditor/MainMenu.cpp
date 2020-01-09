#include "MainMenu.hpp"

TFormEditMainMenu::TFormEditMainMenu() {
    box.size = Vec(238, 195);
    loadButton = createNewMenuButton("Load", NULL, buttonOffset, 21, buttonWidth, buttonHeight);
    addChild(loadButton);
    clearButton = createNewMenuButton("Clear", NULL, buttonWidth + buttonOffset * 2, 21, buttonWidth, buttonHeight);
    addChild(clearButton);
    viewButton = createNewMenuButton("View", NULL, buttonWidth * 2 + buttonOffset * 3, 21, buttonWidth, buttonHeight);
    addChild(viewButton);
    cloneButton = createNewMenuButton("Clone", NULL, buttonWidth * 3 + buttonOffset * 4, 21, buttonWidth, buttonHeight);
    addChild(cloneButton);
    moveButton = createNewMenuButton("Move", NULL, buttonWidth * 4 + buttonOffset * 5, 21, buttonWidth, buttonHeight);
    addChild(moveButton);
    backButton = createNewMenuButton("Back", NULL, buttonWidth * 4 + buttonOffset * 5, 3, buttonWidth, buttonHeight);
    addChild(backButton);

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
        grid->slotButton[row][col]->applyStyle(isFilled ? filledSlotStyle : emptySlotStyle);
    }
}
