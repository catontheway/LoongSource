//-----------------------------------------------------------------------------
// File: msg_queue_ts
// Desc: �ṩ�̰߳�ȫ���Ƚ��ȳ���Ϣ���й���
// Auth: Lyp
// Date: 2003-11-30
// Last: 2004-03-26
//
// Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "msg_queue_ts.h"

//-----------------------------------------------------------------------------
// MsgQueueTS construction / destruction
//-----------------------------------------------------------------------------
MsgQueueTS::MsgQueueTS(BOOL bActiveEvent, BOOL bAddSizeAhead)
{
	m_bEvent = bActiveEvent;
	m_bAddSizeAhead = bAddSizeAhead;
	
	m_nMsg = 0;
	m_hEvent = NULL;
	m_pQueueBegin = NULL;
	m_pQueueEnd = NULL;
	
	InitializeCriticalSection(&m_Lock);

	if( m_bEvent )
		m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// �Զ�����
}

MsgQueueTS::~MsgQueueTS()
{
	MsgItem* pMsg = m_pQueueBegin; 
	while( m_pQueueBegin )
	{
		pMsg = m_pQueueBegin->pNext;
		delete[]( m_pQueueBegin->pData);
		delete(m_pQueueBegin);
		m_pQueueBegin = pMsg;
	}

	if( m_bEvent )
		CloseHandle(m_hEvent);

	DeleteCriticalSection(&m_Lock);
}


//-----------------------------------------------------------------------------
// �����Ϣ������β�������Ƿ���ӳɹ�
//-----------------------------------------------------------------------------
BOOL MsgQueueTS::AddMsg(LPCVOID pMsg, DWORD dwSize)
{
	DWORD dwOriginSize = dwSize;
	if( m_bAddSizeAhead ) // �˳�Ա�ڶ��󴴽���Ͳ���ı�
		dwSize += sizeof(DWORD);

	// �����µĵ�Ԫ
	MsgItem* pNewMsg = new MsgItem;
	if( pNewMsg == NULL )
		return FALSE;

	// ��ʼ��
	pNewMsg->dwDataSize = dwSize;
	pNewMsg->pData = NULL;
	pNewMsg->pNext = NULL;
	
	// �������ݿռ�
	pNewMsg->pData = new BYTE[dwSize];
	if( pNewMsg->pData == NULL )
	{
		delete(pNewMsg);
		return FALSE;
	}

	// ���뻥����
	EnterCriticalSection(&m_Lock);	

	// ��������
	if( m_bAddSizeAhead )
	{
		*(DWORD*)(pNewMsg->pData) = dwOriginSize;
		memcpy(pNewMsg->pData+sizeof(dwOriginSize), pMsg, dwOriginSize);
	}
	else
	{
		memcpy(pNewMsg->pData, pMsg, dwSize);
	}
	
	if( m_pQueueBegin == NULL )	// ���п�
	{
		m_pQueueBegin = pNewMsg;
		m_pQueueEnd = pNewMsg;
	}
	else
	{
		m_pQueueEnd->pNext = pNewMsg;
		m_pQueueEnd = pNewMsg;
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
	
	MsgItem* pTempMsg = m_pQueueBegin;
	m_pQueueBegin = m_pQueueBegin->pNext;

	m_nMsg--;
	LeaveCriticalSection(&m_Lock);

	delete(pTempMsg);
	return pMsg;
}

