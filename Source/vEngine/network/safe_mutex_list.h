//-----------------------------------------------------------------------------
// File: safe_mutex_list.h
// Desc: �ṩ�̰߳�ȫ���Ƚ��ȳ�������й���
// Auth: Lyp
// Date: 2003-11-30
// Last: 2004-09-24
//-----------------------------------------------------------------------------
#pragma once
#include "server_define.h"
namespace vEngine {

//-----------------------------------------------------------------------------
// TSafeMutexList: �ṩ�̰߳�ȫ���Ƚ��ȳ�������й���
//-----------------------------------------------------------------------------
template<typename Type> class TSafeMutexList
{
public:
	// ���Ԫ�ص�����β
	VOID PushBack(Type val)
	{
		std::list<Type>::iterator it;
		EnterCriticalSection(&m_Lock);
		m_list.push_back(val);
		m_nItemNum++;
		::SetEvent(m_hEvent);	// ����event	
		LeaveCriticalSection(&m_Lock);
	}

	// �Ӷ���ͷȡԪ��
	Type PopFront()
	{
		Type val;
		if( m_nItemNum <= 0 )
			return Type(GT_INVALID);

		EnterCriticalSection(&m_Lock);
		if( m_nItemNum <= 0 )
		{
			LeaveCriticalSection(&m_Lock);
			return Type(GT_INVALID);
		}

		val = m_list.front();
		m_list.pop_front();
		m_nItemNum--;
		
		LeaveCriticalSection(&m_Lock);
		return val;
	}


	// ɾ��ָ��Ԫ��
	BOOL Erase(Type val)
	{
		std::list<Type>::iterator it;
		BOOL bResult = FALSE;
		EnterCriticalSection(&m_Lock);
		for(it=m_list.begin(); it!=m_list.end(); ++it)
		{
			if( val == *it )
			{
				m_list.erase(it);
				m_nItemNum--;
				bResult = TRUE;
				break;
			}
		}
		LeaveCriticalSection(&m_Lock);
		return bResult;
	}

	// ���ָ��Ԫ���Ƿ����
	BOOL IsExist(Type& val)
	{
		BOOL bFound = FALSE;
		EnterCriticalSection(&m_Lock);
		std::list<Type>::iterator it;
		for(it=m_list.begin(); it!=m_list.end(); ++it)
		{
			if( val == *it )
			{
				bFound = TRUE;
				break;
			}
		}
		LeaveCriticalSection(&m_Lock);
		return bFound;
	}

	// �������Ԫ��
	VOID Clear()
	{
		EnterCriticalSection(&m_Lock);
		m_list.clear();
		m_nItemNum=0;
		LeaveCriticalSection(&m_Lock);
	}

	// �õ�Ԫ����Ŀ,std::list.size()�����ܱ�֤�̰߳�ȫ,
	// ���������Լ�����һ�ݸ�������
	INT	Size() { return m_nItemNum;	}

	VOID Lock() { EnterCriticalSection(&m_Lock); }
	VOID Unlock() { LeaveCriticalSection(&m_Lock); }

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����,ע��Ҫlock�����ʹ��
	BOOL PeekNext(Type& value)
	{
		if( m_it == m_list.end() )
			return FALSE;
		value = *m_it;
		++m_it;
		return TRUE;
	}

	//! ���ڲ��ĵ�������ʼ����map�Ŀ�ʼ,ע��Ҫlock�����ʹ��
	VOID ResetIterator(){ m_it = m_list.begin(); }

	// ȡ���¼����������м�����Ϣʱ�����¼�������
	HANDLE	GetEvent() { return m_hEvent; }

	TSafeMutexList():m_nItemNum(0) 
	{ 
		m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// �Զ�����
		InitializeCriticalSectionAndSpinCount(&m_Lock, DEFAULT_SPIN_COUNT); 
	}
	~TSafeMutexList()
	{ 
		DeleteCriticalSection(&m_Lock); 
		CloseHandle(m_hEvent);
	}

private:
	std::list<Type>						m_list;
	INT									m_nItemNum;
	HANDLE								m_hEvent;
	CRITICAL_SECTION					m_Lock;
	typename std::list<Type>::iterator	m_it;
};





}