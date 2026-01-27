---
sidebar_position: 3
title: Porous Electrode Theory
---

# Porous Electrode Theory

Porous electrode theory provides a macroscopic description of porous electrodes where solid and liquid phases co-exist. The actual geometric detail of the pores is disregarded and volume-averaged forms of the conservation equations are used. Microstructural dependencies are incorporated through effective, volume-averaged transport properties.

The porosity $\varepsilon$ is defined as the quotient of the void volume to the total volume inside a representative elementary volume (REV). In the following formulations, it is assumed that the void volume is fully saturated with liquid and that all of the void volume is available for liquid to pass through.

## Fluid Motion

For solid-liquid systems, a distinction should be made between the physical fluid velocity $\vec{u}$ and the volume-averaged (superficial) velocity $\vec{v}$. They are related through the porosity:

$$
\vec{v} = \vec{u}\,\varepsilon
$$

The continuity equation in porous media:

$$
\vec{\nabla} \cdot \vec{v} = 0
$$

The flow through a porous medium is described by the Navier-Stokes equation in terms of $\vec{v}$:

$$
\frac{\rho}{\varepsilon}\left(\frac{\partial}{\partial t}\vec{v} + \frac{1}{\varepsilon}\vec{v} \cdot \vec{\nabla}\vec{v}\right) = -\vec{\nabla}p + \frac{\mu}{\varepsilon}\vec{\nabla}^2\vec{v} + \vec{F}
$$

A body force is implemented in the form of a **Darcy-Forchheimer drag** term:

$$
\vec{F} = -\frac{\mu}{K}\vec{v} - \rho\beta|\vec{v}|\vec{v}
$$

where $K$ is the permeability [m²] and $\beta$ is the Forchheimer coefficient [m⁻¹].

The Reynolds number using the square root of permeability as characteristic length scale:

$$
Re_K = \frac{\rho|\vec{v}|\sqrt{K}}{\mu}
$$

When $Re_K \ll 1$, the flow is in the creeping flow regime (Darcy flow). When $Re_K > 1$, the Forchheimer term is needed for non-Darcy flow.

In the limit of free flow ($\varepsilon \to 1$, $K \to \infty$, $\beta \to 0$), the porous Navier-Stokes equation recovers the standard form.

## Species Transport

The differential material balance in a porous domain:

$$
\frac{\partial\varepsilon C_i}{\partial t} = -\vec{\nabla} \cdot \vec{N_i} + S_i
$$

When production/consumption occurs solely through electrochemical reaction at the electrode surface:

$$
S_i = -a\frac{\nu_i}{nF}i_{\text{loc}}
$$

where $a$ is the specific interfacial area [m²·m⁻³] and $i_{\text{loc}}$ [A·m⁻²] is the local current density.

The **Butler-Volmer equation** describes the relation between activation overpotential and current density:

$$
i_{\text{loc}} = i_0 \left[\exp\left(\frac{\alpha_{\text{A}}F}{RT}\eta\right) - \exp\left(\frac{-\alpha_{\text{C}}F}{RT}\eta\right)\right]
$$

where $i_0$ is the exchange current density [A·m⁻²], $\eta$ is the overpotential [V], and $\alpha_{\text{A}}$, $\alpha_{\text{C}}$ are the anodic and cathodic charge transfer coefficients.

### Effective transport properties

The porous structure reduces the cross-sectional area available for transport and increases the path length (tortuosity). These effects are accounted for by using effective transport properties. The **Bruggeman correlation** relates effective to bulk values:

$$
D_i^{\text{eff}} = D_i \cdot \varepsilon^{1.5}
$$

The exponent of 1.5 results from a derivation in which the tortuosity scales with porosity to the power -0.5.

:::note
The Bruggeman correlation works mainly for macro-porous electrodes. In meso- and micro-porous electrodes, tortuosity values can be very large (in the order of thousands), making this correlation inapplicable.
:::

## Charge Transport

The superficial current densities use effective conductivities:

$$
\vec{i_{\text{l}}} = -\kappa^{\text{eff}}\vec{\nabla}\Phi_{\text{l}}
$$

$$
\vec{i_{\text{s}}} = -\sigma^{\text{eff}}\vec{\nabla}\Phi_{\text{s}}
$$

where $\kappa^{\text{eff}} = \kappa \cdot \varepsilon^{1.5}$ and $\sigma^{\text{eff}} = \sigma \cdot (1-\varepsilon)^{1.5}$ according to the Bruggeman correlation.

The charge balance equations for both phases:

$$
\vec{\nabla} \cdot \vec{i_{\text{l}}} = a\,i_{\text{loc}}
$$

$$
\vec{\nabla} \cdot \vec{i_{\text{s}}} = -a\,i_{\text{loc}}
$$

The source terms are of equal magnitude but opposite sign: any charge that exits the liquid phase must enter the solid phase and vice versa.
