// Read in the interpolation dataSet from mesoscale simulations (useDataSet determines whether they are going to used)
List<double> radiusInterpIn(IFstream("constant/radiusInterpIn.txt")());
List<double> permInterpIn(IFstream("constant/permInterpIn.txt")());    
List<double> areaInterpIn(IFstream("constant/areaInterpIn.txt")());
List<double> por(IFstream("constant/por.txt")());
List<double> gamma_vs_por(IFstream("constant/gamma_vs_por.txt")());

//Definition of rmin and rmax (used in gamma-to-r conversion of useInterpInData()!)
const dimensionedScalar rmin = printResLen/2.0;
const dimensionedScalar rmax = (unitCellLen - 2.0 * printResLen) / 4.0;
const double deltaRInterp = radiusInterpIn[1] - radiusInterpIn[0];  

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
    &rmin,
    &rmax,
    &unitCellLen,
    &radiusInterpIn,
    &permInterpIn,    
    &areaInterpIn,
    &por,
    &gamma_vs_por,
    &deltaRInterp,
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
    &mesh.V(),
    &runTime,
};

U.storePrevIter();
p.storePrevIter();
