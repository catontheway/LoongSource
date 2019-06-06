//-----------------------------------------------------------------------------
//!\file gui_staticex.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2008-06-17
//!
//!\brief ����ϵͳstatic ex�ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳ static ex �ؼ�
// ֧�ָ���ת���ַ�
// <font=type,width,height,weight>:�������壬����<font=Arial,0,16,400>
// <pic=filename,left,top,right,bottom>:ͼƬ�ļ�����ʹ��ͼƬ���ĸ���������ʾ
// <color=0xFFFFFFFF>:�������ֻ���ͼƬ��ɫ
// <link=id,LineColor>:��ʾһ�������ӣ��û������ᷢ�ʹ�id��Ϣ���¼�,
// ��<link=0xffffffff,0>��־���ӽ���
//-----------------------------------------------------------------------------
class VENGINE_API GUIStaticEx : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual BOOL Resize(tagPoint& ptSize);

	virtual VOID SetText(LPCTSTR szText){ m_strText = szText; ParseText(); SetRefresh(); }

	// �õ����ݵ��ܸ߶�
	INT	GetTextTotalHeight() { return m_nTotalHeight; }	

	GUIStaticEx();

protected:
	struct tagStaticExItem
	{
		tagGUIFont*		pFont;
		tagGUIImage*	pImage;
		tstring			strText;
		DWORD			dwColor;
		DWORD			dwLinkID;
		DWORD			dwLinkColor;
		tagPoint		ptPos;
		tagPoint		ptSize;

		tagStaticExItem() { pFont = NULL; pImage = NULL; dwLinkID = GT_INVALID; }
	};

	std::list<tagStaticExItem*>	m_listItems;
	std::list<tagStaticExItem*>::iterator m_itCurrent;

	TList<tagGUIFont*>		m_listFont;
	TList<tagGUIImage*>		m_listImage;

	INT						m_nTotalHeight;

	// ���ַ���������items
	VOID ParseText();

	// ���ַ��������ɹؼ���
	BOOL ParseKey(tstring& strKey, tstring& strValue, tagStaticExItem* pItem);

	// ���������Դ
	VOID ClearResource();


	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	INT			m_nRowHeight;					// �и�
};





}	// namespace vEngine {
