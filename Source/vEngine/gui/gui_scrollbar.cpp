//-----------------------------------------------------------------------------
//!\file gui_scrollbar.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-04-28
//!
//!\brief ����ϵͳ scroll bar �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_scrollbar.h"
#include "gui_event_define.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
GUIScrollBar::GUIScrollBar()
{
	m_pVarMgr->Register(_T("Horizon"), &m_bHorizon, _T("no yes"));
	m_pVarMgr->Register(_T("TotalValue"), &m_nTotalValue);
	m_pVarMgr->Register(_T("VisibleValue"), &m_nVisibleValue);
	m_pVarMgr->Register(_T("CurrentValue"), &m_nCurrentValue);

	m_pVarMgr->Register(_T("TopPicFileName"), &m_strTopPic);
	m_pVarMgr->Register(_T("TopPicRect"), &m_rcTopPic);
	m_pVarMgr->Register(_T("TopPicColor"), &m_dwTopPicColor);
	m_pVarMgr->Register(_T("MidPicFileName"), &m_strMidPic);
	m_pVarMgr->Register(_T("MidPicRect"), &m_rcMidPic);
	m_pVarMgr->Register(_T("MidPicColor"), &m_dwMidPicColor);
	m_pVarMgr->Register(_T("DownPicFileName"), &m_strBelowPic);
	m_pVarMgr->Register(_T("DownPicRect"), &m_rcBelowPic);
	m_pVarMgr->Register(_T("BelowPicColor"), &m_dwBelowPicColor);

	m_pVarMgr->Register(_T("TopColor"), &m_dwTopColor);
	m_pVarMgr->Register(_T("BottomColor"), &m_dwBottomColor);

	// ���ó�ʼֵ
	m_bHorizon = false;			// �Ƿ�ˮƽ
	m_nTotalValue = 1000;		// ����
	m_nVisibleValue = 500;		// ����ʾ�������������ı�ֵҲ���ǹ������ö೤
	m_nCurrentValue = 0;		// ��ǰ��

	// ���ó�ʼֵ
	m_dwTopPicColor = 0xffffffff;
	m_dwMidPicColor = 0xffffffff;
	m_dwBelowPicColor = 0xffffffff;

	m_dwTopColor = 0xff000000;
	m_dwBottomColor = 0xFFAFAFAF;

	m_pUpButton = (GUIWnd*)GT_INVALID;
	m_pDownButton = (GUIWnd*)GT_INVALID;
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIScrollBar::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);

	m_pTopPic = m_pRender->CreateImage(m_strTopPic, m_rcTopPic);
	m_pMidPic = m_pRender->CreateImage(m_strMidPic, m_rcMidPic);
	m_pBelowPic = m_pRender->CreateImage(m_strBelowPic, m_rcBelowPic);

	this->CalStick();

	return bResult;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIScrollBar::Destroy()
{
	m_pRender->DestroyImage(m_pTopPic);
	m_pRender->DestroyImage(m_pMidPic);
	m_pRender->DestroyImage(m_pBelowPic);

	GUIStatic::Destroy();
}


//-----------------------------------------------------------------------------
// render
//-----------------------------------------------------------------------------
VOID GUIScrollBar::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	if( m_pTopPic && m_pMidPic && m_pBelowPic )
	{
		if( !m_bHorizon )
		{
			// �Ȼ�����
			m_pCanvas->rc.left = m_ptView.x;
			m_pCanvas->rc.right = m_ptView.x + m_ptSize.x;
			m_pCanvas->rc.top = m_ptView.y + m_nStickStartPos;
			m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_pTopPic->ptSize.y;
			m_pRender->Draw(m_pCanvas, m_pTopPic, m_dwTopPicColor, m_eAlphaWrite);

			if( m_nStickEndPos - m_nStickStartPos > (INT)m_pTopPic->ptSize.y )
			{
				m_pCanvas->rc.top = m_ptView.y + m_nStickEndPos - m_pBelowPic->ptSize.y;
				m_pCanvas->rc.bottom = m_ptView.y + m_nStickEndPos;
				m_pRender->Draw(m_pCanvas, m_pBelowPic, m_dwBelowPicColor, m_eAlphaWrite);

				// �ٻ��м�
				m_pCanvas->rc.top = m_ptView.y + m_nStickStartPos + m_pTopPic->ptSize.y / 2;
				m_pCanvas->rc.bottom = m_ptView.y + m_nStickEndPos - m_pBelowPic->ptSize.y / 2;
				m_pRender->Draw(m_pCanvas, m_pMidPic, m_dwMidPicColor, m_eAlphaWrite);
			}
		}
		else
		{
			// �Ȼ�����
			m_pCanvas->rc.left = m_ptView.x + m_nStickStartPos;
			m_pCanvas->rc.right = m_pCanvas->rc.left + m_pTopPic->ptSize.x; 
			m_pCanvas->rc.top = m_ptView.y;
			m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;
			m_pRender->Draw(m_pCanvas, m_pTopPic, m_dwTopPicColor, m_eAlphaWrite);

			if( m_nStickEndPos - m_nStickStartPos > (INT)m_pTopPic->ptSize.x )
			{
				m_pCanvas->rc.left = m_ptView.x + m_nStickEndPos - m_pBelowPic->ptSize.x;
				m_pCanvas->rc.right = m_ptView.x + m_nStickEndPos;
				m_pRender->Draw(m_pCanvas, m_pBelowPic, m_dwBelowPicColor, m_eAlphaWrite);

				// �ٻ��м�
				m_pCanvas->rc.left = m_ptView.x + m_nStickStartPos + m_pTopPic->ptSize.x / 2;
				m_pCanvas->rc.right = m_ptView.x + m_nStickEndPos - m_pBelowPic->ptSize.x / 2;
				m_pRender->Draw(m_pCanvas, m_pMidPic, m_dwMidPicColor, m_eAlphaWrite);
			}
		}
	}
	else
	{
		if( !m_bHorizon )
		{
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x;
			m_pCanvas->rc.right = m_ptView.x;
			m_pCanvas->rc.top = m_ptView.y + m_nStickStartPos;
			m_pCanvas->rc.bottom = m_ptView.y + m_nStickEndPos;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x;
			m_pCanvas->rc.right = m_ptView.x + m_ptSize.x;
			m_pCanvas->rc.top = m_ptView.y + m_nStickStartPos;
			m_pCanvas->rc.bottom = m_ptView.y + m_nStickStartPos;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x + m_ptSize.x - 1;
			m_pCanvas->rc.right = m_ptView.x + m_ptSize.x - 1;
			m_pCanvas->rc.top = m_ptView.y + m_nStickStartPos + 1;
			m_pCanvas->rc.bottom = m_ptView.y + m_nStickEndPos - 1;
			m_pRender->Line(m_pCanvas, m_dwTopColor);
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x;
			m_pCanvas->rc.right = m_ptView.x + m_ptSize.x;
			m_pCanvas->rc.top = m_ptView.y + m_nStickEndPos - 1;
			m_pCanvas->rc.bottom = m_ptView.y + m_nStickEndPos - 1;
			m_pRender->Line(m_pCanvas, m_dwTopColor);
		}
		else
		{
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x + m_nStickStartPos;
			m_pCanvas->rc.right = m_ptView.x + m_nStickStartPos;
			m_pCanvas->rc.top = m_ptView.y;
			m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x + m_nStickStartPos;
			m_pCanvas->rc.right = m_ptView.x + m_nStickEndPos;
			m_pCanvas->rc.top = m_ptView.y;
			m_pCanvas->rc.bottom = m_ptView.y;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x + m_nStickEndPos - 1;
			m_pCanvas->rc.right = m_ptView.x + m_nStickEndPos - 1;
			m_pCanvas->rc.top = m_ptView.y;
			m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;
			m_pRender->Line(m_pCanvas, m_dwTopColor);
			// ���� ��
			m_pCanvas->rc.left = m_ptView.x + m_nStickStartPos;
			m_pCanvas->rc.right = m_ptView.x + m_nStickEndPos;
			m_pCanvas->rc.top = m_ptView.y + m_ptSize.y;
			m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;
			m_pRender->Line(m_pCanvas, m_dwTopColor);
		}
	}

	// ��Ⱦ����
	m_pCanvas->rc = m_rcText + m_ptView;
	m_pRender->Text(m_pCanvas, m_strText, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

	// ���û�����Ⱦ
	GUIWnd::Render();
}


//-----------------------------------------------------------------------------
// on input msg
//-----------------------------------------------------------------------------
BOOL GUIScrollBar::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch( pMsg->dwType )
	{
	case WM_MOUSEMOVE:
		if( pMsg->dwParam1 == MK_LBUTTON && m_pSystem->GetActive() == this )
		{
			INT nOffset=0, nLen=0, nNewValue=0;
			if( m_bHorizon )
			{
				nOffset = pMsg->pt.x - m_ptLastMouseClickView.x;
				nLen = m_nStickEndPos - m_nStickStartPos;
				m_nStickStartPos += nOffset;
				if( m_nStickStartPos < 0 )
					m_nStickStartPos = 0;

				if( m_nStickStartPos + nLen > m_ptSize.x )
					m_nStickStartPos = m_ptSize.x - nLen;

				m_nStickEndPos = m_nStickStartPos + nLen;
				m_ptLastMouseClickView.x = pMsg->pt.x;	// ʹ�϶�ƽ��

				if( m_ptSize.x-nLen != 0 )	// ���¼��㵱ǰֵ
					nNewValue = (m_nTotalValue-m_nVisibleValue)*m_nStickStartPos/(m_ptSize.x-nLen);
			}
			else
			{
				nOffset = pMsg->pt.y - m_ptLastMouseClickView.y;
				nLen = m_nStickEndPos - m_nStickStartPos;
				m_nStickStartPos += nOffset;
				if( m_nStickStartPos < 0 )
					m_nStickStartPos = 0;

				if( m_nStickStartPos + nLen > m_ptSize.y )
					m_nStickStartPos = m_ptSize.y - nLen;

				m_nStickEndPos = m_nStickStartPos + nLen;
				m_ptLastMouseClickView.y = pMsg->pt.y;	// ʹ�϶�ƽ��
				
				if( m_ptSize.y-nLen != 0 )	// ���¼��㵱ǰֵ
					nNewValue = (m_nTotalValue-m_nVisibleValue)*m_nStickStartPos/(m_ptSize.y-nLen);
			}

			if( m_nCurrentValue != nNewValue )
			{
				m_nCurrentValue = nNewValue;
				tagGUIEvent event(this, EGUIE_Scroll, pMsg);
				event.dwParam1 = (DWORD)m_nCurrentValue;
				event.dwParam2 = (DWORD)m_nTotalValue;
				event.dwParam3 = (DWORD)m_nVisibleValue;
				SendEvent(&event);
			}

			SetRefresh();
		}
		break;

	case WM_LBUTTONDOWN:
		if( PointInWnd(pMsg->pt) )
		{
			if( m_bHorizon )
			{
				// ���ڰ��ϲ���
				if( pMsg->pt.x - m_ptView.x < m_nStickStartPos || pMsg->pt.x - m_ptView.x > m_nStickEndPos )
				{
					INT nLen = m_nStickEndPos - m_nStickStartPos;
					INT nOffset = pMsg->pt.x - m_ptView.x - nLen/2;
					m_nStickStartPos = nOffset;
					if( m_nStickStartPos < 0 )
						m_nStickStartPos = 0;

					if( m_nStickStartPos + nLen > m_ptSize.x )
						m_nStickStartPos = m_ptSize.x - nLen;

					m_nStickEndPos = m_nStickStartPos + nLen;
					// ���¼��㵱ǰֵ
					INT nNewValue = 0;
					if( m_ptSize.x !=nLen ) 
						nNewValue = (m_nTotalValue-m_nVisibleValue)*m_nStickStartPos/(m_ptSize.x-nLen);
					if( m_nCurrentValue != nNewValue )
					{
						m_nCurrentValue = nNewValue;
						tagGUIEvent event(this, EGUIE_Scroll, pMsg);
						event.dwParam1 = (DWORD)m_nCurrentValue;
						event.dwParam2 = (DWORD)m_nTotalValue;
						event.dwParam3 = (DWORD)m_nVisibleValue;
						SendEvent(&event);
					}
					SetRefresh();
				}
			}
			else
			{
				 // ���ڰ��ϲ���
				if( pMsg->pt.y - m_ptView.y < m_nStickStartPos || pMsg->pt.y - m_ptView.y > m_nStickEndPos )
				{
					INT nLen = m_nStickEndPos - m_nStickStartPos;
					INT nOffset = pMsg->pt.y - m_ptView.y - nLen/2;
					m_nStickStartPos = nOffset;
					if( m_nStickStartPos < 0 )
						m_nStickStartPos = 0;

					if( m_nStickStartPos + nLen > m_ptSize.y )
						m_nStickStartPos = m_ptSize.y - nLen;

					m_nStickEndPos = m_nStickStartPos + nLen;
					// ���¼��㵱ǰֵ
					INT nNewValue = 0;
					if( m_ptSize.y !=nLen ) 
						nNewValue = (m_nTotalValue-m_nVisibleValue)*m_nStickStartPos/(m_ptSize.y-nLen);
					if( m_nCurrentValue != nNewValue )
					{
						m_nCurrentValue = nNewValue;
						tagGUIEvent event(this, EGUIE_Scroll, pMsg);
						event.dwParam1 = (DWORD)m_nCurrentValue;
						event.dwParam2 = (DWORD)m_nTotalValue;
						event.dwParam3 = (DWORD)m_nVisibleValue;
						SendEvent(&event);
					}
					SetRefresh();
				}
			}
		}
		break;
	}

	return GUIStatic::OnInputMessage(pMsg);
}


//-----------------------------------------------------------------------------
// ����������ĳ�����ʼλ�õ�
//-----------------------------------------------------------------------------
VOID GUIScrollBar::CalStick()
{
	INT nStickLen = 0;
	//SetRefresh();

	if( 0 == m_nTotalValue || m_nTotalValue <= m_nVisibleValue )
	{
		nStickLen = m_bHorizon ? m_ptSize.x : m_ptSize.y;
		m_nStickStartPos = 0;
		m_nStickEndPos = m_nStickStartPos + nStickLen;
		return;
	}

	if( !m_pTopPic || !m_pMidPic || !m_pBelowPic )
	{
		// �����������ʼ�ͽ���λ��
		if( !m_bHorizon )
		{
			nStickLen = m_nVisibleValue * m_ptSize.y / m_nTotalValue;
			if( nStickLen < (INT)m_ptSize.x )
				nStickLen = (INT)m_ptSize.x;

			m_nStickStartPos = m_nCurrentValue*(m_ptSize.y - nStickLen)
				/(m_nTotalValue-m_nVisibleValue);
		}
		else
		{
			nStickLen = m_nVisibleValue * m_ptSize.x / m_nTotalValue;
			if( nStickLen < (INT)m_ptSize.y )
				nStickLen = (INT)m_ptSize.y;

			m_nStickStartPos = m_nCurrentValue*(m_ptSize.x - nStickLen)
				/(m_nTotalValue-m_nVisibleValue);
		}

		m_nStickEndPos = m_nStickStartPos + nStickLen;
	}
	else
	{
		// �����������ʼ�ͽ���λ��
		if( !m_bHorizon )
		{
			nStickLen = m_nVisibleValue * m_ptSize.y / m_nTotalValue;
			if( nStickLen < (INT)m_pTopPic->ptSize.y )
				nStickLen = (INT)m_pTopPic->ptSize.y;

			m_nStickStartPos = m_nCurrentValue*(m_ptSize.y - nStickLen)
				/(m_nTotalValue-m_nVisibleValue);
		}
		else
		{
			nStickLen = m_nVisibleValue * m_ptSize.x / m_nTotalValue;
			if( nStickLen < (INT)m_pTopPic->ptSize.x )
				nStickLen = (INT)m_pTopPic->ptSize.x;

			m_nStickStartPos = m_nCurrentValue*(m_ptSize.x - nStickLen)
				/(m_nTotalValue-m_nVisibleValue);
		}

		m_nStickEndPos = m_nStickStartPos + nStickLen;

		if( m_bHorizon )
		{
			if( m_nStickStartPos + (INT)m_pTopPic->ptSize.x > m_ptSize.x )
				m_nStickStartPos = m_ptSize.x - m_pTopPic->ptSize.x;
		}
		else
		{
			if( m_nStickStartPos + (INT)m_pTopPic->ptSize.y > m_ptSize.y )
				m_nStickStartPos = m_ptSize.y - m_pTopPic->ptSize.y;
		}
	}
	SetRefresh(TRUE);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID GUIScrollBar::SendEvent(tagGUIEvent* pEvent)
{
	if( m_nTotalValue == 0 || m_ptSize.y == m_nStickEndPos - m_nStickStartPos )
		return GUIStatic::SendEvent(pEvent);

	GUIWnd* pWnd = m_pSystem->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return GUIStatic::SendEvent(pEvent);

	if( m_pUpButton == pWnd )
	{
		if( pEvent->eEvent == EGUIE_Click )
		{
			INT nLen = m_nStickEndPos - m_nStickStartPos;
			INT nNewValue = m_nCurrentValue - 1;
			if( m_bHorizon )
			{
				m_nStickStartPos = nNewValue*(m_ptSize.x - nLen)/(m_nTotalValue-m_nVisibleValue);

				if( m_nStickStartPos < 0 )	// ���¼��㵱ǰֵ,������
					nNewValue = m_nStickStartPos = 0;

				if( m_nStickStartPos + nLen > m_ptSize.x )	// ����
				{	
					m_nStickStartPos = m_ptSize.x - nLen;
					nNewValue = m_nTotalValue - m_nVisibleValue;
				}
			}
			else
			{
				m_nStickStartPos = nNewValue*(m_ptSize.y - nLen)/(m_nTotalValue-m_nVisibleValue);

				if( m_nStickStartPos < 0 )	// ���¼��㵱ǰֵ,������
					nNewValue = m_nStickStartPos = 0;

				if( m_nStickStartPos + nLen > m_ptSize.y )	// ����
				{	
					m_nStickStartPos = m_ptSize.y - nLen;
					nNewValue = m_nTotalValue - m_nVisibleValue;
				}
			}

			m_nStickEndPos = m_nStickStartPos + nLen;
			if( m_nCurrentValue != nNewValue )
			{
				m_nCurrentValue = nNewValue;
				tagGUIEvent event(this, EGUIE_Scroll, NULL);
				event.dwParam1 = (DWORD)m_nCurrentValue;
				event.dwParam2 = (DWORD)m_nTotalValue;
				event.dwParam3 = (DWORD)m_nVisibleValue;
				SendEvent(&event);
			}

			SetRefresh();
		}
	}
	else if( m_pDownButton == pWnd )
	{
		if( pEvent->eEvent == EGUIE_Click )
		{
			INT nLen = m_nStickEndPos - m_nStickStartPos;
			INT nNewValue = m_nCurrentValue+1;
			if( m_bHorizon )
			{
				m_nStickStartPos = nNewValue*(m_ptSize.x - nLen)/(m_nTotalValue-m_nVisibleValue);
				if( m_nStickStartPos < 0 )	// ���¼��㵱ǰֵ,������
					nNewValue = m_nStickStartPos = 0;

				if( m_nStickStartPos + nLen > m_ptSize.x )	// ����
				{
					m_nStickStartPos = m_ptSize.x - nLen;
					nNewValue = m_nTotalValue - m_nVisibleValue;
				}
			}
			else
			{
				m_nStickStartPos = nNewValue*(m_ptSize.y - nLen)/(m_nTotalValue-m_nVisibleValue);
				if( m_nStickStartPos < 0 )	// ���¼��㵱ǰֵ,������
					nNewValue = m_nStickStartPos = 0;

				if( m_nStickStartPos + nLen > m_ptSize.y )	// ����
				{
					m_nStickStartPos = m_ptSize.y - nLen;
					nNewValue = m_nTotalValue - m_nVisibleValue;
				}
			}

			m_nStickEndPos = m_nStickStartPos + nLen;
			if( m_nCurrentValue != nNewValue )
			{
				m_nCurrentValue = nNewValue;
				tagGUIEvent event(this, EGUIE_Scroll, NULL);
				event.dwParam1 = (DWORD)m_nCurrentValue;
				event.dwParam2 = (DWORD)m_nTotalValue;
				event.dwParam3 = (DWORD)m_nVisibleValue;
				SendEvent(&event);
			}

			SetRefresh();
		}
	}

	GUIStatic::SendEvent(pEvent);
}


//-----------------------------------------------------------------------------
// �������ѷ����¼�
//-----------------------------------------------------------------------------
VOID GUIScrollBar::OnEvent(tagGUIEvent* pEvent)
{
	if( EGUIE_Scroll == pEvent->eEvent )
		SetValue(pEvent->dwParam2, pEvent->dwParam3, pEvent->dwParam1);

	GUIStatic::OnEvent(pEvent);
}


//-----------------------------------------------------------------------------
// ����Ӵ���
//-----------------------------------------------------------------------------
VOID GUIScrollBar::AddChild(GUIWnd* pChild)
{
	if( 0 == pChild->GetClassName().compare(_T("CGUIButton")) )
	{
		if( !P_VALID(m_pUpButton) )
		{
			m_pUpButton = pChild;
		}
		else 
		{
			m_pDownButton = pChild;
			if( m_bHorizon )
			{
				if( m_pUpButton->GetClient().x > m_pDownButton->GetClient().x )
					std::swap(m_pUpButton, m_pDownButton);
			}
			else
			{
				if( m_pUpButton->GetClient().y > m_pDownButton->GetClient().y )
					std::swap(m_pUpButton, m_pDownButton);
			}
		}
	}

	GUIStatic::AddChild(pChild);
}


//-----------------------------------------------------------------------------
// ɾ���Ӵ���
//-----------------------------------------------------------------------------
VOID GUIScrollBar::DelChild(GUIWnd* pChild)
{
	if( pChild == m_pUpButton )
		m_pUpButton = (GUIWnd*)GT_INVALID;
	
	if( pChild == m_pDownButton )
		m_pDownButton = (GUIWnd*)GT_INVALID;

	GUIStatic::DelChild(pChild);
}



}	// namespace vEngine {
