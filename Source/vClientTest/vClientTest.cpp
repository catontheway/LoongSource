//-----------------------------------------------------------------------------
//!\file vClientTest.cpp
//!\author Lyp
//!
//!\date 2004-08-11
//! last 2007-10-11
//!
//!\brief vengine���Գ���
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "vClientTest.h"

CONST INT CLIENT_TEST_PORT = 7000;
//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL CClientTest::Init(HINSTANCE hInst)
{
	m_bTerminateThread = FALSE;

	m_render.Init(m_pRender);	// ��vEngine��Ĭ��������render��������
	m_pWindow->Init(&m_render,750,550,TRUE);
	m_pWindow->CreateWnd(_T("vClientTest"), hInst);

	if( !m_pRender->Init(m_pWindow->GetWidth(), m_pWindow->GetHeight()) )
		return FALSE;

	m_pGUI->Init(m_pWindow->GetWidth(), m_pWindow->GetHeight(), &m_render);	
	m_pRender->SetShowFPS(FALSE);

	// ע�Ἰ������̨����
	m_pConsole->Init(NULL);
	m_pConsole->SetCaption(_T("Console - Client Test"));
	m_pConsole->Register(_T("client"), m_Trunk.sfp2(&CClientTest::CreateClient), _T("����ָ����Ŀ�Ŀͻ���"), 2);

	m_lClient = 0;
	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID CClientTest::Destroy()
{
	InterlockedExchange((LONG*)&m_bTerminateThread, TRUE);
	m_pThread->WaitForAllThreadDestroy();

	m_pWindow->Destroy();
	m_pGUI->Destroy();
}


//-----------------------------------------------------------------------------
// Mainloop
//-----------------------------------------------------------------------------
VOID CClientTest::Mainloop()
{
	DWORD dwMsg, dwParam1, dwParam2;
	static DWORD dwTimeKeeper = timeGetTime();

	while( FALSE == m_pWindow->MsgLoop() )
	{
		DWORD dwDeltaTime = timeGetTime() - dwTimeKeeper;
		dwTimeKeeper = timeGetTime();

		// update
		while( m_pWindow->PeekWindowsMsg(dwMsg, dwParam1, dwParam2) )
		{
			m_pGUI->OnWindowsMsg(dwMsg, dwParam1, dwParam2);
			if( dwMsg == WM_QUIT )
				return;
		}

		m_pConsole->Watch(_T("Client"), (DWORD)m_lClient);
		m_pGUI->Update();
		DWORD dwTimeUse = timeGetTime() - dwTimeKeeper;
		if( dwTimeUse < 50 )
			Sleep(50-dwTimeUse);

		// render
		m_pRender->Clear(0, 0, 0);//x1f1f1f);
		m_pGUI->Render();
		m_pRender->Render();
	}
}

//-----------------------------------------------------------------------------
// ����ָ����Ŀ�Ŀͻ���
//-----------------------------------------------------------------------------
DWORD CClientTest::CreateClient(LPCTSTR szDestIP, LPCTSTR szNum)
{
	// ��ֹͣ�����߳�
	InterlockedExchange((LONG*)&m_bTerminateThread, TRUE);
	m_pThread->WaitForAllThreadDestroy();
	InterlockedExchange((LONG*)&m_bTerminateThread, FALSE);

	strcpy(m_szDestIP, m_pUtil->UnicodeToUnicode8(szDestIP));
	DWORD dwNum = _ttol(szNum);

	ASSERT(dwNum < 5000);
	// ���¿�ʼ
	for(INT n=0 ;n<(INT)dwNum; n++)
	{
		tstringstream stream;
		stream << _T("thread_client") << n;
		m_pThread->CreateThread(stream.str().c_str(),
			(THREADPROC)m_Trunk.sfp1(&CClientTest::ThreadXClient), (LPVOID)n);

		// ����ͬ�������ĵ���ǳ��߳�
		stream.clear();
		stream.str(_T(""));
		stream << _T("thread_client_burstlogin") << n;
		m_pThread->CreateThread(stream.str().c_str(),
			(THREADPROC)m_Trunk.sfp1(&CClientTest::ThreadXClientBurstLogin), (LPVOID)n);
	}

	return 0;
}


//-----------------------------------------------------------------------------
// Client�߳�
//-----------------------------------------------------------------------------
DWORD CClientTest::ThreadClient(INT nIndex)
{
	std::stringstream stream;
	stream << "StreamTransport" << nIndex;
	LPCSTR szObj = CreateObj(stream.str().c_str(), "StreamTransport");
	if( !P_VALID(szObj) )
	{
		_endthreadex(0);
		return 0;
	}

	INT nPackage = 0;
	TObjRef<StreamTransport> pTrans = stream.str().c_str();
	pTrans->Init(FALSE);

_thread_connect:
	pTrans->TryToConnect(m_szDestIP, CLIENT_TEST_PORT);
	while( pTrans->IsTryingToConnect() )
	{
		Sleep(rand()%1000 + 200);
		if( m_bTerminateThread )
			goto _thread_exit;
	}

	if( !pTrans->IsConnected() )
		goto _thread_connect;


	// ������
	// IMSG(_T("Client%d connected\r\n"), nIndex);
	// ���͵�һ����֤��
	InterlockedIncrement(&m_lClient);
	tagMsg msg;
	msg.dwIndex = 0;
	pTrans->Send(&msg, sizeof(msg));

	while( !m_bTerminateThread && pTrans->IsConnected() )
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = pTrans->Recv(dwSize);
		if( pRecv )
		{
			tagMsg* pMsg = (tagMsg*)pRecv;
			if( dwSize > 0 )
			{
				if( pMsg->dwIndex > 0 )
					pTrans->Send(pRecv, 100);

				pTrans->FreeRecved(pRecv);
				nPackage++;
			}
		}
		else
		{
			Sleep(10);
		}
	}

	InterlockedDecrement(&m_lClient);
	pTrans->StopTryingConnect();

	if( !m_bTerminateThread )
		goto _thread_connect;

_thread_exit:
	
	KillObj(stream.str().c_str());
	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// XClient�߳�
//-----------------------------------------------------------------------------
DWORD CClientTest::ThreadXClient(INT nIndex)
{
	std::stringstream stream;
	stream << "XClient" << nIndex;
	LPCSTR szObj = CreateObj(stream.str().c_str(), "XClient");
	if( !P_VALID(szObj) )
	{
		_endthreadex(0);
		return 0;
	}

	INT nPackage = 0;
	TObjRef<XClient> pTrans = stream.str().c_str();
	pTrans->Init(FALSE);

_thread_connect:
	pTrans->TryToConnect(m_szDestIP, CLIENT_TEST_PORT);
	while( pTrans->IsTryingToConnect() )
	{
		Sleep(rand()%1000 + 200);
		if( m_bTerminateThread )
			goto _thread_exit;
	}

	if( !pTrans->IsConnected() )
		goto _thread_connect;


	// ������
	// IMSG(_T("Client%d connected\r\n"), nIndex);
	// ���͵�һ����֤��
	InterlockedIncrement(&m_lClient);
	tagMsg msg;
	msg.dwIndex = 0;
	pTrans->Send(&msg, sizeof(msg));

	while( !m_bTerminateThread && pTrans->IsConnected() )
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = pTrans->Recv(dwSize);
		if( pRecv )
		{
			tagMsg* pMsg = (tagMsg*)pRecv;
			if( dwSize > 0 )
			{
				if( pMsg->dwIndex > 0 )
					pTrans->Send(pRecv, 100);

				pTrans->FreeRecved(pRecv);
				nPackage++;
			}
		}
		else
		{
			Sleep(10);
		}
	}

	InterlockedDecrement(&m_lClient);
	pTrans->StopTryingConnect();

	if( !m_bTerminateThread )
		goto _thread_connect;

_thread_exit:

	KillObj(stream.str().c_str());
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// XClient����ǳ��߳�
//-----------------------------------------------------------------------------
DWORD CClientTest::ThreadXClientBurstLogin(INT nIndex)
{
	std::stringstream stream;
	stream << "XClientBurstLogin" << nIndex;
	LPCSTR szObj = CreateObj(stream.str().c_str(), "XClient");
	if( !P_VALID(szObj) )
	{
		_endthreadex(0);
		return 0;
	}

	INT nPackage = 0;
	TObjRef<XClient> pTrans = stream.str().c_str();
	pTrans->Init(FALSE);

_thread_connect:
	pTrans->TryToConnect(m_szDestIP, CLIENT_TEST_PORT);
	while( pTrans->IsTryingToConnect() )
	{
		Sleep(rand()%1000 + 200);
		if( m_bTerminateThread )
			goto _thread_exit;
	}

	if( !pTrans->IsConnected() )
		goto _thread_connect;

	DWORD dwCloseTime = timeGetTime() + (rand() % 5000);

	// ������
	// IMSG(_T("Client%d connected\r\n"), nIndex);
	// ���͵�һ����֤��
	InterlockedIncrement(&m_lClient);
	tagMsg msg;
	msg.dwIndex = 0;
	pTrans->Send(&msg, sizeof(msg));

	while( !m_bTerminateThread && pTrans->IsConnected() )
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = pTrans->Recv(dwSize);
		if( pRecv )
		{
			tagMsg* pMsg = (tagMsg*)pRecv;
			if( dwSize > 0 )
			{
				if( pMsg->dwIndex > 0 )
					pTrans->Send(pRecv, 100);

				pTrans->FreeRecved(pRecv);
				nPackage++;
			}
		}
		else
		{
			Sleep(10);
			if( timeGetTime() - dwCloseTime > 0 )
			{
				pTrans->Disconnect();
			}
		}
	}

	InterlockedDecrement(&m_lClient);
	pTrans->StopTryingConnect();

	if( !m_bTerminateThread )
		goto _thread_connect;

_thread_exit:

	KillObj(stream.str().c_str());
	_endthreadex(0);
	return 0;
}