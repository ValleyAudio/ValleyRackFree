#include "TFormEditorNumberChoice.hpp"

TFormEditorNumberItem::TFormEditorNumberItem(unsigned long itemNumber) {
    _itemNumber = itemNumber;
}

void TFormEditorNumberItem::onAction(const event::Action &e) {
    *choice = _itemNumber;
}

void TFormEditorNumberItem::step() {
    if(choice) {
        if(*choice == _itemNumber) {
            rightText = (*choice == _itemNumber) ? "✔" : "";
        }
    }
    MenuItem::step();
}

TFormEditorNumberChoice::TFormEditorNumberChoice() {
    range = 0;
    choice = std::make_shared<unsigned long>(0);
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    boxOnEnterColor = nvgRGB(0xEF, 0xEF, 0xEF);
    boxOnLeaveColor = nvgRGB(0xAF, 0xAF, 0xAF);
    boxColor = boxOnLeaveColor;

    arrowOnEnterColor = nvgRGB(0xFF, 0x00, 0x00);
    arrowOnLeaveColor = nvgRGB(0xAF, 0xAF, 0xAF);
    arrowColor = arrowOnLeaveColor;

    textColor = nvgRGB(0xEF, 0xEF, 0xEF);
}

void TFormEditorNumberChoice::onAction(const event::Action &e) {
    Menu* menu = createMenu();
    menu->box.pos = getAbsoluteOffset(Vec(0, box.size.y)).round();
	menu->box.size.x = box.size.x;

    for(unsigned long i = 0; i < range; ++i) {
        TFormEditorNumberItem *item = new TFormEditorNumberItem(i);
        item->_itemNumber = i;
        item->choice = choice;
        item->text = std::to_string(i + 1);
        menu->addChild(item);
    }
}

void TFormEditorNumberChoice::onEnter(const event::Enter &e) {
    boxColor = boxOnEnterColor;
    arrowColor = arrowOnEnterColor;
}

void TFormEditorNumberChoice::onLeave(const event::Leave &e) {
    boxColor = boxOnLeaveColor;
    arrowColor = arrowOnLeaveColor;
}

void TFormEditorNumberChoice::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, boxColor);
    nvgStrokeWidth(args.vg, 1.f);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgStroke(args.vg);
    nvgClosePath(args.vg);

    nvgFontSize(args.vg, 12);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0);
    Vec textPos = Vec(3, box.size.y / 2).round();
    nvgFillColor(args.vg, textColor);
    nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    if(choice) {
        nvgText(args.vg, textPos.x, textPos.y, std::to_string(*choice + 1).c_str(), NULL);
    }
    else {
        nvgText(args.vg, textPos.x, textPos.y, "--", NULL);
    }

    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, arrowColor);
    nvgMoveTo(args.vg, box.size.x - 2, 4);
    nvgLineTo(args.vg, box.size.x - 6, box.size.y - 4);
    nvgLineTo(args.vg, box.size.x - 10, 4);
    nvgLineTo(args.vg, box.size.x - 2, 4);
    nvgFill(args.vg);
    nvgClosePath(args.vg);
}
