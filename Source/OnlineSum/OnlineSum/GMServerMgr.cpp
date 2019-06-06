#include "stdafx.h"
#include "time.h"
#include "GMServerMgr.h"
#include "../../ServerDefine/msg_show_sum.h"

GMServerMgr::GMServerMgr()
{
	m_nGMServerCount = 0;
	m_fRate = 1;
	m_dwInterval = 60;
	m_nSumOnline = 0;
}

GMServerMgr::~GMServerMgr()
{

}

void GMServerMgr::Update()
{
	while (true)
	{
		TMap<INT, GMServer*>::TMapIterator it = m_mapGMServer.Begin();
		GMServer* pGMServer = NULL;
		while( m_mapGMServer.PeekNext(it, pGMServer))
		{
			pGMServer->Update();
		}	
	}
}

//void GMServerMgr::Print()
//{
//	while(TRUE)
//	{
//		_tprintf(_T("|---------------------------- Show Online Sum ----------------------------|\r\n"));
//
//		TMap<INT, GMServer*>::TMapIterator it = m_mapGMServer.Begin();
//		GMServer* pGMServer = NULL;
//
//		while( m_mapGMServer.PeekNext(it, pGMServer))
//		{
//			pGMServer->PrintInfo(m_fRate, pGMServer->IsActive());
//			pGMServer->SetActive(FALSE);
//		}	
//
//		_tprintf(_T("|-------------------------------------------------------------------------|\r\n"));
//
//		Sleep(m_dwInterval*100);
//	}
//}

void GMServerMgr::GetGMServerDynInfo()
{
	while ( true )
	{
		INT nSum = 0;
		m_nSumOnline = 0;
		TMap<INT, GMServer*>::TMapIterator it = m_mapGMServer.Begin();
		GMServer* pGMServer = NULL;

		// ���������GMServer����Ϣ������̬��Ϣ
		while( m_mapGMServer.PeekNext(it, pGMServer))
		{
			m_nSumOnline += pGMServer->GetSum();
			tagNSSC_GetDynInfo send;
			pGMServer->SendMsg(&send, send.dwSize);
		}
	
		// ������������Ϣ
		nSum = (INT)(m_nSumOnline * g_GMServerMgr.GetRate());

		// ��ʾ
		m_lock.Acquire();
		
		system("cls");
		_tprintf(_T("\r\n\n\n|---------------------------- Show Online Sum ----------------------------|\r\n"));
		_tprintf(_T(" [ %d ]\r\n"), nSum);
		
		it = m_mapGMServer.Begin();
		pGMServer = NULL;
		
		while( m_mapGMServer.PeekNext(it, pGMServer))
		{
			pGMServer->PrintInfo(g_GMServerMgr.GetRate(), pGMServer->IsActive());
			pGMServer->SetActive(FALSE);
		}	

		_tprintf(_T("|-------------------------------------------------------------------------|\r\n"));
		m_lock.Release();

		Sleep(m_dwInterval*1000);
	}
}

BOOL GMServerMgr::Init()
{
	m_pLog			=	"Log";
	m_pUtil			=	"Util";
	m_pVar			=	"VarContainer";

	// ����log
	m_pLog->Create();

	// ���������ļ�
	TCHAR tszConfigFile[MAX_PATH] = {0};
	_stprintf(tszConfigFile, _T("%s%s"), _T(CONFIG_FILE_PATH),_T(CONFIG_INI));
	TCHAR tszFilePath[MAX_PATH] = {0};
	if (!m_pUtil->GetIniPath(tszFilePath, tszConfigFile)) return false;
	m_pVar->Load("VirtualFileSys", tszFilePath);

	// ��ȡGMServer����
	INT m_nGMServerCount = m_pVar->GetDword(_T("Count"), _T("GMServer"));
	if( m_nGMServerCount <= 0 ) return false;

	// ��ȡ������ʾ��Ϣ
	m_fRate = m_pVar->GetFloat(_T("Rate"), _T("Global"));
	m_dwInterval = m_pVar->GetDword(_T("Interval"), _T("Global"));

	// ��ʼ��GMServer��ͼ
	TCHAR szTemp[X_SHORT_STRING] = {0};
	for(INT n=0; n<m_nGMServerCount; ++n)
	{
		_stprintf(szTemp, _T("GMServer_%d"), n);

		LPCTSTR szGMServerIP = m_pVar->GetString(_T("IP"), szTemp);
		INT nPort = m_pVar->GetInt(_T("Port"), szTemp);

		GMServer* pGMServer = new GMServer(nPort, szGMServerIP);
		if(P_VALID(pGMServer))
		{
			if(pGMServer->Init())
			{
				pGMServer->Connect();
			}
		}
		
		m_mapGMServer.Add(n, pGMServer);
	}

	// -ע��������������
	/*m_pCmdMgr = "NetCmdMgr";
	RegisterAllNetCommand();*/

	// -�����߳�
	m_pThread =	"Thread";
	if(!m_pThread->CreateThread(_T("ConnectGMServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&GMServerMgr::GetGMServerDynInfo), NULL))
	{
		return FALSE;
	}

	while( !m_pThread->IsThreadActive(_T("ConnectGMServer")) )
	{
		continue;
	}

	return TRUE;
}

GMServerMgr g_GMServerMgr;