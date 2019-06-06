//-----------------------------------------------------------------------------
//!\file loongworld_mgr.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief �����������ӽ���LoongWorld,ӵ��StreamServer,������Ϣ������LoongWorld���͵İ�
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_rt_errorcode.h"
#include "../ServerDefine/login_issue.h"
#include "../ServerDefine/login_define.h"
#include "loongworld_mgr.h"
#include "loongworld.h"
#include "login_server.h"
#include "db.h"
#include "fatigue_mgr.h"
#include "proof_policy.h"

//------------------------------------------------------------------------------
// ���캯��
//------------------------------------------------------------------------------
LoongWorldMgr::LoongWorldMgr():m_Trunk(this)
{

}

//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
LoongWorldMgr::~LoongWorldMgr()
{
    Destroy();
}

//-------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------
BOOL LoongWorldMgr::Init()
{
    m_pUtil = "Util";

    // �������ļ�����LoongWorld������
	TObjRef<VarContainer> pVar = "LoginServerConfig";

	INT nWorldNum = pVar->GetDword(_T("num zone_server"));

    for(INT n = 0; n < nWorldNum; ++n)
    {
		// ����world
        LoongWorld* pWorld = new LoongWorld;

		if( FALSE == pWorld->Init(n) )
		{
			SAFE_DEL(pWorld);
			return FALSE;
		}

        m_mapLoongWorld.Add(pWorld->GetID(), pWorld);
		
		//��ʼ�����ݿ�����  ȫ��״̬���-1 songg
		BOOL bRet = sBeton.InsertWorldState(pWorld->GetWorldID());
		if(!bRet)
		{
			sBeton.UpdateWorldState(pWorld->GetWorldID(),0,-1);
		}

		//��¼log
		sBeton.InsertWorldStateLog(pWorld->GetWorldID(),0,-1);
    }

    if( m_mapLoongWorld.Empty() )
        return FALSE;

    // ��ȡ��ɫ����
    m_dwLoongWorldGoldenCode = pVar->GetDword(_T("zone_server golden_code"));
	m_dwLoginServerGoldenCode = pVar->GetDword(_T("login_server golden_code"));

	// �����������������
	CreateObj("NetCmdMgrWorld", "NetCmdMgr");
	m_pMsgCmdMgr = "NetCmdMgrWorld";
	if( !P_VALID(m_pMsgCmdMgr) ) return FALSE;

	// ���ɷ���������ӿ�
	CreateObj("StreamServerWorld", "StreamServer");
	m_pSession = "StreamServerWorld";
	if( !P_VALID(m_pSession) ) return FALSE;

	// ע����������
	RegisterLoongWorldMsg();

	// ��ʼ������
	INT nPort = pVar->GetInt(_T("port"), _T("zone_session"));
	m_pSession->Init((TCPLOGINCALLBACK)m_Trunk.sfp2(&LoongWorldMgr::LoginCallBack),
		(TCPLOGOUTCALLBACK)m_Trunk.sfp1(&LoongWorldMgr::LogoutCallBack), nPort);

	return TRUE;
}

//-------------------------------------------------------------------------------
// ���ٺ���
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::Destroy()
{
    // ���LoongWorld����
    LoongWorld* pLoongWorld = NULL;


	//�������״̬��
	sBeton.ClearWorldStateTable();

    m_mapLoongWorld.ResetIterator();
    while( m_mapLoongWorld.PeekNext(pLoongWorld) )
    {
        SAFE_DEL(pLoongWorld);
    }
	m_mapLoongWorld.Clear();

	// ע������
	m_pSession->Destroy();
	KillObj("StreamServerWorld");

	// ע�����е���������
	UnRegisterLoongWorldMsg();
	KillObj("NetCmdMgrWorld");
}

//-------------------------------------------------------------------------------
// ���º���
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::Update()
{
	// ����ÿһ��world
	LoongWorld* pLoongWorld = NULL;

	m_mapLoongWorld.ResetIterator();
	while( m_mapLoongWorld.PeekNext(pLoongWorld) )
	{
		pLoongWorld->Update();
		//��������״̬
		UpdateWorldState(pLoongWorld);
		
	}
}
//-------------------------------------------------------------------------------
// ��������״̬
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::UpdateWorldState(LoongWorld* pLoongWorld)
{
	//��5�����һ�����ݿ� songg
	DWORD dwCurTime = timeGetTime();
	if( (dwCurTime - pLoongWorld->GetDBUpdateTime()) > sBeton.GetWorldStateUpdateTime() )
	{
		pLoongWorld->SetDBUpdateTime(dwCurTime);

		if(pLoongWorld->GetStatus() == EWS_Well && pLoongWorld->IsValid()) //��½��û�ǳ�
		{
			SHORT  sState = ((DOUBLE)pLoongWorld->GetCurrOnlineDisplay()/pLoongWorld->GetMaxOnlineNum())*100; //�ٷֱ�

			//ˢ������״̬��DB
			sBeton.UpdateWorldState(pLoongWorld->GetWorldID(),pLoongWorld->GetCurrOnlineDisplay(),sState);
		}
		else
		{
			//ˢ������״̬��DB
			sBeton.UpdateWorldState(pLoongWorld->GetWorldID(),0,-1);
		}

	}
	if( (dwCurTime - pLoongWorld->GetDBInsertTime()) > sBeton.GetWorldStateInsertTime() )
	{
		pLoongWorld->SetDBInsertTime(dwCurTime);

		if(pLoongWorld->GetStatus() == EWS_Well && pLoongWorld->IsValid()) //��½��û�ǳ�
		{
			SHORT  sState = ((DOUBLE)pLoongWorld->GetCurrOnlineDisplay()/pLoongWorld->GetMaxOnlineNum())*100; //�ٷֱ�

			//����״̬��log
			sBeton.InsertWorldStateLog(pLoongWorld->GetWorldID(),pLoongWorld->GetCurrOnlineDisplay(),sState);
		}
		else
		{
			//����״̬��log
			sBeton.InsertWorldStateLog(pLoongWorld->GetWorldID(),0,-1);
		}

	}
}
//-------------------------------------------------------------------------------
// ע��������Ϣ
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::RegisterLoongWorldMsg()
{
	m_pMsgCmdMgr->Register("NWL_Certification",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleCertification),		_T("NWL_Certification") );
	m_pMsgCmdMgr->Register("NWL_WorldStatus",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleZoneServerStatus),	_T("NWL_WorldStatus") );
	m_pMsgCmdMgr->Register("NWL_PlayerWillLogin",	(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandlePlayerWillLogin),	_T("NWL_PlayerWillLogin") );
	m_pMsgCmdMgr->Register("NWL_PlayerLogin",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandlePlayerLogin),		_T("NWL_PlayerLogin") );
	m_pMsgCmdMgr->Register("NWL_PlayerLogout",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandlePlayerLogout),		_T("NWL_PlayerLogout") );
	m_pMsgCmdMgr->Register("NWL_KickLog",			(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleKickLog),			_T("NWL_KickLog") );
	m_pMsgCmdMgr->Register("NWL_WorldColsed",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleWorldClosed),		_T("NWL_WorldColsed") );
	m_pMsgCmdMgr->Register("NWL_ForbidAccount",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleForbidAccount),		_T("NWL_ForbidAccount") );
}

//------------------------------------------------------------------------
// ��ע��������Ϣ
//------------------------------------------------------------------------
VOID LoongWorldMgr::UnRegisterLoongWorldMsg()
{
	m_pMsgCmdMgr->Destroy();
}

//------------------------------------------------------------------------
// ��½�ص�
//------------------------------------------------------------------------
DWORD LoongWorldMgr::LoginCallBack(LPBYTE pByte, DWORD dwSize)
{
	tagNWL_Certification* pRecv = (tagNWL_Certification*)pByte;

	// ��֤LoongWorld,�Ƚ������ļ���ɫ����
	if( pRecv->dwGoldenCode != m_dwLoongWorldGoldenCode )
		return GT_INVALID;

	// ��ס������
	g_login.LockUpdate();

	// �õ�world��id
	DWORD dwNameCrc = m_pUtil->Crc32(pRecv->szWorldName);

	// �ҵ�World
	LoongWorld* pWorld = GetLoongWorld(dwNameCrc);
	if( !P_VALID(pWorld) )
	{
		IMSG(_T("one invalid loongworld login, name=%s\r\n"), pRecv->szWorldName);

		// �ͷ���
		g_login.UnlockUpdate();

		return GT_INVALID;
	}

	// ���Գ�ʼ��
	if( !pWorld->WorldLogin(pRecv->dwIP, pRecv->dwPort, pRecv->dwOLAccountID, pRecv->nOLAccountNum) )
	{
		IMSG(_T("one loongworld login which is already online, name=%s\r\n"), pRecv->szWorldName);

		// �ͷ���
		g_login.UnlockUpdate();

		return GT_INVALID;
	}

	// ��ʼ���ɹ�
	IMSG(_T("Hello LoongWorld����%s\r\n"),pRecv->szWorldName);

	// �ͷ���
	g_login.UnlockUpdate();

	// ����ID
	return dwNameCrc;
}

//----------------------------------------------------------------------------
// �ǳ��ص�
//----------------------------------------------------------------------------
DWORD LoongWorldMgr::LogoutCallBack(DWORD dwParam)
{
	DWORD dwWorldID = dwParam;

	// ��ס������
	g_login.LockUpdate();

	// ����world
	LoongWorld* pWorld = GetLoongWorld(dwWorldID);
	if( !P_VALID(pWorld) )
	{
		// �ͷ���
		g_login.UnlockUpdate();

		return 0;
	}

	// ���Եǳ�
	pWorld->WorldLogout();
	IMSG(_T("Bye LoongWorld����%s\r\n"), pWorld->GetName());

	// �ͷ���
	g_login.UnlockUpdate();

	return 0;
}

//------------------------------------------------------------------------------
// ��ӵ���Ϸ����
//------------------------------------------------------------------------------
VOID LoongWorldMgr::AddToWorld(Player* pPlayer, DWORD dwWorldNameCrc)
{
	if( !P_VALID(pPlayer) ) return;

	LoongWorld* pWorld = GetLoongWorld(dwWorldNameCrc);
	if( !P_VALID(pWorld) ) return;

	pWorld->AddPlayer(pPlayer);

	// д�����ݿ�
	sBeton.PlayerLogin(pPlayer->GetAccountID(), dwWorldNameCrc);
}

//------------------------------------------------------------------------------
// ������֤��Ϣ
//------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandleCertification(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	tagNWL_Certification* pRecv = (tagNWL_Certification*)pMsg;

	// ���ͷ�����Ϣ
	tagNLW_Certification send;
	send.dwGoldenCode = m_dwLoginServerGoldenCode;

	SendMsg(pWorld->GetID(), &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// �����������Ϣ��Ϣ
//-------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandleZoneServerStatus(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	tagNWL_WorldStatus* pRecv = (tagNWL_WorldStatus*)pMsg;

	pWorld->UpdateStatus(pRecv->eStatus, pRecv->nCurPlayerNum, pRecv->nPlayerNumLimit, pRecv->nCurPlayerDisplay);

	return 0;
}

//--------------------------------------------------------------------------------
// ��ҵ�½
//--------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandlePlayerLogin(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	tagNWL_PlayerLogin* pRecv = (tagNWL_PlayerLogin*)pMsg;

	// �����½���ɹ�
	if( E_Success != pRecv->dwErrorCode )
	{
		ILOG->Write(_T("Player Login Failure! AccountID:%u ErrorCode=%d\r\n"), pRecv->dwAccountID, pRecv->dwErrorCode);

		// �������ݿ�
		sBeton.PlayerLogout(pRecv->dwAccountID);
		if (g_login.IsEnableFatigueGuard())
			sFatigueMgr.LogoutNotify(pRecv->dwAccountID);
	}
	// �����¼�ɹ�
	else
	{
		// �������ݿ�
		sBeton.PlayerEnterWorld(pRecv->dwAccountID, pWorld->GetID());

		// login log
		const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(pRecv->dwAccountID);
		if (!P_VALID(pAccountData))
		{
			ASSERT(0);
			return GT_INVALID;
		}
		if (g_login.IsEnableFatigueGuard())
			sFatigueMgr.LoginNotify(pRecv->dwAccountID, pWorld->GetID(), pAccountData->bGuard);
		//��¼���ε�¼��ip��time ���´�ʹ��
		sBeton.UpdateAccountLoginInfo(pRecv->dwAccountID, pRecv->dwIP);

		//дlogin��log
		sBeton.LogPlayerAction(pRecv->dwAccountID, pAccountData->szAccountName, pRecv->dwIP, "login");
		sPlayerMgr.CacheIpAddres(pRecv->dwAccountID, pRecv->dwIP);
	}

	return 0;
}

//-----------------------------------------------------------------------------------
// ��ҵǳ�
//-----------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandlePlayerLogout(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	tagNWL_PlayerLogout* pRecv = (tagNWL_PlayerLogout*)pMsg;

	// �������ݿ�
	sBeton.PlayerOutWorld(pRecv->dwAccountID, pWorld->GetID());

	// logout log
	const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(pRecv->dwAccountID);
	if (!P_VALID(pAccountData))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	//orange����Ҫ�ǳ���ʱ�򷢸���Ϣ����
	if(sPlayerMgr.GetProofPolicy() == EPP_Orange)
	{
		ProofPolicy* pPolicy =	sPlayerMgr.GetProofPolicyPtr();
		if(P_VALID(pPolicy))
		{
			pPolicy->SigPlayerLogoutEvent(pRecv->dwAccountID);
		}
	}

	sBeton.LogPlayerAction(pRecv->dwAccountID, pAccountData->szAccountName, pAccountData->dwIp, "logout");
	if (g_login.IsEnableFatigueGuard())
		sFatigueMgr.LogoutNotify(pRecv->dwAccountID);

	return 0;
}

//------------------------------------------------------------------------------------
// LoongWorld֪ͨ������Ϸ�������󷵻ؽ��
//-------------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandlePlayerWillLogin(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	tagNWL_PlayerWillLogin* pRecv = (tagNWL_PlayerWillLogin*)pMsg;

	pWorld->PlayerWillLoginRet(pRecv->dwAccountID, pRecv->dwErrorCode);

	return 0;
}
//-----------------------------------------------------------------------
// LoongWorld�����ر�
//-----------------------------------------------------------------------
DWORD LoongWorldMgr::HandleWorldClosed(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	//�ı��Ӧ��ҵĵ�¼״̬
	sBeton.FixOneWorldPlayerLoginStatus(pWorld->GetID(), EPLS_Null, EPLS_OffLine);

	tagNLW_WorldColsed send;
	SendMsg(pWorld->GetID(), &send, send.dwSize);

	return 0;
}
//-----------------------------------------------------------------------
// LoongWorld�߳����
//-----------------------------------------------------------------------
DWORD LoongWorldMgr::HandleKickLog(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	tagNWL_KickLog* pRecv = (tagNWL_KickLog*)pMsg;

	const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(pRecv->dwAccountID);
	if (!P_VALID(pAccountData))
	{
		ASSERT(0);
		return GT_INVALID;
	}

// 	ILOG->Write(_T("Kick Name: %s  AccountID: %d  Time: %d  Seal: %d ErrCode: %d\r\n "), 
// 				pAccountData->szAccountName, pRecv->dwAccountID, pRecv->dwKickTime, pRecv->bySeal, pRecv->u16ErrCode);

	sBeton.InsertKickLog(pAccountData->szAccountName, pRecv->dwAccountID, pRecv->dwKickTime, pRecv->u16ErrCode, pRecv->bySeal);
	if (pRecv->bySeal && pWorld->IsAutoSeal())
	{
		//songg
		DWORD dwErrorCode = sBeton.ForbidAccount(pRecv->dwAccountID, EPLM_WaiGua);
		if(dwErrorCode == E_RT_Success)
		{
			if( P_VALID(pWorld) )
			{
				tagNLW_KickPlayer send;
				send.dwAccountID = pRecv->dwAccountID;
				pWorld->SendKickPlayerMsg(send);
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------
// LoongWorld�߳����
//-----------------------------------------------------------------------
DWORD LoongWorldMgr::HandleForbidAccount( tagNetCmd* pMsg, LoongWorld* pWorld )
{
	tagNWL_ForbidAccount* pRecv = (tagNWL_ForbidAccount*)pMsg;

	//songg
	DWORD dwErrorCode = sBeton.ForbidAccount(pRecv->dwAccountID, pRecv->dwForbidCode);
	if(dwErrorCode == E_RT_Success)
	{
		if( P_VALID(pWorld) )
		{
			tagNLW_KickPlayer send;
			send.dwAccountID = pRecv->dwAccountID;
			pWorld->SendKickPlayerMsg(send);
		}
	}

	return E_Success;
}


