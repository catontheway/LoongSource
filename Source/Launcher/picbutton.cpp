//-----------------------------------------------------------------------------
// File: picbutton.cpp
// Time: 2006-6-18
// Auth: tidy
// Desc: picture button for mfc ui

// Copyright (c) 2006 Softstar Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "PicButton.h"


// CPicButtion
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPicButtion, CButton)
CPicButtion::CPicButtion()
{
	int i;

	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bTextLeft = FALSE;
	m_bAutoSize = false;
	m_bIsCreate = false;

	m_dwDisableTextColor = -1;
	m_dwTextEfColor = -1;
	m_dwTextHoverEfColor = -1;
	m_dwTextSelEfColor = -1;


	m_pBitmapSel = NULL;	
	m_pBitmapHover = NULL;
	m_pBitmapNormal = NULL;
	m_pBitmapDisable = NULL;


	ZeroMemory(&m_RectText, sizeof(RECT));

	m_dwTextColor = RGB(255,255,255);

	for(i=0;i<4;i++)
	{
		m_dwHandleBitmap[i]=0;
	}
	m_pFont = new CFont;

	int fsize=12;
	m_pFont->CreateFont(fsize, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		g_StrTable[_T("FontName")]); // lpszFac		//Arial
}
//-----------------------------------------------------------------------------
CPicButtion::~CPicButtion()
{
	if(m_pBitmapSel!=NULL)
	{
		m_pBitmapSel->Detach();
		m_pBitmapSel->DeleteObject();
	}
	if(m_pBitmapHover!=NULL)
	{
		m_pBitmapHover->Detach();
		m_pBitmapHover->DeleteObject();
	}
	if(m_pBitmapNormal!=NULL)
	{
		m_pBitmapNormal->Detach();
		m_pBitmapNormal->DeleteObject();
	}
	if(m_pBitmapDisable!=NULL)
	{
		m_pBitmapDisable->Detach();
		m_pBitmapDisable->DeleteObject();
	}
	if(m_pFont!=NULL)
	{
		m_pFont->DeleteObject();
	}
	int i;

	for(i=0;i<4;i++)
	{
		if(m_dwHandleBitmap[i]!=NULL)
		{
			::DeleteObject(m_dwHandleBitmap[i]);
		}
	}
	delete m_pFont;
}

//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPicButtion, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER,OnMouseHover)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------

// CPicButtion ��Ϣ�������

//-----------------------------------------------------------------------------
//����֮ǰ���г�ʼ��
//-----------------------------------------------------------------------------
void CPicButtion::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);	//�����Ի�����

	CButton::PreSubclassWindow();

}
//-----------------------------------------------------------------------------
//���ش���
//-----------------------------------------------------------------------------
BOOL CPicButtion::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_RectText.left = 0;
	m_RectText.top = 0;
	m_RectText.right = rect.right - rect.left;
	m_RectText.bottom = rect.bottom - rect.top;

	RECT r1;
	r1=rect;
	BOOL bRet;
	if(m_bAutoSize)
	{
		r1.right = r1.left+m_dwWidth;
		r1.bottom =r1.top+m_dwHeight;
	}
	else
	{
		m_dwWidth=rect.right-rect.left;
		m_dwHeight=rect.bottom-rect.top;
	}

	bRet =  CButton::Create(lpszCaption, dwStyle, r1, pParentWnd, nID);

	SetFont(m_pFont, TRUE);
	return bRet;
}

//-----------------------------------------------------------------------------
//����ѡ��״̬�µ�λͼ
//-----------------------------------------------------------------------------
VOID CPicButtion::SetSelBitmap(LPCTSTR szPicPath)
{
	if(m_pBitmapSel!=NULL)
	{
		::DeleteObject(m_dwHandleBitmap[3]);
		m_pBitmapSel->Detach();
		m_pBitmapSel->DeleteObject();
	}
	m_pBitmapSel = new CBitmap;
	m_dwHandleBitmap[3]=(HBITMAP)::LoadImage(AfxGetInstanceHandle(), szPicPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_pBitmapSel->Attach(m_dwHandleBitmap[3]); 
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
VOID CPicButtion::SetHoverBitmap(LPCTSTR szPicPath)
{
	if(m_pBitmapHover!=NULL)
	{
		::DeleteObject(m_dwHandleBitmap[2]);
		m_pBitmapHover->Detach();
		m_pBitmapHover->DeleteObject();
	}
	m_pBitmapHover = new CBitmap;
	m_dwHandleBitmap[2]=(HBITMAP)::LoadImage(AfxGetInstanceHandle(), szPicPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_pBitmapHover->Attach(m_dwHandleBitmap[2]); 
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
VOID CPicButtion::SetNormalBitmap(LPCTSTR szPicPath,bool autosize)
{
	if(m_pBitmapNormal!=NULL)
	{
		::DeleteObject(m_dwHandleBitmap[0]);
		m_pBitmapNormal->Detach();
		m_pBitmapNormal->DeleteObject();
	}
	m_pBitmapNormal = new CBitmap;
	m_dwHandleBitmap[0]=(HBITMAP)::LoadImage(AfxGetInstanceHandle(), szPicPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_pBitmapNormal->Attach(m_dwHandleBitmap[0]); 
	m_bAutoSize=autosize;
	if(autosize)
	{
		CFile f;
		f.Open(szPicPath,CFile::modeRead);
		int x[2];
		f.Seek(18,CFile::begin);
		f.Read(x,8);
		f.Close();
		m_dwWidth=x[0];
		m_dwHeight=x[1];
	}
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
VOID CPicButtion::SetDisableBitmap(LPCTSTR szPicPath)
{
	if(m_pBitmapDisable!=NULL)
	{
		::DeleteObject(m_dwHandleBitmap[1]);
		m_pBitmapDisable->Detach();
		m_pBitmapDisable->DeleteObject();
	}
	m_pBitmapDisable = new CBitmap;
	m_dwHandleBitmap[1]=(HBITMAP)::LoadImage(AfxGetInstanceHandle(), szPicPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_pBitmapDisable->Attach(m_dwHandleBitmap[1]); 
}
//-----------------------------------------------------------------------------
//�������ַ�Χ
//-----------------------------------------------------------------------------
VOID CPicButtion::SetTextRect(RECT& rect)
{
	memcpy(&m_RectText, &rect, sizeof(RECT));
}
//-----------------------------------------------------------------------------
//����������ɫ
//-----------------------------------------------------------------------------
VOID CPicButtion::SetTextColor(DWORD dwTextColor)
{
	m_dwTextColor = dwTextColor;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
VOID CPicButtion::SetTextSize(INT nSize)
{
	if(m_pFont!=NULL)
	{
		m_pFont->DeleteObject();
		delete m_pFont;
	}
	m_pFont = new CFont;

	m_pFont->CreateFont(nSize, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		g_StrTable[_T("FontName")]); // lpszFac 

	SetFont(m_pFont, TRUE);
}
//-----------------------------------------------------------------------------
//�Ի溯��
//-----------------------------------------------------------------------------
void CPicButtion::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��ȡ������DRAWITEMSTRUCT�ṹ�����ڻ��ư�ťʱ����ʹ�õ��豸������
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect	rtControl(lpDrawItemStruct->rcItem);

	//�������ݵ��豸������
	CDC* pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	//����ɶ���
	CBitmap * pOldBitmap;

	CString strText;
	GetWindowText(strText);

	pDC->SetBkMode(TRANSPARENT);

	//�жϰ�ť�Ƿ���ѡ��״̬������������ѡ��״̬�İ�ťλͼ���������ṩ��λͼ�У�ѡ��״̬�İ�ťͼƬ�ǵڶ���
	if( FALSE == this->IsWindowEnabled() )
	{
		//��ť�����õ�ʱ��
		pOldBitmap = pMemDC->SelectObject(m_pBitmapDisable);
		CDC TempDC;
		CBitmap MemBitmap;//����һ��λͼ����
		TempDC.CreateCompatibleDC(pDC);
		MemBitmap.CreateCompatibleBitmap(pDC,rtControl.Width(),rtControl.Height());
		TempDC.SelectObject(&MemBitmap);

		TempDC.BitBlt(0, 0, rtControl.Width(), rtControl.Height(), pMemDC, 0, 0, SRCCOPY);

		TempDC.SetBkMode(TRANSPARENT);
		TempDC.SelectObject(pDC->GetCurrentFont());
		if( m_dwDisableTextColor == -1 )
		{
			TempDC.SetTextColor(m_dwTextColor);
		}
		else
		{
			TempDC.SetTextColor(m_dwDisableTextColor);
		}

		if( FALSE == m_bTextLeft )
			TempDC.DrawText(strText, &m_RectText, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		else 
			TempDC.DrawText(strText, &m_RectText, DT_SINGLELINE|DT_VCENTER);

		pDC->BitBlt(0, 0, rtControl.Width(), rtControl.Height(), &TempDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		TempDC.DeleteDC();
	}
	else 
	{
		if(lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			//ѡ��״̬
			pOldBitmap = pMemDC->SelectObject(m_pBitmapSel);
			pDC->BitBlt(0, 0, rtControl.Width(), rtControl.Height(), pMemDC, 0, 0, SRCCOPY);

			if( m_dwTextSelEfColor != -1 )
			{
				RenderText(pDC, m_dwTextSelEfColor, strText);
			}
			else if( m_dwTextEfColor != -1 )
			{
				RenderText(pDC, m_dwTextEfColor, strText);
			}
			pDC->SetTextColor(m_dwTextColor);

		}
		else
		{   //�ж�����Ƿ��뿪�����ڰ�ť���棬�Ա������Ӧ��λͼ
			if(m_bHover)
			{
				//��꾭��
				pOldBitmap = pMemDC->SelectObject(m_pBitmapHover);
				pDC->BitBlt(0, 0, rtControl.Width(), rtControl.Height(), pMemDC, 0, 0, SRCCOPY);

				if( m_dwTextHoverEfColor != -1 )
				{
					RenderText(pDC, m_dwTextHoverEfColor, strText);
				}
				else if( m_dwTextEfColor != -1 )
				{
					RenderText(pDC, m_dwTextEfColor, strText);
				}

				//���������⴦��
				if( m_dwTextColor == RGB(255, 255, 255) && m_dwTextHoverEfColor != -1)
				{
					pDC->SetTextColor(RGB(0, 0, 0));
				}
				else
				{
					pDC->SetTextColor(m_dwTextColor);
				}
			}
			else
			{
				pOldBitmap = pMemDC->SelectObject(m_pBitmapNormal);
				pDC->BitBlt(0, 0, rtControl.Width(), rtControl.Height(), pMemDC, 0, 0, SRCCOPY);

				if( m_dwTextEfColor != -1 )
				{
					//������Ҫ���
					RenderText(pDC, m_dwTextEfColor, strText);
				}

				pDC->SetTextColor(m_dwTextColor);
			}    
		}

		if( FALSE == m_bTextLeft )
		{
			pDC->DrawText(strText, &m_RectText, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		}
		else 
		{
			pDC->DrawText(strText, &m_RectText, DT_SINGLELINE|DT_VCENTER);
		}
	}

	// clean up
	pMemDC->SelectObject(pOldBitmap);
	delete pMemDC;
}
//-----------------------------------------------------------------------------
//�������
//-----------------------------------------------------------------------------
VOID CPicButtion::RenderText(CDC* pDC, DWORD dwTextColor, CString &strText)
{
	pDC->SetTextColor(dwTextColor);

	//��Ҫ���
	RECT rect;

	//��
	rect.left =  m_RectText.left - 1;
	rect.right = m_RectText.right - 1;
	rect.top = m_RectText.top;
	rect.bottom = m_RectText.bottom;

	if( FALSE == m_bTextLeft )
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	else 
	{
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_VCENTER);
	}

	//��
	rect.left =  m_RectText.left + 1;
	rect.right = m_RectText.right + 1;
	rect.top = m_RectText.top;
	rect.bottom = m_RectText.bottom;

	if( FALSE == m_bTextLeft )
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	else 
	{
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_VCENTER);
	}

	//��
	rect.left =  m_RectText.left;
	rect.right = m_RectText.right;
	rect.top = m_RectText.top - 1;
	rect.bottom = m_RectText.bottom - 1;

	if( FALSE == m_bTextLeft )
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	else 
	{
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_VCENTER);
	}

	//��
	rect.left =  m_RectText.left;
	rect.right = m_RectText.right;
	rect.top = m_RectText.top + 1;
	rect.bottom = m_RectText.bottom + 1;

	if( FALSE == m_bTextLeft )
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	else 
	{
		pDC->DrawText(strText, &rect, DT_SINGLELINE|DT_VCENTER);
	}

}

//-----------------------------------------------------------------------------
//����ƶ�
//-----------------------------------------------------------------------------
void CPicButtion::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bTracking == FALSE )
	{
		m_bTracking = TRUE;

		InvalidateRect(NULL,FALSE);

		TRACKMOUSEEVENT	tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		tme.hwndTrack = m_hWnd;

		::TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}
//-----------------------------------------------------------------------------
//����ƿ�
//-----------------------------------------------------------------------------
LRESULT CPicButtion::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;

	//�ػ���ť
	Invalidate(TRUE);
	return 0;
}
//-----------------------------------------------------------------------------
//��꾭��
//-----------------------------------------------------------------------------
LRESULT CPicButtion::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover=TRUE;
	Invalidate(TRUE);
	return 0;
}
//-----------------------------------------------------------------------------



