#include "StdAfx.h"
#include "RichEditBoxForChatInput.h"
#include ".\GameClientDefine.h"
RichEditBoxForChatInput::RichEditBoxForChatInput(void)
{
}

RichEditBoxForChatInput::~RichEditBoxForChatInput(void)
{
}

//�����ͷŵ���Ʒ
VOID RichEditBoxForChatInput::OnEvent(tagGUIEvent* pEvent)
{
	if(!P_VALID(pEvent))
		return;
	if(IsInvisible() == TRUE)
		return;
	if( pEvent->eEvent == EGUISCB_Drop )
	{
		if(pEvent->dwParam1==EDT_Item 
			|| pEvent->dwParam1==EDT_ItemEquip
			|| pEvent->dwParam1==EDT_ItemWare )
		{
			tagPoint pt ( LOWORD( pEvent->dwParam4 ), HIWORD( pEvent->dwParam4 ) );
			if( PointInWnd( pt ) )
			{
				tagGUIEvent event(this,(EGUIEvent)(EXUIE_ShowItemOrEquip));
				event.dwParam1 = pEvent->dwParam1;
				event.dwParam2 = pEvent->dwParam2;
				event.dwParam3 = pEvent->dwParam3;
				SendEvent(&event);
			}
		}
	}
	else
	{
		GUIRichEditBox::OnEvent(pEvent);
	}
}

BOOL RichEditBoxForChatInput::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:
		{
			if(m_pSystem->GetMousePoint() != this)
				break;
			m_bShowCursor = TRUE;
			// �ҵ���������ַ�
			INT nChar = GetCharIndex(pMsg->pt.x, pMsg->pt.y, FALSE);
			if( GT_INVALID != nChar )
			{
				m_nSelectStart = m_nSelectEnd = m_nCursorPos = nChar;	// ȡ��ѡ��

				while( m_nCursorPos > GetLastDisplayIndex() )
					m_nStartDisplayLine++;
			}

			//�Ƿ���������
			BOOL bLink = ClickLink(pMsg);
			//�ı���
			AdptiveCursor(TRUE, bLink);
			this->SetRefresh();
		}
		break;
	case WM_LBUTTONUP:
		{
			//�Ƿ���������
			BOOL bLink = ClickLink(pMsg);
		}
		break;

	case WM_LBUTTONDBLCLK:
		{	//˫��
			if(m_pSystem->GetMousePoint() != this)
				break;
			if( m_bCanSelect == TRUE )
			{
				//����˫��������,�������ֿ�ѡ
				CalSelectForDoubleClick();
				this->SetRefresh();
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			if( m_pSystem->GetActive() != this )
				break;

			//----------------------------------------------
			// ���������״�仯,add by zewu 2005-2-9 20:49

			//�Ƿ��ڳ����ӵ�λ��
			INT nPos = GetCharIndex(pMsg->pt.x, pMsg->pt.y, TRUE);
			m_nMouseLinkIndex = GetLinkIndex(nPos);

			if( GT_INVALID == m_nMouseLinkIndex )
				AdptiveCursor(FALSE, FALSE );
			else
				AdptiveCursor(FALSE, TRUE);
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
		{
			this->SetRefresh();
			BOOL bDown = OnMsgForKeyDown(pMsg->dwParam1);
			if(!bDown)
				GUIStatic::OnInputMessage(pMsg);		// ���� VK_SPACE ������¼�
			else
				return bDown;
		}
		break;
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
			
			// ͳ���ַ����л��лس��ĸ���
			int nNl = 0;
			int nEr = 0;
			for(int i=0; i<(int)m_strText.size(); i++)
			{
				if(m_strText[i]==0x0a)
					nNl++;
				if(m_strText[i]==0x0d)
					nEr++;
			}

			//���ܳ����ֽ�����
			if( (INT)m_strText.size() + 1 < m_nMaxEditTextCount+nNl+nEr+1 )
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
			tagGUIEvent event(this,(EGUIEvent)EXUIE_ContextChange);
			m_pFather->SendEvent(&event);
			return TRUE;
		}
		break;
	default:
		break;
	}

	return GUIWnd::OnInputMessage(pMsg);
}