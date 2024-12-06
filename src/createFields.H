// Create the fields for Field Variables solved for:

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

#include "createPhi.H"

Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, simple.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());

singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

Info<< "Reading field CO\n" << endl;
volScalarField CO
(
    IOobject
    (
        "CO",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field CR\n" << endl;
volScalarField CR
(
    IOobject
    (
        "CR",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
   
Info<< "Reading field Phi1\n" << endl;
volScalarField Phi1
(
    IOobject
    (
        "Phi1",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field Phi2\n" << endl;
volScalarField Phi2
(
    IOobject
    (
        "Phi2",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

// Create the fields for Field Variables that are not solved for (those that stay constant over the simulation):
volScalarField gamma
(
    IOobject
    (
       "gamma",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
  
volScalarField porosity
(
    IOobject
    (
       "porosity",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);
   
volScalarField areaPerVol
(
    IOobject
    (
        "areaPerVol",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField invPermeability
(
    IOobject
    (
        "invPermeability",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


volScalarField forchCoeff
(
    IOobject
    (
        "forchCoeff",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


// Read in the transportProperties file
Info<< "Reading transportProperties\n" << endl;
IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

// Read in variables from the transportProperties file
dimensionedScalar nu ("nu", transportProperties);
dimensionedScalar rho ("rho", transportProperties); 
dimensionedScalar DCO ("DCO", transportProperties);    
dimensionedScalar DCR ("DCR", transportProperties);   

dimensionedScalar kappa1 ("kappa1", transportProperties);
dimensionedScalar kappa2 ("kappa2", transportProperties);

dimensionedScalar i0 ("i0", transportProperties);
dimensionedScalar Temperature ("Temperature", transportProperties);   
dimensionedScalar stdPotential ("stdPotential", transportProperties);
dimensionedScalar alphaA ("alphaA", transportProperties);
dimensionedScalar alphaC ("alphaC", transportProperties);
dimensionedScalar nelec ("nelec", transportProperties);
dimensionedScalar CRef ("CRef", transportProperties);

const dimensionedScalar c1_km ("c1_km", transportProperties);
const dimensionedScalar c2_km ("c2_km", transportProperties);
const dimensionedScalar c3_km ("c3_km", transportProperties);
const dimensionedScalar c4_km ("c4_km", transportProperties);

const dimensionedScalar g_km ("g_km", transportProperties);
const dimensionedScalar b_km ("b_km", transportProperties);

dimensionedScalar unitCellLen ("unitCellLen", transportProperties);
dimensionedScalar printResLen ("printResLen", transportProperties);


// Define physical constants
dimensionedScalar gasConstant
("gasConstant", dimensionSet(1, 2, -2, -1, 0, 0, 0), 
    8.3144598
);
    
dimensionedScalar faradayConstant
("faradayConstant", dimensionSet(0, 0, 1, 0, 0, 1, 0), 
    96485.3329
);
    
dimensionedScalar FRT 
("FRT", faradayConstant/Temperature/gasConstant);

// Define reference variables
const dimensionedScalar velRef
(
    "velRef", 
    dimensionSet(0, 1, -1, 0, 0, 0, 0),1.0
);

const dimensionedScalar lenRef
(
    "lenRef", 
    dimensionSet(0, 1, 0, 0, 0, 0, 0),1.0
);

// Define overpotential, ionic and electric current fields        
volScalarField deltaPhi
(
    IOobject
    (
        "deltaPhi",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    Phi1 -Phi2 - stdPotential
);    

Info<< "Creating field I1\n" << endl;
volVectorField I1
(
    IOobject
    (
        "I1",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    -1.0*kappa1*fvc::grad(Phi1)
);

Info<< "Creating field I2\n" << endl;
volVectorField I2
(
    IOobject
    (
        "I2",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    -1.0*kappa2*fvc::grad(Phi2)
);

// Define constants needed for the Sherwood correlation
/*
const dimensionedScalar c1_km("c1_km", dimless, 0.468);
const dimensionedScalar c2_km("c2_km", dimless, 0.391);
const dimensionedScalar c3_km("c3_km", dimless, 0.4672);
const dimensionedScalar c4_km("c4_km", dimless, -0.1247);
*/

// Define fields needed for the Sherwood correlation
volScalarField bR
(
    IOobject
    (
        "bR",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
        //IOobject::AUTO_WRITE
    ),
    c4_km*porosity
);

volScalarField bO
(
    IOobject
    (
        "bO",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
        //IOobject::AUTO_WRITE
    ),
    c4_km*porosity
);

volScalarField kmR
(
    IOobject
    (
        "kmR",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);    
      
volScalarField kmO
(
    IOobject
    (
        "kmO",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);          

volScalarField gR
(
    IOobject
    (
        "gR",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
        //IOobject::AUTO_WRITE
    ),
    kmO
);

volScalarField gO
(
    IOobject
    (
        "gO",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
        //IOobject::AUTO_WRITE
    ),
    kmR
);


// Define rFiber field 
volScalarField rFiber
(
    IOobject
    (
        "rFiber",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


// List cells belonging to inlet, outlet, top and bottom boundaries
labelList inletCells = mesh.boundary()["inlet"].faceCells();
labelList outletCells = mesh.boundary()["outlet"].faceCells();
labelList topCells = mesh.boundary()["top"].faceCells();
labelList btmCells = mesh.boundary()["btm"].faceCells();



