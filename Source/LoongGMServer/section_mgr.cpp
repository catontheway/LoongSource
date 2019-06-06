//-----------------------------------------------------------------------------
//!\file section_mgr.cpp
//!\brief ����������������÷�������������д���
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/rt_define.h"
#include "../ServerDefine/msg_rt_s.h"
#include "../ServerDefine/msg_gm_tool_c.h"
#include "../ServerDefine/base_define.h"

#include "game_server.h"
#include "game_world.h"
#include "server.h"
#include "section.h"
#include "section_mgr.h"
#include "client.h"

//-----------------------------------------------------------------------------
// ȫ�ֶ���
//-----------------------------------------------------------------------------
SectionMgr g_SectionMgr;

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
SectionMgr::SectionMgr() : m_Trunk(this), m_nPort(0), 
m_nSendInfoTickCountDown(SEND_INFO_TICK_COUNT_DOWN),
m_bSendImmediately(FALSE)
{

}

//------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------
BOOL SectionMgr::Init()
{
	TObjRef<VarContainer> pVar = "VarContainer";

	// �������д���
	if( FALSE == LoadAllServerInfo() )
	{
		return FALSE;
	}

	// �����������������
	CreateObj("NetCmdMgrServer", "NetCmdMgr");
	m_pCmdMgr = "NetCmdMgrServer";
	if( !P_VALID(m_pCmdMgr) ) return FALSE;

	// ���ɷ���������ӿ�
	CreateObj("StreamServerServer", "StreamServer");
	m_pStreamServer= "StreamServerServer";
	if( !P_VALID(m_pStreamServer) ) return FALSE;

	// ע����������
	RegisterServerMsg();

	// ��ʼ������
	m_nPort = pVar->GetInt(_T("port"), _T("server"));
	m_pStreamServer->Init(	(TCPLOGINCALLBACK)m_Trunk.sfp2(&SectionMgr::LoginCallBack),
							(TCPLOGOUTCALLBACK)m_Trunk.sfp1(&SectionMgr::LogoutCallBack),
							m_nPort);

	return TRUE;
}

//------------------------------------------------------------------------------
// �������й���ķ���������
//------------------------------------------------------------------------------
BOOL SectionMgr::LoadAllServerInfo()
{
	TObjRef<VarContainer> pVar = "VarContainer";

	// ��ȡ���п��Թ���Ĵ�������
	INT nSectionCount = pVar->GetDword(_T("num"), _T("section"));
	if( nSectionCount <= 0 ) return FALSE;
	
	// ���ظ�������
	TCHAR szTemp[X_SHORT_NAME] = {'\0'};

	for(INT n = 0; n < nSectionCount; ++n)
	{
		_stprintf(szTemp, _T("section_%d"), n);

		LPCTSTR szSectionName = pVar->GetString(_T("name"), szTemp);

		// �����������ID
		DWORD dwSectionID = pVar->GetDword(_T("id"), szTemp);

		// ���ɴ���
		Section* pSection = new Section(dwSectionID, szSectionName);

		// ��ʼ������
		if( FALSE == pSection->Init() )
		{
			SAFE_DEL(pSection);
			return FALSE;
		}

		// ���뵽�б���
		m_mapSection.Add(pSection->GetID(), pSection);
	}

	return TRUE;
}

//------------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------------
VOID SectionMgr::Destroy()
{
	// ɾ�����еĴ���
	Section* pSection = NULL;
	TMap<DWORD, Section*>::TMapIterator it = m_mapSection.Begin();

	while( m_mapSection.PeekNext(it, pSection) )
	{
		pSection->Destroy();
		SAFE_DEL(pSection);
	}
	m_mapSection.Clear();

	// ע������
	if(P_VALID(m_pStreamServer))	
	{
		m_pStreamServer->Destroy();
		KillObj("StreamServerServer");
	}

	// ע�����е���������
	UnRegisterServerMsg();
	KillObj("NetCmdMgrServer");
	
}

//--------------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------------
VOID SectionMgr::Update()
{
	UpdateSession();
	SendAllServerDynamicInfo();
	
}

//---------------------------------------------------------------------------------------
// ��������������Ϣ
//---------------------------------------------------------------------------------------
VOID SectionMgr::UpdateSession()
{
	// �������еĴ���
	Section* pSection = NULL;
	TMap<DWORD, Section*>::TMapIterator it = m_mapSection.Begin();

	while( m_mapSection.PeekNext(it, pSection) )
	{
		pSection->Update();
	}
}

//----------------------------------------------------------------------------------------
// ע�����������������
//----------------------------------------------------------------------------------------
VOID SectionMgr::RegisterServerMsg()
{
	m_pCmdMgr->Register("NSC_LoginService",	(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleWorldLogin),		_T("NSC_LoginService"));
	m_pCmdMgr->Register("NSC_WorldInfo",	(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleGetWorldInfo),		_T("NSC_WorldInfo"));
	m_pCmdMgr->Register("NLSC_LoginService",(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRTServiceLogin),	_T("NLSC_LoginService"));

	m_pCmdMgr->Register("NSC_Double",		(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRetDouble),			_T("NSC_Double"));
	m_pCmdMgr->Register("NSC_AutoNotice",	(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRetAutoNotice),		_T("NSC_AutoNotice"));
	m_pCmdMgr->Register("NSC_RightNotice",	(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRetRightNotice),	_T("NSC_RightNotice"));
	m_pCmdMgr->Register("NSC_ForbidAccount",(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRetAccountForbid),	_T("NSC_ForbidAccount"));
	m_pCmdMgr->Register("NSC_ForbidIP",		(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRetIPBlacklist),	_T("NSC_ForbidIP"));
	m_pCmdMgr->Register("NSC_CancelDouble",	(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRetCancelDouble),	_T("NSC_CancelDouble"));
	//m_pCmdMgr->Register("NSC_CancelRightNotice",	(NETMSGPROC)m_Trunk.sfp2(&SectionMgr::HandleRetCancelRightNotice),	_T("NSC_CancelRightNotice"));
}

//-----------------------------------------------------------------------------------------
// ע������������������
//-----------------------------------------------------------------------------------------
VOID SectionMgr::UnRegisterServerMsg()
{
	if(P_VALID(m_pCmdMgr))
	{
		m_pCmdMgr->Destroy();
		KillObj("NetCmdMgrServer");
	}	
}

//------------------------------------------------------------------------------------------
// ��������¼�ص�
//------------------------------------------------------------------------------------------
DWORD SectionMgr::LoginCallBack(LPVOID p1, LPVOID p2)
{
	static DWORD dwFirstMsgCrc = IUTIL->Crc32("NSC_LoginService");

	// �鿴��Ϣ
	tagNSC_LoginService* pRecv = (tagNSC_LoginService*)p1;
	if( pRecv->dwID != dwFirstMsgCrc ) return GT_INVALID;

	g_Server.Lock();

	// �ҵ���Ӧ��server
	GameServer* pServer = GetServer(pRecv->dwSectionID, pRecv->dwWorldID, pRecv->eType);
	if( !P_VALID(pServer) )
	{
		g_Server.UnLock();
		return GT_INVALID;
	}

	// �鿴server�ǲ����Ѿ���������
	if( pServer->IsValid() )
	{
		g_Server.UnLock();
		return GT_INVALID;
	}

	// �鿴�ܲ��ܿ����ɹ�
	if( !pServer->ServerStart() )
	{
		g_Server.UnLock();
		return GT_INVALID;
	}

	g_Server.UnLock();
	return pServer->GetID();
}

//-------------------------------------------------------------------------------------------
// �������ǳ��ص�����
//-------------------------------------------------------------------------------------------
DWORD SectionMgr::LogoutCallBack(LPVOID p1)
{
	DWORD dwID = (DWORD)p1;

	g_Server.Lock();

	// �ҵ�server
	GameServer* pServer = FindServer(dwID);
	if( !P_VALID(pServer) )
	{
		g_Server.UnLock();
		return 0;
	}

	// �Ƿ��Ѿ�������
	if( !pServer->IsValid() )
	{
		g_Server.UnLock();
		return 0;
	}

	pServer->ServerShutDown();
	g_Server.UnLock();

	return 0;
}

//-------------------------------------------------------------------------------------------
// ���ɹ���ķ����������еľ�̬��Ϣ��ĳ���ͻ���
//-------------------------------------------------------------------------------------------
VOID SectionMgr::SendAllServerStaticInfoToClient(Client* pClient)
{
	static BYTE byBuff[1024*1024] = {0};		// Ԥ������һ���㹻��Ļ���
	static DWORD dwMsgID = IUTIL->Crc32("NRTS_AllServerStatInfo");

	if( !P_VALID(pClient) ) return;

	tagNRTS_AllServerStatInfo* pSend = (tagNRTS_AllServerStatInfo*)byBuff;
	DWORD dwSize = sizeof(tagNRTS_AllServerStatInfo)-1;		// ��Ϣ�ܴ�С

	// �õ����еĴ�������
	pSend->nSectionNum = GetSectionNum();
	if( pSend->nSectionNum <= 0 ) return;

	// ��ÿ����������Լ��ľ�̬��Ϣ
	LPBYTE pBuff = byBuff + dwSize;

	Section* pSection = NULL;
	TMap<DWORD, Section*>::TMapIterator it = m_mapSection.Begin();

	while( m_mapSection.PeekNext(it, pSection) )
	{
		pSection->GenerateSectionStaticInfo(pBuff, dwSize);
	}

	// ��дID��size
	pSend->dwID = dwMsgID;
	pSend->dwSize = dwSize;

	// ������Ϣ
	pClient->SendMsg(byBuff, dwSize);
}

//----------------------------------------------------------------------------------------
// �������з�����״̬
//----------------------------------------------------------------------------------------
VOID SectionMgr::SendAllServerDynamicInfo()
{
	static BYTE byBuff[1024*1024] = {0};
	static DWORD dwMsgID = IUTIL->Crc32("NRTS_AllServerDynaInfo");

	// ���µ���ʱ
	if( --m_nSendInfoTickCountDown > 0 && !m_bSendImmediately)
	{
		return;
	}

	// ����ʱ���ˣ����з���
	m_bSendImmediately = FALSE;
	tagNRTS_AllServerDynaInfo* pSend = (tagNRTS_AllServerDynaInfo*)byBuff;
	DWORD dwSize = sizeof(tagNRTS_AllServerDynaInfo)-1;		// ��Ϣ�ܴ�С

	// �õ����еĴ�������
	pSend->nSectionNum = GetSectionNum();
	if( pSend->nSectionNum <= 0 ) return;

	// ��ÿ����������Լ��ľ�̬��Ϣ
	LPBYTE pBuff = byBuff + dwSize;

	Section* pSection = NULL;
	TMap<DWORD, Section*>::TMapIterator it = m_mapSection.Begin();

	while( m_mapSection.PeekNext(it, pSection) )
	{
		pSection->GenerateSectionDynamicInfo(pBuff, dwSize);
	}

	// ��дID��size
	pSend->dwID = dwMsgID;
	pSend->dwSize = dwSize;

	// ������Ϣ
	g_ClientMgr.SendAllClientMsg(byBuff, dwSize);
	_tprintf(_T("send msg to rtclient...server dynamic status.\n"));

	// ���õ���ʱ
	m_nSendInfoTickCountDown = SEND_INFO_TICK_COUNT_DOWN;
}

//-------------------------------------------------------------------------------------------
// ����ĳ��������Server
//-------------------------------------------------------------------------------------------
BOOL SectionMgr::StartServer(EServerType eType, DWORD dwSectionID, DWORD dwWorldID)
{
	Section* pSection = GetSection(dwSectionID);
	if( !P_VALID(pSection) ) return FALSE;

	BOOL bSuccess = FALSE;
	switch (eType)
	{
	case EST_Login:
		bSuccess = pSection->StartLoginServer();
		break;

	case EST_DB:
	case EST_World:
		bSuccess = pSection->StartWorld(dwWorldID, eType);
		break;
	}

	return bSuccess;
}

//-------------------------------------------------------------------------------------------
// �ر�ָ��������Server
//-------------------------------------------------------------------------------------------
BOOL SectionMgr::CloseServer(EServerType eType, DWORD dwSectionID, DWORD dwWorldID, BOOL bForce)
{
	Section* pSection = GetSection(dwSectionID);
	if( !P_VALID(pSection) ) return FALSE;

	BOOL bSuccess = FALSE;
	switch (eType)
	{
	case EST_Login:
		bSuccess = pSection->CloseLoginServer(bForce);
		break;

	case EST_DB:
	case EST_World:
		bSuccess = pSection->CloseWorld(dwWorldID, eType, bForce);
		break;
	}

	return bSuccess;
}

//-------------------------------------------------------------------------------------------
// ���ݴ���ID����Ϸ����ID�����͵õ�һ��gameserver
//-------------------------------------------------------------------------------------------
GameServer* SectionMgr::GetServer(DWORD dwSectionID, DWORD dwWorldID, EServerType eType)
{
	Section* pSection = GetSection(dwSectionID);
	if( !P_VALID(pSection) ) return FALSE;

	if( EST_Login == eType )
	{
		return pSection->GetLoginServer();
	}
	else
	{
		GameWorld* pWorld = pSection->GetWorld(dwWorldID);
		if( !P_VALID(pWorld) ) return NULL;

		if( EST_World == eType )
		{
			return pWorld->GetWorld();
		}
		else if( EST_DB == eType )
		{
			return pWorld->GetDB();
		}
		else
		{
			return NULL;
		}
	}
}

//-------------------------------------------------------------------------------------------------
// ����SectionID��WorldID�õ�ָ����ServerDB
//-------------------------------------------------------------------------------------------------
ServerDB* SectionMgr::GetServerDB( DWORD dwSectionID, DWORD dwWorldID )
{
	Section* pSection = g_SectionMgr.GetSection(dwSectionID);
	if (!P_VALID(pSection))	
		return NULL;

	GameWorld* pGameWorld = pSection->GetWorld(dwWorldID);
	if (!P_VALID(pGameWorld))
		return NULL;

	ServerDB* pSeverDB = pGameWorld->GetServerDB();

	return pSeverDB;
}

//-------------------------------------------------------------------------------------------
// �ҵ�һ��IDָ����server
//-------------------------------------------------------------------------------------------
GameServer* SectionMgr::FindServer(DWORD dwID)
{
	// ���ȸ���ID�ҵ���Ӧ�Ĵ���ID
	DWORD dwSectionID = m_mapIDSectionID.Peek(dwID);

	if( !P_VALID(dwSectionID) ) return NULL;

	// �ҵ���Ӧ�Ĵ���
	Section* pSection = m_mapSection.Peek(dwSectionID);
	if( !P_VALID(pSection) ) return NULL;

	// �ڴ��������ҵ�������
	return pSection->FindServer(dwID);
}

//---------------------------------------------------------------------------------------------
// �����ƶ���ID�õ�ָ����loginServer
//---------------------------------------------------------------------------------------------
GameServerLogin* SectionMgr::FindLoginServer(DWORD dwID)
{
	// ���ȸ���ID�ҵ���Ӧ�Ĵ���ID
	DWORD dwSectionID = m_mapIDSectionID.Peek(dwID);

	if( !P_VALID(dwSectionID) ) return NULL;

	// �ҵ���Ӧ�Ĵ���
	Section* pSection = m_mapSection.Peek(dwSectionID);
	if( !P_VALID(pSection) ) return NULL;

	// �ڴ��������ҵ�������
	return pSection->FindLoginServer(dwID);
}

//-----------------------------------------------------------------------------------------------
// �����ƶ���ID�õ�ָ����loongworld
//-----------------------------------------------------------------------------------------------
GameServerWorld* SectionMgr::FindWorldServer(DWORD dwID)
{
	// ���ȸ���ID�ҵ���Ӧ�Ĵ���ID
	DWORD dwSectionID = m_mapIDSectionID.Peek(dwID);

	if( !P_VALID(dwSectionID) ) return NULL;

	// �ҵ���Ӧ�Ĵ���
	Section* pSection = m_mapSection.Peek(dwSectionID);
	if( !P_VALID(pSection) ) return NULL;

	// �ڴ��������ҵ�������
	return pSection->FindWorldServer(dwID);
}

//-------------------------------------------------------------------------------------------------
// �����ƶ���ID�õ�ָ����loongdb
//-------------------------------------------------------------------------------------------------
GameServerDB* SectionMgr::FindDBServer(DWORD dwID)
{
	// ���ȸ���ID�ҵ���Ӧ�Ĵ���ID
	DWORD dwSectionID = m_mapIDSectionID.Peek(dwID);

	if( !P_VALID(dwSectionID) ) return NULL;

	// �ҵ���Ӧ�Ĵ���
	Section* pSection = m_mapSection.Peek(dwSectionID);
	if( !P_VALID(pSection) ) return NULL;

	// �ڴ��������ҵ�������
	return pSection->FindDBServer(dwID);
}

//-------------------------------------------------------------------------------------------------
// �����ƶ���ID�õ�ָ����GameWorld
//-------------------------------------------------------------------------------------------------
GameWorld* SectionMgr::FindGameWorld(DWORD dwID)
{
	// ���ȸ���ID�ҵ���Ӧ�Ĵ���ID
	DWORD dwSectionID = m_mapIDSectionID.Peek(dwID);

	if( !P_VALID(dwSectionID) ) return NULL;

	// �ҵ���Ӧ�Ĵ���
	Section* pSection = m_mapSection.Peek(dwSectionID);
	if( !P_VALID(pSection) ) return NULL;

	return pSection->FindGameWorld(dwID);
}

// ���ݴ��������ҵ�����
Section* SectionMgr::GetSection(LPCTSTR szSection)
{
	TMap<DWORD, Section*>::TMapIterator it = m_mapSection.Begin();
	Section* pSection = NULL;
	while (m_mapSection.PeekNext(it, pSection))
	{
		if ( _tcscmp(pSection->GetSectionName(), szSection) == 0 )
		{
			return pSection;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------------------------------
// �����½
//---------------------------------------------------------------------------------------------------
DWORD SectionMgr::HandleWorldLogin(tagNetCmd* pMsg, DWORD dwID)
{
	tagNSC_LoginService* pRecv = (tagNSC_LoginService*)pMsg;

	// �ҵ�server
	GameServer* pServer = FindServer(dwID);
	if( !P_VALID(pServer) ) return GT_INVALID;

	//pServer->m_dwLastestTick = GetCurrentDWORDTime();
	pServer->RefreshLastTickTime();

	// ���ͷ�����Ϣ
	tagNSS_LoginService send;
	send.dwErrorCode = 0;

	SendMsg(dwID, &send, send.dwSize);

	return 0;
}

//----------------------------------------------------------------------------------------------------
// ���������״̬
//----------------------------------------------------------------------------------------------------
DWORD SectionMgr::HandleGetWorldInfo(tagNetCmd* pMsg, DWORD dwID)
{
	_tprintf(_T("msg from game server...server dynamic status.\n"));
	tagNSC_WorldInfo* pRecv = (tagNSC_WorldInfo*)pMsg;

	// �ҵ�server
	GameServer* pServer = FindServer(dwID);
	if( !P_VALID(pServer) ) return GT_INVALID;
	
	// �����������ʱ��
	//pServer->m_dwLastestTick = GetCurrentDWORDTime();
	pServer->RefreshLastTickTime();

	// ���¸�server��״̬
	pServer->RefreshStatus(pRecv->eStatus, pRecv->nOnlineNum, pRecv->nMaxOnlineNum, pRecv->nOnlineNumDisplay);

	return 0;
}

//----------------------------------------------------------------------------------------------------
// RTService��½ȷ��
//----------------------------------------------------------------------------------------------------
DWORD SectionMgr::HandleRTServiceLogin(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NLSC_LoginService);

	return 0;
}

//----------------------------------------------------------------------------------------------------
// ��GMserver���ز������
//----------------------------------------------------------------------------------------------------
DWORD SectionMgr::HandleRetDouble(tagNetCmd* pMsg, DWORD dwID)
{
	MGET_MSG(pRecv, pMsg, NSC_Double);

	GameWorld* pWorld = g_SectionMgr.FindGameWorld(dwID);
	if (!P_VALID(pWorld))
	{
		return GT_INVALID;
	}

	if ( pRecv->dwErrorCode != 0 )
	{
		tagNGMTS_TimeLimitedMsg send;
		send.eType = ETLM_Double;
		send.dwErrorCode	= pRecv->dwErrorCode;
		send.dwWorldID		= pWorld->GetID();
		g_ClientMgr.SendMsg(pRecv->dwClientID, &send, send.dwSize);
	}
	return 0;
}

DWORD SectionMgr::HandleRetAutoNotice( tagNetCmd* pMsg, DWORD dwID )
{
	MGET_MSG(pRecv, pMsg, NSC_AutoNotice);

	GameWorld* pWorld = g_SectionMgr.FindGameWorld(dwID);
	if (!P_VALID(pWorld))
	{
		return GT_INVALID;
	}
	
	if ( pRecv->dwErrorCode != 0 )
	{
		tagNGMTS_TimeLimitedMsg send;
		send.eType = ETLM_AutoNotice;
		send.dwErrorCode	= pRecv->dwErrorCode;
		send.dwWorldID		= pWorld->GetID();
		g_ClientMgr.SendMsg(pRecv->dwClientID, &send, send.dwSize);
	}

	return 0;
}

DWORD SectionMgr::HandleRetRightNotice( tagNetCmd* pMsg, DWORD dwID )
{
	MGET_MSG(pRecv, pMsg, NSC_RightNotice);

	GameWorld* pWorld = g_SectionMgr.FindGameWorld(dwID);
	if (!P_VALID(pWorld))
	{
		return GT_INVALID;
	}

	if ( pRecv->dwErrorCode != 0 )
	{
		tagNGMTS_TimeLimitedMsg send;
		send.eType = ETLM_RightNotice;
		send.dwErrorCode = pRecv->dwErrorCode;
		send.dwWorldID		= pWorld->GetID();
		g_ClientMgr.SendMsg(pRecv->dwClientID, &send, send.dwSize);
	}
	
	return 0;
}

DWORD SectionMgr::HandleRetAccountForbid(tagNetCmd* pMsg, DWORD dwID)
{
	MGET_MSG(pRecv, pMsg, NSC_ForbidAccount);

	tagNGMTS_AccountForbid send;
	//send.bLast = TRUE; // Ĭ������������һ����Ϣ 
	_tcscpy(send.szAccount, pRecv->szAccount);
	send.dwErrorCode = pRecv->dwErrorCode;

	g_ClientMgr.SendMsg(pRecv->dwClientID, &send, send.dwSize);

	return 0;
}

DWORD SectionMgr::HandleRetIPBlacklist(tagNetCmd* pMsg, DWORD dwID)
{
	MGET_MSG(pRecv, pMsg, NSC_ForbidIP);

	tagNGMTS_Blacklist send;
	send.dwErrorCode = pRecv->dwErrorCode;

	g_ClientMgr.SendMsg(pRecv->dwClientID, &send, send.dwSize);

	return 0;
}

DWORD SectionMgr::HandleRetCancelDouble(tagNetCmd* pMsg, DWORD dwID)
{
	MGET_MSG(pRecv, pMsg, NSC_CancelDouble);

	tagNGMTS_CancelDoubleMsg send;
	send.dwErrorCode = pRecv->dwErrorCode;

	g_ClientMgr.SendMsg(pRecv->dwClientID, &send, send.dwSize);

	return 0;
}

//DWORD SectionMgr::HandleRetCancelRightNotice(tagNetCmd* pMsg, DWORD dwID)
//{
//	MGET_MSG(pRecv, pMsg, NSC_CancelRightNotice);
//
//	tagNGMTS_CancelRightNotice send;
//	send.dwErrorCode = pRecv->dwErrorCode;
//
//	g_ClientMgr.SendMsg(pRecv->dwClientID, &send, send.dwSize);
//
//	return 0;
//}