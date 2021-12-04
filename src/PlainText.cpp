#include "ValleyWidgets.hpp"

PlainText::PlainText() {
    color = nvgRGB(0xCF, 0xCF, 0xCF);
    horzAlignment = NVG_ALIGN_CENTER;
    vertAlignment = NVG_ALIGN_TOP;
    size = 16;
}

void PlainText::draw(const DrawArgs &args) {
    std::shared_ptr<Font> font;
    if (!fontPath.empty()) {
        font = APP->window->loadFont(asset::plugin(pluginInstance, fontPath));
    }

    if (font) {
        nvgFontFaceId(args.vg, font->handle);
        nvgFontSize(args.vg, size);
        nvgTextLetterSpacing(args.vg, 0.f);
        nvgFillColor(args.vg, color);
        nvgTextAlign(args.vg, horzAlignment | vertAlignment);
        nvgText(args.vg, 0.f, 0.f, text.c_str(), NULL);
    }
    Widget::draw(args);
}
