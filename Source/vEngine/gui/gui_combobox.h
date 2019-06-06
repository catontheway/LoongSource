//-----------------------------------------------------------------------------
//!\file xui_combo.h
//!\author tidy / Lyp
//!
//!\date 2004-12-08
//! last 2004-12-15
//!
//!\brief ����ϵͳ combo box �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳcombo box�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIComboBox : public GUIWnd
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID SendEvent(tagGUIEvent* pEvent);

	virtual BOOL Resize(tagPoint& ptSize); // �ı�ؼ���С,�����µĴ�С

	virtual VOID AddChild(GUIWnd* pChild);
	virtual VOID DelChild(GUIWnd* pChild);


	GUIButton* GetButton() { return m_pButton; }
	GUIEditBox* GetEditBox() { return m_pEditBox; }
	GUIListBox* GetListBox() { return m_pListBox; }
	

	GUIComboBox();

protected:
	GUIButton*		m_pButton;
	GUIEditBox*		m_pEditBox;
	GUIListBox*		m_pListBox;

	bool			m_bActive;	// �Ƿ������ӿؼ�������

	
	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	BOOL			m_bAutoResize;	// �Ƿ��Զ������ӿؼ���С

};


}	// namespace vEngine {
