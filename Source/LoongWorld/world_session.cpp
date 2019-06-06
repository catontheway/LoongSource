//-----------------------------------------------------------------------------
//!\file world_session.cpp
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief ����������ͻ����������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "world_session.h"
#include "login_session.h"
#include "player_session.h"
#include "world.h"
#include "game_guarder.h"
#include "TimeChecker.h"
#include "..\WorldDefine\selectrole.h"
#include "../WorldDefine/TreasureChest_define.h"

const INT MAX_RECV_WAIT_UNIT_NUM = 64;		// �ͻ��˵ȴ�����İ�������������������������ߵ�
const INT MAX_SEND_CAST_UNIT_NUM = 1000;	// ���͸��ͻ��˵���û���յ���Ӧ�İ��������������������ߵ�
const INT MAX_SEND_CAST_SIZE = 2*1024*1024;	// ���͸��ͻ��˵���û���յ���Ӧ���ֽ����������������ߵ�

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
WorldSession::WorldSession(): m_Trunk(this), m_lfPlayerNumFactor(1.0)
{
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
WorldSession::~WorldSession()
{

}

//-------------------------------------------------------------------------------
// �õ�һ��session��Ҫע��ָ��õ���İ�ȫ�ԣ�
//-------------------------------------------------------------------------------
PlayerSession* WorldSession::FindSession(DWORD dwID)
{
	m_AllSessionMutex.Acquire();
	PlayerSession* pSession = m_mapAllSessions.Peek(dwID);
	m_AllSessionMutex.Release();

	return pSession;
}

//--------------------------------------------------------------------------------
// �õ�һ��ѡ�˽����session��ֻ�������߳��е��ã�
//--------------------------------------------------------------------------------
PlayerSession* WorldSession::FindGlobalSession(DWORD dwID)
{
	m_GlobalSessionMutex.Acquire();
	PlayerSession* pSession = m_mapGlobalSessions.Peek(dwID);
	m_GlobalSessionMutex.Release();

	return pSession;
}

//--------------------------------------------------------------------------------
// ���һ��sessionID�Ƿ����
//--------------------------------------------------------------------------------
BOOL WorldSession::IsSessionExist(DWORD dwID)
{
	m_AllSessionMutex.Acquire();
	BOOL bExist = m_mapAllSessions.IsExist(dwID);
	m_AllSessionMutex.Release();

	return bExist;
}

//--------------------------------------------------------------------------------
// ����һ���µ�session
//-------------------------------------------------------------------------------
VOID WorldSession::AddSession(PlayerSession* pSession)
{ 
	if( P_VALID(pSession) )
	{
		m_AllSessionMutex.Acquire();
		m_mapAllSessions.Add(pSession->GetSessionID(), pSession);
		m_AllSessionMutex.Release();
	}
}
//-------------------------------------------------------------------------------
// ɾ��һ��session
//-------------------------------------------------------------------------------
VOID WorldSession::RemoveSession(UINT32 dwID)
{
	m_AllSessionMutex.Acquire();
	m_mapAllSessions.Erase(dwID);
	m_AllSessionMutex.Release();
}

//-------------------------------------------------------------------------------
// �ߵ�һ������
//-------------------------------------------------------------------------------
VOID WorldSession::Kick(DWORD dwInternalIndex)
{
	m_pNetSession->Kick(dwInternalIndex);
}
//-------------------------------------------------------------------------------
// ��ѡ�˽���session�м���һ��session
//-------------------------------------------------------------------------------
VOID WorldSession::AddGlobalSession(PlayerSession* pSession)
{
	if( P_VALID(pSession) )
	{
		if( m_GlobalSessionMutex.TryAcquire() )
		{
			m_mapGlobalSessions.Add(pSession->GetSessionID(), pSession);
			m_GlobalSessionMutex.Release();
		}
		else
		{
			m_InsertPoolMutex.Acquire();
			m_listInsertPool.PushBack(pSession);
			m_InsertPoolMutex.Release();
		}
	}
}

//-------------------------------------------------------------------------------
// ��ѡ�˽���session�б���ɾ��һ��session
//-------------------------------------------------------------------------------
VOID WorldSession::RemoveGlobalSession(DWORD dwSessionID)
{
	m_GlobalSessionMutex.Acquire();
	m_mapGlobalSessions.Erase(dwSessionID);
	m_GlobalSessionMutex.Release();
}

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
BOOL WorldSession::Init()
{
	// vEngine����
	m_pUtil = "Util";
	m_pNetSession = "XServer";

	// ���������ļ�
	TObjRef<VarContainer> pVar = "VarContainer";

	m_nPlayerNumLimit = (INT)pVar->GetDword(_T("player_num_limit"), _T("world"));
	m_nPort = (INT)pVar->GetDword(_T("port"), _T("server"));

	m_nMaxRecvWaitUnitNum	= (INT)pVar->GetDword(_T("max_recv_wait_unit_num"), _T("house_keeping"), 64);		// �ͻ��˵ȴ�����İ�������������������������ߵ�
	m_nMaxSendCastUnitNum	= (INT)pVar->GetDword(_T("max_send_cast_unit_num"), _T("house_keeping"), 1000);		// ���͸��ͻ��˵���û���յ���Ӧ�İ��������������������ߵ�
	m_nMaxSendCastSize		= (INT)pVar->GetDword(_T("max_send_cast_size"), _T("house_keeping"), 2097152);		// ���͸��ͻ��˵���û���յ���Ӧ���ֽ����������������ߵ�

	// �趨����ײ�
	tagXServerConfig InitParam;
	InitParam.fnLogIn		=	(XLOGINCALLBACK)m_Trunk.sfp2(&WorldSession::LoginCallBack);
	InitParam.fnLogOut		=	(XLOGOUTCALLBACK)m_Trunk.sfp1(&WorldSession::LogoutCallBack);
	InitParam.bReusePort	=	true;
	InitParam.nPort			=	m_nPort;

	m_pNetSession->Init(InitParam);
	m_nPort = m_pNetSession->GetConfig()->nPort;
	m_nTreasureSum = 0;

	// ע�����пͻ�������
	PlayerSession::RegisterAllPlayerCmd();
	// ע�����з������˷��͵�����
	PlayerSession::RegisterALLSendCmd();

	return TRUE;
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID WorldSession::Update()
{
	InterlockedExchange((LPLONG)&m_nMsgSendThisTick, 0);
	InterlockedExchange((LPLONG)&m_nMsgProceedThisTick, 0);
	InterlockedExchange((LPLONG)&m_nMsgRecvWait, 0);

	// ��InsertPool���ó�Ҫ�����session
	m_InsertPoolMutex.Acquire();
	PlayerSession* pSession = m_listInsertPool.PopFront();
	while( P_VALID(pSession) )
	{
		AddGlobalSession(pSession);
		pSession = m_listInsertPool.PopFront();
	}
	m_InsertPoolMutex.Release();

	// ����ѡ�˽����session
	UpdateSession();
}

//-------------------------------------------------------------------------------
// ���¸����ͻ���session
//-------------------------------------------------------------------------------
VOID WorldSession::UpdateSession()
{
	// ��������ѡ�˽����session
	m_GlobalSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	m_mapGlobalSessions.ResetIterator();

	while( m_mapGlobalSessions.PeekNext(pSession) )
	{
		if( CON_LOST == pSession->Update())
		{
			// IMSG(_T("player logout in worldsession, sessionid=%u, internalid=%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex());

			RemoveSession(pSession->GetSessionID());
			m_mapGlobalSessions.Erase(pSession->GetSessionID());
			g_loginSession.SendPlayerLogout(pSession->GetSessionID());

			PlayerLogout();

			g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
			SAFE_DEL(pSession);
		}
	}

	m_GlobalSessionMutex.Release();
}

//-------------------------------------------------------------------------------
// ��һЩ������������
//-------------------------------------------------------------------------------
VOID WorldSession::DoHouseKeeping()
{
	CHECK_TIME();

	// �����ײ�����㿪ʼ����
	m_pNetSession->ActiveSend();

	g_world.LockNetWork();

	// ��ѯ����session
	m_AllSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	TMap<DWORD, PlayerSession*>::TMapIterator it = m_mapAllSessions.Begin();

	INT nAllSendCast = 0;
	INT nMaxChokeSize = 0;
	DWORD dwMaxChokeSessionID = GT_INVALID;

	while( m_mapAllSessions.PeekNext(it, pSession) )
	{
		// ���ͻ��˽��ܶ��е���Ϣ����
		if( pSession->GetMsgWaitNum() >= GetMaxRecvWaitUnitNum() && !pSession->IsKicked() )
		{
			IMSG(_T("Kick Too Fast Player[%u, %d]\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum());
			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
			continue;
		}

		// ���ÿͻ���δ�յ�ȷ�ϵİ�������
		INT nCastSize = GetSendCastSize(pSession->GetInternalIndex());
		nAllSendCast += nCastSize;

		if( nCastSize >= GetMaxSendCastSize() && !pSession->IsKicked() )
		{
			IMSG(_T("Kick Choke Player[%u, %u, %d]\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex(), nCastSize);
			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
		}
		else if( nCastSize > nMaxChokeSize )
		{
			nMaxChokeSize = nCastSize;
			dwMaxChokeSessionID = pSession->GetSessionID();
		}
	}

	// todo: ��������ڴ治�����������ߵ�������

	// ���ñ�Tick��SendCast��
	SetMsgSendCast(nAllSendCast);

	m_AllSessionMutex.Release();

	g_world.UnlockNetWork();
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID WorldSession::Destroy()
{
	// ɾ�����е�session����Ϊ�����߳��Ѿ�ֹͣ���������ﲻ��Ҫ������
	PlayerSession* pSession = m_listInsertPool.PopFront();
	while( P_VALID(pSession) )
	{
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);
		pSession = m_listInsertPool.PopFront();
	}

	m_mapGlobalSessions.Clear();
	
	m_mapAllSessions.ResetIterator();
	while( m_mapAllSessions.PeekNext(pSession) )
	{
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);
	}
	m_mapAllSessions.Clear();

	m_pNetSession->Destroy();

	// ��������ע������������GM����
	PlayerSession::UnRegisterALL();
}

//-------------------------------------------------------------------------------
// ��½�ص�����
//-------------------------------------------------------------------------------
DWORD WorldSession::LoginCallBack(LPVOID p1, LPVOID p2)
{
	static DWORD dwJoinGameCmdCrc = m_pUtil->Crc32("NC_JoinGame");

	if( g_world.IsShutingdown() )
		return GT_INVALID;

	// �鿴��һ����Ϣ
	tagUnit* pUnit = (tagUnit*)p1;
	tagXLoginParam* pParam = (tagXLoginParam*)p2;
	
	tagNC_JoinGame* pCmd = (tagNC_JoinGame*)pUnit->pBuffer;

	if( pCmd->dwID != dwJoinGameCmdCrc )
	{
		return GT_INVALID;
	}

	// ��ס����
	g_world.LockNetWork();

	// ��½�������ж��ʺ��Ƿ�Ϸ�
	BYTE byPrivilege = 0;
	BOOL bGuard = true;
	DWORD dwAccOLSec = 0;
	char szAccount[X_SHORT_NAME] = {0};

	tagDWORDTime dwPreLoginTime;
	DWORD	dwPreLoginIP=0;
	if( FALSE == g_loginSession.IsAccountValid(pCmd->dwAccountID, pCmd->dwVerifyCode, byPrivilege, bGuard, 
												dwAccOLSec, szAccount,dwPreLoginTime,dwPreLoginIP) )
	{
		g_world.UnlockNetWork();
		return GT_INVALID;
	}

	if( IsSessionExist(pCmd->dwAccountID) )
	{
		g_world.UnlockNetWork();
		return GT_INVALID;
	}

	// ����һ���µ�session
	PlayerSession* pSession = new PlayerSession(pCmd->dwAccountID, pParam->dwHandle, pParam->dwAddress, byPrivilege,
												bGuard, dwAccOLSec, szAccount,dwPreLoginTime,dwPreLoginIP);

	// �ֱ���뵽��session��globlesession��
	AddSession(pSession);
	AddGlobalSession(pSession);

	// ���õ�½
	PlayerLogin();

	// ֪ͨ��½�������ʺŵ�½�ɹ�
	g_loginSession.SendPlayerLogin(pCmd->dwAccountID, pParam->dwAddress, E_Success);

	// ����Ҽ�¼
	g_gameGuarder.Login(pCmd->dwAccountID, szAccount, pParam->dwAddress);

	// ����
	g_world.UnlockNetWork();

	return pSession->GetSessionID();
}

//--------------------------------------------------------------------------------------
// �ǳ��ص�����
//--------------------------------------------------------------------------------------
DWORD WorldSession::LogoutCallBack(LPVOID p)
{
	DWORD dwSessionID = (DWORD)p;

	// IMSG(_T("Log out callback, sessionid=%u\r\n"), dwSessionID);

	if( g_world.IsShutingdown() )
		return GT_INVALID;

	// ��ס����
	g_world.LockNetWork();

	PlayerSession* pSession = FindSession(dwSessionID);

	if( P_VALID(pSession) )
	{
		pSession->SessionLogout();
	}

	// ��������
	g_world.UnlockNetWork();

	return 0;
}

//--------------------------------------------------------------------------------------
// ���������俪��������һ
//--------------------------------------------------------------------------------------
VOID WorldSession::IncTreasureSum()
{
	InterlockedExchangeAdd((LPLONG)&m_nTreasureSum, 1);
	if (m_nTreasureSum > SERVER_CHEST_RATE)
		InterlockedExchange((LPLONG)&m_nTreasureSum, 1);
}

//--------------------------------------------------------------------------------------
// ���÷��������俪������
//--------------------------------------------------------------------------------------
VOID WorldSession::SetTreasureSum(INT nSum)
{
	if (nSum > SERVER_CHEST_RATE)
		nSum = 1;
	InterlockedExchange((LPLONG)&m_nTreasureSum, nSum);
}

//--------------------------------------------------------------------------------------
// ��ȡ�����������AccountId
//--------------------------------------------------------------------------------------
INT WorldSession::GetAllOLAccountID( DWORD* pAccountIDs )
{
	std::list<PlayerSession*> listPlayerSessions;
	m_AllSessionMutex.Acquire();

	m_mapAllSessions.ExportAllValue(listPlayerSessions);

	INT i=0;
	std::list<PlayerSession*>::iterator itr = listPlayerSessions.begin();
	while (itr != listPlayerSessions.end())
	{
		if (P_VALID(*itr) && (*itr)->GetFatigueGuarder().IsGuard())
		{
			pAccountIDs[i++] = (*itr)->GetSessionID();
		}
		++itr;
	}

	m_AllSessionMutex.Release();

	return i;
}
WorldSession g_worldSession;