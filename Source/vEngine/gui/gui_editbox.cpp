//-----------------------------------------------------------------------------
//!\file gui_editbox.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-12-31
//!
//!\brief ����ϵͳ edit box �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_editbox.h"
#include "gui_event_define.h"
#include "..\resource\resource.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
GUIEditBox::GUIEditBox()
{
	m_pVarMgr->Register(_T("ReadOnly"), &m_bReadOnly, _T("no yes"));
	m_pVarMgr->Register(_T("Password"), &m_bPassword, _T("no yes"));
	m_pVarMgr->Register(_T("Numerical"), &m_bNumerical, _T("no yes"));
	m_pVarMgr->Register(_T("CursorColor"), &m_dwCursorColor);
	m_pVarMgr->Register(_T("CursorTimer"), &m_dwCursorTimer);
	m_pVarMgr->Register(_T("MaxTextCount"), &m_nMaxTextCount);

	m_dwTextColor = 0xFF000000;
	m_dwCursorColor = 0xFF000000;
	m_dwCursorTimer = 200;
	m_nMaxTextCount = 65535;
	m_nMaxNumberValue = 2147483647;
	m_bEditable = TRUE;
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIEditBox::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);

	m_bShowCursor = false;		// ����ʾ���
	m_nSelStart = 0;			// ѡ��ʼ��
	m_nSelEnd = 0;				// ѡ�������
	m_nCursorPos = 0;			// ���λ��
	m_nDispStart = 0;			// ��ʾ�ĵ�һ���ַ�����ʵ�ʵĵڼ����ַ�
	m_nDispEnd = 0;				// ��ʾ�����һ���ַ�����ʵ�ʵĵڼ����ַ�

	this->CalCharWidth();
	return bResult;
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID GUIEditBox::Update()
{
	static DWORD dwLastTime = timeGetTime();
	// ���������ʾ����˸
	if( m_pSystem->GetActive() == this )
	{
		if( timeGetTime() - dwLastTime > m_dwCursorTimer )
		{
			m_bShowCursor = !m_bShowCursor;
			dwLastTime = timeGetTime();
			this->SetRefresh();
		}
	}

	GUIStatic::Update();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID GUIEditBox::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	// ��Ⱦ�����ַ���
	tstring strStar;
	m_pCanvas->rc = m_rcText + m_ptView;
	if( m_bPassword )
	{
		strStar.assign(m_strText.size(), _T('*'));
		m_pRender->Text(m_pCanvas, strStar.substr(m_nDispStart, m_nDispEnd - m_nDispStart), 
			m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
	}
	else
	{
		m_pRender->Text(m_pCanvas, m_strText.substr(m_nDispStart, m_nDispEnd - m_nDispStart), 
			m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
	}

	// ��Ⱦѡ���ַ�
	if( m_nSelStart != m_nSelEnd )
	{
		INT nStart = min(m_nSelStart, m_nSelEnd);
		INT nLen = abs(m_nSelEnd - m_nSelStart);
		if( nStart < m_nDispStart )
		{
			nLen -= m_nDispStart - nStart;
			nStart = m_nDispStart;
		}
		if( nStart + nLen >= m_nDispEnd )
			nLen = m_nDispEnd - nStart;

		// ���㷴ɫ��ʼ�ͽ�������
		m_pCanvas->rc.left = m_ptView.x + m_rcText.left;
		for(INT n=m_nDispStart; n<nStart; n++)
			m_pCanvas->rc.left += m_vecCharSize[n];
		m_pCanvas->rc.right = m_pCanvas->rc.left;
		for(INT n=nStart; n<nStart+nLen; n++)
			m_pCanvas->rc.right += m_vecCharSize[n];

		// ��ɫ(Alphaֵ����)
		DWORD dwColor = ((~m_dwTextColor)&0x00ffffff)|(m_dwTextColor&0xff000000); 
		DWORD dwShadeColor = ((~m_dwShadeColor)&0x00ffffff)|(m_dwShadeColor&0xff000000); 
		m_pRender->Draw(m_pCanvas, NULL, m_dwTextColor, m_eAlphaWrite);
		if( m_bPassword )
		{
			m_pRender->Text(m_pCanvas, strStar.substr(nStart, nLen).c_str(), m_pFont, 
				dwColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
		}
		else
		{
			m_pRender->Text(m_pCanvas, m_strText.substr(nStart, nLen).c_str(), m_pFont, 
				dwColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
		}
	}

	// ��Ⱦ��� 
	if( m_bShowCursor && m_nCursorPos >=m_nDispStart && m_nCursorPos <= m_nDispEnd )
	{
		m_pCanvas->rc.left = m_ptView.x + m_rcText.left;
		for(INT n=m_nDispStart; n<m_nCursorPos; n++)
			m_pCanvas->rc.left += m_vecCharSize[n];
		m_pCanvas->rc.right = m_pCanvas->rc.left+2;
		m_pRender->Draw(m_pCanvas, NULL, m_dwCursorColor, m_eAlphaWrite);
	}

	// ���û�����Ⱦ
	GUIWnd::Render();
}


//-----------------------------------------------------------------------------
// ������Ϣ����
//-----------------------------------------------------------------------------
BOOL GUIEditBox::OnInputMessage(tagGUIInputMsg* pMsg)
{
	INT nPos = m_nCursorPos;
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN: // �ҵ���������ַ������ƶ����
		nPos = GetCharIndex(pMsg->pt.x);
		if( GT_VALID(nPos) )
		{
			m_nSelStart = m_nSelEnd = m_nCursorPos = nPos;
			this->SetRefresh();
		}
		break;

	case WM_LBUTTONDBLCLK:	// ����˫����ѡ������
		m_nSelStart = m_strText.find_last_of(' ', m_nCursorPos) + 1;
		m_nSelEnd = m_strText.find_first_of(' ', m_nCursorPos);
		if( m_nSelEnd == std::string::npos )
			m_nSelEnd = m_strText.size();
		m_nCursorPos = m_nSelEnd;
		this->SetRefresh();
		break;

	case WM_MOUSEMOVE:
		if( pMsg->dwParam1 == MK_LBUTTON && m_pSystem->GetActive() == this )	// ��������϶�ѡ��
		{
			if( pMsg->pt.x < m_ptView.x+m_rcText.left )
				this->ChangeDisplayStart(-1);
			if( pMsg->pt.x > m_ptView.x+m_rcText.right )
				this->ChangeDisplayStart(1);

			INT n = GetCharIndex(pMsg->pt.x);
			if( GT_VALID(n) )
				m_nSelEnd = m_nCursorPos = n;
			this->SetRefresh();
		}
		break;

	case WM_KEYDOWN:
		if( VK_LEFT == pMsg->dwParam1 && m_nCursorPos > 0 )	// ���������
		{
			m_nCursorPos--;
			if( m_nCursorPos < m_nDispStart )	// �Ƿ���Ҫ���Ҿ���
				this->ChangeDisplayStart(-1);
		}
		if( VK_RIGHT == pMsg->dwParam1 && m_nCursorPos < (INT)m_strText.size() ) // �����ҷ����
		{
			m_nCursorPos++;
			while( m_nCursorPos > m_nDispEnd ) // ��Ҫ��
				this->ChangeDisplayStart(1);
		}
		if( VK_HOME == pMsg->dwParam1 )	// ����ƶ�������ǰ
		{
			m_nCursorPos = 0;
			this->ChangeDisplayStart(-m_nDispStart);
		}
		if( VK_END == pMsg->dwParam1 )	// ����ƶ�������β
		{
			m_nCursorPos = m_strText.size();
			while( m_nCursorPos > m_nDispEnd )
				this->ChangeDisplayStart(1);
		}
		if( VK_DELETE == pMsg->dwParam1 && !m_bReadOnly )
		{
			if( m_nSelStart != m_nSelEnd )
				this->DelSelected();
			else
			{
				// ɾ���ַ�
				m_strText.erase(m_nCursorPos, 1);
				this->CalCharWidth();
			}
			this->SetRefresh();
			return TRUE;
		}
		if( VK_LEFT == pMsg->dwParam1 || VK_RIGHT == pMsg->dwParam1	// ����SHIFTѡ��
			 || VK_HOME == pMsg->dwParam1 || VK_END == pMsg->dwParam1 )
		{
			if( GetKeyState(VK_SHIFT) < 0 )
				m_nSelStart = (m_nSelStart == m_nSelEnd) ? nPos : m_nSelStart;
			else
				m_nSelStart = m_nCursorPos;
			m_nSelEnd = m_nCursorPos;
			this->SetRefresh();
			return TRUE;
		}
		if( VK_RETURN == pMsg->dwParam1 && !m_bReadOnly )	// ����س�
		{
			tagGUIEvent event(this, EGUIE_EditBoxEnter, pMsg);	// �����¼�
			SendEvent(&event);
			return TRUE;
		}
		break;

	case WM_SYSCHAR:
	case WM_CHAR:
		this->SetRefresh();
		switch( pMsg->dwParam1 )
		{
		case ('c' & 0x1F):	// Ctrl + C
			this->CopyToClipboard();
			return TRUE;
		case ('v' & 0x1F):	// Ctrl + V
			if( !m_bReadOnly )
			{
				this->DelSelected();
				this->PasteFromClipboard();
				this->SetRefresh();
			}
			return TRUE;
		case ('a' & 0x1F):	// Ctrl+A ѡ������Ԫ��
			m_nSelStart = m_nDispStart = m_nCursorPos = 0;
			m_nSelEnd = m_strText.size();
			ChangeDisplayStart(0);	// ����m_nDispEnd
			this->SetRefresh();
			return TRUE;
		case ('x' & 0x1F):	// Ctrl+X
			this->CopyToClipboard();
			if( !m_bReadOnly )
				this->DelSelected();
			this->SetRefresh();
			return TRUE;
		case VK_BACK:	// �˸� �൱�� ���ͷ+del
			if( !m_bReadOnly )
			{
				if( m_nSelStart != m_nSelEnd ) // ��ѡ�У�ɾѡ��
					this->DelSelected();
				else if( m_nCursorPos > 0 )
				{
					m_nCursorPos--;
					if( m_nCursorPos < m_nDispStart )	// �Ƿ���Ҫ���Ҿ��� 
						this->ChangeDisplayStart(-1);

					if( GetKeyState(VK_SHIFT) < 0 )	// ����ѡ��
					{
						m_nSelStart = m_nSelStart == m_nSelEnd ? nPos : m_nSelStart;
						m_nSelEnd = m_nCursorPos;
					}

					// ɾ���ַ�
					m_strText.erase(m_nCursorPos, 1);
					this->CalCharWidth();
				}
				return TRUE;
			}
			break;

		case VK_TAB:
			break;

		case VK_RETURN:	// ����س�
			{
				tagGUIEvent event(this, EGUIE_EditBoxEnter, pMsg);	// �����¼�
				SendEvent(&event);
				return TRUE;
			}
			break;

		default:
			if( m_bReadOnly || GetKeyState(VK_ESCAPE) < 0 
				||(m_bNumerical && (pMsg->dwParam1<0x30||pMsg->dwParam1> 0x39))) //ֻ��������
				break;
			if( (INT)m_strText.size() + 1 >= m_nMaxTextCount )	//�ȼ����ֽ����ϵ�����
				break;

			if( m_nSelStart != m_nSelEnd )	// ������ѡ����ɾ��ѡ�񲿷�
				this->DelSelected();

			m_strText.insert(m_nCursorPos++, (TCHAR*)&pMsg->dwParam1);
			this->CalCharWidth();
			while( m_nCursorPos > m_nDispEnd ) // �Ƿ���Ҫ���Ҿ���
				this->ChangeDisplayStart(1);
			return TRUE;
			break; // case default
		}	
		break;	// case WM_CHAR:
	}	// switch( pMsg->dwType )


	return GUIStatic::OnInputMessage(pMsg);
}


//-----------------------------------------------------------------------------
// ������Ϊ����
//-----------------------------------------------------------------------------
GUIWnd* GUIEditBox::OnActive()
{
	// ����ƶ����ַ���ĩβ
	m_nCursorPos = m_strText.size();
	while( m_nCursorPos > m_nDispEnd )
		this->ChangeDisplayStart(1);

	// ȫ��ѡ��
	m_nSelStart = 0;
	m_nSelEnd = m_nCursorPos;

	this->SetRefresh();
	return GUIStatic::OnActive();
}


//-----------------------------------------------------------------------------
// ��ʧ����
//-----------------------------------------------------------------------------
VOID GUIEditBox::OnDeactive(GUIWnd* pWnd)
{
	m_nSelStart = m_nSelEnd = m_nCursorPos;	// ȡ��ѡ��
	m_bShowCursor = false;	// �����ʧ

	this->SetRefresh();
	GUIStatic::OnDeactive(pWnd);
}


//-----------------------------------------------------------------------------
// �ƶ��ؼ�,���ɸı�ؼ���С
//-----------------------------------------------------------------------------
BOOL GUIEditBox::Resize(tagPoint& ptSize)
{
	tagPoint pt = ptSize;
	BOOL bResult = GUIStatic::Resize(ptSize);
	if( !(pt == ptSize) )
		ChangeDisplayStart(0);	// �ؼ���С�ı�,��Ҫ���¼�����ʾ����

	return bResult;
}


//-----------------------------------------------------------------------------
// ����ַ������޸ģ���Ҫ���ô˺�������ÿ���ַ����
//-----------------------------------------------------------------------------
VOID GUIEditBox::CalCharWidth()
{
	TCHAR szTemp[2] = {0};
	tagPoint ptSize;

	// ��ֵ�����ж��Ƿ񳬳����ֵ
	if(m_bNumerical)
	{
		INT nValue = _tcstol(m_strText.c_str(), NULL, 10);
		if(nValue < 0 || nValue > m_nMaxNumberValue)
		{
			tstringstream ss;
			ss << m_nMaxNumberValue;
			m_strText = ss.str();
			m_nCursorPos = m_strText.size();
		}
	}
	
	m_vecCharSize.assign(m_strText.size()+1, 0); // +1����ѭ���ռ�
	for(INT n=0; n<(INT)m_strText.size(); n++)
	{
		szTemp[0] = m_bPassword ? _T('*') : m_strText[n];
		m_pRender->GetTextSize(szTemp, m_pFont, ptSize);
		m_vecCharSize[n] = ptSize.x;
	}

	ChangeDisplayStart(0);	// ���¼���m_nDispStart/m_nDispEnd
}


//-----------------------------------------------------------------------------
// �޸Ĵ��Ǹ��ַ���ʼ��ʾ(nIndexΪ���ֵ)
//-----------------------------------------------------------------------------
VOID GUIEditBox::ChangeDisplayStart(INT nIndex)
{
	if( m_nDispStart <= 0 && nIndex < 0 )
		return;
	if( m_nDispEnd >= (INT)m_strText.size() && nIndex > 0 )
		return;

	m_nDispStart += nIndex;

	// ����EditBox��ǰ��Ⱦ��������
	INT nLeft = 0;
	for(INT n=m_nDispStart; n<(INT)m_strText.size(); n++)
	{
		if( nLeft + m_vecCharSize[n] >= m_rcText.right - m_rcText.left )
			break;	// ��ʾ������

		nLeft += m_vecCharSize[n];
		m_nDispEnd = n + 1;
	}
}


//-----------------------------------------------------------------------------
// ��view����õ���ӽ��ַ������
//-----------------------------------------------------------------------------
INT GUIEditBox::GetCharIndex(INT nViewX)
{
	// �ҵ���������ַ�
	INT nX = nViewX - m_ptView.x - m_rcText.left;
	INT nBestPos = GT_INVALID;
	INT nLeastDistance = 0x7fffffff;
	INT nCharPos = 0;	// ��¼��ǰ�ַ�λ��

	if( m_strText.size() == 0 || nX < 0 )
		return 0;

	for(INT n=m_nDispStart; n<=m_nDispEnd; n++)
	{
		if( abs(nCharPos - nX) < nLeastDistance )
		{
			nLeastDistance = abs(nCharPos - nX);
			nBestPos = n;
		}

		if( n < (INT)m_vecCharSize.size() )
			nCharPos += m_vecCharSize[n];
	}

	return nBestPos;
}


//-----------------------------------------------------------------------------
// ֱ�������ı�������
//-----------------------------------------------------------------------------
VOID GUIEditBox::SetText(LPCTSTR szText)
{
	//ע���ֽ�����
	INT nTextCount = (INT)lstrlen(szText);
	nTextCount = (nTextCount > m_nMaxTextCount) ? m_nMaxTextCount : nTextCount;

	m_strText.assign(szText, nTextCount);
	m_nCursorPos = m_nSelEnd = m_nSelStart = 0;
	m_nDispStart = m_nDispEnd = 0;
	this->CalCharWidth();
	this->SetRefresh();
}


//-----------------------------------------------------------------------------
// ɾ��ѡ����ַ�
//-----------------------------------------------------------------------------
VOID GUIEditBox::DelSelected()
{
	if( m_nSelStart == m_nSelEnd ) //��û���ı���ѡ��
		return;

	if( m_nSelStart > m_nSelEnd )
		std::swap(m_nSelStart, m_nSelEnd);

	m_strText.erase(m_nSelStart, m_nSelEnd - m_nSelStart);
	m_nCursorPos = m_nSelEnd = m_nSelStart;
	if( m_nSelStart < m_nDispStart )
		m_nDispStart = m_nCursorPos;

	this->CalCharWidth();
}


//-----------------------------------------------------------------------------
// ��ѡ�����ݿ��������� 
//-----------------------------------------------------------------------------
VOID GUIEditBox::CopyToClipboard()
{
	// ����ģʽ/��ѡ�� ������
	if( m_bPassword || m_nSelStart == m_nSelEnd )
		return;	
	
	if( !OpenClipboard(NULL) )
		return;

	EmptyClipboard(); 

	INT nLen = abs(m_nSelEnd - m_nSelStart);
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (nLen+1) * sizeof(TCHAR)); 
	if( NULL == hglbCopy ) 
	{ 
		CloseClipboard();
		return;
	} 

	// Lock the handle and copy the text to the buffer. 
	LPTSTR lptstrCopy = (LPTSTR)GlobalLock( hglbCopy ); 
	INT nStart = m_nSelStart < m_nSelEnd ? m_nSelStart : m_nSelEnd;
	memcpy(lptstrCopy, &m_strText[nStart], nLen * sizeof(TCHAR)); 
	lptstrCopy[nLen] = (TCHAR)0;    // null character 
	GlobalUnlock(hglbCopy); 

	// Place the handle on the clipboard. 
	SetClipboardData(CF_UNICODETEXT, hglbCopy); 
	CloseClipboard();
}


//-----------------------------------------------------------------------------
// �Ӽ����忽���ı�����ǰλ��
//-----------------------------------------------------------------------------
VOID GUIEditBox::PasteFromClipboard()
{
	if( FALSE == IsClipboardFormatAvailable(CF_UNICODETEXT) ) 
		return; 

	if( FALSE == OpenClipboard(NULL) )
		return; 

	HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT); 
	if( hglb != NULL ) 
	{ 
		LPTSTR lpstr = (LPTSTR)GlobalLock(hglb); 
		if( NULL != lpstr ) 
		{ 
			tstring str(lpstr);
			DWORD dwInsertSize = str.size();

			if( m_bNumerical )	// �жϸ��ַ����ǲ��Ǻ��з�����
			{
				tstring strNum(_T("0123456789"));
				if( tstring::npos != str.find_first_not_of(strNum) )
				{
					GlobalUnlock(hglb); 
					CloseClipboard(); 
					return;
				}
			}

			if( m_strText.size() + dwInsertSize > (DWORD)m_nMaxTextCount ) // ��������
				dwInsertSize = m_nMaxTextCount - m_strText.size();

			m_strText.insert(m_nCursorPos, str.substr(0, dwInsertSize));
			m_nCursorPos += dwInsertSize;
			this->CalCharWidth();

			// �����Ƿ���Ҫ�������
			while( m_nCursorPos > m_nDispEnd )
				this->ChangeDisplayStart(1);

			GlobalUnlock(hglb); 
		} 
	} 
	
	CloseClipboard(); 
	return; 
}


}	// namespace vEngine {
