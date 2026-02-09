# RfbFoam: Redox Flow Battery Solver for OpenFOAM

[![OpenFOAM Version](https://img.shields.io/badge/OpenFOAM-v2406+-blue.svg)](https://openfoam.com/)
[![License](https://img.shields.io/badge/License-GPL--3.0-green.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![Documentation](https://img.shields.io/badge/docs-online-blue.svg)](https://mbarzegary.github.io/RfbFoam/)

RfbFoam is a specialized computational fluid dynamics (CFD) solver developed for simulating redox flow battery (RFB) systems in OpenFOAM. The solver couples momentum, mass, and charge transport phenomena to model electrochemical processes in porous electrode structures with accurate representation of Butler-Volmer kinetics and mass transfer effects.

![RfbFoam banner](assets/banner.jpg)

## Overview

This solver addresses the complex multiphysics nature of redox flow batteries by simultaneously solving:
- **Momentum transport**: Fluid flow through porous media (Navier-Stokes with Darcy-Forchheimer terms)
- **Mass transport**: Species diffusion and convection for oxidized (O) and reduced (R) species
- **Charge transport**: Electrical potential in both solid (electrode) and liquid (electrolyte) phases
- **Electrochemical reactions**: Butler-Volmer kinetics with mass transfer limitations

## Key Features

- **Symmetric cell modeling**: Half-cell setup for redox flow battery analysis
- **Porous media treatment**: Support for both Bruggeman correlation and tortuosity effects
- **Advanced boundary conditions**: Custom `fixedCurrent` boundary condition for current density control  
- **Parametric studies**: Automated batch processing for making polarization curves
- **Validation cases**: Comparison with COMSOL and experimental data

## Solver Capabilities

### Transport Phenomena
- Convection-diffusion equations for redox species (CO, CR)
- Poisson equations for electrical potentials (Φ₁ solid, Φ₂ liquid)
- Incompressible flow with porous media effects
- Effective transport properties accounting for porosity and tortuosity

### Electrochemical Modeling
- Butler-Volmer kinetics with anodic and cathodic charge transfer coefficients
- Mass transfer limitations via convective mass transfer coefficients
- Exchange current density parameterization
- Standard electrode potential effects

### Geometric Flexibility
- Structured and unstructured mesh support
- Complex flow field geometries (examples provided for FTFF and IDFF designs)
- 3D porous electrode structures
- Surface area per volume parameterization

## Repository Structure

```
RfbFoam/
├── src/                           # Solver source code
│   ├── RfbFoam.C                  # Main solver application
│   ├── BCs/                       # Custom boundary conditions
│   ├── equations/                 # Transport equation definitions
│   └── Make/                      # Compilation settings
├── examples/                      # Example cases and tutorials
│   ├── cases/                     # Ready-to-run simulation cases
│   │   ├── FTFF_Model/            # Flow-through flow field case
│   │   └── IDFF_Model/            # Interdigitated flow field case
│   └── verification-validation/   # Validation against experimental data
├── batch_run/                     # Python tools for polarization curves
├── doc/                           # Documentation source
│   └── rfbfoam-doc/               # Docusaurus documentation website
├── Allwmake                       # Build script
└── Allwclean                      # Clean script
```

## Quick Start

### Prerequisites
- OpenFOAM v2406 or later (.com version)
- GCC compiler
- Python 3.x (for batch processing tools)

### OpenFOAM Environment Setup

**Important**: RfbFoam requires an active OpenFOAM environment to run. You must set up the OpenFOAM environment before using any of the provided scripts or running simulations.

#### Method 1: User Profile Setup (Recommended)
Add the following line to your `~/.bashrc` file:
```bash
source /usr/lib/openfoam/openfoam2406/etc/bashrc
```
Then reload your shell:
```bash
source ~/.bashrc
```

#### Method 2: Module System (HPC/Cluster environments)
```bash
module load OpenFOAM/v2406-foss-2023a
```

#### Method 3: Manual Activation (per session)
```bash
source /usr/lib/openfoam/openfoam2406/etc/bashrc
```

#### Method 4: Alias Setup
Add to your `~/.bashrc`:
```bash
alias of2406='source /usr/lib/openfoam/openfoam2406/etc/bashrc'
```
Then use `of2406` before working with OpenFOAM.

**Note**: The exact path may vary depending on your OpenFOAM installation. Common locations include:
- `/usr/lib/openfoam/openfoam2406/etc/bashrc` (system installation)
- `/opt/openfoam2406/etc/bashrc`
- `~/OpenFOAM/OpenFOAM-v2406/etc/bashrc` (user installation)

Verify your OpenFOAM environment is active by checking:
```bash
echo $WM_PROJECT_VERSION
```
This should return your OpenFOAM version (e.g., "v2406").

### Installation

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd RfbFoam
   ```

2. **Compile the solver**:
   ```bash
   cd src/
   wmake
   ```

   Or alternatively using the build script in the repository root:

   ```bash
   ./Allwmake
   ```

3. **Verify installation**:
   ```bash
   RfbFoam -help
   ```

### Running a Case

1. **Navigate to an example case**:
   ```bash
   cd examples/cases/FTFF_Model/
   ```
2. **Prepare the mesh and run the simulation**:
   ```bash
   ./Run_All.sh
   ```

Alternatively, you can use `Allrun` and `Allclean` scripts in cases directories to run and clean the simulations.

> **Note on Parallel Execution**: The example cases are configured to run in parallel using **16 CPU cores** by default. If your system has fewer cores, you must modify the `system/decomposeParDict` file before running. Change the `numberOfSubdomains` value to match your available CPU cores (e.g., `numberOfSubdomains 4;` for 4 cores). Running with more subdomains than available cores will cause performance issues or failures.

## Solver Options

RfbFoam supports selective equation solving for computational efficiency:

- **Full simulation** (default): Solves momentum, mass, and charge transport
- **Flow only** (`-onlyU`): Solves momentum equations only
- **Electrochemistry only** (`-onlyScalar`): Solves mass and charge transport only

## Applications

- **Battery design optimization**: Flow field geometry and electrode structure
- **Performance prediction**: Current-voltage characteristics and efficiency
- **Scale-up studies**: From lab-scale to industrial systems
- **Parametric analysis**: Operating conditions and material properties
- **Validation studies**: Model verification against experimental data

## Documentation

Comprehensive documentation is available online [here](https://mbarzegary.github.io/RfbFoam/). The documentation includes detailed installation instructions, solver theory, case setup guides, and tutorials for running simulations.

## Publication

If you use RfbFoam in your research, please cite the following paper:

>  M. Barzegari, M. De Waal, D. Bordignon, A. Rosinaite, B. Liu, A. Forner-Cuenca. *RfbFoam: An open-source framework for porous electrode modeling in redox flow batteries*. ChemRxiv. 2026.
> https://doi.org/10.26434/chemrxiv.10001658/v1

```bibtex
@article{Barzegari2026rfbfoam,
  author    = {Barzegari, Mojtaba and De Waal, Martin and Bordignon, Davide and Rosinaite, Adele and Liu, Baichen and Forner-Cuenca, Antoni},
  title     = {RfbFoam: An open-source framework for porous electrode modeling in redox flow batteries},
  year      = {2026},
  doi       = {10.26434/chemrxiv.10001658/v1},
  publisher = {ChemRxiv},
}
```

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.
