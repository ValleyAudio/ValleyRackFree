#include "DigitalDisplay.hpp"

DigitalDisplay::DigitalDisplay(unsigned long maxDisplayLength) {
    font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DSEG14Classic-Italic.ttf"));
    horzAlignment = NVG_ALIGN_CENTER;
    vertAlignment = NVG_ALIGN_TOP;
    size = 16;
    displayLength = maxDisplayLength;
    backDigits = std::string(maxDisplayLength, '~');
}

void DigitalDisplay::draw(const DrawArgs &args) {
    nvgFontSize(args.vg, size);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.f);
    nvgFillColor(args.vg, colours.bgColour);
    nvgTextAlign(args.vg, horzAlignment | vertAlignment);
    nvgText(args.vg, 0.f, 0.f, backDigits.c_str(), NULL);

    nvgFillColor(args.vg, colours.colour);
    nvgTextAlign(args.vg, horzAlignment | vertAlignment);
    nvgText(args.vg, 0.f, 0.f, text.c_str(), NULL);

    nvgFillColor(args.vg, colours.blur1Colour);
    nvgTextAlign(args.vg, horzAlignment | vertAlignment);
    nvgFontBlur(args.vg, blur1);
    nvgText(args.vg, 0.f, 0.f, text.c_str(), NULL);

    nvgFillColor(args.vg, colours.blur2Colour);
    nvgTextAlign(args.vg, horzAlignment | vertAlignment);
    nvgFontBlur(args.vg, blur2);
    nvgText(args.vg, 0.f, 0.f, text.c_str(), NULL);

    Widget::draw(args);
}

void DigitalDisplay::setText(const std::string& newText) {
    text = newText;
    if (text.size() > displayLength) {
        text.resize(displayLength);
    }
    std::replace(text.begin(), text.end(), ' ', '!');
}

void DigitalDisplay::step() {
    Widget::step();
}
