#include "rack.hpp"
#define DR_WAV_IMPLEMENTATION
using namespace rack;

#include <iomanip> // setprecision
#include <sstream> // stringstream
#define VALLEY_NAME "ValleyDev"

extern Plugin *pluginInstance;

////////////////////
// module widgets
////////////////////

extern Model *modelTopograph;
extern Model *modelUGraph;
extern Model *modelDexter;
extern Model *modelPlateau;
extern Model *modelInterzone;
extern Model *modelAmalgam;
extern Model *modelFeline;
extern Model *modelTerrorform;
