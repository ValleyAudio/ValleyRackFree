#include "ValleyWidgets.hpp"

DynamicSwitchWidget::DynamicSwitchWidget() {
    visibility = nullptr;
    viewMode = ACTIVE_HIGH_VIEW;
    fb = new widget::FramebufferWidget;
	addChild(fb);

	sw = new widget::SvgWidget;
	fb->addChild(sw);
}

void DynamicSwitchWidget::addFrame(std::shared_ptr<Svg> svg) {
    frames.push_back(svg);
    // If this is our first frame, automatically set SVG and size
    if (!sw->svg) {
        sw->setSvg(svg);
        box.size = sw->box.size;
        fb->box.size = sw->box.size;
    }
}

void DynamicSwitchWidget::step() {
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
    else {
        visible = true;
    }
    fb->step();
}

void DynamicSwitchWidget::onChange(const event::Change &e) {
    /*assert(frames.size() > 0);
    float valueScaled = rescale(value, minValue, maxValue, 0, frames.size() - 1);
    int index = clamp((int) roundf(valueScaled), 0, frames.size() - 1);
    sw->setSVG(frames[index]);
    dirty = true;
    ParamWidget::onChange(e);*/
    if (!frames.empty() && paramQuantity) {
		int index = (int) std::round(paramQuantity->getValue());
		index = math::clamp(index, 0, (int) frames.size() - 1);
		sw->setSvg(frames[index]);
		fb->dirty = true;
	}
	ParamWidget::onChange(e);
}
