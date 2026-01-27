---
sidebar_position: 3
title: Solver Options
---

# Solver Options

RfbFoam supports selective equation solving for computational efficiency through command-line flags.

## Execution Modes

| Mode | Flag | Description |
|------|------|-------------|
| **Full simulation** | *(default)* | Solves momentum, mass, and charge transport |
| **Flow only** | `-onlyU` | Solves momentum equations only |
| **Electrochemistry only** | `-onlyScalar` | Solves mass and charge transport only |

### Full Simulation

```bash
RfbFoam
```

Solves all transport equations (momentum, species, and charge) in a coupled manner.

### Flow Only

```bash
RfbFoam -onlyU
```

Solves only the momentum and continuity equations. Use this when you need the velocity and pressure fields without electrochemistry.

### Electrochemistry Only

```bash
RfbFoam -onlyScalar
```

Solves only the species concentration and charge transport equations. Requires a pre-computed velocity field. This is the standard approach for generating polarization curves: solve the flow once, then sweep applied potentials using this mode.

## Parallel Execution

RfbFoam supports parallel execution using OpenFOAM's domain decomposition. The example cases use `scotch` decomposition by default, configured in `system/decomposeParDict`:

```bash
# Decompose the domain
decomposePar

# Run in parallel (e.g., 4 processors)
mpirun -np 4 RfbFoam -parallel

# Reconstruct the results
reconstructPar
```

The provided run scripts (`Run_All.sh`, `Run_U.sh`, `Run_Scalar.sh`) handle decomposition, parallel execution, and reconstruction automatically.
