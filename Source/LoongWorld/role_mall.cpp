//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_mall.cpp
// author: Sxg
// actor:
// data: 2009-2-1
// last:
// brief: �̳Ǵ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_mall.h"
#include "../WorldDefine/mall_define.h"
#include "../ServerDefine/msg_yuanbao.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "role.h"
#include "mall.h"
#include "role_mgr.h"
#include "TradeYuanBao.h"
#include "guild.h"
#include "guild_mgr.h"
#include "world.h"

//-----------------------------------------------------------------------------
// ��ȡ�̳���������Ʒ
//-----------------------------------------------------------------------------
DWORD Role::GetMallAll(OUT DWORD &dwMallTime)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// ��ȡ�������̳Ǽ���ʱ��
	dwMallTime = g_mall.GetMallTime();

	INT nGoodsNum, nSzMsg;

	// ��ͨ��Ʒ
	nGoodsNum = g_mall.GetItemNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallItem) - 1 + sizeof(tagMallItemProto) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallItem);
		pSend->nItemNum = nGoodsNum;
		g_mall.GetAllItems(pSend->byData);

		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	// ��Ʒ��
	nGoodsNum = g_mall.GetPackNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallPack) - 1 + sizeof(tagMallPackProto) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallPack);
		pSend->nPackNum = nGoodsNum;
		g_mall.GetAllPacks(pSend->byData);

		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}
	
	// �����ȡ��Ʒ(ֻ��1��)
	nGoodsNum = g_mall.GetFreeItemNum();
	if(nGoodsNum > 0)
	{
		tagNS_MallFreeItem send;
		g_mall.GetFreeItem(&send.freeItem);
		SendMessage(&send, send.dwSize);
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����̳����г��۸������Ƶ�������Ʒ����
//-----------------------------------------------------------------------------
DWORD Role::UpdateMallAll(OUT DWORD &dwNewMallTime, IN DWORD dwOldMallTime)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// ��ȡ�������̳Ǽ���ʱ��
	dwNewMallTime = g_mall.GetMallTime();

	//-- ���ͻ��˵��̳Ǿ�̬�����Ƿ����������ͬ

	// 1.��ͬ�����·����̵�ԭ����Ϣ
	if(dwNewMallTime != dwOldMallTime)
	{
		return GetMallAll(dwNewMallTime);
	}

	// 2.��ͬ��ֻˢ���и������Ƶ���Ʒ������Ϣ
	INT nGoodsNum, nSzMsg;

	// ��ͨ��Ʒ
	nGoodsNum = g_mall.GetItemNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);
		g_mall.UpdateAllItems(pSend->byData, pSend->nItemNum);

		// ���¼�����Ϣ��С
		if(pSend->nItemNum > 0)
		{
			pSend->dwSize = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate) * pSend->nItemNum;
			SendMessage(pSend, pSend->dwSize);
		}

		MDEL_MSG(pSend);
	}

	// ��Ʒ��
	nGoodsNum = g_mall.GetPackNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);
		g_mall.UpdateAllPacks(pSend->byData, pSend->nItemNum);

		// ���¼�����Ϣ��С
		if(pSend->nItemNum > 0)
		{
			pSend->dwSize = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate) * pSend->nItemNum;
			SendMessage(pSend, pSend->dwSize);
		}

		MDEL_MSG(pSend);
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����̳���Ʒ
//-----------------------------------------------------------------------------
DWORD Role::BuyMallItem(DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellItem(this, GetID(), ELCID_Mall_BuyItem, 
										dwID, nIndex, nUnitPrice, n16BuyNum, sItemSell);

	// ������
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// Ԫ�������̳��п۳�
		
		// ����Ʒ�ŵ���ұ�����
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCID_Mall_BuyItem, TRUE);
		
		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sItemSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, 
							GetNameID(), GT_INVALID, EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCID_Mall_BuyItem_Add);
		}

		// �����������
		if (sItemSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sItemSell.nExVolumeAssign, ELCID_Mall_BuyItem);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);
		
		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����̳���Ʒ��
//-----------------------------------------------------------------------------
DWORD Role::BuyMallPack(DWORD dwID, INT nUnitPrice, BYTE byIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallPackSell sPackSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellPack(this, GetID(), ELCID_Mall_BuyPack, 
										dwID, byIndex, nUnitPrice, sPackSell, TRUE);

	// ������
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{
		// ����Ʒ�ŵ���ұ�����
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			GetItemMgr().Add2Bag(sPackSell.pItem[i], ELCID_Mall_BuyPack, TRUE);
		}

		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sPackSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
								GetNameID(), GT_INVALID, EBBRT_Mall, sPackSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCID_Mall_BuyPack_Add);
		}

		// �����������
		if (sPackSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sPackSell.nExVolumeAssign, ELCID_Mall_BuyPack);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����̳���Ʒ������
//-----------------------------------------------------------------------------
DWORD Role::BuyMallItem(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
						DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// ������ID����Ϸ�������Ƿ����
	if(!g_roleMgr.IsRoleBelongToWorld(dwTgtRoleID))
	{
		return E_Role_Not_ExistIn_World;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellItem(this, dwTgtRoleID, ELCID_Mall_PresentItem, 
										dwID, nIndex, nUnitPrice, n16BuyNum, sItemSell);

	// ������
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// �ٱ�����ʷ��¼
		GetItemMgr().ProcBaiBaoRecord(sItemSell.pItem->dwTypeID, dwTgtRoleID, 
							GetNameID(), EBBRT_Friend, sItemSell.pItem->dw1stGainTime, szLeaveWord);

		// ����Ʒ�ŵ����Ѱٱ�����
		Role *pFriend = g_roleMgr.GetRolePtrByID(dwTgtRoleID);
		if(P_VALID(pFriend))
		{
			pFriend->GetItemMgr().Add2BaiBao(sItemSell.pItem, ELCID_Mall_PresentItem, GetID());
		}
		else
		{
			// �洢��item_baibao����
			ItemMgr::InsertBaiBao2DB(sItemSell.pItem, dwTgtRoleID, ELCID_Mall_PresentItem);
			
			// ɾ����Ʒ
			::Destroy(sItemSell.pItem);
		}

		// �������Ʒ����ŵ����Ѱٱ�����
		if(P_VALID(sItemSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, dwTgtRoleID, 
										GetNameID(), EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCID_Mall_PresentItem_Add);
			}
			else
			{
				// �洢��item_baibao����
				ItemMgr::InsertBaiBao2DB(sItemSell.pPresent, dwTgtRoleID, ELCID_Mall_PresentItem_Add);

				// ɾ����Ʒ
				::Destroy(sItemSell.pPresent);
			}
		}
		
		// ����һ�������
		if (sItemSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sItemSell.nExVolumeAssign, ELCID_Mall_PresentItem);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ������Ʒ��Ʒ��������
//-----------------------------------------------------------------------------
DWORD Role::BuyMallPack(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
						DWORD dwID, INT nUnitPrice, BYTE byIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// ������ID����Ϸ�������Ƿ����
	if(!g_roleMgr.IsRoleBelongToWorld(dwTgtRoleID))
	{
		return E_Role_Not_ExistIn_World;
	}

	tagMallPackSell sPackSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellPack(this, dwTgtRoleID, ELCID_Mall_PresentPack, 
										dwID, byIndex, nUnitPrice, sPackSell, FALSE);

	// ������
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{		
		Role *pFriend = g_roleMgr.GetRolePtrByID(dwTgtRoleID);

		// ����Ʒ�ŵ����Ѱٱ����� -- item_baibao��
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			// ��¼����һ����Ʒ��
			if(0 == i)
			{
				// �ٱ�����ʷ��¼
				GetItemMgr().ProcBaiBaoRecord(sPackSell.pItem[i]->dwTypeID, 
					dwTgtRoleID, GetNameID(), EBBRT_Friend, sPackSell.pItem[i]->dw1stGainTime, szLeaveWord);
			}
			else
			{
				// �ٱ�����ʷ��¼(������)
				GetItemMgr().ProcBaiBaoRecord(sPackSell.pItem[i]->dwTypeID, 
					dwTgtRoleID, GetNameID(), EBBRT_Friend, sPackSell.pItem[i]->dw1stGainTime);
			}

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sPackSell.pItem[i], ELCID_Mall_PresentPack, GetID());
			}
			else
			{
				// �洢��item_baibao����
				ItemMgr::InsertBaiBao2DB(sPackSell.pItem[i], dwTgtRoleID, ELCID_Mall_PresentPack);

				// ɾ����Ʒ
				::Destroy(sPackSell.pItem[i]);
			}
		}

		// �������Ʒ����ŵ����Ѱٱ�����
		if(P_VALID(sPackSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
							dwTgtRoleID, GetNameID(), EBBRT_Mall, sPackSell.pPresent->dw1stGainTime);

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCID_Mall_PresentPack_Add, GetID());
			}
			else
			{
				// �洢��item_baibao����
				ItemMgr::InsertBaiBao2DB(sPackSell.pPresent, dwTgtRoleID, ELCID_Mall_PresentPack_Add);

				// ɾ����Ʒ
				::Destroy(sPackSell.pPresent);
			}
		}

		// ����һ�������
		if (sPackSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sPackSell.nExVolumeAssign, ELCID_Mall_PresentPack);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ�̳������Ʒ
//-----------------------------------------------------------------------------
DWORD Role::GetMallFreeItem(DWORD dwID)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.GrantFreeItem(this, dwID, sItemSell);

	// ������
	if(E_Success == dwErrorCode && P_VALID(sItemSell.pItem))
	{
		// ����Ʒ�ŵ�������
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCID_Mall_FreeItem, TRUE);
	}
	
	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �һ��̳���Ʒ
//-----------------------------------------------------------------------------
DWORD Role::MallItemExchange(DWORD dwMallID, INT nPrice, INT16 n16BuyNum, BYTE byIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.ExchangeItem(this, ELCLD_Mall_ExchangeItem, dwMallID, byIndex, nPrice, n16BuyNum, sItemSell);

	// ������
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// ����Ʒ�ŵ���ұ�����
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCLD_Mall_ExchangeItem, TRUE);

		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sItemSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, 
				GetNameID(), GT_INVALID, EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCLD_Mall_ExchangeItem_Add);
		}

		// �����������
		if (sItemSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sItemSell.nExVolumeAssign, ELCLD_Mall_ExchangeItem);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwMallID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;

}

//-----------------------------------------------------------------------------
// �һ��̳Ǵ����Ʒ
//-----------------------------------------------------------------------------
DWORD Role::MallPackExchange(DWORD dwMallID, INT nPrice, BYTE byIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallPackSell sPackSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.ExchangePack(this, ELCLD_Mall_ExchangePack, dwMallID, byIndex, nPrice, sPackSell);

	// ������
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{
		INT64 n64Serial = sPackSell.pItem[0]->n64Serial;
		INT16 n16BuyNum = sPackSell.pItem[0]->n16Num;
		DWORD dwFstGainTime = g_world.GetWorldTime();

		// ����Ʒ�ŵ���ұ�����
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			GetItemMgr().Add2Bag(sPackSell.pItem[i], ELCLD_Mall_ExchangePack, TRUE);
		}

		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sPackSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
				GetNameID(), GT_INVALID, EBBRT_Mall, dwFstGainTime);

			GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCLD_Mall_ExchangePack_Add);
		}

		// �����������
		if (sPackSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sPackSell.nExVolumeAssign, ELCLD_Mall_ExchangePack);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwMallID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻���Ԫ��
//-----------------------------------------------------------------------------
DWORD Role::SaveYB2Account(DWORD dwID, INT nNum)
{
	// �����ұ���Ԫ������
	if(GetCurMgr().GetBagYuanBao() < nNum)
		return E_Trade_BagYB_NotEnough;

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
	{
		pYBAccount = g_tradeYB.CreateTradeAccount(dwID);
		if(!P_VALID(pYBAccount))
			return GT_INVALID;
	}

	pYBAccount->IncAccountYB(nNum, (DWORD)ELCID_Trade_SaveYuanBao, TRUE);
	GetCurMgr().DecBagYuanBao(nNum, (DWORD)ELCID_Trade_SaveYuanBao);
		
	return E_Success;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻����Ǯ
//-----------------------------------------------------------------------------
DWORD Role::SaveSilver2Account(DWORD dwID, INT64 nNum)
{
	// �����ұ�����Ǯ����
	if(GetCurMgr().GetBagSilver() < nNum)
		return E_Trade_BagSilver_NotEnough;

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
	{
		pYBAccount = g_tradeYB.CreateTradeAccount(dwID);
		if(!P_VALID(pYBAccount))
			return GT_INVALID;
	}

	pYBAccount->IncAccountSilver(nNum, (DWORD)ELCID_Trade_SaveSilver, TRUE);
	GetCurMgr().DecBagSilver(nNum, (DWORD)ELCID_Trade_SaveSilver);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻�ȡԪ��
//-----------------------------------------------------------------------------
DWORD Role::DepositYBAccout(DWORD dwID, INT nNum)
{
	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	// ����˻�Ԫ������
	if(pYBAccount->GetAccountYB() < nNum)
		return E_Trade_AccountYB_NotEnough;

	// �������Ƿ��ύ�����۶���
	tagYuanBaoOrder *pYBOrder = g_tradeYB.GetYBSellOrder(dwID);
	if(P_VALID(pYBOrder))
		return GT_INVALID;

	pYBAccount->DecAccountYuanBao(nNum, (DWORD)ELCID_Trade_DepositYuanBao, TRUE);
	GetCurMgr().IncBagYuanBao(nNum, (DWORD)ELCID_Trade_DepositYuanBao);
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻�ȡ��Ǯ
//-----------------------------------------------------------------------------
DWORD Role::DepositSilverAccount(DWORD dwID, INT64 nNum)
{
	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	// ����˻���Ǯ����
	if(pYBAccount->GetAccountSilver() < nNum)
		return E_Trade_AccountSilver_NotEnough;

	// �������Ƿ��ύ���չ�����
	tagYuanBaoOrder *pYBOrder = g_tradeYB.GetYBBuyOrder(dwID);
	if(P_VALID(pYBOrder))
		return GT_INVALID;

	pYBAccount->DecAccountSilver(nNum, (DWORD)ELCID_Trade_DepositSilver, TRUE);
	GetCurMgr().IncBagSilver(nNum, (DWORD)ELCID_Trade_DepositSilver);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ͬ��Ԫ�����׳�ʼ����Ϣ
//-----------------------------------------------------------------------------
DWORD Role::GetYBTradeInfo()
{
	g_tradeYB.SynBuyPriceList(this);
	g_tradeYB.SynSellPriceList(this);
	g_tradeYB.SynYBAccount(this);
	return E_Success;
}

//-----------------------------------------------------------------------------
// ����ύԪ�����۶���
//-----------------------------------------------------------------------------
DWORD Role::SubmitSellOrder(DWORD dwRoleID, INT nNum, INT nPrice)
{
	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	if(nNum <= 0 || nPrice <= 0)
		return GT_INVALID;
		
	// �Ƿ��Ѿ��ύ�����۶���
	tagYuanBaoOrder* pSellOrder = (tagYuanBaoOrder*)GT_INVALID;
	pSellOrder = g_tradeYB.GetYBSellOrder(dwRoleID);
	if(P_VALID(pSellOrder))
		return E_Trade_SellOrder_Exit;

	// �����˻�Ԫ���Ƿ��㹻
	if(pYBAccount->GetAccountYB() < nNum)
		return E_Trade_AccountYB_NotEnough;

	// ����������Ϊ�ܼ۵�2%
	INT nTax = nPrice * nNum * 2 / 100;
	if(nTax < 1)    nTax = 1;

	// ����������Ƿ��㹻
	if (GetCurMgr().GetBagSilver() < nTax)
		return E_Trade_Tax_NotEnough;


	tagYuanBaoOrder * pYBOrder = g_tradeYB.CreateYBOrder(dwRoleID, EYBOT_SELL, nPrice, nNum);
	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	// �����˻��ж������ύ״̬
	pYBAccount->SetSellOrder(TRUE);

	// �۳������� 
	GetCurMgr().DecBagSilver(nTax, ELCID_Trade_Tax);

	// ����Ԫ��
	g_tradeYB.DealYBSell(pYBOrder);
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// ����ύԪ���չ�����
//-----------------------------------------------------------------------------
DWORD Role::SubmitBuyOrder(DWORD dwRoleID, INT nNum, INT nPrice)
{
	tagYuanBaoAccount *pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	if(nNum <= 0 || nPrice <= 0)
		return GT_INVALID;

	if(nNum * nPrice <= 0)
		return E_Trade_AccountSilver_NotEnough;

	// �Ƿ��ѽ��ύ������
	tagYuanBaoOrder* pBuyOrder = (tagYuanBaoOrder*)GT_INVALID;
	pBuyOrder = g_tradeYB.GetYBBuyOrder(dwRoleID);
	if(P_VALID(pBuyOrder))
		return E_Trade_BuyOrder_Exit;

	// �����˻���Ǯ�Ƿ��㹻
	if(pYBAccount->GetAccountSilver() < nNum * nPrice)
		return E_Trade_AccountSilver_NotEnough;

	// ����������Ϊ�ܼ۵�2%
	INT nTax = (FLOAT)(nPrice * nNum) * 0.02f;
	if(nTax < 1)    nTax = 1;

	// ����������Ƿ��㹻
	if (GetCurMgr().GetBagSilver() < nTax)
		return E_Trade_Tax_NotEnough;

	tagYuanBaoOrder *pYBOrder = g_tradeYB.CreateYBOrder(dwRoleID, EYBOT_BUY, nPrice, nNum);
	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	// �����˻��ж������ύ״̬
	pYBAccount->SetBuyOrder(TRUE);

	// �۳������� 
	GetCurMgr().DecBagSilver(nTax, ELCID_Trade_Tax);

	// ����Ԫ��
	g_tradeYB.DealYBBuy(pYBOrder);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ɾ������
//-----------------------------------------------------------------------------
DWORD Role::DeleteOrder(DWORD dwRoleID, DWORD dwOrderID, EYBOTYPE eYBOType)
{
	if(eYBOType != EYBOT_BUY && eYBOType != EYBOT_SELL)
		return GT_INVALID;

	tagYuanBaoOrder *pYBOrder = (tagYuanBaoOrder*)GT_INVALID;
	if(eYBOType == EYBOT_BUY)
		pYBOrder = g_tradeYB.GetYBBuyOrder(dwRoleID);
	else
		pYBOrder = g_tradeYB.GetYBSellOrder(dwRoleID);

	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	if(pYBOrder->dwID != dwOrderID)
		return GT_INVALID;

	g_tradeYB.DeleteYBOrder(pYBOrder, EYBOM_Cancel);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ѯһ���ڸ���ҵ�Ԫ�����׶���
//-----------------------------------------------------------------------------
DWORD Role::GetYBOrder(DWORD dwRoleID)
{
	tagYuanBaoAccount *pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return  GT_INVALID;

	DWORD dwCurTick = g_world.GetWorldTick();
	if(dwCurTick - pYBAccount->GetQuestTick() > 50)
		pYBAccount->SetQuestTick(dwCurTick);
	else
		return GT_INVALID;

	// �����ݿⷢ�Ͳ�ѯ��Ϣ
	tagNDBC_GetRoleYBOrder	sendDB;
	sendDB.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return E_Success;
}