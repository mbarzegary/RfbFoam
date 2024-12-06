// Extract transport properties from the meso-scale dataset files (if the -useDataSet argument is used)
void useInterpInData()
{
    volScalarField          & porosity          = *modelParams.porosity;
    const cellList          &cells              = *modelParams.cells;
    const dimensionedScalar & rmin              = *modelParams.rmin;
    const dimensionedScalar & rmax              = *modelParams.rmax;
    List<double>            & radiusInterpIn    = *modelParams.radiusInterpIn;
    List<double>            & permInterpIn      = *modelParams.permInterpIn;
    List<double>            & areaInterpIn      = *modelParams.areaInterpIn;    
    dimensionedScalar       & unitCellLen       = *modelParams.unitCellLen;
    const double            & deltaRInterp      = *modelParams.deltaRInterp;
    
    
    (*modelParams.gamma).correctBoundaryConditions();
    
    forAll(cells, cellI)
    {
        // Extract delta from the por.txt field
        double deltaPorInterpln = (*modelParams.por)[1]-(*modelParams.por)[0];

        // Evaluate the indices in por.txt according to the set 'porosity' field
        int nPorInterp = floor( ( porosity[cellI] - (*modelParams.por)[0] )
                                 /deltaPorInterpln);
        double por0 = (*modelParams.por)[nPorInterp];
        double por1 = (*modelParams.por)[nPorInterp+1];  

        // Interpolate between por0 and por1
        double tt = ((*modelParams.porosity)[cellI] - por0) / (por1 - por0);

        // Back-calculate gamma, according to porosity
        (*modelParams.gamma)[cellI] = (  tt * (*modelParams.gamma_vs_por)[nPorInterp+1] 
                                     + (1. - tt) * (*modelParams.gamma_vs_por)[nPorInterp]); 
        
        // Use back-calculated gamma to extract the other relevant values, except porosity itself
        double rIn = rmax.value() 
                             + (*modelParams.gamma)[cellI] 
                             * (rmin.value() - rmax.value());
                               
            int nInterp = floor( ( rIn / unitCellLen.value() 
                                   -radiusInterpIn[0] )
                                 /deltaRInterp );
            
            double r0 = radiusInterpIn[nInterp];
            double r1 = radiusInterpIn[nInterp+1];
            
            double t = ( rIn / unitCellLen.value() - r0 )/
                       ( r1 - r0 );                       

            (*modelParams.rFiber)[cellI] = rIn;     
            (*modelParams.invPermeability)[cellI]= (  t * permInterpIn[nInterp+1] 
                                     + (1. - t) * permInterpIn[nInterp])
                                     / unitCellLen.value() / unitCellLen.value(); 

            (*modelParams.areaPerVol)[cellI] = (  t * areaInterpIn[nInterp+1] 
                                     + (1. - t) * areaInterpIn[nInterp])
                                     / unitCellLen.value();
          

            if (t > 1.0 && t < 1.000001) {t=1.0;}   

            if ( t  < 0.0 || t > 1.0 )
            {
                Info << "INTERPOLATION ERROR! At rIn: " << rIn << endl;
                Info << "rmin: " << rmin << endl;
                Info << "rmax: " << rmax << endl;
                Info << "radiusInterpIn[0]: " << radiusInterpIn[0] << endl;
                Info << "deltaRInterp: " << deltaRInterp << endl;
                Info << "nInterp: " << nInterp << endl;
                Info << "r0 = " << r0 << endl;
                Info << "r1 = " << r1 << endl;
                Info << "unitCellLen = " << unitCellLen.value() << endl;
            }
    }
    
    (*modelParams.porosity).correctBoundaryConditions();
}

// Compute Sherwood from meso-scale data (if the -useDataSet argument is used)
void computeSherwoodCoeff()
{
    volScalarField          & bR                = *modelParams.bR;
    volScalarField          & bO                = *modelParams.bO;  
    const dimensionedScalar & c1_km             = *modelParams.c1_km;
    const dimensionedScalar & c2_km             = *modelParams.c2_km;
    const dimensionedScalar & c3_km             = *modelParams.c3_km;
    const dimensionedScalar & c4_km             = *modelParams.c4_km;
    volScalarField          & porosity          = *modelParams.porosity;
    dimensionedScalar       & DCO               = *modelParams.DCO;
    dimensionedScalar       & DCR               = *modelParams.DCR;
    volScalarField          & rFiber            = *modelParams.rFiber;  
    const dimensionedScalar & velRef            = *modelParams.velRef;
    const dimensionedScalar & lenRef            = *modelParams.lenRef; 


    // Compute parameters needed for the Sherwood correlation
        *modelParams.bR = c3_km + c4_km * porosity;
        *modelParams.bO = bR;
            
        *modelParams.gR = DCR/(rFiber+1.0e-20*lenRef) 
                                          * c1_km/Foam::pow(porosity+1.0e-20,c2_km)
                                          * Foam::pow(velRef*rFiber/DCR,bR);
        *modelParams.gO = DCO/(rFiber+1.0e-20*lenRef) 
                                          * c1_km/Foam::pow(porosity+1.0e-20,c2_km)
                                          * Foam::pow(velRef*rFiber/DCO,bO);
        (*modelParams.gR).correctBoundaryConditions();
        (*modelParams.gO).correctBoundaryConditions();
}
