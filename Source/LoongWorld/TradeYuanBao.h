//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: TradeYuanBao.h
// author: wjiang
// actor:
// data: 2009-3-3
// last:
// brief: Ԫ��������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/currency_define.h"
#include "currency.h"
#include <set>

//--------------------------------------------------------------------------------------------
// ���Ԫ�����׵Ľ����˻�
//--------------------------------------------------------------------------------------------
struct tagYuanBaoAccount
{
	DWORD				dwRoleID;				//���ID
	Currency<INT>		YuanBao;				//Ԫ������
	Currency<INT64>		Silver;					//��Ǯ����
	BOOL				bSellOrder;				//�Ƿ��г��۶���
	BOOL				bBuyOrder;				//�Ƿ����չ�����
	DWORD				dwQuestTick;			//��ѯ����ʱ�䣨��ֹ�ͻ���Ƶ����ѯ���ܶ�����
	tagYuanBaoAccount(DWORD dwID):YuanBao(0, MAX_YB_NUM), Silver(0, MAX_SILVER_NUM)
	{
		dwRoleID = dwID;
		bSellOrder = FALSE;
		bBuyOrder = FALSE;
		dwQuestTick = 0;
	}
	tagYuanBaoAccount(DWORD dwID, INT32 nYuanBao, INT32 nGold, INT32 nSilver, BOOL bSell, BOOL bBuy):YuanBao(nYuanBao, MAX_YB_NUM),
					Silver((MGold2Silver(nGold)+(INT64)nSilver), MAX_SILVER_NUM)
	{
		dwRoleID = dwID;
		bSellOrder = bSell;
		bBuyOrder = bBuy;
		dwQuestTick = 0;
	}

	// ��ƿͻ��˲�ѯʱ��
	VOID			SetQuestTick(DWORD dwTick) { dwQuestTick = dwTick; }
	DWORD			GetQuestTick() { return dwQuestTick; }

	// �����Ƿ��ύ������
	VOID			SetSellOrder(BOOL bSubmit);
	VOID			SetBuyOrder(BOOL bSubmit);

	// �õ��˻���ǰ��Ǯ��
	inline INT		GetAccountYB() { return YuanBao.GetCur(); }
	inline INT64	GetAccountSilver() { return Silver.GetCur(); }
	// ���˻���Ǯ
	BOOL			IncAccountYB(INT nYuanBao, DWORD dwCmdID, BOOL bNoticeClient=FALSE);
	BOOL			IncAccountSilver(INT64 n64Silver, DWORD dwCmdID, BOOL bNoticeClient=FALSE);
	// ���˻�ȡǮ
	BOOL			DecAccountYuanBao(INT32 nYuanBao, DWORD dwCmdID, BOOL bNoticeClient=FALSE);
	BOOL			DecAccountSilver(INT64 n64Silver, DWORD dwCmdID, BOOL bNoticeClient=FALSE);
	// ���Դ���Ľ�Ǯ��Ŀ
	INT64			GetCanIncAccountSilver()	{ return Silver.GetMax() - GetAccountSilver(); }
	INT				GetCanIncAccountYB()	    { return YuanBao.GetMax() - GetAccountYB(); }
};

//--------------------------------------------------------------------------------------------
// Ԫ�����۶�������
//--------------------------------------------------------------------------------------------
struct SellOrderCmp
{
	bool operator()(tagYuanBaoOrder* pOrder1, tagYuanBaoOrder* pOrder2) const
	{
		if(pOrder1->nPrice < pOrder2->nPrice)
			return TRUE;
		else if(pOrder1->nPrice == pOrder2->nPrice)
		{
			if(pOrder1->dwID < pOrder2->dwID)
				return TRUE;
		}
		return FALSE;
	}
};

//--------------------------------------------------------------------------------------------
// Ԫ���չ���������
//--------------------------------------------------------------------------------------------
struct BuyOrderCmp
{
	bool operator()(tagYuanBaoOrder* pOrder1, tagYuanBaoOrder* pOrder2) const
	{
		if(pOrder1->dwID < pOrder2->dwID)
			return TRUE;
		return FALSE;
	}
};

//--------------------------------------------------------------------------------------------
// ���Ԫ��������
//--------------------------------------------------------------------------------------------
class CTradeYB
{
public:
	CTradeYB() {}
	~CTradeYB();
	BOOL	Init();
	VOID	Destroy();
	VOID	OnHour();

	tagYuanBaoAccount*		GetYBAccount(DWORD dwRoleID) { return m_mapYBAccount.Peek(dwRoleID); }
	tagYuanBaoOrder*		GetYBSellOrder(DWORD dwRoleID);
	tagYuanBaoOrder*		GetYBBuyOrder(DWORD dwRoleID);
	tagYuanBaoAccount*		CreateTradeAccount(DWORD dwRoleID);
	tagYuanBaoAccount*		CreateTradeAccount(const tagYBAccount *pYBAccount);
	tagYuanBaoOrder*		CreateYBOrder(DWORD dwRoleID, EYBOTYPE eYBOType, INT nPrice, INT nNum);
	VOID					LoadYOOrder(tagYuanBaoOrder* pYBOrder);
	VOID					RefreshYBOrder(tagYuanBaoOrder* pYBOrder, DWORD dwRoleID, INT nPrice, INT nNum, BOOL bNoticeClient);
	VOID					DeleteYBOrder(tagYuanBaoOrder* pYBOrder, EYBOMODE eYBOMode);
	VOID					DealYBSell(tagYuanBaoOrder* pYBOrder);
	VOID					DealYBBuy(tagYuanBaoOrder* pYBBuyOrder);
	VOID					DealYBTrade(tagYuanBaoOrder* pYBOrder, EYBOTYPE eYBOType);
	DWORD					IsCanTradeYB();

	VOID					SynSellPriceList(Role *pRole);
	VOID					SynBuyPriceList(Role *pRole);
	VOID					SynYBAccount(Role *pRole);

	VOID					SetMaxOrderIndex(DWORD dwIndex)	{ m_dwMaxOrderID = dwIndex; };

private:
	VOID					CalYBSellList();
	VOID					CalYBBuyList();
	VOID					InserYBSellList(INT nPrice, INT nNum);
	VOID					InserYBBuyList(INT nPrice, INT nNum);
	VOID					RemoveYBBuyList(INT nPrice, INT nNum, DWORD dwRoleID, BOOL bNoticeClient=FALSE);
	VOID					RemoveYBSellList(INT nPrice, INT nNum, DWORD dwRoleID, BOOL bNoticeClient=FALSE);

private:
	DWORD										m_dwMaxOrderID;					// ��󶩵�����
	std::set<tagYuanBaoOrder*, SellOrderCmp>	m_setYBSell;					// Ԫ�����۶���
	std::set<tagYuanBaoOrder*, BuyOrderCmp>		m_setYBBuy;						// Ԫ���չ�����
	TMap<DWORD, tagYuanBaoAccount*>				m_mapYBAccount;					// ���Ԫ�������˻�
	TMap<DWORD,	DWORD>							m_mapOrder2Role;				// ����ID�����ID�Ķ�Ӧ����
	TMap<INT, INT>								m_mapYBSellList;				// Ԫ�������б�����ͬ���ͻ��� ���� ������
	TMap<INT, INT>								m_mapYBBuyList;					// Ԫ���չ��б�����ͬ���ͻ��� ���� ������
};

inline tagYuanBaoOrder* CTradeYB::GetYBSellOrder(DWORD dwRoleID)
{
	tagYuanBaoOrder* pSellOrder = (tagYuanBaoOrder*)GT_INVALID;
	std::set<tagYuanBaoOrder*, SellOrderCmp>::iterator it = m_setYBSell.begin();
	while(it != m_setYBSell.end())
	{
		pSellOrder = *it;
		if(pSellOrder->dwRoleID == dwRoleID)
		{
			return pSellOrder;
		}
		++it;
	}

	return (tagYuanBaoOrder*)GT_INVALID;
}

inline tagYuanBaoOrder* CTradeYB::GetYBBuyOrder(DWORD dwRoleID)
{
	tagYuanBaoOrder* pBuyOrder = (tagYuanBaoOrder*)GT_INVALID;
	std::set<tagYuanBaoOrder*, BuyOrderCmp>::iterator it = m_setYBBuy.begin();
	while(it != m_setYBBuy.end())
	{
		pBuyOrder = *it;
		if(pBuyOrder->dwRoleID == dwRoleID)
		{
			return pBuyOrder;
		}
		++it;
	}

	return (tagYuanBaoOrder*)GT_INVALID;
}

extern CTradeYB g_tradeYB;