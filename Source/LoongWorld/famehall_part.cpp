 //-----------------------------------------------------------------------------
//!\file famehall_part.cpp
//!\author xlguo
//!
//!\date 2009-02-24
//! last 
//!
//!\brief �����ö���
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "famehall_part.h"
#include "../WorldDefine/msg_famehall.h"
#include "../ServerDefine/msg_famehall.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "role.h"
#include "role_mgr.h"
#include "clan_treasury.h"
#include "map_creator.h"

#include "item_creator.h"

// ��ʼ����ȡ���������䱦�ṹ
BOOL ClanTreasureActList::Init( ECLanType eClantype )
{
	tagClanTreasure* pTreasure					= NULL;
	const tagClanTreasureProto*	pTreasureProto	= NULL;

	m_mapAllTreasure.clear();
	for (UINT16 u16TreasureID = 0; u16TreasureID < CLAN_TREASURE_NUM; ++u16TreasureID)
	{
		pTreasureProto = g_attRes.GetClanTreasureProto(u16TreasureID);
		ASSERT_P_VALID(pTreasureProto);
		if (!P_VALID(pTreasureProto))
			return FALSE;

		if (pTreasureProto->eClanType == eClantype)
		{
			pTreasure					= new tagClanTreasure;

			ASSERT_P_VALID(pTreasure);
			if (!P_VALID(pTreasure))
				return FALSE;

			pTreasure->eState			= ETRS_UNACT;
			pTreasure->pProto			= pTreasureProto;
			m_mapAllTreasure.insert(PairU16ClanTreasure(u16TreasureID, pTreasure));
		}
	}
	m_nActNum = 0;
	return TRUE;
}

// ����
VOID ClanTreasureActList::Destroy()
{
	MapU16ClanTreasure::iterator itr = m_mapAllTreasure.begin();
	while(itr != m_mapAllTreasure.end())
	{
		SAFE_DEL(itr->second);
		++itr;
	}
	m_mapAllTreasure.clear();
	m_nActNum = 0;
}

// �䱦���ݻ��
VOID	ClanTreasureActList::GetActivatedTreasure(PVOID pData)
{
	tagTreasureData* pTreasure = reinterpret_cast<tagTreasureData*>(pData);
	INT nIndex = 0; 

	for (	MapU16ClanTreasure::iterator itr = m_mapAllTreasure.begin();
			nIndex < m_nActNum && itr != m_mapAllTreasure.end();
			++itr
		)
	{
		const tagClanTreasure* pClanTreasure = itr->second;
		ASSERT_P_VALID(pClanTreasure);
		if (!P_VALID(pClanTreasure))
		{
			continue;
		}

		if (ETRS_ACT == pClanTreasure->eState)
		{
			pTreasure[nIndex].n16TreasureID		= itr->first;
			pTreasure[nIndex].dwNamePrefixID	= pClanTreasure->dwNamePrefixID;

			++nIndex;
		}
	}
	ASSERT( nIndex == m_nActNum );
	for (; nIndex != m_nActNum; ++nIndex)
	{
		pTreasure[nIndex].n16TreasureID		= GT_INVALID;
		pTreasure[nIndex].dwNamePrefixID	= GT_INVALID;
	}
}

// �䱦�������
INT16 ClanTreasureActList::GetActivatedTreasureNum()
{
	if (m_nActNum > (INT)m_mapAllTreasure.size() && m_nActNum < 0)
	{
		ASSERT(0);
		return 0;
	}
	return m_nActNum;	
}

// ���Լ����䱦
DWORD	ClanTreasureActList::TryActiveTreasure(Role* pRole, UINT16 u16TreasureID, ECLanType eclantype)
{
	DWORD dwRtv = CanActiveTreasure(pRole, u16TreasureID, eclantype);
	if (E_FrameHall_Success == dwRtv)
	{
		return ActiveTreasure(pRole, u16TreasureID, eclantype);
	}
	return dwRtv;
}

// ��ȡ�䱦
VOID	ClanTreasureActList::HandleLoadDBTreasure(tagTreasureData* pTreasureData, const INT32 nNum)
{
	ResetAll();

	for (INT32 nIndex = 0; nIndex < nNum; ++nIndex)
	{
		SetAct(pTreasureData[nIndex].n16TreasureID, pTreasureData[nIndex].dwNamePrefixID);
	}		
}

// �ɷ񼤻�
DWORD	ClanTreasureActList::CanActiveTreasure(Role* pRole, UINT16 u16TreasureID, ECLanType eclantype)
{
	ClanData* pClanData = &(pRole->GetClanData());

	DWORD dwRtv = E_FrameHall_Success;
	MapU16ClanTreasure::iterator itr = m_mapAllTreasure.find(u16TreasureID);
	
	// ��鱦��״̬
	ASSERT_P_VALID(pClanData);
	if (!P_VALID(pClanData))
	{
		dwRtv = E_FrameHall_UnknownError;
	}
	else if (m_mapAllTreasure.end() == itr)
	{
		// ���������䱦
		dwRtv = E_FrameHall_ItemIsNotTreasure;
	}
	else if (ETRS_ACT == itr->second->eState)
	{
		// �ѱ�����
		dwRtv = E_FrameHall_TreasureAlreadyAct;
	}
	else if (pClanData->ActCountGetVal(eclantype) <= 0)
	{
		// ��鼤�����
		dwRtv = E_FrameHall_ActCountNotEnough;
	}
	else if (pClanData->ClanConGetVal(eclantype) < itr->second->pProto->nActClanConNeed)
	{
		// ���幱������
		dwRtv = E_FrameHall_ClanColNotEnough;
	}
	return dwRtv;
}

// ����
DWORD	ClanTreasureActList::ActiveTreasure(Role* pRole, UINT16 u16TreasureID, ECLanType eclantype)
{
	MapU16ClanTreasure::iterator itr = m_mapAllTreasure.find(u16TreasureID);
	ASSERT( itr != m_mapAllTreasure.end());
	if (itr == m_mapAllTreasure.end())
	{
		return E_FrameHall_UnknownError;
	}

	const tagClanTreasure* pClanTreasure = itr->second;
	ASSERT_P_VALID(pClanTreasure);
	if (!P_VALID(pClanTreasure))
	{
		return E_FrameHall_UnknownError;
	}

	ASSERT_P_VALID(pRole);
	if (!P_VALID(pRole))
	{
		return E_FrameHall_UnknownError;
	}
	ClanData& clanData = pRole->GetClanData();

	// �����������幱��
	clanData.ClanConDec(pClanTreasure->pProto->nActClanConNeed, eclantype, TRUE);

	// �������ļ������
	BOOL bRtv = clanData.ActCountDec(eclantype);
	ASSERT( bRtv );
	if (bRtv <= 0)
	{
		return E_FrameHall_UnknownError;
	}

	DWORD dwNameID	= pRole->GetNameID();
	DWORD dwTypeID	= pClanTreasure->pProto->dwTypeID;

	// ����
	SetAct(u16TreasureID, dwNameID);

	// ������
	GiveRoleItem(dwNameID, pRole, dwTypeID);

	// �������ݿ�
	tagNDBC_InsertActClanTreasure	sendDb;
	sendDb.dwActNameID	= dwNameID;
	sendDb.dwRoleID		= pRole->GetID();
	sendDb.u16TreasureID	= u16TreasureID;
	sendDb.byClanType		= eclantype;
	sendDb.dwActTime		= GetCurrentDWORDTime();
	g_dbSession.Send(&sendDb, sendDb.dwSize);

	// �㲥
	tagNS_NewActivedTreasure		sendC;
	sendC.dwNameID		= dwNameID;
	sendC.n16TreasureID	= u16TreasureID;
	g_roleMgr.SendWorldMsg(&sendC, sendC.dwSize);

	return E_FrameHall_Success;
}

// ���ü�����ڳ�ʼ����
VOID	ClanTreasureActList::SetAct(UINT16 u16TreasureID, DWORD dwNamePrefixID)
{
	MapU16ClanTreasure::iterator itr = m_mapAllTreasure.find(u16TreasureID);
	ASSERT( itr != m_mapAllTreasure.end() );
	if (itr == m_mapAllTreasure.end())
		return;
	
	tagClanTreasure* pClanTreasure = itr->second;
	ASSERT_P_VALID( pClanTreasure);
	if (!P_VALID(pClanTreasure))
		return;

	ASSERT(ETRS_UNACT == pClanTreasure->eState);
	const tagClanTreasureProto* pTreasureProto = pClanTreasure->pProto;

	pClanTreasure->eState			= ETRS_ACT;
	pClanTreasure->dwNamePrefixID	= dwNamePrefixID;
	
	//tbc:��ӵ����屦����
	Map*	pMap = NULL;
	Shop*	pShop = NULL;

	if (P_VALID(pMap= g_mapCreator.GetMap(pTreasureProto->dwMapID, GT_INVALID)) &&
		P_VALID(pShop = pMap->GetShop(pTreasureProto->dwNpcID)))
	{
		BOOL bRtv = pShop->ActiveTreasure(pTreasureProto->dwTypeID, dwNamePrefixID);
		ASSERT(bRtv);
	}


	++m_nActNum;
}

// ��������
VOID	ClanTreasureActList::ResetAll()
{
	
	for (MapU16ClanTreasure::iterator itr = m_mapAllTreasure.begin();
		itr != m_mapAllTreasure.end();
		++itr)
	{
		itr->second->eState	= ETRS_UNACT;
		itr->second->pProto	= g_attRes.GetClanTreasureProto(itr->first);
	}
	m_nActNum = 0;
}

// ��������Ѽ������Ʒ
VOID ClanTreasureActList::GiveRoleItem( DWORD dwNameID, Role* pRole, DWORD dwTypeID )
{
	tagItem* pActItem = ItemCreator::CreateTreasure(dwNameID, EICM_ActTreasure, pRole->GetID(), dwTypeID);
	if (!P_VALID(pActItem))
	{
		return;
	}

	DWORD dwRtv = pRole->GetItemMgr().Add2Bag(pActItem, ELCLD_ACT_TREASURE, TRUE);
	if(E_Success != dwRtv)
	{
		SAFE_DEL(pActItem);
		return;
	}
}

// tbc������nameid
DWORD ClanTrunk::GetNameID( DWORD dwRoleID )
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	return pRole->GetNameID();
}

// ������ӽ�ɫ
VOID FameRoleTracker::TrackRole( DWORD dwRoleID, ECLanType eClanType )
{
	// ���Ϊ�����ó�Ա
	MarkFameRole(dwRoleID, eClanType);

	// ��¼����
	if ( !IsSnapFull() && !IsSnapAdded(dwRoleID, ClanTrunk::GetNameID(dwRoleID)) )
	{
		AddRoleSnap(dwRoleID, eClanType);
	}
}

// ������ӽ�ɫ
BOOL FameRoleTracker::IsSnapAdded(DWORD dwRoleID, DWORD dwNameID)
{
	tagFameHallEnterSnap member;
	member.dwEnterNameID = dwNameID;
	member.dwRoleID = dwRoleID;
	VecFameMember::iterator itr = find(m_vecAllMembers.begin(), m_vecAllMembers.end(), member);
	return itr != m_vecAllMembers.end();
}

// ���������ǰ�ĳ�Ա
INT32 FameRoleTracker::GetTopSnap(BYTE* pData, INT32 nNum)
{
	DWORD* pMember = reinterpret_cast<DWORD*>(pData);

	VecFameMember::iterator itr = m_vecAllMembers.begin();
	INT32 nMemberIndex = 0;
	for (; nMemberIndex < nNum && itr != m_vecAllMembers.end(); ++nMemberIndex, ++itr)
	{
		pMember[nMemberIndex] = itr->dwEnterNameID;
	}
	return nMemberIndex;
}

// ��ӽ�ɫ
VOID FameRoleTracker::AddRoleSnap( DWORD dwRoleID, ECLanType eClanType )
{
	tagFameHallEnterSnap member;
	// tbd: ����id
	member.dwEnterNameID	= ClanTrunk::GetNameID(dwRoleID);
	member.dwRoleID			= dwRoleID;
	member.dwtEnterTime		= GetCurrentDWORDTime();
	m_vecAllMembers.push_back(member);

	// ���ݿ��¼
	tagNDBC_InsertFameHallSnap snapSend;
	snapSend.dwEnterNameID	= member.dwEnterNameID;
	snapSend.dwEnterTime	= member.dwtEnterTime;
	snapSend.dwRoleID		= member.dwRoleID;
	snapSend.byClanType		= eClanType;
	g_dbSession.Send(&snapSend, snapSend.dwSize);
}
BOOL FameRoleTracker::IsSnapFull()
{
	return m_vecAllMembers.size() >= 50 ;
}
VOID FameRoleTracker::MarkFameRole( DWORD dwRoleID, ECLanType eClanType )
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT_P_VALID(pRole);
	if (!P_VALID(pRole))
		return;

	// ����Ϊ����
	pRole->GetClanData().SetFame(eClanType);
}

// ��ʼ�����س�Ա��ɫ
VOID FameRoleTracker::LoadInitFameHallEnterSnap(tagFameHallEnterSnap* pEnterSnap, const INT32 nNum)
{
	m_vecAllMembers.clear();
	m_vecAllMembers.resize(nNum);
	for (INT32 nIndex = 0; nIndex < nNum; ++nIndex)
	{
		m_vecAllMembers[nIndex] = pEnterSnap[nIndex];
	}
}

// �Ƿ�������
BOOL FameRoleTracker::IsFameRole( DWORD dwRoleID, ECLanType eClanType )
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT_P_VALID(pRole);
	if (!P_VALID(pRole))
	{
		return FALSE;
	}
	return pRole->GetClanData().IsFame(eClanType);
}

// ��ó�Ա����
INT32 FameRoleTracker::GetTopSnapNum(INT32 nNum) const 
{	
	INT32 nRtNum = m_vecAllMembers.size();
	return nRtNum >= nNum ? nNum : nRtNum;
}

// ���Խ���������
BOOL ClanTrunk::TryEnterFameHall(Role* pRole)
{
	BOOL bEnter = FALSE;
	m_EnterLock.Acquire();		
	if (!P_VALID(m_pRoleEnter) && pRole->GetClanData().RepGetVal(m_eClanType) >= m_nEnterFameHallRepLim)
	{
		bEnter = TRUE;
		m_pRoleEnter = pRole;
	}
	m_EnterLock.Release();
	return bEnter;
}	

// �õ�����������ǰ50
VOID ClanTrunk::GetFameHallTop50( BYTE* pData )
{
	m_FameRoleTracker.GetTopSnap(pData, 50);
}


// �õ������õ�ǰ�ߴ�
INT32 ClanTrunk::GetFameHallTop50Num()
{
	return m_FameRoleTracker.GetTopSnapNum(50);
}

// �õ�������������
VOID ClanTrunk::GetRepRank(PVOID pData)
{
	tagRepRankData* pRankData = reinterpret_cast<tagRepRankData*>(pData);
	INT32 nNum = m_vecRepRank.size();
	for (INT32 nIndex = 0; nIndex < nNum; ++nIndex)
	{
		pRankData[nIndex] = m_vecRepRank[nIndex];
	}
}

// �õ�����������������
INT32 ClanTrunk::GetRepRankNum()
{
	return m_vecRepRank.size();
}

// ������������
VOID ClanTrunk::HandleUpdateRepRank(tagRepRankData* pRepOrderData, const INT32 nNum)
{
	m_vecRepRank.clear();
	for (INT32 nIndex = 0; nIndex < nNum; ++nIndex)
	{
		m_vecRepRank.push_back(pRepOrderData[nIndex]);
	}
	m_dwtRepRankUpdateTime = GetCurrentDWORDTime();
}

// ��ʼ������������
VOID ClanTrunk::HandleInitFameHallTop50(tagFameHallEnterSnap* pEnterSnap, const INT32 nNum)
{	
	m_FameRoleTracker.LoadInitFameHallEnterSnap(pEnterSnap, nNum);
}

// ��ʼ�������䱦
VOID ClanTrunk::HandleInitActTreasureList(tagTreasureData* pTreasure, const INT32 nNum)
{
	m_ClanTreasure.HandleLoadDBTreasure(pTreasure, nNum);
}

BOOL ClanTrunk::Init( ECLanType eClanType, INT32 nEnterLim, INT8 n8ActCount )
{
	m_nRepRankUpdateTickCounter = TICK_PER_SECOND * 10;
	m_eClanType = eClanType;
	m_nEnterFameHallRepLim = nEnterLim;
	m_n8ActCountReward = n8ActCount;
	m_dwtRepRankUpdateTime		= 0;
	m_dwtRepResetTime		= 0;
	return m_ClanTreasure.Init(m_eClanType);
}

// ���������䱦
DWORD ClanTrunk::ActiveClanTreasure(Role* pRole, UINT16 u16TreasureID)
{
	return m_ClanTreasure.TryActiveTreasure(pRole, u16TreasureID, m_eClanType);
}

// ��������䱦�б�
VOID ClanTrunk::GetActTreasureList(PVOID pData)
{
	m_ClanTreasure.GetActivatedTreasure(pData);
}

// ��������䱦�ߴ�
INT32 ClanTrunk::GetActTreasureNum()
{
	return m_ClanTreasure.GetActivatedTreasureNum();
}

// ���½���������
BOOL ClanTrunk::UpdateEnter()
{
	if (CanEnter())
		return Enter();
	return FALSE;
}

// ������������
VOID ClanTrunk::UpdateRepRank()
{
	if (--m_nRepRankUpdateTickCounter <= 0)
	{
		m_nRepRankUpdateTickCounter = REP_ORDER_UPDATE_INTERVAL_TICKS;

		tagNDBC_GetRepRankList send;
		send.byClanType = m_eClanType;
		g_dbSession.Send(&send, send.dwSize);
	}
}

// ����������
BOOL ClanTrunk::Enter()
{
	ASSERT_P_VALID(m_pRoleEnter);
	if (!P_VALID(m_pRoleEnter))
		return FALSE;

	BOOL bEnter = TRUE;
	ClanData* pClanData = &(m_pRoleEnter->GetClanData());

	// ���
	m_FameRoleTracker.TrackRole(m_pRoleEnter->GetID(), m_eClanType);

	// ��ɫ��ü����䱦����
	pClanData->ActCountSetVal(m_n8ActCountReward, m_eClanType);

	m_pRoleEnter = NULL;

	// ������������Ϊ������ҵ�����ֵ(Ч��ԭ�򣬾��������߳��ж�ʱupdate)
	m_dwtRepResetTime = GetCurrentDWORDTime();

	tagNDBC_UpDateRepRstTime send;
	send.byClanType			= static_cast<BYTE>(m_eClanType);
	send.dwRepRstTimeStamp	= m_dwtRepResetTime;
	g_dbSession.Send(&send, send.dwSize);

	g_roleMgr.ResetRoleRep(m_eClanType, ERL_Legend, m_dwtRepResetTime);

	return TRUE;
}

// �ܷ����������
BOOL ClanTrunk::CanEnter()
{
	return P_VALID(m_pRoleEnter);
}