//-----------------------------------------------------------------------------
// File: msg_queue
// Desc: �ṩ�򵥵��Ƚ��ȳ���Ϣ���й���
// Auth: Lyp
// Date: 2003-11-30
// Last: 2008-06-12
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

//-----------------------------------------------------------------------------
// MsgQueue: �ṩ�򵥵��Ƚ��ȳ���Ϣ���й���
// ��ȡ��Ϣ��ʱ��ʹ���ڲ�������ڴ�,����踺���ͷ��ڴ�
//-----------------------------------------------------------------------------
class VENGINE_API MsgQueue
{
public:
	// �����Ϣ������β�������Ƿ���ӳɹ�
	__forceinline BOOL AddMsg(LPCVOID pMsg, DWORD dwSize);
	// �Ӷ���ͷȡ��Ϣ������踺���ͷ��ڴ�
	__forceinline LPBYTE GetMsg(DWORD& dwMsgSize);
	// ��Ӧ��GetMsg(),��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
	__forceinline VOID FreeMsg(LPVOID pMsg);

	//-----------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------
	// ���ڲ�ָ��ָ�����ͷ
	__forceinline VOID ResetIterator();
	// ���ڲ�ָ�����
	__forceinline VOID IncIterator();
	// ��õ�ǰ�ڲ�ָ��ָ����Ϣ��������ȡ��
	__forceinline LPBYTE PeekMsg(DWORD& dwMsgSize);
	// �Ƴ���ǰ�ڲ�ָ��ָ����Ϣ��������ָ��
	__forceinline VOID RemoveMsg();
	// ���������Ϣ
	__forceinline VOID Clear();

	// �õ������е���Ϣ��Ŀ
	__forceinline INT GetMsgNum() { return m_nMsg; }
	
	MsgQueue();
	~MsgQueue();

private:
	struct MsgItem
	{
		DWORD			dwDataSize;
		BYTE*			pData;
		MsgItem*		pPrev;
		MsgItem*		pNext;
	};

	INT					m_nMsg;
	MsgItem*			m_pQueueBegin;
	MsgItem*			m_pQueueEnd;
	MsgItem*			m_pCurrentItem;

	MemPool*			m_pMemPool;
};

//-----------------------------------------------------------------------------
// �����Ϣ������β�������Ƿ���ӳɹ�
//-----------------------------------------------------------------------------
BOOL MsgQueue::AddMsg(LPCVOID pMsg, DWORD dwSize)
{
	// �����µĵ�Ԫ
	MsgItem* pNewMsg = (MsgItem*)m_pMemPool->Alloc(sizeof(MsgItem));
	if( pNewMsg == NULL )
		return FALSE;

	// ��ʼ��
	pNewMsg->dwDataSize = dwSize;
	pNewMsg->pData = NULL;
	pNewMsg->pPrev = NULL;
	pNewMsg->pNext = NULL;

	// �������ݿռ�
	pNewMsg->pData = (LPBYTE)m_pMemPool->Alloc(dwSize);
	if( pNewMsg->pData == NULL )
	{
		delete(pNewMsg);
		return FALSE;
	}

	// ��������
	memcpy(pNewMsg->pData, pMsg, dwSize);

	if( m_pQueueBegin == NULL )	// ���п�
	{
		m_pQueueBegin = pNewMsg;
		m_pQueueEnd = pNewMsg;
		m_pCurrentItem = pNewMsg;
	}
	else
	{
		m_pQueueEnd->pNext = pNewMsg;
		pNewMsg->pPrev = m_pQueueEnd;
		m_pQueueEnd = pNewMsg;

	}

	m_nMsg++;
	return TRUE;
}



//-----------------------------------------------------------------------------
// �Ӷ���ͷȡ��Ϣ��������Ϣ���ȣ�����踺���ͷ��ڴ�
//-----------------------------------------------------------------------------
LPBYTE MsgQueue::GetMsg(DWORD &dwMsgSize)
{
	dwMsgSize = 0;
	if( m_nMsg <= 0 )
		return NULL;	// no msg

	// ȡ����Ϣ
	LPBYTE pMsg = m_pQueueBegin->pData;
	dwMsgSize = m_pQueueBegin->dwDataSize;

	MsgItem* pItem = m_pQueueBegin;
	m_pQueueBegin = m_pQueueBegin->pNext;
	if( m_pQueueBegin )
		m_pQueueBegin->pPrev = NULL;
	m_pMemPool->Free(pItem);

	m_nMsg--;
	return pMsg;
}


//-----------------------------------------------------------------------------
// ��Ӧ��GetMsg(),��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
//-----------------------------------------------------------------------------
VOID MsgQueue::FreeMsg(LPVOID pMsg) 
{
	m_pMemPool->Free(pMsg);
}


//-----------------------------------------------------------------------------
// ���ڲ�ָ��ָ�����ͷ
//-----------------------------------------------------------------------------
VOID MsgQueue::ResetIterator()
{
	m_pCurrentItem = m_pQueueBegin;
}


//-----------------------------------------------------------------------------
// ���ڲ�ָ�����
//-----------------------------------------------------------------------------
VOID MsgQueue::IncIterator()
{
	if( !m_pCurrentItem )
		return;

	m_pCurrentItem = m_pCurrentItem->pNext;
}


//-----------------------------------------------------------------------------
// ��õ�ǰ�ڲ�ָ��ָ����Ϣ��������ȡ��
//-----------------------------------------------------------------------------
LPBYTE MsgQueue::PeekMsg(DWORD& dwMsgSize)
{
	dwMsgSize = 0;
	if( !m_pCurrentItem || m_nMsg <= 0 )
		return NULL;

	dwMsgSize = m_pCurrentItem->dwDataSize;
	return m_pCurrentItem->pData;
}


//-----------------------------------------------------------------------------
// �Ƴ���ǰ�ڲ�ָ��ָ����Ϣ��������ָ��
//-----------------------------------------------------------------------------
VOID MsgQueue::RemoveMsg()
{
	if( !m_pCurrentItem )
		return;

	MsgItem* pItem = m_pCurrentItem;
	if( m_pCurrentItem == m_pQueueBegin )
	{
		m_pQueueBegin = m_pQueueBegin->pNext;
		if( m_pQueueBegin )
			m_pQueueBegin->pPrev = NULL;
		m_pCurrentItem = m_pQueueBegin;

	}
	else if( m_pCurrentItem == m_pQueueEnd )
	{
		m_pQueueEnd = m_pQueueEnd->pPrev;
		if( m_pQueueEnd )
			m_pQueueEnd->pNext = NULL;

		m_pCurrentItem = NULL;
	}
	else
	{
		m_pCurrentItem->pPrev->pNext = m_pCurrentItem->pNext;
		m_pCurrentItem->pNext->pPrev = m_pCurrentItem->pPrev;
		m_pCurrentItem = m_pCurrentItem->pNext;
	}

	m_pMemPool->Free(pItem->pData);
	m_pMemPool->Free(pItem);
	m_nMsg--;
	if( 0 == m_nMsg )
	{
		m_pQueueBegin = m_pQueueEnd = m_pCurrentItem = NULL;
	}
}

//-----------------------------------------------------------------------------
// ���������Ϣ
//-----------------------------------------------------------------------------
VOID MsgQueue::Clear()
{
	MsgItem* pMsg = m_pQueueBegin; 
	while( m_pQueueBegin )
	{
		pMsg = m_pQueueBegin->pNext;
		m_pMemPool->Free( m_pQueueBegin->pData);
		m_pMemPool->Free(m_pQueueBegin);
		m_pQueueBegin = pMsg;
	}
	m_nMsg = 0;
	m_pQueueBegin = NULL;
	m_pQueueEnd = NULL;
	m_pCurrentItem = NULL;
}

}