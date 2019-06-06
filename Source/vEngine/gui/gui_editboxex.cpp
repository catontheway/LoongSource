//-----------------------------------------------------------------------------
//!\file gui_editboxex.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2008-01-21
//!
//!\brief ����ϵͳ edit box ex �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_editboxex.h"
#include "gui_event_define.h"
#include "..\resource\resource.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
GUIEditBoxEx::GUIEditBoxEx()
{
	m_pVarMgr->Register(_T("ReadOnly"), &m_bReadOnly, _T("no yes"));
	m_pVarMgr->Register(_T("LineHeight"), &m_nLineHeight);
	m_pVarMgr->Register(_T("CursorColor"), &m_dwCursorColor);
	m_pVarMgr->Register(_T("CursorTimer"), &m_dwCursorTimer);
    m_pVarMgr->Register(_T("MaxTextCount"), &m_nMaxTextCount);
	
	m_dwTextColor = 0xFF000000;
	m_dwShadeColor = 0;
	m_dwCursorColor = 0xFF000000;
    m_dwCursorTimer = 200;
    m_nMaxTextCount = 0xFFFF;
	m_nLineHeight = 16;	// �и�
	m_bShowCursor = false;	// ����ʾ���
	m_nDispStart = m_nCurFirstLine = m_nCursorPos = 0;
	m_bEditable = TRUE;
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIEditBoxEx::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	InitializeCriticalSection(&m_Lock);
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);
	m_nVisibleLine = (m_rcText.bottom - m_rcText.top) / m_nLineHeight;	// һ������ʾ������
	
	tstring strText = m_strText;
	if( m_strText.size() > 0 )
		strText.assign(m_strText.begin(), m_strText.end()-1);
	this->Clear();

	// �ж��Ƿ��ǵȿ�����
	m_bFixWidth = FALSE;	// �Ƿ�ȿ�
	tagPoint pt1, pt2;
	m_pRender->GetTextSize(_T("M"), m_pFont, pt1);
	m_pRender->GetTextSize(_T("i"), m_pFont, pt2);
	if( pt1.x == pt2.x )
	{
		m_bFixWidth = TRUE;		// �Ƿ�ȿ�
		m_nEngWidth = pt1.x;	// Ӣ�Ŀ��
	}

	m_pRender->GetTextSize(_T("��"), m_pFont, pt1);
	m_nChnWidth = pt1.x;	// ���Ŀ��

	this->AddString(0, strText);
	return bResult;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::Destroy()
{
	DeleteCriticalSection(&m_Lock);
	GUIStatic::Destroy();
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::Update()
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
VOID GUIEditBoxEx::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	// ��Ⱦ��ǩ
	if( m_mapBookmark.Size() > 0 )
	{
		for(INT n=m_nCurFirstLine; n<m_nCurFirstLine+m_nVisibleLine; n++)
		{
			if( n >= m_nLine )
				break;

			DWORD dwColor = m_mapBookmark.Peek(n);
			if( GT_VALID(dwColor) )
			{
				m_pCanvas->rc.left = m_ptView.x + m_rcText.left;
				m_pCanvas->rc.right = m_ptView.x + m_rcText.right;
				m_pCanvas->rc.top = m_ptView.y + m_rcText.top + m_nLineHeight*(n-m_nCurFirstLine);
				m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_nLineHeight;
				m_pRender->Draw(m_pCanvas, NULL, dwColor, m_eAlphaWrite);
			}
		}
	}

	// �ڴ˽���������
	EnterCriticalSection(&m_Lock);

	// ������Ҫ��Ⱦ���ַ���
	INT nLine=0, nLast = 0;
	for(INT n=m_nDispStart; n<(INT)m_vecChar.size(); n++)
	{
		if( m_vecChar[n].eType > EGEC_Normal )
			nLine++, nLast = n;
		if( nLine > m_nVisibleLine )
			break;
	}

	// ��Ⱦ�����ַ���
	m_pCanvas->rc = m_rcText + m_ptView;
	m_pRender->Text(m_pCanvas, m_strText.substr(m_nDispStart, nLast - m_nDispStart), 
		m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

	// ��Ⱦѡ���ַ�
	if( m_nSelStart != m_nSelEnd )
	{
		INT nSelStart = m_nSelStart;
		INT nSelEnd = m_nSelEnd;

		if( nSelStart > nSelEnd )
			std::swap(nSelStart, nSelEnd);

		DWORD dwTextColor = ((~m_dwTextColor)&0x00ffffff)|(m_dwTextColor&0xff000000); 

		m_pCanvas->rc = m_rcText + m_ptView;
		m_pCanvas->rc.right = m_pCanvas->rc.left;
		m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_nLineHeight;
		for(INT n=m_nDispStart, nPointer=m_nDispStart; n<=nLast; n++)
		{
			if( m_vecChar[n].eType > EGEC_Normal || n >= nSelEnd )
			{
				m_pRender->Draw(m_pCanvas, NULL, m_dwTextColor, m_eAlphaWrite);
				m_pRender->Text(m_pCanvas, m_strText.substr(nPointer, n - nPointer), 
					m_pFont, dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
				nPointer = n+1;

				m_pCanvas->rc.left = m_pCanvas->rc.right = m_ptView.x + m_rcText.left;
				m_pCanvas->rc.top += m_nLineHeight;
				m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_nLineHeight;
				if( m_pCanvas->rc.bottom > m_ptView.y + m_rcText.bottom )
					m_pCanvas->rc.bottom = m_ptView.y + m_rcText.bottom;

				if( n >= nSelEnd )
					break;
			}

			m_pCanvas->rc.right += m_vecChar[n].nWidth;
			if( n<nSelStart )
			{
				m_pCanvas->rc.left = m_pCanvas->rc.right;
				nPointer=n+1;
			}
		}
	}

	// ��Ⱦ��� 
	if( m_bShowCursor && m_nCursorPos >= m_nDispStart && m_nCursorPos <= nLast )
	{
		m_pCanvas->rc.left = m_ptView.x + m_rcText.left;
		for(INT n = this->FindFirstCharThisLine(m_nCursorPos);n<m_nCursorPos;n++)
			m_pCanvas->rc.left += m_vecChar[n].nWidth;
		m_pCanvas->rc.right = m_pCanvas->rc.left+2;
		
		m_pCanvas->rc.top = m_ptView.y + m_rcText.top;
		m_pCanvas->rc.top += (this->GetLine(m_nCursorPos) - m_nCurFirstLine)*m_nLineHeight; 
		m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_nLineHeight; 
		if( m_pCanvas->rc.bottom > m_ptView.y + m_rcText.bottom )
			m_pCanvas->rc.bottom = m_ptView.y + m_rcText.bottom;
		m_pRender->Draw(m_pCanvas, NULL, m_dwCursorColor, m_eAlphaWrite);
	}

	// �ڴ��뿪������
	LeaveCriticalSection(&m_Lock);

	// ���û�����Ⱦ
	GUIWnd::Render();
}


//-----------------------------------------------------------------------------
// ������Ϣ����
//-----------------------------------------------------------------------------
BOOL GUIEditBoxEx::OnInputMessage(tagGUIInputMsg* pMsg)
{
	INT nLastCursorPos = m_nCursorPos;
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN: // �ҵ���������ַ������ƶ����
		if( m_pSystem->GetMousePoint() == this )
		{
			INT nPos = GetCharIndex(pMsg->pt);
			if( GT_VALID(nPos) )
			{
				m_nSelStart = m_nSelEnd = nPos;
				this->ChangeCursorPos(nPos - m_nCursorPos);
				this->SetRefresh();
			}
		}
		break;
	case WM_LBUTTONDBLCLK:	// ����˫����ѡ������
		if( m_pSystem->GetMousePoint() == this )
		{
			m_nSelStart = m_strText.find_last_of(' ', m_nCursorPos) + 1;
			m_nSelEnd = m_strText.find_first_of(' ', m_nCursorPos);
			if( m_nSelEnd == std::string::npos )
				m_nSelEnd = m_strText.size();
			this->ChangeCursorPos(m_nSelEnd - m_nCursorPos);
			this->SetRefresh();
		}
		break;
	case WM_MOUSEMOVE:
		if( pMsg->dwParam1 == MK_LBUTTON && m_pSystem->GetActive() == this )	// ��������϶�ѡ��
		{
			if( pMsg->pt.y < m_ptView.y+m_rcText.top )	// �������¹�
				this->ChangeDisplayLineStart(-1);
			if( pMsg->pt.y > m_ptView.y+m_rcText.bottom )
				this->ChangeDisplayLineStart(1);

			INT n = GetCharIndex(pMsg->pt);
			if( GT_VALID(n) )
			{
				m_nSelEnd = n;
				ChangeCursorPos(n-m_nCursorPos);
			}
			this->SetRefresh();
		}
		break;
	case WM_MOUSEWHEEL:	// ����������
		if( pMsg->dwParam1 & 0x80000000 )	// ����
			this->ChangeDisplayLineStart(3);
		else
			this->ChangeDisplayLineStart(-3);
	case WM_KEYDOWN:
		this->SetRefresh();
		if( VK_LEFT == pMsg->dwParam1 && m_nCursorPos > 0 )	// ���������
			ChangeCursorPos(-1);
		if( VK_RIGHT == pMsg->dwParam1 && m_nCursorPos < (INT)m_strText.size()-1 ) // �����ҷ����
			ChangeCursorPos(1);
		if( VK_UP == pMsg->dwParam1 && this->GetLine(m_nCursorPos) > 0 )	// �����Ϸ����
		{
			INT nThisLeft = m_nCursorPos - this->FindFirstCharThisLine(m_nCursorPos);
			INT nUpEnd = this->FindFirstCharThisLine(m_nCursorPos)-1;
			INT nUpFirst = this->FindFirstCharThisLine(nUpEnd);
			INT nPos = nUpFirst + nThisLeft;
			if( nUpEnd - nUpFirst < nThisLeft )	// ��һ��û�б�����ô��
				nPos = nUpEnd;
			ChangeCursorPos(nPos-m_nCursorPos);
		}
		if( VK_DOWN == pMsg->dwParam1 && this->GetLine(m_nCursorPos) < m_nLine - 1 )	// �����·����
		{
			INT nThisLeft = m_nCursorPos - this->FindFirstCharThisLine(m_nCursorPos);
			INT nDownFirst = this->FindReturnThisLine(m_nCursorPos)+1;
			INT nDownEnd = this->FindReturnThisLine(nDownFirst);
			INT nPos = nDownFirst + nThisLeft;
			if( nDownEnd - nDownFirst < nThisLeft )	// ��һ��û�б�����ô��
				nPos = nDownEnd;
			ChangeCursorPos(nPos-m_nCursorPos);
		}
		if( VK_HOME == pMsg->dwParam1 )	// ����ƶ�������ǰ
		{
			INT nPos = this->FindFirstCharThisLine(m_nCursorPos) - m_nCursorPos;
			ChangeCursorPos(nPos);
			if( GetKeyState(VK_CONTROL) < 0 )	// �������ĵ���
			{
				this->ChangeDisplayLineStart(-m_nCurFirstLine);
				ChangeCursorPos(0-m_nCursorPos);
			}
		}
		if( VK_END == pMsg->dwParam1 )	// ����ƶ�������β
		{
			INT nPos = this->FindReturnThisLine(m_nCursorPos) - m_nCursorPos;
			ChangeCursorPos(nPos);
			if( GetKeyState(VK_CONTROL) < 0 )	// �������ĵ�β
			{
				this->ChangeDisplayLineStart(0xffffff);
				ChangeCursorPos(m_vecChar.size()-1-m_nCursorPos);
			}
		}
		if( VK_PRIOR == pMsg->dwParam1 )
		{
			this->ChangeDisplayLineStart(-m_nVisibleLine);
			ChangeCursorPos(m_nDispStart-m_nCursorPos);
		}
		if( VK_NEXT == pMsg->dwParam1 )
		{
			this->ChangeDisplayLineStart(m_nVisibleLine);
			ChangeCursorPos(m_nDispStart-m_nCursorPos);
		}
		if( VK_LEFT == pMsg->dwParam1 || VK_RIGHT == pMsg->dwParam1	// ����SHIFTѡ��
			 || VK_HOME == pMsg->dwParam1 || VK_END == pMsg->dwParam1
			 || VK_UP == pMsg->dwParam1 || VK_DOWN == pMsg->dwParam1
			 ||  VK_PRIOR == pMsg->dwParam1 ||  VK_PRIOR == pMsg->dwParam1 )
		{
			if( GetKeyState(VK_SHIFT) < 0 )
				m_nSelStart = (m_nSelStart == m_nSelEnd) ? nLastCursorPos : m_nSelStart;
			else
				m_nSelStart = m_nCursorPos;
			m_nSelEnd = m_nCursorPos;
			return TRUE;
		}
		if( VK_DELETE == pMsg->dwParam1 && !m_bReadOnly ) // ����delɾ��
		{
			if( m_nSelStart != m_nSelEnd )
				this->DelString(m_nSelStart, m_nSelEnd);	// ��ѡ�У�ɾ��ѡ��
			else
				if( m_nCursorPos < (INT)m_strText.size() - 1 )	// ɾ��1���ַ�
					DelString(m_nCursorPos, m_nCursorPos+1);
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
			if( m_bReadOnly )
				break;
			this->DelString(m_nSelStart, m_nSelEnd);
			this->PasteFromClipboard();
			return TRUE;
		case ('a' & 0x1F):	// Ctrl+A ѡ������Ԫ��
			m_nSelStart = 0;
			m_nSelEnd = m_strText.size()-1;
			this->ChangeCursorPos(m_vecChar.size()-1-m_nCursorPos);
			this->ChangeDisplayLineStart(0xffffff);	// Ų�������ĩβ
			return TRUE;
		case ('x' & 0x1F):	// Ctrl+X
			this->CopyToClipboard();
			if( !m_bReadOnly )
				this->DelString(m_nSelStart, m_nSelEnd);
			return TRUE;
		case ('l' & 0x1F):	// ѡ�б��и��Ƶ������壬��ɾ��
			m_nSelStart = this->FindFirstCharThisLine(m_nCursorPos);
			m_nSelEnd = this->FindReturnThisLine(m_nCursorPos)+1;
			this->CopyToClipboard();
			if( !m_bReadOnly )
			{
				this->DelString(m_nSelStart, m_nSelEnd);
				this->ChangeCursorPos(m_nSelStart-m_nCursorPos);
			}
			return TRUE;
		case VK_BACK:	// �˸� �൱�� ���ͷ+del
			if( m_bReadOnly )
				break;
			if( m_nSelStart != m_nSelEnd ) // ��ѡ�У�ɾѡ��
				this->DelString(m_nSelStart, m_nSelEnd);
			else if( m_nCursorPos > 0 && m_vecChar.size() > 1 )
			{
				ChangeCursorPos(-1);
				DelString(m_nCursorPos, m_nCursorPos+1);	// ɾ���ַ�
			}
			return TRUE;
		default:
			if( m_bReadOnly || GetKeyState(VK_ESCAPE) < 0 )
				break;

            if( (INT)m_strText.size() + 1 >= m_nMaxTextCount )	//�ȼ����ֽ����ϵ�����
                break;

			this->DelString(m_nSelStart, m_nSelEnd); // ��ɾ��ѡ�񲿷�
			ChangeCursorPos(AddString(m_nCursorPos, (TCHAR*)&pMsg->dwParam1));
			return TRUE;
		}	
		break;	// case WM_CHAR:
	}	// switch( pMsg->dwType )

	return GUIStatic::OnInputMessage(pMsg);
}



//-----------------------------------------------------------------------------
// �����ӿؼ������¼�
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::SendEvent(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pSystem->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return;

	switch( pEvent->eEvent ) 
	{
	case EGUIE_Scroll:
		if( pWnd != this )
			this->SetCurLine(pEvent->dwParam1, FALSE);
		break;

	default:
		break;
	}

	GUIStatic::SendEvent(pEvent);
}


//-----------------------------------------------------------------------------
// ����Ӵ���
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::AddChild(GUIWnd* pChild)
{
	GUIStatic::AddChild(pChild);

	// �ӿؼ������ǹ��������ù�����ˢ��
	SetCurLine(m_nCurFirstLine); 
}


//-----------------------------------------------------------------------------
// ������Ϊ����
//-----------------------------------------------------------------------------
GUIWnd* GUIEditBoxEx::OnActive()
{
	this->SetRefresh();
	return GUIStatic::OnActive();
}


//-----------------------------------------------------------------------------
// ��ʧ����
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::OnDeactive(GUIWnd* pWnd)
{
	m_nSelStart = m_nSelEnd = m_nCursorPos;	// ȡ��ѡ��
	m_bShowCursor = false;	// �����ʧ

	this->SetRefresh();
	GUIStatic::OnDeactive(pWnd);
}


//-----------------------------------------------------------------------------
// �ƶ��ؼ�,���ɸı�ؼ���С
//-----------------------------------------------------------------------------
BOOL GUIEditBoxEx::Resize(tagPoint& ptSize)
{
	tagPoint pt = m_ptSize;
	BOOL bResult = GUIStatic::Resize(ptSize);
	if( !(pt == m_ptSize) )
	{
		this->ChangeCursorPos(0-m_nCursorPos);
		ReMakeup(0, m_vecChar.size()-1);	// �ؼ���С�ı�,��Ҫ���¼�����ʾ����

		// ����������Ϣ
		tagGUIEvent event(this, EGUIE_Scroll, NULL);
		event.dwParam1 = (DWORD)m_nCurFirstLine;
		event.dwParam2 = (DWORD)m_nLine;
		event.dwParam3 = (DWORD)m_nVisibleLine;
		OnEvent(&event);

		this->SetRefresh();
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// ��ָ��λ�ã�ָ��������ַ��������������ַ���
//-----------------------------------------------------------------------------
INT GUIEditBoxEx::AddString(INT nPos, tstring str)
{
	if( str.empty() || nPos > (INT)m_vecChar.size() - 1 )
		return 0;

	INT nFind = 0;
	while( (nFind = str.find(_T('\t'), nFind)) != str.npos )	// �滻tab
		str.replace(nFind, 1, _T("    "));
	nFind = 0;
	while( (nFind = str.find(_T("\r\n"), nFind)) != str.npos )	// ȥ��/r/nǰ��/r
		str.replace(nFind, 2, _T("\n"));
	nFind = 0;
	while( (nFind = str.find(_T("\r"), nFind)) != str.npos )	// �滻/rΪ/n
		str.replace(nFind, 1, _T("\n"));

	tagPoint ptSize;
	tagGUIEditCharInfo info;
	TCHAR szTemp[2] = {0};
	m_strText.insert(nPos, str);
	m_vecChar.insert(m_vecChar.begin()+nPos, str.size(), info);

	if( m_bFixWidth )	// �Ƿ�ȿ���
	{
		for(INT n=0; n<(INT)str.size(); n++)
		{
			szTemp[0] = str[n];
			if( ((CHAR*)szTemp)[1] != 0 )
				m_vecChar[n+nPos].nWidth = m_nChnWidth;
			else
				m_vecChar[n+nPos].nWidth = m_nEngWidth;
			if( str[n] == _T('\n') )
			{
				m_vecChar[n+nPos].eType = EGEC_Return;
				m_vecChar[n+nPos].nWidth = 0;
				m_nLine++;
			}
		}
	}
	else
	{
		for(INT n=0; n<(INT)str.size(); n++)
		{
			szTemp[0] = str[n];
			m_pRender->GetTextSize(szTemp, m_pFont, ptSize);
			m_vecChar[n+nPos].nWidth = ptSize.x;
			if( str[n] == _T('\n') )
			{
				m_vecChar[n+nPos].eType = EGEC_Return;
				m_nLine++;
			}
		}
	}

	this->SetRefresh();

	// ��Ҫ�����Ű棬�Լ�ͳ��ʵ�������ַ���
	INT nIncrease = str.size() + ReMakeup(nPos, nPos +str.size());

	// ����������Ϣ
	tagGUIEvent event(this, EGUIE_Scroll);
	event.dwParam1 = (DWORD)m_nCurFirstLine;
	event.dwParam2 = (DWORD)m_nLine;
	event.dwParam3 = (DWORD)m_nVisibleLine;
	OnEvent(&event);

	return nIncrease;
}


//-----------------------------------------------------------------------------
// �õ����еĵ�һ����
//-----------------------------------------------------------------------------
INT	GUIEditBoxEx::FindFirstCharThisLine(INT nPos)
{
	if( nPos >= (INT)m_vecChar.size() )
		return nPos = m_vecChar.size() - 1;
	if( nPos <= 0 )
		return nPos;
	
	if( m_vecChar[nPos].eType > EGEC_Normal )	// ָ��λ�þ��ǻس�
	{
		// ���ǰ��Ҳ�ǻس����ͷ����Լ�
		if( m_vecChar[nPos-1].eType > EGEC_Normal )
			return nPos;
		nPos--;
	}

	while( nPos >= 0 )
	{
		if( m_vecChar[nPos].eType > EGEC_Normal )
			return nPos+1;	// ��һ�лس�֮���һ�����Ǳ��еĵ�һ����

		nPos--;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// �õ�ָ���еĵ�һ����
//-----------------------------------------------------------------------------
INT GUIEditBoxEx::FindFirstChar(INT nLine)
{
	INT nPos = 0;
	INT nLineStep = 0;

	while( nPos < (INT)m_vecChar.size() )
	{
		if( m_vecChar[nPos].eType > EGEC_Normal )
		{
			
			if( nLineStep >= nLine )
			{
				if( nPos > 0 )
					return FindFirstCharThisLine(nPos);
				else
					return nPos;
			}

			nLineStep++;
		}

		nPos++;
	}

	return 0;
}



//-----------------------------------------------------------------------------
// �õ����еĻ��з�λ��
//-----------------------------------------------------------------------------
INT	GUIEditBoxEx::FindReturnThisLine(INT nPos)
{
	if( nPos >= (INT)m_vecChar.size() )
		return nPos = m_vecChar.size() - 1;

	while( nPos < (INT)m_vecChar.size() )
	{
		if( m_vecChar[nPos].eType > EGEC_Normal )
			break;

		nPos++;
	}

	return nPos;
}


//-----------------------------------------------------------------------------
// ��ָ��λ�õõ��к�,����ȷ��m_nDispStart�Լ�m_nCurFirstLine��ȷ
//-----------------------------------------------------------------------------
INT	GUIEditBoxEx::GetLine(INT nPos)
{
	INT nPointer = 0;
	INT nCounter = 0;
	if( nPos == 0 )
		return 0;
	if( nPos >= (INT)m_vecChar.size() )
		return m_nLine-1;
	if( nPos == m_nDispStart )
		return m_nCurFirstLine;

	if( nPos > m_nDispStart )	// ��m_nDispStartǰ��ʼͳ�ƱȽ��Ż�
	{
		nPointer=m_nDispStart;
		nCounter = m_nCurFirstLine;
		for(;nPointer<nPos; nPointer++)
		{
			if( m_vecChar[nPointer].eType > EGEC_Normal )
				nCounter++;
		}
	}
	else
	{
		nCounter = m_nCurFirstLine;
		for(nPointer=m_nDispStart-1;nPointer>=nPos; nPointer--)
		{
			if( m_vecChar[nPointer].eType > EGEC_Normal )
				nCounter--;
		}
	}

	return nCounter;
}


//-----------------------------------------------------------------------------
// �����Ű�,���������ַ���
//-----------------------------------------------------------------------------
INT GUIEditBoxEx::ReMakeup(INT nPos, INT nEndPos)
{
	INT nWidth = 0;	// ����ͳ�ƿ��
	INT nIncrease = 0;	// ����ͳ�������Ļ��з�����
	tagGUIEditCharInfo info;
	info.eType = EGEC_VReturn;	// ��س�

	// ��ָ���ַ������еĵ�һ���ֿ�ʼ�Ű�
	for(INT n=FindFirstCharThisLine(nPos); n<(INT)m_vecChar.size()-1; n++)
	{
		nWidth += m_vecChar[n].nWidth;
		if( nWidth > m_rcText.right - m_rcText.left ) // ��ȵ���
		{
			m_strText.insert(n, _T("\n"), 1); // ��һ����س�
			m_vecChar.insert(m_vecChar.begin()+n, info);
			nWidth = 0;		// �������ͳ��
			nIncrease++;	// �ַ����ı�
			nEndPos++;
			m_nLine++;	// �൱��������һ��
			continue;	// ���н���
		}

		if( m_vecChar[n].eType == EGEC_Return ) // ����Ӳ�س�
		{
			if( n >= nEndPos ) // ������Ҫͳ�Ƶķ�Χ
				break;

			nWidth = 0;	// �������ͳ��
			continue;	// ���н���
		}

		if( m_vecChar[n].eType == EGEC_VReturn ) // ������س���ɾ��
		{
			m_strText.erase(n, 1);
			m_vecChar.erase(m_vecChar.begin()+n);
			nIncrease--;	// �ַ����ı�
			m_nLine--;
			n--;
			nEndPos--;
			continue;
		}
	}

	return nIncrease;
}


//-----------------------------------------------------------------------------
// �޸Ĵ���һ�п�ʼ��ʾ(nLineΪ���ֵ)
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::ChangeDisplayLineStart(INT nLine, BOOL bSendEvent)
{
	INT nPointer = m_nDispStart;
	if( nLine < 0 )
	{
		if( nLine < -m_nCurFirstLine )
			nLine = -m_nCurFirstLine;
		if( nLine >= 0 )	// �޷��Ϲ�
			return;

		INT nIncLine = 0;
		while( nIncLine > nLine )
		{
			nPointer--;
			if( nPointer <= 0 )
			{
				nPointer = 0;
				break;
			}

			if( m_vecChar[nPointer].eType > EGEC_Normal )
			{
				nIncLine--;
				m_nCurFirstLine--;
			}
		}
	}
	else
	{
		if( m_nCurFirstLine + nLine + m_nVisibleLine >= m_nLine )	// �����¹�
			nLine = m_nLine - m_nCurFirstLine - m_nVisibleLine;
		if( nLine <= 0 )	// �޷��¹�
			return;

		INT nIncLine = 0;
		while( nIncLine < nLine )
		{
			if( m_vecChar[nPointer].eType > EGEC_Normal )
			{
				nIncLine++;
				m_nCurFirstLine++;
			}
			nPointer++;
			if( nPointer >= (INT)m_vecChar.size() )
			{
				nPointer = m_vecChar.size() - 1;
				break;
			}
		}
	}

	m_nDispStart = this->FindFirstCharThisLine(nPointer);
	SetRefresh();
	
	if( bSendEvent )	// ����������Ϣ
	{
		tagGUIEvent event(this, EGUIE_Scroll);
		event.dwParam1 = (DWORD)m_nCurFirstLine;
		event.dwParam2 = (DWORD)m_nLine;
		event.dwParam3 = (DWORD)m_nVisibleLine;
		OnEvent(&event);
	}
}


//-----------------------------------------------------------------------------
// �޸������λ��(nPosΪ���ֵ)
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::ChangeCursorPos(INT nPos)
{
	m_nCursorPos += nPos;
	if( m_nCursorPos > (INT)m_vecChar.size() - 1 )
		m_nCursorPos = m_vecChar.size() - 1;
	if( m_nCursorPos < 0  )
		m_nCursorPos = 0;

	INT nLine = GetLine(m_nCursorPos);
	if( nLine < m_nCurFirstLine )	// �Ƿ�Ҫ���Ͼ�
		ChangeDisplayLineStart(nLine - m_nCurFirstLine);
	if( nLine >= m_nCurFirstLine + m_nVisibleLine)	// �Ƿ�Ҫ���¾�
		ChangeDisplayLineStart(nLine - m_nCurFirstLine - m_nVisibleLine+1);

	// ��������ƶ���Ϣ
	tagGUIEvent event(this, EGUIE_CursorMove);
	event.dwParam1 = (DWORD)nLine;
	event.dwParam2 = (DWORD)m_nLine;
	event.dwParam3 = (DWORD)m_nVisibleLine;
	SendEvent(&event);

	this->SetRefresh();
}


//-----------------------------------------------------------------------------
// ��view����õ���ӽ��ַ������
//-----------------------------------------------------------------------------
INT GUIEditBoxEx::GetCharIndex(tagPoint pt)
{
	// �ҵ���������ַ�
	INT nX = pt.x - m_ptView.x - m_rcText.left;
	INT nY = pt.y - m_ptView.y - m_rcText.top;
	INT nBestPos = GT_INVALID;
	INT nLeastDistanceX = 0x7fffffff;
	INT nLeastDistanceY = 0x7fffffff;
	tagPoint ptCharPos(0, m_nLineHeight/2);	// ��¼��ǰ�ַ�λ��

	if( m_strText.size() <= 1 )
		return 0;

	for(INT n=m_nDispStart; n<(INT)m_vecChar.size(); n++)
	{
		INT nDistX = (ptCharPos.x - nX)*(ptCharPos.x - nX);
		INT nDistY = (ptCharPos.y - nY)*(ptCharPos.y - nY);
		
		if( nDistY < nLeastDistanceY )	// ���Ȱ�������ƥ��
		{
			nLeastDistanceX = nDistX;
			nLeastDistanceY = nDistY;
			nBestPos = n;
		}

		if( nDistY <= nLeastDistanceY && nDistX <= nLeastDistanceX )
		{
			nLeastDistanceX = nDistX;
			nLeastDistanceY = nDistY;
			nBestPos = n;
		}

		ptCharPos.x += m_vecChar[n].nWidth;
		if( m_vecChar[n].eType > EGEC_Normal )
		{
			ptCharPos.x = 0;
			ptCharPos.y += m_nLineHeight;
			if( ptCharPos.y > m_rcText.bottom-m_rcText.top )
				break;
		}
	}

	return nBestPos;
}


//-----------------------------------------------------------------------------
// ɾ��������Ϣ
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::Clear()
{
	m_nSelStart = m_nSelEnd = 0;	// ѡ��λ��
	this->ChangeCursorPos(0-m_nCursorPos);		// ���λ��

	// ����һ����س�
	tagGUIEditCharInfo info;
	info.eType = EGEC_VReturn;
	m_vecChar.clear();
	m_vecChar.push_back(info);
	m_strText.assign(_T("\n"));
	m_nLine = 1;	// һ��������

	// ����������Ϣ
	tagGUIEvent event(this, EGUIE_Scroll, NULL);
	event.dwParam1 = (DWORD)m_nCurFirstLine;
	event.dwParam2 = (DWORD)m_nLine;
	event.dwParam3 = (DWORD)m_nVisibleLine;
	OnEvent(&event);
	
	this->SetRefresh();
}


//-----------------------------------------------------------------------------
// ���õ�ǰ��
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::SetCurLine(INT n, BOOL bSendEvent)
{ 
	this->ChangeDisplayLineStart(n-m_nCurFirstLine, bSendEvent); 
}


//-----------------------------------------------------------------------------
// bVirtualReturn:�Ƿ���Ҫ����س�
//-----------------------------------------------------------------------------
tstring& GUIEditBoxEx::GetText(BOOL bVirtualReturn)
{
	// Ϊ�˻�ԭ�ı�ԭò�������е�\n�����\r\n,������س���ɾ��
	m_strOut = m_strText;
	for(INT n=0, nP=0; nP<(INT)m_vecChar.size(); n++,nP++)
	{
		if( m_vecChar[nP].eType == EGEC_Return )
		{
			m_strOut.insert(n, 1, _T('\r'));
			n++;
		}
		if( m_vecChar[nP].eType == EGEC_VReturn && !bVirtualReturn )
		{
			m_strOut.erase(n, 1);
			n--;
		}
	}

	return m_strOut;
}



//-----------------------------------------------------------------------------
// �õ��ı���ʵ������, nLen:[in]��Ҫȡ�����ȣ�[out]ʵ��ȡ������
//-----------------------------------------------------------------------------
LPCTSTR	GUIEditBoxEx::GetText(INT nStart, INT& nLen)
{
	if( nLen == 0 )
		nLen = m_vecChar.size() - 1 - nStart;

	m_strOut.clear();
	if( nLen <= 0 || nStart < 0 || nStart > (INT)m_vecChar.size()-1 )
	{
		nLen = 0;
		return m_strOut.c_str();
	}

	// Ϊ�˻�ԭ�ı�ԭò�������е�\n�����\r\n,������س���ɾ��
	m_strOut = m_strText.substr(nStart, nLen);
	for(INT n=0, nP=nStart; n<nLen; n++,nP++)
	{
		if( m_vecChar[nP].eType == EGEC_Return )
		{
			m_strOut.insert(n, 1, _T('\r'));
			n++;
			nLen++;
		}
		if( m_vecChar[nP].eType == EGEC_VReturn )
		{
			m_strOut.erase(n, 1);
			n--;
			nLen--;
		}
	}
	
	return m_strOut.c_str();
}


//-----------------------------------------------------------------------------
// ֱ�������ı�������
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::SetText(LPCTSTR szText)
{
    //ע���ֽ�����
    INT nTextCount = (INT)lstrlen(szText);
    nTextCount = (nTextCount > m_nMaxTextCount) ? m_nMaxTextCount : nTextCount;

	this->Clear();
	this->AddString(0, szText);

    m_nCursorPos = m_nSelEnd = m_nSelStart = 0;
    m_nDispStart = 0;
	this->SetRefresh();
}


//-----------------------------------------------------------------------------
// ɾ��ǰ�������
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::DelFrontLine(INT nLine)
{
	INT nPointer = 0;
	INT nCounter = 0;
	for(nPointer=0;nPointer<(INT)m_vecChar.size()-1; nPointer++)
	{
		if( m_vecChar[nPointer].eType > EGEC_Normal )
		{
			nCounter++;
			if( nCounter >= nLine )
				break;
		}
	} 

	this->DelString(0, nPointer);
}




//-----------------------------------------------------------------------------
// ɾ��ָ���ַ�
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::DelString(INT nPos, INT nEndPos)
{
	if( nPos == nEndPos )
		return;

	if( nPos > nEndPos )
		std::swap(nPos, nEndPos);

	m_nCursorPos = m_nSelEnd = m_nSelStart = nPos;	// ȡ��ѡ��

	INT nStartLine = this->GetLine(nPos);
	INT nEndLine = this->GetLine(nEndPos);
	INT nOriginLines = m_nLine;

	// ����ɾ��������
	for(INT n=nPos; n<nEndPos; n++)
	{
		if( m_vecChar[n].eType > EGEC_Normal )
			m_nLine--;
	}

	m_strText.erase(nPos, nEndPos - nPos);
	m_vecChar.erase(m_vecChar.begin()+nPos, m_vecChar.begin() + nEndPos);

	INT nCharDec = nEndPos - nPos - this->ReMakeup(nPos, 0);	// ʵ�ʼ��ٵ��ַ�
	INT	nLineDec = nOriginLines - m_nLine;	// ʵ�ʼ��ٵ�����

	if( nLineDec <= 0 )
		return;	// û�������ļ��٣�����Ҫ�ټ���

	// ���ɾ��λ����ȫ������ʾ��Χ֮ǰ
	if( nPos < m_nDispStart && nEndPos <= m_nDispStart )
	{
		m_nCurFirstLine -= nLineDec;
		m_nDispStart = FindFirstChar(m_nCurFirstLine);
	}
	else if( nPos < m_nDispStart && nEndPos > m_nDispStart )
	{
		// ���ɾ��λ������ʾλ���غ�
		m_nCurFirstLine -= (m_nCurFirstLine - nStartLine);
		m_nDispStart = FindFirstChar(m_nCurFirstLine);
	}

	// ������û���㹻���ı���ʾ
	if( m_nCurFirstLine + m_nVisibleLine > m_nLine )
	{
		// ��Ҫ����ʾ��ʼλ��Ų�����ʵ�λ��
		if( m_nVisibleLine >= m_nLine )
		{
			m_nCurFirstLine = m_nDispStart = m_nCursorPos = 0;
		}
		else
		{
			m_nCurFirstLine = m_nLine - m_nVisibleLine;
			m_nDispStart = FindFirstChar(m_nCurFirstLine);
		}
	}

	if( m_vecChar.size() == 0 )
		this->Clear();

	// ����������Ϣ
	tagGUIEvent event(this, EGUIE_Scroll, NULL);
	event.dwParam1 = (DWORD)m_nCurFirstLine;
	event.dwParam2 = (DWORD)m_nLine;
	event.dwParam3 = (DWORD)m_nVisibleLine;
	OnEvent(&event);

}


//-----------------------------------------------------------------------------
// ��ѡ�����ݿ��������� 
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::CopyToClipboard()
{
	// ��ѡ�� ������
	if( m_nSelStart == m_nSelEnd )
		return;	
	
	if( !OpenClipboard(NULL) )
		return;

	EmptyClipboard(); 

	// Ϊ�˻�ԭ�ı�ԭò�������е�\n�����\r\n,������س���ɾ��
	INT nLen = abs(m_nSelEnd - m_nSelStart);
 	INT nStart = m_nSelStart < m_nSelEnd ? m_nSelStart : m_nSelEnd;
	LPCTSTR szText = this->GetText(nStart, nLen);
	
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (nLen+1) * sizeof(TCHAR)); 
	if( NULL == hglbCopy ) 
	{ 
		CloseClipboard();
		return;
	} 

	// Lock the handle and copy the text to the buffer. 
	LPTSTR lptstrCopy = (LPTSTR)GlobalLock( hglbCopy );
	memcpy(lptstrCopy, szText, nLen * sizeof(TCHAR)); 
	lptstrCopy[nLen] = (TCHAR)0;    // null character 
	GlobalUnlock(hglbCopy); 

	// Place the handle on the clipboard. 
	SetClipboardData(CF_UNICODETEXT, hglbCopy); 
	CloseClipboard();
}


//-----------------------------------------------------------------------------
// �Ӽ����忽���ı�����ǰλ��
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::PasteFromClipboard()
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
            if( m_strText.size() + dwInsertSize > (DWORD)m_nMaxTextCount ) // ��������
                str = str.substr(0, m_nMaxTextCount - m_strText.size() - 1);

		    INT nAdd = AddString(m_nCursorPos, str.c_str());
		    ChangeCursorPos(nAdd);
		    GlobalUnlock(hglb);
		}
	} 
	
	CloseClipboard(); 
	return; 
}


//-----------------------------------------------------------------------------
// ������ǩ
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::SetBookmark( INT nLine, DWORD dwColor )
{
	if( !GT_VALID(dwColor) )
		m_mapBookmark.Erase(nLine);
	else
		m_mapBookmark.Add(nLine, dwColor);
}


//-----------------------------------------------------------------------------
// ȫ��ѡ��
//-----------------------------------------------------------------------------
VOID GUIEditBoxEx::SelectAll()
{
	m_nSelStart = 0;
	m_nSelEnd = m_strText.size()-1;
	this->ChangeCursorPos(m_vecChar.size()-1-m_nCursorPos);
	this->ChangeDisplayLineStart(0xffffff);	// Ų�������ĩβ
	this->SetRefresh();
}

}	// namespace vEngine {