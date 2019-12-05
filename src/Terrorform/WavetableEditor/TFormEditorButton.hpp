#ifndef TFORM_EDITOR_BUTTON_HPP
#define TFORM_EDITOR_BUTTON_HPP
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"

enum TFormEditorButtonModes {
    IDLE_MODE,
    HOVER_MODE,
    HIGHLIGHT_MODE,
    HIGHLIGHT_HOVER_MODE,
    NUM_BUTTON_MODES
};

struct TFormEditorButtonStyle {
    NVGcolor textColor;
	NVGcolor bgColor;
    NVGcolor borderColor;

    TFormEditorButtonStyle();
    TFormEditorButtonStyle(const NVGcolor& newTextColor,
                           const NVGcolor& newBgColor,
                           const NVGcolor& newBorderColor);
};

enum TFormEditorStyles {
    RED_EDITOR_STYLE,
    YELLOW_EDITOR_STYLE,
    GREEN_EDITOR_STYLE,
    BLUE_EDITOR_STYLE,
    WHITE_EDITOR_STYLE,
    NUM_EDITOR_STYLES
};

struct TFormEditorButton : public OpaqueWidget {
    std::string text;
    std::shared_ptr<Font> font;
    NVGcolor color;
	NVGcolor bgColor;
    NVGcolor borderColor;
    Vec textOffset;
    float fontSize;
    bool enabled;

    TFormEditorButtonStyle buttonStyles[NUM_BUTTON_MODES];
    TFormEditorButtonModes mode;
    TFormEditorButtonModes modeOffset;

    std::function<void()> onClick;

    TFormEditorButton();
    void draw(const DrawArgs& args) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void onDragEnd(const event::DragEnd& e) override;
    void setHighlight(bool highlight);
};

TFormEditorButton* createNewMenuButton(const std::string& text,
                                       const std::function<void()>& onClickCallback,
                                       int x, int y, int width, int height);

#endif
