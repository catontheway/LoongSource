//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Container.cpp
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: ��Ʒ������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Container.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/item_define.h"
#include "../WorldDefine/msg_item.h"
#include "att_res.h"
#include "container_restrict.h"
#include "item_creator.h"

//****************************************** ItemContainer ***********************************************

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
ItemContainer::ItemContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize, 
							 DWORD dwOwnerID, DWORD dwAccountID, ContainerRestrict *pRestrict/*=NULL*/)
							 : Container(eConType, n16CurSize, n16MaxSize), m_TimeLimitMgr(ITEM_UPDATE_TIME)
{
	m_n16MinFreeIndex = 0;

	m_dwOwnerID = dwOwnerID;
	m_dwAccountID = dwAccountID;

	m_pRestrict = (pRestrict != NULL ? pRestrict : new ContainerRestrict);
	m_pRestrict->init(this);
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
ItemContainer::~ItemContainer()
{
	m_n16MinFreeIndex = GT_INVALID;

	SAFE_DEL(m_pRestrict);
}

//-------------------------------------------------------------------------------------------------------
// ���������������Ʒ,����ָ�����λ��,���ش�����
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Add(tagItem* pItem, OUT INT16 &n16Index, OUT tagItemMove &ItemMove, 
						 BOOL bCheckAdd/* = TRUE*/, BOOL bChangeOwner/* = TRUE*/)
{
	ASSERT(pItem != NULL);

	if(pItem->n16Num <= 0)
	{
		return GT_INVALID;
	}

	if(bCheckAdd && !m_pRestrict->CanAdd(pItem))
	{
		return E_Item_CanNotAdd;
	}

	// ��ʼ����������
	n16Index = GT_INVALID;
	ItemMove.pItem2	= NULL;
	ItemMove.n16NumRes1	= 0;
	ItemMove.n16NumRes2	= 0;
	ItemMove.bCreateItem = FALSE;
	ItemMove.bOverlap = FALSE;
	ItemMove.bChangePos = TRUE;

	INT16 n16Add = 0;
	
	if(pItem->pProtoType->n16MaxLapNum > 1)	// �ɶѵ���Ʒ
	{
		TList<tagItem*> itemList;
		GetSameItemList(itemList, pItem->dwTypeID);

		tagItem *pItemInCon;
		INT16 n16CanLap;

		// ��ͬ����Ʒ����һ��
		itemList.ResetIterator();
		while(itemList.PeekNext(pItemInCon))
		{
			n16CanLap = pItemInCon->pProtoType->n16MaxLapNum - pItemInCon->n16Num;
			if(n16CanLap >= pItem->n16Num)
			{
				if(pItem->dwOwnerID != pItemInCon->dwOwnerID
					&& pItem->IsBind())
				{
					continue;
				}
				
				n16Add = pItem->n16Num;
				pItemInCon->n16Num += n16Add;

				// ���ô�������
				ItemMove.pItem2	= pItemInCon;
				ItemMove.n16NumRes1	= 0;
				ItemMove.n16NumRes2	= pItemInCon->n16Num;
				ItemMove.bOverlap	= TRUE;

				n16Index = pItemInCon->n16Index;

				// ���ø���������Ϣλ
				pItemInCon->SetUpdate(EUDBS_Update);

				break;
			}
		}
	}
	
	// ͬ����Ʒ�зŲ���,������λ
	if(0 == n16Add)
	{		
		// ���ô�������
		ItemMove.n16NumRes1	= pItem->n16Num;
		n16Index = GetOneFreeSpace();

		if(GT_INVALID == n16Index)
		{
			IMSG(_T("Container is full!<roleid:%u, eConType:%d>"), m_dwOwnerID, GetConType());
			return E_Con_NotEnoughSpace;
		}
		
		return this->Add(pItem, n16Index, bChangeOwner, FALSE);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ָ��λ���������Ʒ,������ӳɹ���Ʒ����(ָ��λ�ñ���Ϊ��)
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Add(tagItem* pItem, INT16 n16NewIndex, BOOL bChangeOwner/* = TRUE*/, BOOL bCheckAdd/* = TRUE*/)
{
	// ��λ�úϷ����ж�
	//if(n16NewIndex < 0 || n16NewIndex >= GetCurSpaceSize())
	//{
	//	ASSERT(n16NewIndex >=0 && n16NewIndex < GetCurSpaceSize());
	//	return GT_INVALID;
	//}
	
	// �Ƿ�ɷ�������
	if(bCheckAdd && !m_pRestrict->CanAdd(pItem))
	{
		return E_Item_CanNotAdd;
	}
	
	// ������λ���Ƿ�Ϸ�
	if(!IsOnePlaceFree(n16NewIndex))
	{
		return E_Item_Place_NotFree;
	}

	// �ж���Ʒ�����Ϸ���
	if(pItem->n16Num <= 0)
	{
		return GT_INVALID;
	}

	INT16 n16Add = Container::Add(pItem, n16NewIndex);
	if(0 == n16Add)
	{
		return E_Item_AddFailed;
	}
	
	ASSERT(n16Add == pItem->n16Num);
	if(bChangeOwner && !pItem->IsBind())
	{
		pItem->SetOwner(m_dwOwnerID, m_dwAccountID);
	}

	// �Ƿ�Ϊʱ����Ʒ
	if(pItem->IsTimeLimit())
	{
		m_TimeLimitMgr.Add2TimeLeftList(pItem->n64Serial, pItem->pProtoType->dwTimeLimit, pItem->dw1stGainTime);
	}

	//// ���ø���������Ϣλ
	//pItem->SetUpdate(EUDBS_Update);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��ָ����Ʒ,���ش�����(�ô������Ķ��巵��ֵ��ͬ)
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Remove(INT64 n64Serial, BOOL bChangeOwner/* = FALSE*/, BOOL bCheckRemove/* = TRUE*/)
{
	tagItem *pItem = GetItem(n64Serial);
	if(NULL == pItem)
	{
		return E_Item_NotFound;
	}

	if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
	{
		return E_Item_CanNotRemove;
	}

	INT16 n16OldIndex = pItem->n16Index;
	Container::Remove(n64Serial);

	if(bChangeOwner)
	{
		pItem->SetOwner(GT_INVALID, GT_INVALID);
	}

	// ����m_n16MinFreeIndex
	if(n16OldIndex < m_n16MinFreeIndex)
	{
		m_n16MinFreeIndex = n16OldIndex;
	}

	// �Ƿ�Ϊʱ����Ʒ
	if(pItem->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pItem->n64Serial);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��ָ��������ָ����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Remove(INT64 n64Serial, INT16 n16Num, BOOL bCheckRemove/* = TRUE*/)
{
	if(n16Num <= 0)
	{
		ASSERT(n16Num > 0);
		return GT_INVALID;
	}

	tagItem *pItem = GetItem(n64Serial);
	if(NULL == pItem)
	{
		return E_Item_NotFound;
	}

	if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
	{
		return E_Item_CanNotRemove;
	}

	// ����
	if(pItem->n16Num < n16Num)
	{
		// ɾ��ʧ��,���ͼ����Ϣ //??

		ASSERT(pItem->n16Num >= n16Num);
		return GT_INVALID;
	}

	// �պ�
	if(pItem->n16Num == n16Num)
	{
		// ִ�е��ô��������ڴ�й© -- ���Ӧ���ñ�Ľӿ�
		ASSERT(0);
		return this->Remove(n64Serial, TRUE, FALSE);
	}

	// �и���
	pItem->n16Num -= n16Num;

	// ����
	pItem->SetUpdate(EUDBS_Update);

	return E_Success;
}

////-------------------------------------------------------------------------------------------------------
//// ��������ɾ��ĳ����Ʒ�����سɹ�����
////-------------------------------------------------------------------------------------------------------
//INT32 ItemContainer::Remove(DWORD dwTypeID, BOOL bCheckRemove/* = TRUE*/)
//{
//	INT32 nNumDel = 0;
//
//	TList<tagItem*> itemList;
//	GetSameItemList(itemList, dwTypeID);
//
//	tagItem *pItem;
//	itemList.ResetIterator();
//	while(itemList.PeekNext(pItem))
//	{		
//		nNumDel += this->Remove(pItem->n64Serial, TRUE, bCheckRemove);
//	}
//
//	return nNumDel;
//}
//
////-------------------------------------------------------------------------------------------------------
//// ��������ɾ��ָ��������ĳ����Ʒ�����سɹ�����
////-------------------------------------------------------------------------------------------------------
//INT32 ItemContainer::Remove(DWORD dwTypeID, INT32 nNum, BOOL bCheckRemove/* = TRUE*/)
//{
//	if(nNum <= 0)
//	{
//		ASSERT(nNum > 0);
//		return 0;
//	}
//	
//	TList<tagItem*> itemList;
//	INT32 nNumTotal = GetSameItemList(itemList, dwTypeID);
//
//	// ����
//	if(nNumTotal < nNum)
//	{
//		// ɾ��ʧ��,���ͼ����Ϣ
//
//		ASSERT(nNumTotal >= nNum);
//		return 0;
//	}
//
//	// �պ�, ����
//	INT16 n16NumDel = 0;
//	INT32 nNumNeedDel = nNum;
//	tagItem *pItem = NULL;
//	itemList.ResetIterator();
//	while(itemList.PeekNext(pItem) && nNumNeedDel != 0)
//	{
//		if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
//		{
//			ASSERT(0 == n16NumDel);	
//			continue;
//		}
//
//		if(pItem->n16Num <= nNumNeedDel)
//		{
//			n16NumDel = this->Remove(pItem->n64Serial, TRUE, FALSE);
//			ASSERT(n16NumDel == pItem->n16Num);
//			nNumNeedDel -= n16NumDel;
//		}
//		else
//		{
//			pItem->n16Num -= nNumNeedDel;
//			nNumNeedDel = 0;
//
//			// ���ݿⱣ��״̬
//		}
//	}
//
//	return nNum - nNumNeedDel;
//}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ�ƶ���ָ��λ��
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64Serial1, INT16 n16Index2, OUT tagItemMove &ItemMove)
{
	// 0a.Ŀ��λ�úϷ��Լ��
	if(n16Index2 < 0 || n16Index2 >= GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	tagItem *pItem1, *pItem2;

	// 0b.����ƶ�Ŀ��
	pItem1 = GetItem(n64Serial1);
	if(NULL == pItem1 || pItem1->n16Index == n16Index2)
	{
		ASSERT(pItem1 != NULL);
		ASSERT(pItem1->n16Index != n16Index2);
		return GT_INVALID;
	}

	// ��ʼ����������
	ItemMove.pItem2			= NULL;
	ItemMove.n16NumRes1		= pItem1->n16Num;
	ItemMove.n16NumRes2		= 0;
	ItemMove.bCreateItem	= FALSE;
	ItemMove.bOverlap		= FALSE;
	ItemMove.bChangePos		= TRUE;

	// 1��Ŀ��λ��Ϊ��
	if(IsOnePlaceFree(n16Index2))
	{
		DWORD dwErrorCode = this->Remove(n64Serial1, FALSE);
		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}

		return this->Add(pItem1, n16Index2, FALSE);
	}

	// 2��Ŀ��λ�ò�Ϊ��
	pItem2 = GetItem(n16Index2);
	if(!P_VALID(pItem2))
	{
		ASSERT(P_VALID(pItem2));
		return GT_INVALID;
	}
	
	// �õ�Ŀ��λ����Ʒ��Ϣ
	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->n16Num;

	// 2a.��Ʒ����(TypeID || ����ƷOwnerID)��ͬ,��ĳһ����Ʒ�����ﵽ�˶ѵ�����
	if(pItem1->dwTypeID != pItem2->dwTypeID 
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem1->n16Num >= pItem1->pProtoType->n16MaxLapNum
		|| pItem2->n16Num >= pItem2->pProtoType->n16MaxLapNum)
	{
		Swap(pItem1->n16Index, pItem2->n16Index);

		return E_Success;
	}

	// 2b.ͬ������Ʒ�������Ѿ�û�дﵽ�ѵ�����
	INT32 nNum = pItem1->pProtoType->n16MaxLapNum - pItem2->n16Num;
	nNum = min(nNum, pItem1->n16Num);
	pItem1->n16Num -= nNum;
	pItem2->n16Num += nNum;

	// ����������Ʒ��Ϣ
	ItemMove.bOverlap	= TRUE;
	ItemMove.bChangePos	= FALSE;
	ItemMove.n16NumRes1 = pItem1->n16Num;
	ItemMove.n16NumRes2 = pItem2->n16Num;

	// �������ݿⱣ��״̬
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	// ��Ʒ1ȫ���ƶ�����Ʒ2�� -- ���ⲿ��飬�������ڴ�
	if(0 == pItem1->n16Num)
	{
		this->Remove(pItem1->n64Serial, TRUE, FALSE);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ָ��������Ʒ�ƶ���ָ��λ��,�����ƶ��ɹ��ĸ���
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64Serial1, INT16 n16NumMove, INT16 n16Index2, OUT tagItemMove &ItemMove)
{	
	if(n16NumMove <= 0)
	{
		ASSERT(n16NumMove > 0);
		return GT_INVALID;
	}
	
	tagItem *pItem1 = GetItem(n64Serial1);
	if(NULL == pItem1)
	{
		ASSERT(pItem1 != NULL);
		return E_Item_NotFound;
	}

	if(pItem1->n16Num <= n16NumMove)
	{
		// ȫ���ƶ�
		return this->MoveTo(n64Serial1, n16Index2, ItemMove);
	}

	// ��ʼ����������
	ItemMove.pItem2			= NULL;
	ItemMove.n16NumRes1		= pItem1->n16Num;
	ItemMove.n16NumRes2		= 0;
	ItemMove.bCreateItem	= FALSE;
	ItemMove.bOverlap		= FALSE;
	ItemMove.bChangePos		= FALSE;

	// 0.Ŀ��λ�úϷ��Լ��
	if(n16Index2 < 0 || n16Index2 >= GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	// 1��Ŀ��λ��Ϊ��
	if(IsOnePlaceFree(n16Index2))
	{
		pItem1->n16Num -= n16NumMove;

		// �������ݿⱣ��״̬
		pItem1->SetUpdate(EUDBS_Update);

		// ����������Ʒ��Ϣ
		ItemMove.n16NumRes1 = pItem1->n16Num;

		// �����µĶѵ���Ʒ
		tagItem *pNewItem = ItemCreator::Create(*pItem1, n16NumMove);

		// �õ�Ŀ��λ����Ʒ��Ϣ
		ItemMove.pItem2 = pNewItem;
		ItemMove.n16NumRes2 = pNewItem->n16Num;
		ItemMove.bCreateItem = TRUE;

		return this->Add(pNewItem, n16Index2, FALSE, FALSE);
	}

	// 2��Ŀ��λ�ò�Ϊ��
	if(pItem1->n16Index == n16Index2)
	{
		ASSERT(pItem1->n16Index != n16Index2);
		return GT_INVALID;
	}

	// ����Ʒ�ش��ڣ����򣬲���ִ�е��˴�
	tagItem *pItem2 = GetItem(n16Index2);

	// �õ�Ŀ��λ����Ʒ��Ϣ
	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->n16Num;

	// 2a.��Ʒ���Բ�ͬ,���ƶ��󳬹��ѵ�����
	if(pItem1->dwTypeID != pItem2->dwTypeID
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem2->n16Num + n16NumMove > pItem2->pProtoType->n16MaxLapNum)
	{
		return GT_INVALID;
	}

	// 2b.ͬ������Ʒ�����ƶ���Ŀ����ƷҲ����ﵽ�ѵ�����
	pItem1->n16Num -= n16NumMove;
	pItem2->n16Num += n16NumMove;

	// ����������Ʒ��Ϣ
	ItemMove.n16NumRes1 = pItem1->n16Num;
	ItemMove.n16NumRes2 = pItem2->n16Num;
	ItemMove.bOverlap	= TRUE;

	// �������ݿⱣ��״̬
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ�ƶ�������������(�ֿ�ͱ�����)����ָ��Ŀ�������о���λ��
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, 
							OUT tagItemMove &ItemMove, OUT INT16 &n16IndexDst)
{
	// ���Ŀ���������Ƿ��пռ�
	if(conDst.GetFreeSpaceSize() < 1)
	{
		return GT_INVALID;
	}
	
	// ��ԭ������ȡ�����ƶ���Ʒ
	tagItem *pItem = GetItem(n64SerialSrc);
	if(NULL == pItem)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pItem != NULL);
		return GT_INVALID;
	}

	if(!m_pRestrict->CanMoveToOther(pItem, &conDst))
	{
		return E_Item_CanNotMove_Other;
	}

	this->Remove(n64SerialSrc, FALSE, FALSE);
	
	// ����Ŀ��������
	return conDst.Add(pItem, n16IndexDst, ItemMove, FALSE, TRUE);
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ�ƶ�������������(�ֿ�ͱ�����)��ָ��Ŀ�걳���о���λ��
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, INT16 n16IndexDst, OUT tagItemMove &ItemMove)
{
	// 0.Ŀ��λ�úϷ��Լ��
	if(n16IndexDst < 0 || n16IndexDst >= conDst.GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	tagItem *pItem1, *pItem2;

	// ��ô��ƶ���Ʒָ��
	pItem1 = GetItem(n64SerialSrc);
	if(NULL == pItem1)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���		
		ASSERT(pItem1 != NULL);
		return GT_INVALID;
	}

	// ��ʼ����������
	ItemMove.pItem2 = NULL;
	ItemMove.n16NumRes2 = 0;
	ItemMove.n16NumRes1 = pItem1->n16Num;
	ItemMove.bCreateItem = FALSE;
	ItemMove.bOverlap = FALSE;
	ItemMove.bChangePos = TRUE;

	if(!m_pRestrict->CanMoveToOther(pItem1, &conDst))
	{
		return E_Item_CanNotMove_Other;
	}

	// 1��Ŀ��λ��Ϊ��
	if(conDst.IsOnePlaceFree(n16IndexDst))
	{		
		this->Remove(n64SerialSrc);
		return conDst.Add(pItem1, n16IndexDst, TRUE);
	}

	// 2��Ŀ��λ�ò�Ϊ��
	pItem2 = conDst.GetItem(n16IndexDst);

	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->n16Num;

	// 2a.��Ʒ���Բ�ͬ,��ĳһ����Ʒ�����ﵽ�˶ѵ�����
	if(pItem1->dwTypeID != pItem2->dwTypeID
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem1->n16Num >= pItem1->pProtoType->n16MaxLapNum
		|| pItem2->n16Num >= pItem2->pProtoType->n16MaxLapNum)
	{
		if(!conDst.GetRestrict()->CanMoveToOther(pItem2, this))
		{
			return E_Item_CanNotMove_Other;
		}
		
		// ��¼ԭ��λ��
		INT16 n16IndexSrc = pItem1->n16Index;

		// �������������Ʒ
		Container::Remove(n16IndexSrc);
		((Container *)&conDst)->Remove(n16IndexDst);

		// �Ż�
		this->Add(pItem2, n16IndexSrc, TRUE);
		conDst.Add(pItem1, n16IndexDst, TRUE);

		return E_Success;
	}

	// 2b.ͬ������Ʒ�������Ѿ�û�дﵽ�ѵ�����
	INT32 nNum = pItem2->pProtoType->n16MaxLapNum - pItem2->n16Num;
	nNum = min(nNum, pItem1->n16Num);
	pItem1->n16Num -= nNum;
	pItem2->n16Num += nNum;

	ItemMove.n16NumRes1 = pItem1->n16Num;
	ItemMove.n16NumRes2 = pItem2->n16Num;
	ItemMove.bOverlap	= TRUE;
	ItemMove.bChangePos = FALSE;

	// �������ݿⱣ��״̬
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	// ��Ʒ1ȫ���ƶ�����Ʒ2��
	if(0 == pItem1->n16Num)
	{
		this->Remove(pItem1->n64Serial, TRUE);
	}

	return E_Success;
}

//****************************************** EquipContainer ***********************************************

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
EquipContainer::EquipContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize)
								: Container(eConType, n16CurSize, n16MaxSize), m_TimeLimitMgr(ITEM_UPDATE_TIME)
{
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
EquipContainer::~EquipContainer()
{
}

//-------------------------------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Add(tagEquip *pEquip, EEquipPos ePos)
{
	// ����λ
	if(!IsOnePlaceFree(ePos))
	{
		return E_Item_Place_NotFree;
	}
	
	// ����������
	if(Container::Add(pEquip, ePos) < 1)
	{
		return E_Equip_OnFailed;
	}

	//// �������ݿⱣ��״̬
	//pEquip->SetUpdate(EUDBS_Update);

	// �Ƿ�Ϊʱ����Ʒ
	if(pEquip->IsTimeLimit())
	{
		m_TimeLimitMgr.Add2TimeLeftList(pEquip->n64Serial, pEquip->pProtoType->dwTimeLimit, pEquip->dw1stGainTime);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ƶ�(����������ָλ)
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::MoveTo(INT64 n64SerialSrc, EEquipPos ePosDst)
{
	// ���Ŀ��λ���Ƿ�Ϊ��ָλ
	if(ePosDst != EEP_Finger1 && ePosDst != EEP_Finger2)
	{
		return E_Equip_NotRingPos;
	}

	// ��ȡ���ƶ�װ��
	tagEquip *pEquip = GetItem(n64SerialSrc);
	if(NULL == pEquip)
	{
		ASSERT(pEquip != NULL);
		return GT_INVALID;
	}

	// ���ƶ�װ���Ƿ�Ϊring
	if(!MIsRing(pEquip))
	{
		return E_Equip_NotRing;
	}

	// �Ƿ�����Ŀ��λ��
	if(pEquip->n16Index == ePosDst)
	{
		return E_Equip_SamePos;
	}

	// �ƶ�
	if(IsOnePlaceFree(ePosDst))	// Ŀ��λ��Ϊ��
	{
		Container::Remove(n64SerialSrc);
		return this->Add(pEquip, ePosDst);
	}
	else // Ŀ��λ����װ��(ring)
	{
		Container::Swap(pEquip->n16Index, ePosDst);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����װ��
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Equip(ItemContainer &bagSrc, INT64 n64SerialSrc, EEquipPos ePosDst)
{
	// ȡ�ô���Ʒָ��
	tagItem *pItemNew = bagSrc.GetItem(n64SerialSrc);
	if(NULL == pItemNew)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pItemNew != NULL);
		return E_Item_NotFound;
	}

	// �ж��Ƿ�Ϊװ��
	if(!MIsEquipment(pItemNew->dwTypeID))
	{
		return E_Item_NotEquipment;
	}

	tagEquip *pEquipNew = (tagEquip *)pItemNew;

	// �õ���װ��λ��
	EEquipPos ePos = (pEquipNew->pEquipProto)->eEquipPos;
	
	// ����Ƿ����װ����Ŀ��λ��(ringҪ�������ж�)
	// if(!(ePos == ePosDst || (ePos + 1 == ePosDst && MIsRing(pEquipNew))))
	if(ePos != ePosDst && (ePos + 1 != ePosDst || (!MIsRing(pEquipNew) && !MIsWeapon(pEquipNew))))
	{
		return E_Equip_InvalidPos;
	}

	INT16 n16IndexBag = pEquipNew->n16Index;

	// Ŀ��λ��Ϊ��
	if(!IsOnePlaceFree(ePosDst))
	{
		// �ӱ�����ȡ��װ��
		((Container *)&bagSrc)->Remove(n16IndexBag);
		// ��װ������ȡ��װ��
		tagEquip *pEquipOld = Container::Remove((INT16)ePosDst);
		// ��ԭװ�����е�װ�����뱳����
		bagSrc.Add(pEquipOld, n16IndexBag, FALSE);
	}
	else
	{
		// �ӱ�����ȡ��װ��
		bagSrc.Remove(n64SerialSrc);
	}

	// ��ԭ��������Ʒװ����
	return this->Add(pEquipNew, ePosDst);
}

//-------------------------------------------------------------------------------------------------------
// ����װ��
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Unequip(INT64 n64SerialSrc, ItemContainer &bagDst)
{
	//// ��鱳�����Ƿ��пռ�
	//if(bagDst.GetFreeSpaceSize() < 1)
	//{
	//	return E_Bag_NotEnoughSpace;
	//}

	//// ����װ��
	//tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	//if(NULL == pEquipSrc)
	//{
	//	// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
	//	ASSERT(pEquipSrc != NULL);
	//	return E_Item_NotFound;
	//}

	//// ���뱳����
	//bagDst.Add(pEquipSrc, bagDst.GetOneFreeSpace(), FALSE);

	//return E_Success;

	return Unequip(n64SerialSrc, bagDst, bagDst.GetOneFreeSpace());
}

//-------------------------------------------------------------------------------------------------------
// ����װ��(ָ��������λ��)
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Unequip(INT64 n64SerialSrc, ItemContainer &bagDst, INT16 n16IndexDst)
{
	// ���Ŀ��λ���Ƿ�Ϸ�
	if(n16IndexDst < 0 || n16IndexDst > bagDst.GetCurSpaceSize() - 1)
	{
		return GT_INVALID;
	}
	
	// ��鱳����ָ��λ���Ƿ�����Ʒ�������ߴ���װ������
	if(!bagDst.IsOnePlaceFree(n16IndexDst))
	{
		tagEquip *pEquip = GetItem(n64SerialSrc);
		if(NULL == pEquip)
		{
			// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
			ASSERT(pEquip != NULL);
			return E_Item_NotFound;
		}
		
		return Equip(bagDst, bagDst.GetItem(n16IndexDst)->n64Serial, (EEquipPos)pEquip->n16Index);
	}

	// ����װ��
	tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	if(NULL == pEquipSrc)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pEquipSrc != NULL);
		return E_Item_NotFound;
	}

	// �Ƿ�Ϊʱ����Ʒ
	if(pEquipSrc->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pEquipSrc->n64Serial);
	}

	// ���뱳����
	bagDst.Add(pEquipSrc, n16IndexDst, FALSE);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �����ֽ���
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::SwapWeapon()
{
	tagEquip *pEquipRight	= GetItem((INT16)EEP_RightHand);
	tagEquip *pEquipLeft	= GetItem((INT16)EEP_LeftHand);

	if(P_VALID(pEquipRight))
	{
		Container::Remove((INT16)EEP_RightHand);
	}

	if(P_VALID(pEquipLeft))
	{
		Container::Remove((INT16)EEP_LeftHand);
	}

	if(P_VALID(pEquipRight))
	{
		Add(pEquipRight, EEP_LeftHand);
	}

	if(P_VALID(pEquipLeft))
	{
		Add(pEquipLeft, EEP_RightHand);
	}

	return E_Success;
}