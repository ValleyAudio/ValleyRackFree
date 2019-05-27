#include "ValleyWidgets.hpp"

void SVGStepSlider::onChange(const event::Change& e) {
    if(paramQuantity) {
        float v = paramQuantity->getScaledValue();
		// Interpolate handle position
		/*handle->box.pos = Vec(rescale(floorf(value), minValue, maxValue, minHandlePos.x, maxHandlePos.x),
                              rescale(floorf(value), minValue, maxValue, minHandlePos.y, maxHandlePos.y));*/
        handle->box.pos = math::Vec(
			math::rescale(floorf(v), 0.f, 1.f, minHandlePos.x, maxHandlePos.x),
			math::rescale(floorf(v), 0.f, 1.f, minHandlePos.y, maxHandlePos.y));
        fb->dirty = true;
    }
	ParamWidget::onChange(e);
}
