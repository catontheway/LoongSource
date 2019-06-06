#pragma once

#include "StdAfx.h"

//------------------------------------------------------------------------------------------------------
// �ٽ�����ʵ����
//------------------------------------------------------------------------------------------------------
class Mutex
{
public:
	Mutex()		{ if(FALSE == InitializeCriticalSectionAndSpinCount(&cs, 4000)) { abort(); } nCishu = 0; }
	~Mutex()	{ nCishu; DeleteCriticalSection(&cs); }

	VOID Acquire() { EnterCriticalSection(&cs); InterlockedExchangeAdd((LPLONG)&nCishu, 1); }
	VOID Release() { LeaveCriticalSection(&cs); InterlockedExchangeAdd((LPLONG)&nCishu, -1); }
	BOOL TryAcquire() { return TryEnterCriticalSection(&cs); }

private:
	CRITICAL_SECTION cs;
	volatile INT nCishu;

};

//-------------------------------------------------------------------------------------------------------
// �¼���ʵ����
//-------------------------------------------------------------------------------------------------------
class Event
{
public:
	Event() { m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); ASSERT(m_hEvent != NULL); }
	Event(BOOL bManual, BOOL bInitState) { m_hEvent = CreateEvent(NULL, bManual, bInitState, NULL); ASSERT(m_hEvent != NULL); }
	~Event() { CloseHandle(m_hEvent); }

	VOID Set() { SetEvent(m_hEvent); }
	VOID ReSet() { ResetEvent(m_hEvent); }
	VOID Pulse() { PulseEvent(m_hEvent); }
	VOID Wait()	{ WaitForSingleObject(m_hEvent, INFINITE); }

private:
	HANDLE	m_hEvent;
};

//-----------------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------------
class FastMutex
{
public:
	FastMutex() : m_lLock(0), m_dwRecursiveCount(0) {}
	~FastMutex() {}

	VOID Acquire()
	{
		DWORD dwThreadID = GetCurrentThreadId(), dwOwner;

		if( dwThreadID == (DWORD)m_lLock )
		{
			++m_dwRecursiveCount;
			return;
		}

		for(;;)
		{
			dwOwner = InterlockedCompareExchange((LPLONG)&m_lLock, dwThreadID, 0);
			if(dwOwner == 0) break;

			Sleep(0);
		}

		++m_dwRecursiveCount;
	}

	BOOL TryAcquire()
	{
		DWORD dwThreadID = GetCurrentThreadId();
		if( dwThreadID == (DWORD)m_lLock )
		{
			++m_dwRecursiveCount;
			return TRUE;
		}

		DWORD dwOwner = InterlockedCompareExchange((LPLONG)&m_lLock, dwThreadID, 0);
		if( dwOwner == 0 )
		{
			++m_dwRecursiveCount;
			return TRUE;
		}

		return FALSE;
	}

	VOID Release()
	{
		if( (--m_dwRecursiveCount) == 0 )
			InterlockedExchange((LPLONG)&m_lLock, 0);
	}

private:

#pragma pack(push,8)
	VOLATILE LONG	m_lLock;				// ��
#pragma pack(pop)
	DWORD			m_dwRecursiveCount;		// ͬһ�̵߳�ѭ������
};


