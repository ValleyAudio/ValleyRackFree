#include "ValleyWidgets.hpp"

 DynamicText::DynamicText() {
    setFont(FONT_MODE_ALTEDIN);
    size = 16;
    blur = 0.f;
    colorHandle = nullptr;
    viewMode = ACTIVE_HIGH_VIEW;
    horzAlignment = NVG_ALIGN_CENTER;
    vertAlignment = NVG_ALIGN_TOP;
    customColor = nvgRGB(0xFF,0xFF,0xFF);
}

void DynamicText::draw(const DrawArgs &args) {
    std::shared_ptr<Font> font;
    switch(fontMode) {
        case FONT_MODE_ALTEDIN:
            font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
            break;
        case FONT_MODE_7SEG:
            font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DSEG14Classic-Italic.ttf"));
            break;
        default:
            font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
    }

    if (font) {
        nvgFontSize(args.vg, size);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.f);
        Vec textPos = Vec(0.f, 0.f);
        if(colorHandle != nullptr) {
            switch((ColorMode)*colorHandle) {
                case COLOR_MODE_WHITE: textColor = nvgRGB(0xFF,0xFF,0xFF); break;
                case COLOR_MODE_BLACK: textColor = nvgRGB(0x14,0x14,0x14); break;
                case COLOR_MODE_RED: textColor = nvgRGB(0xFF,0x00,0x00); break;
                default: textColor = customColor;
            }
        }
        else {
            textColor = customColor;
        }
    
        if (text) {
            nvgFillColor(args.vg, textColor);
            nvgTextAlign(args.vg, horzAlignment | vertAlignment);
            nvgFontBlur(args.vg, blur);
            nvgText(args.vg, textPos.x, textPos.y, text->c_str(), NULL);
        }
    }

    Widget::draw(args);
}

void DynamicText::step() {
    Widget::step();
}

void DynamicText::setFont(const FontMode& newFontMode) {
    fontMode = newFontMode;
}

DynamicText* createDynamicText(const Vec& pos, int size, std::string text,
                               int* visibilityHandle, DynamicViewMode viewMode) {
    DynamicText* dynText = new DynamicText();
    dynText->size = size;
    dynText->text = std::make_shared<std::string>(text);
    dynText->box.pos = pos;
    dynText->box.size = Vec(82,14);
    dynText->viewMode = viewMode;
    return dynText;
}

DynamicText* createDynamicText(const Vec& pos, int size, std::string text,
                               int* visibilityHandle, int* colorHandle, DynamicViewMode viewMode) {
    DynamicText* dynText = new DynamicText();
    dynText->size = size;
    dynText->text = std::make_shared<std::string>(text);
    dynText->box.pos = pos;
    dynText->box.size = Vec(82,14);
    dynText->viewMode = viewMode;
    dynText->colorHandle = colorHandle;
    return dynText;
}

DynamicText* createDynamicText(const Vec& pos, int size, std::shared_ptr<std::string> text,
                               int* visibilityHandle, DynamicViewMode viewMode) {
    DynamicText* dynText = new DynamicText();
    dynText->size = size;
    dynText->text = text;
    dynText->box.pos = pos;
    dynText->box.size = Vec(82,14);
    dynText->viewMode = viewMode;
    return dynText;
}

DynamicText* createDynamicText(const Vec& pos, int size, std::shared_ptr<std::string> text,
                               int* visibilityHandle, int* colorHandle, DynamicViewMode viewMode) {
    DynamicText* dynText = new DynamicText();
    dynText->size = size;
    dynText->text = text;
    dynText->box.pos = pos;
    dynText->box.size = Vec(82,14);
    dynText->viewMode = viewMode;
    dynText->colorHandle = colorHandle;
    return dynText;
}

DynamicFrameText::DynamicFrameText() {
    itemHandle = nullptr;
}

void DynamicFrameText::addItem(const std::string& item) {
    textItem.push_back(item);
}

void DynamicFrameText::draw(const DrawArgs &args) {
    std::shared_ptr<Font> font;
    switch(fontMode) {
        case FONT_MODE_ALTEDIN:
            font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
            break;
        case FONT_MODE_7SEG:
            font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DSEG14Classic-Italic.ttf"));
            break;
        default:
            font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
    }

    int item = -1;
    if(itemHandle != nullptr) {
        item = *itemHandle;
    }
    else {
        item = 0;
    }

    if((int)textItem.size() && item >= 0 && item < (int)textItem.size() && font) {
        nvgFontSize(args.vg, size);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.f);
        Vec textPos = Vec(0.f, 0.f);

        if(colorHandle != nullptr) {
            switch((ColorMode)*colorHandle) {
                case COLOR_MODE_WHITE: textColor = nvgRGB(0xFF,0xFF,0xFF); break;
                case COLOR_MODE_BLACK: textColor = nvgRGB(0x14,0x14, 0x14); break;
                case COLOR_MODE_RED: textColor = nvgRGB(0xFF,0x00, 0x00); break;
                default: textColor = customColor;
            }
        }
        else {
            textColor = customColor;
        }

        nvgFillColor(args.vg, textColor);
        nvgTextAlign(args.vg, horzAlignment | vertAlignment);
        nvgFontBlur(args.vg, blur);
        nvgText(args.vg, textPos.x, textPos.y, textItem[item].c_str(), NULL);
    }

    Widget::draw(args);
}
