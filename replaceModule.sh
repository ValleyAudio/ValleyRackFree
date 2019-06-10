#!/bin/bash

for plugin in Topograph Interzone Plateau
do
    perl -i -pe 's/: Module\(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS\) \{/{\n\t\tconfig(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);/g' src/${plugin}/*
    perl -i -pe 's/: ModuleWidget\(module\) \{/{\n\t\tsetModule(module);/g' src/${plugin}/*
done
