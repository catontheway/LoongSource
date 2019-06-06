//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: login_session.h
// author: Aslan
// actor:
// data: 2008-7-10
// last:
// brief: ��Ϸ���������½�������ĶԻ���
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/time.h"
#include "mutex.h"

//------------------------------------------------------------------------------
// ������½������������ʺŽṹ
//------------------------------------------------------------------------------
struct tagWillLoginPlayer
{
	DWORD			dwVerifyCode;	// ��֤��
	DWORD			dwTick;			// ֪ͨʱ��Tick
	BYTE			byPrivilege;	// �ʺ�Ȩ��
	BOOL			bGuard;			// ������
	DWORD			dwAccOLTime;	// �ۼƵ�¼ʱ��
	char			szAccount[X_SHORT_NAME];//�˺�

	tagDWORDTime	dwPreLoginTime;				//�ϴε�¼ʱ��
	DWORD			dwPreLoginIP;				//�ϴε�¼ip

	tagWillLoginPlayer(DWORD)
	{
		dwVerifyCode = GT_INVALID;
		dwTick = GT_INVALID;
		byPrivilege = 0;
		bGuard = TRUE;
		dwAccOLTime = 0;

		dwPreLoginTime = 0;
		dwPreLoginIP = 0;
	}

	tagWillLoginPlayer(DWORD verifyCode, DWORD tick, BYTE privilege, BOOL guard, 
						DWORD AccOLTime, LPCSTR Account,tagDWORDTime PreLoginTime,
						DWORD PreLoginIP)
	{
		dwVerifyCode = verifyCode;
		dwTick = tick;
		byPrivilege = privilege;
		bGuard = guard;
		dwAccOLTime = AccOLTime;
		strncpy_s(szAccount, Account, X_SHORT_NAME);

		dwPreLoginTime = PreLoginTime;
		dwPreLoginIP = PreLoginIP;
	}
};

//--------------------------------------------------------------------------------
// Login Session
//--------------------------------------------------------------------------------
class LoginSession
{
public:
	LoginSession();
	~LoginSession();

	BOOL Init();
	VOID Destroy();
	VOID Update();

	BOOL IsWell() { return (m_bInitOK && m_pTran->IsConnected()); }

	VOID Send(LPVOID pMsg, DWORD dwMsgSize)	{ if(FALSE == IsWell() ) return;		m_pTran->Send(pMsg, dwMsgSize); }

	INT	GetUnsendPackageNum() { return m_pTran->GetUnsendPackageNum(); }
	INT	GetReceivedPackageNum() { return m_pTran->GetReceivedPackageNum(); }

public:
	VOID RemoveWillLoginPlayer(DWORD dwAccountID)
	{
		m_WillLoginPlayerMutex.Acquire();
		m_mapWillLoginPlayer.Erase(dwAccountID);
		m_WillLoginPlayerMutex.Release();
	}

public:
	BOOL IsAccountValid(DWORD dwAccountID, DWORD dwVerifyCode, BYTE& byPrivilege, BOOL& bGuard, 
						DWORD& dwAccOLSec, LPSTR szAccount,tagDWORDTime &dwPreLoginTime,DWORD &dwPreLoginIP);
	VOID SendPlayerLogin(DWORD dwAccountID, DWORD dwIP, DWORD dwErrorCode);
	VOID SendPlayerLogout(DWORD dwAccountID);
	VOID SendWorldStatus();
private:
	VOID UpdateSession();
	VOID UpdateWillLoginPlayer();

private:
	// ��ȡ�ļ�, ��ʼ����Ա
	BOOL InitConfig();
	// ע�����е���������
	VOID RegisterAllLoginCommand();

private:
	// �������ݿ������
	DWORD ThreadConnectLogin();

private:
	DWORD HandleCertification(tagNetCmd* pMsg, DWORD);
	DWORD HandleHeartBeat(tagNetCmd* pMsg, DWORD);
	DWORD HandlePlayerWillLogin(tagNetCmd* pMsg, DWORD);
	DWORD HandleKickPlayer(tagNetCmd* pMsg, DWORD);
	DWORD HandleWorldColsed(tagNetCmd* pMsg, DWORD);
	DWORD HandleFatigueNotice(tagNetCmd* pMsg, DWORD);
	
private:
	TSFPTrunk<LoginSession>				m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<StreamTransport>			m_pTran;
	TObjRef<Thread>						m_pThread;
	TObjRef<NetCmdMgr>					m_pMsgCmdMgr;

	// ���Ӳ���
	CHAR								m_szLoginIP[X_IP_LEN];		// ��½������ip
	DWORD								m_dwLoginPort;				// ��½������port
	
	DWORD								m_dwLocalWanIP;				// ����������IP

	DWORD								m_dwGoldenCode;				// ��½��������ɫ����

	volatile BOOL						m_bTermConnect;				// ��¼��ǰ����״̬
	volatile BOOL						m_bInitOK;					// �Ƿ��ʼ�����

	TMap<DWORD, tagWillLoginPlayer>		m_mapWillLoginPlayer;		// ��Ҫ��½�����������������б�
	Mutex								m_WillLoginPlayerMutex;		// ������½���������������б���

	HANDLE								m_hWorldClosed;
};

extern LoginSession g_loginSession;		// Login Sessionȫ�ֶ���