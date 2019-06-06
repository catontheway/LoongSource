//-----------------------------------------------------------------------------
//!\file proof_policy_none.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief �ڲ���������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "proof_policy_none.h"
#include "player_mgr.h"

//-----------------------------------------------------------------------------
// DB��ʼ��
//-----------------------------------------------------------------------------
BOOL ProofPocilyNoneDB::Init()
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

//-----------------------------------------------------------------------------
// DB��ѯ���
//-----------------------------------------------------------------------------
BOOL ProofPocilyNoneDB::QueryAccount(LPCSTR szAccountName,LPCSTR szPsd, tagProofResult* pProofResult)
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

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,mibao,ip,time");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");
	pStream->FillString(" AND psd='").FillString(szPsd, pCon).FillString("'"); //modify by songg

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

	// �ܱ�
	CHAR szMibao[MIBAO_LEN] = {'\0'};
	(*pResult)[6].GetBlob(szMibao, MIBAO_LEN);
	if( '\0' == szMibao[0] )
	{
		pProofResult->bNeedMibao		=	false;
	}
	else
	{
		pProofResult->bNeedMibao		=	true;
	}

	//�ϴε�¼��ʱ���ip
	memcpy(szIP,(*pResult)[7].GetString(),(*pResult)[7].GetLen());
	memcpy(szDateTime,(*pResult)[8].GetString(),(*pResult)[8].GetLen());

	pProofResult->dwPreLoginIP = pTrans->StringIP2IP(szIP);
	DataTime2DwordTime(pProofResult->dwPreLoginTime,szDateTime,(*pResult)[8].GetLen());

	// �黹�����
	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL ProofPolicyNone::Init(PROOFCALLBACK fn)
{
	m_pThread = "Thread";

	SetProofCallback(fn);		// ������֤�ص�����

	// ��ʼ�����ݿ�
	if( FALSE == m_ProofDB.Init() )
	{
		return FALSE;
	}

	// ��ʼ���߳�
	m_bTerminateUpdate = FALSE;

	if( FALSE == m_pThread->CreateThread( _T("thread_update_proof_none"), 
		(THREADPROC)m_Trunk.sfp1(&ProofPolicyNone::ThreadUpdate), NULL) )
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID ProofPolicyNone::Destroy()
{
	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("thread_update_proof_none"), INFINITE);

	// ɾ������list�е�����
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}
}

//------------------------------------------------------------------------------
// �����߳�
//------------------------------------------------------------------------------
VOID ProofPolicyNone::ThreadUpdate()
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

			// ���б���ȡ����֤����
			tagProofResult result;

			tagPlayerProofData* pData = m_listProofData.PopFront();

			while( P_VALID(pData) )
			{
				result.dwClientID = pData->dwClientID;

				BOOL bRet = m_ProofDB.QueryAccount(pData->strAccoutName.c_str(),pData->strPsd.c_str(), &result);

				// ���÷�����
				INT nRet = E_Success;
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

				// ���ûص�����
				(*m_fnCallback)(nRet, &result);

				pData = m_listProofData.PopFront();		// ȡ����һ��
			}

			WaitForSingleObject(m_listProofData.GetEvent(), 500);
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

//---------------------------------------------------------------------------------
// ��֤�ӿ�
//---------------------------------------------------------------------------------
VOID ProofPolicyNone::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) return;

	// ����һ���µ���֤��Ϣ
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName,szPsd);

	m_listProofData.PushBack(pData);
}

//-------------------------------------------------------------------------
// ��֤����������״̬
//-------------------------------------------------------------------------
INT ProofPolicyNone::GetProofServerStatus()
{
	return EWS_Well;
}