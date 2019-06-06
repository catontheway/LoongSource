#include "StdAfx.h"
#include "ButtonEx.h"
#include "CombatEvent.h"
#include "SkillMgr.h"
#include "ItemMgr.h"
#include "PetManager.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\specfunc_define.h"
#include "ItemEvent.h"
#include "Container.h"
#include "ItemProtoData.h"

ButtonEx::ButtonEx(void)
{
	m_fCDCount		= -1.0f;
	m_fRestoreRatio = 0.0f;
}

ButtonEx::~ButtonEx(void)
{

}

RotaterInRect	ButtonEx::m_Rotater;
BOOL ButtonEx::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRes = GUIButton::Init(pSystem, pFather, pXmlElement);
	if(!bRes)
		return bRes;

	m_pShadowFont = m_pRender->CreateFont(g_StrTable[_T("Font_Kaiti")], 15, 20, 1, !m_bNoAntiAliase);
	m_bCanUse = false;
	m_dwDisablePicColor	= 0xFF5B5B5B;


	return TRUE;
}

VOID ButtonEx::Destroy()
{
	m_pRender->DestroyImage(m_pPushPic);
	m_pRender->DestroyImage(m_pMovePic);
	m_pRender->DestroyImage(m_pDisablePic);
	m_pRender->DestroyFont(m_pShadowFont);
	GUIStatic::Destroy();
}

/**
	//	!���������º󣬽���DragBtnͳһ�����϶��¼�����DragBtn�ؼ�OnEvent()
	//	!����Ҽ����º��򸸿ؼ����Ͱ����¼����ɸ��ؼ��Ļص�����������
*/
BOOL ButtonEx::OnInputMessage(tagGUIInputMsg* pMsg)					
{
	if( m_bDisable )
		return GUIStatic::OnInputMessage(pMsg);
	switch( pMsg->dwType )
	{
		case WM_LBUTTONDOWN:
			{				
				tagGUIEvent event(this, EGUIE_Drag, pMsg);
				event.dwParam1 = m_BtnData.GetType();
				event.dwParam2 = m_BtnData.GetData1();
				event.dwParam3 = m_BtnData.GetData2();
				event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);	
				SendEvent(&event);				
			}
			break;
		case WM_RBUTTONDOWN:
			{
				tagGUIEvent event(this, (EGUIEvent)EGUISCB_RightMouseDown, pMsg);
				event.dwParam1 = pMsg->dwType;
				event.dwParam2 = pMsg->pt.x;
				event.dwParam3 = pMsg->pt.y;
				SendEvent(&event);
			}
			break;		
	}
	return GUIStatic::OnInputMessage(pMsg);
}

void ButtonEx::DrawImg()
{
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);

	switch(m_eState)
	{
	case EGUIBS_PushDown:
		m_pRender->Draw(m_pCanvas, m_pPushPic, m_dwPushPicColor, m_eAlphaWrite);
		break;
	case EGUIBS_HighLight:
		m_pRender->Draw(m_pCanvas, m_pMovePic, m_dwMovePicColor, m_eAlphaWrite);
		break;
	default:	
		m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);
		//��ť������ʱ,���ڱ����ϻ���һ����͸���Ľ���ͼƬ,����ͼƬ�ͽ���ͼƬ�ͻ�����һ��
		if( m_bDisable )			
			m_pRender->Draw(m_pCanvas, m_pPic, m_dwDisablePicColor, m_eAlphaWrite);
		break;
	}
	if( !m_pPic )	// ���û��ͼƬ���Լ���Ⱦ
	{
		if( EGUIBS_PushDown == m_eState )
		{
			// ����
			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.bottom = m_pCanvas->rc.top;
			m_pRender->Line(m_pCanvas, m_dwTopColor);

			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.right = m_pCanvas->rc.left;
			m_pRender->Line(m_pCanvas, m_dwTopColor);

			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.right--;
			m_pCanvas->rc.left = m_pCanvas->rc.right ;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);

			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.bottom--;
			m_pCanvas->rc.top = m_pCanvas->rc.bottom;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);
		}
		else
		{
			// ����
			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.bottom = m_pCanvas->rc.top;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);

			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.right = m_pCanvas->rc.left;
			m_pRender->Line(m_pCanvas, m_dwBottomColor);

			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.right--;
			m_pCanvas->rc.left = m_pCanvas->rc.right ;
			m_pRender->Line(m_pCanvas, m_dwTopColor);

			m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
			m_pCanvas->rc.bottom--;
			m_pCanvas->rc.top = m_pCanvas->rc.bottom;
			m_pRender->Line(m_pCanvas, m_dwTopColor);
		}
	}

}

VOID ButtonEx::Render()
{
	// ���û�����Ⱦ
	GUIWnd::Render();
}

VOID ButtonEx::Update()
{
	//������Update()�����SetRefresh()������ˢ��
	CalCDTime((int)m_fCDCount);
	
	GUIButton::Update();
}

//����----�ṩ������ȴ�ָ�ʱ��
int ButtonEx::GetBtnRestoreTime(float& fRestoreRatio)
{
	if( m_BtnData.GetType() == ESBT_Item)
	{
		ItemMgr::Inst()->GetItemCurCDTime(m_BtnData.GetData2(), m_fCDCount, fRestoreRatio);
	}
	else if( m_BtnData.GetType() == ESBT_Skill)
	{
		SkillMgr::Inst()->GetSkillCDTime(MTransSkillID(m_BtnData.GetData1()), m_fCDCount, fRestoreRatio);
	}
    else if ( m_BtnData.GetType() == ESBT_PetSkill)
    {
        PetManager::Inst()->GetSkillCDTime(m_BtnData.GetData1(), m_BtnData.GetData2(), m_fCDCount, fRestoreRatio);
    }

	return 10;
}

POINT pPt[16];
//����ֵ  ʣ��ʱ�� ����CD��ȴ��ת��ͼƬ
int ButtonEx::DrawShadow(int nSquareWidth)
{
	if( m_BtnData.GetType()== ESBT_Null )
		return 0;

	tagPoint ptView = m_ptView - m_pCanvas->ptOffset;
	
	m_bCanUse = true;
	
	if( GetBtnRestoreTime( m_fRestoreRatio )	// ����״̬
		&& m_fCDCount > 0 )		// ��Ҫ��ȴ״̬
	{
		m_bCanUse = false; 
		if(m_fRestoreRatio >= 1.0f)
			m_fRestoreRatio = 0.999999f;
		// ������������
		list<POINT> listPt;
		m_Rotater.GetPointList( m_fRestoreRatio, listPt, m_ptSize.x );
		int nCount = listPt.size();

		ASSERT(nCount+1 < 16);	
		GUIWnd* pDesktop = m_pSystem->GetDesktop();

		pPt[0].x = ptView.x + m_ptSize.x/2 ;
		pPt[0].y = ptView.y + m_ptSize.y/2 ;

		int nIndex = 1;
		for( list<POINT>::iterator it = listPt.begin(); it != listPt.end(); it++ )
		{
			pPt[nIndex].x = (*it).x + pPt[0].x;
			pPt[nIndex].y = (*it).y + pPt[0].y;
			nIndex++;
		}
		// ���ƶ����
		Draw2DPrimitiveUP( EDraw_TriangleFan, pPt, nCount+1, 0xCC1D1A2C );
		
		// ����CDʱ��
		DrawShadowText();

	}
	return (int)m_fCDCount;
}

//ת��CDʱ��(����)
void ButtonEx::CalCDTime(int nTime)
{
	if(nTime<0)
		return;

	TCHAR szTxt [10]  = { 0 };
	if( nTime >= 3600000 )
		_stprintf( szTxt, _T("%dh"), nTime/3600000 );	// Сʱ��
	else if( nTime >= 60000 )
		_stprintf( szTxt, _T("%dm"), nTime/60000 );		// ���Ӽ�
	else if( nTime >= 1000 )
		_stprintf( szTxt, _T("%d"), (nTime+999)/1000 );	// ���
	else if( nTime < 1000 && nTime > 0) 
	{
		_stprintf( szTxt, _T("1") );					// С��1��
	}
	else
		_stprintf( szTxt, _T("") );
	
	m_strCDTime = szTxt;
	SetRefresh();
}

//������ȴʱ�����ʾ
void ButtonEx::DrawShadowText()
{
	if( !m_strCDTime.empty() )
	{
		tagGUIFont* pFont = GetFont();
		switch(m_eState)
		{
		case EGUIBS_PushDown:
			m_pCanvas->rc = m_pCanvas->rc + 1;
			m_pRender->Text(m_pCanvas, m_strCDTime, m_pShadowFont, 0xFFF0F000, 0xA0FFA000, EGUITA_Center, false);
			break;
		case EGUIBS_HighLight:
			m_pRender->Text(m_pCanvas, m_strCDTime, m_pShadowFont, 0xFFF0F000, 0xA0FFA000, EGUITA_Center, false);
			break;
		default:
			m_pRender->Text(m_pCanvas, m_strCDTime, m_pShadowFont, 0xFFF0F000, 0xA0FFA000, EGUITA_Center, false);
			break;
		}
	}
}

//���ƶ����
VOID ButtonEx::Draw2DPrimitiveUP( EDrawType eType, LPPOINT lpPoints, int nVerCount, DWORD color)
{
	IDraw2D::Inst()->Draw( eType, lpPoints, nVerCount, color, IDraw2D::EAWT_NoWrite );	
}

//���ÿ�ݰ�ť��ͼƬ
void ButtonEx::SetWndPic(eWndPicType ePicType, const tstring& strPic)
{
	/*if( strPic.empty() )
		return;*/
	switch(ePicType)
	{
	case EWPICTYPE_CURPIC:
		{
			m_pRender->DestroyImage(m_pPic);						// ɾ������ͼƬ
			m_strPic = strPic;
			m_pPic = m_pRender->CreateImage(m_strPic, m_rcPic);		// ����ͼƬ
			if(P_VALID(m_pPic))
				m_dwPicColor = 0xFFFFFFFF;
			else 
				m_dwPicColor = 0xFF5B5B5B;
		}
		break;
	case EWPICTYPE_PUSHPIC:
		{
			m_pRender->DestroyImage(m_pPushPic);
			m_strPushPicFileName = strPic;
			m_pPushPic = m_pRender->CreateImage(m_strPushPicFileName, m_rcPushPic);
		}
		break;
	case EWPICTYPE_MOVEPIC:
		{
			m_pRender->DestroyImage(m_pMovePic);
			m_strMovePicFileName = strPic;
			m_pMovePic = m_pRender->CreateImage(m_strMovePicFileName, m_rcMovePic);
		}
		break;
	case EWPICTYPE_DISABLEPIC:
		{
			m_pRender->DestroyImage(m_pDisablePic);
			m_strDisablePicFileName = strPic;
			m_pDisablePic = m_pRender->CreateImage(m_strDisablePicFileName, m_rcDisablePic);
		}
		break;
	}
}

//������Ϸ�¼�
void ButtonEx::InvokeGameEvent()
{
	switch( m_BtnData.GetType() )
	{

	case ESBT_Null:
		break;
	case ESBT_Skill:
		{
			SkillMgr::Inst()->OnUseSkill(m_BtnData.GetData1());
		}
		break;
	case ESBT_Item:
		{
			tagClickItemEvent evt(_T("tagClickItemEvent"), NULL);
			evt.eType = EICT_Bag;
			evt.n16Index = INT16(m_BtnData.GetData1());
			evt.dwTypeID = m_BtnData.GetData2();
			TObjRef<GameFrameMgr>()->SendEvent(&evt);
			
			break;
		}
	}
}

void ButtonEx::SetBtnData( EShortcutBtnType eType, DWORD dwParam1, DWORD dwParam2, INT64 n64Param3 )
{
	m_BtnData.Set(eType, dwParam1, dwParam2, n64Param3);
}
