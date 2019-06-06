//-----------------------------------------------------------------------------
//!\file gui_render.cpp
//!\author Lyp
//!
//!\date 2004-12-09
//! last 2004-12-09
//!
//!\brief ����ϵͳ������Ⱦ�ӿ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_render.h"

#include "..\image\image_obj.h"
#include "..\render\render.h"
#include "..\system\window_wrap.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL IGUIRenderNative::Init(IRender* pRender)
{
	m_pCurrentDest = NULL;

	m_pRender = pRender;
	if( NULL == m_pRender )
		return FALSE;

	return TRUE;
}



//-----------------------------------------------------------------------------
// ����ͼƬ��ʧ�ܷ���0,�ɹ����ؾ��(���strImage.empty()�򷵻�NULL)
// ����޷���ȡָ��ͼƬ�򷵻ؿ�������ΪRenderTarget�Ŀ�����
//-----------------------------------------------------------------------------
tagGUIImage* IGUIRenderNative::CreateImage(const tstring str, const tagRect& rc)
{
	if( str.empty() ) 
		return NULL;
	
	tagGUIImage* pImage = new tagGUIImage;
	if( !pImage )
		return NULL;

	pImage->dwID = m_pUtil->Crc32(str.c_str());
	pImage->rc = rc;
	pImage->ptSize.x = rc.right - rc.left;
	pImage->ptSize.y = rc.bottom - rc.top;

	tagGUIImageHandle* pHandle = m_mapImageHandle.Peek(pImage->dwID);
	if( P_VALID(pHandle) )	// ͼƬ��������
	{
		pHandle->nRefCount++;
		pImage->dwHandle = pHandle->dwHandle;
		if( rc.IsEmpty() )
		{
			pImage->ptSize = pHandle->ptSize;
			pImage->rc = pHandle->ptSize;
		}
		return pImage;
	}

	if( GT_INVALID == m_pImageObj->Load(str.c_str()) )
	{
		// TODO(LxC): We need fix here. Most of the time the image is in dds format,
		// while in the config file it is defined as bmp file.  How to load the dds file?
		pImage->dwHandle = m_pRender->CreateSurface(pImage->ptSize.x, pImage->ptSize.y, TRUE, TRUE);
		if( !P_VALID(pImage->dwHandle) )
		{
			SAFE_DEL(pImage);
			return NULL;
		}

		return pImage;
	}

	if( m_pImageObj->GetBitCount() == 32 )
		pImage->dwHandle = m_pRender->CreateSurface(m_pImageObj->GetWidth(), m_pImageObj->GetHeight(), TRUE, FALSE);
	else
		pImage->dwHandle = m_pRender->CreateSurface(m_pImageObj->GetWidth(), m_pImageObj->GetHeight(), FALSE, FALSE);
	if( !P_VALID(pImage->dwHandle) )
	{
		SAFE_DEL(pImage);
		return NULL;
	}

	pHandle = new tagGUIImageHandle;
	pHandle->dwHandle = pImage->dwHandle;
	pHandle->nRefCount = 1;
	pHandle->ptSize.Set(m_pImageObj->GetWidth(), m_pImageObj->GetHeight());
	m_mapImageHandle.Add(pImage->dwID, pHandle);

	if( pImage->rc.IsEmpty() )
	{
		pImage->rc.SetLTWH(0, 0, m_pImageObj->GetWidth(), m_pImageObj->GetHeight());
		pImage->ptSize.Set(m_pImageObj->GetWidth(), m_pImageObj->GetHeight());
	}

	DWORD dwDC = m_pRender->GetDC(pImage->dwHandle);
	if( GT_INVALID != dwDC )
	{
		m_pImageObj->DrawToHdc((HDC)dwDC);
		m_pRender->ReleaseDC(pImage->dwHandle, dwDC);
	}

	return pImage;
}


//-----------------------------------------------------------------------------
// destroy image
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::DestroyImage(tagGUIImage* pImage)
{
	if( !P_VALID(pImage) )
		return;

	tagGUIImageHandle* pHandle = m_mapImageHandle.Peek(pImage->dwID);
	if( P_VALID(pHandle) )
	{
		pHandle->nRefCount--;
		if( pHandle->nRefCount <= 0 )
		{
			if( P_VALID(pHandle->dwHandle) )
				m_pRender->ReleaseSurface(pHandle->dwHandle);

			m_mapImageHandle.Erase(pImage->dwID);
			SAFE_DEL(pHandle);
		}
	}

	SAFE_DEL(pImage);
}


//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
tagGUIFont* IGUIRenderNative::CreateFont(const tstring str, INT nWidth, INT nHeight, INT nWeight
										 , BOOL bAntiAliase)
{
	if( str.empty() )
		return NULL;

	tstringstream ss;
	ss << str << nWidth << nHeight << nWeight << bAntiAliase;
	DWORD dwID = m_pUtil->Crc32(ss.str().c_str());

	tagGUIFontEx* pFont = m_mapFont.Peek(dwID);
	if( !P_VALID(pFont) )
	{
		DWORD dwQuality = bAntiAliase ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;//CLEARTYPE_QUALITY;// 
		HFONT hFont = ::CreateFont(nHeight,nWidth,0,0,nWeight,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, dwQuality, DEFAULT_PITCH, str.c_str());

		pFont = new tagGUIFontEx;
		pFont->dwID = dwID;
		pFont->dwHandle = (DWORD)hFont;
		pFont->nRefCount = 1;
		m_mapFont.Add(dwID, pFont);
		return pFont;
	}
	else
	{
		pFont->nRefCount++;
		return pFont;
	}

	return 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::DestroyFont(tagGUIFont* pFont)
{
	if( !P_VALID(pFont) )
		return;

	tagGUIFontEx* pFontEx = (tagGUIFontEx*)pFont;
	pFontEx->nRefCount--;
	if( pFontEx->nRefCount <= 0 )
	{
		if( P_VALID(pFontEx->dwHandle) )
			DeleteObject((HGDIOBJ)pFontEx->dwHandle);
		
		m_mapFont.Erase(pFontEx->dwID);
		SAFE_DEL(pFontEx);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
tagGUIFont* IGUIRenderNative::CloneFont(tagGUIFont* pFont)
{
	if( !P_VALID(pFont) )
		return NULL;

	tagGUIFontEx* pFontEx = (tagGUIFontEx*)pFont;
	pFontEx->nRefCount++;
	return pFontEx;
}


//-----------------------------------------------------------------------------
// ���!P_VALID(pDest->dwHandle)����Ⱦ����̨����, ���!P_VALID(pSrc)��ɫ��
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::Draw(const tagGUIImage* pDest, const tagGUIImage* pSrc, 
							DWORD dwColor, EGUIAlphaWrite eAlphaWrite, FLOAT z)
{
	if( P_VALID(pSrc) )
	{
		if( !P_VALID(pDest->dwHandle) )	// ������̨����
			m_pRender->Draw(pDest->dwHandle, pSrc->dwHandle, &pDest->rc, &pSrc->rc, dwColor);
		else
		{
			tagRect rc = pDest->rc;
			rc = rc - pDest->ptOffset;
			m_pRender->Draw(pDest->dwHandle, pSrc->dwHandle, &rc, &pSrc->rc, dwColor);
		}
	}
	else
	{
		if( !(dwColor & 0xff000000) )	// ȫ͸�����û�
			return;

		if( !P_VALID(pDest->dwHandle) )	// ������̨����
			m_pRender->Clear(pDest->dwHandle, &pDest->rc, dwColor);
		else
		{
			tagRect rc = pDest->rc;
			rc = rc - pDest->ptOffset;
			m_pRender->Clear(pDest->dwHandle, &rc, dwColor);
		}
	}
}



//-----------------------------------------------------------------------------
// draw text
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::Text(const tagGUIImage* pDest, const tstring& szText, 
							tagGUIFont* pFont, DWORD dwColor, DWORD dwShadeColor,
							EGUITextAlign eAlign, BOOL bWriteAlpha)
{
	if( szText.empty() )
		return;

	HDC hDC = (HDC)m_pRender->GetDC(pDest->dwHandle);
	if( GT_VALID(hDC) )
	{
		// ����
		HGDIOBJ hFont = NULL;
		if( P_VALID(pFont) )
			hFont = SelectObject(hDC, (HGDIOBJ)pFont->dwHandle);

		SetBkMode( hDC, TRANSPARENT );
		DWORD dwTextColor = m_pRender->ARGB2GDI(dwColor);
		DWORD dwTextShadeColor = m_pRender->ARGB2GDI(dwShadeColor);

		tagRect rc = pDest->rc;
		if( P_VALID(pDest->dwHandle) )	// ������̨����
			rc = rc - pDest->ptOffset;

		UINT uFormat = DT_LEFT|DT_TOP|DT_WORDBREAK;	// ��ʽ
		INT nIncX = 1, nIncY = 1;
		switch( eAlign )
		{
		case EGUITA_CenterTop:		uFormat = DT_CENTER|DT_TOP|DT_WORDBREAK; nIncX = 2; break;	
		case EGUITA_RightTop:		uFormat = DT_RIGHT|DT_TOP|DT_WORDBREAK; break;	
		case EGUITA_LeftCenter:		uFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE; nIncY = 2; break;	
		case EGUITA_Center:			uFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE; nIncX = 2; nIncY = 2; break;		
		case EGUITA_RightCenter:	uFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE; nIncY = 2; break;	
		case EGUITA_LeftBottom:		uFormat = DT_LEFT|DT_BOTTOM|DT_SINGLELINE; break;	
		case EGUITA_CenterBottom:	uFormat = DT_CENTER|DT_BOTTOM|DT_SINGLELINE; nIncX = 2; break;
		case EGUITA_RightBottom:	uFormat = DT_RIGHT|DT_BOTTOM|DT_SINGLELINE; break;
		default: break;
		}


		if( dwShadeColor )
		{
			tagRect	rcShade;
			SetTextColor( hDC, dwTextShadeColor );

			if( (dwShadeColor & 0xff000000) == 0 )
			{
				rcShade = rc;
				rcShade.left+=nIncX;rcShade.top+=nIncY;
				::DrawText(hDC, szText.c_str(), -1, (LPRECT)&rcShade, uFormat);
			}
			else
			{
				rcShade = rc;
				rcShade.left-=nIncX;
				::DrawText(hDC, szText.c_str(), -1, (LPRECT)&rcShade, uFormat);
				rcShade = rc;
				rcShade.left+=nIncX;
				::DrawText(hDC, szText.c_str(), -1, (LPRECT)&rcShade, uFormat);
				rcShade = rc;
				rcShade.top-=nIncY;;
				::DrawText(hDC, szText.c_str(), -1, (LPRECT)&rcShade, uFormat);
				rcShade = rc;
				rcShade.top+=nIncY;
				::DrawText(hDC, szText.c_str(), -1, (LPRECT)&rcShade, uFormat);
			}
		}

		SetTextColor( hDC, dwTextColor );
		::DrawText(hDC, szText.c_str(), -1, (LPRECT)&rc, uFormat);
		if( P_VALID(hFont) )
			SelectObject(hDC, hFont);

		m_pRender->ReleaseDC(pDest->dwHandle, (DWORD)hDC);
	}
}


//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::Line(const tagGUIImage* pDest, DWORD dwColor, BOOL bBox)
{
	HDC hDC = (HDC)(m_pRender->GetDC(pDest->dwHandle));
	if( hDC == HDC(-1) )
		return;

	dwColor = m_pRender->ARGB2GDI(dwColor);
	COLORREF pc = (COLORREF)dwColor;	// pen color
	HPEN pen = CreatePen(PS_SOLID, 1, pc);
	HGDIOBJ origin_obj = SelectObject(hDC, pen);

	tagRect rc = pDest->rc;
	if( P_VALID(pDest->dwHandle) )	// ������̨����
		rc = rc - pDest->ptOffset;

	::MoveToEx(hDC, rc.left, rc.top, NULL);
	if( bBox )
	{
		::LineTo(hDC, rc.right, rc.top);
		::LineTo(hDC, rc.right, rc.bottom);
		::LineTo(hDC, rc.left, rc.bottom);
		::LineTo(hDC, rc.left, rc.top);
	}
	else
	{
		::LineTo(hDC, rc.right, rc.bottom);

	}

	SelectObject(hDC, origin_obj);
    DeleteObject(pen);
	m_pRender->ReleaseDC(pDest->dwHandle, (DWORD)hDC);
}



//-----------------------------------------------------------------------------
/// ������client����ת��Ϊ����view����(view����Ϊ�߼�����)
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::ClientToView(tagPoint& pt)
{
	m_pRender->ClientToView(pt);
}


//-----------------------------------------------------------------------------
// ˢ�´�������
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::UpdateWindowPos()
{
	m_pRender->UpdateWindowPos();
}

//-----------------------------------------------------------------------------
// ˢ����Ļ
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::Present()
{
	m_pRender->Render();
}


//-----------------------------------------------------------------------------
// �õ��ı���С
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::GetTextSize(const LPCTSTR szText, tagGUIFont* pFont, tagPoint& ptSize)
{
	HDC hDC = (HDC)m_pRender->GetDC(0);
	if((DWORD)hDC!=GT_INVALID)
	{
		// ����
		HGDIOBJ hFont = NULL;
		if( P_VALID(pFont) )
			hFont = SelectObject(hDC, (HGDIOBJ)pFont->dwHandle);

		GetTextExtentPoint32(hDC, szText, lstrlen(szText), (LPSIZE)&ptSize);
		if( P_VALID(hFont) )
			SelectObject(hDC, hFont);

		m_pRender->ReleaseDC(0, (DWORD)hDC);
	}
}


//-----------------------------------------------------------------------------
// ֱ�Ӵ����µ�Render Target
//-----------------------------------------------------------------------------
DWORD IGUIRenderNative::CreateRenderTarget(const tagRect& rc)
{
	LONG x = rc.right - rc.left;
	LONG y = rc.bottom - rc.top;
	DWORD dwHandle = m_pRender->CreateSurface(x, y, TRUE, TRUE);
	if( !P_VALID(dwHandle) )
		return GT_INVALID;

	// ���render target
	m_pRender->Clear(dwHandle, NULL, 0);

	return dwHandle;
}


//-----------------------------------------------------------------------------
// ֱ���ͷ�Render Target
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::ReleaseRenderTarget(DWORD dwHandle)
{
	if( P_VALID(dwHandle) )
		m_pRender->ReleaseSurface(dwHandle);
}


//-----------------------------------------------------------------------------
// ֱ���ͷ�Render Target
//-----------------------------------------------------------------------------
VOID IGUIRenderNative::ClearRenderTarget()
{
	if( P_VALID(m_pCurrentDest) )
		m_pRender->Clear(m_pCurrentDest->dwHandle, &m_pCurrentDest->rc, 0);
}


}	// namespace vEngine {
