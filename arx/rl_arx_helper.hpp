#pragma once

class rl_arx_fields
{
public:
	static Acad::ErrorStatus dwg_out_fields(const AcGeMatrix3d &matrix, AcDbDwgFiler *pFiler)
	{
		Acad::ErrorStatus es = Acad::eNotImplementedYet;
		for (int m = 0; m < 4; m++)
		{
			for (int n = 0; n < 4; n++)
			{
				es = pFiler->writeDouble(matrix.entry[m][n]);
				if (es != Acad::eOk)
					return es;
			}
		}

		return es;
	}

	static Acad::ErrorStatus dwg_in_fields(AcGeMatrix3d &matrix, AcDbDwgFiler *pFiler)
	{
		Acad::ErrorStatus es = Acad::eNotImplementedYet;
		for (int m = 0; m < 4; m++)
		{
			for (int n = 0; n < 4; n++)
			{
				es = pFiler->readDouble(&matrix.entry[m][n]);
				if (es != Acad::eOk)
					return es;
			}
		}

		return es;
	}

	static Acad::ErrorStatus dwg_out_fields(const AcGePoint3dArray &pnts, AcDbDwgFiler *pFiler)
	{
		Acad::ErrorStatus es = Acad::eNotImplementedYet;
		
		int size = pnts.length();
		es = pFiler->writeInt32(Adesk::Int32(size));
		if (es != Acad::eOk)
			return es;

		for (int i = 0; i < size; i++)
		{
			es = dwg_out_fields(pnts[i], pFiler);
			if (es != Acad::eOk)
				return es;
		}

		return es;
	}

	static Acad::ErrorStatus dwg_in_fields(AcGePoint3dArray &pnts, AcDbDwgFiler *pFiler)
	{
		Acad::ErrorStatus es = Acad::eNotImplementedYet;
		
		Adesk::Int32 size = 0;
		es = pFiler->readInt32(&size);
		if (es != Acad::eOk)
			return es;

		AcGePoint3d pnt;
		for (int i = 0; i < size; i++)
		{
			es = dwg_in_fields(pnt, pFiler);
			if (es != Acad::eOk)
				return es;
			pnts.append(pnt);
		}

		return es;
	}

	static Acad::ErrorStatus dwg_out_fields(const AcGePoint3d &pnt, AcDbDwgFiler *pFiler)
	{
		Acad::ErrorStatus es = Acad::eNotImplementedYet;

		es = pFiler->writeDouble(pnt.x);
		if (es != Acad::eOk)
			return es;

		es = pFiler->writeDouble(pnt.y);
		if (es != Acad::eOk)
			return es;

		es = pFiler->writeDouble(pnt.z);
		if (es != Acad::eOk)
			return es;

		return es;
	}

	static Acad::ErrorStatus dwg_in_fields(AcGePoint3d &pnt, AcDbDwgFiler *pFiler)
	{
		Acad::ErrorStatus es = Acad::eNotImplementedYet;

		es = pFiler->readDouble(&pnt.x);
		if (es != Acad::eOk)
			return es;

		es = pFiler->readDouble(&pnt.y);
		if (es != Acad::eOk)
			return es;

		es = pFiler->readDouble(&pnt.z);
		if (es != Acad::eOk)
			return es;

		return es;
	}
};

class rl_arx_database
{
public:
	static Acad::ErrorStatus append(AcDbObjectId &id, AcDbEntity *entity, AcDbDatabase *db)
	{
		AcDbBlockTable *blkTable = NULL;
		AcDbBlockTableRecord *blkTblRecord = NULL;

		Acad::ErrorStatus es = db->getBlockTable(blkTable, AcDb::kForRead);
		if (es != Acad::eOk)
			return es;
		es = blkTable->getAt(ACDB_MODEL_SPACE, blkTblRecord, AcDb::kForWrite);
		blkTable->close();
		if (es != Acad::eOk)
			return es;

		es = blkTblRecord->appendAcDbEntity(id, entity);
		blkTblRecord->close();

		return es;
	}

	static Acad::ErrorStatus append(AcDbEntity *entity, AcDbDatabase *db)
	{
		AcDbObjectId id;
		return append(id, entity, db);
	}

	static Acad::ErrorStatus append_and_close(AcDbEntity *entity, AcDbDatabase *db)
	{
		Acad::ErrorStatus es = append(entity,db);
		if (es != Acad::eOk)
			return es;
		return entity->close();
	}
};

class rl_arx_geometry
{
public:
	static AcGePoint3d middle(const AcGePoint3d &pnt1, const AcGePoint3d &pnt2)
	{
		return AcGePoint3d((pnt1.x + pnt2.x) / 2.0, (pnt1.y + pnt2.y) / 2.0, (pnt1.z + pnt2.z) / 2.0);
	}
};

class rl_arx_ucs
{
public:
#ifndef _DBXAPP
	/** 返回ecs->wcs的矩阵 */
	static AcGeMatrix3d ecs(const AcGePoint3d &origin = AcGePoint3d(0, 0, 0))
	{
		// - 获取法线
		resbuf buf;
		acedGetVar(_T("VIEWDIR"), &buf);

		// - 计算x轴
		ads_point ecs_xaxis, wcs_vec;
		ecs_xaxis[0] = 1; ecs_xaxis[0] = 0; ecs_xaxis[0] = 0;
		acdbEcs2Wcs(ecs_xaxis, wcs_vec, buf.resval.rpoint, true);

		// - 构造矩阵
		AcGeVector3d normal(buf.resval.rpoint[0], buf.resval.rpoint[1], buf.resval.rpoint[2]);
		AcGeVector3d e0(wcs_vec[0], wcs_vec[1], wcs_vec[2]);
		AcGeVector3d e1 = normal.crossProduct(e0);

		AcGeMatrix3d xform;
		xform.setCoordSystem(origin, e0, e1, normal);

		return xform;
	}
#endif

	/** 0 - wcs 1 - ucs 2 - dcs */
	static void wcs2dcs(AcGePoint3d &in_out)
	{
		resbuf from, to;
		from.restype = RTSHORT;
		from.resval.rint = 1;
		to.restype = RTSHORT;
		to.resval.rint = 2;

		ads_point pnt;
		pnt[0] = in_out.x; pnt[1] = in_out.y; pnt[2] = in_out.z;
		int ret = acedTrans(pnt, &from, &to, FALSE, pnt);
		in_out.set(pnt[0], pnt[1], pnt[2]);
	}
};

class rl_view_helper
{
public:
#ifndef _DBXAPP
	static AcGeVector3d view_direction()
	{
		resbuf buf;
		acedGetVar(_T("VIEWDIR"), &buf);
		return AcGeVector3d(buf.resval.rpoint[0], buf.resval.rpoint[1], buf.resval.rpoint[2]);
	}
#endif
};