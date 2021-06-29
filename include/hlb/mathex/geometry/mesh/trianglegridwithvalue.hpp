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
#include "../coordinate/coordinate.hpp"
/* 代码调试与可靠性验证头文件 */
#include <cassert>



namespace HLB {
  namespace MathEx {
    namespace Geometry {
      namespace Mesh {
        /// @brief 三角形网格带格点高度值的模板.
        /// @tparam _Len 轴上长度单位.
        /// @tparam _Val 高度值单位.
        /// @details 奇数X的格点向X正向移动半个X_STEP.
        template <typename _Len, typename _Val>
        class TriangleGridWithValue {
        public:
          /// @brief 格点的坐标.
          typedef Coordinate::XY<_Len> Dot;
        public:
          /// @brief 构造函数.
          /// @param xGridSize X方向步长.
          /// @param yGridSize Y方向步长,建议值为xGridSize * MathEx::root_three() / 2.0
          /// @param iIdxXBeg X轴的起序号,闭边界.
          /// @param iIdxXEnd X轴的止序号,开边界.
          /// @param iIdxYBeg Y轴的起序号,闭边界.
          /// @param iIdxYEnd Y轴的止序号,开边界.
          TriangleGridWithValue(_Len xGridSize, _Len yGridSize, int iIdxXBeg, int iIdxXEnd, int iIdxYBeg, int iIdxYEnd)
            : X_IDX_BEG(iIdxXBeg), X_IDX_END(iIdxXEnd), Y_IDX_BEG(iIdxYBeg), Y_IDX_END(iIdxYEnd)
            , X_IDX_NUM(iIdxXEnd - iIdxXBeg), Y_IDX_NUM(iIdxYEnd - iIdxYBeg)
            , X_STEP(xGridSize), Y_STEP(yGridSize)
            , m_vBuf(X_IDX_NUM* Y_IDX_NUM) { }
        public:
          /// @brief X和Y轴的起止序号,BEG是闭边界,END是开边界.
          const int X_IDX_BEG, X_IDX_END, Y_IDX_BEG, Y_IDX_END;
          /// @brief X和Y轴的序号个数.
          const int X_IDX_NUM, Y_IDX_NUM;
          /// @brief X和Y轴方向步长.
          const _Len X_STEP, Y_STEP;
          /// @brief 填充所有值.
          /// @tparam _Func 计算格点上的值的函数原型, _Val _Func(Coordinate::XY &lt; _Len &gt; xy)
          /// @param func 计算格点上的值的函数.
          template <typename _Func>
          void FillValues(_Func&& func) {
            _Val* pVal = m_vBuf.data();
            for (int iIdxX = X_IDX_BEG; iIdxX < X_IDX_END; ++iIdxX) {
              for (int iIdxY = Y_IDX_BEG; iIdxY < Y_IDX_END; ++iIdxY) {
                *pVal++ = func(GridAtUnchecked(iIdxX, iIdxY));
              }
            }
          }
          /// @brief 获得指定格点上的坐标.
          /// @param iIdxX X轴序号.
          /// @param iIdxY Y轴序号.
          /// @return 对应的坐标点.
          Dot DotAt(int iIdxX, int iIdxY) const {
            assert(X_IDX_BEG - 1 <= iIdxX && iIdxX <= X_IDX_END + 1 && Y_IDX_BEG - 1 <= iIdxY && iIdxY <= Y_IDX_END + 1);
            return GridAtUnchecked(iIdxX, iIdxY);
          }
          /// @brief 获得指定格点上的值.
          /// @param iIdxX X轴序号.
          /// @param iIdxY Y轴序号.
          /// @return 该格点上值的引用,可赋值.
          _Val& ValueAt(int iIdxX, int iIdxY) {
            assert(X_IDX_BEG <= iIdxX && iIdxX < X_IDX_END&& Y_IDX_BEG <= iIdxY && iIdxY < Y_IDX_END);
            return m_vBuf[XYIdxLogicToArr(iIdxX, iIdxY)];
          }
          /// @brief 获得指定格点上的值.
          /// @param iIdxX X轴序号.
          /// @param iIdxY Y轴序号.
          /// @return 该格点上值的引用,常数版本不可赋值.
          const _Val& ValueAt(int iIdxX, int iIdxY) const {
            assert(X_IDX_BEG <= iIdxX && iIdxX < X_IDX_END&& Y_IDX_BEG <= iIdxY && iIdxY < Y_IDX_END);
            return m_vBuf[XYIdxLogicToArr(iIdxX, iIdxY)];
          }
          /// @brief 获得指定X轴序号的格点上的值的数组,数组以Y轴序号为下标,[Y_IDX_BEG,Y_IDX_END).
          /// @param iIdxX X轴序号.
          /// @return 指定X轴序号各格点上值的数组,长度Y_IDX_NUM,值可读写.
          _Val* XLayer(int iIdxX) {
            assert(X_IDX_BEG <= iIdxX && iIdxX < X_IDX_END);
            return m_vBuf.data() + XYIdxLogicToArr(iIdxX, 0);
          }
          /// @brief 获得指定X轴序号的格点上的值的数组,数组以Y轴序号为下标,[Y_IDX_BEG,Y_IDX_END).
          /// @param iIdxX X轴序号.
          /// @return 指定X轴序号各格点上值的数组,长度Y_IDX_NUM,常数版本只读.
          const _Val* XLayer(int iIdxX) const {
            assert(X_IDX_BEG <= iIdxX && iIdxX < X_IDX_END);
            return m_vBuf.data() + XYIdxLogicToArr(iIdxX, 0);
          }
        protected:
          /// @brief 映射X轴序号到X数组下标.
          /// @param iIdxX X轴序号,[X_IDX_BEG,X_IDX_END)
          /// @return 0起始的X数组下标.
          inline int XIdxLogicToArr(int iIdxX) const { return iIdxX - X_IDX_BEG; }
          /// @brief 映射X轴序号到Y数组下标.
          /// @param iIdxY Y轴序号,[Y_IDX_BEG,Y_IDX_END)
          /// @return 0起始的Y数组下标.
          inline int YIdxLogicToArr(int iIdxY) const { return iIdxY - Y_IDX_BEG; }
          /// @brief 映射X,Y轴序号到m_vBuf数组下标.
          /// @param iIdxX X轴序号,[X_IDX_BEG,X_IDX_END)
          /// @param iIdxY Y轴序号,[Y_IDX_BEG,Y_IDX_END)
          /// @return 0起始的Y数组下标.
          inline int XYIdxLogicToArr(int iIdxX, int iIdxY) const { return XIdxLogicToArr(iIdxX) * Y_IDX_NUM + YIdxLogicToArr(iIdxY); }
        private:
          /// @brief 值缓存.
          std::vector<_Val> m_vBuf;
          /// @brief 获得指定格点上的坐标,不检查参数有效性.
          /// @param iIdxX X轴序号.
          /// @param iIdxY Y轴序号.
          /// @return 对应的坐标点.
          inline Dot GridAtUnchecked(int iIdxX, int iIdxY) const {
            return Dot((0.0 + iIdxX) * X_STEP, ((iIdxX % 2 == 0 ? 0.0 : 0.5) + iIdxY) * Y_STEP);
          }
        };
      }
    }
  }
}
