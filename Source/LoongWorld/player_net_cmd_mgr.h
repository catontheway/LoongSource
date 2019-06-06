//-----------------------------------------------------------------------------
//!\file player_net_cmd_mgr.h
//!\author Aslan
//!
//!\date 2008-06-13
//! last 2008-06-13
//!
//!\brief �ͻ����������������Ҫ���NetCmdMgr�޷���Զ������Ϣ�����ȱ��
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

class PlayerSession;

typedef DWORD (PlayerSession::*NETMSGHANDLER)(tagNetCmd*);

//-----------------------------------------------------------------------------
// �ͻ������������
//-----------------------------------------------------------------------------
class PlayerNetCmdMgr
{
public:
	PlayerNetCmdMgr();
	~PlayerNetCmdMgr();

	VOID Init();
	VOID Destroy();

	VOID LogAllMsg();

	BOOL RegisterRecvProc(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize);
	BOOL RegisterSendProc(LPCSTR szCmd);
	VOID UnRegisterAll();

	NETMSGHANDLER GetHandler(tagNetCmd* pMsg, UINT32 nMsgSize);
	VOID CountServerMsg(DWORD dwMsgID);

	BOOL HandleCmd(tagNetCmd* pMsg, DWORD nMsgSize, PlayerSession* pSession);

	// ȡ����Ϣִ�д���
	UINT32 GetRecvCmdRunTimes(DWORD dwMsgID);


protected:
	typedef struct tagPlayerCmd
	{
		std::string				strCmd;			// ������
		tstring					strDesc;		// ����
		DWORD					dwSize;			// ��Ϣ��С
		NETMSGHANDLER			handler;		// ����ָ��
		volatile UINT32			nTimes;			// ������Ĵ���
	} tagPlayerCmd;

	TObjRef<Util>				m_pUtil;
	TObjRef<Log>				m_pLog;

	TMap<DWORD, tagPlayerCmd*>	m_mapRecvProc;	// ������Ϣ�Ĵ���ͳ��
	TMap<DWORD,	tagPlayerCmd*>	m_mapSendProc;	// ������Ϣ�Ĵ���ͳ��
};