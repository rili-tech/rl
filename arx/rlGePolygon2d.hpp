#pragma once
#include <geplin2d.h>

/**
 * @note - 二维多边形：扩展AcGePolyline2d
 */

class rlGePolygon2d : public AcGePolyline2d
{
public:
	rlGePolygon2d(int numFitPoints)
		: AcGePolyline2d(pnt2d_array_init(numFitPoints + 1))
	{
	}

	/**
	* @note - 清理重合点、清除共线的顶点
	* @param - out
	* 外部释放
	*/
	static void simplify(AcGePolyline2d *&out, const AcGePolyline2d &polygon)
	{
		if (polygon.numFitPoints() < 3)
			return;

		AcGeTol tol;
		tol.setEqualPoint(rlTolerance::equal_point());
		tol.setEqualVector(rlTolerance::equal_vector());

		// - 去除重合点
		AcGePoint2dArray pnts1;
		pnts1.append(polygon.fitPointAt(0));
		int size = polygon.numFitPoints();
		for (int m = 1; m < size - 1; m++)
		{
			AcGePoint2d pnt = polygon.fitPointAt(m);
			if (pnt.isEqualTo(pnts1[pnts1.length() - 1], tol) == false)
				pnts1.append(pnt);
		}
		AcGePoint2d last = polygon.fitPointAt(size - 1);
		if (last.isEqualTo(pnts1[0], tol) == false)
			pnts1.append(last);
		if (pnts1.length() < 3)
			return;

		// - 去除共线点
		AcGePoint2dArray pnts2;
		int size1 = pnts1.length();
		for (int i = 0; i < size1; i++)
		{
			const AcGePoint2d &pnt0 = pnts1[i == 0 ? size1 - 1 : i - 1];
			const AcGePoint2d &pnt1 = pnts1[i];
			const AcGePoint2d &pnt2 = pnts1[i == size1 - 1 ? 0 : i + 1];

			if (_is_on_one_line(pnt0, pnt1, pnt2, tol) == false)
				pnts2.append(pnt1);
		}

		pnts2.append(pnts2[0]);
		out = new AcGePolyline2d(pnts2);
	}

	int numFitPoints() const
	{
		return AcGePolyline2d::numFitPoints() - 1;
	}

	AcGePoint2d fitPointAt(int idx) const
	{
		return AcGePolyline2d::fitPointAt(idx);
	}

	AcGeSplineEnt2d &setFitPointAt(int idx, const AcGePoint2d& point)
	{
		if (idx == 0)
			AcGePolyline2d::setFitPointAt(AcGePolyline2d::numFitPoints() - 1, point);
		return AcGePolyline2d::setFitPointAt(idx, point);
	}

protected:
	rlGePolygon2d(){}

	AcGePoint2dArray pnt2d_array_init(int num)
	{
		AcGePoint2dArray pts;
		for (int i = 0; i < num; i++)
			pts.append(AcGePoint2d(0, 0));
		return pts;
	}

	static bool _is_on_one_line(const AcGePoint2d &pnt0, const AcGePoint2d &pnt1, const AcGePoint2d &pnt2,
		const AcGeTol &tol)
	{
		if (pnt0.isEqualTo(pnt1, tol) || pnt2.isEqualTo(pnt1, tol))
			return true;

		AcGeVector2d v1 = pnt1 - pnt0;
		AcGeVector2d v2 = pnt2 - pnt1;
		if (v1.isParallelTo(v2, tol) == Adesk::kFalse)
			return false;
		return true;
	}
};