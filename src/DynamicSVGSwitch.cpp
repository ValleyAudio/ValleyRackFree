#include "ValleyWidgets.hpp"

DynamicSwitchWidget::DynamicSwitchWidget() {
    _visibility = nullptr;
    _viewMode = ACTIVE_HIGH_VIEW;
}

void DynamicSwitchWidget::step() {
    if(_visibility != nullptr) {
        if(*_visibility) {
            visible = true;
        }
        else {
            visible = false;
        }
        if(_viewMode == ACTIVE_LOW_VIEW) {
            visible = !visible;
        }
    }
    else {
        visible = true;
    }
    SvgSwitch::step();
}
