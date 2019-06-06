//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_mgr.cpp
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݹ����������ڸ���������������ɼ�ɾ�������洢�������ڵ���������
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "role_mgr.h"
#include "role.h"
#include "player_session.h"
#include "group_mgr.h"
#include "social_mgr.h"
#include "famehall.h"
#include "TradeYuanBao.h"
#include "vip_stall.h"
#include "..\\ServerDefine\\role_data_define.h"
#include "..\\ServerDefine\\role_data.h"

#include "../WorldDefine/msg_script.h"



//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID tagRoleMgr::Reset()
{
	nMaxUseIndex	= -1;
	nLastSaveIndex	= 0;
	nFreeHeadIndex	= 0;
	nFreeNodeNum	= MAX_PLAYER_NUM;
	
	for(INT i=0; i<MAX_PLAYER_NUM - 1; ++i)
	{
		sAllRole[i].nNext = i + 1;
		sAllRole[i].pRole = NULL;
	}

	// ���һ��
	sAllRole[MAX_PLAYER_NUM - 1].nNext = GT_INVALID;
	sAllRole[MAX_PLAYER_NUM - 1].pRole = NULL;
}

//------------------------------------------------------------------------------
// ��ȡ�豣��Ľ�ɫ
//------------------------------------------------------------------------------
Role* tagRoleMgr::GetRoleNeedSave()
{
	if(nMaxUseIndex < 0)
	{
		return NULL;
	}
	
	INT nIndexSave = nLastSaveIndex;
	while(true)
	{
		if(nIndexSave >= nMaxUseIndex)
		{
			nIndexSave = 0;
		}
		else
		{
			++nIndexSave;
		}

		if(sAllRole[nIndexSave].pRole != NULL)
		{
			nLastSaveIndex = nIndexSave;
			break;
		}

		if(nIndexSave == nLastSaveIndex)
		{
			// û���ҵ���Ҫ�洢�Ľ�ɫ
			nLastSaveIndex	= 0;
			nMaxUseIndex	= -1;
			
			// �������Ƿ�����©
			if(nFreeNodeNum != MAX_PLAYER_NUM)
			{
				IMSG(_T("\n\n\n\n\nCritical Error in tagRoleMgr!!!!!\n\n"));
			}

			return NULL;
		}
	}

	if(!sAllRole[nLastSaveIndex].pRole->IsNeedSave2DB())
	{
		return NULL;
	}

	return sAllRole[nLastSaveIndex].pRole;
}

//------------------------------------------------------------------------------
// constructor
//------------------------------------------------------------------------------
RoleMgr::RoleMgr()
{
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
RoleMgr::~RoleMgr()
{
	Destroy();
}

//--------------------------------------------------------------------------------
// ��ʼ��
//--------------------------------------------------------------------------------
BOOL RoleMgr::Init()
{
	m_pFastCode = "FastCode";
	m_vctRole.Reset();
	return TRUE;
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID RoleMgr::Destroy()
{
	// ɾ�����е����߽�ɫ
	m_RoleMapMutex.Acquire();

	INT nIndex;
	Role* pRole = NULL;

	RoleMap::TMapIterator it = m_mapRole.Begin();
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.Remove(nIndex);
		Role::Delete(pRole);
	}
	m_mapRole.Clear();
	m_vctRole.Reset();

	m_RoleMapMutex.Release();

	// ɾ�����е����߽�ɫ
	EraseAllRoleInfo();
}

//-----------------------------------------------------------------------------------
// ����һ���µ����߽�ɫ
//-----------------------------------------------------------------------------------
BOOL RoleMgr::CreateRoleInfo(const tagRoleInfo* pInfo)
{
	m_RoleInfoMapMutex.Acquire();
	if (!m_mapRoleInfo.IsExist(pInfo->dwRoleID))
	{
		tagRoleInfo* pNew = new tagRoleInfo;
		if( !P_VALID(pNew) ) return FALSE;

		m_pFastCode->MemCpy(pNew, pInfo, sizeof(tagRoleInfo));
		pNew->bOnline = false;
		m_mapRoleInfo.Add(pNew->dwRoleID, pNew);
		m_mapRoleNameCrcID.Add(pNew->dwRoleNameCrc, pNew->dwRoleID);
	}
	m_RoleInfoMapMutex.Release();

	return TRUE;
}

//------------------------------------------------------------------------------------
// ɾ��һ�����߽�ɫ����һ����ɾ�����ʱ����
//------------------------------------------------------------------------------------
BOOL RoleMgr::DeleteRoleInfo(const DWORD dwRoleID)
{
	m_RoleInfoMapMutex.Acquire();

	tagRoleInfo* pInfo = m_mapRoleInfo.Peek(dwRoleID);
	if( P_VALID(pInfo) )
	{
		DeletAllFriend(dwRoleID);
		g_VIPStall.RemoveRoleVIPStall(dwRoleID);	// VIP̯λ����
		DeleteYBOrder(dwRoleID);
		m_mapRoleInfo.Erase(dwRoleID);
		m_mapRoleNameCrcID.Erase(pInfo->dwRoleNameCrc);
		SAFE_DEL(pInfo);
	}

	m_RoleInfoMapMutex.Release();

	return TRUE;
}

//-------------------------------------------------------------------------------------
// ����һ��������ң�����LoongDB�����ɫ����ʱ����
//-------------------------------------------------------------------------------------
Role* RoleMgr::CreateRole(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession, BOOL& bFirst)
{
	Role* pOld = GetRolePtrByID(dwRoleID);
	if (P_VALID(pOld))
	{
		IMSG(_T("Role already loaded! roleid:%u, accountid:%u!\n"), dwRoleID, pSession->GetSessionID());
		ILOG->Write(_T("Role already loaded! roleid:%u, accountid:%u!\n"), dwRoleID, pSession->GetSessionID());
		return NULL;
	}

	Role* pNew = Role::Create(dwRoleID, pData, pSession);
	if( !P_VALID(pNew) ) return NULL;

	m_RoleMapMutex.Acquire();

	INT nIndex = m_vctRole.Add(pNew);
	if(GT_INVALID == nIndex)
	{
		IMSG(_T("\n\n\n\n\nCritical Error in tagRoleMgr!!!!!\n\n"));
		ASSERT(0);
		Role::Delete(pNew);
		return NULL;
	}

	m_mapRole.Add(pNew->GetID(), nIndex);

	m_RoleMapMutex.Release();

	// �鿴�ǲ��ǵ�һ������
	if( GT_INVALID == pNew->GetOnlineTime() )
	{
		bFirst = TRUE;
	}

	PlayerOnline(pNew, bFirst);

	return pNew;
}

//--------------------------------------------------------------------------------------
// ɾ��һ��������ң���һ������������ʱɾ������һ���߾�ɾ�������ǵ����ݿⷵ���˲�ɾ����
//--------------------------------------------------------------------------------------
BOOL RoleMgr::DeleteRole(const DWORD dwRoleID)
{
	m_RoleMapMutex.Acquire();

	INT nIndex = m_mapRole.Peek(dwRoleID);
	Role* pRole = m_vctRole.GetRole(nIndex);
	if( P_VALID(pRole) )
	{
		PlayerOutLine(pRole);
		m_vctRole.Remove(nIndex);
		m_mapRole.Erase(dwRoleID);
		Role::Delete(pRole);
	}
	m_RoleMapMutex.Release();

	return TRUE;
}

//--------------------------------------------------------------------------------------
// ����һ����ҵ�DB
//--------------------------------------------------------------------------------------
VOID RoleMgr::SaveOneRoleToDB()
{
	if(m_mapRole.Empty())
	{
		return;
	}
	
	Role *pRole = m_vctRole.GetRoleNeedSave();
	if(pRole != NULL)
	{
		pRole->SaveToDB();
	}
}

//--------------------------------------------------------------------------------------
// ����������ҵ�DB��һ����ϵͳͣ��ʱ��
//--------------------------------------------------------------------------------------
VOID RoleMgr::SaveAllRolesToDB()
{
	INT nIndex = GT_INVALID;
	Role* pRole = NULL;
	RoleMap::TMapIterator it = m_mapRole.Begin();

	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			pRole->SaveToDB();
		}
	}
}

//---------------------------------------------------------------------------------------
// �����������CRCֵ�õ����ID
//---------------------------------------------------------------------------------------
DWORD RoleMgr::GetRoleIDByNameCrc(const DWORD dwNameCrc)
{
	return m_mapRoleNameCrcID.Peek(dwNameCrc);
}

//---------------------------------------------------------------------------------------
// �������ID�õ��������
//---------------------------------------------------------------------------------------
VOID RoleMgr::GetRoleNameByID(const DWORD dwRoleID, LPTSTR szName)
{
	tagRoleInfo* pRoleInfo = m_mapRoleInfo.Peek(dwRoleID);
	if( P_VALID(pRoleInfo) )
	{
		_tcsncpy(szName, pRoleInfo->szRoleName, X_SHORT_NAME);
	}
	else
	{
		ZeroMemory(szName, X_SHORT_NAME * sizeof(TCHAR));
	}
}

//---------------------------------------------------------------------------------------
// �������ID�õ����ָ��
//---------------------------------------------------------------------------------------
Role* RoleMgr::GetRolePtrByID(const DWORD dwRoleID)
{
	return m_vctRole.GetRole(m_mapRole.Peek(dwRoleID));
}

//---------------------------------------------------------------------------------------
// �����ȡ�������bMustGetΪ�������ȡ��һ��
//---------------------------------------------------------------------------------------
Role* RoleMgr::GetRandRolePtr( BOOL bGetFirst /*= FALSE*/ )
{
	if (bGetFirst)
	{
		if (m_mapRole.Size() <= 0)
		{
			return NULL;
		}

		return m_vctRole.GetRole(m_mapRole.Begin()->second);
	}
	INT	nMaxUsedIndex = m_vctRole.GetMaxUsedIndex();

	if(nMaxUsedIndex == 0)
		return (Role*)GT_INVALID;

	INT nRandIndex = rand() % nMaxUsedIndex;

	return m_vctRole.GetRole(nRandIndex);
}

struct SendMsg
{
	SendMsg(PVOID pMsg, DWORD dwSize)
		:m_pMsg(pMsg), m_dwSize(dwSize){}
	VOID operator()(Unit* pUnit)
	{
		if (P_VALID(pUnit) && pUnit->IsRole())
		{
			Role* pRole = dynamic_cast<Role*>(pUnit);
			if (P_VALID(pRole))
			{
				pRole->SendMessage(m_pMsg, m_dwSize);
			}			
		}
	}
	PVOID m_pMsg;
	DWORD m_dwSize;
};

//--------------------------------------------------------------------------------------------
// �����������е�ͼ�ڵ���ҷ���Ϣ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::SendWorldMsg(LPVOID pMsg, DWORD dwSize)
{
	ForEachRoleInWorld(SendMsg(pMsg, dwSize));
// 	RoleMap::TMapIterator it = m_mapRole.Begin();
// 	INT nIndex = -1;
// 	Role* pRole = NULL;
// 	while( m_mapRole.PeekNext(it, nIndex) )
// 	{
// 		pRole = m_vctRole.GetRole(nIndex);
// 		if( P_VALID(pRole) )
// 		{
// 			pRole->SendMessage(pMsg, dwSize);
// 		}
// 	}
}

//--------------------------------------------------------------------------------------------
// �õ���ҵļ�����Ϣ��������Ұ��������ߣ�
//--------------------------------------------------------------------------------------------
tagRoleInfo* RoleMgr::GetRoleInfo(const DWORD dwRoleID)
{
	tagRoleInfo* pRoleInfo = m_mapRoleInfo.Peek(dwRoleID);
	return pRoleInfo;	
}

//--------------------------------------------------------------------------------------------
// �����������ش���
//--------------------------------------------------------------------------------------------
VOID RoleMgr::PlayerOnline(Role *pRole, BOOL bFirst)
{
	// ����ǵ�һ�����ߣ�����ó������ߺ���
	pRole->Online(bFirst);

	// ����������ߵ��¼�
	g_socialMgr.SendLoginToFriend(pRole);

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());

	if(P_VALID(pRoleInfo))
		pRoleInfo->bOnline = true;
}

//--------------------------------------------------------------------------------------------
// �����������ش���
//--------------------------------------------------------------------------------------------
VOID RoleMgr::PlayerOutLine(Role* pRole)
{
	// �����ʱ������Ϣ������
	g_socialMgr.SendLogoutToFriend(pRole);

	// ����˳�С��
	DWORD dwTeamID = pRole->GetTeamID();
	if( GT_INVALID != dwTeamID )
	{
		g_groupMgr.RoleOutline(pRole);
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());

	if(P_VALID(pRoleInfo))
	{
		pRoleInfo->bOnline = false;
		pRoleInfo->dwTimeLastLogout = pRole->GetLogoutTime();
	}
}

//--------------------------------------------------------------------------------------------
// ���ɾ����ɫʱ��ɾ�����ݿ�����صĺ�����Ϣ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::DeletAllFriend(DWORD dwRoleID)
{
	tagNDBC_DelAllFriRel	sendFri;
	sendFri.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendFri, sendFri.dwSize);

	tagNDBC_DelAllFriValRel	sendVal;
	sendVal.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendVal, sendVal.dwSize);
}

//--------------------------------------------------------------------------------------------
// ���ɾ����ɫʱ��ɾ������ύ����Ԫ�����׶���
//--------------------------------------------------------------------------------------------
VOID RoleMgr::DeleteYBOrder(DWORD dwRoleID)
{
	tagYuanBaoOrder* pYBSellOrder= g_tradeYB.GetYBSellOrder(dwRoleID);
	if(P_VALID(pYBSellOrder))
		g_tradeYB.DeleteYBOrder(pYBSellOrder, EYBOM_Close);

	tagYuanBaoOrder* pYBBuyOrder= g_tradeYB.GetYBBuyOrder(dwRoleID);
	if(P_VALID(pYBBuyOrder))
		g_tradeYB.DeleteYBOrder(pYBBuyOrder, EYBOM_Close);
}

//--------------------------------------------------------------------------------------------
// ɾ�����е����߽�ɫ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::EraseAllRoleInfo()
{
	m_RoleInfoMapMutex.Acquire();

	tagRoleInfo* pInfo = NULL;
	RoleInfoMap::TMapIterator it2 = m_mapRoleInfo.Begin();
	while( m_mapRoleInfo.PeekNext(it2, pInfo) )
	{
		SAFE_DEL(pInfo);
	}
	m_mapRoleInfo.Clear();

	m_RoleInfoMapMutex.Release();
}

//--------------------------------------------------------------------------------------------
// �����������ֵ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::ResetRoleRep( ECLanType eClanType, EReputationLevel eRepLvl, tagDWORDTime dwtResetTime )
{
	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = GT_INVALID;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			g_fameHall.RoleRepUpdate(pRole, eClanType);
		}
	}
}

//--------------------------------------------------------------------------------------------
// ������������Ҽ�һ��buff��ֻ�������̵߳��ã�
//--------------------------------------------------------------------------------------------
VOID RoleMgr::AddAllRoleBuff(DWORD dwBuffTypeID)
{
	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return;

	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = -1;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			pRole->TryAddBuff(pRole, pProto, NULL, NULL, NULL);
		}
	}
}
//tbd:ͨ������id��ȡ����
VOID RoleMgr::GetRoleNameByNameID( const DWORD dwNameID, LPTSTR szName )
{
	// ��ʱ��RoleID��NameID
	GetRoleNameByID(dwNameID, szName);
}


RoleMgr g_roleMgr;