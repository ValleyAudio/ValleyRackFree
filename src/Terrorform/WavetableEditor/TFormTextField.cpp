#include "TFormTextField.hpp"
#include <iostream>

TFormTextField::TFormTextField() {
    bgColor = nvgRGB(0x00, 0x00, 0x00);
    color = nvgRGB(0x7F, 0x7F, 0x7F);
    textColor = nvgRGB(0xCF, 0xCF, 0xCF);
    multiline = false;
    enabled = true;
    selected = false;
}

void TFormTextField::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, bgColor);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgFill(args.vg);

    // Text
    std::shared_ptr<Font> font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    if (font) {
        const int maxChars = 9;
        bndSetFont(font->handle);

        NVGcolor highlightColor = color;
        highlightColor.a = 0.5;
        int begin = std::min(cursor, selection);
        int end = (this == APP->event->selectedWidget) ? std::max(cursor, selection) : -1;

        if (text.size() > maxChars) {
            text = text.substr(0, maxChars);
            cursor = maxChars;
            selection = maxChars;
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
        bgColor = nvgRGB(0x00, 0x00, 0x00);
        textColor = nvgRGB(0xCF, 0xCF, 0xCF);
    }
    else {
        color = nvgRGB(0x7F, 0x7F, 0x7F);
    }
    selected = true;
}

void TFormTextField::onDeselect(const event::Deselect& e) {
    color = nvgRGB(0x7F, 0x7F, 0x7F);
    if (onDeselectCallback) {
        onDeselectCallback(text);
    }
    selected = false;
}

void TFormTextField::onEnter(const event::Enter& e) {
    if (enabled && !selected) {
        bgColor = nvgRGB(0x2F, 0x2F, 0xAF);
        textColor = nvgRGB(0xEF, 0xEF, 0xEF);
    }
    else {
        bgColor = nvgRGB(0x00, 0x00, 0x00);
        textColor = nvgRGB(0xCF, 0xCF, 0xCF);
    }
}

void TFormTextField::onLeave(const event::Leave& e) {
    bgColor = nvgRGB(0x00, 0x00, 0x00);
    textColor = nvgRGB(0xCF, 0xCF, 0xCF);
}

std::string TFormTextField::getText() const {
    return text.substr(0, 9);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TFormNumberField::TFormNumberField() {
    bgColor = nvgRGB(0x00, 0x00, 0x00);
    color = nvgRGB(0x7F, 0x7F, 0x7F);
    textColor = nvgRGB(0xCF, 0xCF, 0xCF);
    multiline = false;
    minimum = 1;
    maximum = TFORM_MAX_NUM_WAVES;
    value = minimum;
    enabled = true;
    selected = false;
}

void TFormNumberField::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, bgColor);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgFill(args.vg);

    // Text
    std::shared_ptr<Font> font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    if (font) {
        const int maxChars = 2;

        bndSetFont(font->handle);

        NVGcolor highlightColor = color;
        highlightColor.a = 0.5;
        int begin = std::min(cursor, selection);
        int end = (this == APP->event->selectedWidget) ? std::max(cursor, selection) : -1;

        if (text.size() > maxChars) {
            text = text.substr(0, maxChars);
            cursor = maxChars;
            selection = maxChars;
        }
        cursor = std::min((int)text.size(), cursor);
        selection = std::min(cursor, selection);

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

void TFormNumberField::onSelect(const event::Select& e) {
    if (enabled) {
        color = nvgRGB(0xEF, 0xEF, 0xEF);
        bgColor = nvgRGB(0x00, 0x00, 0x00);
        textColor = nvgRGB(0xCF, 0xCF, 0xCF);
    }
    else {
        color = nvgRGB(0x7F, 0x7F, 0x7F);
    }
    selected = true;
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
    if (onChangeCallback) {
        onChangeCallback();
    }
}

void TFormNumberField::onEnter(const event::Enter& e) {
    if (enabled && !selected) {
        bgColor = nvgRGB(0x2F, 0x2F, 0xAF);
        textColor = nvgRGB(0xEF, 0xEF, 0xEF);
    }
    else {
        bgColor = nvgRGB(0x00, 0x00, 0x00);
        textColor = nvgRGB(0xCF, 0xCF, 0xCF);
    }
};

void TFormNumberField::onLeave(const event::Leave& e) {
    bgColor = nvgRGB(0x00, 0x00, 0x00);
    textColor = nvgRGB(0xCF, 0xCF, 0xCF);
};

void TFormNumberField::setMaximum(int newMaximum) {
    if (newMaximum < 1) {
        return;
    }
    maximum = newMaximum;
    if (value > maximum) {
        setValue(maximum);
    }
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
        cursor = text.size();
        selection = text.size();
        return;
    }
    prevText = text;

    int newValue = stoi(text);

    if (newValue >= minimum && newValue <= maximum) {
        value = newValue;
    }
    else {
        text = std::to_string(value);
        cursor = text.size();
        selection = text.size();
        prevText = text;
    }
}
