//--------------------------------------------------------------------------------------------------
// File: SafeStreamQueue.h
// Desc: Thread safe Mystream queue 
// Time: 2008-05-05
// Auth: Aslan
//
// Copyright (c) 2008 Tengreng All rights reserved.
//---------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Config.h"
#include "MysqlStream.h"

namespace Beton {

class MyStream;
class Mutex;
//----------------------------------------------------------------------------------------------------
/**	SafeStream�� - ����MyStream����ָ�룬Ϊ��ʵʱ�����ṩ֧��
1. �ⲿ���ݿ����в���Ҫ����������sql��䣬���ɷ���ö����У��ɱ���̲߳���
2. Ԥ�ȷ����Mystream���������ʱ��������У�������ȡ��ʹ��
*/
//-----------------------------------------------------------------------------------------------------
class SafeStreamQueue
{
public:
	SafeStreamQueue() : m_lNum(0) { m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); }
	virtual ~SafeStreamQueue();

private:
	SafeStreamQueue(const SafeStreamQueue&);
	SafeStreamQueue& operator = (const SafeStreamQueue&);

public:
	BETON_INLINE MyStream* Get();
	BETON_INLINE VOID Add(MyStream* pStream);
	BETON_INLINE LONG Size() const { return m_lNum; }

	BETON_INLINE HANDLE GetEvent() { return m_hEvent; }

private:
	Mutex					m_Mutex;
	HANDLE					m_hEvent;
	std::list<MyStream*>	m_listQueue;
	volatile LONG			m_lNum;
};

//---------------------------------------------------------------------------------------------------------
// �ⲿ��ȡһ��stream
//---------------------------------------------------------------------------------------------------------
MyStream* SafeStreamQueue::Get()
{
	if( m_lNum == 0 ) return NULL;

	m_Mutex.Acquire();

	if( m_lNum == 0 )
	{
		m_Mutex.Release();
		return NULL;
	}
	MyStream* pStream = m_listQueue.front();
	m_listQueue.pop_front();
	InterlockedExchangeAdd((LPLONG)&m_lNum, -1);

	m_Mutex.Release();

	return pStream;
}

//-----------------------------------------------------------------------------------------------------------
// �õ�һ��stream
//-----------------------------------------------------------------------------------------------------------
VOID SafeStreamQueue::Add(MyStream* pStream)
{
	if( NULL == pStream ) return;

	m_Mutex.Acquire();

	m_listQueue.push_back(pStream);
	InterlockedExchangeAdd((LPLONG)&m_lNum, 1);

	if( m_lNum == 1 )
		::SetEvent(m_hEvent);	// �����¼�

	m_Mutex.Release();
}

} // namespace Beton