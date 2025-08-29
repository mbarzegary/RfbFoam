#!/bin/bash
# RfbFoam fluid dynamics simulation runner

decomposePar | tee log.decomposePar

mpirun -np $(grep numberOfSubdomains system/decomposeParDict | awk '{print $2}' | sed 's/;//') --use-hwthread-cpus RfbFoam -onlyU -parallel | tee log.RfbFoam

reconstructPar | tee log.reconstructPar

rm -r processor*

