# Start OpenFOAM

#module load OpenFOAM/v2406-foss-2023a 
#source /sw/rl8/zen/app/OpenFOAM/v2406-foss-2023a/OpenFOAM-v2406/etc/bashrc

source /usr/lib/openfoam/openfoam2406/etc/bashrc

cp 0.orig/* 0/

blockMesh | tee log.blockMesh

surfaceFeatureExtract | tee log.surfaceFeatureExtract

snappyHexMesh -overwrite | tee log.snappyHexMesh

setExprFields | tee log.setExprFields
