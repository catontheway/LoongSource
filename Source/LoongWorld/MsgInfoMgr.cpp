//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: team.cpp
// author: wjiang
// actor:
// data: 2008-12-10
// last:
// brief: ������
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "MsgInfoMgr.h"
#include "role.h"
#include "role_mgr.h"
#include "team.h"

#include "../ServerDefine/base_define.h"

MsgInfoMgr g_MsgInfoMgr;

MsgInfoMgr::~MsgInfoMgr()
{
	MsgInfo* pMsgInfo = (MsgInfo*)GT_INVALID;
	std::list<DWORD>		listMsgInfo;
	m_mapMsgInfo.ExportAllKey(listMsgInfo);

	std::list<DWORD>::iterator it = listMsgInfo.begin();

	for (; it != listMsgInfo.end(); ++it )
	{
		pMsgInfo = m_mapMsgInfo.Peek(*it);
		SAFE_DEL(pMsgInfo);
	}
	
	/*TMap<DWORD, MsgInfo*>::TMapIterator it = m_mapMsgInfo.Begin();
	while(m_mapMsgInfo.PeekNext(it, pMsgInfo))
	{
		SAFE_DEL(pMsgInfo);
	}*/

	m_mapMsgInfo.Clear();	
}

//-------------------------------------------------------------------------------------------------------
// �����ű�ͨ����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD MsgInfoMgr::BeginMsgEvent()
{
	m_Lock.Acquire();
	MsgInfo* pMsgInfo = new MsgInfo(m_dMsgInfoID);
	DWORD	dwMsgInfoID	= m_dMsgInfoID;

	m_mapMsgInfo.Add(m_dMsgInfoID, pMsgInfo);

	++m_dMsgInfoID;
	m_Lock.Release();

	return dwMsgInfoID;
}

//-------------------------------------------------------------------------------------------------------
// ����Ϣ���������¼�����
//-------------------------------------------------------------------------------------------------------
VOID MsgInfoMgr::AddMsgEvent(DWORD dwMsgInfoID, EMsgUnitType eMsgUnitType, LPVOID pData)
{
	MsgInfo *pMsgInfo = m_mapMsgInfo.Peek(dwMsgInfoID);
	if(!P_VALID(pMsgInfo))
		return;

	pMsgInfo->AddMsgUnit(eMsgUnitType, pData);
}

//-------------------------------------------------------------------------------------------------------
// ���ͽű�ͨ����Ϣ�����
//-------------------------------------------------------------------------------------------------------
VOID MsgInfoMgr::DispatchRoleMsgEvent(DWORD dwMsgInfoID, Role *pRole)
{
	if(!P_VALID(pRole))
		return;

	MsgInfo *pMsgInfo = m_mapMsgInfo.Peek(dwMsgInfoID);
	if(!P_VALID(pMsgInfo))
		return;

	LPVOID pSend = pMsgInfo->CreateMsgByMsgInfo(g_MemPoolSafe);
	DWORD dwSize = pMsgInfo->GetMsgSize();

	pRole->SendMessage(pSend, dwSize);
	pMsgInfo->DeleteMsg(pSend, g_MemPoolSafe);

	RemoveMsgInfo(pMsgInfo);
}

//-------------------------------------------------------------------------------------------------------
// �����������е�ͼ�ڵ���ҷ��ͽű�ͨ����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID MsgInfoMgr::DispatchWorldMsgEvent(DWORD dwMsgInfoID)
{
	MsgInfo *pMsgInfo = m_mapMsgInfo.Peek(dwMsgInfoID);
	if(!P_VALID(pMsgInfo))
		return;

	LPVOID pSend = pMsgInfo->CreateMsgByMsgInfo(g_MemPoolSafe);
	DWORD dwSize = pMsgInfo->GetMsgSize();

	g_roleMgr.SendWorldMsg(pSend, dwSize);
	pMsgInfo->DeleteMsg(pSend, g_MemPoolSafe);

	RemoveMsgInfo(pMsgInfo);
}

//-------------------------------------------------------------------------------------------------------
// ��ͬһ��ͼ�ڵ���ҷ��ͽű�ͨ����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID MsgInfoMgr::DispatchMapMsgEvent(DWORD dwMsgInfoID, Map* pMap)
{
	if(!P_VALID(pMap))	return;

	MsgInfo *pMsgInfo = m_mapMsgInfo.Peek(dwMsgInfoID);
	if(!P_VALID(pMsgInfo))
		return;

	LPVOID pSend = pMsgInfo->CreateMsgByMsgInfo(g_MemPoolSafe);
	DWORD dwSize = pMsgInfo->GetMsgSize();

	pMap->SendMapMessage(pSend, dwSize);
	pMsgInfo->DeleteMsg(pSend, g_MemPoolSafe);

	RemoveMsgInfo(pMsgInfo);
}

//-------------------------------------------------------------------------------------------------------
// ��ͬһ��ͼ�ڵ�С�Ӷ��ѷ��ͽű�ͨ����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID MsgInfoMgr::DispatchTeamMapMsgEvent(DWORD dwMsgInfoID, Map *pMap, Team *pTeam)
{
	if( !P_VALID(pTeam) ) return;
	if( !P_VALID(pMap) ) return;

	MsgInfo *pMsgInfo = m_mapMsgInfo.Peek(dwMsgInfoID);
	if(!P_VALID(pMsgInfo))
		return;

	LPVOID pSend = pMsgInfo->CreateMsgByMsgInfo(g_MemPoolSafe);
	DWORD dwSize = pMsgInfo->GetMsgSize();

	pTeam->SendTeamMesInSameMap(pMap->GetMapID(), pSend, dwSize);
	pMsgInfo->DeleteMsg(pSend, g_MemPoolSafe);

	RemoveMsgInfo(pMsgInfo);
}













