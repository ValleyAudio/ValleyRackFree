#include "ValleyWidgets.hpp"

PlainText::PlainText() {
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    color = nvgRGB(0xCF, 0xCF, 0xCF);
    horzAlignment = NVG_ALIGN_CENTER;
    vertAlignment = NVG_ALIGN_TOP;
    size = 16;
}

void PlainText::draw(const DrawArgs &args) {
    nvgFontSize(args.vg, size);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 0.f);
    nvgFillColor(args.vg, color);
    nvgTextAlign(args.vg, horzAlignment | vertAlignment);
    nvgText(args.vg, 0.f, 0.f, text.c_str(), NULL);
    Widget::draw(args);
}
