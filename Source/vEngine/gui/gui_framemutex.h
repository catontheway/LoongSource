//-----------------------------------------------------------------------------
//!\file gui_framemutex.h
//!\author tidy / Lyp
//!
//!\date 2005-4-13
//! last 2007-5-15
//!
//!\brief ����ϵͳ framemutex �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳFrameMutex�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIFrameMutex : public GUIWnd
{
public:
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	VOID SetPush(GUIWnd *pWnd);
};


}	// namespace vEngine {
