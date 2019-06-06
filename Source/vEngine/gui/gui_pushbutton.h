//-----------------------------------------------------------------------------
//!\file gui_pushbutton.h
//!\author Lyp
//!
//!\date 2007-04-20
//! last 2007-04-20
//!
//!\brief ����ϵͳ push button �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳ push button �ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIPushButton : public GUIButton
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent);

	// ���ð���״̬
	VOID SetState(EGUIButtonState eState, BOOL bSendMutexEvent=TRUE);

protected:
	BOOL m_bRadioButton;
};



}	// namespace vEngine {
