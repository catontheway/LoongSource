//-----------------------------------------------------------------------------
//!\file NetSession.h
//!\author Lyp
//!
//!\date 2008-06-10
//! last 2008-06-12
//!
//!\brief net session
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!\brief net session
//! ���Ʒ���Ƶ����ÿ������һ����ͬʱ�ṩ��أ����Բ鿴����Щ��Ϣ�ۻ�
//! ���Է�����滻�����е���Ϣ
//-----------------------------------------------------------------------------
class NetSession
{
public:
	VOID Init();
	VOID Connect(LPCTSTR szIP, INT nPort);
	VOID Disconnect();
	BOOL IsConnect();
	VOID ClearMsgQueue();

	VOID Update();

	VOID Send(tagNetCmd* pNetCmd);
	INT	GetSendQueueSize();
	INT	GetSendQueueActionSize();
	INT	GetSendQueueIdleSize();


	// �õ�Զ��IP
	LPCTSTR GetRemoteIPString() { return m_strRemoteIP.c_str(); }

	
	NetSession():m_Trunk(this){}
	~NetSession();

protected:
	TSFPTrunk<NetSession>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Console>			m_pConsole;
	TObjRef<XClient>			m_pStream;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	tstring						m_strRemoteIP;
	DWORD						m_dwLastSendTime;

	MsgQueue					m_SendQueueAction;	// ��Ϊ����
	MsgQueue					m_SendQueue;		// ��ͨ����
	MsgQueue					m_SendQueueIdle;	// ��Ҫ��Ϣ����

	DWORD SendQueue(LPCTSTR szCommand);	// ��ʾsendQueue����Ϣ
};
