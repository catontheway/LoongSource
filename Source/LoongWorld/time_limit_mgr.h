//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: time_limit_mgr.h
// author: Sxg
// actor:
// data: 2009-04-28
// last:
// brief: ʱ���������������
//-----------------------------------------------------------------------------
#pragma once

//******************** �ඨ�� *****************************//
template<typename KeyType>
class TimeLimitMgr
{
public:
	TimeLimitMgr(DWORD dwUpdateTicks);
	~TimeLimitMgr();

	VOID Update();

	VOID Add2TimeLeftList(KeyType key, DWORD dwTimeLimit, tagDWORDTime dwSrcTime);
	VOID RemoveFromTimeLeftList(KeyType key);

	TList<KeyType>& GetNeedDelList();
	VOID ClearNeedDelList();

private:
	// ��ʱ����(��λ����)
	VOID UpdateTimeLeftList(DWORD dwTimePass);

private:
	// ʱ����Ʒͳ�ƽṹ
	struct tagTimeLeft
	{
		KeyType	key;		
		DWORD	dwTimeLeft;		// ʣ�����ʱ��(��λ����)

		tagTimeLeft(KeyType	key, DWORD dwTimeLeft)
		{
			this->key			= key;
			this->dwTimeLeft	= dwTimeLeft;
		}
	};

	// ���list
	TList<tagTimeLeft*>		m_LstTimeLeft;
	TList<KeyType>			m_LstNeedDel;

	// ����
	DWORD					m_dwUpdateTicks;
	INT32					m_nTickCountDown;
	tagDWORDTime			m_dwLastCalTime;
};


//******************** ��ʵ�� *****************************//
template<typename KeyType>
TimeLimitMgr<KeyType>::TimeLimitMgr(DWORD dwUpdateTicks)
{
	m_dwUpdateTicks		= dwUpdateTicks;
	m_nTickCountDown	= dwUpdateTicks;
	m_dwLastCalTime		= g_world.GetWorldTime();
}

template<typename KeyType>
TimeLimitMgr<KeyType>::~TimeLimitMgr()
{
	if(m_LstTimeLeft.Size() > 0)
	{
		tagTimeLeft *pTimeLeft = NULL;
		TList<tagTimeLeft*>::TListIterator iter = m_LstTimeLeft.Begin();
		while(m_LstTimeLeft.PeekNext(iter, pTimeLeft))
		{
			// ɾ���ýڵ�
			m_LstTimeLeft.Erase(pTimeLeft);
			SAFE_DEL(pTimeLeft);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// update
//-------------------------------------------------------------------------------------------------------
template<typename KeyType>
inline VOID TimeLimitMgr<KeyType>::Update()
{
	--m_nTickCountDown;
	
	if(0 == m_nTickCountDown)
	{
		if(m_LstTimeLeft.Size() > 0)
		{
			UpdateTimeLeftList(m_dwUpdateTicks / TICK_PER_SECOND);
		}
		
		m_nTickCountDown = m_dwUpdateTicks;
	}
}

//-------------------------------------------------------------------------------------------------------
// ʱ����Ʒͳ���б����
//-------------------------------------------------------------------------------------------------------
template<typename KeyType>
VOID TimeLimitMgr<KeyType>::Add2TimeLeftList(KeyType key, DWORD dwTimeLimit, tagDWORDTime dwSrcTime)
{
	ASSERT(dwSrcTime != GT_INVALID);

	DWORD dwTimeLeft = CalcTimeDiff(m_dwLastCalTime, dwSrcTime);
	if(dwTimeLeft >= dwTimeLimit)
	{
		// �ŵ���ɾ���б�
		if(!m_LstNeedDel.IsExist(key))
		{
			m_LstNeedDel.PushBack(key);
		}

		return;
	}

	tagTimeLeft *pTimeLeft = new tagTimeLeft(key, dwTimeLimit - dwTimeLeft);
	ASSERT(P_VALID(pTimeLeft));

	m_LstTimeLeft.PushBack(pTimeLeft);
}

//-------------------------------------------------------------------------------------------------------
// ʱ����Ʒͳ���б����
//-------------------------------------------------------------------------------------------------------
template<typename KeyType>
VOID TimeLimitMgr<KeyType>::RemoveFromTimeLeftList(KeyType key)
{
	ASSERT(m_LstTimeLeft.Size() > 0);

	BOOL bCheck = FALSE;
	tagTimeLeft *pTimeLeft = NULL;
	TList<tagTimeLeft*>::TListIterator iter = m_LstTimeLeft.Begin();
	while(m_LstTimeLeft.PeekNext(iter, pTimeLeft))
	{
		if(pTimeLeft->key == key)
		{
			m_LstTimeLeft.Erase(pTimeLeft);
			SAFE_DEL(pTimeLeft);
			bCheck = TRUE;
			break;
		}
	}

	ASSERT(bCheck);
}

//-------------------------------------------------------------------------------------------------------
// ʱ����Ʒͳ���б����(��λ����)
//-------------------------------------------------------------------------------------------------------
template<typename KeyType>
VOID TimeLimitMgr<KeyType>::UpdateTimeLeftList(DWORD dwTimePass)
{
	tagTimeLeft *pTimeLeft = NULL;
	TList<tagTimeLeft*>::TListIterator iter = m_LstTimeLeft.Begin();
	while(m_LstTimeLeft.PeekNext(iter, pTimeLeft))
	{
		if(pTimeLeft->dwTimeLeft <= dwTimePass)
		{
			if(!m_LstNeedDel.IsExist(pTimeLeft->key))
			{
				m_LstNeedDel.PushBack(pTimeLeft->key);
			}

			// ɾ���ýڵ�
			m_LstTimeLeft.Erase(pTimeLeft);
			SAFE_DEL(pTimeLeft);

			continue;
		}

		pTimeLeft->dwTimeLeft -= dwTimePass;
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��ɾ���б�
//-------------------------------------------------------------------------------------------------------
template<typename KeyType>
inline TList<KeyType>& TimeLimitMgr<KeyType>::GetNeedDelList()
{
	return m_LstNeedDel;
}

//-------------------------------------------------------------------------------------------------------
// ��մ�ɾ���б�
//-------------------------------------------------------------------------------------------------------
template<typename KeyType>
inline VOID TimeLimitMgr<KeyType>::ClearNeedDelList()
{
	m_LstNeedDel.Clear();
}