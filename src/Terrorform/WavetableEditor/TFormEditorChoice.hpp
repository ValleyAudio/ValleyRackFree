#ifndef TFORM_EDITOR_NUMBER_CHOICE_HPP
#define TFORM_EDITOR_NUMBER_CHOICE_HPP
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"

struct TFormEditorChoiceItem : MenuItem {
    unsigned long _itemNumber;
    std::shared_ptr<unsigned long> choice;

    std::function<void()> onChangeCallback;

    TFormEditorChoiceItem(unsigned long itemNumber);
    void onAction(const event::Action &e) override;
    void onDragEnd(const event::DragEnd &e) override;
    void addOnChangeCallback(const std::function<void()>& onChangeCallback);
};

struct TFormEditorChoice : ChoiceButton {
    std::shared_ptr<unsigned long> choice;
    std::vector<std::string> items;
    int maxItems;

    std::shared_ptr<Font> font;
    NVGcolor boxColor;
    NVGcolor arrowColor;
    NVGcolor textColor;
    NVGcolor boxOnEnterColor;
    NVGcolor boxOnLeaveColor;
    NVGcolor arrowOnEnterColor;
    NVGcolor arrowOnLeaveColor;

    std::function<void()> onChangeCallback;

    TFormEditorChoice();
    void onAction(const event::Action &e) override;
    void onEnter(const event::Enter &e) override;
    void onLeave(const event::Leave &e) override;
    void draw(const DrawArgs& args) override;
    int getChoice() const;
};

#endif
