//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_shop.h
// author: Sxg
// actor:
// data: 2008-10-28
// last:
// brief: �̵괦��
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_shop.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "role.h"
#include "creature.h"
#include "shop.h"

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ(��װ��)����ˢ����Ʒ�б�
//-----------------------------------------------------------------------------
DWORD Role::GetShopItems(DWORD dwNPCID, BYTE byShelf)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return E_Shop_NPCNotValid;
	}

	// �ҵ��̵�
	Shop *pShop = pMap->GetShop(pNPC->GetTypeID());
	if(!P_VALID(pShop) || pShop->IsEquipShop())
	{
		return E_Shop_NotValid;
	}

	INT16 n16RareItemNum = pShop->GetRareGoodsNum(byShelf);
	
	// û��ˢ����Ʒ
	if(0 == n16RareItemNum)
	{
		tagNS_GetShopItems Send;
		Send.dwNPCID	= dwNPCID;
		Send.byShelf	= byShelf;
		Send.dwShopID	= pNPC->GetShopID();
		Send.n16RareItemNum = 0;

		SendMessage(&Send, Send.dwSize);
		return E_Success;
	}

	// ��ˢ����Ʒ
	INT32 nSzMsg = sizeof(tagNS_GetShopItems) - 1 + sizeof(tagMsgShopItem) * n16RareItemNum;

	MCREATE_MSG(pSend, nSzMsg, NS_GetShopItems);
	pSend->dwSize	= nSzMsg;
	pSend->dwNPCID	= dwNPCID;
	pSend->byShelf	= byShelf;
	pSend->dwShopID	= pNPC->GetShopID();
	pSend->n16RareItemNum = n16RareItemNum;

	pShop->GetRareItems((tagMsgShopItem*)pSend->byData, n16RareItemNum, byShelf);
	
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ����װ��
//-----------------------------------------------------------------------------
DWORD Role::GetShopEquips(DWORD dwNPCID, BYTE byShelf)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return E_Shop_NPCNotValid;
	}

	// �ҵ��̵�
	Shop *pShop = pMap->GetShop(pNPC->GetTypeID());
	if(!P_VALID(pShop) || !pShop->IsEquipShop())
	{
		return E_Shop_NotValid;
	}

	INT16 n16RareEquipNum = pShop->GetRareGoodsNum(byShelf);

	// û��ˢ����Ʒ
	if(0 == n16RareEquipNum)
	{
		tagNS_GetShopEquips Send;
		Send.dwNPCID		= dwNPCID;
		Send.byShelf		= byShelf;
		Send.dwShopID		= pNPC->GetShopID();
		Send.n16EquipNum	= 0;

		SendMessage(&Send, Send.dwSize);
		return E_Success;
	}

	INT32 nSzMsg = sizeof(tagNS_GetShopEquips) - 1 + sizeof(tagMsgShopEquip) * n16RareEquipNum;

	MCREATE_MSG(pSend, nSzMsg, NS_GetShopEquips);
	pSend->dwSize	= nSzMsg;
	pSend->dwNPCID	= dwNPCID;
	pSend->byShelf	= byShelf;
	pSend->dwShopID	= pNPC->GetShopID();
	pSend->n16EquipNum = n16RareEquipNum;

	pShop->GetRareEquips((tagMsgShopEquip*)pSend->byData, byShelf, pSend->n16EquipNum);

	// ���¼����С
	if(pSend->n16EquipNum != n16RareEquipNum)
	{
		pSend->dwSize -= (n16RareEquipNum - pSend->n16EquipNum) * sizeof(tagMsgShopEquip);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ������Ʒ(��װ��)
//-----------------------------------------------------------------------------
DWORD Role::BuyShopItem(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT16 n16ItemNum)
{
	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return E_Shop_NPCNotValid;
	}

	// �ҵ��̵�
	Shop *pShop = pMap->GetShop(pNPC->GetTypeID());
	if(!P_VALID(pShop) || pShop->IsEquipShop())
	{
		return E_Shop_NotValid;
	}
	
	// �����ұ����Ƿ��еط�
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Shop_NotEnough_SpaceInBag;
	}

	// ͬ����Ʒ�����ﵽ����
	if (!GetItemMgr().CanAddMaxHoldItem(dwTypeID, n16ItemNum))
	{
		return E_Shop_ItemMaxHold;
	}

	tagSellItem SellItem;
	DWORD dwErrorCode = pShop->SellItem(this, byShelf, dwTypeID, n16ItemNum, SellItem);
	if(dwErrorCode != E_Success
		&& dwErrorCode != E_Shop_ItemNotEnough)
	{
		return dwErrorCode;
	}

	// ����ұ����п۳���Ǯ
	if(SellItem.nSilverCost > 0)
	{
		GetCurMgr().DecBagSilver(SellItem.nSilverCost, ELCID_Shop_BuyItem);
	}

	// �۳���������
	if(SellItem.nSpecCost > 0)
	{
		if(ECCT_ItemExchange == SellItem.eCostType)
		{
			GetItemMgr().DelBagSameItem(SellItem.listItem, SellItem.nSpecCost, ELCID_Shop_BuyItem);
		}
		else if(SellItem.eCostType != ECCT_Null)
		{
			GetCurMgr().DecCurrency(SellItem.eCostType, SellItem.nSpecCost, ELCID_Shop_BuyItem);
		}
	}

	// ��װ���ŵ���ұ�����
	if(P_VALID(SellItem.pItemSell))
	{
		GetItemMgr().Add2Bag(SellItem.pItemSell, ELCID_Shop_BuyItem, TRUE);
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if(SellItem.n16RemainNum != GT_INVALID)
	{
		tagNS_BuyShopItem Send;
		Send.dwNPCID		= dwNPCID;
		Send.byShelf		= byShelf;
		Send.n16RemainNum	= SellItem.n16RemainNum;
		Send.dwTypeID		= dwTypeID;
		Send.byIndex		= SellItem.byIndex;

		SendMessage(&Send, Send.dwSize);
	}
	

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��װ��
//-----------------------------------------------------------------------------
DWORD Role::BuyShopEquip(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT64 n64Serial)
{
	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return E_Shop_NPCNotValid;
	}

	// �ҵ��̵�
	Shop *pShop = pMap->GetShop(pNPC->GetTypeID());
	if(!P_VALID(pShop) || !pShop->IsEquipShop())
	{
		return E_Shop_NotValid;
	}

	// �����ұ����Ƿ��еط�
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Shop_NotEnough_SpaceInBag;
	}

	// ͬ����Ʒ�����ﵽ����
	if (!GetItemMgr().CanAddMaxHoldItem(dwTypeID, 1))
	{
		return E_Shop_ItemMaxHold;
	}

	tagSellEquip SellEquip;
	DWORD dwErrorCode = pShop->SellEquip(this, byShelf, dwTypeID, n64Serial, SellEquip);
	if(dwErrorCode != E_Success
		&& dwErrorCode != E_Shop_Equip_Sold
		&& dwErrorCode != E_Shop_CreateEquip_Failed)
	{
		return dwErrorCode;
	}

	// ����ұ����п۳���Ǯ
	if(SellEquip.nSilverCost > 0)
	{
		GetCurMgr().DecBagSilver(SellEquip.nSilverCost, ELCID_Shop_BuyEquip);
	}

	// �۳���������
	if(SellEquip.nSpecCost > 0)
	{
		if(ECCT_ItemExchange == SellEquip.eCostType)
		{
			GetItemMgr().DelBagSameItem(SellEquip.listItem, SellEquip.nSpecCost, ELCID_Shop_BuyEquip);
		}
		else if(SellEquip.eCostType != ECCT_Null)
		{
			GetCurMgr().DecCurrency(SellEquip.eCostType, SellEquip.nSpecCost, ELCID_Shop_BuyEquip);
		}
	}

	// ��װ���ŵ���ұ�����
	if(P_VALID(SellEquip.pEquipSell))
	{
		tagItem *pItem = SellEquip.pEquipSell;
		GetItemMgr().Add2Bag(pItem, ELCID_Shop_BuyEquip, TRUE);
	}

	// ������Ϊ��ͨ����
	if(GT_INVALID == SellEquip.n16RemainNum)
	{
		return dwErrorCode;
	}
	
	// ���͸��º��̵���Ʒ -- ֻ����ϡ������
	if(P_VALID(SellEquip.pEquipNew))
	{
		INT32 nMsgSz = sizeof(tagNS_BuyShopEquip) - 1 + SIZE_EQUIP;

		MCREATE_MSG(pSend, nMsgSz, NS_BuyShopEquip);
		pSend->dwNPCID		= dwNPCID;
		pSend->byShelf		= byShelf;
		pSend->n16RemainNum	= SellEquip.n16RemainNum;
		pSend->dwSize		= nMsgSz;
		pSend->byIndex		= SellEquip.byIndex;
		
		IFASTCODE->MemCpy(pSend->byData, SellEquip.pEquipNew, SIZE_EQUIP);
		
		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}
	else
	{
		// ˢ��װ������û�д���װ�������¸���
		tagNS_BuyShopEquip Send;
		Send.dwNPCID		= dwNPCID;
		Send.byShelf		= byShelf;
		Send.n16RemainNum	= SellEquip.n16RemainNum;
		Send.byIndex		= SellEquip.byIndex;

		ASSERT(0 == SellEquip.n16RemainNum);

		SendMessage(&Send, Send.dwSize);

		GetShopEquips(dwNPCID,byShelf);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���̵������Ʒ&װ��
//-----------------------------------------------------------------------------
DWORD Role::SellToShop(DWORD dwNPCID, INT64 n64Serial)
{
	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return E_Shop_NPCNotValid;
	}

	// �ҵ��̵�
	Shop *pShop = pMap->GetShop(pNPC->GetTypeID());
	if(!P_VALID(pShop))
	{
		return E_Shop_NotValid;
	}

	// �ҵ�������Ʒ
	tagItem *pItem = GetItemMgr().GetBagItem(n64Serial);
	if(!P_VALID(pItem))
	{
		return E_Shop_ItemNotFind;
	}

	// �Ƿ�ɳ���
	if(!GetItemMgr().CanSell(*pItem))
	{
		return E_Shop_ItemCannotSell;
	}

	// �����ۼ�//??û�м���˰��Ӱ��
	INT64 n64Price = pItem->pProtoType->nBasePrice * pItem->n16Num;
	if(MIsEquipment(pItem->dwTypeID))
	{
		FLOAT fFactor;
		MCalPriceFactor(fFactor, ((tagEquip*)pItem)->equipSpec.byQuality);
		n64Price = (INT64)(n64Price * fFactor);
		if(n64Price < 0)
		{
			ASSERT(0);
			n64Price = 0;
		}
	}

	// ���������ɾ����Ʒ
	DWORD dwErrorCode = GetItemMgr().DelFromBag(n64Serial, ELCID_Shop_Sell, GT_INVALID, TRUE);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ������
	float fEarnRate = GetEarnRate() / 10000.0f;
	n64Price = (INT64)(n64Price * fEarnRate);

	// ��һ�ý�Ǯ
	GetCurMgr().IncBagSilver(n64Price, ELCID_Shop_Sell);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �̵���Ϣ����ṹ�������ͻ���
//-----------------------------------------------------------------------------
VOID Role::SendShopFeedbackMsg(DWORD dwErrorCode, DWORD dwNPCID)
{
	// �ɹ�Ҳ�᷵�أ��ͻ�����Ҫ����Ч����
	tagNS_FeedbackFromShop Send;
	Send.dwErrorCode	= dwErrorCode;
	Send.dwNPCID		= dwNPCID;

	SendMessage(&Send, Send.dwSize);
}