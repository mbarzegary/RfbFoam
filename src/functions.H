// Calculate mass transfer coefficient
void setSimpleMTCoeff()
{
    const cellList &cells = *modelParams.cells;
    const dimensionedScalar & b_km             = *modelParams.b_km;
    const dimensionedScalar & g_km             = *modelParams.g_km;

    forAll(cells, cellI)
    {
        (*modelParams.bR)[cellI] = b_km.value() * 1.0;
        (*modelParams.bO)[cellI] = b_km.value() * 1.0;

        (*modelParams.gR)[cellI] = g_km.value() * 1.0;
        (*modelParams.gO)[cellI] = g_km.value() * 1.0;
    }
}

// Print the results of the simulation
void printOutput()
{
    surfaceScalarField      & phi               = *modelParams.phi;

    const fvPatch           & topPatch          = *modelParams.topPatch;
    const fvPatch           & btmPatch          = *modelParams.btmPatch;
    const label             & patchID3          = *modelParams.patchID3;
    const fvPatch           & outletPatch       = *modelParams.outletPatch;
    const label             & patchID4          = *modelParams.patchID4;
    const fvPatch           & inletPatch        = *modelParams.inletPatch;

    dimensionedScalar       & rho               = *modelParams.rho;

    dimensionedScalar       & stdPotential      = *modelParams.stdPotential;


    // Extract the required patch fields
    const fvPatchField<vector>& topI2 =
    topPatch.lookupPatchField<volVectorField, vector>("I2");
    const fvPatchField<vector>& btmI1 =
    btmPatch.lookupPatchField<volVectorField, vector>("I1");

    const fvPatchField<scalar>& topPhi2 =
    topPatch.lookupPatchField<volScalarField, scalar>("Phi2");
    const fvPatchField<scalar>& btmPhi1 =
    btmPatch.lookupPatchField<volScalarField, scalar>("Phi1");

    const fvPatchField<scalar>& inletPressure =
    inletPatch.lookupPatchField<volScalarField, scalar>("p");
    const fvPatchField<scalar>& outletPressure =
    outletPatch.lookupPatchField<volScalarField, scalar>("p");

    const fvPatchField<scalar>& outletCO =
        outletPatch.lookupPatchField<volScalarField, scalar>("CO");
    const fvPatchField<scalar>& outletCR =
        outletPatch.lookupPatchField<volScalarField, scalar>("CR");

    const fvPatchField<scalar>& inletCO =
    inletPatch.lookupPatchField<volScalarField, scalar>("CO");
    const fvPatchField<scalar>& inletCR =
    inletPatch.lookupPatchField<volScalarField, scalar>("CR");

    const fvsPatchField<scalar>& inPhi =
    inletPatch.lookupPatchField<surfaceScalarField, scalar>("phi");


    // Compute printing output variables
    scalar totalTopCurrent = gSum(topI2 & topI2.patch().Sf());  //A
    scalar totalBtmCurrent = gSum( (btmI1 & btmI1.patch().Sf()) ); //A
    scalar totalTopArea = gSum(mag(topI2.patch().Sf())); //m^2
    scalar averagedTopCurrentDensity = totalTopCurrent / totalTopArea; //A m^-2

    scalar volFLowIn = gSum(phi.boundaryField()[patchID4]); //m^3 s^-1
    scalar volFlowOut = gSum(phi.boundaryField()[patchID3]); //m^3 s^-1

    scalar inFlowCO = gSum(inletCO * phi.boundaryField()[patchID4]); //mol s^-1
    scalar inFlowCR = gSum(inletCR * phi.boundaryField()[patchID4]); //mol s^-1
    scalar outFlowCO = gSum(outletCO * phi.boundaryField()[patchID3]); //mol s^-1
    scalar outFlowCR = gSum(outletCR * phi.boundaryField()[patchID3]); //mol s^-1

    // Compute surface-average inlet pressure
    scalar sumInletPressure = gSum(inletPressure * inletPressure.patch().magSf());
    scalar totalInletArea = gSum(inletPressure.patch().magSf());
    scalar surfaceAvgInletPressure = rho.value() * sumInletPressure / totalInletArea;

    // Compute surface-average outlet pressure
    scalar sumOutletPressure = gSum(outletPressure * outletPressure.patch().magSf());
    scalar totalOutletArea = gSum(outletPressure.patch().magSf());
    scalar surfaceAvgOutletPressure = rho.value() * sumOutletPressure / totalOutletArea;

    // Compute power losses and overpotential
    scalar flowLoss = -rho.value()*gSum(inletPressure*inPhi); // J/s
    scalar electricLoss = gSum((topI2 & topI2.patch().Sf()) * (-1. * topPhi2 - stdPotential.value())); // J/s, Beck's definition
    // scalar electricLoss = gSum((topI2 & topI2.patch().Sf()) * (btmPhi1 -1. * topPhi2 - stdPotential.value())); // J/s, Beck's definition

    scalar overpotential = electricLoss / totalTopCurrent;  //V

    // Print the final results of the simulation

    // Print top and bottom current
    Info << "\nTotal Current at top/membrane (A): " << totalTopCurrent << endl;
    Info << "Total Current at btm/curr. collector (A): " << -totalBtmCurrent << endl;

    //Print volumetric flow rates in/out
    Info << "\nFlow rate in (m^3/s): " << -volFLowIn << endl;
    Info << "Flow rate in (mL/min): " << -volFLowIn*1.0e6*60 << endl;
    Info << "\nFlow rate out (m^3/s): " << volFlowOut << endl;
    Info << "Flow rate out (mL/min): " << volFlowOut*1.0e6*60 << endl;

    //Print mole flows in/out
    Info << "\nInlet flow (mol/s) of CO: " << -inFlowCO << endl;
    Info << "Inlet flow (mol/s) of CR: " << -inFlowCR << endl;
    Info << "Outlet flow (mol/s) of CO: " << outFlowCO << endl;
    Info << "Outlet flow (mol/s) of CR: " << outFlowCR << endl;

    //Print tot. conc. of active species, and SoC_in, SoC_out
    Info << "\nTotal active species Concentration (M): " << (inFlowCO+inFlowCR)/(1.0e3*volFLowIn) << endl;
    Info << "Inlet State-of-Charge (%): " << 100.0*inFlowCR/(inFlowCO+inFlowCR) << endl;
    Info << "Outlet State-of-Charge (%): " << 100.0*outFlowCR/(outFlowCO+outFlowCR) << endl;

    //Print the power losses
    Info << "\nFlow Power (J/s): " << flowLoss << endl;
    Info << "Electric Power (J/s): " << electricLoss << endl;
    Info << "Total Power Loss (J/s): " << flowLoss + electricLoss << endl;

    //Print total pressure drop
    Info << "\nPressure drop (Pa): " << surfaceAvgInletPressure - surfaceAvgOutletPressure << endl;

    //Print "polarization curve" data
    Info << "\ni (mA/cm^2) ,  eta (V) : ";
    Info <<  averagedTopCurrentDensity / 10.0 << " , " << overpotential << endl;
}
