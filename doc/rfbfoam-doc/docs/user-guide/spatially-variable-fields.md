---
sidebar_position: 3
title: Spatially-Variable Fields
---

# Spatially-Variable Fields

RfbFoam supports spatially-variable material property fields, enabling realistic electrode configurations such as porosity gradients. This feature uses OpenFOAM's `setExprFields` utility with expressions defined in `system/setExprFieldsDict`.

## Overview

In real electrodes, material properties like porosity often vary spatially due to manufacturing processes or intentional design choices. RfbFoam allows you to define these variations using mathematical expressions that are evaluated at each cell center during case preparation.

### Workflow Integration

The `setExprFields` utility runs during case preparation:

```bash
blockMesh                    # Generate base mesh
surfaceFeatureExtract        # Extract surface features
snappyHexMesh -overwrite     # Refine mesh
setExprFields                # Apply spatial expressions to fields
```

This modifies the field files in the `0/` folder based on expressions defined in `system/setExprFieldsDict`.

### Supported Material Property Fields

The following fields can be configured with spatial expressions:

| Field | Description | Dimensions | Typical Values |
|-------|-------------|------------|----------------|
| `porosity` | Electrode porosity $\varepsilon$ | [0 0 0 0 0 0 0] | 0.7 - 0.95 |
| `invPermeability` | Inverse permeability $K^{-1}$ | [0 -2 0 0 0 0 0] | $10^9$ - $10^{11}$ m$^{-2}$ |
| `kappa1` | Solid-phase conductivity $\kappa_s$ | [-1 -3 3 0 0 2 0] | 100 - 500 S/m |
| `areaPerVol` | Specific surface area $a$ | [0 -1 0 0 0 0 0] | $10^4$ - $10^5$ m$^{-1}$ |
| `tau` | Tortuosity factor $\tau$ | [0 0 0 0 0 0 0] | 1.0 - 3.0 |

:::tip
The provided example cases (`examples/cases/FTFF_Model` and `examples/cases/IDFF_Model`) contain complete `setExprFieldsDict` files with ready-to-edit settings for all material property fields. These serve as practical starting points for your own configurations.
:::

## Dictionary Structure

The `setExprFieldsDict` file has two main sections:

```cpp
// Common parameters shared across all expressions
commonSettings
{
    point1   (x1 y1 z1);    // First corner of electrode region
    point2   (x2 y2 z2);    // Opposite corner
};

// List of field expressions
expressions
(
    fieldName1 { ... }
    fieldName2 { ... }
);
```

### Field Expression Block

Each field expression contains five key components:

```cpp
porosity
{
    field       porosity;              // Field name to modify
    dimensions  [0 0 0 0 0 0 0];       // OpenFOAM dimensional units

    constants
    {
        point1   $commonSettings.point1;
        point2   $commonSettings.point2;
        eps0     0.75;                 // Value at one end
        eps1     0.90;                 // Value at other end
    }

    variables
    (
        // Compute bounds from corner points
        "y_min = min($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
        "y_max = max($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
    );

    fieldMask
    #{
        // Boolean: where to apply the expression
        pos().x() >= x_min && pos().x() <= x_max
    #};

    expression
    #{
        // Mathematical formula for field value
        (($[constants.eps1] - $[constants.eps0]) * (pos().y() - y_min) / (y_max - y_min))
        + $[constants.eps0]
    #};
}
```

## Configuration Examples

### Uniform Field (No Gradient)

For a uniform field value throughout the electrode, set both limit values equal:

```cpp
porosity
{
    field       porosity;
    dimensions  [0 0 0 0 0 0 0];

    constants
    {
        point1   $commonSettings.point1;
        point2   $commonSettings.point2;
        eps0     0.877;    // Lower limit
        eps1     0.877;    // Upper limit (same = uniform)
    }

    variables
    (
        "x_min = min($[(vector)constants.point1].x(), $[(vector)constants.point2].x())"
        "x_max = max($[(vector)constants.point1].x(), $[(vector)constants.point2].x())"
        "y_min = min($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
        "y_max = max($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
        "z_min = min($[(vector)constants.point1].z(), $[(vector)constants.point2].z())"
        "z_max = max($[(vector)constants.point1].z(), $[(vector)constants.point2].z())"
    );

    fieldMask
    #{
        pos().x() >= x_min && pos().x() <= x_max &&
        pos().y() >= y_min && pos().y() <= y_max &&
        pos().z() >= z_min && pos().z() <= z_max
    #};

    expression
    #{
        $[constants.eps0]
    #};
}
```

:::tip
When `eps0 = eps1`, you can simplify the expression to just `$[constants.eps0]` instead of using the linear interpolation formula.
:::

### Linear Gradient (Through-Plane)

For a linear porosity gradient from membrane to current collector (y-direction):

```cpp
porosity
{
    field       porosity;
    dimensions  [0 0 0 0 0 0 0];

    constants
    {
        point1   $commonSettings.point1;
        point2   $commonSettings.point2;
        eps0     0.75;     // Porosity at y_min (membrane side)
        eps1     0.90;     // Porosity at y_max (current collector side)
    }

    variables
    (
        "x_min = min($[(vector)constants.point1].x(), $[(vector)constants.point2].x())"
        "x_max = max($[(vector)constants.point1].x(), $[(vector)constants.point2].x())"
        "y_min = min($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
        "y_max = max($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
        "z_min = min($[(vector)constants.point1].z(), $[(vector)constants.point2].z())"
        "z_max = max($[(vector)constants.point1].z(), $[(vector)constants.point2].z())"
    );

    fieldMask
    #{
        pos().x() >= x_min && pos().x() <= x_max &&
        pos().y() >= y_min && pos().y() <= y_max &&
        pos().z() >= z_min && pos().z() <= z_max
    #};

    expression
    #{
        (($[constants.eps1] - $[constants.eps0]) * (pos().y() - y_min) / (y_max - y_min))
        + $[constants.eps0]
    #};
}
```

This creates a linear interpolation:
- At `y = y_min`: field value = `eps0`
- At `y = y_max`: field value = `eps1`

## Important Considerations

### Electrode Region Definition

The `point1` and `point2` coordinates define the rectangular region where expressions are applied. These should match your electrode geometry:

```cpp
commonSettings
{
    // For FTFF geometry example:
    point1   (0 0 -0.017);           // Corner at membrane
    point2   (0.015 0.00044 0);      // Opposite corner at outlet
};
```

:::warning
Ensure the corner points accurately represent your electrode boundaries. Cells outside this region will retain their default values from the `0.orig/` field files.
:::

### Flow Channel Handling

The `fieldMask` ensures expressions only apply within the electrode region. Cells in flow channels (outside the mask) retain their default values:
- `porosity = 1` (or 0.99 for numerical stability)
- `invPermeability = 0` (free flow)
- `areaPerVol = 0` (no reactions)

### Boundary Conditions

The `setExprFields` utility only modifies the `internalField`. Boundary conditions remain as defined in the `0.orig/` field files. All material property fields typically use `zeroGradient` boundaries:

```cpp
boundaryField
{
    inlet       { type zeroGradient; }
    outlet      { type zeroGradient; }
    walls       { type zeroGradient; }
    currColl    { type zeroGradient; }
    mem         { type zeroGradient; }
}
```

### Physical Consistency

When defining property gradients, ensure physically consistent relationships:

1. **Porosity and permeability**: Higher porosity generally correlates with higher permeability (lower `invPermeability`)

2. **Porosity and specific surface area**: These may have complex relationships depending on electrode microstructure

3. **Porosity and tortuosity**: Higher porosity typically means lower tortuosity

4. **Conductivity**: Solid-phase conductivity may decrease with higher porosity due to reduced solid fraction

:::tip
Consider using correlations like Kozeny-Carman for permeability or Bruggeman for tortuosity if you haven't measured these parameters experimentally.
:::

## Complete Example

Here is a complete `setExprFieldsDict` for an electrode with a through-plane porosity gradient:

```cpp
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    object      setExprFieldsDict;
}

commonSettings
{
    point1   (0 0 -0.017);
    point2   (0.015 0.00044 0);
};

expressions
(
    porosity
    {
        field       porosity;
        dimensions  [0 0 0 0 0 0 0];

        constants
        {
            point1   $commonSettings.point1;
            point2   $commonSettings.point2;
            eps0     0.75;
            eps1     0.90;
        }

        variables
        (
            "x_min = min($[(vector)constants.point1].x(), $[(vector)constants.point2].x())"
            "x_max = max($[(vector)constants.point1].x(), $[(vector)constants.point2].x())"
            "y_min = min($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
            "y_max = max($[(vector)constants.point1].y(), $[(vector)constants.point2].y())"
            "z_min = min($[(vector)constants.point1].z(), $[(vector)constants.point2].z())"
            "z_max = max($[(vector)constants.point1].z(), $[(vector)constants.point2].z())"
        );

        fieldMask
        #{
            pos().x() >= x_min && pos().x() <= x_max &&
            pos().y() >= y_min && pos().y() <= y_max &&
            pos().z() >= z_min && pos().z() <= z_max
        #};

        expression
        #{
            (($[constants.eps1] - $[constants.eps0]) * (pos().y() - y_min) / (y_max - y_min))
            + $[constants.eps0]
        #};
    }

    // Add similar blocks for invPermeability, kappa1, areaPerVol, tau
    // with appropriate correlations
);
```

## Verification

After running `setExprFields`, verify your field initialization using ParaView:

1. Open the case in ParaView
2. Load the material property fields (porosity, etc.)
3. Use the "Slice" filter to visualize cross-sections
4. Confirm the gradient direction and values match your configuration

You can also use OpenFOAM's `postProcess` utility to compute field statistics:

```bash
postProcess -func "fieldMinMax(porosity)"
```
