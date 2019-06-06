#pragma once
#pragma warning(disable:4251)

namespace Cool3D
{
//-----------------------------------------------------------------------------
// ��std::list��װ���̰߳�ȫ
//-----------------------------------------------------------------------------
template<typename Type> class TSafeList
{
public:
	// ���Ԫ�ص�����β
	VOID PushBack(Type val)
	{
		EnterCriticalSection(&m_Lock);
		m_list.push_back(val);
		++m_nItemNum;
		LeaveCriticalSection(&m_Lock);
		::SetEvent(m_hEvent);
	}

	// �Ӷ���ͷȡԪ��
	Type PopFront()
	{
		Type val;
		if( m_nItemNum <= 0 )
			return Type(-1);

		EnterCriticalSection(&m_Lock);
		if( m_nItemNum <= 0 )
		{
			LeaveCriticalSection(&m_Lock);
			return Type(-1);
		}

		val = m_list.front();
		m_list.pop_front();
		m_nItemNum--;

		LeaveCriticalSection(&m_Lock);
		return val;
	}

	// ɾ��ָ��Ԫ��
	BOOL Erase(Type& val)
	{
		std::list<Type>::iterator it;
		EnterCriticalSection(&m_Lock);
		for(it=m_list.begin(); it!=m_list.end(); ++it)
		{
			if( val == *it )
			{
				m_list.erase(it);
				--m_nItemNum;
				LeaveCriticalSection(&m_Lock);
				return TRUE;
			}
		}
		LeaveCriticalSection(&m_Lock);
		return FALSE;
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

	// ��ȫ�Ľ����ݵ���һ����ͨstd::list,�������ݸ���
	INT Export(list<Type>& listDest)
	{
		INT n=0;
		EnterCriticalSection(&m_Lock);
		list<Type>::iterator it;
		for(it=m_list.begin(); it!=m_list.end(); ++it, ++n)
			listDest.push_back(*it);

		LeaveCriticalSection(&m_Lock);
		return n;
	}

	// ȡ���¼����������м�����Ϣʱ�����¼�������
	HANDLE	GetEvent() { return m_hEvent; }

	TSafeList():m_nItemNum(0) { InitializeCriticalSection(&m_Lock); m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL); }	
	~TSafeList(){ DeleteCriticalSection(&m_Lock); CloseHandle(m_hEvent); }

private:
	std::list<Type>		m_list;
	INT					m_nItemNum;
	CRITICAL_SECTION	m_Lock;
	HANDLE				m_hEvent;
};

};