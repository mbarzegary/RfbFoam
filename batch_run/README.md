# RfbFoam Python Control

This script allows for parametric sweeps of fluid dynamics and electrochemistry simulations using the RfbFoam solver.
Configuration in MainControl.py

Define the following variables:

- `module_path`: Path to the directory containing the Python functions.
- `case_directory`: Path to the OpenFOAM case directory.

## Script Behavior Flags

Control the script's execution by setting the following flags:

- `run_fluid_dynamics`: Set to True to enable fluid dynamics simulations, or False to disable them.
- `run_electrochemistry`: Set to True to enable electrochemistry simulations, or False to disable them.
- `continuation_sweep`: Set to True to continue from the latest available simulation results. If False, the initial conditions from the "0" folder will be used.
- `plot_residual`: Enable or disable the plotting of residuals (True/False).
- `save_residual_plot`: Save residual plots as files (True/False).
- `plot_result`: Enable or disable plotting of electrochemistry simulation results (True/False).
- `save_results_plot`: Save the electrochemistry result plots as image files (True/False).
- `save_results`: Save the full simulation field results (True/False). Note: Set to False to reduce data storage on the cluster.

## OpenFOAM Setup

Ensure the controlDict file is configured as follows for proper continuation sweeps:

```
startFrom       latestTime;
stopAt          writeNow;
```

## Output Management

Results are saved automatically in a sub-folder named after the applied potential used in the parametric sweep.
In this folder are saved to default also the log files relative to the simulation.

The main output directory is named based on the specified x, y, z components of the velocity vector.
If these parameter lists are left empty, the folder name refers to the values in the "0" folder.
In this folder is saved a txt file containing the macro results of the simulations as current density, 
overpotential, power losses etc. directly extracted from the log files.

```
📁 Case Directory/
│
├── 📁 Ux0.05_Uy0.00_Uz0.00/    # Main folder named after velocity components (x, y, z)
│   │
│   ├── Results.txt                 # Contains extracted macro results
│   ├── residual_plot_X.svg         # (if enabled) Residuals plot (of the specified x potential simulation)
│   ├── result_plot_X.svg           # (if enabled) Result plot (Overpotential curve up to the specified X potential)
│   │
│   ├── 📁 Potential_0.8V/          # Sub-folder named after applied potential (e.g., 0.8V)
│   │   ├── log.electrochemistry    # Log file for electrochemistry simulation
│   │   └── postProcessing/         # (if enabled) Post-processed OpenFOAM results
│   │
│   ├── 📁 Potential_1.0V/
│   │   └── ...                     # Same structure for each parametric potential
│
├── 📁 Ux0.10_Uy0.00_Uz0.00/
│   └── ...
```

Intermediate time folders (e.g., "1", "2", ...) are deleted after each simulation, preserving only the final 
time step of each sweep.
