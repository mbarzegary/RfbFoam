---
sidebar_position: 6
title: Appendix
---

# Appendix: Details of the Numerical Implementation

## Elimination of Surface Concentrations

To eliminate the surface concentrations from the Butler-Volmer equation, the mass transfer relation is rewritten to express surface concentrations in terms of bulk concentrations:

$$
C_{\text{R}}^{\text{s}} = -\frac{i_{\text{loc}}}{nFk_{\text{m,R}}} + C_{\text{R}}
$$

$$
C_{\text{O}}^{\text{s}} = \frac{i_{\text{loc}}}{nFk_{\text{m,O}}} + C_{\text{O}}
$$

Substituting into the concentration-dependent Butler-Volmer equation and dividing by $\frac{i_0}{C_{\text{ref}}}$:

$$
\frac{C_{\text{ref}}}{i_0} i_{\text{loc}} = \left[\left(-\frac{i_{\text{loc}}}{nFk_{\text{m,R}}} + C_{\text{R}}\right) \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) - \left(\frac{i_{\text{loc}}}{nFk_{\text{m,O}}} + C_{\text{O}}\right) \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)\right]
$$

Collecting all terms containing $i_{\text{loc}}$ on the left-hand side:

$$
\left(\frac{C_{\text{ref}}}{i_0} + \frac{1}{nFk_{\text{m,R}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) + \frac{1}{nFk_{\text{m,O}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)\right) i_{\text{loc}} = C_{\text{R}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) - C_{\text{O}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)
$$

Isolating $i_{\text{loc}}$ yields the final expression:

$$
i_{\text{loc}} = \frac{i_0 \left[\frac{C_{\text{R}}}{C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) - \frac{C_{\text{O}}}{C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)\right]}{1 + \frac{i_0}{nFk_{\text{m,R}}C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) + \frac{i_0}{nFk_{\text{m,O}}C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)}
$$

## Source Term Linearization

The approach is shown for the $\Phi_{\text{s}}$ governing equation (the same procedure applies to $\Phi_{\text{l}}$).

The source term is linearized around the known potential field of iteration $n$ when solving for iteration $n+1$:

$$
\vec{\nabla} \cdot \left(-\sigma^{\text{eff}}\vec{\nabla}\Phi_{\text{s}}^{n+1}\right) + a\left(i_{\text{loc}}^{n} + \frac{d}{d\Phi_{\text{s}}^n}i_{\text{loc}}^{n} \cdot (\Phi_{\text{s}}^{n+1} - \Phi_{\text{s}}^{n})\right) = 0
$$

### Computing $\frac{d}{d\Phi_{\text{s}}^n}i_{\text{loc}}^{n}$

Using the quotient rule:

$$
\frac{d}{d\Phi_{\text{s}}^n}\left(\frac{f}{g}\right) = \frac{g \cdot f' - f \cdot g'}{g^2}
$$

where:

$$
f = i_0 \left[\frac{C_{\text{R}}}{C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) - \frac{C_{\text{O}}}{C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)\right]
$$

$$
f' = i_0 \frac{F}{RT} \left[\alpha_{\text{A}}\frac{C_{\text{R}}}{C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) + \alpha_{\text{C}} \frac{C_{\text{O}}}{C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)\right]
$$

$$
g = 1 + \frac{i_0}{nFk_{\text{m,R}}C_{\text{ref}}} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) + \frac{i_0}{nFk_{\text{m,O}}C_{\text{ref}}} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)
$$

$$
g' = \frac{i_0 F\alpha_{\text{A}}}{nFk_{\text{m,R}}C_{\text{ref}}RT} \exp\left(\frac{\alpha_{\text{A}} F}{RT}\eta\right) - \frac{i_0 F\alpha_{\text{C}}}{nFk_{\text{m,O}}C_{\text{ref}}RT} \exp\left(\frac{-\alpha_{\text{C}} F}{RT}\eta\right)
$$

Substituting and simplifying yields the final expression:

$$
\frac{d}{d\Phi_{\text{s}}^n}i_{\text{loc}}^{n} = \frac{\frac{F}{RT}\left(C_{\text{R}}\alpha_{\text{A}} \exp\left(\frac{\alpha_{\text{A}}F}{RT}\eta\right) + C_{\text{O}}\alpha_{\text{C}} \exp\left(\frac{-\alpha_{\text{C}}F}{RT}\eta\right)\right)}{\left(1 + \frac{i_0}{C_{\text{ref}}nF}\left(\frac{1}{k_{\text{m,R}}} \exp\left(\frac{\alpha_{\text{A}}F}{RT}\eta\right) + \frac{1}{k_{\text{m,O}}} \exp\left(\frac{-\alpha_{\text{C}}F}{RT}\eta\right)\right)\right)^2}
$$

$$
+ \frac{\frac{i_0}{C_{\text{ref}}nF}\frac{F}{RT}(\alpha_{\text{A}} + \alpha_{\text{C}})\left(\frac{C_{\text{R}}}{k_{\text{m,O}}} + \frac{C_{\text{O}}}{k_{\text{m,R}}}\right)\exp\left((\alpha_{\text{A}} - \alpha_{\text{C}})\frac{F}{RT}\Delta\Phi\right)}{\left(1 + \frac{i_0}{C_{\text{ref}}nF}\left(\frac{1}{k_{\text{m,R}}} \exp\left(\frac{\alpha_{\text{A}}F}{RT}\eta\right) + \frac{1}{k_{\text{m,O}}} \exp\left(\frac{-\alpha_{\text{C}}F}{RT}\eta\right)\right)\right)^2}
$$
