//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: db_session.cpp
// author: Sxg
// actor:
// data: 2008-5-19
// last:
// brief: ��Ϸ�����������ݿ�������ĶԻ���
// todo: ��Ϣ��������������Ϣ����ȫ��
//-----------------------------------------------------------------------------
#include "Stdafx.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_yuanbao.h"
#include "../ServerDefine/msg_famehall.h"
#include "../ServerDefine/msg_guild.h"
#include "../WorldDefine/selectrole.h"
#include "../WorldDefine/msg_mall.h"
#include "../ServerDefine/msg_pet.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_vcard.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/msg_vip_netbar.h"
#include "../ServerDefine/msg_strength.h"
#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/msg_role_vcard.h"
#include "../ServerDefine/activity_define.h"
#include "Strength.h"
#include "db_session.h"
#include "world.h"
#include "world_session.h"
#include "role.h"
#include "role_mgr.h"
#include "att_res.h"
#include "item_creator.h"
#include "TradeYuanBao.h"
#include "famehall.h"
#include "guild.h"
#include "guild_mgr.h"
#include "pet_pocket.h"
#include "pet_soul.h"
#include "mall.h"
#include "vip_stall.h"
#include "activity_mgr.h"
#include "vip_netbar.h"
#include "chat_mgr.h"

// Jason v1.3.1 external links
#include "../ServerDefine/msg_sexternal_links.h"

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
DBSession g_dbSession;

DBSession::DBSession() : m_Trunk(this),m_dwInitTime(0),
m_timLastPullExtLink(0)
{
	m_dwDBPort	= GT_INVALID;
	m_dwGoldenCode	= GT_INVALID;
	m_bTermConnect	= FALSE;

	m_bInitOK = FALSE;

	ZeroMemory(m_szDBIP, sizeof(m_szDBIP));
	//ZeroMemory(m_szServerName, sizeof(m_szServerName));
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DBSession::~DBSession()
{
}

//-----------------------------------------------------------------------------
// ��ʼ������
//-----------------------------------------------------------------------------
BOOL DBSession::Init()
{
	// �ò��ֳ�Ա���¸�ֵ����Ϊ���������һ��ȫ�ֱ�����
	m_pUtil = "Util";
	m_pThread = "Thread";

	// ����NetCmdMgr
	CreateObj("LoongDBNetCmdMgr", "NetCmdMgr");
	m_pMsgCmdMgr = "LoongDBNetCmdMgr";

	// �������Ӷ��󣬲���ʼ��
	CreateObj("ToLoongDB", "StreamTransport");
	m_pTran = "ToLoongDB";
	if(!P_VALID(m_pTran))
	{
		ERR(_T("Create ToLoonDB(StreamTransport) obj failed!"));
		return FALSE;
	}
	m_pTran->Init();

	// ��ȡ�ļ�, ��ʼ����Ա
	if(!InitConfig())
	{
		ERR(_T("Init File Read Failed! Please Check......"));
		return FALSE;
	}

	// ע��������������
	RegisterAllDBCommand();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ConnectLoongDB"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&DBSession::ThreadConnectDB), 

		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ConnectLoongDB")))
	{
		continue;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ȡ�ļ�, ��ʼ����Ա
//-----------------------------------------------------------------------------
BOOL DBSession::InitConfig()
{
	//CreateObj("LoongDBVar", "VarContainer");
	TObjRef<VarContainer> pVar = "VarContainer";
	if(!P_VALID(pVar))
	{
		ERR(_T("Create LoongDBVar(VarContainer) obj failed!"));
		return FALSE;
	}

	// ��ȡ���ݿ�������Ķ˿ںźͽ�ɫ����
	m_dwDBPort	= pVar->GetDword(_T("port db_server"));
	m_dwGoldenCode	= pVar->GetDword(_T("golden_code db_server"));

	// ��ȡ������Ϸ����Ļ�����
	/*CHAR szName[X_LONG_STRING];
	strncpy(szName, m_pTran->GetHostName(), X_LONG_STRING);*/
	//_tcsncpy(m_szServerName, m_pUtil->Unicode8ToUnicode(szName), MCalTCharArrayNum(m_szServerName) - 1);

	// ��ȡ���ݷ�����IP��ַ
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip db_server")), MCalTCharArrayNum(szIP) - 1);
	IFASTCODE->MemCpy(m_szDBIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szDBIP) - 1);

	//KillObj("LoongDBVar");

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID DBSession::Destroy()
{
	// �ȴ������߳̽���
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectLoongDB"), INFINITE);

	m_pTran->Destroy();

	KillObj("ToLoongDB");

	// ���ע�����Ϣ
	m_pMsgCmdMgr->Destroy();
	KillObj("LoongDBNetCmdMgr");
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID DBSession::Update()
{
	if( NULL == m_pTran )
		return;

	if(!m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectLoongDB")))
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectLoongDB"), INFINITE);

		// �����������ݿ�����������߳�
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectLoongDB"), (vEngine::THREADPROC)m_Trunk.sfp0(&DBSession::ThreadConnectDB), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectLoongDB")))
		{
			continue;
		}

		return;
	}

	// �����������ݿ����������Ϣ
	while(m_pTran->IsConnected())
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = m_pTran->Recv(dwSize);

		if( !P_VALID(pRecv) )
		{
			UpdateExternalLinksList();
			return;
		}

		// ������Ϣ
		m_pMsgCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);

		// ������Դ
		m_pTran->FreeRecved(pRecv);
	}

	// Jason v1.3.1 external links,�����Ӹ���һ��
	//UpdateExternalLinksList();
}

//-----------------------------------------------------------------------------
// �����߳�(�������ݿ������)
//-----------------------------------------------------------------------------
DWORD DBSession::ThreadConnectDB()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while(FALSE == m_bTermConnect)
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_szDBIP, m_dwDBPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to LoongDB Server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szDBIP), m_dwDBPort);

		// ���͵�½��֤��Ϣ����Ϸ��������ֺ�������Ϸ����Ļ�����
		tagN_Certification	sendCheck;
		sendCheck.dwGoldenCode = g_world.GetGoldenCode();
		_tcsncpy(sendCheck.szWorldName, g_world.GetWorldName(),X_SHORT_NAME - 1);
		//_tcsncpy(sendCheck.szServerName, m_szServerName, X_SHORT_NAME - 1);

		m_pTran->Send(&sendCheck, sendCheck.dwSize);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}


//----------------------------------------------------------------------------------------------
// ע�����е���������
//----------------------------------------------------------------------------------------------
VOID DBSession::RegisterAllDBCommand()
{
	m_pMsgCmdMgr->Register("Heartbeat",				(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleHeartBeat),			_T("Heart Beat"));

	m_pMsgCmdMgr->Register("N_Certification",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleCertification),		_T("DB Server Certification Reply"));
	m_pMsgCmdMgr->Register("NDBS_LoongWorldInitOK", (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoongWorldInitOK),   _T("LoongWorld Init OK Reply"));
	m_pMsgCmdMgr->Register("NDBS_GetItemInfo",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleItemInfo),			_T("Load Item Max&Min Serial"));
	m_pMsgCmdMgr->Register("NDBS_LoadItemNeedLog",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleItemNeedLog),		_T("Load Item TypeID Which Need Log"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllRoleInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllRoleInfo),	_T("Load All Role Info"));

	m_pMsgCmdMgr->Register("NDBS_LoadSimRole",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleEnum),			_T("Load simple Role"));
	m_pMsgCmdMgr->Register("NDBS_CreateRole",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleCreate),			_T("Create Role"));
	m_pMsgCmdMgr->Register("NDBS_DelRole",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleDelete),			_T("Delete Role"));
	m_pMsgCmdMgr->Register("NDBS_ResumeRole",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleResume),			_T("Resume Role"));
	m_pMsgCmdMgr->Register("NDBS_LoadRole",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleLoad),			_T("Load Role all data"));

	m_pMsgCmdMgr->Register("NDBS_LoadBaiBao",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleBaiBaoLoad),			_T("Load New Item Neet Put Into BaiBao Bag"));
	m_pMsgCmdMgr->Register("NDBS_LoadBaiBaoYuanBao",(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleBaiBaoYuanBaoLoad),	_T("Reload Yuanbao Put Into BaiBao Bag"));
	m_pMsgCmdMgr->Register("NDBS_LoadBaiBaoLog",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleBaiBaoLoadLog),		_T("Load BaiBao History Record"));

	m_pMsgCmdMgr->Register("NDBS_LoadAllYBAccount", (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllYBAccount),	_T("Load All Yuan Bao Trade Account"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllOrder",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllYBOrder),		_T("Load All Yuan Bao Order"));
	m_pMsgCmdMgr->Register("NDBS_GetRoleYBOrder",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleGetYBOrder),		_T("Load Role Yuan Bao Order"));

	m_pMsgCmdMgr->Register("NDBS_GetActTreasureList",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetActTreasureList),		_T("Load Clan Treasure"));
	m_pMsgCmdMgr->Register("NDBS_GetFameHallEnterSnap",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetFameHallEnterSnap),	_T("Load FameHall EnterSnap"));
	m_pMsgCmdMgr->Register("NDBS_GetRepResetTimestamp",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRepRstTimeStamp),		_T("Load Reputation RstTimeSamp"));
	m_pMsgCmdMgr->Register("NDBS_GetRepRankList",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRepRankLoad),		_T("Load Reputation Rank"));

	// �������
	m_pMsgCmdMgr->Register("NDBS_LoadAllGuild",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuild),				_T("load guild"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllGuildMember",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildMember),		_T("load guild member"));
	m_pMsgCmdMgr->Register("NDBS_GuildMemberInitOK",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGuildMemberInitOk),		_T("load guild member ok"));
	m_pMsgCmdMgr->Register("NDBS_LoadGuildWareItems",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildWareItems),		_T("load guild warehouse items"));
	m_pMsgCmdMgr->Register("NDBS_LoadFacilitiesInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildUpgradeInfo),	_T("load guild upgrade info"));
	m_pMsgCmdMgr->Register("NDBS_LoadGuildSkillInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildSkillInfo),		_T("load guild skill info"));
	m_pMsgCmdMgr->Register("NDBS_LoadCommodity",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildCommerceInfo),	_T("load guild commerce info"));
	m_pMsgCmdMgr->Register("NDBS_LoadCommerceRank",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadCommerceRankInfo),	_T("load commerce rank info"));
	m_pMsgCmdMgr->Register("NDBS_GuildInitOK",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGuildInitOK),			_T("load guild skill info"));

	// �������
	m_pMsgCmdMgr->Register("NDBS_CreatePetSoul",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleCreatePetSoul),			_T("create pet soul"));

	// �̳����
	m_pMsgCmdMgr->Register("NDBS_GetAllGPInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllGPInfo),			_T("load all group purchase info"));

	// �������
	m_pMsgCmdMgr->Register("NDBS_GetOffLineVCard",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadRoleVCard),			_T("load db vcard"));

	// VIP̯λ
	m_pMsgCmdMgr->Register("NDBS_GetAllVIPStallInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadVIPStallInfo),		_T("load all vip stall info"));

	// �̶��
	m_pMsgCmdMgr->Register("NDBS_LoadActivityData",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadActivityData),		_T("load activity data"));

	// VIP̯λ
	m_pMsgCmdMgr->Register("NDBS_GetVNBData",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadVNBData),			_T("load vip netbar data"));

	// ʵ�����а�
	m_pMsgCmdMgr->Register("NDBS_StrengthRankings",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleInitStrengthRankings),	_T("load strength rankings"));
	m_pMsgCmdMgr->Register("NDBS_UpdateRankings",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRetUpdateRankings),		_T("update strength rankings"));

	// ��ȡ����
	m_pMsgCmdMgr->Register("NDBS_LoadLeftMsg",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadLeftMsg),			_T("load left msg"));

	//m_pMsgCmdMgr->Register("",				(NETMSGPROC)m_Trunk.sfp2(&DBSession::),			_T(""));

	// Jason v1.3.1 �ⲿ����
	m_pMsgCmdMgr->Register("NDBS_GetExternalLinks",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleExternalLinksList),			_T("external links msg"));
}

//-------------------------------------------------------------------------------------------------
// ��֤��Ϣ
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleCertification(tagNetCmd* pMsg, DWORD)
{
	tagN_Certification* pRecv = (tagN_Certification*)pMsg;

	/*if( pRecv->dwGoldenCode == m_dwGoldenCode
		&& 0 == _tcsncmp(pRecv->szServerName, m_szServerName, MCalTCharArrayNum(pRecv->szServerName))
		&& 0 == _tcsncmp(pRecv->szWorldName, g_world.GetWorldName(), MCalTCharArrayNum(pRecv->szWorldName))
	)
	{*/

	m_dwInitTime =  timeGetTime();

	// ��ʼ����������Ϣ
	g_fameHall.SendLoadDBData();

	// ��ȡ��Ʒ��Ϣ
	tagNDBC_GetItemInfo itemInfo;
	Send(&itemInfo, itemInfo.dwSize);
	
	// ���ͻ�ȡ��Ʒ�Ƿ񱣴���Ϣ
	tagNDBC_LoadItemNeedLog itemNeedLog;
	Send(&itemNeedLog, itemNeedLog.dwSize);	
	
	// ���ͻ�ȡ���н�ɫ��Ϣ����Ϣ
	tagNDBC_LoadAllRoleInfo send;
	Send(&send, send.dwSize);

	// ���ͻ���������Ԫ�������˻���Ϣ
	tagNDBC_LoadAllYBAccount YBAccount;
	Send(&YBAccount, YBAccount.dwSize);

	tagNDBC_LoadAllOrder YBOrder;
	Send(&YBOrder, YBOrder.dwSize);

	// ��ȡ���ɼ����ɳ�Ա��Ϣ
	tagNDBC_LoadAllGuild loadGuild;
	Send(&loadGuild, loadGuild.dwSize);

	tagNDBC_LoadAllGuildMember loadGuildMember;
	Send(&loadGuildMember, loadGuildMember.dwSize);

	// ��DB�����ʼ���Ź���Ϣ
	tagNDBC_GetAllGPInfo GPInfo;
	g_dbSession.Send(&GPInfo, GPInfo.dwSize);

	// ��ȡVIP̯λ��Ϣ
	tagNDBC_GetAllVIPStallInfo VIPStallInfo;
	g_dbSession.Send(&VIPStallInfo, VIPStallInfo.dwSize);

	// ��ȡ��ű�����
	tagNDBC_LoadActivityData ActivityData;		 
	g_dbSession.Send(&ActivityData, ActivityData.dwSize);

	// ��ȡ������������
	tagDWORDTime dwToday = GetCurrentDWORDTime();
	dwToday.sec = 0;
	dwToday.min = 0;
	dwToday.hour = 0;
	
	// ����ʵ�����а�
	g_StrengthRankings.Init();

	tagNDBC_GetVNBData VIPNetBar;
	VIPNetBar.dwDate = dwToday;
	g_dbSession.Send(&VIPNetBar, VIPNetBar.dwSize);

	// loongworld��loongDB���͵ĳ�ʼ����ɵ�ȷ����Ϣ����֤���һ�����ͣ�
	tagNDBC_LoongWorldInitOK	sendInitOk;	
	Send(&sendInitOk, sendInitOk.dwSize);
	/*}
	else
	{
		ASSERT(0);	
	}*/

	// Jason 2009-12-29 ��ʼ���ⲿ����
	InitialExternalLinksList();

	return 0;
}

//-------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleHeartBeat(tagNetCmd* pMsg, DWORD)
{
	return 0;
}

//------------------------------------------------------------------------------------------------
// LoongWorld��ʼ����ɺ�LoongDB���ص�ȷ����Ϣ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoongWorldInitOK(tagNetCmd* pMsg, DWORD)
{
	// �Ƿ���Ҫ�ȼ���Ƿ�������Ҫ���ݶ���ʼ���ɹ�//?? ����Ӹ���ʼ��״̬��־λ -- ʵ�ַ�ʽ������
	
	m_dwInitTime = timeGetTime() - m_dwInitTime;
	IMSG(_T("LoongWorld Spend <%d> ms For Wait DB Init!\n"), m_dwInitTime);

	// ��ʼ�����
	InterlockedExchange((LPLONG)&m_bInitOK, TRUE);
	return 0;
}

//-------------------------------------------------------------------------------------------------
// �������ݿ����������������Ϣ
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadAllRoleInfo(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_LoadAllRoleInfo* pRecv = (tagNDBS_LoadAllRoleInfo*)pMsg;

	if( E_Success != pRecv->dwErrorCode )
	{
		return GT_INVALID;
	}
	
	// ɾ�����о�̬��ɫ
	//g_roleMgr.EraseAllRoleInfo();

	// ���õ�ǰ����ɫID
//	g_roleMgr.SetMaxRoleID(pRecv->dwMaxRoleID);

	// ��ʼ���ؽ�ɫ
	tagRoleInfo* pInfo = &pRecv->RoleInfo[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		g_roleMgr.CreateRoleInfo(pInfo);
		pInfo++;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
// �����ɫ�ͻ��˼�����Ϣ
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleItemInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetItemInfo);

	// ������Ʒ����������
	ItemCreator::SetItemSerial(pRecv->n64MaxSerial, pRecv->n64MinSerial);

	return 0;
}

//-------------------------------------------------------------------------------------------------
// �����ɫ�ͻ��˼�����Ϣ
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleItemNeedLog(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadItemNeedLog);

	if( E_Success != pRecv->dwErrorCode )
	{
		return GT_INVALID;
	}

	// ������Դ����������
	g_attRes.ResetItemLog(pRecv->dwTypeID, pRecv->nItemNum);

	return 0;
}


//-------------------------------------------------------------------------------------------------
// �����ɫ�ͻ��˼�����Ϣ
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleEnum(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadSimRole);
	
	// ���ȸ���AccountID�ҵ�session
	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);

	// Ԥ�����dbsession�����ʱ�򲻴���ͻ��˵���Ϣ�������Ͳ������̻߳���������
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleEnuming() ) return GT_INVALID;

	// ��ʼ��
	for(INT i = 0; i < MAX_ROLENUM_ONEACCOUNT; ++i)
	{
		pSession->m_dwRoleID[i] = GT_INVALID;
	}

	pSession->m_n8RoleNum = 0;
	
	if( E_Success == pRecv->dwErrorCode )
	{
		for(INT n = 0; n < pRecv->nNum; n++)
		{
			pSession->m_dwRoleID[n] = pRecv->SimRole[n].dwRoleID;
		}

		// �˺�ͨ����Ϣ
		pSession->m_sAccountCommon = pRecv->sAccountCommon;

		// ��ɫ����
		pSession->m_n8RoleNum = pRecv->nNum;
	}

	pSession->m_bRoleEnuming = false;
	pSession->m_bRoleEnumDone = true;
	pSession->m_bRoleEnumSuccess = (E_Success == pRecv->dwErrorCode ? true : false);

	// ���ظ��ͻ���
	if( E_Success != pRecv->dwErrorCode || pRecv->nNum <= 0 )
	{
		tagNS_EnumRole send;
		send.dwErrorCode = pRecv->dwErrorCode;
		
		send.dwSafeCodeCrc = pRecv->sAccountCommon.sSafeCode.dwSafeCodeCrc;
		send.dwTimeReset = pRecv->sAccountCommon.sSafeCode.dwTimeReset;
		send.nNum = 0;

		send.dwTimeLastLogin = pSession->GetPreLoginTime();
		send.dwIPLast = pSession->GetPreLoginIP();
		send.dwIP = pSession->GetCurLoginIP();

		pSession->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
	else
	{
		DWORD dwSize = sizeof(tagNS_EnumRole) - 1 + sizeof(tagSimRole) *  pRecv->nNum;
		PBYTE pNew = new BYTE[dwSize];
		if( !P_VALID(pNew) ) return GT_INVALID;

		tagNS_EnumRole* pSend = (tagNS_EnumRole*)pNew;

		pSend->dwID = m_pUtil->Crc32("NS_EnumRole");
		pSend->dwSize = dwSize;
		pSend->dwErrorCode = pRecv->dwErrorCode;
		pSend->dwSafeCodeCrc = pRecv->sAccountCommon.sSafeCode.dwSafeCodeCrc;
		pSend->dwTimeReset = pRecv->sAccountCommon.sSafeCode.dwTimeReset;
		pSend->nNum = pRecv->nNum;

		pSend->dwTimeLastLogin = pSession->GetPreLoginTime();
		pSend->dwIPLast = pSession->GetPreLoginIP();
		pSend->dwIP = pSession->GetCurLoginIP();

		IFASTCODE->MemCpy(pSend->bySimpleInfo, pRecv->SimRole, sizeof(tagSimRole) *  pRecv->nNum);

		pSession->SendMessage(pSend, pSend->dwSize);

		SAFE_DEL_ARRAY(pNew);

		return 0;
	}
}

//--------------------------------------------------------------------------------------------------------
// ������ɫ
//--------------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleCreate(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_CreateRole* pRecv = (tagNDBS_CreateRole*)pMsg;

	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleCreating() ) return GT_INVALID;

	// ��������ɹ�
	if( E_Success == pRecv->dwErrorCode )
	{
		ASSERT( P_VALID(pRecv->SimRole.dwRoleID) );

		// ���뵽session�Ľ�ɫ�б���
		pSession->AddRole(pRecv->SimRole.dwRoleID);

		// ����һ���µ��������
		g_roleMgr.CreateRoleInfo(&pRecv->RoleInfo);

	}
	pSession->m_bRoleCreating = false;

	// ���ظ��ͻ���
	if( E_Success != pRecv->dwErrorCode )
	{
		tagNS_CreateRole send;
		send.dwErrorCode = E_CreateRole_NameExist;

		pSession->SendMessage(&send, send.dwSize);

		return GT_INVALID;	
	}

	else
	{
		DWORD dwSize = sizeof(tagNS_CreateRole) - 1 + sizeof(tagSimRole);
		PBYTE pNew = new BYTE[dwSize];
		if( !P_VALID(pNew) ) return GT_INVALID;

		tagNS_CreateRole* pSend = (tagNS_CreateRole*)pNew;

		pSend->dwID = m_pUtil->Crc32("NS_CreateRole");
		pSend->dwSize = dwSize;
		pSend->dwErrorCode = pRecv->dwErrorCode;
		IFASTCODE->MemCpy(pSend->bySimRoleInfo, &pRecv->SimRole, sizeof(tagSimRole));

		pSession->SendMessage(pSend, pSend->dwSize);

		SAFE_DEL_ARRAY(pNew);

		return 0;
	}
}

//---------------------------------------------------------------------------------------------
// ɾ����ɫ
//---------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleDelete(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_DelRole* pRecv = (tagNDBS_DelRole*)pMsg;

	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleDeleting() ) return GT_INVALID;
	
	if( E_Success == pRecv->dwErrorCode )
	{
		// ����ҵĽ�ɫ�б���ɾ��
		pSession->RemoveRole(pRecv->dwRoleID);

		// ɾ��һ���������
		g_roleMgr.DeleteRoleInfo(pRecv->dwRoleID);
	}
	pSession->m_bRoleDeleting = false;

	// ���͸��ͻ���
	tagNS_DeleteRole send;
	send.dwErrorCode = pRecv->dwErrorCode;
	send.dwRoleID = pRecv->dwRoleID;

	pSession->SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------------------------
// ѡ���ɫ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleLoad(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_LoadRole* pRecv = (tagNDBS_LoadRole*)pMsg;

	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleLoading() ) return GT_INVALID;

	// ���ͻ��˷��ͷ�����Ϣ
	tagNS_SelectRole send;
	send.dwErrorCode = pRecv->dwErrorCode;
	send.dwRoleID = pRecv->dwRoleID;
	pSession->SendMessage(&send, send.dwSize);

	if( E_Success == pRecv->dwErrorCode )
	{
		ASSERT( NULL == pSession->GetRole() );
		
		// todo: ���һ��roledata�Ĵ�С���Ϸ���
		
		// ����һ���µ�role
		BOOL bFirst = FALSE;
		Role* pRole = g_roleMgr.CreateRole(pRecv->dwRoleID, &pRecv->RoleData, pSession, bFirst);
		if( NULL == pRole ) return GT_INVALID;
		
		// Todo: ��ɫ���ɺ����������Ҳдһ������

		if( !pSession->FullLogin(pRole, bFirst) )
		{
			g_roleMgr.DeleteRole(pRole->GetID());
		}
		else
		{
			// ����ٱ������п�λ�����������Ƿ���δ�������Ʒ
			if(pRole->GetItemMgr().GetBaiBaoFreeSize() > 0)
			{
				tagNDBC_LoadBaiBao send;
				send.dwAccountID = pRecv->dwAccountID;
				send.n64Serial = GT_INVALID;
				send.nFreeSpaceSize = pRole->GetItemMgr().GetBaiBaoFreeSize();

				g_dbSession.Send(&send, send.dwSize);
			}
		}
	}

	return 0;
}

//------------------------------------------------------------------------------------------------
// ��ٱ��������µ���Ʒ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleBaiBaoLoad(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadBaiBao);

	if(pRecv->dwErrorCode != E_Success || 0 == pRecv->nRetNum)
	{
		return pRecv->dwErrorCode;
	}
	
	PlayerSession* pSession = g_worldSession.FindSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	
	Role *pRole = pSession->GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT32 nItemSize		= sizeof(tagItem);
	INT32 nEquipSize	= sizeof(tagEquip);

	tagItem	*pTmpItem	= NULL;
	tagItem	*pNewItem	= NULL;

	tagNDBC_DelBaiBaoItem	sendItem;
	tagNDBC_DelBaiBaoEquip	sendEquip;

	pTmpItem = (tagItem*)pRecv->byData;
	for(INT32 i=0; i<pRecv->nRetNum; ++i)
	{
		if(!MIsEquipment(pTmpItem->dwTypeID))
		{
			pNewItem = ItemCreator::CreateItemByData(pTmpItem);

			pTmpItem = (tagItem*)((BYTE*)pTmpItem + nItemSize);

			// �ȷ���Ϣ,֪ͨDBServer��item_baibao����ɾ����Ʒ�����������崻��Ȳ����ĸ�������
			if(P_VALID(pNewItem->pProtoType))
			{
				sendItem.n64Serial = pNewItem->n64Serial;
				g_dbSession.Send(&sendItem, sendItem.dwSize);

				// ��¼log //??
			}
		}
		else
		{
			pNewItem = ItemCreator::CreateEquipByData(pTmpItem);

			pTmpItem = (tagEquip*)((BYTE*)pTmpItem + nEquipSize);

			// �ȷ���Ϣ,֪ͨDBServer��item_baibao����ɾ����Ʒ�����������崻��Ȳ����ĸ�������
			if(P_VALID(pNewItem->pProtoType))
			{
				sendEquip.n64Serial = pNewItem->n64Serial;
				g_dbSession.Send(&sendEquip, sendEquip.dwSize);

				// ��¼log //??
			}
		}

		if(!P_VALID(pNewItem->pProtoType))
		{
			ASSERT(P_VALID(pNewItem->pProtoType));
			MAttResCaution(_T("item/equip"), _T("typeid"), pTmpItem->dwTypeID);
			IMSG(_T("The item(SerialNum: %lld) hasn't found proto type!"), pTmpItem->n64Serial);			
			
			::Destroy(pNewItem);
			continue;
		}

		DWORD dwErrorCode = pRole->GetItemMgr().Add2BaiBao(pNewItem, ELCID_BaiBao_LoadFromDB, TRUE);
		if(dwErrorCode != E_Success)
		{
			// ��¼ʧ�ܲ���log���Ա��֤ //??

			::Destroy(pNewItem);
		}
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ���°ٱ���Ԫ������
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleBaiBaoYuanBaoLoad(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadBaiBaoYuanBao);

	// �ҵ�ָ���˺ŵ��������
	PlayerSession* pPlayerSession = g_worldSession.FindSession(pRecv->dwAccountID);

	// �˺Ų�����
	if (!P_VALID(pPlayerSession))
		return GT_INVALID;

	// �������Ƿ���Ϸ��
	Role* pRole = pPlayerSession->GetRole();
	if (P_VALID(pRole))
	{
		// ����������Ұٱ�Ԫ��������(����ֻ���������Ԫ������)
		INT32 nCurYuanBao = pRole->GetCurMgr().GetBaiBaoYuanBao();
		ASSERT(pRecv->nBaiBaoYuanBao >= nCurYuanBao);
		pRole->GetCurMgr().IncBaiBaoYuanBao(pRecv->nBaiBaoYuanBao - nCurYuanBao, ELCID_BaiBao_Bill_YuanBao, FALSE);
	}
	// �Ƿ���ѡ�˽���
	else if (P_VALID(g_worldSession.FindGlobalSession(pRecv->dwAccountID)))
	{
		ASSERT(pRecv->nBaiBaoYuanBao >= 0);
		pPlayerSession->SetBaiBaoYB(pRecv->nBaiBaoYuanBao);
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ������������������
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRepRankLoad(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetRepRankList);

	if(pRecv->dwErrorCode != E_Success || 0 == pRecv->nRecNum)
	{
		return pRecv->dwErrorCode;
	}
	g_fameHall.HandleUpdateRepRank(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ��ȡ������ҵ�Ԫ�������˻�
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadAllYBAccount(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadAllYBAccount);

	// ��ʼ���ؽ����˻�
	tagYBAccount* pYBAccount = &pRecv->Account[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		g_tradeYB.CreateTradeAccount(pYBAccount);
		pYBAccount++;
	}

	return 0;
}

//------------------------------------------------------------------------------------------------
// ��ȡ������ҵ�Ԫ�����׶���
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadAllYBOrder(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadAllOrder);

	// ��ʼ���ؽ����˻�
	g_tradeYB.SetMaxOrderIndex(pRecv->dwMaxOrderID + 1);
	tagYuanBaoOrder* pYBOrder = &pRecv->YBOrder[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		g_tradeYB.LoadYOOrder(pYBOrder);
		pYBOrder++;
	}

	return 0;	
}

//------------------------------------------------------------------------------------------------
// ��ȡһ����ҵ�Ԫ�����׶���
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleGetYBOrder(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetRoleYBOrder);

	INT			nNum = pRecv->nNum;
	DWORD		dwSize = sizeof(tagNS_GetYuanbaoOrder);
	tagYuanBaoOrder* pNDBSOrder = &pRecv->YBOrder[0];
		
	Role *pRole = g_roleMgr.GetRolePtrByID(pNDBSOrder->dwRoleID);
	if(!P_VALID(pRole))
		return GT_INVALID;

	if( nNum > 0 )
	{
		dwSize += (nNum-1) * sizeof(tagYuanBaoOrder);
	}

	MCREATE_MSG(pSend, dwSize, NS_GetYuanbaoOrder);

	// ��ʼ���ؽ����˻�
	tagYuanBaoOrder* pNCOrder = &pSend->Orders[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		*pNCOrder = *pNDBSOrder;
		pNDBSOrder++;
		pNCOrder++;
	}
	pSend->nNum = nNum;
	pRole->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}

//------------------------------------------------------------------------------------------------
// ��ȡָ���˻��ٱ�����ʷ��¼
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleBaiBaoLoadLog(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadBaiBaoLog);

	if (pRecv->dwErrorCode != E_Success)
		return GT_INVALID;

	PlayerSession* pSession = g_worldSession.FindSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;

	// ���ͻ��˷��ͷ�����Ϣ
	INT nSize = pRecv->dwSize - sizeof(tagNDBS_LoadBaiBaoLog) + sizeof(tagNS_InitBaiBaoRecord);
	MCREATE_MSG(pSend, nSize, NS_InitBaiBaoRecord);

	pSend->n16Num = pRecv->nLogNum;
	IFASTCODE->MemCpy(pSend->byData, pRecv->byData, pRecv->dwSize - sizeof(tagNDBS_LoadBaiBaoLog) + sizeof(pRecv->byData));

	pSession->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ��������ʱ���ȡ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRepRstTimeStamp(tagNetCmd* pMsg, DWORD)
{
	g_fameHall.HandleInitRepRstTimeStamp(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ��ȡ�����ý������
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleGetFameHallEnterSnap(tagNetCmd* pMsg, DWORD)
{
	g_fameHall.HandleInitFameHallTop50(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ��ȡ�Ѽ��������䱦
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleGetActTreasureList(tagNetCmd* pMsg, DWORD)
{
	g_fameHall.HandleInitActTreasureList(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ����������Է���
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuild(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(p, pMsg, NDBS_LoadAllGuild);

	g_guildMgr.InitDBGuild(&p->sGuildLoad);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ������ɳ�Ա���Է���
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildMember(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(p, pMsg, NDBS_LoadAllGuildMember);

	if(p->nAllGuildNum <= 0)
	{
		if(g_guildMgr.GetGuildNum() > 0)
		{
			ASSERT(0);
			ILOG->Write(_T("Guild members load failed! Please check db!\n"));
			g_world.ShutDown();
			return GT_INVALID;
		}

		return E_Success;
	}
	else
	{
		if(g_guildMgr.GetGuildNum() <= 0)
		{
			ASSERT(0);
			ILOG->Write(_T("Guild load failed! Please check db!\n"));
			g_world.ShutDown();
			return GT_INVALID;
		}
	}

	g_guildMgr.InitDBGuildMember(p->sGuildMemberLoad, p->nAllGuildNum);

	// ���Ͱ��ɳ�ʼ�����������Ϣ
// 	tagNDBC_GuildInitOK send;
// 	Send(&send, send.dwSize);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ������ɲֿ���Ʒ����
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildWareItems(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadGuildWareItems);

	// ����ID��֤
	if (!GT_VALID(pRecv->dwGuildID))
		return GT_INVALID;

	// ��Ʒ��Ϣ����
	if (pRecv->nItemNum < 0)
		return GT_INVALID;

	// ȡ�ð���
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return GT_INVALID;
	}

	pGuild->GetGuildWarehouse().LoadWarehouseItems(pRecv->byData, pRecv->nItemNum);

	if (!pGuild->GetGuildWarehouse().IsInitOK())
	{
		//check guild
		ILOG->Write(_T("Guild warehouses load failed!<guildid:%u>, <itemnum:%u> Please check db!\n"), pRecv->dwGuildID, pRecv->nItemNum);
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ������ɵ���ʩ������Ϣ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildUpgradeInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadFacilitiesInfo);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nInfoNum < 0)
	{
		return GT_INVALID;
	}

	// ȡ�ð���
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildFacilities().LoadFacilitiesInfo(pRecv->sFacilitiesInfo, pRecv->nInfoNum))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildFacilities().IsInitOK())
	{
		//check guild
		ILOG->Write(_T("Guild upgradeinfo load failed!<guildid:%u>, <infonum:%u> Please check db!\n"), pRecv->dwGuildID, pRecv->nInfoNum);
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ������ɼ�����Ϣ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildSkillInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadGuildSkillInfo);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nInfoNum <= 0)
	{
		return GT_INVALID;
	}

	// ȡ�ð���
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildSkill().LoadGuildSkillIInfo(pRecv->sGuildSkillInfo, pRecv->nInfoNum))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildSkill().IsInitOK())
	{
		//check guild
		ILOG->Write(_T("Guild skills load failed!<guildid:%u>, <skillnum:%u> Please check db!\n"), pRecv->dwGuildID, pRecv->nInfoNum);
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// �������������Ϣ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildCommerceInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadCommodity);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nCommodityNum < 0)
	{
		return GT_INVALID;
	}

	// ȡ�ð���
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagGuildCommerceInfo* pInfo = NULL;
	if (pRecv->nCommodityNum > 0)
	{
		pInfo = pRecv->sCommerceInfo;
	}
	if (!pGuild->GetGuildCommerce().LoadCommerceInfo(pInfo, pRecv->nCommodityNum))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildCommerce().IsInitOK())
	{
		//check guild
		ILOG->Write(_T("Guild commerceinfo load failed!<guildid:%u>, <commoditynum:%u> Please check db!\n"), pRecv->dwGuildID, pRecv->nCommodityNum);
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// �����������������Ϣ
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadCommerceRankInfo(vEngine::tagNetCmd *pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadCommerceRank);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nRankNum < 0)
	{
		return GT_INVALID;
	}

	// ȡ�ð���
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagCommerceRank* pInfo = NULL;
	if (pRecv->nRankNum > 0)
	{
		pInfo = pRecv->sRankInfo;
	}
	if (!pGuild->GetGuildCommerce().LoadCommerceRankInfo(pInfo, pRecv->nRankNum))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ������ɳ�ʼ�����
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleGuildInitOK(tagNetCmd* pMsg, DWORD)
{
	if (!g_guildMgr.IsGuildInitOK())
	{
		// �رշ�����
		ASSERT(0);
		ILOG->Write(_T("Guild init failed! Please check db!\n"));
		g_world.ShutDown();
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleCreatePetSoul( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pRecv, pMsg, NDBS_CreatePetSoul);

	MTRANS_ELSE_RET(pRole, g_roleMgr.GetRolePtrByID(pRecv->petDataLoad.petAtt.dwMasterID), Role, GT_INVALID);

	DWORD dwRtv = E_Success;

	if (E_Success != pRecv->dwErrCode)
	{
		dwRtv = E_Pets_Soul_CreateDBErr;
		goto EXIT;
	}

	// ��ʼ��
	const BYTE* pData = (BYTE*)(&pRecv->petDataLoad);
	PetSoul* pSoulLoad = PetSoul::CreateSoulByDBData(pData, TRUE);
	if (!GT_VALID(pSoulLoad))
	{
		dwRtv = E_Pets_Soul_CreateInitErr;
		goto EXIT;
	}

	// �ܷ��������
	dwRtv = pRole->GetPetPocket()->CanPutIn(pSoulLoad);
	if (E_Success != dwRtv)
		goto EXIT;

	// ���ĳ��ﵰ
	if (E_Success != pRole->GetItemMgr().ItemUsedFromBag(pRecv->n64Serial, 1, ELCLD_PET_ITEM_CREATE_PET))
	{
		dwRtv = E_Pets_Soul_PetEggErr;
		goto EXIT;
	}

	// ��������
	dwRtv = pRole->GetPetPocket()->PutIn(pSoulLoad, FALSE, TRUE);
	if (E_Success != dwRtv)
		goto EXIT;

	// �ٻ�
	pRole->GetPetPocket()->CallPet(pSoulLoad->GetID());

	// ��log
	PetSoul::LogPet(pRole->GetID(), pSoulLoad->GetID(), ELCLD_PET_CREATE_PET);

EXIT:
	// ������ ������ݿ�ɾ��
	if (E_Pets_Success != dwRtv)
	{
		ILOG->Write(_T("Can not create pet soul when dbpet created petid:%u, masterid:%u, errcode:%u\n"), pRecv->petDataLoad.petAtt.dwPetID, pRole->GetID(), dwRtv);
		PetSoul::LogPet(pRole->GetID(), pRecv->petDataLoad.petAtt.dwPetID, ELCLD_PET_DEL_PET_CREATE);

		if(P_VALID(pSoulLoad))
			PetSoul::DeleteSoul(pSoulLoad, TRUE);
	}
		
	// ���ͻ��˻���Ϣ
	tagNS_UsePetEgg send;
	send.dwErrCode = dwRtv;
	send.n64ItemID = pRecv->n64Serial;
	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD DBSession::HandleLoadAllGPInfo( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pRecv, pMsg, NDBS_GetAllGPInfo);

	if (pRecv->dwErrorCode != E_Success)
		return pRecv->dwErrorCode;

	return g_mall.LoadAllGPInfo(pRecv->nGPInfoNum, (LPVOID)(pRecv->GPInfoData));
}

DWORD DBSession::HandleLoadRoleVCard( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pRecv, pMsg, NDBS_GetOffLineVCard);

	if (E_Success != pRecv->dwErrCode)
	{
		return E_Success;
	}

	Role* pQuery = g_roleMgr.GetRolePtrByID(pRecv->dwQueryID);
	if (!P_VALID(pQuery))
	{
		return E_Success;
	}

	tagRoleVCard tmpVCard;

	const BYTE* pByte1 = pRecv->byData;
	tmpVCard.Init(pByte1, NULL);

	tmpVCard.SendVCard2Client(pRecv->dwQueryID);

	return E_Success;
}

DWORD DBSession::HandleLoadVIPStallInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetAllVIPStallInfo);

	if (pRecv->dwErrorCode != E_Success)
		return pRecv->dwErrorCode;

	return g_VIPStall.LoadAllVIPStallInfo(pRecv->VipStallInfo);
}

//------------------------------------------------------------------------------------------------
// ��ȡ�̶������
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadActivityData(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadActivityData);

	if(pRecv->nNum == 0) return 0;

	// ��ʼ�̶���ű�����
	tagActivityData* pActivityData = &pRecv->Activity[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		CActivity* pActivity = g_activityMgr.GetActivity(pActivityData->dwID);
		if(P_VALID(pActivity))
			pActivity->InitScriptData(pActivityData);

		pActivityData++;
	}

	return 0;
}

DWORD DBSession::HandleLoadVNBData( tagNetCmd* pMsg, DWORD )
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_GetVNBData);
	g_VipNetBarMgr.InitData(&pData->players);	

	return E_Success;
}


DWORD DBSession::HandleInitStrengthRankings(tagNetCmd* pMsg, DWORD)
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_StrengthRankings);
	g_StrengthRankings.InitRankings(pData);

	return E_Success;
}

DWORD DBSession::HandleRetUpdateRankings(tagNetCmd* pMsg, DWORD)
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_UpdateRankings);

	if (pData->dwErrorCode == E_Success)
		g_StrengthRankings.Init();
	else
		return E_SystemError;

	return E_Success;
}

DWORD DBSession::HandleLoadLeftMsg( tagNetCmd* pMsg, DWORD )
{
	g_msgMgr.HandleLoadLeftMsg(pMsg);

	return E_Success;
}

DWORD DBSession::HandleGuildMemberInitOk( tagNetCmd* pMsg, DWORD )
{
	tagNDBC_GuildInitOK send;
	g_dbSession.Send(&send, send.dwSize);

	return E_Success;
}

DWORD DBSession::HandleExternalLinksList(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_GetExternalLinks * p = (tagNDBS_GetExternalLinks*)pMsg;
	if( P_VALID(p) )
	{
		RefreshExternalLinksList(p);
	}
	return E_Success;
}


VOID DBSession::SendMsgToPlayerForExtLinks(Role * ply)
{
	m_elExternalLinks.SendMessageToPlayer  (ply);
}

VOID DBSession::RefreshExternalLinksList(tagNetCmd * p)
{
	tagNDBS_GetExternalLinks * pCmd = (tagNDBS_GetExternalLinks *)p;
	if( P_VALID(pCmd) )
	{
		m_lstExterLinks.Clear  ();
		tagSExternalLink t;
		tagExternalLink * p = pCmd->links;
		for( int i = 0 ; i < pCmd->dwElementNum ; ++i )
		{
			t.LinkName	= p[i].linkName;
			t.LinkURL	= p[i].linkValue;
			m_lstExterLinks.PushBack  (t);
		}
		if( pCmd->dwElementNum > 0 )
			m_elExternalLinks.FillExternalLinks(m_lstExterLinks);
	}
}

VOID DBSession::UpdateExternalLinksList(VOID)
{
	//if( 0 == (DWORD)m_timLastPullExtLink )
	//{
	//	m_timLastPullExtLink = g_world.GetWorldTime  ();
	//	tagNDBC_GetExternalLinks msg;
	//	Send(&msg,msg.dwSize);
	//}
	//else
	if( 0 < m_timLastPullExtLink && m_timLastPullExtLink != GT_INVALID )
	{
		tagDWORDTime t = g_world.GetWorldTime  ();
		if( CalcTimeDiff(t,m_timLastPullExtLink) / 60 >= 5 )
		{
			m_timLastPullExtLink = t;
			tagNDBC_GetExternalLinks msg;
			Send(&msg,msg.dwSize);
		}
	}
}

VOID DBSession::InitialExternalLinksList(VOID)
{
	m_timLastPullExtLink = g_world.GetWorldTime  ();
	tagNDBC_GetExternalLinks msg;
	Send(&msg,msg.dwSize);
}
