//-----------------------------------------------------------------------------
//!\file proof_policy_own.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief �Լ���˾����֤���ԡ������뼤��
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "proof_policy.h"

//-----------------------------------------------------------------------------
// ����֤���Ե����ݿ�
//-----------------------------------------------------------------------------
class ProofPolicyOwnDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyOwnDB() {}
	~ProofPolicyOwnDB() {}

	//-------------------------------------------------------------------------
	// ��ʼ��
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// ��ѯ
	//-------------------------------------------------------------------------
	BOOL	QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pResult);
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdatePsd(LPCSTR szAccountName, LPCSTR szNewPsd);
	BOOL	UpdateMibao(LPCSTR szAccountName, LPCSTR szMibao);

private:
	DataBase		m_DB;
};

//-----------------------------------------------------------------------------
// ��֤����
//-----------------------------------------------------------------------------
class ProofPolicyOwn : public ProofPolicy
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyOwn() : m_Trunk(this), m_nPort(0), m_bTerminateUpdate(FALSE), m_bTerminateConnect(FALSE), m_bConnected(FALSE) {}
	~ProofPolicyOwn() {}

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ��֤�ӿ�
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP);

	//-------------------------------------------------------------------------
	// ��֤����������״̬
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// ��ҵǳ��¼�
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID){};

private:
	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	VOID	ThreadUpdate();

	//-------------------------------------------------------------------------
	// �����߳�
	//-------------------------------------------------------------------------
	VOID	ThreadConnectServer();

	//-------------------------------------------------------------------------
	// ��ʼ������
	//-------------------------------------------------------------------------
	BOOL	InitConfig();

	//-------------------------------------------------------------------------
	// ������֤�б�
	//-------------------------------------------------------------------------
	VOID	UpdateProofList();

	//-------------------------------------------------------------------------
	// ������Ϣ
	//-------------------------------------------------------------------------
	VOID	UpdateSession();

	//-------------------------------------------------------------------------
	// ��Ϣע��
	//-------------------------------------------------------------------------
	VOID	RegisterProofMsg();
	VOID	UnregisterProofMsg();

	//-------------------------------------------------------------------------
	// ��Ϣ������
	//-------------------------------------------------------------------------
	DWORD	HandleLogin(tagNetCmd* pCmd, DWORD);
	DWORD	HandleUserLogin(tagNetCmd* pCmd, DWORD);
	DWORD	HandleUserUpdatePwd(tagNetCmd* pCmd, DWORD);
	DWORD	HandleUserBindMibao(tagNetCmd* pCmd, DWORD);
	DWORD	HandleUserUnbindMibao(tagNetCmd* pCmd, DWORD);
	DWORD	HandleBlockAccount(tagNetCmd* pCmd, DWORD);
	DWORD	HandleUnBlockAccount(tagNetCmd* pCmd, DWORD);
	DWORD	HandleSetChenMi(tagNetCmd* pCmd, DWORD);

private:
	TSFPTrunk<ProofPolicyOwn>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<StreamTransport>	m_pTransport;

	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateUpdate;
	volatile BOOL				m_bTerminateConnect;


	//-------------------------------------------------------------------------
	// ���Ӳ���
	//-------------------------------------------------------------------------
	CHAR						m_szIP[X_IP_LEN];		// ����������֤��������IP
	INT							m_nPort;				// ����������֤�������Ķ˿�
	volatile BOOL				m_bConnected;			// ���ӱ�־

	//-------------------------------------------------------------------------
	// ��֤����
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		string		strAccoutName;
		string		strPsd;
		string		strGUID;

		tagPlayerProofData(DWORD _dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID)
		{
			dwClientID		=	_dwClientID;
			strAccoutName	=	szAccoutName;
			strPsd			=	szPsd;
			strGUID			=	szGUID;
		}
	};

	TSafeMutexList<tagPlayerProofData*>	m_listProofData;		// ��֤�û�����

	//--------------------------------------------------------------------------
	// ���ݿ�
	//--------------------------------------------------------------------------
	ProofPolicyOwnDB					m_ProofDB;				// ���ݿ�ӿ�
};