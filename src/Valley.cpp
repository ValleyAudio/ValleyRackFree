#include "Valley.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = TOSTRING(SLUG);
	p->version = TOSTRING(VERSION);
	p->website = "https://valleyaudio.github.io";
	p->manual = "https://valleyaudio.github.io";
    p->addModel(modelTopograph);
    p->addModel(modelUGraph);
    p->addModel(modelDexter);
    p->addModel(modelPlateau);
}
