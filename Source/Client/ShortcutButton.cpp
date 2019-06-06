#include "StdAfx.h"
#include "ShortcutButton.h"
#include ".\ShortcutBarFrame.h"
#include ".\DragBtn.h"
#include "SkillMgr.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "ItemMgr.h"
#include "Container.h"

ShortcutButton::ShortcutButton(void)
{
	m_dwStartPressTime	= 0;
	m_strKeyBar			= _T("");
	m_strItemNum		= _T("");
	m_nItemNum			= 0;
	m_nDragFlag				= 0;
	m_pFrame			= NULL;
	m_bIsInvalite		= FALSE;
	m_curFrame			= 0;
	m_frameTime			= 0;
	m_bCacheImg			= FALSE;
	m_bKeyEnable		= TRUE;
}

ShortcutButton::~ShortcutButton(void)
{
}

VOID ShortcutButton::Destroy()
{
	m_pRender->DestroyImage(m_pBtnInvalitePic);
	for(int i=0; i<4; i++)
	{
		m_pRender->DestroyImage(m_pBtnGlintPic[i]);
	}
	m_pRender->DestroyImage(m_pBtnCachePic);

	ButtonEx::Destroy();
}

BOOL ShortcutButton::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRes = ButtonEx::Init(pSystem, pFather, pXmlElement);
	if(!bRes)
		return bRes;

	
	// ����Ƕ�����ֵ
	static bool bFirst = true;
	if( bFirst )
	{
		bFirst = false;
		m_Rotater.InitSquare();
	}

	m_pBtnInvalitePic = m_pRender->CreateImage(_T("data\\ui\\Sys\\L_icon-wuxiao.bmp"), m_rcPic);	

	for(int i=0; i<4; i++)
	{
		TCHAR szBuff[X_LONG_NAME];
		_stprintf(szBuff, _T("data\\ui\\Main\\L-shortcut-%c.bmp"), _T('a')+i);
		m_pBtnGlintPic[i] =  m_pRender->CreateImage(szBuff, m_rcPic);
	}

	m_pBtnCachePic = m_pRender->CreateImage(_T("data\\ui\\Main\\L-shortcut-a.bmp"), m_rcPic);	

	m_pFrame = (ShortcutBarFrame*)m_pFrameMgr->GetFrame(_T("ShortcutBar"));
											
	return TRUE;
}


VOID ShortcutButton::SendEvent(tagGUIEvent* pEvent)
{
	GUIButton::SendEvent(pEvent);
}

//�����ݰ�ť���յ�GUI�¼�
VOID ShortcutButton::OnEvent(tagGUIEvent* pEvent)
{
	switch(pEvent->eEvent)
	{
	case EGUISCB_Drop:
		{
			if(IsInvisible() == TRUE)
				return;
			tagPoint pt ( LOWORD( pEvent->dwParam4 ), HIWORD( pEvent->dwParam4 ) );
			if( PointInWnd( pt ) )
			{
				GUIWnd* pWnd = m_pSystem->GetWnd(pEvent->dwWndID);
				if(pWnd == this)
				{
					ClearDropFlag();
					return;
				}

				//�����ť����ʱ
				if( this->IsInvisible() )
					return;

				EShortcutBtnType eBtnType = DragType2SCBtnType( EDragType(pEvent->dwParam1) );
				if( eBtnType == ESBT_Null )
					return;

				bool bIsShortcutBtn = _tcscmp( pWnd->GetClassName().c_str(), _T("ShortcutButton") ) == 0;

				if( eBtnType == ESBT_Item )														
				{
					//��Ʒ	
					SetItemData( pEvent->dwParam2, pEvent->dwParam3 );
				}
				else 
				{
					//���漼������
					SetBtnData( eBtnType, pEvent->dwParam2, pEvent->dwParam3, 0 );	
					//��ݰ�ťͼƬ����
					tstring picName = SkillMgr::Inst()->GetSkillIconPath(m_BtnData.GetData1());
					((ButtonEx*)this)->ClearPic();
					SetPicFileName(picName);
					SetItemNum(0);
					tagGameEvent e(_T("Weapon_SwapEx"), NULL);
					m_pFrameMgr->SendEvent(&e);
				}
				m_pFrame->ReflashCDTime();
				m_pFrame->SaveBtnData();
			}
		}
		break;
	case EGUISCB_DropCheck:
		{
			if(m_nDragFlag)
			{
				m_nDragFlag = 0;
				ButtonEx* pDstBtn = NULL;
				tagPoint ptMouse(LOWORD(pEvent->dwParam4), HIWORD(pEvent->dwParam4));
				GUIWnd* pWnd = m_pSystem->GetWnd(pEvent->dwWndID);
				for(int row=0; row<10; row++)
				{
					for(int col=0; col<8; col++)
					{
						pDstBtn = m_pFrame->GetShortcutButtion(row, col);
						if( !P_VALID(pDstBtn) )
							continue;

						if(pDstBtn->PointInWnd(ptMouse))
						{
							//�ͷŵ�ԭ���϶��İ�ť��
							if( pDstBtn == pWnd )		
							{
								ClearDropFlag();
								return;
							}

							//���Ŀ�갴ť������
							if(pDstBtn->IsInvisible())
							{	
								m_nDragFlag = 0;
								SetBtnData( ESBT_Null, 0x0, 0x0, 0x0);
								//��ݰ�ťͼƬ����
								((ButtonEx*)pWnd)->ClearPic();
								SetItemNum(0);
								m_bIsInvalite = false;
								m_pFrame->SaveBtnData();
								return;
							}

							EShortcutBtnType eBtnType = DragType2SCBtnType( EDragType(pEvent->dwParam1) );

							bool bIsShortcutBtn = _tcscmp( pWnd->GetClassName().c_str(), _T("ShortcutButton") ) == 0;

							BtnData dstData = pDstBtn->GetBtnData();
							tstring oldPicName = this->GetPicFileName();

							//���Ŀ���ݰ�ťû������
							if(dstData.GetType() == ESBT_Null)
							{	
								INT64 n64ID = GetBtnData().GetData3();
								//���ԭ��ݰ�ť�������
								SetBtnData( ESBT_Null, 0x0, 0x0, 0x0 );
								SetItemNum(0);

								//����Ŀ���ݰ�ť����								
								if(eBtnType==ESBT_Item && !MIsEquipment(pEvent->dwParam3))
								{
									const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(pEvent->dwParam3);
									INT16 nItemCount;
									//�ǲ��ɶѵ�����Ʒ
									if( pItemProto->n16MaxLapNum == 1 )
									{
										nItemCount = INT16( (pEvent->dwParam2 & 0xFFFF000) >> 16 );
									}
									//�ɶѵ�����Ʒ
									else 
									{
										ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
										nItemCount = pContainer->GetItemQuantity(pEvent->dwParam3);
									}

									((ShortcutButton*)pDstBtn)->SetItemNum(nItemCount);
								}
								else if(eBtnType==ESBT_Item && MIsEquipment(pEvent->dwParam3))
								{
									((ShortcutButton*)pDstBtn)->SetItemNum(0);
								}
								else if(eBtnType==ESBT_Skill)
								{
									((ShortcutButton*)pDstBtn)->SetItemNum(0);
									((ShortcutButton*)pWnd)->SetInvalite(false);
								}
								
								pDstBtn->SetBtnData( eBtnType, pEvent->dwParam2, pEvent->dwParam3, n64ID );
								pDstBtn->SetPicFileName(oldPicName);
								((ButtonEx*)pWnd)->ClearPic();
							}
							//���Ŀ���ݰ�ť�Ǽ���
							else if(dstData.GetType() == ESBT_Skill)
							{
								//����ԭ��ݰ�ť(�ȱ���ԭ��ť��data3����)
								INT64 n64ID = GetBtnData().GetData3();
								SetBtnData(dstData);
								SetItemNum(0);
								tstring picName = pDstBtn->GetPicFileName();
								SetPicFileName(picName);

								//����Ŀ���ݰ�ť
								if( eBtnType==ESBT_Item && !MIsEquipment(pEvent->dwParam3) )
								{
									const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(pEvent->dwParam3);
									INT16 nItemCount;
									//�ǲ��ɶѵ�����Ʒ
									if( pItemProto->n16MaxLapNum == 1 )
									{
										nItemCount = INT16( (pEvent->dwParam2 & 0xFFFF000) >> 16 );
									}
									//�ɶѵ�����Ʒ
									else 
									{
										ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
										nItemCount = pContainer->GetItemQuantity(pEvent->dwParam3);
									}

									((ShortcutButton*)pDstBtn)->SetItemNum(nItemCount);
								}
								else if(eBtnType==ESBT_Item && MIsEquipment(pEvent->dwParam3))
								{
									((ShortcutButton*)pDstBtn)->SetItemNum(0);
								}
								else if(eBtnType==ESBT_Skill)
								{
									((ShortcutButton*)pDstBtn)->SetItemNum(0);
								}
								pDstBtn->SetBtnData( eBtnType, pEvent->dwParam2, pEvent->dwParam3, n64ID  );
								pDstBtn->SetPicFileName(oldPicName);
								
							}
							else if( dstData.GetType() == ESBT_Item )
							{
								//����Դ��ݰ�ť
								INT64 n64ID = GetBtnData().GetData3();
								INT64 n64DstID = dstData.GetData3();
								SetItemData(dstData.GetData1(), dstData.GetData2());
								SetBtnData(ESBT_Item, dstData.GetData1(), dstData.GetData2(), n64DstID);
								
								//����Ŀ���ݰ�ť
								if( eBtnType==ESBT_Item && !MIsEquipment(pEvent->dwParam3) )
								{
									const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(pEvent->dwParam3);
									INT16 nItemCount;
									//�ǲ��ɶѵ�����Ʒ
									if( pItemProto->n16MaxLapNum == 1 )
									{
										nItemCount = INT16( (pEvent->dwParam2 & 0xFFFF000) >> 16 );
									}
									//�ɶѵ�����Ʒ
									else 
									{
										ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
										nItemCount = pContainer->GetItemQuantity(pEvent->dwParam3);
									}

									((ShortcutButton*)pDstBtn)->SetItemNum(nItemCount);
									if( nItemCount==0 )
										((ShortcutButton*)pDstBtn)->SetInvalite(true);
									else
										((ShortcutButton*)pDstBtn)->SetInvalite(false);
								}
								else if(eBtnType==ESBT_Item && MIsEquipment(pEvent->dwParam3))
								{
									((ShortcutButton*)pDstBtn)->SetItemNum(0);
								}
								else if(eBtnType==ESBT_Skill)
								{
									((ShortcutButton*)pDstBtn)->SetItemNum(0);
								}
								pDstBtn->SetBtnData( eBtnType, pEvent->dwParam2, pEvent->dwParam3, n64ID  );
								pDstBtn->SetPicFileName(oldPicName);
							}
							m_pFrame->ReflashCDTime();
							m_pFrame->SetBtnEnable();
							tagGameEvent e(_T("Weapon_SwapEx"), NULL);
							m_pFrameMgr->SendEvent(&e);
							m_pFrame->SaveBtnData();
							return;
						}
					}
				}
				//�ӿ�������ϳ�ʱû���ͷŵ�������ݰ�ťʱ
				m_nDragFlag = 0;
				SetBtnData( ESBT_Null, 0x0, 0x0, 0x0);
				//��ݰ�ťͼƬ����
				((ButtonEx*)pWnd)->ClearPic();
				SetItemNum(0);
				m_bIsInvalite = false;
				m_pFrame->SaveBtnData();
			}
		}
		break;
	}
}

BOOL ShortcutButton::OnInputMessage(tagGUIInputMsg* pMsg)
{
	if( m_bDisable )
		return GUIStatic::OnInputMessage(pMsg);

	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		if( m_pSystem->GetMousePoint() == this ) 
		{
			m_eState = EGUIBS_PopUp;
			m_bHaveMouseKeyDown = true;
			SetRefresh();
		}
		break;
	case WM_LBUTTONUP:
		if( m_bHaveMouseKeyDown && PointInWnd(pMsg->pt) )
		{
			tagGUIEvent event(this, (EGUIEvent)EGUISCB_LeftMouseDown, pMsg);
			SendEvent(&event);
			m_eState = EGUIBS_PopUp;
			m_bHaveMouseKeyDown = false;
			SetRefresh();
			return true;
		}
		else
			m_eState = EGUIBS_PopUp;
		m_bHaveMouseKeyDown = false;
		SetRefresh();
		break;
	case WM_MOUSEMOVE:
		if( m_pSystem->GetMousePoint() == this )
		{ 
			if( (MK_LBUTTON & pMsg->dwParam1) && m_bHaveMouseKeyDown )
			{
				m_eState = EGUIBS_PopUp;
			}
			else
			{
				//��������ݰ�ť(��)
				m_eState = EGUIBS_PopUp;
				tagGUIEvent event(this, (EGUIEvent)EGUIE_MoveIntoCtrl, pMsg);
				event.dwParam1 = m_BtnData.GetType();
				event.dwParam2 = m_BtnData.GetData1();
				event.dwParam3 = m_BtnData.GetData2();
				SendEvent(&event);
			}
		}
		else
		{
			m_eState = EGUIBS_PopUp;
			if( (MK_LBUTTON & pMsg->dwParam1) 
				&& PointInWnd(pMsg->pt) == FALSE )
			{
				m_nDragFlag = 1;
				tagGUIEvent event(this, EGUIE_Drag, pMsg);
				event.dwParam1 = m_BtnData.GetType();
				event.dwParam2 = m_BtnData.GetData1();
				event.dwParam3 = m_BtnData.GetData2();
				event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);	
				SendEvent(&event);
			}	

			//����Ƴ���ݰ�ť(��)
			/*tagGUIEvent event(this, (EGUIEvent)EGUIE_MoveOutOfCtrl, pMsg);
			SendEvent(&event);*/

		}
		SetRefresh();
		break;	
	case WM_RBUTTONDOWN:
		{

		}
		break;
	case WM_RBUTTONUP:
		{
			if(PointInWnd(pMsg->pt))
			{
				tagGUIEvent event(this, (EGUIEvent)EGUISCB_RightMouseDown, pMsg);
				SendEvent(&event);
				return true;
			}
			SetRefresh();
		}
		break;
	}
	return GUIStatic::OnInputMessage(pMsg);
}

VOID ShortcutButton::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();
		
	DrawImg();

	if(m_bIsInvalite)
	{
		m_pRender->Draw(m_pCanvas, m_pBtnInvalitePic, 0xFFFFFFFF, m_eAlphaWrite);
	}

	if(m_bCacheImg)
	{
		m_pRender->Draw(m_pCanvas, m_pBtnCachePic, 0xFFFFFFFF, m_eAlphaWrite);
	}

	//���ƿ�ݼ�ֵ
	DrawBarText();

	DrawPressFrame();

	//����ʹ�ÿ�ݼ��Ĵ���(����)
	//int Time;
	if( m_BtnData.GetType() != ESBT_Null )
	{
		 DrawShadow(m_ptSize.x);
	}

	// ���û�����Ⱦ
	ButtonEx::Render();
}

VOID ShortcutButton::Update()
{
	float FPS = 10;

	m_frameTime += Kernel::Inst()->GetDeltaTime();

	if(m_frameTime > 1.0f/FPS)
	{
		m_curFrame++;
		m_frameTime -= 1.0f/FPS;
	}

	if(m_curFrame >= 4)
		m_curFrame = 0;

	if( m_dwStartPressTime!=0 )
		SetRefresh();


	ButtonEx::Update();
}

//��������ݰ�ťʱ��˸ͼ��ʱ��
BOOL ShortcutButton::BeginPressBtn()
{
	m_dwStartPressTime = timeGetTime();
	m_curFrame	= 0;
	SetRefresh(TRUE);
	return m_bIsInvalite;
}

void ShortcutButton::EndPressBtn()
{
	m_dwStartPressTime = 0;
}

//���Ƶ���������ť��ʱ��ʾ��˸��ͼƬ
void ShortcutButton::DrawPressFrame()
{
	DWORD dwCurTime;
	dwCurTime = timeGetTime();


	if( m_dwStartPressTime != 0 )
	{
		if( dwCurTime - m_dwStartPressTime > 500 )
		{
			m_dwStartPressTime = 0;
		}
		else if( dwCurTime > m_dwStartPressTime )
		{
			m_pRender->Draw(m_pCanvas, m_pBtnGlintPic[m_curFrame], 0xFFFFFFFF, EGUIAW_Add);
		}
	}
}

//���ƿ�ݰ�ť�ϵ�����
void ShortcutButton::DrawBarText()
{
	// ��Ⱦ����
	if( !m_strKeyBar.empty() )
	{
		m_pCanvas->rc = m_rcText + m_ptView;

		if( m_bDisable )
			m_pRender->Text(m_pCanvas, m_strKeyBar, m_pFont, m_dwDisableTextColor, m_dwDisableShadeColor, EGUITA_RightTop, m_bTextWriteAlpha);
		else
		{
			if( m_bKeyEnable )
			{
				switch(m_eState)
				{
				case EGUIBS_PushDown:
					m_pCanvas->rc = m_pCanvas->rc + 1;
					m_pRender->Text(m_pCanvas, m_strKeyBar, m_pFont, m_dwPushTextColor, m_dwPushShadeColor, EGUITA_RightTop, m_bTextWriteAlpha);
					break;
				case EGUIBS_HighLight:
					m_pRender->Text(m_pCanvas, m_strKeyBar, m_pFont, m_dwMoveTextColor, m_dwMoveShadeColor, EGUITA_RightTop, m_bTextWriteAlpha);
					break;
				default:
					m_pRender->Text(m_pCanvas, m_strKeyBar, m_pFont, m_dwTextColor, m_dwShadeColor, EGUITA_RightTop, m_bTextWriteAlpha);
					break;
				}
			}
			else
			{
				switch(m_eState)
				{
				case EGUIBS_PushDown:
					m_pCanvas->rc = m_pCanvas->rc + 1;
					m_pRender->Text(m_pCanvas, m_strKeyBar, m_pFont, 0xFFFF0000, m_dwPushShadeColor, EGUITA_RightTop, m_bTextWriteAlpha);
					break;
				case EGUIBS_HighLight:
					m_pRender->Text(m_pCanvas, m_strKeyBar, m_pFont, 0xFFFF0000, m_dwMoveShadeColor, EGUITA_RightTop, m_bTextWriteAlpha);
					break;
				default:
					m_pRender->Text(m_pCanvas, m_strKeyBar, m_pFont, 0xFFFF0000, m_dwShadeColor, EGUITA_RightTop, m_bTextWriteAlpha);
					break;
				}
			}
		}
	}

	m_pCanvas->rc = m_rcText + m_ptView;
	if( m_bDisable )
		m_pRender->Text(m_pCanvas, m_strItemNum, m_pFont, m_dwDisableTextColor, m_dwDisableShadeColor, EGUITA_RightBottom, m_bTextWriteAlpha);
	else
	{
		switch(m_eState)
		{
		case EGUIBS_PushDown:
			m_pCanvas->rc = m_pCanvas->rc + 1;
			m_pRender->Text(m_pCanvas, m_strItemNum, m_pFont, m_dwPushTextColor, m_dwPushShadeColor, EGUITA_RightBottom, m_bTextWriteAlpha);
			break;
		case EGUIBS_HighLight:
			m_pRender->Text(m_pCanvas, m_strItemNum, m_pFont, m_dwMoveTextColor, m_dwMoveShadeColor, EGUITA_RightBottom, m_bTextWriteAlpha);
			break;
		default:
			m_pRender->Text(m_pCanvas, m_strItemNum, m_pFont, m_dwTextColor, m_dwShadeColor, EGUITA_RightBottom, m_bTextWriteAlpha);
			break;
		}
	}
}

//��Ʒ����
void ShortcutButton::SetItemNum(int itemNum)
{ 
	if( itemNum==0 )
	{
		m_strItemNum = _T("");
		return;
	}
	else
	{
		m_nItemNum = itemNum; 
		TCHAR buff[64] = {0};
		_itot(itemNum, buff, 10);
		m_strItemNum = buff;
	}
}

//������Ʒ���͵�����
void ShortcutButton::SetItemData( DWORD dwParam1, DWORD dwParam2 )
{
	if(MIsEquipment(dwParam2))
	{
		//��ݰ�ťͼƬ����
		tstring picName = ItemMgr::Inst()->GetItemIconPath(dwParam2);
		((ButtonEx*)this)->ClearPic();
		SetPicFileName(picName);
		SetItemNum(0);
		//���漼������
		Item* item = ItemMgr::Inst()->GetPocketItem((INT16)dwParam1);
		if( !P_VALID(item) )
			return;

		INT64 n64ID = item->GetItemId();
		SetBtnData( ESBT_Item, dwParam1, dwParam2, n64ID );	
	}
	else if(!MIsEquipment(dwParam2))
	{
		const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(dwParam2);
		//��������
		//�ǲ��ɶѵ�����Ʒ
		if( pItemProto->n16MaxLapNum == 1 )
		{
			INT16 nItemCount = INT16( (dwParam1 & 0xFFFF000) >> 16 );
			SetItemNum(nItemCount);
			if( nItemCount==0 )
				SetInvalite(true);
			else
				SetInvalite(false);
		}
		//�ɶѵ�����Ʒ
		else 
		{
			ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
			INT16 nItemCount = pContainer->GetItemQuantity(dwParam2);
			SetItemNum(nItemCount);
			if( nItemCount==0 )
				SetInvalite(true);
			else
				SetInvalite(false);
		}
			
		//��ݰ�ťͼƬ����
		tstring picName = ItemMgr::Inst()->GetItemIconPath(dwParam2);
		((ButtonEx*)this)->ClearPic();
		SetPicFileName(picName);

		//���漼������
		Item* item = ItemMgr::Inst()->GetPocketItem((INT16)dwParam1);
		if( !P_VALID(item) )	
			return;
			
		INT64 n64ID = item->GetItemId();
		SetBtnData( ESBT_Item, dwParam1, dwParam2, n64ID);	
	}
}

SpecialSkillButton::SpecialSkillButton( void )
{

}

SpecialSkillButton::~SpecialSkillButton( void )
{

}

BOOL SpecialSkillButton::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bRet = ShortcutButton::Init(pSystem, pFather, pXmlElement);

	return bRet;
}

VOID SpecialSkillButton::Destroy()
{
	ShortcutButton::Destroy();
}

VOID SpecialSkillButton::Render()
{
	ShortcutButton::Render();
}

BOOL SpecialSkillButton::OnInputMessage( tagGUIInputMsg* pMsg )
{
	return ShortcutButton::OnInputMessage( pMsg );
}

VOID SpecialSkillButton::OnEvent( tagGUIEvent* pEvent )
{
	
}