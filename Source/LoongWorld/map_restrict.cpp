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

#include "StdAfx.h"
#include "map_restrict.h"
#include "map.h"
#include "map_mgr.h"
#include "map_creator.h"
#include "role.h"
#include "script_mgr.h"

//-------------------------------------------------------------------------------------------------------
// CONSTS
//-------------------------------------------------------------------------------------------------------
//const INT MAX_STABLE_INSTANCE_NUM = 10;			// �ȶ���������������

//-------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------
MapRestrict::MapRestrict() : m_pMgr(NULL)
{

}

MapRestrict::~MapRestrict()
{
	//m_pScript = NULL;
}

VOID MapRestrict::Init(MapMgr* pMapMgr)
{
	ASSERT(P_VALID(pMapMgr));

	m_pMgr	= pMapMgr;
	m_pInfo	= pMapMgr->GetMapInfo();
}

//---------------------------------------------------------------------------------------------------------
// ��ͨ��ͼ
//---------------------------------------------------------------------------------------------------------
VOID MapRestrictNormal::Init(MapMgr* pMapMgr)
{
	MapRestrict::Init(pMapMgr);
}

Map* MapRestrictNormal::CanEnter(Role* pRole, DWORD dwMisc)
{
	ASSERT( P_VALID(pRole) );

	return m_pMgr->GetSingleMap();
}

Map* MapRestrictNormal::CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	ASSERT( P_VALID(pRole) );

	return m_pMgr->GetSingleMap();
}

BOOL MapRestrictNormal::GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	return FALSE;
}

//----------------------------------------------------------------------------------------------------------
// ������ͼ
//----------------------------------------------------------------------------------------------------------
VOID MapRestrictInstance::Init(MapMgr* pMapMgr)
{
	MapRestrict::Init(pMapMgr);
}

//----------------------------------------------------------------------------------------------------------
// ĳ�������Ƿ��ܹ�����
//----------------------------------------------------------------------------------------------------------
Map* MapRestrictInstance::CanEnter(Role* pRole, DWORD dwMisc)
{
	const tagMapInfo* pInfo = m_pMgr->GetMapInfo();
	if( !P_VALID(pInfo) || EMT_Instance != pInfo->eType ) return NULL;

	MapInstance* pInstance = NULL;

	// �����жϸ�����̬����
	INT nRet = CanEnterByInstanceInfo(pRole);

	if( E_Success == nRet )
	{
		// ���ҵ�����ʵ��
		DWORD dwInstanceID = pRole->GetOwnInstanceID();

		if( P_VALID(dwInstanceID) )
		{
			pInstance =  m_pMgr->GetInstance(dwInstanceID);
			if( !P_VALID(pInstance) )
			{
				// ����ʵ��������
				nRet = E_Instance_Not_Exit;
			}
			else
			{
				// ����ʵ�����ڣ����ʵ�������Ƿ���Խ���
				nRet = pInstance->CanEnter(pRole);
			}
		}
		else
		{
			// ��һ���鶼û�и������򴴽�һ��
			pInstance = m_pMgr->CreateInstance(pRole, dwMisc);
			if( !P_VALID(pInstance) )
			{
				nRet = E_Instance_Full;
			}
		}
	}

	if( E_Success != nRet )		// ���������������룬���͸������Ϣ
	{
		tagNS_EnterInstance send;
		send.dwTimeLimit = GT_INVALID;
		send.dwErrorCode = nRet;

		pRole->SendMessage(&send, send.dwSize);
		return NULL;
	}

	return pInstance;		// ����������
}

//-----------------------------------------------------------------------------------------------------------
// ���ݸ�����̬�����ж��Ƿ��ܹ�����
//-----------------------------------------------------------------------------------------------------------
INT MapRestrictInstance::CanEnterByInstanceInfo(Role* pRole)
{
	const tagInstance* pProto = g_attRes.GetInstanceProto(m_pInfo->dwID);
	if( !P_VALID(pProto) )
	{
		IMSG(_T("Can't find Instance Proto %u\r\n"), m_pInfo->dwID);
		return GT_INVALID;
	}

	// ��һ������ڶ����ǲ����Ѿ�����������
	DWORD dwOwnInstanceMapID = pRole->GetOwnInstanceMapID();
	if( P_VALID(dwOwnInstanceMapID) && dwOwnInstanceMapID != m_pInfo->dwID )
		return E_Instance_Already;

	// �ȼ�����
	if( pProto->nLevelDownLimit > pRole->GetLevel() )
		return E_Instance_Level_Down_Limit;

	if( pProto->nLevelUpLimit < pRole->GetLevel() )
		return E_Instance_Level_Up_Limit;

	// ��������
	if( pProto->nNumDownLimit > 1 )
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
		if( !P_VALID(pTeam) || pTeam->GetMemNum() < pProto->nNumDownLimit ) 
			return E_Instance_Role_Lack;
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------------------
// ������ߺ�Ľӹܴ���
//------------------------------------------------------------------------------------------------------------
Map* MapRestrictInstance::CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	// ����ڸ��������ߣ������ߵĻ�������ͣ���ڸ����ڣ���Ҫ����Ҵ���
	GetExportMapAndCoord(pRole, dwOutMapID, vOut);

	return NULL;
}

//------------------------------------------------------------------------------------------------------------
// �õ����ڵ�ͼ������
//------------------------------------------------------------------------------------------------------------
BOOL MapRestrictInstance::GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	const tagInstance* pProto = g_attRes.GetInstanceProto(m_pInfo->dwID);
	if( !P_VALID(pProto) )
	{
		IMSG(_T("Can't find Instance Proto %u\r\n"), m_pInfo->dwID);
		return FALSE;
	}

	switch( pProto->eExportMode )
	{
	case EEM_Born:			// ������
		{
			dwOutMapID = g_mapCreator.GetBornMapID();
			vOut = g_mapCreator.RandGetOneBornPos();

			return TRUE;
		}
		break;

	case EEM_Reborn:		// �����
	case EEM_Current:		// ��ǰ��
		{
			dwOutMapID = pRole->GetRebornMapID();
			const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwOutMapID);
			if( !P_VALID(pInfo) ) return FALSE;
			vOut = pInfo->vRebornPos;

			return TRUE;
		}
		break;

	case EEM_Appoint:		// ָ��
		{
			dwOutMapID = pProto->dwExportMapID;
			vOut = pProto->vExportPos;

			return TRUE;
		}
		break;

	default:
		break;
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------------------------------------------
VOID MapRestrictStable::Init(MapMgr* pMapMgr)
{
	MapRestrict::Init(pMapMgr);

	// �����ȶ�����
	INT nNum = g_mapCreator.GetStableInstanceNum();

	for(INT n = 0; n < nNum; ++n)
	{
		pMapMgr->CreateInstance(NULL, GT_INVALID);
	}
}

//-------------------------------------------------------------------------------------------------------------------
// �Ƿ���Խ���
//-------------------------------------------------------------------------------------------------------------------
Map* MapRestrictStable::CanEnter(Role* pRole, DWORD dwMisc)
{
	const tagMapInfo* pInfo = m_pMgr->GetMapInfo();
	if( !P_VALID(pInfo) || EMT_System != pInfo->eType ) return NULL;

	// ����һ�����ʵ�map���ϲ�
	MapInstance* pInstance = GetOnePerfectMap();

	return pInstance;
}

//------------------------------------------------------------------------------------------------------------
// ������ߺ�Ľӹܴ���
//------------------------------------------------------------------------------------------------------------
Map* MapRestrictStable::CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	// ���Խӹܣ��ҵ�һ���������ٵ�
	return GetOnePerfectMap();
}

//-------------------------------------------------------------------------------------------------------------
// �õ����ڵ�ͼ������
//-------------------------------------------------------------------------------------------------------------
BOOL MapRestrictStable::GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------
// �ҵ�һ������ʵĸ���ʵ��
//--------------------------------------------------------------------------------------------------------------------
MapInstance* MapRestrictStable::GetOnePerfectMap()
{
	MapInstance* pInstance = NULL;	// �������ٵĸ���
	INT nMinRoleNum = INT_MAX;		// ��������

	// ��mapmgr���ҵ�һ���������ٵĸ���
	MapInstance* pTemp = NULL;
	TMap<DWORD, MapInstance*>::TMapIterator it = m_pMgr->m_mapInstance.Begin();

	while( m_pMgr->m_mapInstance.PeekNext(it, pTemp) )
	{
		INT nNum = pTemp->GetRoleNum();
		if( nNum < nMinRoleNum )
		{
			nMinRoleNum = nNum;
			pInstance = pTemp;
		}
	}

	return pInstance;
}

//------------------------------------------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------------------------------------------
VOID MapRestrictScript::Init(MapMgr* pMapMgr)
{
	MapRestrict::Init(pMapMgr);

	// �ű�Ҳ��������г�ʼ��
	m_pScript = g_ScriptMgr.GetMapScript(pMapMgr->GetMapInfo()->dwID);
}

//-------------------------------------------------------------------------------------------------------------------
// �Ƿ���Խ���
//-------------------------------------------------------------------------------------------------------------------
Map* MapRestrictScript::CanEnter(Role* pRole, DWORD dwMisc)
{
	const tagMapInfo* pInfo = m_pMgr->GetMapInfo();
	if( !P_VALID(pInfo) || EMT_ScriptCreate != pInfo->eType ) return NULL;

	MapInstance* pInstance = NULL;

	if ( P_VALID(m_pScript) )
	{
		if(m_pScript->CanEnter(pRole))
		{
			// ����һ�����ʵ�map���ϲ�
			pInstance = GetOnePerfectMap(pRole);
		}
	}

	return pInstance;
}

//------------------------------------------------------------------------------------------------------------
// ������ߺ�Ľӹܴ���
//------------------------------------------------------------------------------------------------------------
Map* MapRestrictScript::CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	// ���ýű�
	if( P_VALID(m_pScript) )
	{
		 m_pScript->CanTakeOverWhenOnline(pRole, dwOutMapID, vOut);
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------------------------
// �õ����ڵ�ͼ������
//-------------------------------------------------------------------------------------------------------------
BOOL MapRestrictScript::GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut)
{
	if( P_VALID(m_pScript) )
	{
		BOOL  bSuccess = FALSE; 
		bSuccess =  m_pScript->GetExportMapAndCoord(pRole, dwOutMapID, vOut);

		return bSuccess;
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------
// �ҵ�һ������ʵĸ���ʵ��
//--------------------------------------------------------------------------------------------------------------------
MapInstance* MapRestrictScript::GetOnePerfectMap(Role* pRole)
{
	DWORD  dwInstanceID = GT_INVALID;
	if( P_VALID(m_pScript) )
	{
		m_pScript->GetOnePerfectMap(pRole, dwInstanceID);
	}

	return m_pMgr->GetInstance(dwInstanceID);
}