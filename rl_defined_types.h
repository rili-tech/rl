#pragma once
#include <math.h>

#define rlPi 3.1415926535897932384626433832795
#define EPRES	1e-6
typedef unsigned __int64 rlId;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p){if(p)delete(p);p=NULL;}
#define SAFE_DELETE_ARR(p){if(p)delete[]p;p=NULL;}
#endif

/** 参考AcGeTol的阈值计算规则 */
struct rlTolerance
{
	static double equal_point() { return 1.5; }
	static double equal_vector(){ return 0.001; }

	/** equal_vector()大约和弧度相似，角度为1度时大约为.0.0175弧度 */
	static double equal_angle()
	{
		double x = 0, y = 0;
		double equal_vec = equal_vector();
		x = 1 - equal_vec * equal_vec / 2.0;
		y = sqrt(1 - x * x);

		AcGeVector2d v1(1, 0);
		AcGeVector2d v2(x, y);

		return v1.angleTo(v2);
	}
};

/** 
 * @note - 阈值为rlTolerance::equal_point()
 */
struct rl_double_sort
{
	bool operator()(double v1, double v2) const
	{
		if (fabs(v1 - v2) < rlTolerance::equal_point())
			return false;

		if (v1 < v2)
			return true;

		return false;
	}
};

struct scAcGeVector2d
{
	scAcGeVector2d()
	{
		m_vecLength = AcGeVector2d(0, 0);
	}
	scAcGeVector2d(AcGeVector2d vec)
	{
		m_vecLength = vec;
	}

	scAcGeVector2d(const scAcGeVector2d &other)
	{
		*this = other;
	}

	const scAcGeVector2d& operator = (const scAcGeVector2d &other)
	{
		m_vecLength = other.m_vecLength;
		return *this;
	}

	const scAcGeVector2d& operator = (const AcGeVector2d &other)
	{
		m_vecLength = other;
		return *this;
	}

	virtual bool operator < (const scAcGeVector2d&other)
	{
		if (m_vecLength.isEqualTo(other.m_vecLength))
		{
			return m_vecLength.length() < other.m_vecLength.length();
		}
		return false;
	}
	bool operator == (const scAcGeVector2d& other) const
	{
		return m_vecLength.isEqualTo(other.m_vecLength);
	}
	AcGeVector2d m_vecLength;
};

struct rl_scAcGeVector2d_sort
{
	bool operator()(scAcGeVector2d v1, scAcGeVector2d v2) const
	{
		if (fabs(v1.m_vecLength.length() - v2.m_vecLength.length()) < rlTolerance::equal_point())
		{
			if (v1.m_vecLength.isEqualTo(v2.m_vecLength))
				return false;
			else
			{
				if (fabs(v1.m_vecLength.angle() - v2.m_vecLength.angle()) < rlTolerance::equal_point())
					return false;

				if (v1.m_vecLength.angle() < v2.m_vecLength.angle())
					return true;
			}
		}
		if (v1.m_vecLength.length() <  v2.m_vecLength.length())
			return true;

		return false;
	}
};

enum rlBuildingElementType
{
	emColum,
	emBeam,
	emWall,
	emSlab,
};

enum rlClassType
{
	emTopologicalEntity,
	emTopologicalEntity2d,
	emVertex2d,
	emEdge2d,
	emHalfEdge2d,
};

#define rlBaseClassTypeDef(emType)  static rlClassType desc(){ return emType; }\
								  virtual rlClassType type() const{ return emType; }\
								  virtual bool is_kind_of(rlClassType type) const\
								  {\
									  if (type == emType)\
										  return true;\
									  return false;\
								  }

#define rlSubClassTypeDef(BaseClass,emType)  static rlClassType desc(){ return emType; }\
											rlClassType type() const{ return emType; }\
											bool is_kind_of(rlClassType type) const\
											{\
												if (type == emType)\
													return true;\
												return BaseClass::is_kind_of(type);\
											}

class rl_str_convertor
{
public:
	/** 
	 * @ret - 返回值外部释放 
	 */
	static char *wchar_t2char(const wchar_t *wstr)
	{
		size_t wlen = wcslen(wstr);
		size_t size = ::WideCharToMultiByte(CP_ACP, 0, wstr, (int)wlen, NULL, 0, NULL, NULL);
		char *str = new char[size + 1];
		::WideCharToMultiByte(CP_ACP, 0, wstr, (int)wlen, str, (int)size, NULL, NULL);
		str[size] = '\0';
		return str;
	}

	/**
	* @ret - 返回值外部释放
	*/
	static wchar_t *char2wchar_t(const char *str)
	{
		size_t len = strlen(str);
		size_t wsize = ::MultiByteToWideChar(CP_ACP, 0, str, (int)len, NULL, 0);
		wchar_t *wstr = new wchar_t[wsize + 1];
		::MultiByteToWideChar(CP_ACP, 0, str, (int)len, wstr, (int)wsize);
		wstr[wsize] = '\0';
		return wstr;
	}
};