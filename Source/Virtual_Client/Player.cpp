#include "stdafx.h"
#include "Player.h"
#include "ClientMgr.h"

DWORD dwCreatureMINID = 2000000189;
DWORD dwCreatureMAXID = 2000000257;

Player::Player():m_Trunk(this)
{
	m_bTerminate = FALSE;
	m_bCanWalk = FALSE;
	m_bCanCombat = FALSE;
	m_bCanChat = FALSE;
	m_byChatChannel = 5;
	m_bLog = FALSE;
	m_desPos.x = 0.0f;
	m_desPos.y = 0.0f;
	m_desPos.z = 0.0f;
	m_srcPos.x = 0.0f;
	m_srcPos.y = 0.0f;
	m_srcPos.z = 0.0f;
	m_dwCreatureID = 2000000189;
	m_dwMsgCount = 0;
	m_type = 1;
	m_bBlock = FALSE;
	m_pFrameMgr = NULL;
	m_pLoginInfo = NULL;
	m_bAbandon = FALSE;
}

Player::~Player()
{
	Destroy();
}

BOOL Player::Init()
{
	 m_pThread = "Thread";
	 m_pUtil = "Util";
	 m_pVar = "VC_Config";

	 stringstream stream_loginIP;
	 stream_loginIP << "Client_LoginIP_" << m_dwID;
	 m_pLoginIPUtil = CreateObj(stream_loginIP.str().c_str(), "Util");

	 // ��ʼ����Ա����
	 m_strMapName	= m_pVar->GetString(_T("map way_point"));
	 m_bLog			= m_pVar->GetInt(_T("log virtual_client")) != 0 ? TRUE : FALSE;
	 m_strVersion	= m_pVar->GetString(_T("version world_server"));

	 m_it			= m_vecWayPoint.begin();

	 // �����ÿͻ���ר������
	 stringstream stream_login;
	 stream_login << "Client_Login_" << m_dwID;
	 m_pLoginSession = CreateObj(stream_login.str().c_str(), "XClient");

	 stringstream stream_world;
	 stream_world << "Client_World_" << m_dwID;
	 m_pWorldSession = CreateObj(stream_world.str().c_str(), "XClient");
	 m_pWorldSession->Init(FALSE);

	 // ������Ϣ������
	 stringstream stream_player;
	 stream_player << "Player_" << m_dwID;
	 m_pCmdMgr = CreateObj(stream_player.str().c_str(), "NetCmdMgr");
	 m_pCmdMgr->Init();

	 // �����߳���
	 m_pLock = new VCMutex;

	 // ����Frame������
	 m_pFrameMgr = new FrameMgr(this);
	 if (!P_VALID(m_pFrameMgr))
		return FALSE;

	 m_pFrameMgr->Init();

	 // ��ô�����Ϣ
	 if (!g_pClientMgr->GetRandomLoginServer(m_pLoginInfo))
		 return FALSE;

	 this->ChangeFrame(FrameMgr::EF_LOGIN);

	if (m_bLog)
	{
		//����Log
		stringstream ss;
		ss <<"Role_" << m_pUtil->UnicodeToUnicode8(m_strName.c_str());
		log_name = ss.str();

		CreateObj(log_name.c_str(), "Log");
		m_pLog = log_name.c_str();

		if (!P_VALID(m_pLog))
		{
			IMSG(_T("Log Open Faild!\n"));
			return FALSE;
		}

		string file_name = log_name + ".log";
		if (FALSE == m_pLog->Create(m_pUtil->AnsiToUnicode(file_name.c_str())))
		{
			// �ر�log
			m_pLog->Close();
			KillObj(log_name.c_str());
			return FALSE;
		}
	}

	//�������������߳�
	m_strThreadName = _T("Thread_");
	m_strThreadName += m_strName;
	m_pThread->CreateThread(m_strThreadName.c_str(), (vEngine::THREADPROC)m_Trunk.sfp0(&Player::ThreadUpdate), NULL);
	while( FALSE == m_pThread->IsThreadActive(m_strThreadName.c_str()) )
	{
		Sleep(10);
		continue;
	}

	return TRUE;
}

VOID Player::Destroy()
{
	if(m_bLog)	
	{
		// �ر�log
		m_pLog->Close();
		KillObj(log_name.c_str());
	}

	//�ر�Update�߳�
	//KillObj(m_pUtil->UnicodeToAnsi(m_strName.c_str()));
	InterlockedExchange( (LONG*)&m_bTerminate, TRUE );
	m_pThread->WaitForThreadDestroy( m_strThreadName.c_str(), INFINITE );

	SAFE_DEL(m_pFrameMgr);

	// ���ٸÿͻ���ר������
	stringstream stream_login;
	stream_login << "Client_Login_" << m_dwID;
	KillObj(stream_login.str().c_str());

	stringstream stream_world;
	stream_world << "Client_World_" << m_dwID;
	KillObj(stream_world.str().c_str());

	// ������Ϣ������
	stringstream stream_player;
	stream_player << "Player_" << m_dwID;
	KillObj(stream_player.str().c_str());

	stringstream stream_loginIP;
	stream_loginIP << "Client_LoginIP_" << m_dwID;
	KillObj(stream_loginIP.str().c_str());

	SAFE_DEL(m_pLock);
}

VOID Player::ThreadUpdate()
{
	while (!m_bTerminate)
	{
		DWORD dwTime = timeGetTime();
		if (P_VALID(m_pFrameMgr))
		{
			m_pFrameMgr->Update();
		}

		// ��ͣ��������Ϣ
		for(;;)
		{
			DWORD dwRecvSize = 0;
			LPBYTE pRecv = m_pWorldSession->Recv(dwRecvSize);
			if( !P_VALID(pRecv) )
				break;

			// �Ӵ���н��С�����δ���
			LPBYTE pRecvPointer = pRecv;
			DWORD dwMsgSize = 0;
			while( pRecvPointer < pRecv + dwRecvSize )
			{
				tagNetCmd* pRecvCmd = (tagNetCmd*)pRecvPointer;
				dwMsgSize = pRecvCmd->dwSize;
				
				Acquire();
				m_pCmdMgr->HandleCmd(pRecvCmd, dwMsgSize, 0);
				Release();

				pRecvPointer += dwMsgSize;
			}

			m_pWorldSession->FreeRecved(pRecv);
		}

		DWORD dwUsedTime = timeGetTime() - dwTime;
		if (dwUsedTime < 200)
		{
			Sleep(200 - dwUsedTime);
		}
	}
}

BOOL Player::ChangeFrame( FrameMgr::EFrame eFrame )
{
	if (!P_VALID(m_pFrameMgr))
		return FALSE;

	return m_pFrameMgr->ChangeFrame(eFrame);
}

VOID Player::SetPlayerChat( BOOL bChat, BYTE byChannel /*= 5*/ )
{
	m_bCanChat = bChat;
	m_byChatChannel = byChannel;
}

VOID Player::SetPlayerMove( BOOL bMove )
{
	m_bCanWalk = bMove;
}

VOID Player::GetNextWayPoint()
{
	m_it++;
	if (m_it == m_vecWayPoint.end())
	{
		m_it = m_vecWayPoint.begin();
	}

	m_srcPos = m_desPos;
	m_desPos = (*m_it);
}