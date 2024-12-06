vector spU = solve(UEqn == -fvc::grad(p)).initialResidual();  
flowRes += spU[0] + spU[1] + spU[2];                               

fvOptions.correct(U);          

volScalarField rAU(1.0/UEqn.A());
volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));
tUEqn.clear();
surfaceScalarField phiHbyA("phiHbyA", fvc::flux(HbyA));
adjustPhi(phiHbyA, U, p);

// Update the pressure BCs to ensure flux consistency
constrainPressure(p, U, phiHbyA, rAU);

// You deleted the non-orthogonal corector
{
    fvScalarMatrix pEqn
    (
        fvm::laplacian(rAU, p) == fvc::div(phiHbyA)
    );

    pEqn.setReference(pRefCell, pRefValue);
    solverPerformance spPress = pEqn.solve();
    
    flowRes += spPress.initialResidual();

    // Info << "Press Res: " << spPress.initialResidual() << endl;

    // if (simple.finalNonOrthogonalIter())
    {
        phi = phiHbyA - pEqn.flux();
    }
}

// Explicitly relax pressure for momentum corrector
p.relax();

// Momentum corrector
U = HbyA - rAU*fvc::grad(p);
U.correctBoundaryConditions();
fvOptions.correct(U);
