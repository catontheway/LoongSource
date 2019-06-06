#include "StdAfx.h"
#include "RichEditBoxForChatShow.h"
#include "ChatFrame.h"
#include ".\GameClientDefine.h"
#include "CursorMgr.h"
#define TOP_SCROLLBAR_MAX_SIZE 20						
RichEditBoxForChatShow::RichEditBoxForChatShow(void)
{
	m_bFlex = FALSE;
	m_byFlexStatus = -1;
}

RichEditBoxForChatShow::~RichEditBoxForChatShow(void)
{
}



//-----------------------------------------------------------------------------
// ������Ϣ����
//-----------------------------------------------------------------------------
BOOL RichEditBoxForChatShow::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:
		{
			m_bShowCursor = TRUE;
			// �ҵ���������ַ�
			INT nChar = GetCharIndex(pMsg->pt.x, pMsg->pt.y, FALSE);
			if( GT_INVALID != nChar )
			{
				m_nSelectStart = m_nSelectEnd = m_nCursorPos = nChar;	//ȡ��ѡ��

				while( m_nCursorPos > GetLastDisplayIndex() )
					m_nStartDisplayLine++;
			}

			if( m_bFlex && pMsg->pt.y >= m_ptView.y			//����ڴ��ڵ�����
				&& pMsg->pt.y <= m_ptView.y+GUI_DEFAULT_ROW_SPACING )
			{
				m_ptLastMouseClickView.y = pMsg->pt.y;		//ʹ�϶�ƽ��
				m_byFlexStatus = 0;
			}
			else
				m_byFlexStatus = -1;

			//�Ƿ���������
			BOOL bLink = ClickLink(pMsg);
			//�ı���
			AdptiveCursor(m_byFlexStatus==0, bLink);
			this->SetRefresh();
		}
		break;
	case WM_LBUTTONUP:
		{
			//�Ƿ���������
			BOOL bLink = ClickLink(pMsg);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			m_bShowCursor = TRUE;
			// �ҵ���������ַ�
			INT nChar = GetCharIndex(pMsg->pt.x, pMsg->pt.y, FALSE);
			if( GT_INVALID != nChar )
			{
				m_nSelectStart = m_nSelectEnd = m_nCursorPos = nChar;	//ȡ��ѡ��

				while( m_nCursorPos > GetLastDisplayIndex() )
					m_nStartDisplayLine++;
			}

			if( m_bFlex && pMsg->pt.y >= m_ptView.y			//����ڴ��ڵ�����
				&& pMsg->pt.y <= m_ptView.y+GUI_DEFAULT_ROW_SPACING )
			{
				m_ptLastMouseClickView.y = pMsg->pt.y;		//ʹ�϶�ƽ��
				m_byFlexStatus = 0;
			}
			else
				m_byFlexStatus = -1;

			this->SetRefresh();
		}
		break;
	case WM_RBUTTONUP:
		{
			if( m_bCanSelect == TRUE )
			{
				CalSelectForDoubleClick();
				tagGUIEvent event(this,(EGUIEvent)(EXUIE_MouseRClick));
				event.msg = *pMsg;
				SendEvent(&event);
				this->SetRefresh();
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{	//˫��
			if( m_bCanSelect == TRUE )
			{
				//����˫��������,�������ֿ�ѡ
				CalSelectForDoubleClick();
				tagGUIEvent event(this,(EGUIEvent)(EXUIE_MouseDoubleClick));
				SendEvent(&event);
				this->SetRefresh();
			}
		}
		break;
	case WM_MOUSEMOVE:
		{		
			// ���������״�仯 �Ƿ��ڳ����ӵ�λ��
			INT nPos = GetCharIndex(pMsg->pt.x, pMsg->pt.y, TRUE);
			m_nMouseLinkIndex = GetLinkIndex(nPos);

			if( GT_INVALID == m_nMouseLinkIndex )
				AdptiveCursor(FALSE, FALSE );
			else
				AdptiveCursor(FALSE, TRUE);

			if( m_pSystem->GetActive() != this )
			break;
			//-----------------------------------------------

			if( TRUE == PointInWnd(pMsg->pt) )
			{
				// ��������϶�ѡ��
				if( FALSE == m_bCanSelect )
					break;

				if( pMsg->dwParam1 == MK_LBUTTON )
				{
					//�����������ѡ��,��ѡ��״̬����Ϊԭ��,��������Ϊԭ��
					INT nChar = GetCharIndex(pMsg->pt.x, pMsg->pt.y, TRUE);
					if( GT_INVALID != nChar )
					{ //
						if( nChar < m_nSelectStart )
							m_nSelectEnd = m_nCursorPos = nChar;
						else
						{
							nChar  = GetCharIndex(pMsg->pt.x, pMsg->pt.y, FALSE);
							if( GT_INVALID != nChar )
								m_nSelectEnd = m_nCursorPos = nChar;
						}
						this->SetRefresh();
					}
				}
			}
			else
			{
				if( (pMsg->dwParam1 != MK_LBUTTON ) || (GetTickCount()-m_dwLastMoveTime) < 20 )
					break;

				if( FALSE == m_bCanSelect )
					break;

				m_dwLastMoveTime = GetTickCount();

				if( pMsg->pt.y < m_ptView.y )
				{
					//���ƶ�
					CalCursonForUpDown(TRUE);
					while( m_nCursorPos < GetRowHead(m_nStartDisplayLine) )
						m_nStartDisplayLine--;

				}
				else if( pMsg->pt.y > m_ptView.y + m_ptSize.y )
				{
					//���ƶ�
					CalCursonForUpDown(FALSE);
					while( m_nCursorPos > GetLastDisplayIndex() )
						m_nStartDisplayLine++;
				}

				m_nSelectEnd = m_nCursorPos;
				SendScrollEvent();
				this->SetRefresh();
			}
		}
		break;
	case WM_MOUSEWHEEL:	// ����������
		{
			if( pMsg->dwParam1 & 0x80000000 )	// down
			{
				ScrollText(FALSE);
			}
			else // up
			{
				ScrollText(TRUE);
			}
			SendScrollEvent();
			this->SetRefresh();
		}
		break;
	case WM_KEYDOWN:
		this->SetRefresh();
		return OnMsgForKeyDown(pMsg->dwParam1);

	case WM_SYSCHAR:
	case WM_CHAR:
		{
			this->SetRefresh();
			//���ȴ���ctrl+ ()�Ŀ�ݼ�
			if( pMsg->dwParam1 == ('a' & 0x1F) )	// ctrl+a ��Сд�޹�
			{
				m_nSelectStart = 0;
				m_nSelectEnd = m_strText.size() - 2;
				m_nCursorPos = 0;
			}
			else if( pMsg->dwParam1 == ('c' & 0x1F) )	//ctrl+c  ��Сд�޹�
			{
				this->CopyToClipboard();
			}
			else if( pMsg->dwParam1 == ('x' & 0x1F) )	//ctrl+x  ��Сд�޹�
			{
				this->CopyToClipboard();

				if( m_bReadOnly )
					break;

				this->DelSelected();
			}
			else if( pMsg->dwParam1 == ('v' & 0x1F) )	//ctrl+v  ��Сд�޹�
			{
				if( m_bReadOnly )
					break;

				this->DelSelected();
				this->PasteFromClipboard();
			}

			if( m_bReadOnly )
				break;

			if( pMsg->dwParam1 <= 0x1F && pMsg->dwParam1 > 0 )
			{
				// ��������Ч�ַ�,��������ʾ
				return TRUE;
			}

			// ���������������
			// ������ѡ����ɾ��ѡ�񲿷�
			if( m_nSelectStart != m_nSelectEnd )
				this->DelSelected();

			//���ܳ����ֽ�����
			if( (INT)m_strText.size() + 1 < m_nMaxEditTextCount )
			{
				if( m_strText.size() + 1 >= m_dwMaxTextCount )
				{
					m_dwMaxTextCount = m_strText.size() + 2;
					m_VecCharIntro.resize(m_dwMaxTextCount+2);
				}

				InsertText((TCHAR*)&pMsg->dwParam1, m_nCursorPos, m_dwTextColor, m_dwShadeColor);
			}

			while( m_nCursorPos > GetLastDisplayIndex() )
				m_nStartDisplayLine++;

			SendScrollEvent();
			return TRUE;
		}
		break;
	default:
		break;
	}

	return GUIStatic::OnInputMessage(pMsg);
}

void RichEditBoxForChatShow::AdptiveCursor(bool bDrag, BOOL bLink)
{
	if( bLink )
	{
		CursorMgr::Inst()->SetIdleCursor( ECIN_Hyperlink );
	}
	else 
	{
		CursorMgr::Inst()->SetIdleCursor( ECIN_Default );
	}
}

void RichEditBoxForChatShow::ZoomWnd( INT nOffset )
{
	GUIWnd* pGFather = (GUIWnd*)m_pFather->GetFather();
	if( P_VALID(pGFather) )
	{
		tagPoint ptSize = (tagPoint&)pGFather->GetSize();

		ptSize.y += nOffset;
		pGFather->SetResizable(TRUE);
		pGFather->Resize(ptSize);
		pGFather->SetResizable(FALSE);	
		pGFather->SetMovable(TRUE);
		pGFather->Move(tagPoint(0, nOffset));
		pGFather->SetMovable(FALSE);
		pGFather->Align();

		ptSize = (tagPoint&)m_pFather->GetSize();
		ptSize.y += nOffset;
		m_pFather->SetResizable(TRUE);
		m_pFather->Resize(ptSize);
		m_pFather->SetResizable(FALSE);
		m_pFather->Align();

		//֪ͨ�������촰�ڸı��С
		tagGUIEvent event(this, EGUIE_Drag);
		event.dwParam1 = nOffset;
		event.dwParam2 = 1;
		SendEvent(&event);
		SendScrollEvent();		

		//ˢ��
		//pGFather->SetRefresh(TRUE);
		//pGFather->SetInvisible(FALSE);		
	}

}