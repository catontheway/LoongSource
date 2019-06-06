//-----------------------------------------------------------------------------
//!\file	client_mgr.cpp
//!\brief	�ͻ��˹�����
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/msg_rt_c.h"
#include "../ServerDefine/msg_rt_s.h"
#include "../ServerDefine/msg_gm_tool_c.h"
#include "../ServerDefine/msg_show_sum.h"
#include "client.h"
#include "client_db.h"
#include "client_mgr.h"
#include "server.h"
#include "section_mgr.h"
#include "game_world.h"
#include "server_db.h"
#include "section.h"
#include "game_server.h"
#include "../WorldDefine/time.h"

//-----------------------------------------------------------------------------
// ȫ�ֶ���
//-----------------------------------------------------------------------------
ClientMgr g_ClientMgr;
TimeLimitMsgMgr g_TimeLimitMsgMgr;

//===========================================�ޕr��Ϣ��������ʵ��=============================================//

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
TimeLimitMsgMgr::TimeLimitMsgMgr()
{
	m_dwUpdateTicks		= 1000/TICK_TIME;
	m_nTickCountDown	= 1000/TICK_TIME;
	//m_nCountForAutoNotice = 0;
	//m_dwLastCalTime		= timeGetTime();
}


//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
TimeLimitMsgMgr::~TimeLimitMsgMgr()
{
	if(m_LstMsgTimeDeadLine.Size() > 0)
	{
		tagMsgTimeDeadLine *pItem = NULL;
		TList<tagMsgTimeDeadLine*>::TListIterator iter = m_LstMsgTimeDeadLine.Begin();
		while(m_LstMsgTimeDeadLine.PeekNext(iter, pItem))
		{
			// ɾ���ýڵ�
			m_LstMsgTimeDeadLine.Erase(pItem);
			//SAFE_DEL(pItem->pMsg);
			free(pItem);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ����Ϣʱ����������һ���µ���Ϣ��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID TimeLimitMsgMgr::AddMsg2LMTDL(DWORD dwMsgCrc, DWORD dwOpenTime, BOOL bType, tagNetCmd* pMsg, DWORD dwClientID)
{
 	tagMsgTimeDeadLine* pNewItem = new tagMsgTimeDeadLine; // ��UpdateTimeLeftList�������ͷ�
	
	pNewItem->dwMsgCrc = dwMsgCrc;
	pNewItem->dwDeadLine = dwOpenTime;
	pNewItem->bType = bType;
	pNewItem->pMsg = pMsg; 
	pNewItem->dwClientID = dwClientID;

	m_LstMsgTimeDeadLine.PushBack(pNewItem);
}

//-------------------------------------------------------------------------------------------------------
// ��ռ�ʱ��Ϣlist
//-------------------------------------------------------------------------------------------------------
VOID TimeLimitMsgMgr::ClearMsgList()
{
	if(m_LstMsgTimeDeadLine.Size() > 0)
	{
		tagMsgTimeDeadLine *pItem = NULL;
		TList<tagMsgTimeDeadLine*>::TListIterator iter = m_LstMsgTimeDeadLine.Begin();
		while(m_LstMsgTimeDeadLine.PeekNext(iter, pItem))
		{
			// ɾ���ýڵ�
			m_LstMsgTimeDeadLine.Erase(pItem);
			//SAFE_DEL(pItem->pMsg);
			free(pItem->pMsg);
			SAFE_DEL(pItem);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// update
//-------------------------------------------------------------------------------------------------------
VOID TimeLimitMsgMgr::Update()
{
	--m_nTickCountDown; // ÿ������m_nTickCountDown��tick��ÿ���Ӹ���һ��TimeLimitMsgMgr

	if(0 == m_nTickCountDown)
	{
		if(m_LstMsgTimeDeadLine.Size() > 0)
		{
			UpdateTimeLeftList();
		}

		m_nTickCountDown = m_dwUpdateTicks;
	}
}

//-------------------------------------------------------------------------------------------------------
// ʱ����Ϣ��ʱ����
//-------------------------------------------------------------------------------------------------------
VOID TimeLimitMsgMgr::UpdateTimeLeftList()
{
	tagMsgTimeDeadLine *pItem = NULL;
	TList<tagMsgTimeDeadLine*>::TListIterator iter = m_LstMsgTimeDeadLine.Begin();

	while(m_LstMsgTimeDeadLine.PeekNext(iter, pItem))
	{
		// ��ȡ���ڵ�ʱ��
		tagDWORDTime dwNow;
		dwNow = GetCurrentDWORDTime();

		if( pItem->bType == TRUE && pItem->dwDeadLine <= dwNow) // ��ʱ������Ϣ�����͸���Ϣ��ʱ���ѵ�
		{
			// ���͸���Ϣ
			SendTimeLimitMsg(pItem->dwMsgCrc, pItem->pMsg, pItem->dwClientID);

			// ����Ϣʱ��������ɾ���ýڵ�
			m_LstMsgTimeDeadLine.Erase(pItem);

			if((pItem->dwMsgCrc == IUTIL->Crc32("NGMTC_AutoNotice")) && (((tagNGMTC_AutoNotice*)(pItem->pMsg))->nCirNum > 1)) 
			{
				((tagNGMTC_AutoNotice*)(pItem->pMsg))->nCirNum--;
				SAFE_DEL(pItem);
			}
			else
			{
				//SAFE_DEL(pItem);
				free(pItem->pMsg);
				SAFE_DEL(pItem);
			}
		
			continue;
		}

		if (pItem->bType == FALSE) // ÿ�հ��㷢����Ϣ
		{
			tagDWORDTime dwNow;
			dwNow = GetCurrentDWORDTime();

			tagDWORDTime dwThisTime = (tagDWORDTime)pItem->dwDeadLine;
			
			if ( dwThisTime.hour == dwNow.hour && dwThisTime.min == dwNow.min && dwNow.sec <= 1/2 ) // ����ʱ���ѵ�
			{
				// ���͸���Ϣ
				SendTimeLimitMsg(pItem->dwMsgCrc, pItem->pMsg, pItem->dwClientID);

				continue;
			}

		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ���Ͷ�ʱ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD TimeLimitMsgMgr::SendTimeLimitMsg(DWORD dwMsgCrc, tagNetCmd* pMsg, DWORD dwClientID)
{
	if ( dwMsgCrc == IUTIL->Crc32("NGMTC_Double") )
		SendDoubleMsg(pMsg, dwClientID);
	else if ( dwMsgCrc == IUTIL->Crc32("NGMTC_RightNotice") )
		SendRightNotice(pMsg, dwClientID);
	else if( dwMsgCrc == IUTIL->Crc32("NGMTC_AutoNotice") )
		SendAutoNotice(pMsg, dwClientID);
	else if ( dwMsgCrc == IUTIL->Crc32("NGMTC_AutoChatNotice"))
		SendAutoChatNotice(pMsg, dwClientID);
	else
		return FALSE;

	//SAFE_DEL(pMsg);
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���Ͷ�ʱ˫��������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD TimeLimitMsgMgr::SendDoubleMsg(tagNetCmd* pMsg, DWORD dwClientID)
{
	tagNGMTC_Double* pToSend = (tagNGMTC_Double*)pMsg; 
	
	// �����ϢҪ�����Ĵ�����ID
	Section* pSection = g_SectionMgr.GetSection(pToSend->dwSectionID);
	if(!P_VALID(pSection))
		return FALSE;

	tagNSS_Double send;
	send.dwClientID = dwClientID;
	send.dwOpenTime = pToSend->dwOpenTime;
	send.dwRatio = pToSend->dwRatio;
	send.dwLastTime = pToSend->dwLastTime;
	send.eDType = pToSend->eDType;
	
	// ���������Ϣ��world�ĸ���
	INT nNum = (pToSend->dwSize - (sizeof(tagNGMTC_Double) - sizeof(DWORD))) / sizeof(DWORD);

	// ������Ŀ��world������Ϣ
	pSection->ForAllGameWorld(pToSend->dwWorldID, nNum, (FUN_GAMEWORLD)&(GameWorld::SendWorldMsg), &send);

	return TRUE;

}

//-------------------------------------------------------------------------------------------------------
// ���Ͷ�ʱ������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD TimeLimitMsgMgr::SendRightNotice(tagNetCmd* pMsg, DWORD dwClientID)
{
	tagNGMTC_RightNotice* pToSend = (tagNGMTC_RightNotice*)pMsg; 

	// �����ϢҪ�����Ĵ�����ID
	Section* pSection = g_SectionMgr.GetSection(pToSend->dwSectionID);
	if(!P_VALID(pSection))
		return FALSE;

	tagNSS_RightNotice send;
	send.dwClientID = dwClientID;
	memcpy(send.szContent, pToSend->szContent, sizeof(pToSend->szContent));
	memcpy(send.szLink, pToSend->szLink, sizeof(pToSend->szLink));
	memcpy(send.szTitle, pToSend->szTitle, sizeof(pToSend->szTitle));

	INT nNum = (pToSend->dwSize - (sizeof(tagNGMTC_RightNotice) - sizeof(DWORD))) / sizeof(DWORD);

	pSection->ForAllGameWorld(pToSend->dwWorldID, nNum, (FUN_GAMEWORLD)&(GameWorld::SendWorldMsg), &send);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// �����Զ�ѭ��������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD TimeLimitMsgMgr::SendAutoNotice(tagNetCmd* pMsg, DWORD dwClientID)
{
	tagNGMTC_AutoNotice* pToSend = (tagNGMTC_AutoNotice*)pMsg; 

	// �����ϢҪ�����Ĵ�����ID
	Section* pSection = g_SectionMgr.GetSection(pToSend->dwSectionID);
	if(!P_VALID(pSection))
		return FALSE;

	tagNSS_AutoNotice send;
	send.dwClientID = dwClientID;
	send.eType = pToSend->eType;
	memcpy(send.szContent, pToSend->szContent, sizeof(pToSend->szContent));
	send.nCirInterval = pToSend->nCirInterval;

	INT nNum = (pToSend->dwSize - (sizeof(tagNGMTC_AutoNotice) - sizeof(DWORD))) / sizeof(DWORD);

	pSection->ForAllGameWorld(pToSend->dwWorldID, nNum, (FUN_GAMEWORLD)&(GameWorld::SendWorldMsg), &send);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��������Ƶ��������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD TimeLimitMsgMgr::SendAutoChatNotice(tagNetCmd* pMsg, DWORD dwClientID)
{
	tagNGMTC_AutoChatNotice* pToSend = (tagNGMTC_AutoChatNotice*)pMsg; 

	// �����ϢҪ�����Ĵ�����ID
	Section* pSection = g_SectionMgr.GetSection(pToSend->dwSectionID);
	if(!P_VALID(pSection))
		return FALSE;

	tagNSS_AutoChatNotice send;
	send.dwClientID = dwClientID;
	memcpy(send.szNotice, pToSend->szNotice, sizeof(pToSend->szNotice));

	INT nNum = (pToSend->dwSize - (sizeof(tagNGMTC_AutoChatNotice) - sizeof(DWORD))) / sizeof(DWORD);

	pSection->ForAllGameWorld(pToSend->dwWorldID, nNum, (FUN_GAMEWORLD)&(GameWorld::SendWorldMsg), &send);

	return TRUE;
}

DWORD TimeLimitMsgMgr::CancelTimeLimitedMsg(DWORD dwMsgCrc)
{
	tagMsgTimeDeadLine* pItem = NULL;
	TList<tagMsgTimeDeadLine*>::TListIterator iter = m_LstMsgTimeDeadLine.Begin();

	while(m_LstMsgTimeDeadLine.PeekNext(iter, pItem))
	{
		if (pItem->dwMsgCrc == dwMsgCrc)
		{
			m_LstMsgTimeDeadLine.Erase(pItem);
			free(pItem->pMsg);
			SAFE_DEL(pItem);
		}
		else
			continue;
	}

	return TRUE;
}

//===========================================�ͻ��˹�����ʵ��=============================================//

ClientMgr::ClientMgr():m_Trunk(this), m_nPort(0), m_dwIDGen(0)
{

}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL ClientMgr::Init()
{
	TObjRef<VarContainer> pVar = "VarContainer";

	// ��ʼ�����ݿ�
	LPCTSTR	szHost		=	pVar->GetString(_T("host"),	_T("client_db"));
	LPCTSTR	szUser		=	pVar->GetString(_T("user"),	_T("client_db"));
	LPCTSTR	szPsd		=	pVar->GetString(_T("psd"),	_T("client_db"));
	LPCTSTR	szDBName	=	pVar->GetString(_T("name"),	_T("client_db"));
	INT		nPort		=	pVar->GetInt(_T("port"),	_T("client_db"));

	if( FALSE == m_ClientDB.Init(szHost, szUser, szPsd, szDBName, nPort) ) return FALSE;

	// �����������������
	CreateObj("NetCmdMgrClient", "NetCmdMgr");
	m_pCmdMgr = "NetCmdMgrClient";
	if( !P_VALID(m_pCmdMgr) ) return FALSE;

	// ���ɷ���������ӿ�
	CreateObj("StreamServerClient", "StreamServer");
	m_pStreamServer= "StreamServerClient";
	if( !P_VALID(m_pStreamServer) ) return FALSE;

	// ע����������
	RegisterClientMsg();

	// ��ʼ��Ȩ���б�
	InitPrivilegeInfo();

	// ��ʼ������
	m_nPort = pVar->GetInt(_T("port"), _T("client"));
	
	m_pStreamServer->Init(	(TCPLOGINCALLBACK)m_Trunk.sfp2(&ClientMgr::LoginCallBack),
							(TCPLOGOUTCALLBACK)m_Trunk.sfp1(&ClientMgr::LogoutCallBack),
							m_nPort);

	return TRUE;
}

//---------------------------------------------------------------------------
// ע����������
//---------------------------------------------------------------------------
VOID ClientMgr::RegisterClientMsg()
{
	// ��ά����
	m_pCmdMgr->Register("NRTC_Challenge",		(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleChallenge),		_T("NRTC_Challenge"));
	
	m_pCmdMgr->Register("NSSC_Challenge",		(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleShowSumChallenge),		_T("NSSC_Challenge"));
	m_pCmdMgr->Register("NRTC_Close",			(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleCloseWorld),		_T("NRTC_Close"));
	m_pCmdMgr->Register("NRTC_Start",			(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleStartWorld),		_T("NRTC_Start"));
	m_pCmdMgr->Register("NRTC_SystemCmd",		(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleSystemCmd),		_T("NRTC_SystemCmd"));
	m_pCmdMgr->Register("NRTC_MaxNum",			(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleSetMaxNum),		_T("NRTC_MaxNum"));
	m_pCmdMgr->Register("NRTC_UpdateMall",		(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleUpdateMall),		_T("NRTC_UpdateMall"));
	m_pCmdMgr->Register("NRTC_BindIP",			(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleChangeIPBind),	_T("NRTC_BindIP"));
	m_pCmdMgr->Register("NSSC_GetDynInfo",		(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleShowSumReq),		_T("NSSC_GetDynInfo"));

	// GM����
	m_pCmdMgr->Register("NGMTC_GMCreate",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleGMCreate),		_T("NRTC_Start"));
	m_pCmdMgr->Register("NGMTC_GMModify",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleGMModify),		_T("NGMTC_GMModify")  );
	m_pCmdMgr->Register("NGMTC_GMDelete",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleGMDelete),		_T("NGMTC_GMDelete")  );
	m_pCmdMgr->Register("NGMTC_GMQuery",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleGMQuery),		_T("NGMTC_GMQuery")  );
	m_pCmdMgr->Register("NGMTC_RoleResume",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleResumeRole),		_T("NGMTC_RoleResume"));
	m_pCmdMgr->Register("NGMTC_ProModify",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleModifyRole),		_T("NGMTC_ProModify") );
	m_pCmdMgr->Register("NGMTC_Double",		(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleDouble),			_T("NGMTC_Double")    );
	m_pCmdMgr->Register("NGMTC_CancelDoubleMsg",(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleCancelDouble),	_T("NGMTC_CancelDouble"));
	m_pCmdMgr->Register("NGMTC_RightNotice",(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleRightNotice),	_T("NGMTC_Notice")    );
	m_pCmdMgr->Register("NGMTC_CancelRightNotice",(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleCancelRightNotice),	_T("NGMTC_CancelRightNotice"));
	m_pCmdMgr->Register("NGMTC_AutoNotice",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleAutoNotice),		_T("NGMTC_AutoNotice"));

	m_pCmdMgr->Register("NGMTC_ItemCreate",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleCreateItem),		_T("NGMTC_ItemCreate"));
	m_pCmdMgr->Register("NGMTC_ItemDelete",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleDeleteItem),		_T("NGMTC_ItemDelete"));
	m_pCmdMgr->Register("NGMTC_ClearBaibao",(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleClearBaibao),	_T("NGMTC_ClearBaibao"));
	m_pCmdMgr->Register("NGMTC_Blacklist",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleBlacklist),		_T("NGMTC_Blacklist"));
	m_pCmdMgr->Register("NGMTC_ItemResume",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleItemResume),		_T("NGMTC_ItemResume"));
	m_pCmdMgr->Register("NGMTC_AccountForbid",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleAccountForbid),		_T("NGMTC_AccountForbid"));

	m_pCmdMgr->Register("NGMTC_AutoChatNotice",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleAutoChatNotice),		_T("NGMTC_AutoChatNotice"));
	m_pCmdMgr->Register("NGMTC_CancelAutoChatNotice",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleCancelAutoChatNotice),		_T("NGMTC_CancelAutoChatNotice"));
	m_pCmdMgr->Register("NGMTS_GMSetFackNumer",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleModifyFackNum),		_T("NGMTC_OnlineNumDisplay"));
	//m_pCmdMgr->Register("NGMTC_CancelTimeLimitedMsg",	(NETMSGPROC)m_Trunk.sfp2(&ClientMgr::HandleCancelTimeLimitedMsg),		_T("NGMTC_CancelTimeLimitedMsg"));
}

//---------------------------------------------------------------------------
// ��ע����������
//---------------------------------------------------------------------------
VOID ClientMgr::UnRegisterClientMsg()
{
	m_pCmdMgr->Destroy();
}

//---------------------------------------------------------------------------
// ��ʼ��Ȩ�ޱ�
//---------------------------------------------------------------------------
BOOL ClientMgr::InitPrivilegeInfo()
{
	m_mapPrivilege.Clear();
	return m_ClientDB.DBQueryPrivilegeInfo(m_mapPrivilege);
}

//---------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------
void ClientMgr::Destroy()
{
	// ɾ�����е����߿ͻ���
	Client* pClient = NULL;
	TMap<DWORD, Client*>::TMapIterator it_1 = m_mapClient.Begin();

	while( m_mapClient.PeekNext(it_1, pClient) )
	{
		SAFE_DEL(pClient);
	}
	m_mapClient.Clear();

	// ע������
	m_pStreamServer->Destroy();
	KillObj("StreamServerClient");

	// ע�����е���������
	UnRegisterClientMsg();
	KillObj("NetCmdMgrClient");

	// ���ݿ���������ʱ����Զ�ɾ��
}

//-----------------------------------------------------------------
// ��ָ֤���û��Ƿ���Ȩ�޴���ָ����Ϣ
//-----------------------------------------------------------------
BOOL ClientMgr::HaveEnoughPrivilege(const tagNetCmd* pCmd, const Client* pClient)
{
	static DWORD dwChallengeMsgID = IUTIL->Crc32("NRTC_Challenge");
	static DWORD dwShowSumChallengeMsgID = IUTIL->Crc32("NSSC_Challenge");
	static DWORD dwShowSumMsgID = IUTIL->Crc32("NSSC_GetDynInfo");

	// �õ��ͻ���Ȩ��
	EPrivilege ePrivilege = pClient->GetPrivilege();

	// �����û����֤ͨ��
	if( EP_Null == ePrivilege )
	{
		if( pCmd->dwID == dwChallengeMsgID || pCmd->dwID == dwShowSumChallengeMsgID || pCmd->dwID == dwShowSumMsgID )
			return TRUE;
		else
			return FALSE;
	}

	// ������ϢIDȡ������Ȩ�ޱ��ж�Ӧ��һ����Ϣ
	DWORD dwPrivilegeFlag = 0;
	dwPrivilegeFlag = m_mapPrivilege.Peek(pCmd->dwID);
	if( !P_VALID(dwPrivilegeFlag) ) return TRUE;	// ���û�ҵ����������Ϣ����ҪȨ���޶�

	// ���Ȩ��
	if( dwPrivilegeFlag & (1 << ePrivilege) ) return TRUE;
	else return FALSE;
}

//-----------------------------------------------------------------
// ����
//-----------------------------------------------------------------
VOID ClientMgr::Update()
{
	// ��ʱ����
	LPBYTE	pRecv		=	NULL;
	DWORD	dwMsgSize	=	0;
	INT		nUnRecved	=	0;

	// ��ѯ���пͻ���
	Client* pClient = NULL;
	TMap<DWORD, Client*>::TMapIterator it = m_mapAllClient.Begin();

	while( m_mapAllClient.PeekNext(it, pClient) )
	{
		ASSERT(P_VALID(pClient));

		pRecv = m_pStreamServer->Recv(pClient->GetID(), dwMsgSize, nUnRecved);

		if( !P_VALID(pRecv) ) continue;

		// ת����Ϣ
		tagNetCmd* pCmd = (tagNetCmd*)pRecv;

		// ���һ��������Ϣ��Ӧ��Ȩ�ޣ����Ȩ�޲���������
		if( HaveEnoughPrivilege(pCmd, pClient) == FALSE )
		{
			m_pStreamServer->FreeRecved(pClient->GetID(), pRecv);
			continue;
		}

		// ����������Ϣ
		m_pCmdMgr->HandleCmd(pCmd, dwMsgSize, (DWORD)pClient);
		m_pStreamServer->FreeRecved(pClient->GetID(), pRecv);
	}
}

//-----------------------------------------------------------------
// �ͻ��˵�½�ص�  
//-----------------------------------------------------------------
DWORD ClientMgr::LoginCallBack(LPVOID p1, LPVOID p2)
{
	static DWORD dwLoginCmdCrc = IUTIL->Crc32("NRTC_Challenge");	// ������Ϣ��CRCֵ
	static DWORD dwLoginShowSumCmdCrc = IUTIL->Crc32("NSSC_Challenge");	// ������Ϣ��CRCֵ

	tagNetCmd* pCmd = (tagNetCmd*) p1;

	// ����ǲ���login��Ϣ
	if( (pCmd->dwID != dwLoginCmdCrc) && (pCmd->dwID != dwLoginShowSumCmdCrc) ) return GT_INVALID;

	/*if (pCmd->dwID == dwLoginCmdCrc)
	{
		tagNRTC_Challenge* pCmd = (tagNRTC_Challenge*)p1;
	}
	else if (pCmd->dwID == dwLoginShowSumCmdCrc)
	{
		tagNSSC_Challenge* pCmd = (tagNSSC_Challenge*)p1;
	}
	else
		return GT_INVALID;*/

	// �����ǰ�Ѿ��رգ��Ͳ���Ӧ��
	if( g_Server.IsServerShutDown() ) return GT_INVALID;

	// ��ס
	g_Server.Lock();

	// ����һ���µ�ID
	InterlockedExchangeAdd((LPLONG)&m_dwIDGen, 1);
	Client* pClient = new Client(m_dwIDGen);
	m_mapAllClient.Add(pClient->GetID(), pClient);

	g_Server.UnLock();

	return pClient->GetID();
}

//-----------------------------------------------------------------
//    
//-----------------------------------------------------------------
DWORD ClientMgr::LogoutCallBack(LPVOID p1)
{
	// �õ�ID
	DWORD dwID = (DWORD)p1;

	g_Server.Lock();

	// �ҵ��ͻ���
	Client* pClient = m_mapAllClient.Peek(dwID);
	if( P_VALID(pClient) )
	{
		m_mapAllClient.Erase(dwID);

		// ����Ѿ��ɹ���½����Ҫ���ѵ�¼�б���ɾ��
		DWORD dwAccountID = pClient->GetAccountID();

		if( P_VALID(dwAccountID) )
		{
			m_mapClient.Erase(dwAccountID);
		}
	}

	// ɾ���ڴ�
	SAFE_DEL(pClient);

	g_Server.UnLock();

	return 0;
}

//------------------------------------------------------------------
// �����������ߵĿͻ�����Ϣ
//------------------------------------------------------------------
VOID ClientMgr::SendAllClientMsg(LPBYTE pMsg, DWORD dwSize)
{
	Client* pClient = NULL;
	TMap<DWORD, Client*>::TMapIterator it = m_mapClient.Begin();

	while( m_mapClient.PeekNext(it, pClient) )
	{
		pClient->SendMsg(pMsg, dwSize);
	}
}

//-----------------------------------------------------------------
// ����֤ͨ�����û�������Ȩ����Ϣ
//-----------------------------------------------------------------
VOID ClientMgr::SendPrivilegeInfo(Client* pClient)
{
	INT nNum = m_mapPrivilege.Size();	// Ȩ�޶�����
	if( nNum <= 0 ) return;

	// ������Ϣ
	DWORD dwSize = sizeof(tagNRTS_PrivilegeInfo) - sizeof(DWORD) + sizeof(DWORD) * 2 * nNum;
	MCREATE_MSG(pSend, dwSize, NRTS_PrivilegeInfo);

	pSend->nNum = nNum;

	// ��֯��Ϣ
	DWORD dwMsgID = GT_INVALID;
	DWORD dwPrivilegeFlag = GT_INVALID;
	INT nTemp = 0;
	TMap<DWORD, DWORD>::TMapIterator it = m_mapPrivilege.Begin();
	while( m_mapPrivilege.PeekNext(it, dwMsgID, dwPrivilegeFlag) )
	{
		pSend->dwInfo[nTemp++] = dwMsgID;
		pSend->dwInfo[nTemp++] = dwPrivilegeFlag;
	}

	// ����
	SendMsg(pClient->GetID(), pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-----------------------------------------------------------------
// ��֤��Ϣ
//-----------------------------------------------------------------
DWORD ClientMgr::HandleChallenge(tagNetCmd* pMsg, Client* pClient)
{
	tagNRTC_Challenge* pRecv = (tagNRTC_Challenge*)pMsg;

	// ����Ѿ���½���򲻽��ܸ���Ϣ
	if( pClient->GetAccountID() != GT_INVALID )
	{
		return GT_INVALID;
	}

	// �������ݿ�
	DWORD dwAccountID = GT_INVALID;
	INT nPrivilege = GT_INVALID;

	BOOL bRet = m_ClientDB.DBQueryClientData(pRecv->szAccountName, pRecv->dwPsdCrc, dwAccountID, nPrivilege);
	if (!MIS_RTCLIENT_PRI(nPrivilege) && !MIS_GMTOOL_PRI(nPrivilege))
	{
		tagNRTS_Challenge send;
		send.dwErrorCode = E_GMTool_Err;
		send.ePrivilege = EP_Null;
		pClient->SendMsg(&send, send.dwSize);
		return GT_INVALID;
	}

	if (bRet && MIS_RTCLIENT_PRI(nPrivilege))
	{
		if (!m_ClientDB.DBCheckIPBind(pRecv->szAccountName, GetClientIP(pClient->GetID())))
		{
			tagNRTS_Challenge send;
			send.dwErrorCode = E_RTClient_Ip_Err;
			send.ePrivilege = EP_Null;
			pClient->SendMsg(&send, send.dwSize);
			return GT_INVALID;
		}
	}

	// �����֤ͨ������ʼ�����˺ţ����뵽����֤�б���
	if( bRet )
	{
		pClient->Init(dwAccountID, nPrivilege, pRecv->szAccountName, GetClientIP(pClient->GetID()));
		m_mapClient.Add(pClient->GetAccountID(), pClient);
	}

	// ���ͷ�����Ϣ
	tagNRTS_Challenge send;
	send.dwErrorCode = (bRet ? E_GMTool_Success : E_GMTool_Err);
	send.ePrivilege = pClient->GetPrivilege();
	pClient->SendMsg(&send, send.dwSize);

	// ��������server�ľ�̬��Ϣ���ͻ���
	if( bRet )
	{
		// ����û��������з������ľ�̬��Ϣ
		g_SectionMgr.SendAllServerStaticInfoToClient(pClient);
		// ����û�������Ȩ����Ϣ
		g_ClientMgr.SendPrivilegeInfo(pClient);
	}

	return 0;
}

//-----------------------------------------------------------------
// ��֤��Ϣ
//-----------------------------------------------------------------
DWORD ClientMgr::HandleShowSumChallenge(tagNetCmd* pMsg, Client* pClient)
{
	tagNSSC_Challenge* pRecv = (tagNSSC_Challenge*)pMsg;

	// ����Ѿ���½���򲻽��ܸ���Ϣ
	if( pClient->GetAccountID() != GT_INVALID )
	{
		return GT_INVALID;
	}

	pClient->Init(GT_INVALID, EP_Null, _T(" "), GetClientIP(pClient->GetID()));
	m_mapClient.Add(pClient->GetAccountID(), pClient);

	return 0;
}

//-----------------------------------------------------------------
// �ر���Ϸ�������������Ϣ��������ά�ͻ��ˣ�RtClient����������ά��������RTService_1��
//-----------------------------------------------------------------
DWORD ClientMgr::HandleCloseWorld(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from rtclient...Close World.\n"));

	tagNRTC_Close *pRecv = (tagNRTC_Close*)pMsg;
	
	BOOL bRet = g_SectionMgr.CloseServer(pRecv->eType, pRecv->dwSectionID, pRecv->dwWorldID, pRecv->bForce);

	//��¼LOG
	if (bRet)
	{
		//����Log��ϸ����
		Section* pSection = g_SectionMgr.GetSection(pRecv->dwSectionID);
		if (!P_VALID(pSection))
		{
			return GT_INVALID;
		}
		tstring strDetails = pSection->GetSectionName();

		GameWorld* pWorld = NULL;
		if (pRecv->eType != EST_Login)
		{
			pWorld = pSection->GetWorld(pRecv->dwWorldID);
			if (!P_VALID(pWorld))
			{
				return GT_INVALID;
			}
			strDetails += _T("-");
			strDetails += pWorld->GetWorldName();
		}


		switch (pRecv->eType)
		{
		case EST_Login:
			strDetails += _T("-LoginServer");
			break;

		case EST_DB:
			strDetails += _T("-DBServer");
			break;

		case EST_World:
			strDetails += _T("-WorldServer");
			break;
		}

		if (pRecv->bForce)
		{
			LogGMAction(pClient, EAT_KillServer, strDetails.c_str());
		}
		else
		{
			LogGMAction(pClient, EAT_CloseServer, strDetails.c_str());
		}
	}
	
	return 0;
}

//-----------------------------------------------------------------
// ������Ϸ�������������Ϣ��������ά�ͻ��ˣ�RtClient����������ά��������RTService_1��
//-----------------------------------------------------------------
DWORD ClientMgr::HandleStartWorld(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from rtclient...Start World.\n"));
	tagNRTC_Start *pRecv = (tagNRTC_Start*)pMsg;

	BOOL bRet = g_SectionMgr.StartServer(pRecv->eType, pRecv->dwSectionID, pRecv->dwWorldID);

	//��¼LOG
	if (bRet)
	{
		//����Log��ϸ����
		Section* pSection = g_SectionMgr.GetSection(pRecv->dwSectionID);
		if (!P_VALID(pSection))
		{
			return GT_INVALID;
		}
		tstring strDetails = pSection->GetSectionName();

		GameWorld* pWorld = NULL;
		if (pRecv->eType != EST_Login)
		{
			pWorld = pSection->GetWorld(pRecv->dwWorldID);
			if (!P_VALID(pWorld))
			{
				return GT_INVALID;
			}
			strDetails += _T("-");
			strDetails += pWorld->GetWorldName();
		}

		
		switch (pRecv->eType)
		{
		case EST_Login:
			strDetails += _T("-LoginServer");
			break;

		case EST_DB:
			strDetails += _T("-DBServer");
			break;

		case EST_World:
			strDetails += _T("-WorldServer");
			break;
		}
		
		LogGMAction(pClient, EAT_StartServer, strDetails.c_str());
	}

	return 0;
}

//-----------------------------------------------------------------
// ��ָ���ķ����������ⲿ���������ά�ͻ��ˣ�RtClient����������ά��������RTService_1��
//-----------------------------------------------------------------
DWORD ClientMgr::HandleSystemCmd(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from rtclient...Send System Cmd.\n"));

	tagNRTC_SystemCmd *pRecv = (tagNRTC_SystemCmd*)pMsg;

	Section* pSection = g_SectionMgr.GetSection(pRecv->dwSectionID);
	if( !P_VALID(pSection) ) return FALSE;

	BOOL bSuccess = FALSE;

	tagNLSS_SystemCmd send;
	send.dwClientID = pClient->GetID();
	wcscpy(send.szCmd, pRecv->szCmd);

	switch (pRecv->eType)
	{
	case EST_Login:
		bSuccess = pSection->SendSystemCmd2LoginServer(&send);
		break;

	case EST_DB:
		bSuccess = pSection->SendSystemCmd2World(pRecv->dwWorldID, EST_DB, &send);
		break;

	case EST_World:
		bSuccess = pSection->SendSystemCmd2World(pRecv->dwWorldID, EST_World, &send);
		break;
	}

	TCHAR szTempBuff[X_LONG_STRING] = {0};
	_stprintf(szTempBuff, _T("Have Sent System Command [%s] to Server(%d, %d)."), send.szCmd, pRecv->dwSectionID, pRecv->dwWorldID);
	LogGMAction(pClient, EAT_SendSystemCmd, szTempBuff);

	tagNRTS_SystemCmd ret;
	ret.dwErrorCode = bSuccess;
	SendMsg(pClient->GetID(), &ret, ret.dwSize);

	return bSuccess;
}

//-----------------------------------------------------------------
// �޸������������
//-----------------------------------------------------------------
DWORD ClientMgr::HandleSetMaxNum(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from rtclient...Change Max online.\n"));

	tagNRTC_MaxNum *pRecv = (tagNRTC_MaxNum*)pMsg;

	Section* pSection = g_SectionMgr.GetSection(pRecv->dwSectionID);
	if( !P_VALID(pSection) ) return FALSE;

	BOOL bSuccess = FALSE;

	if (pRecv->eType != EST_World)
		return bSuccess;

	tagNSS_MaxNum send;
	send.dwClientID = pClient->GetID();
	send.nMaxNum = pRecv->nMaxNum;
	bSuccess = pSection->SetMaxNum(pRecv->dwWorldID, &send);

	tagNRTS_MaxNum ret;
	ret.dwErrorCode = bSuccess;
	SendMsg(pClient->GetID(), &ret, ret.dwSize);

	return bSuccess;
}

//-----------------------------------------------------------------
// �����̳�
//-----------------------------------------------------------------
DWORD ClientMgr::HandleUpdateMall(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from rtclient...Update Mall.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNRTC_UpdateMall);

	BOOL bSuccess = FALSE;

	Section* pSection = g_SectionMgr.GetSection(pRecv->dwSectionID);
	if( !P_VALID(pSection) ) return bSuccess;

	if(pRecv->eType != EST_World)
		return bSuccess;

	tagNSS_UpdateMall send;
	send.dwClientID = pClient->GetID();
	bSuccess = pSection->UpdateMall(pRecv->dwWorldID, &send);

	tagNRTS_UpdateMall ret;
	ret.dwErrorCode = bSuccess;
	SendMsg(pClient->GetID(), &ret, ret.dwSize);

	return bSuccess;
}

//-----------------------------------------------------------------
// �˺ŷ�ͣ
//-----------------------------------------------------------------
DWORD ClientMgr::HandleAccountForbid(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...Account Forbid.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_AccountForbid);

	BOOL bSuccess = FALSE;

	Section* pSection = g_SectionMgr.GetSection((LPCTSTR)pRecv->szSectionName);
	if (!P_VALID(pSection)) return bSuccess;

	INT nLength = sizeof(tagNSS_ForbidAccount) + _tcslen(pRecv->szAccountStr)*sizeof(TCHAR);
	tagNSS_ForbidAccount* pSend = (tagNSS_ForbidAccount*)malloc(nLength);
	pSend->dwID = IUTIL->Crc32("NSS_ForbidAccount");
	pSend->dwSize = nLength;
	pSend->dwClientID = pClient->GetID();
	//_tcscpy_s(pSend->szWorld, X_SHORT_NAME, pRecv->szWorldName);
	memcpy(pSend->szWorld, pRecv->szWorldName, X_SHORT_NAME);
	pSend->bForbid = pRecv->nAction;
	_tcscpy_s(pSend->szAccount, _tcslen(pRecv->szAccountStr)+1, pRecv->szAccountStr);
	pSend->szAccount[_tcslen(pRecv->szAccountStr)] = _T('\0');

	bSuccess = pSection->AccountForbid(pSend, pSend->dwSize);
	if(P_VALID(pSend)) free(pSend);

	return bSuccess;
}


//-----------------------------------------------------------------
// GM����
//-----------------------------------------------------------------
DWORD ClientMgr::HandleGMCreate(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...Create GM account.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_GMCreate);
	
	DWORD dwErr = E_GMTool_Success;

	// ���Ѵ���
	if (m_ClientDB.DBGMExist(pRecv->szName))
	{
		dwErr = E_GMTool_GM_AlreadyExist;
	}
	// ����GM
	else
	{
		if (MIS_GMOPGM(pClient->GetPrivilege(),	pRecv->ePrivilege)	&& pRecv->ePrivilege != EP_Pri_GM_S ||
			MIS_RTOPRT(pClient->GetPrivilege(),	pRecv->ePrivilege)	
			)
		{
			BOOL bRtv = m_ClientDB.DBCreateGameMaster(pRecv->szName, pRecv->dwPswCrc, pRecv->ePrivilege);
			if (bRtv)
			{
				dwErr = E_GMTool_Success;
				TCHAR tszBuffer[500] = {0};
				_stprintf(tszBuffer, _T("�����ȼ�Ȩ��Ϊ%d���˺�%s"), pRecv->ePrivilege, pRecv->szName);
				LogGMAction(pClient, EAT_CreateGM, tszBuffer);
			}
			else
			{
				dwErr = E_GMTool_Err;
			}
		}
		else
		{
			dwErr = E_GMTool_Err;
		}
	}

	tagNGMTS_GMCreate send;
	send.dwErrorCode	= dwErr;
	pClient->SendMsg(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------
// GM�޸�
//-----------------------------------------------------------------
DWORD	ClientMgr::HandleGMModify(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...Modify GM Account.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_GMModify);
	
	DWORD dwErr = E_GMTool_Success;

	if (!m_ClientDB.DBGMExist(pRecv->szName))
	{
		dwErr	= E_GMTool_GM_NotExist;
	}
	else
	{
		if (MIS_GMOPGM(pClient->GetPrivilege(),	pRecv->ePrivilege) && pRecv->ePrivilege != EP_Pri_GM_S||
			MIS_RTOPRT(pClient->GetPrivilege(),	pRecv->ePrivilege)
			)
		{
			if (m_ClientDB.DBChangeGMPrivilege(pRecv->szName, pRecv->ePrivilege))
			{
				dwErr = E_GMTool_Success;
				TCHAR tszBuffer[500] = {0};
				_stprintf(tszBuffer, _T("��%s�˺�Ȩ�޸���Ϊ%d"), pRecv->szName, pRecv->ePrivilege);
				LogGMAction(pClient, EAT_ModifyGM, tszBuffer);
			}
			else
			{
				dwErr = E_GMTool_Err;
			}
		}
		else
		{
			dwErr = E_GMTool_Err;
		}
	}

	tagNGMTS_GMModify send;
	send.dwErrorCode	=  dwErr;
	pClient->SendMsg(&send, send.dwSize);
	return 0;
}

//-----------------------------------------------------------------
// GMɾ��
//-----------------------------------------------------------------
DWORD	ClientMgr::HandleGMDelete(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...Delete GM Account.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_GMDelete);

	DWORD dwErr = E_GMTool_Success;
	INT nPrivilege = GT_INVALID;

	if (!m_ClientDB.DBQueryAccountPrivilege(pRecv->szName, nPrivilege))
	{
		dwErr	= E_GMTool_GM_NotExist;
	}
	else if (GT_VALID(nPrivilege))
	{
		if (MIS_GMOPGM(pClient->GetPrivilege(), nPrivilege)||
			MIS_RTOPRT(pClient->GetPrivilege(),	nPrivilege)	
			)
		{
			if (m_ClientDB.DBDeleteGameMaster(pRecv->szName))
			{
				dwErr = E_GMTool_Success;
				TCHAR tszBuffer[500] = {0};
				_stprintf(tszBuffer, _T("ɾ���˺�%s"), pRecv->szName);
				LogGMAction(pClient, EAT_DeleteGM, tszBuffer);
			}
			else
			{
				dwErr = E_GMTool_Err;
			}
		}
		else
		{
			dwErr	= E_GMTool_Err;
		}
	}
	else
	{
		ASSERT(0);
		dwErr	= E_GMTool_Err;
	}

	tagNGMTS_GMDelete send;
	send.dwErrorCode	= dwErr;
	pClient->SendMsg(&send, send.dwSize);


	return 0;
}

//-----------------------------------------------------------------
// 7��GM��ѯ����6��GM�����
//-----------------------------------------------------------------
DWORD ClientMgr::HandleGMQuery(tagNetCmd* pMsg, Client* pClient)
{
	MTRANS_POINTER(pRecv, pMsg, tagNRTC_GMQuery);
	
	WORD dwErr = E_GMTool_Success;
	INT nPrivilege = GT_INVALID;

	if (!m_ClientDB.DBQueryAccountPrivilege(pClient->GetName(), nPrivilege))
		dwErr = E_GMTool_GM_NotExist;
	else
	{
		if(nPrivilege == 7)
		{
			// todo
		}
		else
			dwErr = R_RTClient_NotEnoughPri;
	}

	tagNRTS_GMQuery send;
	send.dwErrorCode	= dwErr;
	pClient->SendMsg(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------
// ����ָ�
//-----------------------------------------------------------------
DWORD	ClientMgr::HandleResumeRole(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...Resume role.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_RoleResume);

	ServerDB* pSeverDB = g_SectionMgr.GetServerDB(pRecv->dwSectionID, pRecv->dwWorldID);
	if (!P_VALID(pSeverDB))		return GT_INVALID;

	DWORD dwErrCode = E_GMTool_Success;

	DWORD dwRoleID = GT_INVALID;
	TCHAR	szRoleName[X_SHORT_NAME] = {0};
	
	if (!pSeverDB->DBQueryNameByRoleID(pRecv->nOldID, szRoleName))
	{
		// �ָ��Ľ�ɫ������
		dwErrCode = E_GMTool_ResumeRole_RoleNotExist;
	}
	
	else if ( _tcslen(pRecv->szNewName) > 0 && pSeverDB->DBQueryRoleIDByName(pRecv->szNewName, dwRoleID))
	{
		// ������Ѵ���
		dwErrCode = E_GMTool_ResumeRole_NameAlreadyExist;
	}
	else if (!pSeverDB->DBResumeRole(pRecv->nOldID, pRecv->szNewName))
	{
		// ���ݿ����
		dwErrCode = E_GMTool_Err;
	}
	else
	{
		TCHAR tszBuffer[500] = {0};
		_stprintf(tszBuffer, _T("%s��ɫ�ָ�Ϊ%s��ɫ"), szRoleName, pRecv->szNewName);
		LogGMAction(pClient, EAT_ResumeRole, tszBuffer);
	}

	tagNGMTS_RoleResume send;
	send.dwErrorCode = dwErrCode;
	pClient->SendMsg(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------
// ��ɫ�����޸�
//-----------------------------------------------------------------
DWORD	ClientMgr::HandleModifyRole(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...Modify Role attribute.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_ProModify);

	ServerDB* pSeverDB = g_SectionMgr.GetServerDB(pRecv->dwSectionID, pRecv->dwWorldID);
	if (!P_VALID(pSeverDB))		return GT_INVALID;

	DWORD dwErrCode = E_GMTool_Success;

	DWORD dwRoleID = GT_INVALID;
	if (!pSeverDB->DBQueryRoleIDByName(pRecv->szName, dwRoleID))
	{
		// �����������
		dwErrCode = E_GMTool_ModifyRole_RoleNotExist;
	}
	else if (!pSeverDB->DBRoleModify(pRecv->szName, pRecv->nLel, pRecv->nSkill, pRecv->nYuanbao, pRecv->n64Exp, pRecv->n64Money))
	{
		// ���ݿ����
		dwErrCode = E_GMTool_Err;
	}
	else
	{
		TCHAR tszBuffer[500] = {0};
		_stprintf(tszBuffer, _T("%s��ɫ���Ը���Ϊ���ȼ�%d������%d�����ʵ�%d����Ǯ%d��Ԫ����%d"), pRecv->szName, pRecv->nLel, pRecv->nSkill, pRecv->nYuanbao, pRecv->n64Exp, pRecv->n64Money);
		LogGMAction(pClient, EAT_ProModify, tszBuffer);
	}

	tagNGMTS_ProModify send;
	send.dwErrorCode = dwErrCode;
	pClient->SendMsg(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------
// ˫��
//-----------------------------------------------------------------
DWORD	ClientMgr::HandleDouble(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...double.\n"));

	tagNGMTC_Double* pTemp = (tagNGMTC_Double *)pMsg;
	INT nNum = (pMsg->dwSize - (sizeof(tagNGMTC_Double) - sizeof(DWORD))) / sizeof(DWORD);

	tagNGMTC_Double *pRecv = (tagNGMTC_Double*)malloc(sizeof(tagNGMTC_Double) + nNum*sizeof(DWORD) - sizeof(DWORD)); // ��UpdateTimeLeftList,~TimeLimitMsgMgr���ͷ�
	pRecv->dwID = IUTIL->Crc32("NGMTC_Double");
	pRecv->dwSize = sizeof(tagNGMTC_Double) + nNum*sizeof(DWORD) - sizeof(DWORD);
	pRecv->dwOpenTime = pTemp->dwOpenTime;
	pRecv->dwRatio = pTemp->dwRatio;
	pRecv->dwSectionID = pTemp->dwSectionID;
	pRecv->dwLastTime = pTemp->dwLastTime;
	pRecv->eDType = pTemp->eDType;
	
	TCHAR szWorldStr[X_LONG_STRING] = {0};
	TCHAR szWorldID[X_SHORT_STRING] = {0};
	for (int i=0;i<nNum;++i)
	{
		pRecv->dwWorldID[i] = pTemp->dwWorldID[i];
		_stprintf(szWorldID, _T("%d"), pTemp->dwWorldID[i]);
		if (i>0) _tcscat_s(szWorldStr, X_LONG_STRING, _T(","));
		_tcscat_s(szWorldStr, X_LONG_STRING, szWorldID);
		ZeroMemory(szWorldID, X_SHORT_STRING);
	}

	// ��ȡ���ڵ�ʱ��
	tagDWORDTime dwNow;
	dwNow = GetCurrentDWORDTime();

	if (pRecv->dwOpenTime <= dwNow) // ���õĿ�ʼʱ���ѹ�
	{
		tagNGMTS_TimeLimitedMsg send;
		send.eType = ETLM_Double;
		send.dwErrorCode = E_GMTool_TimeSetError;
		pClient->SendMsg(&send,send.dwSize);
	
		return TRUE;
	}
	
	// ������Ϣ������ʱ��Ϣ�������е� m_LstMsgTimeDeadLine ������
	g_TimeLimitMsgMgr.AddMsg2LMTDL(IUTIL->Crc32("NGMTC_Double"), pRecv->dwOpenTime, TRUE, pRecv, pClient->GetID());

	// ��log
	TCHAR szLog[X_LONG_STRING] = {0};
	tagDWORDTime dwOpenTime = pTemp->dwOpenTime;
	_stprintf(szLog, _T("200%d-%d-%d %d:%d send double to section-%d(world %s) last %d second"), dwOpenTime.year, dwOpenTime.month, dwOpenTime.day, dwOpenTime.hour, dwOpenTime.min, pTemp->dwSectionID, szWorldStr, pTemp->dwLastTime);
	LogGMAction(pClient, EAT_DoubleStart, szLog);

	// ��ͻ��˷�����Ϣ
	tagNGMTS_TimeLimitedMsg ret;
	ret.eType = ETLM_Double;
	ret.dwErrorCode = 0;
	pClient->SendMsg(&ret, ret.dwSize);

	return TRUE;
}

DWORD ClientMgr::HandleCancelDouble(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...cancel double.\n"));

	if (g_TimeLimitMsgMgr.CancelTimeLimitedMsg(IUTIL->Crc32("NGMTC_Double")))
	{
		// ���δ��������
		Section* pSection = NULL;
		TMap<DWORD, Section*> SectionMap = g_SectionMgr.GetSectionMap();
		TMap<DWORD, Section*>::TMapIterator iter = SectionMap.Begin();
		while (SectionMap.PeekNext(iter, pSection))
		{
			// ���δ���ô����µ�������Ϸ����
			GameWorld* pWorld = NULL;
			TMap<DWORD, GameWorld*> mapWorld = pSection->GetWorldMap();
			TMap<DWORD, GameWorld*>::TMapIterator secondIter = mapWorld.Begin();

			while (mapWorld.PeekNext(secondIter, pWorld))
			{
				if (pWorld->GetWorld()->GetType() == EST_World)
				{
					tagNSS_CancelDouble send;
					send.dwClientID = pClient->GetID();

					g_SectionMgr.SendMsg(pWorld->GetWorld()->GetID(), &send, send.dwSize);
				}
			}
		}
	}
	else
	{
		tagNGMTS_CancelDoubleMsg ret;
		ret.dwErrorCode = E_GMTool_Err;
		pClient->SendMsg(&ret, ret.dwSize);
		return FALSE;
	}

	// ��log
	LogGMAction(pClient, EAT_DoubleEnd, _T("cancel double"));

	return TRUE;
}

//-----------------------------------------------------------------
// ����
//-----------------------------------------------------------------
DWORD	ClientMgr::HandleRightNotice(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...right notice.\n"));

	tagNGMTC_RightNotice* pTemp = (tagNGMTC_RightNotice *)pMsg;
	
	INT nNum = (pMsg->dwSize - (sizeof(tagNGMTC_RightNotice) - sizeof(DWORD))) / sizeof(DWORD);

	tagNGMTC_RightNotice *pRecv = (tagNGMTC_RightNotice *)malloc(sizeof(tagNGMTC_RightNotice)+nNum*sizeof(DWORD)-sizeof(DWORD)) ; // ��UpdateTimeLeftList,~TimeLimitMsgMgr���ͷ�	
	pRecv->dwID = IUTIL->Crc32("NGMTC_RightNotice");
	pRecv->dwSize = sizeof(tagNGMTC_RightNotice) + nNum*sizeof(DWORD) - sizeof(DWORD);
	pRecv->dwNoticeTime = pTemp->dwNoticeTime;
	pRecv->dwSectionID = pTemp->dwSectionID;
	_tcscpy_s(pRecv->szContent, Notice_Content, pTemp->szContent);
	_tcscpy_s(pRecv->szLink, Notice_Link, pTemp->szLink);
	_tcscpy_s(pRecv->szTitle, Notice_Title, pTemp->szTitle);
	TCHAR szWorldStr[X_LONG_STRING] = {0};
	TCHAR szWorldID[X_SHORT_STRING] = {0};
	for (int i=0;i<nNum;++i)
	{
		pRecv->dwWorldID[i] = pTemp->dwWorldID[i];
		_stprintf(szWorldID, _T("%d"), pTemp->dwWorldID[i]);
		if (i>0) _tcscat_s(szWorldStr, X_LONG_STRING, _T(","));
		_tcscat_s(szWorldStr, X_LONG_STRING, szWorldID);
		ZeroMemory(szWorldID, X_SHORT_STRING);
	}
	
	if (pRecv->dwNoticeTime == 0) // ��ʱ����
	{
		// �ҵ�����
		Section* pSection = g_SectionMgr.GetSection(pRecv->dwSectionID);
		if (!P_VALID(pSection)) return GT_INVALID;

		tagNSS_RightNotice send;
		send.dwClientID = pClient->GetID();
		memcpy(send.szContent, pRecv->szContent, sizeof(send.szContent));
		memcpy(send.szLink, pRecv->szLink, sizeof(send.szLink));
		memcpy(send.szTitle, pRecv->szTitle, sizeof(send.szTitle));

		// �����������Ϸ����ĸ���
		//INT nNum = (pRecv->dwSize - (sizeof(tagNGMTC_RightNotice) - sizeof(DWORD))) / sizeof(DWORD);
		
		// ��ʼ���͹���
		pSection->ForAllGameWorld(pRecv->dwWorldID, nNum, (FUN_GAMEWORLD)&(GameWorld::SendWorldMsg), &send);

		// ��log
		TCHAR szLog[X_LONG_STRING] = {0};
		_stprintf(szLog, _T("send right-notice(\"%s\") to section-%d(world %s) currently"), pTemp->szContent, pTemp->dwSectionID, szWorldStr);
		LogGMAction(pClient, EAT_NoticeStart, szLog);

		// ��ͻ��˷�����Ϣ
		tagNGMTS_TimeLimitedMsg ret;
		ret.eType = ETLM_RightNotice;
		ret.dwErrorCode = 0;
		pClient->SendMsg(&ret, ret.dwSize);
		
		return TRUE;
	}
	else if(pRecv->dwNoticeTime <= GetCurrentDWORDTime()) // �������õĿ�ʼʱ���ѹ�
	{
		tagNGMTS_TimeLimitedMsg send;
		send.eType = ETLM_RightNotice;
		send.dwErrorCode = E_GMTool_TimeSetError;
		pClient->SendMsg(&send,send.dwSize);

		return TRUE;
	}

	// ������Ϣ������ʱ��Ϣ�������е� m_LstMsgTimeDeadLine ������
	g_TimeLimitMsgMgr.AddMsg2LMTDL(IUTIL->Crc32("NGMTC_RightNotice"), pRecv->dwNoticeTime, TRUE, pRecv, pClient->GetID());

	// ��ͻ��˷�����Ϣ
	tagNGMTS_TimeLimitedMsg ret;
	ret.eType = ETLM_RightNotice;
	ret.dwErrorCode = 0;
	pClient->SendMsg(&ret, ret.dwSize);

	// ��log
	TCHAR szLog[X_LONG_STRING] = {0};
	tagDWORDTime dwOpenTime = pTemp->dwNoticeTime;
	_stprintf(szLog, _T("200%d-%d-%d %d:%d send right_notice(\"%s\") to section-%d(world %s)"), dwOpenTime.year, dwOpenTime.month, dwOpenTime.day, dwOpenTime.hour, dwOpenTime.min, pTemp->szContent, pTemp->dwSectionID, szWorldStr);
	LogGMAction(pClient, EAT_NoticeStart, szLog);

	return TRUE;
}


DWORD ClientMgr::HandleCancelRightNotice(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...cancel right notice.\n"));

	if (g_TimeLimitMsgMgr.CancelTimeLimitedMsg(IUTIL->Crc32("NGMTC_RightNotice")))
	{
		tagNGMTS_CancelRightNotice ret;
		ret.dwErrorCode = E_GMTool_Success;
		pClient->SendMsg(&ret, ret.dwSize);
	}
	else
	{
		tagNGMTS_CancelRightNotice ret;
		ret.dwErrorCode = E_GMTool_Err;
		pClient->SendMsg(&ret, ret.dwSize);
		return FALSE;
	}

	LogGMAction(pClient, EAT_NoticeStart, _T("cancel right notice"));
	return TRUE;
}

//-----------------------------------------------------------------
// �Զ�ѭ������
//-----------------------------------------------------------------
DWORD	ClientMgr::HandleAutoNotice(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...auto notice.\n"));
	tagNGMTC_AutoNotice* pTemp = (tagNGMTC_AutoNotice *)pMsg;
	INT nNum = (pMsg->dwSize - (sizeof(tagNGMTC_AutoNotice) - sizeof(DWORD))) / sizeof(DWORD);

	tagNGMTC_AutoNotice *pRecv = (tagNGMTC_AutoNotice *)malloc(sizeof(tagNGMTC_AutoNotice) + nNum*sizeof(DWORD) - sizeof(DWORD)); // ��UpdateTimeLeftList,~TimeLimitMsgMgr���ͷ�
	pRecv->dwID = IUTIL->Crc32("NGMTC_AutoNotice");
	pRecv->dwSize = sizeof(tagNGMTC_AutoNotice) + nNum*sizeof(DWORD) - sizeof(DWORD);
	pRecv->dwSectionID = pTemp->dwSectionID;
	pRecv->eType = pTemp->eType;
	pRecv->nCirInterval = pTemp->nCirInterval;
	pRecv->nCirNum = pTemp->nCirNum;
	pRecv->nTimeInterval = pTemp->nTimeInterval;
	_tcscpy_s(pRecv->szContent, Notice_Content, pTemp->szContent);
	
	TCHAR szWorldStr[X_LONG_STRING] = {0};
	TCHAR szWorldID[X_SHORT_STRING] = {0};
	for (int i=0;i<nNum;++i)
	{
		pRecv->dwWorldID[i] = pTemp->dwWorldID[i];
		_stprintf(szWorldID, _T("%d"), pTemp->dwWorldID[i]);
		if (i>0) _tcscat_s(szWorldStr, X_LONG_STRING, _T(","));
		_tcscat_s(szWorldStr, X_LONG_STRING, szWorldID);
		ZeroMemory(szWorldID, X_SHORT_STRING);
	}

	//g_TimeLimitMsgMgr.SetCountForAutoNotice(pRecv->nCirNum);

	tagDWORDTime dwOpenTime = GetCurrentDWORDTime();
	for(int i = 0;i<pRecv->nCirNum;++i)
	{
		//���㿪ʼʱ��
		dwOpenTime =+ i*pRecv->nTimeInterval;

		// ������Ϣ������ʱ��Ϣ�������е� m_LstMsgTimeDeadLine ������
		g_TimeLimitMsgMgr.AddMsg2LMTDL(IUTIL->Crc32("NGMTC_AutoNotice"), dwOpenTime, TRUE, pRecv, pClient->GetID());

		//new tagNGMTC_AutoNotice(*pRecv);
		// ��log
		TCHAR szLog[X_LONG_STRING] = {0};
		_stprintf(szLog, _T("send auto_notice(\"%s\") to section-%d(world %s) circle last: %ds, circle internal: %ds, circle count: %d"),
			pTemp->szContent, pTemp->dwSectionID, szWorldStr, pTemp->nTimeInterval, pTemp->nCirInterval, pTemp->nCirNum);
		LogGMAction(pClient, EAT_NoticeStart, szLog);
	}

	tagNGMTS_TimeLimitedMsg ret;
	ret.eType = ETLM_AutoNotice;
	ret.dwErrorCode = 0;
	pClient->SendMsg(&ret, ret.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------
// 
//-----------------------------------------------------------------
DWORD ClientMgr::HandleCreateItem( tagNetCmd* pMsg, Client* pClient )
{
	_tprintf(_T("Msg from gmtool...cancel auto chat_notice.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_ItemCreate);

	ServerDB* pServerDB = g_SectionMgr.GetServerDB(pRecv->dwSectionID, pRecv->dwWorldID);
	if (!P_VALID(pServerDB)) return GT_INVALID;

	DWORD dwRoleID = GT_INVALID;
	DWORD dwAccountID = GT_INVALID;
	
	DWORD dwRtv = E_GMTool_Success;

	if (!pServerDB->DBQueryRoleIDByName(pRecv->szName, dwRoleID))
	{
		dwRtv = E_GMTool_RoleNotExist;
	}
	else if (!pServerDB->DBQueryAccountIDByRoleID(dwRoleID, dwAccountID))
	{
		dwRtv = E_GMTool_RoleNotExist;
	}
	else if (MIsEquipment(pRecv->dwItemTypeID) && pRecv->nItemNum != 1)
	{
		dwRtv = E_GMTool_CreateItem_EquipNumErr;
	}
// 	else if (!MIsEquipment(pRecv->dwItemTypeID) && pRecv->byQuality != -1)
// 	{
// 		dwRtv = E_GMTool_CreateItem_ItemQualityErr;
// 	}
	else if	(!pServerDB->DBCreateItem(dwAccountID, dwRoleID, pRecv->dwItemTypeID, pRecv->nItemNum, pRecv->byQuality))
	{
		dwRtv = E_GMTool_Err;
	}
	else
	{
		TCHAR tszBuffer[500] = {0};
		_stprintf(tszBuffer, _T("%s��ɫ���ɵ�%dƷ�ʡ�%d������%d���ߣ�������Ϊ%d"), pRecv->szName, pRecv->byQuality, pRecv->nItemNum, pRecv->dwItemTypeID);
		LogGMAction(pClient, EAT_ItemCreate, tszBuffer);
	}

	tagNGMTS_ItemCreate send;
	send.dwErrorCode = dwRtv;
	pClient->SendMsg(&send, send.dwSize);

	return 0;
}

DWORD ClientMgr::HandleDeleteItem( tagNetCmd* pMsg, Client* pClient )
{
	_tprintf(_T("Msg from gmtool...delete object.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_ItemDelete);

	ServerDB* pServerDB = g_SectionMgr.GetServerDB(pRecv->dwSectionID, pRecv->dwWorldID);
	if (!P_VALID(pServerDB)) return GT_INVALID;

	DWORD dwRoleID	= GT_INVALID;
	DWORD dwRtv = E_GMTool_Success;
	if (!pServerDB->DBQueryRoleIDByName(pRecv->szName, dwRoleID))
	{
		dwRtv = E_GMTool_RoleNotExist;
	}
	else
	{
		TCHAR szTmpBigStr[X_HUGE_STRING*X_HUGE_STRING] = {0};
		_tcscpy_s(szTmpBigStr, X_HUGE_STRING*X_HUGE_STRING, pRecv->sz64IDString);
		int nLength = _tcslen(szTmpBigStr);

		TCHAR szItem64ID[X_LONG_STRING] = {0};
		int i = 0;
		int start = 0;
		int total = 0;
		BOOL bLast = FALSE;
		BOOL bErrorHappen = FALSE;

		while( szTmpBigStr[i] != '\0' )
		{
			total++;

			if ( szTmpBigStr[i] == ',' || total == nLength )
			{
				if ( total == nLength )
				{
					i++;
					//total = 0;
					bLast = TRUE;
				}

				szTmpBigStr[i] = '\0';
				
				_tcscpy_s( szItem64ID, (i-start)*sizeof(TCHAR), szTmpBigStr );

				{
					INT64 n64ItemID= 0;
					n64ItemID = _tstoi64(szItem64ID);

					dwRtv = pServerDB->DBDeleteItem(dwRoleID, n64ItemID);
					if ( dwRtv != E_GMTool_Success )
					{
						tagNGMTS_ItemDelete send;
						send.dwErrorCode = dwRtv;
						pClient->SendMsg(&send, send.dwSize);

						bErrorHappen = TRUE;
					}
					else
					{
						TCHAR tszBuffer[X_LONG_STRING] = {0};
						_stprintf( tszBuffer, _T("%s��ɫɾ�����к�Ϊ%d����Ʒ"), pRecv->szName, n64ItemID );
						LogGMAction(pClient, EAT_ItemDelete, tszBuffer);
					}
				}
				
				if( !bLast )
				{
					start = i+1;
					TCHAR* pTmp = &(szTmpBigStr[start]);
					_tcscpy_s(szTmpBigStr, X_HUGE_STRING*X_HUGE_STRING, pTmp);
				}
				else
				{
					break;
				}

				i = 0;
				start = 0;
				
				continue;
			}
			else
			{
				i++;
			}
		}

		if ( bErrorHappen == FALSE )
		{
			tagNGMTS_ItemDelete send;
			send.dwErrorCode = E_GMTool_Success;
			pClient->SendMsg(&send, send.dwSize);
		}
	}
	
	return 0;
}

//-----------------------------------------------------------------
// �ָ�����
//-----------------------------------------------------------------
DWORD ClientMgr::HandleItemResume(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...object resume.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNGMTC_ItemResume);

	ServerDB* pServerDB = g_SectionMgr.GetServerDB(pRecv->dwSectionID, pRecv->dwWorldID);
	if (!P_VALID(pServerDB)) return GT_INVALID;

	DWORD dwRtv = E_GMTool_Success;

	// ���Ŀ���ɫ���Ƿ���ڣ����ҵ���Ӧ��roleID��AccountID
	DWORD dwRoleID	= GT_INVALID;
	DWORD dwAccountID = GT_INVALID;
	if ( _tcscmp(pRecv->szTargetRole, _T("")) != 0 ) // ��д�˻ָ�Ŀ��
	{
		if ( pServerDB->DBQueryRoleIDByName(pRecv->szTargetRole, dwRoleID) )
		{
			if ( !pServerDB->DBQueryAccountIDByRoleID(dwRoleID, dwAccountID) )
				dwRtv = E_GMTool_RoleNotExist;;
		}
		else
		{
			dwRtv = E_GMTool_RoleNotExist;
		}
	}
	else // δ��д�ָ�Ŀ��
	{
		dwRoleID = 0;
		dwAccountID = 0;
	}
	
	if ( dwRtv == E_GMTool_RoleNotExist )
	{
		tagNGMTS_ItemResume send;
		send.dwErrorCode = dwRtv;
		pClient->SendMsg(&send, send.dwSize);

		return 0;
	}

	TCHAR szTmpBigStr[X_HUGE_STRING*X_HUGE_STRING] = {0};
	_tcscpy_s(szTmpBigStr, X_HUGE_STRING*X_HUGE_STRING, pRecv->sz64IDString);
	int nLength = _tcslen(szTmpBigStr);

	TCHAR szItem64ID[X_LONG_STRING] = {0};
	int i = 0;
	int start = 0;
	int total = 0;
	BOOL bLast = FALSE;
	BOOL bErrorHappen = FALSE;

	while( szTmpBigStr[i] != '\0' )
	{
		total++;

		if ( szTmpBigStr[i] == ',' || total == nLength )
		{
			if ( total == nLength )
			{
				i++;
				//total = 0;
				bLast = TRUE;
			}

			szTmpBigStr[i] = '\0';

			_tcscpy_s( szItem64ID, (i-start)*sizeof(TCHAR), szTmpBigStr );

			{
				INT64 n64ItemID= 0;
				n64ItemID = _tstoi64(szItem64ID);
			
				dwRtv = pServerDB->DBResumeItem( n64ItemID, dwRoleID, dwAccountID );

				if ( dwRtv != E_GMTool_Success )
				{
					tagNGMTS_ItemResume send;
					send.dwErrorCode = dwRtv;
					pClient->SendMsg(&send, send.dwSize);

					bErrorHappen = TRUE;
				}
				else
				{
					TCHAR tszBuffer[ X_LONG_STRING ] = {0};
					_stprintf(tszBuffer, _T("Ϊ��ɫ%s�ָ����к�Ϊ%d����Ʒ"), pRecv->szTargetRole, n64ItemID );
					LogGMAction(pClient, EAT_ItemDelete, tszBuffer);
				}
			}

			if( !bLast )
			{
				start = i+1;
				TCHAR* pTmp = &(szTmpBigStr[start]);
				_tcscpy_s(szTmpBigStr, X_HUGE_STRING*X_HUGE_STRING, pTmp);
			}
			else
			{
				break;
			}

			i = 0;
			start = 0;

			continue;
		}
		else
		{
			i++;
		}
	}

	if ( bErrorHappen == FALSE )
	{
		tagNGMTS_ItemResume send;
		send.dwErrorCode = E_GMTool_Success;
		pClient->SendMsg(&send, send.dwSize);
	}

	return 0;
}

// �޸�IP��
DWORD ClientMgr::HandleChangeIPBind(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...change IP bind.\n"));

	MTRANS_POINTER(pRecv, pMsg, tagNRTC_BindIP);

	DWORD dwErr = E_GMTool_Success;
	INT nPrivilege = GT_INVALID;

	if (!m_ClientDB.DBQueryAccountPrivilege(pRecv->szAccountName, nPrivilege))
	{
		dwErr	= E_GMTool_GM_NotExist;
	}
	else if (GT_VALID(nPrivilege))
	{
		if (MIS_GMOPGM(pClient->GetPrivilege(), nPrivilege)||
			MIS_RTOPRT(pClient->GetPrivilege(),	nPrivilege)	
			)
		{
			if (m_ClientDB.DBReplaceBindIp(pRecv->szAccountName, pRecv->dwIP))
			{
				dwErr = E_GMTool_Success;

				TCHAR tszBuffer[500] = {0};
				TObjRef<StreamTransport> pTrans = "StreamTransport";
				TObjRef<Util> pUtil = "Util";
				_stprintf(	tszBuffer, _T(" ���˺�%s��ipΪ%s"), 
							pRecv->szAccountName, 
							pUtil->Unicode8ToUnicode(pTrans->IP2String(pRecv->dwIP))
							);
				LogGMAction(pClient, EAT_IpBind, tszBuffer);
			}
			else
			{
				dwErr = E_GMTool_Err;
			}
		}
		else
		{
			dwErr	= E_RTClient_IpBind_Err;
		}
	}
	else
	{
		ASSERT(0);
		dwErr	= E_RTClient_IpBind_Err;
	}

	tagNRTS_BindIP send;
	send.dwErrorCode	= dwErr;
	pClient->SendMsg(&send, send.dwSize);

	return 0;
}

DWORD ClientMgr::GetClientIP( DWORD dwClientID )
{
	sockaddr_in ip;
	ZeroMemory(&ip, sizeof(ip));
	m_pStreamServer->GetClientSockName(&ip, dwClientID);
	DWORD dwClientIP = (DWORD)(ip.sin_addr.S_un.S_addr);
	return dwClientIP;
}

VOID ClientMgr::LogGMAction( Client* pClient, EActionType eActionType, LPCTSTR szDetails )
{
	if (!P_VALID(pClient) || !IsActionValid(eActionType))
	{
		return;
	}

	m_ClientDB.DBLogGMAction(pClient->GetAccountID(), pClient->GetName(), pClient->GetIP(), eActionType, szDetails);
}

//-----------------------------------------------------------------
// IP������
//-----------------------------------------------------------------
DWORD ClientMgr::HandleBlacklist(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...IP blacklist.\n"));

	tagNGMTC_Blacklist* pRecv = (tagNGMTC_Blacklist*) pMsg;

	Section* pSection = g_SectionMgr.GetSection(pRecv->szSectionName);
	if ( !P_VALID(pSection) ) return FALSE;
	
	tagNSS_ForbidIP send;
	send.bForbid = (BOOL)pRecv->nAction;
	send.dwIP = pRecv->dwIP;
	send.dwClientID = pClient->GetID();
	BOOL bSuccess = pSection->IPBlacklist(&send);

	return bSuccess;
}

//-----------------------------------------------------------------
// ��հٱ���
//-----------------------------------------------------------------
DWORD ClientMgr::HandleClearBaibao(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...Clear Baibao.\n"));

	tagNGMTC_ClearBaibao* pRecv = (tagNGMTC_ClearBaibao*) pMsg;

	Section* pSection = g_SectionMgr.GetSection(pRecv->dwSectionID);
	if ( !P_VALID(pSection) ) return FALSE;

	tagNSS_ClearBaibao send;
	send.dwClientID = pClient->GetID();
	_tcscpy_s(send.szOwnerName, X_SHORT_NAME, pRecv->szOwnerName);
	pClient->SendMsg(&send, send.dwSize);

	return TRUE;
}

DWORD ClientMgr::HandleAutoChatNotice(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...auto chat_notice.\n"));

	tagNGMTC_AutoChatNotice* pTemp = (tagNGMTC_AutoChatNotice *)pMsg;
	INT nNum = (pMsg->dwSize - (sizeof(tagNGMTC_AutoChatNotice) - sizeof(DWORD))) / sizeof(DWORD);

	tagNGMTC_AutoChatNotice *pRecv = (tagNGMTC_AutoChatNotice *)malloc(sizeof(tagNGMTC_AutoChatNotice)+nNum*sizeof(DWORD)-sizeof(DWORD)) ; // ��UpdateTimeLeftList,~TimeLimitMsgMgr���ͷ�	
	pRecv->dwID = IUTIL->Crc32("NGMTC_AutoChatNotice");
	pRecv->dwSize = sizeof(tagNGMTC_AutoChatNotice) + nNum*sizeof(DWORD) - sizeof(DWORD);
	pRecv->byHour = pTemp->byHour;
	pRecv->byMinute = pTemp->byMinute;
	pRecv->dwSectionID = pTemp->dwSectionID;
	_tcscpy_s(pRecv->szNotice, X_HUGE_STRING, pTemp->szNotice);
	
	TCHAR szWorldStr[X_LONG_STRING] = {0};
	TCHAR szWorldID[X_SHORT_STRING] = {0};
	for (int i=0;i<nNum;++i)
	{
		pRecv->dwWorldID[i] = pTemp->dwWorldID[i];
		_stprintf(szWorldID, _T("%d"), pTemp->dwWorldID[i]);
		if (i>0) _tcscat_s(szWorldStr, X_LONG_STRING, _T(","));
		_tcscat_s(szWorldStr, X_LONG_STRING, szWorldID);
		ZeroMemory(szWorldID, X_SHORT_STRING);
	}

	// ������Ϣ������ʱ��Ϣ�������е� m_LstMsgTimeDeadLine ������
	tagDWORDTime time(0, pTemp->byMinute, pTemp->byHour, 0, 0, 0);
	DWORD dwMsgCrc = IUTIL->Crc32("NGMTC_AutoChatNotice");
	g_TimeLimitMsgMgr.AddMsg2LMTDL(dwMsgCrc, time, FALSE, pRecv, pClient->GetID());

	// ��log
	TCHAR szLog[X_LONG_STRING] = {0};
	_stprintf(szLog, _T("%d:%d send auto-chat-notice(\"%s\") to section-%d(world %s)"), pTemp->byHour, pTemp->byMinute, pTemp->szNotice, pTemp->dwSectionID, szWorldStr);
	LogGMAction(pClient, EAT_AutoChatStart, szLog);

	// ��ͻ��˷�����Ϣ
	tagNGMTS_TimeLimitedMsg ret;
	ret.eType = ETLM_AutoChatNotice;
	ret.dwErrorCode = 0;
	pClient->SendMsg(&ret, ret.dwSize);

	return TRUE;
}

DWORD ClientMgr::HandleCancelAutoChatNotice(tagNetCmd* pMsg, Client* pClient)
{
	_tprintf(_T("Msg from gmtool...cancel auto chat_notice.\n"));

	DWORD dwMsgCrc = IUTIL->Crc32("NGMTC_AutoChatNotice");
	if (g_TimeLimitMsgMgr.CancelTimeLimitedMsg(dwMsgCrc))
	{
		tagNGMTS_CancelAutoChatNotice ret;
		ret.dwErrorCode = E_GMTool_Success;
		pClient->SendMsg(&ret, ret.dwSize);
	}
	else
	{
		tagNGMTS_CancelAutoChatNotice ret;
		ret.dwErrorCode = E_GMTool_Err;
		pClient->SendMsg(&ret, ret.dwSize);
		return FALSE;
	}

	// ��log
	TCHAR szLog[X_LONG_STRING] = {0};
	_stprintf(szLog, _T("cancel auto_chat_notice"));
	LogGMAction(pClient, EAT_AutoChatEnd, szLog);

	return TRUE;
}

//DWORD ClientMgr::HandleCancelTimeLimitedMsg(tagNetCmd* pMsg, Client* pClient)
//{
//	g_TimeLimitMsgMgr.ClearMsgList();
//
//	tagNGMTS_CancelTimeLimitedMsg send;
//	send.dwErrorCode = 0;
//
//	pClient->SendMsg(&send, send.dwSize);
//
//	return TRUE;
//}

DWORD ClientMgr::HandleShowSumReq(tagNetCmd* pMsg, Client* pClient)
{
	// ��ô����������ܵ�world������������Ϣ��С
	INT nSectionCount = g_SectionMgr.GetSectionNum();
	INT nWorldCount = 0;
	INT nMsgSize = 0;

	TMap<DWORD, Section*> mapTmpSection = g_SectionMgr.GetSectionMap();
	TMap<DWORD, Section*>::TMapIterator it = mapTmpSection.Begin();
	Section* pSection= NULL;

	nMsgSize += sizeof(tagNSSS_GetDynInfo) - sizeof(tagSectionNode);
	while (mapTmpSection.PeekNext(it, pSection))
	{
		INT nThisSectionWorldCount = 0;
		nMsgSize += sizeof(tagSectionNode) - sizeof(tagWorldNode);
		
		nThisSectionWorldCount = pSection->GetWorldNum();
		nWorldCount += nThisSectionWorldCount; 
		
		nMsgSize += ( sizeof(tagWorldNode) * nThisSectionWorldCount );
	}

	tagNSSS_GetDynInfo* pSend = (tagNSSS_GetDynInfo*) malloc( nMsgSize);
	if(!pSend) return FALSE;

	pSend->dwID = IUTIL->Crc32("NSSS_GetDynInfo");
	pSend->dwSize = nMsgSize;
	pSend->nSectionCount = nSectionCount;


	// ��ʼ��װ��Ϣ
	//pSend->nSectionCount = g_SectionMgr.GetSectionNum();

	//TMap<DWORD, Section*> mapTmpSection = g_SectionMgr.GetSectionMap();
	it = mapTmpSection.Begin();
	pSection= NULL;
	
	LPBYTE pBuf = (LPBYTE) pSend->SectionInfo;
	while (mapTmpSection.PeekNext(it, pSection))
	{
		tagSectionNode* pSectionNode = (tagSectionNode*)pBuf;
		pSectionNode->dwSectionID = pSection->GetID();
		memcpy( pSectionNode->szSectionName, pSection->GetSectionName(), X_SHORT_NAME*sizeof(TCHAR));
		pSectionNode->nNum = pSection->GetWorldNum();
		
		TMap<DWORD, GameWorld*> mapTmpWorld = pSection->GetWorldMap();
		TMap<DWORD, GameWorld*>::TMapIterator it = mapTmpWorld.Begin();
		GameWorld* pWorld = NULL;
		INT m = 0;
		while (mapTmpWorld.PeekNext(it, pWorld))
		{
			pSectionNode->World[m].dwWorldID = pWorld->GetID();
			memcpy( pSectionNode->World[m].szWorldName, pWorld->GetWorldName(), X_SHORT_NAME*sizeof(TCHAR));
			pSectionNode->World[m].nOnline = pWorld->GetWorld()->GetOnlineDisplay();
			pSectionNode->World[m].nRealOnline = pWorld->GetWorld()->GetOnline();		
			
			m++;
		}

		pBuf += sizeof(tagSectionNode) - sizeof(tagWorldNode);
		pBuf += sizeof(tagWorldNode) * pSectionNode->nNum;
	}
	pClient->SendMsg(pSend, pSend->dwSize);
	if(pSend)free(pSend);
	return TRUE;
}

DWORD ClientMgr::HandleModifyFackNum(tagNetCmd* pMsg, Client* pClient)
{
	tagNGMTS_GMSetFackNumer* pRecv = (tagNGMTS_GMSetFackNumer*)pMsg;
	float rate = pRecv->fFackRate;
	if(rate <=0.0) return FALSE;

	tagNSS_SetFackNumer send;
	send.fFackRate = rate;
	TMap<DWORD, Section*> seciontMap = g_SectionMgr.GetSectionMap();
	TMap<DWORD, Section*>::TMapIterator it = seciontMap.Begin();
	Section* pSection = NULL;
	DWORD worldIds[1];
	worldIds[0] = GT_INVALID;
	while(seciontMap.PeekNext(it, pSection))
	{
		pSection->ForAllGameWorld(worldIds, 0, (FUN_GAMEWORLD)&(GameWorld::SendWorldMsg), &send);
	}
	return TRUE;
}