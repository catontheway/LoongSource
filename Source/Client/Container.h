#pragma once
#include "Item.h"

/**	class	\Container
	brief	\������
	remarks	\������Ҫͨ����ֵ��λ�ý���˫������Ķ�����д洢�������
*/
template<typename K, typename V> class Container
{
public:
	Container():m_bLocked(FALSE),m_nMaxSize(SPACE_ONE_BAG){}
	virtual ~Container(void){}
	
	/** \���
	*/
	virtual BOOL Add(V* value)
	{
		// ��������򲻿������
		if(m_mapContainer.Size() >= m_nMaxSize)
			return FALSE;
		
		INT16 pos = GetPos(value);
		K key = GetKey(value);
		m_mapPosition.Add(key, pos);
		m_mapContainer.Add(pos, value);
		
		return TRUE;
	}
	
	/** \ɾ��
	*/
	virtual V* Remove(INT16 pos)
	{
		V* value = m_mapContainer.Peek(pos);
		if(P_VALID(value))
		{
			K key = GetKey(value);
			m_mapPosition.Erase(key);
			m_mapContainer.Erase(pos);
		}
		
		return value;
	}

	/** \����
	*/
	virtual VOID Destroy()
	{
		V* value;
		m_mapContainer.ResetIterator();
		while(m_mapContainer.PeekNext(value))
		{
			if (P_VALID(value))
			{
				SAFE_DEL(value);
			}
		}
		m_mapContainer.Clear();
		m_mapPosition.Clear();
	}
	
	/** \���
	*/
	virtual VOID Clear(TMap<K, V*> &mapContainer)
	{
		V* value;
		m_mapContainer.ResetIterator();
		while(m_mapContainer.PeekNext(value))
		{
			mapContainer.Add(GetKey(value), value);
		}
		m_mapContainer.Clear();
		m_mapPosition.Clear();
	}

	/** \��ȡ
	*/
	virtual V* GetValue(INT16 pos)
	{
		return m_mapContainer.Peek(pos);
	}

	/** \����
	*/
	virtual VOID Lock(){m_bLocked = TRUE;}
	/** \����
	*/
	virtual VOID Unlock(){m_bLocked = FALSE;}

	/** \������С
	*/
	INT Size(){return m_mapContainer.Size();}
	/** \�����ɴ��������
	*/
	INT MaxSize(){return m_nMaxSize;}
	VOID SetMaxSize(INT nMax){m_nMaxSize = nMax;}


	/** \��������
	*/
	BOOL IsFull(INT &nBlank){nBlank = m_nMaxSize - Size(); return nBlank > 0;}

	/** \�Ƿ����
	*/
	BOOL IsExist(INT16 pos)
	{
		return m_mapContainer.IsExist(pos);
	}
	/** \��ȡ��һ����λ
	*/
	INT16 GetBlank()
	{
		for (INT i=0; i<m_nMaxSize; ++i)
		{
			if ( !IsExist(i) )
				return i;
		}
		return GT_INVALID;
	}

protected:
	/** \���ݼ�ֵ��ȡλ��
	*/
	INT16 FindPosition(K key){return m_mapPosition.Peek(key);}
	
	virtual INT16 GetPos(V* value)=0;
	virtual K GetKey(V* value)=0;

protected:
	TMap<INT16, V*>		m_mapContainer;
	TMap<K, INT16>		m_mapPosition;
	INT					m_nMaxSize;
	BOOL				m_bLocked;
};

/** class	\ItemContainer
	brief	\��Ʒ������
	remarks \�洢ʵ����Ʒ
*/
class ItemContainer : public Container<INT64, Item>
{
public:
	ItemContainer();
	~ItemContainer();

	ItemContainer(EItemConType eType, INT nMaxSize, INT nPageSize);

	virtual BOOL Add(Item* value);
	virtual Item* Remove(INT16 pos);

	virtual VOID Lock();
	virtual VOID Unlock(bool bForSettle = false);

	/** \������Ʒ��ģ���Ų�ѯ�����еĸ�����Ʒ����
	*/
	INT GetItemQuantity(DWORD dwTypeID);
	/** \������Ʒ��ģ���Ų�ѯ�����еĸ�����Ʒ����λ��
	*/
	INT16 GetItemPosition(DWORD dwTypeID);
	/** \������Ʒ��ģ���Ų�ѯ�����еĸ�����Ʒ���ٶѵ�id
	*/
	INT64 GetItemSerialID(DWORD dwTypeID);
	/** \��ȡ�����е�һ��ָ�����⹦�����͵���ƷID
	*/
	INT64 GetSpecFuncItemID(EItemSpecFunc eType);
	/** \��ȡ���������⹦�����͵���ƷTypeID�б�
	*/
	void GetSpecFuncItemTypeList(EItemSpecFunc eType, TList<DWORD>& List);
	/** \������ƷID��ȡ��Ʒ
	*/
	Item* GetItem(INT64 n64ItemId){return GetValue(GetPos(n64ItemId));}

	/** \����ҳ����ȡ��Ӧҳ�ĵ�һ����λ
	*/
	INT16 GetBlank(INT nIndex = 0);
	/** \�Զ�����
	*/
	VOID AutoSettle(DWORD dwNPCID = GT_INVALID);

	/** \��Ϥ��������Ʒ������
	*/
	INT32 GetContainerItemQuantity();

	/** \��ôӱ����л�õ�һ���������͵���Ʒ
		\ֻ�ܶԱ���ʹ�ã���Ե���ϵͳ��ӵ�
	*/
	Item* GetSpecFuncItem( EItemConType eItemConType, EItemSpecFunc eSpecFunc );

	/** ͬGetSpecFuncItem��EItemConType��EItemSpecFunc������ר��
	*/
	Item* GetSpecFuncItem(  EItemConType eItemConType, EItemSpecFunc eSpecFunc, DWORD dwItemTypeID );

	/* \ͳ�����������⹦�ܵ���Ʒ
	*/
	void GetSpecFuncMap( EItemSpecFunc eSpecFunc, std::list<Item*>& lst );

protected:
	virtual INT16 GetPos(Item* value){return value->GetPos();}
	virtual INT64 GetKey(Item* value){return value->GetItemId();}

	/** \����ҳ����ȡ��Ӧҳ����ʼλ
	*/
	INT16 GetFirst(INT nIndex = 0);
	/** \����ҳ����ȡ��Ӧҳ����ֹλ
	*/
	INT16 GetLast(INT nIndex = 0);

	
	/** \������ƷID��ȡ��Ʒλ��
	*/
	INT16 GetPos(INT64 n64ItemId){return m_mapPosition.Peek(n64ItemId);}

protected:
	EItemConType	m_eConType;		// ��������
	INT				m_nPageSize;	// ��ҳ��С
	bool			m_bSettle;		// �Զ������־λ
};

/** struct	\tagWareItem
	brief	\������Ʒ
*/
struct tagWareItem
{
	DWORD	dwTypeID;		// ��Ʒ��ģ��ID
	INT16	n16Pos;			// λ��
	INT16	n16Quantity;	// ����
	INT64	n64Price;		// �۸�
	
	tagWareItem(){dwTypeID = n16Quantity = GT_INVALID; n64Price = n16Pos = 0;}
	tagWareItem(DWORD dwID){dwTypeID = dwID; n16Quantity = GT_INVALID; n64Price = n16Pos = 0;}
};

/** class	\WareContainer
	brief	\��Ʒ������
	remarks \�洢��Ʒ
*/
class WareContainer : public Container<DWORD, tagWareItem>
{
public:
	WareContainer(){}
	~WareContainer(){}

protected:
	virtual INT16 GetPos(tagWareItem* value){return value->n16Pos;}
	virtual DWORD GetKey(tagWareItem* value){return value->dwTypeID;}
};

/** struct	\tagTemporaryItem
	brief	\��ʱ��Ʒ
	remarks \������ʾ���ס��ʼġ���̯�ȹ���ʱ�Լ����������Ʒ
*/
struct tagTemporaryItem
{
	INT64	n64ItemId;		// ��ƷID
	INT16	n16Pos;			// λ��
	bool	bShelf;			// �Ƿ��ϼ�(��̯���ã�������Ϊ����λ)
	INT8	n8Dummy;		// ����λ
	INT64	n64Price;		// ��Ʒ����(��̯����)
	Item*	pItem;			// ��Ӧ�����е���Ʒָ��

	tagTemporaryItem(){n16Pos = 0; pItem = NULL;}
	tagTemporaryItem(Item* pt, INT16 n16Index)
	{
		n64ItemId = pt->GetItemId();
		n16Pos = n16Index;
		pItem = pt;
		bShelf = false;
		n64Price = 0;
	}
};

/** class	\TemporaryContainer
	brief	\��ʱ������
	remarks	\�����洢��ʱ��Ʒ
*/
class TemporaryContainer : public Container<INT64, tagTemporaryItem>
{
public:
	TemporaryContainer(INT nMaxSize){m_nMaxSize = nMaxSize;}
	~TemporaryContainer(){}

	tagTemporaryItem* RemoveById(INT64 n64Serial){return Remove(FindPosition(n64Serial));}

protected:
	virtual INT16 GetPos(tagTemporaryItem* value){return value->n16Pos;}
	virtual INT64 GetKey(tagTemporaryItem* value){return value->n64ItemId;}

	
};


