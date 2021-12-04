#ifndef TFORM_EDITOR_BUTTON_HPP
#define TFORM_EDITOR_BUTTON_HPP
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"

struct TFormEditorButtonColors {
    NVGcolor textColor;
	NVGcolor bgColor;
    NVGcolor borderColor;

    TFormEditorButtonColors();
    TFormEditorButtonColors(char txtR, char txtG, char txtB, char txtA,
                            char bgR,  char bgG,  char bgB,  char bgA,
                            char brdR, char brdG, char brdB, char brdA);
};

struct TFormEditorButtonStyleSet {
    TFormEditorButtonColors disabledColors;
    TFormEditorButtonColors idleColors;
    TFormEditorButtonColors hoverColors;
    TFormEditorButtonColors highlightIdleColors;
    TFormEditorButtonColors highlightHoverColors;
    float fontSize;

    TFormEditorButtonStyleSet();
};

struct TFormEditorButton : public OpaqueWidget {
    enum TFormEditorButtonModes {
        DISABLED_MODE,
        IDLE_MODE,
        HOVER_MODE,
        HIGHLIGHT_IDLE_MODE,
        HIGHLIGHT_HOVER_MODE,
        NUM_BUTTON_MODES
    };
    TFormEditorButtonModes mode;
    TFormEditorButtonStyleSet style;
    std::string text;
    Vec textOffset;
    bool respondToMouse;

    std::function<void()> onClick;

    TFormEditorButton();
    void draw(const DrawArgs& args) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void onDragEnd(const event::DragEnd& e) override;
    void applyStyle(const TFormEditorButtonStyleSet& newStyle);
    void setEnable(bool enable);
    void setHighlight(bool highlight);
    void setFilled(bool filled);
protected:
    TFormEditorButtonColors colors;
    bool isEnabled;
    bool isHighlighted;
    bool isFilled;
    void setMode(const TFormEditorButtonModes& newMode);
};

template<class T = TFormEditorButton>
T* createNewMenuButton(const std::string& text,
                       const std::function<void()>& onClickCallback,
                       int x, int y, int width, int height) {
    T* newButton = createWidget<T>(Vec(x, y));
    newButton->text = text;
    newButton->box.size.x = width;
    newButton->box.size.y = height;
    newButton->onClick = onClickCallback;
    return newButton;
}

#endif
