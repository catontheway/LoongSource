//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: TradeYuanBao.cpp
// author: wjiang
// actor:
// data: 2009-3-3
// last:
// brief: Ԫ��������
//-------------------------------------------------------------------------------------------------------

#include "StdAfx.h"
#include "TradeYuanBao.h"
#include "world.h"
#include "role_mgr.h"
#include "role.h"
#include "db_session.h"

#include "../WorldDefine/msg_mall.h"
#include "../ServerDefine/msg_yuanbao.h"

CTradeYB g_tradeYB;

VOID tagYuanBaoAccount::SetSellOrder(BOOL bSubmit) 
{ 
	bSellOrder = bSubmit; 

	// ͬ�����ݿ�
	tagNDBC_SynSubmitSell	sendDB;
	sendDB.dwRoleID = dwRoleID;
	sendDB.bSellOrder = (bool)bSellOrder;
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}

VOID tagYuanBaoAccount::SetBuyOrder(BOOL bSubmit) 
{
	bBuyOrder = bSubmit;

	// ͬ�����ݿ�
	tagNDBC_SynSubmitBuy	sendDB;
	sendDB.dwRoleID = dwRoleID;
	sendDB.bBuyOrder = (bool)bBuyOrder;
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}

BOOL tagYuanBaoAccount::IncAccountYB(INT32 nYuanBao, DWORD dwCmdID, BOOL bNoticeClient)
{
	if(nYuanBao <= 0)
	{
		return FALSE;
	}

	INT32 nInc = YuanBao.Gain(nYuanBao);

	// ��¼log//??
	
	// ��ͻ��˷���Ϣ
	if(bNoticeClient == TRUE)
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			tagNS_SynAccoutYB	send;
			send.nYuanBao = GetAccountYB();
			pRole->SendMessage(&send, sizeof(send));
		}
	}

	// ͬ�����ݿ�
	tagNDBC_SynAccountYB	sendDB;
	sendDB.dwRoleID = dwRoleID;
	sendDB.nYuanBao = GetAccountYB();
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return TRUE;
}

BOOL tagYuanBaoAccount::IncAccountSilver(INT64 n64Silver, DWORD dwCmdID, BOOL bNoticeClient)
{
	if(n64Silver <= 0)
	{
		return FALSE;
	}

	INT64 n64Inc = Silver.Gain(n64Silver);

	// ��¼log//??

	// ��ͻ��˷���Ϣ
	if(bNoticeClient == TRUE)
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			tagNS_SynAccoutSilver	send;
			send.nSilver = GetAccountSilver();
			pRole->SendMessage(&send, sizeof(send));
		}
	}

	// ͬ�����ݿ�
	tagNDBC_SynAccountSilver	sendDB;
	sendDB.dwRoleID = dwRoleID;
	sendDB.Money.nGold = MSilver2DBGold(GetAccountSilver());
	sendDB.Money.nSilver = MSilver2DBSilver(GetAccountSilver());
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return TRUE;
}

BOOL tagYuanBaoAccount::DecAccountYuanBao(INT32 nYuanBao, DWORD dwCmdID, BOOL bNoticeClient)
{
	if(nYuanBao <= 0)
	{
		return FALSE;
	}

	INT64 n64Dec = YuanBao.Spend(nYuanBao);

	// ��¼log//??

	// ��ͻ��˷���Ϣ
	if(bNoticeClient == TRUE)
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			tagNS_SynAccoutYB	send;
			send.nYuanBao = GetAccountYB();
			pRole->SendMessage(&send, sizeof(send));
		}
	}

	// ͬ�����ݿ�
	tagNDBC_SynAccountYB	sendDB;
	sendDB.dwRoleID = dwRoleID;
	sendDB.nYuanBao = GetAccountYB();
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return TRUE;
}

BOOL tagYuanBaoAccount::DecAccountSilver(INT64 n64Silver, DWORD dwCmdID, BOOL bNoticeClient)
{
	if(n64Silver <= 0)
	{
		return FALSE;
	}

	INT64 n64Dec = Silver.Spend(n64Silver);

	// ��¼log//??

	// ��ͻ��˷���Ϣ
	if(bNoticeClient == TRUE)
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			tagNS_SynAccoutSilver	send;
			send.nSilver = GetAccountSilver();
			pRole->SendMessage(&send, sizeof(send));
		}
	}

	// ͬ�����ݿ�
	tagNDBC_SynAccountSilver	sendDB;
	sendDB.dwRoleID = dwRoleID;
	sendDB.Money.nGold = MSilver2DBGold(GetAccountSilver());
	sendDB.Money.nSilver = MSilver2DBSilver(GetAccountSilver());
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return TRUE;
}


BOOL CTradeYB::Init()
{
	m_mapYBAccount.Clear();
	m_setYBBuy.clear();
	m_setYBSell.clear();
	m_mapYBBuyList.Clear();
	m_mapYBSellList.Clear();
	m_mapOrder2Role.Clear();

	m_dwMaxOrderID	= 0;

	return TRUE;
}

CTradeYB::~CTradeYB()
{
	Destroy();
}

//-------------------------------------------------------------------------------------------------------
// ÿСʱִ��һ��
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::OnHour()
{
	// �������׶���
	tagYuanBaoOrder*	pYBOrder = NULL;
	tagDWORDTime		cur_time = GetCurrentDWORDTime();
	DWORD				dwCloseTime = DecreaseTime(cur_time, 2 * 24 * 60 * 60);

	// �����ύ��������ʱ�䣬��Ϊ�ر�״̬
	std::set<tagYuanBaoOrder*, BuyOrderCmp>::iterator itOrderBuy = m_setYBBuy.begin();
	while(itOrderBuy != m_setYBBuy.end())
	{
		pYBOrder = *itOrderBuy;
		itOrderBuy++;

		if(dwCloseTime >= pYBOrder->dwStartTime)
			DeleteYBOrder(pYBOrder, EYBOM_Close);
	}

	std::set<tagYuanBaoOrder*, SellOrderCmp>::iterator itOrderSell = m_setYBSell.begin();
	while(itOrderSell != m_setYBSell.end())
	{
		pYBOrder = *itOrderSell;
		itOrderSell++;

		if(dwCloseTime >= pYBOrder->dwStartTime)
			DeleteYBOrder(pYBOrder, EYBOM_Close);
	}

	// ɾ�����ݿ���ʱ�䳬����������ж���
	DWORD	dwDeleteTime = DecreaseTime(cur_time, 7 * 24 * 60 * 60);
	tagNDBC_DeleteYBOrder	sendDB;
	sendDB.dwDeleteTime = dwDeleteTime;
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}

VOID CTradeYB::Destroy()
{
	tagYuanBaoAccount* pYBAccount = NULL;
	TMap<DWORD, tagYuanBaoAccount*>::TMapIterator itAccount = m_mapYBAccount.Begin();
	while( m_mapYBAccount.PeekNext(itAccount, pYBAccount) )
	{
		SAFE_DEL(pYBAccount);
	}

	tagYuanBaoOrder* pYBOrder = NULL;
	std::set<tagYuanBaoOrder*, BuyOrderCmp>::iterator itOrderBuy = m_setYBBuy.begin();
	while(itOrderBuy != m_setYBBuy.end())
	{
		SAFE_DEL(*itOrderBuy);
		itOrderBuy++;
	}
	m_setYBBuy.clear();

	std::set<tagYuanBaoOrder*, SellOrderCmp>::iterator itOrderSell = m_setYBSell.begin();
	while(itOrderSell != m_setYBSell.end())
	{
		SAFE_DEL(*itOrderSell);
		itOrderSell++;
	}
	m_setYBSell.clear();
	
	m_mapYBBuyList.Clear();
	m_mapYBSellList.Clear();
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
tagYuanBaoAccount* CTradeYB::CreateTradeAccount(DWORD dwRoleID)
{
	tagYuanBaoAccount *pYBAccount = new tagYuanBaoAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return (tagYuanBaoAccount*)GT_INVALID;

	m_mapYBAccount.Add(dwRoleID, pYBAccount);

	// insert to db
	tagNDBC_InsertAccount	sendDB;
	sendDB.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return pYBAccount;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
tagYuanBaoAccount* CTradeYB::CreateTradeAccount(const tagYBAccount *pYBAccount)
{
	tagYuanBaoAccount *pYuanBaoAccount = new tagYuanBaoAccount(pYBAccount->dwRoleID, pYBAccount->nYuanBao,
						pYBAccount->nGold, pYBAccount->nSilver, pYBAccount->bSellOrder, pYBAccount->bBuyOrder);
	if(!P_VALID(pYuanBaoAccount))
		return (tagYuanBaoAccount*)GT_INVALID;

	m_mapYBAccount.Add(pYBAccount->dwRoleID, pYuanBaoAccount);

	return pYuanBaoAccount;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������б�
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::CalYBSellList()
{
	tagYuanBaoOrder* pYBOrder = NULL;

	set<tagYuanBaoOrder*, SellOrderCmp>::iterator itOrderSell = m_setYBSell.begin();
	while( itOrderSell != m_setYBSell.end())
	{
		pYBOrder = *itOrderSell;
		InserYBSellList(pYBOrder->nPrice, pYBOrder->GetRemainNum());
		++itOrderSell;
	}
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ���չ��б�
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::CalYBBuyList()
{
	tagYuanBaoOrder* pYBOrder = NULL;

	set<tagYuanBaoOrder*, BuyOrderCmp>::iterator itOrderBuy = m_setYBBuy.begin();
	while( itOrderBuy != m_setYBBuy.end())
	{
		pYBOrder = *itOrderBuy;
		InserYBBuyList(pYBOrder->nPrice, pYBOrder->GetRemainNum());
		++itOrderBuy;
	}
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������б�
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::InserYBSellList(INT nPrice, INT nNum)
{
	m_mapYBSellList.ModifyValue(nPrice, nNum);
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ���չ��б�
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::InserYBBuyList(INT nPrice, INT nNum)
{
	m_mapYBBuyList.ModifyValue(nPrice, nNum);
}

//-------------------------------------------------------------------------------------------------------
// ɾ��Ԫ�������б���ͬ�۸�Ķ���
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::RemoveYBSellList(INT nPrice, INT nNum, DWORD dwRoleID, BOOL bNoticeClient)
{
	m_mapYBSellList.ModifyValue(nPrice, -nNum);
	INT		nTotalNum = m_mapYBSellList.Peek(nPrice);

	if(nTotalNum == 0)
	{
		m_mapYBSellList.Erase(nPrice);
	}

	if(nTotalNum < 0)
	{
		ILOG->Write(_T("YuanBao Num in m_mapYBSellList < 0  %u\r\n"), nPrice);
	}

	// ͬ���ͻ���
	if(bNoticeClient == TRUE)
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			tagNS_SynSellPriceList	send;
			send.nPrice = nPrice;
			send.nNum = nTotalNum;
			pRole->SendMessage(&send, send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ɾ��Ԫ���չ��б���ͬ�۸�Ķ���(dwRoleID, ��Ҫͬ�������ID��
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::RemoveYBBuyList(INT nPrice, INT nNum, DWORD dwRoleID, BOOL bNoticeClient)
{
	m_mapYBBuyList.ModifyValue(nPrice, -nNum);
	INT		nTotalNum = m_mapYBBuyList.Peek(nPrice);

	if(nTotalNum == 0)
	{
		m_mapYBBuyList.Erase(nPrice);
	}

	if(nTotalNum < 0)
	{
		ILOG->Write(_T("YuanBao Num in m_mapYBBuyList < 0  %u\r\n"), nPrice);
	}

	// ͬ���ͻ���
	if(bNoticeClient == TRUE)
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			tagNS_SynBuyPriceList	send;
			send.nPrice = nPrice;
			send.nNum = nTotalNum;
			pRole->SendMessage(&send, send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
tagYuanBaoOrder* CTradeYB::CreateYBOrder(DWORD dwRoleID, EYBOTYPE eYBOType, INT nPrice, INT nNum)
{
	tagYuanBaoOrder* pYBOrder = new tagYuanBaoOrder;
	if(!P_VALID(pYBOrder))		
		return (tagYuanBaoOrder*)GT_INVALID;

	if(eYBOType != 	EYBOT_BUY && eYBOType != EYBOT_SELL)
		return (tagYuanBaoOrder*)GT_INVALID;

	pYBOrder->dwID = m_dwMaxOrderID;
	pYBOrder->dwRoleID = dwRoleID;
	pYBOrder->eYBOType = eYBOType;
	pYBOrder->nPrice = nPrice;
	pYBOrder->nNum = nNum;
	pYBOrder->eYBOMode = EYBOM_Submit;
	pYBOrder->dwStartTime = g_world.GetWorldTime();

	if(eYBOType == 	EYBOT_BUY)
	{
		m_setYBBuy.insert(pYBOrder);
		InserYBBuyList(nPrice, nNum);
	}
	else
	{
		m_setYBSell.insert(pYBOrder);
		InserYBSellList(nPrice, nNum);
	}

	// insert to db
	tagNDBC_InsertOrder		sendDB;
	sendDB.YBOrder.dwID = pYBOrder->dwID;
	sendDB.YBOrder.dwRoleID = pYBOrder->dwRoleID;
	sendDB.YBOrder.nType = pYBOrder->eYBOType;
	sendDB.YBOrder.nPrice = pYBOrder->nPrice;
	sendDB.YBOrder.nNum = pYBOrder->nNum;
	sendDB.YBOrder.nYBMode = pYBOrder->eYBOMode;
	sendDB.YBOrder.dwStartTime = pYBOrder->dwStartTime;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// ���붩��ID�����ID�Ķ�Ӧ����
	m_mapOrder2Role.Add(m_dwMaxOrderID, dwRoleID);
	// ����������һ
	++m_dwMaxOrderID;

	return pYBOrder;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::LoadYOOrder(tagYuanBaoOrder* pYuanBaoOrder)
{
	if(!P_VALID(pYuanBaoOrder))
		return;

	tagYuanBaoOrder* pYBOrder = new tagYuanBaoOrder;
	if(!P_VALID(pYBOrder))		
		return;

	*pYBOrder = *pYuanBaoOrder;

	if(pYBOrder->eYBOType == EYBOT_BUY)
	{
		m_setYBBuy.insert(pYBOrder);
		InserYBBuyList(pYBOrder->nPrice, pYBOrder->GetRemainNum());
	}
	else
	{
		m_setYBSell.insert(pYBOrder);
		InserYBSellList(pYBOrder->nPrice, pYBOrder->GetRemainNum());
	}

	// ���붩��ID�����ID�Ķ�Ӧ����
	m_mapOrder2Role.Add(pYBOrder->dwID, pYBOrder->dwRoleID);

	// �õ���󶩵�����
	//if(pYBOrder->dwID > m_dwMaxOrderID)
	//	m_dwMaxOrderID = pYBOrder->dwID +1;
}

//-------------------------------------------------------------------------------------------------------
// ������ر�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::DeleteYBOrder(tagYuanBaoOrder* pYBOrder, EYBOMODE eYBOMode)
{
	if(eYBOMode != 	EYBOM_Cancel && eYBOMode != EYBOM_Close)
		return;

	BOOL bNoticeClient = FALSE;
	if(EYBOM_Cancel == eYBOMode)
		bNoticeClient = TRUE;

	// �õ���ҽ����˻�
	tagYuanBaoAccount* pYBAccount = m_mapYBAccount.Peek(pYBOrder->dwRoleID);
	if(!P_VALID(pYBAccount))
		return;

	if(pYBOrder->eYBOType == EYBOT_BUY)
	{
		pYBOrder->eYBOMode = eYBOMode;
		pYBOrder->dwEndTime = g_world.GetWorldTime();

		RemoveYBBuyList(pYBOrder->nPrice, pYBOrder->GetRemainNum(), pYBOrder->dwRoleID, bNoticeClient);
		m_setYBBuy.erase(pYBOrder);
		pYBAccount->SetBuyOrder(FALSE);
	}
	else
	{
		pYBOrder->eYBOMode = eYBOMode;
		pYBOrder->dwEndTime = g_world.GetWorldTime();

		RemoveYBSellList(pYBOrder->nPrice, pYBOrder->GetRemainNum(), pYBOrder->dwRoleID, bNoticeClient);
		m_setYBSell.erase(pYBOrder);
		pYBAccount->SetSellOrder(FALSE);
	}

	// Update to db
	tagNDBC_CompleteOrder	sendDB;
	sendDB.CompleteOrder.dwID = pYBOrder->dwID;
	sendDB.CompleteOrder.nYBOMode = eYBOMode;
	sendDB.CompleteOrder.dwEndTime = pYBOrder->dwEndTime;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// ɾ������ID�����ID�Ķ�Ӧ����
	m_mapOrder2Role.Erase(pYBOrder->dwID);
	SAFE_DEL(pYBOrder);
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���(���ض���ʣ��Ԫ������
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::RefreshYBOrder(tagYuanBaoOrder* pYBOrder, DWORD dwRoleID, INT nPrice, INT nNum, BOOL bNoticeClient)
{
	if(nNum == 0)
		return;

	if(!P_VALID(pYBOrder))
		return;

	tagYuanBaoAccount* pYBAccount = m_mapYBAccount.Peek(pYBOrder->dwRoleID);
	if(!P_VALID(pYBAccount))
		return;

	if(pYBOrder->eYBOType == EYBOT_BUY)
	{
		pYBOrder->nAvgPrice = (pYBOrder->nAvgPrice * pYBOrder->nDealNum + nPrice * nNum) / (pYBOrder->nDealNum + nNum);
		pYBOrder->nDealNum += nNum;
		
		// �������Ԫ�������˻�
		pYBAccount->IncAccountYB(nNum, GT_INVALID, bNoticeClient);
		pYBAccount->DecAccountSilver(nPrice*nNum, GT_INVALID, bNoticeClient);

		// ɾ��Ԫ���չ��б���ͬ�۸�Ķ���
		RemoveYBBuyList(pYBOrder->nPrice, nNum, dwRoleID, TRUE);
	}
	else
	{
		pYBOrder->nAvgPrice = (pYBOrder->nAvgPrice * pYBOrder->nDealNum + nPrice * nNum) / (pYBOrder->nDealNum + nNum);
		pYBOrder->nDealNum += nNum;

		// �������Ԫ�������˻�
		pYBAccount->DecAccountYuanBao(nNum, GT_INVALID, bNoticeClient);
		pYBAccount->IncAccountSilver(nPrice*nNum, GT_INVALID, bNoticeClient);

		// ɾ��Ԫ�������б���ͬ�۸�Ķ���
		RemoveYBSellList(pYBOrder->nPrice, nNum, dwRoleID, TRUE);
	}
	
	// Update to db
	tagNDBC_RefreshOrder	sendDB;
	sendDB.RefreshOrder.dwID = pYBOrder->dwID;
	sendDB.RefreshOrder.nAvgPrice = pYBOrder->nAvgPrice;
	sendDB.RefreshOrder.nDealNum = pYBOrder->nDealNum;
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ��
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::DealYBSell(tagYuanBaoOrder* pYBSellOrder)
{
	BOOL	bNoticeClient = FALSE;	

	// �����չ�������Ѱ��ƥ��Ķ���
	tagYuanBaoOrder* pYBBuyOrder = (tagYuanBaoOrder*)GT_INVALID;
	std::set<tagYuanBaoOrder*, BuyOrderCmp>::iterator itBuyOrder = m_setYBBuy.begin();
	while (itBuyOrder != m_setYBBuy.end())
	{
		pYBBuyOrder = (*itBuyOrder);
		++itBuyOrder;

		// ���׶����໥ƥ��
		if (pYBBuyOrder->nPrice >= pYBSellOrder->nPrice && pYBBuyOrder->dwRoleID != pYBSellOrder->dwRoleID)
		{
			//�չ�Ԫ����
			tagYuanBaoAccount* pYBAccount = m_mapYBAccount.Peek(pYBBuyOrder->dwRoleID);
			if(!P_VALID(pYBAccount))
				return;

			INT	nRemainNum = 0;
			nRemainNum = pYBSellOrder->GetRemainNum() >= pYBBuyOrder->GetRemainNum() ? pYBBuyOrder->GetRemainNum():pYBSellOrder->GetRemainNum();
			
			//������Ԫ���˻�������������׵�
			if(pYBSellOrder->nPrice*nRemainNum > pYBAccount->GetAccountSilver())
				continue;

			bNoticeClient = TRUE;

			if(pYBSellOrder->GetRemainNum() > pYBBuyOrder->GetRemainNum())
			{
				RefreshYBOrder(pYBBuyOrder, pYBSellOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				RefreshYBOrder(pYBSellOrder, pYBSellOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				DeleteYBOrder(pYBBuyOrder, EYBOM_Close);
			}
			else if(pYBSellOrder->GetRemainNum() < pYBBuyOrder->GetRemainNum())
			{
				RefreshYBOrder(pYBBuyOrder, pYBSellOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				RefreshYBOrder(pYBSellOrder, pYBSellOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				DeleteYBOrder(pYBSellOrder, EYBOM_Close);
				break;
			}
			else
			{
				RefreshYBOrder(pYBBuyOrder, pYBSellOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				RefreshYBOrder(pYBSellOrder, pYBSellOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				DeleteYBOrder(pYBBuyOrder, EYBOM_Close);
				DeleteYBOrder(pYBSellOrder, EYBOM_Close);
				break;
			}
		}
	}

	// û��ƥ��Ķ���ʱ��ͬ��һ�¼۸��б�
	if(bNoticeClient == FALSE)
	{
		INT		nTotalNum = m_mapYBSellList.Peek(pYBSellOrder->nPrice);
		Role	*pRole = g_roleMgr.GetRolePtrByID(pYBSellOrder->dwRoleID);

		if(P_VALID(pRole))
		{
			tagNS_SynSellPriceList	send;
			send.nPrice = pYBSellOrder->nPrice;
			send.nNum = nTotalNum;
			pRole->SendMessage(&send, send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// �չ�Ԫ��
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::DealYBBuy(tagYuanBaoOrder* pYBBuyOrder)
{
	// �������۶�����Ѱ��ƥ��Ķ���
	BOOL	bNoticeClient = FALSE;

	tagYuanBaoOrder* pYBSellOrder = (tagYuanBaoOrder*)GT_INVALID;
	std::set<tagYuanBaoOrder*, SellOrderCmp>::iterator itSellOrder = m_setYBSell.begin();
	while (itSellOrder != m_setYBSell.end())
	{
		pYBSellOrder = (*itSellOrder);
		++itSellOrder;

		// ���׶����໥ƥ��
		if(pYBBuyOrder->nPrice >= pYBSellOrder->nPrice && pYBBuyOrder->dwRoleID != pYBSellOrder->dwRoleID)
		{
			//�չ�Ԫ����
			tagYuanBaoAccount* pYBAccount = m_mapYBAccount.Peek(pYBBuyOrder->dwRoleID);
			if(!P_VALID(pYBAccount))
				return;

			INT	nRemainNum = 0;
			nRemainNum = pYBSellOrder->GetRemainNum() >= pYBBuyOrder->GetRemainNum() ? pYBBuyOrder->GetRemainNum():pYBSellOrder->GetRemainNum();

			//������Ԫ���˻�������������׵�
			if(pYBSellOrder->nPrice*nRemainNum > pYBAccount->GetAccountSilver())
				continue;

			bNoticeClient = TRUE;

			if(pYBBuyOrder->GetRemainNum() > pYBSellOrder->GetRemainNum())
			{
				RefreshYBOrder(pYBSellOrder, pYBBuyOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				RefreshYBOrder(pYBBuyOrder, pYBBuyOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				DeleteYBOrder(pYBSellOrder, EYBOM_Close);
			}
			else if(pYBBuyOrder->GetRemainNum() < pYBSellOrder->GetRemainNum())
			{
				RefreshYBOrder(pYBSellOrder, pYBBuyOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				RefreshYBOrder(pYBBuyOrder, pYBBuyOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				DeleteYBOrder(pYBBuyOrder, EYBOM_Close);
				break;
			}
			else
			{
				RefreshYBOrder(pYBSellOrder, pYBBuyOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				RefreshYBOrder(pYBBuyOrder, pYBBuyOrder->dwRoleID, pYBSellOrder->nPrice, nRemainNum, TRUE);
				DeleteYBOrder(pYBBuyOrder, EYBOM_Close);
				DeleteYBOrder(pYBSellOrder, EYBOM_Close);
				break;
			}
		}
		else
			break;
	}

	// û��ƥ��Ķ���ʱ��ͬ��һ�¼۸��б�
	if(bNoticeClient == FALSE)
	{
		INT		nTotalNum = m_mapYBBuyList.Peek(pYBBuyOrder->nPrice);
		Role	*pRole = g_roleMgr.GetRolePtrByID(pYBBuyOrder->dwRoleID);

		if(P_VALID(pRole))
		{
			tagNS_SynBuyPriceList	send;
			send.nPrice = pYBBuyOrder->nPrice;
			send.nNum = nTotalNum;
			pRole->SendMessage(&send, send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ͬ���ͻ����������۵ļ۸��б�
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::SynSellPriceList(Role *pRole)
{
	INT		nListNum = m_mapYBSellList.Size();
	DWORD	dwSize = sizeof(tagNS_GetSellPriceList);
	INT		nIndex = 0;
	INT		nPrice = 0;
	INT		nNum = 0;

	if(!P_VALID(pRole))
		return;

	if(nListNum >0)
	{
		dwSize += (nListNum - 1) * sizeof(tagYBPriceList);
	}

	MCREATE_MSG(pSend, dwSize, NS_GetSellPriceList);

	m_mapYBSellList.ResetIterator();
	while(m_mapYBSellList.PeekNext(nPrice, nNum))
	{
		pSend->YBPriceList[nIndex].nPrice = nPrice;
		pSend->YBPriceList[nIndex].nNum = nNum;
		++nIndex;
	}

	pSend->nListNum = nListNum;
	pRole->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------------------------------
// ͬ���ͻ��������չ��ļ۸��б�
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::SynBuyPriceList(Role *pRole)
{
	INT		nListNum = m_mapYBBuyList.Size();
	DWORD	dwSize = sizeof(tagNS_GetBuyPriceList);
	INT		nIndex = 0;
	INT		nPrice = 0;
	INT		nNum = 0;

	if(!P_VALID(pRole))
		return;

	if(nListNum > 0)
	{
		dwSize += (nListNum - 1) * sizeof(tagYBPriceList);
	}

	MCREATE_MSG(pSend, dwSize, NS_GetBuyPriceList);

	m_mapYBBuyList.ResetIterator();
	while(m_mapYBBuyList.PeekNext(nPrice, nNum))
	{
		pSend->YBPriceList[nIndex].nPrice = nPrice;
		pSend->YBPriceList[nIndex].nNum = nNum;
		++nIndex;
	}

	pSend->nListNum = nListNum;
	pRole->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------------------------------
// ͬ���ͻ���Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
VOID CTradeYB::SynYBAccount(Role *pRole)
{
	if(!P_VALID(pRole))
		return;

	tagYuanBaoAccount *pYBAccount = m_mapYBAccount.Peek(pRole->GetID());
	if(!P_VALID(pYBAccount))
		return;

	tagNS_GetYBAccount	send;
	send.dwRoleID = pYBAccount->dwRoleID;
	send.nYuanBao = pYBAccount->GetAccountYB();
	send.nSilver = pYBAccount->GetAccountSilver();
	send.bSellOrder = pYBAccount->bSellOrder;
	send.bBuyOrder = pYBAccount->bBuyOrder;
	pRole->SendMessage(&send, send.dwSize);
};

