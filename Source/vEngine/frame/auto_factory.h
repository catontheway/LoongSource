//-----------------------------------------------------------------------------
//!\file auto_factory.h
//!\author Lyp
//!\date 2004/3/11
//! last 2004/3/13
//!
//!\brief ģ����: �Զ��೧
//-----------------------------------------------------------------------------
#pragma once
#include "binder.h"

namespace vEngine {
//-----------------------------------------------------------------------------
//!\brief �Զ��೧
//!
//!	ʹ����ע�ᶼ�ܷ�����Զ��೧
//!	ʹ��Register( TBinder<YOUR_CLASS>(), DWORD dwID )�ĸ�ʽ��ע��
//!	Ȼ��ʹ��Create( DWORD dwID )���������ʵ��
//-----------------------------------------------------------------------------
template<typename abstract_class>	// ������Ҫ��ָ��������ģ�����
class TAutoFactory
{
public:
	//! ע�������
	template<typename concrete_class>
	BOOL Register(TWrap<concrete_class>, DWORD dwID)
	{ 
		TBinder<concrete_class>* pBinder = new TBinder<concrete_class>;
		if( NULL == pBinder )
			return FALSE;

		ASSERT( m_map.find(dwID) == m_map.end() );	// �������ظ�ע��
		m_map.insert(std::pair<DWORD, BinderAbstract*>(dwID, pBinder));
		return TRUE;
	}

	template<typename concrete_class>
	BOOL Unregister(TWrap<concrete_class>, DWORD dwID)
	{
		std::map<DWORD, BinderAbstract*>::iterator iter = m_map.find(dwID);
		if( iter == m_map.end() )
			return FALSE;

		delete((TBinder<concrete_class>*)(iter->second));
		m_map.erase(dwID);
		return TRUE;
	}

	//! ����ID��������ʵ��
	abstract_class* Create(DWORD dwID)
	{
		std::map<DWORD, BinderAbstract*>::iterator iter = m_map.find(dwID);
		if( iter == m_map.end() )
			return NULL;

		return (abstract_class*)iter->second->Create();
	}

	//! ɾ��ID��������ʵ��
	VOID Destroy(LPVOID p, DWORD dwID)
	{
		std::map<DWORD, BinderAbstract*>::iterator iter = m_map.find(dwID);
		if( iter != m_map.end() )
			iter->second->Destroy(p);
	}

	~TAutoFactory() 
	{	
		std::map<DWORD, BinderAbstract*>::iterator iter;
		for( iter=m_map.begin(); iter!=m_map.end(); ++iter )
			delete(iter->second);
	}

private:
	std::map<DWORD, BinderAbstract*> m_map;
};


} // namespace vEngine {
