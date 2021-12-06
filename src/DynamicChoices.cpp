#include "ValleyWidgets.hpp"

DynamicItem::DynamicItem(unsigned long itemNumber) {
    _itemNumber = itemNumber;
    _choice = nullptr;
    updateChoice = nullptr;
}

void DynamicItem::onAction(const event::Action &e) {
    if(_choice != nullptr) {
        *_choice = _itemNumber;
        if(updateChoice) {
            updateChoice(_itemNumber);
        }
    }
}

void DynamicItem::step() {
    if(_choice != nullptr) {
        rightText = (*_choice == _itemNumber) ? "✔" : "";
    }
    MenuItem::step();
}

DynamicChoice::DynamicChoice() {
    _choice = nullptr;
    _oldChoice = -1;
    _visibility = nullptr;
    _viewMode = ACTIVE_HIGH_VIEW;
    _text = std::make_shared<std::string>("");
    _textSize = 14;
    _transparent = false;
    updateChoice = nullptr;
}

void DynamicChoice::onAction(const event::Action &e) {
    Menu* menu = createMenu();
    menu->box.pos = getAbsoluteOffset(Vec(0, box.size.y)).round();
	menu->box.size.x = box.size.x;
    for(unsigned long i = 0; i < _items.size(); ++i){
        DynamicItem *item = new DynamicItem(i);
        item->_choice = _choice;
        item->_itemNumber = i;
        item->text = _items[i];
        item->updateChoice = updateChoice;
        menu->addChild(item);
    }
    if(onOpen) {
        onOpen();
    }
}

void DynamicChoice::onEnter(const event::Enter &e) {
    if(onMouseEnter) {
        onMouseEnter();
    }
}

void DynamicChoice::onLeave(const event::Leave &e) {
    if(onMouseLeave) {
        onMouseLeave();
    }
}


void DynamicChoice::step() {
    if(_visibility != nullptr) {
        if(*_visibility) {
            visible = true;
        }
        else {
            visible = false;
        }
        if(_viewMode == ACTIVE_LOW_VIEW) {
            visible = !visible;
        }
    }
    else {
        visible = true;
    }
    if(_choice != nullptr) {
        *_text = _items[*_choice];
    }
}

void DynamicChoice::draw(const DrawArgs &args) {
    if(_transparent) {
        return;
    }
    nvgBeginPath(args.vg);
    NVGcolor bgColor = nvgRGB(0x1A, 0x1A, 0x1A);
    nvgFillColor(args.vg, bgColor);
    nvgStrokeWidth(args.vg, 0.f);
    nvgRect(args.vg, 0, 0, this->box.size.x, this->box.size.y - 3);
    nvgFill(args.vg);
    nvgClosePath(args.vg);

    nvgBeginPath(args.vg);
    NVGcolor outlineColor = nvgRGB(0xF9, 0xF9, 0xF9);
    nvgStrokeColor(args.vg, outlineColor);
    nvgStrokeWidth(args.vg, 1.f);
    nvgMoveTo(args.vg, 0.f, 0.f);
    nvgLineTo(args.vg, this->box.size.x, 0.f);
    nvgLineTo(args.vg, this->box.size.x, this->box.size.y - 3);
    nvgLineTo(args.vg, 0.f, this->box.size.y -3);
    nvgLineTo(args.vg, 0.f, 0.f);
    nvgStroke(args.vg);
    nvgClosePath(args.vg);

    if(_choice != nullptr) {
        *_text = _items[*_choice];
    }

    std::shared_ptr<Font> font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
    if (font) {
        nvgFontSize(args.vg, _textSize);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.f);
        Vec textPos = Vec(this->box.size.x / 2.f, this->box.size.y / 2.f - 2.f);
        NVGcolor textColor = nvgRGB(0xFF,0xFF,0xFF);
        nvgFillColor(args.vg, textColor);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(args.vg, textPos.x, textPos.y, _text->c_str(), NULL);
    }
}

DynamicChoice* createDynamicChoice(const Vec& pos,
                                   float width,
                                   const std::vector<std::string>& items,
                                   unsigned long* choiceHandle,
                                   int* visibilityHandle,
                                   DynamicViewMode viewMode) {
    DynamicChoice* choice = new DynamicChoice;
    choice->_choice = choiceHandle;
    choice->box.pos = pos;
    choice->box.size.x = width;
    choice->_items = items;
    choice->_visibility = visibilityHandle;
    choice->_viewMode = viewMode;
    return choice;
}
