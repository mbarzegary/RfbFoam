#!/bin/bash
# RfbFoam mesh preparation script

cp 0.orig/* 0/

blockMesh | tee log.blockMesh

surfaceFeatureExtract | tee log.surfaceFeatureExtract

snappyHexMesh -overwrite | tee log.snappyHexMesh

setExprFields | tee log.setExprFields
