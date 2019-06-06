//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_commerce.cpp
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ������̹���
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "../WorldDefine/msg_guild.h"

#include "guild.h"
#include "guild_commodity.h"
#include "guild_CofC.h"
#include "role.h"
#include "db_session.h"
#include "role_mgr.h"

//-----------------------------------------------------------------------------
// ���������
//-----------------------------------------------------------------------------
GuildCommerce::GuildCommerce()
{
	m_pGuild	= NULL;
	m_bCommend	= FALSE;
	m_mapCommodity.Clear();
	m_vecRank.clear();
	m_bInitOK	= FALSE;
}

GuildCommerce::~GuildCommerce()
{
	Destory();
}

//-----------------------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------------------
BOOL GuildCommerce::Init( Guild* pGuild, BOOL bRequest /*= FALSE*/ )
{
	if (!P_VALID(pGuild))
	{
		return FALSE;
	}
	m_pGuild	= pGuild;
	m_bCommend	= pGuild->GetGuildAtt().bCommendation;
	m_mapCommodity.Clear();
	m_vecRank.clear();

	if (bRequest)
	{
		// ��DB��������
		tagNDBC_LoadCommodity send;
		send.dwGuildID	= pGuild->GetGuildAtt().dwID;
		g_dbSession.Send(&send, send.dwSize);

		tagNDBC_LoadCommerceRank send_rank;
		send_rank.dwGuildID	= pGuild->GetGuildAtt().dwID;
		g_dbSession.Send(&send_rank, send_rank.dwSize);

		m_bInitOK		= FALSE;
	}
	else
	{
		m_bInitOK	= TRUE;
	}

	return TRUE;
}

VOID GuildCommerce::Destory()
{
	m_bInitOK	= FALSE;

	GuildCommodity* pCommodity = NULL;
	MapCommodity::TMapIterator iter_commodity = m_mapCommodity.Begin();
	while (m_mapCommodity.PeekNext(iter_commodity, pCommodity))
	{
		SAFE_DEL(pCommodity);
	}
	m_mapCommodity.Clear();

	VectorRank::iterator iter_rank = m_vecRank.begin();
	while (iter_rank != m_vecRank.end())
	{
		tagCommerceRank* pRankInfo	= *iter_rank;
		SAFE_DEL(pRankInfo);
		iter_rank++;
	}
	m_vecRank.clear();
}

//-----------------------------------------------------------------------------
// ��ȡ����
//-----------------------------------------------------------------------------
DWORD GuildCommerce::AcceptCommerce( Role* pRole )
{
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	// �ȼ�����(����ط����Զ�ȡ�ű�)
	if (pRole->GetLevel() < 20)
	{
		return E_GuildMember_Level_Limit;
	}

	// ����״̬
	if (m_pGuild->IsInGuildStateAny(EGDSS_Warfare))
	{
		return E_Guild_State_Limit;
	}

	// ְλ�ж�
	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}
	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bCommerce)
	{
		return E_Guild_Power_NotEnough;
	}

	// ��֤���Ƿ��㹻
	const tagCommerceInfo* pCommerceInfo = g_attRes.GetGuildCommerceInfo(pRole->GetLevel());
	if (pRole->GetCurMgr().GetBagSilver() < pCommerceInfo->sTaelInfo.nDeposit)
	{
		return E_BagSilver_NotEnough;
	}

	// ��ɫ�Ƿ���������
	if (pRole->IsInRoleState(ERS_Commerce))
	{
		return E_GuildCommerce_Status_Error;
	}

	// �۳���ҽ�Ǯ
	pRole->GetCurMgr().DecBagSilver(pCommerceInfo->sTaelInfo.nDeposit, ELCLD_Guild_Commerce);

	// ������������
	GuildCommodity* pCommodity = m_mapCommodity.Peek(pRole->GetID());
	if (P_VALID(pCommodity))
	{
		SAFE_DEL(pCommodity);
		m_mapCommodity.Erase(pRole->GetID());
	}
	pCommodity = new GuildCommodity;
	pCommodity->Init(pRole->GetID(), pRole->GetLevel(), &pCommerceInfo->sTaelInfo, &pCommerceInfo->sRedoundInfo);
	m_mapCommodity.Add(pRole->GetID(), pCommodity);

	tagNDBC_CreateCommodity send;
	send.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
	send.dwRoleID	= pRole->GetID();
	send.nLevel		= pRole->GetLevel();
	send.nTael		= pCommerceInfo->sTaelInfo.nBeginningTael;
	g_dbSession.Send(&send, send.dwSize);

	// �������״̬
	pRole->SetRoleState(ERS_Commerce);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
DWORD GuildCommerce::CompleteCommerce( Role* pRole, INT32& nFund )
{
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	// ����״̬
	if (m_pGuild->IsInGuildStateAny(EGDSS_Warfare))
	{
		return E_Guild_State_Limit;
	}

	// ���״̬
	if (!pRole->IsInRoleState(ERS_Commerce))
	{
		return E_GuildCommerce_Status_Error;
	}

	// �ж������Ƿ���
	GuildCommodity* pCommodity = m_mapCommodity.Peek(pRole->GetID());
	if (!P_VALID(pCommodity))
	{
		return E_GuildCommerce_Status_Error;
	}
	if (pCommodity->GetTaelProgress() < 100)
	{
		return E_GuildCommerce_Tael_NotEnough;
	}

	// ���Ӱ����ʽ�
	nFund	= (INT32)(pCommodity->GetGain() * 0.7f + 0.5f);
	m_pGuild->IncGuildFund(pRole->GetID(), nFund, ELCLD_Guild_Commerce);

	// ��ҽ���
	const tagRedoundInfo* pRedoundInfo	= pCommodity->GetRedoundInfo();
	const tagTaelInfo* pTaelInfo		= pCommodity->GetTaelInfo();
	if (P_VALID(pRedoundInfo) && P_VALID(pTaelInfo))
	{
		pRole->ExpChange(pRedoundInfo->nExp);													// ����
		m_pGuild->ChangeMemberContribution(pRole->GetID(), pRedoundInfo->nContribution, TRUE);	// ����

		//���������30%+��֤���90%������ȡ������10%+��֤���120%��
		INT32 nRedoundSilver = 0;
		if (m_pGuild->IsInGuildStateAny(EGDSS_Distress | EGDSS_Shortage))
		{
			nRedoundSilver = (INT32)(pCommodity->GetGain() * 0.3f + pCommodity->GetTaelInfo()->nDeposit * 1.2f + 0.5f);
		}
		else
		{
			nRedoundSilver = (INT32)(pCommodity->GetGain() * 0.3f + pCommodity->GetTaelInfo()->nDeposit * 0.9f + 0.5f);
		}
		pRole->GetCurMgr().IncBagSilver(nRedoundSilver, ELCLD_Guild_Commerce);
	}

	//���ű�
	g_ScriptMgr.GetRoleScript()->OnCompleteCommerce(pRole,pCommodity->GetTaelProgress());


	// �������״̬
	pRole->UnsetRoleState(ERS_Commerce);

	// ���浽���а�
	Add2CommerceRank(pRole->GetID(), nFund);

	// ɾ����������
	SAFE_DEL(pCommodity);
	m_mapCommodity.Erase(pRole->GetID());

	tagNDBC_RemoveCommodity send;
	send.dwRoleID	= pRole->GetID();
	g_dbSession.Send(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD GuildCommerce::AbandonCommerce( DWORD dwRoleID, BOOL bClearRank /*= FALSE*/ )
{
	if (!GT_VALID(dwRoleID))
	{
		return GT_INVALID;
	}

	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	// �������������Ϣ
	if (bClearRank)
	{
		RemoveCommerceRank(dwRoleID);
	}

	GuildCommodity* pCommodity	= m_mapCommodity.Peek(dwRoleID);
	if (!P_VALID(pCommodity))
	{
		return E_GuildCommerce_Status_Error;
	}

	// ���Ӱ����ʽ�
	INT32 nFund	= (INT32)(pCommodity->GetGain() * 0.9f + 0.5f);
	m_pGuild->IncGuildFund(dwRoleID, nFund, ELCLD_Guild_Commerce);

	// �����̻�����
	SAFE_DEL(pCommodity);
	m_mapCommodity.Erase(dwRoleID);

	// ֪ͨ�ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		pRole->UnsetRoleState(ERS_Commerce);
		tagNS_AbandonCommerce send;
		pRole->SendMessage(&send, send.dwSize);
	}

	// ֪ͨ���ݿ�
	tagNDBC_RemoveCommodity send;
	send.dwRoleID	= dwRoleID;
	g_dbSession.Send(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ���а���Ϣ
//-----------------------------------------------------------------------------
DWORD GuildCommerce::GetCommerceRankInfo( tagCommerceRank* pRankInfo, INT& nRankNum, BOOL& bCommend )
{
	// �ϲ㱣֤������С
	if (!P_VALID(pRankInfo))
	{
		return GT_INVALID;
	}

	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	nRankNum	= 0;
	bCommend	= m_bCommend;

	// ��ȡǰʮ��Rank��Ϣ
	VectorRank	vecRankInfo(MAX_COMMERCE_RANK_INFO);
	partial_sort_copy(m_vecRank.begin(), m_vecRank.end(), vecRankInfo.begin(), vecRankInfo.end(), RankCompare());

	VectorRank::iterator iter_rank = vecRankInfo.begin();
	for (; iter_rank != vecRankInfo.end(); iter_rank++)
	{
		if (!P_VALID(*iter_rank))
		{
			break;
		}
		pRankInfo[nRankNum++] = *(*iter_rank);
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ����/�ر����̼ν�
//-----------------------------------------------------------------------------
DWORD GuildCommerce::SwitchCommendation( DWORD dwRoleID, BOOL bSwitchON )
{
	if (!GT_VALID(dwRoleID))
	{
		return GT_INVALID;
	}

	// δ��ʼ��
	if (!m_bInitOK)
	{
		return GT_INVALID;
	}

	// Ȩ��
	tagGuildMember* pMember	= m_pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}
	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bSetCommend)
	{
		return E_Guild_Power_NotEnough;
	}

	// ���ɵȼ�
	if (m_pGuild->GetGuildAtt().byLevel < 3)
	{
		return E_Guild_Level_Limit;
	}

	// ���üν�״̬
	if (m_bCommend != bSwitchON)
	{
		m_bCommend	= bSwitchON;

		tagNDBC_SetCommendation send;
		send.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
		send.bCommend	= m_bCommend;
		g_dbSession.Send(&send, send.dwSize);

		// �ϲ�㲥�ν�״̬
	}
	else
	{
		return E_GuildCommerce_Commend_Error;
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// �������̽���
//-----------------------------------------------------------------------------
VOID GuildCommerce::ProvideBonus()
{
	// δ��ʼ��
	if (!m_bInitOK)			return;

	// ���ɵȼ�
	if (m_pGuild->GetGuildAtt().byLevel < 3)	return;
	
	// ���а�
	if (m_vecRank.empty())	return;

	// �ν�״̬
	if (m_bCommend)
	{
		VectorRank vecResult(MAX_COMMEND_PLAYER);
		partial_sort_copy(m_vecRank.begin(), m_vecRank.end(), vecResult.begin(), vecResult.end(), RankCompare());

		for (INT n=1; n <= MAX_COMMEND_PLAYER; n++)
		{
			if (!P_VALID(vecResult[n-1]))
			{
				break;
			}

			// �����������żν�
			Role* pRole	= g_roleMgr.GetRolePtrByID(vecResult[n-1]->dwRoleID);
			switch (n)
			{
			case 1:
				m_pGuild->ChangeMemberContribution(vecResult[0]->dwRoleID, 20, TRUE);
				if (P_VALID(pRole))
				{
					pRole->GetCurMgr().IncWareSilver(vecResult[0]->nTael/100, ELCLD_Guild_Commerce);
				}
				else
				{
					CurrencyMgr::ModifyWareSilver(vecResult[0]->dwRoleID, vecResult[0]->nTael/100, ELCLD_Guild_Commerce);
				}
				break;

			case 2:
				m_pGuild->ChangeMemberContribution(vecResult[1]->dwRoleID, 10, TRUE);
				if (P_VALID(pRole))
				{
					pRole->GetCurMgr().IncWareSilver(vecResult[1]->nTael*5/1000, ELCLD_Guild_Commerce);
				}
				else
				{
					CurrencyMgr::ModifyWareSilver(vecResult[1]->dwRoleID, vecResult[1]->nTael*5/1000, ELCLD_Guild_Commerce);
				}
				break;

			case 3:
				m_pGuild->ChangeMemberContribution(vecResult[2]->dwRoleID, 5, TRUE);
				if (P_VALID(pRole))
				{
					pRole->GetCurMgr().IncWareSilver(vecResult[2]->nTael*2/1000, ELCLD_Guild_Commerce);
				}
				else
				{
					CurrencyMgr::ModifyWareSilver(vecResult[2]->dwRoleID, vecResult[2]->nTael*2/1000, ELCLD_Guild_Commerce);
				}
				break;

			default:
				break;
			}
		}
	}

	// �������������Ϣ
	RemoveCommerceRank(GT_INVALID);
}

//-----------------------------------------------------------------------------
// ����������Ϣ
//-----------------------------------------------------------------------------
DWORD GuildCommerce::LoadCommerceInfo( tagGuildCommerceInfo* pInfo, INT nInfoNum )
{
	if (nInfoNum < 0)
	{
		return GT_INVALID;
	}

	for (int n=0; n<nInfoNum; n++)
	{
		GuildCommodity* pCommodity	= new GuildCommodity;
		if (pCommodity->LoadCommodityInfo(&pInfo[n]) == E_Success)
		{
			m_mapCommodity.Add(pInfo[n].dwRoleID, pCommodity);
		}
	}

	// ��ʼ�����
	m_bInitOK	= TRUE;
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������������Ϣ
//-----------------------------------------------------------------------------
DWORD GuildCommerce::LoadCommerceRankInfo(tagCommerceRank *pInfo, INT nInfoNum)
{
	if (nInfoNum < 0)
	{
		return GT_INVALID;
	}

	for (int n=0; n<nInfoNum; n++)
	{
		Add2CommerceRank(pInfo[n].dwRoleID, pInfo[n].nTael, pInfo[n].nTimes, FALSE);		
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ���ָ������̻�
//-----------------------------------------------------------------------------
DWORD GuildCommerce::GetCommodityGoodInfo( Role* pRole, tagCommerceGoodInfo* pGoodInfo, INT& nGoodNum, INT32& nTael, INT& nLevel )
{
	if (!P_VALID(pRole) || !P_VALID(pGoodInfo))
	{
		return GT_INVALID;
	}

	// �ж����״̬
	if (!pRole->IsInRoleState(ERS_Commerce))
	{
		return E_GuildCommerce_Status_Error;
	}

	// ȡ����������
	GuildCommodity* pCommodity = GetCommodity(pRole->GetID());
	if (!P_VALID(pCommodity))
	{
		return E_GuildCommerce_Status_Error;
	}

	// ��ȡ�̻�������Ϣ
	if (!pCommodity->GetCommodityInfo(pGoodInfo, nGoodNum, nTael, nLevel))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ���̻Ṻ���̻�
//-----------------------------------------------------------------------------
DWORD GuildCommerce::BuyGoods( Role* pRole, DWORD dwNPCID, DWORD dwGoodID, BYTE byBuyNum )
{
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ���״̬
	if (!pRole->IsInRoleState(ERS_Commerce))
	{
		return E_GuildCommerce_Status_Error;
	}

	// ȡ���̻���Ϣ
	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(dwNPCID);
	if (!P_VALID(pCofC))
	{
		return E_CofC_NotExist;
	}

	// ����Ƿ���Ҫռ�����
	if (!m_pGuild->IsHoldCity(pCofC->GetHolderCity()))
	{
		return E_CofC_HoldCity_Limit;
	}

	// ������������������
	GuildCommodity* pCommodity = GetCommodity(pRole->GetID());
	if (!P_VALID(pCommodity))
	{
		return E_GuildCommerce_Status_Error;
	}

	DWORD dwErrorCode = pCommodity->IsFull(dwGoodID, byBuyNum);
	if (E_Success != dwErrorCode)
	{
		return dwErrorCode;
	}

	// �����̻�
	INT32 nCostTael = 0;
	dwErrorCode = pCofC->SellItem(pCommodity->GetTael(), dwGoodID, byBuyNum, nCostTael);
	if (E_Success != dwErrorCode)
	{
		return dwErrorCode;
	}

	// ����ҷ����̻�
	pCommodity->AddGood(dwGoodID, byBuyNum, pCofC->GetGoodPrice(dwGoodID));
	pCommodity->DecTael(nCostTael);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����̻����̻�
//-----------------------------------------------------------------------------
DWORD GuildCommerce::SellGoods( Role* pRole, DWORD dwNPCID, DWORD dwGoodID, BYTE bySellNum )
{
	if (!P_VALID(pRole) || bySellNum <= 0)
	{
		return GT_INVALID;
	}

	// ���״̬
	if (!pRole->IsInRoleState(ERS_Commerce))
	{
		return E_GuildCommerce_Status_Error;
	}

	// ȡ���̻���Ϣ
	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(dwNPCID);
	if (!P_VALID(pCofC))
	{
		return E_CofC_NotExist;
	}

	// ����Ƿ����ز��̻�
	if (pCofC->GetHolderCity() != 0)
	{
		return E_CofC_ItemCannotSell;
	}

	// ��������������
	GuildCommodity* pCommodity = GetCommodity(pRole->GetID());
	if (!P_VALID(pCommodity))
	{
		return E_GuildCommerce_Status_Error;
	}

	DWORD dwErrorCode = pCommodity->IsExist(dwGoodID, bySellNum);
	if (E_Success != dwErrorCode)
	{
		return dwErrorCode;
	}

	// �����̻�
	INT32 nCost = pCofC->GetGoodPrice(dwGoodID);
	if (!GT_VALID(nCost))
	{
		return E_CofC_ItemCannotSell;
	}

	// �۳�����̻�
	pCommodity->RemoveGood(dwGoodID, bySellNum, pCofC->GetGoodPrice(dwGoodID));
	pCommodity->IncTael(nCost*bySellNum);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ���̳�ʼ��Ϣ
//-----------------------------------------------------------------------------
DWORD GuildCommerce::GetCommerceBeginningInfo( DWORD dwRoleID, INT& nLevel, tagTaelInfo& sTaelInfo )
{
	GuildCommodity* pCommodity = GetCommodity(dwRoleID);
	if (!P_VALID(pCommodity))
	{
		return E_GuildCommerce_Status_Error;
	}

	nLevel		= pCommodity->GetLevel();
	sTaelInfo	= *(pCommodity->GetTaelInfo());

	return E_Success;
}

//-----------------------------------------------------------------------------
// ���浽���а�
//-----------------------------------------------------------------------------
VOID GuildCommerce::Add2CommerceRank( DWORD dwRoleID, INT32 nTael, INT nTimes /*= GT_INVALID*/, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID) || nTael < 0)
	{
		return;
	}

	tagCommerceRank* pRankInfo = NULL;
	VectorRank::iterator iter_rank	= m_vecRank.begin();
	for (; iter_rank != m_vecRank.end(); iter_rank++)
	{
		if (!P_VALID(*iter_rank))
		{
			continue;
		}

		if ((*iter_rank)->dwRoleID == dwRoleID)
		{
			pRankInfo	= *iter_rank;
			break;
		}
	}
	if (iter_rank == m_vecRank.end())
	{
		pRankInfo				= new tagCommerceRank;
		pRankInfo->dwRoleID		= dwRoleID;
		pRankInfo->nTael		= nTael;
		if (!GT_VALID(nTimes))
		{
			nTimes = 1;
		}
		pRankInfo->nTimes		= nTimes;
		m_vecRank.push_back(pRankInfo);
	}
	else
	{
		(*iter_rank)->nTael += nTael;
		(*iter_rank)->nTimes++;
	}

	// ���µ����ݿ�
	if (bSave2DB && P_VALID(pRankInfo))
	{
		tagNDBC_UpdateCommerceRank send_rank;
		send_rank.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
		send_rank.sRankInfo	= *pRankInfo;
		g_dbSession.Send(&send_rank, send_rank.dwSize);
	}
}

//-----------------------------------------------------------------------------
// �������������Ϣ
//-----------------------------------------------------------------------------
VOID GuildCommerce::RemoveCommerceRank( DWORD dwRoleID )
{
	if (GT_VALID(dwRoleID))
	{
		// �������������Ϣ�Ƿ����
		VectorRank::iterator iter_rank	= m_vecRank.begin();
		while (iter_rank != m_vecRank.end())
		{
			if (!P_VALID(*iter_rank))
			{
				iter_rank++;
				continue;
			}
			if ((*iter_rank)->dwRoleID == dwRoleID)
			{
				// ɾ����������Ϣ
				SAFE_DEL(*iter_rank);
				m_vecRank.erase(iter_rank);
				break;
			}
			iter_rank++;
		}
	}
	else
	{
		// ɾ������������Ϣ
		VectorRank::iterator iter_rank	= m_vecRank.begin();
		while (iter_rank != m_vecRank.end())
		{
			if (!P_VALID(*iter_rank))
			{
				iter_rank++;
				continue;
			}
			// ɾ����������Ϣ
			SAFE_DEL(*iter_rank);
			iter_rank++;
		}
		m_vecRank.clear();
	}

	// ֪ͨ���ݿ�
	tagNDBC_RemoveCommerceRank send_rank;
	send_rank.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
	send_rank.dwRoleID	= dwRoleID;
	g_dbSession.Send(&send_rank, send_rank.dwSize);
}