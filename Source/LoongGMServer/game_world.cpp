//-----------------------------------------------------------------------------
//!\file game_world.cpp
//!\brief ��Ϸ����
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "game_world.h"
#include "game_server.h"
#include "server.h"
#include "section_mgr.h"
#include "server_db.h"


//-----------------------------------------------------------------------------
// CONSTRUCT
//-----------------------------------------------------------------------------
GameWorld::GameWorld() : m_dwWorldID(GT_INVALID), m_pWorldServer(NULL), m_pDBServer(NULL)
{

}

GameWorld::~GameWorld()
{
	SAFE_DEL(m_pWorldServer);
	SAFE_DEL(m_pDBServer);
}

//------------------------------------------------------------------------------
// ��ʼ����nIndex������Ϸ��������к�
//------------------------------------------------------------------------------
BOOL GameWorld::Init(INT nIndex)
{
	// �õ�world����������
	TCHAR szWorld[X_SHORT_NAME] = {_T('\0')};
	_stprintf(szWorld, _T("world_%d"), nIndex);

	TObjRef<VarContainer> pVar = "SectionContainer";

	// �õ���Ϸ��������ƺ�ID
	LPCTSTR szWorldName = pVar->GetString(_T("name"), szWorld);
	memcpy(m_szWorldName, szWorldName, sizeof(m_szWorldName));
	m_dwWorldID = pVar->GetDword(_T("id"), szWorld);

	// �������ݿ����
	if (!InitDB(szWorld))
	{
		return FALSE;
	}
	
	// ����world��db����
	LPCTSTR szWorldIP = pVar->GetString(_T("loong_world_ip"), szWorld);
	LPCTSTR szDBIP = pVar->GetString(_T("loong_db_ip"), szWorld);

	DWORD dwLoongWorldID = g_SectionMgr.CreateNewClientID();
	m_pWorldServer = new GameServerWorld(dwLoongWorldID, IUTIL->UnicodeToUnicode8(szWorldIP));

	DWORD dwLoongDBID = g_SectionMgr.CreateNewClientID();
	m_pDBServer = new GameServerDB(dwLoongDBID, IUTIL->UnicodeToUnicode8(szDBIP));

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
VOID GameWorld::Update()
{
	m_pWorldServer->Update();
	m_pDBServer->Update();
}

//------------------------------------------------------------------------------
// ���ɸ�World�ľ�̬��Ϣ
//------------------------------------------------------------------------------
VOID GameWorld::GenerateWorldStaticInfo(LPBYTE &pBuff, DWORD& dwSize)
{
	// ��̬��Ϣ������world�����֣�loongworld��ip��loongdb��ip
	LPBYTE pOriBuff = pBuff;

	// ��������world������
	memcpy(pBuff, m_szWorldName, X_SHORT_NAME*sizeof(TCHAR));
	pBuff += X_SHORT_NAME*sizeof(TCHAR);

	// ������world��ID
	*(DWORD*)pBuff = m_dwWorldID;
	pBuff += sizeof(DWORD);

	// ������loongworld��ip
	strncpy((LPSTR)pBuff, GetWorld()->GetIP(), X_SHORT_NAME);
	pBuff += X_SHORT_NAME*sizeof(CHAR);

	// ������loongdb��ip
	strncpy((LPSTR)pBuff, GetDB()->GetIP(), X_SHORT_NAME);
	pBuff += X_SHORT_NAME*sizeof(CHAR);

	// ����size
	dwSize += pBuff - pOriBuff;
}

//-------------------------------------------------------------------------------
// ���ɸ�world�Ķ�̬��Ϣ
//-------------------------------------------------------------------------------
VOID GameWorld::GenerateWorldDynamicInfo(LPBYTE &pBuff, DWORD& dwSize)
{
	// ��̬��Ϣ������world��id��loongworld�Ķ�̬��Ϣ��loongdb�Ķ�̬��Ϣ
	LPBYTE pOriBuff = pBuff;

	// ����ID
	*(DWORD*)pBuff = m_dwWorldID;
	pBuff += sizeof(DWORD);

	// ����loongworld�Ķ�̬��Ϣ
	tagServerInfo* pInfoWorld = (tagServerInfo*)pBuff;
	GetWorld()->FillServerInfo(pInfoWorld);
	pBuff += sizeof(tagServerInfo);

	// ����loongdb�Ķ�̬��Ϣ
	tagServerInfo* pInfoDB = (tagServerInfo*)pBuff;
	GetDB()->FillServerInfo(pInfoDB);
	pBuff += sizeof(tagServerInfo);

	// �ۻ�size
	dwSize += pBuff - pOriBuff;
}

BOOL GameWorld::InitDB( LPCTSTR szWorld )
{
	TObjRef<VarContainer> pVar = "SectionContainer";
	LPCTSTR szIp	= pVar->GetString(_T("db_host"), szWorld);
	LPCTSTR szDBName= pVar->GetString(_T("db_name"), szWorld);
	LPCTSTR szUser	= pVar->GetString(_T("db_user"), szWorld);
	LPCTSTR szPwd	= pVar->GetString(_T("db_psd"), szWorld);
	INT		nPort	= pVar->GetInt(_T("db_port"), szWorld);
	m_pDB = new ServerDB;
	BOOL bRtv = m_pDB->Init(szIp, szUser, szPwd, szDBName, nPort);
	if (!bRtv)
	{
		SAFE_DEL(m_pDB);
	}
	return bRtv;
}

VOID GameWorld::SendWorldMsg( tagNetCmd* pCmd )
{
	g_SectionMgr.SendMsg(m_pWorldServer->GetID(), pCmd, pCmd->dwSize);
}