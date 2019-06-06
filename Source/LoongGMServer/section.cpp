//-----------------------------------------------------------------------------
//!\file section.cpp
//!\brief ����
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "server.h"
#include "section.h"
#include "game_world.h"
#include "game_server.h"
#include "section_mgr.h"
#include "../ServerDefine/msg_rt_s.h"

//-----------------------------------------------------------------------------
//    
//-----------------------------------------------------------------------------
Section::Section(DWORD dwSectionID, LPCTSTR szSectionName) : m_dwSectionID(dwSectionID), m_pLoginServer(NULL)
{
	memcpy(m_szSectionName, szSectionName, sizeof(m_szSectionName));
}

//-----------------------------------------------------------------
//    
//-----------------------------------------------------------------
BOOL Section::Init()
{
	// ���·��
	TCHAR tszFilePath[X_LONG_STRING] = {0};
	GetModuleFileName(NULL, tszFilePath, X_LONG_STRING);
	int nCount = _tcslen(tszFilePath);
	while( tszFilePath[nCount-1] != '\\' )
	{
		nCount--;
	}
	tszFilePath[nCount] = '\0';

	// ���ɴ��������ļ���
	TCHAR szSectionCfg[MAX_PATH] = {_T('\0')};
	_stprintf(szSectionCfg, _T("%ssection_%s"), tszFilePath, m_szSectionName);
	TObjRef<Util> pUtil = "Util";
	TCHAR tszFile[MAX_PATH] = {0};
	if (!pUtil->GetIniPath(tszFile, szSectionCfg))
		return FALSE;

	// ��ȡ���������ļ�
	CreateObj("SectionContainer", "VarContainer");
	TObjRef<VarContainer> pVar = "SectionContainer";
	pVar->Load("VirtualFileSys", tszFile);

	// ���ȳ�ʼ��login
	DWORD	dwLoginID	=	g_SectionMgr.CreateNewClientID();
	LPCTSTR	szIP		=	pVar->GetString(_T("ip"), _T("login"));
	m_pLoginServer = new GameServerLogin(dwLoginID, IUTIL->UnicodeToUnicode8(szIP));

	// ���������е�world
	INT nWorldNum = pVar->GetInt(_T("num"), _T("world"));

	for(INT n = 0; n < nWorldNum; ++n)
	{
		GameWorld* pWorld = new GameWorld;
		if( !pWorld->Init(n) )
		{
			SAFE_DEL(pWorld);
			return FALSE;
		}

		// ���뵽�б���
		m_mapAllWorld.Add(pWorld->GetID(), pWorld);
	}

	// ����Login��ID�Ĵ���ע��
	g_SectionMgr.RegisterClientID(dwLoginID, m_dwSectionID);

	// ע��World
	GameWorld* pWorld = NULL;
	TMap<DWORD, GameWorld*>::TMapIterator it = m_mapAllWorld.Begin();
	while( m_mapAllWorld.PeekNext(it, pWorld) )
	{
		// ע��ID����Ϸ����Ķ�Ӧ
		m_mapIDWorldID.Add(pWorld->GetWorld()->GetID(), pWorld->GetID());
		m_mapIDWorldID.Add(pWorld->GetDB()->GetID(), pWorld->GetID());

		// ע��ID�ʹ����Ķ�Ӧ
		g_SectionMgr.RegisterClientID(pWorld->GetWorld()->GetID(), m_dwSectionID);
		g_SectionMgr.RegisterClientID(pWorld->GetDB()->GetID(), m_dwSectionID);
	}

	// ɾ������������
	KillObj("SectionContainer");

	return TRUE;
}

//-----------------------------------------------------------------
// ����
//-----------------------------------------------------------------
VOID Section::Destroy()
{
	// ��ɾ��Login
	SAFE_DEL(m_pLoginServer);

	// ��ɾ������ĸ���World
	GameWorld* pWorld = NULL;
	TMap<DWORD, GameWorld*>::TMapIterator it = m_mapAllWorld.Begin();
	while( m_mapAllWorld.PeekNext(it, pWorld) )
	{
		SAFE_DEL(pWorld);
	}
	m_mapAllWorld.Clear();
}

//-----------------------------------------------------------------
// ����  
//-----------------------------------------------------------------
VOID Section::Update()
{
	// �ȸ���login
	m_pLoginServer->Update();

	// �ٸ������е�world
	GameWorld* pWorld = NULL;
	TMap<DWORD, GameWorld*>::TMapIterator it = m_mapAllWorld.Begin();

	while( m_mapAllWorld.PeekNext(it, pWorld) )
	{
		pWorld->Update();
	}
}

//------------------------------------------------------------------
// ����Login
//------------------------------------------------------------------
BOOL Section::StartLoginServer()
{
	if( m_pLoginServer->IsValid() ) return TRUE;
	return m_pLoginServer->StartServer();
}

//------------------------------------------------------------------
// ����World
//------------------------------------------------------------------
BOOL Section::StartWorld(DWORD dwWorldID, EServerType eType)
{
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return FALSE;

	BOOL bRet = TRUE;
	switch (eType)
	{
	case EST_DB:
		if( !pWorld->GetDB()->IsValid() )
		{
			bRet = pWorld->GetDB()->StartServer();
		}
		break;

	case EST_World:
		if( !pWorld->GetWorld()->IsValid() )
		{
			bRet = pWorld->GetWorld()->StartServer();
		}
		break;
	}

	return bRet;
}

//------------------------------------------------------------------
// �ر�login
//------------------------------------------------------------------
BOOL Section::CloseLoginServer(BOOL bForce/* =FALSE */)
{
	if( !m_pLoginServer->IsValid() ) return TRUE;

	// ����ǿ�ƹرգ����login��������Ϣ
	if( !bForce )
	{
		// ͨ��g_sectionMgr������Ϣ
		tagNSS_Close send;
		g_SectionMgr.SendMsg(m_pLoginServer->GetID(), &send, sizeof(tagNSS_Close));
		return TRUE;
	}
	// ǿ�ƹرգ���֪ͨ�����ػ�����ɱ������
	else
	{
		return m_pLoginServer->ShutdownServer();
	}
}

//-------------------------------------------------------------------
// �ر�World
//-------------------------------------------------------------------
BOOL Section::CloseWorld(DWORD dwWorldID, EServerType eType, BOOL bForce/* =FALSE */)
{
	// �ҵ�World
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return FALSE;

	// ����ǿ�ƹر�
	if( !bForce )
	{
		// ͨ��g_SectionMgr������Ϣ
		tagNSS_Close send;
		switch (eType)
		{
		case EST_DB:
			g_SectionMgr.SendMsg((pWorld->GetDB()->GetID()), &send, sizeof(tagNSS_Close));
			break;

		case EST_World:
			g_SectionMgr.SendMsg((pWorld->GetWorld()->GetID()), &send, sizeof(tagNSS_Close));
			break;
		}
		
		return TRUE;
	}
	else
	{
		BOOL bRet = TRUE;

		switch (eType)
		{
		case EST_DB:
			if( pWorld->GetDB()->IsValid() )
			{
				bRet = pWorld->GetDB()->ShutdownServer();
			}
			break;

		case EST_World:
			if( pWorld->GetWorld()->IsValid() )
			{
				bRet = pWorld->GetWorld()->ShutdownServer();
			}
			break;
		}

		return bRet;
	}
}

//-------------------------------------------------------------------
// ��loginserver����ϵͳָ��
//-------------------------------------------------------------------
BOOL Section::SendSystemCmd2LoginServer(tagNetCmd* pMsg)
{
	g_SectionMgr.SendMsg(m_pLoginServer->GetID(), pMsg, sizeof(tagNLSS_SystemCmd));
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��world����ϵͳָ��
//-------------------------------------------------------------------
BOOL Section::SendSystemCmd2World(DWORD dwWorldID, EServerType eType, tagNetCmd* pMsg)
{
	// �ҵ�World
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return FALSE;

	if (eType == EST_DB)
	{
		pWorld->GetDB()->ExecuteCmd(pMsg);
	}
	else if (eType == EST_World)
	{
		pWorld->GetWorld()->ExecuteCmd(pMsg);
	}
	else
		return FALSE;

	return TRUE;
}

BOOL Section::SetMaxNum(DWORD dwWorldID, LPVOID pMsg)
{
	// �ҵ�World
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return FALSE;

	g_SectionMgr.SendMsg(pWorld->GetWorld()->GetID(), pMsg, sizeof(tagNSS_MaxNum));

	return TRUE;
}

BOOL Section::UpdateMall(DWORD dwWorldID, LPVOID pMsg)
{
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return FALSE;

	g_SectionMgr.SendMsg(pWorld->GetWorld()->GetID(), pMsg, sizeof(tagNSS_UpdateMall));

	return TRUE;
}

BOOL Section::AccountForbid(LPVOID pMsg, DWORD dwSize)
{
	GameServerLogin* pLoginServer = GetLoginServer();
	if( !P_VALID(pLoginServer) ) return FALSE;

	g_SectionMgr.SendMsg(pLoginServer->GetID(), pMsg, dwSize);

	return TRUE;
}

BOOL Section::IPBlacklist(LPVOID pMsg)
{
	GameServerLogin* pLoginServer = GetLoginServer();
	if( !P_VALID(pLoginServer) ) return FALSE;

	g_SectionMgr.SendMsg(pLoginServer->GetID(), pMsg, sizeof(tagNSS_ForbidIP));
	return TRUE;
}
//---------------------------------------------------------------------------
// ���ɸô����ľ�̬��Ϣ
//---------------------------------------------------------------------------
VOID Section::GenerateSectionStaticInfo(LPBYTE &pBuff, DWORD& dwSize)
{
	// ��̬��Ϣ����������login��ip��world������ÿ��world����Ϣ

	// ���ȼ��ش�����
	memcpy(pBuff, m_szSectionName, X_SHORT_NAME*sizeof(TCHAR));
	pBuff	+=	X_SHORT_NAME*sizeof(TCHAR);
	dwSize	+=	X_SHORT_NAME*sizeof(TCHAR);

	// ���ش�����ID
	*(DWORD*)pBuff = m_dwSectionID;
	pBuff	+=	sizeof(DWORD);
	dwSize	+=	sizeof(DWORD);

	// �ټ���login��ip
	strncpy((LPSTR)pBuff, m_pLoginServer->GetIP(), X_SHORT_NAME);
	pBuff	+=	X_SHORT_NAME*sizeof(CHAR);
	dwSize	+=	X_SHORT_NAME*sizeof(CHAR);

	// world������
	*(DWORD*)pBuff = GetWorldNum();
	pBuff	+=	sizeof(DWORD);
	dwSize	+=	sizeof(DWORD);

	// ���ظ���world
	GameWorld* pWorld = NULL;
	TMap<DWORD, GameWorld*>::TMapIterator it = m_mapAllWorld.Begin();

	while( m_mapAllWorld.PeekNext(it, pWorld) )
	{
		pWorld->GenerateWorldStaticInfo(pBuff, dwSize);
	}
}

//---------------------------------------------------------------------------
// ���ɸô����Ķ�̬��Ϣ
//---------------------------------------------------------------------------
VOID Section::GenerateSectionDynamicInfo(LPBYTE &pBuff, DWORD& dwSize)
{
	// ��̬��Ϣ������id��login��״̬��world������ÿ��world����Ϣ

	// ���ȼ��ش���id
	*(DWORD*)pBuff = m_dwSectionID;
	pBuff	+=	sizeof(DWORD);
	dwSize	+=	sizeof(DWORD);

	// �ټ���login��״̬
	tagServerInfo* pInfoLogin = (tagServerInfo*)pBuff;
	m_pLoginServer->FillServerInfo(pInfoLogin);
	pBuff	+=	sizeof(tagServerInfo);
	dwSize	+=	sizeof(tagServerInfo);

	// world������
	*(DWORD*)pBuff = GetWorldNum();
	pBuff	+=	sizeof(DWORD);
	dwSize	+=	sizeof(DWORD);

	// ���ظ���world
	GameWorld* pWorld = NULL;
	TMap<DWORD, GameWorld*>::TMapIterator it = m_mapAllWorld.Begin();

	while( m_mapAllWorld.PeekNext(it, pWorld) )
	{
		pWorld->GenerateWorldDynamicInfo(pBuff, dwSize);
	}
}

//---------------------------------------------------------------------------
// �õ�LoongWorld
//---------------------------------------------------------------------------
GameServerWorld* Section::GetWorldServer(DWORD dwWorldID)
{
	GameWorld* pWorld = GetWorld(dwWorldID);

	if( !P_VALID(pWorld) ) return NULL;

	return pWorld->GetWorld();
}

//----------------------------------------------------------------------------
// �õ�LoongDB
//----------------------------------------------------------------------------
GameServerDB* Section::GetDBServer(DWORD dwWorldID)
{
	GameWorld* pWorld = GetWorld(dwWorldID);

	if( !P_VALID(pWorld) ) return NULL;

	return pWorld->GetDB();
}

//----------------------------------------------------------------------------
// ����Login
//----------------------------------------------------------------------------
GameServerLogin* Section::FindLoginServer(DWORD dwID)
{
	if( m_pLoginServer->GetID() != dwID )
	{
		return NULL;
	}

	return m_pLoginServer;
}

//-----------------------------------------------------------------------------
// ����World
//-----------------------------------------------------------------------------
GameServerWorld* Section::FindWorldServer(DWORD dwID)
{
	// �Ȳ鿴���ID�������ĸ���Ϸ�����
	DWORD dwWorldID = m_mapIDWorldID.Peek(dwID);
	if( !P_VALID(dwWorldID) ) return NULL;

	// �ҵ�world
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return NULL;

	// �����ǲ���LoongWorld��ID
	if( pWorld->GetWorld()->GetID() != dwID )
	{
		return NULL;
	}

	return pWorld->GetWorld();
}

//------------------------------------------------------------------------------
// ����DB
//------------------------------------------------------------------------------
GameServerDB* Section::FindDBServer(DWORD dwID)
{
	// �Ȳ鿴���ID�������ĸ���Ϸ�����
	DWORD dwWorldID = m_mapIDWorldID.Peek(dwID);
	if( !P_VALID(dwWorldID) ) return NULL;

	// �ҵ�world
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return NULL;

	// �����ǲ���LoongDB��ID
	if( pWorld->GetDB()->GetID() != dwID )
	{
		return NULL;
	}

	// ����LoongDBָ��
	return pWorld->GetDB();
}

GameWorld* Section::FindGameWorld(DWORD dwID)
{
	DWORD dwWorldID = m_mapIDWorldID.Peek(dwID);
	if (!GT_VALID(dwWorldID))
	{
		return NULL;
	}
	return m_mapAllWorld.Peek(dwWorldID);
}

//-------------------------------------------------------------------------------
// ����ID����Server
//-------------------------------------------------------------------------------
GameServer* Section::FindServer(DWORD dwID)
{
	// ���Ȳ����ǲ���Login
	if( m_pLoginServer->GetID() == dwID )
		return m_pLoginServer;

	// ����login�������ʱ�ĸ���Ϸ�����
	DWORD dwWorldID = m_mapIDWorldID.Peek(dwID);
	if( !P_VALID(dwWorldID) ) return NULL;

	// �ҵ�world
	GameWorld* pWorld = GetWorld(dwWorldID);
	if( !P_VALID(pWorld) ) return NULL;

	// �鿴�ǲ���LoongWorld��ID
	if( pWorld->GetWorld()->GetID() == dwID )
	{
		return pWorld->GetWorld();
	}
	// �鿴�ǲ���LoongDB��ID
	else if( pWorld->GetDB()->GetID() == dwID )
	{
		return pWorld->GetDB();
	}
	// ��û�в��ҵ�������NULL
	else
	{
		return NULL;
	}
}

BOOL Section::ForAllGameWorld( DWORD* pWorldIDs, INT nNum, FUN_GAMEWORLD pFun,  PVOID pPara)
{
	BOOL bRtv = TRUE;
	if (!GT_VALID(*pWorldIDs))
	{
		GameWorld* pWorld = NULL;
		TMap<DWORD, GameWorld*>::TMapIterator itr = m_mapAllWorld.Begin();
		while( m_mapAllWorld.PeekNext(itr, pWorld) )
		{
			if (P_VALID(pWorld))
			{
				(pWorld->*pFun)((DWORD)pPara);
			}
			else
			{
				bRtv = FALSE;
			}
		}
	}
	else
	{
		GameWorld* pWorld = NULL;
		for (INT i=0; i<nNum; ++i)
		{
			pWorld = m_mapAllWorld.Peek(pWorldIDs[i]);
			if (P_VALID(pWorld))
			{
				(pWorld->*pFun)((DWORD)pPara);
			}
			else
			{
				bRtv = FALSE;
			}
		}
	}
	return bRtv;
}