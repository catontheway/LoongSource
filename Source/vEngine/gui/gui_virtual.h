//-----------------------------------------------------------------------------
//!\file gui_virtual.h
//!\author Lyp
//!
//!\date 2005-05-26
//! last 2005-05-27
//!
//!\brief ����ϵͳwindow/windowEx�ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳvirtual�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIVirtual : public GUIWnd
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual BOOL PointInWnd(tagPoint& pt);
};


}	// namespace vEngine {
