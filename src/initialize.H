// Assign names to patches  
const label& patchID = mesh.boundaryMesh().findPatchID("mem"); 
const fvPatch& topPatch = mesh.boundary()[patchID];     
const label& patchID2 = mesh.boundaryMesh().findPatchID("currColl"); 
const fvPatch& btmPatch = mesh.boundary()[patchID2]; 
const label& patchID3 = mesh.boundaryMesh().findPatchID("outlet"); 
const fvPatch& outletPatch = mesh.boundary()[patchID3];        
const label& patchID4 = mesh.boundaryMesh().findPatchID("inlet"); 
const fvPatch& inletPatch = mesh.boundary()[patchID4];  

//Initialize modelParams
modelParams =
{        
    &invPermeability,
    &forchCoeff,
    &porosity,
    &areaPerVol,    
    &gamma,  
    &rFiber,   
    &mesh.cells(),
    // &rmin,
    // &rmax,
    &unitCellLen,
    // &radiusInterpIn,
    // &permInterpIn,    
    // &areaInterpIn,
    // &por,
    // &gamma_vs_por,
    // &deltaRInterp,
    &mesh.C(),
    &kappa1,    
    &tau,    
    &kappa2,    
    &DCO,    
    &DCR,
    &U,
    &phi,
    &p,
    &turbulence,
    &pRefCell,
    &pRefValue,
    &nu,
    &rho,
    &fvOptions,
    &laminarTransport,
    &inletPatch,
    &topPatch,
    &outletPatch,
    &btmPatch,
    &patchID3,
    &patchID4,
    &bR,
    &bO,
    &gR,
    &gO,
    &c1_km,
    &c2_km,
    &c3_km,
    &c4_km,
    &g_km,
    &b_km,
    &velRef,
    &lenRef,
    &CO,
    &CR,
    &Phi1,
    &Phi2,
    &kmR,
    &kmO,
    &alphaA,
    &alphaC,
    &CRef,
    &i0,
    &FRT,
    &nelec,
    &stdPotential,
    &faradayConstant,
    &deltaPhi,
    &I1,
    &I2,
    &i_loc,
    &mesh.V(),
    &runTime,
};

U.storePrevIter();
p.storePrevIter();
