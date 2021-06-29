/// \file
/// 三维固定坐标系.

#pragma once

/* 资源头文件 */
/* 配置,设计模式 */
/* 基类,接口,函数模板的头文件 */
/* C标准库头文件 */
#include <cmath>
/* C++标准库头文件 */
/* *NIX头文件 */
/* MFC/ATL/WTL头文件 */
/* Windows头文件 */
/* 第三方库头文件 */
/* 自己的库头文件 */
/* 项目内头文件 */
#include "../../angle.hpp"
/* 代码调试与可靠性验证头文件 */

namespace HLB {
  namespace MathEx {
    namespace Geometry {
      namespace Coordinate {

        /// <summary>D维直角坐标.</summary>
        template <class _Len, int _Dim>
        struct Coord {
          static const int D = _Dim;
          /// <summary>维度数组.</summary>
          _Len v[_Dim];
        };

        template <class _Len>
        struct Coord<_Len, 2> {
          static const int D = 2;
          /// <summary>维度数组.</summary>
          _Len v[2];

          _Len& x() { return v[0]; }
          const _Len& x() const { return v[0]; }
          _Len& y() { return v[1]; }
          const _Len& y() const { return v[1]; }
        };

        template <class _Len>
        struct Coord<_Len, 3> {
          static const int D = 3;
          /// <summary>维度数组.</summary>
          _Len v[3];

          _Len& x() { return v[0]; }
          const _Len& x() const { return v[0]; }
          _Len& y() { return v[1]; }
          const _Len& y() const { return v[1]; }
          _Len& z() { return v[2]; }
          const _Len& z() const { return v[2]; }
        };



        /// <summary>二维直角坐标.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        template <class _Len>
        struct XY {
          /// <summary>X.</summary>
          _Len x;
          /// <summary>Y.</summary>
          _Len y;
        public:
          /// <summary>构造函数.</summary>
          /// param [in]     x X.
          /// param [in]     y Y.
          XY(const _Len& x = _Len(), const _Len& y = _Len())
            : x(x), y(y)
          { }
        };

        /// <summary>归一化二维直角坐标.</summary>
        typedef XY<double> Point2D;



        /// <summary>向量求反.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]原向量.</param>
        /// <returns>反向量.</returns>
        template <class _Len>
        XY<_Len> operator-(const XY<_Len>& a) {
          return XY<_Len>(-a.x, -a.y);
        };

        /// <summary>向量加法.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// param [in]     a 被加数.
        /// <param name="b">[in]加数.</param>
        /// <returns>和.</returns>
        template <class _Len>
        XY<_Len> operator+(const XY<_Len>& a, const XY<_Len>& b) {
          return XY<_Len>(a.x + b.x, a.y + b.y);
        };

        /// <summary>向量减法.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]被减数.</param>
        /// <param name="b">[in]减数.</param>
        /// <returns>差.</returns>
        template <class _Len>
        XY<_Len> operator-(const XY<_Len>& a, const XY<_Len>& b) {
          return XY<_Len>(a.x - b.x, a.y - b.y);
        };

        /// <summary>向量左乘标量.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Scalar">标量类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]标量.</param>
        /// <param name="v">[in]向量.</param>
        /// <returns>结果.</returns>
        template <typename _Len, typename _Scalar>
        auto operator*(const _Scalar& a, const XY<_Len>& v) ->XY<decltype(a* v.x)> {
          return XY<decltype(a* v.x)>(a * v.x, a * v.y);
        };

        /// <summary>向量右乘标量.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Scalar">标量类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="v">[in]向量.</param>
        /// <param name="a">[in]标量.</param>
        /// <returns>结果.</returns>
        template <typename _Len, typename _Scalar>
        auto operator*(const XY<_Len>& v, const _Scalar& a) ->XY<decltype(a* v.x)> {
          return XY<decltype(a* v.x)>(a * v.x, a * v.y);
        };

        /// <summary>向量右除标量.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Scalar">标量类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="v">[in]向量.</param>
        /// <param name="a">[in]标量.</param>
        /// <returns>结果.</returns>
        template <typename _Len, typename _Scalar>
        auto operator/(const XY<_Len>& v, const _Scalar& a) ->XY<decltype(v.x / a)> {
          return XY<decltype(v.x / a)>(v.x / a, v.y / a);
        };

        /// <summary>向量内积.</summary>
        /// <typeparam name="_Len1">坐标值类型1(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Len2">坐标值类型2(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]向量1.</param>
        /// <param name="b">[in]向量2.</param>
        /// <returns>内积.</returns>
        template <typename _Len1, typename _Len2>
        auto innerProduct(const XY<_Len1>& a, const XY<_Len2>& b) ->decltype(a.x* b.x) {
          return a.x * b.x + a.y * b.y;
        };

        /// <summary>向量取模.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="p">[in]要取模的点.</param>
        /// <returns>模长.</returns>
        template <class _Len>
        _Len norm(const XY<_Len>& p) {
          return sqrt(innerProduct(p, p));
        };

        /// <summary>点对的欧几里得距离.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]点1.</param>
        /// <param name="b">[in]点2.</param>
        /// <returns>距离.</returns>
        template <class _Len>
        _Len dist(const XY<_Len>& a, const XY<_Len>& b) {
          return norm(a - b);
        };

        /// <summary>向量单位化.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="p">[in]要单位化的点.</param>
        /// <returns>单位化(并且去量纲)结果.</returns>
        template <class _Len>
        Point2D normalize(const XY<_Len>& p) {
          return (p / norm(p));
        };



        /// <summary>二维极坐标.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Ang">幅角类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <remarks>方向一定是左手系.</remarks>
        template <typename _Len, typename _Ang>
        struct Polar {
          /// <summary>模.</summary>
          _Len r;
          /// <summary>方向.</summary>
          _Ang th;
        public:
          Polar(const _Len& r = _Len(0), const _Ang& th = _Ang(0))
            : r(r), th(th)
          { }
          Polar(const XY<_Len>& p)
            : r(norm(p))
            , th(atan2(p.y, p.x))
          { }
          XY<_Len> ToXY() const {
            XY<_Len> p;
            p.x = this->r * cos(this->th);
            p.y = this->r * sin(this->th);
            return p;
          }
          operator XY<_Len>() const { return this->ToXY(); }
        };



        /// <summary>三维直角坐标.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        template <class _Len>
        struct XYZ {
          /// <summary>X.</summary>
          _Len x;
          /// <summary>Y.</summary>
          _Len y;
          /// <summary>Z.</summary>
          _Len z;
        public:
          /// <summary>构造函数.</summary>
          /// <param name="x">[in]X.</param>
          /// <param name="y">[in]Y.</param>
          /// <param name="z">[in]Z.</param>
          XYZ(const _Len& x = _Len(), const _Len& y = _Len(), const _Len& z = _Len())
            : x(x), y(y), z(z)
          { }
        };

        /// <summary>归一化三维直角坐标.</summary>
        typedef XYZ<double> Point3D;



        /// <summary>向量求反.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]原向量.</param>
        /// <returns>反向量.</returns>
        template <class _Len>
        XYZ<_Len> operator-(const XYZ<_Len>& a) {
          return XYZ<_Len>(-a.x, -a.y, -a.z);
        };

        /// <summary>向量加法.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]被加数.</param>
        /// <param name="b">[in]加数.</param>
        /// <returns>和.</returns>
        template <class _Len>
        XYZ<_Len> operator+(const XYZ<_Len>& a, const XYZ<_Len>& b) {
          return XYZ<_Len>(a.x + b.x, a.y + b.y, a.z + b.z);
        };

        /// <summary>向量减法.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]被减数.</param>
        /// <param name="b">[in]减数.</param>
        /// <returns>差.</returns>
        template <class _Len>
        XYZ<_Len> operator-(const XYZ<_Len>& a, const XYZ<_Len>& b) {
          return XYZ<_Len>(a.x - b.x, a.y - b.y, a.z - b.z);
        };

        /// <summary>向量左乘标量.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Scalar">标量类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]标量.</param>
        /// <param name="v">[in]向量.</param>
        /// <returns>结果.</returns>
        template <typename _Len, typename _Scalar>
        auto operator*(const _Scalar& a, const XYZ<_Len>& v) ->XYZ<decltype(a* v.x)> {
          return XYZ<decltype(a* v.x)>(a * v.x, a * v.y, a * v.z);
        };

        /// <summary>向量右乘标量.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Scalar">标量类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="v">[in]向量.</param>
        /// <param name="a">[in]标量.</param>
        /// <returns>结果.</returns>
        template <typename _Len, typename _Scalar>
        auto operator*(const XYZ<_Len>& v, const _Scalar& a) ->XYZ<decltype(a* v.x)> {
          return XYZ<decltype(a* v.x)>(a * v.x, a * v.y, a * v.z);
        };

        /// <summary>向量右除标量.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Scalar">标量类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="v">[in]向量.</param>
        /// <param name="a">[in]标量.</param>
        /// <returns>结果.</returns>
        template <typename _Len, typename _Scalar>
        auto operator/(const XYZ<_Len>& v, const _Scalar& a) ->XYZ<decltype(v.x / a)> {
          return XYZ<decltype(v.x / a)>(v.x / a, v.y / a, v.z / a);
        };

        /// <summary>向量内积.</summary>
        /// <typeparam name="_Len1">坐标值类型1(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Len2">坐标值类型2(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]向量1.</param>
        /// <param name="b">[in]向量2.</param>
        /// <returns>内积.</returns>
        template <typename _Len1, typename _Len2>
        auto innerProduct(const XYZ<_Len1>& a, const XYZ<_Len2>& b) ->decltype(a.x* b.x) {
          return a.x * b.x + a.y * b.y + a.z * b.z;
        };

        /// <summary>向量外积.</summary>
        /// <typeparam name="_Len1">坐标值类型1(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Len2">坐标值类型2(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]向量1.</param>
        /// <param name="b">[in]向量2.</param>
        /// <returns>外积.</returns>
        template <typename _Len1, typename _Len2>
        auto outerProduct(const XYZ<_Len1>& a, const XYZ<_Len2>& b) ->XYZ<decltype(a.x* b.x)> {
          return XYZ<decltype(a.x* b.x)>(
            a.y * b.z - b.y * a.z,
            a.z * b.x - b.z * a.x,
            a.x * b.y - b.x * a.y);
        };

        /// <summary>向量取模.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="p">[in]要取模的点.</param>
        /// <returns>模长.</returns>
        template <class _Len>
        _Len norm(const XYZ<_Len>& p) {
          return sqrt(innerProduct(p, p));
        };

        /// <summary>点对的欧几里得距离.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="a">[in]点1.</param>
        /// <param name="b">[in]点2.</param>
        /// <returns>距离.</returns>
        template <class _Len>
        _Len dist(const XYZ<_Len>& a, const XYZ<_Len>& b) {
          return norm(a - b);
        };

        /// <summary>向量单位化.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <param name="p">[in]要单位化的点.</param>
        /// <returns>单位化(并且去量纲)结果.</returns>
        template <class _Len>
        Point3D normalize(const XYZ<_Len>& p) {
          return (p / norm(p));
        };



        /// <summary>三维柱坐标.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Ang">幅角类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <remarks>方向一定是左手系.</remarks>
        template <typename _Len, typename _Ang>
        struct Cylindrical {
          /// <summary>模.</summary>
          _Len r;
          /// <summary>方向.</summary>
          _Ang th;
          /// <summary>高.</summary>
          _Len z;
        public:
          Cylindrical(const _Len& r = _Len(), const _Ang& th = 0, const _Len& z = _Len())
            : r(r), th(th), z(z)
          { }
          Cylindrical(const XYZ<_Len>& p)
            : r(sqrt(p.x* p.x + p.y * p.y))
            , th(atan2(p.y, p.x))
            , z(p.z)
          { }
          XYZ<_Len> ToXYZ() const {
            XYZ<_Len> p;
            p.x = this->r * cos(this->th);
            p.y = this->r * sin(this->th);
            p.z = this->z;
            return p;
          }
          operator XYZ<_Len>() const { return this->ToXYZ(); }
        };



        /// <summary>三维球坐标.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Ang">角度值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <remarks>方向一定是左手系.</remarks>
        template <typename _Len, typename _Ang>
        struct Spherical {
          /// <summary>模.</summary>
          _Len r;
          /// <summary>经度.</summary>
          _Ang th;
          /// <summary>纬度.</summary>
          _Ang phi;
        public:
          Spherical(const _Len& r = _Len(), const _Ang& th = _Ang(0), const _Ang& phi = _Ang(0))
            : r(r), th(th), phi(phi)
          { }
          Spherical(const XYZ<_Len>& p)
            : r(norm(p))
            , th(atan2(p.y, p.x))
            , phi(*&r == 0 ? 0 : asin(p.z / r))
          { }
          XYZ<_Len> ToXYZ() const {
            XYZ<_Len> p;
            p.x = this->r * cos(this->phi) * cos(this->th);
            p.y = this->r * cos(this->phi) * sin(this->th);
            p.z = this->r * sin(this->phi);
            return p;
          }
          operator XYZ<_Len>() const { return this->ToXYZ(); }
        };

        /// <summary>维度各种坐标.</summary>
        /// <typeparam name="_Dim">维度.</typeparam>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Angle">角度值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        template <int _Dim, class _Len, class _Angle = AngleRad>
        struct CoordSys {
          /// <summary>长度.</summary>
          typedef _Len Len;
          /// <summary>角度.</summary>
          typedef _Angle Angle;
        };

        /// <summary>2维各种坐标.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Angle">角度值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        template <class _Len, class _Angle>
        struct CoordSys<2, _Len, _Angle> {
          /// <summary>长度.</summary>
          typedef _Len Len;
          /// <summary>角度.</summary>
          typedef _Angle Angle;
          /// <summary>直角坐标.</summary>
          typedef XY<_Len> Coord;
          /// <summary>柱坐标.</summary>
          typedef Polar<_Len, _Angle> Cylin;
          /// <summary>球坐标.</summary>
          typedef Polar<_Len, _Angle> Spher;
        };
        /// <summary>3维各种坐标.</summary>
        /// <typeparam name="_Len">坐标值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        /// <typeparam name="_Angle">角度值类型(带或不带单位).应该是CValueWithUnitBase派生类型.代码兼容原生类型.</typeparam>
        template <class _Len, class _Angle>
        struct CoordSys<3, _Len, _Angle> {
          /// <summary>长度.</summary>
          typedef _Len Len;
          /// <summary>角度.</summary>
          typedef _Angle Angle;
          /// <summary>直角坐标.</summary>
          typedef XYZ<_Len> Coord;
          /// <summary>柱坐标.</summary>
          typedef Cylindrical<_Len, _Angle> Cylin;
          /// <summary>球坐标.</summary>
          typedef Spherical<_Len, _Angle> Spher;
        };
      }
    }
  }
}
