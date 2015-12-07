#pragma once
#include "rl_defined_types.h"
#include "rlGePolygon2d.hpp"

/**
 * @note - 二维矩形：扩展AcGe
 */

class rlGeRectangle2d : private rlGePolygon2d
{
public:
	static bool is_rectangle(rlGeRectangle2d &rect, const AcGePolyline2d &pline)
	{
		// - 去除共线的顶点
		AcGePolyline2d *simplified = NULL;
		simplify(simplified, pline);
		if (simplified == NULL)
			return false;

		AcGeTol tol;
		tol.setEqualPoint(rlTolerance::equal_point());
		tol.setEqualVector(rlTolerance::equal_vector());

		// - 初步判断
		int size = simplified->numFitPoints();
		bool is_equal = simplified->fitPointAt(0).isEqualTo(simplified->fitPointAt(4), tol);
		if (size != 5 || is_equal == false)
		{
			SAFE_DELETE(simplified);
			return false;
		}

		// - 计算每一个角度
		AcGePoint2d p0 = simplified->fitPointAt(0);
		AcGePoint2d p1 = simplified->fitPointAt(1);
		AcGePoint2d p2 = simplified->fitPointAt(2);
		AcGePoint2d p3 = simplified->fitPointAt(3);
		SAFE_DELETE(simplified);

		AcGeVector2d v1 = p1 - p0; v1.normalize();
		AcGeVector2d v2 = p3 - p0; v2.normalize();
		if (v1.isPerpendicularTo(v2, tol) == Adesk::kFalse)
			return false;

		v1 = p2 - p1; v1.normalize();
		v2 = p0 - p1; v2.normalize();
		if (v1.isPerpendicularTo(v2, tol) == Adesk::kFalse)
			return false;

		v1 = p3 - p2; v1.normalize();
		v2 = p1 - p2; v2.normalize();
		if (v1.isPerpendicularTo(v2, tol) == Adesk::kFalse)
			return false;

		v1 = p0 - p3; v1.normalize();
		v2 = p2 - p3; v2.normalize();
		if (v1.isPerpendicularTo(v2, tol) == Adesk::kFalse)
			return false;

		double width = p1.distanceTo(p0);
		double height = p2.distanceTo(p1);

		AcGeVector2d e0 = p1 - p0; e0.normalize();
		AcGeVector2d e1 = p2 - p1; e1.normalize();
		AcGePoint2d center = p0;
		center.transformBy(e0 * width / 2.0);
		center.transformBy(e1 * height / 2.0);

		rect.set(center, e0, width, height);

		return true;
	}


	rlGeRectangle2d()
		: rlGePolygon2d(4)
	{

	}

	rlGeRectangle2d(const AcGePoint2d &center, const AcGeVector2d &e0, 
		double width, double height)
		: rlGePolygon2d(4)
	{
		_init(center, e0, width, height);
	}

	void set(const AcGePoint2d &center, const AcGeVector2d &e0,
		double width, double height)
	{
		_init(center, e0, width, height);
	}

	AcGePoint2d center() const
	{
		AcGePoint2d p0 = rlGePolygon2d::fitPointAt(0);
		AcGeVector2d e0 = rlGePolygon2d::fitPointAt(1) - p0;
		AcGeVector2d e1 = rlGePolygon2d::fitPointAt(3) - p0;
		p0.transformBy(e0 / 2.0);
		p0.transformBy(e1 / 2.0);
		return p0;
	}

	AcGeVector2d e0() const
	{
		AcGeVector2d e0 = rlGePolygon2d::fitPointAt(1) - rlGePolygon2d::fitPointAt(0);
		e0.normalize();
		return e0;
	}

	double width() const
	{
		return rlGePolygon2d::fitPointAt(0).distanceTo(rlGePolygon2d::fitPointAt(1));
	}

	double height() const
	{
		return rlGePolygon2d::fitPointAt(3).distanceTo(rlGePolygon2d::fitPointAt(0));
	}

	int numFitPoints() const
	{
		return rlGePolygon2d::numFitPoints();
	}

	AcGePoint2d fitPointAt(int idx) const
	{
		return rlGePolygon2d::fitPointAt(idx);
	}

private:
	void _init(const AcGePoint2d & center, const AcGeVector2d & e0, double width, double height)
	{
		AcGeVector2d _e0 = e0;
		_e0.normalize();

		AcGeVector2d e1 = _e0;
		e1.rotateBy(rlPi / 2);

		AcGePoint2d pt = center;
		pt.transformBy(-_e0 * width / 2.0);
		pt.transformBy(-e1 * height / 2.0);
		rlGePolygon2d::setFitPointAt(0, pt);

		pt.transformBy(_e0 * width);
		rlGePolygon2d::setFitPointAt(1, pt);

		pt.transformBy(e1 * height);
		rlGePolygon2d::setFitPointAt(2, pt);

		pt.transformBy(-_e0 * width);
		rlGePolygon2d::setFitPointAt(3, pt);
	}

};