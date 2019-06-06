//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: world_net_cmd_mgr.h
// author: Sxg
// actor:
// data: 2009-2-2
// last:
// brief: ��Ϣ������ -- ��Ҫ��ͼ�߳��ϲ㴦�����Ϣ����
//-------------------------------------------------------------------------------
#pragma once

#include "mutex.h"
#include "player_net_cmd_mgr.h"
#include "world_session.h"
//-------------------------------------------------------------------------------
// ������Ϣ�ṹ��չ
//-------------------------------------------------------------------------------
struct tagMsgEx
{
	DWORD			dwID;		// session id
	LPBYTE			pMsg;
	DWORD			dwSize;

	tagMsgEx(DWORD dwSessionID, LPBYTE _pMsg, DWORD _dwSize)
	{
		dwID	=	dwSessionID;
		pMsg	=	_pMsg;
		dwSize	=	_dwSize;
	}

	~tagMsgEx()
	{
		if( P_VALID(pMsg) )
		{
			g_worldSession.ReturnMsg(pMsg);
		}
	}
};


//-------------------------------------------------------------------------------
// ��ͼ�ϲ���Ϣ������
//-------------------------------------------------------------------------------
class WorldNetCmdMgr
{
public:
	WorldNetCmdMgr();
	~WorldNetCmdMgr();

	BOOL Init() { return TRUE; }
	VOID Update();

public:
	VOID Add(DWORD dwSessionID, LPBYTE pMsg, DWORD dwSize);

public:
	//VOID RegisterAll();
	VOID RegisterRecvProc(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize);
	VOID ResetMsgCounter(LPCSTR szCmd);
	VOID ResetMsgCounter(DWORD dwMsgID);

	UINT32 GetRunTimesPerTick(DWORD dwMsgID);

private:
	TList<tagMsgEx*>	m_lstMsgEx;
	Mutex				m_mutex;

	PlayerNetCmdMgr		m_WorldNetMgr;

	// ��ϢÿTickͳ��
	TMap<DWORD, UINT32>	m_mapMsgCount;
};

extern WorldNetCmdMgr g_worldNetCmdMgr;