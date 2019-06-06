//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_instance.cpp
// author: wjiang
// actor:
// data: 2008-1-15
// last:
// brief: ��ͨ������ͼ
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_map.h"
#include "../WorldDefine/MapAttDefine.h"

#include "map_instance.h"
#include "map_creator.h"
#include "team.h"
#include "group_mgr.h"
#include "att_res.h"
#include "role.h"
#include "role_mgr.h"
#include "creature.h"
#include "map_mgr.h"
#include "NPCTeam.h"
#include "NPCTeam_mgr.h"


MapInstanceNormal::MapInstanceNormal() : MapInstance(), m_pInstance(NULL), m_dwCreatorID(GT_INVALID),
										 m_dwTeamID(GT_INVALID), m_dwStartTick(GT_INVALID), m_dwEndTick(GT_INVALID),
										 m_eInstanceHardMode(EIHM_NULL), m_bNoEnter(TRUE)
{
}

MapInstanceNormal::~MapInstanceNormal()
{
	Destroy();
}

//------------------------------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------------------------------
BOOL MapInstanceNormal::Init(const tagMapInfo* pInfo, DWORD dwInstanceID, Role* pCreator, DWORD dwMisc)
{
	ASSERT( P_VALID(pInfo) );
	ASSERT( EMT_Instance == pInfo->eType );

	if( !P_VALID(pCreator) ) return FALSE;	// ����Ҫ�д����ߵ�

	// ��ȡ������̬����
	m_pInstance = g_attRes.GetInstanceProto(pInfo->dwID);
	if( !P_VALID(m_pInstance) )	return FALSE;

	// ��ͼ�������
	m_pUtil = "Util";
	m_pInfo = pInfo;
	m_mapSession.Clear();
	m_mapRole.Clear();
	m_mapShop.Clear();
	m_mapCofC.Clear();
	m_mapGroundItem.Clear();

	// �����������
	m_dwID = m_pInfo->dwID;
	m_dwInstanceID = dwInstanceID;
	m_dwCreatorID = pCreator->GetID();
	m_eInstanceHardMode = (EInstanceHardMode)dwMisc;
	m_dwStartTick = g_world.GetWorldTick();
	m_dwEndTick = GT_INVALID;
	m_dwTeamID = pCreator->GetTeamID();

	m_pTeamMgr = new NPCTeamMgr(this);
	if(!P_VALID(m_pTeamMgr))
		return FALSE;

	// ���뵼��ͼ���ݣ�Clone������
	NavMap* pNavMap = g_mapCreator.GetInstanceTemplate(pInfo->dwID);
	if( !P_VALID(pNavMap) ) return FALSE;
	m_pNav = new NavMap;
	pNavMap->Clone(m_pNav);
	if( !P_VALID(m_pNav) ) return FALSE;

	// ����mapinfo����ʼ����ͼ�Ĺ�������б����Ϣ
	if( FALSE == InitLogicalInfo() )
	{
		SAFE_DEL(m_pNav);
		return FALSE;
	}

	// �������ɹ��ˣ�������һ������ڶ������������ID
	if( P_VALID(m_dwTeamID) )
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(m_dwTeamID);
		if( P_VALID(pTeam) )
		{
			pTeam->SetOwnInstanceMapID(m_pInfo->dwID);
			pTeam->SetOwnInstanceID(m_dwInstanceID);
		}
	}
	else
	{
		pCreator->SetMyOwnInstanceMapID(m_pInfo->dwID);
		pCreator->SetMyOwnInstanceID(m_dwInstanceID);
	}

	return TRUE;		
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID MapInstanceNormal::Update()
{
	Map::Update();
	UpdateTimedIssue();
}

//---------------------------------------------------------------------------------
// ʱ����صĸ���
//---------------------------------------------------------------------------------
VOID MapInstanceNormal::UpdateTimedIssue()
{
	CHECK_TIME();

	// ����Ѿ����ڴ�ɾ��״̬���Ͳ�������
	if( IsDelete() ) return;

	// ʱ�޸���
	if( IsTimeLimit() && !IsEnd() )
	{
		DWORD dwTick = g_world.GetWorldTick();
		if( (dwTick - m_dwStartTick) >= m_pInstance->dwTimeLimit * TICK_PER_SECOND )
		{
			m_dwEndTick = g_world.GetWorldTick();
			SetEnd();
		}
	}

	// �رյ���ʱ
	if( IsEnd() )
	{
		DWORD dwTick = g_world.GetWorldTick();
		if( (dwTick - m_dwEndTick) > m_pInstance->dwEndTime * TICK_PER_SECOND )
		{
			SetDelete();
		}
	}

	// �������д��˳��Ľ�ɫ��ʱ��
	if( !m_mapWillOutRoleID.Empty() )
	{
		TMap<DWORD, INT>::TMapIterator it = m_mapWillOutRoleID.Begin();
		DWORD dwRoleID = GT_INVALID;
		INT nTick = GT_INVALID;

		while( m_mapWillOutRoleID.PeekNext(it, dwRoleID, nTick) )
		{
			--nTick;	// ��һ�µ���ʱ
			if( nTick <= 0 )
			{
				// ʱ�䵽�ˣ�����Ҵ��ͳ�ȥ
				Role* pRole = FindRole(dwRoleID);
				if( P_VALID(pRole) )
				{
					MapMgr* pMapMgr = g_mapCreator.GetMapMgr(m_pInfo->dwID);
					if( P_VALID(pMapMgr) )
					{
						pMapMgr->RoleInstanceOut(pRole);
						// Jason 2009-11-26 ���ӳ����������һ���ˣ�������ʱ����ʱ��ɾ����������ɾ����ǣ���
						// û�п���ʱ��
						if( m_dwCreatorID == pRole->GetID  () )
							SetDelete();
					}
				}
				
				m_mapWillOutRoleID.Erase(dwRoleID);
			}
			else
			{
				m_mapWillOutRoleID.ChangeValue(dwRoleID, nTick);
			}
		}
	}
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID MapInstanceNormal::Destroy()
{

}

//---------------------------------------------------------------------------------
// ��ʽ����һ����ң���ֻ���ɹ���õ�ͼ��MapMgr����
//---------------------------------------------------------------------------------
VOID MapInstanceNormal::AddRole(Role* pRole)
{
	Map::AddRole(pRole);

	// ���ùرյȴ�
	if( IsEnd() )
	{
		m_dwEndTick = GT_INVALID;
		m_bEnd = FALSE;
	}

	// ����ǵ�һ�����븱�������
	if( m_bNoEnter )
	{
		m_bNoEnter = FALSE;

		// ����ж��飬����֪ͨ����ȷ��
		if( GT_INVALID != pRole->GetTeamID() && m_pInstance->bNoticeTeamate )
		{
			tagNS_InstanceNofity send;
			pRole->SendMessage(&send, send.dwSize);
		}
	}
	
	// ���ͽ��븱����Ϣ
	tagNS_EnterInstance send;
	send.dwErrorCode = E_Success;
	send.dwTimeLimit = CalTimeLimit();
	pRole->SendMessage(&send, send.dwSize);
}

//---------------------------------------------------------------------------------
// ����뿪��ͼ��ֻ���������߳��������
//---------------------------------------------------------------------------------
VOID MapInstanceNormal::RoleLeaveMap(DWORD dwID, BOOL bChgMap)
{
	Map::RoleLeaveMap(dwID, bChgMap);

	// �Ƿ����ȴ��ر�
	if( m_mapRole.Empty() && !IsEnd() && m_pInstance->dwEndTime != GT_INVALID )
	{
		m_dwEndTick = g_world.GetWorldTick();
		m_bEnd = TRUE;
	}

	// ����������ڵȴ��뿪���б�����Ƴ�
	m_mapWillOutRoleID.Erase(dwID);
}
//---------------------------------------------------------------------------------
// �Ƿ��ܽ��븱��
//---------------------------------------------------------------------------------
INT MapInstanceNormal::CanEnter(Role *pRole)
{
	// �ȼ��ͨ���ж�
	INT nErrorCode = MapInstance::CanEnter(pRole);
	if( E_Success != nErrorCode ) return nErrorCode;

	// �����������
	if( m_pInstance->nNumUpLimit <= GetRoleNum() )
		return E_Instance_Role_Full;

	// ������
	if( m_dwTeamID != GT_INVALID )
	{
		if( pRole->GetTeamID() != m_dwTeamID )
			return E_Instance_Not_Same_Team;
	}
	// ������
	else
	{
		if( pRole->GetID() != m_dwCreatorID )
			return E_Instance_Not_Same_Team;
	}

	return E_Success;
}

//---------------------------------------------------------------------------------
// �Ƿ����ɾ��
//---------------------------------------------------------------------------------
BOOL MapInstanceNormal::CanDestroy()
{
	return MapInstance::CanDestroy();
}


//---------------------------------------------------------------------------------
// ��ʼ��ˢ�ֵ����
//---------------------------------------------------------------------------------
BOOL MapInstanceNormal::InitAllSpawnPointCreature()
{
	if( EICM_Appoint == m_pInstance->eInstanceCreateMode )
		return TRUE;

	if( FALSE == RecalHardMode() ) return FALSE;

	DWORD		dwTypeID = GT_INVALID;
	INT			nBaseLevel = 0;
	INT			nLevel = 0;
	Vector3		vec3;

	// ���������������ȼ�
	if( FALSE == GetCreatureBaseLevel(nBaseLevel) )
		return FALSE;

	tagMapSpawnPointInfo* pMapSpawnInfo = NULL;
	const tagRandSpawnPointInfo* pSpawnProto = NULL;
	const tagCreatureProto* pProto = NULL;

	m_pInfo->mapSpawnPoint.ResetIterator();
	while( m_pInfo->mapSpawnPoint.PeekNext(pMapSpawnInfo) )
	{
		DWORD dwSpawnPoint = TransmitBigID(nBaseLevel, pMapSpawnInfo);

		pSpawnProto = g_attRes.GetSpawnPointProto(dwSpawnPoint);
		if( !P_VALID(pSpawnProto) )
		{
			IMSG(_T("Detect a unknown Spawn Point in map, dwSpawnPointID=%u\r\n"), pSpawnProto->dwSpawnPointID);
			continue;
		}

		dwTypeID = CalCreatureTypeID(pSpawnProto);
		if( !P_VALID(dwTypeID) )
		{
			IMSG(_T("Detect a unknown Instance Creature TypeID\n"));
			continue;
		}

		pProto = g_attRes.GetCreatureProto(dwTypeID);
		if( !P_VALID(pProto) )
		{
			IMSG(_T("Detect a unknown creature in map, dwObjID=%u\r\n"), pSpawnProto->dwSpawnPointID);
			continue;
		}

		// ȡ��һ��ID
		DWORD dwID = m_CreatureIDGen.GetNewCreatureID();
		ASSERT( IS_CREATURE(dwID) );

		// ���һ������
		Vector3 vFaceTo;
		FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
		vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
		vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
		vFaceTo.y = 0.0f;

		// ���ɹ���
		Creature* pCreature = Creature::Create(dwID, m_dwID, pProto, pMapSpawnInfo->vPos, vFaceTo, ECST_Load, GT_INVALID, pMapSpawnInfo->bCollide, NULL);

		// ���뵽��ͼ��
		AddCreatureOnLoad(pCreature);

		// ����ǳ�Ѩ������س�Ѩ����
		if( pCreature->IsNest() )
		{
			InitNestCreature(pCreature);
		}

		// ����ǹ���С�ӣ������С�ӹ���
		if( pCreature->IsTeam() )
		{
			InitTeamCreature(pCreature);
		}
	}

	return TRUE;


}

//---------------------------------------------------------------------------------
// ���¼��㸱���Ѷ�
//---------------------------------------------------------------------------------
BOOL MapInstanceNormal::RecalHardMode()
{
	// ����ѡ�����Ѷ�
	if( !m_pInstance->bSelectHard )
	{
		m_eInstanceHardMode = EIHM_Normal;
		return TRUE;
	}
	 // ��ѡ
	else
	{
		switch(m_eInstanceHardMode)
		{
			// ��ͨ
		case EIHM_Normal:
			{
				if( !m_pInstance->bSelectNormal ) return FALSE;
				else return TRUE;
			}
			break;

			// ��Ӣ
		case EIHM_Elite:
			{
				if( !m_pInstance->bSelectElite ) return FALSE;
				else return TRUE;
			}
			break;

			// ħ��
		case EIHM_Devil:
			{
				if( !m_pInstance->bSelectDevil ) return FALSE;
				else return TRUE;
			}
			break;

		default:
			return FALSE;
			break;
		}

	}
}

//---------------------------------------------------------------------------------
// ���ݸ������ɹ��򣬵õ��������Ļ����ȼ�
//---------------------------------------------------------------------------------
BOOL MapInstanceNormal::GetCreatureBaseLevel(INT& nBaseLevel)
{
	if( m_dwTeamID != GT_INVALID && m_pInstance->nNumUpLimit > 1 )
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(m_dwTeamID);
		if( !P_VALID(pTeam) ) return FALSE;

		switch( m_pInstance->eInstanceCreateMode )
		{
			// ƽ���ȼ�
		case EICM_AvgLevel:
			{
				nBaseLevel = pTeam->GetTeamAvgLevel();
				return TRUE;
			}
			break;

			// �ӳ��ȼ�
		case EICM_LeaderLevel:
			{
				Role* pLeader = pTeam->GetMemPtr(0);
				if( !P_VALID(pLeader) )
					return FALSE;
				else nBaseLevel = pLeader->GetLevel();
				return TRUE;
			}
			break;

			// ������ߵȼ�
		case EICM_MaxLevel:
			{
				nBaseLevel = pTeam->GetTeamMaxLevel();
				return TRUE;
			}
			break;

			// ������͵ȼ�
		case EICM_MinLevel:
			{
				nBaseLevel = pTeam->GetTeamMinLevel();
				return TRUE;
			}
			break;

		default:
			return FALSE;
			break;
		}
	}
	else
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(m_dwCreatorID);
		if( !P_VALID(pRole) )
			return FALSE;

		nBaseLevel = pRole->GetLevel();
		return TRUE;
	}
}


//---------------------------------------------------------------------------------
// ���ݹ�������ȼ��õ���������TypeID
//---------------------------------------------------------------------------------
DWORD MapInstanceNormal::CalCreatureTypeID(const tagRandSpawnPointInfo* pRandSpawnPoint)
{	
	INT nIndex = IUTIL->Rand() % RAND_CTEATUTE_NUM;
	
	switch(m_eInstanceHardMode)
	{
	case EIHM_Normal:
		return pRandSpawnPoint->dwNormalID[nIndex];
	case EIHM_Elite:
		return pRandSpawnPoint->dwEliteID[nIndex];
	case EIHM_Devil:
		return pRandSpawnPoint->dwDevilID[nIndex];
	default:
		return GT_INVALID;
	}
}




//---------------------------------------------------------------------------------
// ���ݸ��������ȼ���ˢ�ֵ�ĵȼ���������ˢ�ֵ�СID������ˢ�ֵ��ID
//---------------------------------------------------------------------------------
DWORD MapInstanceNormal::TransmitBigID(INT nBaseLevel, tagMapSpawnPointInfo* pMapSpawnInfo)
{
	if(!P_VALID(pMapSpawnInfo))
		return GT_INVALID;

	const tagLevelMapping *pLevelMapping = g_attRes.GetLevelMapping(nBaseLevel + pMapSpawnInfo->nLevelInc);
	return pMapSpawnInfo->dwSpawnPointID + (DWORD)pLevelMapping->nTransmitLevel;
}

//---------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------
VOID MapInstanceNormal::OnDelete()
{
	// �Ƴ������ڵ�ͼ�ڵ����
	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(m_dwID);
	if( !P_VALID(pMapMgr) ) return;

	Role* pRole = (Role*)GT_INVALID;

	RoleMap::TMapIterator it = m_mapRole.Begin();
	while( m_mapRole.PeekNext(it, pRole) )
	{
		pMapMgr->RoleInstanceOut(pRole);
	}
}

//-----------------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------------
VOID MapInstanceNormal::OnDestroy()
{
	// ���һ�¶���ĸ���ID
	if( P_VALID(m_dwTeamID) )
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(m_dwTeamID);
		if( P_VALID(pTeam) )
		{
			pTeam->SetOwnInstanceMapID(GT_INVALID);
			pTeam->SetOwnInstanceID(GT_INVALID);
		}
	}
	// ���һ����ҵĸ���ID
	else
	{
		Role* pCreator = g_roleMgr.GetRolePtrByID(m_dwCreatorID);
		if( P_VALID(pCreator) )
		{
			pCreator->SetMyOwnInstanceMapID(GT_INVALID);
			pCreator->SetMyOwnInstanceID(GT_INVALID);
		}
	}

}

//-----------------------------------------------------------------------------------
// ����ʱ�޸�����ʣʱ��
//-----------------------------------------------------------------------------------
DWORD MapInstanceNormal::CalTimeLimit()
{
	DWORD dwTimeLeft = GT_INVALID;
	if(m_pInstance->dwTimeLimit > 0 && m_pInstance->dwTimeLimit != GT_INVALID)
	{
		DWORD dwCurrentTick = g_world.GetWorldTick();
		DWORD dwTimePass = (dwCurrentTick - m_dwStartTick) / TICK_PER_SECOND;
		dwTimeLeft = m_pInstance->dwTimeLimit - dwTimePass;
	}

	return dwTimeLeft;
}

//---------------------------------------------------------------------------------------------------
// �����鴴��
//---------------------------------------------------------------------------------------------------
VOID MapInstanceNormal::OnTeamCreate(const Team* pTeam)
{
	if( !P_VALID(pTeam) ) return;

	m_dwTeamID = pTeam->GetTeamID();
}

//----------------------------------------------------------------------------------------------------
// ������ɾ��
//----------------------------------------------------------------------------------------------------
VOID MapInstanceNormal::OnTeamDelete(const Team* pTeam)
{
	if( m_dwTeamID != pTeam->GetTeamID() ) return;
	if( pTeam->GetMemNum() != 1 ) return;
	if( pTeam->GetMemID(0) != m_dwCreatorID ) return;

	m_dwTeamID = GT_INVALID;

	// �ҵ�������Ψһһ����ҵ����ָ�룬����Ϊ���˸���
	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwCreatorID);
	if( P_VALID(pRole) )
	{
		pRole->SetMyOwnInstanceID(m_dwInstanceID);
		pRole->SetMyOwnInstanceMapID(m_dwID);
		// Jason 2009-11-26 �����鵹���ڶ����˶�ʱ��Ҳ�����鼴����ɾ��ʱ��
		// �ı�ԭ���������������˸�������������Ҳ����ʱ�˳�
		m_mapWillOutRoleID.Add(m_dwCreatorID, ROLE_LEAVE_INSTANCE_TICK_COUNT_DOWN);
	}
}

//-----------------------------------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------------------------------
VOID MapInstanceNormal::OnRoleLeaveTeam(DWORD dwRoleID, const Team* pTeam)
{
	if( m_dwTeamID != pTeam->GetTeamID() ) return;

	// ���������Ǹ����Ĵ����ߣ��򽫴������ƽ�����ǰ����Ķӳ�
	if( dwRoleID == m_dwCreatorID )
	{
		ASSERT( pTeam->GetMemNum() > 0 );
		m_dwCreatorID = pTeam->GetMemID(0);
	}

	// ��������Ŀǰ�ڸ������������ҵ��뿪��������ʱ
	Role* pRole = FindRole(dwRoleID);
	if( P_VALID(pRole) )
	{
		m_mapWillOutRoleID.Add(dwRoleID, ROLE_LEAVE_INSTANCE_TICK_COUNT_DOWN);
	}
}

//------------------------------------------------------------------------------------------------------
// ���������
//------------------------------------------------------------------------------------------------------
VOID MapInstanceNormal::OnRoleEnterTeam(DWORD dwRoleID, const Team* pTeam)
{
	if( m_dwTeamID != pTeam->GetTeamID() ) return;

	// ��������Ŀǰ�ڸ�����������ҵ��뿪��������ʱ
	Role* pRole = FindRole(dwRoleID);
	if( P_VALID(pRole) )
	{
		m_mapWillOutRoleID.Erase(dwRoleID);
	}
}