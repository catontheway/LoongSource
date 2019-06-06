//-----------------------------------------------------------------------------
//!\file section.h
//!\brief ����
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

//#include "../ServerDefine/msg_rt.h"
#include "../ServerDefine/rt_define.h"
#include "../WorldDefine/base_define.h"

class GameWorld;
class GameServer;
class GameServerLogin;
class GameServerWorld;
class GameServerDB;

typedef DWORD (GameWorld::*FUN_GAMEWORLD)(DWORD);

//-----------------------------------------------------------------------------
// ��Ϸ����
//-----------------------------------------------------------------------------
class Section
{
public:
	Section(DWORD dwSectionID, LPCTSTR szSectionName);
	~Section(){};

	//--------------------------------------------------------------------------
	// ��ʼ�������¼�����
	//--------------------------------------------------------------------------
	BOOL				Init();
	VOID				Destroy();
	VOID				Update();

	//---------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------
	BOOL				StartLoginServer();
	BOOL				StartWorld(DWORD dwWorldID, EServerType eType);
	BOOL				CloseLoginServer(BOOL bForce=FALSE);
	BOOL				CloseWorld(DWORD dwWorldID, EServerType eType, BOOL bForce=FALSE);
	BOOL				SendSystemCmd2LoginServer(tagNetCmd*);
	BOOL				SendSystemCmd2World(DWORD dwWorldID, EServerType eType, tagNetCmd* pMsg);
	BOOL				SetMaxNum(DWORD dwWorldID, LPVOID pMsg);
	BOOL				UpdateMall(DWORD dwWorldID, LPVOID pMsg);
	BOOL				AccountForbid(LPVOID pMsg, DWORD dwSize);
	BOOL				IPBlacklist(LPVOID pMsg);

	//---------------------------------------------------------------------------
	// ϵͳ��Ϣ���
	//---------------------------------------------------------------------------
	VOID				GenerateSectionStaticInfo(LPBYTE &pBuff, DWORD& dwSize);
	VOID				GenerateSectionDynamicInfo(LPBYTE &pBuff, DWORD& dwSize);

	//---------------------------------------------------------------------------
	// ����Get
	//---------------------------------------------------------------------------
	DWORD				GetID() const						{ return m_dwSectionID; }
	GameWorld*			GetWorld(DWORD dwWorldID)			{ return m_mapAllWorld.Peek(dwWorldID); }
	TMap<DWORD, GameWorld*>	GetWorldMap()					{ return m_mapAllWorld;}
	INT					GetWorldNum()						{ return m_mapAllWorld.Size(); }
	GameServerLogin*	GetLoginServer()					{ return m_pLoginServer; }
	GameServerWorld*	GetWorldServer(DWORD dwWorldID);
	GameServerDB*		GetDBServer(DWORD dwWorldID);

	LPCTSTR				GetSectionName() const				{ return m_szSectionName; }

	//---------------------------------------------------------------------------
	// �����������
	//---------------------------------------------------------------------------
	GameServer*			FindServer(DWORD dwID);
	GameServerLogin*	FindLoginServer(DWORD dwID);
	GameServerWorld*	FindWorldServer(DWORD dwID);
	GameServerDB*		FindDBServer(DWORD dwID);
	GameWorld*			FindGameWorld(DWORD dwID);
	BOOL				ForAllGameWorld( DWORD* pWorldIDs, INT nNum, FUN_GAMEWORLD pFun,  PVOID pPara);

private:
	DWORD									m_dwSectionID;						// ����ID
	TCHAR									m_szSectionName[X_SHORT_NAME];		// ��������

	GameServerLogin*						m_pLoginServer;						// ������login������
																				// todo���ô������ʺ����ݿ�
	TMap<DWORD, GameWorld*>					m_mapAllWorld;						// �ô������е���Ϸ����

	TMap<DWORD, DWORD>						m_mapIDWorldID;						// ����ID����Ϸ����Ķ�Ӧ��ϵ
};
