// PicProgressCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PicProgress.h"


// CPicProgress

IMPLEMENT_DYNAMIC(CPicProgress, CStatic)
CPicProgress::CPicProgress()
{
	m_nLValue = 1;
	m_nHValue = 100;
	m_nPos = 0;
	m_pDCBack = new CDC;
	m_pDCFore = new CDC;
}

CPicProgress::~CPicProgress()
{
	delete m_pDCBack;
	delete m_pDCFore;
}



BEGIN_MESSAGE_MAP(CPicProgress, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPicProgress ��Ϣ�������

//-----------------------------------------------------------------------------
//����ǰ��ɫ
//-----------------------------------------------------------------------------
void CPicProgress::SetForceBmp(LPCTSTR szPicPath)
{
	m_bmpFore.Attach((HBITMAP)::LoadImage(AfxGetInstanceHandle(), szPicPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION));
}
//-----------------------------------------------------------------------------
//���ñ���ɫ
//-----------------------------------------------------------------------------
void CPicProgress::SetBackBmp(LPCTSTR szPicPath)
{
	m_bmpBack.Attach((HBITMAP)::LoadImage(AfxGetInstanceHandle(), szPicPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION));
}
//-----------------------------------------------------------------------------
//����ֵ�Ĳ�����Χ
//-----------------------------------------------------------------------------
void CPicProgress::SetRange(int nLower, int nUpper)
{
	//���߲������
	ASSERT( nLower != nUpper );

	if(nUpper > nLower)
	{
		m_nLValue = nLower;
		m_nHValue = nUpper;
	}
	else
	{
		m_nLValue = nUpper;
		m_nHValue = nLower;
	}
}
//-----------------------------------------------------------------------------
void CPicProgress::GetRange(int &lower, int &upper)
{
	lower = m_nLValue;
	upper = m_nHValue;
}
//-----------------------------------------------------------------------------
//���õ�ǰֵ
//-----------------------------------------------------------------------------
BOOL CPicProgress::SetPos(int nPos)
{
	if( nPos > m_nHValue || nPos < m_nLValue )
		return FALSE;

	m_nPos = nPos;
	InvalidateRect(FALSE, FALSE);
	return TRUE;
}
//-----------------------------------------------------------------------------
//ƫ��һ�β�ˢ��
//-----------------------------------------------------------------------------
int CPicProgress::OffsetPos(int nPos)
{
	CRect tempRect = m_rect;
	m_nPos += nPos;

	//ֻ�ػ�ƫ�Ƶ�һ��
	if( nPos == 0 )
		return FALSE;

	if( nPos > 0 )
	{
		if( m_nPos > m_nHValue )
			m_nPos = m_nHValue;

		tempRect.left = (LONG)(m_fLength*m_rect.Width());
		tempRect.right +=  (LONG)((float)m_nPos/(m_nHValue-m_nLValue)*m_rect.Width());
	}
	else
	{
		if( m_nPos < m_nLValue )
			m_nPos = m_nLValue;

		tempRect.right = (LONG)(m_fLength*m_rect.Width());
		tempRect.left += (LONG)((float)m_nPos/(m_nHValue-m_nLValue)*m_rect.Width());
	}

	//ˢ�±仯����
	InvalidateRect(&tempRect);
	return TRUE;

}
//-----------------------------------------------------------------------------
//������Ⱦ
//-----------------------------------------------------------------------------
void CPicProgress::OnPaint() 
{
	m_fLength = (float)m_nPos/(m_nHValue-m_nLValue);
	CPaintDC dc(this);
	GetClientRect(&m_rect);
	m_pDCBack->CreateCompatibleDC(&dc);
	m_pDCFore->CreateCompatibleDC(&dc);


	m_bmpBack.GetBitmap(&m_bmback);
	m_bmpFore.GetBitmap(&m_bmfore);

	//������ɫ
	m_pDCBack->SelectObject(m_bmpBack);
	dc.StretchBlt(0, 0, m_rect.Width(), m_rect.Height(), m_pDCBack, 0, 0, 
		m_bmback.bmWidth, m_bmback.bmHeight, SRCCOPY);

	//��ǰ��ɫ
	(CBitmap*)m_pDCFore->SelectObject(m_bmpFore);
	dc.StretchBlt(3, 3, (LONG)((m_rect.Width()-6)*m_fLength), m_rect.Height()-5, m_pDCFore, 0, 0, 
		m_bmfore.bmWidth, m_bmfore.bmHeight, SRCCOPY);

	m_pDCBack->DeleteDC();
	m_pDCFore->DeleteDC();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CPicProgress::GetPos()
{
	return m_nPos;
}
//-----------------------------------------------------------------------------


