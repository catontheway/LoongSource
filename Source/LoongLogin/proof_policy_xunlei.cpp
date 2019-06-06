#include "StdAfx.h"
#include "proof_policy_xunLei.h"

#include "../ServerDefine/base_define.h"

#include "proof_policy.h"
#include "login_server.h"

#include "proof_policy_dll_api.h" //��֤���Զ�̬��api

#include "proof_policy_xunlei_define.h"

//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyXunLeiDB::Init()
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
BOOL ProofPolicyXunLeiDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
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
BOOL ProofPolicyXunLeiDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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
BOOL ProofPolicyXunLeiDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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
/	Ѹ����֤����
/
*/

//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyXunLei::Init(PROOFCALLBACK fn)
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
//songg
	m_hXunLeiDll = LoadLibrary(_T("Login.dll"));
	if( m_hXunLeiDll == NULL )//if the handle is NULL, it is failed.
	{
		ASSERT(0);
		return FALSE;
	}
	else
	{
		InitProofXunleiInterface(m_hXunLeiDll);
	}

	// ��ʼ������
	CreateObj("StreamTransportPPXunLei", "StreamTransport");
	m_pTransport = "StreamTransportPPXunLei";
	if( !P_VALID(m_pTransport) ) return FALSE;

	m_pTransport->Init(); //
	m_pTransport->SetPackageSizeEndian(TRUE); //�����հ�ʱ�İ�ͷsize���ֽ�˳��
	// ���������߳�
	if(!m_pThread->CreateThread(_T("ProofPolicyXunLeiConnectServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyXunLei::ThreadConnectServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyXunLeiConnectServer")))
	{
		continue;
	}

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ProofPolicyXunLeiThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyXunLei::ThreadUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyXunLeiThreadUpdate")))
	{
		continue;
	}

	return TRUE;
}
//-------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------
VOID ProofPolicyXunLei::Destroy()
{

	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyXunLeiThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyXunLeiConnectServer"), INFINITE);

	// ��������
	m_pTransport->Destroy();
	KillObj("StreamTransportPPXunLei");

	// ɾ������list�е�����
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}


	FreeLibrary(m_hXunLeiDll);
}
//-------------------------------------------------------------------------
// �����߳�
//-------------------------------------------------------------------------
VOID ProofPolicyXunLei::ThreadConnectServer()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTerminateConnect )
	{
		if( !m_pTransport->IsConnected() )
		{
			if( !m_pTransport->IsTryingToConnect() )
			{
				m_pTransport->TryToConnect(m_szIP, m_nPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to center server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szIP), m_nPort);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
}
//-------------------------------------------------------------------------
// �����߳�
//-------------------------------------------------------------------------
VOID ProofPolicyXunLei::ThreadUpdate()
{
	DWORD dwTime = 0;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateUpdate )
		{
			// �õ���ǰʱ��
			dwTime = timeGetTime();

			// ������֤�б�
			UpdateProofList();

			// ������Ϣ
			UpdateSession();

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
BOOL ProofPolicyXunLei::InitConfig()
{
	// ���������ļ�
	CreateObj("ProofPolicyXunLei", "VarContainer");
	TObjRef<VarContainer> pVar = "ProofPolicyXunLei";

	if( !P_VALID(pVar) ) return FALSE;

	pVar->Load("VirtualFileSys", _T("server_config/login/xunlei_proof.ini"));

	// ��ȡIP�Ͷ˿�
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip server")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	m_nPort = pVar->GetInt(_T("port server"));

	// ɾ�������ļ�����
	KillObj("ProofPolicyXunLei");

	return TRUE;
}
//-------------------------------------------------------------------------
// ������֤�б�
//-------------------------------------------------------------------------
VOID ProofPolicyXunLei::UpdateProofList()
{
	if(!m_pTransport->IsConnected())
		return;

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif


	// ���б���ȡ����֤����
	tagProofResult result;

	tagPlayerProofData* pData = m_listProofData.PopFront();

	//ֱ�ӷ��첽����֤��Ϣ
	stXunLeiRequireData stXunLeiRequire;
	stXunLeiLoginFrame  stXunLeiRequireFrame;


	//�洢stXunLeiRequire���ܺ������
	CHAR *pcEncryptData = new CHAR[XL_ENCRYPT_REQUIRE_DATA];


	int iNum = 0;

	//һ�δ���50��
	while( P_VALID(pData) && iNum<50)
	{

		++iNum;

		//����	���к�	�˺�������ܰ�  112byte
		memcpy(stXunLeiRequire.szCommand,XL_CMD_LOGIN,5);
		//�����п��ܳ�����
		stXunLeiRequire.usSerialNum = pData->dwClientID;
		memcpy(stXunLeiRequire.byAccountData,pData->byAccountData,100);

		//����Ѹ���ṩ�ļ����㷨 ���ܺ�����116�ֽ� songg
		PROOF_XUNLEI_AesEncrypt((char *)&stXunLeiRequire,pcEncryptData,XL_DECRYPT_REQUIRE_DATA);

		//////////////////////////////////////////////////////////////////////////

		//126��byte ֡�Ĵ�С
		DWORD dwFrameSize = sizeof(stXunLeiRequireFrame.szGameID)\
			+sizeof(stXunLeiRequireFrame.szProtocolVersion)+ XL_ENCRYPT_REQUIRE_DATA;
		//��Ϸid
		memcpy(stXunLeiRequireFrame.szGameID,XL_GAMEID,5);
		//Э��汾 ��������ո�

		//ʵ������
		memcpy(stXunLeiRequireFrame.szEncryptData,pcEncryptData,XL_ENCRYPT_REQUIRE_DATA);

		//////////////////////////////////////////////////////////////////////////

		//��������
		m_pTransport->Send(&stXunLeiRequireFrame,dwFrameSize);

		//��ȫɾ��
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();		// ȡ����һ��
	}

	//��ȫɾ��
	SAFE_DEL_ARRAY(pcEncryptData);



#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif


}

//-------------------------------------------------------------------------
// ������Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyXunLei::UpdateSession()
{

	if( NULL == m_pTransport )
		return;

	// ������
	if(!m_pTransport->IsConnected() && !m_pThread->IsThreadActive(_T("ProofPolicyXunLeiConnectServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateConnect, TRUE);
		m_pTransport->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ProofPolicyXunLeiConnectServer"), INFINITE);

		// ����������֤�����������߳�
		InterlockedExchange((LONG*)&m_bTerminateConnect, FALSE);
		m_pThread->CreateThread(_T("ProofPolicyXunLeiConnectServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyXunLei::ThreadConnectServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ProofPolicyXunLeiConnectServer")))
		{
			m_pThread->CreateThread(_T("ProofPolicyXunLeiConnectServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyXunLei::ThreadConnectServer), NULL);
			continue;
		}

		return;
	}

	// ��������xunlei����������Ϣ
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransport->Recv(dwSize);

	while( P_VALID(pRecv) )
	{	
		//����xunlei��֤������Ϣ
		HandleXunLeiLogin(pRecv);

		//xunlei��֤�ķ�����Ϣ
		m_pTransport->FreeRecved(pRecv);

		pRecv = m_pTransport->Recv(dwSize);
		

	}

}
//-------------------------------------------------------------------------
// �����ַ���
//-------------------------------------------------------------------------
BOOL ProofPolicyXunLei::DeleteRightSideBlankOfString(string &str)
{
	// ȥ���ո�
	INT nFind = 0;
	if((nFind = str.find(' ')) != str.npos)
	{
		str = str.substr(0,nFind);
	}

	// ����û����Ƿ�Ϊ��
	return !str.empty();
}
//-------------------------------------------------------------------------
// ����xunlei��֤��Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyXunLei::HandleXunLeiLogin(LPBYTE pRecv)
{
	//ֱ��ת
	stXunLeiLoginFrame *pXunLeiRecv = (stXunLeiLoginFrame *)(pRecv);

	tagProofResult result;

	string strPsd = "***";
	// �鿴������
	INT nRet = E_Success;	

	//��֤gameid
	if(0 != memcmp(pXunLeiRecv->szGameID,XL_GAMEID,5))
	{
		ASSERT(0);

		nRet = E_ProofResult_Account_No_Match;
		// ���ûص�����
		(*m_fnCallback)(nRet, &result);
		return;
	}
	//��֤Э��汾 ����Ӧ��5���ո�
	if(0 != memcmp(pXunLeiRecv->szProtocolVersion,"     ",5))
	{
		ASSERT(0);

		nRet = E_ProofResult_Account_No_Match;
		// ���ûص�����
		(*m_fnCallback)(nRet, &result);
		return;
	}
	//��������
	stXunLeiReturnData stXunleiReturn;
	//����Ѹ���ṩ�Ľ����㷨 songg
	PROOF_XUNLEI_AesDecrypt((char *)pXunLeiRecv->szEncryptData,(char *)&stXunleiReturn);

	//�жϷ�����
	/*
	0000--OK
	0001--�ʺ������
	0002--�����빦�����ã����Ҵ��û������Ѽ����û�֮��
	0003--��֤ʱ�����쳣
	0004--��ȡ�û�IDʧ��
	0007--�û��ѱ����������
	*/


	//�����û���
	string strAccountName(stXunleiReturn.szAccountName,20);
	BOOL bRet = DeleteRightSideBlankOfString(strAccountName);

	//�ͻ�id
	result.dwClientID = stXunleiReturn.usSerialNum;

	if(bRet == FALSE)
	{
		nRet = E_ProofResult_Account_No_Match;
		// ���ûص�����
		(*m_fnCallback)(nRet, &result);
		return;
	}

	if(0 != memcmp(stXunleiReturn.szReturnCode,"0000",sizeof(stXunleiReturn.szReturnCode)))
	{
		nRet = E_ProofResult_Account_No_Match;
	}
	else 
	{
/*
		do 
		{

			//����stXunleiReturn.szAccountName��ClientID
			if(TRUE == m_mapAccountVsClientID.IsExist(strAccountName))
			{
				//�ͻ�id
				result.dwClientID = m_mapAccountVsClientID.Peek(strAccountName);
				m_mapAccountVsClientID.Erase(strAccountName);
			}
			else
			{
				nRet = E_ProofResult_Account_No_Match;
				break;
			}
*/

			//������
			BOOL bGuard = FALSE;
			if(1 == stXunleiReturn.szGuard[0] )
			{
				bGuard = TRUE;
			}
			CHAR szMibao[MIBAO_LEN] = ""; //û���ܱ�
			//�������ݿ�
			BOOL bRet = m_ProofDB.InsertAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			if( !bRet )
			{
				bRet = m_ProofDB.UpdateAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			}

//		} while (0);
	}
	if( E_ProofResult_Account_No_Match != nRet )
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
					if(result.dwFrobidMask & EPLM_MemberCenter)
					{
						nRet = E_ProofResult_Forbid_MemberCenter;						
						break;
					}
					if(result.dwFrobidMask & EPLM_GMTool)
					{
						//������������ûص�����ʱ��Ҫworld_forbid������һ��
						nRet = E_ProofResult_Forbid_GMTool;						
						break;
					}
					if(result.dwFrobidMask & EPLM_CellPhone)
					{
						nRet = E_ProofResult_Forbid_CellPhone;						
						break;
					}
					if(result.dwFrobidMask & EPLM_MiBao)
					{
						nRet = E_ProofResult_Forbid_MiBao;						
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
//-------------------------------------------------------------------------
// ��֤�ӿ�
//-------------------------------------------------------------------------
VOID ProofPolicyXunLei::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) return;

	//�����û���
	string strAccountName(szAccoutName,20);
	DeleteRightSideBlankOfString(strAccountName);

	// ����һ���µ���֤��Ϣ
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, strAccountName.c_str(), szPsd, szGUILD);

	m_listProofData.PushBack(pData);

/*
	if(TRUE == m_mapAccountVsClientID.IsExist(strAccountName))
	{	
		//��ʾ���û�֮ǰû��ͨ��Ѹ����֤��Ѹ�ײ�����AccoutName�޷�ɾ�� \
		  �����ڴ�ɾ�������½�����Ӧ��ϵ��
		m_mapAccountVsClientID.Erase(strAccountName);
	}
	m_mapAccountVsClientID.Add(strAccountName,dwClientID);
*/
}

//-------------------------------------------------------------------------
// ��֤����������״̬
//-------------------------------------------------------------------------
INT ProofPolicyXunLei::GetProofServerStatus()
{
	if (P_VALID(m_pTransport) && m_pTransport->IsConnected())
	{
		return EWS_Well;
	}

	return EWS_ProofError;
}