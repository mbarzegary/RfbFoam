/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    RfbFoam

Description
    Solves the steady transport equation of meomentum, mass and charge for the 
    symmetric cell (half cell) setup of a redox flow battery (RFB).


\*---------------------------------------------------------------------------*/

#include "RfbFoam.H"
#include "mesoInterpolation.H"
#include "functions.H"
#include "solveEqns.H"

int main(int argc, char *argv[])
{
    // Enable the use of an argument to the solver that controls if the dataset is going to be used
    argList::addBoolOption
    (
     "useDataSet",
     "Employs .txt datasets"
    );

    argList::addBoolOption
    (
     "onlyU",
     "Solves the momentum transport only"
    );

    argList::addBoolOption
    (
     "onlyScalar",
     "Solves the charge and mass transport only"
    );

    Foam::argList args(argc, argv); 
    onlyU = args.found("onlyU");
    onlyScalar = args.found("onlyScalar");

    // Check for fatal errors     
    if (!args.checkRootCase())
    {
        Foam::FatalError.exit();
    } 

    Info << "Create time\n" << endl;
    Foam::Time runTime(Foam::Time::controlDictName, args);

    // Print the time from which simulation commences
    Info << "Create mesh for time = " << runTime.timeName() << nl << endl;

    // Create the mesh 
    Foam::fvMesh mesh
    (
        Foam::IOobject
        (
            Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            Foam::IOobject::MUST_READ
        )
    );
    
    simpleControl simple(mesh);

    // Read in the fields from the createFields.H file
    #include "createFields.H"
    
    fv::options& fvOptions(fv::options::New(mesh));

    turbulence->validate();

    // Initialize variables and datasets
    #include "initialize.H"

    if (args.found("useDataSet"))
    {
      Info << "Found -useDataSet argument \nUsing mesoscale data and Sherwood correlation. " << endl;
        // Use mesoscale Interpolation data to assign values of fiber radius, permeability, area per volume according to porosity via gamma
        useInterpInData();

        // Compute the coefficients for the mesoscale Sherwood mass transfer correlation
        computeSherwoodCoeff();
    }
    else
    {
      Info << "Using simple MT correlation of the form km = g_km *mag(U)^b_km" << endl;
        // Set variables "g_km" and "b_km" for a simple mass transfer correlation of the form km = g_km *mag(U)^b_km
        setSimpleMTCoeff();
    }

    // Correct properties in flow field channels
    correctChannelProperties();

    // START of the solution loop
     while (simple.loop())
     {
        // Print time at which simulation starts
        Info << nl << "Time = " << runTime.timeName() << nl << endl;                        
      
        // Solve the governing equations       
        solveGoverningEqns();                                         

        // Write selected field variables to the corresponding time folder    
        runTime.write();  
        
        // Print the final results of the simulation
        printOutput();

        Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
             << "  ClockTime = " << runTime.elapsedClockTime() << " s"
             << nl << endl;
                    
      }
    
    Info << "End" << nl << endl;

    return 0;
}