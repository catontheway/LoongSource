//-----------------------------------------------------------------------------
//!\file bill_server.cpp
//!\author Aslan
//!
//!\date 2009-06-09
//! last 2009-06-09
//!
//!\brief ֱ�仮��������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "world.h"
#include "world_mgr.h"
#include "bill_server.h"
#include "login_db.h"
BillServer g_Server;

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL BillServer::Init(HINSTANCE hInst)
{
	// vEngine��ض���
	m_pLog			=	"Log";
	m_pRender		=	"GDIRender";
	m_pWindowWrap	=	"WindowWrap";
	m_pUtil			=	"Util";
	m_pGUI			=	"GUISystem";
	m_pConsole		=	"Console";

	// ����VFS�ļ�ϵͳ,��ȡXML��Ҫ�õ�
	CreateObj("VFS_System", "VirtualFileSys");

	// ����log
	m_pLog->Create();

	// ���������ļ�
	CreateObj("BillConfig", "VarContainer");
	TObjRef<VarContainer> pVar = "BillConfig";
	TCHAR tszFilePath[MAX_PATH] = {0};
	if (!m_pUtil->GetIniPath(tszFilePath, _T("server_bill_config/config")))
		return FALSE;

	pVar->Load("VirtualFileSys", tszFilePath);

	// ����ID
	m_dwSectionID	=	pVar->GetDword(_T("id"), _T("section"));

	// ���ڴ�С
	m_dwViewWidth	=	pVar->GetDword(_T("width"), _T("window"));
	m_dwViewHeight	=	pVar->GetDword(_T("height"), _T("window"));

	// ����GUI��Ⱦ
	m_pGUIRender = new IGUIRenderNative;
	if( !P_VALID(m_pGUIRender) ) return FALSE;
	m_pGUIRender->Init(m_pRender);

	// ��������
	m_pWindowWrap->Init(m_pGUIRender, m_dwViewWidth, m_dwViewHeight, TRUE);
	m_pWindowWrap->CreateWnd(_T("LoongBillServer"), hInst);

	// ����GUIϵͳ
	if( !m_pRender->Init(m_pWindowWrap->GetWidth(), m_pWindowWrap->GetHeight()) )
		return FALSE;

	m_pGUI->Init(m_pWindowWrap->GetWidth(), m_pWindowWrap->GetHeight(), m_pGUIRender);

	// ����Console
	m_pConsole->Init(NULL, 1000, TRUE);

	// ϵͳ����
	m_nCPUNum = TObjRef<SysInfo>()->GetProcessorNum();

	//��ʼ��logindb
	if( !g_LoginDB.Init() )
	{
		ILOG->Write(_T("login db init failed!\r\n"));
		return FALSE;
	}

	// ��ʼ����Ϸ���������
	if( !g_WorldMgr.Init() )
	{
		ILOG->Write(_T("world mgr init failed!\r\n"));
		return FALSE;
	}

	// ��ʼ���ɹ�
	ILOG->Write(_T("bill server init ok!\r\n"));
	IMSG(_T("bill server init ok!\r\n"));
	InitOK();

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------
VOID BillServer::Destroy()
{
	// ɾ��worldmgr
	g_WorldMgr.Destroy();

	// ���ٴ��ڼ�GUI��ϵͳ
	m_pWindowWrap->Destroy();
	m_pGUI->Destroy();
	m_pRender->Destroy();
	m_pConsole->Destroy();
	m_pLog->Close();

	SAFE_DEL(m_pGUIRender);
}

//-----------------------------------------------------------------------------------
// ��ѭ��
//-----------------------------------------------------------------------------------
VOID BillServer::MainLoop()
{
	DWORD dwMsg=0, dwParam1=0, dwParam2=0;

	while( FALSE == m_pWindowWrap->MsgLoop() && !IsShutingDown() )
	{
		if( FALSE == m_pWindowWrap->IsWindowActive() )
		{
			Sleep(30);
			continue;
		}

		while( m_pWindowWrap->PeekWindowsMsg(dwMsg, dwParam1, dwParam2) )
		{
			m_pGUI->OnWindowsMsg(dwMsg, dwParam1, dwParam2);
			if( dwMsg == WM_QUIT )
				return;
		}

		m_pGUI->Update();

		// render
		m_pRender->Clear(0, 0, 0);
		m_pGUI->Render();
		m_pRender->Render();

		// �õ��ڴ�ռ��
		UpdateMemoryUsage();

		// ��ʾ����ʱ��Ϣ
		DisplayRunTimeInfo();

		Sleep(50);
	}
}

//-----------------------------------------------------------------------------
// �����ڴ��ռ�����
//-----------------------------------------------------------------------------
VOID BillServer::UpdateMemoryUsage()
{
	MEMORYSTATUS memStatus;
	memStatus.dwLength = sizeof(memStatus);
	GlobalMemoryStatus(&memStatus);

	m_dwTotalPhys = memStatus.dwTotalPhys;
	m_dwAvailPhys = memStatus.dwAvailPhys;

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

	m_nQuotaNonPagedPoolUsage = pmc.QuotaNonPagedPoolUsage;
}

//-----------------------------------------------------------------------------
// ��ʾ����ʱ��Ϣ
//-----------------------------------------------------------------------------
VOID BillServer::DisplayRunTimeInfo()
{
	// CPU�ڴ�
	m_pConsole->Watch(_T("cpu_num:"),				GetCPUNum());
	m_pConsole->Watch(_T("mem_total:"),				m_dwTotalPhys/1024/1024);
	m_pConsole->Watch(_T("mem_avail:"),				m_dwAvailPhys/1024/1024);
	m_pConsole->Watch(_T("non_paged_pool:"),		m_nQuotaNonPagedPoolUsage);

	// TICK
	m_pConsole->Watch(_T("tick:"),					GetTick());

	// ����ID
	m_pConsole->Watch(_T("section"),				GetSectionID());

	// world��Ϣ
	g_WorldMgr.DisPlayerRunTimeInfo(m_pConsole);

}

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
INT APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	// ���ý������ȼ�
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	// ��ֹ��Ļ��������Դ����
	::SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);

	// ���ñ���ʱ��������
	//Exception::m_bSendLogWindow = FALSE;

	vEngine::InitNetwork();
	vEngine::InitEngine();

#ifdef DEBUG
	EXCEPTION_PROTECT;
#endif

	if( g_Server.Init(hInst) )
	{
		g_Server.MainLoop();
	}
	g_Server.Destroy();

	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();
	return 0;
}