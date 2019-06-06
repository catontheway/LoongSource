//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_commodity.cpp
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ��������������
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../ServerDefine/msg_guild.h"
#include "../WorldDefine/msg_guild.h"

#include "att_res.h"
#include "db_session.h"
#include "guild_commodity.h"
#include "role_mgr.h"
#include "role.h"

//------------------------------------------------------------------------------
// ����������
//------------------------------------------------------------------------------
GuildCommodity::GuildCommodity()
{
	m_nCurTael	= 0;
	m_mapGoods.Clear();
}

GuildCommodity::~GuildCommodity()
{
	Destory();
}

//------------------------------------------------------------------------------
// ��ʼ��������
//------------------------------------------------------------------------------
BOOL GuildCommodity::Init( DWORD dwRoleID, INT nLevel, const tagTaelInfo* pTaelInfo /*= NULL*/, const tagRedoundInfo* pRedoundInfo /*= NULL*/ )
{
	if (!GT_VALID(dwRoleID))
	{
		return FALSE;
	}

	m_dwOwnerID	= dwRoleID;
	m_nLevel	= nLevel;
	m_mapGoods.Clear();

	if (P_VALID(pTaelInfo) && P_VALID(pRedoundInfo))
	{
		m_sRedoundInfo	= *pRedoundInfo;
		m_sTaelInfo		= *pTaelInfo;
		m_nCurTael		= pTaelInfo->nBeginningTael;
	}

	return TRUE;
}

VOID GuildCommodity::Destory()
{
	tagCommerceGoodInfo* pInfo	= NULL;
	MapGoods::TMapIterator iter	= m_mapGoods.Begin();
	while (m_mapGoods.PeekNext(iter, pInfo))
	{
		SAFE_DEL(pInfo);
	}
	m_mapGoods.Clear();
}

//------------------------------------------------------------------------------
// �����ݿ�������������Ϣ
//------------------------------------------------------------------------------
DWORD GuildCommodity::LoadCommodityInfo( tagGuildCommerceInfo* pLoadInfo )
{
	if (!P_VALID(pLoadInfo))
	{
		return GT_INVALID;
	}

	// ����
	m_dwOwnerID		= pLoadInfo->dwRoleID;

	// ��̬����
	const tagCommerceInfo* pInfo = g_attRes.GetGuildCommerceInfo(pLoadInfo->nLevel);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}
	m_nLevel		= pLoadInfo->nLevel;
	m_sTaelInfo		= pInfo->sTaelInfo;
	m_sRedoundInfo	= pInfo->sRedoundInfo;

	// ����
	m_nCurTael		= pLoadInfo->nTael;

	// �̻�
	m_mapGoods.Clear();
	for (int n=0; n<MAX_COMMODITY_CAPACITY; n++)
	{
		if (!pLoadInfo->sGoodInfo[n].IsValid())
		{
			break;
		}
		tagCommerceGoodInfo* pGoodInfo = new tagCommerceGoodInfo(pLoadInfo->sGoodInfo[n]);
		m_mapGoods.Add(pLoadInfo->sGoodInfo[n].dwGoodID, pGoodInfo);
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ��ǰ��������
//------------------------------------------------------------------------------
BOOL GuildCommodity::IncTael( INT32 nTael )
{
	if (nTael <= 0 || m_nCurTael > m_sTaelInfo.nMaxTael)
	{
		return FALSE;
	}

	m_nCurTael += nTael;
	if (m_nCurTael > m_sTaelInfo.nMaxTael || m_nCurTael <= 0)	// ���ǵ�INT32���
	{
		m_nCurTael = m_sTaelInfo.nMaxTael;
	}

	// ֪ͨ���ݿ�
	SaveTael2DB();

	// ֪ͨ�ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwOwnerID);
	if (P_VALID(pRole))
	{
		tagNS_GetCommodityInfo	send;
		send.nCurTael		= m_nCurTael;
		send.nLevel			= m_nLevel;
		send.nGoodNum		= 0;
		pRole->SendMessage(&send, send.dwSize);
	}

	return TRUE;
}

BOOL GuildCommodity::DecTael( INT32 nTael )
{
	if (nTael <= 0 || m_nCurTael <= 0)
	{
		return FALSE;
	}

	m_nCurTael -= nTael;
	if (m_nCurTael < 0)
	{
		m_nCurTael = 0;
	}

	// ֪ͨ���ݿ�
	SaveTael2DB();

	// ֪ͨ�ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwOwnerID);
	if (P_VALID(pRole))
	{
		tagNS_GetCommodityInfo	send;
		send.nCurTael		= m_nCurTael;
		send.nLevel			= m_nLevel;
		send.nGoodNum		= 0;
		pRole->SendMessage(&send, send.dwSize);
	}

	return TRUE;
}

//------------------------------------------------------------------------------
// ������Ʒ����
//------------------------------------------------------------------------------
BOOL GuildCommodity::AddGood( DWORD dwGoodID, BYTE byNum, INT32 nPrice /*= 0*/ )
{
	if (!GT_VALID(dwGoodID))
	{
		return FALSE;
	}

	tagCommerceGoodInfo* pGoodInfo = NULL;
	if (m_mapGoods.IsExist(dwGoodID))
	{
		// ȡ�õ�ǰ���̻�����
		pGoodInfo = m_mapGoods.Peek(dwGoodID);
		if (!P_VALID(pGoodInfo))
		{
			return FALSE;
		}
		INT16 n16Num = pGoodInfo->byGoodNum + byNum;
		if (n16Num > MAX_COMMODITY_GOOD_NUM)
		{
			n16Num = MAX_COMMODITY_GOOD_NUM;
		}
		pGoodInfo->byGoodNum = (BYTE)n16Num;
	}
	else if (m_mapGoods.Size() >= MAX_COMMODITY_CAPACITY)
	{
		return FALSE;
	}
	else
	{
		// byNum�����ܳ���255�����ﲻ�������ж�
		pGoodInfo = new tagCommerceGoodInfo;
		pGoodInfo->dwGoodID		= dwGoodID;
		pGoodInfo->byGoodNum	= byNum;
		pGoodInfo->nCost		= nPrice;
		m_mapGoods.Add(dwGoodID, pGoodInfo);
	}

	// ֪ͨ���ݿ�
	SaveCommodity2DB();

	// ֪ͨ�ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwOwnerID);
	if (P_VALID(pRole))
	{
		tagNS_GetCommodityInfo	send;
		send.nCurTael		= m_nCurTael;
		send.nLevel			= m_nLevel;
		send.nGoodNum		= 1;
		send.sGoodInfo[0]	= *pGoodInfo;
		pRole->SendMessage(&send, send.dwSize);
	}

	return TRUE;
}

BOOL GuildCommodity::RemoveGood( DWORD dwGoodID, BYTE byNum, INT32 nPrice /*= 0*/ )
{
	if (!GT_VALID(dwGoodID) || !m_mapGoods.IsExist(dwGoodID))
	{
		return FALSE;
	}

	tagCommerceGoodInfo* pGoodInfo = m_mapGoods.Peek(dwGoodID);
	if (!P_VALID(pGoodInfo))
	{
		return FALSE;
	}

	BYTE byRemainNum = pGoodInfo->byGoodNum;
	if (byRemainNum < byNum)
	{
		return FALSE;
	}
	else
	{
		pGoodInfo->byGoodNum	= byRemainNum - byNum;
		pGoodInfo->nCost		= nPrice;
	}

	// ֪ͨ�ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwOwnerID);
	if (P_VALID(pRole))
	{
		tagNS_GetCommodityInfo	send;
		send.nCurTael		= m_nCurTael;
		send.nLevel			= m_nLevel;
		send.nGoodNum		= 1;
		send.sGoodInfo[0]	= *pGoodInfo;
		pRole->SendMessage(&send, send.dwSize);
	}

	if (pGoodInfo->byGoodNum == 0)
	{
		m_mapGoods.Erase(dwGoodID);
		SAFE_DEL(pGoodInfo);
	}

	// ֪ͨ���ݿ�
	SaveCommodity2DB();

	return TRUE;
}

//------------------------------------------------------------------------------
// ��ȡ�������̻���Ϣ(��������)
//------------------------------------------------------------------------------
DWORD GuildCommodity::GetCommodityInfo( tagCommerceGoodInfo* pGoods, INT& nGoodNum, INT32& nTael, INT32& nLevel )
{
	// �ϲ㱣֤��������
	if (!P_VALID(pGoods))
	{
		return GT_INVALID;
	}

	// ����
	nTael	= m_nCurTael;
	nLevel	= m_nLevel;

	// �̻�
	nGoodNum		= 0;

	DWORD dwGoodID	= GT_INVALID;
	tagCommerceGoodInfo* pGoodInfo = NULL;

	MapGoods::TMapIterator iter = m_mapGoods.Begin();
	while (m_mapGoods.PeekNext(iter, dwGoodID, pGoodInfo))
	{
		if (!P_VALID(pGoodInfo))
		{
			m_mapGoods.Erase(dwGoodID);
			continue;
		}

		if (!pGoodInfo->IsValid() || nGoodNum >= MAX_COMMODITY_CAPACITY)
		{
			m_mapGoods.Erase(dwGoodID);
			continue;
		}

		pGoods[nGoodNum++]	= *pGoodInfo;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ���ݿ����
//------------------------------------------------------------------------------
VOID GuildCommodity::SaveTael2DB()
{
	tagNDBC_ChangeTael send;
	send.dwRoleID	= m_dwOwnerID;
	send.nTael		= m_nCurTael;

	g_dbSession.Send(&send, send.dwSize);
}

VOID GuildCommodity::SaveCommodity2DB()
{
	tagNDBC_SaveCommodity send;
	send.dwRoleID	= m_dwOwnerID;

	DWORD dwGoodID					= GT_INVALID;
	tagCommerceGoodInfo* pGoodInfo	= NULL;
	INT nGoodType					= 0;

	MapGoods::TMapIterator iter = m_mapGoods.Begin();
	while (m_mapGoods.PeekNext(iter, dwGoodID, pGoodInfo))
	{
		if (!P_VALID(pGoodInfo))
		{
			m_mapGoods.Erase(dwGoodID);
			continue;
		}
		if (!pGoodInfo->IsValid() || nGoodType >= MAX_COMMODITY_CAPACITY)
		{
			m_mapGoods.Erase(dwGoodID);
			continue;
		}
		send.sGoodInfo[nGoodType++] = *pGoodInfo;
	}
	// ��λ����
	for (int n=nGoodType; n<MAX_COMMODITY_CAPACITY; n++)
	{
		send.sGoodInfo[n].dwGoodID	= GT_INVALID;
		send.sGoodInfo[n].byGoodNum	= 0;
		send.sGoodInfo[n].nCost		= 0;
	}

	g_dbSession.Send(&send, send.dwSize);
}

//------------------------------------------------------------------------------
// �Ƿ�����ɸ��̻�
//------------------------------------------------------------------------------
DWORD GuildCommodity::IsFull( DWORD dwGoodID, BYTE byNum )
{
	tagCommerceGoodInfo* pGoodInfo = m_mapGoods.Peek(dwGoodID);

	INT nResultNum = byNum;
	if (P_VALID(pGoodInfo))
	{
		nResultNum += pGoodInfo->byGoodNum;
		if (nResultNum > MAX_COMMODITY_GOOD_NUM)
		{
			return E_GuildCommodity_ItemMaxHold;
		}
	}
	else if (GetGoodsNum() >= MAX_COMMODITY_CAPACITY)
	{
		return E_GuildCommodity_NotEnough_Space;
	}
	else if (nResultNum > MAX_COMMODITY_GOOD_NUM)
	{
		return E_GuildCommodity_NotEnough_Space;
	}
	
	return E_Success;
}

//------------------------------------------------------------------------------
// �̻��Ƿ��㹻
//------------------------------------------------------------------------------
DWORD GuildCommodity::IsExist( DWORD dwGoodID, BYTE byNum )
{
	tagCommerceGoodInfo* pGoodInfo = m_mapGoods.Peek(dwGoodID);
	if (!P_VALID(pGoodInfo))
	{
		return E_CofC_ItemNotFind;
	}
	else if (pGoodInfo->byGoodNum < byNum)
	{
		return E_CofC_ItemNotEnough;
	}
	
	return E_Success;
}

//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
INT32 GuildCommodity::GetGain()
{
	if (m_nCurTael > m_sTaelInfo.nBeginningTael)
	{
		return m_nCurTael - m_sTaelInfo.nBeginningTael;
	}

	return 0;
}

//------------------------------------------------------------------------------
// �����ͷ�
//------------------------------------------------------------------------------
VOID GuildCommodity::DeadPenalty()
{
	// �����ͷ�
	DecTael((INT32)(m_nCurTael * 0.2f + 0.5f));

	// �̻��ͷ�

	// ͳ����Ҫ�ͷ����̻�����
	MapGoods::TMapIterator iter = m_mapGoods.Begin();
	INT32 nGoodNum = 0;
	tagCommerceGoodInfo* pGoodInfo = NULL;
	while (m_mapGoods.PeekNext(iter, pGoodInfo))
	{
		if (!P_VALID(pGoodInfo))
			continue;

		nGoodNum += pGoodInfo->byGoodNum;
	}

	nGoodNum = (INT32)(nGoodNum * 0.2f);

	// ���ѡȡ�̻��۳�
	DWORD dwGoodID = GT_INVALID;
	while (nGoodNum > 0)
	{
		m_mapGoods.RandPeek(dwGoodID, pGoodInfo);
		if (!GT_VALID(dwGoodID) || !P_VALID(pGoodInfo))
		{
			continue;
		}

		// �������۳����̻�������
		BYTE byNum = IUTIL->RandomInRange(1, min(pGoodInfo->byGoodNum, nGoodNum));

		// �۳��̻�
		RemoveGood(pGoodInfo->dwGoodID, byNum);

		nGoodNum -= byNum;
	}
}