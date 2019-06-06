//-----------------------------------------------------------------------------
//!\file x_server.cpp
//!\author Lyp
//!
//!\date 2004-04-03
//! last 2009-06-15
//!
//!\brief �������˴���ײ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "x_server.h"

#include "..\debug\exception.h"
#include "..\system\sysinfo.h"
#include "..\filter\FilterMgr.h"
#include "..\debug\debug.h"
#include "..\console\console_gui.h"

#pragma intrinsic(memcpy)

namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
XServer::XServer():m_Trunk(this)
{
	m_hCompletionPort = 0;
	m_lpfnAcceptEx = 0;
	m_lpfnGetAcceptExSockAddr = 0;
	m_bTerminateWorkThread = FALSE;
	m_Pool = NULL;
	m_nPoolNum = 0;
	m_nAcceptExs = 0;
	m_n64SendSize = 0;
	m_n64RealSendSize = 0;

	m_hEventAccepEx = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// �Զ�����
	m_hEventSend = ::CreateEvent(NULL, FALSE, TRUE, NULL);		// �Զ�����
	m_hEventDestroy = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// �Զ�����
	m_pFilter = CreateObj("XServerFilter", "FilterMgr");
	m_pFilter->InitFilter(FMF_MiniLzo);
	m_pUtil = "Util";
}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
XServer::~XServer()
{
	KillObj("XServerFilter");
	CloseHandle(m_hEventAccepEx);
	CloseHandle(m_hEventSend);
	CloseHandle(m_hEventDestroy);
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL XServer::Init(tagXServerConfig& param)
{
	ASSERT(param.fnLogIn && param.fnLogOut);
	memcpy(&m_Config, &param, sizeof(m_Config)); // �ڲ�����һ������
	
	m_Client = new tagXClient[m_Config.nMaxServerLoad];	// ����client data
	if( !m_Client )
		return FALSE;

	ZeroMemory(m_Client, sizeof(tagXClient)*m_Config.nMaxServerLoad);
	for(INT n=0; n<m_Config.nMaxServerLoad; n++)
	{
		m_Client[n].pRecvQueue = new XQueue<tagUnit*>;
		m_Client[n].pSendQueue = new XQueue<tagRawUnit*>;
		m_listFreeClient.PushBack(&m_Client[n]);
	}

	m_sockListen = socket(AF_INET, SOCK_STREAM, 0);	// ����socket

	// reuse��ɶ�ΰ󶨳ɹ������ܲ����ҵ���ȷ���õĶ˿�
	// �����̨����������ͬһ̨������ʹ��ͬһ���˿ڣ���ɻ���
	if( m_Config.bReusePort )
	{
		DWORD dwReUseAddr = TRUE;	
		setsockopt( m_sockListen, SOL_SOCKET, SO_REUSEADDR, 
			(char*)&dwReUseAddr, sizeof(dwReUseAddr) );
	}

	// disables the NAGLE algorithm for send coalescing
	if( !m_Config.bNagle )
	{
		BOOL bNoDelay = TRUE;
		setsockopt(m_sockListen, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
	}

	sockaddr_in address;
    ZeroMemory(&address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port        = htons(m_Config.nPort);

    while( SOCKET_ERROR == bind(m_sockListen, (sockaddr*)&address, sizeof(address)) )
	{
		if( WSAEADDRINUSE == WSAGetLastError() )
		{
			++m_Config.nPort;	// ���γ��Ժ���Ķ˿�
			address.sin_port = htons(m_Config.nPort);
		}
		else
		{
			closesocket(m_sockListen);
			ERR(_T("bind() failed"));
			return FALSE;
		}
	}

    m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    listen( m_sockListen, SOMAXCONN );
	CreateIoCompletionPort((HANDLE)m_sockListen, m_hCompletionPort, (DWORD)0, 0);

	// ���AcceptEx\GetAcceptExSockAddrs�ĺ���ָ�룬ֱ�ӵ��ø��ӿ��
	GUID Guid = WSAID_ACCEPTEX, Guid2 = WSAID_GETACCEPTEXSOCKADDRS;
	DWORD dwBytes = 0;
	WSAIoctl(m_sockListen, SIO_GET_EXTENSION_FUNCTION_POINTER, &Guid,
		sizeof(GUID), &m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx), &dwBytes, 0, 0);
	WSAIoctl(m_sockListen, SIO_GET_EXTENSION_FUNCTION_POINTER, &Guid2,
		sizeof(GUID), &m_lpfnGetAcceptExSockAddr,sizeof(m_lpfnGetAcceptExSockAddr), &dwBytes, 0, 0);

	// �������ļ�ָ������Ŀ���������߳�
	INT nWorkerThread = (TObjRef<SysInfo>()->GetProcessorNum() * m_Config.nPerCpuWorkerThread) 
		+ m_Config.nAdditionWorkerThread;
	
	if( FALSE == CreateWorkThread(nWorkerThread) )	// ���������߳��Լ��ڴ��
		return FALSE;
	
	if( FALSE == CreateSetupAccepExThread() )	// SetupAccpetEx�߳�
		return FALSE;

	if( FALSE == CreateSendThread() )	// send�߳�
		return FALSE;
	
	if( FALSE == CreateUnauthHeartbeatThread() )	// unauth heartbeat�߳�
		return FALSE;

	if( FALSE == CreateDestroyThread() )	// destroy�߳�
		return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL XServer::Destroy()
{
	SAFE_CLOSE_HANDLE(m_hCompletionPort);

	// �ȴ������߳̽���
	InterlockedExchange((LONG*)&m_bTerminateWorkThread, TRUE);
	for(INT n=(INT)m_vecThreadHandle.size()-1; n>=0; n--)
	{
		if( m_vecThreadHandle[n] )
		{
			WaitForSingleObject(m_vecThreadHandle[n], INFINITE);
			SAFE_CLOSE_HANDLE(m_vecThreadHandle[n]);
		}
	}

	for(INT n=0; n<m_Config.nMaxServerLoad; n++)	// ���� index
	{
		if( !m_Client[n].bShutDown )
		{
			closesocket(m_Client[n].sock);
			// ����������assert˵�������ϲ�δ�����������Ϣ����
			// ��recv queue��resend queue
			SAFE_DEL(m_Client[n].pRecvQueue);
			SAFE_DEL(m_Client[n].pSendQueue);
		}
	}

	SAFE_DEL_ARRAY(m_Pool);
	SAFE_DEL_ARRAY(m_Client);
	return TRUE;
}


//-----------------------------------------------------------------------------
// �߳��ͻ���
//-----------------------------------------------------------------------------
VOID XServer::Kick(DWORD dwHandle)
{ 
	PreDestroyClient((tagXClient*)dwHandle, NULL);
}

//-----------------------------------------------------------------------------
// setup acceptEx thread
//-----------------------------------------------------------------------------
BOOL XServer::CreateSetupAccepExThread()
{
	// ����setup acceptEx�߳�
	INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadSetupAcceptEx), 0, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create setup acceptex thread failed"));
		return FALSE;
	}

	// ����event
	::SetEvent(m_hEventAccepEx);
	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// thread setup acceptEx
//-----------------------------------------------------------------------------
UINT XServer::ThreadSetupAcceptEx(LPVOID)
{
	tagXClient* pClient = NULL;
	BOOL bResult = FALSE;
	DWORD dwBytes = 0;

	while( FALSE == m_bTerminateWorkThread )
	{
		while( m_nAcceptExs < m_Config.nAcceptExNum 
			&& GT_VALID(pClient = m_listFreeClient.PopFront()) )
		{
			pClient->bShutDown = FALSE;		// �Ƿ��Ѿ���shutdown
			pClient->sock = socket(AF_INET, SOCK_STREAM, 0);
			pClient->dwClientID = GT_INVALID;
			pClient->dwConnectTime = GT_INVALID;
			pClient->lSendCast = 0;
			pClient->lRecvCast = 1;
			pClient->dwSendSize = 0;
			
			tagUnit* pUnit = (tagUnit*)m_Pool[0].Alloc(UNIT_SIZE(PARAM_SIZE));
			pUnit->pPool = &m_Pool[0];
			pUnit->ov.hEvent = NULL;
			pUnit->eState = EXS_Accept;
			pUnit->pNext = NULL;
			((tagXLoginParam*)(&pUnit->pBuffer[PARAM_INDEX]))->dwHandle = (DWORD)pClient;
			
			bResult = m_lpfnAcceptEx(m_sockListen, pClient->sock, pUnit->pBuffer, 0, 
				sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &dwBytes, (OVERLAPPED*)pUnit);//m_lpfnAcceptEx
			if( !bResult && ERROR_IO_PENDING != WSAGetLastError() )
			{
				DestroyClient(pClient);
				pUnit->pPool->Free(pUnit);
				continue;
			}

			InterlockedIncrement((LONG*)&m_nAcceptExs);
		}

		WaitForSingleObject(m_hEventAccepEx, X_SLEEP_TIME);
	}

	shutdown(m_sockListen, SD_BOTH);
	closesocket(m_sockListen);
	return 0;
}


//-----------------------------------------------------------------------------
// create worker thread
//-----------------------------------------------------------------------------
BOOL XServer::CreateWorkThread(INT nThreadNum)
{
	m_Pool = new SafeMemPool[nThreadNum+1];
	m_nPoolNum = nThreadNum+1;

    for(INT n=1; n<=nThreadNum; n++)
    {
		m_Pool[n].SetMaxSize(m_Config.dwPoolSize);

		// ���������߳�
        INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)
			m_Trunk.sfp1(&vEngine::XServer::ThreadWorker), (LPVOID)n, 0, NULL);
		if( 0 == nReturn || -1 == nReturn )
		{
			ERR(_T("[s]create worker thread failed"));
			return FALSE;
		}
		m_vecThreadHandle.push_back((HANDLE)nReturn);
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
// work thread
//-----------------------------------------------------------------------------
UINT XServer::ThreadWorker(INT	nThreadIndex)
{
	INT nResult = 0;
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	BOOL bDontLinger = TRUE;
	sockaddr_in *pRemoteAddr, *pLocalAddr;
	DWORD dwRemoteAddrSize = 0, dwLocalAddrSize = 0;
	
	WSAOVERLAPPED*	pOverlapped = NULL;
	tagXClient* pClient = NULL;
	tagUnit* pUnitGot = NULL;
	tagUnit* pUnit = NULL;

	WSABUF wbuf;
	DWORD dwBytesNeed = 0;
	DWORD dwBytesComp = 0;
        
	while( !m_bTerminateWorkThread )
	{
		// �õ����״̬
		nResult = GetQueuedCompletionStatus(m_hCompletionPort, &dwBytesComp, (DWORD*)&pClient, &pOverlapped, X_SLEEP_TIME);
		if( !pOverlapped )
			continue;

		pUnitGot = CONTAINING_RECORD(pOverlapped, tagUnit, ov);
		if( EXS_Accept == pUnitGot->eState )	// accept����
		{
			pClient = (tagXClient*)(((tagXLoginParam*)(&pUnitGot->pBuffer[PARAM_INDEX]))->dwHandle);
			InterlockedDecrement((LONG*)&m_nAcceptExs);	// �ٴ���AcceptEx
			SetEvent(m_hEventAccepEx);
		}

		if( !nResult )	// ��ɶ˿��յ�ʧ��֪ͨ
		{
			PreDestroyClient(pClient, pUnitGot); 
			continue;
		}

		switch( pUnitGot->eState )
		{
		case EXS_Accept:
			setsockopt(pClient->sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_sockListen, sizeof(m_sockListen));
			setsockopt(pClient->sock, SOL_SOCKET, SO_DONTLINGER, (char*)&bDontLinger, sizeof(bDontLinger));
			m_lpfnGetAcceptExSockAddr(pUnitGot->pBuffer, 0, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, 
				(sockaddr**)&pLocalAddr, (LPINT)&dwLocalAddrSize, (sockaddr**)&pRemoteAddr, (LPINT)&dwRemoteAddrSize);
			((tagXLoginParam*)(&pUnitGot->pBuffer[PARAM_INDEX]))->dwAddress = pRemoteAddr->sin_addr.S_un.S_addr;
			((tagXLoginParam*)(&pUnitGot->pBuffer[PARAM_INDEX]))->dwPort = pRemoteAddr->sin_port;

			if( !CreateIoCompletionPort((HANDLE)pClient->sock, m_hCompletionPort, (ULONG_PTR)pClient, 0) )	// ����ɶ˿�
			{
				PreDestroyClient(pClient, pUnitGot); 
				break;
			}

			pClient->dwConnectTime = timeGetTime();
			m_listUnauthClient.PushBack(pClient);	// δ��֤�ͻ�

			// ����һ��recv length�ص�IO����
			pUnit = (tagUnit*)m_Pool[nThreadIndex].Alloc(UNIT_SIZE(m_Config.dwMaxRecvSize));
			pUnit->pPool = &m_Pool[nThreadIndex];
			pUnit->ov.hEvent = NULL;
			pUnit->eState = EXS_RecvLength;
			pUnit->dwBytesReady = 0;
			pUnit->dwSize = sizeof(DWORD);
			pUnit->pNext = pUnitGot;

			wbuf.buf = pUnit->pBuffer;
			wbuf.len = pUnit->dwSize;
			if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnit->ov, NULL) )
			{
				if( WSA_IO_PENDING != WSAGetLastError() )
					PreDestroyClient(pClient, pUnit);
			}	
			break;

		case EXS_RecvLength:	// ���������
			if( dwBytesComp == 0 )
			{
				PreDestroyClient(pClient, pUnitGot);
				break;
			}

			dwBytesNeed = pUnitGot->dwSize - pUnitGot->dwBytesReady;
			pUnitGot->dwBytesReady += dwBytesComp;
			if( dwBytesComp == dwBytesNeed )
			{
				pUnitGot->dwBytesReady = 0;
				pUnitGot->dwSize = (*(DWORD*)&pUnitGot->pBuffer[0]);
				XCS_FIX_SIZE(pUnitGot->dwSize);		//	�������
				if( pUnitGot->dwSize > m_Config.dwMaxRecvSize )	// �ͻ��˷��������İ�
				{
					PreDestroyClient(pClient, pUnitGot);
					break;
				}
				pUnitGot->ov.hEvent = NULL;
				pUnitGot->eState = EXS_Recv;
				wbuf.buf = pUnitGot->pBuffer;
				wbuf.len = pUnitGot->dwSize;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnitGot->ov, NULL) )
				{
					if( WSA_IO_PENDING != WSAGetLastError() )
						PreDestroyClient(pClient, pUnitGot);
				}	
			}
			else
			{
				pUnitGot->ov.hEvent = NULL;
				wbuf.buf = pUnitGot->pBuffer + pUnitGot->dwBytesReady;
				wbuf.len = pUnitGot->dwSize - pUnitGot->dwBytesReady;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnitGot->ov, NULL) )
				{
					if( WSA_IO_PENDING != WSAGetLastError() )
						PreDestroyClient(pClient, pUnitGot);
				}		
			}
			break;

		case EXS_Recv:	
			if( dwBytesComp == 0 )
			{
				PreDestroyClient(pClient, pUnitGot);
				break;
			}

			dwBytesNeed = pUnitGot->dwSize - pUnitGot->dwBytesReady;
			pUnitGot->dwBytesReady += dwBytesComp;
			if( dwBytesComp == dwBytesNeed )	// ʵ�ʽ������
			{
				if( pUnitGot->pNext )	// ��ҵĵ�һ����
				{
					if( TRUE == m_listUnauthClient.Erase(pClient) )
					{
						// �յ���һ����,�û���Ŀ����,֪ͨ�ⲿ,���ûص�����
						tagXLoginParam* pParam = (tagXLoginParam*)(&pUnitGot->pNext->pBuffer[PARAM_INDEX]);
						pClient->dwClientID = (m_Config.fnLogIn)(pUnitGot, pParam);
						if( GT_INVALID == pClient->dwClientID )	// δͨ�������֤,�߳�
						{
							PreDestroyClient(pClient, pUnitGot);
							break;
						}

						pUnitGot->pNext->pPool->Free(pUnitGot->pNext);
						pUnitGot->pNext = NULL;
					}
					else	// �û��Ѿ�������֤ʱ��
					{
						PreDestroyClient(pClient, pUnitGot);
						break;
					}
				}

				pClient->pRecvQueue->Add(pUnitGot);	// ���յ��ļ�����ջ���

				pUnit = (tagUnit*)m_Pool[nThreadIndex].Alloc(UNIT_SIZE(m_Config.dwMaxRecvSize));
				pUnit->pPool = &m_Pool[nThreadIndex];
				pUnit->ov.hEvent = NULL;
				pUnit->eState = EXS_RecvLength;
				pUnit->dwSize = sizeof(DWORD);
				pUnit->dwBytesReady = 0;
				pUnit->pNext = NULL;

				wbuf.buf = pUnit->pBuffer;
				wbuf.len = pUnit->dwSize;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnit->ov, NULL) )
				{
					if( WSA_IO_PENDING != WSAGetLastError() )
						PreDestroyClient(pClient, pUnit);
				}				
			}
			else
			{
				pUnitGot->ov.hEvent = NULL;
				wbuf.buf = pUnitGot->pBuffer + pUnitGot->dwBytesReady;
				wbuf.len = pUnitGot->dwSize - pUnitGot->dwBytesReady;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnitGot->ov, NULL) )
				{
					if( WSA_IO_PENDING != WSAGetLastError() )
						PreDestroyClient(pClient, pUnitGot);
				}
			}
			break;

		case EXS_Send:	// �����������,����unit���ڴ��
			pClient->dwSendSize -= pUnitGot->dwSize;
			pUnitGot->pPool->Free(pUnitGot);
			InterlockedDecrement(&pClient->lSendCast);
			break;

		default:
			{
				//	...
				IMSG(_T("heat beat map !!!\r\n"));
				__asm int 3;
			}	break;	// ״̬�쳣
		}
	}
	
	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// create send thread
//-----------------------------------------------------------------------------
BOOL XServer::CreateSendThread()
{
    INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadSend), NULL, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("create send thread failed"));
		return FALSE;
    }

	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// send thread
//-----------------------------------------------------------------------------
UINT XServer::ThreadSend(LPVOID)
{
	CHAR*		pBuff = new CHAR[m_Config.dwPerTickSendSize];
	tagRawUnit*	pRaw = NULL;
	tagUnit*	pUnit = NULL;
	DWORD		dwBytes = 0;
	DWORD		dwTotal = 0;
	WSABUF		wbuf;
	INT			nFakeRandom = ( timeGetTime() % (m_nPoolNum-1) ) + 1;
	tagXClient* pClient = NULL;
	tagFilterParam param;
	param.bCompress = TRUE;
	param.bSuccess = FALSE;
	param.eFilter = FMF_MiniLzo;
	param.lpMemIn = pBuff;
	param.dwInBufferSize = m_Config.dwPerTickSendSize;

	while( !m_bTerminateWorkThread )
	{
		for(INT n=0; n<m_Config.nMaxServerLoad; ++n)	// ע���ڲ���break
		{
			pClient = &m_Client[n];
			if( 0 == pClient->pSendQueue->GetNum() )
				continue;

			if( pClient->bShutDown )
			{
				while( pClient->pSendQueue->GetNum() > 0 )
				{
					pRaw = pClient->pSendQueue->Get();
					InterlockedDecrement(&pClient->lSendCast);
					pRaw->pPool->Free(pRaw);
				}
				continue;
			}

			InterlockedIncrement(&pClient->lSendCast);

			// ƴ��
			dwTotal = 0;
			while( pClient->pSendQueue->GetNum() > 0 )
			{
				pRaw = pClient->pSendQueue->Get();
				if(pRaw->dwSize + dwTotal >= m_Config.dwPerTickSendSize)
				{
					pClient->pSendQueue->AddFront(pRaw);
					break;
				}
				InterlockedDecrement(&pClient->lSendCast);
				pClient->dwSendSize -= pRaw->dwSize;
				memcpy(pBuff + dwTotal, pRaw->pBuffer, pRaw->dwSize);
				dwTotal += pRaw->dwSize;
				pRaw->pPool->Free(pRaw);
			}

			if( dwTotal == 0 )	// �հ���
			{
				InterlockedDecrement(&pClient->lSendCast);
				continue;
			}

			// Ѱ�ҿ��Խ����������ڴ��
			for( pUnit=0;!pUnit; ) 
			{
				if( ++nFakeRandom >= m_nPoolNum )
					nFakeRandom = 1;
				pUnit = (tagUnit*)m_Pool[nFakeRandom].TryAlloc(UNIT_SIZE(dwTotal)+LZO_ADD(dwTotal)+sizeof(DWORD)*2);
			} 

			// ѹ��
			param.dwInDataSize = dwTotal;
			param.dwOutBufferSize = dwTotal + LZO_ADD(dwTotal);
			param.dwOutDataSize = 0;
			param.lpMemOut = pUnit->pBuffer + sizeof(DWORD) * 2;
			m_pFilter->Filter(&param);

			// �ڰ�ͷ��д��С��ѹ��ǰ��С
			*(DWORD*)pUnit->pBuffer = param.dwOutDataSize;
			*(DWORD*)(pUnit->pBuffer + sizeof(DWORD)) = dwTotal;
			pUnit->dwSize = param.dwOutDataSize + sizeof(DWORD) * 2;
			pUnit->ov.hEvent = NULL;
			pUnit->dwBytesReady = 0;
			pUnit->pPool = &m_Pool[nFakeRandom];
			pUnit->eState = EXS_Send;
			pUnit->pNext = NULL;

			pClient->dwSendSize += pUnit->dwSize;

			wbuf.buf = pUnit->pBuffer;
			wbuf.len = pUnit->dwSize;
			if( WSASend(pClient->sock, &wbuf, 1, &dwBytes, 0, &pUnit->ov, NULL) )
			{
				if( WSA_IO_PENDING != WSAGetLastError() )
				{
					PreDestroyClient(pClient, pUnit);
					break;
				}
			}
			
			m_n64SendSize += dwTotal + sizeof(DWORD) * 2; 
			m_n64RealSendSize += param.dwOutDataSize + sizeof(DWORD);
		}
		WaitForSingleObject(m_hEventSend, X_SLEEP_TIME);
	}
	
	SAFE_DEL_ARRAY(pBuff);
	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// �����߳�δ��֤�ͻ����߳�
//-----------------------------------------------------------------------------
BOOL XServer::CreateUnauthHeartbeatThread()
{
	INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadUnauthHeartbeat), (LPVOID)(m_Config.dwAuthWaitTime), 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create heart beat to unauth client thread failed"));
		return FALSE;
	}

	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// �߳�δ��֤�ͻ����߳�
//-----------------------------------------------------------------------------
UINT XServer::ThreadUnauthHeartbeat(DWORD dwWaitForAuthTime)
{
	tagXClient* pClient = NULL;

	while( !m_bTerminateWorkThread )
	{
		if( m_listUnauthClient.Size() > 0 )
		{
			DWORD dwTime = timeGetTime();
			m_listUnauthClient.Lock();
			m_listUnauthClient._ResetIterator();
			while( m_listUnauthClient._Peek(pClient) )
			{
				if( dwTime - pClient->dwConnectTime > dwWaitForAuthTime )
				{
					PreDestroyClient(pClient, NULL);
					m_listUnauthClient._EraseCurrent();
					break;
				}
				m_listUnauthClient._AddIterator();
			}
			m_listUnauthClient.Unlock();
		}
		
		Sleep(X_SLEEP_TIME);
	}

	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// ������Դ
//-----------------------------------------------------------------------------
VOID XServer::PreDestroyClient(tagXClient* pClient, tagUnit* pUnit)
{
	if( FALSE == InterlockedCompareExchange((LPLONG)(&pClient->bShutDown), TRUE, FALSE) )
	{
		shutdown(pClient->sock, SD_BOTH);

		if( GT_VALID(pClient->dwClientID) )
			(m_Config.fnLogOut)(pClient->dwClientID);	// �ص��û��ǳ�����
			
		m_listDestroyClient.PushBack(pClient);
		SetEvent(m_hEventDestroy);

		closesocket(pClient->sock);
	}

	if( pUnit )
	{
		if( EXS_Send == pUnit->eState )	// �������ü���
			InterlockedDecrement(&pClient->lSendCast);
		else
			InterlockedDecrement(&pClient->lRecvCast); 

		if( pUnit->pNext )	// �ͷ�unit
			pUnit->pNext->pPool->Free(pUnit->pNext);
		pUnit->pPool->Free(pUnit);
	}
}


//-----------------------------------------------------------------------------
// ������Դ�����߳�
//-----------------------------------------------------------------------------
BOOL XServer::CreateDestroyThread()
{
	INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadDestroy), 0, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create destroy client thread failed"));
		return FALSE;
	}

	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ��Դ�����߳�
//-----------------------------------------------------------------------------
UINT XServer::ThreadDestroy(LPVOID)
{
	tagXClient*	pClient = NULL;

	while( !m_bTerminateWorkThread )
	{
		if( !GT_VALID(pClient = m_listDestroyClient.PopFront()) )
		{
			WaitForSingleObject(m_hEventDestroy, X_SLEEP_TIME);
			continue;
		}

		if( 0 == pClient->lSendCast && 0 == pClient->lRecvCast )
		{
			DestroyClient(pClient);	// ����������һ���
		}
		else
		{
			WaitForSingleObject(m_hEventDestroy, X_SLEEP_TIME);
			m_listDestroyClient.PushBack(pClient);
		}
	}

	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// ������Դ
//-----------------------------------------------------------------------------
VOID XServer::DestroyClient(tagXClient* pClient)
{
	m_listUnauthClient.Erase(pClient);

	// �������յ������а�������
	tagUnit* pUnit = NULL;
	while( pUnit = pClient->pRecvQueue->Get() )
		pUnit->pPool->Free(pUnit);

	tagRawUnit* pSendUnit = NULL;
	while( pSendUnit = pClient->pSendQueue->Get() )
		pSendUnit->pPool->Free(pSendUnit);
	
	pClient->bShutDown = TRUE;
	pClient->lSendCast = 0;
	pClient->lRecvCast = 0;
	pClient->dwSendSize = 0;
	pClient->sock = INVALID_SOCKET;
	pClient->dwClientID = GT_INVALID;	// �ͻ���ID,�ϲ�ȷ�ϵ�һ�����Ժ����Ч
		
	// �ջ�client data
	m_listFreeClient.PushBack(pClient);
	
}

} // namespace vEngine {
