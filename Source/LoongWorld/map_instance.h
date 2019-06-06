//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_instance.h
// author: wjing
// actor:
// data: 2008-1-15
// last:
// brief: ��ͨ������
//-----------------------------------------------------------------------------
#pragma once

#pragma once
#include "map.h"
#include "../ServerDefine/instance_define.h"

class Team;

//-----------------------------------------------------------------------------
// CONST
//-----------------------------------------------------------------------------
const INT ROLE_LEAVE_INSTANCE_TICK_COUNT_DOWN	= 60 * TICK_PER_SECOND;		// ��ɫ�ڸ������뿪����ʱ�����ͳ������ĵ���ʱ��60��


//-----------------------------------------------------------------------------
// ��ͨ�����࣬������һ���鴴���ĸ��������������ȵ�
//-----------------------------------------------------------------------------
class MapInstanceNormal : public MapInstance
{
public:
	MapInstanceNormal();
	virtual ~MapInstanceNormal();

	virtual BOOL		Init(const tagMapInfo* pInfo, DWORD dwInstanceID, Role* pCreator=NULL, DWORD dwMisc=GT_INVALID);
	virtual VOID		Update();
	virtual VOID		Destroy();

	virtual VOID		AddRole(Role* pRole);
	virtual	VOID		RoleLeaveMap(DWORD dwID, BOOL bChgMap);
	virtual INT			CanEnter(Role *pRole);
	virtual BOOL		CanDestroy();
	virtual VOID		OnDestroy();

	DWORD				GetCreatorID()			{ return m_dwCreatorID; }
	EInstanceHardMode	GetInstanceHard()		{ return m_eInstanceHardMode; }
	DWORD				CalTimeLimit();
	const tagInstance*	GetInstanceProto()		{ return m_pInstance; }

	VOID				OnTeamCreate(const Team* pTeam);
	VOID				OnTeamDelete(const Team* pTeam);
	VOID				OnRoleLeaveTeam(DWORD dwRoleID, const Team* pTeam);
	VOID				OnRoleEnterTeam(DWORD dwRoleID, const Team* pTeam);

protected:
	virtual VOID		OnDelete();
	virtual BOOL		InitAllSpawnPointCreature();
	
private:
	VOID				UpdateTimedIssue();
	BOOL				IsTimeLimit()		{ return m_pInstance->dwTimeLimit > 0 && m_pInstance->dwTargetLimit != GT_INVALID; }

	BOOL				RecalHardMode();
	BOOL				GetCreatureBaseLevel(INT& nBaseLevel);
	DWORD				CalCreatureTypeID(const tagRandSpawnPointInfo* pRandSpawnPoint);
	DWORD				TransmitBigID(INT nBaseLevel, tagMapSpawnPointInfo* pMapSpawnInfo);

private:
	BOOL					m_bNoEnter;						// �����Ƿ�û�˽����
	DWORD					m_dwCreatorID;					// ����������ID
	DWORD					m_dwTeamID;						// ����������С��ID
	DWORD					m_dwStartTick;					// ��ʼʱ��
	DWORD					m_dwEndTick;					// ������ʼ���õ���ʱ
	EInstanceHardMode		m_eInstanceHardMode;			// �����Ѷ�

	TMap<DWORD, INT>		m_mapWillOutRoleID;				// ����������������ȴ������ȥ������б�

	const tagInstance*		m_pInstance;					// ������̬����
};


