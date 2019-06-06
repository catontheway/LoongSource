//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: �����ܶ���ʵ��
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_leftmsg.h"
#include "../WorldDefine/svn_revision.h"
#include "LoongDB.h"
#include "Loong.h"
#include "rt_session.h"
#include "TimeChecker.h"
//-------------------------------------------------------------------------------------------------------
//constructor
//-------------------------------------------------------------------------------------------------------
CLoong::CLoong()
{
	m_bTerminate			= FALSE;
	m_bGameServerLogout		= FALSE;
	m_bGameServerConnect	= FALSE;
	m_pDB					= new CLoongDB;
	m_dwLogTime				= 0;
	m_szLogFilePostfix[0]	= '\0';

	// ��ʼ���ַ���
	ZeroMemory(m_szWorldName, sizeof(m_szWorldName));
	//ZeroMemory(m_szServerName, sizeof(m_szServerName));

	// ��ʼ��������Ϣ(��Ҫ��m_pLoadSimRole,m_pCreateRole��m_pLoadRoleʹ��ZeroMemory)
	m_pLoadSimRole = new tagNDBS_LoadSimRole;	// ����ĳ���ʺ��µĻ�����ɫ��Ϣ
	m_pCreateRole = new tagNDBS_CreateRole;		// �����´����Ľ�ɫ�Ļ�����Ϣ
	m_pLoadBaiBao = new tagNDBS_LoadBaiBao;		// ������ȡ�İٱ�����Ʒ��Ϣ

	m_pLoadBaiBaoLog = (tagNDBS_LoadBaiBaoLog*)m_pBuffer;	// �������ṹ�����ȡ��ɫ����buffer(���߳�ʱ��������Σ��)

	// �ýṹ(m_pLoadRole)��С����,�ʵ�������
	m_pLoadRole = (tagNDBS_LoadRole *)m_pBuffer;

	// �汾��Ϣ(�ڸô���ʼ�������ٰ汾�仯Ӱ��)
	m_Status.dwVerLoongDB		= LOONGDB_BUILD_REVISION;
	m_Status.dwVerBeton			= BETON_BUILD_REVISION;
	m_Status.dwVerVEngine		= VENGINE_BUILD_REVISION;
	m_Status.dwVerServerDefine	= SERVERDEFINE_BUILD_REVISION;
	m_Status.dwVerXML			= XMLREADER_BUILD_REVISION;

	// ��ʼ����Ϣ-������������
	m_MsgCmdMgr.Init();
}

//-------------------------------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------------------------------
CLoong::~CLoong()
{
	// �ر�����Ӧ�ò㿪�����߳�
	InterlockedExchange((long*)(&m_bTerminate), TRUE);

	g_rtSession.Destroy();
	m_pThread->WaitForAllThreadDestroy();

	m_MsgCmdMgr.Destroy();

	// �����ڴ�
	SAFE_DEL(m_pCreateRole);
	SAFE_DEL(m_pLoadSimRole);
	SAFE_DEL(m_pLoadBaiBao);

	SAFE_DEL(m_pDB);

	// ���ٴ��ڼ�GUI��ϵͳ
	m_pWindow->Destroy();
	m_pConsole->Destroy();
	m_pGUI->Destroy();
	m_pRender->Destroy();
}
//-------------------------------------------------------------------------------------------------------
// ע�����̨����
//-------------------------------------------------------------------------------------------------------
VOID CLoong::RegisterTestCmd(TObjRef<Console> pConsole)
{
	pConsole->Register(_T("dbcheck"), m_Trunk.sfp0(&CLoong::TestDBReadFunction), _T("check the db read funcation"), 0);

}
//-------------------------------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------------------------------
BOOL CLoong::Init(HINSTANCE hInst)
{
	// ��ʼ������
	InitConfig();

	// ��������
	if(!CreateUI(hInst))
	{
		return FALSE;
	}

	//ע�����̨����
	RegisterTestCmd(m_pConsole);
	
	// ???���ο���̨���ڹرհ�ť
	//m_pConsole->GetWnidow()->GetCloseButton()->SetEnable(false);


	//// ������Ӧ
	//m_pInput->Init();
	//m_pInputMap->Register(0, _T("Hotkey_Console"), 
	//	(INPUTMAPEVENTHANDLE)m_pConsole->GetTrunk().sfp2(&vEngine::Console::Switch), DIK_F5, TRUE, TRUE);
	
	//ע�������ģ��
	RegisterDBCmd();

	//���õ���ǳ��ص�����
	m_pServer->Init((TCPLOGINCALLBACK)m_Trunk.sfp2(&CLoong::LoginCallBack),
					(TCPLOGOUTCALLBACK)m_Trunk.sfp1(&CLoong::LogoutCallBack),
					m_nPort);

	// Զ�̼�س�ʼ��
	g_rtSession.Init(this);

	// ���������߳�
	if(!m_pThread->CreateThread(_T("rec_msg"), (THREADPROC)m_Trunk.sfp1(&CLoong::ThreadReceiveMsg), NULL)
		|| !m_pThread->CreateThread(_T("proc_msg"), (THREADPROC)m_Trunk.sfp1(&CLoong::ThreadProcessMsg), NULL)
		|| !m_pThread->CreateThread(_T("broadcast"), (THREADPROC)m_Trunk.sfp1(&CLoong::ThreadBroadcast), NULL))
	{
		return FALSE;
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
//�ֵ�һЩ�����ĳ�ʼ������
//-------------------------------------------------------------------------------------------------------
BOOL CLoong::InitConfig()
{
	TCHAR tszPath[MAX_PATH];
	ZeroMemory(tszPath, sizeof(tszPath));
	if (!m_pUtil->GetIniPath(tszPath, _T("server_config/db/world")))
	{
		return FALSE;
	}

	// ����һ������
	CreateObj("LoongVar", "VarContainer");
	TObjRef<VarContainer> pVar = "LoongVar";

	// ���������ļ�,����������
	pVar->Load("VirtualFileSys", tszPath);

	m_nPort				= pVar->GetDword(_T("db_server port"));
	m_dwDBGoldenCode	= pVar->GetDword(_T("db_server golden_code"));
	m_dwGameGoldenCode	= pVar->GetDword(_T("game_server golden_code"));
	
	// �������
	pVar->Clear();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// �㲥(������)
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ThreadBroadcast(LPVOID)
{
	tagHeartbeat Heartbeat;
	
	while(true)
	{
		// �رշ�����
		if(m_bTerminate)
		{
			return 0;
		}

		m_Status.dwHeartbeat++;

		// Զ�̼�ظ���
		g_rtSession.Update();

		//// ����Ϸ��������������/*m_pServer->IsClientExsit(ID_GAMESERVER)*/
		//if(m_bGameServerConnect)
		//{
		//	Heartbeat.dwGoldenCode		= m_dwDBGoldenCode;			// ��������ɫ����
		//	Heartbeat.dwHeartbeat		= m_Status.dwHeartbeat;		// ��������������

		//	m_Status.dwUnSendMsgNum = SendMsg( &Heartbeat, sizeof(Heartbeat));
		//}

		// ÿ������һ��
		Sleep(1000);
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ϸ��������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ThreadReceiveMsg(LPVOID)
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif
	
	LPBYTE	pRecv		= NULL;
	DWORD	dwMsgSize	= GT_INVALID;
	INT		nUnrecv		= GT_INVALID;
	
	while(true)
	{	
		Sleep(SLEEPTIME);
		
		// �ж��߳��Ƿ����
		if(m_bTerminate)
		{
			break;
		}

		// �ж��Ƿ������ӣ����û�У������޵ȴ�
		if(!m_bGameServerConnect)
		{
			continue;
		}

		// ����Ƿ���Ҫ�����µ�log��
		if(GetCurrentDWORDTime().day != m_dwLogTime.day)
		{
			if(m_MsgQue.GetMsgNum() == 0)
			{
				// ȷ���޸ı��׺ʱ�������к������ʸ��ֶ�//??
				Sleep(SLEEPTIME);
				CreateCurDataLogTable();
			}
		}
		
		// ��ȡ��Ϸ��������Ϣ����,�����浽��������Ϣ����
		while(true)
		{
			pRecv = m_pServer->Recv(ID_GAMESERVER, dwMsgSize, nUnrecv);
			m_Status.dwUnRecvMsgNum = nUnrecv;
			if(!P_VALID(pRecv))
			{
				break;
			}

			if(dwMsgSize != ((tagNetCmd*)pRecv)->dwSize)
			{
				ASSERT(0);
				IMSG(_T("Invalid net command size[<cmd>%u <size>%d]\r\n"), 
					((tagNetCmd*)pRecv)->dwID, ((tagNetCmd*)pRecv)->dwSize);
				continue;
			}

			// ��m_MsgQue�Ĺ��캯����һ������Ϊtrueʱ���ú����Զ������߳�
			m_MsgQue.AddMsg(pRecv);

			// ͳ�ƽ�����Ϣ����
			++m_Status.dwRecvMsgNum;

			// ������Դ -- �ظ����ø��ڴ棬����û��Ҫ���ڴ濽��
			//m_pServer->FreeRecved(pRecv);
		}
	}	

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
	return 0;
}

//-------------------------------------------------------------------------------------------------------
// �����������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ThreadProcessMsg(LPVOID)
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	LPBYTE	pMsg		= NULL;
	DWORD	dwMsgSize	= GT_INVALID;
	//����һ��time���
	CHECK_TIME_WITH_INFO(NULL,500);

	while(true)
	{
		// �ж��߳��Ƿ����
		if(m_bTerminate)
		{
			break;
		}
		
		::WaitForSingleObject(m_MsgQue.GetEvent(), SLEEPTIME);

		while(true)
		{
			pMsg = m_MsgQue.GetMsg();
			if(!P_VALID(pMsg))
			{
				break;
			}

			std::string strCmpName = m_MsgCmdMgr.GetHadleCmdString((tagNetCmd*)pMsg);
			CHECK_TIME_START(strCmpName.c_str());

			// ������Ϣ
			m_MsgCmdMgr.HandleCmd((tagNetCmd*)pMsg, ((tagNetCmd*)pMsg)->dwSize, GT_INVALID);

			CHECK_TIME_COMMIT();

			++m_Status.dwProcMsgNum;

			// ������Դ
			m_pServer->FreeRecved(ID_GAMESERVER, pMsg);
		}

		// �����Ϸ�������Ƿ��˳�
		if( m_bGameServerLogout )
		{
			m_bGameServerLogout = FALSE;
			
			TermWorld();
			
			// ��������״̬,��֤�����������ܹ�����
			InterlockedExchange((LONG *)&m_bGameServerConnect, FALSE);
		}
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��Ϸ�������ӵ����֪ͨ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoginCallBack(LPBYTE pByte, DWORD dwSize)
{
	CResGuard::CGuard rg(m_Lock);

	// ��ǰ����һ����Ϸ�������ӻ�������֤����
	if(m_bGameServerConnect != FALSE
		|| *(DWORD*)pByte != m_pUtil->Crc32("N_Certification"))
	{
		IMSG(_T("A new game world apply for connecting��or connection message is wrong!\r\n"));
		return GT_INVALID;
	}

	MTRANS_POINTER(pNc, pByte, tagN_Certification);
	if(pNc->dwGoldenCode != m_dwGameGoldenCode)
	{
		IMSG(_T("Illegal connection, golden code is wrong!"));
		return GT_INVALID;
	}

	if(InitWorld(pNc->szWorldName) != E_Success)
	{
		return GT_INVALID;
	}

	// ��֤ĳ��ʱ��ֻ��һ������
	InterlockedExchange((LONG *)&m_bGameServerConnect, TRUE);
	m_bGameServerLogout	= FALSE;

	IMSG(_T("LoongWorld Server of %s login!\n"), pNc->szWorldName);
	
	return ID_GAMESERVER; // ÿ����Ϸ�������һ����Ϸ������
}

//-------------------------------------------------------------------------------------------------------
// ��Ϸ�������ӵǳ���֪ͨ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LogoutCallBack(DWORD dwDummy)
{
	CResGuard::CGuard rg(m_Lock);

	// ����������Ϣ
	LPBYTE pRecv = NULL;
	DWORD dwMsgSize = 0;
	INT nUnrecv = 0;
	while(true)
	{
		LPBYTE pRecv = m_pServer->Recv(ID_GAMESERVER, dwMsgSize, nUnrecv);
		m_Status.dwUnRecvMsgNum = nUnrecv;
		if(!P_VALID(pRecv))
		{
			break;
		}

		if(dwMsgSize != ((tagNetCmd*)pRecv)->dwSize)
		{
			ASSERT(0);
			IMSG(_T("Invalid net command size[<cmd>%u <size>%d]\r\n"), 
				((tagNetCmd*)pRecv)->dwID, ((tagNetCmd*)pRecv)->dwSize);
			continue;
		}

		m_MsgQue.AddMsg(pRecv);

		// ͳ�ƽ�����Ϣ����
		++m_Status.dwRecvMsgNum;
	}

	// ����world�������ǳ�
	m_bGameServerLogout	= TRUE;

	IMSG(_T("LoongWorld Server of %s logout!\n"), m_szWorldName);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ʼ����Ϸ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InitWorld(LPCTSTR szWorldName)
{
	TCHAR szIniFile[MAX_PATH];

	// ��ȡ�����ļ�����
	_tcscpy(szIniFile, _T("server_config/db/"));
	_tcscat(szIniFile, szWorldName);

	TCHAR tszPath[MAX_PATH];
	ZeroMemory(tszPath, sizeof(tszPath));
	if (!m_pUtil->GetIniPath(tszPath, szIniFile))
	{
		return FALSE;
	}

	// ��ʼ�����ݿ�����
	if(!m_pDB->Init(tszPath))
	{
		IMSG(_T("Database for %s connected failed!\n"), m_szWorldName);

		// ���ط�����Ϣ //??

		return GT_INVALID;
	}

	// ����log��
	CreateCurDataLogTable();

	// ��¼��Ϸ�������ƺ�������Ϸ����Ļ�����
	_tcsncpy(m_szWorldName, szWorldName, MCalTCharArrayNum(m_szWorldName) - 1);
	//_tcsncpy(m_szServerName, szServerName, MCalTCharArrayNum(m_szServerName) - 1);

	// ��ʼ��ͳ������
	m_Status.Init();

	IMSG(_T("Database for %s connected!\n"), m_szWorldName);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ر���Ϸ����
//-------------------------------------------------------------------------------------------------------
VOID CLoong::TermWorld()
{
	// ����������Ϣ��������
	LPVOID pMsg = NULL;
	DWORD dwMsgSize = 0;

	while(true)
	{
		LPBYTE pMsg = m_MsgQue.GetMsg();
		if(!P_VALID(pMsg)) break;

		// ������Ϣ
		m_MsgCmdMgr.HandleCmd((tagNetCmd*)pMsg, ((tagNetCmd*)pMsg)->dwSize, GT_INVALID);

		++m_Status.dwProcMsgNum;

		// ������Դ
		m_pServer->FreeRecved(ID_GAMESERVER, pMsg);
	}

	// ɾ��DB
	m_pDB->Destroy();
	m_dwLogTime = 0;
	m_szLogFilePostfix[0]	= '\0';

	// �����Ϸ��������������Ϸ����Ļ�����
	ZeroMemory(m_szWorldName, sizeof(m_szWorldName));
	//ZeroMemory(m_szServerName, sizeof(m_szServerName));
}


//-------------------------------------------------------------------------------------------------------
// ����UI����
//-------------------------------------------------------------------------------------------------------
BOOL CLoong::CreateUI(HINSTANCE hInst)
{
	// ��ʼ����ʾ
	m_Render.Init(m_pRender.operator->());	// ��vEngine��Ĭ��������render��������
	m_pWindow->Init(&m_Render,800,600,TRUE);
	m_pWindow->CreateWnd(_T("LoongDB"), hInst);

	if(!m_pRender->Init(800, 600))
	{
		return FALSE;
	}

	m_pGUI->Init(700, 500, &m_Render);	
	m_pRender->SetShowFPS(FALSE);

	return m_pConsole->Init(NULL);
}

//-------------------------------------------------------------------------------------------------------
// ����UI����
//-------------------------------------------------------------------------------------------------------
VOID CLoong::RefreshUI()
{
	m_pGUI->Update();
	m_pRender->Clear(0, 0, 0);
	m_pGUI->Render();
	m_pRender->Render();

	const CLoong::tagStatus *pStatus = GetStatus();

	// ������ϵͳ����
	m_pConsole->Watch(_T("cpu_num:"), pStatus->nCpuNum);
	m_pConsole->Watch(_T("mem_total(M):"), pStatus->dwTotalPhys/1024/1024);
	m_pConsole->Watch(_T("mem_avail(M):"), pStatus->dwAvailPhys/1024/1024);
	m_pConsole->Watch(_T("non_paged_pool_usage:"), pStatus->nQuotaNonPagedPoolUsage);

	m_pConsole->Watch(_T("-- Version --"), 0);

	// �汾��Ϣ
	m_pConsole->Watch(_T("LoongDB:"), m_Status.dwVerLoongDB);
	//m_pConsole->Watch(_T("Beton:"), m_Status.dwVerBeton);
	//m_pConsole->Watch(_T("vEngine:"), m_Status.dwVerVEngine);
	//m_pConsole->Watch(_T("ServerDef:"), m_Status.dwVerServerDefine);
	//m_pConsole->Watch(_T("XML:"), m_Status.dwVerXML);

	m_pConsole->Watch(_T("-- Status --"), 0);

	// ��������
	m_pConsole->Watch(_T("connect_status:"), m_bGameServerConnect);
	m_pConsole->Watch(_T("heart_beat:"), pStatus->dwHeartbeat);

	// ��Ϣ����
	m_pConsole->Watch(_T("-- Total --"), 0);
	m_pConsole->Watch(_T("RecvNum:"), pStatus->dwRecvMsgNum);
	m_pConsole->Watch(_T("ProcMsgNum:"), pStatus->dwProcMsgNum);
	m_pConsole->Watch(_T("SendNum:"), pStatus->dwSendMsgNum);
	m_pConsole->Watch(_T("RecvSz:"), pStatus->dwRecvMsgSz);
	m_pConsole->Watch(_T("SendSz:"), pStatus->dwSendMsgSz);

	m_pConsole->Watch(_T("ReadNum:"), pStatus->dwReadNum);
	m_pConsole->Watch(_T("WriteNum:"), pStatus->dwWriteNum);

	m_pConsole->Watch(_T("-- Tick(ms) --"), pStatus->dwInterval);
	m_pConsole->Watch(_T("TickRecvNum:"), pStatus->dwTickRecvNum);
	m_pConsole->Watch(_T("TickSendNum:"), pStatus->dwTickSendNum);
	m_pConsole->Watch(_T("TickRecvSz:"), pStatus->dwTickRecvSz);
	m_pConsole->Watch(_T("TickSendSz:"), pStatus->dwTickSendSz);

	m_pConsole->Watch(_T("TickProcNum:"), pStatus->dwTickProcNum);
	m_pConsole->Watch(_T("TickReadNum:"), pStatus->dwTickReadNum);
	m_pConsole->Watch(_T("TickWriteNum:"), pStatus->dwTickWriteNum);

	m_pConsole->Watch(_T("-- Caution: --"), 0);
	m_pConsole->Watch(_T("UnSendMsgNum:"), pStatus->dwUnSendMsgNum);
	m_pConsole->Watch(_T("UnRecvMsgNum: --"), pStatus->dwUnRecvMsgNum);
	m_pConsole->Watch(_T("MaxSendedMsgSize: --"), pStatus->dwMaxSendedMsgSize);
	m_pConsole->Watch(_T("MaxSendedMsgID: --"), pStatus->dwMaxSendedMsgID);
}

//-------------------------------------------------------------------------------------------------------
// �õ�������״̬��Ϣ
//-------------------------------------------------------------------------------------------------------
const CLoong::tagStatus* CLoong::GetStatus()
{
	// ϵͳ���
	MEMORYSTATUS memStatus;
	memStatus.dwLength = sizeof(memStatus);
	GlobalMemoryStatus(&memStatus);
	m_Status.dwTotalPhys = memStatus.dwTotalPhys;
	m_Status.dwAvailPhys = memStatus.dwAvailPhys;

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	m_Status.nQuotaNonPagedPoolUsage = pmc.QuotaNonPagedPoolUsage;

	// ϵͳʱ��
	DWORD dwTick = GetTickCount();
	m_Status.dwInterval = dwTick - m_Status.dwLastTick;
	m_Status.dwLastTick = dwTick;

	// ��Ϸ�������
	m_Status.dwReadNum		= m_pDB->GetTotalReadTimes();
	m_Status.dwWriteNum		= m_pDB->GetTotalWriteTimes();

	m_Status.dwSendMsgNum	= m_pServer->GetMsgSend();
	m_Status.dwRecvMsgSz	= m_pServer->GetBytesRecv();
	m_Status.dwSendMsgSz	= m_pServer->GetBytesSend();

	m_Status.dwTickRecvNum	= m_Status.dwRecvMsgNum - m_Status.dwLastRecvNum;
	m_Status.dwTickProcNum	= m_Status.dwProcMsgNum - m_Status.dwLastProcNum;
	m_Status.dwTickSendNum	= m_Status.dwSendMsgNum - m_Status.dwLastSendNum;

	m_Status.dwTickRecvSz	= m_Status.dwRecvMsgSz - m_Status.dwLastRecvSz;
	m_Status.dwTickSendSz	= m_Status.dwSendMsgSz - m_Status.dwLastSendSz;

	m_Status.dwTickReadNum	= m_Status.dwReadNum - m_Status.dwLastReadNum;
	m_Status.dwTickWriteNum	= m_Status.dwWriteNum - m_Status.dwLastWriteNum;

	m_Status.dwLastRecvNum	= m_Status.dwRecvMsgNum;
	m_Status.dwLastProcNum	= m_Status.dwProcMsgNum;
	m_Status.dwLastSendNum	= m_Status.dwSendMsgNum;

	m_Status.dwLastRecvSz	= m_Status.dwRecvMsgSz;
	m_Status.dwLastSendSz	= m_Status.dwSendMsgSz;

	m_Status.dwLastReadNum	= m_Status.dwReadNum;
	m_Status.dwLastWriteNum	= m_Status.dwWriteNum;

	return &m_Status;
}

//-------------------------------------------------------------------------------------------------------
// ����UI����
//-------------------------------------------------------------------------------------------------------
VOID CLoong::MainLoop()
{
	DWORD dwMsg, dwParam1, dwParam2;
	
	while(!m_pWindow->MsgLoop() && !IsTerminate())
	{	
		if(FALSE == m_pWindow->IsWindowActive())
		{
			Sleep(50);
			continue;
		}
		
		while(m_pWindow->PeekWindowsMsg(dwMsg, dwParam1, dwParam2))
		{
			m_pGUI->OnWindowsMsg(dwMsg, dwParam1, dwParam2);
			if(dwMsg == WM_QUIT)
				return;
		}
		
		// ���½���
		RefreshUI();

		//// ��ݼ���Ӧ
		//m_pInput->Update();
		//m_pInputMap->Update();

		Sleep(500);
	}
}

DWORD CLoong::LoadLeftMsg( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_LoadLeftMsg);

	BYTE buffer[1024 * MAX_LEFTMSG_NUM] = {0};
	tagNDBS_LoadLeftMsg* pSend = reinterpret_cast<tagNDBS_LoadLeftMsg*>(buffer);
	pSend->dwID = m_pUtil->Crc32("NDBS_LoadLeftMsg");
	pSend->dwRoleID = pRecv->dwRoleID;
	DWORD dwAllLeftMsgSize = 0;
	BOOL bHasLeftMsg = m_pDB->LoadLeftMsg(pRecv->dwRoleID, pSend->byData, dwAllLeftMsgSize);
	if (bHasLeftMsg)
	{
		pSend->dwSize = sizeof(tagNDBS_LoadLeftMsg) - 1 + dwAllLeftMsgSize;
	}
	else
	{
		pSend->dwSize = sizeof(tagNDBS_LoadLeftMsg);
	}
	
	SendMsg( pSend, pSend->dwSize);

	return E_Success;
}

DWORD CLoong::InsertLeftMsg( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_InsertLeftMsg);
	DWORD dwIndexes[MAX_LEFTMSG_NUM] = {0};
	INT nNum = 0;
	m_pDB->LoadLeftMsgIndexes(dwIndexes, pRecv->data.dwRoleID, nNum);
	if (nNum >= MAX_LEFTMSG_NUM)
	{
		m_pDB->DelLeftMsg(pRecv->data.dwRoleID, dwIndexes[0]);
	}
	m_pDB->InsertLeftMsg(pRecv->data.dwRoleID, pRecv->data.dwDateTime, &(pRecv->data));
	return E_Success;
}

DWORD CLoong::ClearLeftMsg( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_ClearLeftMsg);
	m_pDB->ClearLeftMsg(pRecv->dwRoleID);
	return E_Success;
}

DWORD CLoong::UpdateRoleAtt( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_UpdateRoleAtt);
	m_pDB->UpdateRoleAtt(pRecv->dwRoleID, pRecv);
	return E_Success;
}

DWORD CLoong::UpdateRoleAttPoint( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_UpdateRoleAttPoint);
	m_pDB->UpdateRoleAttPoint(pRecv->dwRoleID, (PVOID)pDBMsg);
	return E_Success;
}

DWORD CLoong::UpdateRoleTalentPoint( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_UpdateRoleTalentPoint);
	m_pDB->UpdateRoleTalentPoint(pRecv->dwRoleID, (PVOID)pDBMsg);
	return E_Success;
}
DWORD CLoong::UpdateRoleItemTransport( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_UpdateRoleItemTransport);
	m_pDB->UpdateRoleItemTranport(pRecv->dwRoleID, (PVOID)pDBMsg);
	return E_Success;
}
VOID CLoong::SendMsg( PVOID pSend, DWORD dwSize )
{
	tagNetCmd* pSendCmd = (tagNetCmd*)pSend;
	if (dwSize > m_Status.dwMaxSendedMsgSize)
	{
		m_Status.dwMaxSendedMsgSize = dwSize;
		m_Status.dwMaxSendedMsgID = pSendCmd->dwID;
	}

	m_pServer->Send(ID_GAMESERVER, pSend, dwSize);
}