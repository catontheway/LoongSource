//-----------------------------------------------------------------------------
//!\file stream_transport.cpp
//!\author Lyp
//!
//!\date 2003-03-17
//! last 2008-04-17
//!
//!\brief �������ͻ���ͨѶ�ײ�
/*
  in http://tangentsoft.net/wskfaq/newbie.html#howclose 

	The proper sequence for closing a TCP connection is:
	1.Finish sending data. 
	2.Call shutdown() with the how parameter set to 1. 
	3.Loop on recv() until it returns 0. 
	4.Call closesocket(). 

  in MSDN:
	To assure that all data is sent and received on a connected socket 
	before it is closed, an application should use shutdown to close connection 
	before calling closesocket. For example, to initiate a graceful disconnect:
	1.Call WSAAsyncSelect to register for FD_CLOSE notification. 
	2.Call shutdown with how=SD_SEND. 
	3.When FD_CLOSE received, call recv until zero returned, or SOCKET_ERROR. 
	4.Call closesocket. 

*/
// Copyright (c) 1985-2007 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "stream_transport.h"

// ʹ����������
#include "msg_queue_ts.h"

#define SECT_KIND			256
#define SECT_LEN			512
#define ONE_SECOND			1000

//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
StreamTransport::StreamTransport()
{
	m_bEncrypt = FALSE;
	m_pSect = new BYTE[SECT_KIND*SECT_LEN];
	CreateSect();

	Clear();
}

StreamTransport::~StreamTransport()
{
	Destroy();
	SAFE_DEL_ARRAY(m_pSect);
	::WSACleanup();
}


//-----------------------------------------------------------------------------
// �������Դ
//-----------------------------------------------------------------------------
VOID StreamTransport::CreateSect()
{
	INT nHolder = 0xDeadBeef;
	for(INT n=0; n<SECT_KIND*SECT_LEN; n++)
	{
		INT r = ((nHolder = nHolder * 214013L + 2531011L) >> 16) & 0x7fff;
		m_pSect[n] = (BYTE)r;
	}
}



//-----------------------------------------------------------------------------
// clear
//-----------------------------------------------------------------------------
VOID StreamTransport::Clear()
{
	m_pSendMsgQueue = NULL;
	m_pRecvMsgQueue = NULL;

	m_szIP[0] = 0;
	m_nPort = 0;
	m_Sock = 0;
	m_bNagle = TRUE;

	m_hConnect = 0;
	m_hSend = 0;
	m_hReceive = 0;

	::InterlockedExchange((LONG*)&m_bTerminateConnect, FALSE);
	::InterlockedExchange((LONG*)&m_bTerminateSend, FALSE);
	::InterlockedExchange((LONG*)&m_bTerminateReceive, FALSE);
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);

	ZeroMemory(m_szHostName, sizeof(m_szHostName));
	m_bEncrypt = FALSE;
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL StreamTransport::Init(BOOL bNagle, BOOL bPackageMode)
{
	Clear();

	m_bNagle = bNagle;
	m_bPackageMode = bPackageMode;
	WORD wVersionRequested;
	WSADATA wsaData;
 
	// �ȳ���winsocket2.2
	wVersionRequested = MAKEWORD( 2, 2 );
 	INT nErr = WSAStartup( wVersionRequested, &wsaData );
	if( 0 != nErr ) 
	{
		// ����winsocket2.1
		wVersionRequested = MAKEWORD( 2, 1 );
	 	nErr = WSAStartup( wVersionRequested, &wsaData );
		if( 0 != nErr ) 
		{
			// ����winsocket1.1
			wVersionRequested = MAKEWORD( 1, 1 );
			nErr = WSAStartup( wVersionRequested, &wsaData );
			if( 0 != nErr ) 
			{
				return FALSE;
			}
		}
	}
	
	// ���뷢�ͽ��ܻ���
	m_pSendMsgQueue = new MsgQueueTS(TRUE, m_bPackageMode);
	m_pRecvMsgQueue = new MsgQueueTS(FALSE, FALSE);
	if( NULL == m_pSendMsgQueue || NULL == m_pRecvMsgQueue )
	{
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	//����socket
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if( INVALID_SOCKET == m_Sock )
	{
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	// ʹ�÷�������ʽ,linger��Ϊ
	DWORD dwCmd = 1;
	INT nResult = ioctlsocket( m_Sock, FIONBIO, &dwCmd );
	if( SOCKET_ERROR == nResult )
	{
		closesocket( m_Sock );
		m_Sock = 0;
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	// disables the Nagle algorithm for send coalescing
	if( !m_bNagle )
	{
		BOOL bNoDelay = TRUE;
		setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
	}

	// set addr reusabel
	DWORD dwReUseAddr = TRUE;
	setsockopt( m_Sock, SOL_SOCKET, SO_REUSEADDR, 
		(char*)&dwReUseAddr, sizeof(dwReUseAddr) );

	// hard shut down
	linger lin;
	lin.l_linger = 0;
	lin.l_onoff = 1;
	setsockopt( m_Sock, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin) );

	// ÿ������ͳ��
	m_dwLastStatOutTime = timeGetTime();
	m_dwLastStatInTime = timeGetTime();
	m_dwByteOut = 0;
	m_dwByteIn = 0;
	m_dwByteOutCounter = 0;
	m_dwByteInCounter = 0;

	m_dwSendPackNum = 0;
	m_dwRecvPackNum = 0;

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
void StreamTransport::Destroy()
{
	// ���ý����̱߳�־
	::InterlockedExchange((LONG*)&m_bTerminateConnect, TRUE);
	::InterlockedExchange((LONG*)&m_bTerminateSend, TRUE);
	::InterlockedExchange((LONG*)&m_bTerminateReceive, TRUE);

	// �ȴ��߳̽���,һֱ����ȥ
	GracefulCloseThread(m_hConnect, INFINITE);
	GracefulCloseThread(m_hSend, INFINITE);
	GracefulCloseThread(m_hReceive, INFINITE);

	// �����ر�socket
	if( TRUE == m_bConnected )
	{
		GracefulCloseSocket(m_Sock, 20);	// ���� 20 ��
	}
	else
	{
		// m_bConnected��δΪ��֮ǰ����ʱ���ڿ����Ѿ�����
		// ������shutdownһ�£����޸�����
		shutdown(m_Sock, SD_SEND);
		closesocket(m_Sock);
		m_Sock = 0;
	}

	// �رջ���
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	SAFE_DEL( m_pSendMsgQueue );
	SAFE_DEL( m_pRecvMsgQueue );
}



//-----------------------------------------------------------------------------
// �����ر�socket
//-----------------------------------------------------------------------------
BOOL StreamTransport::GracefulCloseSocket(SOCKET& socket, INT nRetryTimes)
{
	if( 0 == socket )
		return TRUE;

	shutdown(socket, SD_SEND);
	
	INT nRetry = 0;
	fd_set fdread;
	timeval block_time;	// ��������ʱ��
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;	// 150 ms 
	
	while( nRetry++ < nRetryTimes )
	{
		FD_ZERO(&fdread);
		FD_SET(socket, &fdread);
		if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
		{
			CHAR chTemp;
			INT nResult = recv(socket, &chTemp, 1, 0);
			if( 0 == nResult )	// receive FIN, ok
			{
				closesocket( socket );
				socket = 0;
				return TRUE;
			}
			
			if( SOCKET_ERROR == nResult )	// network down
				break;
		}
	}
	

	closesocket( socket );
	// a hard shutdown
	socket = 0;
	return FALSE;
}


//-----------------------------------------------------------------------------
// ��������thread
//-----------------------------------------------------------------------------
BOOL StreamTransport::GracefulCloseThread(HANDLE& thread, INT nRetryTime)
{
	if( 0 == thread )
		return TRUE;

	DWORD dwResult = WaitForSingleObject(thread, nRetryTime);
	SAFE_CLOSE_HANDLE(thread);
	thread = 0;

	if( WAIT_TIMEOUT == dwResult )
		return FALSE;
	else
		return TRUE;
}




//-----------------------------------------------------------------------------
// ����������Ϣ
//-----------------------------------------------------------------------------
BOOL StreamTransport::Send(LPVOID pMsg, DWORD dwMsgSize)
{
	ASSERT(NULL != pMsg);
	ASSERT(dwMsgSize > 0 && dwMsgSize < 0xFFFFFF);	// ���ݹ��ֳ�
	// ��������Ϣ��ӵ�������Ϣ����
	if( FALSE == m_bConnected )
		return FALSE;

	//��ʼ����Ϣͳ��
	m_dwSendPackNum++;
	return m_pSendMsgQueue->AddMsg((LPBYTE)pMsg, dwMsgSize);
}



//-----------------------------------------------------------------------------
//! ����������Ϣ,����FALSE˵��û����Ϣ
//! �ⲿ��Ҫ�����ͷŷ��ص��ڴ�ָ��
//-----------------------------------------------------------------------------
LPBYTE StreamTransport::Recv(DWORD& dwMsgSize)
{
	if( !P_VALID(m_pRecvMsgQueue) )
		return NULL;

	// �ӽ��ܻ��������Ϣ
	return m_pRecvMsgQueue->GetMsg(dwMsgSize);
}



//-----------------------------------------------------------------------------
// ��������ָ����ַ
//-----------------------------------------------------------------------------
VOID StreamTransport::TryToConnect(LPCSTR szIP, INT nPort)
{
	Disconnect();
	
	strcpy(m_szIP, szIP);
	m_nPort = nPort;

	// �����߳� connect
	m_hConnect = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadConnect, this, 0, NULL);
}


//-----------------------------------------------------------------------------
//! �����Ƿ����ڳ�������
//! ��������������ȷ������Զ��������û����������������ô�ͻ�һֱ��������
//! ����������緢�����󣬾��п���ֹͣ��������
//-----------------------------------------------------------------------------
BOOL StreamTransport::IsTryingToConnect()
{
	if( !m_hConnect )
		return FALSE;

	DWORD dwConnectExitCode = STILL_ACTIVE;
	GetExitCodeThread(m_hConnect, &dwConnectExitCode);
	if( STILL_ACTIVE == dwConnectExitCode )
		return TRUE;
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// ������������
//-----------------------------------------------------------------------------
VOID StreamTransport::StopTryingConnect()
{
	// ��������Ѿ��������ˣ���ôҲ��Ͽ�
	Destroy();
	Init(m_bNagle, m_bPackageMode);
}

//-----------------------------------------------------------------------------
// �����Ͽ�����
//-----------------------------------------------------------------------------
VOID StreamTransport::Disconnect()
{
	this->StopTryingConnect();
}


//-----------------------------------------------------------------------------
// �õ�δ���Ͱ��������������Դ����ж����縺��
//-----------------------------------------------------------------------------
INT	StreamTransport::GetUnsendPackageNum()
{
	if( !m_bConnected )
		return 0;

	return m_pSendMsgQueue->GetMsgNum();
}


//-----------------------------------------------------------------------------
// �õ��ѽ��ܰ��������������Դ����ж����縺��
//-----------------------------------------------------------------------------
INT	StreamTransport::GetReceivedPackageNum()
{
	if( !m_bConnected )
		return 0;

	return m_pRecvMsgQueue->GetMsgNum();
}


//-----------------------------------------------------------------------------
// thread connect
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadConnect(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	INT nResult = 0;
	sockaddr_in address;
	ZeroMemory(&address, sizeof(address));
	address.sin_addr.s_addr = inet_addr(pStreamTransport->m_szIP);
	address.sin_family = AF_INET;
	address.sin_port = htons(pStreamTransport->m_nPort);

	// ���IP�Ƿ�Ϸ�
	if( INADDR_NONE == address.sin_addr.s_addr )
	{
		goto __connected_terminated;
	}
	
	fd_set fdwrite;
	timeval block_time;	// ��������ʱ��
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;
	
	// ���ӵ�������
	// For connection-oriented, nonblocking sockets, it is often not possible 
	// to complete the connection immediately. In such a case, "connect"
	// returns the error WSAEWOULDBLOCK. However, the operation proceeds.
	nResult = connect(pStreamTransport->m_Sock, (sockaddr*)&(address), sizeof(address));
	if( SOCKET_ERROR == nResult )
	{	
		if( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			for(INT n=0; n<64; n++) // ��ͣ����,�ⲿ����ͨ������StopTryingConnectֹͣ����
			{
				FD_ZERO(&fdwrite);
				FD_SET(pStreamTransport->m_Sock, &fdwrite);

				if( 1 == select(0, NULL, &fdwrite, NULL, &block_time) )
					goto __connected;

				if( pStreamTransport->m_bTerminateConnect )
					goto __connected_terminated;
			}
		}

		// ���ӳ���ʧ��
		goto __connected_terminated;
	}
	
__connected:

	//��ʼ����Ϣͳ��
	pStreamTransport->m_dwSendPackNum = 0;
	pStreamTransport->m_dwRecvPackNum = 0; 

	// �����߳� send
	pStreamTransport->m_hSend = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadSend, pStreamTransport, 0, NULL);

	// �����߳� receive
	if( pStreamTransport->m_bPackageMode )
	{
		pStreamTransport->m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadReceive, pStreamTransport, 0, NULL);
	}
	else
	{
		pStreamTransport->m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadStreamReceive, pStreamTransport, 0, NULL);
	}

	// ���ӳɹ�
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, TRUE);
	_endthreadex(0);
	return 0;

__connected_terminated:
		
	// ����ʧ��
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// thread send
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadSend(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	fd_set fdwrite;
	timeval block_time;	// ���÷�������ʱ��
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;	// 150ms

	LPBYTE	pMsg = NULL, pTempPtr = NULL;
	DWORD	dwMsgSize = 0;
	INT		nReturn = 0;

	while( FALSE == pStreamTransport->m_bTerminateSend )
	{
		for(;;)	// ע���ڲ���break
		{
			// ����ͳ��
			DWORD dwTime = timeGetTime();
			if( dwTime - pStreamTransport->m_dwLastStatOutTime >= ONE_SECOND ) 
			{
				pStreamTransport->m_dwLastStatOutTime = dwTime;
				pStreamTransport->m_dwByteOut = pStreamTransport->m_dwByteOutCounter;
				pStreamTransport->m_dwByteOutCounter = 0;
			}

			pTempPtr = pMsg = pStreamTransport->m_pSendMsgQueue->GetMsg(dwMsgSize);
			if( NULL == pMsg )
				break;	// ����Ϊֹ

			if( pStreamTransport->m_bEncrypt )	// ��ʼ����
			{
				DWORD dwRand = rand() % SECT_KIND;
				if( 0 == dwRand )
					dwRand = 1;
				pStreamTransport->Encrypt(pMsg, dwMsgSize, dwRand);
			}

			// send ���ܲ���һ�ν����ݷ���
			while( dwMsgSize > 0 && FALSE == pStreamTransport->m_bTerminateSend )
			{
				nReturn = send(pStreamTransport->m_Sock, (char*)pTempPtr, dwMsgSize, 0);
				if( SOCKET_ERROR == nReturn )
				{
					switch(WSAGetLastError())
					{
					case WSAEWOULDBLOCK:
					case WSAENOBUFS:
						// no buffer space is available within the transport 
						// system to hold the data to be transmitted
						// �ȴ����ݿ��Է��͵�ʱ��
						FD_ZERO(&fdwrite);
						FD_SET(pStreamTransport->m_Sock, &fdwrite);
						select(0, NULL, &fdwrite, NULL, &block_time);
						break;

					default:	// ����
						shutdown(pStreamTransport->m_Sock, SD_SEND);
						goto __thread_send_end;
					}
				}
				else
				{
					pTempPtr += nReturn;
					dwMsgSize -= nReturn;
					pStreamTransport->m_dwByteOutCounter += nReturn;	// ����ͳ��
				}
			}

			delete[](pMsg);
			pTempPtr = pMsg = NULL;
		}

		::WaitForSingleObject(pStreamTransport->m_pSendMsgQueue->GetEvent(), 120);
	}


__thread_send_end:
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// receive thread function
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadReceive(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	fd_set fdread;
	timeval block_time;	// ��������ʱ��
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	INT nResult = 0;
	DWORD dwMsgSize = 0;
	DWORD dwTempSize = 0;
	CHAR* pTempPtr = NULL;

	// ѭ���������
	while( FALSE == pStreamTransport->m_bTerminateReceive )
	{
		// �Ƚ���DWORD�͵���Ϣ������Ϣ
		dwTempSize = sizeof(DWORD);
		pTempPtr = (char*)&dwMsgSize;

		// ����ͳ��
		DWORD dwTime = timeGetTime();
		if( dwTime - pStreamTransport->m_dwLastStatInTime >= ONE_SECOND )
		{
			pStreamTransport->m_dwLastStatInTime = dwTime;
			pStreamTransport->m_dwByteIn = pStreamTransport->m_dwByteInCounter;
			pStreamTransport->m_dwByteInCounter = 0;
		}

		while( dwTempSize > 0 )
		{
			// ��ȡ�����ݵĴ�С
			FD_ZERO(&fdread);
			FD_SET(pStreamTransport->m_Sock, &fdread);
			if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
			{
				nResult = recv(pStreamTransport->m_Sock, pTempPtr, dwTempSize, 0);
				if( 0 == nResult )	// FIN
					goto __thread_receive_end;

				if( SOCKET_ERROR == nResult )
				{
					// �������
					shutdown(pStreamTransport->m_Sock, SD_SEND);
					goto __thread_receive_end;
				}
				else
				{
					dwTempSize -= nResult;
					pTempPtr += nResult;

					// ����ͳ��
					pStreamTransport->m_dwByteInCounter += nResult;
				}
			}
			
			if( pStreamTransport->m_bTerminateReceive )
				goto __thread_receive_end;
		}

		BYTE byEncryptType = (BYTE)(dwMsgSize >> 24);	// ���8λΪ����״̬
		dwMsgSize &= 0xffffff;	// ��24λΪ������Ϣ����

		// ASSERT(dwMsgSize < 40960);	// �쳣������
		if( SOCKET_ERROR != nResult )
		{
			char* pTemp = (char*)new BYTE[dwMsgSize];
			dwTempSize = dwMsgSize;
			pTempPtr = pTemp;
			while( dwTempSize > 0 )
			{
				FD_ZERO(&fdread);
				FD_SET(pStreamTransport->m_Sock, &fdread);
				if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
				{
					nResult = recv(pStreamTransport->m_Sock, pTempPtr, dwTempSize, 0);
					if( 0 == nResult )	// FIN
					{
						// IMSG(_T("client get a FIN\r\n"));
						delete[](pTemp);
						goto __thread_receive_end;
					}

					if( nResult == SOCKET_ERROR )
					{
						// �������
						shutdown(pStreamTransport->m_Sock, SD_SEND);
						goto __thread_receive_end;
					}
					else
					{
						dwTempSize -= nResult;
						pTempPtr += nResult;

						// ����ͳ��
						pStreamTransport->m_dwByteInCounter += nResult;
					}
					
					if( pStreamTransport->m_bTerminateReceive )
					{
						delete[](pTemp);
						goto __thread_receive_end;
					}
				}
			}
			
			if( 0 != byEncryptType )
			{
				// ���ü��ܱ�־
				::InterlockedExchange((LONG*)&pStreamTransport->m_bEncrypt, TRUE);
				pStreamTransport->Decrypt(pTemp, dwMsgSize, (DWORD)byEncryptType);
			}

			//��ʼ����Ϣͳ��
			pStreamTransport->m_dwRecvPackNum++; 
			pStreamTransport->m_pRecvMsgQueue->AddMsg(pTemp, dwMsgSize);
			delete[](pTemp);
		}
	}

__thread_receive_end:
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// stream receive thread function
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadStreamReceive(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	fd_set fdread;
	timeval block_time;	// ��������ʱ��
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	CONST INT RECV_BUFF_SIZE = 2048;	// 2KB recv buff
	INT nResult = 0;
	LPBYTE pBuffer = new BYTE[RECV_BUFF_SIZE];

	// ѭ���������
	while( FALSE == pStreamTransport->m_bTerminateReceive )
	{
		// ����ͳ��
		DWORD dwTime = timeGetTime();
		if( dwTime - pStreamTransport->m_dwLastStatInTime >= ONE_SECOND )
		{
			pStreamTransport->m_dwLastStatInTime = dwTime;
			pStreamTransport->m_dwByteIn = pStreamTransport->m_dwByteInCounter;
			pStreamTransport->m_dwByteInCounter = 0;
		}

		FD_ZERO(&fdread);
		FD_SET(pStreamTransport->m_Sock, &fdread);
		if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
		{
			nResult = recv(pStreamTransport->m_Sock, (CHAR*)pBuffer, RECV_BUFF_SIZE, 0);
			if( 0 == nResult )	// FIN
			{
				goto __thread_stream_receive_end;
			}
	
			if( nResult == SOCKET_ERROR )
			{
				// �������
				shutdown(pStreamTransport->m_Sock, SD_SEND);
				goto __thread_stream_receive_end;
			}
			else
			{
				// ����ͳ��
				pStreamTransport->m_dwByteInCounter += nResult;
				pStreamTransport->m_dwRecvPackNum++;

				// ��������
				pStreamTransport->m_pRecvMsgQueue->AddMsg(pBuffer, nResult);
			}
		}

	}

__thread_stream_receive_end:
	SAFE_DEL_ARRAY(pBuffer);
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// �õ�����������
//-----------------------------------------------------------------------------
CHAR* StreamTransport::GetHostName()
{
	gethostname(m_szHostName, sizeof(m_szHostName)-10);
	return m_szHostName;
}


//-----------------------------------------------------------------------------
// �õ�ָ��������IP
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetIPByHostName(DWORD& dwIP, LPCSTR lpHostName)
{
	ASSERT(lpHostName);
	hostent* p = gethostbyname(lpHostName);
	if( p )
	{
		INT n=0;
		while((DWORD*)p->h_addr_list[n])
		{
			dwIP = *((DWORD*)p->h_addr_list[n]);
			n++;
		}
		return TRUE;
	}
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// �õ�������������ַ
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetLocalLANIP(DWORD& dwIP)
{
    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if( sd == SOCKET_ERROR )
	{
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
		closesocket(sd);
		return FALSE;
    }

	closesocket(sd);

	INT nNumInterfaces = dwBytesReturned / sizeof(INTERFACE_INFO);
    for( INT i = 0; i < nNumInterfaces; ++i) 
	{
		u_long nFlags = InterfaceList[i].iiFlags;
		if( !(nFlags & IFF_UP) )	// �������
			continue;

		if( nFlags & IFF_LOOPBACK ) // ������ loopback
			continue;

        sockaddr_in *pAddress;
        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
        dwIP = pAddress->sin_addr.S_un.S_addr;

		if( pAddress->sin_addr.S_un.S_un_b.s_b1 == 10 
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 172 && pAddress->sin_addr.S_un.S_un_b.s_b2 >= 16 && pAddress->sin_addr.S_un.S_un_b.s_b2 <= 31)
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 192 && pAddress->sin_addr.S_un.S_un_b.s_b2 == 168) )
			return TRUE;
    }

	return FALSE;
}


//-----------------------------------------------------------------------------
// �õ�������������ַ
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetLocalWANIP(DWORD& dwIP)
{
    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if( sd == SOCKET_ERROR )
	{
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
		closesocket(sd);
		return FALSE;
    }

	closesocket(sd);

	INT nNumInterfaces = dwBytesReturned / sizeof(INTERFACE_INFO);
    for( INT i = 0; i < nNumInterfaces; ++i) 
	{
		u_long nFlags = InterfaceList[i].iiFlags;
		if( !(nFlags & IFF_UP) )	// �������
			continue;

		if( nFlags & IFF_LOOPBACK ) // ������ loopback
			continue;

        sockaddr_in *pAddress;
        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
        dwIP = pAddress->sin_addr.S_un.S_addr;

   		if( !( pAddress->sin_addr.S_un.S_un_b.s_b1 == 10 
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 172 && pAddress->sin_addr.S_un.S_un_b.s_b2 >= 16 && pAddress->sin_addr.S_un.S_un_b.s_b2 <= 31)
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 192 && pAddress->sin_addr.S_un.S_un_b.s_b2 == 168) ))
			return TRUE;
    }

	return FALSE;
}



//-----------------------------------------------------------------------------
// ������IP��ַת��ΪIP��ַ�ַ���
//-----------------------------------------------------------------------------
CHAR* StreamTransport::IP2String(DWORD dwIP)
{
	in_addr addr;
	addr.S_un.S_addr = dwIP;
	return inet_ntoa(addr);
}

//-----------------------------------------------------------------------------
// ��IP��ַ�ַ���ת��Ϊ����IP��ַ
//-----------------------------------------------------------------------------
DWORD StreamTransport::StringIP2IP(CHAR* szIP)
{
	return inet_addr(szIP);
}


//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID StreamTransport::Encrypt(LPVOID pMsg, DWORD dwRealSize, DWORD dwEncryptType)
{
	// ע�����ﴫ������dwRealSize����ǰ��һ��DWORD����
	INT nSect = dwEncryptType % SECT_KIND;
	PBYTE p1 = (PBYTE)pMsg;
	*(p1+sizeof(DWORD)-1) = BYTE(nSect);
	PBYTE p2_start = &m_pSect[nSect*SECT_LEN];
	PBYTE p2 = p2_start;

	// ��ͷ�����⴦��
	p1 += sizeof(DWORD);

	DWORD dwSize = dwRealSize / 8 * 8;
	if( dwSize > dwRealSize - sizeof(DWORD) )
		dwSize = dwRealSize - sizeof(DWORD);

	while( dwSize-- )
	{
		*p1++ = (*p1)^(*p2++);
		if( p2 >= p2_start + SECT_LEN )
			p2 = p2_start;
	}
}


//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID StreamTransport::Decrypt(LPVOID pMsg, DWORD dwRealSize, DWORD dwEncryptType)
{
	INT nSect = dwEncryptType % SECT_KIND;
	PBYTE p1 = (PBYTE)pMsg;
	PBYTE p2_start = &m_pSect[nSect*SECT_LEN];
	PBYTE p2 = p2_start;

	dwRealSize = ((dwRealSize+sizeof(DWORD)) / 8 * 8) - sizeof(DWORD);

	// ��ͷ�����⴦��
	p2 += sizeof(DWORD);
	while(dwRealSize--)
	{
		*p1++ = (*p1)^(*p2++);
		if( p2 >= p2_start + SECT_LEN )
			p2 = p2_start;
	}

}
