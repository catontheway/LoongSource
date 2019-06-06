//-----------------------------------------------------------------------------
//!\file proof_policy_own.cpp
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
#include "stdafx.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/msg_rt_errorcode.h"
#include "proof_policy_own_define.h"
#include "proof_policy.h"
#include "proof_policy_own.h"
#include "login_server.h"
#include "db.h"
//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::Init()
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
BOOL ProofPolicyOwnDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
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
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0) return FALSE;

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
BOOL ProofPolicyOwnDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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
BOOL ProofPolicyOwnDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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

//-------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::UpdatePsd(LPCSTR szAccountName, LPCSTR szNewPsd)
{
	if( !P_VALID(szAccountName) || !P_VALID(szNewPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("psd='").FillString(szNewPsd, pCon).FillString("'");
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

//-------------------------------------------------------------------------
// �����ܱ�
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::UpdateMibao(LPCSTR szAccountName, LPCSTR szMibao)
{
	if( !P_VALID(szAccountName) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("mibao='").FillBlob(szMibao, MIBAO_LEN, pCon).FillString("'");
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

//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyOwn::Init(PROOFCALLBACK fn)
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

	// �����������������
	CreateObj("NetCmdMgrProofPolicyOwn", "NetCmdMgr");
	m_pCmdMgr = "NetCmdMgrProofPolicyOwn";
	if( !P_VALID(m_pCmdMgr) ) return FALSE;

	// ��ʼ������
	CreateObj("StreamTransportProofPolicyOwn", "StreamTransport");
	m_pTransport = "StreamTransportProofPolicyOwn";
	if( !P_VALID(m_pTransport) ) return FALSE;
	m_pTransport->Init();

	// ע����������
	RegisterProofMsg();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ProofPolicyOwnConnectServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadConnectServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyOwnConnectServer")))
	{
		continue;
	}

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ProofPolicyOwnThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyOwnThreadUpdate")))
	{
		continue;
	}

	return TRUE;
}

//-------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------
BOOL ProofPolicyOwn::InitConfig()
{
	// ���������ļ�
	CreateObj("ProofPolicyOwn", "VarContainer");
	TObjRef<VarContainer> pVar = "ProofPolicyOwn";

	if( !P_VALID(pVar) ) return FALSE;

	pVar->Load("VirtualFileSys", _T("server_config/login/center_proof.ini"));

	// ��ȡIP�Ͷ˿�
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip server")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	m_nPort = pVar->GetInt(_T("port server"));

	// ɾ�������ļ�����
	KillObj("ProofPolicyOwn");
	
	return TRUE;
}

//-------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::Destroy()
{

	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyOwnThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyOwnConnectServer"), INFINITE);

	// ��������
	m_pTransport->Destroy();
	KillObj("StreamTransportProofPolicyOwn");

	// ����������Ϣ������
	UnregisterProofMsg();
	KillObj("NetCmdMgrProofPolicyOwn");

	// ɾ������list�е�����
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}
}

//-------------------------------------------------------------------------
// ע��������Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::RegisterProofMsg()
{
	m_pCmdMgr->Register("NS_LOGIN",				(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleLogin),				_T("login return") );
	m_pCmdMgr->Register("NS_USERLOGIN",			(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserLogin),			_T("accout login return") );
	m_pCmdMgr->Register("NS_USERUPDATEPWD",		(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserUpdatePwd),		_T("accout update password") );
	m_pCmdMgr->Register("NS_USERBINDMIBAO",		(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserBindMibao),		_T("accout bind mibao") );
	m_pCmdMgr->Register("NS_USERUNBINDMIBAO",	(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserUnbindMibao),	_T("accout unbind mibao") );

	m_pCmdMgr->Register("NS_BLOCKACCOUNT",		(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleBlockAccount),	_T("block accout ") );
	m_pCmdMgr->Register("NS_UNBLOCKACCOUNT",	(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUnBlockAccount),	_T("unblock accout ") );
	m_pCmdMgr->Register("NS_USERUPDATECHENMI",	(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleSetChenMi),			_T("set chenmi") );

}

//-------------------------------------------------------------------------
// ���������Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::UnregisterProofMsg()
{
	m_pCmdMgr->Destroy();
}

//-------------------------------------------------------------------------
// �����߳�
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::ThreadUpdate()
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

			// �ȴ�
			WaitForSingleObject(m_listProofData.GetEvent(), 50);
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
// ������֤�б�
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::UpdateProofList()
{
	// ���б���ȡ����֤����
	tagProofResult result;
	string strPsd;

	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		result.dwClientID = pData->dwClientID;
		BOOL bRet = m_ProofDB.QueryAccount(pData->strAccoutName.c_str(), strPsd, &result);

		// ���÷�����
		BOOL bNeedCenterProof = FALSE;

		INT nRet = E_Success;
		if( bRet )
		{
			// �Ƚ������ǲ���һ��
			if( strPsd != pData->strPsd )
			{
				nRet = E_ProofResult_Account_No_Match;
				bNeedCenterProof = TRUE;
			}
			else if( EPLS_OffLine != result.eLoginStatus )
			{
				nRet = E_ProofResult_Account_In_Use;
			}
			else if( result.dwFrobidMask != 0  )
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
			bNeedCenterProof = TRUE;
		}

		// �����Ҫ���ļ������û������
		if( bNeedCenterProof && !m_bConnected )
		{
			nRet = E_SelectWorld_Server_Maintenance;
			bNeedCenterProof = FALSE;
		}

		if( bNeedCenterProof )
		{
			// �������ļ���
			tagNC_USERLOGIN send;

			send.dwLoginID	=	g_login.GetSectionID();
			send.dwClientID	=	pData->dwClientID;
			strncpy(send.szGUID, pData->strGUID.c_str(), X_STRING_LEN);
			strncpy(send.szAccountName, pData->strAccoutName.c_str(), X_STRING_LEN);
			strncpy(send.szPsd, pData->strPsd.c_str(), X_STRING_LEN);

			m_pTransport->Send(&send, send.dwSize);
		}
		else
		{
			// ���ûص�����
			(*m_fnCallback)(nRet, &result);
		}
		
		SAFE_DEL(pData);
		pData = m_listProofData.PopFront();		// ȡ����һ��
	}
}

//-------------------------------------------------------------------------
// ������Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::UpdateSession()
{
	if( NULL == m_pTransport )
		return;

	// ������
	if(!m_pTransport->IsConnected() && !m_pThread->IsThreadActive(_T("ProofPolicyOwnConnectServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateConnect, TRUE);
		InterlockedExchange((LONG*)&m_bConnected, FALSE);
		m_pTransport->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ProofPolicyOwnConnectServer"), INFINITE);

		// �����������ݿ�����������߳�
		InterlockedExchange((LONG*)&m_bTerminateConnect, FALSE);
		m_pThread->CreateThread(_T("ProofPolicyOwnConnectServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadConnectServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ProofPolicyOwnConnectServer")))
		{
			m_pThread->CreateThread(_T("ProofPolicyOwnConnectServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadConnectServer), NULL);
			continue;
		}

		return;
	}

	// �����������ļ������������Ϣ
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransport->Recv(dwSize);

	while( P_VALID(pRecv) )
	{
		// ������Ϣ
		m_pCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);
		m_pTransport->FreeRecved(pRecv);

		// ��ȡ��һ����Ϣ
		pRecv = m_pTransport->Recv(dwSize);
	}
}

//-------------------------------------------------------------------------
// �����߳�
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::ThreadConnectServer()
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

		// ���ͳ�ʼ��Ϣ
		tagNC_LOGIN	send;
		send.dwLoginID = g_login.GetSectionID();
		m_pTransport->Send(&send, send.dwSize);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
}

//-------------------------------------------------------------------------
// �����½
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleLogin(tagNetCmd* pCmd, DWORD)
{
	tagNS_LOGIN* pRecv = (tagNS_LOGIN*)pCmd;

	if( 0 != pRecv->byResult )
	{
		ASSERT(0);
	}

	InterlockedExchange((LONG*)&m_bConnected, TRUE);	// ��������

	return 0;
}

//-------------------------------------------------------------------------
// ������ҵ�½
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserLogin(tagNetCmd* pCmd, DWORD)
{
	tagNS_USERLOGIN* pRecv = (tagNS_USERLOGIN*)pCmd;

	tagProofResult result;
	result.dwClientID = pRecv->dwClientID;

	// �鿴������
	INT nRet = E_Success;

	// �Ƿ�������ݿ�
	if( 2 == pRecv->byResult )	// �ʺŲ�����
	{
		nRet = E_ProofResult_Account_No_Match;
	}
	else if( 1 == pRecv->byResult )	// �������
	{
		nRet = E_ProofResult_Account_No_Match;
	
		// ֻ���Ը������ݿ⣬�������룬��Ϊ�������ǵ�һ�ε�½����ʱ���뼤�û�н�����˺ŷ��뼤���б����ܻᵼ�»���ʱû�и�����¼
		m_ProofDB.UpdateAccout(pRecv->szAccountName, pRecv->szPsd, pRecv->szMibao, pRecv->byGuard);
	}
	else if( 0 == pRecv->byResult )	// ��ȷ
	{
		// ���Բ��룬�������ʧ�����Ը���
		BOOL bRet = m_ProofDB.InsertAccout(pRecv->szAccountName, pRecv->szPsd, pRecv->szMibao, pRecv->byGuard);
		if( !bRet )
		{
			bRet = m_ProofDB.UpdateAccout(pRecv->szAccountName, pRecv->szPsd, pRecv->szMibao, pRecv->byGuard);
		}
	}
	else
	{
		nRet = E_ProofResult_Account_No_Match;
	}

	// �Ƿ�����Ҫ���²�ѯ
	if( E_ProofResult_Account_No_Match != nRet )
	{
		string strPsd;
		BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);

		if( bRet )
		{
			if( EPLS_OffLine != result.eLoginStatus )
			{
				nRet = E_ProofResult_Account_In_Use;
			}
			else if( pRecv->byState == 1 )
			{
				//��ʾ�����ͱ���ͣ��~
				sBeton.ForbidAccount(result.dwAccountID,pRecv->byBlocktype);
				//���µõ�result
				m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);
			}
			//�����ͣ���벻Ϊ�� �� ��ǰ��¼״̬��δ�ı��E_ProofResult_Account_In_Use
			if( (result.dwFrobidMask != 0)  && (nRet != E_ProofResult_Account_In_Use) )
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



	return 0;
}

//-------------------------------------------------------------------------
// ������Ҹ�������
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserUpdatePwd(tagNetCmd* pCmd, DWORD)
{
	tagNS_USERUPDATEPWD* pRecv = (tagNS_USERUPDATEPWD*)pCmd;

	// ���Ը������ݿ�
	BOOL bRet = m_ProofDB.UpdatePsd(pRecv->szAccountName, pRecv->szPsd);

	// ���ͷ�����Ϣ
	tagNC_USERUPDATEPWD send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);

	m_pTransport->Send(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------
// ������Ұ��ܱ�
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserBindMibao(tagNetCmd* pCmd, DWORD)
{
	tagNS_USERBINDMIBAO* pRecv = (tagNS_USERBINDMIBAO*)pCmd;

	// ���Ը������ݿ�
	BOOL bRet = m_ProofDB.UpdateMibao(pRecv->szAccountName, pRecv->szMibao);

	// ���ͷ�����Ϣ
	tagNC_USERBINDMIBAO send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);

	m_pTransport->Send(&send, send.dwSize);


	return 0;
}

//-------------------------------------------------------------------------
// ���ȡ�����ܱ�
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserUnbindMibao(tagNetCmd* pCmd, DWORD)
{
	tagNS_USERUNBINDMIBAO* pRecv = (tagNS_USERUNBINDMIBAO*)pCmd;

	// ���Ը������ݿ�
	BOOL bRet = m_ProofDB.UpdateMibao(pRecv->szAccountName, pRecv->szMibao);

	// ���ͷ�����Ϣ
	tagNC_USERUNBINDMIBAO send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);

	m_pTransport->Send(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------
// �����˺ŷ�ͣ
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleBlockAccount(tagNetCmd* pCmd, DWORD)
{
	tagNS_BLOCKACCOUNT* pRecv = (tagNS_BLOCKACCOUNT*)pCmd;

	// ���б���ȡ����֤����
	tagProofResult result;
	string strPsd;
	BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);

	if(bRet)
	{
		DWORD dwRet = sBeton.ForbidAccount(result.dwAccountID,pRecv->byBlockType);
		bRet = ((dwRet==E_RT_Success) ? 1 : 0);
	}

	tagNC_BLOCKACCOUNT send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);
	m_pTransport->Send(&send, send.dwSize);


	return 0;
}
//-------------------------------------------------------------------------
// �����˺Ž��
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUnBlockAccount(tagNetCmd* pCmd, DWORD)
{
	tagNS_UNBLOCKACCOUNT* pRecv = (tagNS_UNBLOCKACCOUNT *)pCmd;

	// ���б���ȡ����֤����
	tagProofResult result;
	string strPsd;
	BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);

	if(bRet)
	{
		DWORD dwRet = sBeton.RemoveAccountForbid(result.dwAccountID,pRecv->byUnBlockType);
		bRet = ((dwRet==E_RT_Success) ? 1 : 0);
	}

	tagNC_UNBLOCKACCOUNT send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);
	m_pTransport->Send(&send, send.dwSize);


	return 0;
}

//-------------------------------------------------------------------------
// ���÷�����
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleSetChenMi(tagNetCmd* pCmd, DWORD)
{
	tagNS_USERUPDATECHENMI* pRecv = (tagNS_USERUPDATECHENMI*)pCmd;

	// ���б���ȡ����֤����
	tagProofResult result;
	string strPsd;
	BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);

	if(bRet)
	{
		DWORD dwRet = sBeton.SetAccountChenMi(result.dwAccountID,pRecv->byChenMi);
		bRet = ((dwRet==E_RT_Success) ? 1 : 0);
	}

	tagNC_USERUPDATECHENMI send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);
	m_pTransport->Send(&send, send.dwSize);


	return 0;
}


//-------------------------------------------------------------------------
// ��֤�ӿ�
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) return;

	// ����һ���µ���֤��Ϣ
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName, szPsd, szGUILD);

	m_listProofData.PushBack(pData);
}

//-------------------------------------------------------------------------
// ��֤����������״̬
//-------------------------------------------------------------------------
INT ProofPolicyOwn::GetProofServerStatus()
{
	if (P_VALID(m_pTransport) && m_pTransport->IsConnected())
	{
		return EWS_Well;
	}

	return EWS_ProofError;
}