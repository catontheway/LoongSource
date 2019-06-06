//-----------------------------------------------------------------------------
//!\file gui_tree.cpp
//!\author Lyp
//!
//!\date 2007-05-17
//! last 2005-05-17
//!
//!\brief ����ϵͳ tree �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "gui_tree.h"
#include "gui_event_define.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
GUITree::GUITree()
{
	m_pVarMgr->Register(_T("RowHeight"), &m_nRowHeight);
	m_pVarMgr->Register(_T("SelectStickColor"), &m_dwSelectStickColor);
	m_pVarMgr->Register(_T("PicInterval"), &m_dwPicInterval);
	m_pVarMgr->Register(_T("PicTextInterval"), &m_dwPicTextInterval);

	m_pVarMgr->Register(_T("PlusPicFileName"), &m_strPlusPic);
	m_pVarMgr->Register(_T("PlusPicRect"), &m_rcPlusPic);
	m_pVarMgr->Register(_T("PlusPicColor"), &m_dwPlusPicColor);
	m_pVarMgr->Register(_T("MinusPicFileName"), &m_strMinusPic);
	m_pVarMgr->Register(_T("MinusPicRect"), &m_rcMinusPic);
	m_pVarMgr->Register(_T("MinusPicColor"), &m_dwMinusPicColor);
	m_pVarMgr->Register(_T("BagPicFileName"), &m_strBagPic);
	m_pVarMgr->Register(_T("BagPicRect"), &m_rcBagPic);
	m_pVarMgr->Register(_T("BagPicColor"), &m_dwBagPicColor);

	m_nRowHeight = 16;
	m_dwSelectStickColor = 0xb0808080;
	m_dwPlusPicColor = 0xffffffff;
	m_dwMinusPicColor = 0xffffffff;
	m_dwBagPicColor = 0xffffffff; 
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUITree::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);
	if( FALSE == bResult )
		return bResult;
	
	DWORD dwWidth=0, dwHeight=0;
	m_pPlusPic = m_pRender->CreateImage(m_strPlusPic, m_rcPlusPic);
	m_pMinusPic = m_pRender->CreateImage(m_strMinusPic, m_rcMinusPic);
	m_pBagPic = m_pRender->CreateImage(m_strBagPic, m_rcBagPic);

	m_mapItem.Clear();
	m_pRootItem = 0;		// ���ڵ�
	m_pCurSelItem = 0;		// ��ǰѡ�еĽڵ�
	m_nCurrentTextRow = 0;	// ��ǰ��ʾ�ĵ�һ����ʵ�ʵĶ�����
	m_nTotalRow = 0;		// ������

	return bResult;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUITree::Destroy()
{
	tagGUITreeItem* pItem = NULL;
	m_mapItem.ResetIterator();
	while(m_mapItem.PeekNext(pItem))
		delete(pItem);

	GUIStatic::Destroy();

	// ɾ��ͼƬ
	m_pRender->DestroyImage(m_pPlusPic);
	m_pRender->DestroyImage(m_pMinusPic);
	m_pRender->DestroyImage(m_pBagPic);
}


//-----------------------------------------------------------------------------
// render
//-----------------------------------------------------------------------------
VOID GUITree::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	m_listVisibleItem.Clear();

	INT nCurRow = 0, nCurY = 0;
	if( P_VALID(m_pRootItem) )	// �Ƿ��и��ؼ�
		Render(m_pRootItem, nCurRow, nCurY);

	// ���û�����Ⱦ
	GUIWnd::Render();
}


//-----------------------------------------------------------------------------
// ��Ⱦ��������
//-----------------------------------------------------------------------------
VOID GUITree::Render(tagGUITreeItem* pItem, INT& nCurRow, INT& nCurY)
{
	if( FALSE == P_VALID(pItem) )
		return;

	if( nCurY >= m_ptSize.y )
		return;

	if( !P_VALID(m_pPlusPic) || !P_VALID(m_pMinusPic) )
		return;

	// ��Ⱦ�Լ�
	if( nCurRow >= m_nCurrentTextRow )
	{
		INT nOffset = m_pPlusPic->ptSize.x + m_dwPicInterval;
		if( !pItem->listChildren.empty() )
		{
			m_pCanvas->rc.Set(m_ptView.x + m_rcText.left + pItem->nLayer*16, 
				m_ptView.y + nCurY + (m_nRowHeight - m_pPlusPic->ptSize.y)/2, 
				m_ptView.x + m_rcText.left + pItem->nLayer*16 + m_pPlusPic->ptSize.x, 
				m_ptView.y + nCurY + (m_nRowHeight - m_pPlusPic->ptSize.y)/2 + m_pPlusPic->ptSize.y);
			if( !pItem->bExpended )
				m_pRender->Draw(m_pCanvas, m_pPlusPic, m_dwPlusPicColor, m_eAlphaWrite);
			else
				m_pRender->Draw(m_pCanvas, m_pMinusPic, m_dwMinusPicColor, m_eAlphaWrite);
			pItem->rcPlus = m_pCanvas->rc;
		}
		else
			pItem->rcPlus.Clear();
		

		if( P_VALID(m_pBagPic) )	// ��Ⱦ����ǰ��ͼƬ
		{
			m_pCanvas->rc.Set(m_ptView.x + m_rcText.left + pItem->nLayer*16 + nOffset, 
				m_ptView.y + nCurY + (m_nRowHeight - m_pBagPic->ptSize.y)/2, 
				m_ptView.x + m_rcText.left + pItem->nLayer*16 + nOffset + m_pBagPic->ptSize.x, 
				m_ptView.y + nCurY + (m_nRowHeight - m_pBagPic->ptSize.y)/2 + m_pBagPic->ptSize.y);
			m_pRender->Draw(m_pCanvas, m_pBagPic, m_dwBagPicColor, m_eAlphaWrite);
			nOffset += m_pBagPic->ptSize.x + m_dwPicTextInterval;
		}

		m_pCanvas->rc.Set(m_ptView.x + m_rcText.left + pItem->nLayer*16 + nOffset, 
			m_ptView.y + nCurY + m_rcText.top, m_ptView.x + m_rcText.right, m_ptView.y + nCurY + m_nRowHeight);
		if( m_pCanvas->rc.bottom > m_ptView.y + m_rcText.bottom )
			m_pCanvas->rc.bottom = m_ptView.y + m_rcText.bottom;

		if( m_pCanvas->rc.left + pItem->nTextWidth < m_pCanvas->rc.right )
			m_pCanvas->rc.right = m_pCanvas->rc.left + pItem->nTextWidth;

		if( m_pCurSelItem == pItem )	// ��Ⱦѡ��
		{
			// ��ɫ(Alphaֵ����)
			DWORD dwColor = ((~pItem->dwColor)&0x00ffffff)|(pItem->dwColor&0xff000000);
			DWORD dwShadeColor = ((~m_dwShadeColor)&0x00ffffff)|(m_dwShadeColor&0xff000000); 
			m_pRender->Draw(m_pCanvas, NULL, pItem->dwColor, m_eAlphaWrite);
			m_pRender->Text(m_pCanvas, pItem->strShow, m_pFont, dwColor, dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
		}
		else
		{
			m_pRender->Text(m_pCanvas, pItem->strShow, m_pFont, pItem->dwColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
		}

		nCurY+=m_nRowHeight;
		if( P_VALID(m_pBagPic) )
			m_pCanvas->rc.left -= m_pBagPic->ptSize.x + m_dwPicTextInterval;
		pItem->rcText = m_pCanvas->rc;
		m_listVisibleItem.PushBack(pItem);
	}
	nCurRow++;

	if( pItem->bExpended )
	{
		std::list<tagGUITreeItem*>::iterator it;
		for(it=pItem->listChildren.begin(); it!=pItem->listChildren.end(); ++it)
			Render((*it), nCurRow, nCurY);
	}
}



//-----------------------------------------------------------------------------
// OnInputMessage
//-----------------------------------------------------------------------------
BOOL GUITree::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:	// ѡ��
	case WM_LBUTTONDBLCLK:
		if( PointInWnd(pMsg->pt) )
		{
			tagGUITreeItem* pItem;
			m_listVisibleItem.ResetIterator();
			while(m_listVisibleItem.PeekNext(pItem))
			{
				if( pItem->rcText.IsContain(pMsg->pt) )
				{
					m_pCurSelItem = pItem;
					if( WM_LBUTTONDBLCLK == pMsg->dwType )
						pItem->bExpended = true;	// ˫��չ��
					m_nTotalRow = CalItemTakeupRow(m_pRootItem);
					SetCurrentTextRow(m_nCurrentTextRow);	// �ù�����ˢ��

					// �����¼�
					tagGUIEvent event(this, EGUIE_ItemClick, pMsg);
					event.dwParam1 = m_pUtil->Crc32(pItem->strName.c_str());
					SendEvent(&event);
					break;
				}

				if( pItem->rcPlus.IsContain(pMsg->pt) )
				{
					pItem->bExpended = !pItem->bExpended;
					m_nTotalRow = CalItemTakeupRow(m_pRootItem);
					SetCurrentTextRow(m_nCurrentTextRow);	// �ù�����ˢ��
					break;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:	// ����������
		if( pMsg->dwParam1 & 0x80000000 )	// ����
			this->SetCurrentTextRow(m_nCurrentTextRow+1);
		else
			this->SetCurrentTextRow(m_nCurrentTextRow-1);
		break;

	case WM_KEYDOWN:
		switch(pMsg->dwParam1)
		{
		case VK_UP:
			SetCurrentTextRow(m_nCurrentTextRow-1);
			return TRUE;
		case VK_DOWN:
			SetCurrentTextRow(m_nCurrentTextRow+1);
			return TRUE;
		case VK_PRIOR:	// ��ǰ��ҳ
			if( FALSE == SetCurrentTextRow(m_nCurrentTextRow-m_ptSize.y / m_nRowHeight) )
				SetCurrentTextRow(0);
			return TRUE;
		case VK_NEXT:	// ���ҳ
			if( FALSE == SetCurrentTextRow(m_nCurrentTextRow+m_ptSize.y / m_nRowHeight) )
				SetCurrentTextRow(m_nTotalRow - m_ptSize.y / m_nRowHeight);	// �������1��
			return TRUE;
		}
		break;
	}

	return GUIStatic::OnInputMessage(pMsg);
}


//-----------------------------------------------------------------------------
// ���¼��������ص������������ϴ���,����TRUE��ʾ��Ϣ���ⲿ�ص���������
//-----------------------------------------------------------------------------
VOID GUITree::SendEvent(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pSystem->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return;

	switch( pEvent->eEvent ) 
	{
	case EGUIE_Scroll:
		if( pWnd != this )
			SetCurrentTextRow(pEvent->dwParam1, FALSE);
		break;
	}

	GUIWnd::SendEvent(pEvent);
}



//-----------------------------------------------------------------------------
// ���õ�ǰ��ʾ�ĵ�һ����ʵ�ʵ���һ��
//-----------------------------------------------------------------------------
BOOL GUITree::SetCurrentTextRow(INT nY, BOOL bSendEvent)
{
	this->SetRefresh();

	if( nY < 0 )
		nY = 0;

	INT nMax = m_nTotalRow - m_ptSize.y / m_nRowHeight;
	if( nMax < 0 )
		nMax = 0;

	if( nY > nMax )
		nY = nMax;

	m_nCurrentTextRow = nY;
	if( bSendEvent )	// ����������Ϣ
	{
		tagGUIEvent event(this, EGUIE_Scroll, NULL);
		event.dwParam1 = (DWORD)nY;
		event.dwParam2 = (DWORD)m_nTotalRow;
		event.dwParam3 = (DWORD)m_ptSize.y / m_nRowHeight;
		OnEvent(&event);
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// ���õ�ǰѡ�е�item,������ItemClick�¼�
//-----------------------------------------------------------------------------
BOOL GUITree::SetCurrentSelItem(DWORD dwID, BOOL bSendEvent)
{
	tagGUITreeItem* pItem = m_mapItem.Peek(dwID);
	if( FALSE == P_VALID(pItem) )
		return FALSE;

	this->SetRefresh();
	m_pCurSelItem = pItem;
	pItem->bExpended = true;

	// չ�����и��ڵ�
	tagGUITreeItem* pFather = pItem->pFather;
	while( pFather )
	{
		pFather->bExpended = true;
		pFather = pFather->pFather;
	}

	m_nTotalRow = CalItemTakeupRow(m_pRootItem);
	SetCurrentTextRow(CalItemStartRow(pItem));	// �ù�����ˢ��

	// �����¼�
	if( bSendEvent )
	{
		tagGUIEvent event(this, EGUIE_ItemClick, NULL);
		event.dwParam1 = m_pUtil->Crc32(pItem->strName.c_str());
		SendEvent(&event);
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// �õ���ǰѡ�нڵ�
//-----------------------------------------------------------------------------
DWORD GUITree::GetCurrentSelItem()
{
	if( !P_VALID(m_pCurSelItem) )
		return GT_INVALID;

	return m_pUtil->Crc32(m_pCurSelItem->strName.c_str());
}



//-----------------------------------------------------------------------------
// ����һ���ڵ㼰���ӽڵ���ռ��
//-----------------------------------------------------------------------------
INT GUITree::CalItemTakeupRow(tagGUITreeItem* pItem)
{
	INT nNum = 1;
	if( pItem->bExpended )
	{
		std::list<tagGUITreeItem*>::iterator it;
		for(it=pItem->listChildren.begin(); it!=pItem->listChildren.end(); ++it)
			nNum += CalItemTakeupRow((*it));
	}
	return nNum;
}


//-----------------------------------------------------------------------------
// ����һ���ڵ���ʼ��(root���0��)
//-----------------------------------------------------------------------------
INT GUITree::CalItemStartRow(tagGUITreeItem* pItem)
{
	INT nRow = 0;
	CalItemStartRowHelp(m_pRootItem, pItem,nRow);

	return nRow;
}

//-----------------------------------------------------------------------------
// �����ݹ�: ����һ���ڵ���ʼ��(root���0��)
//-----------------------------------------------------------------------------
BOOL GUITree::CalItemStartRowHelp(tagGUITreeItem* pItem, tagGUITreeItem* pTargetItem,
								  INT& nRow)
{
	if( pItem == pTargetItem )
		return FALSE;

	nRow++;
	if( pItem->bExpended )
	{
		std::list<tagGUITreeItem*>::iterator it;
		for(it=pItem->listChildren.begin(); it!=pItem->listChildren.end(); ++it)
		{
			if( FALSE == CalItemStartRowHelp(*it, pTargetItem, nRow) )
				return FALSE;
		}
	}
	return TRUE;
}



//-----------------------------------------------------------------------------
// ������Ŀ(���szFaterName==NULL���ʾ��Ӹ��ڵ�)
//-----------------------------------------------------------------------------
DWORD GUITree::InsterItem(LPCTSTR szFatherName, LPCTSTR szText, LPCTSTR szShow)
{
	this->SetRefresh();
	tagPoint ptFont;
	if( FALSE == P_VALID(szFatherName) )	// ������ڵ�
	{
		if( P_VALID(m_pRootItem) )
			return GT_INVALID;	// ���ڵ��Ѿ�����

		m_pRootItem = new tagGUITreeItem;
		m_pRootItem->strShow = szShow;
		m_pRootItem->strText.assign(szText);
		m_pRootItem->strName.assign(_T("\\"));
		m_pRootItem->strName.append(szText);
		m_pRootItem->dwColor = m_dwTextColor;
		m_pRender->GetTextSize(szShow, m_pFont, ptFont);
		m_pRootItem->nTextWidth = ptFont.x;
		DWORD dwID = m_pUtil->Crc32(m_pRootItem->strName.c_str());
		m_mapItem.Add(dwID, m_pRootItem);
		m_nTotalRow=1;		// ������
		SetCurrentTextRow(m_nCurrentTextRow);
		return dwID;
	}

	DWORD dwCrc32 = m_pUtil->Crc32(szFatherName);
	tagGUITreeItem* pFater = m_mapItem.Peek(dwCrc32);
	if( FALSE == P_VALID(pFater) )
		return GT_INVALID;	// �Ҳ���ָ�����ڵ�

	tstring strNewName(szFatherName);
	strNewName.append(_T("\\"));
	strNewName.append(szText);
	DWORD dwNewCrc32 = m_pUtil->Crc32(strNewName.c_str());
	if( m_mapItem.IsExist(dwNewCrc32) )
		return GT_INVALID;	// ��ͬ���ӽڵ�

	tagGUITreeItem* pItem = new tagGUITreeItem;
	pItem->strShow = szShow;
	pItem->strText = szText;
	pItem->strName = strNewName;
	pItem->dwColor = m_dwTextColor;
	pItem->pFather = pFater;
	pItem->nLayer = pFater->nLayer + 1;
	m_pRender->GetTextSize(szShow, m_pFont, ptFont);
	pItem->nTextWidth = ptFont.x;
	pFater->listChildren.push_back(pItem);
	m_mapItem.Add(dwNewCrc32, pItem);
	m_nTotalRow++;		// ������

	// �ù�����ˢ��
	SetCurrentTextRow(m_nCurrentTextRow); 
	return dwNewCrc32;
}


//-----------------------------------------------------------------------------
// ɾ����Ŀ
//-----------------------------------------------------------------------------
BOOL GUITree::RemoveItem(LPCTSTR szFullName)
{
	// ���ڵ��Ƿ����
	DWORD dwCrc32 = m_pUtil->Crc32(szFullName);
	tagGUITreeItem* pItem = m_mapItem.Peek(dwCrc32);
	if( FALSE == P_VALID(pItem) )
		return FALSE;

	this->SetRefresh();

	if( P_VALID(pItem->pFather) )	// �Ӹ��ڵ�Ķ����б���ɾ���Լ�
		pItem->pFather->listChildren.remove(pItem);
	this->RemoveItem(pItem);

	if( pItem == m_pRootItem )
		m_pRootItem = NULL;

	if( P_VALID(m_pRootItem) )
		m_nTotalRow = this->CalItemTakeupRow(m_pRootItem); // ������
	else
		m_nTotalRow = 0;

	// �ù�����ˢ��
	SetCurrentTextRow(m_nCurrentTextRow); 
	return TRUE;
}


//-----------------------------------------------------------------------------
// ɾ����Ŀ
//-----------------------------------------------------------------------------
BOOL GUITree::RemoveItem(tagGUITreeItem* pItem)
{
	this->SetRefresh();
	if( !pItem->listChildren.empty() )
	{
		std::list<tagGUITreeItem*>::iterator it;
		for(it=pItem->listChildren.begin(); it!=pItem->listChildren.end();)
		{
			RemoveItem(*it);
			it = pItem->listChildren.erase(it);
		}
	}

	DWORD dwCrc32 = m_pUtil->Crc32(pItem->strName.c_str());
	m_mapItem.Erase(dwCrc32);

	tagGUITreeItem* pVisItem;
	m_listVisibleItem.ResetIterator();
	while(m_listVisibleItem.PeekNext(pVisItem))
	{
		if( pVisItem == pItem )
			m_listVisibleItem.Erase(pVisItem);
	}

	if( pItem == m_pCurSelItem )
		m_pCurSelItem = NULL;

	delete(pItem);
	return TRUE;
}


}	// namespace vEngine {
