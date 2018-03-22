#ifndef DSJ_VALLEY_RACK_DYNAMIC_KNOB_HPP
#define DSJ_VALLEY_RACK_DYNAMIC_KNOB_HPP
#include "DynamicBase.hpp"

enum DynamicKnobMotion {
    SMOOTH_MOTION,
    SNAP_MOTION
};

struct DynamicKnob : virtual Knob, FramebufferWidget {
	/** Angles in radians */
	float minAngle, maxAngle;
	/** Not owned */
	TransformWidget *tw;
	SVGWidget *sw;
    CircularShadow *shadow;
    int* _visibility;
    DynamicViewMode _viewMode;

	DynamicKnob();
	void setSVG(std::shared_ptr<SVG> svg);
	void step() override;
	void onChange(EventChange &e) override;
};

template <class TDynamicKnob>
DynamicKnob* createDynamicKnob(const Vec& pos,
                               Module* module,
                               int paramId,
                               int* visibilityHandle,
                               DynamicViewMode viewMode,
                               float minValue,
                               float maxValue,
                               float defaultValue,
                               DynamicKnobMotion motion) {
    DynamicKnob* knob = new TDynamicKnob;
    knob->module = module;
    knob->box.pos = pos;
    knob->paramId = paramId;
    knob->setLimits(minValue, maxValue);
    knob->setDefaultValue(defaultValue);
    knob->_visibility = visibilityHandle;
    knob->_viewMode = viewMode;
    if(motion == SNAP_MOTION) {
        knob->snap = true;
    }
    return knob;
}

#endif // DSJ_VALLEY_RACK_DYNAMIC_KNOB_HPP
