//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Container.h
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: ��Ʒ������
//-----------------------------------------------------------------------------
#pragma once

#include "container_template.h"
#include "time_limit_mgr.h"
#include "../WorldDefine/ItemDefine.h"

class ContainerRestrict;

struct tagItemMove;
//-----------------------------------------------------------------------------
// ��Ʒ&װ������
//-----------------------------------------------------------------------------
class ItemContainer: public Container<tagItem, INT64>
{
public:
	ItemContainer(EItemConType eConType, INT16 n16CurSize, INT16 n16MaxSize, 
					DWORD dwOwnerID, DWORD dwAccountID, ContainerRestrict *pRestrict = NULL);
	~ItemContainer();

	VOID Update();

public:
	// ���������������Ʒ,����ָ�����λ��,���ش�����
	DWORD Add(tagItem* pItem, OUT INT16 &n16Index, OUT tagItemMove &ItemMove, BOOL bCheckAdd = TRUE, BOOL bChangeOwner = TRUE);
	
	// ��������ָ��λ���������Ʒ,���ش�����(ָ��λ�ñ���Ϊ��).
	DWORD Add(tagItem* pItem, INT16 n16NewIndex, BOOL bChangeOwner = TRUE, BOOL bCheckAdd = TRUE);

	// ��������ɾ��ָ����Ʒ,���ش�����
	DWORD Remove(INT64 n64Serial, BOOL bChangeOwner = FALSE, BOOL bCheckRemove = TRUE);
	DWORD Remove(INT64 n64Serial, INT16 n16Num, BOOL bCheckRemove = TRUE);
	
	// ����Ʒ�ƶ���ָ��λ��(ͬһ������)
	DWORD MoveTo(INT64 n64Serial1, INT16 n16Index2, OUT tagItemMove &ItemMove);
	DWORD MoveTo(INT64 n64Serial1, INT16 n16NumMove, INT16 n16Index2, OUT tagItemMove &ItemMove);

	// ����Ʒ�ƶ�������������(�ֿ�ͱ�����)
	DWORD MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, OUT tagItemMove &ItemMove, OUT INT16 &n16IndexDst);
	DWORD MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, INT16 n16IndexDst, OUT tagItemMove &ItemMove);

	// ����
	VOID IncreaseSize(INT16 n16Size);

	// ��ָ��λ������
	BOOL Reorder(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num);
	BOOL ReorderEx(IN LPVOID pData, OUT LPVOID pOutData, OUT INT16 &n16OutNum, const INT16 n16Num);

public:
	// ���һ����λ��ʧ�ܷ���GT_INVALID
	INT16 GetOneFreeSpace();

	// ���������������Ʒ����ΪdwTypeID����Ʒ����
	INT32 GetSameItemCount(IN DWORD dwTypeID);

	// �����������Ʒ����ΪdwTypeID��lis, ����ʵ�ʻ�ø���t -- ָ��nNumʱ�����ҵ�nNum����Ʒ����
	INT32 GetSameItemList(OUT TList<tagItem*> &list, IN DWORD dwTypeID, IN INT32 nNum = INT_MAX);

	ContainerRestrict*	GetRestrict()	const;
	DWORD				GetOwnerID()	const;

	// ʱ�޹���ӿ�
	TList<INT64>& GetNeedDelList();
	VOID ClearNeedDelList();

private:
	// ���������п���λ�õ���С�±�
	VOID ResetMinFreeIndex();

protected:
	DWORD				m_dwOwnerID;			// ����������
	DWORD				m_dwAccountID;			// ����˺�ID, �ٱ����ͽ�ɫ�ֿ���(ͬһ�ʺ��½�ɫ����)

private:
	// ������Ա
	INT16				m_n16MinFreeIndex;		// �����п���λ�õ���С�±�(== m_n16CurSizeʱ���޿���λ��)
	ContainerRestrict*	m_pRestrict;			// ��Ʒ��������Լ����
	TimeLimitMgr<INT64>	m_TimeLimitMgr;			// ʱ����Ʒ������
};


//-----------------------------------------------------------------------------
// װ������
//-----------------------------------------------------------------------------
class EquipContainer: public Container<tagEquip, INT64>
{
public:
	EquipContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize);
	~EquipContainer();

	VOID Update();

public: 
	// ����װ��
	DWORD Equip(ItemContainer &bagSrc, INT64 n64SerialSrc, EEquipPos ePosDst);
	// ����װ��
	DWORD Unequip(INT64 n64SerialSrc, ItemContainer &bagDst);
	// ����װ��(ָ��������λ��)
	DWORD Unequip(INT64 n64SerialSrc, ItemContainer &bagDst, INT16 n16IndexDst);
	// �ƶ�(����������ָλ)
	DWORD MoveTo(INT64 n64SerialSrc, EEquipPos ePosDst);
	// �����ֽ���
	DWORD SwapWeapon();

	// ʱ�޹���ӿ�
	TList<INT64>& GetNeedDelList();
	VOID ClearNeedDelList();

public:
	// װ��
	DWORD Add(tagEquip *pEquip, EEquipPos ePos);

private:
	// ����
	TimeLimitMgr<INT64>	m_TimeLimitMgr;			// ʱ����Ʒ������
};

//************************ ��ͨ��������ʵ�� ************************//

//----------------------------------------------------------------------
// ����
//----------------------------------------------------------------------
inline VOID ItemContainer::IncreaseSize(INT16 n16Size) 
{ 
	INT16 n16CurSize = GetCurSpaceSize();

	if(n16Size <= 0 || n16Size + n16CurSize > GetMaxSpaceSize())
	{
		ASSERT(n16Size > 0);
		ASSERT(n16Size + n16CurSize <= GetMaxSpaceSize());
		return ;
	}

	if(GetOneFreeSpace() == GT_INVALID)
	{
		m_n16MinFreeIndex = n16CurSize;
	}

	Container<tagItem>::Resize(n16Size + n16CurSize);
}

//----------------------------------------------------------------------
// ��ָ��λ������
//----------------------------------------------------------------------
inline BOOL ItemContainer::Reorder(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num)
{
	if(Container::Reorder(pData, pOutData, n16Num))
	{
		// ��С�����±�������
		m_n16MinFreeIndex = n16Num;
		ResetMinFreeIndex();
		return TRUE;
	}

	return FALSE;
}

inline BOOL ItemContainer::ReorderEx(IN LPVOID pData, OUT LPVOID pOutData, 
									 OUT INT16 &n16OutNum, const INT16 n16Num)
{
	if(Container::ReorderEx(pData, pOutData, n16OutNum, n16Num))
	{
		// ��С�����±�������
		ResetMinFreeIndex();
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------
// ���һ����λ��ʧ�ܷ���GT_INVALID
//----------------------------------------------------------------------
inline INT16 ItemContainer::GetOneFreeSpace()
{
	ResetMinFreeIndex();
	return m_n16MinFreeIndex >= GetCurSpaceSize() ? GT_INVALID : m_n16MinFreeIndex;
}

//----------------------------------------------------------------------
// ���������������Ʒ����ΪdwTypeID����Ʒ����
//----------------------------------------------------------------------
__forceinline INT32 ItemContainer::GetSameItemCount(IN DWORD dwTypeID)
{
	INT32 nNumRet = 0;

	for(INT16 i=0; i<Container::GetCurSpaceSize(); ++i)
	{
		if(!Container::IsOnePlaceFree(i) && (Container::GetItem(i))->dwTypeID == dwTypeID)
		{
			nNumRet += Container::GetItem(i)->n16Num;
		}
	}

	return nNumRet;
}

//----------------------------------------------------------------------
// �����������Ʒ����ΪdwTypeID��lis, ����ʵ�ʻ�ø���t -- ָ��nNumʱ�����ҵ�nNum����Ʒ����
//----------------------------------------------------------------------
__forceinline INT32 ItemContainer::GetSameItemList(OUT TList<tagItem*> &list, 
												   IN DWORD dwTypeID, IN INT32 nNum/* = INT_MAX*/)
{
	INT32 nNumRet = 0;

	for(INT16 i=0; i<Container::GetCurSpaceSize(); ++i)
	{
		if(!Container::IsOnePlaceFree(i) && (Container::GetItem(i))->dwTypeID == dwTypeID)
		{
			nNumRet += Container::GetItem(i)->n16Num;
			list.PushBack(Container::GetItem(i));

			if(nNumRet > nNum)
			{
				break;
			}
		}
	}

	return nNumRet;
}

//----------------------------------------------------------------------
// ��ȡԼ����ָ��
//----------------------------------------------------------------------
inline ContainerRestrict* ItemContainer::GetRestrict() const
{
	return m_pRestrict;
}

//----------------------------------------------------------------------
// ��ȡ����������RoleID
//----------------------------------------------------------------------
inline DWORD ItemContainer::GetOwnerID() const
{
	return m_dwOwnerID;
}

//----------------------------------------------------------------------
// ���������п���λ�õ���С�±�
//----------------------------------------------------------------------
__forceinline VOID ItemContainer::ResetMinFreeIndex()
{
	if(GetFreeSpaceSize() <= 0)
	{
		m_n16MinFreeIndex = GT_INVALID;
		return;
	}

	while(m_n16MinFreeIndex < GetCurSpaceSize())
	{
		if(IsOnePlaceFree(m_n16MinFreeIndex))
		{
			return;
		}

		++m_n16MinFreeIndex;
	}

	// ����д���������������bug��������ѭ��
	if(GetCurSpaceSize() == m_n16MinFreeIndex)
	{
		m_n16MinFreeIndex = 0;
	}

	while(m_n16MinFreeIndex < GetCurSpaceSize())
	{
		if(IsOnePlaceFree(m_n16MinFreeIndex))
		{
			return;
		}

		++m_n16MinFreeIndex;
	}

	// ��Ҫ��ִ�е��˴���˵�������е�m_n16RemainSize������
	ASSERT(0);
}

//----------------------------------------------------------------------
// update
//----------------------------------------------------------------------
inline VOID ItemContainer::Update()
{
	m_TimeLimitMgr.Update();
}

//----------------------------------------------------------------------
// ��ȡ��ɾ���б�
//----------------------------------------------------------------------
inline TList<INT64>& ItemContainer::GetNeedDelList()
{
	return m_TimeLimitMgr.GetNeedDelList();
}

//----------------------------------------------------------------------
// ��մ�ɾ���б�
//----------------------------------------------------------------------
inline VOID ItemContainer::ClearNeedDelList()
{
	m_TimeLimitMgr.ClearNeedDelList();
}


//************************ װ��������ʵ�� ************************//

//----------------------------------------------------------------------
// update
//----------------------------------------------------------------------
inline VOID EquipContainer::Update()
{
	m_TimeLimitMgr.Update();
}

//----------------------------------------------------------------------
// ��ȡ��ɾ���б�
//----------------------------------------------------------------------
inline TList<INT64>& EquipContainer::GetNeedDelList()
{
	return m_TimeLimitMgr.GetNeedDelList();
}

//----------------------------------------------------------------------
// ��մ�ɾ���б�
//----------------------------------------------------------------------
inline VOID EquipContainer::ClearNeedDelList()
{
	m_TimeLimitMgr.ClearNeedDelList();
}