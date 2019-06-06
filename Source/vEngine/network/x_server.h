//-----------------------------------------------------------------------------
//!\file x_server.h
//!\author Lyp
//!
//!\date 2004-04-03
//! last 2009-03-19
//!
//!\brief �������˴���ײ�
//-----------------------------------------------------------------------------
#pragma once

// ��ΪĳЩ����inline, �������ﲻ�ò�����һЩͷ�ļ�
#include "x_define.h"

namespace vEngine {
class SafeMemPool;
//-----------------------------------------------------------------------------
// �������˴���ײ�
//-----------------------------------------------------------------------------
class VENGINE_API XServer
{
public:
	BOOL Init(tagXServerConfig& param);
	BOOL Destroy();

	//-----------------------------------------------------------------------------
	// �����������
	//-----------------------------------------------------------------------------
	// ����
	__forceinline VOID Send(DWORD dwHandle, LPVOID pMsg, DWORD dwSize);
	// ������߳�
	__forceinline VOID ActiveSend() { ::SetEvent(m_hEventSend);	}
	// ����,�����Եõ���ǰ�ײ��յ�������nRecvNum��
	__forceinline LPBYTE Recv(DWORD dwHandle, DWORD& dwSize, INT& nRecvNum);
	// ����recv���ڲ��ͷ�
	__forceinline VOID FreeRecved(LPBYTE pMsg);

	// �߳��ͻ���
	VOID Kick(DWORD dwHandle);

	//-----------------------------------------------------------------------------
	// ��Ϣ����
	//-----------------------------------------------------------------------------
	tagXServerConfig* GetConfig() { return &m_Config; }	// �ں�ʵ�ʶ˿ںŵ���Ϣ
	// �õ�����wsasend����û���յ����֪ͨ�İ���Ŀ
	LONG GetSendCast(DWORD dwHandle) { return ((tagXClient*)dwHandle)->lSendCast; }
	DWORD GetSendCastSize(DWORD dwHandle) { return ((tagXClient*)dwHandle)->dwSendSize; }
	INT GetPoolNum() { return m_nPoolNum; }	// �ڴ��״̬
	SafeMemPool* GetPool(INT n) { return &m_Pool[n]; }	// �ڴ��״̬
	INT64 GetSendSize() { return m_n64SendSize; }			// �ϲ㷢�ʹ�С
	INT64 GetRealSendSize() { return m_n64RealSendSize; }	// ѹ����ʵ�ʷ��ʹ�С
	INT GetFreeClientNum() { return m_listFreeClient.Size(); }

	XServer();
	~XServer();

private:
	TSFPTrunk<XServer>	m_Trunk;
	TObjRef<FilterMgr>	m_pFilter;
	TObjRef<Util>		m_pUtil;

	LPFN_ACCEPTEX				m_lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS	m_lpfnGetAcceptExSockAddr;

	XList<tagXClient*>	m_listFreeClient;
	XList<tagXClient*>	m_listUnauthClient;
	XList<tagXClient*>	m_listDestroyClient;
	
	std::vector<HANDLE>	m_vecThreadHandle;		// ���������̵߳�vector
	BOOL				m_bTerminateWorkThread;	// �Ƿ���������߳�
	HANDLE				m_hEventAccepEx;
	HANDLE				m_hEventSend;
	HANDLE				m_hEventDestroy;

	tagXServerConfig	m_Config;				// ��������
    SOCKET				m_sockListen;			// ����socket
	HANDLE				m_hCompletionPort;		// ��������ɶ˿�

	tagXClient*			m_Client;				// client data
	SafeMemPool*		m_Pool;					// �ڴ��
	INT					m_nPoolNum;				// �ڴ�ظ���

	INT	VOLATILE		m_nAcceptExs;			// �ų���AcceptEx����
	INT64				m_n64SendSize;			// ͳ������
	INT64				m_n64RealSendSize;		// ͳ������

	// ��������������߳�
	BOOL	CreateSetupAccepExThread();
	UINT	ThreadSetupAcceptEx(LPVOID);
	// ���֪ͨ�����߳�
	BOOL	CreateWorkThread(INT nThreadNum);
	UINT	ThreadWorker(INT nThreadIndex);
	// �߳�δ��֤�ͻ����߳�
	BOOL	CreateUnauthHeartbeatThread();
	UINT	ThreadUnauthHeartbeat(DWORD dwWaitForAuthTime);
	// ����������߳�	
	BOOL	CreateSendThread();
	UINT	ThreadSend(LPVOID);
	// ��Դ�����߳�
	BOOL	CreateDestroyThread();
	UINT	ThreadDestroy(LPVOID);

	// ������Դ
	VOID	PreDestroyClient(tagXClient* pClient, tagUnit* pUnit);
	VOID	DestroyClient(tagXClient* pClient);
};


//-----------------------------------------------------------------------------
// Send
//-----------------------------------------------------------------------------
VOID XServer::Send(DWORD dwHandle, LPVOID pMsg, DWORD dwSize)
{
	tagXClient* pClient = (tagXClient*)dwHandle;
	if( !pClient->bShutDown )
	{
		// Ѱ�ҿ��Խ����������ڴ��
		INT n = (dwHandle%(m_nPoolNum-1))+1;
		tagRawUnit* pSendUnit=0;
		for(;!pSendUnit;)
		{
			if( ++n >= m_nPoolNum )
				n = 1;
			pSendUnit = (tagRawUnit*)m_Pool[n].TryAlloc(RAWUNIT_SIZE(dwSize));
		}

		pSendUnit->dwSize = dwSize;
		pSendUnit->pPool = &m_Pool[n];
		memcpy(pSendUnit->pBuffer, pMsg, dwSize);
		InterlockedIncrement(&pClient->lSendCast);
		pClient->dwSendSize += dwSize;
		pClient->pSendQueue->Add(pSendUnit);
	}
}


//-----------------------------------------------------------------------------
// ����unit,�����Եõ���ǰ�յ���unit��Ŀ�ܺ�
//-----------------------------------------------------------------------------
LPBYTE XServer::Recv(DWORD dwHandle, DWORD& dwSize, INT& nRecvNum)
{
	tagXClient* pClient = (tagXClient*)dwHandle;
	if( !pClient->bShutDown )
	{
		nRecvNum = pClient->pRecvQueue->GetNum();
		tagUnit* pUnit = pClient->pRecvQueue->Get();
		if( !pUnit )
			return NULL;

		dwSize = pUnit->dwSize;
		return (LPBYTE)pUnit->pBuffer;
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// ����unit,�����Եõ���ǰ�յ���unit��Ŀ�ܺ�
//-----------------------------------------------------------------------------
VOID XServer::FreeRecved(LPBYTE pMsg)
{
	tagUnit* pUnit = CONTAINING_RECORD(pMsg, tagUnit, pBuffer);
	pUnit->pPool->Free(pUnit);
}


} // namespace vEngine {
