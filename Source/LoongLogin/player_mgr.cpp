//-----------------------------------------------------------------------------
//!\file player_mgr.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ��ҹ�����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/login.h"

#include "player_mgr.h"
#include "player.h"
#include "loongworld_mgr.h"
#include "loongworld.h"
#include "db.h"
#include "proof_policy_mgr.h"
#include "proof_policy.h"
#include "login_server.h"

//-------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------
PlayerMgr::PlayerMgr() : m_Trunk(this), m_nPlayerNum(0), m_nPlayerLoging(0), m_nProofResultNum(0),
						m_dwClientIDGen(0), m_dwCurrVer(0), m_nPort(0), m_ePolicy(EPP_Null),m_pPolicy(NULL),m_bUseGMAccess(TRUE)
{
}

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
PlayerMgr::~PlayerMgr()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------
BOOL PlayerMgr::Init()
{
	// vEngine
	m_pUtil = "Util";

	// ��ȡ�����ļ�
	TObjRef<VarContainer> pVar = "LoginServerConfig";

	//��ʼ��
	if(!InitConfig())
	{
		return FALSE;
	}

	sBeton.LoadCacheAccountData(m_mapAccountData, m_mapAccountNameCrc2AccountID);

	// ��ȡ��֤��ʽ
	m_ePolicy = (enum EProofPolicy)pVar->GetInt(_T("policy"), _T("proof"));
	if( m_ePolicy < EPP_Test || m_ePolicy >= EPP_End )
	{
		return FALSE;
	}

	// ������֤��ʽ
	m_pPolicy = sProofPolicyMgr.CreateProofPolicy(m_ePolicy);
	if( !P_VALID(m_pPolicy) )
	{
		return FALSE;
	}
	if( !m_pPolicy->Init((PROOFCALLBACK)m_Trunk.sfp2(&PlayerMgr::ProofCallBack)) )
	{
		return FALSE;
	}

	// ��ȡ�汾��
	LPCTSTR szVerNum = pVar->GetString(_T("version version"));
	m_dwCurrVer = m_pUtil->Crc32(szVerNum);

	// ��ȡ�˿ں�
	m_nPort = pVar->GetInt(_T("port player_session"));

	// ��ʼ������
	m_pNetSession = "XServer";

	tagXServerConfig InitParam;
	InitParam.fnLogIn			=	(XLOGINCALLBACK)m_Trunk.sfp2(&PlayerMgr::LoginCallBack);
	InitParam.fnLogOut			=	(XLOGOUTCALLBACK)m_Trunk.sfp1(&PlayerMgr::LogoutCallBack);
	InitParam.bReusePort		=	true;
	InitParam.nPort				=	m_nPort;
	InitParam.nMaxServerLoad	=	20000;
	InitParam.nAcceptExNum		=	128;

	m_pNetSession->Init(InitParam);
	m_nPort = m_pNetSession->GetConfig()->nPort;

	// ע��������Ϣ
	Player::RegisterPlayerMsg();

	return TRUE;
}
//-------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------
BOOL PlayerMgr::InitConfig()
{
	TObjRef<VarContainer> pVar = "GMAccess";
	if( !P_VALID(pVar) ) return FALSE;

	std::set <tstring> setAccountName;
	
	m_bUseGMAccess = pVar->GetInt(_T("on switch"));//�Ƿ�ʹ��gm��ַ����

	//��ȡaccount��
	INT iAccountNum = pVar->GetInt(_T("num account"));

	TCHAR szTemp[X_SHORT_NAME] = {_T('\0')};
	for(int iLoop = 1; iLoop<=iAccountNum; ++iLoop)
	{
		_stprintf(szTemp,_T("name%d"),iLoop);
		setAccountName.insert(pVar->GetString(szTemp,_T("account")));
	}
	CHAR cIP[X_IP_LEN]= {_T('\0')};
	CHAR cName[X_SHORT_NAME]= {_T('\0')};
	std::set <tstring>::iterator itLoop = setAccountName.begin();
	for(;itLoop != setAccountName.end();++itLoop)
	{
		INT iIPNum = pVar->GetInt(_T("num"), (*itLoop).c_str());
		for(int iLoop = 1; iLoop<=iIPNum; ++iLoop)
		{
			_stprintf(szTemp,_T("ip%d"),iLoop);

			//ת��ansi
			tstring strIP = pVar->GetString(szTemp,(*itLoop).c_str());
			WideCharToMultiByte(CP_OEMCP,NULL,strIP.c_str(),-1,cIP,X_IP_LEN,NULL,FALSE);

			//ת��DWORD
			DWORD dwIP = inet_addr(cIP);
			m_mapGMvsIP.insert( make_pair((*itLoop).c_str(),dwIP) );

		}		
	}
	return TRUE;
}
//-------------------------------------------------------------------------------
// ���ٺ���
//-------------------------------------------------------------------------------
VOID PlayerMgr::Destroy()
{
	// ȡ��ע��������Ϣ
	Player::UnRegisterPlayerMsg();

	// ע������
	m_pNetSession->Destroy();

    // ������ڵ�¼�����
    Player* pPlayer = NULL;
    m_mapLoginingPlayer.ResetIterator();
    while( m_mapLoginingPlayer.PeekNext(pPlayer) )
    {
        SAFE_DEL(pPlayer);
    }

	// ��֤��������
	m_pPolicy->Destroy();

	// ɾ����֤����
	sProofPolicyMgr.DestroyProofPolicy(m_ePolicy, m_pPolicy);

	// ����˺�������
	CleanCachedAccountDatas();
}

//-------------------------------------------------------------------------------
// ���º���
//-------------------------------------------------------------------------------
VOID PlayerMgr::Update()
{
	// ���������Ϣ
    UpdateSession();

	// ������֤���
	UpdateProofResult();

	// �õ�ͳ����Ϣ
	InterlockedExchange((LPLONG)&m_nPlayerNum,		m_mapAllPlayer.Size());
	InterlockedExchange((LPLONG)&m_nPlayerLoging,	m_mapLoginingPlayer.Size());
	InterlockedExchange((LPLONG)&m_nProofResultNum,	m_listProofResult.Size());
}

//-------------------------------------------------------------------------------
// ���������Ϣ
//-------------------------------------------------------------------------------
VOID PlayerMgr::UpdateSession()
{
	Player* pPlayer = NULL;
	m_mapLoginingPlayer.ResetIterator();
	while( m_mapLoginingPlayer.PeekNext(pPlayer) )
	{
		INT nRet = pPlayer->HandleMessage();

		if( CON_LOST == nRet )	// ����
		{
			m_mapLoginingPlayer.Erase(pPlayer->GetClientID());
			PlayerLogout(pPlayer);
		}
	}
}

//-------------------------------------------------------------------------------
// ������֤����
//-------------------------------------------------------------------------------
VOID PlayerMgr::UpdateProofResult()
{
	// ÿ��Tick��ദ��20����
	INT nNum = 20;

	// ȡ����֤���
	tagProofResultFull* pResult = m_listProofResult.PopFront();

	while( P_VALID(pResult) )
	{
		// �ҵ����
		Player* pPlayer = m_mapLoginingPlayer.Peek(pResult->dwClientID);
		if( !P_VALID(pPlayer) )
		{
			SAFE_DEL(pResult);
			pResult = m_listProofResult.PopFront();
			continue;
		}

		if( E_Success != pResult->nRet )
		{
			// ��������ʺ��Ѿ���½
			if( E_ProofResult_Account_In_Use == pResult->nRet )
			{
				// ������ݿ�������״̬������world����������
				if( EPLS_Online == pResult->eLoginStatus )
				{
					LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pResult->dwWorldNameCrc);
					if( P_VALID(pWorld) )
					{
						tagNLW_KickPlayer send;
						send.dwAccountID = pResult->dwAccountID;
						pWorld->SendKickPlayerMsg(send);
					}
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// ע������ط�
					}
				}
				// �����ʱ���ʺŴ��ڵ�½��
				else if( EPLS_Loging == pResult->eLoginStatus )
				{
					// �ڱ��ز����Ƿ���˺���login���棬����У�������
					if( IsAccountExist(pResult->dwAccountID) )
					{

					}
					// ���û�У����������Ϊ��Щ������²�ͬ������ô��ֱ��������ݿ⣬�����óɹ�
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// ע������ط�
					}
				}
			}
			//�����GM���߷�ͣ ��world_forbid��������һ��
			else if( pResult->nRet == E_ProofResult_Forbid_GMTool )
			{
				BOOL bForbid = sBeton.IfWorldForbid(pResult->dwAccountID,pResult->dwWorldNameCrc);

				// ���ʾ���˺ſ����Ǳ����Ϸ���类��ͣ��
				if( bForbid == FALSE )
				{
					pResult->nRet = E_Success;	// ע������ط�
				}
			}
			// �������
			else
			{

			}
		}
		// ��֤�ɹ�
		else
		{
			// �����Ȩ��Ϊgm��m_mapGMvsIP������
			if( m_bUseGMAccess && (pResult->byPrivilege != 0) )
			{
				BOOL bFind = FALSE;
				typedef std::multimap<tstring, DWORD>::iterator itFinder;

				itFinder beg = m_mapGMvsIP.lower_bound(pPlayer->GetAccoutNameT()),
					end = m_mapGMvsIP.upper_bound(pPlayer->GetAccoutNameT());
				while( beg != end )
				{
					if( beg->second == pPlayer->GetPlayerData().dwIP )
					{
						bFind = TRUE;
						break;
					}
					++beg;
				}
				if( bFind == FALSE )
				{
					// Ŀǰ��Ӧ�����
					pResult->nRet = E_ProofResult_Account_No_Match;	// ע������ط�

					TObjRef<StreamTransport> pTrans;
					pTrans = "StreamTransport";
					char szIP[X_IP_LEN];
					strcpy(szIP,pTrans->IP2String(pPlayer->GetPlayerData().dwIP));

					TCHAR tszIP[X_IP_LEN];
#ifdef UNICODE
					MultiByteToWideChar(CP_ACP,NULL,szIP,-1,tszIP,X_SHORT_NAME);
#else
					strcpy(tszIP,szIP);
#endif
					ILOG->Write(_T("GM:%s�Ƿ���¼������!!	IP:%s\r\n"),pPlayer->GetAccoutNameT(),tszIP);
				}	
			}
		}

		// �����Ӧ�ͻ��˵�ip������
		if( TRUE == sBeton.IfIPForbid(pPlayer->GetPlayerData().dwIP) )
		{
			//Ŀǰ��Ӧ�����
			pResult->nRet = E_ProofResult_Account_No_Match;	// ע������ط�
		}

		// �����֤�ɹ�����ʱ����֤һ����Ϸ����
		if( E_Success == pResult->nRet )
		{
			// �ҵ������Ҫ����������Ϸ����
			LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pPlayer->GetLoginWorldNameCrc());
			if( !P_VALID(pWorld) || EWS_Well != pWorld->GetStatus() )
			{
				pResult->nRet = E_SelectWorld_Server_Maintenance;
			}
		}


		// �����֤���
		if( E_Success == pResult->nRet )
		{
			// ���������֤���
			pPlayer->ProofReturn(pResult);

			//��������ϴε�¼ip��ʱ��
			pPlayer->SetPreLoginIP(pResult->dwPreLoginIP);
			pPlayer->SetPreLoginTime(pResult->dwPreLoginTime);

			// �������һ���Ҫ�ܱ�
			if( pPlayer->IsNeedMibao() )
			{
				tagNLS_MiBao send;
				pPlayer->GenerateMibao(send.szMiBao);	// �����ܱ�
				pPlayer->SendMessage(&send, send.dwSize);
			}
			// ����Ҫ�ܱ�������뵽��Ϸ�����б��еȴ�
			else
			{
				RemovePlayerFromLogining(pPlayer->GetClientID());

				// �����ʺ�
				AddAccount(pPlayer->GetAccountID(), pPlayer->GetClientID());

				// ����Ҽ��뵽LoongWorld������б���
				sLoongWorldMgr.AddToWorld(pPlayer, pPlayer->GetLoginWorldNameCrc());
			}
		}
		// �������û����֤�ɹ�
		else
		{
			tagNLS_ProofResult send;
			send.dwErrorCode = pResult->nRet;

			pPlayer->SendMessage(&send, send.dwSize);
		}

		SAFE_DEL(pResult);

		// ��һ���ˣ��������0���˳�
		if( --nNum <= 0 ) break;

		pResult = m_listProofResult.PopFront();
	}
}

//-------------------------------------------------------------------------------
// ��¼�ص�����
//-------------------------------------------------------------------------------
DWORD PlayerMgr::LoginCallBack(LPVOID p1, LPVOID p2)
{
    //�鿴��һ����Ϣ�Ƿ���ȷ
    static DWORD dwMsgCrc = m_pUtil->Crc32("NLC_SafeProof");

	tagUnit* pUnit = (tagUnit*)p1;
	tagXLoginParam* pParam = (tagXLoginParam*)p2;

	tagNLC_SafeProof* pCmd = (tagNLC_SafeProof*)pUnit->pBuffer;

	// �����Ϣ��Crc�Ƿ���ͬ
	if( pCmd->dwID != dwMsgCrc )
	{
		return GT_INVALID;
	}

	//������֤key
	DWORD dwProofKey = 0;
	ProofKey(dwProofKey,pCmd);
	if(dwProofKey != pCmd->dwProofKey)
	{
		return GT_INVALID; //��֤key����
	}

	// ��סUpdate
	g_login.LockUpdate();

	// ����һ���µĿͻ���ID
	DWORD dwClientID = GenerateClientID();
    
	// �������
	Player* pPlayer = new Player(dwClientID, pParam->dwHandle, pParam->dwAddress, pCmd->dwWorldNameCrc);
	if( !P_VALID(pPlayer) )
	{
		g_login.UnlockUpdate();
		return GT_INVALID;
	}

	// ����Ҽ��뵽�б���
	AddPlayerToAll(pPlayer);
	// ����Ҽ��뵽��½����б���
	AddPlayerToLogining(pPlayer);

	// �ͷ�Update
	g_login.UnlockUpdate();

    return dwClientID;
}

//-------------------------------------------------------------------------------
// �ǳ��ص�����
//-------------------------------------------------------------------------------
DWORD PlayerMgr::LogoutCallBack(LPVOID p)
{
	// ��סUpdate
	g_login.LockUpdate();

	// �õ�ID
    DWORD dwClientID = (DWORD)p;

    // ����������в������
    Player* pPlayer = m_mapAllPlayer.Peek(dwClientID);
    if( P_VALID(pPlayer) )
    {
        // ���ô���ҶϿ����ӱ�־
		pPlayer->SetConnLost();
    }

	// �ͷ�Update
	g_login.UnlockUpdate();

    return 0;
}

//---------------------------------------------------------------------------------
// ��ҵǳ�
//---------------------------------------------------------------------------------
VOID PlayerMgr::PlayerLogout(Player* pPlayer)
{
	BOOL bLogoutFromDB = TRUE;		// �Ƿ��������ݿ��и����Ϊ����

	if( !pPlayer->IsProofEnd() || GT_INVALID == pPlayer->GetAccountID() )	// ��û����֤�ɹ������ݿ������û�в�����
	{
		bLogoutFromDB = FALSE;
	}
	else if( pPlayer->IsSelectWorldOk() )	// world�Ѿ����˻�Ӧ��˵�����Ͼͻ���뵽world��
	{
		bLogoutFromDB = FALSE;
	}

	// �����Ҫ�������ݿ�
	if( bLogoutFromDB )
	{
		sBeton.PlayerLogout(pPlayer->GetAccountID());
	}

	// ����ʺźϷ������Ƴ��ʺ�
	if( GT_INVALID != pPlayer->GetAccountID() )
	{
		RemoveAccount(pPlayer->GetAccountID());
	}

	// ���б���ɾ�������
	RemovePlayerFromAll(pPlayer->GetClientID());

	// ɾ�����
	SAFE_DEL(pPlayer);
}

//------------------------------------------------------------------------------------
// ������֤
//------------------------------------------------------------------------------------
VOID PlayerMgr::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP)
{
	if( !P_VALID(szAccoutName) || !P_VALID(szPsd) || !P_VALID(szGUID) )
		return;

	// ���͸���Ӧ����֤���Խ�����֤
	m_pPolicy->Proof(dwClientID, szAccoutName, szPsd, szGUID, dwIP);
}

//--------------------------------------------------------------------------------------
// ��֤�ص�����
//--------------------------------------------------------------------------------------
VOID PlayerMgr::ProofCallBack(INT nRet, tagProofResult* pResult)
{
	tagProofResultFull* pResultFull = new tagProofResultFull;
	memcpy(pResultFull, pResult, sizeof(tagProofResult));
	pResultFull->nRet = nRet;

	m_listProofResult.PushBack(pResultFull);
}

VOID PlayerMgr::CacheAccountName( DWORD dwAccountID, const CHAR* szAccountName)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	memcpy(pAccountData->szAccountName, szAccountName, X_SHORT_NAME);
}

VOID PlayerMgr::CacheIpAddres( DWORD dwAccountID, DWORD dwIp)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	pAccountData->dwIp = dwIp;
}

VOID PlayerMgr::CacheGuard( DWORD dwAccountID, BOOL bGuard)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	pAccountData->bGuard = bGuard;
}

VOID PlayerMgr::EraseCachedAccountData( DWORD dwAccountID )
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);
	if (P_VALID(pAccountData))
	{
		SAFE_DEL(pAccountData);
		m_mapAccountData.Erase(dwAccountID);
	}
}

VOID PlayerMgr::CleanCachedAccountDatas()
{
	std::list<DWORD> listIDs;
	m_mapAccountData.ExportAllKey(listIDs);
	std::list<DWORD>::iterator itr = listIDs.begin();
	while(itr != listIDs.end())
	{
		EraseCachedAccountData(*itr);
		++itr;
	}
}

VOID PlayerMgr::MapAccountName2AccountID( LPCSTR szAccountName, DWORD dwAccountID )
{
	DWORD dwNameCrc = m_pUtil->Crc32(szAccountName);
	if (!GT_VALID(m_mapAccountNameCrc2AccountID.Peek(dwNameCrc)))
	{
		m_mapAccountNameCrc2AccountID.Add(dwNameCrc, dwAccountID);
	}
}

DWORD PlayerMgr::GetAccountIDByAccountName( LPCSTR szAccountName )
{
	DWORD dwNameCrc = m_pUtil->Crc32(szAccountName);
	return m_mapAccountNameCrc2AccountID.Peek(dwNameCrc);
}






