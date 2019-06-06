//-----------------------------------------------------------------------------
//!\file gui_combo.cpp
//!\author tidy / Lyp
//!
//!\date 2004-12-08
//! last 2004-12-15
//!
//!\brief ����ϵͳ combo box �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_combobox.h"

#include "gui_button.h"
#include "gui_editbox.h"
#include "gui_listbox.h"
#include "gui_event_define.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
GUIComboBox::GUIComboBox()
{
	m_pVarMgr->Register(_T("AutoResize"), &m_bAutoResize, _T("no yes"));
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIComboBox::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIWnd::Init(pSystem, pFather, pXmlElement);

	m_pEditBox = NULL;
	m_pButton = NULL;
	m_pListBox = NULL;

	m_pFont = m_pRender->CloneFont(m_pFather->GetFont());

	return bResult;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIComboBox::Destroy()
{
	GUIWnd::Destroy();

	// ɾ������
	m_pRender->DestroyFont(m_pFont);
}


//-----------------------------------------------------------------------------
// ���¼��������ص������������ϴ���,����TRUE��ʾ��Ϣ���ⲿ�ص���������
//-----------------------------------------------------------------------------
VOID GUIComboBox::SendEvent(tagGUIEvent* pEvent)
{
	if( !m_pEditBox || !m_pButton || !m_pListBox )
		return GUIWnd::SendEvent(pEvent);

	switch(pEvent->eEvent)
	{
	case EGUIE_Click:	// ����listbox
		m_pListBox->SetInvisible(!m_pListBox->IsInvisible());
		if( !m_pListBox->IsInvisible() )
			m_pSystem->SetActive(m_pListBox);
		break;

	case EGUIE_ItemClick:	// ����edit���ı�����
		m_pListBox->SetInvisible(true);
		if( P_VALID(m_pListBox->GetText(pEvent->dwParam1, 0)) )
		{
			m_pEditBox->SetText(m_pListBox->GetText(pEvent->dwParam1, 0));
			m_pSystem->SetActive(m_pEditBox);
			
			// ���ⲿ��������ComboBox EGUIE_EditBoxEnter
			tagGUIEvent event(this, EGUIE_EditBoxEnter);
			GUIWnd::SendEvent(&event);
		}
		break;

	case EGUIE_Deactive:
		if( pEvent->dwParam1 != (DWORD)this	// ��Ҫ����Ĳ��������Լ�
			&& pEvent->dwParam1 != (DWORD)m_pEditBox 
			&& pEvent->dwParam1 != (DWORD)m_pButton 
			&& pEvent->dwParam1 != (DWORD)m_pListBox )
		{
			m_pListBox->SetInvisible(true);	// �ر�listBox
			// ���ⲿ��������ComboBox Deactive
			tagGUIEvent event(this, EGUIE_Deactive, NULL, pEvent->dwParam1);
			GUIWnd::SendEvent(&event);
		}

		return;
		break;
	}

	return GUIWnd::SendEvent(pEvent);
}


//-----------------------------------------------------------------------------
// �ı��С
//-----------------------------------------------------------------------------
BOOL GUIComboBox::Resize(tagPoint& ptSize)
{
	BOOL bResult = GUIWnd::Resize(ptSize);
	
	if( bResult && m_bAutoResize )	
	{
		// �Զ��ı��ӿؼ���С��λ��
		m_pEditBox->SetMovable(true);
		m_pButton->SetMovable(true);
		m_pListBox->SetMovable(true);
		m_pEditBox->SetResizable(true);
		m_pButton->SetResizable(true);
		m_pListBox->SetResizable(true);
		
		tagPoint ptEditSize = m_pEditBox->GetSize();
		ptSize.x -= m_pButton->GetSize().x;
		m_pEditBox->Resize(ptSize);

		tagPoint pt(m_pEditBox->GetSize().x - ptEditSize.x, 0);
		ptSize.x = m_pButton->GetSize().x;
		m_pButton->Move(pt);
		m_pButton->Resize(ptSize);

		pt.Set(0, m_pEditBox->GetSize().y - m_pListBox->GetClient().y) ;
		ptSize.Set(m_pEditBox->GetSize().x, m_pListBox->GetSize().y);
		m_pListBox->Move(pt);
		m_pListBox->Resize(ptSize);

		// ���listbox �Ƿ��в�������Ļ֮��
		if( m_pListBox->GetView().y + m_pListBox->GetSize().y 
			> m_pSystem->GetDesktop()->GetSize().y )
		{
			pt.y = -(m_pEditBox->GetSize().y + m_pListBox->GetSize().y);
			m_pListBox->Move(pt);
		}

		m_pEditBox->SetMovable(false);
		m_pButton->SetMovable(false);
		m_pListBox->SetMovable(false);
		m_pEditBox->SetResizable(false);
		m_pButton->SetResizable(false);
		m_pListBox->SetResizable(false);
	}


	return bResult;

}



//-----------------------------------------------------------------------------
// ����Ӵ���
//-----------------------------------------------------------------------------
VOID GUIComboBox::AddChild(GUIWnd* pChild)
{
	if( pChild->GetClassName() == _T("CGUIEditBox") )
		m_pEditBox = (GUIEditBox*)pChild;
	
	if( pChild->GetClassName() == _T("CGUIButton") )
		m_pButton = (GUIButton*)pChild;

	if( pChild->GetClassName() == _T("CGUIListBox") )
		m_pListBox = (GUIListBox*)pChild;

	GUIWnd::AddChild(pChild);
}


//-----------------------------------------------------------------------------
// ɾ���Ӵ���
//-----------------------------------------------------------------------------
VOID GUIComboBox::DelChild(GUIWnd* pChild)
{
	if( pChild == m_pEditBox )
		m_pEditBox = NULL;
	if( pChild == m_pButton )
		m_pButton = NULL;
	if( pChild == m_pListBox )
		m_pListBox = NULL;

	GUIWnd::DelChild(pChild);
}

}	// namespace vEngine {
