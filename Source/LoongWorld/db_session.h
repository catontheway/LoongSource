//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: db_session.h
// author: Sxg
// actor:
// data: 2008-5-19
// last:
// brief: ��Ϸ�����������ݿ�������ĶԻ���
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"

#include "..\WorldDefine\base_define.h"

// Jason [v1.3.1] �ⲿ����
#include "external_links.h"

//-----------------------------------------------------------------------------
class DBSession
{
public:
	DBSession();
	~DBSession();

public:
	BOOL Init();
	VOID Destroy();

	VOID Update();
	BOOL IsWell() { return (m_bInitOK && m_pTran->IsConnected()); }

	VOID Send(LPVOID pMsg, DWORD dwMsgSize)	{ m_pTran->Send(pMsg, dwMsgSize); }

	LPBYTE Recv(DWORD& dwMsgSize) { return m_pTran->Recv( dwMsgSize ); }

	// ����յ���������Ϣ
	VOID FreeRecved(LPVOID pMsg) { m_pTran->FreeRecved((LPBYTE)pMsg); }

	INT	GetUnsendPackageNum() { return m_pTran->GetUnsendPackageNum(); }
	INT	GetReceivedPackageNum() { return m_pTran->GetReceivedPackageNum(); }

private:
	// ��ȡ�ļ�, ��ʼ����Ա
	BOOL InitConfig();

	// ע�����е���������
	VOID RegisterAllDBCommand();

private:
	// �������ݿ������
	DWORD ThreadConnectDB();

private:
	// ��֤������
	DWORD HandleCertification(tagNetCmd* pMsg, DWORD);
	DWORD HandleHeartBeat(tagNetCmd* pMsg, DWORD);

	// ��ʼ����Ϣ
	DWORD HandleLoongWorldInitOK(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadAllRoleInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleItemInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleItemNeedLog(tagNetCmd* pMsg, DWORD);

	// ���ﴴ����ɾ���Ͷ�ȡ
	DWORD HandleRoleEnum(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleCreate(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleDelete(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleResume(tagNetCmd* pMsg, DWORD) { return 0; }
	DWORD HandleRoleLoad(tagNetCmd* pMsg, DWORD);

	// ��ٱ��������µ���Ʒ
	DWORD HandleBaiBaoLoad(tagNetCmd* pMsg, DWORD);
	// ���°ٱ����е�Ԫ��
	DWORD HandleBaiBaoYuanBaoLoad(tagNetCmd* pMsg, DWORD);

	// Ԫ���������
	DWORD HandleLoadAllYBAccount(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadAllYBOrder(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleGetYBOrder(tagNetCmd* pMsg, DWORD);

	// �ٱ�����ʷ��¼���
	DWORD HandleBaiBaoLoadLog(tagNetCmd* pMsg, DWORD);

	// ������
	DWORD HandleRepRankLoad(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetActTreasureList(tagNetCmd* pMsg, DWORD);
	DWORD HandleRepRstTimeStamp(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetFameHallEnterSnap(tagNetCmd* pMsg, DWORD);

	// �������
	DWORD HandleLoadGuild(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildMember(tagNetCmd* pMsg, DWORD);
	DWORD HandleGuildMemberInitOk(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildWareItems(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildUpgradeInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildSkillInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildCommerceInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadCommerceRankInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleGuildInitOK(tagNetCmd* pMsg, DWORD);

	// ����
	DWORD HandleCreatePetSoul(tagNetCmd* pMsg, DWORD);
	
	// �̳����
	DWORD HandleLoadAllGPInfo(tagNetCmd* pMsg, DWORD);

	// ��ɫ����
	DWORD HandleLoadRoleVCard(tagNetCmd* pMsg, DWORD);

	// VIP̯λ
	DWORD HandleLoadVIPStallInfo(tagNetCmd* pMsg, DWORD);

	// �̶��
	DWORD HandleLoadActivityData(tagNetCmd* pMsg, DWORD);

	// VIP����
	DWORD HandleLoadVNBData(tagNetCmd* pMsg, DWORD);

	// ʵ�����а����
	DWORD HandleInitStrengthRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleRetUpdateRankings(tagNetCmd* pMsg, DWORD);

	// ��ȡ����
	DWORD HandleLoadLeftMsg(tagNetCmd* pMsg, DWORD);
	
private:
	TSFPTrunk<DBSession>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<StreamTransport>	m_pTran;
	TObjRef<Thread>				m_pThread;
	TObjRef<NetCmdMgr>			m_pMsgCmdMgr;

	// ���Ӳ���
	CHAR						m_szDBIP[X_IP_LEN];		// ���ݿ������ip
	DWORD						m_dwDBPort;				// ���ݿ������port

	// �������Ӻ��跢�͵�����
	DWORD						m_dwGoldenCode;					// ���ݿ��������ɫ����
	//TCHAR						m_szServerName[X_LONG_STRING];	// ������Ϸ����Ļ�������

	volatile BOOL				m_bTermConnect;					// ��¼��ǰ����״̬
	volatile BOOL				m_bInitOK;						// �Ƿ��ʼ�����

	DWORD						m_dwInitTime;					//��db��ʼ��������������ʱ�� ����

	// Jason v[1.3.1] �ⲿ����
	TList<tagSExternalLink>	m_lstExterLinks;
	ExternalLinks			m_elExternalLinks;
	tagDWORDTime			m_timLastPullExtLink;

	VOID RefreshExternalLinksList(tagNetCmd * pCmd);
	DWORD HandleExternalLinksList(tagNetCmd* pMsg, DWORD);

	VOID UpdateExternalLinksList(VOID);

	VOID InitialExternalLinksList(VOID);

public:
	VOID SendMsgToPlayerForExtLinks(Role * ply);
};

extern DBSession g_dbSession;		// DB sessionȫ�ֶ���
