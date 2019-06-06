//-----------------------------------------------------------------------------
//!\file transport.cpp
//!\author Lyp
//!
//!\date 2004-04-03
//! last 2008-04-17
//!
//!\brief �������˴���ײ�
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "transport.h"
#include "crypt.h"

#include "..\system\sysinfo.h"
#include "..\debug\mini_dump.h"
namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
CompletionTransport::CompletionTransport():m_Trunk(this)
{
	m_pAcceptUnitPool = NULL;
	ZeroMemory(m_pUnitPool, sizeof(m_pUnitPool));
	m_pSendQueue = NULL;

	m_pCrypt = new SectCrypt();
	ZeroMemory(m_szInfo, sizeof(m_szInfo));


	// ��½ע���ص�����
	m_fnLogIn = NULL;
	m_fnLogOut = NULL;
	// ���ܽ���unit�ص�����
	m_fnEncrypt = (CRYPTCALLBACK)m_Trunk.sfp1(&vEngine::CompletionTransport::DefaultEncryptFunction);
	m_fnDecrypt = (CRYPTCALLBACK)m_Trunk.sfp1(&vEngine::CompletionTransport::DefaultDecryptFunction);
	// �Ƿ��û��ٱ��ص�����
	m_fnHackReport = NULL;

	m_hCompletionPort = 0;

	m_nPort = 0;
	m_bTerminateWorkThread = FALSE;
	m_bPreShutdown = FALSE;	// �ر�socket�߳��Ƿ����
	m_lThreadCounter = 0;

	m_nSmallPool = 1;
	m_nBigPool = 1;
	m_bOverBalance = FALSE;

	m_hEventSetupAccepEx
		= ::CreateEvent(NULL, FALSE, TRUE, NULL);	// �Զ�����;
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
CompletionTransport::~CompletionTransport()
{
	this->Destroy();
	CloseHandle(m_hEventSetupAccepEx);
	delete(m_pCrypt);
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL CompletionTransport::Init(tagServerInitParam* pInitParam)
{
	if( P_VALID(pInitParam) )
		memcpy(&m_InitParam, pInitParam, sizeof(m_InitParam));

	m_nPort = m_InitParam.nPort;

	// �������ͻ����غ�����
	m_nMaxLoad = m_InitParam.nMaxServerLoad;

	// ����client data
	m_dwClientSerialID = 0;
	m_ClientData = new tagClientData[m_nMaxLoad];
	if( NULL == m_ClientData )
		return FALSE;
	ZeroMemory(m_ClientData, sizeof(tagClientData)*m_nMaxLoad);

	m_listFreeClientData.Clear();

	
	for(INT n=0; n<m_nMaxLoad; n++)	// ���� index
		m_ClientData[n].nIndex = n;
	for(INT n=0; n<m_nMaxLoad; n++)	// ȫ������
		m_listFreeClientData.PushBack(&m_ClientData[n]);

	m_listCloseClient.Clear();
	m_listUnauthClient.Clear();
	m_vecWorkThreadHandle.clear();
	m_mapClientSerialIndex.Clear();	// Client SerialID -> index

    // Open a TCP socket connection to the server.
    m_sockListener = socket(AF_INET, SOCK_STREAM, 0);
    if( m_sockListener < 0 )
    {
		ERR(_T("socket() failed"));
        return FALSE;
    }
	

	// reuse��ɶ�ΰ󶨳ɹ������ܲ����ҵ���ȷ���õĶ˿�
	// �����̨����������ͬһ̨������ʹ��ͬһ���˿ڣ���ɻ���
	// set addr reusabel
	if( pInitParam->bReusePort )
	{
		DWORD dwReUseAddr = TRUE;	
		setsockopt( m_sockListener, SOL_SOCKET, SO_REUSEADDR, 
			(char*)&dwReUseAddr, sizeof(dwReUseAddr) );
	}

	// disables the Nagle algorithm for send coalescing
	BOOL bNoDelay = TRUE;
	setsockopt(m_sockListener, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));

    // Bind our local address
	sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port        = htons(m_nPort);

	INT nErr = bind( m_sockListener, (sockaddr*)&address, sizeof(address) );
    while( SOCKET_ERROR == nErr )
	{
		INT nResult = WSAGetLastError();
		if( WSAEADDRINUSE == nResult )
		{
			++m_nPort;
			address.sin_port = htons(m_nPort);
			nErr = bind( m_sockListener, (sockaddr*)&address, sizeof(address) );
		}
		else
		{
			closesocket(m_sockListener);
			ERR(_T("bind() failed"));
			return FALSE;
		}
	}

	// create the completion port
    m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    if( m_hCompletionPort == NULL )
	{
		closesocket(m_sockListener);
		ERR(_T("CreateIoCompletionPort() failed"));
		return FALSE;
	}

	// ���ô�socketΪ����socket
    listen( m_sockListener, SOMAXCONN );

	// Associate the socket with the completion port
	// Use default # of threads
	::CreateIoCompletionPort((HANDLE)m_sockListener, m_hCompletionPort, (DWORD)0, 0);
	
	// ��������ļ��������๤���߳���Ŀ
	m_vecWorkThreadHandle.clear();

	// �������ļ�ָ������Ŀ���������߳�
	INT nPerCPU = m_InitParam.nPerCpuWorkerThread;
	INT nAddition = m_InitParam.nAdditionalWorkerThread;
	INT nWorkerThreadNum = TObjRef<SysInfo>()->GetProcessorNum() * nPerCPU + nAddition;

	if( nWorkerThreadNum > MAX_UNIT_POOL_NUM - 1 )
		nWorkerThreadNum = MAX_UNIT_POOL_NUM -1;

	m_lThreadCounter = 0;
	CreateWorkThread(nWorkerThreadNum);

	// ����accept unit pool
	m_pAcceptUnitPool = new UnitPool;
	if( NULL == m_pAcceptUnitPool )
	{
		ERR(_T("create accept unit pool failed"));
		return FALSE;
	}

	INT nAcceptExNum = m_InitParam.nAcceptExNum;
	if( FALSE == m_pAcceptUnitPool->Init(nAcceptExNum*2) )
	{
		ERR(_T("init accept unit pool failed"));
		return FALSE;
	}

	// ����SetupAccpetEx�߳�
	if( FALSE == CreateSetupAccepExThread(nAcceptExNum) )
		return FALSE;

	// ����send�߳�
	if( FALSE == CreateSendThread() )
		return FALSE;
	
	// ����close socket�߳�
	if( FALSE == CreateCloseSocketThread() )
		return FALSE;

	// ����unauth heartbeat�߳�
	if( FALSE == CreateUnauthHeartbeatThread() )
		return FALSE;

	// ����pool balance�߳�
	if( FALSE == CreatePoolBalanceThread() )
		return FALSE;

#ifdef _DEBUG_UNIT
	mapUnitAddress.Clear(); // FOR DEBUG 
#endif

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL CompletionTransport::Destroy()
{
#ifdef _DEBUG_UNIT
	this->CheckUnit();
#endif

	SAFE_CLOSE_HANDLE(m_hCompletionPort);

	// �ȴ������߳̽���
	InterlockedExchange((LONG*)&m_bTerminateWorkThread, TRUE);
	for(INT n=(INT)m_vecWorkThreadHandle.size()-1; n>=0; n--)
	{
		if( m_vecWorkThreadHandle[n] )
		{
			WaitForSingleObject(m_vecWorkThreadHandle[n], INFINITE);
			SAFE_CLOSE_HANDLE(m_vecWorkThreadHandle[n]);
		}
	}

	SAFE_DEL(m_pAcceptUnitPool);

    for(INT n=0; n<MAX_UNIT_POOL_NUM; n++)
		SAFE_DEL(m_pUnitPool[n]);

	SAFE_DEL(m_pSendQueue);

	if( m_ClientData )
	{
		for(INT n=0; n<m_nMaxLoad; n++)	// ���� index
		{
			if( m_ClientData[n].bValid )
			{
				closesocket(m_ClientData[n].sock);

				// ����������assert˵�������ϲ�δ�����������Ϣ����
				// ��recv queue��resend queue
				SAFE_DEL(m_ClientData[n].pRecvQueue);
			}
		}
	}

	SAFE_DEL_ARRAY(m_ClientData);
	return TRUE;
}



//-----------------------------------------------------------------------------
// ע���½ע���ص�����
//-----------------------------------------------------------------------------
VOID CompletionTransport::SetLogCallback(LOGINCALLBACK fnLogIn, 
										  LOGOUTCALLBACK fnLogOut)
{
	m_fnLogIn = fnLogIn;
	m_fnLogOut = fnLogOut;
}

//-----------------------------------------------------------------------------
// ���ü��ܽ���unit�ص�����
//-----------------------------------------------------------------------------
VOID CompletionTransport::SetSecureCallBack(CRYPTCALLBACK fnEncrypt, 
												CRYPTCALLBACK fnDecrypt)
{
	m_fnEncrypt = fnEncrypt;
	m_fnDecrypt = fnDecrypt;
}

//-----------------------------------------------------------------------------
// ���÷Ƿ��û��ٱ��ص�����
//-----------------------------------------------------------------------------
VOID CompletionTransport::SetHackReportCallBack(HACKREPORTCALLBACK fnHackReport)
{
	m_fnHackReport = fnHackReport;
}


//-----------------------------------------------------------------------------
// setup acceptEx thread
//-----------------------------------------------------------------------------
BOOL CompletionTransport::CreateSetupAccepExThread(INT nAcceptNum)
{
	// �ų�ָ��������AcceptEx
	m_nAcceptExs = 0;

	// ����setup acceptEx�߳�
	INT nReturn = _beginthreadex(NULL, 0, 
		(THREADPROC)m_Trunk.sfp1(&vEngine::CompletionTransport::ThreadSetupAcceptEx), 
		(LPVOID)nAcceptNum, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create setup acceptex thread failed"));
		return FALSE;
	}

	m_vecWorkThreadHandle.push_back((HANDLE)nReturn);
	// ����event
	::SetEvent(m_hEventSetupAccepEx);	

	return TRUE;
}


//-----------------------------------------------------------------------------
// thread setup acceptEx
//-----------------------------------------------------------------------------
UINT CompletionTransport::ThreadSetupAcceptEx(LPVOID p)
{
	EXCEPTION_PROTECT_START;;

	INT nRequiredAcceptNum = (INT)p;
	SOCKET sockIncome;

	while( FALSE == m_bTerminateWorkThread )
	{
		while( m_nAcceptExs < nRequiredAcceptNum )
		{
__get_free_client_data:
			// first ����һ���µ� client data ���ݿ�
			tagClientData* pClient = m_listFreeClientData.PopFront();
			if( FALSE == GT_VALID(pClient) )
			{
				// no free client data, wait and try again
				Sleep(120);
				if( TRUE == m_bTerminateWorkThread ) // ��ֹ��ѭ��
					break;

				goto __get_free_client_data;
			}

			sockIncome = socket(AF_INET, SOCK_STREAM, 0);
			tagUnitData* pUnit = m_pAcceptUnitPool->AllocUnit();

			pClient->bValid = TRUE;
			pClient->bShutDown = FALSE;
			pClient->bClosed = FALSE;
			pClient->bReadyForRecv = FALSE;

			pClient->sock = sockIncome;
			pClient->dwClientID = GT_INVALID;
			pClient->dwSerialID = m_dwClientSerialID;
			pClient->nSendCast = 0;
			pClient->nRecvCast = 1;

			m_mapClientSerialIndex.Add(m_dwClientSerialID, pClient->nIndex);
			m_dwClientSerialID++;

			pUnit->eState = EUS_Accept;
			pUnit->nCDIndex = pClient->nIndex;
			pUnit->wbuf.len = GT_MAX_PACKAGE_LEN;
			pUnit->ov.hEvent = NULL;

			DWORD dwBytes = 0;	// ��ʱʹ��
			BOOL bResult = AcceptEx(m_sockListener, sockIncome, 
				pUnit->wbuf.buf, 0, sizeof(sockaddr_in)+16, 
				sizeof(sockaddr_in)+16, &dwBytes, (OVERLAPPED*)pUnit);

			if( FALSE == bResult && ERROR_IO_PENDING != WSAGetLastError() )
			{
				ClearAndReturnClientData(pClient);
				m_pAcceptUnitPool->FreeUnit(pUnit);
				continue;
			}

			// ��Ҫ���������ɹ�
			InterlockedIncrement((LONG*)&m_nAcceptExs);
		}

		WaitForSingleObject(m_hEventSetupAccepEx, 120);
	}

	shutdown(m_sockListener, SD_BOTH);
	closesocket(m_sockListener);

	EXCEPTION_PROTECT_END
	return 0;
}




//-----------------------------------------------------------------------------
// create worker thread
//-----------------------------------------------------------------------------
BOOL CompletionTransport::CreateWorkThread(INT nThreadNum)
{
	LONG lUnit = m_InitParam.nUnitPerWorkerThread;

    for(INT n=0; n<nThreadNum; n++)
    {
		// ���Ƚ���worker thread��Ӧ��unit pool
		m_pUnitPool[n+1] = new UnitPool;
		if( NULL == m_pUnitPool[n+1] )
		{
			ERR(_T("[s]create unit mem pool failed"));
			return FALSE;
		}
		if( FALSE == m_pUnitPool[n+1]->Init(lUnit) )
		{
			ERR(_T("[s]init unit mem pool failed"));
			return FALSE;
		}

		// ���������߳�
        INT nReturn = _beginthreadex
			(NULL, 0, (THREADPROC)m_Trunk.sfp1(&vEngine::CompletionTransport::ThreadWorker), NULL, 0, NULL);
		if( 0 == nReturn || -1 == nReturn )
		{
			ERR(_T("[s]create worker thread failed"));
			return FALSE;
		}

		m_vecWorkThreadHandle.push_back((HANDLE)nReturn);
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
// work thread
//-----------------------------------------------------------------------------
UINT CompletionTransport::ThreadWorker(LPVOID pDataEntry)
{

	EXCEPTION_PROTECT_START;

	// ��¼�Լ��ı��
	INT	nThreadIndex = InterlockedIncrement((LONG*)&m_lThreadCounter);

	// ��ʱ����
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	DWORD* pKey=0;

	sockaddr_in *pRemoteAddr, *pLocalAddr;
	DWORD dwRemoteAddrSize = 0, dwLocalAddrSize = 0;
	WSABUF wbuf;
	DWORD dwBytesNeed = 0;
	DWORD dwBytesComp = 0;
	BOOL bResult= FALSE;
	WSAOVERLAPPED*	pOverlapped = NULL;
	tagUnitData* pUnitGot = NULL;
        
	while( FALSE == m_bTerminateWorkThread )
	{
		// �õ����״̬
		bResult = ::GetQueuedCompletionStatus(m_hCompletionPort, &dwBytesComp, 
			(DWORD*)&pKey, &pOverlapped, 500);

		if( 0 == bResult )
		{
			if( NULL != pOverlapped )
			{
				switch(GetLastError())
				{
				case WAIT_TIMEOUT:ERR(_T("work thread wait time out"));break;
				case ERROR_NETNAME_DELETED:break;
				case ERROR_NO_SYSTEM_RESOURCES:break;
				case ERROR_SEM_TIMEOUT:break;
				case ERROR_OPERATION_ABORTED:break;
				case ERROR_HOST_UNREACHABLE:break;
				case ERROR_CONNECTION_ABORTED:break;
				default:ERR(_T("work thread error"));break;
				}

				pUnitGot = CONTAINING_RECORD(pOverlapped, tagUnitData, ov);
				tagClientData* pClient = &m_ClientData[pUnitGot->nCDIndex];
			
				switch( pUnitGot->eState )
				{
				case EUS_Accept: // accept ����ʧ��
					// ��ɨclient data,����unit
					this->ClearAndReturnClientData(pClient);
					m_pAcceptUnitPool->FreeUnit(pUnitGot);

					// ����setup acceptex thread event
					InterlockedDecrement((LONG*)&m_nAcceptExs);
					::SetEvent(m_hEventSetupAccepEx);	
					break;

				default:	// ��������ʧ��,�����ж�,������Դ
					this->PerformClientLogout(pUnitGot);
					break;
				}
			}
			continue;
		}

		// �õ�ʵ��ָ��
		pUnitGot = CONTAINING_RECORD(pOverlapped, tagUnitData, ov);
		tagClientData* pClient = &m_ClientData[pUnitGot->nCDIndex];

		// unit״̬����
		switch( pUnitGot->eState )
		{
		case EUS_Accept:
			{
				// ����һЩsocket����
				// �̳�����
				INT nResult = 0;
				nResult = setsockopt(pClient->sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, 
					(char*)&m_sockListener, sizeof(m_sockListener));

				// hard shut down
				linger lin;
				lin.l_linger = 0;
				lin.l_onoff = 1;
				nResult = setsockopt(pClient->sock, SOL_SOCKET, SO_LINGER, 
					(char*)&lin, sizeof(lin));

				// ����ͻ���IP��
				::GetAcceptExSockaddrs(pUnitGot->wbuf.buf, 0, 
					sizeof(sockaddr_in)+16,	sizeof(sockaddr_in)+16, 
					(sockaddr**)&pLocalAddr, (LPINT)&dwLocalAddrSize, 
					(sockaddr**)&pRemoteAddr, (LPINT)&dwRemoteAddrSize);

				// ��¼ ip/port
				pClient->dwAddress = pRemoteAddr->sin_addr.S_un.S_addr;
				pClient->wPort = pRemoteAddr->sin_port;

				// ����setup acceptex thread event
				InterlockedDecrement((LONG*)&m_nAcceptExs);
				::SetEvent(m_hEventSetupAccepEx);

				// Associate the socket with the completion port
				if( NULL == ::CreateIoCompletionPort((HANDLE)pClient->sock, m_hCompletionPort, 0, 0) )
				{
					this->ClearAndReturnClientData(pClient);
					m_pAcceptUnitPool->FreeUnit(pUnitGot);
					break;
				}

				// δ��֤�ͻ�
				pClient->dwConnectTime = timeGetTime();
				m_listUnauthClient.PushBack(pClient->nIndex);

				// ����һ��recv length�ص�IO����
				tagUnitData* pUnit = m_pUnitPool[nThreadIndex]->AllocUnit();
				pUnit->ov.hEvent = NULL;
				pUnit->eState = EUS_RecvLength;
				pUnit->dwBytesReady = 0;
				pUnit->wbuf.len = sizeof(DWORD);
				pUnit->nCDIndex = pUnitGot->nCDIndex;
				pUnit->bFirstRecvUnit = true;	// ��һ�����հ�

				dwFlags = 0;
				if( 0 != WSARecv(pClient->sock, &pUnit->wbuf, 1, &dwBytes, &dwFlags,
					&pUnit->ov, NULL) )
				{
					// �����쳣�����Զ�����unit
					if( WSA_IO_PENDING != WSAGetLastError() )
					{
						// �����ж�,������Դ
						this->PerformClientLogout(pUnit);
					}
				}	

				// ����accept unit
				m_pAcceptUnitPool->FreeUnit(pUnitGot);
			}
			break;

		case EUS_RecvLength:	// �ȶ��������
			{
				pUnitGot->eState = EUS_RecvLengthComplete;
				if( dwBytesComp == 0 )
				{
					// �����ж�,������Դ
					this->PerformClientLogout(pUnitGot);
					break;
				}

				dwBytesNeed = pUnitGot->wbuf.len - pUnitGot->dwBytesReady;
				pUnitGot->dwBytesReady += dwBytesComp;
				if( dwBytesComp == dwBytesNeed )
				{
					pUnitGot->dwBytesReady = 0;
					pUnitGot->wbuf.len = (*(DWORD*)&pUnitGot->wbuf.buf[0]) & 0xffffff;	// ȡ����λ
					pUnitGot->byEncryptNum = *(BYTE*)&pUnitGot->wbuf.buf[3];	// DWORD���λ
					// �ͻ��˿��ܷǷ����������İ�
					if( pUnitGot->wbuf.len > GT_MAX_PACKAGE_LEN )
					{
						// �����ж�,������Դ
						// ����,������������
						pUnitGot->wbuf.len = GT_MAX_PACKAGE_LEN;
						this->PerformClientLogout(pUnitGot);
						break;

						// �Ƿ��û��ٱ��ص�����
						if( m_fnHackReport )
							(m_fnHackReport)(pClient->dwSerialID);
					}

					pUnitGot->eState = EUS_Recv;
					pUnitGot->bFirstUnitOfPackage = true;
					pUnitGot->ov.hEvent = NULL;
					dwFlags = 0;
					if( 0 != WSARecv(pClient->sock, &pUnitGot->wbuf, 1, &dwBytes, 
						&dwFlags, &pUnitGot->ov, NULL) )
					{
						// �����쳣�����Զ�����unit
						if( WSA_IO_PENDING != WSAGetLastError() )
						{
							// �����ж�,������Դ
							this->PerformClientLogout(pUnitGot);
						}
					}	
				}
				else
				{
					// The array of WSABUF structures pointed to by the lpBuffers 
					// parameter is transient. If this operation completes in an 
					// overlapped manner, it is the service provider's responsibility 
					// to capture these WSABUF structures before returning from this 
					// call. This enables applications to build stack-based WSABUF arrays
					wbuf.buf = pUnitGot->wbuf.buf + pUnitGot->dwBytesReady;
					wbuf.len = pUnitGot->wbuf.len - pUnitGot->dwBytesReady;
					pUnitGot->ov.hEvent = NULL;
					pUnitGot->eState = EUS_RecvLength;
					dwFlags = 0;
					if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags,
						&pUnitGot->ov, NULL) )
					{
						// �����쳣�����Զ�����unit
						if( WSA_IO_PENDING != WSAGetLastError() )
						{
							// �����ж�,������Դ
							this->PerformClientLogout(pUnitGot);
						}
					}		
				}
			}
			break;

		case EUS_Recv:	// ʵ�ʽ������
			{
				pUnitGot->eState = EUS_RecvComplete;
				if( dwBytesComp == 0 )
				{
					// �����ж�,������Դ
					this->PerformClientLogout(pUnitGot);
					break;
				}

				dwBytesNeed = pUnitGot->wbuf.len - pUnitGot->dwBytesReady;
				pUnitGot->dwBytesReady += dwBytesComp;

				if( dwBytesComp == dwBytesNeed )
				{
					// ���ûص�����,�����ݰ����н���
					if( 0 != pUnitGot->byEncryptNum )
						(m_fnDecrypt)(pUnitGot);

					// ͨ����һ�������û���ݽ���У��
					if( true == pUnitGot->bFirstRecvUnit )
					{
						if( TRUE == m_listUnauthClient.Erase(pUnitGot->nCDIndex) )
						{
							// �յ���һ����,�û���Ŀ����,֪ͨ�ⲿ,���ûص�����
							// ���ҵ�һ����Ҳ����뵽���ջ���
							tagLoginParam login_param;
							login_param.dwAddress = pClient->dwAddress;	// Զ�̵�ַ
							login_param.wPort = pClient->wPort;	// Զ�̶˿�

							InterlockedExchange((LONG*)&pClient->bReadyForRecv, FALSE);
							pClient->dwClientID = (m_fnLogIn)(pUnitGot, &login_param);

							if( GT_INVALID == pClient->dwClientID )
							{
								// δͨ�������֤,�߳�
								this->PerformClientLogout(pUnitGot);
								break;
							}

							while( NULL == pClient->pRecvQueue )	// ��ͣ��������
							{
								pClient->pRecvQueue = new SafeQueue<tagUnitData*>;
							}

							// ׼���ý�����Ϣ
							InterlockedExchange((LONG*)&pClient->bReadyForRecv, TRUE);
						}
						else	// �û��Ѿ�������֤ʱ��
						{
							this->PerformClientLogout(pUnitGot);
							break;
						}
					}
					
					// �����µ�unit
					tagUnitData* pUnit = m_pUnitPool[nThreadIndex]->AllocUnit();
					pUnit->ov.hEvent = NULL;
					pUnit->eState = EUS_RecvLength;
					pUnit->wbuf.len = sizeof(DWORD);
					pUnit->dwBytesReady = 0;
					pUnit->nCDIndex = pUnitGot->nCDIndex;
					pUnit->bFirstRecvUnit = false;

					// ���յ��ļ�����ջ���
					if( FALSE == pClient->pRecvQueue->Add(pUnitGot) )
					{
						// ���ܻ���������ߵ��ͻ���
						m_pUnitPool[nThreadIndex]->FreeUnit(pUnit);
						this->PerformClientLogout(pUnitGot);
						break;
					}

					// ����һ��recv length�ص�IO����
					dwFlags = 0;
					if( 0 != WSARecv(pClient->sock, &pUnit->wbuf, 1, &dwBytes, &dwFlags,
						&pUnit->ov, NULL) )
					{
						// �����쳣����
						if( WSA_IO_PENDING != WSAGetLastError() )
						{
							// �����ж�,������Դ
							this->PerformClientLogout(pUnit);
						}
					}				
				}
				else
				{
					pUnitGot->eState = EUS_Recv;
					wbuf.buf = pUnitGot->wbuf.buf + pUnitGot->dwBytesReady;
					wbuf.len = pUnitGot->wbuf.len - pUnitGot->dwBytesReady;
					pUnitGot->ov.hEvent = NULL;
					dwFlags = 0;
					if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags,
						&pUnitGot->ov, NULL) )
					{
						// �����쳣����
						if( WSA_IO_PENDING != WSAGetLastError() )
						{
							// �����ж�,������Դ
							this->PerformClientLogout(pUnitGot);
						}
					}
				}
			}
			break;

		case EUS_Send:	// �������
			{
				InterlockedDecrement((LONG*)&pClient->nSendCast);

				// �����������,����unit���ڴ��
				if( FALSE == m_pUnitPool[m_nSmallPool]->TryFreeUnit(pUnitGot) )
				{
					BOOL bFreed = FALSE;
					for( INT n=0; n<m_lThreadCounter; n++ )
					{
						if( m_pUnitPool[n+1]->TryFreeUnit(pUnitGot) )
						{
							bFreed = TRUE;
							break;
						}
					}
					if( !bFreed )
						m_pUnitPool[nThreadIndex]->FreeUnit(pUnitGot);
				}

			}	// US_Send
			break;

		default:	// ����û�з�����
			ASSERT(0);
			TObjRef<Log>()->Write(_T("err unit state %d\r\n"), pUnitGot->eState);
			
			if( FALSE == m_pUnitPool[m_nSmallPool]->TryFreeUnit(pUnitGot) )
				m_pUnitPool[nThreadIndex]->FreeUnit(pUnitGot);
			break;
		}
	}
	
	EXCEPTION_PROTECT_END;
	_endthreadex(0);
	return 0;
}





//-----------------------------------------------------------------------------
// create send thread
//-----------------------------------------------------------------------------
BOOL CompletionTransport::CreateSendThread()
{
	// ���Ƚ���send thread��Ӧ��send queue
	m_pSendQueue = new SafeQueue<tagUnitData*>(TRUE);
	if( NULL == m_pSendQueue )
	{
		ERR(_T("create send unit queue failed"));
		return FALSE;
	}

    INT nReturn = _beginthreadex
		(NULL, 0, (THREADPROC)m_Trunk.sfp1(&vEngine::CompletionTransport::ThreadSend), NULL, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("create send thread failed"));
		return FALSE;
    }

	SetThreadPriority((HANDLE)nReturn, HIGH_PRIORITY_CLASS);

	m_vecWorkThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// send thread
//-----------------------------------------------------------------------------
UINT CompletionTransport::ThreadSend(LPVOID)
{
	EXCEPTION_PROTECT_START;

	tagUnitData*	pUnit = NULL;
	INT				nReturn = 0;
	DWORD			dwBytes = 0;

	while( FALSE == m_bTerminateWorkThread )
	{
		for(;;)	// ע���ڲ���break
		{
			pUnit = m_pSendQueue->Get();
			if( NULL == pUnit )
				break;	// ����Ϊֹ

			tagClientData* pClient = &m_ClientData[pUnit->nCDIndex];
			if( TRUE == pClient->bShutDown )
			{
				// ���˿����Ѿ��ǳ�
				InterlockedDecrement((LONG*)&pClient->nSendCast);
				this->ReturnUnit(pUnit);
				continue;
			}

			// ����unit�ص�����
			if( pUnit->byEncryptNum )
				(m_fnEncrypt)(pUnit);

			pUnit->ov.hEvent = NULL;
			pUnit->dwBytesReady = 0;

			nReturn = WSASend(pClient->sock, &pUnit->wbuf, 1, &dwBytes, 0, &pUnit->ov, NULL);
			if( nReturn )
			{
				if( WSA_IO_PENDING != WSAGetLastError() )
				{
					// ��������,�����ж�,������Դ
					this->PerformClientLogout(pUnit);
				}
			}
		}

		WaitForSingleObject(m_pSendQueue->GetEvent(), 120);
	}
	
	EXCEPTION_PROTECT_END;
	_endthreadex(0);
	return 0;
}




//-----------------------------------------------------------------------------
// ����ƽ�� unit pool �߳�
//-----------------------------------------------------------------------------
BOOL CompletionTransport::CreatePoolBalanceThread()
{
    INT nReturn = _beginthreadex
		(NULL, 0, (THREADPROC)m_Trunk.sfp1(&vEngine::CompletionTransport::ThreadPoolBalance), NULL, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create pool balance thread failed"));
		return FALSE;
    }

	m_vecWorkThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}



//-----------------------------------------------------------------------------
// ƽ�� unit pool �߳�
//-----------------------------------------------------------------------------
UINT CompletionTransport::ThreadPoolBalance(LPVOID)
{
	EXCEPTION_PROTECT_START;

	INT nSmallPool = 1;
	INT nBigPool = 1;
	INT nSmallPoolSize = 0x7fffffff;
	INT nBigPoolSize = 0;
	BOOL bChanged = FALSE;
	INT nTotalSize = 0;

	while( FALSE == m_bTerminateWorkThread )
	{
		nSmallPool = 1;
		nBigPool = 1;
		nSmallPoolSize = 0x7fffffff;
		nBigPoolSize = 0;
		bChanged = FALSE;
		nTotalSize = 0;

		m_szInfo[0] = 0;
		// ͳ�Ƴ���С�����unit��
		INT nAllSize = 0;
		for(INT n=0; n<m_lThreadCounter; n++)
		{
			INT nSize = m_pUnitPool[n+1]->GetSize();
			nTotalSize += m_pUnitPool[n+1]->GetTotalNum();	// һ���������ڴ�alloc����unit

			if( nSize <= nSmallPoolSize )
			{
				nSmallPool = n+1;
				nSmallPoolSize = nSize;
				bChanged = TRUE;
			}

			if( nSize > nBigPoolSize )
			{
				nBigPool = n+1;
				nBigPoolSize = nSize;
				bChanged = TRUE;
			}

			nAllSize += nSize;	// ͳ���ܴ�С
		}

		// ��ʾ��unit���������Լ����ǵĺ�
		/*_stprintf( m_szInfo, _T("%d<%d ac%d cd%d cl%d uc%d"), 
			nAllSize, nTotalSize, 
			m_nAcceptExs, m_nMaxLoad-m_listFreeClientData.Size(), 
			m_listCloseClient.Size(), m_listUnauthClient.Size());
		*/
		if( (nSmallPoolSize * 16) < nBigPoolSize )
			InterlockedExchange((LONG*)&m_bOverBalance, TRUE);
		else
			InterlockedExchange((LONG*)&m_bOverBalance, FALSE);

		if( bChanged )
		{
			InterlockedExchange((LONG*)&m_nSmallPool, nSmallPool);
			InterlockedExchange((LONG*)&m_nBigPool, nBigPool);
		}

		Sleep(200);	// run per 200 ms
	}

	EXCEPTION_PROTECT_END;
	_endthreadex(0);
	return 0;
}





//-----------------------------------------------------------------------------
// �õ��ͻ���sockname
//-----------------------------------------------------------------------------
BOOL CompletionTransport::GetClientSockName(sockaddr_in& addr_in, DWORD dwSerialID)
{
	INT nIndex = m_mapClientSerialIndex.Peek(dwSerialID);
	if( GT_INVALID == nIndex )
		return FALSE;

	tagClientData* pClient = &m_ClientData[nIndex];
	if( FALSE == pClient->bValid  )
		return FALSE;

	// ����һ�����Թ��κ�socket����ʹ�õĺϷ���ַ
	ZeroMemory(addr_in.sin_zero, sizeof(addr_in.sin_zero));
	addr_in.sin_family = AF_INET;   // internetwork: UDP, TCP, etc.
	addr_in.sin_addr.S_un.S_addr = pClient->dwAddress; 
	addr_in.sin_port = pClient->wPort; 
	return TRUE;
}



//-----------------------------------------------------------------------------
// �߳��ͻ���
//-----------------------------------------------------------------------------
VOID CompletionTransport::Kick(DWORD dwSerialID)
{
	INT nCDIndex = m_mapClientSerialIndex.Peek(dwSerialID);
	if( nCDIndex != GT_INVALID )
	{
		if( FALSE == m_ClientData[nCDIndex].bClosed )
		{
			shutdown(m_ClientData[nCDIndex].sock, SD_BOTH);
		}
	}
}


//-----------------------------------------------------------------------------
// �������ж�,������Դʱ����
//-----------------------------------------------------------------------------
VOID CompletionTransport::PerformClientLogout(tagUnitData* pUnit)
{
	tagClientData* pClient = &m_ClientData[pUnit->nCDIndex];

	if( FALSE == pClient->bClosed )
	{
		if( FALSE == pClient->bShutDown )
		{
			InterlockedExchange((LONG*)&pClient->bShutDown, TRUE);
			shutdown(pClient->sock, SD_BOTH);

			// �û���Ŀ���� ֪ͨ�ⲿ,���ûص�����
			(m_fnLogOut)(pClient->dwClientID);

			m_listCloseClient.PushBack(pUnit->nCDIndex);
		}
	}


	// ��¼�ǳ�ʱ��
	pClient->dwLogoutTime = timeGetTime();

	switch(pUnit->eState)
	{
	case EUS_Send: 
	case EUS_SendComplete: 
		InterlockedDecrement((LONG*)&pClient->nSendCast); 
		break;
	case EUS_RecvLength: 
	case EUS_RecvLengthComplete:
	case EUS_Recv:
	case EUS_RecvComplete:
		InterlockedDecrement((LONG*)&pClient->nRecvCast); 
		break;
	default:
		ASSERT(0);
		break;
	}

	this->ReturnUnit(pUnit);
}


//-----------------------------------------------------------------------------
// create close socket thread
//-----------------------------------------------------------------------------
BOOL CompletionTransport::CreateCloseSocketThread()
{
	// �������ļ�
	INT nCloseWaitTime = m_InitParam.nCloseSocketWaitTime;

	// �������ж���
	m_listCloseClient.Clear();

	// ����close socket thread
	INT nReturn = _beginthreadex(NULL, 0, 
		(THREADPROC)m_Trunk.sfp1(&vEngine::CompletionTransport::ThreadCloseSocket), 
		(LPVOID)(nCloseWaitTime), 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create close socket thread failed"));
		return FALSE;
	}

	m_vecWorkThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// thread close socket
//-----------------------------------------------------------------------------
UINT CompletionTransport::ThreadCloseSocket(LPVOID p)
{
	EXCEPTION_PROTECT_START;

	DWORD			dwWaitForCloseTime = (DWORD)p;
	INT				nIndex = 0;
	tagClientData*	pClient = NULL;

	while( FALSE == m_bTerminateWorkThread )
	{
		// ����ر�ʱ��thread_worker�����Ѿ�����������unit���ܲ�����ȫ���գ�
		for(;!m_bTerminateWorkThread;)	// ע���ڲ���break
		{
			nIndex = m_listCloseClient.PopFront();
			if( GT_INVALID == nIndex )
				break;	// ������Ϊֹ

			pClient = &m_ClientData[nIndex];
			
			// �ȴ�ʱ�����
			while( timeGetTime() - pClient->dwLogoutTime <= dwWaitForCloseTime )
				WaitForSingleObject(m_listCloseClient.GetEvent(), 1000);

			if( 0 == pClient->nSendCast && 0 == pClient->nRecvCast )
			{
				ClearAndReturnClientData(pClient);	// ������һ���clientdata
			}
			else
			{
				if( FALSE == pClient->bClosed )
				{
					InterlockedExchange((LONG*)&pClient->bClosed, TRUE);
					closesocket(pClient->sock);
				}

				m_listCloseClient.PushBack(nIndex);
			}
		} // for(;;)

		WaitForSingleObject(m_listCloseClient.GetEvent(), 120);
	}

	EXCEPTION_PROTECT_END;
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// create thread heart beat to unauth client
//-----------------------------------------------------------------------------
BOOL CompletionTransport::CreateUnauthHeartbeatThread()
{
	// �������ļ�
	INT nAuthWaitTime = m_InitParam.nAuthWaitTime;

	// �������ж���
	m_listUnauthClient.Clear();

	// ���� heart beat to unauth client thread
	INT nReturn = _beginthreadex
		(NULL, 0, (THREADPROC)m_Trunk.sfp1(&vEngine::CompletionTransport::ThreadUnauthHeartbeat), 
		(LPVOID)(nAuthWaitTime), 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create heart beat to unauth client thread failed"));
		return FALSE;
	}

	m_vecWorkThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}



//-----------------------------------------------------------------------------
// thread heart beat to unauth client
//-----------------------------------------------------------------------------
UINT CompletionTransport::ThreadUnauthHeartbeat(LPVOID p)
{
	EXCEPTION_PROTECT_START;

	DWORD	dwWaitForAuthTime = (DWORD)p;	// �ȴ���֤��ʱ��
	INT		nIndex = 0;

	while( FALSE == m_bTerminateWorkThread )
	{
		m_listUnauthClient.Lock();
		m_listUnauthClient.ResetIterator();

		while( m_listUnauthClient.PeekNext(nIndex) )
		{
			tagClientData* pClient = &m_ClientData[nIndex];
			if( timeGetTime() - pClient->dwConnectTime > dwWaitForAuthTime )
			{
				if( FALSE == pClient->bClosed )
				{
					InterlockedExchange((LONG*)&pClient->bShutDown, TRUE);
					shutdown(pClient->sock, SD_BOTH);
				}

				// ��¼�ǳ�ʱ��
				pClient->dwLogoutTime = timeGetTime();
				m_listCloseClient.PushBack(nIndex);
				m_listUnauthClient.Erase(nIndex);
				break;
			}
		}

		m_listUnauthClient.Unlock();
		Sleep(120);
	}

	EXCEPTION_PROTECT_END;
	_endthreadex(0);
	return 0;
}





//-----------------------------------------------------------------------------
// ������һ���client data
//-----------------------------------------------------------------------------
VOID CompletionTransport::ClearAndReturnClientData(tagClientData* pClient)
{
	if( m_mapClientSerialIndex.Erase(pClient->dwSerialID) )
	{
		if( FALSE == pClient->bClosed )
		{
			InterlockedExchange((LONG*)&pClient->bClosed, TRUE);
			closesocket(pClient->sock);
		}

		// �������յ������а�������
		tagUnitData* pUnit = NULL;
		if( pClient->pRecvQueue )
		{
			while( pUnit = pClient->pRecvQueue->Get() )
				ReturnUnit(pUnit);
		}

		pClient->bValid = FALSE;
		pClient->bShutDown = TRUE;
		pClient->bReadyForRecv = FALSE;

		// �ջ�client data
		m_listFreeClientData.PushBack(pClient);	
	}
}




//-----------------------------------------------------------------------------
// Ĭ�ϼ��ܺ���
//-----------------------------------------------------------------------------
UINT CompletionTransport::DefaultEncryptFunction(tagUnitData* pUnit)
{
	m_pCrypt->Encrypt(pUnit);
	return 0;
}

//-----------------------------------------------------------------------------
// Ĭ�Ͻ��ܺ���
//-----------------------------------------------------------------------------
UINT CompletionTransport::DefaultDecryptFunction(tagUnitData* pUnit)
{
	m_pCrypt->Decrypt(pUnit);
	return 0;
}


//-----------------------------------------------------------------------------
// for DEBUG
// ���unitй©���
// ���unit�Ƿ���ȫ�鷵,����û��,���ҳ����ǵĵ�ַ
//-----------------------------------------------------------------------------
VOID CompletionTransport::CheckUnit()
{
#ifdef _DEBUG_UNIT

	std::list<tagUnitData*>	listUnit;
	std::list<tagUnitData*>::iterator it;
	INT nNum = 0;

	listUnit.clear();
	INT nSize = mapUnitAddress.Size();
	mapUnitAddress.ExportAllKey(listUnit);

	tagUnitData* pUnit = NULL;

	for(it=listUnit.begin(); it!=listUnit.end(); ++it)
	{
		pUnit = *it;
		if( pUnit && pUnit->eState != EUS_Accept )
		{
			// ASSERT(0);
			TObjRef<Log>()->Write("unit%d kind%d\r\n", nNum, pUnit->eState);
			nNum++;
		}
	}

	TObjRef<Log>()->Write("total leaked unit=%d\r\n", nNum);
	TObjRef<Log>()->Write("close client list:%d\r\n", m_listCloseClient.Size());
	
	while( m_listCloseClient.Size() > 0 )
	{
		INT nIndex = m_listCloseClient.PopFront();
		if( GT_INVALID == nIndex )
				break;	// ������Ϊֹ

		tagClientData* pClient = &m_ClientData[nIndex];
			
		// ���쳣��������ļ����
		TObjRef<Log>()->Write("nIndex%d SendCast=%d\r\n", nIndex, pClient->nSendCast);
		TObjRef<Log>()->Write("nIndex%d RecvCast=%d\r\n", nIndex, pClient->nRecvCast);
	}

	TObjRef<Log>()->Write("unit in accept queue:%d\r\n", 
		m_pAcceptUnitPool->GetSize());


#endif // #ifdef _DEBUG_UNIT
}



} // namespace vEngine {
