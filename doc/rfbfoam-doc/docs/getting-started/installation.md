---
sidebar_position: 1
title: Installation
---

# Installation

## Prerequisites

- **OpenFOAM v2406 or later** ([openfoam.com](https://www.openfoam.com/) version)
- **GCC compiler**
- **Python 3.x** (for batch processing tools)

## OpenFOAM Environment Setup

RfbFoam requires an active OpenFOAM environment. You must set up the OpenFOAM environment before compiling the solver or running simulations.

### Method 1: User Profile Setup (Recommended)

Add the following line to your `~/.bashrc` file:

```bash
source /usr/lib/openfoam/openfoam2406/etc/bashrc
```

Then reload your shell:

```bash
source ~/.bashrc
```

### Method 2: Module System (HPC/Cluster Environments)

```bash
module load OpenFOAM/v2406-foss-2023a
```

### Method 3: Manual Activation (Per Session)

```bash
source /usr/lib/openfoam/openfoam2406/etc/bashrc
```

### Method 4: Alias Setup

Add to your `~/.bashrc`:

```bash
alias of2406='source /usr/lib/openfoam/openfoam2406/etc/bashrc'
```

Then use `of2406` before working with OpenFOAM.

:::tip
The exact path may vary depending on your OpenFOAM installation. Common locations include:
- `/usr/lib/openfoam/openfoam2406/etc/bashrc` (system installation)
- `/opt/openfoam2406/etc/bashrc`
- `~/OpenFOAM/OpenFOAM-v2406/etc/bashrc` (user installation)
:::

### Verify OpenFOAM Environment

```bash
echo $WM_PROJECT_VERSION
```

This should return your OpenFOAM version (e.g., `v2406`).

## Compiling RfbFoam

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
