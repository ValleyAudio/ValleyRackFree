#ifndef DSJ_VALLEY_TRIXIE_ROGAN_HPP
#define DSJ_VALLEY_TRIXIE_ROGAN_HPP
#include "DynamicKnob.hpp"
using namespace std;

struct DexterDynamicSVGSwitch : virtual Switch, FramebufferWidget {
	std::vector<std::shared_ptr<SVG>> frames;
	/** Not owned */
	SVGWidget *sw;
    int* visibility;
    DynamicViewMode viewMode;

    DexterDynamicSVGSwitch() {
        visibility = nullptr;
        viewMode = ACTIVE_HIGH;
        sw = new SVGWidget();
        addChild(sw);
    }
	/** Adds an SVG file to represent the next switch position */
	void addFrame(std::shared_ptr<SVG> svg) {
    	frames.push_back(svg);
    	// If this is our first frame, automatically set SVG and size
    	if (!sw->svg) {
    		sw->setSVG(svg);
    		box.size = sw->box.size;
    	}
    }

	void step() override {
        if(visibility != nullptr) {
            if(*visibility) {
                visible = true;
            }
            else {
                visible = false;
            }
            if(viewMode == ACTIVE_LOW) {
                visible = !visible;
            }
        }
        else {
            visible = true;
        }
    	FramebufferWidget::step();
    }

	void onChange(EventChange &e) override {
        assert(frames.size() > 0);
    	float valueScaled = rescalef(value, minValue, maxValue, 0, frames.size() - 1);
    	int index = clampi((int) roundf(valueScaled), 0, frames.size() - 1);
    	sw->setSVG(frames[index]);
    	dirty = true;
    	Switch::onChange(e);
    }
};

template <class TDynamicSwitch>
DexterDynamicSVGSwitch* createDexterDynamicSVGSwitch(Vec pos, Module *module, int paramId,
                                         float minValue, float maxValue, float defaultValue,
                                         int* visibilityHandle, DynamicViewMode viewMode) {
	DexterDynamicSVGSwitch *dynSwitch = new TDynamicSwitch();
	dynSwitch->box.pos = pos;
	dynSwitch->module = module;
	dynSwitch->paramId = paramId;
	dynSwitch->setLimits(minValue, maxValue);
	dynSwitch->setDefaultValue(defaultValue);
    dynSwitch->visibility = visibilityHandle;
    dynSwitch->viewMode = viewMode;
	return dynSwitch;
}

struct DynamicModuleLightWidget : MultiLightWidget {
	Module *module = NULL;
	int firstLightId;
    int* visibility = nullptr;
    DynamicViewMode viewMode = ACTIVE_HIGH;

	void step() override{
        if(visibility != nullptr) {
            if(*visibility) {
                visible = true;
            }
            else {
                visible = false;
            }
            if(viewMode == ACTIVE_LOW) {
                visible = !visible;
            }
        }
        else {
            visible = true;
        }

    	assert(module);
    	assert(module->lights.size() >= firstLightId + baseColors.size());
    	std::vector<float> values(baseColors.size());

    	for (size_t i = 0; i < baseColors.size(); i++) {
    		float value = module->lights[firstLightId + i].getBrightness();
    		value = clampf(value, 0.0, 1.0);
    		values[i] = value;
    	}
    	setValues(values);
    }
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

struct RoganMedGreen : Rogan {
    RoganMedGreen() {
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSGreen.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct RoganSmallGreen : Rogan {
    RoganSmallGreen() {
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSGreen.svg")));
        box.size = Vec(19.8, 19.8);
    }
};

struct RoganMedBlue : Rogan {
    RoganMedBlue() {
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSBlue.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct RoganMedBlueSnap : Rogan {
    RoganMedBlueSnap() {
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSBlue.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
        snap = true;
    }
};

struct RoganSmallBlue : Rogan {
    RoganSmallBlue() {
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSBlue.svg")));
        box.size = Vec(19.8, 19.8);
    }
};

struct RoganMedRed : Rogan {
    RoganMedRed() {
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSRed.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct RoganSmallRed : Rogan {
    RoganSmallRed() {
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSRed.svg")));
        box.size = Vec(19.8, 19.8);
    }
};


struct Rogan1PSPurple : Rogan {
    Rogan1PSPurple() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSPurple.svg")));
    }
};

struct RoganMedPurple : Rogan {
    RoganMedPurple() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSPurple.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct RoganSmallPurple : Rogan {
    RoganSmallPurple() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSPurple.svg")));
        box.size = Vec(19.8, 19.8);
    }
};

struct Rogan1PSMustard : Rogan {
    Rogan1PSMustard() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSMustard.svg")));
    }
};

struct RoganSmallMustard : Rogan {
    RoganSmallMustard() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSMustard.svg")));
        box.size = Vec(19.8, 19.8);
    }
};

struct Rogan1PSOrange : Rogan {
    Rogan1PSOrange() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSOrange.svg")));
    }
};

struct RoganMedOrange : Rogan {
    RoganMedOrange() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSOrange.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct RoganSmallOrange : Rogan {
    RoganSmallOrange() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSOrange.svg")));
        box.size = Vec(19.8, 19.8);
    }
};

struct DexterLightLEDButton : DexterDynamicSVGSwitch, MomentarySwitch {
    DexterLightLEDButton() {
        addFrame(SVG::load(assetPlugin(plugin, "res/LightLEDButton80.svg")));
    }
};

struct DynRogan1PSRed : DynamicKnob {
    DynRogan1PSRed() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSRed.svg")));
    }
};

struct DynRogan1PSBlue : DynamicKnob {
    DynRogan1PSBlue() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSBlue.svg")));
    }
};

struct DynRogan1PSGreen : DynamicKnob {
    DynRogan1PSGreen() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSGreen.svg")));
    }
};

struct DynRoganMedRed : DynamicKnob {
    DynRoganMedRed() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSRed.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct DynRoganMedBlue : DynamicKnob {
    DynRoganMedBlue() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSBlue.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct DynRoganMedGreen : DynamicKnob {
    DynRoganMedGreen() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSVG(SVG::load(assetGlobal("res/ComponentLibrary/Rogan1PSGreen.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct DynRoganMedPurple : DynamicKnob {
    DynRoganMedPurple() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSVG(SVG::load(assetPlugin(plugin, "res/Rogan1PSPurple.svg")));
        box.size.x *= 0.8;
        box.size.y *= 0.8;
    }
};

struct RedDynamicLight : DynamicModuleLightWidget {
	RedDynamicLight() {
		addBaseColor(COLOR_RED);
	}
};

struct PJ301MDarkPort : SVGPort {
	PJ301MDarkPort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/PJ301MDark.svg"));
		background->wrap();
		box.size = background->box.size;
	}
};

#endif
