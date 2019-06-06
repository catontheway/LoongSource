//-----------------------------------------------------------------------------
//!\file gui_tab.h
//!\author tidy / Lyp
//!
//!\date 2005-4-13
//! last 2005-4-21
//!
//!\brief ����ϵͳ tab �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳtab�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUITab : public GUIWnd
{
public:
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	virtual VOID OnEvent(tagGUIEvent* pEvent);
	virtual GUIWnd* OnActive();

	virtual VOID AddChild(GUIWnd* pChild);	// ����Ӵ���
	virtual VOID DelChild(GUIWnd* pChild);	// ɾ���Ӵ���

	GUITab():m_pWnd((GUIWnd*)GT_INVALID),m_pRadioButton((GUIWnd*)GT_INVALID){}

protected:
	GUIWnd*	m_pRadioButton;
	GUIWnd*	m_pWnd;		//����һ���ؼ�(�����Ǹ��Ͽؼ�)

};


}	// namespace vEngine {
