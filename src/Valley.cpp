#include "Valley.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->website = "https://github.com/ValleyAudio/ValleyRackFree";
	p->manual = "https://github.com/ValleyAudio/ValleyRackFree/blob/master/README.md";
    p->addModel(modelTopograph);
    /*p->addModel(modelUGraph);
    p->addModel(modelDexter);
    p->addModel(modelPlateau);
    p->addModel(modelAmalgam);
    p->addModel(modelInterzone);
    p->addModel(modelFeline);*/
}
