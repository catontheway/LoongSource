#include "stdafx.h"
#include "RichEditWithZ.h"
#include "ChatSysDefine.h"
#include ".\RoleMgr.h"
#include ".\LocalPlayer.h"
#include ".\ChatShowPop.h"
#include ".\WorldFrame.h"
#define CURSOR_FLICKER_TIME 500  // �����˸���

#define GUI_MAX_TRANSFER_LEN	50
#define GUI_DEFAULT_ROW_SPACING_ADDON 0

#define SET_FONT_SIZE	 0
#define SET_FONT_COLOR   1
#define SET_FONT_STYLE	 2
#define SET_KEY_START	 3     //���ùؼ���  �ؼ��ֲ��ܳ����������л���100�ֽ�
#define SET_KEY_END		 4
#define SET_LINK_START	 5	   //���ó�������Ϣ
#define SET_LINK_END     6   

const INT	TEX_W = 171;
const INT	TEX_H = 46;
const INT	OFFSET_X = 16;
const INT	OFFSET_Y = 0;

RichEditWithZ::RichEditWithZ( void )
{
}

RichEditWithZ::~RichEditWithZ( void )
{

}

BOOL RichEditWithZ::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bResult = GUIRichEditBox::Init(pSystem, pFather, pXmlElement);
	
	m_pDepthFont = Device()->NewDepthFont();
	//m_pDepthFont->Create(m_nFontWidth, m_nFontHeight, m_nFontWeight, FALSE, m_strFont.c_str());
	m_pDepthFont->Create(8, 18, 18, FALSE);

	m_bScrollLast = FALSE;
	m_bShowCursor = FALSE;
	m_bHoldCursor = FALSE;
	m_bCanSelect  = FALSE;

	return bResult;
}

void RichEditWithZ::RegEmtionPic( const TCHAR* szTran,int index )
{
	if(index<0 || index>=MAX_EMOTION)
		return;

	TCHAR szPicPath[_MAX_PATH] = {0};
	_stprintf(szPicPath, _T("data\\ui\\chat\\emotion\\emo%02d.bmp"), index+1);

	//��ʼע��ת���ַ������Ӧ�ı���
	TCHAR szTranName[32] = {0};
	_tcscpy(szTranName, szTran);	

	if(!this->RegisterPic(szTranName,szPicPath))
		IMSG(_T("������ʾPOPע����ͼʧ��"));
}

VOID RichEditWithZ::Update()
{

}

void RichEditWithZ::DrawInfo( const POINT& pt,float z )
{
	////ȷ���߶Ⱥ�������ʾ�߶�
	if( z<=0.0f || z>=0.5f)
		SetInvisible(TRUE);
	else 
		SetInvisible(FALSE);
	INT nVisLine=0;
	INT nRowNum = GetRowNum(nVisLine);

	m_ptView.x = pt.x + OFFSET_X;
	m_ptView.y = pt.y - (nVisLine-1) * OFFSET_Y - m_ptSize.y;

}