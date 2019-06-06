#include "StdAfx.h"
#include "DragBtn.h"
#include "ButtonEx.h"
#include "ShortcutButton.h"
#include "ItemEvent.h"
DragBtn::DragBtn(void)
{
	m_eDragType		= EDT_Null;
	m_pSrcWnd		= NULL;

	m_dwSrcWndID	= 0;
	m_dwParam1		= 0;								
	m_dwParam2		= 0;								
	m_dwParam3		= 0;								
	m_dwParam4		= 0;			
}

DragBtn::~DragBtn(void)
{
}

BOOL DragBtn::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRes = GUIStatic::Init(pSystem, pFather, pXmlElement);
	if(!bRes)
		return bRes;
			

	return TRUE;
}

DragBtn* DragBtn::NewInstance(GUISystem *pGUISys)			//��ʹ��һ��ʵ��
{
	XmlElement ele;
	ele.SetAttribute("Name", "DragButton");
	ele.SetAttribute("ClassName", "DragBtn");
	ele.SetAttribute("Size_x", 36);
	ele.SetAttribute("Size_y", 36);
	ele.SetAttribute("LogicPos_x", 0);
	ele.SetAttribute("LogicPos_y", 0);
	ele.SetAttribute("TextRect_left", 0);
	ele.SetAttribute("TextRect_top", 0);
	ele.SetAttribute("TextRect_right", 36);
	ele.SetAttribute("TextRect_bottom", 36);
	ele.SetAttribute("PicColor", 0xDDFF6666);
	ele.SetAttribute("Resizable", "no");
	ele.SetAttribute("Movable", "yes");
	ele.SetAttribute("CanMoveOutOfFather", "yes");
	DragBtn* pDragBtn =  (DragBtn*)pGUISys->CreateWnd(_T("\\desktop"), &ele);	
	ASSERT(pDragBtn);
	pDragBtn->SetInvisible(TRUE);
	return pDragBtn;
}

void DragBtn::DestroyDragBtn()
{
	DragBtn* pDrag = (DragBtn*)TObjRef<GUISystem>()->GetWnd(_T("\\desktop\\DragButton"));
	if(P_VALID(pDrag))
	{
		pDrag->EndDrag();
		TObjRef<GUISystem>()->DestroyWnd(pDrag);
	}
}

BOOL DragBtn::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch(pMsg->dwType) 
	{
	case WM_MOUSEMOVE:
		{
			//-�ƶ���ָ��λ��
			//if(m_pSystem->GetMousePoint() == this)
			{
				MoveToPos(pMsg->pt.x,pMsg->pt.y);	
				return TRUE;
			}			
		}
		break;
	case WM_RBUTTONUP:				//�Ҽ�̧��ʱ����϶���ť
		{
			EndDrag();
		}
		break;
	case WM_LBUTTONUP:
		{
			m_nClickUp++;

			// ��һ�ν���WM_LBUTTONUP��Ϣ����������
			if( m_nClickUp == 1 )
				return FALSE;

			// �ڶ��ν���WM_LBUTTONUP��Ϣ����Drog����
			switch( m_eDragType )
			{
			case EDT_Null:
				break;
			case EDT_Skill:
				{
					SetInvisible(TRUE);
					if(!P_VALID(m_pSrcWnd))
						return FALSE;
					
					tagGUIEvent event( m_pSrcWnd, (EGUIEvent)EGUISCB_Drop, pMsg );
					event.dwParam1 = (DWORD)m_eDragType;
					event.dwParam2 = m_dwParam1;
					event.dwParam3 = m_dwParam2;
					event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);

					//����Ǵӿ�����ϳ�������Ҫ�򸸴��ڷ���
					if( _tcscmp(m_pSrcWnd->GetClassName().c_str(), _T("ShortcutButton")) != 0 )
					{
						m_pFather->OnEvent(&event);
					}
					event.eEvent = (EGUIEvent)EGUISCB_DropCheck;
					m_pSrcWnd->OnEvent(&event);
					
					//�������
					Clear();
				}
				break;
			case EDT_Item:
				{
					SetInvisible(TRUE);
					if(!P_VALID(m_pSrcWnd))
						return FALSE;
				
					// ��Ʒ���Ա�����
					if(PointInScene(pMsg->pt)&&_tcscmp(m_pSrcWnd->GetClassName().c_str(), _T("ShortcutButton")) != 0)
					{
						tagPutDownItemEvent eve(_T("PutDownItem"), NULL);
						eve.eType	 = m_eDragType;
						eve.dwParam	 = m_dwParam1;
						eve.dwTypeID = m_dwParam2;
						TObjRef<GameFrameMgr>()->SendEvent(&eve);
						Clear();
						break;
					}

					tagGUIEvent event( m_pSrcWnd, (EGUIEvent)EGUISCB_Drop, pMsg );
					event.dwParam1 = (DWORD)m_eDragType;
					event.dwParam2 = m_dwParam1;
					event.dwParam3 = m_dwParam2;
					event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);
					
					//����Ǵӿ�����ϳ�������Ҫ�򸸴��ڷ���
					if( _tcscmp(m_pSrcWnd->GetClassName().c_str(), _T("ShortcutButton")) != 0 )
					{
						m_pFather->OnEvent(&event);
					}
					event.eEvent = (EGUIEvent)EGUISCB_DropCheck;
					m_pSrcWnd->OnEvent(&event);

					//�������
					Clear();
				}
				break;
			case EDT_ItemEquip:
			case EDT_ItemWare:
			case EDT_ItemSplit:
			case EDT_ItemShop:
				{
					SetInvisible(TRUE);
					if(!P_VALID(m_pSrcWnd))
						return FALSE;

					tagGUIEvent event( m_pSrcWnd, (EGUIEvent)EGUISCB_Drop, pMsg );
					event.dwParam1 = (DWORD)m_eDragType;
					event.dwParam2 = m_dwParam1;
					event.dwParam3 = m_dwParam2;
					event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);

					m_pFather->OnEvent(&event);
					
					//�������
					Clear();
				}
				break;
            case EDT_GuildWare:
                {
                    SetInvisible(TRUE);
                    if(!P_VALID(m_pSrcWnd))
                        return FALSE;

                    tagGUIEvent event( m_pSrcWnd, (EGUIEvent)EGUISCB_Drop, pMsg );
                    event.dwParam1 = (DWORD)m_eDragType;
                    event.dwParam2 = m_dwParam1;
                    event.dwParam3 = m_dwParam2;
                    event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);

                    //����Ǵӿ�����ϳ�������Ҫ�򸸴��ڷ���
                    if( _tcscmp(m_pSrcWnd->GetClassName().c_str(), _T("ShortcutButton")) != 0 )
                    {
                        m_pFather->OnEvent(&event);
                    }
                    event.eEvent = (EGUIEvent)EGUISCB_DropCheck;
                    m_pSrcWnd->OnEvent(&event);

                    //�������
                    Clear();
                }
                break;
            case EDT_PetEquip:
                {
                    SetInvisible(TRUE);
                    if(!P_VALID(m_pSrcWnd))
                        return FALSE;

                    tagGUIEvent event( m_pSrcWnd, (EGUIEvent)EGUISCB_Drop, pMsg );
                    event.dwParam1 = (DWORD)m_eDragType;
                    event.dwParam2 = m_dwParam1;
                    event.dwParam3 = m_dwParam2;
                    event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);

                    //����Ǵӿ�����ϳ�������Ҫ�򸸴��ڷ���
                    if( _tcscmp(m_pSrcWnd->GetClassName().c_str(), _T("ShortcutButton")) != 0 )
                    {
                        m_pFather->OnEvent(&event);
                    }
                    event.eEvent = (EGUIEvent)EGUISCB_DropCheck;
                    m_pSrcWnd->OnEvent(&event);

                    //�������
                    Clear();
                }
                break;
			}
		}
		break;
	}
	return GUIStatic::OnInputMessage(pMsg);
}

VOID DragBtn::OnEvent(tagGUIEvent* pEvent)
{
	switch(pEvent->eEvent)
	{
	case EGUIE_Drag:
		{
			m_pSrcWnd = m_pSystem->GetWnd(pEvent->dwWndID);
			m_eDragType	= (EDragType)pEvent->dwParam1;
			m_dwParam1	= pEvent->dwParam2;
			m_dwParam2	= pEvent->dwParam3;
			m_dwParam3	= pEvent->dwParam4;
			BeginDrage();
		}
		break;
	}
}

VOID DragBtn::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// ��Ⱦ��ͼ
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, 0xFFF5F5F5, m_eAlphaWrite);

}

VOID DragBtn::MoveToPos(INT x,INT y)
{
	if( IsInvisible() == TRUE)
		return;

	m_ptView.x = x - m_ptSize.x/3;				//�����������λ��
	m_ptView.y = y - m_ptSize.y/3;
	
	SetRefresh();
}

VOID DragBtn::BeginDrage()
{
	if( !P_VALID(m_pSrcWnd) )
		return;
	if( !P_VALID(((ButtonEx*)m_pSrcWnd)) )
		return;

	if(m_eDragType == EDT_Null)
	{
		EndDrag();
		return;
	}

	tstring picName = ((ButtonEx*) m_pSrcWnd)->GetPicFileName();
	m_pRender->DestroyImage(m_pPic);						// ɾ������ͼƬ
	m_strPic = picName;
	m_pPic = m_pRender->CreateImage(m_strPic, m_rcPic);		// ����ͼƬ
	

	SetInvisible(FALSE);
	m_nClickUp = 0;
	POINT pt = { LOWORD(m_dwParam3), HIWORD(m_dwParam3)};	
	MoveToPos(pt.x,pt.y);
	OnActive();
	m_pSystem->SetActive(this);				//���ý���ؼ�
	this->FlipToTop();
	this->SetTopMost(true);
	m_ptLastMouseClickView = pt;			//��¼�϶���ʼλ��
}

VOID DragBtn::EndDrag()
{
	Clear();								//����϶���ť��¼������
	m_nClickUp = 0;
	SetInvisible(TRUE);
}

bool DragBtn::PointInScene( tagPoint& pt )
{
	GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
	if(!P_VALID(pDesktop) || !pDesktop->PointInWnd(pt, false))
		return false;

	// �����Ƿ������洰��������Ӵ�����
	list<GUIWnd*> listChild = pDesktop->GetAllChild();
	list<GUIWnd*>::iterator it;
	for(it=listChild.begin(); it!=listChild.end(); it++)
	{
		if( ((GUIWnd*)(*it))->PointInWnd(pt) )
			return false;
	}
	return true;
}