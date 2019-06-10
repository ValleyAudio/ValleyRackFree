#include "ValleyWidgets.hpp"

void PanelBorderWidget::draw(const DrawArgs &args) {
    NVGcolor borderColor = nvgRGBAf(0.5, 0.5, 0.5, 0.5);
    nvgBeginPath(args.vg);
    nvgRect(args.vg, 0.5, 0.5, box.size.x - 1.0, box.size.y - 1.0);
    nvgStrokeColor(args.vg, borderColor);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStroke(args.vg);
}

DynamicPanelWidget::DynamicPanelWidget() {
    mode = nullptr;
    oldMode = -1;
    visiblePanel = new SvgWidget();
    addChild(visiblePanel);
    border = new PanelBorderWidget();
    addChild(border);
}

void DynamicPanelWidget::addPanel(std::shared_ptr<Svg> svg) {
    panels.push_back(svg);
    if(!visiblePanel->svg) {
        visiblePanel->setSvg(svg);
        box.size = visiblePanel->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);
        border->box.size = box.size;
    }
}

void DynamicPanelWidget::step() {
    if (isNear(APP->window->pixelRatio, 1.0)) {
        oversample = 2.f;
    }
    /*if(mode != nullptr){
        if(*mode != oldMode) {
            visiblePanel->setSvg(panels[*mode]);
            oldMode = *mode;
            dirty = true;
        }
    }*/
    visiblePanel->setSvg(panels[0]);
}
