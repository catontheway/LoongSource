//-----------------------------------------------------------------------------
//!\file transport.h
//!\author Lyp
//!
//!\date 2004-04-03
//! last 2008-04-17
//!
//!\brief �������˴���ײ�
//-----------------------------------------------------------------------------
#pragma once

// ��ΪĳЩ����inline, �������ﲻ�ò�����һЩͷ�ļ�
#include "safe_unit_queue.h"	
#include "safe_mutex_list.h"
#include "unit_pool.h"

namespace vEngine {
//-----------------------------------------------------------------------------
//!\brief �������˴���ײ�
//!
//!
//-----------------------------------------------------------------------------
class VENGINE_API CompletionTransport
{
public:
	BOOL Init(tagServerInitParam* pInitParam);
	BOOL Destroy();

	// ����ע��ע���ص�����
	VOID SetLogCallback(LOGINCALLBACK fnLogIn, LOGOUTCALLBACK fnLogOut);
	// ���ü��ܽ���unit�ص�����
	VOID SetSecureCallBack(CRYPTCALLBACK fnEncrypt, CRYPTCALLBACK fnDecrypt);
	// ���÷Ƿ��û��ٱ��ص�����
	VOID SetHackReportCallBack(HACKREPORTCALLBACK fnHackReport);


	// �õ���δ������ɵ�unit��Ŀ
	__forceinline INT GetUnsendUnitNum() { return m_pSendQueue->GetNum(); } 
	// ����unit, ���ش��û����ж��ٰ�δ����ȥ
	__forceinline INT SendUnit(tagUnitData* pUnit);
	// �õ�����wsasend����û���յ����֪ͨ�İ���Ŀ
	__forceinline INT	GetSendCast(INT nCDIndex) 
	{return m_ClientData[nCDIndex].nSendCast;}

	// ����unit,�����Եõ���ǰ�յ���unit��Ŀ�ܺ�
	__forceinline tagUnitData* RecvUnit(DWORD& dwRecvedNum, INT nCDIndex);
	
	// ����unit��transport
	__forceinline VOID ReturnUnit(tagUnitData* pUnit);
	// ��transportȡ��unit
	__forceinline tagUnitData* GetFreeUnit();

	// �õ�����ID
	DWORD GetSerialID(INT nCDIndex) { return m_ClientData[nCDIndex].dwSerialID; }
	// �õ��ͻ���sockname
	BOOL GetClientSockName(sockaddr_in& addr_in, DWORD dwSerialID);

	// �߳��ͻ���
	VOID Kick(DWORD dwSerialID);

	// �õ��˿ں�
	INT GetPort() { return m_nPort; }
	TCHAR* GetInfo() { return m_szInfo; }

	CompletionTransport();
	~CompletionTransport();

private:
	TSFPTrunk<CompletionTransport>	m_Trunk;

	// callback functions
	LOGINCALLBACK		m_fnLogIn;		// ��½�ص�
	LOGOUTCALLBACK		m_fnLogOut;		// �ǳ��ص�
	CRYPTCALLBACK		m_fnEncrypt;	// ���ܻص�
	CRYPTCALLBACK		m_fnDecrypt;	// ���ܻص�
	HACKREPORTCALLBACK	m_fnHackReport;	// �����ص�

	// unit data
	UnitPool*				m_pAcceptUnitPool;
	UnitPool*				m_pUnitPool[MAX_UNIT_POOL_NUM];
	SafeQueue<tagUnitData*>*	m_pSendQueue;
	SectCrypt*				m_pCrypt;
	
	TSafeList<tagClientData*>	m_listFreeClientData;

	TSafeMutexList<INT>	m_listCloseClient;	// ��Ҫ�رյ�client data����
	TSafeMutexList<INT> m_listUnauthClient;

	// client data
	VOLATILE DWORD		m_dwClientSerialID;
	tagClientData*		m_ClientData;
	

	VOLATILE BOOL		m_bPreShutdown;		// �Ƿ���Ҫ׼���ر�

	// thread info
	VOLATILE BOOL		m_bTerminateWorkThread;	// �Ƿ���������߳�
	VOLATILE LONG		m_lThreadCounter;	// �������̼߳���
	std::vector<HANDLE>	m_vecWorkThreadHandle;	// ���������̵߳�vector

	// pool balance
	VOLATILE INT		m_nSmallPool;
	VOLATILE INT		m_nBigPool;
	VOLATILE BOOL		m_bOverBalance;	// �ǳ���ƽ��
	
	// base config
	VOLATILE INT		m_nMaxLoad;		// �������������
    VOLATILE SOCKET		m_sockListener;	// ����socket
	VOLATILE HANDLE		m_hCompletionPort; // ��������ɶ˿�
	VOLATILE INT		m_nPort;		// �������˿�
	VOLATILE INT		m_nAcceptExs;	// �ų���AcceptEx����

	TSafeMap<DWORD, INT>	m_mapClientSerialIndex;	// Client SerialID -> index

	// thread stuff
	HANDLE	m_hEventSetupAccepEx;

	tagServerInitParam	m_InitParam;	// ��������
	TCHAR				m_szInfo[1024];	// ������Ϣ���ɹ��ⲿ��ʾ

	// ר�ŷų���������������߳�
	BOOL	CreateSetupAccepExThread(INT nAcceptNum);
	UINT	ThreadSetupAcceptEx(LPVOID);
	// ר�������ر�socket���߳�
	BOOL	CreateCloseSocketThread();
	UINT	ThreadCloseSocket(LPVOID);
	// ����δ��֤�ͻ��˵���ʱ�߳��߳�
	BOOL	CreateUnauthHeartbeatThread();
	UINT	ThreadUnauthHeartbeat(LPVOID);
	// ��ɶ˿�֪ͨ�����߳�
	BOOL	CreateWorkThread(INT nThreadNum);
	UINT	ThreadWorker(LPVOID);
	// ����������߳�	
	BOOL	CreateSendThread();
	UINT	ThreadSend(LPVOID);
	// �ڴ��ƽ���߳� 	
	BOOL	CreatePoolBalanceThread();
	UINT	ThreadPoolBalance(LPVOID);

	// ���û�����ʱ����
	VOID	PerformClientLogout(tagUnitData* pUnit);
	// ������һ���client data
	VOID	ClearAndReturnClientData(tagClientData* pClient);


	// Ĭ�ϼ��ܽ��ܺ���
	UINT	DefaultEncryptFunction(tagUnitData*);
	UINT	DefaultDecryptFunction(tagUnitData*);

	// ���unit�Ƿ���ȫ�鷵,����û��,���ҳ����ǵĵ�ַ
	VOID	CheckUnit();

};



//-----------------------------------------------------------------------------
// ����unit�����ش��û����ж��ٰ�δ����ȥ
//-----------------------------------------------------------------------------
INT CompletionTransport::SendUnit(tagUnitData* pUnit)
{
	pUnit->eState = EUS_Send;

	// �����ڴ˴���nSendCast,�����ܷ���threadsend
	// ��Ϊ�϶�����Ϸ��ѭ�����ô˺�������logout�ص�������δ����
	// �����Ϳ��Ա�֤�߼�������
	tagClientData* pClient = &m_ClientData[pUnit->nCDIndex];
	InterlockedIncrement((LONG*)&pClient->nSendCast);

	m_pSendQueue->Add(pUnit);
	return pClient->nSendCast;
}


//-----------------------------------------------------------------------------
// ����unit,�����Եõ���ǰ�յ���unit��Ŀ�ܺ�
//-----------------------------------------------------------------------------
tagUnitData* CompletionTransport::RecvUnit(DWORD& dwRecvedNum, INT nCDIndex)
{
	ASSERT(nCDIndex >= -1 && nCDIndex < m_nMaxLoad);

	tagClientData* pClient = &m_ClientData[nCDIndex];
	if( FALSE == pClient->bReadyForRecv )
		return NULL;

	dwRecvedNum = pClient->pRecvQueue->GetNum();
	return pClient->pRecvQueue->Get();
}



//-----------------------------------------------------------------------------
// return unit
//-----------------------------------------------------------------------------
VOID CompletionTransport::ReturnUnit(tagUnitData* pUnit)
{
	ASSERT(pUnit);
	// ���Ƚ�ƽ��,������ѡ����˲�������������pool��return
	if( FALSE == m_bOverBalance )	
	{
		if( m_pUnitPool[m_nSmallPool]->TryFreeUnit(pUnit) )
			return;

		for( INT n=0; n<m_lThreadCounter; n++ )
		{
			if( m_pUnitPool[n+1]->TryFreeUnit(pUnit) )
				return;
		}
	}

	// ���ȴ�����critical section
	m_pUnitPool[m_nSmallPool]->FreeUnit(pUnit);
}



//-----------------------------------------------------------------------------
// get free unit
//-----------------------------------------------------------------------------
tagUnitData* CompletionTransport::GetFreeUnit()
{
	tagUnitData* pUnit = NULL;
	// ���Ƚ�ƽ��,������ѡ����˲�������������pool��get
	if( FALSE == m_bOverBalance )	
	{
		if( pUnit = m_pUnitPool[m_nBigPool]->TryAllocUnit() )
			return pUnit;

		for( INT n=0; n<m_lThreadCounter; n++ )
		{
			if( pUnit = m_pUnitPool[n+1]->TryAllocUnit() )
				return pUnit;
		}
	}
	
	// ���ȴ�����critical section
	return m_pUnitPool[m_nBigPool]->AllocUnit();
}


} // namespace vEngine {
