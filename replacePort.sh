#!/bin/bash

for plugin in Feline
do
    perl -i -pe 's/, PortWidget::INPUT//g' src/${plugin}/*
    perl -i -pe 's/addInput\(createPort/addInput(createInput/g' src/${plugin}/*
    perl -i -pe 's/, PortWidget::OUTPUT//g' src/${plugin}/*
    perl -i -pe 's/addOutput\(createPort/addOutput(createOutput/g' src/${plugin}/*
done
