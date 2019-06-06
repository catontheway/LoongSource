//-----------------------------------------------------------------------------
//!\file gui_httppic.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2008-04-24
//!
//!\brief ����ϵͳhttp picture�ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
class HttpDownload;
//-----------------------------------------------------------------------------
// ����ϵͳstatic�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIHttpPic : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual VOID Update();

	GUIHttpPic();
	virtual ~GUIHttpPic();

protected:
	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------


	HttpDownload* m_pHttp;


	
};

}