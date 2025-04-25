fvScalarMatrix COEqn
            (
                fvm::div(phi, CO)
              - fvm::laplacian((DCO*porosity/tau), CO)
              //////// Bruggeman correlation 
              // Comment out the above line and uncomment the one below to revert back to Bruggeman
              // - fvm::laplacian(sqrt(Foam::pow(porosity,3))*DCO, CO)
              + fvm::Sp(
                        areaPerVol*Foam::exp(-1.0*alphaC * FRT * deltaPhi )
                        /
                        (   nelec * faradayConstant * CRef / i0
                          + Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                          + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) / kmO
                        )
                        , CO)
              - areaPerVol * Foam::exp(alphaA * FRT * deltaPhi ) * CR
                        /
                        (   nelec * faradayConstant * CRef / i0
                          + Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                          + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) / kmO
                        )        
            );
                                    
            COEqn.relax();
            solverPerformance spCO = COEqn.solve();
            
            fvScalarMatrix CREqn
            (
                fvm::div(phi, CR)
              - fvm::laplacian((DCR*porosity/tau), CR)
              //////// Bruggeman correlation 
              // Comment out the above line and uncomment the one below to revert back to Bruggeman
              // - fvm::laplacian(sqrt(Foam::pow(porosity,3))*DCR, CR)
              + fvm::Sp(
                        areaPerVol*Foam::exp(alphaA * FRT * deltaPhi )
                        /
                        (   nelec * faradayConstant * CRef / i0
                          + Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                          + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) / kmO
                        )
                        , CR)
              - areaPerVol * Foam::exp(-1.0*alphaC * FRT * deltaPhi ) * CO
                        /
                        (   nelec * faradayConstant * CRef / i0
                          + Foam::exp(alphaA * FRT * deltaPhi ) / kmR
                          + Foam::exp(-1.0*alphaC * FRT * deltaPhi ) / kmO
                        )     
            );
            
            CREqn.relax();
            solverPerformance spCR = CREqn.solve();
