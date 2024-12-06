fvScalarMatrix Phi1Eqn
            (
                -1.0*fvm::laplacian(sqrt(Foam::pow(1-porosity,3))*kappa1, Phi1)
                + fvm::Sp(areaPerVol * i0 / CRef.value() * //division by CRef added
                            ( FRT *
                                  (
                                     CR * alphaA 
                                   * Foam::exp(alphaA * FRT * deltaPhi)
                                   + CO * alphaC 
                                   * Foam::exp(-1.0*alphaC * FRT * deltaPhi)
                                  )
                              +   
                                  (
                                     i0 / nelec / CRef * FRT / faradayConstant
                                   * Foam::exp((alphaA-alphaC) * FRT * deltaPhi)
                                   * ( alphaA + alphaC )
                                   * ( CR / kmO + CO / kmR )
                                  )
                             )
                          /
                             Foam::pow(
                                  1. + i0 / nelec / faradayConstant / CRef
                                 *
                                 (  
                                    Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                                  + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) 
                                    / kmO
                                 )
                                 , 2)                            
                           , Phi1)                            
                + areaPerVol * i0   / CRef.value() * //division by CRef added
                  (
                     CR * Foam::exp(alphaA * FRT * deltaPhi)
                   - CO * Foam::exp(-1.0*alphaC * FRT * deltaPhi)
                  )
                  /
                  ( 1. + i0/nelec/faradayConstant/CRef
                       * (
                             Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                           + Foam::exp(-1.0*alphaC * FRT * deltaPhi) / kmO
                         )
                  )
                  - areaPerVol * i0 * Phi1 / CRef.value() * //division by CRef added
                            ( FRT *
                                  (
                                     CR * alphaA 
                                   * Foam::exp(alphaA * FRT * deltaPhi)
                                   + CO * alphaC 
                                   * Foam::exp(-1.0*alphaC * FRT * deltaPhi)
                                  )
                              +   
                                  (
                                     i0 / nelec / CRef * FRT / faradayConstant
                                   * Foam::exp((alphaA-alphaC) * FRT * deltaPhi)
                                   * ( alphaA + alphaC )
                                   * ( CR / kmO + CO / kmR )
                                  )
                             )
                          /
                             Foam::pow(
                                  1. + i0 / nelec / faradayConstant / CRef
                                 *
                                 (  
                                    Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                                  + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) 
                                    / kmO
                                 )
                                 , 2)                           
                            
            );
            
            //Phi1Eqn.relax();
            solverPerformance spPhi1 = Phi1Eqn.solve();
            
            fvScalarMatrix Phi2Eqn
            (
                -1.0*fvm::laplacian(sqrt(Foam::pow(porosity,3))*kappa2, Phi2) 
                + fvm::Sp(areaPerVol * i0 / CRef.value() * //division by CRef added
                            ( FRT *
                                  (
                                     CR * alphaA 
                                   * Foam::exp(alphaA * FRT * deltaPhi)
                                   + CO * alphaC 
                                   * Foam::exp(-1.0*alphaC * FRT * deltaPhi)
                                  )
                              +   
                                  (
                                     i0 / nelec / CRef * FRT / faradayConstant
                                   * Foam::exp((alphaA-alphaC) * FRT * deltaPhi)
                                   * ( alphaA + alphaC )
                                   * ( CR / kmO + CO / kmR )
                                  )
                             )
                          /
                             Foam::pow(
                                  1. + i0 / nelec / faradayConstant / CRef
                                 *
                                 (  
                                    Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                                  + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) 
                                    / kmO
                                 )
                                 , 2)                            
                           , Phi2)                            
                - areaPerVol * i0 / CRef.value() * //division by CRef added 
                  (
                     CR * Foam::exp(alphaA * FRT * deltaPhi)
                   - CO * Foam::exp(-1.0*alphaC * FRT * deltaPhi)
                  )
                  /
                  ( 1. + i0/nelec/faradayConstant/CRef
                       * (
                             Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                           + Foam::exp(-1.0*alphaC * FRT * deltaPhi) / kmO
                         )
                  )
                  - areaPerVol * i0 * Phi2 / CRef.value() * //division by CRef added
                            ( FRT *
                                  (
                                     CR * alphaA 
                                   * Foam::exp(alphaA * FRT * deltaPhi)
                                   + CO * alphaC 
                                   * Foam::exp(-1.0*alphaC * FRT * deltaPhi)
                                  )
                              +   
                                  (
                                     i0 / nelec / CRef * FRT / faradayConstant
                                   * Foam::exp((alphaA-alphaC) * FRT * deltaPhi)
                                   * ( alphaA + alphaC )
                                   * ( CR / kmO + CO / kmR )
                                  )
                             )
                          /
                             Foam::pow(
                                  1. + i0 / nelec / faradayConstant / CRef
                                 *
                                 (  
                                    Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                                  + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) 
                                    / kmO
                                 )
                                 , 2)       
            );
            
            //Phi2Eqn.relax();
            solverPerformance spPhi2 = Phi2Eqn.solve();
