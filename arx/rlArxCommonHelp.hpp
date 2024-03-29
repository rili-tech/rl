#pragma once
#include "rl_defined_types.h"
#include "brgbl.h"
#include "brbrep.h"
#include "..\..\..\utils\brep\inc\brbvtrav.h"
#include "..\..\..\utils\brep\inc\breltrav.h"
#include "..\..\..\utils\brep\inc\brvetrav.h"
#include "..\..\..\utils\brep\inc\brvltrav.h"
#include "..\..\..\utils\brep\inc\brloop.h"
#include "..\..\..\utils\brep\inc\bredge.h"
#include "..\..\..\utils\brep\inc\brbftrav.h"
#include "..\..\..\utils\brep\inc\brface.h"
#include "..\..\..\utils\brep\inc\brbctrav.h"
#include "..\..\..\utils\brep\inc\brcstrav.h"
#include "..\..\..\utils\brep\inc\brshell.h"
#include "..\..\..\utils\brep\inc\brsftrav.h"
#include "..\..\..\utils\brep\inc\brfltrav.h"
#include "..\..\..\utils\brep\inc\brletrav.h"
#include "..\..\..\utils\brep\inc\brlvtrav.h"
#include "brvtx.h"
#pragma comment(lib,"acbr19.lib")

// #include <boost/polygon/polygon.hpp>
// 
// typedef boost::polygon::polygon_data<double> B_Polygon;
// typedef boost::polygon::polygon_traits<B_Polygon>::point_type B_Point;
/**
 * @note - 基本公共通用函数
 */

// bool rlArxCommonHelp::_polygon_is_counterclockwise(const AcGePoint3dArray &vertices)
// {
// 	B_Point *pts = new B_Point[vertices.length()];
// 	for (int i = 0; i < vertices.length(); i++)
// 		pts[i] = boost::polygon::construct<B_Point>(vertices[i].x, vertices[i].y);
// 
// 	B_Polygon polygon;
// 	boost::polygon::set_points(polygon, pts, pts + vertices.length());
// 	boost::polygon::direction_1d ret = boost::polygon::winding(polygon);
// 	if (pts)
// 	{
// 		delete[]pts;
// 		pts = NULL;
// 	}
// 	if (ret == boost::polygon::COUNTERCLOCKWISE)
// 		return true;
// 	return false;
// }

//面数据
struct RegionPtData
{
	RegionPtData()
	{
		m_ptRangeArry.removeAll();
		m_ptHolePtArry.removeAll();
	}

	AcGePoint3dArray m_ptRangeArry;
	AcArray<AcGePoint3dArray> m_ptHolePtArry;
};

namespace rlArxCommonHelp
{
	class  IcDocLock
	{
	protected:
		AcApDocument *m_pDoc;
		Acad::ErrorStatus m_esLock;
	public:
		IcDocLock(AcApDocument *pDoc = NULL, bool bPrompt = false)
		{
			if (pDoc != NULL)
				m_pDoc = pDoc;
			else
				m_pDoc = curDoc();
			if (m_pDoc != NULL && acDocManager != NULL)
				m_esLock = acDocManager->lockDocument(m_pDoc, AcAp::kWrite, NULL, NULL, bPrompt);
		}
		~IcDocLock()
		{
			if (m_pDoc != NULL && m_esLock == Acad::eOk && acDocManager != NULL)
			{
				acDocManager->unlockDocument(m_pDoc);
			}
		}

		Acad::ErrorStatus Status()const
		{
			return m_esLock;
		}
	};

	class  IcGtol
	{
	protected:
		AcGeTol m_tol;
	public:
		IcGtol(double dEqualPt, double dEqualVec)
		{
			m_tol.setEqualPoint(AcGeContext::gTol.equalPoint());
			m_tol.setEqualVector(AcGeContext::gTol.equalVector());
			AcGeContext::gTol.setEqualPoint(dEqualPt);
			AcGeContext::gTol.setEqualVector(dEqualVec);
		}

		IcGtol(AcGeTol tol)
		{
			m_tol.setEqualPoint(AcGeContext::gTol.equalPoint());
			m_tol.setEqualVector(AcGeContext::gTol.equalVector());
			AcGeContext::gTol.setEqualPoint(tol.equalPoint());
			AcGeContext::gTol.setEqualVector(tol.equalVector());
		}
		~IcGtol()
		{
			AcGeContext::gTol.setEqualPoint(m_tol.equalPoint());
			AcGeContext::gTol.setEqualVector(m_tol.equalVector());
		}
	};

	class  IcUCSMatManager
	{
	private:
		AcGeMatrix3d m_Oldmat;
		AcGeMatrix3d m_Newmat;
	public:
		IcUCSMatManager(const AcGeMatrix3d& mat)
		{
			acedGetCurrentUCS(m_Oldmat);
			acedSetCurrentUCS(mat);
			m_Newmat = mat;
		}

		~IcUCSMatManager()
		{
			acedSetCurrentUCS(m_Oldmat);
		}
		const AcGeMatrix3d& getNewUcsMat() { return m_Newmat; }
	};

	class  IcWorkDataBase
	{
	protected:
		AcDbDatabase *m_pOldPdb;
	public:
		IcWorkDataBase(AcDbDatabase *pDb = NULL, bool bPrompt = false)
		{
			m_pOldPdb = acdbCurDwg();
			acdbHostApplicationServices()->setWorkingDatabase(pDb);
		}
		~IcWorkDataBase()
		{
			acdbHostApplicationServices()->setWorkingDatabase(m_pOldPdb);
		}
	};

	static bool floatIsEqual(double d1, double d2)
	{
		if (fabs(d1 - d2) < EPRES)
			return TRUE;
		return FALSE;
	}

	//截取字符串
	static BOOL splitStr(AcArray<std::wstring>& strArry, const std::wstring& strRes, const std::wstring& strSplit)
	{
		strArry.removeAll();
		if (strRes.empty())
			return FALSE;
		if (strSplit.empty())
		{
			strArry.append(strRes);
			return TRUE;
		}
		int nIndexTmp = 0;
		int nIndex = (int)strRes.find(strSplit, nIndexTmp);
		if (nIndex < 0)
		{
			strArry.append(strRes);
			return TRUE;
		}
		std::wstring str1;
		while (nIndex >= 0)
		{
			str1 = strRes.substr(nIndexTmp, nIndex - nIndexTmp);
			if (!str1.empty())
				strArry.append(str1);
			nIndexTmp = nIndex + (int)strSplit.length();
			nIndex = (int)strRes.find(strSplit, nIndexTmp);
		}
		str1 = strRes.substr(nIndexTmp, (int)strRes.length() - nIndexTmp);
		if (!str1.empty())
			strArry.append(str1);
		return !strArry.isEmpty();
	}

	static std::string getStringFromWString(std::wstring str)
	{
#ifdef UNICODE
		std::string pOut;
		const wchar_t* wszString = str.c_str();
		int u8Len = ::WideCharToMultiByte(CP_ACP, NULL, wszString, (int)wcslen(wszString), NULL, 0, NULL, NULL);

		char* szU8 = new char[u8Len + 1];
		::WideCharToMultiByte(CP_ACP, NULL, wszString, (int)wcslen(wszString), szU8, u8Len, NULL, NULL);
		szU8[u8Len] = '\0';
		pOut = szU8;

		delete[] szU8;
		szU8 = NULL;
		return pOut;
#else
		return str;
#endif
	}

	static std::wstring getWStringFromString(std::string str)
	{
#ifdef UNICODE
		int iBufferSize = (int)str.size() + 1;
		wchar_t *szBuffer = new wchar_t[iBufferSize];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, szBuffer, iBufferSize);
		std::wstring sRet = szBuffer;
		delete[] szBuffer;
		return sRet;
#else
		return str;
#endif
	}

	static void replace(std::wstring& strDim,const std::string& str1,const std::string& str2)
	{
		std::string strText = rlArxCommonHelp::getStringFromWString(strDim);
		if (strText.empty())
			return;

		int length = (int)strText.size();
		int i = 0, j = length - 1;

		while (i < length && isspace(strText[i]))
		{
			i++;
		}

		while (j >= 0 && isspace(strText[j]))
		{
			j--;
		}

		if (j < i)
			strText.clear();
		else
			strText = strText.substr(i, j - i + 1);
		int iNdex = (int)strText.find(str1);
		while (iNdex >= 0)
		{
			strText.replace(iNdex, 2, str2);
			iNdex = (int)strText.find(str1);
		}
		strDim = rlArxCommonHelp::getWStringFromString(strText);
	}

	static AcGeTol getPrjGeTol()
	{
		AcGeTol tol;
		tol.setEqualPoint(rlTolerance::equal_point());
		tol.setEqualVector(rlTolerance::equal_vector());
		return tol;
	}

	//处理向量准确度 ,0.000001的归零
	static void vectorNormal(AcGeVector3d& vec, double dEqualVector = rlTolerance::equal_vector())
	{
		vec.normalize();
		if (fabs(vec.x) < dEqualVector)
			vec.x = 0;
		if (fabs(vec.y) < dEqualVector)
			vec.y = 0;
		if (fabs(vec.z) < dEqualVector)
			vec.z = 0;
		vec.normalize();
	}

	//获取允许角度范围内的向量
	static BOOL getVecNormalInAngle(AcGeVector3d& vecNormal, const AcGeVector3d& vecOffset, double dAngle)
	{
		AcGeVector3d vecRes = vecOffset.normal();

		double dAngleRes = vecOffset.angleTo(vecNormal);
		if (fabs(dAngleRes) < dAngle *rlPi / 180)
		{
			return TRUE;
		}
		else if (fabs(dAngleRes - rlPi) < dAngle* rlPi / 180)
		{
			vecNormal = -vecNormal.normal();
			return TRUE;
		}
		vectorNormal(vecNormal);
		return FALSE;
	}

	static void DelStringZero(std::wstring& str)
	{
		int nL = (int)str.find(_T("."));
		if (nL > 0)
		{
			std::wstring strTmp1 = str.substr(0, nL);
			std::wstring strTmp2 = str.substr(nL + 1, (int)str.length() - nL - 1);
			int nLenght = (int)strTmp2.length();
			while (strTmp2.at(nLenght - 1) == '0')
			{
				strTmp2 = strTmp2.substr(0, nLenght - 1);
				nLenght = (int)strTmp2.length();
				if (nLenght == 0)
					break;
			}
			if (!strTmp2.empty())
			{
				str = strTmp1;
				str += _T(".");
				str += strTmp2;
			}
			else
				str = strTmp1;
		}
	}

	static AcGePoint3d middle(const AcGePoint3d &pnt1, const AcGePoint3d &pnt2)
	{
		return AcGePoint3d((pnt1.x + pnt2.x) / 2.0, (pnt1.y + pnt2.y) / 2.0, (pnt1.z + pnt2.z) / 2.0);
	}

	//旋转点组
	static void transPtsFromMatrix3d(AcGePoint3dArray& ptArtry, const AcGeMatrix3d& mat)
	{
		int nLen = ptArtry.length();
		for (int i = 0; i < nLen; i++)
			ptArtry[i].transformBy(mat);
	}

	//获取两条向量的夹角
	static double GetAngleBetweenTwoVec(AcGeVector3d vec1, AcGeVector3d vec2, int flag)
	{
		return vec1.angleTo(vec2, AcGeVector3d(0, 0, flag));
	}

	//获取两条向量的夹角
	static double GetAngleBetweenTwoVec(AcGeVector3d vec1, AcGeVector3d vec2)
	{
		return vec1.angleTo(vec2);
	}

	static void releaseVoidPtr(AcGeVoidPointerArray& pEnts)
	{
		int nLen = pEnts.length();
		for (int i = 0; i < nLen; i++)
		{
			AcDbObject *pObj = static_cast<AcDbObject*>(pEnts[i]);
			if (pObj)
			{
				delete pObj;
				pObj = NULL;
			}
		}
		pEnts.removeAll();
	}

	//获取面
	static AcDbRegion* getRegionFromPtArry(const AcGePoint3dArray& ptArry)
	{
		if (ptArry.length() < 3)
			return NULL;

		AcDbVoidPtrArray arCurves;
		AcDb3dPolyline * pPolyLine = new AcDb3dPolyline(AcDb::k3dSimplePoly, AcGePoint3dArray(ptArry), Adesk::kTrue);
		pPolyLine->setClosed(true);
		arCurves.append(pPolyLine);

		AcDbVoidPtrArray arRegions;
		AcDbRegion::createFromCurves(arCurves, arRegions);
		if (arRegions.length() == 0)
		{
			releaseVoidPtr(arCurves);
			releaseVoidPtr(arRegions);
			return NULL;
		}
		AcDbRegion * pReg = (AcDbRegion*)arRegions.at(0);
		AcDbRegion* pCloneReg = AcDbRegion::cast(pReg->clone());

		releaseVoidPtr(arCurves);
		releaseVoidPtr(arRegions);

		return pCloneReg;
	}

	//获取点集的面积 三个点以上
	static double getRegionArea(const AcGePoint3dArray& ptArry)
	{
		AcDbRegion * pReg = getRegionFromPtArry(ptArry);
		if (pReg == NULL)
			return 0.0;

		double dAraer;
		pReg->getArea(dAraer);
		SAFE_DELETE(pReg);
		return dAraer;
	}

	//获取点数据
	static BOOL getRegionPtArry(AcArray<RegionPtData>& ptFaceArrys, AcDbRegion* pRegin)
	{
		if (pRegin == NULL)
			return FALSE;
		AcBrBrep brepEntity;
		brepEntity.set(*pRegin);

		AcBrBrepComplexTraverser brepComplexTrav;
		AcBr::ErrorStatus returnValue = brepComplexTrav.setBrep(brepEntity);
		if (returnValue != AcBr::eOk)
			return FALSE;

		while (!brepComplexTrav.done() && (returnValue == AcBr::eOk))
		{
			AcBrComplexShellTraverser complexShellTrav;
			returnValue = complexShellTrav.setComplex(brepComplexTrav);
			if (returnValue != AcBr::eOk)
				return FALSE;

			while (!complexShellTrav.done() && (returnValue == AcBr::eOk))
			{
				AcBrShellFaceTraverser shellFaceTrav;
				returnValue = shellFaceTrav.setShell(complexShellTrav);
				if (returnValue != AcBr::eOk)
					return FALSE;

				while (!shellFaceTrav.done() && (returnValue == AcBr::eOk))
				{
					RegionPtData regionData;
					AcBrFace currentFace;
					returnValue = shellFaceTrav.getFace(currentFace);
					if (returnValue != AcBr::eOk)
						return FALSE;

					if (currentFace.isEqualTo(&brepEntity))
						return FALSE;

					AcBrFaceLoopTraverser faceLoopTrav;
					returnValue = faceLoopTrav.setFace(shellFaceTrav);
					if (returnValue != AcBr::eOk)
					{
						if (returnValue != AcBr::eDegenerateTopology)
							return FALSE;
						else
							returnValue = AcBr::eOk;
					}
					else while (!faceLoopTrav.done() && (returnValue == AcBr::eOk))
					{
						AcBrLoop loop;
						returnValue = faceLoopTrav.getLoop(loop);
						if (returnValue != AcBr::eOk)
							return FALSE;
						
						AcBr::LoopType ltype;
						loop.getType(ltype);

						AcGePoint3dArray ptArry;
						AcBrLoopVertexTraverser loopVtxTrav;
						returnValue = loopVtxTrav.setLoop(faceLoopTrav);
						if (returnValue != AcBr::eOk)
							return FALSE;
						else while (!loopVtxTrav.done() && (returnValue == AcBr::eOk))
						{
							AcBrVertex loopPoint;
							returnValue = loopVtxTrav.getVertex(loopPoint);
							if (returnValue != AcBr::eOk)
								return FALSE;
							AcGePoint3d vertexPoint;
							returnValue = loopPoint.getPoint(vertexPoint);
							if (returnValue != AcBr::eOk)
								return FALSE;
							ptArry.append(vertexPoint);
							returnValue = loopVtxTrav.next();
							if (returnValue != AcBr::eOk)
								return FALSE;
						}
						if (ptArry.length() > 3)
						{
							if (ltype == AcBr::kInterior)
							{
								//减掉
								regionData.m_ptHolePtArry.append(ptArry);
							}
							else if (ltype == AcBr::kLoopExterior)
							{
								//增加
								regionData.m_ptRangeArry = ptArry;
							}
						}
						returnValue = faceLoopTrav.next();
						if (returnValue != AcBr::eOk)
							return FALSE;
					}
					if (!regionData.m_ptRangeArry.isEmpty())
						ptFaceArrys.append(regionData);

					returnValue = shellFaceTrav.next();
					if (returnValue != AcBr::eOk)
						return FALSE;
				}

				returnValue = complexShellTrav.next();
				if (returnValue != AcBr::eOk)
					return FALSE;
			}

			returnValue = brepComplexTrav.next();
			if (returnValue != AcBr::eOk)
				return FALSE;
		}
		return !ptFaceArrys.isEmpty();
	}

	static void mergeOnLines(AcArray<AcGeLineSeg3d>& ptOnZarry, const AcGeTol& tol = getPrjGeTol())
	{
		AcArray<AcGeLineSeg3d> ptOnResZarry;

		while (true)
		{
			int nLen = ptOnZarry.length();
			BOOL bRemoveData = FALSE;
			for (int i = 0; i < nLen; i++)
			{
				AcGePoint3d ptStart1 = ptOnZarry[i].startPoint();
				AcGePoint3d ptEnd1 = ptOnZarry[i].endPoint();
				AcGeLineSeg3d line3d1(ptStart1, ptEnd1);
				AcGeVector3d vec1 = ptEnd1 - ptStart1;
				if (vec1.isZeroLength(tol))
				{
					ptOnZarry.removeAt(i);
					bRemoveData = TRUE;
					break;
				}
				for (int j = i + 1; j < nLen; j++)
				{
					AcGePoint3d ptStart2 = ptOnZarry[j].startPoint();
					AcGePoint3d ptEnd2 = ptOnZarry[j].endPoint();
					AcGeLineSeg3d line3d2(ptStart2, ptEnd2);
					AcGeVector3d vec2 = ptEnd2 - ptStart2;
					if (vec1.isZeroLength(tol))
					{
						ptOnZarry.removeAt(j);
						bRemoveData = TRUE;
						break;
					}
					else if (!vec1.isParallelTo(vec2, tol))
					{
						continue;
					}
					else if (line3d1.isOn(ptEnd2, tol)
						&& line3d1.isOn(ptStart2, tol))
					{
						ptOnZarry.removeAt(j);
						bRemoveData = TRUE;
						break;
					}
					else if (line3d2.isOn(ptStart1, tol)
						&& line3d2.isOn(ptEnd1, tol))
					{
						ptOnZarry.removeAt(i);
						bRemoveData = TRUE;
						break;
					}

					else if (line3d1.isOn(ptStart2, tol)
						&& !line3d1.isOn(ptEnd2, tol))
					{
						if (vec1.normal().isEqualTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptStart1, ptEnd2);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						else if (vec1.normal().isParallelTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptEnd2, ptEnd1);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						break;
					}
					else if (line3d1.isOn(ptEnd2, tol)
						&& !line3d1.isOn(ptStart2, tol))
					{
						if (vec1.normal().isEqualTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptStart2, ptEnd1);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						else if (vec1.normal().isParallelTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptStart1, ptStart2);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						break;
					}

					else if (line3d2.isOn(ptStart1, tol)
						&& !line3d2.isOn(ptEnd1, tol))
					{
						if (vec1.normal().isEqualTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptStart2, ptEnd1);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						else if (vec1.normal().isParallelTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptEnd2, ptEnd1);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						break;
					}
					else if (line3d2.isOn(ptEnd1, tol)
						&& !line3d2.isOn(ptStart1, tol))
					{
						if (vec1.normal().isEqualTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptStart1, ptEnd2);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						else if (vec1.normal().isParallelTo(vec2.normal(), tol))
						{
							AcGeLineSeg3d lineSeg(ptStart2, ptStart1);
							ptOnZarry.removeAt(j);
							ptOnZarry.removeAt(i);
							ptOnZarry.append(lineSeg);
							bRemoveData = TRUE;
							break;
						}
						break;
					}
				}
				if (bRemoveData)
					break;
			}
			if (!bRemoveData)
				break;
		}
	}

	//删除点集中两个相邻一样的点
	static void delNextPointIsEqual(AcGePoint3dArray& ptArry, const AcGeTol& tol = getPrjGeTol())
	{
		AcGePoint3dArray ptResArry;
		int nLen = ptArry.length();
		if (nLen < 2)
			return;
		AcGePoint3d ptTmp = ptArry[0];
		ptResArry.append(ptTmp);
		for (int i = 1; i < nLen; i++)
		{
			AcGePoint3d pt = ptArry[i];
			if (pt.isEqualTo(ptTmp, tol))
			     continue;
			else
			{
				ptTmp = pt;
				ptResArry.append(ptTmp);
			}
		}
		ptArry = ptResArry;
	}

	//合并共线点
	static void mergePolyline(AcGePoint3dArray& ptArry, const AcGeTol& tol = getPrjGeTol())
	{
		delNextPointIsEqual(ptArry);
		int nLen = ptArry.length();
		if (nLen < 3)
			return;
		BOOL bIsoLoop = ptArry.first().isEqualTo(ptArry.last(), tol);
		AcArray<AcArray<AcGeLineSeg3d>> lineSegs;
		AcGeLineSeg3d lineSeg(ptArry[0], ptArry[1]);

		AcArray<AcGeLineSeg3d> lineSegArry;
		lineSegArry.append(lineSeg);

		int nEndIndex = nLen - 1;
		if (bIsoLoop)
		{
			for (int i = nLen - 1; i > 0; i--)
			{
				AcGePoint3d pt1 = ptArry[i];
				AcGePoint3d pt2 = ptArry[i - 1];
				AcGeLineSeg3d lineSegTmp(pt1, pt2);

				if (lineSeg.isParallelTo(lineSegTmp, tol))
				{
					lineSegArry.append(lineSegTmp);
					nEndIndex--;
					continue;
				}
				break;
			}
		}
		
		for (int i = 1; i < nEndIndex; i++)
		{
			AcGePoint3d pt1 = ptArry[i];
			AcGePoint3d pt2 = ptArry[i + 1];
			AcGeLineSeg3d lineSegTmp(pt1, pt2);

			if (lineSeg.isParallelTo(lineSegTmp, tol))
			{
				lineSegArry.append(lineSegTmp);
			}
			else
			{
				if (!lineSegArry.isEmpty())
					lineSegs.append(lineSegArry);
				lineSegArry.removeAll();
				lineSeg = lineSegTmp;
				lineSegArry.append(lineSeg);
			}
		}
		if (!lineSegArry.isEmpty())
		    lineSegs.append(lineSegArry);
		if (lineSegs.isEmpty())
			return;

		AcGePoint3dArray vertices;
		for (int i = 0; i < lineSegs.length(); i++)
		{
			mergeOnLines(lineSegs[i]);
			assert(lineSegs[i].length() == 1);
			vertices.append(lineSegs[i][0].startPoint());
		}
		vertices.append(lineSegs.last()[0].endPoint());
		ptArry = vertices;
// 		if (bIsoLoop)
// 		{
// 			AcGePoint3d pt1 = ptArry[nLen - 1];
// 			AcGePoint3d pt2 = ptArry[nLen - 2];
// 			AcGeVector3d vecNormal = (pt2 - pt1).normal();
// 			while (ptArry.length() > 1)
// 			{
// 				pt2 = ptArry[1];
// 				if (pt2.isEqualTo(pt1, tol))
// 				{
// 					ptArry.removeFirst();
// 					ptArry.last() = ptArry.first();
// 					continue;
// 				}
// 				AcGeVector3d vecNormalTmp = (pt2 - pt1).normal();
// 				if (vecNormalTmp.isParallelTo(vecNormal, tol))
// 				{
// 					ptArry.removeFirst();
// 					ptArry.last() = ptArry.first();
// 					continue;
// 				}
// 				break;
// 			}
// 		}
// 		if (ptArry.length() < 2)
// 			return;
// 		if (bIsoLoop)
// 			ptArry.removeLast();
// 		while (true)
// 		{
// 			BOOL bRemoveData = FALSE;
// 			for (int i = 0; i < ptArry.length(); i++)
// 			{
// 				if (i == nLen)
// 					i = 0;
// 
// 				int iStartIndex = i - 1;
// 				if (iStartIndex < 0)
// 					iStartIndex = ptArry.length() - 1;
// 
// 				int iEndIndex = i + 1;
// 				if (iEndIndex == ptArry.length())
// 					iEndIndex = 0;
// 
// 				AcGePoint3d pt1 = ptArry[iStartIndex];
// 				AcGePoint3d pt2 = ptArry[i];
// 				AcGePoint3d pt3 = ptArry[iEndIndex];
// 
// 				AcGeVector3d vec1 = (pt2 - pt1).normal();
// 				AcGeVector3d vec2 = (pt3 - pt2).normal();
// 				double dDist1 = pt2.distanceTo(pt1);
// 				double dDist2 = pt2.distanceTo(pt3);
// 
// 
// 				if (fabs(dDist1) < tol.equalPoint())
// 				{
// 					ptArry.removeAt(i);
// 					bRemoveData = TRUE;
// 				}
// 				else if (fabs(dDist2) < tol.equalPoint())
// 				{
// 					ptArry.removeAt(iEndIndex);
// 					bRemoveData = TRUE;
// 				}
// 
// 				else if (vec1.isEqualTo(vec2, tol))
// 				{
// 					ptArry.removeAt(i);
// 					bRemoveData = TRUE;
// 					break;
// 				}
// 				else if (vec1.isParallelTo(vec2, tol))
// 				{
// 					if (fabs(dDist1 - dDist2) < EPRES || dDist1 > dDist2)
// 						ptArry.removeAt(iEndIndex);
// 					else
// 					{
// 						ptArry[iStartIndex] = ptArry[iEndIndex];
// 						ptArry.removeAt(iEndIndex);
// 					}
// 					bRemoveData = TRUE;
// 					break;
// 				}
// 				if (bRemoveData)
// 					break;
// 			}
// 			if (!bRemoveData)
// 				break;
// 		}
// 		if (bIsoLoop)
// 			ptArry.append(ptArry.first());
	}

	//判断点是否在点集的轮廓里 0在边界上 1在内 -1在外面
	static int PtOnArea(const AcGePoint3dArray& ptarray, AcGePoint3d Pt, BOOL b2dPlane = FALSE, const AcGeTol& tol = getPrjGeTol())
	{
		AcGePoint3dArray ptTmpArry = ptarray;
		mergePolyline(ptTmpArry, tol);
		int nLen = ptTmpArry.length();
		if (nLen < 3)
			return -1;

		AcGePlane plane(ptTmpArry[0], ptTmpArry[1], ptTmpArry[2]);
		if (!b2dPlane)
		{
			if (!plane.isOn(Pt, tol))
				return -1;
		}
		else
		{
			Pt = plane.closestPointTo(Pt, tol);
		}

		AcGeVector3d vec1 = plane.normal();
		AcGePoint3d origin;
		AcGeVector3d axisX, axisY;
		plane.getCoordSystem(origin, axisX, axisY);

		AcGeMatrix3d mat;
		mat.setCoordSystem(origin, axisX, axisY, plane.normal());
		mat = mat.inverse();

		transPtsFromMatrix3d(ptTmpArry, mat);
		Pt.transformBy(mat);

		BOOL bIsLoop = ptTmpArry.first().isEqualTo(ptTmpArry.last(), tol);
		if (!bIsLoop)
			ptTmpArry.append(ptTmpArry.first());

		int Len = ptTmpArry.length();

		// 	if (b2dPlane)
		// 	{
		// 		double minX = 0, minY = 0, maxX = 0, maxY = 0;
		// 		minX = ptTmpArry[0].x;
		// 		minY = ptTmpArry[0].y;
		// 		maxX = ptTmpArry[0].x;
		// 		maxY = ptTmpArry[0].y;
		// 		for (int i = 0; i < ptTmpArry.length(); i++)
		// 		{
		// 			if (ptTmpArry[i].x < minX)
		// 				minX = ptTmpArry[i].x;
		// 			if (ptTmpArry[i].y < minY)
		// 				minY = ptTmpArry[i].y;
		// 			if (ptTmpArry[i].x > maxX)
		// 				maxX = ptTmpArry[i].x;
		// 			if (ptTmpArry[i].y > maxY)
		// 				maxY = ptTmpArry[i].y;
		// 		}
		// 		if (ptTest.x < minX || ptTest.x > maxX || ptTest.y < minY || ptTest.y > maxY)
		// 			return -1;
		// 		int i, j, c = 0;
		// 		for (i = 0, j = nLen - 1; i < nLen; j = i++)
		// 		{
		// 			if (((ptTmpArry[i].y > ptTest.y) != (ptTmpArry[j].y>ptTest.y)) &&
		// 				(ptTest.x < (ptTmpArry[j].x - ptTmpArry[i].x) * (ptTest.y - ptTmpArry[i].y) / (ptTmpArry[j].y - ptTmpArry[i].y) + ptTmpArry[i].x))
		// 				c = !c;
		// 		}
		// 		return c;
		// 	}
		double Ang = 0.0, TAng = 0.0, T = 0.0;

		AcGeVector3d Vec1, Vec2;

		for (int i = 0; i < Len - 1; i++)
		{
			Vec1 = ptTmpArry[i] - Pt;
			Vec2 = ptTmpArry[i + 1] - Pt;
			AcGeLineSeg3d linseg(ptTmpArry[i], ptTmpArry[i + 1]);
			if (linseg.isOn(Pt, tol))
				return 0;
			Ang = Vec1.angleTo(Vec2);
			T = (ptTmpArry[i][X] - Pt[X])*(ptTmpArry[i + 1][Y] - Pt[Y]) - (ptTmpArry[i + 1][X] - Pt[X])*(ptTmpArry[i][Y] - Pt[Y]);
			if (T > 0.)
				TAng += Ang;
			else
				TAng += -Ang;
		}

		if (fabs(fabs(TAng) - 2 * rlPi) < 0.0001)
			return 1;

		return -1;
	}

	//判断是否逆时针
	static bool _polygon_is_counterclockwise(const AcGePoint3dArray &vertices, AcGeVector3d vecNormal = AcGeVector3d(0, 0, 1), const AcGeTol& tol = getPrjGeTol())
	{
		bool bIsClockwise = false;
		AcGePoint3dArray ptTmpArry = vertices;
		mergePolyline(ptTmpArry, tol);
		int nLen = ptTmpArry.length();
		if (nLen < 3)
			return bIsClockwise;

		BOOL bIsLoop = ptTmpArry.first().isEqualTo(ptTmpArry.last(), tol);
		if (!bIsLoop)
			ptTmpArry.append(ptTmpArry.first());

		double dRotateAngle = rlPi / 2.0;

		for (int i = 0; i < ptTmpArry.length() - 1; i++)
		{
			AcGeVector3d vec = ptTmpArry[i + 1] - ptTmpArry[i];
			vec = vec.rotateBy(dRotateAngle, vecNormal);
			AcGePoint3d ptMind = middle(ptTmpArry[i + 1], ptTmpArry[i]);
			ptMind += vec.normal() * (tol.equalPoint() + 0.1);
			int iType = PtOnArea(ptTmpArry, ptMind,FALSE,tol);
			if (iType == 0)
				continue;
			if (iType == 1)
				bIsClockwise = true;
			if (iType == -1)
				bIsClockwise = false;
			break;
		}
		return bIsClockwise;
	}

	//判断是否逆时针
	static bool _polygon_is_counterclockwise(const AcGePoint2dArray &vertices, AcGeVector3d vecNormal = AcGeVector3d(0, 0, 1), const AcGeTol& tol = getPrjGeTol())
	{
		AcGePoint3dArray ptTmpArry;
		for (int i = 0; i < vertices.length(); i++)
		{
			AcGePoint2d pt = vertices[i];
			ptTmpArry.append(AcGePoint3d(pt.x, pt.y, 0));
		}
		return _polygon_is_counterclockwise(ptTmpArry, vecNormal, tol);
	}

	//将点往外扩dDist的距离
	static void getExtendPtArry(double& dRotateAngle, AcGePoint3dArray& ptResArry, double dDist)
	{
		AcGePoint3dArray ptTmpArry = ptResArry;
		rlArxCommonHelp::mergePolyline(ptTmpArry);
		if (fabs(dDist) < EPRES || ptTmpArry.length() < 3)
			return;

		AcGeVector3d vec1 = (ptTmpArry[1] - ptTmpArry[0]).normal();
		AcGeVector3d vec2 = (ptTmpArry[2] - ptTmpArry[1]).normal();
		AcGeVector3d vecNorm = vec1.crossProduct(vec2).normal();

		ptResArry.removeAll();
		dRotateAngle = -rlPi / 2.0;
		BOOL bIsLoop = ptTmpArry.first().isEqualTo(ptTmpArry.last(), rlArxCommonHelp::getPrjGeTol());
		if (bIsLoop)
			ptTmpArry.removeLast();

		AcGePoint3d ptTmp = rlArxCommonHelp::middle(ptTmpArry[1], ptTmpArry[0]);
		AcGeVector3d vecPre = vec1.rotateBy(dRotateAngle, vecNorm);
		ptTmp = ptTmp + vecPre.normal() * (AcGeContext::gTol.equalPoint() + 0.1);

		int bOn = rlArxCommonHelp::PtOnArea(ptTmpArry, ptTmp, FALSE, AcGeContext::gTol);
		if (bOn == 1 && dDist > EPRES)
			dRotateAngle = -dRotateAngle;
		else if (bOn != 1 && dDist < EPRES)
			dRotateAngle = -dRotateAngle;

		dDist = fabs(dDist);
		int nLen = ptTmpArry.length();
		for (int i = 0; i < nLen; i++)
		{
			int iStartIndex = i - 1;
			int iEndIndex = i + 1;

			if (i == 0)
				iStartIndex = nLen - 1;
			if (i == nLen - 1)
				iEndIndex = 0;

			if (iStartIndex == iEndIndex)
				continue;

			AcGePoint3d pt = ptTmpArry[i];
			AcGePoint3d ptStart = ptTmpArry[iStartIndex];
			AcGePoint3d ptEnd = ptTmpArry[iEndIndex];
			AcGeVector3d vecX = pt - ptStart;
			AcGeVector3d vecY = ptEnd - pt;
			vecX = vecX.rotateBy(dRotateAngle, vecNorm).normal();
			vecY = vecY.rotateBy(dRotateAngle, vecNorm).normal();
			AcGeLine3d line1(pt + vecX*dDist, pt - ptStart);
			AcGeLine3d line2(pt + vecY*dDist, ptEnd - pt);
			line1.intersectWith(line2, pt);
			ptResArry.append(pt);
		}
		if (ptResArry.isEmpty())
			ptResArry = ptTmpArry;

		if (bIsLoop&&!ptResArry.isEmpty())
			ptResArry.append(ptResArry.first());

		return;
	}

	static void getExtendPtArry(AcGePoint3dArray& ptResArry, double dDist)
	{
		AcGePoint3dArray ptTmpArry = ptResArry;
		rlArxCommonHelp::mergePolyline(ptTmpArry);
		if (fabs(dDist) < EPRES || ptTmpArry.length() < 3)
			return;

		AcGeVector3d vec1 = (ptTmpArry[1] - ptTmpArry[0]).normal();
		AcGeVector3d vec2 = (ptTmpArry[2] - ptTmpArry[1]).normal();
		AcGeVector3d vecNorm = vec1.crossProduct(vec2).normal();

		ptResArry.removeAll();
		double dRotateAngle = -rlPi / 2.0;
		BOOL bIsLoop = ptTmpArry.first().isEqualTo(ptTmpArry.last(), rlArxCommonHelp::getPrjGeTol());
		if (bIsLoop)
			ptTmpArry.removeLast();

		AcGePoint3d ptTmp = rlArxCommonHelp::middle(ptTmpArry[1], ptTmpArry[0]);
		AcGeVector3d vecPre = vec1.rotateBy(dRotateAngle, vecNorm);
		ptTmp = ptTmp + vecPre.normal() *  (AcGeContext::gTol.equalPoint() + 0.1);

		int bOn = rlArxCommonHelp::PtOnArea(ptTmpArry, ptTmp, FALSE, AcGeContext::gTol);
		if (bOn == 1 && dDist > EPRES)
			dRotateAngle = -dRotateAngle;
		else if (bOn != 1 && dDist < EPRES)
			dRotateAngle = -dRotateAngle;

		dDist = fabs(dDist);
		int nLen = ptTmpArry.length();
		for (int i = 0; i < nLen; i++)
		{
			int iStartIndex = i - 1;
			int iEndIndex = i + 1;

			if (i == 0)
				iStartIndex = nLen - 1;
			if (i == nLen - 1)
				iEndIndex = 0;

			if (iStartIndex == iEndIndex)
				continue;

			AcGePoint3d pt = ptTmpArry[i];
			AcGePoint3d ptStart = ptTmpArry[iStartIndex];
			AcGePoint3d ptEnd = ptTmpArry[iEndIndex];
			AcGeVector3d vecX = pt - ptStart;
			AcGeVector3d vecY = ptEnd - pt;
			vecX = vecX.rotateBy(dRotateAngle, vecNorm).normal();
			vecY = vecY.rotateBy(dRotateAngle, vecNorm).normal();
			AcGeLine3d line1(pt + vecX*dDist, pt - ptStart);
			AcGeLine3d line2(pt + vecY*dDist, ptEnd - pt);
			line1.intersectWith(line2, pt);
			ptResArry.append(pt);
		}
		if (ptResArry.isEmpty())
			ptResArry = ptTmpArry;

		if (bIsLoop&&!ptResArry.isEmpty())
			ptResArry.append(ptResArry.first());

		return;
	}

	static AcDb3dSolid* get3dSolid(const AcGePoint3dArray& ptArry, AcGePoint3dArray ptPathArry,
		double dFacebAccer = 0.0, double dPathAccer = 0.0)
	{
		double dAngle = 0.0;
		AcGePoint3dArray ptResArry = ptArry;
		getExtendPtArry(dAngle, ptResArry, dFacebAccer);

		if (fabs(dPathAccer) > EPRES)
		{
			AcGeMatrix3d mat;
			AcGeVector3d vecNorm = (ptPathArry[0] - ptPathArry[1]).normal();
			mat.setToTranslation(vecNorm*dPathAccer);
			rlArxCommonHelp::transPtsFromMatrix3d(ptResArry, mat);

			ptPathArry.first() += vecNorm.normal()*dPathAccer;
			ptPathArry.last() += (ptPathArry.last() - ptPathArry[ptPathArry.length() - 2]).normal()*dPathAccer;
		}

		if (ptResArry.length() < 3 || ptPathArry.length() < 2)
			return NULL;

		AcDbVoidPtrArray arCurves;
		AcDb3dPolyline * pPolyLine = new AcDb3dPolyline(AcDb::k3dSimplePoly, ptResArry, Adesk::kTrue);
		arCurves.append(pPolyLine);

		AcDbVoidPtrArray arRegions;
		AcDbRegion::createFromCurves(arCurves, arRegions);
		if (arRegions.length() == 0)
		{
			rlArxCommonHelp::releaseVoidPtr(arCurves);
			rlArxCommonHelp::releaseVoidPtr(arRegions);
			return NULL;
		}

		AcDb3dPolyline * pPathPolyLine = new AcDb3dPolyline(AcDb::k3dSimplePoly, ptPathArry, Adesk::kFalse);
		arCurves.append(pPathPolyLine);
		AcDbRegion * pReg = (AcDbRegion*)arRegions.at(0);
		AcDb3dSolid * pSolid = new AcDb3dSolid;
		Acad::ErrorStatus es;
		es = pSolid->extrudeAlongPath(pReg, pPathPolyLine);
		if (es != Acad::eOk)
			SAFE_DELETE(pSolid);

		rlArxCommonHelp::releaseVoidPtr(arCurves);
		rlArxCommonHelp::releaseVoidPtr(arRegions);
		return pSolid;
	}

	static AcDb3dSolid* get3dSolid(const AcGePoint3dArray& ptArry1, const AcGePoint3dArray& ptArry2,
			AcGePoint3dArray ptPathArry, double dFacebAccer = 0.0, double dPathAccer = 0.0)
	{
		double dAngle = 0.0;
		AcGePoint3dArray ptResArry1 = ptArry1;
		rlArxCommonHelp::getExtendPtArry(dAngle, ptResArry1, dFacebAccer);
		AcGePoint3dArray ptResArry2 = ptArry2;
		rlArxCommonHelp::getExtendPtArry(dAngle, ptResArry2, dFacebAccer);

		if (ptResArry1.length() < 3 || ptResArry1.length() < 3 || ptPathArry.length() < 2)
			return NULL;

		if (fabs(dPathAccer) > EPRES)
		{
			AcGeMatrix3d mat1;
			AcGeVector3d vecNorm = (ptPathArry[0] - ptPathArry[1]).normal();
			ptPathArry.first() += vecNorm*dPathAccer;
			mat1.setToTranslation(vecNorm*dPathAccer);
			rlArxCommonHelp::transPtsFromMatrix3d(ptResArry1, mat1);

			AcGeMatrix3d mat2;
			vecNorm = (ptPathArry.last() - ptPathArry[ptPathArry.length() - 2]).normal();
			ptPathArry.last() += vecNorm*dPathAccer;
			mat2.setToTranslation(vecNorm*dPathAccer);
			rlArxCommonHelp::transPtsFromMatrix3d(ptResArry2, mat2);
		}

		AcDb3dPolylineVertex* pPLineVetex = NULL;
		AcDb3dPolyline*       pPolyLine1 = new AcDb3dPolyline(AcDb::k3dSimplePoly, ptResArry1, Adesk::kTrue);
		AcDb3dPolyline*       pPolyLine2 = new AcDb3dPolyline(AcDb::k3dSimplePoly, ptResArry2, Adesk::kTrue);

		AcArray<AcDbEntity*>arSecs;
		arSecs.append(pPolyLine1);
		arSecs.append(pPolyLine2);

		AcDb3dPolyline * pPathPolyLine = new AcDb3dPolyline(AcDb::k3dSimplePoly, ptPathArry, Adesk::kFalse);

		AcArray<AcDbEntity*>arGuits;
		AcDbLoftOptions option;
		//option.setNormal(AcDbLoftOptions::kAllNormal);
		Acad::ErrorStatus es;
		AcDb3dSolid * pSolid = new AcDb3dSolid;
		//放样
		es = pSolid->createLoftedSolid(arSecs, arGuits, pPathPolyLine, option);
		arSecs.append(pPathPolyLine);

		if (es != Acad::eOk)
			SAFE_DELETE(pSolid);

		for (int i = 0; i < arSecs.length(); ++i)
			SAFE_DELETE(arSecs[i]);

		return pSolid;
	}

}