//
// Most of the code here is janky AF! Most will be deprecated so that I can instead use 'vanilla'
// Rack API code where possible, and of course make it polymorphic and more maintainable and
// minimise the use of pointers.
//

#ifndef DSJ_VALLEY_WIDGETS_HPP
#define DSJ_VALLEY_WIDGETS_HPP

#include "Valley.hpp"
#include <functional>
#include <settings.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////

struct SvgStepSlider : app::SvgSlider {
	void onChange(const event::Change& e) override;
};

struct PlainText : TransparentWidget {
    std::string fontPath = "res/ShareTechMono-Regular.ttf";
    std::string text;
    NVGcolor color;
    NVGalign horzAlignment;
    NVGalign vertAlignment;
    int size;

    PlainText();
    void draw(const DrawArgs &args) override;
};

////////////////////////////////////////////////////////////////////////////////
// Dynamic Menu (An improved dynamic choice)

struct DynamicMenuItem : MenuItem {
    DynamicMenuItem(int itemNumber);
    void onAction(const event::Action &e) override;
    int* _choice;
    int _itemNumber;
    bool _showTick;
    void step() override;
    std::function<void(int)> setChoice;
};

struct DynamicSubMenu : MenuItem {
    Menu* createChildMenu() override;
	std::vector<std::string> items;
	int itemOffset;
	int* choice;
	bool showTick;
	std::function<void(int)> setChoice;
};

struct DynamicMenu : ChoiceButton {
    DynamicMenu();
    void step() override;
    void onAction(const event::Action &e) override;
    void onEnter(const event::Enter &e) override;
    void onLeave(const event::Leave &e) override;
    void draw(const DrawArgs &args) override;

    int _choice;
	int _subMenuGroupSize;
    std::vector<std::string> _items;
    std::shared_ptr<std::string> _text;
    bool _isTransparent;
    bool _showTick;
    int _textSize;

    std::function<void()> onMouseEnter;
    std::function<void()> onMouseLeave;
    std::function<void()> onOpen;
    std::function<void(int)> setChoice;
};

DynamicMenu* createDynamicMenu(const Vec& pos, const Vec& size,
                               const std::vector<std::string>& items,
                               bool isTransparent, bool showTick, int subMenuGroupSize);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-Value / Custom Text Tooltips

struct NonValueParamTooltip : ui::Tooltip {
    ParamWidget* paramWidget;
    std::shared_ptr<std::string> nonValueText;

    void step() override {
        ParamQuantity* paramQuantity = paramWidget->getParamQuantity();
        if (paramQuantity) {
            // Quantity string
            text = paramQuantity->getLabel();
            text += ": ";

            if (nonValueText) {
                text += *nonValueText;
            }

            // Param description
            std::string description = paramQuantity->description;
            if (!description.empty()) {
                text += "\n" + description;
            }
        }
        Tooltip::step();
        // Position at bottom-right of parameter
        box.pos = paramWidget->getAbsoluteOffset(paramWidget->box.size).round();
        // Fit inside parent (copied from Tooltip.cpp)
        assert(parent);
        box = box.nudge(parent->box.zeroPos());
    }
};

struct ValleyRogan : Rogan {
    std::shared_ptr<std::string> modeText;
    NonValueParamTooltip* tooltip;

    ValleyRogan() {
        modeText = std::make_shared<std::string>();
        tooltip = NULL;
    }

    void onEnter(const EnterEvent& e) override {
        if (!settings::tooltips) {
            return;
        }
        if (tooltip) {
            return;
        }
        if (!module) {
            return;
        }
        NonValueParamTooltip* paramTooltip = new NonValueParamTooltip;
        paramTooltip->nonValueText = modeText;
        paramTooltip->paramWidget = this;
        APP->scene->addChild(paramTooltip);
        tooltip = paramTooltip;
    }

    void onLeave(const LeaveEvent& e) override {
        if (!tooltip) {
            return;
        }
        APP->scene->removeChild(tooltip);
        delete tooltip;
        tooltip = NULL;
    }

    void setModeText(const std::string& newModeText) {
        if (modeText) {
            *modeText = newModeText;
        }
    }
};

#endif
