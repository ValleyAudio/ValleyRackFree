#pragma once
#include "../../Valley.hpp"
#include <app/LedDisplay.hpp>

struct TFormTextField : TextField {
    std::shared_ptr<Font> font;
    NVGcolor bgColor;
    NVGcolor color;
    NVGcolor textColor;
    bool enabled;
    bool selected;

    std::function<void(const std::string&)> onDeselectCallback;

    TFormTextField();
    void draw(const DrawArgs& args) override;
    void onSelect(const event::Select& e) override;
    void onDeselect(const event::Deselect& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    std::string getText() const;
};

struct TFormNumberField : TextField {
    std::shared_ptr<Font> font;
    NVGcolor bgColor;
    NVGcolor color;
    NVGcolor textColor;
    bool enabled;
    bool selected;

    int minimum;
    int maximum;
    int value;

    std::function<void()> onChangeCallback;

    TFormNumberField();
    void draw(const DrawArgs& args) override;
    void onSelect(const event::Select& e) override;
    void onDeselect(const event::Deselect& e) override;
    void onAction(const event::Action& e) override;
    void onDragMove(const event::DragMove& e) override;
    void onEnter(const event::Enter& e) override;
    void onLeave(const event::Leave& e) override;
    void setValue(int newValue);
private:
    void updateText(const std::string& newText);
    std::string prevText;
};
