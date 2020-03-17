#include "TextField.hpp"

TFormTextField::TFormTextField() {
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    color = nvgRGB(0x7F, 0x7F, 0x7F);
    textColor = nvgRGB(0xCF, 0xCF, 0xCF);
    multiline = false;
    enabled = true;
}

void TFormTextField::draw(const DrawArgs& args) {
    float undersize = 0;

    // Text
    if (font->handle >= 0) {
        bndSetFont(font->handle);

        NVGcolor highlightColor = color;
        highlightColor.a = 0.5;
        int begin = std::min(cursor, selection);
        int end = (this == APP->event->selectedWidget) ? std::max(cursor, selection) : -1;

        if (text.size() > 9) {
            text = text.substr(0, 9);
        }
        if (enabled) {
            bndIconLabelCaret(args.vg, 0, -3,  box.size.x, box.size.y,
                              -1, textColor, 12, text.c_str(), highlightColor, begin, end);
        }
        else {
            nvgFontSize(args.vg, 12);
            nvgFillColor(args.vg, textColor);
            nvgFontFaceId(args.vg, font->handle);
            nvgTextLetterSpacing(args.vg, 0.0);
            nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
            nvgText(args.vg, 3.85, 1.65, text.c_str(), NULL);
        }
        bndSetFont(APP->window->uiFont->handle);
    }

    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, color);
    nvgStrokeWidth(args.vg, 1.0);
    nvgMoveTo(args.vg, 0, 0);
    nvgLineTo(args.vg, box.size.x, 0);
    nvgLineTo(args.vg, box.size.x, box.size.y);
    nvgLineTo(args.vg, 0, box.size.y);
    nvgLineTo(args.vg, 0, 0);
    nvgStroke(args.vg);
    Widget::draw(args);
}

void TFormTextField::onSelect(const event::Select& e) {
    if (enabled) {
        color = nvgRGB(0xEF, 0xEF, 0xEF);
    }
    else {
        color = nvgRGB(0x7F, 0x7F, 0x7F);
    }
}

void TFormTextField::onDeselect(const event::Deselect& e) {
    color = nvgRGB(0x7F, 0x7F, 0x7F);
    if (onDeselectCallback) {
        onDeselectCallback(text);
    }
}

std::string TFormTextField::getText() const {
    return text.substr(0, 9);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormNumberField::TFormNumberField() {
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    color = nvgRGB(0x7F, 0x7F, 0x7F);
    multiline = false;
    minimum = 1;
    maximum = 64;
    value = minimum;
    selected = false;
}

void TFormNumberField::draw(const DrawArgs& args) {
    //nvgScissor(args.vg, RECT_ARGS(args.clipBox));
    float undersize = 0;

    // Text
    if (font->handle >= 0) {
        bndSetFont(font->handle);

        NVGcolor highlightColor = color;
        highlightColor.a = 0.5;
        int begin = std::min(cursor, selection);
        int end = (this == APP->event->selectedWidget) ? std::max(cursor, selection) : -1;

        if (text.size() > 2) {
            text = text.substr(0, 2);
        }
        bndIconLabelCaret(args.vg, 0, -3,  box.size.x, box.size.y,
                          -1, color, 12, text.c_str(), highlightColor, begin, end);
        bndSetFont(APP->window->uiFont->handle);
    }

    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, color);
    nvgStrokeWidth(args.vg, 1.0);
    nvgMoveTo(args.vg, 0, 0);
    nvgLineTo(args.vg, box.size.x, 0);
    nvgLineTo(args.vg, box.size.x, box.size.y);
    nvgLineTo(args.vg, 0, box.size.y);
    nvgLineTo(args.vg, 0, 0);
    nvgStroke(args.vg);

    //nvgResetScissor(args.vg);
    Widget::draw(args);
}

void TFormNumberField::onSelect(const event::Select& e) {
    selected = true;
    color = nvgRGB(0xEF, 0xEF, 0xEF);
}

void TFormNumberField::onDeselect(const event::Deselect& e) {
    selected = false;
    color = nvgRGB(0x7F, 0x7F, 0x7F);
    updateText(text);
    if (onChangeCallback) {
        onChangeCallback();
    }
}

void TFormNumberField::onAction(const event::Action& e) {
    updateText(text);
    if (onChangeCallback) {
        onChangeCallback();
    }
}

void TFormNumberField::onDragMove(const event::DragMove& e) {
    int newValue = value - (int) e.mouseDelta.y;
    setValue(newValue);
}

void TFormNumberField::setValue(int newValue) {
    if (newValue >= minimum && newValue <= maximum) {
        value = newValue;
        text = std::to_string(value);
        prevText = text;
    }
}

void TFormNumberField::updateText(const std::string& newText) {
    auto isNumeric = [](const std::string& str) -> bool {
        for(int i = 0; i < str.size(); ++i) {
            if(!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    };

    if (!isNumeric(text) || text.size() < 1) {
        text = prevText;
        return;
    }
    prevText = text;

    int newValue = stoi(text);

    if (newValue >= minimum && newValue <= maximum) {
        value = newValue;
    }
    else {
        text = std::to_string(value);
        prevText = text;
    }
}
