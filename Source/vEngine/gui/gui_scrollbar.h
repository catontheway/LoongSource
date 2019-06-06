//-----------------------------------------------------------------------------
//!\file gui_scrollbar.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-04-28
//!
//!\brief ����ϵͳ scroll bar �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳscrollbar�ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIScrollBar : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent);
	virtual VOID SendEvent(tagGUIEvent* pEvent);

	virtual VOID AddChild(GUIWnd* pChild);	// ����Ӵ���
	virtual VOID DelChild(GUIWnd* pChild);	// ɾ���Ӵ���

	VOID SetValue(INT nTotal, INT nVisible, INT nCurrent)
	{ 
		m_nTotalValue = nTotal;
		m_nVisibleValue = nVisible;
		m_nCurrentValue = nCurrent;
		CalStick();
	}
	
	VOID SetValue(INT nCurrent) { m_nCurrentValue = nCurrent; CalStick(); }
	INT GetValue() { return m_nCurrentValue; }

	GUIScrollBar();

protected:

	GUIWnd*		m_pUpButton;
	GUIWnd*		m_pDownButton;
	INT			m_nStickStartPos;			// ��������ʼλ��
	INT			m_nStickEndPos;				// ��������ʼλ��+������������

	tagGUIImage*	m_pTopPic;				// �ϲ�ͼƬhandle
	tagGUIImage*	m_pMidPic;				// �в�ͼƬhandle
	tagGUIImage*	m_pBelowPic;			// �²�ͼƬhandle

	VOID		CalStick();					// ����������ĳ�����ʼλ�õ�

	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	BOOL			m_bHorizon;				// �Ƿ�ˮƽ
	INT				m_nTotalValue;			// ����
	INT				m_nVisibleValue;		// ����ʾ����
	INT				m_nCurrentValue;		// ��ǰ��

	tstring			m_strTopPic;			// �ϲ�ͼƬ�ļ���
	tagRect			m_rcTopPic;				// �ϲ�ͼƬʹ������(ͼƬ����)
	DWORD			m_dwTopPicColor;		// �ϲ�ͼƬ������ɫ
	tstring			m_strMidPic;			// �в�ͼƬ�ļ���
	tagRect			m_rcMidPic;				// �в�ͼƬʹ������(ͼƬ����)
	DWORD			m_dwMidPicColor;		// �в�ͼƬ������ɫ
	tstring			m_strBelowPic;			// �²�ͼƬ�ļ���
	tagRect			m_rcBelowPic;			// �²�ͼƬʹ������(ͼƬ����)
	DWORD			m_dwBelowPicColor;		// �²�ͼƬ������ɫ

	DWORD 			m_dwTopColor;			// �ϲ�����ɫ
	DWORD			m_dwBottomColor;		// �²�����ɫ


};

}	// namespace vEngine {
