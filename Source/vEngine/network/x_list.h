//-----------------------------------------------------------------------------
//!\file x_list.h
//!\author Lyp
//!
//!\date 2004-07-07
//! last 2009-06-11
//!
//!\brief	��std::list��װ���̰߳�ȫ
//!
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

//-----------------------------------------------------------------------------
// ��std::list��װ���̰߳�ȫ
//-----------------------------------------------------------------------------
template<typename Type> class XList
{
public:
	__forceinline VOID PushBack(Type val)	// ���Ԫ�ص�����β
	{
		Lock();
		m_list.push_back(val);
		++m_nItemNum;
		Unlock();
	}

	__forceinline Type PopFront()	// �Ӷ���ͷȡԪ��
	{
		if( m_nItemNum <= 0 )
			return Type(GT_INVALID);

		Lock();
		if( m_nItemNum <= 0 )
		{
			Unlock();
			return Type(GT_INVALID);
		}
		Type val = m_list.front();
		m_list.pop_front();
		m_nItemNum--;
		Unlock();
		return val;
	}
	
	__forceinline BOOL Erase(Type& val)	// ɾ��ָ��Ԫ��
	{
		std::list<Type>::iterator it;
		Lock();
		for(it=m_list.begin(); it!=m_list.end(); ++it)
		{
			if( val == *it )
			{
				m_list.erase(it);
				--m_nItemNum;
				Unlock();
				return TRUE;
			}
		}
		Unlock();
		return FALSE;
	}

	__forceinline BOOL IsExist(Type& val)	// ���ָ��Ԫ���Ƿ����
	{
		Lock();
		std::list<Type>::iterator it;
		for(it=m_list.begin(); it!=m_list.end(); ++it)
		{
			if( val == *it )
			{
				Unlock();
				return TRUE
			}
		}
		Unlock();
		return FALSE;
	}

	__forceinline VOID Clear()	// �������Ԫ��
	{
		Lock();
		m_list.clear();
		m_nItemNum=0;
		Unlock();
	}

	// �õ�Ԫ����Ŀ,std::list.size()�����ܱ�֤�̰߳�ȫ,���������Լ�����һ�ݸ�������
	__forceinline INT	Size() { return m_nItemNum;	}

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����,ע��Ҫlock�����ʹ��
	__forceinline BOOL _Peek(Type& value)
	{
		if( m_it == m_list.end() )
			return FALSE;
		value = *m_it;
		return TRUE;
	}

	//! ���ڲ��ĵ�������ʼ����map�Ŀ�ʼ,ע��Ҫlock�����ʹ��
	__forceinline VOID _ResetIterator(){ m_it = m_list.begin(); }
	__forceinline VOID _AddIterator(){ ++m_it; }
	__forceinline VOID _EraseCurrent(){ m_list.erase(m_it); }

	__forceinline XList():m_nItemNum(0), m_lLock(0) {}
	__forceinline ~XList(){ }//ASSERT( 0 == m_nItemNum); }
	__forceinline VOID	Lock() { while(InterlockedCompareExchange((LPLONG)&m_lLock, 1, 0)!= 0) Sleep(0); }
	__forceinline VOID	Unlock() { InterlockedExchange((LPLONG)(&m_lLock), 0); }

private:
	std::list<Type>						m_list;
	typename std::list<Type>::iterator	m_it;
	
	INT	VOLATILE	m_nItemNum;
	LONG VOLATILE	m_lLock;
};

}