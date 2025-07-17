# Start OpenFOAM

#module load OpenFOAM/v2406-foss-2023a 
#source /sw/rl8/zen/app/OpenFOAM/v2406-foss-2023a/OpenFOAM-v2406/etc/bashrc

source /usr/lib/openfoam/openfoam2406/etc/bashrc

blockMesh | tee log.blockMesh

surfaceFeatureExtract | tee log.surfaceFeatureExtract

snappyHexMesh -overwrite | tee log.snappyHexMesh

setExprFields | tee log.setExprFields

decomposePar | tee log.decomposePar

mpirun -np $(grep numberOfSubdomains system/decomposeParDict | awk '{print $2}' | sed 's/;//') --use-hwthread-cpus RfbFoam -parallel | tee log.RfbFoam

reconstructPar | tee log.reconstructPar

rm -r processor*
