---
sidebar_position: 4
title: Governing Equations
---

# Model Assumptions and Governing Equations

## Model Assumptions

The mathematical model was developed under the following assumptions:

1. The electrochemical half-cell operates at **steady-state** and isothermal conditions.
2. The electrolyte is modeled as an **incompressible, Newtonian** fluid under laminar flow conditions.
3. The **dilute solution approximation** is applied to the electrolyte.
4. The transport of active species occurs by **advection and diffusion**, neglecting migration.
5. The electronic and ionic current densities are both described by **Ohm's law**.
6. The redox half-reaction comprises a **single-electron transfer** step, and the formation of intermediates or side-products is ignored.

The dilute solution approximation is valid when the electrolyte consists primarily of solvent molecules. Ignoring migration is justified when considering the presence of an excess of supporting electrolyte, relative to the concentration of active species.

## Governing Equations

### Momentum transport

$$
\vec{\nabla} \cdot \vec{v} = 0
$$

$$
\frac{\rho}{\varepsilon^2}\vec{v} \cdot \vec{\nabla}\vec{v} = -\vec{\nabla}p + \frac{\mu}{\varepsilon}\vec{\nabla}^2\vec{v} - \frac{\mu}{K}\vec{v} - \rho\beta|\vec{v}|\vec{v}
$$

### Species transport

$$
\vec{\nabla} \cdot \left(\vec{v}C_{\text{O}} - D_{\text{O}}^{\text{eff}}\vec{\nabla}C_{\text{O}}\right) = a\frac{i_{\text{loc}}}{nF}
$$

$$
\vec{\nabla} \cdot \left(\vec{v}C_{\text{R}} - D_{\text{R}}^{\text{eff}}\vec{\nabla}C_{\text{R}}\right) = -a\frac{i_{\text{loc}}}{nF}
$$

### Charge transport

$$
\vec{\nabla} \cdot \left(-\kappa^{\text{eff}}\vec{\nabla}\Phi_{\text{l}}\right) = a\,i_{\text{loc}}
$$

$$
\vec{\nabla} \cdot \left(-\sigma^{\text{eff}}\vec{\nabla}\Phi_{\text{s}}\right) = -a\,i_{\text{loc}}
$$

## Electrochemical Kinetics

The local current density is described by the concentration-dependent Butler-Volmer equation:

$$
i_{\text{loc}} = i_0 \left[\frac{C_{\text{R}}^{\text{s}}}{C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) - \frac{C_{\text{O}}^{\text{s}}}{C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)\right]
$$

where $C_{\text{R}}^{\text{s}}$ and $C_{\text{O}}^{\text{s}}$ are surface concentrations, and $C_{\text{ref}}$ is the reference concentration at which $i_0$ was measured.

The local surface overpotential is defined as:

$$
\eta \equiv \Phi_{\text{s}} - \Phi_{\text{l}} - E_{\text{eq}}
$$

where $E_{\text{eq}} = E^0 + \frac{RT}{F}\ln\left(\frac{C_{\text{O}}}{C_{\text{R}}}\right)$ is given by the Nernst equation.

### Mass transfer limitation

A linear Nernst diffusion layer describes the mass transfer resistance near the electrode surface. At steady-state:

$$
k_{\text{m,R}}(C_{\text{R}} - C_{\text{R}}^{\text{s}}) = k_{\text{m,O}}(C_{\text{O}}^{\text{s}} - C_{\text{O}}) = \frac{i_{\text{loc}}}{nF}
$$

Using the mass transfer coefficient to eliminate surface concentrations yields:

$$
i_{\text{loc}} = \frac{i_0 \left[\frac{C_{\text{R}}}{C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) - \frac{C_{\text{O}}}{C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)\right]}{1 + \frac{i_0}{nFk_{\text{m,R}}C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) + \frac{i_0}{nFk_{\text{m,O}}C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)}
$$

See the [Appendix](../appendix) for the detailed derivation.

### Source term linearization

The non-linear source terms in the charge transport equations are linearized around the known potential field of the previous iteration $n$:

$$
\vec{\nabla} \cdot \left(-\sigma^{\text{eff}}\vec{\nabla}\Phi_{\text{s}}^{n+1}\right) + a\left(i_{\text{loc}}^{n} + \frac{d}{d\Phi_{\text{s}}^n}i_{\text{loc}}^{n} \cdot (\Phi_{\text{s}}^{n+1} - \Phi_{\text{s}}^{n})\right) = 0
$$

The linearization of the species transport equations was not required because of the linear dependence of $i_{\text{loc}}$ on $C_{\text{R}}$ and $C_{\text{O}}$. See the [Appendix](../appendix) for the detailed derivation of $\frac{d}{d\Phi_{\text{s}}^n}i_{\text{loc}}^{n}$.
