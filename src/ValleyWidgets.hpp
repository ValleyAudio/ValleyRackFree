#ifndef DSJ_VALLEY_WIDGETS_HPP
#define DSJ_VALLEY_WIDGETS_HPP

#include "Valley.hpp"
#include "window.hpp"
#include <functional>

// Dynamic Panel

struct PanelBorderWidget : TransparentWidget {
	void draw(const DrawArgs &args) override;
};

struct DynamicPanelWidget : FramebufferWidget {
    int* mode;
    int oldMode;
    std::vector<std::shared_ptr<Svg>> panels;
    SvgWidget* visiblePanel;
    PanelBorderWidget* border;

    DynamicPanelWidget();
    void addPanel(std::shared_ptr<Svg> svg);
    void step() override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// View mode

enum DynamicViewMode {
    ACTIVE_HIGH_VIEW,
    ACTIVE_LOW_VIEW,
    ALWAYS_ACTIVE_VIEW
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Switch

struct DynamicSwitchWidget : SvgSwitch {
    int* _visibility;
    DynamicViewMode _viewMode;

    DynamicSwitchWidget();
    void step() override;
};

template <class TDynamicSwitch>
DynamicSwitchWidget* createDynamicSwitchWidget(Vec pos, Module *module, int paramId,
                                               float minValue, float maxValue, float defaultValue,
                                               int* visibilityHandle, DynamicViewMode viewMode) {
	DynamicSwitchWidget *dynSwitch = new TDynamicSwitch();
	dynSwitch->box.pos = pos;
	dynSwitch->paramQuantity->paramId = paramId;
    if (module) {
		module->configParam(paramId, minValue, maxValue, defaultValue);
	}
    dynSwitch->_visibility = visibilityHandle;
    dynSwitch->_viewMode = viewMode;
	return dynSwitch;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic lights

struct DynamicModuleLightWidget : MultiLightWidget {
	Module *module = NULL;
	int firstLightId;
    int* visibility = nullptr;
    DynamicViewMode viewMode = ACTIVE_HIGH_VIEW;

	void step() override;
};

template<class TDynamicModuleLightWidget>
DynamicModuleLightWidget *createDynamicLight(Vec pos, Module *module, int firstLightId,
                                             int* visibilityHandle, DynamicViewMode viewMode) {
	DynamicModuleLightWidget *light = new TDynamicModuleLightWidget();
	light->box.pos = pos;
	light->module = module;
	light->firstLightId = firstLightId;
    light->visibility = visibilityHandle;
    light->viewMode = viewMode;
	return light;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic knob

enum DynamicKnobMotion {
    SMOOTH_MOTION,
    SNAP_MOTION
};

struct DynamicKnob : virtual Knob {
	/** Angles in radians */
	float minAngle, maxAngle;
	/** Not owned */
    FramebufferWidget *fb;
	TransformWidget *tw;
	SvgWidget *sw;
    CircularShadow *shadow;
    int* _visibility;
    DynamicViewMode _viewMode;

	DynamicKnob();
	void setSvg(std::shared_ptr<Svg> svg);
};

struct DynamicSvgKnob : SvgKnob {
    int* _visibility;
    DynamicViewMode _viewMode;

	DynamicSvgKnob();
	void step() override;
};

template <class TParamWidget>
TParamWidget *createDynamicParam(math::Vec pos, engine::Module *module,
                                 int paramId, int* visibilityHandle,
                                 DynamicViewMode viewMode,
                                 DynamicKnobMotion motion) {
	TParamWidget *o = new TParamWidget;
	o->box.pos = pos;
	if (module) {
		o->paramQuantity = module->paramQuantities[paramId];
        o->_visibility = visibilityHandle;
	}
    o->_viewMode = viewMode;
    if(motion == SNAP_MOTION) {
        o->snap = true;
    }
	return o;
}

template <class TParamWidget>
TParamWidget *createDynamicParam(math::Vec pos, engine::Module *module,
                                 int paramId, int* visibilityHandle,
                                 DynamicViewMode viewMode) {
	TParamWidget *o = new TParamWidget;
	o->box.pos = pos;
	if (module) {
		o->paramQuantity = module->paramQuantities[paramId];
        o->_visibility = visibilityHandle;
	}
    o->_viewMode = viewMode;
	return o;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct SvgStepSlider : app::SvgSlider {
	void onChange(const event::Change& e) override;
};

struct PlainText : TransparentWidget {
    std::string text;
    std::shared_ptr<Font> font;
    NVGcolor color;
    NVGalign horzAlignment;
    NVGalign vertAlignment;
    int size;

    PlainText();
    void draw(const DrawArgs &args) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic text

struct DynamicText : TransparentWidget {
    std::shared_ptr<std::string> text;
    std::shared_ptr<Font> font;
    int size;
    float blur;
    int* visibility;
    DynamicViewMode viewMode;

    enum ColorMode {
        COLOR_MODE_WHITE = 0,
        COLOR_MODE_BLACK,
        COLOR_MODE_RED
    };

    enum FontMode {
        FONT_MODE_ALTEDIN = 0,
        FONT_MODE_7SEG
    };

    int* colorHandle;
    NVGcolor textColor;
    NVGcolor customColor;
    NVGalign horzAlignment;
    NVGalign vertAlignment;

    DynamicText();
    virtual void draw(const DrawArgs &args) override;
    void step() override;
    void setFont(const FontMode& newFontMode);
};

DynamicText* createDynamicText(const Vec& pos, int size, std::string text,
                               int* visibilityHandle, DynamicViewMode viewMode);
DynamicText* createDynamicText(const Vec& pos, int size, std::string text,
                               int* visibilityHandle, int* colorHandle, DynamicViewMode viewMode);
DynamicText* createDynamicText(const Vec& pos, int size, std::shared_ptr<std::string> text,
                               int* visibilityHandle, DynamicViewMode viewMode);
DynamicText* createDynamicText(const Vec& pos, int size, std::shared_ptr<std::string> text,
                               int* visibilityHandle, int* colorHandle, DynamicViewMode viewMode);

struct DynamicFrameText : DynamicText {
    int* itemHandle;
    std::vector<std::string> textItem;

    DynamicFrameText();
    void addItem(const std::string& item);
    void draw(const DrawArgs &args) override;
};

template<class T>
class DynamicValueText : public TransformWidget {
public:
    std::shared_ptr<Font> font;
    int size;
    int* visibility;
    DynamicViewMode viewMode;

    enum ColorMode {
        COLOR_MODE_WHITE = 0,
        COLOR_MODE_BLACK
    };
    int* colorHandle;
    NVGcolor textColor;

    DynamicValueText(std::shared_ptr<T> value, std::function<std::string(T)> valueToText)  {
        font = APP->window->loadFont(asset::plugin(pluginInstance, "res/din1451alt.ttf"));
        size = 16;
        visibility = nullptr;
        colorHandle = nullptr;
        viewMode = ACTIVE_HIGH_VIEW;
        _value = value;
        _valueToText = valueToText;
    }

    void draw(const DrawArgs &args) override {
        nvgFontSize(args.vg, size);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.f);
        Vec textPos = Vec(0.f, 0.f);
        if(colorHandle != nullptr) {
            switch((ColorMode)*colorHandle) {
                case COLOR_MODE_WHITE: textColor = nvgRGB(0xFF,0xFF,0xFF); break;
                case COLOR_MODE_BLACK: textColor = nvgRGB(0x14,0x14, 0x14); break;
                default: textColor = nvgRGB(0xFF,0xFF,0xFF);
            }
        }
        else {
            textColor = nvgRGB(0xFF,0xFF,0xFF);
        }

        nvgFillColor(args.vg, textColor);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
        nvgText(args.vg, textPos.x, textPos.y, _text.c_str(), NULL);
    }

    void step() override {
        if(_value == nullptr) {
            return;
        }
        _text = _valueToText(*_value);
        if(visibility != nullptr) {
            if(*visibility) {
                visible = true;
            }
            else {
                visible = false;
            }
            if(viewMode == ACTIVE_LOW_VIEW) {
                visible = !visible;
            }
        }
    }
private:
    std::shared_ptr<T> _value;
    std::function<std::string(T)> _valueToText;
    std::string _text;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Choices

struct DynamicItem : MenuItem {
    unsigned long _itemNumber;
    unsigned long* _choice;
    std::function<void(int)> updateChoice;
    DynamicItem(unsigned long itemNumber);
    void onAction(const event::Action &e) override;
    void step() override;
};

struct DynamicChoice : ChoiceButton {
    DynamicViewMode _viewMode;
    DynamicChoice();
    void step() override;
    void onAction(const event::Action &e) override;
    void onEnter(const event::Enter &e) override;
    void onLeave(const event::Leave &e) override;
    void draw(const DrawArgs &args) override;

    unsigned long* _choice;
    long _oldChoice;
    std::vector<std::string> _items;
    std::shared_ptr<std::string> _text;
    std::shared_ptr<Font> _font;
    bool _transparent;
    int* _visibility;
    int _textSize;

    std::function<void()> onMouseEnter;
    std::function<void()> onMouseLeave;
    std::function<void()> onOpen;
    std::function<void(int)> updateChoice;
};

DynamicChoice* createDynamicChoice(const Vec& pos,
                                   float width,
                                   const std::vector<std::string>& items,
                                   unsigned long* choiceHandle,
                                   int* visibilityHandle,
                                   DynamicViewMode viewMode);

template<class T = SvgKnob>
T *createValleyKnob(Vec pos, Module *module, int paramId, float minAngle, float maxAngle, DynamicKnobMotion motion) {
    T *o = createParam<T>(pos, module, paramId);
    o->minAngle = minAngle;
    o->maxAngle = maxAngle;
    if(motion == SNAP_MOTION) {
        o->snap = true;
    }
	return o;
}

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
    std::shared_ptr<Font> _font;
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

#endif
