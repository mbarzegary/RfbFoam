---
sidebar_position: 3
title: Verification & Validation
---

# Verification and Validation

RfbFoam has been verified against COMSOL Multiphysics and validated against experimental data from iron symmetric RFB cells. The validation case is located in `examples/verification-validation/`.

## Running the Validation Study

The validation workflow consists of three steps: mesh preparation, parametric batch simulation, and plotting.

### 1. Prepare the Mesh

```bash
cd examples/verification-validation/FTFF_Model.run_470um
./Run_Prepare_Only.sh
```

This generates the mesh (`blockMesh`, `snappyHexMesh`) and initializes the material property fields (`setExprFields`).

### 2. Run the Parametric Sweep

From the repository root, launch the batch controller:

```bash
cd batch_run
python MainControl.py
```

The script sweeps 10 applied potentials at two inlet velocities, producing two polarization curves. All sweep parameters and flags are pre-configured in `MainControl.py` but can be customized (see [Batch Processing](../user-guide/batch-processing) for details).

### 3. Plot the Results

Once all simulations have completed, generate the comparison plots using MATLAB:

```bash
cd examples/verification-validation/plot_results/compare_comsol
matlab -batch "Plot"    # Overpotential comparison with COMSOL

cd ../validation
matlab -batch "Plot"    # Polarization curve validation against experimental data
```

The scripts produce SVG figures saved in their respective directories.

## Verification: Comparison with COMSOL

The verification was conducted through comparison with an equivalent model implemented in COMSOL Multiphysics, with both solvers operating under identical boundary conditions, material properties, and geometric configurations.

![COMSOL vs OpenFOAM comparison](/img/Comsol_OF_comparison.png)
*Comparing the COMSOL model output with RfbFoam results for the FTFF case with 10.58 mL/min flow rate. All geometrical and physical parameters are identical.*

The comparison demonstrates close agreement between the two implementations, with both curves exhibiting nearly identical trends and magnitudes throughout the entire current density range. The slight deviations observed at higher current densities (above 800 mA/cm²) are attributed to differences in numerical discretization schemes and convergence criteria between the two software packages.

## Experimental Validation

The experimental validation was conducted through comparison with laboratory measurements from all-iron RFB cells with NIPS electrodes for the FTFF case.

![Validation results](/img/Validation_plots.jpg)
*Validation results for the experiments with NIPS electrodes in the FTFF setup: (a) half-cell polarization curves at two different inlet superficial velocities, and (b) pressure drop versus flow rate.*

### Polarization Curves

The polarization curves comparing experimental data with model predictions at two different inlet flow rates (4.23 and 10.58 mL/min, converted to 0.51 and 1.37 cm/s inlet velocities) show close agreement with no parameter fitting, with deviations remaining within the experimental uncertainty bounds.

### Pressure Drop

The pressure drop validation confirms the model's ability to accurately predict hydrodynamic behavior. The linear relationship between pressure loss and flow rate is well-reproduced, with predicted values consistently falling within experimental error margins across 5 to 40 mL/min.

### Computational Performance

The validation simulations were executed using the provided Python parametric sweep code on a local workstation employing 16 CPU cores, with each data point of the polarization curve requiring approximately 2 minutes of computational time. For more demanding applications, the model can be readily deployed on HPC environments.

## Experimental Setup and Parameters

The electrode characterization was carried out within a symmetric Fe²⁺/Fe³⁺ cell:

- **Electrode**: NIPS electrode with 2.55 cm² active area, 0.470 mm thickness (~20% compression)
- **Membrane**: Cationic exchange membrane (CEM) FS-950 by Fumasep, 0.05 mm thickness
- **Electrolyte**: 50 mL solution of 0.25M Fe²⁺ and 0.25M Fe³⁺ in 2M HCl
- **Flow rates tested**: 4.23 and 10.58 mL/min (corresponding to 0.51 and 1.37 cm/s inlet velocities)

### Inlet Velocities

| Internal linear velocity [cm/s] | Flow rate [mL/min] | Inlet velocity [cm/s] |
|------|------|------|
| 1 | 4.23 | 0.51 |
| 2.5 | 10.58 | 1.37 |

### Electrode Properties

| Symbol | Description | Value | Source |
|--------|-------------|-------|--------|
| $K$ | Permeability | $(7.30 \pm 2.0) \times 10^{-12}$ m² | Measured |
| $\varepsilon$ | Porosity (compressed) | 0.91 (0.83) | Measured |
| $a$ | Surface area | $58000 \pm 2000$ m²/m³ | Measured |
| $\tau$ | Tortuosity | $1.18 \pm 0.26$ | Measured |
| $\sigma$ | Conductivity | $275 \pm 64$ S/m | Measured |
| $i_0$ | Exchange current density | $165 \pm 32$ mA/cm² | Measured |
| $a_{\text{km}}$ | Mass transfer prefactor | $1.10 \times 10^{-3}$ | Fitted |
| $b_{\text{km}}$ | Mass transfer exponent | 0.9847 | Fitted |

### Electrolyte Properties

| Symbol | Description | Value | Source |
|--------|-------------|-------|--------|
| $D_{\text{Fe}^{2+}}$ | Fe²⁺ diffusion coefficient | $5.7 \times 10^{-10}$ m²/s | Literature |
| $D_{\text{Fe}^{3+}}$ | Fe³⁺ diffusion coefficient | $4.8 \times 10^{-10}$ m²/s | Literature |
| $C_{\text{Fe}^{2+}}$ | Fe²⁺ initial concentration | 250 mol/m³ | Estimated |
| $C_{\text{Fe}^{3+}}$ | Fe³⁺ initial concentration | 250 mol/m³ | Estimated |
| $\rho$ | Electrolyte density | 1015 kg/m³ | Literature |
| $\mu$ | Electrolyte viscosity | $1.143 \times 10^{-3}$ Pa·s | Literature |
| $\sigma$ | Electrolyte conductivity | $34.0 \pm 1.0$ S/m | Measured |
| $E^0$ | Equilibrium potential | 0.771 V | Literature |

### Mass Transfer Correlation

The mass transfer coefficients are set uniformly as a function of the superficial velocity $v_{\text{e}}$:

$$
k_{\text{m,R}} = k_{\text{m,O}} = a \cdot v_{\text{e}}^b = 1.10 \times 10^{-3} \cdot v_{\text{e}}^{0.98}
$$

### Experimental Half-Cell Overpotential

Since the modeling domain comprises only a single half-cell, experimental polarization data are corrected to represent a single half-cell overpotential:

$$
\eta_{\text{HC,exp}} = \frac{E_{\text{cable-corr}} - R_{\text{m}} A_{\text{m}} \cdot i_{\text{exp}}}{2}
$$

where $A_{\text{m}} = 2.55$ cm² is the geometric area and $R_{\text{m}}$ is the ohmic resistance of the membrane. The division by 2 reflects equal distribution of overpotential between both sides of the symmetric cell.

:::note
This assumption might not hold for sluggish or asymmetric redox couples (like in vanadium RFBs).
:::
