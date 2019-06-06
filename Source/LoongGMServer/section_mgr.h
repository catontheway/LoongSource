//-----------------------------------------------------------------------------
//!\file section_mgr.h
//!\brief ����������������÷�������������д���
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/base_define.h"
#include "../ServerDefine/rt_define.h"

class GameWorld;
class GameServer;
class GameServerLogin;
class GameServerWorld;
class GameServerDB;
class Section;
class Client;
class ServerDB;

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
class SectionMgr
{
public:
	SectionMgr();
	~SectionMgr(){};

	//--------------------------------------------------------------------------
	// ��ʼ�������¼�����
	//--------------------------------------------------------------------------
	BOOL		Init();
	VOID		Destroy();
	VOID		Update();

	//---------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------
	LPBYTE		RecvMsg(DWORD dwID, DWORD& dwMsgSize, INT& nUnRecved)	{ return m_pStreamServer->Recv(dwID, dwMsgSize, nUnRecved); }
	VOID		ReturnMsg(DWORD dwID, LPBYTE pMsg)	{ m_pStreamServer->FreeRecved(dwID, pMsg); }
	VOID		SendMsg(DWORD dwID, LPVOID pMsg, DWORD dwSize) { m_pStreamServer->Send(dwID, pMsg, dwSize); }
	VOID		HandleCmd(LPVOID pMsg, DWORD dwSize, DWORD dwID)	{ m_pCmdMgr->HandleCmd((tagNetCmd*)pMsg, dwSize, dwID); }

	VOID		SendAllServerStaticInfoToClient(Client* pClient);


	//---------------------------------------------------------------------------
	// ����IDӳ�����
	//---------------------------------------------------------------------------
	DWORD		CreateNewClientID() { InterlockedExchangeAdd((LPLONG)&m_dwIDGen, 1); return m_dwIDGen; }
	VOID		RegisterClientID(DWORD dwID, DWORD dwSectionID)	{ m_mapIDSectionID.Add(dwID, dwSectionID); }

	//---------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------
	BOOL		StartServer(EServerType eType, DWORD dwSectionID, DWORD dwWorldID);
	BOOL		CloseServer(EServerType eType, DWORD dwSectionID, DWORD dwWorldID, BOOL bForce=FALSE);

	//---------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------
	TMap<DWORD, Section*> GetSectionMap(){return m_mapSection;}
	Section*	GetSection(DWORD dwSectionID)		{ return m_mapSection.Peek(dwSectionID); }
	Section*	GetSection(LPCTSTR);
	INT			GetSectionNum()						{ return m_mapSection.Size(); }
	GameServer* GetServer(DWORD dwSectionID, DWORD dwWorldID, EServerType eType);
	ServerDB*	GetServerDB(DWORD dwSectionID, DWORD dwWorldID);
	GameWorld*	FindGameWorld(DWORD dwID);
	VOID		SendDynamicInfo()					{ InterlockedExchange((LPLONG)&m_bSendImmediately, TRUE); }

private:
	//---------------------------------------------------------------------------
	// ������Ϣע��
	//---------------------------------------------------------------------------
	VOID		RegisterServerMsg();
	VOID		UnRegisterServerMsg();

	//---------------------------------------------------------------------------
	// ������Ϣ������
	//---------------------------------------------------------------------------
	DWORD		HandleWorldLogin(tagNetCmd* pMsg, DWORD);
	//DWORD		HandleStartWorld(tagNetCmd* pMsg, DWORD);
	//DWORD		HandleCloseWorld(tagNetCmd* pMsg, DWORD);
	DWORD		HandleGetWorldInfo(tagNetCmd* pMsg, DWORD);
	DWORD		HandleRTServiceLogin(tagNetCmd* pMsg, DWORD);

	//---------------------------------------------------------------------------
	// ��ͻ��˷��ز������
	//---------------------------------------------------------------------------
	DWORD		HandleRetDouble(tagNetCmd* pMsg, DWORD dwID);
	DWORD		HandleRetAutoNotice(tagNetCmd* pMsg, DWORD dwID);
	DWORD		HandleRetRightNotice(tagNetCmd* pMsg, DWORD dwID);
	DWORD		HandleRetAccountForbid(tagNetCmd* pMsg, DWORD dwID);
	DWORD		HandleRetIPBlacklist(tagNetCmd* pMsg, DWORD dwID);
	DWORD		HandleRetCancelDouble(tagNetCmd* pMsg, DWORD dwID);
	//DWORD		HandleRetCancelRightNotice(tagNetCmd* pMsg, DWORD dwID);

	//---------------------------------------------------------------------------
	// ����ͨ�����
	//---------------------------------------------------------------------------
	DWORD		LoginCallBack(LPVOID, LPVOID);			// ���������ӻص�����
	DWORD		LogoutCallBack(LPVOID);					// ���������ӶϿ��ص�����

	//---------------------------------------------------------------------------
	// ��ʼ�����
	//---------------------------------------------------------------------------
	BOOL		LoadAllServerInfo();

	//---------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------
	VOID		UpdateSession();
	VOID		SendAllServerDynamicInfo();

	//---------------------------------------------------------------------------
	// �����������
	//---------------------------------------------------------------------------
	GameServer*			FindServer(DWORD dwID);
	GameServerLogin*	FindLoginServer(DWORD dwID);
	GameServerWorld*	FindWorldServer(DWORD dwID);
	GameServerDB*		FindDBServer(DWORD dwID);

private:
	static const INT			SEND_INFO_TICK_COUNT_DOWN = 5 * TICK_PER_SECOND;	// ���ͷ�������Ϣ�ĵ���ʱ���

private:
	TSFPTrunk<SectionMgr>		m_Trunk;
	TObjRef<NetCmdMgr>			m_pCmdMgr;					// �������������
	TObjRef<StreamServer>		m_pStreamServer;			// ����ӿ�
	INT							m_nPort;					// ���ؼ����Ķ˿�

	TMap<DWORD, Section*>		m_mapSection;				// ���д���

	volatile DWORD				m_dwIDGen;					// ID������
	TMap<DWORD, DWORD>			m_mapIDSectionID;			// ����ID�����ID��Ӧ��

	INT							m_nSendInfoTickCountDown;	// ���ͷ�������Ϣ�ĵ���ʱ 
	volatile BOOL				m_bSendImmediately;
};

extern SectionMgr g_SectionMgr;