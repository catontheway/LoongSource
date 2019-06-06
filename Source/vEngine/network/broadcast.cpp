//-----------------------------------------------------------------------------
//!\file broadcast.cpp
//!\author Lyp
//!
//!\date 2004-08-04
//! last 2004-08-04
//!
//!\brief �������㲥����
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "broadcast.h"


// ʹ����������
#include "..\frame\msg_queue_ts.h"


namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
Broadcast::Broadcast():m_Trunk(this)
{
	m_hThreadBroadcast = 0;
	m_hThreadRecvBroadcast = 0;
	m_bTerminateBroadcast = FALSE;
	m_bTerminateRecvBroadcast = FALSE;

	m_pSendMsgQueue = 0;
	m_pRecvMsgQueue = 0;
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
Broadcast::~Broadcast()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// ��ʼ���㲥����
// szLocalIP �󶨵�����ʲôIP������Ϊ��
//-----------------------------------------------------------------------------
BOOL Broadcast::Init(INT nPort, LPCSTR szLocalIP)
{
	this->Destroy();

	m_nPort = nPort;
	if( szLocalIP )
		strcpy((char*)m_szLocalIP, szLocalIP);

	m_hThreadBroadcast = 0;
	m_hThreadRecvBroadcast = 0;
	m_bTerminateBroadcast = FALSE;
	m_bTerminateRecvBroadcast = FALSE;

	// ���뷢�ͽ��ܻ���
	m_pSendMsgQueue = new MsgQueueTS(TRUE, FALSE);
	m_pRecvMsgQueue = new MsgQueueTS(FALSE, FALSE);
	if( NULL == m_pSendMsgQueue || NULL == m_pRecvMsgQueue )
	{
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID Broadcast::Destroy()
{
	// ��Ҫ�ر��߳�
	InterlockedExchange((LONG*)&m_bTerminateBroadcast, TRUE);
	InterlockedExchange((LONG*)&m_bTerminateRecvBroadcast, TRUE);

	if( m_hThreadBroadcast )
	{
		::WaitForSingleObject( m_hThreadBroadcast, INFINITE );
		SAFE_CLOSE_HANDLE( m_hThreadBroadcast );
	}

	if( m_hThreadRecvBroadcast )
	{
		::WaitForSingleObject( m_hThreadRecvBroadcast, INFINITE );
		SAFE_CLOSE_HANDLE( m_hThreadRecvBroadcast );
	}

	SAFE_DEL(m_pSendMsgQueue);
	SAFE_DEL(m_pRecvMsgQueue);

	m_bTerminateBroadcast = FALSE;
	m_bTerminateRecvBroadcast = FALSE;
}


//-----------------------------------------------------------------------------
// ��ʼ�㲥
//-----------------------------------------------------------------------------
BOOL Broadcast::StartBroadcast()
{
	// �����߳� broadcast
	m_hThreadBroadcast = (HANDLE)_beginthreadex
		(NULL, 0, (THREADPROC)m_Trunk.sfp1(&vEngine::Broadcast::ThreadBroadcast), NULL, 0, NULL);

	if( m_hThreadBroadcast )
		return TRUE;
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// ��ʼ�����㲥
//-----------------------------------------------------------------------------
BOOL Broadcast::StartRecvBroadcast()
{
	// �����߳� broadcast
	m_hThreadRecvBroadcast = (HANDLE)_beginthreadex
		(NULL, 0, (THREADPROC)m_Trunk.sfp1(&vEngine::Broadcast::ThreadRecvBroadcast), NULL, 0, NULL);

	if( m_hThreadRecvBroadcast )
		return TRUE;
	else
		return FALSE;
}
//-----------------------------------------------------------------------------
//�������չ㲥�߳�
//-----------------------------------------------------------------------------
BOOL Broadcast::TerminateRecvBroadcast(DWORD dwTimeOut)
{
	InterlockedExchange((LONG*)&m_bTerminateRecvBroadcast, TRUE);

	if( m_hThreadRecvBroadcast )
	{
		DWORD dwRet = ::WaitForSingleObject( m_hThreadRecvBroadcast, dwTimeOut );
		
		if( dwRet == WAIT_TIMEOUT )
			return FALSE;

		SAFE_CLOSE_HANDLE( m_hThreadRecvBroadcast );
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
//�����㲥�߳�
//-----------------------------------------------------------------------------
BOOL Broadcast::TerminateBroadcast(DWORD dwTimeOut)
{
	InterlockedExchange((LONG*)&m_bTerminateBroadcast, TRUE);

	if( m_hThreadBroadcast )
	{
		DWORD dwRet = ::WaitForSingleObject( m_hThreadBroadcast, dwTimeOut );

		if( dwRet == WAIT_TIMEOUT )
			return FALSE;

		SAFE_CLOSE_HANDLE( m_hThreadBroadcast );
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
// �㲥
//-----------------------------------------------------------------------------
VOID Broadcast::Broad(LPBYTE pMsg, DWORD dwMsgSize)
{
	ASSERT(NULL != pMsg);
	ASSERT(dwMsgSize > 0 && dwMsgSize < 1024);
	// ��������Ϣ��ӵ�������Ϣ����
	m_pSendMsgQueue->AddMsg(pMsg, dwMsgSize);
}


//-----------------------------------------------------------------------------
// �õ����յ��Ĺ㲥��Ϣ��û�оͷ���NULL
//-----------------------------------------------------------------------------
LPBYTE Broadcast::RecvBroadcast(DWORD& dwMsgSize)
{
	return m_pRecvMsgQueue->GetMsg(dwMsgSize);
}


//-----------------------------------------------------------------------------
// ��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
//-----------------------------------------------------------------------------
VOID Broadcast::FreeRecved(LPBYTE pMsg)
{
	delete[](pMsg);
}


//-----------------------------------------------------------------------------
// �㲥�߳�
//-----------------------------------------------------------------------------
UINT Broadcast::ThreadBroadcast(LPVOID)
{
	SOCKET sock = socket(PF_INET, SOCK_DGRAM, 0);
	if( sock == INVALID_SOCKET )
	{
		ERR(_T("couldn't create socket in thread broadcast"));
		return 0;
	}

	// set addr reusabel
	DWORD dwReUseAddr = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, 
		(char*)&dwReUseAddr, sizeof(dwReUseAddr) );

	// ��
	sockaddr_in	add;
	ZeroMemory(&add, sizeof(add));
	add.sin_family = PF_INET;
	add.sin_addr.s_addr = htonl( INADDR_ANY );
	add.sin_port = htons( (unsigned short)m_nPort );

	if( bind( sock, (struct sockaddr *)&add, sizeof(add) ) != 0 )
	{
		// ���ܱ����Ѿ�������һ���ͻ���
		ERR(_T("bind error in thread broadcast"));
		closesocket(sock);
		return 0;
	}

	// ���ù㲥����
	BOOL opt = TRUE;
	INT nResult = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(BOOL));
	if( SOCKET_ERROR == nResult )
	{
		ERR(_T("setsockopt failed"));
		closesocket(sock);
		return 0;
	}
	
	sockaddr_in dest;	// ����Ŀ���ַ
	ZeroMemory(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl( INADDR_BROADCAST );
	dest.sin_port = htons( (unsigned short) m_nPort );

	while( FALSE == m_bTerminateBroadcast )
	{
		while(1)
		{
			DWORD dwSize = 0;
			LPBYTE pMsg = m_pSendMsgQueue->GetMsg(dwSize);
			if( NULL == pMsg )
				break;

			INT nSent = sendto( sock, (const char*)pMsg, dwSize, 
				0, (sockaddr*)&dest, sizeof(dest) );

			if( nSent != (INT)dwSize )
				ERR(_T("send udp broadcast message failed"));

		}

		::WaitForSingleObject(m_pSendMsgQueue->GetEvent(), 120);
	}

	closesocket( sock );
	return 0;
}

//-----------------------------------------------------------------------------
// ���㲥�߳�
//-----------------------------------------------------------------------------
UINT Broadcast::ThreadRecvBroadcast(LPVOID)
{
	SOCKET sock = socket(PF_INET, SOCK_DGRAM, 0);
	if( sock == INVALID_SOCKET )
	{
		ERR(_T("create socket failed in thread recv broadcast"));
		return 0;
	}

	// sock ʹ�÷�������ʽ
	DWORD dwCmd = 1;
	INT nResult = ioctlsocket( sock, FIONBIO, &dwCmd );
	if( SOCKET_ERROR == nResult )
	{
		ERR(_T("set nonblocking mode failed in thread recv broadcast"));
		closesocket( sock );
		return 0;
	}

	// set addr reusabel
	DWORD dwReUseAddr = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, 
		(char*)&dwReUseAddr, sizeof(dwReUseAddr) );

	sockaddr_in	add;
	ZeroMemory(&add, sizeof(add));
	add.sin_family = PF_INET;
	add.sin_addr.s_addr = htonl( INADDR_ANY );
	add.sin_port = htons( (unsigned short)m_nPort );
	if( SOCKET_ERROR == bind(sock, (struct sockaddr*)&add, sizeof(add)) )
	{
		ERR(_T("bind error in thread recv broadcast"));
		closesocket( sock );
		return 0;
	}

	// ��������ʱ��
	timeval block_time;
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	sockaddr_in	source;
	ZeroMemory(&source, sizeof(source));

	while( FALSE == m_bTerminateRecvBroadcast )
	{
		// "select" will change fdread, so this must put in the loop
		fd_set fdread;
		FD_ZERO(&fdread);
		FD_SET(sock, &fdread);
		nResult = select(0, &fdread, NULL, NULL, &block_time);

		if( 1 == nResult )
		{
			BYTE msg[1024];
			INT nLen = sizeof(sockaddr_in);

			nResult = recvfrom(sock, (char*)msg, sizeof(msg)-4, 0, 
				(sockaddr*)&source, &nLen);

			if( SOCKET_ERROR == nResult || 0 == nResult )
			{
				ERR(_T("recvfrom failed in thread recv broadcast"));
			}
			else
			{
				// ��ԭ��ַ�����������Ϣ��4�ֽ�
				*(DWORD*)&msg[nResult] = source.sin_addr.S_un.S_addr; 
				m_pRecvMsgQueue->AddMsg(&msg, nResult+4);
			}
		}
	}

	closesocket( sock );
	return 0;
}



} // namespace vEngine {
