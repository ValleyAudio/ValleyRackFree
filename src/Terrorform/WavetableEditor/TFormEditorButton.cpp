#include "TFormEditorButton.hpp"

TFormEditorButtonColors::TFormEditorButtonColors() {
    textColor = nvgRGB(0x00, 0x00, 0x00);
    bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
    borderColor = nvgRGB(0x7F, 0x7F, 0x7F);
}

TFormEditorButtonColors::TFormEditorButtonColors(char txtR, char txtG, char txtB, char txtA,
                                                 char bgR,  char bgG,  char bgB,  char bgA,
                                                 char brdR, char brdG, char brdB, char brdA) {
    textColor = nvgRGBA(txtR, txtG, txtB, txtA);
    bgColor = nvgRGBA(bgR, bgG, bgB, bgA);
    borderColor = nvgRGBA(brdR, brdG, brdB, brdA);
}

TFormEditorButtonStyleSet::TFormEditorButtonStyleSet() {
    disabledColors =       TFormEditorButtonColors(0x7F, 0x7F, 0x7F, 0xFF,
                                                   0x00, 0x00, 0x00, 0xFF,
                                                   0x4F, 0x4F, 0x4F, 0xFF);

    idleColors =           TFormEditorButtonColors(0xCF, 0xCF, 0xCF, 0xFF,
                                                   0x00, 0x00, 0x00, 0xFF,
                                                   0x7F, 0x7F, 0x7F, 0xFF);

    hoverColors =          TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                   0x2F, 0x2F, 0xAF, 0xFF,
                                                   0x7F, 0x7F, 0x7F, 0xFF);

    highlightIdleColors =  TFormEditorButtonColors(0xEF, 0xEF, 0xEF, 0xFF,
                                                   0x5F, 0x5F, 0xFF, 0xFF,
                                                   0x7F, 0x7F, 0x7F, 0xFF);

    highlightHoverColors = TFormEditorButtonColors(0xFF, 0xFF, 0xFF, 0xFF,
                                                   0x5F, 0x5F, 0xFF, 0xFF,
                                                   0xEF, 0xEF, 0xEF, 0xFF);
    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
    fontSize = 12;
}

TFormEditorButton::TFormEditorButton() {
    isEnabled = true;
    isHighlighted = false;
    respondToMouse = true;
    setMode(IDLE_MODE);
}

void TFormEditorButton::draw(const DrawArgs& args) {
    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, colors.bgColor);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgFill(args.vg);

    nvgBeginPath(args.vg);
    nvgStrokeColor(args.vg, colors.borderColor);
    nvgStrokeWidth(args.vg, 1.0);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    nvgStroke(args.vg);

    nvgFillColor(args.vg, colors.textColor);
    nvgFontFaceId(args.vg, style.font->handle);
    nvgTextLetterSpacing(args.vg, 0.0);

    nvgFontSize(args.vg, style.fontSize);
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgText(args.vg, box.size.x / 2, box.size.y / 2, text.c_str(), NULL);
}

void TFormEditorButton::onEnter(const event::Enter& e) {
    if (isEnabled) {
        if (respondToMouse) {
            setMode(HOVER_MODE);
        }
        else {
            setMode(IDLE_MODE);
        }
    }
    else {
        setMode(DISABLED_MODE);
    }
}

void TFormEditorButton::onLeave(const event::Leave& e) {
    if(isEnabled && respondToMouse) {
        setMode(IDLE_MODE);
    }
}

void TFormEditorButton::onDragEnd(const event::DragEnd& e) {
    if(onClick && isEnabled && respondToMouse) {
        onClick();
    }
}

void TFormEditorButton::setEnable(bool isEnabled) {
    if(this->isEnabled == isEnabled) {
        return;
    }

    this->isEnabled = isEnabled;
    if(isEnabled) {
        setMode(IDLE_MODE);
    }
    else {
        setMode(DISABLED_MODE);
    }
}

void TFormEditorButton::applyStyle(const TFormEditorButtonStyleSet& newStyle) {
    style = newStyle;
    setMode(mode);
}

void TFormEditorButton::setMode(const TFormEditorButtonModes& newMode) {
    mode = newMode;
    switch(mode) {
        case DISABLED_MODE:
            colors = style.disabledColors;
            break;
        case IDLE_MODE:
        case HIGHLIGHT_IDLE_MODE:
            colors = isHighlighted ? style.highlightIdleColors : style.idleColors;
            break;
        case HOVER_MODE:
        case HIGHLIGHT_HOVER_MODE:
            colors = isHighlighted ? style.highlightHoverColors : style.hoverColors;
            break;
        default:
            colors = style.idleColors;
    }
}

void TFormEditorButton::setHighlight(bool highlight) {
    if(isHighlighted == highlight) {
        return;
    }
    isHighlighted = highlight;
    setMode(mode);
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
