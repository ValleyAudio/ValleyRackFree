#ifndef DSJ_VALLEY_WIDGETS_HPP
#define DSJ_VALLEY_WIDGETS_HPP

#include "Valley.hpp"

// Dynamic Panel

struct PanelBorderWidget : TransparentWidget {
	void draw(NVGcontext *vg) override;
};

struct DynamicPanelWidget : FramebufferWidget {
    int* mode;
    int oldMode;
    std::vector<std::shared_ptr<SVG>> panels;
    SVGWidget* visiblePanel;
    PanelBorderWidget* border;

    DynamicPanelWidget();
    void addPanel(std::shared_ptr<SVG> svg);
    void step() override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// View mode

enum DynamicViewMode2 {
    ACTIVE_HIGH_VIEW,
    ACTIVE_LOW_VIEW,
    ALWAYS_ACTIVE_VIEW
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Switch

struct DynamicSwitchWidget : virtual ParamWidget, FramebufferWidget {
    std::vector<std::shared_ptr<SVG>> frames;
    SVGWidget* sw;
    int* visibility;
    DynamicViewMode2 viewMode;

    DynamicSwitchWidget();
    void addFrame(std::shared_ptr<SVG> svg);
    void step() override;
    void onChange(EventChange &e) override;
};

template <class TDynamicSwitch>
DynamicSwitchWidget* createDynamicSwitchWidget(Vec pos, Module *module, int paramId,
                                               float minValue, float maxValue, float defaultValue,
                                               int* visibilityHandle, DynamicViewMode2 viewMode) {
	DynamicSwitchWidget *dynSwitch = new TDynamicSwitch();
	dynSwitch->box.pos = pos;
	dynSwitch->module = module;
	dynSwitch->paramId = paramId;
	dynSwitch->setLimits(minValue, maxValue);
	dynSwitch->setDefaultValue(defaultValue);
    dynSwitch->visibility = visibilityHandle;
    dynSwitch->viewMode = viewMode;
	return dynSwitch;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic text
struct DynamicText : TransparentWidget {
    std::shared_ptr<std::string> text;
    std::shared_ptr<Font> font;
    int size;
    int* visibility;
    DynamicViewMode2 viewMode;

    enum ColorMode {
        COLOR_MODE_WHITE = 0,
        COLOR_MODE_BLACK
    };
    int* colorHandle;
    NVGcolor textColor;

    DynamicText();
    virtual void draw(NVGcontext* vg) override;
    void step() override;
};

DynamicText* createDynamicText(const Vec& pos, int size, std::string text,
                               int* visibilityHandle, DynamicViewMode2 viewMode);
DynamicText* createDynamicText(const Vec& pos, int size, std::string text,
                               int* visibilityHandle, int* colorHandle, DynamicViewMode2 viewMode);
DynamicText* createDynamicText(const Vec& pos, int size, std::shared_ptr<std::string> text,
                               int* visibilityHandle, DynamicViewMode2 viewMode);
DynamicText* createDynamicText(const Vec& pos, int size, std::shared_ptr<std::string> text,
                               int* visibilityHandle, int* colorHandle, DynamicViewMode2 viewMode);

struct DynamicFrameText : DynamicText {
    int* itemHandle;
    std::vector<std::string> textItem;

    DynamicFrameText();
    void addItem(const std::string& item);
    void draw(NVGcontext* vg) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Choices

struct DynamicItem : MenuItem {
    unsigned long _itemNumber;
    unsigned long* _choice;
    DynamicItem(unsigned long itemNumber);
    void onAction(EventAction &e) override;
};

struct DynamicChoice : ChoiceButton {
    unsigned long* _choice;
    std::vector<std::string> _items;
    std::shared_ptr<std::string> _text;
    std::shared_ptr<Font> _font;
    int* _visibility;
    int _textSize;
    DynamicViewMode2 _viewMode;
    DynamicChoice();
    void step() override;
    void onAction(EventAction &e) override;
    void draw(NVGcontext* vg) override;
};

DynamicChoice* createDynamicChoice(const Vec& pos,
                                   float width,
                                   const std::vector<std::string>& items,
                                   unsigned long* choiceHandle,
                                   int* visibilityHandle,
                                   DynamicViewMode2 viewMode);

#endif
