//-----------------------------------------------------------------------------
//!\file gui_window.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-06-15
//!
//!\brief ����ϵͳ window �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳ window �ؼ�
//-----------------------------------------------------------------------------
class VENGINE_API GUIWindow : public GUIWnd
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual VOID Update();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	
	virtual BOOL PointInWnd(tagPoint& pt, bool bCheckChild=true); // ָ�����Ƿ��ڱ�������
	virtual BOOL Move(tagPoint& ptClient); // �ƶ��ؼ�,����������
	virtual VOID RefreshViewCoord(); // ˢ���߼�����(view)

	virtual VOID AddChild(GUIWnd* pChild);
	virtual VOID DelChild(GUIWnd* pChild);
	virtual VOID SetInvisible(BOOL b);

	VOID SetZ(FLOAT z) { m_fZ = z; }

	VOID ReCalScroll();	// ���¼���Scroll����
	GUIButton* GetCloseButton() { return (GUIButton*)this->GetChild(m_strCloseButton.c_str()); }
	GUIWindow();

protected:
	tagGUIImage*			m_pNewCanvas;

	GUIScrollBar*			m_pScrollBar;
	INT						m_nMinY;
	INT						m_nMaxY;
	INT						m_nCurrentY;
	FLOAT					m_fZ;

	tagPoint				m_ptCurrentSize;		// ��ǰ��С
	tagPoint				m_ptCurrentView;		// ��ǰλ��
	FLOAT					m_fCurrentSizeRatio;	// ��ǰ��С����
	INT						m_nPopProgress;			// �Ƿ��ڷŴ�״̬ 1�Ŵ� 2��С 0����
	DWORD					m_dwPopStartTime;


	VOID ScrollChild(INT nY);	// �����ӿؼ�


	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	tstring			m_strCloseButton;		// �رհ�ť�ؼ���
	BOOL			m_bRealDestroty;		// �Ƿ������ر�
	BOOL			m_bNotUseRenderTarget;	// �Ƿ�ʹ����Ⱦ����
	BOOL			m_bScroll;				// �Ƿ�֧�ֹ�������
	tstring			m_strPopupSound;		// �Ƿ��������Ч
	BOOL			m_bNoPopupEffect;		// �Ƿ�رյ���Ч��

};


}	// namespace vEngine {
