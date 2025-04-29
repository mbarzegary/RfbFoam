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

    // dimensionedScalar       & stdPotential      = *modelParams.stdPotential;

    volScalarField          & i_loc             = *modelParams.i_loc;
    volScalarField          & Phi1              = *modelParams.Phi1;
    volScalarField          & Phi2              = *modelParams.Phi2;
    volScalarField          & deltaPhi          = *modelParams.deltaPhi;
    volVectorField          & I1                = *modelParams.I1;
    volVectorField          & I2                = *modelParams.I2;
    const volScalarField::Internal & meshVolumes= *modelParams.meshVolumes;

    // Extract the required patch fields
    const fvPatchField<vector>& topI2 =
    topPatch.lookupPatchField<volVectorField, vector>("I2");
    const fvPatchField<vector>& btmI1 =
    btmPatch.lookupPatchField<volVectorField, vector>("I1");

    // const fvPatchField<scalar>& topPhi2 =
    // topPatch.lookupPatchField<volScalarField, scalar>("Phi2");
    // const fvPatchField<scalar>& btmPhi1 =
    // btmPatch.lookupPatchField<volScalarField, scalar>("Phi1");

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
    scalar sumInletPressure = gSum(inletPressure * inletPressure.patch().magSf()); //m^2 s^-2 kinematik pressure
    scalar totalInletArea = gSum(inletPressure.patch().magSf());
    scalar surfaceAvgInletPressure = rho.value() * sumInletPressure / totalInletArea;

    // Compute surface-average outlet pressure
    scalar sumOutletPressure = gSum(outletPressure * outletPressure.patch().magSf());
    scalar totalOutletArea = gSum(outletPressure.patch().magSf());
    scalar surfaceAvgOutletPressure = rho.value() * sumOutletPressure / totalOutletArea;

    // Compute power losses and overpotential
    scalar flowLoss = -rho.value()*gSum(inletPressure*inPhi); // J/s
    // scalar electricLoss = gSum((topI2 & topI2.patch().Sf()) * (-1. * topPhi2 - stdPotential.value())); // Remove btmPhi1 because it's zero
    // scalar electricLoss = gSum((topI2 & topI2.patch().Sf()) * (btmPhi1 -1. * topPhi2 - stdPotential.value())); // J/s, Beck's definition

    // scalar overpotential = electricLoss / totalTopCurrent;  //V
    
    /*Post-processing variables*/
    volVectorField gradPhi1 = fvc::grad(Phi1);
    volVectorField gradPhi2 = fvc::grad(Phi2);
    volScalarField::Internal P1 = mag(gradPhi1 & I1); // Electrode power loss
    volScalarField::Internal P2 = mag(gradPhi2 & I2); // Electrolyte power loss
    volScalarField::Internal P3 = mag(deltaPhi * i_loc); // Kinetik power loss

    volScalarField::Internal P11 = P1 * meshVolumes; // Temporary variables
    volScalarField::Internal P22 = P2 * meshVolumes; 
    volScalarField::Internal P33 = P3 * meshVolumes; 
    
    scalar electrodePowerLoss =  gSum(P11);
    scalar electrolytePowerLoss =  gSum(P22);
    scalar kineticPower =  gSum(P33);
    scalar electric_power_loss = kineticPower + electrolytePowerLoss + electrodePowerLoss; // total overpotential
    
    scalar electrodeOhmicLoss =  electrodePowerLoss / totalTopCurrent;
    scalar electrolyteOhmicLoss =  electrolytePowerLoss / totalTopCurrent;
    scalar kineticLoss =  kineticPower / totalTopCurrent;
    scalar total_overpotential = kineticLoss + electrolyteOhmicLoss + electrodeOhmicLoss;

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
    Info << "\nElectrode ohmic losses (V): " << electrodeOhmicLoss << endl;
    Info << "Electrolyte ohmic losses (V): " << electrolyteOhmicLoss << endl;        
    Info << "Kinetic/Transport losses (V): " << kineticLoss << endl;

    Info << "\nFlow Power (J/s): " << flowLoss << endl;
    Info << "Electric Power (J/s): " << electric_power_loss << endl;        //electricLoss
    Info << "Total Power Loss (J/s): " << flowLoss + electric_power_loss << endl;    

    //Print total pressure drop
    Info << "\nInlet peressure (Pa): " << surfaceAvgInletPressure << endl;
    Info << "\nOutlet peressure (Pa): " << surfaceAvgOutletPressure << endl;
    Info << "\nPressure drop (Pa): " << surfaceAvgInletPressure - surfaceAvgOutletPressure << endl;

    //Print "polarization curve" data
    Info << "\ni (mA/cm^2) ,  eta (V) : ";
    Info <<  averagedTopCurrentDensity / 10.0 << " , " << total_overpotential << endl;
    // Division by 10 is due to conversion from A/m^2 to mA/cm^2
}
