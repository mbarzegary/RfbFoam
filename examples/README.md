# Examples and Validation Cases

This directory contains ready-to-run simulation cases and validation studies for the RfbFoam solver. All cases model a symmetric half-cell setup of a redox flow battery, containing flow-through (FTFF) or interdigitated (IDFF) flow field configurations.

## Prerequisites

- OpenFOAM v2406 or later (openfoam.com version) with the environment sourced
- RfbFoam solver compiled (run `./Allwmake` from the repository root)

## Cases

### cases/FTFF_Model

Flow-Through Flow Field (FTFF) design. The electrolyte enters from one face of the porous electrode and exits from the opposite face, passing entirely through the electrode thickness. The mesh is generated using `blockMesh` and `snappyHexMesh` with STL geometry files defining the inlet, outlet, membrane, current collector, and walls.

### cases/IDFF_Model

Interdigitated Flow Field (IDFF) design. The electrolyte is forced through the porous electrode between alternating inlet and outlet channels, creating a predominantly through-plane flow pattern. Uses the same meshing approach and solver configuration as the FTFF case with a different channel geometry.

## Verification and Validation

### verification-validation/FTFF_Model.run_470um

A validation study of the FTFF case with a thickness of 470 µm. Results from this case are compared against COMSOL simulations and experimental data. The comparison plots are located in:

- `verification-validation/plot_results/compare_comsol/` — overpotential comparison with COMSOL
- `verification-validation/plot_results/validation/` — polarization curve validation against experimental measurements

## Running a Case

Each case provides an `Allrun` script for execution and an `Allclean` script for resetting:

```bash
cd cases/FTFF_Model
./Allrun       # Prepare mesh and run the simulation
./Allclean     # Remove all generated files and reset the case
```

### Additional Run Scripts

Each case also provides individual scripts for running specific stages:

| Script | Description |
|--------|-------------|
| `Run_Prepare_Only.sh` | Mesh generation and field initialization only |
| `Run_All.sh` | Full workflow: mesh generation, decomposition, parallel solve, and reconstruction |
| `Run_U.sh` | Solve momentum equations only (`-onlyU` flag) |
| `Run_Scalar.sh` | Solve mass and charge transport only (`-onlyScalar` flag) |

A typical workflow for decoupled simulations is to run the flow field first, then solve the electrochemistry on the converged velocity field:

```bash
./Run_Prepare_Only.sh
./Run_U.sh
./Run_Scalar.sh
```

## Case Structure

Each case follows the standard OpenFOAM directory layout:

```
<case>/
├── Allrun                  # Run script
├── Allclean                # Clean script
├── 0/                      # Initial and boundary conditions
│   ├── U, p                # Flow fields
│   ├── CO, CR              # Species concentration fields
│   ├── Phi1, Phi2          # Potential fields
│   └── porosity, kappa1, … # Material property fields
├── 0.orig/                 # Original fields overwritten by setExprFields
│   ├── areaPerVol          # Specific surface area
│   ├── invPermeability     # Inverse permeability
│   ├── kappa1              # Solid-phase conductivity
│   ├── porosity            # Electrode porosity
│   └── tau                 # Tortuosity
├── constant/
│   ├── transportProperties # Physical and electrochemical parameters
│   ├── turbulenceProperties
│   └── triSurface/         # STL geometry files
└── system/
    ├── controlDict         # Simulation control
    ├── fvSchemes           # Discretization schemes
    ├── fvSolution          # Linear solver settings
    ├── blockMeshDict       # Background mesh definition
    ├── snappyHexMeshDict   # Mesh refinement
    └── decomposeParDict    # Parallel decomposition settings
```
