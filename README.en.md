# HLB.CPP.Geometry

## Description
A C++ lib of geometry.

## Software Architecture
prerequest: boost.math , boost.units

## Installation

header-only

## Instructions
Basic usage:
```cpp
#include <hlb/mathex/geometry/contours/trianglemarchingsquares.hpp>

using TMS = HLB::MathEx::Geometry::Contours::TriangleMarchingSquares<_XYLengthType, _ZLengthType>;
// func: _ZLengthType(HLB::MathEx::Geometry::Coordinate::XY<_XYLengthType>)
// calculate the value at (xIdx*xGridSize, yIdx*yGridSize)
TMS contoursObj(xGridSize, yGridSize, iIdxXBeg, iIdxXEnd, iIdxYBeg, iIdxYEnd, func);

//implement TMS::IContoursDrawer to get one contours curve.
TMS::IContoursDrawer* pDrawer;

contoursObj.InitLevelBuilder(oneZValueWantedToBuildContoursCurve);
while (contoursObj.AddNextLayer()) { }
contoursObj.FinishCurves();
// output one curve
contoursObj.DrawCurrentCurves(pDrawer);
```

## Contribution

1.  Fork the repository
2.  Create Feat_xxx branch
3.  Commit your code
4.  Create Pull Request


## Description of lib

Main repository at [Gitee](https://gitee.com/heliumleobob/cpp-geometry.git)
Mirror at [GitHub](https://github.com/HeliumLeoBob/HLB.CPP.Geometry.git) 
