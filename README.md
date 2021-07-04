# HLB.CPP.Geometry

## 介绍
C++几何库.

## 软件架构
需要boost.math和boost.units


## 安装教程

纯头文件

## 使用说明
基本用法：
```cpp
#include <hlb/mathex/geometry/contours/trianglemarchingsquares.hpp>

using TMS = HLB::MathEx::Geometry::Contours::TriangleMarchingSquares<_XYLengthType, _ZLengthType>;
// func: _ZLengthType(HLB::MathEx::Geometry::Coordinate::XY<_XYLengthType>)
// calculate the value at (xIdx*xGridSize, yIdx*yGridSize)
TMS contoursObj(xGridSize, yGridSize, iIdxXBeg, iIdxXEnd, iIdxYBeg, iIdxYEnd, func);

//实现一个 TMS::IContoursDrawer 用于接收等高线.
TMS::IContoursDrawer* pDrawer;

contoursObj.InitLevelBuilder(oneZValueWantedToBuildContoursCurve);
while (contoursObj.AddNextLayer()) { }
contoursObj.FinishCurves();
// output one curve
contoursObj.DrawCurrentCurves(pDrawer);
```

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


## 库说明

主库在 [Gitee](https://gitee.com/heliumleobob/cpp-geometry.git)
[GitHub](https://github.com/HeliumLeoBob/HLB.CPP.Geometry.git) 上是镜像。
