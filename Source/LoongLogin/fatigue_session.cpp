#include "StdAfx.h"
#include "fatigue_session.h"
#include "../ServerDefine/msg_fatigue.h"
#include "../ServerDefine/base_define.h"
#include "login_server.h"
#include "player_mgr.h"
#include "fatigue_mgr.h"
#include "db.h"

BOOL FatigueSession::Init()
{
	m_pUtil		=	"Util";
	m_pThread	=	"Thread";

	// ��ȡ�����ļ�
	if( !InitConfig() )
	{
		return FALSE;
	}

	// ��ʼ������
	CreateObj("StreamTransportFatigueSession", "StreamTransport");
	m_pTransport = "StreamTransportFatigueSession";
	if( !P_VALID(m_pTransport) ) return FALSE;
	m_pTransport->Init();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("FatigueSessionConnectServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&FatigueSession::ThreadConnectServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("FatigueSessionConnectServer")))
	{
		continue;
	}

	return TRUE;
}

VOID FatigueSession::Destroy()
{
	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("FatigueSessionConnectServer"), INFINITE);

	// ��������
	m_pTransport->Destroy();
	KillObj("StreamTransportProofPolicyOwn");
}

VOID FatigueSession::ThreadConnectServer()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTerminateConnect )
	{
		if( !m_pTransport->IsConnected() )
		{
			if( !m_pTransport->IsTryingToConnect() )
			{
				m_pTransport->TryToConnect(m_szIP, m_nPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to fatigue server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szIP), m_nPort);

		// ���ͳ�ʼ��Ϣ
		tagNC_LOGIN_CM send;
		send.dwSectionID = g_login.GetSectionID();
		m_pTransport->Send(&send, send.dwSize);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);

}

BOOL FatigueSession::InitConfig()
{
	// ���������ļ�
	CreateObj("FatigueSessionVar", "VarContainer");
	TObjRef<VarContainer> pVar = "FatigueSessionVar";

	if( !P_VALID(pVar) ) return FALSE;

	TObjRef<Util> pUtil = "Util";
	TCHAR tszPath[MAX_PATH];
	ZeroMemory(tszPath, sizeof(tszPath));
	if (!pUtil->GetIniPath(tszPath, _T("server_config\\login\\login"))||
		!pVar->Load("VirtualFileSys", tszPath))
	{
		ERR(_T("�����ļ�δ�ҵ�"));
		return FALSE;
	}


	// ��ȡIP�Ͷ˿�
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip fatigue_guard")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	m_nPort = pVar->GetInt(_T("port fatigue_guard"));

	// ɾ�������ļ�����
	KillObj("FatigueSessionVar");

	return TRUE;
}

VOID FatigueSession::Update()
{
	if (!g_login.IsEnableFatigueGuard())
		return;

	if( NULL == m_pTransport )
		return;

	// ������
	if(!m_pTransport->IsConnected() && !m_pThread->IsThreadActive(_T("FatigueSessionConnectServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateConnect, TRUE);
		InterlockedExchange((LONG*)&m_bConnected, FALSE);
		m_pTransport->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("FatigueSessionConnectServer"), INFINITE);

		// �����������ݿ�����������߳�
		InterlockedExchange((LONG*)&m_bTerminateConnect, FALSE);
		m_pThread->CreateThread(_T("FatigueSessionConnectServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&FatigueSession::ThreadConnectServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("FatigueSessionConnectServer")))
		{
			continue;
		}

		return;
	}

	// �����������ļ������������Ϣ
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransport->Recv(dwSize);

	while( P_VALID(pRecv) )
	{
		tagNetCmd* pCmd = (tagNetCmd*)pRecv;
		if(pCmd->dwID == m_pUtil->Crc32("NS_ANTIENTHRALL"))
			HandleGuardAccountNotify(pRecv);

		m_pTransport->FreeRecved(pRecv);

		// ��ȡ��һ����Ϣ
		pRecv = m_pTransport->Recv(dwSize);
	}
}

VOID FatigueSession::SendQueryStatus(DWORD dwAccount)
{
	if (!g_login.IsEnableFatigueGuard())	return;

	const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(dwAccount);
	if (P_VALID(pAccountData))
	{
		tagNC_ANTIQUERYSTATE send;
		memcpy(send.szAccountName, pAccountData->szAccountName, 16);
		m_pTransport->Send(&send, send.dwSize);
	}
}

VOID FatigueSession::SendOLGuardAccounts( DWORD *pOLAccounts, DWORD dwNum )
{
	if (!g_login.IsEnableFatigueGuard())	return;

	if (dwNum < 0)
		return;

	if (dwNum == 0)
	{
		tagNC_ANTIENTHRALL send;
		send.dwAccountNum = 0;
		// ������Ϣ
		m_pTransport->Send(&send, send.dwSize);

		return;
	}

	DWORD dwMsgSize = sizeof(tagNC_ANTIENTHRALL) + dwNum * 16 * sizeof(CHAR) - sizeof(CHAR);

	BYTE* pByte = new BYTE[dwMsgSize];
	tagNC_ANTIENTHRALL* pSend = (tagNC_ANTIENTHRALL*)pByte;
	pSend->dwID = m_pUtil->Crc32("NC_ANTIENTHRALL");
	pSend->dwSize = dwMsgSize;

	pSend->dwAccountNum = 0;
	for (int i=0; i<(INT)dwNum; ++i)
	{
		const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(pOLAccounts[i]);
		if (!P_VALID(pAccountData))
		{
			continue;
		}

		LPSTR szName = pSend->szAccountName + 16 * pSend->dwAccountNum;
		memcpy(szName, pAccountData->szAccountName, sizeof(CHAR) * 16);
		++pSend->dwAccountNum;
	}

	// ������Ϣ
	m_pTransport->Send(pSend, pSend->dwSize);

	SAFE_DEL_ARRAY(pSend);
}

DWORD FatigueSession::HandleGuardAccountNotify( PVOID pMsg )
{
	if (!g_login.IsEnableFatigueGuard())	return E_Success;

	MTRANS_POINTER(pRecv, pMsg, tagNS_ANTIENTHRALL);

	tagFatigueNotify Notify;
	Notify.dwAccountID = sPlayerMgr.GetAccountIDByAccountName(pRecv->szAccountName);
	Notify.dwState = (DWORD)pRecv->byColor - 1;
	Notify.dwAccOnTimeMin = pRecv->dwAccOLSeconds / 60;

	if (g_login.IsEnableFatigueGuard())
		sFatigueMgr.NotifyPlayer(&Notify);

	return 0;
}

