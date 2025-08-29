#!/bin/bash
# RfbFoam simulation runner - Run all steps

blockMesh | tee log.blockMesh

surfaceFeatureExtract | tee log.surfaceFeatureExtract

snappyHexMesh -overwrite | tee log.snappyHexMesh

setExprFields | tee log.setExprFields

decomposePar | tee log.decomposePar

mpirun -np $(grep numberOfSubdomains system/decomposeParDict | awk '{print $2}' | sed 's/;//') --use-hwthread-cpus RfbFoam -parallel | tee log.RfbFoam

reconstructPar | tee log.reconstructPar

rm -r processor*
