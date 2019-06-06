//-----------------------------------------------------------------------------
//!\file safe_list.h
//!\author Lyp
//!
//!\date 2004-07-07
//! last 2004-07-12
//!
//!\brief	��std::list��װ���̰߳�ȫ
//!
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

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
	}

	// ���Ԫ�ص������ײ�
	VOID PushFront(Type val)
	{
		EnterCriticalSection(&m_Lock);
		m_list.push_front(val);
		++m_nItemNum;
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
	INT Export(std::list<Type>& listDest)
	{
		INT n=0;
		EnterCriticalSection(&m_Lock);
		std::list<Type>::iterator it;
		for(it=m_list.begin(); it!=m_list.end(); ++it, ++n)
			listDest.push_back(*it);
            
		LeaveCriticalSection(&m_Lock);
		return n;
	}


	TSafeList():m_nItemNum(0) { InitializeCriticalSection(&m_Lock); }
	~TSafeList(){ DeleteCriticalSection(&m_Lock); }

private:
	std::list<Type>		m_list;
	INT					m_nItemNum;
	CRITICAL_SECTION	m_Lock;
};



}