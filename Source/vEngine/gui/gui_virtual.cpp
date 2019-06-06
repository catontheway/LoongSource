//-----------------------------------------------------------------------------
//!\file gui_virtual.cpp
//!\author Lyp
//!
//!\date 2005-05-26
//! last 2005-05-27
//!
//!\brief ����ϵͳ����ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_virtual.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIVirtual::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIWnd::Init(pSystem, pFather, pXmlElement);
	if( FALSE == bResult )
		return FALSE;

	m_ptClient.Clear();
	m_ptSize = pFather->GetSize();

	// ���ʵ������
	m_ptView = m_ptClient;
	ClientToView(m_ptView);

	m_bMovable = false;		// ���ǲ����ƶ�
	return bResult;
}




//-----------------------------------------------------------------------------
// ָ�����Ƿ������Ǵ�����
//-----------------------------------------------------------------------------
BOOL GUIVirtual::PointInWnd(tagPoint& pt)
{
	if( m_bInvisible )
		return FALSE;

	// �����Ƿ������ǵ������Ӵ�����
	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
	{
		if( TRUE == (*it)->PointInWnd(pt) )
			return TRUE;
	}

	return FALSE;
}

}	// namespace vEngine {
