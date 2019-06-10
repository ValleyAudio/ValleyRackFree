#!/bin/bash

for plugin in Feline
do
    perl -i -pe 's/(params\[.*?\])\.value/$1.getValue()/g' src/${plugin}/*
    perl -i -pe 's/(inputs\[.*?\])\.value/$1.getVoltage()/g' src/${plugin}/*
    perl -i -pe 's/(outputs\[.*?\])\.value = (.*?);/$1.setVoltage($2);/g' src/${plugin}/*
    perl -i -pe 's/(inputs\[.*?\])\.active/$1.isConnected()/g' src/${plugin}/*
    perl -i -pe 's/(outputs\[.*?\])\.active/$1.isConnected()/g' src/${plugin}/*
done
