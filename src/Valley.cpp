#include "Valley.hpp"

// The pluginInstance-wide instance of the Plugin class
Plugin *pluginInstance;

void init(rack::Plugin *p) {
	pluginInstance = p;
    p->addModel(modelTopograph);
    p->addModel(modelUGraph);
    p->addModel(modelDexter);
    p->addModel(modelPlateau);
    p->addModel(modelInterzone);
    p->addModel(modelAmalgam);
    p->addModel(modelFeline);
    p->addModel(modelTerrorform);
}
