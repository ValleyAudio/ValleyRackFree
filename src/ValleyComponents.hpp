#ifndef DSJ_VALLEY_COMPONENTS_HPP
#define DSJ_VALLEY_COMPONENTS_HPP

#include "ValleyWidgets.hpp"

struct Rogan1PSBrightRed : Rogan {
    Rogan1PSBrightRed() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBrightRed.svg")));
    }
};

struct RoganSmallBrightRed : Rogan {
    RoganSmallBrightRed() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBrightRedSmall.svg")));
    }
};

struct Rogan1PSYellow : Rogan {
    Rogan1PSYellow() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSYellow.svg")));
    }
};

struct RoganSmallYellow : Rogan {
    RoganSmallYellow() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSYellowSmall.svg")));
    }
};

struct RoganMedWhite : Rogan {
    RoganMedWhite() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSWhiteMed.svg")));
    }
};

struct RoganMedSmallWhite : Rogan {
    RoganMedSmallWhite() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSWhiteMedSmall.svg")));
    }
};

struct RoganSmallWhite : Rogan {
    RoganSmallWhite() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSWhiteSmall.svg")));
    }
};

struct RoganMedGreen : Rogan {
    RoganMedGreen() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSGreenMed.svg")));
    }
};

struct RoganSmallGreen : Rogan {
    RoganSmallGreen() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSGreenSmall.svg")));
    }
};

struct RoganMedBlue : Rogan {
    RoganMedBlue() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBlueMed.svg")));
    }
};

struct RoganMedBlueSnap : Rogan {
    RoganMedBlueSnap() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBlueMed.svg")));
        snap = true;
    }
};

struct RoganSmallBlue : Rogan {
    RoganSmallBlue() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBlueSmall.svg")));
    }
};

struct RoganMedRed : Rogan {
    RoganMedRed() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSRedMed.svg")));
    }
};

struct RoganSmallRed : Rogan {
    RoganSmallRed() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSRedSmall.svg")));
    }
};

struct Rogan1PSPurple : Rogan {
    Rogan1PSPurple() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSPurple.svg")));
    }
};

struct RoganMedPurple : Rogan {
    RoganMedPurple() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSPurpleMed.svg")));
    }
};

struct RoganSmallPurple : Rogan {
    RoganSmallPurple() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSPurpleSmall.svg")));
    }
};

struct Rogan1PSMustard : Rogan {
    Rogan1PSMustard() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSMustard.svg")));
    }
};

struct RoganSmallMustard : Rogan {
    RoganSmallMustard() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSMustardSmall.svg")));
    }
};

struct Rogan1PSOrange : Rogan {
    Rogan1PSOrange() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSOrange.svg")));
    }
};

struct RoganMedOrange : Rogan {
    RoganMedOrange() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSOrangeMed.svg")));
    }
};

struct RoganSmallOrange : Rogan {
    RoganSmallOrange() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSOrangeSmall.svg")));
    }
};

struct DynRogan1PSRed : DynamicKnob {
    DynRogan1PSRed() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/Rogan1PSRed.svg")));
    }
};

struct DynRogan1PSBlue : DynamicKnob {
    DynRogan1PSBlue() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/Rogan1PSBlue.svg")));
    }
};

struct DynRogan1PSGreen : DynamicKnob {
    DynRogan1PSGreen() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/Rogan1PSGreen.svg")));
    }
};

struct DynRoganMedRed : DynamicKnob {
    DynRoganMedRed() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSRedMed.svg")));
    }
};

struct DynRoganMedBlue : DynamicKnob {
    DynRoganMedBlue() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBlueMed.svg")));
    }
};

struct DynRoganMedSmallBlue : DynamicKnob {
    DynRoganMedSmallBlue() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSBlueMedSmall.svg")));
    }
};

struct DynRoganMedGreen : DynamicKnob {
    DynRoganMedGreen() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSGreenMed.svg")));
    }
};

struct DynRoganMedPurple : DynamicKnob {
    DynRoganMedPurple() {
        minAngle = -0.83*M_PI;
        maxAngle = 0.83*M_PI;
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Rogan1PSPurpleMed.svg")));
    }
};

struct LightLEDButton : DynamicSwitchWidget {
    LightLEDButton() {
        //addFrame(SVG::load(asset::plugin(pluginInstance, "res/LightLEDButton80.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LightLEDButton80.svg")));
    }
};

struct RedDynamicLight : DynamicModuleLightWidget {
	RedDynamicLight() {
		addBaseColor(SCHEME_RED);
	}
};

struct PJ301MDarkPort : SvgPort {
	PJ301MDarkPort() {
		//setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PJ301MDark.svg")));
		/*background->wrap();
		box.size = background->box.size;
        shadow->box.size = background->box.size;
    	shadow->box.pos = Vec(0, background->box.size.y * 0.1);*/
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PJ301MDark.svg")));
	}
};

struct PJ301MDarkSmall : SvgPort {
	PJ301MDarkSmall() {
		/*background->svg = SVG::load(asset::plugin(pluginInstance, "res/PJ301MDarkSmall.svg"));
		background->wrap();
		box.size = background->box.size;
        shadow->box.size = background->box.size;
    	shadow->box.pos = Vec(0, background->box.size.y * 0.1);*/
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PJ301MDarkSmall.svg")));
	}
};

struct PJ301MDarkSmallOut : SvgPort {
	PJ301MDarkSmallOut() {
		/*background->svg = SVG::load(asset::plugin(pluginInstance, "res/PJ301MDarkSmallOut.svg"));
		background->wrap();
		box.size = background->box.size;
        shadow->box.size = background->box.size;
    	shadow->box.pos = Vec(0, background->box.size.y * 0.1);*/
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PJ301MDarkSmallOut.svg")));
	}
};

struct ValleySlider : SvgSlider {
    Vec margin = Vec(-1, -0.55);
    ValleySlider() {
        background->svg = SVG::load(asset::plugin(pluginInstance,"res/valleySliderBackground.svg"));
		background->wrap();
		background->box.pos = margin;
		box.size = background->box.size.plus(margin.mult(2));
    }
};

struct ValleyStepSlider : SVGStepSlider {
    Vec margin = Vec(-1, -0.55);
    ValleyStepSlider() {
        background->svg = SVG::load(asset::plugin(pluginInstance,"res/valleySliderBackground.svg"));
		background->wrap();
		background->box.pos = margin;
		box.size = background->box.size.plus(margin.mult(2));
    }
};

struct RedSlider : ValleySlider {
	RedSlider() {
		handle->svg = SVG::load(asset::plugin(pluginInstance,"res/sliderRed.svg"));
		handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
		minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
	}
};

struct OrangeSlider : ValleySlider {
	OrangeSlider() {
		handle->svg = SVG::load(asset::plugin(pluginInstance,"res/sliderOrange.svg"));
		handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
		minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
	}
};

struct YellowSlider : ValleySlider {
	YellowSlider() {
		handle->svg = SVG::load(asset::plugin(pluginInstance,"res/sliderGreen.svg"));
		handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
		minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
	}
};

struct GreenSlider : ValleySlider {
	GreenSlider() {
		handle->svg = SVG::load(asset::plugin(pluginInstance,"res/sliderGreen.svg"));
		handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
		minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
	}
};

struct BlueSlider : ValleySlider {
	BlueSlider() {
		handle->svg = SVG::load(asset::plugin(pluginInstance,"res/sliderBlue.svg"));
		handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 1.5).plus(margin);
		minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
	}
};

struct YellowStepSlider : ValleyStepSlider {
	YellowStepSlider() {
		handle->svg = SVG::load(asset::plugin(pluginInstance,"res/sliderYellow.svg"));
		handle->wrap();
        maxHandlePos = Vec((float)handle->box.size.x * 0.45, 0.5).plus(margin);
		minHandlePos = Vec((float)handle->box.size.x * 0.45, 61.5).plus(margin);
	}
};

#endif // DSJ_VALLEY_COMPONENTS_HPP
