//------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_restrict.h
// author: aslan
// actor:
// data: 2009-03-26
// last:
// brief: ��ͼ�����࣬��Ҫ���ڵ�ͼ�Ľ����жϼ������ж�
// remark: Ƕ�뵽��ͼ�������У�����ĳһ���ͼ�������жϣ�����ڸ���ʵ�����жϣ����ɸ��������ĵ�ͼ������ʵ��
//------------------------------------------------------------------------------------------------------
#pragma once

class Map;
class MapMgr;
class MapScript;
class MapInstance;

struct tagMapInfo;

//------------------------------------------------------------------------------------------------------
// ��ͼ���Ƴ�����
//------------------------------------------------------------------------------------------------------
class MapRestrict
{
public:
	virtual VOID Init(MapMgr* pMapMgr);
	virtual Map* CanEnter(Role* pRole, DWORD dwMisc) = 0;
	virtual Map* CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) = 0;
	virtual BOOL GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) = 0;

	const tagMapInfo* GetMapInfo()	{ return m_pInfo; }

protected:
	MapRestrict();
	virtual ~MapRestrict();

private:
	MapRestrict(const MapRestrict& rh);
	MapRestrict& operator=(const MapRestrict& rh);

protected:
	const tagMapInfo*	m_pInfo;
	MapMgr*				m_pMgr;
};

//--------------------------------------------------------------------------------------------------------
// ��ͨ��ͼ������
//--------------------------------------------------------------------------------------------------------
class MapRestrictNormal : public MapRestrict
{
public:
	virtual VOID Init(MapMgr* pMapMgr);
	virtual Map* CanEnter(Role* pRole, DWORD dwMisc);
	virtual Map* CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);
	virtual BOOL GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);
};

//--------------------------------------------------------------------------------------------------------
// ��ͨ����������
//--------------------------------------------------------------------------------------------------------
class MapRestrictInstance : public MapRestrict
{
public:
	virtual VOID Init(MapMgr* pMapMgr);
	virtual Map* CanEnter(Role* pRole, DWORD dwMisc);
	virtual Map* CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);
	virtual BOOL GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);

private:
	INT CanEnterByInstanceInfo(Role* pRole);
};

//---------------------------------------------------------------------------------------------------------
// �ȶ����������ࣨ���ָ�����
//---------------------------------------------------------------------------------------------------------
class MapRestrictStable : public MapRestrict
{
public:
	virtual VOID Init(MapMgr* pMapMgr);
	virtual Map* CanEnter(Role* pRole, DWORD dwMisc);
	virtual Map* CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);
	virtual BOOL GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);

private:
	MapInstance* GetOnePerfectMap();
};

//---------------------------------------------------------------------------------------------------------
// �ű������ĸ���������
//---------------------------------------------------------------------------------------------------------
class MapRestrictScript : public MapRestrict
{
public:
	virtual VOID Init(MapMgr* pMapMgr);
	virtual Map* CanEnter(Role* pRole, DWORD dwMisc);
	virtual Map* CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);
	virtual BOOL GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);

private:
	MapInstance* GetOnePerfectMap(Role* pRole);

private:
	const MapScript*	m_pScript;
};