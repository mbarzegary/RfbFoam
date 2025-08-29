#!/bin/bash
# RfbFoam mesh preparation script

blockMesh | tee log.blockMesh

surfaceFeatureExtract | tee log.surfaceFeatureExtract

snappyHexMesh -overwrite | tee log.snappyHexMesh

setExprFields | tee log.setExprFields
