#include "Valley.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = "Valley Free";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#else
    p->version = "0.5.6";
#endif

	p->website = "https://github.com/ValleyAudio/ValleyRackFree";
	p->manual = "https://github.com/ValleyAudio/ValleyRackFree/blob/master/README.md";
    p->addModel(createModel<TopographWidget>("Valley", "Topograph", "Topograph", SEQUENCER_TAG));
    p->addModel(createModel<DexterWidget>("Valley", "Dexter", "Dexter", OSCILLATOR_TAG));
}
