#include "rack.hpp"

using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct TopographWidget : ModuleWidget {
	TopographWidget();
    Menu* createContextMenu() override;
};

struct DexterWidget : ModuleWidget {
    DexterWidget();
    Menu* createContextMenu() override;
};
