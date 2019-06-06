//-----------------------------------------------------------------------------
//!\file game_world.h
//!\brief ��Ϸ����
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/base_define.h"

class GameServer;
class GameServerWorld;
class GameServerDB;
class ServerDB;
//-----------------------------------------------------------------------------
// ��Ϸ����
//-----------------------------------------------------------------------------
class GameWorld
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	GameWorld();
	~GameWorld();

	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL Init(INT nIndex);
	VOID Update();

	//-------------------------------------------------------------------------
	// ϵͳ��Ϣ���
	//-------------------------------------------------------------------------
	VOID GenerateWorldStaticInfo(LPBYTE &pBuff, DWORD& dwSize);
	VOID GenerateWorldDynamicInfo(LPBYTE &pBuff, DWORD& dwSize);

	//-------------------------------------------------------------------------
	// ����Get
	//-------------------------------------------------------------------------
	DWORD GetID() const						{ return m_dwWorldID;	}

	GameServerWorld*	GetWorld()	const	{ return m_pWorldServer;}
	GameServerDB*		GetDB() const		{ return m_pDBServer;	}
	ServerDB*			GetServerDB() const		{ return m_pDB;			}

	LPCTSTR				GetWorldName() const	{ return m_szWorldName; }	

	VOID				SendWorldMsg(tagNetCmd* pCmd);

private:
	BOOL InitDB( LPCTSTR szWorld );

private:
	DWORD					m_dwWorldID;				// ��Ϸ����ID
	TCHAR					m_szWorldName[X_SHORT_NAME];// ��Ϸ��������
	GameServerWorld*		m_pWorldServer;				// LoongWorld������
	GameServerDB*			m_pDBServer;				// LoongDB������
	ServerDB*				m_pDB;						// todo: ����Ϸ��������ݿ�����
};