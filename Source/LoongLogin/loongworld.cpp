//-----------------------------------------------------------------------------
//!\file loongworld.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ����ĳ����������Ϸ����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/login.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/login_issue.h"

#include "loongworld.h"
#include "loongworld_mgr.h"
#include "player_mgr.h"
#include "player.h"
#include "db.h"
#include "login_server.h"
#include "fatigue_mgr.h"

//------------------------------------------------------------------------------
// CONSTANTS
//------------------------------------------------------------------------------
const INT	ALLOW_PLAYER_LOGIN	=	2;		// �������һ���˽�������
const INT	UPDATE_QUEUE_TIME	=	2000;	// �����Ŷ��б�ʱ����
const FLOAT	LOGIN_LIMIT			=	0.9f;	// ���ﵽLoongWorld�������޵�90%,�Ϳ�ʼ�Ŷ�

//-------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------
LoongWorld::LoongWorld() : m_Data(), m_dwTime(0)
{

}

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
LoongWorld::~LoongWorld()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------
BOOL LoongWorld::Init(INT nIndex)
{
	if( nIndex < 0 ) return FALSE;

	m_pUtil		=	"Util";
	m_dwTime	=	timeGetTime();
	m_dwDBUpdateTime = timeGetTime();
	m_dwDBInsertTime = timeGetTime();
	// �������ļ��еõ���Ϸ����ľ�������
	TObjRef<VarContainer> pVar = "LoginServerConfig";

	TCHAR szTemp[X_SHORT_NAME] = {_T('\0')};
	_stprintf(szTemp, _T("zone%d"), nIndex);

	// �õ�world������
	tstring strWorldName = pVar->GetString(_T("name"), szTemp);
	if( strWorldName.empty() ) return FALSE;

	// ����LoongWorldData�е�����
	m_Data.bValid			=	FALSE;
	m_Data.eStatus			=	EWS_InitNotDone;
	m_Data.nCurOnlineNum	=	0;
	m_Data.nMaxOnlineNum	=	0;
	m_Data.nCurOnlineDisplay=	0;
	m_Data.dwNameCrc		=	m_pUtil->Crc32(strWorldName.c_str());
	m_Data.dwIP				=	0;
	m_Data.nPort			=	0;
	m_Data.bAutoSeal		=	pVar->GetDword(_T("auto_seal"), szTemp);
	_tcsncpy(m_Data.szName, strWorldName.c_str(), X_SHORT_NAME);
	//temp add by songg
  	m_Data.dwWorldID		=	pVar->GetDword(_T("id"), szTemp); 
    return TRUE;
}

//-------------------------------------------------------------------------------
// ���º���
//-------------------------------------------------------------------------------
VOID LoongWorld::Update()
{
	if( IsValid() )
	{
		// ��������������Ϣ
		UpdateSession();

		// �������м��뵽��Ϸ��������
		UpdateInsertPlayer();

		// ���������Ŷӵ����
		UpdateQueuedPlayer();

		// �������еȴ��б��е����
		UpdateWaitingPlayer();

		// �����ߵ��б��е����
		UpdateKickedPlayer();
	}
	else
	{
		// �����ߵ��б��е����
		UpdateKickedPlayer();
	}
}

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
VOID LoongWorld::UpdateSession()
{
	LPBYTE	pRecv		=	NULL;
	DWORD	dwSize		=	0;
	INT		nUnRecv		=	0;
	DWORD	dwTime		=	0;

	// ������Ϣ
	pRecv = sLoongWorldMgr.RecvMsg(m_Data.dwNameCrc, dwSize, nUnRecv);

	while( P_VALID(pRecv) )
	{
		// ������Ϣ
		sLoongWorldMgr.HandleCmd((tagNetCmd*)pRecv, dwSize, (DWORD)this);

		// �黹��Ϣ
		sLoongWorldMgr.ReturnMsg(m_Data.dwNameCrc, pRecv);

		// ��ȡ��һ����Ϣ
		pRecv = sLoongWorldMgr.RecvMsg(m_Data.dwNameCrc, dwSize, nUnRecv);
	}
}

//-------------------------------------------------------------------------------
// ���²��������б�
//-------------------------------------------------------------------------------
VOID LoongWorld::UpdateInsertPlayer()
{
	if( !m_Data.bValid ) return;
	if( EWS_Well != m_Data.eStatus ) return;

	INT nBeginWaitNum = m_Data.nMaxOnlineNum * LOGIN_LIMIT;			// ��ʼ�ŶӵĽ綨ֵ
	INT nTemp = m_Data.nCurOnlineNum + m_mapWaitting.Size();		// �綨ֵ

	Player* pPlayer	= m_listInsertPlayer.PopFront();
	while( P_VALID(pPlayer) )
	{
		// �鿴�Ŷ��б����Ƿ�Ϊ��
		if( m_listQueue.Empty() )
		{
			// �����ǰ���������������ŶӵĻ��ߣ���ʼ�Ŷ�
			if( nTemp >= nBeginWaitNum )
			{
				AddIntoQueue(pPlayer);
			}
			// ����Ҫ�Ŷ�
			else
			{
				AddIntoWaitingMap(pPlayer);
				++nTemp;		// �ۼ�һ������
			}
		}
		// �Ŷ��б�Ϊ�գ�ֱ����ӵ��Ŷ��б�
		else
		{
			AddIntoQueue(pPlayer);
		}

		// �ٻ�ȡһ�����
		pPlayer = m_listInsertPlayer.PopFront();
	}
}

//-------------------------------------------------------------------------------
// �����Ŷ��б�
//-------------------------------------------------------------------------------
VOID LoongWorld::UpdateQueuedPlayer()
{
	if( !m_Data.bValid ) return;
	if( EWS_Well != m_Data.eStatus ) return;

	if( m_listQueue.Empty() ) return;

	// �����Ƿ��˸����б��ʱ��
	BOOL bUpdateQueue = FALSE;
	if( timeGetTime() - m_dwTime > UPDATE_QUEUE_TIME )
	{
		m_dwTime = timeGetTime();
		bUpdateQueue = TRUE;
	}

	// �����Ƿ��Ѿ��������˵ı�־
	BOOL bSlowEnter = FALSE;

	INT nBeginWaitNum = m_Data.nMaxOnlineNum * LOGIN_LIMIT;							// ��ʼ�ŶӵĽ綨ֵ
	INT nNoWaitNum = nBeginWaitNum - m_Data.nCurOnlineNum - m_mapWaitting.Size();	// ����ֱ�ӽ�����Ϸ���������

	// ������ܽ��룬��ֱ������bSlowEnterΪTRUE
	INT nHavePlayer = m_Data.nMaxOnlineNum - m_Data.nCurOnlineNum - m_mapWaitting.Size();
	if( nHavePlayer <= 0 ) bSlowEnter = TRUE;

	// ��ѯÿ�����
	m_listQueue.ResetIterator();
	Player* pPlayer = NULL;

	INT nIndexInQueue = 0;		// �ڶ����е�����

	while( m_listQueue.PeekNext(pPlayer) )
	{
		if( pPlayer->IsConnLost() )		// ��ҵ�����
		{
			m_listQueue.Erase(pPlayer);
			sPlayerMgr.PlayerLogout(pPlayer);
			continue;
		}

		// ������˸��µ�ʱ��
		if( bUpdateQueue )
		{
			if( nNoWaitNum > 0 )			// �鿴�Ƿ񻹿���ֱ�ӽ�
			{
				m_listQueue.Erase(pPlayer);

				tagNLS_Queue send;
				send.dwPosition = 0;
				send.dwTime = 0;
				pPlayer->SendMessage(&send, send.dwSize);

				AddIntoWaitingMap(pPlayer);
				--nNoWaitNum;
			}
			else if( !bSlowEnter )			// ÿ�η�һ����
			{
				m_listQueue.Erase(pPlayer);

				tagNLS_Queue send;
				send.dwPosition = 0;
				send.dwTime = 0;

				pPlayer->SendMessage(&send, send.dwSize);

				AddIntoWaitingMap(pPlayer);
				bSlowEnter = TRUE;
			}
			else							// ��Щ�˼����Ŷ���
			{
				tagNLS_Queue send;
				send.dwPosition = ++nIndexInQueue;
				send.dwTime = nIndexInQueue * ALLOW_PLAYER_LOGIN;
				pPlayer->SendMessage(&send, send.dwSize);
			}
		}
	}
}

//-------------------------------------------------------------------------------
// ���µȴ������
//-------------------------------------------------------------------------------
VOID LoongWorld::UpdateWaitingPlayer()
{
	if( m_mapWaitting.Empty() ) return;

	Player* pPlayer = NULL;
	m_mapWaitting.ResetIterator();

	while( m_mapWaitting.PeekNext(pPlayer) )
	{
		if( pPlayer->IsConnLost() )				// ������
		{
			m_mapWaitting.Erase(pPlayer->GetAccountID());
			sPlayerMgr.PlayerLogout(pPlayer);
		}
		else if( pPlayer->IsSelectWorldOk() )		// �Ѿ���֤�ɹ���
		{
			if( CalcTimeDiff(GetCurrentDWORDTime(), pPlayer->GetKickTime()) > 5 )	// ��֤�ɹ���������5���Ӳ��Ͽ����ӣ����ߵ�
			{
				m_mapWaitting.Erase(pPlayer->GetAccountID());
				KickPlayer(pPlayer);
			}
		}
	}
}

//-------------------------------------------------------------------------------
// �����ߵ�������б�
//-------------------------------------------------------------------------------
VOID LoongWorld::UpdateKickedPlayer()
{
	if( m_listKickPlayer.Empty() ) return;

	Player* pPlayer = NULL;
	m_listKickPlayer.ResetIterator();

	while( m_listKickPlayer.PeekNext(pPlayer) )
	{
		if( pPlayer->IsConnLost() )		// ���ߵ���
		{
			m_listKickPlayer.Erase(pPlayer);
			sPlayerMgr.PlayerLogout(pPlayer);
		}
	}
}

//-------------------------------------------------------------------------------
// �ͷ���Դ
//-------------------------------------------------------------------------------
VOID LoongWorld::Destroy()
{
	// ȥ��logtime
}

//-------------------------------------------------------------------------------
// ���뵽�Ŷ��б���
//-------------------------------------------------------------------------------
VOID LoongWorld::AddIntoQueue(Player* pPlayer)
{
	if( !P_VALID(pPlayer) ) return;

	m_listQueue.PushBack(pPlayer);

	// ���ͻ��˷�����Ϣ
	tagNLS_ProofResult send;
	send.bGuard			=	pPlayer->IsGuard();
	send.dwErrorCode	=	E_ProofResult_Queue;

	pPlayer->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------
// ���뵽�ȴ��б���
//-------------------------------------------------------------------------------
VOID LoongWorld::AddIntoWaitingMap(Player* pPlayer)
{
	if( !P_VALID(pPlayer) ) return;

	m_mapWaitting.Add(pPlayer->GetAccountID(), pPlayer);

	// ��world������Ϣ
	tagNLW_PlayerWillLogin send;
	send.dwVerifyCode	=	pPlayer->GetVerifyCode();
	send.dwAccountID	=	pPlayer->GetAccountID();
	send.byPrivilege	=	pPlayer->GetPrivilege();
	send.bGuard			=	pPlayer->GetPlayerData().bGuard;
	send.dwAccOLSec		=	pPlayer->GetPlayerData().nAccOLSec;
	//add for prevent thief
	send.dwPreLoginIP		=	pPlayer->GetPreLoginIP();
	send.dwPreLoginTime		=	pPlayer->GetPreLoginTime();

	strncpy_s(send.szAccount, pPlayer->GetAccoutName(), X_SHORT_NAME);

	sLoongWorldMgr.SendMsg(m_Data.dwNameCrc, &send, send.dwSize);
}

//-------------------------------------------------------------------------------
// LoongWorld��½
//-------------------------------------------------------------------------------
BOOL LoongWorld::WorldLogin(DWORD dwIP, INT nPort, DWORD* pOLAccountIDs, INT nOLAccountIDNum)
{
	if( IsValid() ) return FALSE;	// ����Ƿ��Ѿ�������

	// ���õ�½��Ϣ
	m_Data.bValid			=	TRUE;
	m_Data.dwIP				=	dwIP;
	m_Data.nPort			=	nPort;
	m_Data.eStatus			=	EWS_InitNotDone;
	m_Data.nCurOnlineNum	=	0;
	m_Data.nMaxOnlineNum	=	0;
	m_Data.nCurOnlineDisplay=	0;

	// tdb:worldnamecrcΪ��world��״̬ΪEPLS_Online�����״̬ΪEPLS_Unknown
	sBeton.FixOneWorldPlayerLoginStatus(m_Data.dwNameCrc, EPLS_Online, EPLS_Unknown);

	// tbd:�������״̬��ָ����Ҹ���ΪΪEPLS_Online
	sBeton.ResetPlayersLoginStatus(pOLAccountIDs, nOLAccountIDNum, EPLS_Online);

	// tdb:worldnamecrcΪ��world��״̬ΪEPLS_Unknown�����״̬ΪEPLS_Offline
	sBeton.FixOneWorldPlayerLoginStatus(m_Data.dwNameCrc, EPLS_Unknown, EPLS_OffLine);

	if (g_login.IsEnableFatigueGuard())
		sFatigueMgr.ResetWorldAccounts(m_Data.dwNameCrc, pOLAccountIDs, nOLAccountIDNum);

	return TRUE;
}

//-------------------------------------------------------------------------------
// LoongWorld�ǳ�
//-------------------------------------------------------------------------------
BOOL LoongWorld::WorldLogout()
{
	if( !IsValid() ) return FALSE;	// ����Ƿ�һ��ʼ��û������

	// ���õǳ���Ϣ
	m_Data.bValid			=	FALSE;
	m_Data.dwIP				=	0;
	m_Data.nPort			=	0;
	m_Data.eStatus			=	EWS_InitNotDone;
	m_Data.nCurOnlineNum	=	0;
	m_Data.nMaxOnlineNum	=	0;
	m_Data.nCurOnlineDisplay=	0;

	// �ߵ����и���Ϸ�����ڵ����
	Player* pPlayer = NULL;

	// �����ߵ�insertlist�е����
	pPlayer = m_listInsertPlayer.PopFront();
	while( P_VALID(pPlayer) )
	{
		KickPlayer(pPlayer);

		pPlayer = m_listInsertPlayer.PopFront();
	}

	// ���ߵ��Ŷ��б��е����
	pPlayer = m_listQueue.PopFront();
	while( P_VALID(pPlayer) )
	{
		KickPlayer(pPlayer);

		pPlayer = m_listQueue.PopFront();
	}

	// ���ߵ��ȴ��б��е����
	m_mapWaitting.ResetIterator();
	while( m_mapWaitting.PeekNext(pPlayer) )
	{
		KickPlayer(pPlayer);
	}
	m_mapWaitting.Clear();

	if (g_login.IsEnableFatigueGuard())
		sFatigueMgr.ResetWorldAccounts(m_Data.dwNameCrc, NULL, 0);

	return TRUE;
}

//-----------------------------------------------------------------------
// ��ҵ�¼���
//-----------------------------------------------------------------------
VOID LoongWorld::PlayerWillLoginRet(DWORD dwAccountID, DWORD dwErrorCode)
{
	// �ڵȴ��б��в������
	Player* pPlayer = m_mapWaitting.Peek(dwAccountID);
	if( P_VALID(pPlayer) )
	{
		// �������֤�ɹ���������ѡ����Ϸ�������
		if( E_Success == dwErrorCode )
		{
			pPlayer->SetSelectWorldOK();
		}

		// ����ҷ��ͷ��ؽ��
		tagNLS_ProofResult send;
		send.bGuard			=	pPlayer->IsGuard();
		send.dwIndex		=	0;
		send.dwIP			=	m_Data.dwIP;
		send.dwPort			=	m_Data.nPort;
		send.dwAccountID	=	dwAccountID;
		send.dwVerifyCode	=	pPlayer->GetVerifyCode();

		if( E_Success == dwErrorCode )
			send.dwErrorCode	=	E_Success;
		else
			send.dwErrorCode	=	E_ProofResult_Account_No_Match;

		pPlayer->SendMessage(&send, send.dwSize);

		// �������Ҫ���ߵ���ʱ��
		pPlayer->SetKickTime(GetCurrentDWORDTime());
	}
}


////-----------------------------------------------------------------------
//// ��֤��Ϣ
////-----------------------------------------------------------------------
//VOID LoongWorld::HandleCertification(tagNetCmd* pMsg,LPVOID param)
//{
//     tagNWL_Certification* pRecv = (tagNWL_Certification*)pMsg;
//    
//     m_Data.nPort = pRecv->dwPort;
//     
//	 //�õ�IP
//	 /*sockaddr_in addr_in;
//	 m_pSession->GetClientSockName(&addr_in, m_dwClientID);
//	 m_pData->dwIP       = addr_in.sin_addr.S_un.S_addr;*/
//	m_pData->dwIP	= pRecv->dwIP;
//
//	 in_addr addr;
//	 addr.S_un.S_addr =  m_pData->dwIP;
//	 tstring str = m_pUtil->Unicode8ToUnicode(inet_ntoa( addr));
//	 str += _T("\n");
//	IMSG(str.c_str());
//     tagNLW_Certification send;
//     send.dwGoldenCode = sLoongWorldMgr.m_dwLoginServerGoldenCode;
//	 m_pSession->Send( m_pData->dwClientID, &send, send.dwSize );
//}
//    
////-----------------------------------------------------------------------
//// ״̬��Ϣ
////-----------------------------------------------------------------------
//VOID LoongWorld::HandleZoneServerStatus(tagNetCmd* pMsg, LPVOID param)
//{
//    tagNWL_WorldStatus* pRecv = (tagNWL_WorldStatus*)pMsg;
//	
//	m_Data.eStatus			=	pRecv->eStatus;
//    m_Data.nMaxOnlineNum	=	pRecv->nPlayerNumLimit;
//	m_Data.nCurOnlineNum	=	pRecv->nCurPlayerNum;
//}
//
////-----------------------------------------------------------------------
//// ��ҵ�¼LoongWorld
////-----------------------------------------------------------------------
//VOID LoongWorld::HandlePlayerLogin(tagNetCmd* pMsg,LPVOID param)
//{
//    tagNWL_PlayerLogin* pRecv = (tagNWL_PlayerLogin*)pMsg;
//    
//
//	// �����ۼ�����ʱ��
//	m_LockLogTime.Acquire();
//	tagLogTimeData* pLogTimeData = m_mapLogTime.Peek(pRecv->dwAccountID);
//	m_LockLogTime.Release();
//
//	ASSERT(P_VALID(pLogTimeData));
//	if (!P_VALID(pLogTimeData))
//	{
//		m_LockLogTime.Acquire();
//		m_mapLogTime.Erase(pRecv->dwAccountID);
//		SAFE_DEL(pLogTimeData);
//		m_LockLogTime.Release();
//		return;
//	}
//	pLogTimeData->SetLogin(GetCurrentDWORDTime());
//	pLogTimeData->UpdateAccULSec();
//
//    if(pRecv->dwErrorCode != E_Success)
//    {
//
//		//���뵽KickList
//		//sPlayerMgr.AddKickList(pPlayer);
//		IMSG( _T("Login ErrorCode =%d  \r\n"), pRecv->dwErrorCode );
//		
//		sBeton.UpdatePlayerIntoWorld(LOGIN_STATUS_OFFLINE,pLogTimeData->dwtLogin, pLogTimeData->GetAccULSec(), pLogTimeData->GetAccOLSec(), pRecv->dwAccountID);
//		pLogTimeData->SetLogout(GetCurrentDWORDTime());
//		pLogTimeData->UpdateAccOLSec();
//		sBeton.sBeton.UpdatePlayerOutWorld(LOGIN_STATUS_OFFLINE, pLogTimeData->dwtLogout, pLogTimeData->GetCurOLSec(), pLogTimeData->GetAccOLSec(), pRecv->dwAccountID);
//		
//		ILOG->Write(_T("Player Login Failure! AccountID:%u ErrorCode=%d\r\n"), pRecv->dwAccountID, pRecv->dwErrorCode);
//
//		// ɾ��ʱ������
//		m_LockLogTime.Acquire();
//		m_mapLogTime.Erase(pRecv->dwAccountID);
//		SAFE_DEL(pLogTimeData);
//		m_LockLogTime.Release();
//
//		return;
//    }
//    
//    //�������ݿ�����
//    sBeton.UpdatePlayerIntoWorld(LOGIN_STATUS_ONLINE,pLogTimeData->dwtLogin, pLogTimeData->GetAccULSec(), pLogTimeData->GetAccOLSec(), pRecv->dwAccountID);
//
//	//���Ҵ��û�ID������
//	tstring name = sBeton.QueryPlayerAccountName(pRecv->dwAccountID);
//
//	IMSG( _T("Player AccountID =%u Name = %s Login \r\n"), pRecv->dwAccountID, name.c_str());
//	ILOG->Write(_T("Player AccountID:%u Name = %s Login\r\n"), pRecv->dwAccountID, name.c_str());
//}
//
////-----------------------------------------------------------------------
//// ��ҵǳ�LoongWorld
////-----------------------------------------------------------------------
//VOID LoongWorld::HandlePlayerLogout(tagNetCmd* pMsg,LPVOID param)
//{
//    tagNWL_PlayerLogout* pRecv = (tagNWL_PlayerLogout*)pMsg;
//
//	// �����ۼ�����ʱ��
//	m_LockLogTime.Acquire();
//	tagLogTimeData* pLogTimeData = m_mapLogTime.Peek(pRecv->dwAccountID);
//	m_LockLogTime.Release();
//
//	if (P_VALID(pLogTimeData))
//	{
//		pLogTimeData->SetLogout(GetCurrentDWORDTime());
//		pLogTimeData->UpdateAccOLSec();
//
//		//�������ݿ�����
//		sBeton.UpdatePlayerOutWorld(LOGIN_STATUS_OFFLINE, pLogTimeData->dwtLogout, pLogTimeData->GetCurOLSec(), pLogTimeData->GetAccOLSec(), pRecv->dwAccountID);
//
//		// ɾ��ʱ������
//		m_LockLogTime.Acquire();
//		m_mapLogTime.Erase(pRecv->dwAccountID);
//		SAFE_DEL(pLogTimeData);
//		m_LockLogTime.Release();
//	}
//	else
//	{
//		sBeton.UpdatePlayerOutWorld(LOGIN_STATUS_OFFLINE, GetCurrentDWORDTime(), 0, 0, pRecv->dwAccountID);
//	}
//
//	//���Ҵ��û�ID������
//	tstring name = sBeton.QueryPlayerAccountName(pRecv->dwAccountID);
//
//	//��¼��ҵǳ�Log
//	sBeton.InsertLogoutLog(pRecv->dwAccountID);
//	IMSG( _T("Player AccountID =%u Name = %s Logout \r\n"), pRecv->dwAccountID, name.c_str());
//	ILOG->Write(_T("Player AccountID:%u Name = %s Logout\r\n"), pRecv->dwAccountID, name.c_str());
//}
//
////-----------------------------------------------------------------------
//// �����LoongWorld������¼����
////-----------------------------------------------------------------------
// VOID LoongWorld::HandlePlayerWillLogin(tagNetCmd* pMsg,LPVOID param)
//{
//    tagNWL_PlayerWillLogin* pRecv = (tagNWL_PlayerWillLogin*)pMsg;
//    
//    //�������
//    Player* pPlayer = m_mapWaitting.Peek(pRecv->dwAccountID);
//    if(!P_VALID(pPlayer))
//	{
//		//ֱ�Ӹ������ݿ�
//		//tbc
//		sBeton.UpdatePlayerOutWorld(LOGIN_STATUS_OFFLINE, (DWORD)GetCurrentDWORDTime(), 0, 0, pRecv->dwAccountID);
//		IMSG( _T("HandlePlayerWillLogin Player Not Found \r\n") );
//		ILOG->Write(_T("HandlePlayerWillLogin Player Not Found! AccountID:%u \r\n"), pRecv->dwAccountID);
//        return;
//	}
//    //����ҷ��� NLS_ProofResult��Ϣ
//    tagNLS_ProofResult send;
//    send.bGuard = pPlayer->IsFatigueGuard();
//    send.dwErrorCode = E_Success;
//    send.dwIndex = 0;
//    send.dwIP = m_Data.dwIP;
//    send.dwPort = m_Data.nPort;
//    send.dwAccountID = pRecv->dwAccountID;
//    send.dwVerifyCode = pPlayer->GetRandomNum();
//	pPlayer->SendMsg(&send, send.dwSize);
//
//	////����Kick�б�
//	pPlayer->SetKickTime(GetCurrentDWORDTime());
//	//sPlayerMgr.AddKickList(pPlayer);
//	pPlayer->SetProofResult(TRUE);
//
//	// ����û���¼ʱ��
//	m_LockLogTime.Acquire();
//	tagLogTimeData* pLogTimeData = m_mapLogTime.Peek(pPlayer->GetAccountID());
//	if (!P_VALID(pLogTimeData))
//	{
//		pLogTimeData = new tagLogTimeData;
//		m_mapLogTime.Add(pPlayer->GetAccountID(), pLogTimeData);
//	}
//	m_LockLogTime.Release();
//	pLogTimeData->bGuard = pPlayer->GetPlayerData()->bGuard;
//	pLogTimeData->dwtLastLogout = pPlayer->GetPlayerData()->dwLastLogoutTime;
//	pLogTimeData->dwAccOLSec = pPlayer->GetPlayerData()->dwAccOLSec;
//	pLogTimeData->dwAccULSec = pPlayer->GetPlayerData()->dwAccULSec;
//}
//-----------------------------------------------------------------------
// LoongWorld�߳����
//-----------------------------------------------------------------------
// VOID LoongWorld::HandleKickLog(tagNetCmd* pMsg,LPVOID param)
// {
// 	MGET_MSG(pRecv, pMsg, NWL_KickLog);
// 
// 	tstring tsAccountName = sBeton.QueryPlayerAccountName(pRecv->dwAccountID).c_str();
// 
// #ifdef _DEBUG
// 	IMSG(_T("************GameGuarderKick*************\r\n"));
// 	IMSG(_T("*Kick Name: %s\r\n*AccountID: %d\r\n*Time: %d\r\n*Seal: %d\r\n*ErrCode: %d\r\n"), tsAccountName.c_str(), pRecv->dwAccountID, pRecv->dwKickTime, pRecv->bySeal, pRecv->u16ErrCode);
// 	IMSG(_T("************GameGuarderKick*************\r\n"));
// #endif
// 
// 	ILOG->Write(_T("Kick Name: %s  AccountID: %d  Time: %d  Seal: %d ErrCode: %d\r\n "), tsAccountName.c_str(), pRecv->dwAccountID, pRecv->dwKickTime, pRecv->bySeal, pRecv->u16ErrCode);
// 
// 	sBeton.InsertKickLog(tsAccountName.c_str(), pRecv->dwAccountID, pRecv->dwKickTime, pRecv->u16ErrCode, pRecv->bySeal);
// 	if (pRecv->bySeal && m_Data.bAutoSeal)
// 	{
// 		sBeton.UpdateSealAccount(pRecv->dwAccountID, pRecv->dwKickTime, pRecv->u16ErrCode, pRecv->bySeal);
// 	}
// 	
// }
// 
// 
// 
// 
