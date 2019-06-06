//-----------------------------------------------------------------------------
//!\file game_server.h
//!\brief ���������࣬LoginServer��LoongWorld��LoongDB���Ӹû���̳�
//!
//!\date 2009-04-20
//! last 2009-04-20
//!\author zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/login_define.h"
#include "../ServerDefine/msg_rt_c.h"
#include "../ServerDefine/msg_rt_s.h"

#define MAX_WAIT_FOR_TICK_M		1 
//------------------------------------------------------------------------------
// ����������
//------------------------------------------------------------------------------
class GameServer
{
protected:
	//--------------------------------------------------------------------------
	// CONSTRUCT
	//--------------------------------------------------------------------------
	GameServer(DWORD dwID, LPCSTR szIP);
	virtual ~GameServer();

public:
	//--------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------
	VOID			Update();

	//--------------------------------------------------------------------------
	// ��ʼ��������
	//--------------------------------------------------------------------------
	BOOL			ServerStart();
	BOOL			ServerShutDown();

	//--------------------------------------------------------------------------
	// ����Get
	//--------------------------------------------------------------------------
	EServerType		GetType() const			{ return m_eType; }
	DWORD			GetID()	const			{ return m_dwID; }
	LPCSTR			GetIP() const			{ return m_szIP.c_str(); }
	BOOL			IsValid() const			{ return m_bValid; }
	EWorldStatus	GetStatus() const		{ return m_eStatus; }
	INT				GetOnline() const		{ return m_nPlayerNum; }
	INT				GetOnlineDisplay() const{ return m_nPlayerNumDisplay; }

	//--------------------------------------------------------------------------
	// ���ɶ�̬��Ϣ
	//--------------------------------------------------------------------------
	VOID			FillServerInfo(tagServerInfo* pInfo) const;

	//--------------------------------------------------------------------------
	// ����״̬
	//--------------------------------------------------------------------------
	VOID			RefreshStatus(EWorldStatus eStatus, INT nOnlineNum, INT nMaxOnlineNum, INT nOnlineNumDisplay);
	VOID			ChangeStatus(EWorldStatus eStatus);
	VOID			RefreshLastTickTime(){m_dwLastestTick = GetCurrentDWORDTime();}

	//--------------------------------------------------------------------------
	// ���ط�����
	//--------------------------------------------------------------------------
	BOOL			StartServer();
	BOOL			ShutdownServer();

	//--------------------------------------------------------------------------
	// ִ��ϵͳ����
	//--------------------------------------------------------------------------
	BOOL			ExecuteCmd(tagNetCmd*);

	BOOL			IsConnected(){return m_pTran->IsConnected();}

protected:
	VOID			UpdateSession();
	VOID			UpdateConnection(); //�����server�ı�����ά���������
	
protected:
	DWORD					m_dwID;				// ȫ��ΨһID
	BOOL					m_bValid;			// �Ƿ�����
	std::string				m_szIP;				// IP��ַ

	StreamTransport*		m_pTran;			// �����������ػ���������ӣ�����������Ӳ�ر�Զ�̷�����
	BOOL					m_bTryingConnect;	// ����������
	INT						m_nPort;

	EServerType				m_eType;			// ����������
	EWorldStatus			m_eStatus;			// ��ǰ��״̬
	INT						m_nPlayerNum;		// ����������login��world��˵�����壬��db������
	INT						m_nPlayerNumDisplay;// ������ʾ�ģ���db������
	INT						m_nMaxPlayerNum;	// �������������login��world��˵�����壬��db������
	tagDWORDTime			m_dwLastestTick;	// �÷����������һ��������ʱ��
};

//--------------------------------------------------------------------------------------------------
// ����server��״̬
//--------------------------------------------------------------------------------------------------
inline VOID GameServer::FillServerInfo(tagServerInfo* pInfo) const
{
	pInfo->bValid			=	m_bValid;
	pInfo->eStatus			=	m_eStatus;
	pInfo->nMaxPlayerNum	=	m_nMaxPlayerNum;
	pInfo->nPlayerOnline	=	m_nPlayerNumDisplay;//m_nPlayerNum;
	pInfo->nRealOnline		=	m_nPlayerNum;
}

//-------------------------------------------------------------------------------------------------
// Login������
//-------------------------------------------------------------------------------------------------
class GameServerLogin : public GameServer
{
public:
	//----------------------------------------------------------------------------------------------
	// CONSTRUCT
	//----------------------------------------------------------------------------------------------
	GameServerLogin(DWORD dwID, LPCSTR szIP);
	~GameServerLogin() {}

};

//---------------------------------------------------------------------------------------------------
// World������
//---------------------------------------------------------------------------------------------------
class GameServerWorld : public GameServer
{
public:
	//-----------------------------------------------------------------------------
	// CONSTRUCT
	//-----------------------------------------------------------------------------
	GameServerWorld(DWORD dwID, LPCSTR szIP);
	~GameServerWorld() {}

	//-----------------------------------------------------------------------------
	// ����˫��
	//-----------------------------------------------------------------------------
	DWORD SetDoubleRate(DWORD dwTime, DWORD dwOpenTime, DWORD dwRatio, DWORD dwDType);

};

//---------------------------------------------------------------------------------------------------
// DB������
//---------------------------------------------------------------------------------------------------
class GameServerDB : public GameServer
{
public:
	//-----------------------------------------------------------------------------
	// CONSTRUCT
	//-----------------------------------------------------------------------------
	GameServerDB(DWORD dwID, LPCSTR szIP);
	~GameServerDB() {}
};
