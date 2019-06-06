//-----------------------------------------------------------------------------
// File: picbutton.h
// Time: 2006-6-18
// Auth: tidy
// Desc: picture button for mfc ui

// Copyright (c) 2006 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------

#pragma once


// CPicButtion

class CPicButtion : public CButton
{
	DECLARE_DYNAMIC(CPicButtion)

public:
	CPicButtion();
	virtual ~CPicButtion();

public:
	VOID SetSelBitmap(LPCTSTR szPicPath);		//����ѡ��״̬�µ�λͼ
	VOID SetHoverBitmap(LPCTSTR szPicPath);
	VOID SetNormalBitmap(LPCTSTR szPicPath,bool autosize=false);
	VOID SetDisableBitmap(LPCTSTR szPicPath);

	//��������
	VOID SetTextRect(RECT& rect);
	//���ִ�С
	VOID SetTextSize(INT nSize);
	//�����Ƿ����
	VOID SetTextLeft(BOOL bTextLeft){ m_bTextLeft = bTextLeft; }

	//����������ɫ
	VOID SetTextColor(DWORD dwTextColor);
	//������ߵ���ɫ
	VOID SetTextEfColor(DWORD dwTextEfColor) { m_dwTextEfColor = dwTextEfColor; }
	//��꾭��ʱ������ߵ���ɫ
	VOID SetTextHoverEfColor(DWORD dwTextEfColor) { m_dwTextHoverEfColor = dwTextEfColor; }
	//ѡ��״̬������ߵ���ɫ
	VOID SetTextSelEfColor(DWORD dwTextEfColor) { m_dwTextSelEfColor = dwTextEfColor; }
	//��ť��Чʱ��������ɫ
	VOID SetDisableTextColor(DWORD dwTextColor){  m_dwDisableTextColor = dwTextColor;}
	//��ߴ���
	VOID RenderText(CDC* pDC, DWORD dwTextColor, CString &strText);

private:
	BOOL		m_bHover;			//����Ƿ��ڰ�ť��	
	BOOL		m_bTracking;

	CBitmap*	m_pBitmapSel;		//ѡ��״̬
	CBitmap*	m_pBitmapHover;		//��꾭��״̬
	CBitmap*	m_pBitmapNormal;	//����״̬
	CBitmap*	m_pBitmapDisable;	//������״̬

	DWORD		m_dwTextColor;
	DWORD		m_dwTextEfColor;
	DWORD		m_dwTextHoverEfColor;
	DWORD		m_dwTextSelEfColor;

	DWORD		m_dwDisableTextColor;
	RECT		m_RectText;
	BOOL		m_bTextLeft;

	CFont*		m_pFont;

	HBITMAP		m_dwHandleBitmap[4];
	bool		m_bAutoSize;		//�Ƿ��Զ����ƴ�С
	bool		m_bIsCreate;		//�Ƿ��Ѿ�����
public:
	char		Stretch;
	int			m_dwWidth;			//�ؼ����
	int			m_dwHeight;			//�ؼ��߶�
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);  //�Ի�
	virtual void PreSubclassWindow();	//����֮ǰ���г�ʼ��
	virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	


protected:

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);	
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return TRUE; }

	DECLARE_MESSAGE_MAP()
};


