#include "StdAfx.h"
#include "clan_treasury.h"
#include "role.h"
#include "currency.h"
#include "item_creator.h"

#include "../WorldDefine/msg_shop.h"

//*********************************** ItemClanTreasury **********************************

//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
ItemClanTreasury::ItemClanTreasury(DWORD dwNPCID, const tagShopProto *pShopProto)
: Shop(dwNPCID, pShopProto)
{
	ZeroMemory(m_n16ActNum, sizeof(m_n16ActNum));
	ZeroMemory(m_n16ActRareNum, sizeof(m_n16ActRareNum));
	m_bInitOK = Init(pShopProto);
}

ItemClanTreasury::~ItemClanTreasury()
{
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL ItemClanTreasury::Init(const tagShopProto *pShopProto)
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

		tagTreasuryItem *pItem = new tagTreasuryItem;
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
		//tbc:�Ƿ񱻼���
		pItem->SetAct(GT_INVALID);

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

		tagTreasuryItem *pItem = new tagTreasuryItem;
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
		//tbc:�Ƿ񱻼���
		pItem->SetAct(GT_INVALID);

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
VOID ItemClanTreasury::Update()
{
	// ��ȡ��ǰʱ��
	DWORD dwCurTime = g_world.GetWorldTime();

	// ��¼��ҳ��Ʒ�±�
	INT16	n16ItemNum[MAX_SHOP_SHELF_NUM];

	IFASTCODE->MemCpy(n16ItemNum, m_n16RareIndexStart, sizeof(m_n16RareIndexStart));

	// ���������Ʒ�Ƿ���Ҫ����
	tagTreasuryItem *pItem = NULL;
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

		// δ����Ĳ�����
		if (!pItem->IsAct()) continue;

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
VOID ItemClanTreasury::GetRareItems(OUT tagMsgShopItem* pRareItems, INT16 n16RareItemNum, BYTE byShelf)
{
	ASSERT(GetRareGoodsNum(byShelf) == n16RareItemNum);

	tagTreasuryItem *pShopItem = NULL;
	for(INT16 i=m_n16RareIndexStart[byShelf]; i<m_Shelf[byShelf].GetCurSpaceSize(); ++i)
	{
		pShopItem = m_Shelf[byShelf].GetItem(i);
		ASSERT(P_VALID(pShopItem));
		
		// δ����Ĳ�����
		if (!pShopItem->IsAct()) continue;

		pRareItems[i].dwTypeID		= pShopItem->dwTypeID;
		pRareItems[i].n16RemainNum	= pShopItem->n16RemainNum;
		pRareItems[i].n16Index		= pShopItem->n16Index;
		pRareItems[i].byProtoIndex	= pShopItem->byProtoIndex;
	}
}

//-----------------------------------------------------------------------------
// ������Ʒ
//-----------------------------------------------------------------------------
DWORD ItemClanTreasury::SellItem(Role *pRole, BYTE byShelf, DWORD dwTypeID, INT16 n16BuyNum, OUT tagSellItem &SellItem)
{
	ASSERT(P_VALID(m_pShopProto));
	ASSERT(P_VALID(pRole));

	// ������Ʒ
	tagTreasuryItem *pShopItem = m_Shelf[byShelf].GetItem(dwTypeID);
	if(!P_VALID(pShopItem))
	{
		// ִ�е������ζ����Ϣ�е�dwTypeID�Ƿ�
		ASSERT(P_VALID(pShopItem));
		return E_Shop_ItemNotFind;
	}

	// tbc:�Ƿ񼤻�
	if(!pShopItem->IsAct())
	{
		return E_Shop_ItemNotActived;
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

	// ������Ʒtbc:
	ASSERT(pShopItem->IsAct());
	tagItem *pItemSell = ItemCreator::CreateTreasure(pShopItem->dwNameID, EICM_ShopNPC, m_dwNPCID, dwTypeID, n16BuyNum);
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

//-----------------------------------------------------------------------------
// �����䱦
//-----------------------------------------------------------------------------
BOOL ItemClanTreasury::ActiveTreasure( DWORD dwTypeID, DWORD dwNameID )
{
	ASSERT(GT_VALID(dwNameID));

	tagTreasuryItem* pToAct = NULL;
	for (INT i=0; i<MAX_SHOP_SHELF_NUM; ++i)
	{
		pToAct = m_Shelf[i].GetItem(dwTypeID);
		if (P_VALID(pToAct))
		{
			pToAct->SetAct(dwNameID);
			m_n16ActNum[i]++;
			if (GT_VALID(pToAct->n16RemainNum))
			{
				// ϡ���䱦
				m_n16ActRareNum[i]++;
			}
			return TRUE;
		}
	}
	// ��Դ������
	ASSERT(0);
	return FALSE;
}

//*********************************** EquipClanTreasury **********************************

//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
EquipClanTreasury::EquipClanTreasury(DWORD dwNPCID, const tagShopProto *pShopProto)
: Shop(dwNPCID, pShopProto)
{
	ZeroMemory(m_n16ActNum, sizeof(m_n16ActNum));
	ZeroMemory(m_n16ActRareNum, sizeof(m_n16ActRareNum));
	m_bInitOK = Init(pShopProto);
}

EquipClanTreasury::~EquipClanTreasury()
{
	// ɾ�������ɵ�װ��
	tagTreasuryEquip *pShopEquip = NULL;
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
BOOL EquipClanTreasury::Init(const tagShopProto *pShopProto)
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

		tagTreasuryEquip *pEquip = new tagTreasuryEquip;
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
		//tbc:
		pEquip->SetAct(GT_INVALID);


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

		tagTreasuryEquip *pEquip = new tagTreasuryEquip;
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
		//tbc:
		pEquip->SetAct(GT_INVALID);

		// �ŵ���������
		ASSERT(pRareProto->byShelf < MAX_SHOP_SHELF_NUM);
		m_Shelf[pRareProto->byShelf].Add(pEquip, n16ItemNum[pRareProto->byShelf]++);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID EquipClanTreasury::Update()
{
	// ��ȡ��ǰʱ��
	DWORD dwCurTime = g_world.GetWorldTime();

	// ��¼��ҳ��Ʒ�±�
	INT16	n16ItemNum[MAX_SHOP_SHELF_NUM];

	IFASTCODE->MemCpy(n16ItemNum, m_n16RareIndexStart, sizeof(m_n16RareIndexStart));

	// ���������Ʒ�Ƿ���Ҫ����
	tagTreasuryEquip *pEquip = NULL;
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

		ASSERT(GT_VALID(pEquip->n16RemainNum));

		// δ����Ĳ�����
		if (!pEquip->IsAct()) continue;
		
		if(CalcTimeDiff(dwCurTime, pEquip->dwRefreshTime) >= pRareProto->dwRefreshTime)
		{
			if(P_VALID(pEquip->pEquip))
			{
				ASSERT(pEquip->pEquip->dwTypeID == pRareProto->dwTypeID);

				SAFE_DEL(pEquip->pEquip);
			}

			// ����װ��tbc:
			ASSERT(pEquip->IsAct());
			pEquip->pEquip = (tagEquip*)ItemCreator::CreateTreasureEx(pEquip->dwNameID, EICM_ShopNPC, m_dwNPCID, pRareProto->dwTypeID, 1, (EItemQuality)pRareProto->byQuality);

			pEquip->n16RemainNum	= pRareProto->byRefreshNum;
			pEquip->dwRefreshTime	= dwCurTime;
		}
	}
}

//-----------------------------------------------------------------------------
// ��ȡ����װ��
//-----------------------------------------------------------------------------
VOID EquipClanTreasury::GetRareEquips(OUT tagMsgShopEquip* pEquips, BYTE byShelf, INT16 &n16RareEquipNum)
{
	tagTreasuryEquip *pShopEquip = NULL;
	INT16 n16RealNum = 0;
	for(INT16 i=m_n16RareIndexStart[byShelf]; i< m_Shelf[byShelf].GetCurSpaceSize(); ++i)
	{
		pShopEquip = m_Shelf[byShelf].GetItem((INT16)(i));
		ASSERT(P_VALID(pShopEquip));

		if(	pShopEquip->IsAct() &&
			P_VALID(pShopEquip->pEquip))
		{
			pEquips[n16RealNum].n16RemainNum		= pShopEquip->n16RemainNum;
			pEquips[n16RealNum].n16Index			= pShopEquip->n16Index;
			pEquips[n16RealNum].byProtoIndex		= pShopEquip->byProtoIndex;

			IFASTCODE->MemCpy(&pEquips[n16RealNum].Equip, pShopEquip->pEquip, SIZE_EQUIP);
			++n16RealNum;
		}
	}
	ASSERT(n16RealNum == GetRareGoodsNum(byShelf));
	n16RareEquipNum = n16RealNum;
}

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD EquipClanTreasury::SellEquip(Role *pRole, BYTE byShelf, DWORD dwTypeID, INT64 n64Serial, OUT tagSellEquip &SellEquip)
{
	ASSERT(P_VALID(pRole));
	ASSERT(P_VALID(m_pShopProto));

	// ����װ��
	tagTreasuryEquip *pShopEquip = m_Shelf[byShelf].GetItem(dwTypeID);
	if(!P_VALID(pShopEquip))
	{
		// ִ�е������ζ����Ϣ�е�dwTypeID�Ƿ�
		ASSERT(P_VALID(pShopEquip));
		return E_Shop_ItemNotFind;
	}

	// �Ƿ񱻼���tbc:
	if (!pShopEquip->IsAct())
	{
		return E_Shop_ItemNotActived;
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
		ASSERT(pShopEquip->IsAct());
		pShopEquip->pEquip = (tagEquip*)ItemCreator::CreateTreasureEx(	pShopEquip->dwNameID, EICM_ShopNPC, m_dwNPCID, dwTypeID, 
																		1, (EItemQuality)SellEquip.pEquipSell->equipSpec.byQuality);
		if(!P_VALID(pShopEquip->pEquip))
		{
			// ִ�е�����,Ӧ�����ڴ�����ʧ��.(ԭ�Ϳ϶������ҵ�)
			ASSERT(P_VALID(pShopEquip->pEquip));
			// return E_Shop_CreateEquip_Failed;
			return E_Success;
		}
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

//-----------------------------------------------------------------------------
// �����䱦
//-----------------------------------------------------------------------------
BOOL EquipClanTreasury::ActiveTreasure( DWORD dwTypeID, DWORD dwNameID )
{
	ASSERT(GT_VALID(dwNameID));
	BOOL bRtv = FALSE;
	tagTreasuryEquip* pToAct = NULL;

	for (INT i=0; i<MAX_SHOP_SHELF_NUM; ++i)
	{
		pToAct = m_Shelf->GetItem(dwTypeID);
		if (P_VALID(pToAct))
		{
			pToAct->SetAct(dwNameID);
			
			EItemQuality eQuality = EIQ_White;
			if (GT_VALID(pToAct->n16RemainNum))
			{
				eQuality = (EItemQuality)pToAct->pRareProto->byQuality;
				m_n16ActRareNum[i]++;
			}
			m_n16ActNum[i]++;

			ASSERT(!P_VALID(pToAct->pEquip));
			pToAct->pEquip = (tagEquip *)ItemCreator::CreateTreasureEx(dwNameID, EICM_ShopNPC, m_dwNPCID, dwTypeID, 1, eQuality);
			
			bRtv = TRUE;
			break;
		}
	}
	return bRtv;
}