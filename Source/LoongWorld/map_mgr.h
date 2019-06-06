//------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_mgr.h
// author: aslan
// actor:
// data: 2008-6-27
// last:
// brief: ��ͼ������
// remark: ÿ����ͼ����������һ���̣߳��Ըõ�ͼ�е��������Ｐ����������и��²�����ͬʱ��ɵ�ͼ��ͬ��
//------------------------------------------------------------------------------------------------------
#pragma once

#include "mutex.h"
#include "map.h"

struct tagMapInfo;
struct tagInstance;
class Map;
class MapInstance;
class MapCreator;
class Role;
class MapRestrict;

class MapMgr
{
	friend class MapRestrictNormal;
	friend class MapRestrictInstance;
	friend class MapRestrictStable;
	friend class MapRestrictPVP;

public:
	MapMgr();
	~MapMgr();

	//-------------------------------------------------------------------------------
	// ��ʼ�������º�����
	//-------------------------------------------------------------------------------
	BOOL				Init(tagMapInfo* pInfo);
	DWORD				ThreadUpdate();
	VOID				Destroy();

	//--------------------------------------------------------------------------------------------------------
	// �̹߳���
	//--------------------------------------------------------------------------------------------------------
	static VOID			StopThread()					{ InterlockedExchange((LPLONG)&m_bTerminate, TRUE); }
	tstring&			GetThreadName()					{ return m_strThreadName; }

	//--------------------------------------------------------------------------------------------------------
	// ����Get
	//--------------------------------------------------------------------------------------------------------
	Map*				GetSingleMap()					{ return m_pSingleMap; }
	MapInstance*		GetInstance(DWORD dwInstanceID) { return m_mapInstance.Peek(dwInstanceID); }
	const tagMapInfo*	GetMapInfo()					{ return m_pInfo; }

	BOOL		IsNormalMap()	{ return EMT_Normal == m_pInfo->eType; }
	BOOL		IsInstanceMap()	{ return EMT_Instance == m_pInfo->eType; }

	//--------------------------------------------------------------------------------------------------------
	// �����Ĵ�����ɾ������
	//--------------------------------------------------------------------------------------------------------
	MapInstance*		CreateInstance(Role* pCreator, DWORD dwMisc);
	BOOL				CreateScriptInstance(DWORD dwInstanceID);
	VOID				DestroyInstance(MapInstance* pInstance);
	INT					GetCurInstanceNum()	{ return m_nInstanceNum; }
	VOID				ChangeInstanceNum(INT nNum)		{ m_nInstanceNum += nNum; }

	//--------------------------------------------------------------------------------------------------------
	// ��������ڼ������ж�
	//--------------------------------------------------------------------------------------------------------
	Map*				CanEnter(Role* pRole, DWORD dwMisc=GT_INVALID);
	Map*				CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut);

	//--------------------------------------------------------------------------------------------------------
	// ��Ҵ�������
	//--------------------------------------------------------------------------------------------------------
	VOID				RoleInstanceOut(Role* pRole);


private:
	TSFPTrunk<MapMgr>			m_Trunk;
	TObjRef<Thread>				m_pThread;
	tstring						m_strThreadName;					// �߳�����
	static VOLATILE BOOL		m_bTerminate;						// �߳�ֹͣ��־

	const tagMapInfo*			m_pInfo;							// ��ͼ�������Զ���
	Map*						m_pSingleMap;						// �������ͨ��ͼ�����ָ��ָ���Ψһ��ͼ
	TMap<DWORD, MapInstance*>   m_mapInstance;						// ����Ǹ�����ͼ���������ڸõ�ͼ���������и���

	MapRestrict*				m_pRestrict;						// ���Զ���
	INT							m_nInstanceNum;						// ��ǰ�Ѿ������ĸ�������
};
