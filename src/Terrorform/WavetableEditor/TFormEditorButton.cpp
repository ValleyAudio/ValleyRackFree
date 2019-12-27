#include "TFormEditorButton.hpp"

TFormEditorButtonStyle::TFormEditorButtonStyle() {
    textColor = nvgRGB(0x00, 0x00, 0x00);
    bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
    borderColor = nvgRGB(0x7F, 0x7F, 0x7F);
}

TFormEditorButtonStyle::TFormEditorButtonStyle(const NVGcolor& newTextColor,
                                               const NVGcolor& newBgColor,
                                               const NVGcolor& newBorderColor) {
    textColor = newTextColor;
    bgColor = newBgColor;
    borderColor = newBorderColor;
}

TFormEditorButton::TFormEditorButton() {
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    mode = IDLE_MODE;
    modeOffset = IDLE_MODE;
    buttonStyles[IDLE_MODE] = TFormEditorButtonStyle(nvgRGB(0xCF, 0xCF, 0xCF),
                                                     nvgRGB(0x00, 0x00, 0x00),
                                                     nvgRGB(0x7F, 0x7F, 0x7F));
    buttonStyles[HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                      nvgRGB(0x32, 0x7F, 0x32),
                                                      nvgRGB(0x7F, 0x7F, 0x7F));
    buttonStyles[HIGHLIGHT_MODE] = TFormEditorButtonStyle(nvgRGB(0x00, 0x00, 0x00),
                                                          nvgRGB(0xDF, 0x00, 0x00),
                                                          nvgRGB(0x7F, 0x7F, 0x7F));
    buttonStyles[HIGHLIGHT_HOVER_MODE] = TFormEditorButtonStyle(nvgRGB(0xFF, 0xFF, 0xFF),
                                                                nvgRGB(0xDF, 0x00, 0x00),
                                                                nvgRGB(0x7F, 0x7F, 0x7F));
    fontSize = 12;
    enabled = true;
}

void TFormEditorButton::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, buttonStyles[mode + modeOffset].bgColor);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgFill(args.vg);

    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, buttonStyles[mode + modeOffset].borderColor);
    nvgStrokeWidth(args.vg, 1.0);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgStroke(args.vg);

    if (font->handle >= 0) {
        nvgFillColor(args.vg, buttonStyles[mode + modeOffset].textColor);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);

        nvgFontSize(args.vg, fontSize);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(args.vg, box.size.x / 2, box.size.y / 2, text.c_str(), NULL);
    }
}

void TFormEditorButton::onEnter(const event::Enter& e) {
    mode = IDLE_MODE;
    if(enabled) {
        mode = HOVER_MODE;
    }
}

void TFormEditorButton::onLeave(const event::Leave& e) {
    mode = IDLE_MODE;
}

void TFormEditorButton::onDragEnd(const event::DragEnd& e) {
    if(onClick != NULL && enabled) {
        onClick();
    }
}

void TFormEditorButton::setHighlight(bool highlight) {
    modeOffset = IDLE_MODE;
    if(highlight) {
        modeOffset = HIGHLIGHT_MODE;
    }
}

TFormEditorButton* createNewMenuButton(const std::string& text,
                                       const std::function<void()>& onClickCallback,
                                       int x, int y, int width, int height) {
    TFormEditorButton* newButton = createWidget<TFormEditorButton>(Vec(x, y));
    newButton->text = text;
    newButton->box.size.x = width;
    newButton->box.size.y = height;
    newButton->onClick = onClickCallback;
    return newButton;
}
