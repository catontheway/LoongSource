#pragma once
#include "Singleton.h"

//-----------------------------------------------------------------------------
// ������session
//-----------------------------------------------------------------------------
class FatigueSession : public Singleton<FatigueSession>
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	FatigueSession() : m_Trunk(this), m_nPort(0), m_bTerminateUpdate(FALSE), m_bTerminateConnect(FALSE), m_bConnected(FALSE) {}
	~FatigueSession() {}

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init();
	VOID	Destroy();

	VOID	SendQueryStatus(DWORD dwAccount);
	VOID	SendOLGuardAccounts( DWORD *pOLAccounts, DWORD dwNum );
	DWORD	HandleGuardAccountNotify( PVOID pMsg );

	//-------------------------------------------------------------------------
	// ������Ϣ
	//-------------------------------------------------------------------------
	VOID	Update();

	//-------------------------------------------------------------------------
	// ״̬��Ϣ
	//-------------------------------------------------------------------------
	BOOL	IsConnected()	{	return m_pTransport->IsConnected();	}

private:
	//-------------------------------------------------------------------------
	// �����߳�
	//-------------------------------------------------------------------------
	VOID	ThreadConnectServer();

	//-------------------------------------------------------------------------
	// ��ʼ������
	//-------------------------------------------------------------------------
	BOOL	InitConfig();


private:
	TSFPTrunk<FatigueSession>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;
	TObjRef<StreamTransport>	m_pTransport;

	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateUpdate;
	volatile BOOL				m_bTerminateConnect;

	//-------------------------------------------------------------------------
	// ���Ӳ���
	//-------------------------------------------------------------------------
	CHAR						m_szIP[X_IP_LEN];		// ���ӷ����Է�������IP
	INT							m_nPort;				// ���ӷ����Է������Ķ˿�
	volatile BOOL				m_bConnected;			// ���ӱ�־

};

#define sFatigueSession FatigueSession::getSingleton()

