#!/bin/bash

for plugin in Topograph uGraph Dexter Plateau Interzone Amalgam Feline
do
    perl -i -pe 's/toJson/dataToJson/g' src/${plugin}/*
    perl -i -pe 's/fromJson/dataFromJson/g' src/${plugin}/*
done
