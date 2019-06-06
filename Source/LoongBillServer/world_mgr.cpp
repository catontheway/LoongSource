//-----------------------------------------------------------------------------
//!\file world_mgr.cpp
//!\author Aslan
//!
//!\date 2009-06-11
//! last 2009-06-11
//!
//!\brief ��Ϸ���������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_bill_s.h"
#include "../ServerDefine/bill_define.h"

#include "world.h"
#include "world_mgr.h"
#include "bill_server.h"

//------------------------------------------------------------------------------
// ȫ�ֱ�������
//------------------------------------------------------------------------------
WorldMgr g_WorldMgr;

//------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------
BOOL WorldMgr::Init()
{
	m_pUtil = "Util";

	// ��������world
	TObjRef<VarContainer> pVar = "BillConfig";

	// �����������������
	CreateObj("NetCmdMgrWorld", "NetCmdMgr");
	m_pCmdMgr = "NetCmdMgrWorld";
	if( !P_VALID(m_pCmdMgr) ) return FALSE;

	// ���ɷ���������ӿ�
	CreateObj("StreamServerWorld", "StreamServer");
	m_pTrans = "StreamServerWorld";
	if( !P_VALID(m_pTrans) ) return FALSE;

	// ע����������
	RegisterWorldCmd();

	// ��������world
	INT nWorldNum = pVar->GetInt(_T("num world"));
	for(INT n = 0; n < nWorldNum; ++n)
	{
		World* pWorld = new World;
		if( !pWorld->Init(n) ) return FALSE;

		m_mapWorld.Add(pWorld->GetID(), pWorld);
	}

	// ��ʼ������
	m_nPort = pVar->GetInt(_T("port"), _T("server"));
	m_pTrans->Init((TCPLOGINCALLBACK)m_Trunk.sfp2(&WorldMgr::LoginCallBack),
					(TCPLOGOUTCALLBACK)m_Trunk.sfp1(&WorldMgr::LogoutCallBack),
					m_nPort);

	return TRUE;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID WorldMgr::Destroy()
{
	// ɾ�����е���Ϸ����
	World* pWorld = NULL;
	m_mapWorld.ResetIterator();

	while( m_mapWorld.PeekNext(pWorld) )
	{
		SAFE_DEL(pWorld);
	}
	m_mapWorld.Clear();

	// ɾ������
	if( P_VALID(m_pTrans) )	
	{
		m_pTrans->Destroy();
		KillObj("StreamServerWorld");
	}

	// ע�����е���������
	UnregisterWorldCmd();
	KillObj("NetCmdMgrWorld");
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID WorldMgr::Update()
{

}

//-------------------------------------------------------------------------------
// ע����Ϸ�����������Ϣ
//-------------------------------------------------------------------------------
VOID WorldMgr::RegisterWorldCmd()
{
	m_pCmdMgr->Register("NWB_Login", (NETMSGPROC)m_Trunk.sfp2(&WorldMgr::HandleWorldLogin), _T("world login"));
}

//-------------------------------------------------------------------------------
// ע����Ϸ�����������Ϣ
//-------------------------------------------------------------------------------
VOID WorldMgr::UnregisterWorldCmd()
{
	m_pCmdMgr->Destroy();
}

//--------------------------------------------------------------------------------
// ��½����
//--------------------------------------------------------------------------------
DWORD WorldMgr::LoginCallBack(LPVOID p1, LPVOID p2)
{
	static DWORD dwFirstMsgCrc = m_pUtil->Crc32("NWB_Login");

	// �鿴��Ϣ
	tagNWB_Login* pRecv = (tagNWB_Login*)p1;
	if( pRecv->dwID != dwFirstMsgCrc ) return GT_INVALID;

	// �������û�г�ʼ����ϻ������ڹرգ��򷵻�
	if( !g_Server.IsInitOK() || g_Server.IsShutingDown() ) return GT_INVALID;

	// �ҵ���Ӧ��world
	World* pWorld = GetWorld(pRecv->dwWorldID);
	if( !P_VALID(pWorld) ) return GT_INVALID;

	// �鿴world�ǲ����Ѿ���������
	if( pWorld->IsValid() ) return GT_INVALID;

	// world����
	pWorld->LoongWorldLogin();
	return pWorld->GetID();
}

//---------------------------------------------------------------------------------
// �ǳ�����
//---------------------------------------------------------------------------------
DWORD WorldMgr::LogoutCallBack(LPVOID p)
{
	DWORD dwWorldID = (DWORD)p;

	// �ҵ�world
	World* pWorld = GetWorld(dwWorldID);

	if( P_VALID(pWorld) )
	{
		pWorld->LoongWorldLogout();
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ��Ϣ����������
//---------------------------------------------------------------------------------
DWORD WorldMgr::HandleWorldLogin(tagNetCmd* pCmd, World* pWorld)
{
	tagNBW_Login send;
	send.dwErrorCode = 0;

	SendMsg(pWorld->GetID(), &send, send.dwSize);

	return 0;
}

//---------------------------------------------------------------------------------
// �õ�����ʱ��Ϣ
//---------------------------------------------------------------------------------
VOID WorldMgr::DisPlayerRunTimeInfo(TObjRef<Console>& pConsole)
{
	World* pWorld = NULL;
	TMap<DWORD, World*>::TMapIterator it = m_mapWorld.Begin();

	tstring strWorldConn;
	tstring strWorldDBConn;
	tstring strWorldYBSuccTimes;
	tstring strWorldItemSuccTimes;

	while( m_mapWorld.PeekNext(it, pWorld) )
	{
		TCHAR szTemp[X_SHORT_NAME] = {_T('\0')};
		_stprintf(szTemp, _T("%u_"), pWorld->GetID());

		// world��Ϣ
		strWorldConn.clear();
		strWorldDBConn.clear();
		strWorldYBSuccTimes.clear();
		strWorldItemSuccTimes.clear();

		strWorldConn			+=	szTemp;
		strWorldConn			+=	_T("wc");
		strWorldDBConn			+=	szTemp;
		strWorldDBConn			+=	_T("dbc");
		strWorldYBSuccTimes		+=	szTemp;
		strWorldYBSuccTimes		+=	_T("ybst");
		strWorldItemSuccTimes	+=	szTemp;
		strWorldItemSuccTimes	+=	_T("itemst");

		pConsole->Watch(strWorldConn.c_str(),			pWorld->IsValid());
		pConsole->Watch(strWorldDBConn.c_str(),			!pWorld->IsDBLost());
		pConsole->Watch(strWorldYBSuccTimes.c_str(),	pWorld->GetBillYuanbaoSuccessTimes());
		pConsole->Watch(strWorldItemSuccTimes.c_str(),	pWorld->GetBillItemSuccessTimes());
	}
}