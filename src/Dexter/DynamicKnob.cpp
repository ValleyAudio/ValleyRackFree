#include "DynamicKnob.hpp"

DynamicKnob::DynamicKnob() {
	tw = new TransformWidget();
	addChild(tw);
	sw = new SVGWidget();
	tw->addChild(sw);
    _visibility = nullptr;
    _viewMode = ACTIVE_HIGH;
}

void DynamicKnob::setSVG(std::shared_ptr<SVG> svg) {
	sw->svg = svg;
	sw->wrap();
	tw->box.size = sw->box.size;
	box.size = sw->box.size;
}

void DynamicKnob::step() {
	// Re-transform TransformWidget if dirty
    if(_visibility != nullptr) {
        if(*_visibility) {
            visible = true;
        }
        else {
            visible = false;
        }
        if(_viewMode == ACTIVE_LOW) {
            visible = !visible;
        }
    }
    else {
        visible = true;
    }
	if (dirty) {
		tw->box.size = box.size;
		float angle = rescalef(value, minValue, maxValue, minAngle, maxAngle);
		tw->identity();
		// Scale SVG to box
		tw->scale(box.size.div(sw->box.size));
		// Rotate SVG
		Vec center = sw->box.getCenter();
		tw->translate(center);
		tw->rotate(angle);
		tw->translate(center.neg());
	}
	FramebufferWidget::step();
}

void DynamicKnob::onChange(EventChange &e) {
	dirty = true;
	Knob::onChange(e);
}
