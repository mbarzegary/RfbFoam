---
sidebar_position: 4
title: Batch Processing
---

# Batch Processing

RfbFoam includes a Python automation framework for performing parametric sweeps of applied potentials and inlet velocities, making it straightforward to generate polarization curves. The scripts are located in the `batch_run/` directory.

## Overview

The batch processing system automates:
1. Running a single flow simulation for each inlet velocity
2. Sweeping applied potentials using the converged velocity field
3. Extracting macro results (current density, overpotential, power losses) from log files
4. Optional plotting and saving of results and residuals

## Configuration

The main entry point is `batch_run/MainControl.py`. Define the following variables:

- **`case_directory`**: Path to the OpenFOAM case directory

### Parametric Sweep Definitions

Define the parameters for the parametric sweep:

| Parameter | Description |
|-----------|-------------|
| `potentials` | List of applied potentials to sweep (e.g., `[-0.77, -0.8, -0.85, -0.9]`) |
| `velocity` | List of inlet velocity vectors as tuples (e.g., `[(0, 0, -0.005), (0, 0, -0.01)]`) |
| `results_folder_name` | Index of the velocity component to use for naming result folders (0=x, 1=y, 2=z) |

### Script Behavior Flags

Control the script's execution by setting these flags in `MainControl.py`:

| Flag | Description |
|------|-------------|
| `run_fluid_dynamics` | Enable/disable fluid dynamics simulations |
| `run_electrochemistry` | Enable/disable electrochemistry simulations |
| `continuation_sweep` | `True`: continue from latest results; `False`: use initial conditions from `0/` folder |
| `plot_residual` | Enable/disable plotting of residuals |
| `save_residual_plot` | Save residual plots as files |
| `plot_result` | Enable/disable plotting of electrochemistry results |
| `save_results_plot` | Save result plots as image files |
| `save_results` | Save the full simulation field results (set to `False` to reduce storage on clusters) |

### OpenFOAM Setup

Ensure the `controlDict` file is configured as follows for proper continuation sweeps:

```
startFrom       latestTime;
stopAt          writeNow;
```

## Running a Parametric Sweep

```bash
cd batch_run
python MainControl.py
```

The script will iterate over the specified velocities and applied potentials, running each simulation and collecting results.

## Output Structure

Results are organized in a directory hierarchy based on velocity and potential:

```
Case_Directory/
├── Ux0.05_Uy0.00_Uz0.00/          # Folder named after velocity components
│   ├── Results.txt                  # Extracted macro results (current density,
│   │                                # overpotential, power losses, etc.)
│   ├── residual_plot_X.svg          # (if enabled) Residual plot
│   ├── result_plot_X.svg            # (if enabled) Overpotential curve plot
│   │
│   ├── Potential_0.8V/              # Sub-folder for each applied potential
│   │   ├── log.electrochemistry     # Log file for the simulation
│   │   └── postProcessing/          # (if enabled) Post-processed results
│   │
│   └── Potential_1.0V/
│       └── ...
│
└── Ux0.10_Uy0.00_Uz0.00/
    └── ...
```

- The main output directory is named based on the velocity vector components (x, y, z). If velocity parameter lists are left empty, the folder name uses values from the `0/` directory.
- A `Results.txt` file in each velocity folder contains the macro results extracted from the log files.
- Intermediate time folders (e.g., `1`, `2`, ...) are deleted after each simulation, preserving only the final time step.

## Script Architecture

The batch processing system is organized into modules in `batch_run/Functions/`:

| Module | Purpose |
|--------|---------|
| `main_funct.py` | Core orchestration logic |
| `run_funct.py` | OpenFOAM case execution |
| `control_funct.py` | Control and configuration management |
| `plot_funct.py` | Result visualization and plotting |
