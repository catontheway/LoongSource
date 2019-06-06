//-----------------------------------------------------------------------------
// File: interface_helper
// Auth: Lyp
// Date: 2004/3/31
// Last: 2004/3/31
//
// Desc: �ӿڸ�����
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {
//-----------------------------------------------------------------------------
// ָ�����֧����
//-----------------------------------------------------------------------------
template<typename T> class TObjRef
{
public:
	T* operator->(){ return (T*)m_pRaw; }	// ��׼ָ��
	operator BOOL() { return (m_pRaw!=NULL); }	// ���Ժ�NULL���бȽ�
	operator T*() { return (T*)m_pRaw; }	// �Զ�ת��Ϊָ������

	T* operator=(LPCSTR szObjName) 
	{ 
		if( !vEngine::g_pInterfaceMgr )
			return NULL;

		if( szObjName )
			m_pRaw = vEngine::g_pInterfaceMgr->Get(szObjName);
		else
			m_pRaw = vEngine::g_pInterfaceMgr->GetByTypeName(typeid(T*).raw_name()); 
		return (T*)m_pRaw;
	}

	TObjRef(LPCSTR szObjName=NULL)
	{ 
		operator=(szObjName); 
	}

private:
	LPVOID	m_pRaw;
};


} // namespace vEngine {
