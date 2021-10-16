#include "ValleyWidgets.hpp"

void SvgStepSlider::onChange(const event::Change& e) {
    ParamQuantity* paramQuantity = getParamQuantity();
    if(paramQuantity) {
        float v = paramQuantity->getValue();
        // Interpolate handle position
        handle->box.pos = math::Vec(
            math::rescale(floorf(v), paramQuantity->getMinValue(), paramQuantity->getMaxValue(), minHandlePos.x, maxHandlePos.x),
            math::rescale(floorf(v), paramQuantity->getMinValue(), paramQuantity->getMaxValue(), minHandlePos.y, maxHandlePos.y));
        fb->dirty = true;
    }
	ParamWidget::onChange(e);
}
