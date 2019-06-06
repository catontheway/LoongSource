//-----------------------------------------------------------------------------
//!\file game_server.h
//!\brief ���������࣬LoginServer��LoongWorld��LoongDB���Ӹû���̳�
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_rt_s.h"

#include "game_server.h"
#include "section_mgr.h"


//-----------------------------------------------------------------------------
// GameServer���ࡪ��CONSTRUCT
//-----------------------------------------------------------------------------
GameServer::GameServer(DWORD dwID, LPCSTR szIP) : m_dwID(dwID), m_bValid(FALSE), m_pTran(NULL),
												  m_eStatus(EWS_InitNotDone), 
												  m_nPlayerNum(0),
												  m_nPlayerNumDisplay(0),
												  m_nMaxPlayerNum(0),
												  m_bTryingConnect(FALSE), m_szIP(szIP)
{
	//m_dwLastestTick = GetCurrentDWORDTime();

	// ��������Զ�˽��̵�����
	m_pTran = new StreamTransport;
	m_pTran->Init();

	TObjRef<VarContainer> pVar = "VarContainer";
	m_nPort = pVar->GetInt(_T("port rt_service"));

	m_pTran->TryToConnect(m_szIP.c_str(), m_nPort);
	m_bTryingConnect = TRUE;
}

GameServer::~GameServer()
{
	if( P_VALID(m_pTran) )
	{
		m_pTran->Destroy();
		SAFE_DEL(m_pTran);
	}
}

//-----------------------------------------------------------------------------
// GameServer���ࡪ������
//-----------------------------------------------------------------------------
VOID GameServer::Update()
{
	UpdateConnection();
	UpdateSession();
}
//-----------------------------------------------------------------------------
// GameServer���ࡪ����������
//-----------------------------------------------------------------------------
VOID GameServer::UpdateConnection()
{
	// ���������ӵ�״̬��
	if( m_pTran->IsConnected() )
	{
		// ����Ǹ������ϣ����͵�һ����Ϣ
		if( m_bTryingConnect )
		{
			m_bTryingConnect = FALSE;

			// ���͵�һ����Ϣ
			tagNLSS_LoginService send;
			send.eType = m_eType;
			m_pTran->Send(&send, sizeof(tagNLSS_LoginService));
		}
		
		//if(m_eStatus == EWS_InitNotDone)
		//{	
		//	tagDWORDTime dwNow;
		//	dwNow = GetCurrentDWORDTime();
		//	if (IncreaseTime( m_dwLastestTick, 60 * MAX_WAIT_FOR_TICK_M ) <= dwNow)
		//		// �������1����û���յ���server�������������ӻ��ڣ���ô����Ϊ��server����
		//		ChangeStatus(EWS_SystemError);
		//	else
		//		ChangeStatus(EWS_Well);
		//}

		if (m_eStatus == EWS_Well)
		{
			tagDWORDTime dwNow;
			dwNow = GetCurrentDWORDTime();
			
			if (IncreaseTime( m_dwLastestTick, 10 * MAX_WAIT_FOR_TICK_M ) <= dwNow)
				// �������10����û���յ���server�������������ӻ��ڣ���ô����Ϊ��server����
				ChangeStatus(EWS_SystemError);
		}

		// todo���շ���������Ϣ
	}
	// ��ǰ��û�����ӵ�״̬
	else
	{
		tagDWORDTime dwNow;
		dwNow = GetCurrentDWORDTime();
		
		// �������10����û���յ���server�����������������Ѿ����ˣ���ô����Ϊ��server����δ��ʼ��״̬
		if (IncreaseTime( m_dwLastestTick, 10 * MAX_WAIT_FOR_TICK_M ) <= dwNow)
			ChangeStatus(EWS_InitNotDone);

		// �����ǰ�������ӣ���һֱ��������
		if( m_bTryingConnect )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_szIP.c_str(), m_nPort);
			}
		}
		// ��������;�ϵ��ˣ���������
		else
		{
			m_pTran->TryToConnect(m_szIP.c_str(), m_nPort);
			m_bTryingConnect = TRUE;
		}
	}
}
//-----------------------------------------------------------------------------
// GameServer���ࡪ������������Ϣ
//-----------------------------------------------------------------------------
VOID GameServer::UpdateSession()
{
	LPBYTE pMsg = NULL;
	DWORD dwSize = 0;
	INT nUnRecved = 0;

	pMsg = g_SectionMgr.RecvMsg(m_dwID, dwSize, nUnRecved);

	while( P_VALID(pMsg) )
	{
		if ( m_eStatus != EWS_Well)
			m_eStatus = EWS_Well;

		g_SectionMgr.HandleCmd(pMsg, dwSize, m_dwID);
		g_SectionMgr.ReturnMsg(m_dwID, pMsg);

		pMsg = g_SectionMgr.RecvMsg(m_dwID, dwSize, nUnRecved);
		//return;
	}
}
//-----------------------------------------------------------------------------
// GameServer���ࡪ�������������ص�
//-----------------------------------------------------------------------------
BOOL GameServer::ServerStart()
{
	if( IsValid() ) return FALSE;

	m_bValid = TRUE;
	ChangeStatus(EWS_InitNotDone);
	m_nPlayerNum	= 0;
	m_nMaxPlayerNum	= 0;
	m_nPlayerNumDisplay = 0;

	return TRUE;
}
//-----------------------------------------------------------------------------
// GameServer���ࡪ���������رջص�
//-----------------------------------------------------------------------------
BOOL GameServer::ServerShutDown()
{
	m_bValid		= FALSE;
	ChangeStatus(EWS_InitNotDone);
	m_nPlayerNum	= 0;
	m_nMaxPlayerNum	= 0;
	m_nPlayerNumDisplay = 0;
	return TRUE;
}

//-----------------------------------------------------------------------------
// GameServer���ࡪ��ˢ��״̬
//-----------------------------------------------------------------------------
VOID GameServer::RefreshStatus(EWorldStatus eStatus, INT nOnlineNum, INT nMaxOnlineNum, INT nOnlineNumDisplay)
{
	m_nPlayerNum = nOnlineNum;
	m_nMaxPlayerNum = nMaxOnlineNum;
	m_nPlayerNumDisplay = nOnlineNumDisplay;
	ChangeStatus(eStatus);
}

VOID GameServer::ChangeStatus(EWorldStatus eStatus)
{
	if (m_eStatus != eStatus)
	{
		g_SectionMgr.SendDynamicInfo();
	}
	m_eStatus = eStatus;
}

//-----------------------------------------------------------------------------
// GameServer���ࡪ������������
//-----------------------------------------------------------------------------
BOOL GameServer::StartServer()
{
	if( !m_pTran->IsConnected() ) return FALSE;

	ChangeStatus(EWS_InitNotDone);
	m_nPlayerNum	= 0;
	m_nMaxPlayerNum	= 0;
	m_nPlayerNumDisplay = 0;

	// ���Ϳ�������Ϣ
	tagNLSS_Start send;
	m_pTran->Send(&send, sizeof(tagNLSS_Start));

	return TRUE;
}
//-----------------------------------------------------------------------------
// GameServer���ࡪ���رշ�����
//-----------------------------------------------------------------------------
BOOL GameServer::ShutdownServer()
{
	if( !m_pTran->IsConnected() ) return FALSE;

	ChangeStatus(EWS_InitNotDone);
	m_nPlayerNum	= 0;
	m_nMaxPlayerNum	= 0;
	m_nPlayerNumDisplay = 0;

	// ����ǿ�ƹط���������Ϣ
	tagNLSS_Close send;
	m_pTran->Send(&send, sizeof(tagNLSS_Close));

	return TRUE;
}

//-----------------------------------------------------------------------------
// GameServer���ࡪ��ִ��ϵͳ����
//-----------------------------------------------------------------------------
BOOL GameServer::ExecuteCmd(tagNetCmd* pMsg)
{
	// ���ͼ�¼��ϵͳ�������Ϣ
	m_pTran->Send((tagNLSS_SystemCmd*)pMsg, sizeof(tagNLSS_SystemCmd));

	return TRUE;
}

//--------------------------------------------------------------------------------
// GameServerLogin����CONSTRUCT
//--------------------------------------------------------------------------------
GameServerLogin::GameServerLogin(DWORD dwID, LPCSTR szIP)
: GameServer(dwID, szIP)
{
	m_eType = EST_Login;
}

//--------------------------------------------------------------------------------
// GameServerWorld����CONSTRUCT
//--------------------------------------------------------------------------------
GameServerWorld::GameServerWorld(DWORD dwID, LPCSTR szIP)
: GameServer(dwID, szIP)
{
	m_eType = EST_World;
}

DWORD GameServerWorld::SetDoubleRate( DWORD dwLastTime, DWORD dwOpenTime, DWORD dwRatio, DWORD dwDType )
{
	if(!P_VALID(m_pTran))		return E_GMTool_Err;

	tagNSS_Double send;	
	send.dwLastTime	= dwLastTime;
	send.dwOpenTime	= dwOpenTime;
	send.dwRatio	= dwRatio;
	send.eDType		= (EDoubleType)dwDType;

	g_SectionMgr.SendMsg(GetID(), &send, send.dwSize);
	
	return E_GMTool_Success;
}
//--------------------------------------------------------------------------------
// GameServerDB����CONSTRUCT
//--------------------------------------------------------------------------------
GameServerDB::GameServerDB(DWORD dwID, LPCSTR szIP)
: GameServer(dwID, szIP)
{
	m_eType = EST_DB;
}