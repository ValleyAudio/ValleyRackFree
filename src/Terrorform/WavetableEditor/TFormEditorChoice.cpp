#include "TFormEditorChoice.hpp"

TFormEditorChoiceItem::TFormEditorChoiceItem(unsigned long itemNumber) {
    _itemNumber = itemNumber;
}

void TFormEditorChoiceItem::onAction(const event::Action &e) {
    *choice = _itemNumber;
}

void TFormEditorChoiceItem::onDragEnd(const event::DragEnd &e) {
    if (onChangeCallback) {
        onChangeCallback();
    }
}

void TFormEditorChoiceItem::addOnChangeCallback(const std::function<void()>& onChangeCallback) {
    this->onChangeCallback = onChangeCallback;
}

TFormEditorChoice::TFormEditorChoice() {
    choice = std::make_shared<unsigned long>(0);
    maxItems = 0;
    boxOnEnterColor = nvgRGB(0xEF, 0xEF, 0xEF);
    boxOnLeaveColor = nvgRGB(0xAF, 0xAF, 0xAF);
    boxColor = boxOnLeaveColor;

    arrowOnEnterColor = nvgRGB(0xFF, 0x00, 0x00);
    arrowOnLeaveColor = nvgRGB(0xAF, 0xAF, 0xAF);
    arrowColor = arrowOnLeaveColor;

    textColor = nvgRGB(0xEF, 0xEF, 0xEF);
}

void TFormEditorChoice::onAction(const event::Action &e) {
    Menu* menu = createMenu();
    menu->box.pos = getAbsoluteOffset(Vec(0, box.size.y)).round();
	menu->box.size.x = box.size.x;

    for(unsigned long i = 0; i < maxItems; ++i) {
        TFormEditorChoiceItem *item = new TFormEditorChoiceItem(i);
        item->_itemNumber = i;
        item->choice = choice;
        item->rightText = (*choice == i) ? "✔" : "";
        item->text = items[i];
        item->addOnChangeCallback(onChangeCallback);
        menu->addChild(item);
    }
}

void TFormEditorChoice::onEnter(const event::Enter &e) {
    boxColor = boxOnEnterColor;
    arrowColor = arrowOnEnterColor;
}

void TFormEditorChoice::onLeave(const event::Leave &e) {
    boxColor = boxOnLeaveColor;
    arrowColor = arrowOnLeaveColor;
}

void TFormEditorChoice::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, boxColor);
    nvgStrokeWidth(args.vg, 1.f);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgStroke(args.vg);
    nvgClosePath(args.vg);

    std::shared_ptr<Font> font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    if (font) {
        nvgFontSize(args.vg, 12);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0);
        Vec textPos = Vec(3, box.size.y / 2).round();
        nvgFillColor(args.vg, textColor);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        if(choice) {
            nvgText(args.vg, textPos.x, textPos.y, items[*choice].c_str(), NULL);
        }
        else {
            nvgText(args.vg, textPos.x, textPos.y, "--", NULL);
        }
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

int TFormEditorChoice::getChoice() const {
    return *choice;
}
