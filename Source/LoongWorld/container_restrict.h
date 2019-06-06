//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: container_restrict.h
// author: Sxg
// actor:
// data: 2008-08-05
// last:
// brief: ��Ʒ��������Լ����
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "container.h"
#include "../WorldDefine/ItemDefine.h"

//****************** ��Ʒ��������Լ���� -- ���� **********************
class ContainerRestrict
{
public:
	ContainerRestrict() {};
	virtual ~ContainerRestrict() {};

public:
	VOID init(ItemContainer *pCon) { ASSERT(pCon != NULL); m_pItemContainer = pCon; };

public:
	virtual BOOL CanAdd(const tagItem *pItem)		{ return TRUE; }
	virtual BOOL CanMove(const tagItem *pItem)		{ return TRUE; }
	virtual BOOL CanRemove(const tagItem *pItem)	{ return TRUE; }
	BOOL CanMoveToOther(const tagItem *pItem, ItemContainer *pConDst) 
	{ 
		return (this->CanRemove(pItem) && pConDst->GetRestrict()->CanAdd(pItem)); 
	}

protected:
	ItemContainer*		m_pItemContainer;
};


//************************ ��ɫ�ֿ����Լ���� ************************
class WareRestrict: public ContainerRestrict
{
public:
	virtual BOOL CanRemove(const tagItem *pItem)
	{
		// ���Ϊ����Ʒ,��ֻ��������ɫ������ȡ
		if(pItem->IsBind())
		{
			if(pItem->dwOwnerID != m_pItemContainer->GetOwnerID())
			{
				return FALSE;
			}
		}

		return TRUE;
	}
};

//************************ ������Ʒ������Լ���� ************************
class QusetItemBarRestrict: public ContainerRestrict
{
public:
	virtual BOOL CanAdd(const tagItem *pItem)
	{
		if(!MIsQuestItem(pItem->pProtoType))
		{
			return FALSE;
		}

		return TRUE;
	}

	virtual BOOL CanMove(const tagItem *pItem)		{ return FALSE; }
	virtual BOOL CanRemove(const tagItem *pItem)	{ return FALSE; }
};

//************************ �ٱ�������Լ���� ************************
class BaiBaoBagRestrict: public ContainerRestrict
{
public:
	virtual BOOL CanAdd(const tagItem *pItem)	{ return FALSE; }
	virtual BOOL CanMove(const tagItem *pItem)	{ return FALSE; }
	virtual BOOL CanRemove(const tagItem *pItem)
	{
		// ���Ϊ����Ʒ,��ֻ��������ɫ������ȡ
		if(pItem->IsBind())
		{
			if(pItem->dwOwnerID != m_pItemContainer->GetOwnerID())
			{
				return FALSE;
			}
		}

		return TRUE;
	}
};

//************************ ���ɲֿ����Լ���� ***********************
class GuildWareRestrict : public ContainerRestrict
{
public:
	virtual BOOL CanAdd(const tagItem *pItem)
	{
		if (pItem->IsBind() || pItem->IsTimeLimit() || pItem->bLock)
		{
			return FALSE;
		}
		
		return TRUE;
	}
};