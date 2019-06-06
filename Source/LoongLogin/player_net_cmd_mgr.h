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

//-----------------------------------------------------------------------------
// �ͻ������������
//-----------------------------------------------------------------------------

class Player;

typedef DWORD (Player::*NETMSGHANDLER)(tagNetCmd*);

class PlayerNetCmdMgr
{
public:
	PlayerNetCmdMgr();
	~PlayerNetCmdMgr();

	VOID Destroy();

	BOOL Register(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc);
	VOID UnRegisterAll();

	NETMSGHANDLER GetHandler(tagNetCmd* pMsg, UINT32 nMsgSize);

	BOOL HandleCmd(tagNetCmd* pMsg, DWORD nMsgSize, Player* pSession);
protected:
	typedef struct tagPlayerCmd
	{
		std::string				strCmd;		// ������
		tstring					strDesc;	// ����
		NETMSGHANDLER			handler;	// ����ָ��
		UINT32					nTimes;		// �յ�������Ĵ���
	} tagPlayerCmd;

	TObjRef<Util>				m_pUtil;

	TMap<DWORD, tagPlayerCmd*>	m_mapProc;
};

