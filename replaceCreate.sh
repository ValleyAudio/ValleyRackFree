#!/bin/bash

for plugin in Topograph uGraph Dexter Plateau Interzone Amalgam Feline
do
    perl -i -pe 's/Model::create/createModel/g' src/${plugin}/*
    perl -i -pe 's/ParamWidget::create/createParam/g' src/${plugin}/*
    perl -i -pe 's/ModuleLightWidget::create/createLight/g' src/${plugin}/*
    perl -i -pe 's/Port::create/createPort/g' src/${plugin}/*
    perl -i -pe 's/Port::OUTPUT/PortWidget::OUTPUT/g' src/${plugin}/*
    perl -i -pe 's/Port::INPUT/PortWidget::INPUT/g' src/${plugin}/*
    perl -i -pe 's/Widget::create/createWidget/g' src/${plugin}/*
    perl -i -pe 's/MenuEntry::create\(\)/new MenuEntry/g' src/${plugin}/*
    perl -i -pe 's/MenuLabel::create/createMenuLabel/g' src/${plugin}/*
    perl -i -pe 's/MenuItem::create/createMenuItem/g' src/${plugin}/*
done
