//-----------------------------------------------------------------------------
// File: safe_unit_queue.h
// Desc: �ṩ�̰߳�ȫ���Ƚ��ȳ�unit���й���
// Auth: Lyp
// Date: 2003-11-30
// Last: 2005-11-01
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

//-----------------------------------------------------------------------------
// SafeQueue: �ṩ�̰߳�ȫ���Ƚ��ȳ�ָ����й���
//-----------------------------------------------------------------------------
template<typename ITEM_POINTER>
class SafeQueue
{
public:
	// ��ӵ�����β
	__forceinline BOOL Add(ITEM_POINTER pItem)
	{
		EnterCriticalSection(&m_Lock);
		if( 0 == m_lItemNum )
			m_pFirst = pItem;
		else
			m_pLast->pNext = pItem;
		m_pLast = pItem;
		++m_lItemNum;
		LeaveCriticalSection(&m_Lock);
		if( m_bSetEvent )	// ����event
			::SetEvent(m_hEvent);
		return TRUE;
	}

	// �Ӷ���ͷȡitem������踺���ͷ�
	__forceinline ITEM_POINTER Get()
	{
		if( m_lItemNum <= 0 )	// �Ȳ����ٽ�critical section
			return NULL;
		EnterCriticalSection(&m_Lock);
		if( m_lItemNum <= 0 )
		{
			LeaveCriticalSection(&m_Lock);
			return NULL;
		}
		ITEM_POINTER pItem = m_pFirst;	// ȡ��ITEM_POINTER
		m_pFirst = m_pFirst->pNext;
		--m_lItemNum;
		LeaveCriticalSection(&m_Lock);
		return pItem;
	}

	// ���Խ�������������ӵ�����β
	__forceinline BOOL TryAdd(ITEM_POINTER pItem)
	{
#if _WIN32_WINNT < 0x0400
		EnterCriticalSection(&m_Lock);
		if( 0 == m_lItemNum )
			m_pFirst = pItem;
		else
			m_pLast->pNext = pItem;
		m_pLast = pItem;
		++m_lItemNum;
		LeaveCriticalSection(&m_Lock);
		if( m_bSetEvent )	// ����event
			::SetEvent(m_hEvent);
		return TRUE;
#else
		if( TryEnterCriticalSection(&m_Lock) )
		{
			if( 0 == m_lItemNum )
				m_pFirst = pItem;
			else
				m_pLast->pNext = pItem;
			m_pLast = pItem;
			++m_lItemNum;
			LeaveCriticalSection(&m_Lock);
			if( m_bSetEvent )	// ����event
				::SetEvent(m_hEvent);
			return TRUE;
		}
		return FALSE;
#endif
	}

	// ���Խ������������Ӷ���ͷȡ
	__forceinline ITEM_POINTER TryGet()
	{
		if( m_lItemNum <= 0 )
			return NULL;
#if _WIN32_WINNT < 0x0400
		EnterCriticalSection(&m_Lock);
		if( m_lItemNum <= 0 )
		{
			LeaveCriticalSection(&m_Lock);
			return NULL;
		}
		ITEM_POINTER pItem = m_pFirst;	// ȡ��ITEM_POINTER
		m_pFirst = m_pFirst->pNext;
		--m_lItemNum;
		LeaveCriticalSection(&m_Lock);
		return pItem;
#else
		if( TryEnterCriticalSection(&m_Lock) )
		{
			if( m_lItemNum <= 0 )
			{
				LeaveCriticalSection(&m_Lock);
				return NULL;
			}
			ITEM_POINTER pItem = m_pFirst;	// ȡ��ITEM_POINTER
			m_pFirst = m_pFirst->pNext;
			--m_lItemNum;
			LeaveCriticalSection(&m_Lock);
			return pItem;
		}
		return NULL;
#endif
	}

	// �õ������е�items��Ŀ
	__forceinline LONG GetNum() { return m_lItemNum; }
	// ȡ���¼����������м�����Ϣʱ�����¼�������
	__forceinline HANDLE GetEvent() { return m_hEvent; }

	SafeQueue(BOOL bSetEvent=FALSE):m_lItemNum(0),m_pFirst(0), m_pLast(0)
	{
		InitializeCriticalSection(&m_Lock);
		m_bSetEvent = bSetEvent;
		if( m_bSetEvent )
			m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// �Զ�����
	}
	~SafeQueue()
	{
		// ȷ�ϻ�����û��unit
		ASSERT( 0 == m_lItemNum );
		DeleteCriticalSection(&m_Lock);
		if( m_bSetEvent )
			CloseHandle(m_hEvent);
	}

private:
	CRITICAL_SECTION	m_Lock;
	HANDLE				m_hEvent;

	ITEM_POINTER		m_pFirst;
	ITEM_POINTER		m_pLast;

	BOOL				m_bSetEvent;
	VOLATILE LONG		m_lItemNum;
	
};

}
