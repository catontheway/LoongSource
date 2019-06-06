//-----------------------------------------------------------------------------
//!\file to_rt_session.h
//!\author Lyp
//!
//!\date 2005-04-05
//! last 2005-04-05
//!
//!\brief ���������ط���ĶԻ���
//!
//!	Copyright (c) 2005 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "Singleton.h"

static const INT	SEND_INFO_INTERVAL	=	TICK_PER_SECOND;		// ÿ���೤ʱ�䷢��һ�η�������Ϣ

//-----------------------------------------------------------------------------
// ���������ط���ĶԻ���
//-----------------------------------------------------------------------------
class RTSession : public Singleton<RTSession>
{
public:
	//-------------------------------------------------------------------------
	// Constructor
	//-------------------------------------------------------------------------
	RTSession();
	~RTSession();

	BOOL	Init();
	VOID	Destroy();
	VOID	Update();

	BOOL	IsConnected()	{ return m_pTran->IsConnected(); }

private:
	VOID	SendServerInfo();
	VOID	RegisterAllRTCommand();
	VOID	UpdateSession();

	DWORD	ThreadConnectRT();

	BOOL    Forbid_DeleteBlankOfString(tstring &str);
	DWORD	Forbid_GetForbidWorldCrc(tagNetCmd* pMsg,vector<DWORD> &vecWorldCrc);
	VOID	Forbid_GetForbidAccountName(tstring &strMultiAccountName,vector<tstring> &vecAccountName);
private:
	DWORD	HandleServerLogin(tagNetCmd* pMsg, DWORD);
	DWORD	HandleCloseServer(tagNetCmd* pMsg, DWORD);
	DWORD	HandleForbidAccount(tagNetCmd* pMsg, DWORD);
	DWORD	HandleForbidIP(tagNetCmd* pMsg, DWORD);


private:
	TSFPTrunk<RTSession>		m_Trunk;
	TObjRef<StreamTransport>	m_pTran;
	TObjRef<NetCmdMgr>			m_pMsgCmdMgr;
	TObjRef<Thread>				m_pThread;
	TObjRef<Util>				m_pUtil;

	volatile BOOL				m_bTermConnect;
	volatile BOOL				m_bInitOK;
	INT							m_nSendInfoTickCountDown;

	tstring						m_strIP;
	DWORD						m_dwPort;
	DWORD						m_dwSectionID;
	INT							m_nZoneNum;
	DWORD*						m_dwWorldID;
	DWORD*						m_dwWorldNameCRC;
	tstring*					m_strWorldName;
};


#define sRTSession RTSession::getSingleton()