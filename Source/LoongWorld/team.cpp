//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: team.cpp
// author: wjiang
// actor:
// data: 2008-12-10
// last:
// brief: ������
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_group.h"

#include "team.h"
#include "role.h"
#include "role_mgr.h"
#include "creature.h"
#include "map_creator.h"
#include "map.h"
#include "map_instance.h"

// Jason 2009-11-29
#include "../WorldDefine/msg_show_item.h"
#include "../WorldDefine/chat_define.h"

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
Team::Team(DWORD dwTeamID, Role* pLeaderPtr, Role* pFirstMemPtr)
: m_dwTeamID(dwTeamID), m_dwGroupID(GT_INVALID), m_dwOwnInstanceID(GT_INVALID), m_dwOwnMapID(GT_INVALID),
  m_nMemNum(2), m_ePickMode(EPUM_Order), m_fExpFactor(0.0f), m_bNeedDelete(FALSE)
{
	m_dwTeamSynTick	=	g_world.GetWorldTick();

	// ��ʼ����Ա����
	memset(m_dwMemID, 0xFF, sizeof(m_dwMemID));
	memset(m_pMemPtr, 0xFF, sizeof(m_pMemPtr));

	// ���ö�Ա
	m_dwMemID[0]	= pLeaderPtr->GetID();
	m_pMemPtr[0]	= pLeaderPtr;
	m_dwMemID[1]	= pFirstMemPtr->GetID();
	m_pMemPtr[1]	= pFirstMemPtr;

	// ����������Ա�Ķ���ID
	pLeaderPtr->SetTeamID(dwTeamID);
	pFirstMemPtr->SetTeamID(dwTeamID);

	// ��ʼ��ʰȡ�б�
	m_nPickList.push_back(0);
	m_nPickList.push_back(1);
	
	// ������ߵȼ�����͵ȼ�
	m_nMaxLevel = max(pLeaderPtr->GetLevel(), pFirstMemPtr->GetLevel());
	m_nMinLevel = min(pLeaderPtr->GetLevel(), pFirstMemPtr->GetLevel());

	// ������龭�����
	CalExpFactor();
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
Team::~Team()
{
	for(INT i = 0; i < MAX_TEAM_NUM; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) )
			m_pMemPtr[i]->SetTeamID(GT_INVALID);
		else
			break;
	}

	m_nPickList.clear();
}

//-------------------------------------------------------------------------------------------------------
// ���Ͷ�����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamMessage(LPVOID pMsg, DWORD dwSize)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( !P_VALID(m_pMemPtr[i]) )
			break;
		m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// �����еĶ�ԱID����
//-------------------------------------------------------------------------------------------------------
VOID Team::ExportAllMemID(DWORD dwMemID[MAX_TEAM_NUM]) const
{
	IFASTCODE->MemCpy(dwMemID, m_dwMemID, sizeof(DWORD)*MAX_TEAM_NUM);
}

//-------------------------------------------------------------------------------------------------------
// ������Ϣ������
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamateMessage(DWORD dwRoleID, LPVOID pMsg, DWORD dwSize)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) || m_dwMemID[i] != dwRoleID )
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ������Ϣ��ͬ��ͼ�Ķ���
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamMesInSameMap(DWORD dwMapID, LPVOID pMsg, DWORD dwSize)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && dwMapID == m_pMemPtr[i]->GetMapID())
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// �������������С�ӳ�Ա����Ϣ(������Լ���)
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamMesOutBigVis(Role* pRole, LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pRole) ) return;

	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && pRole != m_pMemPtr[i] 
			&& (pRole->GetMapID() != m_pMemPtr[i]->GetMapID() || !pRole->GetMap()->InSameBigVisTile(pRole, m_pMemPtr[i])) )
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ֪ͨ���ѽ��븱��
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamInstanceNotigy(Role* pRole, LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pRole) ) return;

	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && pRole != m_pMemPtr[i] 
		&& (pRole->GetMapID() != m_pMemPtr[i]->GetMapID() || !pRole->GetMap()->InSameBigVisTile(pRole, m_pMemPtr[i])) 
		&& !m_pMemPtr[i]->IsInRoleState(ERS_PrisonArea) 
		// Jason 2009-11-16 ����״̬�Ķ��ѣ���֪ͨ���븱��
		/*&& !m_pMemPtr[i]->IsInState(ES_Dead)*/ )
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// �õ�С�ӵ�ƽ���ȼ�
//-------------------------------------------------------------------------------------------------------
INT Team::GetTeamAvgLevel() const
{
	INT nTotalLevel = 0;
	for(INT i = 1; i < m_nMemNum; ++i)
	{
		if( !P_VALID(m_pMemPtr[i]) )
			break;
		nTotalLevel += m_pMemPtr[i]->GetLevel();
	}

	return nTotalLevel / m_nMemNum;
}

//-------------------------------------------------------------------------------------------------------
// ���Ӷ���
//-------------------------------------------------------------------------------------------------------
INT Team::AddMember(Role* pInviter, Role* pReplyer)
{
	INT nRet = CanAddMember(pInviter, pReplyer);

	if( E_Success != nRet ) return nRet;

	pReplyer->SetTeamID(m_dwTeamID);	// ���ö���ID

	// ���ö����Ա
	m_dwMemID[m_nMemNum] = pReplyer->GetID();
	m_pMemPtr[m_nMemNum] = pReplyer;

	// ���ö�����ߵȼ�����͵ȼ�
	m_nMaxLevel = max(m_nMaxLevel, pReplyer->GetLevel());
	m_nMinLevel = min(m_nMinLevel, pReplyer->GetLevel());

	// ���ö��龭�����
	CalExpFactor();

	// �ۼӶ�������
	++m_nMemNum;

	// ����ʰȡ
	m_nPickList.push_back(m_nMemNum - 1);

	// ��������¼�
	OnAddMember(pReplyer);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ƿ��ܼ���С��
//-------------------------------------------------------------------------------------------------------
INT Team::CanAddMember(Role* pInviter, Role* pReplyer)
{
	// �����߲��Ƕӳ�
	if( !IsLeader(pInviter->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// ���������Ѿ��ж�
	if( GT_INVALID != pReplyer->GetTeamID() )
	{
		return E_Team_Target_Have_Team;
	}

	// ��������
	if( m_nMemNum >= MAX_TEAM_NUM )
	{
		return E_Team_Member_Full;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ߵ�����
//-------------------------------------------------------------------------------------------------------
INT Team::KickMember(Role* pSrc, Role* pDest)
{
	if( !P_VALID(pSrc) || !P_VALID(pDest) ) return GT_INVALID;

	INT nIndex = GT_INVALID;

	INT nRet = CanKickMember(pSrc, pDest, nIndex);

	if( E_Success != nRet ) return nRet;
	
	Map* pMap = pDest->GetMap();
	if ( P_VALID(pMap) )
	{
		nRet = pMap->CanKickMember();
		if( E_Success != nRet ) return nRet;
	}
	
	// �����ߵ����ѣ����ߵ�
	DeleteMem(nIndex);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ƿ�������ˣ���������ߣ����ز����д洢�����ߵ�����
//-------------------------------------------------------------------------------------------------------
INT Team::CanKickMember(Role* pSrc, Role* pDest, INT& nIndex)
{
	// ���Ƕӳ���������
	if( !IsLeader(pSrc->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// �ǲ����Լ�
	if( pSrc->GetID() == pDest->GetID() )
	{
		return E_Team_Target_Not_Exit;
	}

	// ��鱻�����ǲ����ڶ�����
	nIndex = IsRoleInTeam(pDest->GetID());
	if( GT_INVALID == nIndex || 0 == nIndex )	// �����ߵ��ӳ�
	{
		return E_Team_Role_Not_In_Team;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �뿪С��
//-------------------------------------------------------------------------------------------------------
INT Team::LeaveTeam(Role* pRole)
{
	INT nIndex = GT_INVALID;

	INT nRet = CanLeaveTeam(pRole, nIndex);

	if( E_Success != nRet ) return nRet;

	// �ж���ĳ��ͼ�Ƿ���������뿪
	Map* pMap = pRole->GetMap();
	if ( P_VALID(pMap) )
	{
		nRet = pMap->CanLeaveTeam();
		if( E_Success != nRet ) return nRet;
	}

	// ����ɾ��
	DeleteMem(nIndex);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ƿ�����뿪С��
//-------------------------------------------------------------------------------------------------------
INT Team::CanLeaveTeam(Role* pRole, INT& nIndex)
{
	// �������ǲ����ڶ�����
	nIndex = IsRoleInTeam(pRole->GetID());
	if( GT_INVALID == nIndex )
	{
		return E_Team_Role_Not_In_Team;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ɾ��ĳ��������С�ӳ�Ա
//-------------------------------------------------------------------------------------------------------
VOID Team::DeleteMem(const INT nIndex)
{
	if( nIndex < 0 || nIndex >= m_nMemNum ) return;

	// �õ���С�ӳ�Ա
	Role* pMem = m_pMemPtr[nIndex];
	if( !P_VALID(pMem) ) return;

	// ���øó�Ա��С��ID
	pMem->SetTeamID(GT_INVALID);

	// ���ø�С�ӳ�Ա�Ľṹ
	for(INT n = nIndex; n < m_nMemNum - 1; ++n)
	{
		m_dwMemID[n] = m_dwMemID[n+1];
		m_pMemPtr[n] = m_pMemPtr[n+1];
	}
	m_dwMemID[m_nMemNum - 1] = GT_INVALID;
	m_pMemPtr[m_nMemNum - 1] = (Role*)GT_INVALID;
	--m_nMemNum;

	// ����ʰȡ˳��
	m_nPickList.remove(nIndex);
	list<INT>::iterator it = m_nPickList.begin();
	while( it != m_nPickList.end() )
	{
		if(*it > nIndex)
			(*it)--;
		++it;
	}

	// ���¼���С�����,��͵ȼ�
	ReCalTeamLevel();

	// ������龭�����
	CalExpFactor();

	// �뿪�����¼�
	OnDelMember(pMem);

	// �����������С�ڵ���1�������ö���ɾ��
	if( m_nMemNum <= 1 )
	{
		m_bNeedDelete = TRUE;
	}
}

//-------------------------------------------------------------------------------------------------------
// ����С�ӵ�ʰȡģʽ
//-------------------------------------------------------------------------------------------------------
INT Team::SetPickMode(Role* pRole, EPickMode ePickMode)
{
	// ���Ƕӳ���������
	if( !IsLeader(pRole->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// �ж�ʰȡģʽ�Ƿ�Ƿ�
	if( ePickMode != EPUM_Free && ePickMode != EPUM_Order )
	{
		return E_Team_Pick_Model_Not_Exit;
	}

	// �жϣ��ɹ�������ʰȡģʽ
	m_ePickMode = ePickMode;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ı�ӳ�
//-------------------------------------------------------------------------------------------------------
INT Team::ChangeLeader(Role* pSrc, Role* pDest)
{
	// ���Ƕӳ������ܸı�ӳ�
	if( !IsLeader(pSrc->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// �鿴�ǲ����Լ�
	if( pSrc->GetID() == pDest->GetID() )
	{
		return E_Team_Target_Not_Exit;
	}

	// �鿴����ǲ����ڶ�����
	INT nIndex = IsRoleInTeam(pDest->GetID());
	if( GT_INVALID == nIndex || 0 == nIndex )
	{
		return E_Team_Role_Not_In_Team;
	}

	// �ж���ĳ��ͼ�Ƿ������ƽ��ӳ�
	Map* pMap = pSrc->GetMap();
	if ( P_VALID(pMap) )
	{
		INT nRet = E_Success;
		nRet = pMap->CanChangeLeader();
		if( E_Success != nRet ) return nRet;
	}

	// �жϳɹ������������˵�λ��
	swap(m_dwMemID[0], m_dwMemID[nIndex]);
	swap(m_pMemPtr[0], m_pMemPtr[nIndex]);

	// ����ʰȡ˳��
	list<INT>::iterator it = m_nPickList.begin();
	while (it != m_nPickList.end())
	{
		if(*it == nIndex)
		{
			*it = 0;
		}
		else if(*it == 0)	
		{
			*it = nIndex;
		}

		++it;
	}

	// ������������ľ������
	CalExpFactor();

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �����Ա�ĵȼ��ı�
//-------------------------------------------------------------------------------------------------------
VOID Team::OnMemLevelChange(Role* pRole)
{
	INT nIndex = IsRoleInTeam(pRole->GetID());

	if( GT_INVALID == nIndex ) return;

	m_nMaxLevel = max(m_nMaxLevel, pRole->GetLevel());
	m_nMinLevel = min(m_nMinLevel, pRole->GetLevel());
}

//-------------------------------------------------------------------------------------------------------
// �ӳ�ͳ��ֵ�ı�
//-------------------------------------------------------------------------------------------------------
VOID Team::OnLeaderReinChange(Role* pRole)
{
	if( !IsLeader(pRole->GetID()) ) return;

	// ����ͳ��ֵ
	CalExpFactor();
}

//-------------------------------------------------------------------------------------------------------
// ���¼����������ͬ��������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID Team::SendRoleInitStateToTeam(Role* pNewMem)
{
	if( !P_VALID(pNewMem) ) return;

	for(INT n = 0; n < m_nMemNum; ++n)
	{
		if( !P_VALID(m_pMemPtr[n]) ) continue;

		// ������Լ�������
		if( m_pMemPtr[n] == pNewMem ) continue;

		// ���¶�Ա����Ϣ���͸��϶�Ա
		tagNS_RoleStateToTeam sendToOld;

		sendToOld.dwRoleID	=	pNewMem->GetID();
		sendToOld.dwMapID	=	pNewMem->GetMapID();
		sendToOld.eClassEx	=	pNewMem->GetClassEx();
		sendToOld.bySex		=	pNewMem-> GetSex();
		sendToOld.nLevel	=	pNewMem->GetLevel();
		sendToOld.nMaxHP	=	pNewMem->GetAttValue(ERA_MaxHP);
		sendToOld.nHP		=	pNewMem->GetAttValue(ERA_HP);
		sendToOld.nMaxMP	=	pNewMem->GetAttValue(ERA_MaxMP);
		sendToOld.nMP		=	pNewMem->GetAttValue(ERA_MP);
		sendToOld.fX		=	pNewMem->GetCurPos().x;
		sendToOld.fZ		=	pNewMem->GetCurPos().z;
		for(INT i = 0; i < TEAM_DISPLAY_NUM; ++i)
		{
			sendToOld.dwEquipTypeID[i] = pNewMem->GetAvatarEquip().AvatarEquip[i].dwTypeID;
		}
		memcpy(&sendToOld.AvatarAtt, pNewMem->GetAvatar(), sizeof(tagAvatarAtt));

		m_pMemPtr[n]->SendMessage(&sendToOld, sizeof(sendToOld));

		// ���϶�Ա����Ϣ���͸��¶�Ա
		tagNS_RoleStateToTeam sendToNew;

		sendToNew.dwRoleID	=	m_pMemPtr[n]->GetID();
		sendToNew.dwMapID	=	m_pMemPtr[n]->GetMapID();
		sendToNew.eClassEx	=	m_pMemPtr[n]->GetClassEx();
		sendToNew.bySex		=	m_pMemPtr[n]-> GetSex();
		sendToNew.nLevel	=	m_pMemPtr[n]->GetLevel();
		sendToNew.nMaxHP	=	m_pMemPtr[n]->GetAttValue(ERA_MaxHP);
		sendToNew.nHP		=	m_pMemPtr[n]->GetAttValue(ERA_HP);
		sendToNew.nMaxMP	=	m_pMemPtr[n]->GetAttValue(ERA_MaxMP);
		sendToNew.nMP		=	m_pMemPtr[n]->GetAttValue(ERA_MP);
		sendToNew.fX		=	m_pMemPtr[n]->GetCurPos().x;
		sendToNew.fZ		=	m_pMemPtr[n]->GetCurPos().z;
		for(INT i = 0; i < TEAM_DISPLAY_NUM; ++i)
		{
			sendToNew.dwEquipTypeID[i] = m_pMemPtr[n]->GetAvatarEquip().AvatarEquip[i].dwTypeID;
		}
		memcpy(&sendToNew.AvatarAtt,m_pMemPtr[n]->GetAvatar(), sizeof(tagAvatarAtt));

		pNewMem->SendMessage(&sendToNew, sizeof(sendToNew));


	}
}

//-------------------------------------------------------------------------------------------------------
// ���Ͷ�����Ϣ�����
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamState(Role* pNewMem)
{
	if( !P_VALID(pNewMem) ) return;

	tagNS_GetTeamID		send; 
	send.dwTeamID = GetTeamID();
	send.ePickMode = GetPickMode();

	pNewMem->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ͬ������λ��
//-------------------------------------------------------------------------------------------------------
VOID Team::UpdateTeamatePos()
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if(P_VALID(m_pMemPtr[i]))
		{
			tagNS_RolePosToTeam		send;
			send.dwRoleID = m_pMemPtr[i]->GetID();
			send.dwMapID = m_pMemPtr[i]->GetMapID();
			send.fX = m_pMemPtr[i]->GetCurPos().x;
			send.fZ = m_pMemPtr[i]->GetCurPos().z;
			SendTeamMesOutBigVis(m_pMemPtr[i], &send, send.dwSize);
		}	
	}
}

//-------------------------------------------------------------------------------------------------------
// �������ʱ,���¼���С�����,��͵ȼ�
//-------------------------------------------------------------------------------------------------------
VOID Team::ReCalTeamLevel()
{
	if(P_VALID(m_pMemPtr[0]))
	{
		m_nMaxLevel = m_pMemPtr[0]->GetLevel();
		m_nMinLevel = m_pMemPtr[0]->GetLevel();
	}

	for(INT i = 1; i < m_nMemNum; ++i)
	{
		if(!P_VALID(m_pMemPtr[i]))
			continue;

		if(m_pMemPtr[i]->GetLevel() > m_nMaxLevel)
			m_nMaxLevel = m_pMemPtr[i]->GetLevel();

		if(m_pMemPtr[i]->GetLevel() < m_nMinLevel)
			m_nMinLevel = m_pMemPtr[i]->GetLevel();
	}
}

//-------------------------------------------------------------------------------------------------------
// ����С�ӵ�ɱ�־�������
//-------------------------------------------------------------------------------------------------------
VOID Team::CalExpFactor()
{
	FLOAT fLeaderRein = (FLOAT)m_pMemPtr[0]->GetAttValue(ERA_Rein);

	// De2= ((150-��1-�ӳ�ͳ����/999��*��F-A��)/150)^4, С�Ӿ�����ҵȼ�����ֵ
	FLOAT fDe2 = pow((150.0f - (1.0f - fLeaderRein / 999.0f) * ((FLOAT)m_nMaxLevel - (FLOAT)m_nMinLevel)) / 150.0f, 4.0f);

	m_fExpFactor = fDe2;
}

//-------------------------------------------------------------------------------------------------------
// ���С�ӵ���ʰȡ���
//-------------------------------------------------------------------------------------------------------
Role* Team::GetTeamPickRole(Creature* pCreature) const
{
	if(!P_VALID(pCreature)) return NULL;

	// ����ʰȡ
	if( EPUM_Order == m_ePickMode )
	{
		m_Lock.Acquire();
		list<INT>::iterator it = m_nPickList.begin();
		while( it != m_nPickList.end() )
		{
			if(P_VALID(m_pMemPtr[*it]) && pCreature->IsLootShareDistance(m_pMemPtr[*it]))
			{
				INT nIndex = *it;
				m_nPickList.remove(nIndex);
				m_nPickList.push_back(nIndex);
				m_Lock.Release();
				return m_pMemPtr[nIndex];
			}

			++it;
		}
		m_Lock.Release();

		// ���û�У�����ȫ��
		return NULL;
	}
	// ����ʰȡ
	else if( EPUM_Free == m_ePickMode )
	{
		return NULL;
	}
	// ����
	else
	{
		return NULL;
	}
}

//-----------------------------------------------------------------------------------------------------------
// ���鴴��ʱ
//-----------------------------------------------------------------------------------------------------------
VOID Team::OnCreate()
{
	// ���ӳ��ĸ���ID����Ϊ����ĸ���ID��ͬʱ��նӳ��ĸ���ID
	SetOwnInstanceMapID(m_pMemPtr[0]->GetMyOwnInstanceMapID());
	SetOwnInstanceID(m_pMemPtr[0]->GetMyOwnInstanceID());
	m_pMemPtr[0]->SetMyOwnInstanceMapID(GT_INVALID);
	m_pMemPtr[0]->SetMyOwnInstanceID(GT_INVALID);

	// �������ID��Ϊ�գ�������ԭ�����˸���Ϊ���鸱��
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// ȷ������ͨ����
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnTeamCreate(this);
			}
		}
	}

	// ����Ա�Ƿ񴴽�������
	DWORD dwMemInstanceMapID = m_pMemPtr[1]->GetMyOwnInstanceMapID();
	DWORD dwMemInstanceID = m_pMemPtr[1]->GetMyOwnInstanceID();

	if( GT_INVALID != dwMemInstanceMapID && GT_INVALID != dwMemInstanceID )
	{
		m_pMemPtr[1]->SetMyOwnInstanceMapID(GT_INVALID);
		m_pMemPtr[1]->SetMyOwnInstanceID(GT_INVALID);

		g_mapCreator.SetInstanceDelete(dwMemInstanceMapID, dwMemInstanceID);
	}
}

//-----------------------------------------------------------------------------------------------------------
// ����ɾ��ʱ
//-----------------------------------------------------------------------------------------------------------
VOID Team::OnDelete()
{
	// ����ö����и����������ø���ɾ��
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// ȷ������ͨ����
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnTeamDelete(this);
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// ������һ�����
//------------------------------------------------------------------------------------------------------------
VOID Team::OnAddMember(Role* pRole)
{
	if( !P_VALID(pRole) ) return;

	// �����������Ƿ񴴽��˸���
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// ȷ������ͨ����
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnRoleEnterTeam(pRole->GetID(), this);
			}
		}
	}

	// ����Ա�Ƿ񴴽�������
	DWORD dwMemInstanceMapID = pRole->GetMyOwnInstanceMapID();
	DWORD dwMemInstanceID = pRole->GetMyOwnInstanceID();

	if( GT_INVALID != dwMemInstanceMapID && GT_INVALID != dwMemInstanceID )
	{
		pRole->SetMyOwnInstanceMapID(GT_INVALID);
		pRole->SetMyOwnInstanceID(GT_INVALID);

		g_mapCreator.SetInstanceDelete(dwMemInstanceMapID, dwMemInstanceID);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ��ɾ��һ�����
//-------------------------------------------------------------------------------------------------------------
VOID Team::OnDelMember(Role* pRole)
{
	if( !P_VALID(pRole) ) return;

	// �����������Ƿ񴴽��˸���
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// ȷ������ͨ����
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnRoleLeaveTeam(pRole->GetID(), this);
			}
		}
	}
}

VOID Team::BroadcastLootItemMsgInTeam(Role * pOwner,tagItem * pItem)
{
	if( !P_VALID(pOwner) ) return;
	if( !P_VALID(pItem) ) return;

	DWORD dwTeamID = pOwner->GetTeamID();
	
	if(GT_INVALID != dwTeamID)
	{
		Team * pTeam = const_cast<Team*>( g_groupMgr.GetTeamPtr  (dwTeamID) );
		if( P_VALID(pTeam) )
		{
			BOOL canbroadcast = FALSE;
			if( MIsEquipment(pItem->dwTypeID) )
			{
				MTRANS_POINTER(pEquip,pItem,tagEquip)
				if( pEquip->equipSpec.byQuality != EIQ_White )
					canbroadcast = TRUE;
			}
			else if( pItem->pProtoType->byQuality != EIQ_White )
			{
				canbroadcast = TRUE;
			}
			if(canbroadcast)
			{
				tagNS_RoleShowDroptItemInTeam msg;
				msg.byChannel	= ESCC_Combat;
				msg.dwSrcRoleID = pOwner->GetID();
				msg.dwTypeID	= pItem->dwTypeID;
				g_roleMgr.GetRoleNameByID  (pOwner->GetID  (),msg.szPickerName);
				pTeam->SendTeamMessage(&msg,msg.dwSize);
			}
		}
	}
}

VOID	Team::BroadcastLootItemMsgInTeamExceptPicker(Role * pPicker,tagItem * pItem)
{
	if( !P_VALID(pPicker) ) return;
	if( !P_VALID(pItem) ) return;

	DWORD dwTeamID = pPicker->GetTeamID();

	if(GT_INVALID != dwTeamID)
	{
		Team * pTeam = const_cast<Team*>( g_groupMgr.GetTeamPtr  (dwTeamID) );
		if( P_VALID(pTeam) )
		{
			BOOL canbroadcast = FALSE;
			if( MIsEquipment(pItem->dwTypeID) )
			{
				MTRANS_POINTER(pEquip,pItem,tagEquip)
					if( pEquip->equipSpec.byQuality != EIQ_White )
						canbroadcast = TRUE;
			}
			else if( pItem->pProtoType->byQuality != EIQ_White )
			{
				canbroadcast = TRUE;
			}
			if(canbroadcast)
			{
				tagNS_RoleShowDroptItemInTeam msg;
				msg.byChannel	= ESCC_Combat;
				msg.dwSrcRoleID = pPicker->GetID();
				msg.dwTypeID	= pItem->dwTypeID;
				g_roleMgr.GetRoleNameByID  (pPicker->GetID  (),msg.szPickerName);
				pTeam->SendTeamMessageExceptSomeone(&msg,msg.dwSize,pPicker);
			}
		}
	}
}
VOID			Team::SendTeamMessageExceptSomeone(LPVOID pMsg, DWORD dwSize,Role* pExpRole)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && pExpRole != m_pMemPtr[i] )
		{
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
		}
	}
}
