//-----------------------------------------------------------------------------
//!\file famehall.cpp
//!\author xlguo
//!
//!\date 2009-02-24
//! last 
//!
//!\brief ������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "db_session.h"
#include "att_res.h"
#include "clandata.h"
#include "famehall.h"
#include "role.h"

#include "../ServerDefine/msg_famehall.h"

FameHall g_fameHall;

// ���̵߳���
VOID FameHall::Update()
{
	for (INT32 nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		m_ArrClanTrunk[nClanType].Update();
	}
}

// ��������������Ϣ
VOID FameHall::HandleUpdateRepRank(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NDBS_GetRepRankList);
	ASSERT(JDG_VALID(ECLT, pRecv->byClanType)); 
	m_ArrClanTrunk[pRecv->byClanType].HandleUpdateRepRank(pRecv->repRank, pRecv->nRecNum);
}


// ��ͼ�̵߳���
BOOL FameHall::TryEnterFameHall(Role* pRole, ECLanType eClanType)
{
	return m_ArrClanTrunk[eClanType].TryEnterFameHall(pRole);
}

// ��ȡ���������ǰ50��nameid��
VOID FameHall::GetMemberTop50(BYTE* pData, ECLanType eClanType)
{
	m_ArrClanTrunk[eClanType].GetFameHallTop50(pData);
}

// ��ȡ���������ǰ50������Ŀ
INT32 FameHall::GetMemberTop50Num(ECLanType byClanType)
{
	return m_ArrClanTrunk[byClanType].GetFameHallTop50Num();
}

// �������������tagRepRankData��
VOID FameHall::GetRepRankTop(PVOID pData, ECLanType eClanType)
{
	m_ArrClanTrunk[eClanType].GetRepRank(pData);
}

// �������������С
INT32 FameHall::GetRepRankNum(ECLanType byClanType)
{
	return m_ArrClanTrunk[byClanType].GetRepRankNum();
}
// ����Ѽ��������䱦�б�
VOID FameHall::GetActClanTreasure(PVOID pData, ECLanType eClanType)
{
	m_ArrClanTrunk[eClanType].GetActTreasureList(pData);
}

// ����Ѽ��������䱦��С
INT32 FameHall::GetActClanTreasureNum( ECLanType byClanType )
{
	return m_ArrClanTrunk[byClanType].GetActTreasureNum();
}

BOOL FameHall::Init()
{
	for (INT32 nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		if (FALSE == m_ArrClanTrunk[nClanType].Init((ECLanType)nClanType, ENTER_FAMEHALL_REP_LIM, ENTER_ACTCOUNT_REWARD))
			return FALSE;
	}
	m_bInitOK = FALSE;
	return TRUE;
}

// �������ݿ���Ϣ
VOID FameHall::SendLoadDBData()
{
	tagNDBC_FameHallInitStart	fameHallInitStart;

	g_dbSession.Send(&fameHallInitStart, fameHallInitStart.dwSize);
}

// �����ʼ�������䱦�б���Ϣ
VOID FameHall::HandleInitActTreasureList(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NDBS_GetActTreasureList);
	INT nOffset = 0;
	for (INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		m_ArrClanTrunk[nClanType].HandleInitActTreasureList(&(pRecv->treasureData[nOffset]), pRecv->nRecNum[nClanType]);
		nOffset += pRecv->nRecNum[nClanType];
		ASSERT( nOffset <= MAX_ACT_TREASURE_NUM );
	}

}

// ��ʼ����������Ϣ
VOID FameHall::HandleInitFameHallTop50(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NDBS_GetFameHallEnterSnap);
	INT nOffset = 0;
	for (INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		ASSERT ( pRecv->nRecNum[nClanType] <= MAX_ENTER_FAMEHALL_SNAP_NUM );
		m_ArrClanTrunk[nClanType].HandleInitFameHallTop50(&(pRecv->enterSnap[nOffset]), pRecv->nRecNum[nClanType]);
		nOffset += pRecv->nRecNum[nClanType];
	}
	
}

// ��ʼ����������ʱ��
VOID FameHall::HandleInitRepRstTimeStamp(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NDBS_GetRepResetTimestamp);

	for (INT16 nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		m_ArrClanTrunk[nClanType].HandleInitRepRstTimeStamp(tagDWORDTime(pRecv->dwResetTime[nClanType]));
	}
	InterlockedExchange((LPLONG)&m_bInitOK, TRUE);
}

// ���������䱦�����������̣߳�
DWORD FameHall::ActClanTreasure( Role* pRole, UINT16 u16TreasureID )
{
	const tagClanTreasureProto* pTreasureProto = g_attRes.GetClanTreasureProto(u16TreasureID);
	ASSERT( P_VALID(pTreasureProto) );
	return m_ArrClanTrunk[pTreasureProto->eClanType].ActiveClanTreasure(pRole, u16TreasureID);
}

VOID FameHall::RoleRepUpdate( Role* pRole, ECLanType eClanType )
{
	ASSERT(P_VALID(pRole));
	ClanData* pClanData = &pRole->GetClanData();

	BOOL bBeReset = FALSE;

	if (eClanType != ECLT_NULL)
	{
		bBeReset = pClanData->ResetReputation(m_ArrClanTrunk[eClanType].GetEnterSnapUpdateTime(), eClanType, ERL_Legend);
	}
	else
	{
		for (INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
		{
			if ( pClanData->ResetReputation(m_ArrClanTrunk[nClanType].GetEnterSnapUpdateTime(), (ECLanType)nClanType, ERL_Legend) )
			{
				bBeReset = TRUE;
			}
		}

	}
	if (bBeReset)
	{
		pClanData->SetRepRstTimeStamp(g_world.GetWorldTime());
	}
}