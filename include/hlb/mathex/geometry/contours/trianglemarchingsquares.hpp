#pragma once

/* 资源头文件 */
/* 配置,设计模式 */
/* 基类,接口,函数模板的头文件 */
/* C标准库头文件 */
/* C++标准库头文件 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <vector>
/* *NIX头文件 */
/* MFC/ATL/WTL头文件 */
/* Windows头文件 */
/* 第三方库头文件 */
/* 自己的库头文件 */
/* 项目内头文件 */
#include "../mesh/trianglegridwithvalue.hpp"
/* 代码调试与可靠性验证头文件 */



namespace HLB {
  namespace MathEx {
    namespace Geometry {
      namespace Contours {
        template <typename _Len, typename _Val>
        class TriangleMarchingSquares : public Mesh::TriangleGridWithValue<_Len, _Val> {
          using BaseGrid = Mesh::TriangleGridWithValue<_Len, _Val>;
          using ThisAlgo = TriangleMarchingSquares<_Len, _Val>;
        public:
          using typename BaseGrid::Dot;
          /// @brief 表达一条等高线的曲线.
          typedef std::list<Dot> Curve;
          /// @brief 等高线绘制器.
          class IContoursDrawer {
          public:
            virtual ~IContoursDrawer() { }
          public:
            /// @brief 设置要画的线的数量.
            /// @param bIsActive 要画的是否当前正活动的线,false表示画已存档的线.
            /// @param uNum 线的数量.
            virtual void SetCurvesNum(bool bIsActive, size_t uNum) = 0;
            /// @brief 画某根线.
            /// @param bIsActive 要画的是否当前正活动的线,false表示画已存档的线.
            /// @param uCurveIdx 0起始的线的编号. 
            /// @param curve 要画的线.
            virtual void DrawCurve(bool bIsActive, size_t uCurveIdx, const Curve& curve) = 0;
          };

        public:
          /// @brief 构造函数.
          /// @tparam _Func 计算格点上的值的函数原型, _Val _Func(Coordinate::XY &lt; _Len &gt; xy)
          /// @param xGridSize X方向步长.
          /// @param yGridSize Y方向步长,建议值为xGridSize * MathEx::root_three() / 2.0
          /// @param iIdxXBeg X轴的起序号,闭边界.
          /// @param iIdxXEnd X轴的止序号,开边界.
          /// @param iIdxYBeg Y轴的起序号,闭边界.
          /// @param iIdxYEnd Y轴的止序号,开边界.
          /// @param func 计算格点上的值的函数.
          template <typename _Func>
          TriangleMarchingSquares(_Len xGridSize, _Len yGridSize, int iIdxXBeg, int iIdxXEnd, int iIdxYBeg, int iIdxYEnd, _Func&& func);
        public:
          /// @brief 初始化某等级等高线的处理.
          /// @param level 等高线高度.
          void InitLevelBuilder(_Val level);
          /// @brief 新处理一个X序号层的等高线.
          /// @return 是否还有需要添加的X序号层.
          bool AddNextLayer();
          /// @brief 存档所有线并结束按层分析.
          void FinishCurves();
          /// @brief 绘制当前等高线组.
          /// @param pDrawer 绘制器.
          /// @detail 如果还在推算,那么输出存档的线和活动的线;如果已经结束了,只输出存档的线.
          void DrawCurrentCurves(IContoursDrawer* pDrawer) const;
        private:
          /// @brief 线信息.
          struct CurveInfo {
            /// @brief 线终点基础块.
            Coordinate::XY<int> m_coordEnd;
            /// @brief 线起点基础块.
            Coordinate::XY<int> m_coordBeg;
            /// @brief 线上各点坐标.
            Curve* m_pCurve;
            /// @brief 翻转整条线.
            void Reverse();
            /// @brief 析构函数.
            ~CurveInfo() { delete m_pCurve; }
          };
          /// @brief 当前层上线段Y序号到穿过该线段的等高线的查找表.
          typedef std::map<int, CurveInfo*> ActiveCurves;
          /// @brief 当前层上线段Y序号到起点穿过该线段的等高线的查找表.
          /// @detail 起点的定义:左右均在当前层上则为左端点,只有一个在当前层上则为该点.
          ActiveCurves m_ActiveCurves;
          /// @brief 高一层上线段Y序号到起点穿过该线段的等高线的查找表.
          ActiveCurves m_ActiveCurvesNext;
          /// @brief 已归档的等高线.
          typedef std::list<Curve> ArchivedCurves;
          /// @brief 已归档的等高线.
          ArchivedCurves m_ArchivedCurves;
          /// @brief 当前等高线处理的高度.
          _Val m_Level;
          /// @brief 当前层的X序号.
          int m_iCurrentIdxX;
        private:
          /// @brief 获得指定线段上等高线穿过的点,线性内插.
          /// @param iIdxX 起点的X轴序号.
          /// @param iIdxY 起点的Y轴序号.
          /// @param iDiffX 终点的X轴序号相对于起点的差值.
          /// @param iDiffY 终点的Y轴序号相对于起点的差值.
          /// @return 线段上的点.
          /// @detail 由调用者保证有这样的点.
          /// 特殊情况是两点正好都是等高线值,根据分点算法,不应该在这样的线段上计算穿过点.
          Dot LineZeroPoint(int iIdxX, int iIdxY, int iDiffX, int iDiffY);
          /// @brief 三角形内线的走向.
          /// @detail
          ///       /\     \¯¯¯¯/\¯¯¯¯/                                           <br/>
          ///      /  \     \ M2  2M /     细分的线走向有5种,用{开始高度,结束高度}标记 <br/>
          ///     /_MM_\     \/    \/      并使用 00 表示三角形里没有线的情况.        <br/>
          ///    /\    /\     \¯MM¯/                                              <br/>
          ///   / M1  1M \     \  /                                               <br/>
          ///  /____\/____\     \/                                                <br/>
          enum class StageStep {/** 空 */ ss00, /** 向低一层穿 */ssM1, /** 低一层来 */ss1M, /** 保持 */ssMM, /** 向高一层穿 */ ssM2, /** 高一层来 */ss2M, };
          /// @brief 计算基本三角形的内部连线.
          /// @tparam _ValType 顶点01量化值类型.
          /// @param upward  是否尖向上的三角形,是则采用MM-1M-M1分析,否则采用MM-2M-M2分析.
          /// @param top     三角形尖点的01量化值.
          /// @param bottomL 三角形底边左点的01量化值.
          /// @param bottomR 三角形底边右点的01量化值.
          /// @return 基本三角形内等高线走向模式.
          template <typename _ValType>
          static StageStep TriangleToStep(bool upward, _ValType top, _ValType bottomL, _ValType bottomR);
          /// @brief 存档某线并置空.
          /// @param pCurveInfo [in,out] 要存档的线,m_pCurve存档,信息本身清空.
          void ArchiveCurve(CurveInfo*& pCurveInfo);
          /// @brief 查找线时的选项.
          enum class IndexReset {
            /// @brief 将指定IdxY调整为终止.
            PreferAsTail,
            /// @brief 将指定IdxY调整为起始.
            PreferAsHead,
            /// @brief 保持原样,不调整.
            KeepNothing,
          };
          /// @brief 查询当前活动线中经过指定位置者.
          /// @param iIdxY m_iCurrentIdxX层上要查找的Y序号.
          /// @param oper  如果找到的话,如何调整线的首尾方向.
          /// @return 如果找到线则返回线信息指针,否则返回nullptr.
          CurveInfo* QueryActiveCurve(int iIdxY, IndexReset oper);
          /// @brief 创建当前层指定Y序号开始的线.
          /// @param iIdxY Y序号.
          /// @return 包含新线的线信息.
          CurveInfo* CreateCurveInfo(int iIdxY) { return new CurveInfo{ { this->X_IDX_END, this->Y_IDX_END, }, { m_iCurrentIdxX, iIdxY, }, new Curve() }; }
          /// @brief 当前层A边上的等高线内插点.
          /// @param iIdxY 基础块的Y序号.
          /// @return 线性内插得到的等高线在A边的交点.
          Dot DotEdgeA(int iIdxY) { return LineZeroPoint(m_iCurrentIdxX + 0, iIdxY + 0, +1, +0); }
          /// @brief 当前层B边上的等高线内插点.
          /// @param iIdxY 基础块的Y序号.
          /// @return 线性内插得到的等高线在B边的交点.
          Dot DotEdgeB(int iIdxY) { return LineZeroPoint(m_iCurrentIdxX + 0, iIdxY + 0, +0, +1); }
          /// @brief 当前层C边上的等高线内插点.
          /// @param iIdxY 基础块的Y序号.
          /// @return 线性内插得到的等高线在C边的交点.
          Dot DotEdgeC(int iIdxY) { return LineZeroPoint(m_iCurrentIdxX + 0, iIdxY + 1, +1, +0); }
          /// @brief 当前层D边上的等高线内插点.
          /// @param iIdxY 基础块的Y序号.
          /// @return 线性内插得到的等高线在D边的交点.
          Dot DotEdgeD(int iIdxY) { return LineZeroPoint(m_iCurrentIdxX + 1, iIdxY + 0, +0, +1); }
          /// @brief 当前层E边上的等高线内插点(偶数层).
          /// @param iIdxY 基础块的Y序号.
          /// @return 线性内插得到的等高线在E边的交点.
          Dot DotEdgeE_Even(int iIdxY) { return LineZeroPoint(m_iCurrentIdxX, iIdxY + 1, +1, -1); }
          /// @brief 当前层E边上的等高线内插点(偶数层).
          /// @param iIdxY 基础块的Y序号.
          /// @return 线性内插得到的等高线在E边的交点.
          Dot DotEdgeE_Odd(int iIdxY) { return LineZeroPoint(m_iCurrentIdxX, iIdxY + 0, +1, +1); }
        };
      }
    }
  }
}

namespace HLB {
  namespace MathEx {
    namespace Geometry {
      namespace Contours {
        template<typename _Len, typename _Val>
        template<typename _Func>
        inline TriangleMarchingSquares<_Len, _Val>::TriangleMarchingSquares(
          _Len xGridSize, _Len yGridSize, int iIdxXBeg, int iIdxXEnd, int iIdxYBeg, int iIdxYEnd, _Func&& func)
          : BaseGrid(xGridSize, yGridSize, iIdxXBeg, iIdxXEnd, iIdxYBeg, iIdxYEnd)
        {
          this->FillValues(std::move(func));
        }

        template<typename _Len, typename _Val>
        template<typename _ValType>
        inline typename TriangleMarchingSquares<_Len, _Val>::StageStep
          TriangleMarchingSquares<_Len, _Val>::TriangleToStep(bool upward, _ValType top, _ValType bottomL, _ValType bottomR)
        {
          if (bottomL == bottomR) {
            return top == bottomL ? StageStep::ss00 : StageStep::ssMM;
          } else if (top == bottomL) {
            return upward ? StageStep::ss1M : StageStep::ss2M;
          } else {
            return upward ? StageStep::ssM1 : StageStep::ssM2;
          }
        }

        template<typename _Len, typename _Val>
        inline void TriangleMarchingSquares<_Len, _Val>::InitLevelBuilder(_Val level) {
          m_Level = level;
          m_iCurrentIdxX = this->X_IDX_BEG;
          m_ActiveCurves.clear();
          m_ActiveCurvesNext.clear();
          m_ArchivedCurves.clear();
        }

        template<typename _Len, typename _Val>
        inline bool TriangleMarchingSquares<_Len, _Val>::AddNextLayer() {
          const _Val* aLayer1 = this->XLayer(m_iCurrentIdxX);
          const _Val* aLayer2 = this->XLayer(m_iCurrentIdxX + 1);
          std::vector<int> viFlag1(this->Y_IDX_NUM + 1), viFlag2(this->Y_IDX_NUM + 1); // 这里不用vector<bool>是因为vector<bool>是压缩的,无法取到bool*达成自定义下标范围的效果.
          int* const aiFlag1 = viFlag1.data() + this->YIdxLogicToArr(0);
          int* const aiFlag2 = viFlag2.data() + this->YIdxLogicToArr(0);
          for (int iIdxY = this->Y_IDX_BEG; iIdxY < this->Y_IDX_END; ++iIdxY) { aiFlag1[iIdxY] = aLayer1[iIdxY] >= m_Level ? 1 : 0; }
          for (int iIdxY = this->Y_IDX_BEG; iIdxY < this->Y_IDX_END; ++iIdxY) { aiFlag2[iIdxY] = aLayer2[iIdxY] >= m_Level ? 1 : 0; }
          aiFlag1[this->Y_IDX_END] = aiFlag1[this->Y_IDX_END - 1];
          aiFlag2[this->Y_IDX_END] = aiFlag2[this->Y_IDX_END - 1];
          // 当前层是否偶数层.
          const bool IS_EVEN_LAYER = m_iCurrentIdxX % 2 == 0;

          //     下偶上奇                                              下奇上偶
          // -1 --- 0 --- 1 --- 2     1 -d- 2                    0 --- 1 --- 2     1 -d- 2
          //       / \   / \   /    a/ \e  / |ecdRightToLeft      \   / \   / \     \   / \c ↑
          //      /   \ /   \ /     /   \ /c ↓                     \ /   \ /   \    a\ /e  \ |ecdRightToLeft
          //     0 --- 1 --- 2     1 -b- 2                   -1 --- 0 --- 1 --- 2     1 -b- 2
          // 基础块都是左下角(x,y)右上角(x+1,y+1),区别是生成中间点使用的对角线不同.除了首个基础块,其它情况应该都是a边进入.

          // 新线的构造器.
          auto DotEdgeE = IS_EVEN_LAYER
            ? std::function<Dot(int)>([this](int iIdxY) { return DotEdgeE_Even(iIdxY); })
            : std::function<Dot(int)>([this](int iIdxY) { return DotEdgeE_Odd(iIdxY); });
          auto StageCalcDir = [&](bool bStageE, int iIdxY) {
            bool upward = bStageE != IS_EVEN_LAYER;
            const int* aiTop = upward ? aiFlag2 : aiFlag1;
            const int* aiBtm = upward ? aiFlag1 : aiFlag2;
            return TriangleToStep(upward, aiTop[iIdxY + (bStageE ? 1 : 0)], aiBtm[iIdxY], aiBtm[iIdxY + 1]);
          };
          auto UpdateIterVar = [](bool& bStageE, int& iIdxY) {
            if (bStageE) { ++iIdxY; }
            bStageE = !bStageE;
          };

          // 当前操作的线.
          CurveInfo* pCurveInfo = nullptr;
          // 当前是否正在处理e边.
          bool bStageE = false;

          // 处理开头,补齐到a边.
          if (aiFlag1[this->Y_IDX_BEG] != aiFlag2[this->Y_IDX_BEG]) {
            // 如果遇到左侧进入开头的,补出a边.
            // 开头向上是下奇上偶左侧开新边,开头向下是是下偶上奇左侧开新边.
            pCurveInfo = CreateCurveInfo(this->Y_IDX_BEG - 1);
            pCurveInfo->m_pCurve->push_back(this->DotEdgeA(this->Y_IDX_BEG));
          }

          // 遍历Y序号处理正常的边.
          for (int iIdxY = this->Y_IDX_BEG; iIdxY < this->Y_IDX_END - 1; UpdateIterVar(bStageE, iIdxY)) {
            StageStep nd = StageCalcDir(bStageE, iIdxY);
            if (pCurveInfo != nullptr) {
              // 当前在试图延续一根线.
              if (nd == StageStep::ssMM) {
                // 处理普通的右向线段.
                if (bStageE) {
                  pCurveInfo->m_pCurve->push_back(this->DotEdgeC(iIdxY));
                } else {
                  pCurveInfo->m_pCurve->push_back(DotEdgeE(iIdxY));
                }
              } else if (nd == StageStep::ssM2) {
                // 这是要高一层继续接下去的.
                // 接上线头.
                pCurveInfo->m_pCurve->push_back(this->DotEdgeD(iIdxY));
                pCurveInfo->m_coordEnd.m_X = m_iCurrentIdxX + 1;
                pCurveInfo->m_coordEnd.m_Y = iIdxY;
                // 移到输出里.
                m_ActiveCurvesNext.emplace(iIdxY, pCurveInfo);
                pCurveInfo = nullptr;
              } else if (nd == StageStep::ssM1) {
                // 这是要接低一层的.
                // 看看有没有低一层要接的.
                auto pCurveInfoLow = QueryActiveCurve(iIdxY, IndexReset::PreferAsHead);
                if (pCurveInfoLow == nullptr) {
                  // 没有要接的线头,自己结束掉.
                  pCurveInfo->m_pCurve->push_back(this->DotEdgeB(iIdxY));
                  pCurveInfo->m_coordEnd.m_X = m_iCurrentIdxX;
                  pCurveInfo->m_coordEnd.m_Y = iIdxY;
                  m_ActiveCurves.emplace(iIdxY, pCurveInfo);
                } else {
                  // 正常来说其它线走不到这个基础块了.
                  m_ActiveCurves.erase(iIdxY);
                  // 接上线头.
                  pCurveInfo->m_pCurve->splice(pCurveInfo->m_pCurve->end(), *(pCurveInfoLow->m_pCurve));
                  pCurveInfo->m_coordEnd = pCurveInfoLow->m_coordEnd;
                  // 看看新结尾在不在当前层.
                  if (pCurveInfo->m_coordEnd.m_X == m_iCurrentIdxX && pCurveInfo->m_coordEnd.m_Y < this->Y_IDX_END - 1) {
                    // 居然是复杂的来回扭动,那么要刷掉原来的记录.
                    m_ActiveCurves[pCurveInfo->m_coordEnd.m_Y] = pCurveInfo;
                  } else if (pCurveInfo->m_coordBeg.m_X == m_iCurrentIdxX && pCurveInfo->m_coordBeg.m_Y >= this->Y_IDX_BEG) {
                    // 虽然结尾不是当前层,但起点是.线建立的时候应该已经存在当前层里了,无操作即可.
                  } else if (pCurveInfo->m_coordBeg.m_X == m_iCurrentIdxX + 1) {
                    // 是高一层扑下来的线,线建立的时候应该已经存在高一层里了,无操作即可.
                  } else {
                    // 首尾都不在当前层了或高一层了,这线无主了,存档吧.
                    ArchiveCurve(pCurveInfo);
                    delete pCurveInfo;
                  }
                }
                // 不管是哪种,当前线不会接下去了.
                pCurveInfo = nullptr;
              } else {
                assert(false);
              }
            } else {
              // 当前没有已识别出来的线.
              if (nd == StageStep::ss00) {
                // 无事发生.
              } else if (nd == StageStep::ss1M) {
                // 低一层来的.
                auto pCurveInfoLow = QueryActiveCurve(iIdxY, IndexReset::PreferAsTail);
                if (pCurveInfoLow == nullptr) {
                  // 仅初始层情况,在iIdxY起点新建一条线.
                  assert(m_iCurrentIdxX == this->X_IDX_BEG);
                  pCurveInfoLow = CreateCurveInfo(iIdxY);
                  pCurveInfoLow->m_pCurve->push_back(this->DotEdgeB(iIdxY));
                  if (bStageE) {
                    pCurveInfoLow->m_pCurve->push_back(this->DotEdgeC(iIdxY));
                  } else {
                    pCurveInfoLow->m_pCurve->push_back(DotEdgeE(iIdxY));
                  }
                  m_ActiveCurves[iIdxY] = pCurveInfoLow;
                } else {
                  // 其它层应该能找到低一层对应线.iIdxY处穿通了所以不再是活动位置.
                  m_ActiveCurves.erase(iIdxY);
                }
                // 取到合适的线了,记录为活动线.
                pCurveInfo = pCurveInfoLow;
              } else if (nd == StageStep::ss2M) {
                // 高一层来的.肯定没有记录但肯定要记录.
                pCurveInfo = CreateCurveInfo(iIdxY);
                pCurveInfo->m_coordBeg.m_X += 1;
                pCurveInfo->m_pCurve->push_back(this->DotEdgeD(iIdxY));
                m_ActiveCurvesNext.emplace(iIdxY, pCurveInfo);
              } else {
                assert(false);
              }
            }
          }
          // 存在一直向右结果没有结束的情况,要结束掉.
          if (pCurveInfo != nullptr) {
            // 线头点已经在上一个基础块的c边做好了,填写位置即可.
            pCurveInfo->m_coordEnd.m_X = m_iCurrentIdxX;
            pCurveInfo->m_coordEnd.m_Y = this->Y_IDX_END;
            // 需要存档的情况仅有起点在低层,否则应该保存在某个ActiveCurves里了.
            if (pCurveInfo->m_coordBeg.m_X < m_iCurrentIdxX) {
              ArchiveCurve(pCurveInfo);
            } else {
              pCurveInfo = nullptr;
            }
          }
          // 把从当前层跨到高一层的线从当前层查找表中删掉.
          for (auto& pairNext : m_ActiveCurvesNext) {
            typename ActiveCurves::iterator it;
            if (pairNext.second->m_coordBeg.m_X == m_iCurrentIdxX) {
              it = m_ActiveCurves.find(pairNext.second->m_coordBeg.m_Y);
            } else if (pairNext.second->m_coordEnd.m_X == m_iCurrentIdxX) {
              it = m_ActiveCurves.find(pairNext.second->m_coordEnd.m_Y);
            } else {
              continue;
            }
            if (it != m_ActiveCurves.end()) {
              // 如果两层对该线具有相同的iIdxY,那么会不进这个分支.
              m_ActiveCurves.erase(it);
            }
          }
          for (auto itNext = m_ActiveCurvesNext.begin(); itNext != m_ActiveCurvesNext.end(); ) {
            if (itNext->second->m_pCurve->empty()) {
              ArchiveCurve(itNext->second);
              itNext = m_ActiveCurvesNext.erase(itNext);
            } else {
              ++itNext;
            }
          }
          // 归档没有传递到高一层的线.
          for (auto& pairBak : m_ActiveCurves) {
            ArchiveCurve(pairBak.second);
          }
          m_ActiveCurves.clear();
          // 转移状态到高一层.
          ++m_iCurrentIdxX;
          for (auto& pairNext : m_ActiveCurvesNext) {
            if (pairNext.second->m_coordBeg.m_X == m_iCurrentIdxX) {
              m_ActiveCurves[pairNext.second->m_coordBeg.m_Y] = pairNext.second;
            }
            if (pairNext.second->m_coordEnd.m_X == m_iCurrentIdxX) {
              m_ActiveCurves[pairNext.second->m_coordEnd.m_Y] = pairNext.second;
            }
          }
          m_ActiveCurvesNext.clear();
          // 是否结束
          return m_iCurrentIdxX < this->X_IDX_END - 1;
        }

        template<typename _Len, typename _Val>
        inline void TriangleMarchingSquares<_Len, _Val>::FinishCurves() {
          std::map<CurveInfo*, int> buf;
          for (auto& active : m_ActiveCurves) {
            buf[active.second] += 1;
          }
          m_ActiveCurves.clear();
          while (!buf.empty()) {
            auto pCurveInfo = buf.begin()->first;
            buf.erase(buf.begin());
            ArchiveCurve(pCurveInfo);
          }
        }

        template<typename _Len, typename _Val>
        inline void TriangleMarchingSquares<_Len, _Val>::DrawCurrentCurves(IContoursDrawer* pDrawer) const {
          if (pDrawer == nullptr) { return; }
          size_t idx;
          pDrawer->SetCurvesNum(false, m_ArchivedCurves.size());
          idx = 0;
          for (const auto& curve : m_ArchivedCurves) {
            pDrawer->DrawCurve(false, idx++, curve);
          }
          if (m_iCurrentIdxX == this->X_IDX_END - 1) { return; }
          pDrawer->SetCurvesNum(true, m_ActiveCurves.size());
          idx = 0;
          for (const auto& active : m_ActiveCurves) {
            pDrawer->DrawCurve(true, idx++, *(active.second->m_pCurve));
          }
        }

        template<typename _Len, typename _Val>
        inline typename TriangleMarchingSquares<_Len, _Val>::Dot
          TriangleMarchingSquares<_Len, _Val>::LineZeroPoint(int iIdxX, int iIdxY, int iDiffX, int iDiffY)
        {
          _Val v1 = this->ValueAt(iIdxX, iIdxY), v2 = this->ValueAt(iIdxX + iDiffX, iIdxY + iDiffY);
          Dot d1 = this->DotAt(iIdxX, iIdxY), d2 = this->DotAt(iIdxX + iDiffX, iIdxY + iDiffY);
          double coeff = 0.5;
          if (v1 != v2) {
            coeff = (v1 - m_Level) / (v1 - v2);
            if (isnan(coeff)) {
              coeff = 0.5;
            } else {
              coeff = (std::max)((std::min)(coeff, 1.0), 0.0);
            }
          }
          return coeff * d2 + (1.0 - coeff) * d1;
        }

        template<typename _Len, typename _Val>
        inline void TriangleMarchingSquares<_Len, _Val>::ArchiveCurve(CurveInfo*& pCurveInfo) {
          if (pCurveInfo->m_pCurve->size() > 0) {
            m_ArchivedCurves.push_back(std::move(*pCurveInfo->m_pCurve));
          }
          delete pCurveInfo;
          pCurveInfo = nullptr;
        }

        template<typename _Len, typename _Val>
        inline typename TriangleMarchingSquares<_Len, _Val>::CurveInfo* TriangleMarchingSquares<_Len, _Val>::QueryActiveCurve(int iIdxY, IndexReset oper) {
          // 先找有没有首尾在指定Y序号的线.
          typename ActiveCurves::iterator it = m_ActiveCurves.find(iIdxY);
          if (it == m_ActiveCurves.end()) { return nullptr; }
          CurveInfo* res = it->second;
          // 判断是否要翻转.
          bool bGoodTail = res->m_coordEnd.m_Y == iIdxY && res->m_coordEnd.m_X == m_iCurrentIdxX;
          bool bReversing = oper == IndexReset::PreferAsTail && !bGoodTail;
          bReversing |= oper == IndexReset::PreferAsHead && bGoodTail;
          if (bReversing) { res->Reverse(); }
          // 好了.
          return res;
        }

        template<typename _Len, typename _Val>
        inline void TriangleMarchingSquares<_Len, _Val>::CurveInfo::Reverse() {
          std::swap(m_coordEnd, m_coordBeg);
          if (m_pCurve != nullptr) { m_pCurve->reverse(); }
        }
      }
    }
  }
}
