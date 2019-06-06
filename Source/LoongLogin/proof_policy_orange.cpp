#include "StdAfx.h"
#include "proof_policy_orange.h"
#include "login_server.h"
#include "../ServerDefine/base_define.h"

//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyOrangeDB::Init()
{
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	if( !P_VALID(pVar) ) return FALSE;

	BOOL bRet = m_DB.Init(pVar->GetString(_T("ip database")), 
		pVar->GetString(_T("user database")),
		pVar->GetString(_T("psd database")), 
		pVar->GetString(_T("name database")),
		(INT)pVar->GetDword(_T("port database")) );

	return bRet;
}
//-------------------------------------------------------------------------
// ��ѯ
//-------------------------------------------------------------------------
BOOL ProofPolicyOrangeDB::QueryAccount(DWORD dwAccountID, string& strAccoutName,string& strIP)
{
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	pStream->SetSelect("account", "name,ip");
	pStream->SetWhere();
	pStream->FillString("id=")<<dwAccountID;

	QueryResult* pResult = m_DB.Query(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)|| pResult->GetRowCount() <= 0 ) return FALSE;

	// ��¼����
	strAccoutName = (*pResult)[0].GetString();
	// ��¼ip
	strIP =	(*pResult)[1].GetString();

	return TRUE;

}
//-------------------------------------------------------------------------
// ��ѯ
//-------------------------------------------------------------------------
BOOL ProofPolicyOrangeDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
{
	if( !P_VALID(szAccountName) || !P_VALID(pProofResult) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	char szIP[X_IP_LEN] = "";
	char szDateTime[X_DATATIME_LEN + 1]= "";
	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,psd,mibao,ip,time");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)|| pResult->GetRowCount() <= 0 ) return FALSE;

	// ���ý��
	pProofResult->dwAccountID		=	(*pResult)[0].GetDword();
	pProofResult->dwFrobidMask		=	(*pResult)[1].GetDword();
	pProofResult->byPrivilege		=	(*pResult)[2].GetByte();
	pProofResult->bGuard			=	(*pResult)[3].GetBool();
	pProofResult->nGuardAccuTime	=	0;
	pProofResult->eLoginStatus		=	(EPlayerLoginStatus)(*pResult)[4].GetInt();
	pProofResult->dwWorldNameCrc	=	(*pResult)[5].GetDword();

	// ��¼����
	strPsd = (*pResult)[6].GetString();

	// ��¼�ܱ�
	CHAR szMibao[MIBAO_LEN] = {'\0'};
	BOOL bRet = (*pResult)[7].GetBlob(szMibao, MIBAO_LEN);
	if( '\0' == szMibao[0] )
	{
		pProofResult->bNeedMibao = FALSE;
	}
	else
	{
		pProofResult->bNeedMibao = TRUE;
	}

	//�ϴε�¼��ʱ���ip
	memcpy(szIP,(*pResult)[8].GetString(),(*pResult)[8].GetLen());
	memcpy(szDateTime,(*pResult)[9].GetString(),(*pResult)[9].GetLen());

	pProofResult->dwPreLoginIP = pTrans->StringIP2IP(szIP);
	DataTime2DwordTime(pProofResult->dwPreLoginTime,szDateTime,(*pResult)[9].GetLen());

	// �黹�����
	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-------------------------------------------------------------------------
// ������Ҽ�¼
//-------------------------------------------------------------------------
BOOL ProofPolicyOrangeDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetInsert("account");
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("',");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}

//----------------------------------------------------------------------------
// ������Ҽ�¼
//----------------------------------------------------------------------------
BOOL ProofPolicyOrangeDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}

/*
/	orange��֤����
/
*/
//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyOrange::Init(PROOFCALLBACK fn)
{
	m_pUtil		=	"Util";
	m_pThread	=	"Thread";

	m_fnCallback = fn;

	// ��ȡ�����ļ�
	if( !InitConfig() )
	{
		return FALSE;
	}

	// ��ʼ�����ݿ�
	if( !m_ProofDB.Init() )
	{
		return FALSE;
	}
	

	// ��ʼ������

	//����
	CreateObj("StreamTransPPOrangeLogin", "StreamTransport");
	m_pTransLogin = "StreamTransPPOrangeLogin";
	if( !P_VALID(m_pTransLogin) ) return FALSE;

	//�ǳ�
	CreateObj("StreamTransPPOrangeLogout", "StreamTransport");
	m_pTransLogout = "StreamTransPPOrangeLogout";
	if( !P_VALID(m_pTransLogout) ) return FALSE;


	m_pTransLogin->Init(TRUE,ESTM_Stream); //
	// ���������߳�
	if(!m_pThread->CreateThread(_T("PPOrangeConnectLoginServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadConnectLoginServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPOrangeConnectLoginServer")))
	{
		continue;
	}

	// ���������߳�
	if(!m_pThread->CreateThread(_T("PPOrangeLoginThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadLoginServerUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPOrangeLoginThreadUpdate")))
	{
		continue;
	}


	m_pTransLogout->Init(TRUE,ESTM_Stream); //
	// ���������߳�
	if(!m_pThread->CreateThread(_T("PPOrangeConnectLogoutServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadConnectLogoutServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPOrangeConnectLogoutServer")))
	{
		continue;
	}

	// ���������߳�
	if(!m_pThread->CreateThread(_T("PPOrangeLogoutThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadLogoutServerUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPOrangeLogoutThreadUpdate")))
	{
		continue;
	}
	
	return TRUE;
}


//-------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::Destroy()
{

	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateLoginUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPOrangeLoginThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateLoginConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPOrangeConnectLoginServer"), INFINITE);

	//�ǳ�
	InterlockedExchange((LPLONG)&m_bTerminateLogoutUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPOrangeLogoutThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateLogoutConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPOrangeConnectLogoutServer"), INFINITE);


	// ��������
	m_pTransLogin->Destroy();
	KillObj("StreamTransPPOrangeLogin");

	m_pTransLogout->Destroy();
	KillObj("StreamTransPPOrangeLogout");

	// ɾ������list�е�����
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}

}
//-------------------------------------------------------------------------
// ���ӶԷ�login�������߳�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::ThreadConnectLoginServer()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTerminateLoginConnect )
	{
		if( !m_pTransLogin->IsConnected() )
		{
			if( !m_pTransLogin->IsTryingToConnect() )
			{
				m_pTransLogin->TryToConnect(m_szLoginIP, m_nLoginPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to center login server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szLoginIP), m_nLoginPort);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
}

//-------------------------------------------------------------------------
// ���ӶԷ�logout�������߳�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::ThreadConnectLogoutServer()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTerminateLogoutConnect )
	{
		if( !m_pTransLogout->IsConnected() )
		{
			if( !m_pTransLogout->IsTryingToConnect() )
			{
				m_pTransLogout->TryToConnect(m_szLogoutIP, m_nLogoutPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to center logout server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szLogoutIP), m_nLogoutPort);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
}

//-------------------------------------------------------------------------
// ����logout�߳�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::ThreadLogoutServerUpdate()
{
	DWORD dwTime = 0;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateLogoutUpdate )
		{
			// �õ���ǰʱ��
			dwTime = timeGetTime();

			// ������֤�б�
			UpdatePlayerLogouList();

			// ������Ϣ
			UpdateLogoutSession();

			// ��Ϣ
			Sleep(50);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		//����ǵ��ԣ����׳��쳣
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

	_endthreadex(0);
}
//-------------------------------------------------------------------------
// ����login�߳�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::ThreadLoginServerUpdate()
{
	DWORD dwTime = 0;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateLoginUpdate )
		{
			// �õ���ǰʱ��
			dwTime = timeGetTime();

			// ������֤�б�
			UpdateProofList();

			// ������Ϣ
			UpdateLoginSession();

			// ��Ϣ
			Sleep(50);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		//����ǵ��ԣ����׳��쳣
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

	_endthreadex(0);
}
//-------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------
BOOL ProofPolicyOrange::InitConfig()
{
	// ���������ļ�
	CreateObj("ProofPolicyOrange", "VarContainer");
	TObjRef<VarContainer> pVar = "ProofPolicyOrange";

	if( !P_VALID(pVar) ) return FALSE;

	pVar->Load("VirtualFileSys", _T("server_config/login/orange_proof.ini"));

	// ��ȡIP�Ͷ˿�
	//��¼��
	TCHAR szIP[X_IP_LEN] = {'\0'};
	_tcsncpy(szIP, pVar->GetString(_T("ip login_server")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szLoginIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szLoginIP) - 1);

	m_nLoginPort = pVar->GetInt(_T("port login_server"));

	//�ǳ���
	_tcsncpy(szIP, pVar->GetString(_T("ip logout_server")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szLogoutIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szLogoutIP) - 1);

	m_nLogoutPort = pVar->GetInt(_T("port logout_server"));

	//Server Code
	TCHAR szServiceCode[X_OG_SC_LEN] = {'\0'};
	_tcsncpy(szServiceCode, pVar->GetString(_T("servercode orange")), MCalTCharArrayNum(szServiceCode) - 1);
	memcpy(m_szServiceCode, m_pUtil->UnicodeToUnicode8(szServiceCode), sizeof(m_szServiceCode));

	//Region
	TCHAR szRegion[X_OG_REGION_LEN] = {'\0'};
	_tcsncpy(szRegion, pVar->GetString(_T("region orange")), MCalTCharArrayNum(szRegion) - 1);
	memcpy(m_szRegion, m_pUtil->UnicodeToUnicode8(szRegion), sizeof(m_szRegion));

	// ɾ�������ļ�����
	KillObj("ProofPolicyOrange");

	return TRUE;
}
//-------------------------------------------------------------------------
// ������ҵǳ��б�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::UpdatePlayerLogouList()
{
	if(!m_pTransLogin->IsConnected())
		return;

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	int iNum = 0;
	DWORD dwAccountID = m_listPlayerLogout.PopFront();
	while( P_VALID(dwAccountID) && iNum<50)
	{

		++iNum;

		string strAccountName;
		string strIP;
		BOOL bRet = m_ProofDB.QueryAccount(dwAccountID,strAccountName,strIP);
		if(bRet == TRUE)
		{
			//Send To: 610108'TF'Username'R'''61.62.183.73'11731016;Username'
			//ƴ��֤����
			stringstream strLogoutFrame;
			strLogoutFrame<<m_szServiceCode;
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<m_szRegion;
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<strAccountName;
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<"R";
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<""; //
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<""; //
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<strIP;// ��¼ip
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<"";//
			strLogoutFrame<<X_OG_SEND_DIVIDE_SIGN;
			strLogoutFrame<<"\r\n";

			//��������
			m_pTransLogout->Send((void *)strLogoutFrame.str().c_str(),strLogoutFrame.str().length());
			
		}
		dwAccountID = m_listPlayerLogout.PopFront();		// ȡ����һ��
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif


}
//-------------------------------------------------------------------------
// ������֤�б�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::UpdateProofList()
{
	if(!m_pTransLogin->IsConnected())
		return;

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif


	// ���б���ȡ����֤����
	tagProofResult result;

	tagPlayerProofData* pData = m_listProofData.PopFront();
	int iNum = 0;

	//һ�δ���50��
	while( P_VALID(pData) && iNum<50)
	{

		++iNum;

		//////////////////////////////////////////////////////////////////////////
		//�������ݶ���ansi

		TObjRef<StreamTransport> pTrans;
		pTrans = "StreamTransport";

		//psd����
		string strAccoutName;
		string strEncryptPsd; //�Ѽ��ܵ�����
		vector <string> vecOrangeData;
		string strAccountData = (CHAR *)pData->byAccountData;
		//��������
		Proof_GetOrangeData(strAccountData,vecOrangeData,X_CLIENT_RECEIVE_DIVIDE_SIGN);
		if(vecOrangeData.size() < 2)
			continue;
		strAccoutName = vecOrangeData[0];
		strEncryptPsd = vecOrangeData[1];

		//��������
		INT32 nLen = strEncryptPsd.length();
		LPSTR pDecryptPsd = new CHAR[nLen];
		ZeroMemory(pDecryptPsd, nLen);
		IUTIL->Decrypt((void *)strEncryptPsd.c_str(), pDecryptPsd, nLen, 0);

		//Send To: 610108'TF'Username'Password'1'I'0'218.168.85.252'319612568;Username'
		//ƴ��֤����
		stringstream strLoginFrame;
		strLoginFrame<<m_szServiceCode;
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<m_szRegion;
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<strAccoutName;
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<pDecryptPsd;
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<"1"; //1:AuthenticationFlag     1��ͬ�r������C (Authentication)
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<"I"; //I�ڙ� (Authorization) (Default)	P��ԃʣ�N���c��
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<"0"; //DuplicateCheck
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<pTrans->IP2String(pData->dwIP);// ��¼ip
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<pData->dwClientID;// dwClientID
		strLoginFrame<<X_OG_SEND_DIVIDE_SIGN;
		strLoginFrame<<"\r\n";

		//////////////////////////////////////////////////////////////////////////

		//��������
		m_pTransLogin->Send((void *)strLoginFrame.str().c_str(),strLoginFrame.str().length());

		//��ȫɾ��
		SAFE_DEL(pData);
		SAFE_DEL(pDecryptPsd);

		pData = m_listProofData.PopFront();		// ȡ����һ��
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif


}
//-------------------------------------------------------------------------
// ����Logout��Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::UpdateLogoutSession()
{
	if( NULL == m_pTransLogout )
		return;

	// ������
	if(!m_pTransLogout->IsConnected() && !m_pThread->IsThreadActive(_T("PPOrangeConnectLogoutServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateLogoutConnect, TRUE);
		m_pTransLogout->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("PPOrangeConnectLogoutServer"), INFINITE);

		// ����������֤�����������߳�
		InterlockedExchange((LONG*)&m_bTerminateLogoutConnect, FALSE);
		m_pThread->CreateThread(_T("PPOrangeConnectLogoutServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadConnectLogoutServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("PPOrangeConnectLogoutServer")))
		{
			m_pThread->CreateThread(_T("PPOrangeConnectLogoutServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadConnectLogoutServer), NULL);
			continue;
		}

		return;
	}

	// ��������Orange����������Ϣ
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransLogout->Recv(dwSize);

	while( P_VALID(pRecv) )
	{	
		//Orange��֤�ķ�����Ϣ
		m_pTransLogout->FreeRecved(pRecv);

		pRecv = m_pTransLogout->Recv(dwSize);
	}
}

//-------------------------------------------------------------------------
// ����Login��Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::UpdateLoginSession()
{
	if( NULL == m_pTransLogin )
		return;

	// ������
	if(!m_pTransLogin->IsConnected() && !m_pThread->IsThreadActive(_T("PPOrangeConnectLoginServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, TRUE);
		m_pTransLogin->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("PPOrangeConnectLoginServer"), INFINITE);

		// ����������֤�����������߳�
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, FALSE);
		m_pThread->CreateThread(_T("PPOrangeConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadConnectLoginServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("PPOrangeConnectLoginServer")))
		{
			m_pThread->CreateThread(_T("PPOrangeConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOrange::ThreadConnectLoginServer), NULL);
			continue;
		}

		return;
	}

	// ��������Orange����������Ϣ
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransLogin->Recv(dwSize);

	while( P_VALID(pRecv) )
	{	
		//����Orange��֤������Ϣ
		HandleOrangeLogin(pRecv);

		//Orange��֤�ķ�����Ϣ
		m_pTransLogin->FreeRecved(pRecv);

		pRecv = m_pTransLogin->Recv(dwSize);
	}

}

//-------------------------------------------------------------------------
// ����Orange��֤��Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::HandleOrangeLogin(LPBYTE pRecv)
{
	//Response		Type	Length	Description
	//ResultCode	String	max 2	1:�ɹ� ,	0:ʧ�� , -1:ϵ�y�e�`
	//Outstring		String			���������f��
	//G-ConsoleCode String			�Է�Ψһ��ʾһ���û���code
	//UniqueID		String			�_�l��ݔ��ķ��ID (�˅�����option)

 	tagProofResult result;

	string strAccountName;
	//��������
	string strPsd = "***";
	// �鿴������
	INT nRet = E_Success;	

	vector <string> vecOrangeData;
	//ֱ��ת
	string strOrangeRecv = (CHAR*)pRecv;
	//��������
	Proof_GetOrangeData(strOrangeRecv,vecOrangeData,X_OG_RECEIVE_DIVIDE_SIGN);
	//���������������
	if (vecOrangeData.size() < 4)
	{
		nRet = E_ProofResult_Account_No_Match;
		// ���ûص�����
		(*m_fnCallback)(nRet, &result);
		return;
	}

	//�ͻ�id
	result.dwClientID = atoi(vecOrangeData[3].c_str());

	if(0 != memcmp(vecOrangeData[0].c_str(),"1",vecOrangeData[0].size()))
	{
		//��̖���i
		if(0 == memcmp(vecOrangeData[1].c_str(),X_FROBID_MEMBER_CENTER,vecOrangeData[1].size()))
		{
			nRet = E_ProofResult_Forbid_MemberCenter;
		}
		else
		{
			nRet = E_ProofResult_Account_No_Match;
		}
	}
	else 
	{
		do 
		{
			strAccountName = vecOrangeData[2];

			//������
			BOOL bGuard = FALSE;
			CHAR szMibao[MIBAO_LEN] = ""; //û���ܱ�
			//�������ݿ�
			BOOL bRet = m_ProofDB.InsertAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			if( !bRet )
			{
				bRet = m_ProofDB.UpdateAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			}

		} while (0);
	}
	if( E_ProofResult_Account_No_Match != nRet && E_ProofResult_Forbid_MemberCenter != nRet )
	{

		BOOL bRet = m_ProofDB.QueryAccount(strAccountName.c_str(), strPsd, &result);

		if( bRet )
		{
			if( EPLS_OffLine != result.eLoginStatus )
			{
				nRet = E_ProofResult_Account_In_Use;
			}
			else if( result.dwFrobidMask != 0 )
			{
				do{
					if(result.dwFrobidMask & EPLM_GMTool)
					{
						//������������ûص�����ʱ��Ҫworld_forbid������һ��
						nRet = E_ProofResult_Forbid_GMTool;						
						break;
					}
					if(result.dwFrobidMask & EPLM_WaiGua)
					{
						nRet = E_ProofResult_Disabled;						
						break;
					}
				}while(0);
			}
		}
		else
		{
			nRet = E_ProofResult_Account_No_Match;
		}
	}

	// ���ûص�����
	(*m_fnCallback)(nRet, &result);
}
//-----------------------------------------------------------------------------------------------
// ȥ���س�����
//-----------------------------------------------------------------------------------------------
BOOL ProofPolicyOrange::CheckString(string& str)
{
	// �Իس����н���
	INT nFind = 0;
	while( (nFind = str.find('\r', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// 
	nFind = 0;
	while( (nFind = str.find('\n', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// ����û����Ƿ�Ϊ��
	return str.empty();
}
//-----------------------------------------------------------------------------------------------
// �õ����������� �����ŷָ�
//-----------------------------------------------------------------------------------------------
VOID ProofPolicyOrange::Proof_GetOrangeData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		string strTemp;

		icurpos = strFrameString.find(cDivideSign,iprepos);
		//���һ��
		if(icurpos == -1)
		{
			strTemp = strFrameString.substr(iprepos,strFrameString.length() - iprepos);

			//������һ���ַ���
			CheckString(strTemp);

			vecOrangeData.push_back(strTemp);
			break;
		}
		strTemp = strFrameString.substr(iprepos,icurpos-iprepos);

		vecOrangeData.push_back(strTemp);
		iprepos = icurpos + 1;
	}
}

//-------------------------------------------------------------------------
// ��ҵǳ��¼�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::SigPlayerLogoutEvent(DWORD dwAccountID)
{
	if(!P_VALID(dwAccountID)) return;
	m_listPlayerLogout.PushBack(dwAccountID);
}

//-------------------------------------------------------------------------
// ��֤�ӿ�
//-------------------------------------------------------------------------
VOID ProofPolicyOrange::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) return;

	// ����һ���µ���֤��Ϣ
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName, szPsd, szGUILD,dwIP);

	m_listProofData.PushBack(pData);

}

//-------------------------------------------------------------------------
// ��֤����������״̬
//-------------------------------------------------------------------------
INT ProofPolicyOrange::GetProofServerStatus()
{
	if (P_VALID(m_pTransLogin) && m_pTransLogin->IsConnected())
	{
		return EWS_Well;
	}

	return EWS_ProofError;
}