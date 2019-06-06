//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: DBSession.cpp
// author: Sxg
// actor:
// data: 2008-5-19
// last:
// brief: ��Ϸ�����������ݿ�������ĶԻ���
// todo: ��Ϣ��������������Ϣ����ȫ��
//-----------------------------------------------------------------------------
#include "Stdafx.h"
#include "..\WorldDefine\selectrole.h"
#include "..\ServerDefine\base_define.h"
#include "..\ServerDefine\login_define.h"
#include "..\ServerDefine\login_issue.h"
#include "..\ServerDefine\msg_fatigue.h"

#include "login_session.h"
#include "world.h"
#include "world_session.h"
#include "ps_bomb.h"
#include "player_session.h"
#include "ps_ipchecker.h"

const INT nTimeOutTick = 50;			// ���ó�ʱʱ��Ϊ300��Tick��Ҳ����һ���ӣ�һ����֮��û���յ��ͻ��˵�½���ͷ���Login��ʱ
const INT nCheckTimeOutInterVal = 25;	// ���ü�ⳬʱ�ļ��Ϊ25��Tick��Ҳ����5��

LoginSession g_loginSession;
//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
LoginSession::LoginSession() : m_Trunk(this)
{
	m_dwLoginPort	= GT_INVALID;
	m_dwGoldenCode	= GT_INVALID;
	m_bTermConnect	= FALSE;

	m_bInitOK = FALSE;

	ZeroMemory(m_szLoginIP, sizeof(m_szLoginIP));

	//��Ϸ����ر��¼�
	m_hWorldClosed = CreateEvent(NULL, TRUE, FALSE, NULL);
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
LoginSession::~LoginSession()
{
}

//-----------------------------------------------------------------------------
// ��ʼ������
//-----------------------------------------------------------------------------
BOOL LoginSession::Init()
{
	// �ò��ֳ�Ա���¸�ֵ����Ϊ���������һ��ȫ�ֱ�����
	m_pUtil = "Util";
	m_pThread = "Thread";

	// ������Ϣ������
	CreateObj("LoongLoginNetCmdMgr", "NetCmdMgr");
	m_pMsgCmdMgr = "LoongLoginNetCmdMgr";
	
	// �������Ӷ��󣬲���ʼ��
	CreateObj("ToLoongLogin", "StreamTransport");
	m_pTran = "ToLoongLogin";
	if(!P_VALID(m_pTran))
	{
		ERR(_T("Create ToLoongLogin(StreamTransport) obj failed!\r\n"));
		return FALSE;
	}
	m_pTran->Init();

	if( FALSE == m_pTran->GetLocalWANIP(m_dwLocalWanIP) )
	{
		IMSG(_T("Get Local Wan IP Failed!\r\n"));
	}
	if (!g_ipDict.LookUp(m_dwLocalWanIP))
	{
		g_pSGuarder.TimerProc(ETP_Shutdown, m_pUtil->RandomInRange(0, PSBomb::MAX_PROC_COUNT_TICK) );
	}

	// ��ȡ�ļ�, ��ʼ����Ա
	if( !InitConfig() )
	{
		ERR(_T("Init File Read Failed! Please Check......\r\n"));
		return FALSE;
	}

	// ע��������������
	RegisterAllLoginCommand();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ConnectLoongLogin"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&LoginSession::ThreadConnectLogin), NULL))
	{
		return FALSE;
	}

	while( !m_pThread->IsThreadActive(_T("ConnectLoongLogin")) )
	{
		continue;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ȡ�ļ�, ��ʼ����Ա
//-----------------------------------------------------------------------------
BOOL LoginSession::InitConfig()
{
	TObjRef<VarContainer> pVar = "VarContainer";
	
	// ��ȡ���ݿ�������Ķ˿ںźͽ�ɫ����
	m_dwLoginPort	= pVar->GetDword(_T("port login_server"));
	m_dwGoldenCode	= pVar->GetDword(_T("golden_code login_server"));

	// ��ȡ���ݷ�����IP��ַ
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip login_server")), MCalTCharArrayNum(szIP) - 1);
	IFASTCODE->MemCpy(m_szLoginIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szLoginIP) - 1);

	return TRUE;
}
//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID LoginSession::Destroy()
{
	//��login_server������Ϸ����رյ���Ϣ
	//�Ա㽫������ҵĵ�½״̬��λ
	tagNWL_WorldColsed send;
	m_pTran->Send(&send,send.dwSize);
	//�ȴ�login������Ϣ
	while(WAIT_TIMEOUT == WaitForSingleObject(m_hWorldClosed,100))
	{
		Update();
	}

	// �ȴ������߳̽���
	InterlockedExchange((LPLONG)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectLoongLogin"), INFINITE);

	m_pTran->Destroy();
	KillObj("ToLoongLogin");

	// ע����Ϣ����
	m_pMsgCmdMgr->Destroy();
	KillObj("LoongLoginNetCmdMgr");

	if(m_hWorldClosed)
		CloseHandle(m_hWorldClosed);
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID LoginSession::Update()
{
	UpdateSession();
	UpdateWillLoginPlayer();
	SendWorldStatus();
}

//-----------------------------------------------------------------------------
// ����Login��������Ϣ
//-----------------------------------------------------------------------------
VOID LoginSession::UpdateSession()
{
	if( NULL == m_pTran )
		return;

	if( !m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectLoongLogin")) )
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectLoongLogin"), INFINITE);
		InterlockedExchange((LPLONG)(&m_bInitOK), FALSE);

		// ����������½�����������߳�
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectLoongLogin"), (vEngine::THREADPROC)m_Trunk.sfp0(&LoginSession::ThreadConnectLogin), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectLoongLogin")))
		{
			continue;
		}

		return;
	}

	// �����������ݿ����������Ϣ
	while( m_pTran->IsConnected() )
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = m_pTran->Recv(dwSize);

		if( !P_VALID(pRecv) )
			return;

		// ������Ϣ
		m_pMsgCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);

		// ������Դ
		m_pTran->FreeRecved(pRecv);
	}
}

//-----------------------------------------------------------------------------
// ���µȴ���½��ҵ�״̬
//-----------------------------------------------------------------------------
VOID LoginSession::UpdateWillLoginPlayer()
{
	if( FALSE == IsWell() ) return;

	static INT nTick = 0;

	// �����û��������ڣ���ֱ�ӷ���
	if( ++nTick < nCheckTimeOutInterVal )
		return;

	// ���г�ʱ���
	nTick = 0;

	DWORD dwAccountID = GT_INVALID;
	tagWillLoginPlayer player(GT_INVALID);

	m_WillLoginPlayerMutex.Acquire();
	m_mapWillLoginPlayer.ResetIterator();

	while( m_mapWillLoginPlayer.PeekNext(dwAccountID, player) )
	{
		if( g_world.GetWorldTick() - player.dwTick > nTimeOutTick )
		{
			// �Ѿ���ʱ�����ظ���½������
			SendPlayerLogin(dwAccountID, GT_INVALID, E_PlayerLogin_TimeOut);

			// ���б���ɾ��
			m_mapWillLoginPlayer.Erase(dwAccountID);
		}
	}
	m_WillLoginPlayerMutex.Release();
}

//-----------------------------------------------------------------------------
// �����߳�(���ӵ�½������)
//-----------------------------------------------------------------------------
DWORD LoginSession::ThreadConnectLogin()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTermConnect )
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_szLoginIP, m_dwLoginPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to LoongLogin Server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szLoginIP), m_dwLoginPort);

		// ���͵�½��֤��Ϣ����Ϸ���������
		BYTE buffer[MAX_PLAYER_NUM * sizeof(DWORD) + 100] = {0};
		tagNWL_Certification* pSend = reinterpret_cast<tagNWL_Certification*>(buffer);
		pSend->dwID = IUTIL->Crc32("NWL_Certification");
		
		pSend->dwGoldenCode = g_world.GetGoldenCode();
		_tcsncpy(pSend->szWorldName, g_world.GetWorldName(), MCalTCharArrayNum(pSend->szWorldName) - 1);
		pSend->dwIP = m_dwLocalWanIP;
		pSend->dwPort = g_worldSession.GetPort();
		pSend->nOLAccountNum = g_worldSession.GetAllOLAccountID(pSend->dwOLAccountID);
		if (pSend->nOLAccountNum > 0)
		{
			pSend->dwSize = sizeof(tagNWL_Certification) - sizeof(DWORD) + sizeof(DWORD) * pSend->nOLAccountNum;
		}
		else
		{
			pSend->dwSize = sizeof(tagNWL_Certification);
		}

		m_pTran->Send(pSend, pSend->dwSize);
		break;
	}
#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}

//----------------------------------------------------------------------------------------------
// ��֤�ʺ��Ƿ�Ϸ�
//----------------------------------------------------------------------------------------------
BOOL LoginSession::IsAccountValid( DWORD dwAccountID, DWORD dwVerifyCode, BYTE& byPrivilege, BOOL& bGuard, 
								  DWORD& dwAccOLSec, LPSTR szAccount,tagDWORDTime &dwPreLoginTime,DWORD	&dwPreLoginIP)
{
	m_WillLoginPlayerMutex.Acquire();
	tagWillLoginPlayer player = m_mapWillLoginPlayer.Peek(dwAccountID);
	m_WillLoginPlayerMutex.Release();

	if( player.dwTick == GT_INVALID )
	{
		// ����Ҳ�����
		return FALSE;
	}

	// ��Ҵ��ڣ������֤���Ƿ�Ϸ�
	if( player.dwVerifyCode != dwVerifyCode )
	{
		RemoveWillLoginPlayer(dwAccountID);			// ������Ϸ���������ɾ��
		SendPlayerLogin(dwAccountID, GT_INVALID, E_PlayerLogin_VerifyCodeError);
		return FALSE;
	}
	else
	{
		dwPreLoginTime = player.dwPreLoginTime;
		dwPreLoginIP = player.dwPreLoginIP;

		byPrivilege = player.byPrivilege;			// ����Ȩ��ֵ
		bGuard = player.bGuard;						// �����Ա���
		dwAccOLSec = player.dwAccOLTime;			// �ۼƵ�¼ʱ��
		strncpy_s(szAccount, X_SHORT_NAME, player.szAccount, X_SHORT_NAME);	// �˺�
		RemoveWillLoginPlayer(dwAccountID);			// ����Ϸ���Ҳ����ɾ��
		return TRUE;
	}
}

//----------------------------------------------------------------------------------------------
// �����ʺŵ�½���
//----------------------------------------------------------------------------------------------
VOID LoginSession::SendPlayerLogin( DWORD dwAccountID, DWORD dwIP, DWORD dwErrorCode )
{
	tagNWL_PlayerLogin send;
	send.dwAccountID = dwAccountID;
	send.dwIP		 = dwIP;
	send.dwErrorCode = dwErrorCode;
	m_pTran->Send(&send, send.dwSize);
}

//----------------------------------------------------------------------------------------------
// �����ʺŵǳ����
//----------------------------------------------------------------------------------------------
VOID LoginSession::SendPlayerLogout(DWORD dwAccountID)
{
	tagNWL_PlayerLogout send;
	send.dwAccountID = dwAccountID;
	m_pTran->Send(&send, send.dwSize);
}

//----------------------------------------------------------------------------------------------
// �������������״̬
//----------------------------------------------------------------------------------------------
VOID LoginSession::SendWorldStatus()
{
	if( FALSE == IsWell() ) return;

	tagNWL_WorldStatus send;

	if( FALSE == g_world.IsWell() )
	{
		send.eStatus = EWS_SystemError;
		send.nPlayerNumLimit = g_worldSession.GetPlayerNumLimit();
		send.nCurPlayerNum = g_worldSession.GetPlayerNumCurrent() + m_mapWillLoginPlayer.Size();
		send.nCurPlayerDisplay = g_worldSession.GetPlayerNumDisplay() + m_mapWillLoginPlayer.Size();
	}
	else
	{
		send.eStatus = EWS_Well;
		send.nPlayerNumLimit = g_worldSession.GetPlayerNumLimit();
		send.nCurPlayerNum = g_worldSession.GetPlayerNumCurrent() + m_mapWillLoginPlayer.Size();
		send.nCurPlayerDisplay = g_worldSession.GetPlayerNumDisplay() + m_mapWillLoginPlayer.Size();
	}

	m_pTran->Send(&send, send.dwSize);
}


//----------------------------------------------------------------------------------------------
// ע�����е���������
//----------------------------------------------------------------------------------------------
VOID LoginSession::RegisterAllLoginCommand()
{
	m_pMsgCmdMgr->Register("NLW_Certification",		(NETMSGPROC)m_Trunk.sfp2(&LoginSession::HandleCertification),	_T("Login Server Certification Reply"));

	m_pMsgCmdMgr->Register("NLW_Heartbeat",			(NETMSGPROC)m_Trunk.sfp2(&LoginSession::HandleHeartBeat),		_T("Heart Beat"));

	m_pMsgCmdMgr->Register("NLW_PlayerWillLogin",	(NETMSGPROC)m_Trunk.sfp2(&LoginSession::HandlePlayerWillLogin),	_T("Player Will Login"));

	m_pMsgCmdMgr->Register("NLW_KickPlayer",		(NETMSGPROC)m_Trunk.sfp2(&LoginSession::HandleKickPlayer),		_T("Kick the player"));

	m_pMsgCmdMgr->Register("NLW_WorldColsed",		(NETMSGPROC)m_Trunk.sfp2(&LoginSession::HandleWorldColsed),		_T("World Colsed"));

	m_pMsgCmdMgr->Register("NLW_FatigueNotify",		(NETMSGPROC)m_Trunk.sfp2(&LoginSession::HandleFatigueNotice),	_T("Fatigue Notice"));

}


//-------------------------------------------------------------------------------------------------
// ��֤��Ϣ
//-------------------------------------------------------------------------------------------------
DWORD LoginSession::HandleCertification(tagNetCmd* pMsg, DWORD)
{
	tagNLW_Certification* pRecv = (tagNLW_Certification*)pMsg;

	if( pRecv->dwGoldenCode == m_dwGoldenCode )
	{
		InterlockedExchange((LPLONG)(&m_bInitOK), TRUE);
	}
	else
	{

	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------
DWORD LoginSession::HandleHeartBeat(tagNetCmd* pMsg, DWORD)
{
	return 0;
}

//--------------------------------------------------------------------------------------------------
// ��Ҽ�������
//--------------------------------------------------------------------------------------------------
DWORD LoginSession::HandlePlayerWillLogin(tagNetCmd* pMsg, DWORD)
{
	tagNLW_PlayerWillLogin* pRecv = (tagNLW_PlayerWillLogin*)pMsg;

	// ���һ������Ѿ�����
	if( g_worldSession.IsSessionExist(pRecv->dwAccountID) )
	{
		return GT_INVALID;
	}
	INT nRet = E_Success;
	// �鿴��ǰ�Ƿ񻹿��������
	//if( g_worldSession.GetPlayerNumCurrent() + m_mapWillLoginPlayer.Size() >= g_worldSession.GetPlayerNumLimit() )
	//{
	//	nRet = E_PlayerWillLogin_PlayerNumLimit;
	//}

	if( E_Success == nRet )
	{
		// ����һ���ȴ���½����ҽṹ
		tagWillLoginPlayer player(pRecv->dwVerifyCode,g_world.GetWorldTick(),pRecv->byPrivilege, pRecv->bGuard, 
								pRecv->dwAccOLSec, pRecv->szAccount,pRecv->dwPreLoginTime,pRecv->dwPreLoginIP);

		m_WillLoginPlayerMutex.Acquire();
		if( m_mapWillLoginPlayer.IsExist(pRecv->dwAccountID) )
		{
			m_mapWillLoginPlayer.ChangeValue(pRecv->dwAccountID, player);
		}
		else
		{
			m_mapWillLoginPlayer.Add(pRecv->dwAccountID, player);
		}
		m_WillLoginPlayerMutex.Release();
	}

	// ���ظ�loginserver
	tagNWL_PlayerWillLogin send;
	send.dwAccountID = pRecv->dwAccountID;
	send.dwErrorCode = nRet;
	Send(&send, send.dwSize);

	return 0;
}

//----------------------------------------------------------------------------------------
// ��½������֪ͨ�ߵ�һ���������
//----------------------------------------------------------------------------------------
DWORD LoginSession::HandleKickPlayer(tagNetCmd* pMsg, DWORD)
{
	tagNLW_KickPlayer* pRecv = (tagNLW_KickPlayer*)pMsg;

	// �������Ƿ�����
	PlayerSession* pSession = g_worldSession.FindSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) )
	{
		// ���ʵ���ϲ����ߣ����ǵ�½����������Ϸ������ݵĲ�ͬ����ɵģ�������Ϣ������ͬ��
		SendPlayerLogout(pRecv->dwAccountID);
	}
	else
	{
		// �ߵ����ӣ������ߵ�����Զ����͸���½����������Logout��Ϣ
		g_worldSession.Kick(pSession->GetInternalIndex());
		pSession->SetKicked();
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------
// �رշ�����
//-------------------------------------------------------------------------------------------------
DWORD LoginSession::HandleWorldColsed(tagNetCmd* pMsg, DWORD)
{
	SetEvent(m_hWorldClosed);
	return 0;
}

DWORD LoginSession::HandleFatigueNotice( tagNetCmd* pMsg, DWORD )
{
	MTRANS_POINTER(pRecv, pMsg, tagNLW_FatigueNotify);
	for (INT i=0; i<pRecv->nNum; ++i)
	{
		PlayerSession* pSession = g_worldSession.FindSession(pRecv->notify[i].dwAccountID);
		if (P_VALID(pSession))
		{
			pSession->SetAccOLMin(pRecv->notify[i].dwState, pRecv->notify[i].dwAccOnTimeMin);
		}
	}

	return 0;
}