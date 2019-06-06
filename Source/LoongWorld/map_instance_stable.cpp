//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_instance.cpp
// author: wjiang
// actor:
// data: 2008-1-15
// last:
// brief: �ȶ���ͼ����
// remark: ��Ҫ�������ִ�����µȸ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "map_instance_stable.h"
#include "map_creator.h"
#include "role.h"
#include "creature.h"
#include "map_mgr.h"
#include "NPCTeam.h"
#include "NPCTeam_mgr.h"
#include "../WorldDefine/msg_map.h"
#include "../WorldDefine/MapAttDefine.h"


MapInstanceStable::MapInstanceStable() : MapInstance()
{
}

MapInstanceStable::~MapInstanceStable()
{
	Destroy();
}

//------------------------------------------------------------------------------------------------------
// ��ʼ���������߲���Ҫ��
//------------------------------------------------------------------------------------------------------
BOOL MapInstanceStable::Init(const tagMapInfo* pInfo, DWORD dwInstanceID, Role* pCreator, DWORD dwMisc)
{
	ASSERT( P_VALID(pInfo) );
	ASSERT( EMT_System == pInfo->eType );

	// ��ͼ�������
	m_pUtil = "Util";
	m_pInfo = pInfo;
	m_mapSession.Clear();
	m_mapRole.Clear();
	m_mapShop.Clear();
	m_mapCofC.Clear();
	m_mapGroundItem.Clear();

	// �����������
	m_dwID = m_pInfo->dwID;
	m_dwInstanceID = dwInstanceID;

	m_pTeamMgr = new NPCTeamMgr(this);
	if(!P_VALID(m_pTeamMgr))
		return FALSE;

	// ���뵼��ͼ���ݣ�Clone������
	NavMap* pNavMap = g_mapCreator.GetInstanceTemplate(pInfo->dwID);
	if( !P_VALID(pNavMap) ) return FALSE;
	m_pNav = new NavMap;
	pNavMap->Clone(m_pNav);
	if( !P_VALID(m_pNav) ) return FALSE;

	// ����mapinfo����ʼ����ͼ�Ĺ�������б����Ϣ
	if( FALSE == InitLogicalInfo() )
	{
		SAFE_DEL(m_pNav);
		return FALSE;
	}

	return TRUE;		
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID MapInstanceStable::Update()
{
	Map::Update();
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID MapInstanceStable::Destroy()
{

}

//---------------------------------------------------------------------------------
// ��ʽ����һ����ң���ֻ���ɹ���õ�ͼ��MapMgr����
//---------------------------------------------------------------------------------
VOID MapInstanceStable::AddRole(Role* pRole)
{
	Map::AddRole(pRole);
}

//---------------------------------------------------------------------------------
// ����뿪��ͼ��ֻ���������߳��������
//---------------------------------------------------------------------------------
VOID MapInstanceStable::RoleLeaveMap(DWORD dwID, BOOL bChgMap)
{
	Map::RoleLeaveMap(dwID, bChgMap);
}
//---------------------------------------------------------------------------------
// �Ƿ��ܽ��븱��
//---------------------------------------------------------------------------------
INT MapInstanceStable::CanEnter(Role *pRole)
{
	return E_Success;
}

//---------------------------------------------------------------------------------
// �Ƿ����ɾ��
//---------------------------------------------------------------------------------
BOOL MapInstanceStable::CanDestroy()
{
	return MapInstance::CanDestroy();
}


//---------------------------------------------------------------------------------
// ��ʼ��ˢ�ֵ����
//---------------------------------------------------------------------------------
BOOL MapInstanceStable::InitAllSpawnPointCreature()
{
	return Map::InitAllSpawnPointCreature();
}

//---------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------
VOID MapInstanceStable::OnDelete()
{
	
}

//-----------------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------------
VOID MapInstanceStable::OnDestroy()
{
}
