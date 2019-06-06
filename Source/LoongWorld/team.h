//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: team.h
// author: wjiang
// actor:
// data: 2008-12-10
// last:
// brief: ������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/group_define.h"

#include <list>
#include "mutex.h"
#include "role.h"

class Creature;

class Team
{
	friend class GroupMgr;

public:
	//---------------------------------------------------------------------------------------------------
	// CONSTRUCT
	//---------------------------------------------------------------------------------------------------
	Team(DWORD dwTeamID, Role* pLeader, Role* pFirstMem);
	~Team();

	//---------------------------------------------------------------------------------------------------
	// ����Get
	//---------------------------------------------------------------------------------------------------
	DWORD			GetTeamID()	const							{ return m_dwTeamID; }
	DWORD			GetGroupID() const							{ return m_dwGroupID; }
	INT				GetMemNum()	const							{ return m_nMemNum; }
	DWORD			GetTeamSynTick() const						{ return m_dwTeamSynTick; }
	INT				GetTeamMaxLevel() const						{ return m_nMaxLevel; }
	INT				GetTeamMinLevel() const						{ return m_nMinLevel; }
	INT				GetTeamAvgLevel() const;
	Role*			GetMemPtr(INT nIndex) const; 
	DWORD			GetMemID(INT nIndex) const;
	BOOL			IsLeader(DWORD dwRoleID) const				{ return m_dwMemID[0] == dwRoleID; }
	VOID			ExportAllMemID(DWORD dwMemID[MAX_TEAM_NUM]) const;
	BOOL			IsNeedDelete()								{ return m_bNeedDelete; }

	EPickMode		GetPickMode() const							{ return m_ePickMode; }
	FLOAT			GetExpFactor() const						{ return m_fExpFactor; }
	Role*			GetTeamPickRole(Creature* pCreature) const;

	DWORD			GetOwnInstanceMapID() const					{ return m_dwOwnMapID; }
	DWORD			GetOwnInstanceID() const					{ return m_dwOwnInstanceID; }


	//---------------------------------------------------------------------------------------------------
	// ����Set
	//---------------------------------------------------------------------------------------------------
	VOID			SetMaxLevel(INT nLevel)						{ m_nMaxLevel = nLevel; }
	VOID			SetMinLevel(INT nLevel)						{ m_nMinLevel = nLevel; }
	VOID			SetSynTick(DWORD dwTick)					{ m_dwTeamSynTick = dwTick; }

	VOID			SetGroupID(DWORD dwGroupID)					{ m_dwGroupID = dwGroupID; }

	VOID			SetOwnInstanceID(DWORD dwInstanceID) const	{ m_dwOwnInstanceID = dwInstanceID; }
	VOID			SetOwnInstanceMapID(DWORD dwMapID) const	{ m_dwOwnMapID = dwMapID; }

	VOID			CalExpFactor();

	//---------------------------------------------------------------------------------------------------
	// ���鹦�����
	//---------------------------------------------------------------------------------------------------
	INT				AddMember(Role* pInviter, Role* pReplyer);
	INT				KickMember(Role* pSrc, Role* pDest);
	INT				LeaveTeam(Role* pRole);
	INT				SetPickMode(Role* pRole, EPickMode ePickMode);
	INT				ChangeLeader(Role* pSrc, Role* pDest);
	
	VOID			OnMemLevelChange(Role* pRole);
	VOID			OnLeaderReinChange(Role* pRole);

	VOID			SendTeamMesInSameMap(DWORD dwMapID, LPVOID pMsg, DWORD dwSize);

	template<typename UnitOperation>
	VOID			ForEachRoleInTeam( UnitOperation oper ) const;

	static VOID			BroadcastLootItemMsgInTeam(Role * pOwner,tagItem * pItem);
	static VOID			BroadcastLootItemMsgInTeamExceptPicker(Role * pPicker,tagItem * pItem);
private:
	//----------------------------------------------------------------------------------------------------
	// ������Ϣ���
	//----------------------------------------------------------------------------------------------------
	VOID			SendTeamInstanceNotigy(Role* pRole, LPVOID pMsg, DWORD dwSize);
	VOID			SendTeamMessage(LPVOID pMsg, DWORD dwSize);

	VOID			SendTeamMessageExceptSomeone(LPVOID pMsg, DWORD dwSize,Role* pExpRole);

	VOID			SendTeamateMessage(DWORD dwRoleID, LPVOID pMsg, DWORD dwSize);
	VOID            SendTeamMesOutBigVis(Role* pRole, LPVOID pMsg, DWORD dwSize);
	VOID			SendRoleInitStateToTeam(Role* pNewMem);
	VOID			SendTeamState(Role* pNewMem);
	VOID			UpdateTeamatePos();

	//----------------------------------------------------------------------------------------------------
	// ���ܺ���
	//----------------------------------------------------------------------------------------------------
	INT				IsRoleInTeam(DWORD dwMem) const;
	VOID			DeleteMem(const INT nIndex);
	VOID			ReCalTeamLevel();

	INT				CanAddMember(Role* pInviter, Role* pReplyer);
	INT				CanKickMember(Role* pSrc, Role* pDest, INT& nIndex);
	INT				CanLeaveTeam(Role* pRole, INT& nIndex);

	//----------------------------------------------------------------------------------------------------
	// �¼�����
	//----------------------------------------------------------------------------------------------------
	VOID			OnCreate();
	VOID			OnDelete();
	VOID			OnAddMember(Role* pRole);
	VOID			OnDelMember(Role* pRole);

private:
	DWORD				m_dwTeamID;					// С��ID
	DWORD				m_dwGroupID;				// �����Ŷ�ID
	INT					m_nMemNum;					// ��Ա����
	DWORD				m_dwMemID[MAX_TEAM_NUM];	// ��ԱID(��һ���Ƕӳ�)
	Role*				m_pMemPtr[MAX_TEAM_NUM];	// ��Աָ��
	DWORD				m_dwTeamSynTick;			// С�����ͬ��λ�õ�Tick(ÿ5��ͬ��һ��)

	INT					m_nMaxLevel;				// С�ӳ�Ա��ߵȼ�
	INT					m_nMinLevel;				// С�ӳ�Ա��͵ȼ�

	mutable DWORD		m_dwOwnMapID;				// С�Ӵ����ĸ�����ͼID
	mutable DWORD		m_dwOwnInstanceID;			// С�Ӵ����ĸ���ID

	EPickMode			m_ePickMode;				// ʰȡģʽ
	FLOAT				m_fExpFactor;				// ɱ�־�������
	mutable list<INT>	m_nPickList;				// ˳��ʰȡ�б�
	mutable Mutex		m_Lock;						// ʰȡ�����е�꣬���ܲ���������ð취��

	BOOL				m_bNeedDelete;				// �Ƿ���Ҫɾ��
};

//-------------------------------------------------------------------------------------------
// �õ���Աָ��
//-------------------------------------------------------------------------------------------
inline Role* Team::GetMemPtr(INT nIndex) const 
{ 
	ASSERT(nIndex < MAX_TEAM_NUM && nIndex >= 0);	
	return m_pMemPtr[nIndex]; 
}

//--------------------------------------------------------------------------------------------
// �õ���ԱID
//--------------------------------------------------------------------------------------------
inline DWORD Team::GetMemID(INT nIndex) const
{
	ASSERT(nIndex < MAX_TEAM_NUM && nIndex >= 0);	
	return m_dwMemID[nIndex];	
}

//---------------------------------------------------------------------------------------------
// ����Ƿ��ڸö����У�����ڣ������ڶ����е����������򷵻�GT_INVALID
//---------------------------------------------------------------------------------------------
inline INT Team::IsRoleInTeam(DWORD dwMemID) const
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( m_dwMemID[i] == dwMemID )
			return i;
	}
	return GT_INVALID;
}

//---------------------------------------------------------------------------------------------
// ���ڶ�����ÿ������Աִ��UnitOperation
//---------------------------------------------------------------------------------------------
template<typename UnitOperation>
VOID Team::ForEachRoleInTeam( UnitOperation oper ) const
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( !P_VALID(m_pMemPtr[i]) )
			break;
		oper(m_pMemPtr[i]);
	}
}