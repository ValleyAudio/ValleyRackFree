#ifndef DSJ_VALLEY_COMPONENTS_HPP
#define DSJ_VALLEY_COMPONENTS_HPP

#include "ValleyWidgets.hpp"

struct Rogan1PSBrightRed : Rogan {
    Rogan1PSBrightRed() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Rogan1PSBrightRed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Rogan1PS-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Rogan1PSBrightRed-fg.svg")));
    }
};

struct RoganSmallBrightRed : Rogan {
    RoganSmallBrightRed() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSBrightRedSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSBrightRedSmall-fg.svg")));
    }
};

struct Rogan1PSYellow : Rogan {
    Rogan1PSYellow() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/v2/Rogan1PSYellow.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Rogan1PS-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Rogan1PSYellow-fg.svg")));
    }
};

struct RoganSmallYellow : Rogan {
    RoganSmallYellow() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSYellowSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSYellowSmall-fg.svg")));
    }
};

struct RoganMedWhite : Rogan {
    RoganMedWhite() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSWhiteMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSWhiteMed-fg.svg")));
    }
};

struct RoganMedSmallWhite : Rogan {
    RoganMedSmallWhite() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSWhiteMedSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMedSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSWhiteMedSmall-fg.svg")));
    }
};

struct RoganSmallWhite : Rogan {
    RoganSmallWhite() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSWhiteSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSWhiteSmall-fg.svg")));
    }
};

struct RoganMedGreen : Rogan {
    RoganMedGreen() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSGreenMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSGreenMed-fg.svg")));
    }
};

struct RoganMedGreenWithModeText : ValleyRogan {
    RoganMedGreenWithModeText() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSGreenMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSGreenMed-fg.svg")));
    }
};

struct RoganSmallGreen : Rogan {
    RoganSmallGreen() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSGreenSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSGreenSmall-fg.svg")));
    }
};

struct RoganMedBlue : Rogan {
    RoganMedBlue() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSBlueMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSBlueMed-fg.svg")));
    }
};

struct RoganMedSmallBlue : Rogan {
    RoganMedSmallBlue() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSBlueMedSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMedSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSBlueMedSmall-fg.svg")));
    }
};

struct RoganMedBlueSnap : Rogan {
    RoganMedBlueSnap() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBlueMed.svg")));
        snap = true;
    }
};

struct RoganMedSmallBlueSnap : Rogan {
    RoganMedSmallBlueSnap() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBlueMedSmall.svg")));
        snap = true;
    }
};

struct RoganSmallBlue : Rogan {
    RoganSmallBlue() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSBlueSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSBlueSmall-fg.svg")));
    }
};

struct RoganMedRed : Rogan {
    RoganMedRed() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSRedMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSRedMed-fg.svg")));
    }
};

struct RoganMedRedWithModeText : ValleyRogan {
    RoganMedRedWithModeText() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSRedMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSRedMed-fg.svg")));
    }
};

struct RoganSmallRed : Rogan {
    RoganSmallRed() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSRedSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSRedSmall-fg.svg")));
    }
};

struct Rogan1PSPurple : Rogan {
    Rogan1PSPurple() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSPurple.svg")));
    }
};

struct RoganMedPurple : Rogan {
    RoganMedPurple() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSPurpleMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSPurpleMed-fg.svg")));
    }
};

struct RoganMedPurpleWithModeText : ValleyRogan {
    RoganMedPurpleWithModeText() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSPurpleMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSPurpleMed-fg.svg")));
    }
};

struct RoganSmallPurple : Rogan {
    RoganSmallPurple() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSPurpleSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSPurpleSmall-fg.svg")));
    }
};

struct Rogan1PSMustard : Rogan {
    Rogan1PSMustard() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSMustard.svg")));
    }
};

struct RoganMedMustard : Rogan {
    RoganMedMustard() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMustardMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMustardMed-fg.svg")));
    }
};

struct RoganSmallMustard : Rogan {
    RoganSmallMustard() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSMustardSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSMustardSmall-fg.svg")));
    }
};

struct Rogan1PSOrange : Rogan {
    Rogan1PSOrange() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/v2/Rogan1PSOrange.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Rogan1PS-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Rogan1PSOrange-fg.svg")));
    }
};

struct RoganMedOrange : Rogan {
    RoganMedOrange() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSOrangeMed.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSMed-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Med/Rogan1PSOrangeMed-fg.svg")));
    }
};

struct RoganSmallOrange : Rogan {
    RoganSmallOrange() {
        setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSOrangeSmall.svg")));
        bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSSmall-bg.svg")));
        fg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/v2/Small/Rogan1PSOrangeSmall-fg.svg")));
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct LightLEDButton : SvgSwitch {
    LightLEDButton() {
        momentary = true;
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LightLEDButton80.svg")));
    }
};

struct LightLEDButtonNonDyn : SvgSwitch {
    LightLEDButtonNonDyn() {
        momentary = true;
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LightLEDButton80.svg")));
    }
};

struct LightLEDButton2 : SvgSwitch {
    std::function<void()> onClick;
    LightLEDButton2() {
        momentary = true;
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LightLEDButton80.svg")));
    }
    void onDragEnd(const event::DragEnd& e) override {
        if(onClick) {
            onClick();
        }
        SvgSwitch::onDragEnd(e);
    }
};

struct LightLEDButton3 : SvgSwitch {
    std::function<void()> onClick;
    LightLEDButton3() {
        momentary = true;
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LightLEDButton.svg")));
    }
};

struct LightLEDButtonWithModeText : SvgSwitch {
    std::function<void()> onClick;
    std::shared_ptr<std::string> modeText;
    NonValueParamTooltip* tooltip;

    LightLEDButtonWithModeText() {
        momentary = true;
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LightLEDButton80.svg")));
        modeText = std::make_shared<std::string>();
        tooltip = NULL;
    }

    void onDragEnd(const event::DragEnd& e) override {
        if(onClick) {
            onClick();
        }
        SvgSwitch::onDragEnd(e);
    }

    void onEnter(const event::Enter& e) override {
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

struct RedDynamicLight : DynamicModuleLightWidget {
    RedDynamicLight() {
        addBaseColor(SCHEME_RED);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct PJ301MDarkPort : SvgPort {
    PJ301MDarkPort() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PJ301MDark.svg")));
    }
};

struct PJ301MDarkSmall : SvgPort {
    PJ301MDarkSmall() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PJ301MDarkSmall.svg")));
    }
};

struct PJ301MDarkSmallOut : SvgPort {
    PJ301MDarkSmallOut() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PJ301MDarkSmallOut.svg")));
    }
};

struct ValleySlider : SvgSlider {
    Vec margin = Vec(-1, -0.55);
    ValleySlider() {
        background->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/valleySliderBackground.svg"));
        background->wrap();
        background->box.pos = margin;
        box.size = background->box.size.plus(margin.mult(2));
    }
};

struct ValleyStepSlider : SvgStepSlider {
    Vec margin = Vec(-1, -0.55);
    ValleyStepSlider() {
        background->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/valleySliderBackground.svg"));
        background->wrap();
        background->box.pos = margin;
        box.size = background->box.size.plus(margin.mult(2));
    }
};

struct RedSlider : ValleySlider {
    RedSlider() {
    handle->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/sliderRed.svg"));
    handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
        minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
    }
};

struct OrangeSlider : ValleySlider {
    OrangeSlider() {
    handle->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/sliderOrange.svg"));
    handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
        minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
    }
};

struct YellowSlider : ValleySlider {
    YellowSlider() {
    handle->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/sliderGreen.svg"));
    handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
        minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
    }
};

struct GreenSlider : ValleySlider {
    GreenSlider() {
    handle->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/sliderGreen.svg"));
    handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
        minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
    }
};

struct BlueSlider : ValleySlider {
    BlueSlider() {
    handle->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/sliderBlue.svg"));
    handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
        minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
    }
};

struct YellowStepSlider : ValleyStepSlider {
    YellowStepSlider() {
    handle->svg = APP->window->loadSvg(asset::plugin(pluginInstance,"res/sliderYellow.svg"));
    handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 0.5).plus(margin);
        minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
    }
};

#endif // DSJ_VALLEY_COMPONENTS_HPP
