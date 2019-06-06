//-----------------------------------------------------------------------------
// File: msg_queue_ts
// Desc: �ṩ�̰߳�ȫ���Ƚ��ȳ���Ϣ���й���
// Auth: Lyp
// Date: 2003-11-30
// Last: 2004-03-26
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

//-----------------------------------------------------------------------------
// MsgQueueTS: �ṩ�̰߳�ȫ���Ƚ��ȳ���Ϣ���й���
// ��ȡ��Ϣ��ʱ��ʹ���ڲ�������ڴ�,����踺���ͷ��ڴ�
// ������Ϣ����ʱ�����Լ���һ��Event���ʺ�������߳��շ�
// ��������Ϣǰ�������Ϣ���ȣ��ʺ����紫��
//-----------------------------------------------------------------------------
class VENGINE_API MsgQueueTS
{
public:

	// bActiveEvent: ������Ϣ����ʱ���Ƿ񼤻�һ���ڽ�event
	// bAddSizeAhead: �Ƿ�����Ϣͷ��ǰ������Ϣ���ȣ���ҪΪ����ͨ��׼��
	MsgQueueTS(BOOL bActiveEvent, BOOL bAddSizeAhead);
	~MsgQueueTS();


	// �����Ϣ������β�������Ƿ���ӳɹ�
	__forceinline BOOL AddMsg(LPCVOID pMsg, DWORD dwSize);

	// �Ӷ���ͷȡ��Ϣ������踺���ͷ��ڴ�
	__forceinline LPBYTE GetMsg(DWORD& dwMsgSize);

	// ��Ӧ��GetMsg(),��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
	__forceinline VOID FreeMsg(LPVOID pMsg);

	// ȡ���¼����������м�����Ϣʱ�����¼�������
	__forceinline HANDLE GetEvent() { return m_hEvent; }

	// �õ������е���Ϣ��Ŀ
	__forceinline INT GetMsgNum() { return m_nMsg; }

	//	�Ƿ���Ϣͷ���г�����Ϣ
	__forceinline BOOL IsAddSizeAhead()	{ return m_bAddSizeAhead; }
	

private:
	struct MsgItem
	{
		DWORD			dwDataSize;
		BYTE*			pData;
		MsgItem*		pNext;
	};

	SafeMemPool*		m_pMemPool;
	CRITICAL_SECTION	m_Lock;
	HANDLE				m_hEvent;
	INT					m_nMsg;
	BOOL				m_bEvent;
	BOOL				m_bAddSizeAhead;	

	MsgItem*			m_pQueueBegin;
	MsgItem*			m_pQueueEnd;
};



//-----------------------------------------------------------------------------
// �����Ϣ������β�������Ƿ���ӳɹ�
//-----------------------------------------------------------------------------
BOOL MsgQueueTS::AddMsg(LPCVOID pMsg, DWORD dwSize)
{
	DWORD dwOriginSize = dwSize;
	if( m_bAddSizeAhead ) // �˳�Ա�ڶ��󴴽���Ͳ���ı�
		dwSize += sizeof(DWORD);

	// �����µĵ�Ԫ
	MsgItem* pMsgItem = (MsgItem*)m_pMemPool->Alloc(sizeof(MsgItem));
	if( pMsgItem == NULL )
		return FALSE;

	// ��ʼ��
	pMsgItem->dwDataSize = dwSize;
	pMsgItem->pData = NULL;
	pMsgItem->pNext = NULL;

	// �������ݿռ�
	pMsgItem->pData = (LPBYTE)m_pMemPool->Alloc(dwSize);
	if( pMsgItem->pData == NULL )
	{
		delete(pMsgItem);
		return FALSE;
	}

	// ���뻥����
	EnterCriticalSection(&m_Lock);	

	// ��������
	if( m_bAddSizeAhead )
	{
		*(DWORD*)(pMsgItem->pData) = dwOriginSize;
		memcpy(pMsgItem->pData+sizeof(dwOriginSize), pMsg, dwOriginSize);
	}
	else
	{
		memcpy(pMsgItem->pData, pMsg, dwSize);
	}

	if( m_pQueueBegin == NULL )	// ���п�
	{
		m_pQueueBegin = pMsgItem;
		m_pQueueEnd = pMsgItem;
	}
	else
	{
		m_pQueueEnd->pNext = pMsgItem;
		m_pQueueEnd = pMsgItem;
	}

	m_nMsg++;

	if( m_bEvent )	// �����߳�
		::SetEvent(m_hEvent);	

	LeaveCriticalSection(&m_Lock);
	return TRUE;
}



//-----------------------------------------------------------------------------
// �Ӷ���ͷȡ��Ϣ��������Ϣ���ȣ�����踺���ͷ��ڴ�
//-----------------------------------------------------------------------------
LPBYTE MsgQueueTS::GetMsg(DWORD &dwMsgSize)
{
	dwMsgSize = 0;
	if( m_nMsg <= 0 )	// Ԥ�ȼ�飬���ⲻ��Ҫ�Ľ���critical section
		return NULL;	// no msg

	EnterCriticalSection(&m_Lock);

	if( m_nMsg <= 0 )	// �ڲ����м��
	{
		LeaveCriticalSection(&m_Lock);
		return NULL;	// no msg
	}

	// ȡ����Ϣ
	LPBYTE pMsg = m_pQueueBegin->pData;
	dwMsgSize = m_pQueueBegin->dwDataSize;

	MsgItem* pMsgItem = m_pQueueBegin;
	m_pQueueBegin = m_pQueueBegin->pNext;

	m_nMsg--;
	LeaveCriticalSection(&m_Lock);

	m_pMemPool->Free(pMsgItem);
	return pMsg;
}


//-----------------------------------------------------------------------------
// ��Ӧ��GetMsg(),��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
//-----------------------------------------------------------------------------
VOID MsgQueueTS::FreeMsg(LPVOID pMsg)
{
	m_pMemPool->Free(pMsg);
}


}