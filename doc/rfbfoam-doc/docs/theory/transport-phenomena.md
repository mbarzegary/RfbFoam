---
sidebar_position: 2
title: Transport Phenomena
---

# Transport Phenomena

The transport processes occurring in porous electrodes and flow channels include the transfer of momentum, species and charge. In this section, the physical equations that describe a single solid or fluid phase are presented. In the next section on [Porous Electrode Theory](porous-electrode-theory), they serve as a basis to yield an averaged description for solid-liquid systems.

## Fluid Motion

The two governing equations describing fluid motion are the continuity equation and the equation of motion. The continuity equation (conservation of mass) for an incompressible fluid in an isothermal system reduces to:

$$
\vec{\nabla} \cdot \vec{u} = 0
$$

The equation of motion (conservation of momentum) in Cartesian coordinates is:

$$
\frac{\partial}{\partial t}\rho\vec{u} + \vec{\nabla} \cdot \rho\vec{u}\vec{u} = -\vec{\nabla}p - \vec{\nabla} \cdot \vec{\vec{\tau}} + \vec{F}
$$

where $\vec{\vec{\tau}}$ is the viscous stress tensor [Pa], $p$ is the pressure [Pa], and $\vec{F}$ represents a body force per unit volume [N·m⁻³].

For incompressible flow of an isotropic, Newtonian fluid, this simplifies to the Navier-Stokes equation:

$$
\rho\left(\frac{\partial}{\partial t}\vec{u} + \vec{u} \cdot \vec{\nabla}\vec{u}\right) = -\vec{\nabla}p + \mu\vec{\nabla}^2\vec{u} + \vec{F}
$$

## Species Transport

The conservation of mass for a species $i$ dissolved in solution:

$$
\frac{\partial C_i}{\partial t} = -\vec{\nabla} \cdot \vec{N_i} + S_i
$$

where $C_i$ is the solution-phase concentration [mol·m⁻³], $\vec{N_i}$ the concentration flux [mol·m⁻²·s⁻¹], and $S_i$ is a source term [mol·m⁻³·s⁻¹].

The concentration flux of charged species is described by the Nernst-Planck equation:

$$
\vec{N_i} = -D_i\vec{\nabla}C_i - z_i m_i F C_i\vec{\nabla}\Phi_{\text{l}} + \vec{u}C_i
$$

where $D_i$ is the diffusion coefficient [m²·s⁻¹], $z_i$ the charge number, $m_i$ the ion mobility [m²·V⁻¹·s⁻¹], and $\vec{\nabla}\Phi_{\text{l}}$ the potential gradient in the liquid phase [V·m⁻¹].

This formulation is valid under the dilute-solution theory assumption, where ion-ion interactions are neglected.

## Charge Transport

Within a porous electrode, an exchange between electronic current in the solid phase and ionic current in the liquid phase takes place.

### Ionic current

The ionic current density $\vec{i_{\text{l}}}$ [A·m⁻²] arises through the net movement of charged species and is related to the flux by Faraday's law:

$$
\vec{i_{\text{l}}} = F\sum_{i} z_i\vec{N_i}
$$

Substituting the Nernst-Planck equation and assuming electroneutrality ($\sum_i z_i C_i = 0$) and negligible concentration gradients simplifies to Ohm's law:

$$
\vec{i_{\text{l}}} = -\kappa\vec{\nabla}\Phi_{\text{l}}
$$

where the liquid-phase conductivity $\kappa$ [S·m⁻¹] is defined as $\kappa \equiv F^2\sum_i z_i^2 m_i C_i$.

### Electronic current

The electronic current density in the solid phase is described by Ohm's law:

$$
\vec{i_{\text{s}}} = -\sigma \cdot \vec{\nabla}\Phi_{\text{s}}
$$

where $\sigma$ is the solid-phase conductivity [S·m⁻¹] and $\Phi_{\text{s}}$ the solid-phase potential.
