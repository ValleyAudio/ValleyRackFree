#!/bin/bash

for plugin in Topograph uGraph Dexter Plateau Interzone Amalgam Feline
do
    perl -i -pe 's/void (\w+::)?step\(\)/void $1process(const ProcessArgs &args)/g' src/${plugin}/*
    perl -i -pe 's/void draw\(NVGcontext \*vg\)/void draw(const DrawArgs &args)/g' src/${plugin}/*
    perl -i -pe 's/\bvg\b/args.vg/g' src/${plugin}/*
done
