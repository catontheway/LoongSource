//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Container.h
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: ��Ʒ������ -- ģ�����
//------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/container_define.h"
#include "../ServerDefine/base_define.h"

#include "world.h"

//****************** ������ �ò��ֲ������޸���Ʒ�������� *********************
template<class T, class MapKey = INT64>
class Container
{
public:
	Container();
	Container(EItemConType eConType, INT16 n16CurSize, INT16 n16MaxSize);
	virtual ~Container();

	VOID Init(EItemConType eConType, INT16 n16CurSize, INT16 n16MaxSize);

private:
	VOID Destroy();	// �������������Զ�����

private:
	Container(const Container&);
	Container& operator = (const Container&);

public:
	// ��������
	EItemConType GetConType()	const;

	// �õ������п��пռ�Ĵ�С
	INT16 GetFreeSpaceSize()	const;

	// �õ������ܿռ��С
	INT16 GetMaxSpaceSize()		const;

	// �õ�������ǰ�ռ��С
	INT16 GetCurSpaceSize()		const;

	// �ж�ָ��λ���Ƿ�Ϊ��
	BOOL IsOnePlaceFree(INT16 n16Index) const;

	// ����key�õ���Ʒָ��,����NULL��ʾָ����Ʒ���ڸ�������
	T* GetItem(MapKey key);

	// ����index�õ���Ʒָ��,����NULL��ʾָ����Ʒ���ڸ�������
	T* GetItem(INT16 n16Index);

	// ��ȡ��ʼ������
	typename TMap<MapKey, INT16>::TMapIterator Begin()	{ return m_mapItem.Begin(); }

	// ���ݵ�����������������Ʒ
	BOOL GetNextItem(typename TMap<MapKey, INT16>::TMapIterator& iter, T*& pItem);

	BOOL GetRandom(MapKey &key);

public:
	// ����Ʒ����ָ��λ��,��������ӳɹ�����(Ŀ��λ��������Ʒ,���ᱻ����)
	INT16 Add(T* pItem, INT16 n16Index);

	// �������������Ʒ�����ر�ɾ����Ʒ��ָ��(NULL��ʾָ����Ʒ������������)
	T* Remove(MapKey key);	// �ýӿڵ��÷�ʽΪContainer::Remove()
	T* Remove(INT16 n16Index);

protected:
	// ����Ʒλ�ý���,������Ʒ1����(ֱ�ӽ����������κ���֤)
	INT16 Swap(INT16 n16Index1, INT16 n16Index2);

	// �����������е�λ��,GT_INVALID��ʾ����������
	INT16 GetIndex(MapKey key);
	INT16 GetIndex(const T* pItem);

	// ��������������С
	VOID Resize(INT16 n16Size);

	// ��ָ��λ������
	BOOL Reorder(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num);
	BOOL ReorderEx(IN LPVOID pData, OUT LPVOID pOutData, OUT INT16& n16OutNum, const INT16 n16Num);

private:
	T**					m_ppItem;			// ����(�������д洢������Ʒ�ṹ��ָ��)
	TMap<MapKey, INT16>	m_mapItem;			// <64ΪID��������λ��>
	EItemConType		m_eContainerType;	// ��������
	INT16				m_n16MaxSize;		// �����ռ����ֵ
	INT16				m_n16CurSize;		// ������ǰ�ռ��С
	INT16				m_n16RemainSize;	// δʹ�õ�������С
};

//******************** ʵ�� �ò��ֲ������޸���Ʒ�������� *********************

//------------------------------------------------------------------------------
// ���캯��
//------------------------------------------------------------------------------
template<class T, class MapKey>
Container<T, MapKey>::Container()
{
	m_ppItem = NULL;

	m_mapItem.Clear();
	m_n16CurSize		= 0;
	m_n16RemainSize		= 0;
	m_n16MaxSize		= 0;
	m_eContainerType	= EICT_Null;
}

template<class T, class MapKey>
Container<T, MapKey>::Container(EItemConType eConType, INT16 n16CurSize, INT16 n16MaxSize)
{
	Init(eConType, n16CurSize, n16MaxSize);	
}

//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
template<class T, class MapKey>
Container<T, MapKey>::~Container()
{
	Destroy();
}

//------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------
template<class T, class MapKey>
VOID Container<T, MapKey>::Init(EItemConType eConType, INT16 n16CurSize, INT16 n16MaxSize)
{
	m_ppItem = new T*[n16CurSize];
	ZeroMemory(m_ppItem, n16CurSize * sizeof(T*));

	m_mapItem.Clear();
	m_n16CurSize		= n16CurSize;
	m_n16RemainSize		= n16CurSize;
	m_n16MaxSize		= n16MaxSize;
	m_eContainerType	= eConType;
}

//------------------------------------------------------------------------------
// ����(������ʾ����)
//------------------------------------------------------------------------------
template<class T, class MapKey>
VOID Container<T, MapKey>::Destroy()
{
	// ɾ����Ʒ
	for(INT32 i=0; i<m_n16CurSize; ++i)
	{
		::Destroy(m_ppItem[i]);
	}

	SAFE_DEL_ARRAY(m_ppItem);

	m_mapItem.Clear();
	m_n16CurSize	= 0;
	m_n16RemainSize = 0;
	m_n16MaxSize	= 0;
}

//------------------------------------------------------------------------------
// ����Ʒ����ָ��λ��,��������ӳɹ�����(Ŀ��λ��������Ʒ,���ᱻ����)
//------------------------------------------------------------------------------
template<class T, class MapKey>
INT16 Container<T, MapKey>::Add(T* pItem, INT16 n16Index)
{
	// �˴����ж�,��Ϊ���ⲿ���÷���(����Ч��: ���ܻ����˶���ж�)
	//if(n16Index < 0 || n16Index >= m_n16CurSize)
	//{
	//	return 0;
	//}

	ASSERT(IsOnePlaceFree(n16Index));

	m_ppItem[n16Index] = pItem;
	m_mapItem.Add(pItem->GetKey(), n16Index);

	// �����п��ÿռ����1
	--m_n16RemainSize;

	// ������Ʒ����λ��<��������,�±�>���������������Ĳ���(���ÿ�����Ʒ�м�¼��λ����Ϣ�仯)
	pItem->SetPos(m_eContainerType, n16Index);

	// ���ø���������Ϣλ
	pItem->SetUpdate(EUDBS_Update);

	return pItem->GetNum();
}

//------------------------------------------------------------------------------
// ����64λid�������������Ʒ�����ر�ɾ����Ʒ��ָ��
//------------------------------------------------------------------------------
template<class T, class MapKey>
T* Container<T, MapKey>::Remove(MapKey key)
{
	INT16 n16Index = m_mapItem.Peek(key);
	if(!GT_VALID(n16Index))
	{
		return NULL;
	}

	// ��map�����
	m_mapItem.Erase(key);

	// ������ָ�����
	T* pRetItem = m_ppItem[n16Index];
	if(NULL == pRetItem)
	{
		ASSERT(pRetItem != NULL);
		return NULL;
	}

	m_ppItem[n16Index] = NULL;

	// �����п��Կռ�����1
	++m_n16RemainSize;

	pRetItem->SetPos(EICT_Ground, GT_INVALID);

	return pRetItem;
}

//------------------------------------------------------------------------------
// ����16λindex�������������Ʒ�����ر�ɾ����Ʒ��ָ��
//------------------------------------------------------------------------------
template<class T, class MapKey>
T* Container<T, MapKey>::Remove(INT16 n16Index)
{
	// �˴����ж�,��Ϊ���ⲿ���÷���(����Ч��: ���ܻ����˶���ж�)
	if(n16Index < 0 || n16Index >= m_n16CurSize)
	{
		return NULL;
	}

	// ������ָ�����
	T* pRetItem = m_ppItem[n16Index];
	if(NULL == pRetItem)
	{
		ASSERT(pRetItem != NULL);
		return NULL;
	}

	m_ppItem[n16Index] = NULL;

	// ��map�����
	m_mapItem.Erase(pRetItem->GetKey());

	// �����п��Կռ�����1
	++m_n16RemainSize;

	pRetItem->SetPos(EICT_Ground, GT_INVALID);

	return pRetItem;
}

//-----------------------------------------------------------------------------
// ��ָ��λ������
//-----------------------------------------------------------------------------
template<class T, class MapKey>
BOOL Container<T, MapKey>::Reorder(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num)
{
	// ����⣺�����Ƿ��������������
	if(n16Num != GetCurSpaceSize() - GetFreeSpaceSize())
	{
		return FALSE;
	}
	
	MTRANS_POINTER(pSrcPos, pData, INT16);
	MTRANS_POINTER(pOut, pOutData, INT16);

	// ����������Ʒ���ݱ���
	T** ppBak = new T*[GetCurSpaceSize()];
	memcpy(ppBak, m_ppItem, sizeof(T*) * GetCurSpaceSize());

	// ���������������Ʒ����
	m_mapItem.Clear();
	ZeroMemory(m_ppItem, sizeof(T*) * GetCurSpaceSize());
	m_n16RemainSize = GetCurSpaceSize();

	// �������µ�˳����Ʒ��������
	INT16 n16CurIndex = 0;
	for(INT16 i=0; i<n16Num; ++i)
	{
		// ��Ϣ�����ݲ���ȷ����ָ��λ��û����Ʒ
		if(pSrcPos[i] < 0 || pSrcPos[i] > GetCurSpaceSize()
			|| !P_VALID(ppBak[pSrcPos[i]]))
		{
			continue;
		}
		
		// ����������
		Add(ppBak[pSrcPos[i]], n16CurIndex);

		// ����
		pOut[n16CurIndex] = pSrcPos[i];
		++n16CurIndex;
		ppBak[pSrcPos[i]] = NULL;
	}

	// ����Ƿ�������Ʒ���ѷŵ�������
	if(n16CurIndex != n16Num)
	{
		// ִ�е��ˣ�˵����Ϣ���д������ݣ������⴦��
		for(INT16 i=0; i<GetCurSpaceSize(); ++i)
		{
			if(!P_VALID(ppBak[i]))
			{
				continue;
			}

			// ����������
			Add(ppBak[i], n16CurIndex);

			// ����
			pOut[n16CurIndex] = i;
			++n16CurIndex;
			ppBak[i] = NULL;
		}

		ASSERT(n16Num == n16CurIndex);
	}

	// �ͷ���ʱ�ڴ�
	SAFE_DEL_ARRAY(ppBak);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ָ��λ������ -- ���ƶ���Ʒ�����ݽṹ�����溯����ͬ
//-----------------------------------------------------------------------------
template<class T, class MapKey>
BOOL Container<T, MapKey>::ReorderEx(IN LPVOID pData, OUT LPVOID pOutData, 
									 OUT INT16& n16OutNum, const INT16 n16Num)
{
	n16OutNum = 0;
	
	// ����⣺�����Ƿ��������������
	if(n16Num > GetCurSpaceSize() - GetFreeSpaceSize())
	{
		return FALSE;
	}

	MTRANS_POINTER(pSrcPos, pData, tagItemOrder);
	MTRANS_POINTER(pOut, pOutData, tagItemOrder);

	// �����������ƶ�����Ʒ���ݱ���
	T** ppBak = new T*[n16Num];
	ZeroMemory(ppBak, sizeof(T*) * n16Num);

	for(INT16 i=0; i<n16Num; ++i)
	{
		// ��Ϣ�����ݲ���ȷ����ָ��λ��û����Ʒ
		if(pSrcPos[i].n16OldIndex < 0 || pSrcPos[i].n16OldIndex > GetCurSpaceSize()
			|| pSrcPos[i].n16NewIndex < 0 || pSrcPos[i].n16NewIndex > GetCurSpaceSize()
			|| !P_VALID(GetItem(pSrcPos[i].n16OldIndex)))
		{
			continue;
		}

		// ��������ȡ��,�����뱸��������
		ppBak[i] = Remove(pSrcPos[i].n16OldIndex);
	}

	for(INT16 i=0; i<n16Num; ++i)
	{
		// ����Ƿ�����Ʒ
		if(!P_VALID(ppBak[i]))
		{
			continue;
		}

		// ���Ŀ��λ���Ƿ�Ϊ��
		if(!IsOnePlaceFree(pSrcPos[i].n16NewIndex))
		{
			continue;
		}

		// ����������
		Add(ppBak[i], pSrcPos[i].n16NewIndex);

		// ����
		pOut[n16OutNum++] = pSrcPos[i];
		ppBak[i] = NULL;
	}

	// ����Ƿ�������Ʒ���ѷŵ�������
	if(n16OutNum != n16Num)
	{
		// ִ�е��ˣ�˵����Ϣ���д������� -- ����޸���Ϣ��ɣ������������ݿ�����Ʒ������ʧ
	}

	// �ͷ���ʱ�ڴ�
	SAFE_DEL_ARRAY(ppBak);

	return TRUE;
}


//*************************** �������� ****************************************

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline EItemConType Container<T, MapKey>::GetConType() const
{
	return m_eContainerType;
}

//-----------------------------------------------------------------------------
// �õ������п��пռ�Ĵ�С
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline INT16 Container<T, MapKey>::GetFreeSpaceSize() const
{
	return m_n16RemainSize;
}

//-----------------------------------------------------------------------------
// �õ������ܿռ��С
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline INT16 Container<T, MapKey>::GetMaxSpaceSize() const
{
	return m_n16MaxSize;
}

//-----------------------------------------------------------------------------
// �õ�������ǰ�ռ��С
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline INT16 Container<T, MapKey>::GetCurSpaceSize() const
{
	return m_n16CurSize;
}

//-----------------------------------------------------------------------------
// �ж�ָ��λ���Ƿ�Ϊ��
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline BOOL Container<T, MapKey>::IsOnePlaceFree(INT16 n16Index) const
{
	if(n16Index < 0 || n16Index >= m_n16CurSize)
	{
		ASSERT(0);
		return FALSE;
	}
	
	return NULL == m_ppItem[n16Index];
}

//-----------------------------------------------------------------------------
// ����key�õ���Ʒָ��,����NULL��ʾָ����Ʒ���ڸ�������
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline T* Container<T, MapKey>::GetItem(MapKey key)
{
	INT16 n16Index = m_mapItem.Peek(key);
	if(!GT_VALID(n16Index))
	{
		return NULL;
	}

	return m_ppItem[n16Index];
}

//-----------------------------------------------------------------------------
// ����index�õ���Ʒָ��,����NULL��ʾָ����Ʒ���ڸ�������
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline T* Container<T, MapKey>::GetItem(INT16 n16Index) 
{
	if(n16Index>=0 && n16Index<m_n16CurSize) 
	{
		return m_ppItem[n16Index];
	}
	return NULL; 
}

//-----------------------------------------------------------------------------
// ���ݵ�������������������
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline BOOL Container<T, MapKey>::GetNextItem(typename TMap<MapKey, INT16>::TMapIterator& iter, T*& pItem)
{
	INT64 n64Serial	= GT_INVALID;
	INT16 n16Index	= GT_INVALID;
	if (m_mapItem.PeekNext(iter, n64Serial, n16Index) && GT_VALID(n16Index))
	{
		pItem = m_ppItem[n16Index];
		return TRUE;
	}

	pItem = NULL;

	return FALSE;
}

//-----------------------------------------------------------------------------
// �����ȡ
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline BOOL Container<T, MapKey>::GetRandom(MapKey &key)
{
	INT16 n16Index;
	return m_mapItem.RandPeek(key, n16Index);
}

//-----------------------------------------------------------------------------
// ����Ʒλ�ý���,������Ʒ1����(ֱ�ӽ����������κ���֤)
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline INT16 Container<T, MapKey>::Swap(INT16 n16Index1, INT16 n16Index2)
{
	// ɾ��
	T *pItem1 = Remove(n16Index1);
	T *pItem2 = Remove(n16Index2);

	// ���
	Add(pItem1, n16Index2);
	Add(pItem2, n16Index1);

	// �������ݿⱣ��״̬
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	return pItem1->GetNum();
}

//-----------------------------------------------------------------------------
// ������Ʒָ��洢�������е�λ��,GT_INVALID��ʾ����������
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline INT16 Container<T, MapKey>::GetIndex(MapKey key)
{
	return m_mapItem.Peek(key);
}

template<class T, class MapKey>
inline INT16 Container<T, MapKey>::GetIndex(const T* pItem)
{
	return m_mapItem.Peek(pItem->GetKey());
}

//-----------------------------------------------------------------------------
// ��������������С
//-----------------------------------------------------------------------------
template<class T, class MapKey>
inline VOID Container<T, MapKey>::Resize(INT16 n16Size) 
{ 
	ASSERT(n16Size > m_n16CurSize);
	ASSERT(n16Size <= m_n16MaxSize);

	// ����ռ䣬����������
	T **ppNew = new T*[n16Size];
	ZeroMemory(ppNew, n16Size * sizeof(T*));
	IFASTCODE->MemCpy(ppNew, m_ppItem, m_n16CurSize * sizeof(T*));

	// ������������
	SAFE_DEL_ARRAY(m_ppItem);
	m_ppItem = ppNew;
	m_n16RemainSize += n16Size - m_n16CurSize;
	m_n16CurSize = n16Size;
}