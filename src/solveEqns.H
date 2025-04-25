void solveGoverningEqns()
{ 
    volScalarField          & invPermeability   = *modelParams.invPermeability;
    volScalarField          & forchCoeff        = *modelParams.forchCoeff;
    volScalarField          & porosity          = *modelParams.porosity;
    volScalarField          & areaPerVol        = *modelParams.areaPerVol;       
    volScalarField          & kappa1            = *modelParams.kappa1;
    volScalarField          & tau               = *modelParams.tau;
    dimensionedScalar       & kappa2            = *modelParams.kappa2;
    dimensionedScalar       & DCO               = *modelParams.DCO;
    dimensionedScalar       & DCR               = *modelParams.DCR;
    volVectorField          & U                 = *modelParams.U;
    surfaceScalarField      & phi               = *modelParams.phi;
    volScalarField          & p                 = *modelParams.p;
    label                   & pRefCell          = *modelParams.pRefCell;
    scalar                  & pRefValue         = *modelParams.pRefValue;
    dimensionedScalar       & nu                = *modelParams.nu;
    fv::options             & fvOptions         = *modelParams.fvOptions;
    singlePhaseTransportModel &laminarTransport = *modelParams.laminarTransport; 
    volScalarField          & bR                = *modelParams.bR;
    volScalarField          & bO                = *modelParams.bO;
    volScalarField          & gR                = *modelParams.gR;
    volScalarField          & gO                = *modelParams.gO;   
    const dimensionedScalar & velRef            = *modelParams.velRef;
    volScalarField          & CO                = *modelParams.CO;
    volScalarField          & CR                = *modelParams.CR;
    volScalarField          & Phi1              = *modelParams.Phi1;
    volScalarField          & Phi2              = *modelParams.Phi2;
    volScalarField          & kmR               = *modelParams.kmR;
    volScalarField          & kmO               = *modelParams.kmO;    
    dimensionedScalar       & alphaA            = *modelParams.alphaA;
    dimensionedScalar       & alphaC            = *modelParams.alphaC;
    dimensionedScalar       & CRef              = *modelParams.CRef;
    dimensionedScalar       & i0                = *modelParams.i0;
    dimensionedScalar       & FRT               = *modelParams.FRT;
    dimensionedScalar       & nelec             = *modelParams.nelec;
    dimensionedScalar       & stdPotential      = *modelParams.stdPotential;          
    dimensionedScalar       & faradayConstant   = *modelParams.faradayConstant;
    volScalarField          & deltaPhi          = *modelParams.deltaPhi;
    volVectorField          & I1                = *modelParams.I1;
    volVectorField          & I2                = *modelParams.I2;

    if (!onlyScalar) // If we need to solve the momentum transport equation
    {
        int iterVel = 0;
        
        // Pressure-velocity SIMPLE corrector
        double flowRes = 1.1;

        //while ( (iterVel < 2000 && flowRes > 5.0e-6) || iterVel < 5)
        while ( (iterVel < maxFlowIters && flowRes > flowResTol) || iterVel < 5)
        {
            flowRes = 0.0;

            #include "equations/UEqn.H"
            #include "equations/pEqn.H"
        
            U.storePrevIter();
            p.storePrevIter();
        
            iterVel +=1;

            Info << "Flow Iter: " << iterVel
                << ", Flow Res: " << flowRes << endl;                                    
        }

        Info << nl << "Flow loop finished after " << iterVel 
                << " iterations and final residual of " << flowRes << nl <<  endl;
    }
    

    if (!onlyU) // If we need to solve the mass and charge transport equation
    { // Deleted non-orthogonal corrector 
        //Compute mass transfer coefficient according to the solved velocity field

        kmR = gR * Foam::pow(mag(U/velRef)+1.e-20,bR)+1.0e-20*velRef;
        kmO = gO * Foam::pow(mag(U/velRef)+1.e-20,bO)+1.0e-20*velRef;           
                    
        kmR.correctBoundaryConditions();
        kmO.correctBoundaryConditions();


        int iterPot = 0;
        
        double scalarsRes = 1.0;
        
        while ( (iterPot < maxScalarIters && scalarsRes > scalarResTol) || iterPot < 5 )
        {  
        
            scalarsRes = 0.0;
                                                      
            #include "equations/massEqns.H"
            #include "equations/chargeEqns.H"
          
            //deltaPhi = Phi1 -Phi2 - stdPotential;
            deltaPhi = Phi1 -Phi2 - (stdPotential + 1.0/FRT * Foam::log(CO/CR)); //Nerst-Eqn corrected form

            I2 = -1.0*sqrt(Foam::pow(porosity,3))*kappa2*fvc::grad(Phi2);
            I1 = -1.0*sqrt(Foam::pow(1.-porosity,3))*kappa1*fvc::grad(Phi1);    
            
            scalarsRes += spCO.initialResidual() + spCR.initialResidual()
                          + spPhi1.initialResidual() + spPhi2.initialResidual();

            iterPot +=1;

            Info << "Iter Scalars: " << iterPot  
                  <<", Sum of Scalars Residuals: " << scalarsRes 
                  << endl; 
        }
        
        Info << nl << "Scalar loop finished after " << iterPot 
             << " iterations and final residual of " << scalarsRes << nl <<  endl;
    }     
        
    laminarTransport.correct();           
}

