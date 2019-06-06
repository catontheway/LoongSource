//-----------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_mgr.cpp
// author: aslan
// actor:
// data: 2008-6-27
// last:
// brief: ��ͼ������
// remark: ÿ����ͼ����������һ���̣߳��Ըõ�ͼ�е��������Ｐ����������и��²�����ͬʱ��ɵ�ͼ��ͬ��
//------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "map_mgr.h"
#include "map.h"
#include "map_creator.h"
#include "map_restrict.h"
#include "role.h"

VOLATILE BOOL MapMgr::m_bTerminate = FALSE;
//-------------------------------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------------------------------
MapMgr::MapMgr() : m_pInfo(NULL), m_pSingleMap(NULL), m_pRestrict(NULL), m_nInstanceNum(0)
{
	m_mapInstance.Clear();
}

//-------------------------------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------------------------------
MapMgr::~MapMgr()
{
	Destroy();
}

//-------------------------------------------------------------------------------------------------------
// ��ʼ����ͼ��Ϣ�������ĳ�ʼ�������������
//-------------------------------------------------------------------------------------------------------
BOOL MapMgr::Init(tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pInfo) );

	m_pInfo = pInfo;

	// ���ɵ�ͼ����
	m_pRestrict = g_mapCreator.CreateAFMapRestrict(m_pInfo);
	m_pRestrict->Init(this);

	// ���ݵ�ͼ��Ϣ�ǲ��Ǹ��������Ƿ�����NavMap
	if( EMT_Normal == pInfo->eType )
	{
		m_pSingleMap = g_mapCreator.CreateAFMap(pInfo);
		if( !P_VALID(m_pSingleMap) )
			return FALSE;

		if( FALSE == m_pSingleMap->Init(pInfo) )
		{
			g_mapCreator.DestroyAFMap(m_pSingleMap);
			return FALSE;
		}
	}

	// ����Update�߳�
	m_strThreadName = _T("Thread_");
	m_strThreadName += m_pInfo->szMapFileName;
	m_pThread->CreateThread(m_strThreadName.c_str(), (vEngine::THREADPROC)m_Trunk.sfp0(&MapMgr::ThreadUpdate), NULL);

	while( FALSE == m_pThread->IsThreadActive(m_strThreadName.c_str()) )
		continue;

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// �̸߳��º���
//-------------------------------------------------------------------------------------------------------
DWORD MapMgr::ThreadUpdate()
{
#ifdef _DEBUG
	_set_se_translator(vEngine::SEH_Translation);

	try
	{
#endif
		INT nWhichEvent = 0;

		while(TRUE)
		{
			g_mapCreator.GetAllMapStartEvent(nWhichEvent).Wait();

			if( m_bTerminate )
				break;

			// ��ͨ��ͼ
			if( EMT_Normal == m_pInfo->eType )
			{
				// ���õ�ͼ�ĸ���
				m_pSingleMap->Update();
			}

			// ������ͼ
			else
			{
				// ����ÿһ������
				m_mapInstance.ResetIterator();
				MapInstance* pInstance = NULL;
				while( m_mapInstance.PeekNext(pInstance) )
				{
					pInstance->Update();

					// ��������ɾ����ɾ������
					if( pInstance->CanDestroy() )
					{
						g_mapCreator.InstanceDestroyed(m_pInfo->dwID, pInstance->GetInstanceID());
					}
				}
			}

			// ֪ͨmapcreator�Ѿ�ִ�����
			g_mapCreator.OneMapMgrEnd();

			nWhichEvent = ( (nWhichEvent == 0) ? 1 : 0 );
		}
#ifdef _DEBUG
	}
	catch(vEngine::Exception)
	{
#ifdef _DEBUG
		__asm int 3;
#endif

		// ����ShutDown
		g_world.ShutDown();

		Sleep(3000);

		// ֪ͨmapcreator�Ѿ�ִ�����
		g_mapCreator.OneMapMgrEnd();
	}
#endif
	_endthreadex(0);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------
VOID MapMgr::Destroy()
{
	// ��ɾ����ͼ
	if( EMT_Normal == m_pInfo->eType )
	{
		g_mapCreator.DestroyAFMap(m_pSingleMap);
	}
	else
	{
		m_mapInstance.ResetIterator();
		MapInstance* pInstance = NULL;

		while( m_mapInstance.PeekNext(pInstance) )
		{
			g_mapCreator.DestroyAFMap(pInstance);
		}
		m_mapInstance.Clear();
	}

	// ��ɾ������
	g_mapCreator.DestroyAFMapRestrict(m_pRestrict);
}

//--------------------------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------------------------
VOID MapMgr::RoleInstanceOut(Role *pRole)
{
	Vector3 vDestPos = pRole->GetCurPos();
	DWORD	dwDestMapID = GT_INVALID;

	// �õ����ڵ�ͼ������
	if( FALSE == m_pRestrict->GetExportMapAndCoord(pRole, dwDestMapID, vDestPos) )
	{
		ILOG->Write(_T("Try to out of the instance failed, mapid=%d\n"), m_pInfo->dwID);
		return;
	}

	pRole->GotoNewMap(dwDestMapID, vDestPos.x, vDestPos.y, vDestPos.z);
}

//--------------------------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------------------------
MapInstance* MapMgr::CreateInstance(Role* pCreator, DWORD dwMisc)
{
	if( !g_mapCreator.CanCreateInstance(m_pInfo, this) ) return NULL;

	MapInstance* pInstance = g_mapCreator.CreateAFMapInstance(m_pInfo);
	if( !P_VALID(pInstance) ) return NULL;

	g_mapCreator.AddInstance(m_pInfo, this);

	DWORD dwInstanceID = g_mapCreator.CreateNewInstanceID();

	if( FALSE == pInstance->Init(m_pInfo, dwInstanceID, pCreator, dwMisc) )
	{
		DestroyInstance(pInstance);
		return NULL;
	}

	m_mapInstance.Add(pInstance->GetInstanceID(), pInstance);

	return pInstance;
}

//--------------------------------------------------------------------------------------------------------
// �ű���������
//--------------------------------------------------------------------------------------------------------
BOOL MapMgr::CreateScriptInstance(DWORD dwInstanceID)
{
	if( !g_mapCreator.CanCreateInstance(m_pInfo, this) ) return FALSE;

	if( EMT_ScriptCreate != m_pInfo->eType ) return FALSE;

	MapInstance* pInstance = g_mapCreator.CreateAFMapInstance(m_pInfo);
	if( !P_VALID(pInstance) ) return FALSE;

	g_mapCreator.AddInstance(m_pInfo, this);

	if( FALSE == pInstance->Init(m_pInfo, dwInstanceID, NULL, GT_INVALID) )
	{
		DestroyInstance(pInstance);
		return FALSE;
	}

	m_mapInstance.Add(pInstance->GetInstanceID(), pInstance);

	return TRUE;
}

//---------------------------------------------------------------------------------------------------------
// ɾ������
//---------------------------------------------------------------------------------------------------------
VOID MapMgr::DestroyInstance(MapInstance* pInstance)
{
	if( !P_VALID(pInstance) ) return;

	pInstance->OnDestroy();

	m_mapInstance.Erase(pInstance->GetInstanceID());
	g_mapCreator.RemoveInstance(m_pInfo, this);
	g_mapCreator.DestroyAFMap(pInstance);
}

//---------------------------------------------------------------------------------------------------------
// �Ƿ��ܹ�����
//---------------------------------------------------------------------------------------------------------
Map* MapMgr::CanEnter(Role* pRole, DWORD dwMisc/* =GT_INVALID */)
{
	return m_pRestrict->CanEnter(pRole, dwMisc);
}

//---------------------------------------------------------------------------------------------------------
// ���ߺ��Ƿ��ܹ����룬���򷵻س��ڵ�ͼ������
//---------------------------------------------------------------------------------------------------------
Map* MapMgr::CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	return m_pRestrict->CanTakeOverWhenOnline(pRole, dwOutMapID, vOut);
}

