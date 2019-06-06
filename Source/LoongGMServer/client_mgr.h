//-----------------------------------------------------------------------------
//!\file	client_mgr.h
//!\brief	�ͻ��˹�����
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "client_db.h"

class ClientDB;
class Client;

//===========================================��ʱ����Ϣ������===============================================//
class TimeLimitMsgMgr
{
public:
	TimeLimitMsgMgr();
	~TimeLimitMsgMgr();

	VOID Update();
	VOID AddMsg2LMTDL(DWORD, DWORD, BOOL, tagNetCmd*, DWORD);
	//VOID SetCountForAutoNotice(INT nCount){ m_nCountForAutoNotice = nCount; }
	VOID ClearMsgList();
	DWORD CancelTimeLimitedMsg(DWORD dwMsgCrc);

private:
	// ʱ����Ϣͳ�ƽṹ
	struct tagMsgTimeDeadLine
	{
		DWORD					dwMsgCrc;			// ��Ϣ����Crcֵ
		DWORD					dwDeadLine;			// ����ʱ�䣬���˸�ʱ��㽫����Ϣ����
		DWORD					dwClientID;			// ��¼���ĸ�Rtclient���͵ĸ�����
		BOOL					bType;				// true: ��ʱ���͵���Ϣ; false: ÿ�찴�㷢�͵���Ϣ
		
		tagNetCmd*				pMsg;	
	};

	// ���list
	TList<tagMsgTimeDeadLine*>	m_LstMsgTimeDeadLine;
	//TList<tagNetCmd*>			m_LstMsgNeedDel;	// ʱ���ѵ�����Ҫɾ������Ϣ��������Ϣ��CrcΪ����

	// ����
	DWORD						m_dwUpdateTicks;
	INT32						m_nTickCountDown;
	//INT						m_nCountForAutoNotice;

	// ��ʱ����(��λ����)
	VOID UpdateTimeLeftList();

	// ʱ����Ϣ�ķ���
	DWORD SendTimeLimitMsg(DWORD, tagNetCmd*, DWORD);
	DWORD SendDoubleMsg(tagNetCmd*, DWORD);
	DWORD SendRightNotice(tagNetCmd*, DWORD);
	DWORD SendAutoNotice(tagNetCmd*, DWORD);
	DWORD SendAutoChatNotice(tagNetCmd*, DWORD);
};



//===========================================�ͻ��˹�����===============================================//
class ClientMgr
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ClientMgr();
 	~ClientMgr(){};

	//-------------------------------------------------------------------------
	// ��ʼ�������¼�����
	//-------------------------------------------------------------------------
	BOOL	Init();
	VOID	Update();
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ��Ϣ���
	//-------------------------------------------------------------------------
	VOID	SendMsg(DWORD dwClientID, LPVOID pMsg, DWORD dwSize)	{ m_pStreamServer->Send(dwClientID, pMsg, dwSize); }
	VOID	SendAllClientMsg(LPBYTE pMsg, DWORD dwSize);

private:
	//-------------------------------------------------------------------------
	// �����ص�����
	//-------------------------------------------------------------------------
	DWORD	LoginCallBack(LPVOID, LPVOID);
	DWORD	LogoutCallBack(LPVOID);

	//-------------------------------------------------------------------------
	// ��Ϣע��
	//-------------------------------------------------------------------------
	VOID	RegisterClientMsg();
	VOID	UnRegisterClientMsg();

	//-------------------------------------------------------------------------
	// Ȩ�޹���
	//-------------------------------------------------------------------------
	BOOL	InitPrivilegeInfo();
	BOOL	HaveEnoughPrivilege(const tagNetCmd* pCmd, const Client* pClient);
	VOID	SendPrivilegeInfo(Client*);

	DWORD	GetClientIP(DWORD dwClientID);

	//-------------------------------------------------------------------------
	// ��¼LOG
	//-------------------------------------------------------------------------
	VOID	LogGMAction(Client* pClient, EActionType eActionType, LPCTSTR szDetails);

	//-------------------------------------------------------------------------
	// �ͻ�����Ϣ������
	//-------------------------------------------------------------------------
	DWORD	HandleChallenge(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleShowSumChallenge(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleCloseWorld(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleStartWorld(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleSystemCmd(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleSetMaxNum(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleUpdateMall(tagNetCmd* pMsg, Client* pClient);
	
	DWORD	HandleGMCreate(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleGMModify(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleGMDelete(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleGMQuery(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleResumeRole(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleModifyRole(tagNetCmd* pMsg, Client* pClient);
	
	DWORD	HandleDouble(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleCancelDouble(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleRightNotice(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleCancelRightNotice(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleAutoNotice(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleAutoChatNotice(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleCancelAutoChatNotice(tagNetCmd* pMsg, Client* pClient);
	
	DWORD	HandleCreateItem(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleDeleteItem(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleItemResume(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleClearBaibao(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleAccountForbid(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleBlacklist(tagNetCmd* pMsg, Client* pClient);
	
	DWORD	HandleChangeIPBind(tagNetCmd* pMsg, Client* pClient);

	DWORD	HandleShowSumReq(tagNetCmd* pMsg, Client* pClient);
	DWORD	HandleModifyFackNum(tagNetCmd* pMsg, Client* pClient);
	
	//DWORD	HandleCancelTimeLimitedMsg(tagNetCmd* pMsg, Client* pClient);
private:
	TSFPTrunk<ClientMgr>		m_Trunk;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<StreamServer>		m_pStreamServer;		// ������ͨ�ŵײ�
	INT							m_nPort;				// �����˿�

	ClientDB					m_ClientDB;				// �ͻ����ʺ����ݿ�

	volatile DWORD				m_dwIDGen;				// ID������
	TMap<DWORD, Client*>		m_mapAllClient;			// ������½�Ŀͻ����б�ȫ��ΨһID����ָ�룩
	TMap<DWORD, Client*>		m_mapClient;			// �Ѿ���½�Ŀͻ����б��ʺ�ID����ָ�룩
	TMap<DWORD, DWORD>			m_mapPrivilege;			// Ȩ�ޱ� ���ڴ�Ÿ���Ϣ��Ȩ����Ϣ

	TimeLimitMsgMgr				m_tlMsgMgr;				// ��ʱ��Ϣ������
};

extern ClientMgr g_ClientMgr;
extern TimeLimitMsgMgr g_TimeLimitMsgMgr;