//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: group_mgr.h
// author: wjiang
// actor:
// data: 2009-1-04
// last:
// brief: �Ŷӹ�����
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "event_mgr.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"

class Team;

class GroupMgr : public EventMgr<GroupMgr>
{
public:
	//---------------------------------------------------------------------------------------------------
	// CONSTRUCT
	//---------------------------------------------------------------------------------------------------
	~GroupMgr();

	//---------------------------------------------------------------------------------------------------
	// ��ʼ�������¼�����
	//---------------------------------------------------------------------------------------------------
	BOOL	Init();
	VOID	Update();
	VOID	Destroy();

	//---------------------------------------------------------------------------------------------------
	// С�����
	//---------------------------------------------------------------------------------------------------
	VOID	OnInviteJoinTeam(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnInviteJoinTeamReply(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnLeaderKickMember(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnMemberLeaveTeam(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnSetPickMode(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnChangeLeader(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnChangeLevel(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnChangeLeaderRein(DWORD dwSenderID, LPVOID pEventMessage);

	/*
	Jason 2009-11-25 ����������Э�鴦��
	*/
	// ��������������ߣ�����������
	VOID NotifyApplyToJoinTeam(DWORD RoleID, DWORD errcode, DWORD dwTeamLeaderID = 0, DWORD dwApplicantID = 0, BOOL bTeamLeaderAgree = FALSE);
	// �����������Э��Ĵ�����
	VOID OnApplyJoinTeam(DWORD dwSenderID, LPVOID pEventMessage);
	// ����ӳ������������Э��
	VOID OnReplyForApplingToJoinTeam(DWORD dwSenderID, LPVOID pEventMessage);


	VOID    SendTeamInstanceNotify(Role* pRole, DWORD dwTeamID, LPVOID pMsg, DWORD dwSize);
	VOID	SendTeamMessage(DWORD dwTeamID, LPVOID pMsg, DWORD dwSize);
	VOID	SendTeamMesOutBigVis(Role *pRole, DWORD dwTeamID, LPVOID pMsg, DWORD dwSize);
	VOID	SendTeamateMessage(DWORD dwTeamID, DWORD dwRole, LPVOID pMsg, DWORD dwSize);
	VOID	RoleOutline(Role* pRole);

	const Team* GetTeamPtr(DWORD dwTeamID)	{ return m_mapTeam.Peek(dwTeamID); }

	//---------------------------------------------------------------------------------------------------
	// ������������������Ҽ�һ��buff
	//---------------------------------------------------------------------------------------------------
	VOID	OnAddAllRoleBuff(DWORD dwSenderID, LPVOID pEventMessage);

	//---------------------------------------------------------------------------------------------------
	// �ű�����С������¼�
	//---------------------------------------------------------------------------------------------------
	VOID	OnCreateTeam(DWORD dwSenderID, LPVOID pEventMessage);
	VOID	OnAddMember(DWORD dwSenderID, LPVOID pEventMessage);

private:
	static VOID	RegisterGroupEventFunc();
	VOID		UpdateTeamatePos();

	Team*		CreateTeam(Role* pInviter, Role* pReplyer);
	VOID		DeleteTeam(Team* pTeam);

private:
	TMap<DWORD, Team*>			m_mapTeam;
	DWORD						m_dwTeamIndex;			// С��ID����
};

extern GroupMgr g_groupMgr;