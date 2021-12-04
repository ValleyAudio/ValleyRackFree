#include "ValleyChoiceMenu.hpp"

ValleyChoiceMenu::ValleyChoiceMenu() {
    box.size = mm2px(math::Vec(0, 19.0 / 3));
    color = nvgRGB(0xFF, 0xFF, 0xFF);
    bgColor = nvgRGBA(0x00, 0x00, 0x00, 0xFF);
    outlineColor = nvgRGBA(0xF9, 0xF9, 0xF9, 0xFF);
    fontSize = 14;
}

void ValleyChoiceMenu::draw(const DrawArgs& args) {
    nvgScissor(args.vg, RECT_ARGS(args.clipBox));
    if (bgColor.a > 0.0) {
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
        nvgFillColor(args.vg, bgColor);
        nvgFill(args.vg);
    }

    if (outlineColor.a > 0.0) {
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
        nvgStrokeColor(args.vg, outlineColor);
        nvgStrokeWidth(args.vg, 2.0);
        nvgStroke(args.vg);
    }

    std::shared_ptr<Font> font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
    if (font) {
        textOffset = math::Vec(box.size.x / 2.0, box.size.y / 2.0);
        nvgFillColor(args.vg, color);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, fontSize);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(args.vg, textOffset.x, textOffset.y, text.c_str(), NULL);
    }
    nvgResetScissor(args.vg);
}

void ValleyChoiceMenu::onButton(const event::Button& e) {
    OpaqueWidget::onButton(e);

    if (e.action == GLFW_PRESS && (e.button == GLFW_MOUSE_BUTTON_LEFT || e.button == GLFW_MOUSE_BUTTON_RIGHT)) {
        event::Action eAction;
        onAction(eAction);
        e.consume(this);
    }
}
