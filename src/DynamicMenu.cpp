#include "ValleyWidgets.hpp"

DynamicMenuItem::DynamicMenuItem(int itemNumber) {
    _itemNumber = itemNumber;
    _choice = nullptr;
    setChoice = nullptr;
    _showTick = true;
}

void DynamicMenuItem::onAction(const event::Action &e) {
    if(setChoice) {
        *_choice = _itemNumber;
        setChoice(_itemNumber);
    }
}

void DynamicMenuItem::step() {
    if(_showTick) {
        rightText = (*_choice == _itemNumber) ? "✔" : "";
    }
    MenuItem::step();
}

DynamicMenu::DynamicMenu() {
    _font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
    _text = std::make_shared<std::string>("");
    _textSize = 14;
    _isTransparent = false;
    _showTick = true;
    _choice = 0;
    setChoice = nullptr;
}

void DynamicMenu::onAction(const event::Action &e) {
    Menu* menu = createMenu();
    menu->box.pos = getAbsoluteOffset(Vec(0, box.size.y)).round();
	menu->box.size.x = box.size.x;
    for(unsigned long i = 0; i < _items.size(); ++i){
        DynamicMenuItem *item = new DynamicMenuItem(i);
        item->_choice = &_choice;
        item->_itemNumber = i;
        item->text = _items[i];
        item->setChoice = setChoice;
        item->_showTick = _showTick;
        menu->addChild(item);
    }
    if(onOpen) {
        onOpen();
    }
}

void DynamicMenu::onEnter(const event::Enter &e) {
    if(onMouseEnter) {
        onMouseEnter();
    }
}

void DynamicMenu::onLeave(const event::Leave &e) {
    if(onMouseLeave) {
        onMouseLeave();
    }
}


void DynamicMenu::step() {
    *_text = _items[_choice];
}

void DynamicMenu::draw(const DrawArgs &args) {
    if(_isTransparent) {
        return;
    }

    nvgFontSize(args.vg, _textSize);
    nvgFontFaceId(args.vg, _font->handle);
    nvgTextLetterSpacing(args.vg, 0.f);
    Vec textPos = Vec(this->box.size.x / 2.f, this->box.size.y / 2.f - 2.f);
    NVGcolor textColor = nvgRGB(0xFF,0xFF,0xFF);
    nvgFillColor(args.vg, textColor);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgText(args.vg, textPos.x, textPos.y, _text->c_str(), NULL);
}

DynamicMenu* createDynamicMenu(const Vec& pos, const Vec& size,
                               const std::vector<std::string>& items,
                               bool isTransparent, bool showTick) {
    DynamicMenu* newMenu = new DynamicMenu;
    newMenu->box.pos = pos;
    newMenu->box.size = size;
    newMenu->_items = items;
    newMenu->_isTransparent = isTransparent;
    newMenu->_showTick = showTick;
    return newMenu;
}
