//-----------------------------------------------------------------------------
//!\file gui_patch.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-06-15
//!
//!\brief ����ϵͳ patch �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_patch.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------
GUIPatch::GUIPatch()
{
	// ע�����пɱ༭����
	m_pVarMgr->Register(_T("TopPicFileName"), &m_strTopPic);
	m_pVarMgr->Register(_T("TopPicRect"), &m_rcTopPic);
	m_pVarMgr->Register(_T("MidPicFileName"), &m_strMidPic);
	m_pVarMgr->Register(_T("MidPicRect"), &m_rcMidPic);
	m_pVarMgr->Register(_T("DownPicFileName"), &m_strDownPic);
	m_pVarMgr->Register(_T("DownPicRect"), &m_rcDownPic);
	m_pVarMgr->Register(_T("LeftPicFileName"), &m_strLeftPic);
	m_pVarMgr->Register(_T("LeftPicRect"), &m_rcLeftPic);

	m_pVarMgr->Register(_T("RightPicFileName"), &m_strRightPic);
	m_pVarMgr->Register(_T("RightPicRect"), &m_rcRightPic);
	m_pVarMgr->Register(_T("LeftTopPicFileName"), &m_strLeftTopPic);
	m_pVarMgr->Register(_T("LeftTopPicRect"), &m_rcLeftTopPic);
	m_pVarMgr->Register(_T("RightTopPicFileName"), &m_strRightTopPic);
	m_pVarMgr->Register(_T("RightTopPicRect"), &m_rcRightTopPic);
	m_pVarMgr->Register(_T("LeftDownPicFileName"), &m_strLeftDownPic);
	m_pVarMgr->Register(_T("LeftDownPicRect"), &m_rcLeftDownPic);
	m_pVarMgr->Register(_T("RightDownPicFileName"), &m_strRightDownPic);
	m_pVarMgr->Register(_T("RightDownPicRect"), &m_rcRightDownPic);
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIPatch::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);
	if( FALSE == bResult )
		return FALSE;

	// ����ͼƬ
	m_pMidPic = m_pRender->CreateImage(m_strMidPic, m_rcMidPic);
	m_pTopPic = m_pRender->CreateImage(m_strTopPic, m_rcTopPic);
	m_pDownPic = m_pRender->CreateImage(m_strDownPic, m_rcDownPic);
	m_pLeftPic = m_pRender->CreateImage(m_strLeftPic, m_rcLeftPic);
	m_pRightPic = m_pRender->CreateImage(m_strRightPic, m_rcRightPic);
	m_pLeftTopPic = m_pRender->CreateImage(m_strLeftTopPic, m_rcLeftTopPic);
	m_pRightTopPic = m_pRender->CreateImage(m_strRightTopPic, m_rcRightTopPic);
	m_pLeftDownPic = m_pRender->CreateImage(m_strLeftDownPic, m_rcLeftDownPic);
	m_pRightDownPic = m_pRender->CreateImage(m_strRightDownPic, m_rcRightDownPic);
	return bResult;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIPatch::Destroy()
{
	// ɾ��ͼƬ
	m_pRender->DestroyImage(m_pMidPic);
	m_pRender->DestroyImage(m_pTopPic);
	m_pRender->DestroyImage(m_pDownPic);
	m_pRender->DestroyImage(m_pLeftPic);
	m_pRender->DestroyImage(m_pRightPic);
	m_pRender->DestroyImage(m_pLeftTopPic);
	m_pRender->DestroyImage(m_pRightTopPic);
	m_pRender->DestroyImage(m_pLeftDownPic);
	m_pRender->DestroyImage(m_pRightDownPic);

	GUIStatic::Destroy();
}


//-----------------------------------------------------------------------------
// ƴͼ���ڵ���Ⱦ
//-----------------------------------------------------------------------------
VOID GUIPatch::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// �ȼ���Ƿ���Ҫ��Ⱦ���ڵ�ͼ
	if( P_VALID(m_pPic) )
	{
		tagPoint ptBKTile = m_ptSize / m_pPic->ptSize;
		tagPoint ptAddon = m_ptSize - ptBKTile * m_pPic->ptSize;
		tagPoint ptOffset;
		tagRect rcSrcRight, rcSrcBottom;

		rcSrcRight.left = rcSrcRight.top = rcSrcBottom.left = rcSrcBottom.top = 0;
		rcSrcRight.right = ptAddon.x; 
		rcSrcRight.bottom = m_pPic->ptSize.y;
		rcSrcBottom.right = m_pPic->ptSize.x;
		rcSrcBottom.bottom = ptAddon.y;

		for(INT m=0; m<ptBKTile.y; m++)
		{
			m_pCanvas->rc.top = m_ptView.y + ptOffset.y;
			m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_pPic->ptSize.y;
			for(INT n=0; n<ptBKTile.x; n++)
			{
				m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
				m_pCanvas->rc.right = m_pCanvas->rc.left + m_pPic->ptSize.x;
				m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);
				ptOffset.x += m_pPic->ptSize.x;
			}

			if( ptAddon.x > 0 )	// �����ұߵİ��ͼ��
			{
				m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
				m_pCanvas->rc.right = m_pCanvas->rc.left + ptAddon.x;
				m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);
			}

			ptOffset.x = 0;
			ptOffset.y += m_pPic->ptSize.y;
		}

		if( ptAddon.y > 0 )	// �����±ߵİ��ͼ��
		{
			m_pCanvas->rc.top = m_ptView.y + ptOffset.y;
			m_pCanvas->rc.bottom = m_pCanvas->rc.top + ptAddon.y;

			for(INT n=0; n<ptBKTile.x; n++)
			{
				m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
				m_pCanvas->rc.right = m_pCanvas->rc.left + m_pPic->ptSize.x;
				m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);
				ptOffset.x += m_pPic->ptSize.x;
			}

			if( ptAddon.x > 0 ) // �������µİ��ͼ��
			{
				m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
				m_pCanvas->rc.right = m_pCanvas->rc.left + ptAddon.x;
				rcSrcBottom.right = ptAddon.x;
				m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);
			}
		}

		// ��Ⱦ����
		m_pCanvas->rc = m_rcText + m_ptView;
		m_pRender->Text(m_pCanvas, m_strText, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

		GUIWnd::Render();
		return;
	}


	if( !P_VALID(m_pMidPic) || !P_VALID(m_pTopPic) 
		|| !P_VALID(m_pDownPic) || !P_VALID(m_pLeftPic) 
		|| !P_VALID(m_pRightPic) || !P_VALID(m_pLeftTopPic) 
		|| !P_VALID(m_pRightTopPic) || !P_VALID(m_pLeftDownPic)
		|| !P_VALID(m_pRightDownPic) )
	{
		// ��Ⱦ����
		m_pCanvas->rc = m_rcText + m_ptView;
		m_pRender->Text(m_pCanvas, m_strText, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

		GUIWnd::Render();
		return;
	}


	// ����ƴ�Ӹ���
	tagPoint ptTile(1, 1);
	if( m_pMidPic->ptSize.x > 0 && m_pMidPic->ptSize.y > 0 )
		ptTile = (m_ptSize / m_pMidPic->ptSize) - 2;
	tagPoint ptAddon = m_ptSize - (ptTile+2) * m_pMidPic->ptSize;
	tagPoint ptOffset;
	tagRect rcSrc;

	// ���Ͻ�
	m_pCanvas->rc = m_pLeftTopPic->ptSize;
	m_pCanvas->rc = m_pCanvas->rc + m_ptView + ptOffset;
	m_pRender->Draw(m_pCanvas, m_pLeftTopPic, m_dwPicColor, m_eAlphaWrite);

	// �ϱ߿� 
	for(INT n=0; n<ptTile.x; n++)
	{
		ptOffset.x += m_pMidPic->ptSize.x;
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
		m_pRender->Draw(m_pCanvas, m_pTopPic, m_dwPicColor, m_eAlphaWrite);
	}

	ptOffset.x += m_pMidPic->ptSize.x;
	// ����ϱ߿�
	if( ptAddon.x > 0 )
	{
		m_pTopPic->rc.left = m_rcTopPic.left, 
		m_pTopPic->rc.top = m_rcTopPic.top, 
		m_pTopPic->rc.right = m_pTopPic->rc.left + ptAddon.x, 
		m_pTopPic->rc.bottom = m_rcTopPic.bottom;
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.right = m_pCanvas->rc.left + ptAddon.x;
		m_pRender->Draw(m_pCanvas, m_pTopPic, m_dwPicColor, m_eAlphaWrite);
		m_pTopPic->rc = m_rcTopPic;
		ptOffset.x += ptAddon.x;
	}
    
	// ���Ͻ�
	m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
	m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
	m_pRender->Draw(m_pCanvas, m_pRightTopPic, m_dwPicColor, m_eAlphaWrite);

	// ���м䲿��
	for(INT m=0; m<ptTile.y; m++)
	{
		ptOffset.x = 0;
		ptOffset.y += m_pMidPic->ptSize.y;
		
		// ����߿�
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.top = m_ptView.y + ptOffset.y;
		m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
		m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_pMidPic->ptSize.y;
		m_pRender->Draw(m_pCanvas, m_pLeftPic, m_dwPicColor, m_eAlphaWrite);

		
		// ���м�
		for(INT n=0; n<ptTile.x; n++)
		{
			ptOffset.x += m_pMidPic->ptSize.x;
			m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
			m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
			m_pRender->Draw(m_pCanvas, m_pMidPic, m_dwPicColor, m_eAlphaWrite);
		}

		
		ptOffset.x += m_pMidPic->ptSize.x;
		// ���м�����Ұ��
		if( ptAddon.x > 0 )
		{
			m_pMidPic->rc.left = m_rcMidPic.left; 
			m_pMidPic->rc.top = m_rcMidPic.top; 
			m_pMidPic->rc.right = m_pMidPic->rc.left + ptAddon.x; 
			m_pMidPic->rc.bottom = m_rcMidPic.bottom;
			m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
			m_pCanvas->rc.right = m_pCanvas->rc.left + ptAddon.x;
			m_pRender->Draw(m_pCanvas, m_pMidPic, m_dwPicColor, m_eAlphaWrite);
			m_pMidPic->rc = m_rcMidPic;
			ptOffset.x += ptAddon.x;
		}

		// ���ұ߿�
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
		m_pRender->Draw(m_pCanvas, m_pRightPic, m_dwPicColor, m_eAlphaWrite);
	}

	ptOffset.y += m_pMidPic->ptSize.y;
	// ���м����°��
	if( ptAddon.y > 0 )
	{
		ptOffset.x = 0;
		// ����߿�
		m_pLeftPic->rc.left = m_rcLeftPic.left; 
		m_pLeftPic->rc.top = m_rcLeftPic.top; 
		m_pLeftPic->rc.right = m_rcLeftPic.right; 
		m_pLeftPic->rc.bottom = m_pLeftPic->rc.top + ptAddon.y;
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.top = m_ptView.y + ptOffset.y;
		m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
		m_pCanvas->rc.bottom = m_pCanvas->rc.top + ptAddon.y;
		m_pRender->Draw(m_pCanvas, m_pLeftPic, m_dwPicColor, m_eAlphaWrite);
		m_pLeftPic->rc = m_rcLeftPic;

		// ���м�
		for(INT n=0; n<ptTile.x; n++)
		{
			ptOffset.x += m_pMidPic->ptSize.x;
			m_pMidPic->rc.left = m_rcMidPic.left;
			m_pMidPic->rc.top = m_rcMidPic.top;
			m_pMidPic->rc.right = m_rcMidPic.right;
			m_pMidPic->rc.bottom = m_pMidPic->rc.top + ptAddon.y;
			m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
			m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
			m_pRender->Draw(m_pCanvas, m_pMidPic, m_dwPicColor, m_eAlphaWrite);
			m_pMidPic->rc = m_rcMidPic;
		}

		ptOffset.x += m_pMidPic->ptSize.x;
		// ���м�����Ұ��
		if( ptAddon.x > 0 )
		{
			m_pMidPic->rc.left = m_rcMidPic.left; 
			m_pMidPic->rc.top = m_rcMidPic.top; 
			m_pMidPic->rc.right = m_pMidPic->rc.left + ptAddon.x; 
			m_pMidPic->rc.bottom = m_pMidPic->rc.top + ptAddon.y;
			m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
			m_pCanvas->rc.right = m_pCanvas->rc.left + ptAddon.x;
			m_pRender->Draw(m_pCanvas, m_pMidPic, m_dwPicColor, m_eAlphaWrite);
			m_pMidPic->rc = m_rcMidPic;
			ptOffset.x += ptAddon.x;
		}

		// ���ұ߿�
		m_pRightPic->rc.left = m_rcRightPic.left; 
		m_pRightPic->rc.top = m_rcRightPic.top; 
		m_pRightPic->rc.right = m_rcRightPic.right; 
		m_pRightPic->rc.bottom = m_pRightPic->rc.top + ptAddon.y;
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
		m_pRender->Draw(m_pCanvas, m_pRightPic, m_dwPicColor, m_eAlphaWrite);
		m_pRightPic->rc = m_rcRightPic;
		ptOffset.y += ptAddon.y;
	}


	// ���½�
	ptOffset.x = 0;
	m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
	m_pCanvas->rc.top = m_ptView.y + ptOffset.y;
	m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
	m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_pMidPic->ptSize.y;
	m_pRender->Draw(m_pCanvas, m_pLeftDownPic, m_dwPicColor, m_eAlphaWrite);

	// �±߿� 
	for(INT n=0; n<ptTile.x; n++)
	{
		ptOffset.x += m_pMidPic->ptSize.x;
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
		m_pRender->Draw(m_pCanvas, m_pDownPic, m_dwPicColor, m_eAlphaWrite);
	}
    ptOffset.x += m_pMidPic->ptSize.x;

	// ����±߿�
	if( ptAddon.x > 0 )
	{
		m_pDownPic->rc.left = m_rcDownPic.left, 
		m_pDownPic->rc.top = m_rcDownPic.top, 
		m_pDownPic->rc.right = m_pDownPic->rc.left + ptAddon.x, 
		m_pDownPic->rc.bottom = m_rcDownPic.bottom;
		m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
		m_pCanvas->rc.right = m_pCanvas->rc.left + ptAddon.x;
		m_pRender->Draw(m_pCanvas, m_pDownPic, m_dwPicColor, m_eAlphaWrite);
		m_pDownPic->rc = m_rcDownPic;
		ptOffset.x += ptAddon.x;
	}
    
	// ���½�
	m_pCanvas->rc.left = m_ptView.x + ptOffset.x;
	m_pCanvas->rc.right = m_pCanvas->rc.left + m_pMidPic->ptSize.x;
	m_pRender->Draw(m_pCanvas, m_pRightDownPic, m_dwPicColor, m_eAlphaWrite);

	// ��Ⱦ����
	m_pCanvas->rc = m_rcText + m_ptView;
	m_pRender->Text(m_pCanvas, m_strText, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

	// ���û�����Ⱦ
	GUIWnd::Render();
}

}	// namespace vEngine {
