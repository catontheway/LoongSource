//-----------------------------------------------------------------------------
//!\file to_rt_session.cpp
//!\author Lyp
//!
//!\date 2005-04-05
//! last 2005-04-05
//!
//!\brief �����������������������Ʒ���ĶԻ���
//!
//!	Copyright (c) 2005 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_rt_s.h"

#include "rt_session.h"
#include "Loong.h"

//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
RTSession::RTSession() : m_Trunk(this)
{
	m_bInitOK					=	FALSE;
	m_bTermConnect				=	FALSE;
	//m_nSendInfoTickCountDown	=	SEND_INFO_INTERVAL;
	m_pLoong					=	NULL;
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
RTSession::~RTSession()
{
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL RTSession::Init(CLoong *pLoong)
{
	m_pThread		=	"Thread";
	m_pMsgCmdMgr	=	"NetCmdMgr";
	m_pThread		=	"Thread";
	m_pUtil			=	"Util";
	m_pLoong		=	pLoong;

	TObjRef<VarContainer> pVar = "LoongVar";
	TCHAR tszPath[MAX_PATH];
	ZeroMemory(tszPath, sizeof(tszPath));
	if (!m_pUtil->GetIniPath(tszPath, _T("server_config/db/world"))||
		!pVar->Load("VirtualFileSys", tszPath))
	{
		ERR(_T("�����ļ�δ�ҵ�"));
		return FALSE;
	}

	m_strIP			= pVar->GetString(_T("ip gm_server"));
	m_dwPort		= pVar->GetDword(_T("port gm_server"));
	m_dwSectionID	= pVar->GetDword(_T("id section"));
	m_dwWorldID		= pVar->GetDword(_T("id world"));

	pVar->Clear();

	// �������Ӷ��󣬲���ʼ��
	CreateObj("ToRT", "StreamTransport");
	m_pTran = "ToRT";
	if( !P_VALID(m_pTran) )
	{
		ERR(_T("Create ToRT(StreamTransport) obj failed!\r\n"));
		return FALSE;
	}
	m_pTran->Init();

	// ע��������������
	RegisterAllRTCommand();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ConnectRT"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&RTSession::ThreadConnectRT), NULL))
	{
		return FALSE;
	}

	while( !m_pThread->IsThreadActive(_T("ConnectRT")) )
	{
		continue;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID RTSession::Destroy()
{
	// �ȴ������߳̽���
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectRT"), INFINITE);

	m_pTran->Destroy();
	KillObj("ToRT");
}



//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID RTSession::Update()
{
	UpdateSession();
	SendServerInfo();
}

//-----------------------------------------------------------------------------
// ������Ϣ
//-----------------------------------------------------------------------------
VOID RTSession::UpdateSession()
{
	if( NULL == m_pTran )
		return;

	if( !m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectRT")) )
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectRT"), INFINITE);

		// ����������½�����������߳�
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectRT"), (vEngine::THREADPROC)m_Trunk.sfp0(&RTSession::ThreadConnectRT), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectRT")))
		{
			continue;
		}

		return;
	}

	while(TRUE)
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = m_pTran->Recv(dwSize);
		if( !P_VALID(pRecv) )
			break;

		// ������Ϣ
		m_pMsgCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);

		// ������Դ
		m_pTran->FreeRecved(pRecv);

		
	}	
}

//-----------------------------------------------------------------------------
// ���ͷ��������
//-----------------------------------------------------------------------------
VOID RTSession::SendServerInfo()
{
	if( !m_pTran->IsConnected() || !m_bInitOK ) return;

	if (!P_VALID(m_pLoong))
		return;

	//if( --m_nSendInfoTickCountDown > 0 )
	//	return;

	//m_nSendInfoTickCountDown = SEND_INFO_INTERVAL;

	tagNSC_WorldInfo send;
	
	const CLoong::tagStatus *pLoonState = m_pLoong->GetStatus();


	if( m_pLoong->IsConnect() )
	{
		send.eStatus = EWS_Well;
		
		if((pLoonState->dwRecvMsgNum - pLoonState->dwProcMsgNum) > 5000)
		{
			send.eStatus = EWS_DBBlock;
		}
	}
	else
	{
		send.eStatus = EWS_SystemError;
	}
	send.nMaxOnlineNum = GT_INVALID;
	send.nOnlineNum = GT_INVALID;

	m_pTran->Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// �����߳�(���Ӽ�ط�����)
//-----------------------------------------------------------------------------
DWORD RTSession::ThreadConnectRT()
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
				m_pTran->TryToConnect(m_pUtil->UnicodeToUnicode8(m_strIP.c_str()), m_dwPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to RT Server at %s: %d\r\n"), m_strIP.c_str(), m_dwPort);

		tagNSC_LoginService send;
		send.dwSectionID	=	m_dwSectionID;
		send.dwWorldID		=	m_dwWorldID;
		send.eType			=	EST_DB;
		m_pTran->Send(&send, send.dwSize);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}


//----------------------------------------------------------------------------------------------
// ע�����е���������
//----------------------------------------------------------------------------------------------
VOID RTSession::RegisterAllRTCommand()
{
	m_pMsgCmdMgr->Register("NSS_LoginService",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleServerLogin),	_T("RT Server Certification Reply"));
	m_pMsgCmdMgr->Register("NSS_Close",			(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleCloseServer),	_T("Close Server"));
}

//----------------------------------------------------------------------------------------------
// ��������֤������Ϣ
//----------------------------------------------------------------------------------------------
DWORD RTSession::HandleServerLogin(tagNetCmd* pMsg, DWORD)
{
	tagNSS_LoginService* pRecv = (tagNSS_LoginService*)pMsg;

	InterlockedExchange((LPLONG)&m_bInitOK, TRUE);
	return 0;
}

//-----------------------------------------------------------------------------------------------
// �������ر�������Ϣ
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleCloseServer(tagNetCmd* pMsg, DWORD)
{
	if(P_VALID(m_pLoong))
	{
		m_pLoong->ShutDown();
		m_pLoong = NULL;
	}
	
	return 0;
}

RTSession g_rtSession;
