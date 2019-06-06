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
#pragma once

#include "..\ServerDefine\login_issue.h"

#include "Mutex.h"
#include "loongworld_mgr.h"
#include "player_mgr.h"
#include "player.h"

class Player;

class LoongWorld
{

public:
	//--------------------------------------------------------------------------
	// CONSTRUCT
	//--------------------------------------------------------------------------
    LoongWorld();
    ~LoongWorld();

    //--------------------------------------------------------------------------
	// ��ʼ�������¼�����
	//--------------------------------------------------------------------------
	BOOL			Init(INT nIndex);
	VOID			Update();
	VOID			Destroy();

	//--------------------------------------------------------------------------
	// �����͹ر�
	//--------------------------------------------------------------------------
	BOOL			WorldLogin(DWORD dwIP, INT nPort, DWORD* pOLAccountIDs, INT nOLAccountIDNum);
	BOOL			WorldLogout();

	//--------------------------------------------------------------------------
	// ����Get
	//--------------------------------------------------------------------------
	DWORD			GetID()				{ return m_Data.dwNameCrc; }
	//temp add by songg
	DWORD			GetWorldID()		{ return m_Data.dwWorldID; }
	EWorldStatus	GetStatus()			{ return m_Data.eStatus; }
	INT				GetMaxOnlineNum()	{ return m_Data.nMaxOnlineNum; }
	INT				GetCurrOnlineNum()	{ return m_Data.nCurOnlineNum; }
	INT				GetCurrOnlineDisplay()	{ return m_Data.nCurOnlineDisplay; }
	LPCTSTR			GetName()			{ return m_Data.szName; }
	INT				GetListQueue()		{ return m_listQueue.Size(); }
	DWORD			GetDBUpdateTime()	{ return m_dwDBUpdateTime;	}
	DWORD			GetDBInsertTime()	{ return m_dwDBInsertTime;	}
	//--------------------------------------------------------------------------
	// ����Set
	//--------------------------------------------------------------------------
	VOID			SetDBUpdateTime(DWORD dwTime)	{ m_dwDBUpdateTime = dwTime;}
	VOID			SetDBInsertTime(DWORD dwTime)	{ m_dwDBInsertTime = dwTime;}

	BOOL			IsValid()			{ return m_Data.bValid; }
	BOOL			IsAutoSeal()		{ return m_Data.bAutoSeal; }

	//--------------------------------------------------------------------------
	// ����״̬
	//--------------------------------------------------------------------------
	VOID			UpdateStatus(EWorldStatus eStatus, INT nCurOnline, INT nMaxOnline, INT nCurOnlineDisplay=-1);

	//--------------------------------------------------------------------------
	// ��������
	//--------------------------------------------------------------------------
	VOID			KickPlayer(Player* pPlayer);
	VOID			AddPlayer(Player* pPlayer);

	//----------------------------------------------------------------------------
	// ������������
	//----------------------------------------------------------------------------
	VOID			PlayerWillLoginRet(DWORD dwAccountID, DWORD dwErrorCode);

	//----------------------------------------------------------------------------
	// ��LoongWorld����������Ϣ
	//----------------------------------------------------------------------------
	VOID			SendKickPlayerMsg(tagNLW_KickPlayer& send) { sLoongWorldMgr.SendMsg(m_Data.dwNameCrc, &send, send.dwSize); }

private:
	//----------------------------------------------------------------------------
	// �����������ӣ��Ŷ��б�͵ȴ��б�
	//----------------------------------------------------------------------------
	VOID			UpdateSession();
	VOID			UpdateInsertPlayer();
	VOID			UpdateQueuedPlayer();
	VOID			UpdateWaitingPlayer();
	VOID			UpdateKickedPlayer();

	//----------------------------------------------------------------------------
	// ��������б�
	//----------------------------------------------------------------------------
	VOID			AddIntoQueue(Player* pPlayer);
	VOID			AddIntoWaitingMap(Player* pPlayer);

private:
    TObjRef<Util>					m_pUtil;
	
	//---------------------------------------------------------------------------------
	// ��Ϸ����״̬
	//---------------------------------------------------------------------------------
	tagLoongWorldData				m_Data;
	DWORD							m_dwTime;					// ���ڶ�ʱ�����Ŷ��б�

	DWORD							m_dwDBUpdateTime;			// ����״̬����ʱ��
	DWORD							m_dwDBInsertTime;			// ����״̬����ʱ��
	//----------------------------------------------------------------------------------
	// ����б�
	//----------------------------------------------------------------------------------
	TList<Player*>					m_listInsertPlayer;			// �����б�
	TList<Player*>					m_listQueue;				// �Ŷ��б�
	TMap<DWORD, Player*>			m_mapWaitting;				// �ȴ�������Ϸ���������б�
	TList<Player*>					m_listKickPlayer;			// ��������ԭ���ߵ����

};

//----------------------------------------------------------------------------------------
// �ߵ�����Ϸ�����е�һ�����
//----------------------------------------------------------------------------------------
inline VOID LoongWorld::KickPlayer(Player* pPlayer)
{
	if( !P_VALID(pPlayer) ) return;

	sPlayerMgr.Kick(pPlayer->GetCDIndex());
	m_listKickPlayer.PushBack(pPlayer);
}

//----------------------------------------------------------------------------------------
// ����Ҽ������Ϸ�����б�
//----------------------------------------------------------------------------------------
inline VOID LoongWorld::AddPlayer(Player* pPlayer)
{
	if( P_VALID(pPlayer) )
	{
		m_listInsertPlayer.PushFront(pPlayer);
	}
}

//-----------------------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------------------
inline VOID LoongWorld::UpdateStatus(EWorldStatus eStatus, INT nCurOnline, INT nMaxOnline, INT nCurOnlineDisplay/*=-1*/)
{
	m_Data.eStatus			=	eStatus;
	m_Data.nMaxOnlineNum	=	nMaxOnline;
	m_Data.nCurOnlineNum	=	nCurOnline;
	m_Data.nCurOnlineDisplay=	nCurOnlineDisplay;
}




