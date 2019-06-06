//-----------------------------------------------------------------------------
// File: x_queue.h
// Desc: �ṩ�̰߳�ȫ���Ƚ��ȳ�unit���й���
// Auth: Lyp
// Date: 2003-11-30
// Last: 2009-06-11
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {
//-----------------------------------------------------------------------------
// XQueue: �ṩ�̰߳�ȫ���Ƚ��ȳ�ָ����й���
//-----------------------------------------------------------------------------
template<typename ITEM_POINTER> class XQueue
{
public:
	__forceinline BOOL Add(ITEM_POINTER pItem)	// ��ӵ�����β
	{
		Lock();
		if( 0 == m_lItemNum )
			m_pFirst = pItem;
		else
			m_pLast->pNext = pItem;
		m_pLast = pItem;
		++m_lItemNum;
		Unlock();
		return TRUE;
	}

	__forceinline ITEM_POINTER Get()	// �Ӷ���ͷȡitem������踺���ͷ�
	{
		if( m_lItemNum <= 0 )	// �Ȳ����ٽ�critical section
			return NULL;
		Lock();
		if( m_lItemNum <= 0 )
		{
			Unlock();
			return NULL;
		}
		ITEM_POINTER pItem = m_pFirst;	// ȡ��ITEM_POINTER
		m_pFirst = m_pFirst->pNext;
		--m_lItemNum;
		Unlock();
		return pItem;
	}

	__forceinline BOOL AddFront(ITEM_POINTER pItem)	// ��ӵ�������
	{
		Lock();
		if( 0 == m_lItemNum )
			m_pLast = pItem;
		else
			pItem->pNext = m_pFirst;
		m_pFirst = pItem;
		++m_lItemNum;
		Unlock();
		return TRUE;
	}
	
	__forceinline LONG GetNum() { return m_lItemNum; }	// �õ������е�items��Ŀ
	__forceinline XQueue():m_lItemNum(0),m_pFirst(0), m_pLast(0), m_lLock(0) {}
	__forceinline ~XQueue() { }//ASSERT( 0 == m_lItemNum ); }

private:
	LONG VOLATILE		m_lLock;
	LONG VOLATILE		m_lItemNum;
	ITEM_POINTER		m_pFirst;
	ITEM_POINTER		m_pLast;

	__forceinline VOID Lock() { while(InterlockedCompareExchange((LPLONG)&m_lLock, 1, 0)!= 0) Sleep(0); }
	__forceinline VOID Unlock() { InterlockedExchange((LPLONG)(&m_lLock), 0); }
};
}
