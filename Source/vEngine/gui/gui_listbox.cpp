//-----------------------------------------------------------------------------
//!\file gui_listbox.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-04-23
//!
//!\brief ����ϵͳ list box �ؼ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_static.h"
#include "..\variable\var_container.h"
#include "gui_listbox.h"

#include "gui_event_define.h"
#include "gui_editbox.h"
#include "gui_combobox.h"
#include "gui_button.h"
#include "gui_pushbutton.h"

#include "Commdlg.h"
#include <algorithm>

namespace vEngine {
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
GUIListBox::GUIListBox()
{
	m_pVarMgr->Register(_T("MoveSelect"), &m_bMoveSelect, _T("no yes"));
	m_pVarMgr->Register(_T("CanMoveCol"), &m_bCanMoveCol, _T("no yes"));
	m_pVarMgr->Register(_T("OpenFileDialog"), &m_bOpenFileDialog, _T("no yes"));
	m_pVarMgr->Register(_T("OpenColorDialog"), &m_bOpenColorDialog, _T("no yes"));
	m_pVarMgr->Register(_T("RowHeight"), &m_nRowHeight);

	m_pVarMgr->Register(_T("SelectStickPicFileName"), &m_strSelectStickPic);
	m_pVarMgr->Register(_T("SelectStickPicRect"), &m_rcSelectStickPic);
	m_pVarMgr->Register(_T("SelectStickColor"), &m_dwSelectStickColor);
	m_pVarMgr->Register(_T("SelectStickActiveColor"), &m_dwSelectStickActiveColor);
	m_pVarMgr->Register(_T("RowLineColor"), &m_dwRowLineColor);
	m_pVarMgr->Register(_T("SelectTextColor"), &m_dwSelectTextColor);
	m_pVarMgr->Register(_T("SelectShadeColor"), &m_dwSelectShadeColor);
	m_pVarMgr->Register(_T("PicOffset"), &m_ptPicOffset);
	m_pVarMgr->Register(_T("PicSize"), &m_ptPicSize);

	m_nRowHeight = 16;
	m_dwSelectStickColor = 0xffA0A0A0;
	m_dwSelectStickActiveColor = 0xff808080;
	m_dwSelectTextColor = 0xff000000;
	m_dwRowLineColor = 0xffffffff;
	m_ptPicSize.Set(16, 16);
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIListBox::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bResult = GUIStatic::Init(pSystem, pFather, pXmlElement);
	if( FALSE == bResult )
		return bResult;
	
	// ����ѡ���ͼƬ
	m_pSelectStickPic = m_pRender->CreateImage(m_strSelectStickPic, m_rcSelectStickPic);

	m_pEditBox = NULL;
	m_pComboBox = NULL;		// ��Ͽ�

	m_mapWidth.Clear();
	m_mapWidth.Add(0, 70);	// �������Ĭ����
	m_mapWidth.Add(1, 70);
	m_mapWidth.Add(2, 70);

	m_nCurrentTextRow = 0;
	m_nTotalRow = m_ptSize.y / m_nRowHeight;
	m_nTotalTextRow = 0;
	m_nCurSelectedSegment = GT_INVALID;	// ��ǰ���ѡ�е����ָ���
	m_nCurSelectedRow = 0;	// ��ǰѡ�е���
	m_nCurEditRow = 0;	// ��ǰ�༭����
	m_nCurEditCol = 0;	// ��ǰ�༭����
	m_nCurSelectedRowWhenMouseDown = GT_INVALID;// �����������µ�ʱ��ѡ�����

	return bResult;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIListBox::Destroy()
{
	tagGUIListItem* pItem = 0;
	m_mapItem.ResetIterator();
	while( m_mapItem.PeekNext(pItem) )
	{
		m_pRender->DestroyImage(pItem->pPic);
		delete(pItem);
	}

	m_pRender->DestroyImage(m_pSelectStickPic);
	GUIStatic::Destroy();
}

//-----------------------------------------------------------------------------
// render
//-----------------------------------------------------------------------------
VOID GUIListBox::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	// ����ѡ���
	for(INT nY=0, nRow = m_nCurrentTextRow; nY<m_ptSize.y; nY += m_nRowHeight, nRow++)
	{
		if( nRow == m_nCurSelectedRow )
		{
			m_pCanvas->rc.Set(m_ptView.x, m_ptView.y + nY, m_ptView.x + m_ptSize.x, 
				m_ptView.y + nY + m_nRowHeight);
			if( m_pCanvas->rc.bottom > m_ptView.y + m_ptSize.y )
				m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;

			if( nRow == m_nCurSelectedRowWhenMouseDown )
				m_pRender->Draw(m_pCanvas, m_pSelectStickPic, m_dwSelectStickActiveColor, m_eAlphaWrite);
			else
				m_pRender->Draw(m_pCanvas, m_pSelectStickPic, m_dwSelectStickColor, m_eAlphaWrite);
		}
	}

	if( m_dwRowLineColor )	// �Ƿ���Ҫ�����ֽ���
	{
		for(INT nCol=0, nWidth = 0; nCol<m_mapWidth.Size()-1; nCol++)	// ������
		{
			nWidth += m_mapWidth.Peek(nCol);
			if( GT_INVALID != nWidth && nWidth < m_ptSize.x )
			{
				m_pCanvas->rc.Set(m_ptView.x + nWidth, m_ptView.y, m_ptView.x + nWidth, 
					m_ptView.y + m_ptSize.y);
				m_pRender->Line(m_pCanvas, m_dwRowLineColor);
			}
		}
		
		for(INT nY=m_nRowHeight; nY < m_ptSize.y; nY += m_nRowHeight)	// ����
		{
			m_pCanvas->rc.Set(m_ptView.x, nY + m_ptView.y, m_ptView.x + m_ptSize.x, nY + m_ptView.y);
			m_pRender->Line(m_pCanvas, m_dwRowLineColor);
		}
	}

	// ��ʾlistbox�е��ı�
	tagGUIListItem* pItem = NULL;
	INT nLeftX = m_rcText.left;
	for(INT nCol=0; nCol<m_mapWidth.Size(); nCol++)
	{
		INT nTextY = m_nCurrentTextRow; // Ӧ�ô���һ���ı���ʼ��ʾ
		for(INT nYOffset = m_rcText.top; 
			nYOffset < m_ptSize.y; nYOffset += m_nRowHeight, nTextY++)
		{
			DWORD dwPos = MAKELONG(nTextY, nCol);
			pItem = m_mapItem.Peek(dwPos);
			if( !P_VALID(pItem) )	// ��ǰ��Ԫ��û������
				continue;

			INT nPicOffsetX = 0;
			if( P_VALID(pItem->pPic) )
			{
				m_pCanvas->rc.SetLTWH(m_ptView.x+nLeftX+m_ptPicOffset.x-m_rcText.left, 
					m_ptView.y+nYOffset+m_ptPicOffset.y-m_rcText.top, 
					m_ptPicSize.x, m_ptPicSize.y);
				m_pRender->Draw(m_pCanvas, pItem->pPic, pItem->dwPicColor, m_eAlphaWrite);
				nPicOffsetX = m_ptPicOffset.x + m_ptPicSize.x;
			}

			m_pCanvas->rc.left = m_ptView.x + nLeftX + nPicOffsetX;
			m_pCanvas->rc.right = m_pCanvas->rc.left + m_mapWidth.Peek(nCol) - m_rcText.left - nPicOffsetX;
			if( nCol == m_mapWidth.Size() - 1 ) // ���ұ�һ�е��п���ԼӴ�
				m_pCanvas->rc.right = m_ptView.x + m_ptSize.x;

			m_pCanvas->rc.top = m_ptView.y + nYOffset;
			m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_nRowHeight - 2;

			if( m_pCanvas->rc.right > m_ptView.x + m_ptSize.x )
				m_pCanvas->rc.right = m_ptView.x + m_ptSize.x;

			if( m_pCanvas->rc.bottom > m_ptView.y + m_ptSize.y )
				m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;

			if( nTextY == m_nCurSelectedRow )
				m_pRender->Text(m_pCanvas, pItem->strText.c_str(), m_pFont, 
					m_dwSelectTextColor, m_dwSelectShadeColor, m_eTextAlign, m_bTextWriteAlpha);
			else
				m_pRender->Text(m_pCanvas, pItem->strText.c_str(), m_pFont, 
					pItem->dwColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
 		}
		
		nLeftX += m_mapWidth.Peek(nCol);
		if( GT_INVALID == nLeftX || nLeftX > m_ptSize.x )
			break;
	}

	// ���û�����Ⱦ
	GUIWnd::Render();
}



//-----------------------------------------------------------------------------
// ��ָ����Ԫ�������ı�
//-----------------------------------------------------------------------------
VOID GUIListBox::SetText(INT nRow, INT nCol, LPCTSTR szText, DWORD dwColor)
{
	ASSERT( nRow >=0 && nRow <= 0xffff );	// ���65536
	ASSERT( nCol >=0 && nCol <= 0xffff );	// ���65536

	DWORD dwPos = MAKELONG(nRow, nCol);
	tagGUIListItem* pItem = m_mapItem.Peek(dwPos);
	if( !P_VALID(pItem) )
	{
		pItem = new tagGUIListItem;
		if( !P_VALID(pItem) )
			return;

		m_mapItem.Add(dwPos, pItem);
	}

	pItem->strText.assign(szText);
	if( GT_VALID(dwColor) )
		pItem->dwColor = dwColor;
	else
		pItem->dwColor = m_dwTextColor;

	//�����������
	if( nRow >= m_nTotalTextRow )
		m_nTotalTextRow = nRow + 1;

	if( nRow >= m_nTotalRow )
	{
		m_nTotalRow = nRow + 1;
		SetCurrentTextRow(m_nCurrentTextRow); // �ù�����ˢ��
	}

	SetRefresh();
}


//-----------------------------------------------------------------------------
// �õ�ָ����Ԫ����ַ�������
//-----------------------------------------------------------------------------
LPCTSTR GUIListBox::GetText(INT nRow, INT nCol)
{
	DWORD dwPos = MAKELONG(nRow, nCol);
	tagGUIListItem* pItem = m_mapItem.Peek(dwPos);
	if( P_VALID(pItem) )
		return pItem->strText.c_str();
	else
		return (LPCTSTR)GT_INVALID;	// ָ����Ԫ��������,����GT_INVALID
}



//-----------------------------------------------------------------------------
// ���õ�Ԫ����Ϣ
//-----------------------------------------------------------------------------
VOID GUIListBox::SetItemPic(INT nRow, INT nCol, LPCTSTR szPic, const tagRect& rc, DWORD dwColor)
{
	DWORD dwPos = MAKELONG(nRow, nCol);
	tagGUIListItem* pItem = m_mapItem.Peek(dwPos);
	if( !P_VALID(pItem) )
	{
		this->SetText(nRow, nCol, _T(""));
		pItem = m_mapItem.Peek(dwPos);
		if( !P_VALID(pItem) )
			return;
	}

	if( P_VALID(pItem->pPic) )
		m_pRender->DestroyImage(pItem->pPic);
	
	pItem->pPic = m_pRender->CreateImage(szPic, rc);
	pItem->dwPicColor = dwColor;
	SetRefresh();
}


//-----------------------------------------------------------------------------
// ���õ�Ԫ����Ϣ
//-----------------------------------------------------------------------------
VOID GUIListBox::SetItemEditable(INT nRow, INT nCol, BOOL bEditable)
{
	DWORD dwPos = MAKELONG(nRow, nCol);
	tagGUIListItem* pItem = m_mapItem.Peek(dwPos);
	if( !P_VALID(pItem) )
		return;

	pItem->bEditable = bEditable;
}



//-----------------------------------------------------------------------------
// ���õ�Ԫ����Ϣ
//-----------------------------------------------------------------------------
VOID GUIListBox::SetItemOptionText(INT nRow, INT nCol, std::list<tstring>& listOptionText)
{
	DWORD dwPos = MAKELONG(nRow, nCol);
	tagGUIListItem* pItem = m_mapItem.Peek(dwPos);
	if( !P_VALID(pItem) )
		return;

	pItem->listOptionText = listOptionText;
}


//-----------------------------------------------------------------------------
// ��ӿհ���
//-----------------------------------------------------------------------------
VOID GUIListBox::InsertRow(INT nRow)
{
	ASSERT( nRow <= 0xffff );	// ���65535��
	if( nRow >= m_nTotalTextRow )	// �����������ǲ���Ҫ����
		return;

	DWORD dwPos = 0;
	tagGUIListItem* pItem = NULL;
	TMap<DWORD, tagGUIListItem*> mapTemp = m_mapItem;
	m_mapItem.Clear();

	mapTemp.ResetIterator();
	while(mapTemp.PeekNext(dwPos, pItem))
	{
		DWORD dwKey = dwPos;
		INT nThisRow = LOWORD(dwPos);
		if( nThisRow >= nRow )
			dwKey = MAKELONG(nThisRow+1, HIWORD(dwPos));

		m_mapItem.Add(dwKey, pItem);
	}

	//�����������
	m_nTotalTextRow++;

	//���¼���listbox�߶�
	if( m_nTotalTextRow > m_nTotalRow )
	{
		m_nTotalRow = m_nTotalTextRow;
		SetCurrentTextRow(m_nCurrentTextRow); // �ù�����ˢ��
	}

	SetRefresh();
}



//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
VOID GUIListBox::SetColNum(INT nColNum, INT nDefaultColWidth)
{
	m_mapWidth.Clear();
	for(INT nCol=0; nCol<nColNum; nCol++)
		m_mapWidth.Add(nCol, nDefaultColWidth);

	m_nCurSelectedSegment = GT_INVALID;	// ȡ�����ѡ��ķָ���
	SetRefresh();
}




//-----------------------------------------------------------------------------
// ����ָ���еĿ��,�Ҳ���ָ���з���FALSE
//-----------------------------------------------------------------------------
BOOL GUIListBox::SetColWidth(INT nColIndex, INT nColWidth)
{
	m_nCurSelectedSegment = GT_INVALID;	// ȡ�����ѡ��ķָ���
	SetRefresh();
	return m_mapWidth.ChangeValue(nColIndex, nColWidth);
}


//-----------------------------------------------------------------------------
// ɾ����������
//-----------------------------------------------------------------------------
VOID GUIListBox::Clear()
{
	m_nCurrentTextRow = 0;
	m_nTotalTextRow = 0;
	m_nTotalRow = m_ptSize.y / m_nRowHeight;
	m_nCurSelectedSegment = GT_INVALID;	// ��ǰ���ѡ�е����ָ���
	
	if( m_nCurSelectedRow >= m_nTotalRow )
		m_nCurSelectedRow = 0;	// ��ǰѡ�е���

	m_mapItem.ResetIterator();
	tagGUIListItem* pItem = 0;
	while( m_mapItem.PeekNext(pItem) )
	{
		m_pRender->DestroyImage(pItem->pPic);
		delete(pItem);
	}

	m_mapItem.Clear();

	// ����������Ϣ
	tagGUIEvent event(this, EGUIE_Scroll, NULL);
	event.dwParam1 = (DWORD)m_nCurSelectedRow;
	event.dwParam2 = (DWORD)m_nTotalRow;
	event.dwParam3 = (DWORD)m_nTotalRow;
	OnEvent(&event);
	SetRefresh();
}



//-----------------------------------------------------------------------------
// ���õ�ǰ��ʾ�ĵ�һ����ʵ�ʵ���һ��
//-----------------------------------------------------------------------------
BOOL GUIListBox::SetCurrentTextRow(INT nY, BOOL bSendEvent)
{
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

	SetRefresh();
	return TRUE;
}



//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
BOOL GUIListBox::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:case WM_RBUTTONDOWN:
		if( PointInWnd(pMsg->pt, false) )
		{
			INT nX = m_ptView.x;	// ������ѡ�еķָ���
			for(INT nCol=0; nCol<m_mapWidth.Size()-1; nCol++)
			{
				INT nCurWidth = m_mapWidth.Peek(nCol);
				nX += nCurWidth;
				if( pMsg->pt.x > nX - 5 && pMsg->pt.x < nX + 5 )
				{
					m_nCurSelectedSegment = nCol;
				}
			}

			// ѡ������ƶ���ѡ��
			for(INT nY=0, nRow = m_nCurrentTextRow; nY<m_ptSize.y; 
				nY += m_nRowHeight, nRow++)
			{
				if( pMsg->pt.y - m_ptView.y >= nY 
					&& pMsg->pt.y - m_ptView.y < nY + m_nRowHeight)
				{
					m_nCurSelectedRow = nRow;
					m_nCurSelectedRowWhenMouseDown = nRow;
				}
			}
			SetRefresh();
		}
		break;

	case WM_LBUTTONUP:case WM_RBUTTONUP:
		for(INT nY=0, nRow = m_nCurrentTextRow; nY<m_ptSize.y; 
			nY += m_nRowHeight, nRow++)
		{
			if( pMsg->pt.y - m_ptView.y >= nY 
				&& pMsg->pt.y - m_ptView.y < nY + m_nRowHeight)
			{
				// �����������µ�ʱ��ѡ�����
				if( m_nCurSelectedRowWhenMouseDown == nRow )
				{
					tagGUIEvent event(this, EGUIE_ItemClick, pMsg);
					event.dwParam1 = m_nCurSelectedRow;
					SendEvent(&event);
				}
			}
		}

		// �����������µ�ʱ��ѡ�����
		m_nCurSelectedRowWhenMouseDown = GT_INVALID;
		m_nCurSelectedSegment = GT_INVALID;	// ȡ�����ѡ��ķָ���
		SetRefresh();
		break;

	case WM_LBUTTONDBLCLK:	// ˫���༭
		for(INT nY=0, nRow=m_nCurrentTextRow; nY<m_ptSize.y; nY+=m_nRowHeight, nRow++)
		{
			if( pMsg->pt.y - m_ptView.y >= nY && pMsg->pt.y - m_ptView.y < nY + m_nRowHeight )	// ��λ��
			{
				for(INT nCol=0, nX=0; nCol<m_mapWidth.Size(); nCol++)
				{
					INT nCurWidth = m_mapWidth.Peek(nCol);
					// ���һ�еĿ�ȿ��ܴ����趨���
					if( nCol == m_mapWidth.Size() - 1 && nX+nCurWidth < m_ptSize.x )
						nCurWidth += m_ptSize.x - nX + nCurWidth;	

					if( pMsg->pt.x-m_ptView.x > nX && pMsg->pt.x-m_ptView.x < nX+nCurWidth ) // ��λ��
					{
						m_nCurEditRow = nRow;
						m_nCurEditCol = nCol;

						tagPoint ptClient(nX+1, nY+1), ptSize(nCurWidth-1, m_nRowHeight-1);
						if( nX+nCurWidth > m_ptSize.x )
							ptSize.x -= nX + nCurWidth - m_ptSize.x;

						// �����ڲ��ı༭��
						ActiveEditor(nRow, nCol, ptClient, ptSize);
					}
					nX+=nCurWidth;
				}

				tagGUIEvent event(this, EGUIE_ItemDblClick, pMsg);	// ˫��
				event.dwParam1 = m_nCurEditRow;
				event.dwParam2 = m_nCurEditCol;
				SendEvent(&event);	
				break;
			}
		}
		break;

	case WM_MOUSEMOVE:
		if( FALSE == this->PointInWnd(pMsg->pt) )
		{
			if( P_VALID(m_nCurSelectedRowWhenMouseDown) )
			{
				SetRefresh();
				m_nCurSelectedRowWhenMouseDown = GT_INVALID;	
			}
		}

		// ��������϶��п�ָ���
		if( m_bCanMoveCol == TRUE && pMsg->dwParam1 == MK_LBUTTON && GT_VALID(m_nCurSelectedSegment) )
		{
			INT nX = m_ptView.x;
			for(INT nCol=0; nCol<=m_nCurSelectedSegment; nCol++)
			{
				INT nCurWidth = m_mapWidth.Peek(nCol);
				nX += nCurWidth;
			}

			INT nDiff = pMsg->pt.x - nX;
			if( m_mapWidth.Peek(m_nCurSelectedSegment) + nDiff > 0 )
			{
				if( nX + nDiff > m_ptView.x + m_ptSize.x - 2 )
					nDiff = m_ptView.x + m_ptSize.x - 2 - nX;

				m_mapWidth.ChangeValue(m_nCurSelectedSegment, m_mapWidth.Peek(m_nCurSelectedSegment) + nDiff);
			}
			else
				m_mapWidth.ChangeValue(m_nCurSelectedSegment, 1);
			SetRefresh();
		}

		// ѡ������ƶ�
		if( m_bMoveSelect && PointInWnd(pMsg->pt, false) )
		{
			for(INT nY=0, nRow = m_nCurrentTextRow; nY < m_ptSize.y; 
				nY += m_nRowHeight, nRow++)
			{
				if( pMsg->pt.y - m_ptView.y >= nY 
					&& pMsg->pt.y - m_ptView.y < nY + m_nRowHeight)
				{
					m_nCurSelectedRow = nRow;
					SetRefresh();
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
		SetRefresh();
		switch(pMsg->dwParam1)
		{
		case VK_UP:
			if( m_nCurSelectedRow > m_nCurrentTextRow )
				m_nCurSelectedRow--;
			else	// �������
			{
				if( m_nCurrentTextRow > 0 )	
				{
					SetCurrentTextRow(m_nCurrentTextRow-1);
					m_nCurSelectedRow--;
				}
			}
			return TRUE;
		case VK_DOWN:
			if( m_nCurSelectedRow < m_nTotalRow-1 )
			{
				m_nCurSelectedRow++;
				if( m_nCurSelectedRow > m_nCurrentTextRow + m_ptSize.y / m_nRowHeight - 1 )
					SetCurrentTextRow(m_nCurrentTextRow+1);
			}
			return TRUE;
		case VK_PRIOR:	// ��ǰ��ҳ
			if( SetCurrentTextRow(m_nCurrentTextRow-m_ptSize.y / m_nRowHeight) )
				m_nCurSelectedRow = m_nCurrentTextRow;
			else
			{
				SetCurrentTextRow(0);
				m_nCurSelectedRow = 0;
			}
			return TRUE;
		case VK_NEXT:	// ���ҳ
			if( SetCurrentTextRow(m_nCurrentTextRow+m_ptSize.y / m_nRowHeight) )
				m_nCurSelectedRow = m_nCurrentTextRow;
			else
			{
				SetCurrentTextRow(m_nTotalRow - m_ptSize.y / m_nRowHeight, TRUE);	// �������1��
				m_nCurSelectedRow = m_nTotalTextRow-1;
			}
			return TRUE;
		case VK_F2:	// ����༭��ǰ��
			{
				if( !P_VALID(m_nCurSelectedRow) || m_nCurSelectedRow < m_nCurrentTextRow 
					|| m_nCurSelectedRow > m_nCurrentTextRow + (m_ptSize.y/m_nRowHeight) )
					break;	// ȷ����ǰѡ��������Ұ��

				INT nY = (m_nCurSelectedRow - m_nCurrentTextRow) * m_nRowHeight;
				for(INT nCol=0, nX=0; nCol<m_mapWidth.Size(); nCol++)
				{
					INT nCurWidth = m_mapWidth.Peek(nCol);
					// ���һ�еĿ�ȿ��ܴ����趨���
					if( nCol == m_mapWidth.Size() - 1 && nX+nCurWidth < m_ptSize.x )
						nCurWidth += m_ptSize.x - nX + nCurWidth;	

					DWORD dwPos = MAKELONG(m_nCurSelectedRow, nCol);
					tagGUIListItem* pItem = m_mapItem.Peek(dwPos);
					if( P_VALID(pItem) && pItem->bEditable )	// ��Ԫ��Ƿ��򲻿ɱ༭
					{
						m_nCurEditRow = m_nCurSelectedRow;
						m_nCurEditCol = nCol;

						tagPoint ptClient(nX+1, nY+1), ptSize(nCurWidth-1, m_nRowHeight-1);
						if( nX+nCurWidth > m_ptSize.x )
							ptSize.x -= nX + nCurWidth - m_ptSize.x;

						// �����ڲ��ı༭��
						ActiveEditor(m_nCurSelectedRow, nCol, ptClient, ptSize);
						return TRUE;
					}

					nX+=nCurWidth;
				}
			}
			return TRUE;
		case VK_RETURN:	// �൱����굥��
			{
				if( !P_VALID(m_nCurSelectedRow) || m_nCurSelectedRow < m_nCurrentTextRow 
					|| m_nCurSelectedRow > m_nCurrentTextRow + (m_ptSize.y/m_nRowHeight) )
					break;	// ȷ����ǰѡ��������Ұ��

				m_nCurSelectedRow = m_nCurSelectedRow;
				m_nCurSelectedRowWhenMouseDown = m_nCurSelectedRow;

				tagGUIEvent event(this, EGUIE_ItemClick, pMsg);
				event.dwParam1 = m_nCurSelectedRow;
				SendEvent(&event);
			}
		}
		break;
	}

	return GUIStatic::OnInputMessage(pMsg);
}


//-----------------------------------------------------------------------------
// �����ӿؼ������¼�
//-----------------------------------------------------------------------------
VOID GUIListBox::SendEvent(tagGUIEvent* pEvent)
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

	case EGUIE_EditBoxEnter:
		if( pWnd == m_pEditBox )
			m_pSystem->SetActive(this);	// ���������Ϣ����
		if( P_VALID(m_pComboBox) && pWnd == m_pComboBox )
			m_pSystem->SetActive(this);	// ���������Ϣ����
		break;

	case EGUIE_Deactive:
		if( pWnd == m_pEditBox )
		{
			SetText(m_nCurEditRow, m_nCurEditCol, m_pEditBox->GetText());
			m_pEditBox->SetInvisible(true);

			tagGUIEvent event(this, EGUIE_UserChangeListContext, NULL);	// �û��ı�List����
			event.dwParam1 = m_nCurEditRow;
			event.dwParam2 = m_nCurEditCol;
			SendEvent(&event);
		}
		if( P_VALID(m_pComboBox) && pWnd == m_pComboBox )
		{
			SetText(m_nCurEditRow, m_nCurEditCol, m_pComboBox->GetEditBox()->GetText());
			m_pComboBox->SetInvisible(true);

			tagGUIEvent event(this, EGUIE_UserChangeListContext, NULL);	// �û��ı�List����
			event.dwParam1 = m_nCurEditRow;
			event.dwParam2 = m_nCurEditCol;
			SendEvent(&event);
		}
		SetRefresh(TRUE);
		break;
	}

	GUIWnd::SendEvent(pEvent);
}


//-----------------------------------------------------------------------------
// ����Ӵ���
//-----------------------------------------------------------------------------
VOID GUIListBox::AddChild(GUIWnd* pChild)
{
	GUIWnd::AddChild(pChild);

	if( pChild->GetClassName() == _T("CGUIEditBox") )	// �༭��
	{
		m_pEditBox = (GUIEditBox*)pChild;
		m_pEditBox->SetInvisible(true);
	}

	if( pChild->GetClassName() == _T("CGUIComboBox") )	// ��Ͽ�
	{
		m_pComboBox = (GUIComboBox*)pChild;
		m_pComboBox->SetInvisible(true);
	}

	// �ӿؼ������ǹ��������ù�����ˢ��
	SetCurrentTextRow(m_nCurrentTextRow); 
}

//-----------------------------------------------------------------------------
// ɾ���Ӵ���
//-----------------------------------------------------------------------------
VOID GUIListBox::DelChild(GUIWnd* pChild)
{
	if( m_pEditBox == pChild )
		m_pEditBox = NULL;
	if( m_pComboBox == pChild )
		m_pComboBox = NULL;

	GUIWnd::DelChild(pChild);
}


//-----------------------------------------------------------------------------
// �����ڲ��ı༭��
//-----------------------------------------------------------------------------
VOID GUIListBox::ActiveEditor(INT nRow, INT nCol, tagPoint& ptPos, tagPoint& ptSize)
{
	DWORD dwPos = MAKELONG(nRow, nCol);
	tagGUIListItem* pItem = m_mapItem.Peek(dwPos);
	if( !P_VALID(pItem) || !pItem->bEditable )	// ��Ԫ��Ƿ��򲻿ɱ༭
		return;

	if( !pItem->listOptionText.empty())
	{
		if( P_VALID(m_pComboBox) )
		{
			// ����EditBox
			m_pComboBox->SetInvisible(false);
			m_pSystem->SetActive(m_pComboBox);

			m_pComboBox->SetResizable(true);
			m_pComboBox->SetMovable(true);
			tagPoint ptClient = ptPos - m_pComboBox->GetClient();
			m_pComboBox->Resize(ptSize);
			m_pComboBox->Move(ptClient);
			m_pComboBox->SetMovable(false);
			m_pComboBox->SetResizable(false);

			m_pComboBox->GetEditBox()->SetText(pItem->strText.c_str());
			GUIListBox* pListBox = m_pComboBox->GetListBox();
			if( P_VALID(pListBox) )
			{
				pListBox->SetInvisible(false);
				pListBox->SetMovable(true);
				pListBox->SetResizable(true);
				tagPoint ptListSize(pListBox->GetSize());
				ptListSize.y = 16*pItem->listOptionText.size();
				tagPoint ptListPos(0, -ptListSize.y);
				tagPoint ptClient = ptListPos - pListBox->GetClient();
				pListBox->Resize(ptListSize);
				pListBox->Move(ptClient);
				pListBox->SetMovable(false);
				pListBox->SetResizable(false);

				pListBox->Clear();
				pListBox->SetColNum(1, 100);
				std::list<tstring>::iterator it = pItem->listOptionText.begin();
				for(INT n=0;it!=pItem->listOptionText.end(); ++it, ++n)
					pListBox->SetText(n, 0, (*it).c_str());
				m_pSystem->SetActive(pListBox);
			}
		}
	}
	else
	{
		if( P_VALID(m_pEditBox) )
		{
			// ����EditBox
			m_pEditBox->SetInvisible(false);
			m_pEditBox->SetResizable(true);
			m_pEditBox->SetMovable(true);
			tagPoint ptClient = ptPos - m_pEditBox->GetClient();
			m_pEditBox->Resize(ptSize);
			m_pEditBox->Move(ptClient);
			m_pEditBox->SetMovable(false);
			m_pEditBox->SetResizable(false);
			m_pEditBox->SetText(pItem->strText.c_str());
			m_pSystem->SetActive(m_pEditBox);

			if( m_bOpenFileDialog || m_bOpenColorDialog )
			{
				DWORD dwPos0 = MAKELONG(nRow, 0);
				tagGUIListItem* pItem0 = m_mapItem.Peek(dwPos0);
				if( pItem0->strText.find(_T("FileName")) != std::string::npos ) 
				{
					TCHAR mess[MAX_PATH];
					lstrcpy(mess, pItem->strText.c_str());
					OPENFILENAME file;
					ZeroMemory(&file, sizeof(file));
					file.lStructSize = sizeof(file);
					file.hwndOwner = (HWND)m_pGlobeVar->GetDword(_T("HWND"));

					file.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST 
						| OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
					file.lpstrFilter = _T("image files\0*.bmp;*.jpg\0\0");
					file.lpstrFile = mess;
					file.nMaxFile = MAX_PATH;
					file.lpstrTitle = _T("SelectPicFile");
					if( ::GetOpenFileName(&file) )
					{
						SetDefaultDir();
						// ���Ƿ���Ѱ�ҳ����·��
						TCHAR szDir[MAX_PATH];
						::GetCurrentDirectory(MAX_PATH, szDir);
						tstring strSelect(mess);
						tstring strDir(szDir);
						std::transform(strSelect.begin(), strSelect.end(), strSelect.begin(), tolower);
						std::transform(strDir.begin(), strDir.end(), strDir.begin(), tolower);
						if( strSelect.find(strDir) != tstring::npos 
							&& strSelect.size() > strDir.size()+1 )
						{
							strSelect = strSelect.substr(strDir.size()+1, strSelect.size() - strDir.size()-1);
						}

						m_pEditBox->SetText(strSelect.c_str());
					}
				}

				if( pItem0->strText.find(_T("Color")) != std::string::npos ) 
				{
					DWORD dwAlpha = _tcstoul(pItem->strText.c_str(), NULL, 16) & 0xff000000;
					COLORREF temp[16]={0};
					CHOOSECOLOR color;
					ZeroMemory(&color, sizeof(color));
					color.lStructSize = sizeof(color);
					color.hwndOwner = (HWND)m_pGlobeVar->GetDword(_T("HWND"));
					color.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_PREVENTFULLOPEN;
					color.rgbResult = SwapColor(_tcstoul(pItem->strText.c_str(), NULL, 16));
					color.lpCustColors = temp;
					if( ::ChooseColor(&color) )
					{
						TCHAR szMess[X_LONG_STRING];
						wsprintf(szMess, _T("0x%08X"), SwapColor(color.rgbResult)|dwAlpha);
						m_pEditBox->SetText(szMess);
					}
				}
			}
		}
	}

}



}	// namespace vEngine {
