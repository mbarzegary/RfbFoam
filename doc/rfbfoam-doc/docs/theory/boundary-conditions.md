---
sidebar_position: 5
title: Boundary Conditions
---

# Boundary Conditions

## Inlet ($\Gamma_{\text{inlet}}$)

A uniform volume-averaged velocity is set:

$$
\vec{v}_{\text{inlet}} = -\frac{Q}{A_{\text{inlet}}}\vec{n}
$$

where $\vec{n}$ is the outward unit normal vector. For IDFF, the total flow rate $Q$ is divided equally among the multiple inlet channels. Pressure is set to zero-gradient, and species concentrations are prescribed as inlet values $C_{\text{O,in}}$ and $C_{\text{R,in}}$.

## Outlet ($\Gamma_{\text{outlet}}$)

- Velocity: zero-gradient
- Pressure: uniform reference $p = 0$

The overall pressure drop $\Delta p$ is obtained from simulations by averaging the pressure over the inlet boundary:

$$
\Delta p = \frac{\int_{\Gamma_{\text{inlet}}} p \, d\Gamma}{\int_{\Gamma_{\text{inlet}}} d\Gamma}
$$

## Current Collector ($\Gamma_{\text{cc}}$)

- $\Phi_{\text{s}} = 0$ V (reference potential)
- $\Phi_{\text{l}}$: zero-gradient (no ionic current on bipolar plate)

## Membrane ($\Gamma_{\text{mem}}$)

- $\Phi_{\text{s}}$: zero-gradient (no electronic current through membrane)
- $\Phi_{\text{l}}$: depends on the operation mode (see below)

### Potentiostatic mode

A fixed, homogeneous value of $\Phi_{\text{l}}$ is set:

$$
\Phi_{\text{l,mem}} = -(U_0 + \eta_{\text{HC,sim}})
$$

The average current density is computed as:

$$
i_{\text{avg,sim}} = \frac{\int_{\Gamma_{\text{mem}}} \vec{i_{\text{l}}} \cdot \vec{n} \, d\Gamma}{\int_{\Gamma_{\text{mem}}} d\Gamma}
$$

### Galvanostatic mode

A Neumann condition enforces a homogeneous applied ionic current density:

$$
i_{\text{app,mem}} = -\kappa^{\text{eff}} \frac{\partial\Phi_{\text{l}}}{\partial\vec{n}}
$$

The half-cell overpotential is then computed from the simulated fields.

## Walls ($\Gamma_{\text{walls}}$)

- Velocity: no-slip
- Pressure: zero-gradient
- All other fields: zero-gradient

## Summary Table

| Symbol | $\Gamma_{\text{inlet}}$ | $\Gamma_{\text{outlet}}$ | $\Gamma_{\text{mem}}$ | $\Gamma_{\text{cc}}$ | $\Gamma_{\text{walls}}$ |
|--------|---------|----------|---------|---------|---------|
| $\vec{v}$ | Fixed velocity | Zero-gradient | No-slip | No-slip | No-slip |
| $p$ | Zero-gradient | $p = 0$ | Zero-gradient | Zero-gradient | Zero-gradient |
| $C_{\text{O}}$ | $C_{\text{O,in}}$ | Zero-gradient | Zero-gradient | Zero-gradient | Zero-gradient |
| $C_{\text{R}}$ | $C_{\text{R,in}}$ | Zero-gradient | Zero-gradient | Zero-gradient | Zero-gradient |
| $\Phi_{\text{s}}$ | Zero-gradient | Zero-gradient | Zero-gradient | $\Phi_{\text{s}} = 0$ | Zero-gradient |
| $\Phi_{\text{l}}$ | Zero-gradient | Zero-gradient | Fixed / Flux | Zero-gradient | Zero-gradient |

## Half-Cell Overpotential

The total simulated potential loss in the half-cell:

$$
\eta_{\text{HC,sim}} = \frac{\int_{\Gamma_{\text{mem}}}(-\Phi_{\text{l}} - U_0) \, \vec{i_{\text{l}}} \cdot \vec{n} \, d\Gamma}{\int_{\Gamma_{\text{mem}}} \vec{i_{\text{l}}} \cdot \vec{n} \, d\Gamma}
$$

In either operational mode, the pair $(\eta_{\text{HC,sim}}, \, i_{\text{avg,sim}})$ provides a data point for simulated polarization curves.
