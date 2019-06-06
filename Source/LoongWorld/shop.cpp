//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: shop.cpp
// author: Sxg
// actor:
// data: 2008-10-22
// last:
// brief: �̵���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/msg_shop.h"
#include "../ServerDefine/base_define.h"
#include "shop.h"
#include "role.h"
#include "att_res.h"
#include "item_creator.h"
#include "clan_treasury.h"

//*********************************** Shop **************************************

//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
Shop::Shop(DWORD dwNPCID, const tagShopProto *pShopProto)
{
	ASSERT(P_VALID(pShopProto));

	// �����Ʒ����
	INT16 n16TotalNum = 0;
	for(INT32 i=0; i<MAX_SHOP_SHELF_NUM; ++i)
	{
		ASSERT(pShopProto->n16Num[i] >= 0);
		n16TotalNum += pShopProto->n16Num[i];
	}

	ASSERT(n16TotalNum <= MAX_SHOP_TOTAL_ITEM);
	
	// ��ʼ��
	m_bInitOK		= FALSE;
	m_dwNPCID		= dwNPCID;
	m_pShopProto	= pShopProto;

	ZeroMemory(m_n16RareIndexStart, sizeof(m_n16RareIndexStart));
}

Shop::~Shop()
{
}

//-----------------------------------------------------------------------------
// �����̵�
//-----------------------------------------------------------------------------
Shop* Shop::Create(DWORD dwNPCID, DWORD dwShopID)
{
	const tagShopProto *pShopProto = g_attRes.GetShopProto(dwShopID);
	if(!P_VALID(pShopProto))
	{
		ASSERT(P_VALID(pShopProto));
		return NULL;
	}

	Shop *pShop = NULL;

	if (pShopProto->bClanTreasury)
	{
		if (pShopProto->bEquip)
		{
			pShop = new EquipClanTreasury(dwNPCID, pShopProto);
		}
		else
		{
			pShop = new ItemClanTreasury(dwNPCID, pShopProto);
		}
	}
	else
	{
		if(pShopProto->bEquip)
		{
			pShop = new EquipShop(dwNPCID, pShopProto);
		}
		else
		{
			pShop = new ItemShop(dwNPCID, pShopProto);
		}
	}

	
	if(!pShop->IsInitOK())
	{
		ASSERT(pShop->IsInitOK());
		SAFE_DEL(pShop);
	}

	return pShop;
}

//-----------------------------------------------------------------------------
// �����������
//-----------------------------------------------------------------------------
DWORD Shop::CheckSpecCost(Role *pRole, ECurCostType eCostType, INT nSpecCost, 
			  DWORD dwCostItemID, BYTE byRepLevelLimit, OUT TList<tagItem*> &listItem)
{
	ASSERT(P_VALID(pRole));
	
	if(nSpecCost < 0)
	{
		ASSERT(nSpecCost >= 0);
		return GT_INVALID;
	}
	
	listItem.Clear();
	
	switch(eCostType)
	{
	case ECCT_Null:			// û����������
		break;
	case ECCT_BagSilver:	// ���ı�����Ǯ�е����ֶ�
		return GT_INVALID;
		break;
	case ECCT_ItemExchange:	// ��Ʒ�һ�
		if(pRole->GetItemMgr().GetBagSameItemList(listItem, dwCostItemID, nSpecCost) < nSpecCost)
		{
			return E_Item_NotEnough;
		}
		break;
	default:				// ��������������
		ECLanType eClanType = MTRANS_ECCT2ECLT(eCostType);
		if( JDG_VALID(ECLT, eClanType) && byRepLevelLimit > pRole->GetClanData().RepGetLvl(eClanType))
		{
			return E_Shop_RepLevel_Low;
		}

		// ��������Ʒ���� -- ʹ��ͨ�ýӿ�(�߻������󣬳������޸�)
		if(!pRole->GetCurMgr().IsEnough(eCostType, nSpecCost))
		{
			return E_Shop_NotEnough_SpecCost;
		}
		break;
	}
	
	return E_Success;
}


//*********************************** ItemShop **********************************

//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
ItemShop::ItemShop(DWORD dwNPCID, const tagShopProto *pShopProto)
					: Shop(dwNPCID, pShopProto)
{
	m_bInitOK = Init(pShopProto);
}

ItemShop::~ItemShop()
{
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL ItemShop::Init(const tagShopProto *pShopProto)
{
	ASSERT(P_VALID(pShopProto));
	ASSERT(!pShopProto->bEquip);

	// ��ʼ������
	for(INT32 n=0; n<MAX_SHOP_SHELF_NUM; ++n)
	{
		m_Shelf[n].Init(EICT_Shop, pShopProto->n16Num[n], pShopProto->n16Num[n]);
	}

	// ��¼��ҳ��Ʒ����
	INT16	n16ItemNum[MAX_SHOP_SHELF_NUM];
	ZeroMemory(n16ItemNum, sizeof(n16ItemNum));

	// ��ȡ��ǰʱ��
	DWORD dwCurTime = g_world.GetWorldTime();
	
	// ��ʼ����ͨ��Ʒ
	INT32 i,j;
	const tagShopItemProto *pItemProto = NULL;
	for(i=0; i<MAX_SHOP_COMMON_ITEM; ++i)
	{
		pItemProto = &pShopProto->Item[i];

		if(GT_INVALID == pItemProto->dwTypeID)
		{
			break;
		}
		
		tagShopItem *pItem = new tagShopItem;
		if(!P_VALID(pItem))
		{
			ASSERT(P_VALID(pItem));
			return FALSE;
		}

		pItem->dwTypeID			= pItemProto->dwTypeID;
		pItem->n16RemainNum		= GT_INVALID;
		pItem->n16Index			= GT_INVALID;
		pItem->byProtoIndex		= i;
		pItem->dwRefreshTime	= dwCurTime;
		pItem->pProto			= pItemProto;

		// �ŵ���������
		ASSERT(pItemProto->byShelf < MAX_SHOP_SHELF_NUM);
		m_Shelf[pItemProto->byShelf].Add(pItem, n16ItemNum[pItemProto->byShelf]++);
	}

	// ��ʼ��������Ʒ��ʼ�±�
	IFASTCODE->MemCpy(m_n16RareIndexStart, n16ItemNum, sizeof(m_n16RareIndexStart));

	// ��ʼ��������Ʒ
	const tagShopRareItemProto *pRareProto = NULL;
	for(j=0; j<MAX_SHOP_RARE_ITEM; ++i, ++j)
	{
		pRareProto = &pShopProto->RareItem[j];

		if(GT_INVALID == pRareProto->dwTypeID)
		{
			break;
		}

		tagShopItem *pItem = new tagShopItem;
		if(!P_VALID(pItem))
		{
			ASSERT(P_VALID(pItem));
			return FALSE;
		}
		
		pItem->dwTypeID			= pRareProto->dwTypeID;
		pItem->n16RemainNum		= pRareProto->byRefreshNum;
		pItem->n16Index			= GT_INVALID;
		pItem->byProtoIndex		= j;
		pItem->dwRefreshTime	= dwCurTime;
		pItem->pRareProto		= pRareProto;

		// �ŵ���������
		ASSERT(pRareProto->byShelf < MAX_SHOP_SHELF_NUM);
		m_Shelf[pRareProto->byShelf].Add(pItem, n16ItemNum[pRareProto->byShelf]++);
	}

	// ����̵���Դ�Ƿ�������
	for(INT32 n=0; n<MAX_SHOP_SHELF_NUM; ++n)
	{
		if(pShopProto->n16Num[n] != n16ItemNum[n])
		{
			ASSERT(0);
			IMSG(_T("Shop proto mybe has problem, please check<shopid:%u, shelf:%d>\n"), pShopProto->dwID, n);
			return FALSE;
		}
	}
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID ItemShop::Update()
{
	// ��ȡ��ǰʱ��
	DWORD dwCurTime = g_world.GetWorldTime();

	// ��¼��ҳ��Ʒ�±�
	INT16	n16ItemNum[MAX_SHOP_SHELF_NUM];

	IFASTCODE->MemCpy(n16ItemNum, m_n16RareIndexStart, sizeof(m_n16RareIndexStart));

	// ���������Ʒ�Ƿ���Ҫ����
	tagShopItem *pItem = NULL;
	const tagShopRareItemProto *pRareProto = NULL;
	for(INT32 i=0; i<MAX_SHOP_RARE_ITEM; ++i)
	{
		pRareProto = &m_pShopProto->RareItem[i];
		if(GT_INVALID == pRareProto->dwTypeID)
		{
			break;
		}

		pItem = m_Shelf[pRareProto->byShelf].GetItem(n16ItemNum[pRareProto->byShelf]++);
		ASSERT(P_VALID(pItem) && pItem->dwTypeID == pRareProto->dwTypeID);

		if(CalcTimeDiff(dwCurTime, pItem->dwRefreshTime) >= pRareProto->dwRefreshTime)
		{
			pItem->n16RemainNum	= pRareProto->byRefreshNum;
			pItem->dwRefreshTime = dwCurTime;
		}
	}
}

//-----------------------------------------------------------------------------
// ��ȡˢ����Ʒ
//-----------------------------------------------------------------------------
VOID ItemShop::GetRareItems(OUT tagMsgShopItem* pRareItems, INT16 n16RareItemNum, BYTE byShelf)
{
	ASSERT(GetRareGoodsNum(byShelf) == n16RareItemNum);

	tagShopItem *pShopItem = NULL;
	for(INT16 i=0; i<n16RareItemNum; ++i)
	{
		pShopItem = m_Shelf[byShelf].GetItem((INT16)(m_n16RareIndexStart[byShelf] + i));
		ASSERT(P_VALID(pShopItem));

		pRareItems[i].dwTypeID		= pShopItem->dwTypeID;
		pRareItems[i].n16RemainNum	= pShopItem->n16RemainNum;
		pRareItems[i].n16Index		= pShopItem->n16Index;
		pRareItems[i].byProtoIndex	= pShopItem->byProtoIndex;
	}
}

//-----------------------------------------------------------------------------
// ������Ʒ
//-----------------------------------------------------------------------------
DWORD ItemShop::SellItem(Role *pRole, BYTE byShelf, DWORD dwTypeID, INT16 n16BuyNum, OUT tagSellItem &SellItem)
{
	ASSERT(P_VALID(m_pShopProto));
	ASSERT(P_VALID(pRole));

	// ������Ʒ
	tagShopItem *pShopItem = m_Shelf[byShelf].GetItem(dwTypeID);
	if(!P_VALID(pShopItem))
	{
		// ִ�е������ζ����Ϣ�е�dwTypeID�Ƿ�
		ASSERT(P_VALID(pShopItem));
		return E_Shop_ItemNotFind;
	}

	// �����Ʒ����
	if(pShopItem->n16RemainNum != GT_INVALID && pShopItem->n16RemainNum < n16BuyNum)
	{
		SellItem.n16RemainNum = pShopItem->n16RemainNum;
		return E_Shop_ItemNotEnough;
	}

	// �����н�Ǯ -- ��ʹ��ͨ�ýӿ�
	INT nSilverCost = pShopItem->pProto->nSilver * n16BuyNum;
	if(nSilverCost > pRole->GetCurMgr().GetBagSilver())
	{
		return E_Shop_NotEnough_SilverInBag;
	}

	// ��������Ʒ���� -- ʹ��ͨ�ýӿ�(�߻������󣬳������޸�)
	INT nSpecCost = pShopItem->pProto->nCostNum * n16BuyNum;
	DWORD dwErrorCode = CheckSpecCost(pRole, m_pShopProto->eCostType, nSpecCost, 
		m_pShopProto->dwItemTypeID, pShopItem->pProto->byRepLevel, SellItem.listItem);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ������Ʒ
	tagItem *pItemSell = ItemCreator::Create(EICM_ShopNPC, m_dwNPCID, dwTypeID, n16BuyNum);
	if(!P_VALID(pItemSell))
	{
		ASSERT(P_VALID(pItemSell));
		return E_Shop_CreateItem_Failed;
	}

	// �����̵�����Ʒ����
	if(pShopItem->n16RemainNum != GT_INVALID)
	{
		pShopItem->n16RemainNum -= n16BuyNum;
	}

	// ���ô�������
	SellItem.pItemSell		= pItemSell;
	SellItem.n16RemainNum	= pShopItem->n16RemainNum;
	SellItem.nSilverCost	= nSilverCost;
	SellItem.eCostType		= m_pShopProto->eCostType;
	SellItem.nSpecCost		= nSpecCost;
	SellItem.byIndex		= (BYTE)pShopItem->n16Index;

	return E_Success;
}


//*********************************** EquipShop **********************************

//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
EquipShop::EquipShop(DWORD dwNPCID, const tagShopProto *pShopProto)
					: Shop(dwNPCID, pShopProto)
{
	m_bInitOK = Init(pShopProto);
}

EquipShop::~EquipShop()
{
	// ɾ�������ɵ�װ��
	tagShopEquip *pShopEquip = NULL;
	for(INT32 i=0; i<MAX_SHOP_SHELF_NUM; ++i)
	{
		for(INT16 j=0; j<m_pShopProto->n16Num[i]; ++j)
		{
			pShopEquip = m_Shelf[i].GetItem(j);
			if(!P_VALID(pShopEquip))
			{
				break;
			}

			SAFE_DEL(pShopEquip->pEquip);
		}
	}
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL EquipShop::Init(const tagShopProto *pShopProto)
{
	ASSERT(P_VALID(pShopProto));
	ASSERT(pShopProto->bEquip);

	// ��ʼ������
	for(INT32 n=0; n<MAX_SHOP_SHELF_NUM; ++n)
	{
		m_Shelf[n].Init(EICT_Shop, pShopProto->n16Num[n], pShopProto->n16Num[n]);
	}

	// ��¼��ҳ��Ʒ����
	INT16	n16ItemNum[MAX_SHOP_SHELF_NUM];
	ZeroMemory(n16ItemNum, sizeof(n16ItemNum));

	// ��ȡ��ǰʱ��
	DWORD dwCurTime = g_world.GetWorldTime();

	// ��ʼ����ͨ��Ʒ
	INT32 i,j;
	const tagShopItemProto *pEquipProto = NULL;
	for(i=0; i<MAX_SHOP_COMMON_ITEM; ++i)
	{
		pEquipProto = &pShopProto->Item[i];
		if(GT_INVALID == pEquipProto->dwTypeID)
		{
			break;
		}

		tagShopEquip *pEquip = new tagShopEquip;
		if(!P_VALID(pEquip))
		{
			ASSERT(P_VALID(pEquip));
			return FALSE;
		}


		pEquip->n16RemainNum	= GT_INVALID;
		pEquip->n16Index		= GT_INVALID;
		pEquip->byProtoIndex	= i;
		pEquip->dwRefreshTime	= dwCurTime;
		pEquip->pProto			= pEquipProto;
		pEquip->pEquip			= NULL;

		// �Ƿ���װ��
		if(!MIsEquipment(pEquipProto->dwTypeID))
		{
			return FALSE;
		}

		// ����װ��
		pEquip->pEquip = (tagEquip*)ItemCreator::Create(EICM_ShopNPC, m_dwNPCID, pEquipProto->dwTypeID, 1, GT_INVALID);
		if(!P_VALID(pEquip->pEquip))
		{
			ASSERT(P_VALID(pEquip->pEquip));
			return FALSE;
		}

		// ����Ϊ��ɫװ��
		ItemCreator::IdentifyEquip(pEquip->pEquip, EIQ_White);

		// �ŵ���������
		ASSERT(pEquipProto->byShelf < MAX_SHOP_SHELF_NUM);
		m_Shelf[pEquipProto->byShelf].Add(pEquip, n16ItemNum[pEquipProto->byShelf]++);
	}

	// ��ʼ��������Ʒ��ʼ�±�
	IFASTCODE->MemCpy(m_n16RareIndexStart, n16ItemNum, sizeof(m_n16RareIndexStart));

	// ��ʼ��������Ʒ
	const tagShopRareItemProto *pRareProto = NULL;
	for(j=0; j<MAX_SHOP_RARE_ITEM; ++i, ++j)
	{
		pRareProto = &pShopProto->RareItem[j];
		if(GT_INVALID == pRareProto->dwTypeID)
		{
			break;
		}

		tagShopEquip *pEquip = new tagShopEquip;
		if(!P_VALID(pEquip))
		{
			ASSERT(P_VALID(pEquip));
			return FALSE;
		}

		pEquip->n16RemainNum	= pRareProto->byRefreshNum;
		pEquip->n16Index		= GT_INVALID;
		pEquip->byProtoIndex	= j;
		pEquip->dwRefreshTime	= dwCurTime;
		pEquip->pRareProto		= pRareProto;
		pEquip->pEquip			= NULL;

		// �Ƿ���װ��
		if(!MIsEquipment(pRareProto->dwTypeID))
		{
			return FALSE;
		}

		// ����װ��
		pEquip->pEquip = (tagEquip*)ItemCreator::Create(EICM_ShopNPC, m_dwNPCID, pRareProto->dwTypeID, 1, GT_INVALID);
		if(!P_VALID(pEquip->pEquip))
		{
			ASSERT(P_VALID(pEquip->pEquip));
			return FALSE;
		}

		// ����Ϊָ��Ʒ��װ��
		ItemCreator::IdentifyEquip(pEquip->pEquip, (EItemQuality)pRareProto->byQuality);
		
		// �ŵ���������
		ASSERT(pRareProto->byShelf < MAX_SHOP_SHELF_NUM);
		m_Shelf[pRareProto->byShelf].Add(pEquip, n16ItemNum[pRareProto->byShelf]++);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID EquipShop::Update()
{
	// ��ȡ��ǰʱ��
	DWORD dwCurTime = g_world.GetWorldTime();

	// ��¼��ҳ��Ʒ�±�
	INT16	n16ItemNum[MAX_SHOP_SHELF_NUM];

	IFASTCODE->MemCpy(n16ItemNum, m_n16RareIndexStart, sizeof(m_n16RareIndexStart));

	// ���������Ʒ�Ƿ���Ҫ����
	tagShopEquip *pEquip = NULL;
	const tagShopRareItemProto *pRareProto = NULL;
	for(INT32 i=0; i<MAX_SHOP_RARE_ITEM; ++i)
	{
		pRareProto = &m_pShopProto->RareItem[i];
		if(GT_INVALID == pRareProto->dwTypeID)
		{
			break;
		}

		pEquip = m_Shelf[pRareProto->byShelf].GetItem(n16ItemNum[pRareProto->byShelf]++);
		ASSERT(P_VALID(pEquip));

		if(CalcTimeDiff(dwCurTime, pEquip->dwRefreshTime) >= pRareProto->dwRefreshTime)
		{
			if(P_VALID(pEquip->pEquip))
			{
				ASSERT(pEquip->pEquip->dwTypeID == pRareProto->dwTypeID);

				SAFE_DEL(pEquip->pEquip);
			}

			// ����װ��
			pEquip->pEquip = (tagEquip*)ItemCreator::Create(EICM_ShopNPC, m_dwNPCID, pRareProto->dwTypeID, 1, GT_INVALID);
			//ASSERT(P_VALID(pEquip->pEquip));

			// ����Ϊָ��Ʒ��װ��
			ItemCreator::IdentifyEquip(pEquip->pEquip, (EItemQuality)pRareProto->byQuality);
			
			pEquip->n16RemainNum	= pRareProto->byRefreshNum;
			pEquip->dwRefreshTime	= dwCurTime;
		}
	}
}

//-----------------------------------------------------------------------------
// ��ȡ����װ��
//-----------------------------------------------------------------------------
VOID EquipShop::GetRareEquips(OUT tagMsgShopEquip* pEquips, BYTE byShelf, INT16 &n16RareEquipNum)
{
	ASSERT(GetRareGoodsNum(byShelf) == n16RareEquipNum);

	tagShopEquip *pShopEquip = NULL;
	INT16 n16RealNum = 0;
	for(INT16 i=0; i<n16RareEquipNum; ++i)
	{
		pShopEquip = m_Shelf[byShelf].GetItem((INT16)(i + m_n16RareIndexStart[byShelf]));
		ASSERT(P_VALID(pShopEquip));

		if(P_VALID(pShopEquip->pEquip))
		{
			pEquips[n16RealNum].n16RemainNum		= pShopEquip->n16RemainNum;
			pEquips[n16RealNum].n16Index			= pShopEquip->n16Index;
			pEquips[n16RealNum].byProtoIndex		= pShopEquip->byProtoIndex;
			
			IFASTCODE->MemCpy(&pEquips[n16RealNum].Equip, pShopEquip->pEquip, SIZE_EQUIP);
			++n16RealNum;
		}
	}

	n16RareEquipNum = n16RealNum;
}

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD EquipShop::SellEquip(Role *pRole, BYTE byShelf, DWORD dwTypeID, INT64 n64Serial, OUT tagSellEquip &SellEquip)
{
	ASSERT(P_VALID(pRole));
	ASSERT(P_VALID(m_pShopProto));
	
	// ����װ��
	tagShopEquip *pShopEquip = m_Shelf[byShelf].GetItem(dwTypeID);
	if(!P_VALID(pShopEquip))
	{
		// ִ�е������ζ����Ϣ�е�dwTypeID�Ƿ�
		ASSERT(P_VALID(pShopEquip));
		return E_Shop_ItemNotFind;
	}

	// �����ϡ��װ�����ָ��װ���Ƿ��Ѿ�������
	if(pShopEquip->n16RemainNum >= 0)
	{
		if(!P_VALID(pShopEquip->pEquip) || pShopEquip->pEquip->n64Serial != n64Serial)
		{
			// ���ô�������
			SellEquip.pEquipNew		= pShopEquip->pEquip;
			SellEquip.n16RemainNum	= pShopEquip->n16RemainNum;

			return E_Shop_Equip_Sold;
		}
	}

	// �����н�Ǯ
	if(pShopEquip->pProto->nSilver > pRole->GetCurMgr().GetBagSilver())
	{
		return E_Shop_NotEnough_SilverInBag;
	}
	
	// �����׶��Ƿ�����
	DWORD dwErrorCode = CheckSpecCost(pRole, m_pShopProto->eCostType, 
									pShopEquip->pProto->nCostNum, m_pShopProto->dwItemTypeID, 
									pShopEquip->pProto->byRepLevel, SellEquip.listItem);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ���64λid�Ƿ�Ϸ����Ƿ��Ļ�������������һ��
	if(pShopEquip->pEquip->n64Serial < MIN_ITEM_SERIAL_INTERNAL)
	{
		ItemCreator::CreateItemSerial(pShopEquip->pEquip->n64Serial);
	}
	
	// ���ô�������
	SellEquip.pEquipSell	= pShopEquip->pEquip;
	SellEquip.nSilverCost	= pShopEquip->pProto->nSilver;
	SellEquip.eCostType		= m_pShopProto->eCostType;
	SellEquip.nSpecCost		= pShopEquip->pProto->nCostNum;

	// �����̵�����Ʒ����
	if(pShopEquip->n16RemainNum != GT_INVALID)
	{
		--pShopEquip->n16RemainNum;
	}

	// ������װ��
	if(pShopEquip->n16RemainNum != 0)
	{
		pShopEquip->pEquip = (tagEquip*)ItemCreator::Create(EICM_ShopNPC, m_dwNPCID, dwTypeID);
		if(!P_VALID(pShopEquip->pEquip))
		{
			// ִ�е�����,Ӧ�����ڴ�����ʧ��.(ԭ�Ϳ϶������ҵ�)
			ASSERT(P_VALID(pShopEquip->pEquip));
			// return E_Shop_CreateEquip_Failed;
			return E_Success;
		}

		// ����Ϊָ��Ʒ��װ��
		ItemCreator::IdentifyEquip(pShopEquip->pEquip, (EItemQuality)SellEquip.pEquipSell->equipSpec.byQuality);
	}
	else
	{
		pShopEquip->pEquip = NULL;
	}

	// ���ô�������
	SellEquip.pEquipNew		= pShopEquip->pEquip;
	SellEquip.n16RemainNum	= pShopEquip->n16RemainNum;
	SellEquip.byIndex		= (BYTE)pShopEquip->n16Index;

	return E_Success;
}