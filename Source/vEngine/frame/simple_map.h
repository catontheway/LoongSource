//-----------------------------------------------------------------------------
//!\file simple_map.h
//!\author Lyp
//!
//!\date 2004-10-27
//! last 2004-10-27
//!
//!\brief	�ṩ�򵥵�map����
//!
//!
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

//-----------------------------------------------------------------------------
// �ṩ�򵥵�map����
//-----------------------------------------------------------------------------
template<class KeyType, class ValueType> class TMap
{
public:
	typedef typename std::map<KeyType, ValueType>::iterator TMapIterator;

public:
	//! ���Ԫ��
	VOID Add(KeyType key, ValueType value)
	{ m_map.insert(std::make_pair(key, value)); }
	
	//! ��ȡԪ��
	ValueType Peek(KeyType key)
	{
		std::map<KeyType, ValueType>::iterator it = m_map.find(key);
		if( it == m_map.end() )
			return ValueType(GT_INVALID);
		else
			return it->second;
	}
	
	//! �ı�ָ��key��Ӧ��ֵ
	BOOL ChangeValue(KeyType key, ValueType new_value)
	{
		std::map<KeyType, ValueType>::iterator it = m_map.find(key);
		if( it == m_map.end() )
			return FALSE;

		it->second = new_value;
		return TRUE;
	}

	//! �ۼ�ָ��key��Ӧ��ֵ�����key�����ڣ���ֱ����ӣ�Ҫ��ValueType������operator+��operator=���أ�
	VOID ModifyValue(KeyType key, ValueType mod_value)
	{
		std::map<KeyType, ValueType>::iterator it = m_map.find(key);
		if( it == m_map.end() )
		{
			m_map.insert(std::make_pair(key, mod_value));
		}
		else
		{
			it->second = it->second + mod_value;
		}
	}

	//! ���ָ��Ԫ���Ƿ����
	BOOL IsExist(KeyType key)
	{
		std::map<KeyType, ValueType>::iterator it = m_map.find(key);
		if( it == m_map.end() )
			return FALSE;
		else
			return TRUE;
	}

	//! ɾ��һ��ָ��Ԫ��
	BOOL Erase(KeyType key)
	{ 
		std::map<KeyType, ValueType>::iterator it = m_map.find(key);
		if( it == m_map.end() )
			return FALSE;

        m_map.erase(it);
		return TRUE;
	}

	//! �������Ԫ��
	VOID Clear() { m_map.clear(); }

	//! �õ�Ԫ�ظ���
	INT	Size() { return (INT)m_map.size(); }

	BOOL Empty() { return m_map.empty(); }

	//! ���ڲ��ĵ�������ʼ����map�Ŀ�ʼ
	VOID ResetIterator()
	{ m_it = m_map.begin(); }

	// ! �õ�map��ͷ
	TMapIterator Begin()
	{ return m_map.begin(); }

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	BOOL PeekNext(ValueType& value)
	{
		if( m_it == m_map.end() )
			return FALSE;
		value = m_it->second;
		++m_it;
		return TRUE;
	}

	//! ȡ��ָ��������ָ���Ԫ�أ������Ƹõ�����
	BOOL PeekNext(TMapIterator& it, ValueType& value)
	{
		if( it == m_map.end() )
			return FALSE;
		value = it->second;
		++it;
		return TRUE;
	}

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	BOOL PeekNext(KeyType& key, ValueType& value)
	{
		if( m_it == m_map.end() )
			return FALSE;
		key = m_it->first;
		value = m_it->second;
		++m_it;
		return TRUE;
	}

	//! ȡ��ָ���ĵ�����ָ���Ԫ�أ������Ƶ�����
	BOOL PeekNext(TMapIterator& it, KeyType& key, ValueType& value)
	{
		if( it == m_map.end() )
			return FALSE;
		key = it->first;
		value = it->second;
		++it;
		return TRUE;
	}

	//! ��list���������ȡһ��Ԫ��
	BOOL RandPeek(KeyType& key, ValueType& value)
	{
		INT nSize = m_map.size();
		if( nSize <= 0 )
			return FALSE;

		INT nRand = rand() % nSize;

		std::map<KeyType, ValueType>::iterator it = m_map.begin();
		for(INT n=0; n<nRand; n++)
			++it;

		key = it->first;
		value = it->second;
		return TRUE;
	}


	//! ������key������һ������std::list
	VOID ExportAllKey(std::list<KeyType>& listDest)
	{
		std::map<KeyType, ValueType>::iterator it;
		for(it = m_map.begin(); it != m_map.end(); ++it)
			listDest.push_back(it->first);
	}
	
	//! ������value������һ������std::list
	VOID ExportAllValue(std::list<ValueType>& listDest)
	{
		std::map<KeyType, ValueType>::iterator it;
		for(it = m_map.begin(); it != m_map.end(); ++it)
			listDest.push_back(it->second);
	}


private:
	std::map<KeyType, ValueType>					m_map;
	typename std::map<KeyType, ValueType>::iterator m_it;
};


}