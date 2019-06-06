//-----------------------------------------------------------------------------
//!\file gui_button.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2004-12-10
//!
//!\brief ����ϵͳbutton�ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳbutton�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIButton : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	// ���ð�ť�Ƿ���Ч
	virtual VOID SetEnable(bool b) { m_bDisable = !b; }
	virtual BOOL IsEnable() { return !m_bDisable; }

	// �õ���ť״̬
	virtual EGUIButtonState	GetState() { return m_eState; }

	// ������ɫ
	virtual VOID SetMoveTextColor(DWORD dwColor) { m_dwMoveTextColor = dwColor; }
	virtual VOID SetPushTextColor(DWORD dwColor) { m_dwPushTextColor = dwColor; }

	GUIButton();

protected:

	tagGUIImage*	m_pPushPic;				// ����ͼƬhandle
	tagGUIImage*	m_pMovePic;				// ����ͼƬhandle
	tagGUIImage*	m_pDisablePic;			// ��ЧͼƬhandle
	EGUIButtonState	m_eState;				// ��ť״̬
	bool			m_bHaveMouseKeyDown;	// �Ƿ��������¹����

	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	BOOL			m_bDisable;				// �Ƿ���Ч
	tstring			m_strPushPicFileName;	// ����ͼƬ�ļ���
	tagRect			m_rcPushPic;			// ����ͼƬʹ������(ͼƬ����)
	DWORD			m_dwPushPicColor;		// ����ͼƬ������ɫ
	DWORD			m_dwPushTextColor;		// �����ı���ɫ
	DWORD			m_dwPushShadeColor;		// ����������Ӱ��ɫ

	tstring			m_strMovePicFileName;	// ����ͼƬ�ļ���
	tagRect			m_rcMovePic;			// ����ͼƬʹ������(ͼƬ����)
	DWORD			m_dwMovePicColor;		// ����ͼƬ������ɫ
	DWORD			m_dwMoveTextColor;		// �����ı���ɫ
	DWORD			m_dwMoveShadeColor;		// ����������Ӱ��ɫ

	tstring			m_strDisablePicFileName;// ��ЧͼƬ�ļ���
	tagRect			m_rcDisablePic;			// ��ЧͼƬʹ������(ͼƬ����)
	DWORD			m_dwDisablePicColor;	// ��ЧͼƬ������ɫ
	DWORD			m_dwDisableTextColor;	// ��Ч�ı���ɫ
	DWORD			m_dwDisableShadeColor;	// ��Ч������Ӱ��ɫ

	DWORD 			m_dwTopColor;			// �ϲ�����ɫ
	DWORD			m_dwBottomColor;		// �²�����ɫ

};





}	// namespace vEngine {
