#include "stdafx.h"
#include "ChatShowPop.h"
#include "..\WorldDefine\msg_common_errorcode.h"
#include ".\RoleMgr.h"
#include ".\LocalPlayer.h"
#include ".\MapMgr.h"
#include ".\EmotionFrame.h"
#include ".\RichEditWithZ.h"
#include ".\WorldFrame.h"
#include "FilterData.h"

const float TIME_DRAW = 10.0f;		//һ��ʱ��󲻻���
const float TIME_SAVE = 5.0f;		//һ��ʱ������ٴ��ڻ�����Դ
const int	WND_W = 171;
const int	WND_H = 64;
const INT	OFFSET_W = 14;

const TCHAR* STR_COLOR_BLUE = _T("<color=(254,227,163)>");	//ǳ��ɫ

const TCHAR* STR_COLOR_GOLD = _T("<color=(224,247,255)>");	//��ɫ

const TCHAR* STR_WND_NAME = _T("ZChat_");				//���ڶ�Ӧ��ǰ׺

const char*	WND_NAME = "ZChat_";						//�������ڶ�Ӧ��XmlElement��name

ChatShowPop::ChatShowPop( void )
{

}

ChatShowPop::~ChatShowPop( void )
{

}

static ChatShowPop g_chatShowPop;
ChatShowPop* ChatShowPop::Inst()
{
	return &g_chatShowPop;
}

void ChatShowPop::PushChatInfo( DWORD dwRoleID,const TCHAR* szText )
{
	if(dwRoleID==GT_INVALID)return;
	if(szText==NULL|| _tcslen(szText)==0)
		return;

	map<DWORD,float>::iterator it = m_mapChatShow.find(dwRoleID);
	float curTime = Kernel::Inst()->GetAccumTime();
	if(it!=m_mapChatShow.end())
	{
		//�Ѿ����ڣ���ˢ��ʱ�伴��
		(*it).second=curTime;
	}
	else
	{
		//û�б�����˵�����µĺ���,�ȱ���
		m_mapChatShow[dwRoleID]=curTime;
	}

	//��������
	TCHAR szWndName[X_LONG_NAME]={0};
	_stprintf(szWndName, _T("%s%s%ld"), _T("\\desktop\\"), STR_WND_NAME, dwRoleID);
	RichEditWithZ* pWnd = NULL;
	pWnd = (RichEditWithZ*)TObjRef<GUISystem>()->GetWnd(szWndName);

	if( !P_VALID(pWnd) )
	{
		char szName[32]= {0};
		sprintf(szName, "%s%ld", WND_NAME, dwRoleID);
		XmlElement ele;
		ele.SetAttribute("Name", szName);
		ele.SetAttribute("ClassName", "RichEditWithZ");
		ele.SetAttribute("Size_x", WND_W);
		ele.SetAttribute("Size_y", WND_H);
		ele.SetAttribute("LogicPos_x", 0);
		ele.SetAttribute("LogicPos_y", 0);
		ele.SetAttribute("Font", TObjRef<Util>()->UnicodeToUnicode8(g_StrTable[_T("Font_MSYH")]));
		ele.SetAttribute("FontWidth", 8);
		ele.SetAttribute("FontHeight", 16);
		ele.SetAttribute("TextRect_left", 6);
		ele.SetAttribute("TextRect_top", 5);		
		ele.SetAttribute("TextRect_right", WND_W);
		ele.SetAttribute("TextRect_bottom", WND_H);
		ele.SetAttribute("TextAlign", "LeftTop");
		ele.SetAttribute("AlphaWrite", EGUIAW_Add);
		ele.SetAttribute("TextColor", 0xFFFFF2A4);
		ele.SetAttribute("ShadowColor", 0xFF290404);
		ele.SetAttribute("PicColor", 0x11FFFFFF);
		ele.SetAttribute("PicFileName", "data\\ui\\Chat\\L_chat-diban-diwen.bmp");
		ele.SetAttribute("Resizable", "yes");
		ele.SetAttribute("CanMoveOutOfFather", "yes");
		ele.SetAttribute("ReadOnly", "yes");
		ele.SetAttribute("BottomMost", "yes");
		pWnd = (RichEditWithZ*)TObjRef<GUISystem>()->CreateWnd(_T("\\desktop"), &ele);
		if( !P_VALID(pWnd) )
			return;
		
		pWnd->SetMaxTextCount(MAX_CHAT_LEN);

		TCHAR szEmnName[32] = {0};
		EmotionFrame* pEmotionFrame = (EmotionFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("Emotion") );
		if( !P_VALID(pEmotionFrame) )
			return;
		for(int i=0; i<MAX_EMOTION; i++)
		{
			_tcscpy(szEmnName, pEmotionFrame->GetEmtionTran(i));
			ASSERT(_tcslen(szEmnName)>1);
			pWnd->RegEmtionPic(szEmnName, i);
		}
	}

	if( P_VALID(pWnd) )
	{
		pWnd->ClsText();
		pWnd->SetWidth(WND_W);
		tstring strMsg = szText;
		pWnd->FilterString(strMsg);
		pWnd->AddText(strMsg.c_str());

		//ȷ����Ⱥ͸߶�
		INT nVisLine = 0;
		INT nRowNum = pWnd->GetRowNum(nVisLine);
		bool bFullW=true;
		if(nRowNum==1)
		{
			//ֻ��һ��ʱ��Ҫ������
			INT nRowW = pWnd->GetRowRenderWidth(0);
			if( nRowW > 0 )
			{
				pWnd->SetWidth(nRowW+OFFSET_W);
				bFullW=false;
			}
		}
		if(bFullW)
		{
			pWnd->SetWidth(WND_W);
		}
		
		pWnd->SetHeight(pWnd->GetRowSpacing()*nRowNum+10);

	}

}

void ChatShowPop::NetRecvRoleChat( tagNS_RoleChat* pMsg )
{
	if(pMsg->dwErrorCode!=E_Success)
		return;
	//�ж��Ƿ��Ƕ���
	tstring strText;
	if(pMsg->dwSrcRoleID == RoleMgr::Inst()->GetLocalPlayer()->GetID())
	{
		//�Լ�˵��,�ý�ɫ
		strText.append(STR_COLOR_GOLD);
	}
	else
	{
				
	}
	tstring strTemp = pMsg->szMsg;
	FilterData::Inst()->WordsFilteror(strTemp);
	strText += strTemp;

	//���˵��
	PushChatInfo(pMsg->dwSrcRoleID, strText.c_str());
}

void ChatShowPop::DeleteChatShow( DWORD dwRoleID )
{
	TCHAR szWndName[X_LONG_NAME] = {0};
	_stprintf(szWndName, _T("%s%s%ld"), _T("\\desktop\\"), STR_WND_NAME, dwRoleID);
	GUIWnd* pWnd = TObjRef<GUISystem>()->GetWnd(szWndName);

	if( P_VALID(pWnd) )
		TObjRef<GUISystem>()->DestroyWnd(pWnd);
	//ɾ������
	m_mapChatShow.erase(dwRoleID);
}

void ChatShowPop::Destroy()
{
	if(m_mapChatShow.empty())
		return;

	size_t nNum = m_mapChatShow.size();
	for(size_t i=0; i<nNum; i++)
	{
		map<DWORD,float>::iterator it = m_mapChatShow.begin();
		DWORD dwRoleID = it->first;
		DeleteChatShow(dwRoleID);
	}
}

void ChatShowPop::DrawInfo()
{
	if(m_mapChatShow.empty())
		return;

	map<DWORD,float>::iterator it = m_mapChatShow.begin();
	for(; it!=m_mapChatShow.end(); it++)
	{
		float curTime=Kernel::Inst()->GetAccumTime();
		DWORD dwRoleID = it->first;
		if( (*it).second+TIME_SAVE<curTime )
		{
			//��������ʱ�䣬��ɾ�������ID��ʱ�䣬�������ٶ�Ӧ�Ĵ���
			DeleteChatShow(dwRoleID);
			return;	
		}
		else
		{
			Role *pRole = RoleMgr::Inst()->FindRole(dwRoleID, false, true);
			if( !P_VALID(pRole) )
			{
				DeleteChatShow(dwRoleID);
				return;
			}
			Vector3 worldPos;
			pRole->GetHeadPos(worldPos);	
			Vector3 srcPos;
			srcPos = GetWorldFrm()->GetCamera()->WorldToScreenF(worldPos);
			srcPos.z /= 2;

			POINT pt = GetWorldFrm()->GetCamera()->WorldToScreen(worldPos);

			float fDrawTime = IS_PLAYER(dwRoleID)? TIME_DRAW : 5;
			if( (*it).second+fDrawTime>=curTime )
			{
				//С�ڻ���ʱ�䣬����Ҫ����Ļ�ϻ��
				TCHAR szWndName[X_LONG_NAME]={0};
				_stprintf(szWndName, _T("%s%s%ld"), _T("\\desktop\\"), STR_WND_NAME, dwRoleID);
				RichEditWithZ* pWnd = NULL;
				pWnd = (RichEditWithZ*)TObjRef<GUISystem>()->GetWnd(szWndName);

				if( P_VALID(pWnd) )
					pWnd->DrawInfo(pt, srcPos.z);
			}
		}
	}
}	