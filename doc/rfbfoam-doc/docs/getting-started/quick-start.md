---
sidebar_position: 2
title: Quick Start
---

# Quick Start

This guide walks you through running your first RfbFoam simulation using the provided example cases. Make sure you have completed the [Installation](installation) steps first.

## Running a Case

1. **Navigate to an example case**:
   ```bash
   cd examples/cases/FTFF_Model/
   ```

2. **Run the full simulation**:
   ```bash
   ./Allrun
   ```

   This script handles mesh generation, field initialization, domain decomposition, parallel solving, and reconstruction.

3. **Clean up** (to reset the case):
   ```bash
   ./Allclean
   ```

## Available Run Scripts

Each example case provides individual scripts for running specific stages:

| Script | Description |
|--------|-------------|
| `Allrun` | Full workflow: prepare, solve, and reconstruct |
| `Allclean` | Remove all generated files and reset the case |
| `Run_Prepare_Only.sh` | Mesh generation and field initialization only |
| `Run_All.sh` | Full workflow: mesh generation, decomposition, parallel solve, and reconstruction |
| `Run_U.sh` | Solve momentum equations only (`-onlyU` flag) |
| `Run_Scalar.sh` | Solve mass and charge transport only (`-onlyScalar` flag) |

## Decoupled Workflow

A common workflow is to solve the flow field first, then run the electrochemistry on the converged velocity field. This is useful for parametric studies where the velocity field remains fixed across different applied potentials:

```bash
./Run_Prepare_Only.sh   # Generate mesh and initialize fields
./Run_U.sh              # Solve momentum equations
./Run_Scalar.sh         # Solve species and charge transport
```

:::tip
The decoupled approach is the recommended workflow for generating polarization curves, since the velocity field does not change with applied potential. Solve the flow once and reuse the results.
:::
