#pragma once

/* 资源头文件 */
/* 配置,设计模式 */
/* 基类,接口,函数模板的头文件 */
/* C标准库头文件 */
#include <cmath>
/* C++标准库头文件 */
#include <type_traits>
#include <boost/math/constants/constants.hpp>
#include <boost/units/cmath.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/angle/degrees.hpp>
/* *NIX头文件 */
/* MFC/ATL/WTL头文件 */
/* Windows头文件 */
/* 第三方库头文件 */
/* 自己的库头文件 */
/* 项目内头文件 */
/* 代码调试与可靠性验证头文件 */

namespace HLB {
  namespace MathEx {
    using AngleDeg = boost::units::quantity<boost::units::degree::plane_angle, double>;
    using deg_t = AngleDeg::unit_type;
    BOOST_UNITS_STATIC_CONSTANT(deg, deg_t);
    using AngleRad = boost::units::quantity<boost::units::si::plane_angle, double>;
    using rad_t = AngleRad::unit_type;
    BOOST_UNITS_STATIC_CONSTANT(rad, rad_t);

    template <typename _Val> struct IsAngleClassifier {
      template <int _N> struct A { char m_aForSize[_N]; };
      static A<1> f(const AngleDeg&);
      //template <typename _Val2, typename _Scale> static A<1> f(const ValScaled<Dim::plane_angle_deg, _Val2, _Scale>&);
      static A<2> f(const AngleRad&);
      //template <typename _Val2, typename _Scale> static A<2> f(const ValScaled<Dim::plane_angle_rad, _Val2, _Scale>&);
      static A<0> f(...);

      static constexpr bool IS_DEG = sizeof(f(typename std::remove_cv<_Val>::type())) == 1;
      static constexpr bool IS_RAD = sizeof(f(typename std::remove_cv<_Val>::type())) == 2;
    };
    template <typename _Ty> struct IsDegAngle { static constexpr bool value = IsAngleClassifier<_Ty>::IS_DEG; };
    template <typename _Ty> struct IsRadAngle { static constexpr bool value = IsAngleClassifier<_Ty>::IS_RAD; };
  }
  namespace MathEx {
    template <typename _Val> struct Angle {
      static constexpr double CIRCLE_VALUE = IsDegAngle<_Val>::value ? 360.0
        : IsRadAngle<_Val>::value ? boost::math::constants::two_pi<double>()
        : boost::math::constants::two_pi<double>();
      static constexpr _Val CIRCLE() { return _Val::from_value(CIRCLE_VALUE); }
      static constexpr _Val HALF() { return _Val::from_value(CIRCLE_VALUE / 2.0); }
    };
    /// @return a in [-180deg, 180deg)
    template <typename _Val> constexpr _Val AngleToZero(_Val a) {
      return _Val::from_value(a.value() - Angle<_Val>::CIRCLE_VALUE * round(a.value() / Angle<_Val>::CIRCLE_VALUE));
    }
    /// @return a in [0deg, 360deg)
    template <typename _Val> constexpr _Val AngleToNormalized(_Val a) {
      return _Val::from_value(a.value() - Angle<_Val>::CIRCLE_VALUE * floor(a.value() / Angle<_Val>::CIRCLE_VALUE));
    }
    /// @return a - b in [-180deg, 180deg)
    template <typename _Val> constexpr _Val DiffAngleToZero(_Val a, _Val b) {
      return AngleToZero(a - b);
    }
    /// @return a - b in [0deg, 360deg)
    template <typename _Val> constexpr _Val DiffAngleToPositive(_Val a, _Val b) {
      return AngleToNormalized(a - b);
    }
  }
}
