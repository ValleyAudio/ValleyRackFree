#ifndef TFORM_EDITOR_NUMBER_CHOICE_HPP
#define TFORM_EDITOR_NUMBER_CHOICE_HPP
#include "../../Valley.hpp"
#include "../../ValleyComponents.hpp"

struct TFormEditorNumberItem : MenuItem {
    unsigned long _itemNumber;
    std::shared_ptr<unsigned long> choice;

    TFormEditorNumberItem(unsigned long itemNumber);
    void onAction(const event::Action &e) override;
    void step() override;
};

struct TFormEditorNumberChoice : ChoiceButton {
    std::shared_ptr<unsigned long> choice;
    unsigned long range;

    std::shared_ptr<Font> font;
    NVGcolor boxColor;
    NVGcolor arrowColor;
    NVGcolor textColor;
    NVGcolor boxOnEnterColor;
    NVGcolor boxOnLeaveColor;
    NVGcolor arrowOnEnterColor;
    NVGcolor arrowOnLeaveColor;

    TFormEditorNumberChoice();
    void onAction(const event::Action &e) override;
    void onEnter(const event::Enter &e) override;
    void onLeave(const event::Leave &e) override;
    void draw(const DrawArgs& args) override;
};

#endif
