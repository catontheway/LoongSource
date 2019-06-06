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


#define SECT_KIND			256
#define SECT_LEN			512
#define ONE_SECOND			1000
#define RECV_BUFFER_SIZE	(512*1024)
//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
StreamTransport::StreamTransport():m_Trunk(this)
{
	m_pRecvBuffer = new BYTE[RECV_BUFFER_SIZE];
	m_hReceiveEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// �Զ�����
	Clear();
}

StreamTransport::~StreamTransport()
{
	SAFE_DEL_ARRAY(m_pRecvBuffer);
	CloseHandle(m_hReceiveEvent);
	this->Destroy();
	::WSACleanup();
}


//-----------------------------------------------------------------------------
// clear
//-----------------------------------------------------------------------------
VOID StreamTransport::Clear()
{
	m_pSendMsgQueue = NULL;
	m_pRecvMsgQueue = NULL;
	ZeroMemory(m_pRecvBuffer, sizeof(BYTE)*RECV_BUFFER_SIZE);
	m_n64RecvSize = -1;	// ��Ϣ���������������Ϊ-1��ʾ���յ����оͲ�������

	m_szIP[0] = 0;
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
	m_Trunk.sfp_clear();
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL StreamTransport::Init(BOOL bNagle)
{
	this->Clear();

	m_bNagle = bNagle;
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
				IMSG(_T("WSAStartup failed\r\n"));
				return FALSE;
			}
		}
	}
	
	// ���뷢�ͽ��ܻ���
	m_pSendMsgQueue = new MsgQueueTS(TRUE, FALSE);
	m_pRecvMsgQueue = new MsgQueueTS(TRUE, FALSE);
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
		IMSG(_T("could not create socket\r\n"));
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	// ʹ�÷�������ʽ,linger��Ϊ
	DWORD dwCmd = 1;
	INT nResult = ioctlsocket( m_Sock, FIONBIO, &dwCmd );
	if( SOCKET_ERROR == nResult )
	{
		IMSG(_T("set nonblocking mode failed in init client\r\n"));
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


	//����KeepAlive    
	BOOL bKeepAlive = TRUE;    
	nResult = ::setsockopt(m_Sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));
	if( nResult != 0 )
	{
		IMSG(_T("Winsock error Socket SetOpt failed"));
		return FALSE;
	}    

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
	ASSERT(dwMsgSize > 0 && dwMsgSize < 40960);	// ���ݹ��ֳ�
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
	if( FALSE == m_bConnected )
		return NULL;

	// �ӽ��ܻ��������Ϣ
	return m_pRecvMsgQueue->GetMsg(dwMsgSize);
}

//-----------------------------------------------------------------------------
// ������Ϣ���������������Ϊ-1��ʾ���յ����оͲ�������
//-----------------------------------------------------------------------------
VOID StreamTransport::SetRecv(INT64 n64Size)
{
	m_n64RecvSize = n64Size;
	::SetEvent(m_hReceiveEvent);
}



//-----------------------------------------------------------------------------
// ��������ָ����ַ
//-----------------------------------------------------------------------------
VOID StreamTransport::TryToConnect(LPCSTR szIP, INT nPort)
{
	this->Disconnect();
	
	strcpy(m_szIP, szIP);
	m_nPort = nPort;

	// �����߳� connect
	m_hConnect = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&StreamTransport::ThreadConnect), NULL, 0, NULL);
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
	this->Destroy();
	this->Init(m_bNagle);
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
UINT StreamTransport::ThreadConnect(LPVOID)
{
	INT nResult = 0;
	sockaddr_in address;
	ZeroMemory(&address, sizeof(address));
	address.sin_addr.s_addr = inet_addr(m_szIP);
	address.sin_family = AF_INET;
	address.sin_port = htons(m_nPort);

	// ���IP�Ƿ�Ϸ�
	if( INADDR_NONE == address.sin_addr.s_addr )
	{
		IMSG(_T("[c] invalid IP:[%s]\r\n"), m_szIP);
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
	nResult = connect(m_Sock, (sockaddr*)&(address), sizeof(address));
	if( SOCKET_ERROR == nResult )
	{	
		if( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			for(INT n=0; n<64; n++) // ��ͣ����,�ⲿ����ͨ������StopTryingConnectֹͣ����
			{
				FD_ZERO(&fdwrite);
				FD_SET(m_Sock, &fdwrite);

				if( 1 == select(0, NULL, &fdwrite, NULL, &block_time) )
					goto __connected;

				if( m_bTerminateConnect )
					goto __connected_terminated;
			}
		}

		// ���ӳ���ʧ��
		goto __connected_terminated;
	}
	
__connected:

	//��ʼ����Ϣͳ��
	m_dwSendPackNum = 0;
	m_dwRecvPackNum = 0; 

	// �����߳� send
	m_hSend = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&StreamTransport::ThreadSend), NULL, 0, NULL);

	// �����߳� receive
	m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&StreamTransport::ThreadReceive), NULL, 0, NULL);

	// ���ӳɹ�
	::InterlockedExchange((LONG*)&m_bConnected, TRUE);
	
	_endthreadex(0);
	return 0;

__connected_terminated:
		
	// ����ʧ��
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// thread send
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadSend(LPVOID)
{
	fd_set fdwrite;
	timeval block_time;	// ���÷�������ʱ��
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;	// 150ms

	LPBYTE	pMsg = NULL, pTempPtr = NULL;
	DWORD	dwMsgSize = 0;
	INT		nReturn = 0;

	while( FALSE == m_bTerminateSend )
	{
		for(;;)	// ע���ڲ���break
		{
			// ����ͳ��
			DWORD dwTime = timeGetTime();
			if( dwTime - m_dwLastStatOutTime >= ONE_SECOND ) 
			{
				m_dwLastStatOutTime = dwTime;
				m_dwByteOut = m_dwByteOutCounter;
				m_dwByteOutCounter = 0;
			}

			pTempPtr = pMsg = m_pSendMsgQueue->GetMsg(dwMsgSize);
			if( NULL == pMsg )
				break;	// ����Ϊֹ

			// send ���ܲ���һ�ν����ݷ���
			while( dwMsgSize > 0 && FALSE == m_bTerminateSend )
			{
				nReturn = send(m_Sock, (char*)pTempPtr, dwMsgSize, 0);
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
						FD_SET(m_Sock, &fdwrite);
						select(0, NULL, &fdwrite, NULL, &block_time);
						break;

					default:	// ����
						shutdown(m_Sock, SD_SEND);
						goto __thread_send_end;
					}
				}
				else
				{
					pTempPtr += nReturn;
					dwMsgSize -= nReturn;
					m_dwByteOutCounter += nReturn;	// ����ͳ��
				}
			}

			delete[](pMsg);
			pTempPtr = pMsg = NULL;
		}

		::WaitForSingleObject(m_pSendMsgQueue->GetEvent(), 120);
	}


__thread_send_end:
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// receive thread function
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadReceive(LPVOID)
{
	fd_set fdread;
	timeval block_time;	// ��������ʱ��
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	INT nResult = 0;
	DWORD dwMsgSize = 0;
	CHAR* pTempPtr = NULL;

	// ѭ���������
	while( FALSE == m_bTerminateReceive )
	{
		// �Ƚ���DWORD�͵���Ϣ������Ϣ
		pTempPtr = (CHAR*)m_pRecvBuffer;

		// ����ͳ��
		DWORD dwTime = timeGetTime();
		if( dwTime - m_dwLastStatInTime >= ONE_SECOND )
		{
			m_dwLastStatInTime = dwTime;
			m_dwByteIn = m_dwByteInCounter;
			m_dwByteInCounter = 0;
		}

		// �����������һ���ǰ��ջس���������Ϣ��һ���ǰ���Ҫ����յ��ֽ���������Ϣ
		if( m_n64RecvSize < 0 )
		{
			dwMsgSize = 0;
			while( true )	// ���ջس���������Ϣ
			{
				// ��ȡ�����ݵĴ�С
				FD_ZERO(&fdread);
				FD_SET(m_Sock, &fdread);
				if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
				{
					nResult = recv(m_Sock, pTempPtr, 1, 0);	// ��1���ֽ�
					if( 0 == nResult )	// FIN
						goto __thread_receive_end;

					if( SOCKET_ERROR == nResult )
					{
						// �������
						shutdown(m_Sock, SD_SEND);
						goto __thread_receive_end;
					}
					else
					{
						dwMsgSize+=nResult;
						if( *pTempPtr == '\n' )
						{
							//��ʼ����Ϣͳ��
							m_dwRecvPackNum++; 
							m_pRecvMsgQueue->AddMsg(m_pRecvBuffer, dwMsgSize);
							break;
						}

						// ����ͳ��
						m_dwByteInCounter += nResult;
						pTempPtr += nResult;

					}
				}
				else
				{
					if( dwMsgSize == 0 )
						break;
				}

				if( m_bTerminateReceive )
					goto __thread_receive_end;
			}
		}
		else
		{
			while( m_n64RecvSize > 0 )
			{
				// ����ͳ��
				DWORD dwTime = timeGetTime();
				if( dwTime - m_dwLastStatInTime >= ONE_SECOND )
				{
					m_dwLastStatInTime = dwTime;
					m_dwByteIn = m_dwByteInCounter;
					m_dwByteInCounter = 0;
				}

				dwMsgSize = m_n64RecvSize < RECV_BUFFER_SIZE ? (DWORD)m_n64RecvSize : RECV_BUFFER_SIZE;

				FD_ZERO(&fdread);
				FD_SET(m_Sock, &fdread);
				if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
				{
					nResult = recv(m_Sock, pTempPtr, dwMsgSize, 0);
					if( 0 == nResult )	// FIN
					{
						// IMSG(_T("client get a FIN\r\n"));
						goto __thread_receive_end;
					}

					if( nResult == SOCKET_ERROR )
					{
						// �������
						shutdown(m_Sock, SD_SEND);
						goto __thread_receive_end;
					}
					else
					{
						m_n64RecvSize -= nResult;
						m_dwByteInCounter += nResult;	// ����ͳ��
						//��ʼ����Ϣͳ��
						m_dwRecvPackNum++; 
						m_pRecvMsgQueue->AddMsg(m_pRecvBuffer, nResult);
					}
				}

				if( m_bTerminateReceive )
					goto __thread_receive_end;
			}
		}

		::WaitForSingleObject(m_hReceiveEvent, 120);
	}

__thread_receive_end:
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
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
        IMSG(_T("failed to get a socket.\r\n"));
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
        IMSG(_T("failed calling WSAIoctl\r\n"));
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
        IMSG(_T("failed to get a socket.\r\n"));
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
        IMSG(_T("failed calling WSAIoctl\r\n"));
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
