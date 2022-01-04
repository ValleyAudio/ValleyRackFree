#include "DigitalDisplay.hpp"

DigitalDisplayColours::DigitalDisplayColours(const std::array<unsigned char, 4>& bgColourHex,
                                             const std::array<unsigned char, 4>& blur1ColourHex,
                                             const std::array<unsigned char, 4>& blur2ColourHex,
                                             const std::array<unsigned char, 4>& colourHex) {
    bgColour = nvgRGBA(bgColourHex[0], bgColourHex[1], bgColourHex[2], bgColourHex[3]);
    colour = nvgRGBA(colourHex[0], colourHex[1], colourHex[2], colourHex[3]);
    blur1Colour = nvgRGBA(blur1ColourHex[0], blur1ColourHex[1], blur1ColourHex[2], blur1ColourHex[3]);
    blur2Colour = nvgRGBA(blur2ColourHex[0], blur2ColourHex[1], blur2ColourHex[2], blur2ColourHex[3]);
}

DigitalDisplay::DigitalDisplay(unsigned long maxDisplayLength) {
    horzAlignment = NVG_ALIGN_CENTER;
    vertAlignment = NVG_ALIGN_TOP;
    size = 16;
    displayLength = maxDisplayLength;
    backDigits = std::string(maxDisplayLength, '~');
}

void DigitalDisplay::drawLayer(const DrawArgs &args, int layer) {
    if (layer == 1) {
        std::shared_ptr<Font> font = APP->window->loadFont(asset::plugin(pluginInstance, "res/DSEG14Classic-Italic.ttf"));
        if (font) {
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
        }
    }
    Widget::drawLayer(args, layer);
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
