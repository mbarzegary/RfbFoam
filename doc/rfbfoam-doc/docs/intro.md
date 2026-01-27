---
sidebar_position: 1
title: Introduction
---

# RfbFoam

RfbFoam is an open-source computational fluid dynamics (CFD) solver for simulating redox flow battery (RFB) systems in [OpenFOAM](https://www.openfoam.com/). It couples momentum, mass, and charge transport phenomena to model electrochemical processes in porous electrode structures with Butler-Valmer kinetics and mass transfer effects.

Redox flow batteries are rechargeable electrochemical devices that store electrical energy in reversible redox couples dissolved in flowing electrolyte solutions. They are a promising technology for grid-scale stationary energy storage, offering the key advantage of decoupling power density (stack size) from energy capacity (electrolyte volume). RfbFoam provides a continuum-scale simulation tool to study the coupled hydrodynamic and electrochemical performance of RFB half-cells, enabling design optimization and performance prediction.

## Key Features

- **Coupled multiphysics**: Simultaneous solving of momentum, mass, and charge transport with electrochemical reactions
- **Symmetric cell modeling**: Half-cell setup for redox flow battery analysis
- **Porous media treatment**: Support for both Bruggeman correlation and explicit tortuosity effects
- **Advanced boundary conditions**: Custom `fixedCurrent` boundary condition for current density control
- **Parametric studies**: Automated batch processing via Python scripts for generating polarization curves
- **Geometric flexibility**: Complex flow field geometries including FTFF and IDFF designs via STL-based meshing
- **Validation cases**: Comparison with COMSOL Multiphysics and experimental data

## What Does RfbFoam Solve?

RfbFoam addresses the multiphysics nature of redox flow batteries by simultaneously solving:

| Physics | Description |
|---------|-------------|
| **Momentum transport** | Fluid flow through porous media (Navier-Stokes with Darcy-Forchheimer terms) |
| **Mass transport** | Species diffusion and convection for oxidized (O) and reduced (R) species |
| **Charge transport** | Electrical potential in both solid (electrode) and liquid (electrolyte) phases |
| **Electrochemical reactions** | Butler-Volmer kinetics with mass transfer limitations |

## Repository Structure

```
RfbFoam/
├── src/                     # Solver source code
│   ├── RfbFoam.C           # Main solver application
│   ├── BCs/                # Custom boundary conditions
│   ├── equations/          # Transport equation definitions
│   └── Make/               # Compilation settings
├── examples/               # Example cases and tutorials
│   ├── cases/              # Ready-to-run simulation cases
│   └── verification-validation/  # Validation against experimental data
├── batch_run/              # Python tools for parametric studies
└── doc/                    # This documentation
```

## Applications

- **Battery design optimization**: Flow field geometry and electrode structure evaluation
- **Performance prediction**: Current-voltage characteristics and efficiency analysis
- **Scale-up studies**: From lab-scale to industrial systems
- **Parametric analysis**: Operating conditions and material properties sweeps
- **Validation studies**: Model verification against experimental data

## License

RfbFoam is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html).

## Acknowledgements

This research has received funding from the European Union's Horizon 2020 research and innovation programme under the Marie Sklodowska-Curie grant agreement No 101150565 (TopeSmash project). Moreover, this work is supported by the ERC grant FAIR-RFB (ERC-2021-STG 101042844).

:::note
Detailed modeling of membrane processes (determining ohmic losses and crossover) is not included in the current version.
:::
