#pragma once
#include "proof_policy.h"
#include "proof_policy_orange_define.h"

//-----------------------------------------------------------------------------
// ����֤���Ե����ݿ�
//-----------------------------------------------------------------------------
class ProofPolicyOrangeDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyOrangeDB() {}
	~ProofPolicyOrangeDB() {}

	//-------------------------------------------------------------------------
	// ��ʼ��
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// ��ѯ
	//-------------------------------------------------------------------------
	BOOL	QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pResult);
	BOOL	QueryAccount(DWORD dwAccountID, string& strAccoutName,string& strIP);

	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);


private:
	DataBase		m_DB;
};

class ProofPolicyOrange :
	public ProofPolicy
{
public:
	ProofPolicyOrange(void): m_Trunk(this), m_nLoginPort(0),m_nLogoutPort(0), m_bTerminateLoginUpdate(FALSE), m_bTerminateLoginConnect(FALSE)
		, m_bTerminateLogoutUpdate(FALSE), m_bTerminateLogoutConnect(FALSE){}
	~ProofPolicyOrange(void){}

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ��֤�ӿ�
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID,DWORD dwIP);

	//-------------------------------------------------------------------------
	// ��֤����������״̬
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// ��ҵǳ��¼�
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID);

private:

	//-----------------------------------------------------------------------------------------------
	// �õ����������� �����ŷָ�
	//-----------------------------------------------------------------------------------------------
	VOID Proof_GetOrangeData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign);

	//-------------------------------------------------------------------------
	// ����login���������߳�
	//-------------------------------------------------------------------------
	VOID	ThreadLoginServerUpdate();

	//-------------------------------------------------------------------------
	// ����logout���������߳�
	//-------------------------------------------------------------------------
	VOID	ThreadLogoutServerUpdate();

	//-------------------------------------------------------------------------
	// ���ӶԷ�login�������߳�
	//-------------------------------------------------------------------------
	VOID	ThreadConnectLoginServer();

	//-------------------------------------------------------------------------
	// ���ӶԷ�logout�������߳�
	//-------------------------------------------------------------------------
	VOID	ThreadConnectLogoutServer();

	//-------------------------------------------------------------------------
	// ��ʼ������
	//-------------------------------------------------------------------------
	BOOL	InitConfig();

	//-------------------------------------------------------------------------
	// ������֤�б�
	//-------------------------------------------------------------------------
	VOID	UpdateProofList();

	//-------------------------------------------------------------------------
	// ����login��������Ϣ
	//-------------------------------------------------------------------------
	VOID	UpdateLoginSession();

	//-------------------------------------------------------------------------
	// ����logout��������Ϣ
	//-------------------------------------------------------------------------
	VOID	UpdateLogoutSession();

	//-------------------------------------------------------------------------
	// ����Orange��֤��Ϣ
	//-------------------------------------------------------------------------
	VOID	HandleOrangeLogin(LPBYTE pRecv);

	//-------------------------------------------------------------------------
	// ���µǳ��б�
	//-------------------------------------------------------------------------
	VOID UpdatePlayerLogouList();

	//-----------------------------------------------------------------------------------------------
	// ȥ���س�����
	//-----------------------------------------------------------------------------------------------
	BOOL CheckString(string& str);

private:

	TSFPTrunk<ProofPolicyOrange>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;

	//����
	TObjRef<StreamTransport>	m_pTransLogin;

	//�ǳ�
	TObjRef<StreamTransport>	m_pTransLogout;


	//��ThreadUpdate���� ����Ҫ�����̰߳�ȫ
	std::map<DWORD, string>		m_mapClientIDVsAccount;	// ClientID������˺ŵĶ��ձ�
	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateLoginUpdate;
	volatile BOOL				m_bTerminateLoginConnect;

	volatile BOOL				m_bTerminateLogoutUpdate;
	volatile BOOL				m_bTerminateLogoutConnect;
	//-------------------------------------------------------------------------
	// ���Ӳ���
	//-------------------------------------------------------------------------
	CHAR						m_szLoginIP[X_IP_LEN];		// ����orange��֤��������IP
	INT							m_nLoginPort;				// ����orange��֤�������Ķ˿�

	CHAR						m_szLogoutIP[X_IP_LEN];		// ����orange��֤��������IP
	INT							m_nLogoutPort;				// ����orange��֤�������Ķ˿�

	CHAR						m_szServiceCode[X_OG_SC_LEN];		// ���[�������ṩ
	CHAR						m_szRegion[X_OG_REGION_LEN];		// ���[�������ṩ

	//-------------------------------------------------------------------------
	// ��֤����
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		string		strAccoutName;
		string		strPsd;
		BYTE		byAccountData[X_LONG_NAME];		//
		DWORD		dwIP;				//ip

		tagPlayerProofData(DWORD _dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID,DWORD _dwIP)
		{
			dwClientID		=	_dwClientID;
			strAccoutName	=	szAccoutName;
			strPsd			=	szPsd;
			memcpy(byAccountData,szGUID,X_LONG_NAME);
			dwIP			=	_dwIP;
		}
	};

	TSafeList<tagPlayerProofData*>		m_listProofData;		// ��֤�û�����

	TSafeList<DWORD>					m_listPlayerLogout;		// ��ҵǳ��ж�			
	//--------------------------------------------------------------------------
	// ���ݿ�
	//--------------------------------------------------------------------------
	ProofPolicyOrangeDB					m_ProofDB;				// ���ݿ�ӿ�
};
