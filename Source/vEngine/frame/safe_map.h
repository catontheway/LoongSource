//-----------------------------------------------------------------------------
//!\file safe_map.h
//!\author Lyp
//!
//!\date 2004-07-09
//! last 2004-08-16
//!
//!\brief	�ṩ���������̰߳�ȫ��map����
//!			
//!
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

//-----------------------------------------------------------------------------
// �ṩ���������̰߳�ȫ��map����
//-----------------------------------------------------------------------------
template<typename KeyType, typename ValueType> class TSafeMap
{
public:
	//! ���Ԫ��
	VOID Add(KeyType key, ValueType value)
	{
		EnterCriticalSection(&m_Lock);
		m_map.insert(std::pair<KeyType, ValueType>(key, value));
		LeaveCriticalSection(&m_Lock);
	}

	//! ��ȡԪ��
	ValueType Peek(KeyType key)
	{
		ValueType temp;
		std::map<KeyType, ValueType>::iterator it;
		EnterCriticalSection(&m_Lock);
		it = m_map.find(key);
		if( it == m_map.end() )
			temp = ValueType(GT_INVALID);
		else
			temp = it->second;
		LeaveCriticalSection(&m_Lock);
		return temp;
	}

	//! �ı�ָ��key��Ӧ��ֵ
	BOOL ChangeValue(KeyType key, ValueType new_value)
	{
		std::map<KeyType, ValueType>::iterator it;
		EnterCriticalSection(&m_Lock);
		it = m_map.find(key);
		
		if( it == m_map.end() )
		{
			LeaveCriticalSection(&m_Lock);
			return FALSE;
		}
		else
		{
			it->second = new_value;
			LeaveCriticalSection(&m_Lock);
			return TRUE;
		}
	}

	//! ���ָ��Ԫ���Ƿ����
	BOOL IsExist(KeyType key)
	{
		BOOL bResult = FALSE;
		std::map<KeyType, ValueType>::iterator it;
		EnterCriticalSection(&m_Lock);
		it = m_map.find(key);
		
		if( it == m_map.end() )
			bResult = FALSE;
		else
			bResult = TRUE;
		LeaveCriticalSection(&m_Lock);

		return bResult;
	}


	//! ɾ��ָ��Ԫ��
	BOOL Erase(KeyType key)
	{
		BOOL bResult = FALSE;
		std::map<KeyType, ValueType>::iterator it;
		
		EnterCriticalSection(&m_Lock);
		it = m_map.find(key);
		if( it == m_map.end() )
		{
			bResult = FALSE;
		}
		else
		{
			m_map.erase(it);
			bResult = TRUE;
		}
		LeaveCriticalSection(&m_Lock);
		return bResult;
	}


	//! �������Ԫ��
	VOID Clear()
	{
		EnterCriticalSection(&m_Lock);
		m_map.clear();
		LeaveCriticalSection(&m_Lock);
	}

	//! �õ�Ԫ�ظ���
	INT	Size() { return (INT)m_map.size(); }


	//! ������key������һ������std::list,�����ص����ĸ���
	INT ExportAllKey(std::list<KeyType>& listDest)
	{
		INT n=0;
		EnterCriticalSection(&m_Lock);
		std::map<KeyType, ValueType>::iterator it;
		for(it = m_map.begin(); it != m_map.end(); ++it, ++n)
			listDest.push_back(it->first);
		LeaveCriticalSection(&m_Lock);
		return n;
	}

	//! ������value������һ������std::list,�����ص����ĸ���
	INT ExportAllValue(std::list<ValueType>& listDest)
	{
		INT n=0;
		EnterCriticalSection(&m_Lock);
		std::map<KeyType, ValueType>::iterator it;
		for(it = m_map.begin(); it != m_map.end(); ++it, ++n)
			listDest.push_back(it->second);
		LeaveCriticalSection(&m_Lock);
		return n;
	}

	VOID Lock() { EnterCriticalSection(&m_Lock); }
	VOID Unlock() { LeaveCriticalSection(&m_Lock); }


	TSafeMap() { InitializeCriticalSection(&m_Lock); }
	~TSafeMap()	{ DeleteCriticalSection(&m_Lock); }


private:
	std::map<KeyType, ValueType>			m_map;
	CRITICAL_SECTION						m_Lock;
	
	

};



}