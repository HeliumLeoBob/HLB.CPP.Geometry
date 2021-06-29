/// \file
/// 球面计算(球面大地主题正反算).

#pragma once

/* 资源头文件 */
/* 配置,设计模式 */
/* 基类,接口,函数模板的头文件 */
/* C标准库头文件 */
/* C++标准库头文件 */
/* *NIX头文件 */
/* MFC/ATL/WTL头文件 */
/* Windows头文件 */
/* 第三方库头文件 */
/* 自己的库头文件 */
/* 项目内头文件 */
#include "coordinate.hpp"
/* 代码调试与可靠性验证头文件 */

namespace HLB {
  namespace MathEx {
    namespace Geometry {
      namespace Coordinate {

        /// <summary>距离方位求球面坐标(大地主题正解).</summary>
        /// <param name="aLon">[in]点1经度.</param>
        /// <param name="aLat">[in]点1纬度.</param>
        /// <param name="dist">[in]球面距离(弧度).</param>
        /// <param name="azi">[in]方位(弧度表示的真北角).</param>
        /// <returns>点2的经纬度.</returns>
        template <typename _Angle>
        XY<AngleRad> sphericalPolar(
          _Angle aLon,
          _Angle aLat,
          AngleRad dist,
          _Angle azi)
        {
          _Angle bLat, difLon;
          double
            sinDist = sin(dist), cosDist = cos(dist),
            sinALat = sin(aLat), cosALat = cos(aLat),
            sinAzi = sin(azi), cosAzi = cos(azi);
          // sin(bLat) = sin(aLat) cos(dist) + cos(aLat) sin(dist) cos(azi)
          double sinBLat = sinALat * cosDist + cosALat * sinDist * cosAzi;
          if (sinBLat >= 1) {
            bLat = Angle< _Angle>::CIRCLE() / 4.0;
          } else if (sinBLat <= -1) {
            bLat = -Angle< _Angle>::CIRCLE() / 4.0;
          } else {
            bLat = asin(sinBLat);
          }
          double cosBLat = cos(bLat);
          difLon = atan2(sinDist * sinAzi, cosALat * cosDist - sinALat * sinDist * cosAzi);
          _Angle bLon = AngleToZero(aLon + difLon);
          return XY<_Angle>(bLon, bLat);
        }

        /// <summary>球面坐标求距离方位(大地主题反解).</summary>
        /// <param name="aLon">[in]点1经度.</param>
        /// <param name="aLat">[in]点1纬度.</param>
        /// <param name="bLon">[in]点2经度.</param>
        /// <param name="bLat">[in]点2纬度.</param>
        /// <returns>点1和点2的球面投影点的球面距离(弧度)和方位(弧度表示的真北角).</returns>
        template <typename _Angle>
        Polar<AngleRad, _Angle> sphericalDist(
          _Angle aLon,
          _Angle aLat,
          _Angle bLon,
          _Angle bLat)
        {
          AngleRad dist; _Angle azi;
          _Angle difLon = bLon - aLon;
          double
            sinDifLon = sin(difLon), cosDifLon = cos(difLon),
            sinALat = sin(aLat), cosALat = cos(aLat),
            sinBLat = sin(bLat), cosBLat = cos(bLat);
          // cos(dist) = sin(aLat) sin(bLat) + cos(aLat) cos(bLat) cos(difLon)
          double cosDist = sinALat * sinBLat + cosALat * cosBLat * cosDifLon;
          if (cosDist >= 1) {
            dist = 0.0 * rad;
          } else if (cosDist <= -1) {
            dist = Angle<AngleRad>::HALF();
          } else {
            dist = acos(cosDist);
          }
          double sinDist = sin(dist);
          // sin(dist) sin(azi) = sin(difLon) cos(bLat)
          // sin(dist) cos(azi) = cos(aLat) sin(bLat) - sin(aLat) cos(bLat) cos(difLon)
          azi = atan2(sinDifLon * cosBLat, cosALat * sinBLat - sinALat * cosBLat * cosDifLon);

          return Polar<AngleRad, _Angle>(dist, azi);
        }
      }
    }
  }
}
