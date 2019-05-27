#include "ValleyWidgets.hpp"

void DynamicModuleLightWidget::step() {
    std::vector<float> brightnesses(baseColors.size());
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

    /*assert(module);
    assert(module->lights.size() >= firstLightId + baseColors.size());
    std::vector<float> values(baseColors.size());

    for (size_t i = 0; i < baseColors.size(); i++) {
        float value = module->lights[firstLightId + i].getBrightness();
        value = clamp(value, 0.0, 1.0);
        values[i] = value;
    }
    setValues(values);*/
    if (module) {
		assert(module->lights.size() >= firstLightId + baseColors.size());

		for (size_t i = 0; i < baseColors.size(); i++) {
			float brightness = module->lights[firstLightId + i].getBrightness();
			if (!std::isfinite(brightness))
				brightness = 0.f;
			// Because LEDs are nonlinear, this seems to look more natural.
			brightness = std::sqrt(brightness);
			brightness = math::clamp(brightness, 0.f, 1.f);
			brightnesses[i] = brightness;
		}
	}
	else {
		// Turn all lights on
		for (size_t i = 0; i < baseColors.size(); i++) {
			brightnesses[i] = 1.f;
		}
	}

	setBrightnesses(brightnesses);
}
