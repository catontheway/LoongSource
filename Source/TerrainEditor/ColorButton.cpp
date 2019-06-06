// ColorButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColorButton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)
CColorButton::CColorButton()
{
	m_color=0;
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CColorButton::SetColor(COLORREF color)
{
	m_color=color;
	Invalidate();
}

// CColorButton ��Ϣ�������

void CColorButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CButton::OnPaint()
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc,m_color);
}

COLORREF CColorButton::GetColor()
{
	return m_color;
}