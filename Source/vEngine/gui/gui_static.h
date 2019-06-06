//-----------------------------------------------------------------------------
//!\file gui_static.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2004-12-10
//!
//!\brief ����ϵͳstatic�ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳstatic�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIStatic : public GUIWnd
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual BOOL Resize(tagPoint& ptSize);
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	virtual VOID SetText(LPCTSTR szText){ m_strText = szText; SetRefresh(); }

	// ���߽ӿ�
	CONST tstring& GetText() { return m_strText; }
	VOID SetTextColor(DWORD dwColor) { m_dwTextColor = dwColor; }
	VOID SetShadeColor(DWORD dwColor) { m_dwShadeColor = dwColor; }
	VOID SetPicColor(DWORD dwColor) { m_dwPicColor = dwColor; }
	CONST tstring& GetTips() { return m_strTips; }
	VOID SetTips(LPCTSTR szText){ m_strTips = szText; SetRefresh(); }

	GUIStatic();

protected:
	tagGUIImage*		m_pPic;				// ͼƬ
	bool				m_bPointInWnd;		// ����Ƿ��ڿؼ���

	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	tstring				m_strText;			// �ı�
	tstring				m_strFont;			// ����
	INT					m_nFontWidth;		// ������
	INT					m_nFontHeight;		// ����߶�
	INT					m_nFontWeight;		// �����߿�
	tagRect				m_rcText;			// ��������
	DWORD				m_dwTextColor;		// �ı���ɫ
	DWORD				m_dwShadeColor;		// ������Ӱ��ɫ
	BOOL				m_bNoAntiAliase;	// �����Ƿ�رշ�����
	BOOL				m_bTextWriteAlpha;	// �����Ƿ�дAlphaͨ��
	EGUITextAlign		m_eTextAlign;		// ���ֶ��뷽ʽ

	tstring				m_strPic;			// ͼƬ�ļ���
	tagRect				m_rcPic;			// ͼƬʹ������(ͼƬ����)
	DWORD				m_dwPicColor;		// ͼƬ������ɫ
	EGUIAlphaWrite		m_eAlphaWrite;		// Alphaͨ��д��ʽ

	tstring				m_strTips;			// ������ʾ

};





}	// namespace vEngine {
