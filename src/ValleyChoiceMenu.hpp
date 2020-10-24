#pragma once

#include "Valley.hpp"

struct ValleyChoiceMenu : widget::OpaqueWidget {
    std::string text;
    std::shared_ptr<Font> font;
    math::Vec textOffset;
    NVGcolor color;
    NVGcolor bgColor;
    NVGcolor outlineColor;
    float fontSize;
    ValleyChoiceMenu();
    void draw(const DrawArgs& args) override;
    void onButton(const event::Button& e) override;
};
