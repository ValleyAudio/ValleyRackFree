//
// Topograph.cpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 5/12/2017
//
// Topograph, a port of "Mutable Instruments Grids" for VCV Rack
// Original author: Emilie Gillet (emilie.o.gillet@gmail.com)
// https://github.com/pichenettes/eurorack/tree/master/grids
// Copyright 2012 Emilie Gillet.
//
// This source code is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include "../Valley.hpp"
#include "../ValleyComponents.hpp"
#include "../Common/Metronome.hpp"
#include "../Common/Oneshot.hpp"
#include "TopographPatternGenerator.hpp"
#include <iomanip> // setprecision
#include <sstream> // stringstream

struct Topograph : Module {
   enum ParamIds {
       RESET_BUTTON_PARAM,
       RUN_BUTTON_PARAM,
       TEMPO_PARAM,
       MAPX_PARAM,
       MAPY_PARAM,
       CHAOS_PARAM,
       BD_DENS_PARAM,
       SN_DENS_PARAM,
       HH_DENS_PARAM,
       SWING_PARAM,
       NUM_PARAMS
   };

   enum InputIds {
       CLOCK_INPUT,
       RESET_INPUT,
       MAPX_CV,
       MAPY_CV,
       CHAOS_CV,
       BD_FILL_CV,
       SN_FILL_CV,
       HH_FILL_CV,
       SWING_CV,
       RUN_INPUT,
       NUM_INPUTS
   };

   enum OutputIds {
       BD_OUTPUT,
       SN_OUTPUT,
       HH_OUTPUT,
       BD_ACC_OUTPUT,
       SN_ACC_OUTPUT,
       HH_ACC_OUTPUT,
       NUM_OUTPUTS
   };

   enum LightIds {
       RUNNING_LIGHT,
       RESET_LIGHT,
       BD_LIGHT,
       SN_LIGHT,
       HH_LIGHT,
       NUM_LIGHTS
   };

   Metronome metro;
   PatternGenerator grids;
   uint8_t numTicks;
   dsp::SchmittTrigger clockTrig;
   dsp::SchmittTrigger resetTrig;
   dsp::SchmittTrigger resetButtonTrig;
   dsp::SchmittTrigger runButtonTrig;
   dsp::SchmittTrigger runInputTrig;
   bool initExtReset = true;
   int running = 0;
   bool externalClockConnected = false;
   bool inExternalClockMode = false;
   bool advStep = false;
   long seqStep = 0;
   float swing = 0.5;
   float swingHighTempo = 0.0;
   float swingLowTempo = 0.0;
   long elapsedTicks = 0;

   float tempoParam = 0.0;
   float tempo = 120.0;
   float mapX = 0.0;
   float mapY = 0.0;
   float chaos = 0.0;
   float BDFill = 0.0;
   float SNFill = 0.0;
   float HHFill = 0.0;

   uint8_t state = 0;

   // LED Triggers
   Oneshot drumLED[3];
   const LightIds drumLEDIds[3] = {BD_LIGHT, SN_LIGHT, HH_LIGHT};
   Oneshot BDLed;
   Oneshot SNLed;
   Oneshot HHLed;
   Oneshot resetLed;
   Oneshot runningLed;

   // Drum Triggers
   Oneshot drumTriggers[6];
   bool gateState[6];
   const OutputIds outIDs[6] = {BD_OUTPUT, SN_OUTPUT, HH_OUTPUT,
                                BD_ACC_OUTPUT, SN_ACC_OUTPUT, HH_ACC_OUTPUT};

   enum SequencerMode {
       HENRI,
       ORIGINAL,
       EUCLIDEAN
   };
   SequencerMode sequencerMode = HENRI;
   int inEuclideanMode = 0;

   enum TriggerOutputMode {
       PULSE,
       GATE
   };
   TriggerOutputMode triggerOutputMode = PULSE;

   enum AccOutputMode {
       INDIVIDUAL_ACCENTS,
       ACC_CLK_RST
   };
   AccOutputMode accOutputMode = INDIVIDUAL_ACCENTS;

   enum ExtClockResolution {
       EXTCLOCK_RES_4_PPQN,
       EXTCLOCK_RES_8_PPQN,
       EXTCLOCK_RES_24_PPQN,
   };
   ExtClockResolution extClockResolution = EXTCLOCK_RES_24_PPQN;

   enum ChaosKnobMode {
       CHAOS,
       SWING
   };
   ChaosKnobMode chaosKnobMode = CHAOS;

   enum RunMode {
       TOGGLE,
       MOMENTARY
   };
   RunMode runMode = TOGGLE;

   int panelStyle;
   int textVisible = 1;

   Topograph();
   json_t* dataToJson() override;
   void dataFromJson(json_t *rootJ) override;
   void process(const ProcessArgs &args) override;
   void onSampleRateChange() override;
   void onReset() override;
   void updateUI();
   void updateOutputs();
};

struct TempoKnob : Rogan1PSBlue {
    bool randomizationAllowed = true;
    void randomize();
};

struct TopographWidget : ModuleWidget {
    TopographWidget(Topograph *topograph);
    void appendContextMenu(Menu* menu) override;
    void step() override;

    SvgPanel* darkPanel;
    SvgPanel* lightPanel;
    TempoKnob* tempoKnob;
    PlainText* tempoText;
    PlainText* mapXText;
    PlainText* mapYText;
    PlainText* chaosText;
    NVGcolor darkPanelTextColour = nvgRGB(0xFF, 0xFF, 0xFF);
    NVGcolor lightPanelTextColour = nvgRGB(0x00, 0x00, 0x00);
    NVGcolor panelTextColours[2] = {darkPanelTextColour, lightPanelTextColour};
    bool isInExtClockMode = false;
};
