//-----------------------------------------------------------------------------
//!\file gui_window.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-05-15
//!
//!\brief ����ϵͳ window �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_window.h"

#include "gui_button.h"
#include "gui_event_define.h"
#include "gui_scrollbar.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------
GUIWindow::GUIWindow()
{
	// ע�����пɱ༭����
	m_pVarMgr->Register(_T("CloseButtonName"), &m_strCloseButton);
	m_pVarMgr->Register(_T("TrulyClose"), &m_bRealDestroty, _T("no yes"));
	m_pVarMgr->Register(_T("NotUseRenderTarget"), &m_bNotUseRenderTarget, _T("no yes"));
	m_pVarMgr->Register(_T("Scroll"), &m_bScroll, _T("no yes"));
	m_pVarMgr->Register(_T("PopupSound"), &m_strPopupSound);
	m_pVarMgr->Register(_T("NoPopupEffect"), &m_bNoPopupEffect, _T("no yes"));

	m_pCanvas = NULL;
	m_pNewCanvas = NULL;
	m_pScrollBar = NULL;
	m_nCurrentY = 0;
	m_fZ = 1.0f;
	m_strPopupSound = _T("window_popup");	// Ĭ��ֵ
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIWindow::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	GUIWnd::Init(pSystem, pFather, pXmlElement);
	
	if( m_bScroll )	// ����������
		m_bNotUseRenderTarget = TRUE;

	m_ptCurrentSize = m_ptSize; // ��ǰ��С
	m_ptCurrentView = m_ptView; // ��ǰλ��
	m_fCurrentSizeRatio = 1.0f;	// ��ǰ��С����
	m_nPopProgress = 0;			// �Ƿ��ڷŴ�״̬ 1�Ŵ� 2��С 0����
	m_dwPopStartTime = 0;

	if( !m_bNotUseRenderTarget )
	{
		tagRect rc;
		rc = m_ptSize;
		m_pNewCanvas = m_pRender->CreateImage(m_strFullName, rc);
		if( P_VALID(m_pNewCanvas) )	
		{
			m_pNewCanvas->ptOffset.x = m_ptView.x;
			m_pNewCanvas->ptOffset.y = m_ptView.y;
			m_pNewCanvas->bRefresh = TRUE;
			if( !m_bNoPopupEffect )
			{
				m_ptCurrentSize.Clear();// ��ǰ��С
				m_ptCurrentView.x = m_ptView.x + (m_ptSize.x - m_ptCurrentSize.x) / 2; // ��ǰλ��
				m_ptCurrentView.y = m_ptView.y + (m_ptSize.y - m_ptCurrentSize.y) / 2; // ��ǰλ��
				m_fCurrentSizeRatio=0;	// ��ǰ��С����
				m_nPopProgress=1;		// �Ƿ��ڷŴ�״̬ 1�Ŵ� 2��С 0����
				m_dwPopStartTime = timeGetTime();
			}
		}
		else
		{
			// ����RenderTargetʧ��
			m_bNotUseRenderTarget = TRUE;
		}
	}

// 	if( !m_strPopupSound.empty() )
// 		m_pRender->PlaySound(m_strPopupSound.c_str(), 0.4f);

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIWindow::Destroy()
{
	GUIWnd::Destroy();

	// ɾ������
	if( !m_bNotUseRenderTarget )
		m_pRender->ReleaseRenderTarget(m_pNewCanvas->dwHandle);
	m_pRender->DestroyImage(m_pNewCanvas);
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIWindow::Render()
{
	if( m_bNotUseRenderTarget || m_bInvisible )
	{
		if( m_bScroll )
		{
			tagRect rect;
			rect.SetLTWH(m_ptView, m_ptSize);
			rect = rect - m_pCanvas->ptOffset;
			m_pRender->SetScissorRect(&rect);
		}
		GUIWnd::Render();
		if( m_bScroll )
			m_pRender->SetScissorRect(NULL);
		return;
	}

	if( m_bRefresh || (P_VALID(m_pCanvas->dwHandle)&&m_pCanvas->bRefresh) )
		m_pNewCanvas->bRefresh = TRUE;

	m_pRender->PushRenderTarget(m_pNewCanvas);

	// ���û�����Ⱦ
	if( m_pNewCanvas->bRefresh )
		m_pRender->ClearRenderTarget();

	GUIWnd::Render();
	m_pRender->PopRenderTarget(m_pNewCanvas);

	// ��Ⱦ�������
	if( 1 == m_nPopProgress )
		m_pCanvas->rc.SetLTWH(m_ptCurrentView, m_ptCurrentSize);
	else
			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pNewCanvas->rc.SetLTWH(0, 0, m_ptSize.x, m_ptSize.y);
	m_pRender->Draw(m_pCanvas, m_pNewCanvas, 0xFFFFFFFF, EGUIAW_Write, m_fZ);
	
	m_pNewCanvas->bRefresh = FALSE;
	m_bRefresh = FALSE;
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID GUIWindow::Update()
{
	GUIWnd::Update();

	if( 1 == m_nPopProgress )
	{
		m_fCurrentSizeRatio += (FLOAT)(timeGetTime() - m_dwPopStartTime)/300.0f;
		if( m_fCurrentSizeRatio > 1.0f )
		{
			m_nPopProgress = 0;
			m_fCurrentSizeRatio = 1.0f;
			m_ptCurrentSize = m_ptSize;
			m_ptCurrentView = m_ptView; 
		}
		else
		{
			m_ptCurrentSize.x = (LONG)((FLOAT)m_ptSize.x * m_fCurrentSizeRatio);
			m_ptCurrentSize.y = (LONG)((FLOAT)m_ptSize.y * m_fCurrentSizeRatio);
			m_ptCurrentView.x = m_ptView.x + (m_ptSize.x - m_ptCurrentSize.x) / 2; // ��ǰλ��
			m_ptCurrentView.y = m_ptView.y + (m_ptSize.y - m_ptCurrentSize.y) / 2; // ��ǰλ��
		}
	}
}



//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
BOOL GUIWindow::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch( pMsg->dwType )
	{
	case WM_MOUSEWHEEL:	// ����������
		if( pMsg->dwParam1 & 0x80000000 )	// ����
			ScrollChild(m_nCurrentY + 20);
		else
			ScrollChild(m_nCurrentY - 20);
		if( P_VALID(m_pScrollBar) )
			m_pScrollBar->SetValue(m_nCurrentY);
		break;

	case WM_KEYDOWN:
		switch(pMsg->dwParam1)
		{
		case VK_UP:
			ScrollChild(m_nCurrentY - 20);
			if( P_VALID(m_pScrollBar) )
				m_pScrollBar->SetValue(m_nCurrentY);
			return TRUE;
		case VK_DOWN:
			ScrollChild(m_nCurrentY + 20);
			if( P_VALID(m_pScrollBar) )
				m_pScrollBar->SetValue(m_nCurrentY);
			return TRUE;
		}
		break;
	}

	return GUIWnd::OnInputMessage(pMsg);
}


//-----------------------------------------------------------------------------
// ���¼��������ص������������ϴ���,����TRUE��ʾ��Ϣ���ⲿ�ص���������
//-----------------------------------------------------------------------------
VOID GUIWindow::SendEvent(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pSystem->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return GUIWnd::SendEvent(pEvent);

	switch(pEvent->eEvent)
	{
	case EGUIE_Click:	// �رհ�ť���
		if( pWnd == this->GetChild(m_strCloseButton.c_str()) )
		{
			this->SetInvisible(true);
			if( m_bRealDestroty )
				m_pSystem->AddToDestroyList(this);
		}
		break;

	case EGUIE_Scroll:
		{
			if( pWnd == (GUIWnd*)m_pScrollBar )
				ScrollChild((INT)pEvent->dwParam1);
		}
		break;

	case EGUIE_Resize:
		{
			if( pWnd != (GUIWnd*)m_pScrollBar )
				this->ReCalScroll();
		}
		break;
	}

	return GUIWnd::SendEvent(pEvent);
}



//-----------------------------------------------------------------------------
// ָ�����Ƿ������Ǵ�����
//-----------------------------------------------------------------------------
BOOL GUIWindow::PointInWnd(tagPoint& pt, bool bCheckChild)
{
	if( m_bInvisible )
		return FALSE;

	tagRect rc;
	rc.SetLTWH(m_ptView, m_ptSize);
	if( !m_bInvisible && rc.IsContain(pt) )
		return TRUE;

	// �ټ����Ƿ������ǵ������Ӵ�����
	if( bCheckChild && m_bNotUseRenderTarget && !m_bScroll )
	{
		std::list<GUIWnd*>::iterator it;
		for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
		{
			if( TRUE == (*it)->PointInWnd(pt, bCheckChild) )
				return TRUE;
		}
	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// �ƶ��ؼ�,����������
//-----------------------------------------------------------------------------
BOOL GUIWindow::Move(tagPoint& ptClient)
{
	if( m_bNotUseRenderTarget )
		return GUIWnd::Move(ptClient);

	BOOL bRefresh = m_bRefresh;
	BOOL bResult = GUIWnd::Move(ptClient);
	m_bRefresh = bRefresh;	// windows�ؼ��ƶ�����Ҫ�ػ�����

	return bResult;
}

//-----------------------------------------------------------------------------
// ���ؼ�client����ת������Ϸ���ڵ��߼�����(view)
//-----------------------------------------------------------------------------
VOID GUIWindow::RefreshViewCoord()
{
	GUIWnd::RefreshViewCoord();
	if( !m_bNotUseRenderTarget )
	{
		m_pNewCanvas->ptOffset.x = m_ptView.x;
		m_pNewCanvas->ptOffset.y = m_ptView.y;
	}
}


//-----------------------------------------------------------------------------
// ����Ӵ���
//-----------------------------------------------------------------------------
VOID GUIWindow::AddChild(GUIWnd* pChild)
{
	GUIWnd::AddChild(pChild);

	if( !m_bNotUseRenderTarget )
	{
		pChild->SetCanvas(m_pNewCanvas);
		m_pNewCanvas->bRefresh = TRUE;
	}

	if( pChild->GetClassName() == _T("CGUIScrollBar") )
		m_pScrollBar = (GUIScrollBar*)pChild;
	else
		this->ReCalScroll();
}


//-----------------------------------------------------------------------------
// ɾ���Ӵ���
//-----------------------------------------------------------------------------
VOID GUIWindow::DelChild(GUIWnd* pChild)
{
	GUIWnd::DelChild(pChild);
	
	if( !m_bNotUseRenderTarget )
		m_pNewCanvas->bRefresh = TRUE;

	if( pChild == (GUIWnd*)m_pScrollBar )
		m_pScrollBar = NULL;
}


//-----------------------------------------------------------------------------
// ���ô����Ƿ�����
//-----------------------------------------------------------------------------
VOID GUIWindow::SetInvisible(BOOL b)
{
	if( m_bInvisible == b )	// ��ֹ�ظ�����ɾ��
		return;

	GUIWnd::SetInvisible(b);

	if( b )
	{
		if( !m_bNotUseRenderTarget )
		{
			// ɾ������,�ͷ�render target
			m_pRender->ReleaseRenderTarget(m_pNewCanvas->dwHandle);
			m_pNewCanvas->dwHandle = GT_INVALID;
		}
	}
	else
	{
		if( !m_strPopupSound.empty() )
			m_pRender->PlaySound(m_strPopupSound.c_str(), 0.4f);

		if( !m_bNotUseRenderTarget )
		{
			tagRect rc;
			rc = m_ptSize;
			m_pNewCanvas->dwHandle = m_pRender->CreateRenderTarget(rc);
			if( !GT_VALID(m_pNewCanvas->dwHandle) )	
			{
				// ����RenderTargetʧ��
				m_bNotUseRenderTarget = TRUE;
			}
			else
			{
				if( !m_bNoPopupEffect )
				{
					m_ptCurrentSize.Clear();// ��ǰ��С
					m_ptCurrentView.x = m_ptView.x + (m_ptSize.x - m_ptCurrentSize.x) / 2; // ��ǰλ��
					m_ptCurrentView.y = m_ptView.y + (m_ptSize.y - m_ptCurrentSize.y) / 2; // ��ǰλ��
					m_fCurrentSizeRatio=0;	// ��ǰ��С����
					m_nPopProgress=1;		// �Ƿ��ڷŴ�״̬ 1�Ŵ� 2��С 0����
					m_dwPopStartTime = timeGetTime();
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// ���¼���Scroll����
//-----------------------------------------------------------------------------
VOID GUIWindow::ReCalScroll()
{
	if( !m_bScroll )
		return;

	// �����С�����ֵ
	m_nMinY = m_nMaxY = 0;
	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
	{
		GUIWnd* pWnd = *it;
		if( pWnd == (GUIWnd*)m_pScrollBar )
			continue;

		const tagPoint& ptClient = pWnd->GetClient();
		const tagPoint& ptSize = pWnd->GetSize();

		if( ptClient.y < m_nMinY )
			m_nMinY = ptClient.y;

		if( ptClient.y + ptSize.y > m_nMaxY )
			m_nMaxY = ptClient.y + ptSize.y;
	}

	// ������ǰֵ
	if( m_nCurrentY < m_nMinY )
		m_nCurrentY = m_nMinY;
	if( m_nCurrentY > m_nMaxY )
		m_nCurrentY = m_nMaxY;

	if( m_pScrollBar )
	{
		// ����������Ϣ
		tagGUIEvent event(this, EGUIE_Scroll, NULL);
		event.dwParam1 = (DWORD)m_nCurrentY;
		event.dwParam2 = (DWORD)(((m_nMaxY - m_nMinY)>(INT)m_ptSize.y)?m_nMaxY - m_nMinY:m_ptSize.y);
		event.dwParam3 = (DWORD)m_ptSize.y;
		OnEvent(&event);
	}

}


//-----------------------------------------------------------------------------
// �����ӿؼ�
//-----------------------------------------------------------------------------
VOID GUIWindow::ScrollChild( INT nY )
{
	if( !m_bScroll )
		return;

	if( m_nMaxY - m_nMinY <= (INT)m_ptSize.y )
		return;

	INT nMax = m_nMaxY - m_nMinY - (INT)m_ptSize.y;

	if( nY < 0 )
		nY = 0;
	if( nY > nMax )
		nY = nMax;

	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
	{
		GUIWnd* pWnd = *it;
		if( pWnd == (GUIWnd*)m_pScrollBar )
			continue;

		tagPoint ptClient;
		ptClient.y = m_nCurrentY - nY;
		BOOL bOrigin = pWnd->IsMovable();
		pWnd->SetMovable(TRUE);
		pWnd->Move(ptClient);
		pWnd->SetMovable(bOrigin);
	}

	m_nCurrentY = nY;
}

}	// namespace vEngine {
