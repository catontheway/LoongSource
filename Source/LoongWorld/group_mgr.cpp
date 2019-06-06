//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: group_mgr.h
// author: wjiang
// actor:
// data: 2009-1-04
// last:
// brief: �Ŷӹ�����
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/group_define.h"
#include "../WorldDefine/msg_group.h"

#include "../ServerDefine/role_data.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "group_mgr.h"
#include "player_session.h"
#include "unit.h"
#include "role.h"
#include "role_mgr.h"
#include "team.h"
#include "map_creator.h"
#include "map.h"
#include "map_instance.h"
#include "map_mgr.h"
#include "TimeChecker.h"

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
GroupMgr::~GroupMgr()
{
}

//-------------------------------------------------------------------------------------------------------
// �������ʱ��
//-------------------------------------------------------------------------------------------------------
BOOL GroupMgr::Init()
{
	m_dwTeamIndex = 3000000001;
	m_mapTeam.Clear();
	RegisterGroupEventFunc();
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------
// ע���¼�
//--------------------------------------------------------------------------------------------------------
VOID GroupMgr::RegisterGroupEventFunc()
{
	RegisterEventFunc(EVT_JoinTeam,			&GroupMgr::OnInviteJoinTeam);
	RegisterEventFunc(EVT_JoinTeamRepley,	&GroupMgr::OnInviteJoinTeamReply);
	RegisterEventFunc(EVT_KickMember,		&GroupMgr::OnLeaderKickMember);
	RegisterEventFunc(EVT_LeaveTeam,		&GroupMgr::OnMemberLeaveTeam);
	RegisterEventFunc(EVT_SetPickMol,		&GroupMgr::OnSetPickMode);
	RegisterEventFunc(EVT_ChangeLeader,		&GroupMgr::OnChangeLeader);
	RegisterEventFunc(EVT_ChangeLevel,      &GroupMgr::OnChangeLevel);
	RegisterEventFunc(EVT_ChangeRein,		&GroupMgr::OnChangeLeaderRein);
	RegisterEventFunc(EVT_AddAllRoleBuff,	&GroupMgr::OnAddAllRoleBuff);
	RegisterEventFunc(EVT_CreateTeam,		&GroupMgr::OnCreateTeam);
	RegisterEventFunc(EVT_AddMember,		&GroupMgr::OnAddMember);

	//Jason 2009-11-25 ����������Э�鴦��ע��
	RegisterEventFunc(EVT_ApplyToJoinTeam,		&GroupMgr::OnApplyJoinTeam);
	RegisterEventFunc(EVT_ApplyReply,		&GroupMgr::OnReplyForApplingToJoinTeam);
}

//----------------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------------
VOID GroupMgr::Update()
{
	CHECK_TIME();
	EventMgr<GroupMgr>::Update();

	// ͬ����Ա֮���λ��
	UpdateTeamatePos();
}

//----------------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------------
VOID GroupMgr::Destroy()
{
	TMap<DWORD, Team*>::TMapIterator it = m_mapTeam.Begin();
	Team* pTeam = NULL;

	while( m_mapTeam.PeekNext(it, pTeam) )
	{
		SAFE_DEL(pTeam);
	}
	m_mapTeam.Clear();
}

//----------------------------------------------------------------------------------------------------------
// ͬ��ÿ�����������Ա��λ��
//----------------------------------------------------------------------------------------------------------
VOID GroupMgr::UpdateTeamatePos()
{
	DWORD dwTick = g_world.GetWorldTick();

	Team* pTeam = NULL;
	TMap<DWORD, Team*>::TMapIterator it = m_mapTeam.Begin();

	while( m_mapTeam.PeekNext(it, pTeam) )
	{
		if( dwTick - pTeam->GetTeamSynTick() > TEAM_SYN_TICK )
		{
			pTeam->UpdateTeamatePos();
			pTeam->SetSynTick(dwTick); 
		}
	}
}

//--------------------------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------------------------
Team* GroupMgr::CreateTeam(Role* pInviter, Role* pReplyer)
{
	Team* pTeam = new Team(m_dwTeamIndex, pInviter, pReplyer);
	m_mapTeam.Add(m_dwTeamIndex, pTeam);

	// С��ID������һ
	++m_dwTeamIndex;

	pTeam->OnCreate();

	return pTeam;
}

//----------------------------------------------------------------------------------------------------------
// ɾ������
//----------------------------------------------------------------------------------------------------------
VOID GroupMgr::DeleteTeam(Team* pTeam)
{
	if( !P_VALID(pTeam) ) return;

	pTeam->OnDelete();

	m_mapTeam.Erase(pTeam->GetTeamID());
	SAFE_DEL(pTeam);
}

//---------------------------------------------------------------------------------------------------------
// ������Ҽ���С��
//---------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnInviteJoinTeam(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNC_InviteJoinTeam* pRecv = (tagNC_InviteJoinTeam*)pEventMessage;

	DWORD	dwSrcRoleID = dwSenderID;
	DWORD	dwDestRoleID = pRecv->dwDestRoleID;

	if( dwSrcRoleID == dwDestRoleID ) return;	// �����Լ�����ң�

	Role* pSrcRole	=	g_roleMgr.GetRolePtrByID(dwSrcRoleID);
	Role* pDestRole	=	g_roleMgr.GetRolePtrByID(dwDestRoleID);

	if( !P_VALID(pSrcRole) )
		return;

	DWORD dwErrorCode = E_Success;


	// �ж��Ƿ���ĳ��ͼ�������������
	Map* pMap = pSrcRole->GetMap();
	if(P_VALID(pMap))
		dwErrorCode = pMap->CanInviteJoinTeam();

	// �������˲�����
	if( !P_VALID(pDestRole) )
	{
		dwErrorCode = E_Team_Target_Not_Online;
	}
	// Jason 2009-12-11 ����bug��Ҫ�ж�ָ��
	// �жϱ������ߵ�״̬ �Ƿ���Խ����������
	else if(pDestRole->IsInStateCantBeInviteJoinTeam())
	{
		dwErrorCode = E_Team_Target_Not_Exit;
	}

	// Jason 2009-11-26 �������˴�������״̬
	//else if( pDestRole->IsInState(ES_Dead) )
	//{
	//	dwErrorCode = E_Team_Target_Not_Exit;
	//}

	// ����������С��
	else if( GT_INVALID != pDestRole->GetTeamID() )
	{
		dwErrorCode = E_Team_Target_Have_Team;
	}

	// �鿴���������ǲ������ڱ�����
	else if( GT_INVALID != pDestRole->GetTeamInvite() )
	{
		dwErrorCode = E_Team_Target_Busy;
	}

	// �������ǲ����ж���
	else if( GT_INVALID != pSrcRole->GetTeamID() )
	{	
		Team* pTeam = m_mapTeam.Peek(pSrcRole->GetTeamID());
		if( !P_VALID(pTeam) ) return;

		// �Ƿ�Ϊ�ӳ�
		if( !pTeam->IsLeader(pSrcRole->GetID()) )
			dwErrorCode = E_Team_Not_Leader;

		// С�������Ƿ�����
		if( pTeam->GetMemNum() >= MAX_TEAM_NUM )
			dwErrorCode = E_Team_Member_Full;
	}

	// ����жϳɹ�����������
	if( E_Success == dwErrorCode )
	{
		pDestRole->SetTeamInvite(pSrcRole->GetID());

		tagNS_InviteJoinTeam send;
		send.dwLeaderID		=	dwSrcRoleID;
		send.dwDestRoleID	=	dwDestRoleID;
		pDestRole->SendMessage(&send, send.dwSize);
	}
	// ���ɹ������ش���
	else
	{
		tagNS_InviteToLeader send;
		send.dwErrorCode = dwErrorCode;
		pSrcRole->SendMessage(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ����Ƿ�ͬ�����С��
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnInviteJoinTeamReply(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNC_InviteReply* pRecv = (tagNC_InviteReply*)pEventMessage;

	DWORD dwReplyRoleID = dwSenderID;

	Role* pReplyRole = g_roleMgr.GetRolePtrByID(dwReplyRoleID);
	if( !P_VALID(pReplyRole) ) return;

	DWORD dwInviteRoleID = pReplyRole->GetTeamInvite();
	if( GT_INVALID == dwInviteRoleID ) return;

	pReplyRole->SetTeamInvite(GT_INVALID);	// ���ñ������ߵ�������ID

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwInviteRoleID);
	if( !P_VALID(pSrcRole) ) return;

	// �ж��Ƿ���ĳ��ͼ�������������
	Map* pMap = pSrcRole->GetMap();
	if(P_VALID(pMap))
		if(pMap->CanInviteJoinTeam())
			return;

	// ��ʼ���ж�
	INT nRet = E_Success;
	BOOL bNewGroup = FALSE;
	Team* pTeam = NULL;

	// ���ͬ�����
	if( pRecv->bAgree )
	{
		// ��������Ƿ��ж�
		if( GT_INVALID != pReplyRole->GetTeamID() )
		{
			nRet = E_Team_Target_Have_Team;
		}
		// �޶�
		else
		{
			// ����������Ƿ��ж�
			if( GT_INVALID != pSrcRole->GetTeamID() )
			{
				pTeam = m_mapTeam.Peek(pSrcRole->GetTeamID());
				if( !P_VALID(pTeam) ) return;

				nRet = pTeam->AddMember(pSrcRole, pReplyRole);
			}
			// �������޶�
			else
			{
				bNewGroup = TRUE;
				pTeam = CreateTeam(pSrcRole, pReplyRole);
			}
		}
	}

	// ����Է�ͬ�⣬�����жϳɹ�������ȫ�������Ϣ
	if( E_Success == nRet && pRecv->bAgree && P_VALID(pTeam) )
	{
		// ���͸������������ɹ�
		tagNS_InviteReply send;
		send.bAgree = pRecv->bAgree;
		send.dwErrorCode = nRet;
		pTeam->ExportAllMemID(send.dwTeamMemID);

		pTeam->SendTeamMessage(&send, send.dwSize);

		// ���Ͷ�Ա��ʼ��Ϣ
		pTeam->SendRoleInitStateToTeam(pReplyRole);

		// ���͸��¼�������
		pTeam->SendTeamState(pReplyRole);

		// ����´������飬�����öӳ��Ķ�����Ϣ
		if( bNewGroup ) pTeam->SendTeamState(pSrcRole);
	}
	// �����ͬ�⣬�����ж�ʧ�ܣ���ֻ���͸�˫��
	else
	{
		tagNS_InviteReply send;
		send.dwTeamMemID[0] = dwInviteRoleID;
		send.dwTeamMemID[1] = dwReplyRoleID;
		send.bAgree = pRecv->bAgree;
		send.dwErrorCode = nRet;

		pSrcRole->SendMessage(&send, send.dwSize);
		pReplyRole->SendMessage(&send, send.dwSize);
	}
}


//-------------------------------------------------------------------------------------------------------
// ����ҳ�С��
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnLeaderKickMember(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNC_KickMember* pRecv = (tagNC_KickMember*)pEventMessage;

	DWORD	dwSrcRoleID = pRecv->dwSrcRoleID;
	DWORD	dwDestRoleID = pRecv->dwDestRoleID;

	if( dwSrcRoleID == dwDestRoleID ) return;	// �����߳��Լ�

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);
	Role* pDestRole = g_roleMgr.GetRolePtrByID(dwDestRoleID);

	if( !P_VALID(pDestRole) || !P_VALID(pSrcRole) )
		return;

	// �ҵ�����
	Team* pTeam = m_mapTeam.Peek(pSrcRole->GetTeamID());
	if( !P_VALID(pTeam) ) return;

	INT nRet = pTeam->KickMember(pSrcRole, pDestRole);

	if( E_Success == nRet )
	{
		// ������˳ɹ����������������Ϣ
		tagNS_KickMember send;
		send.dwDestRoleID = dwDestRoleID;
		send.dwErrorCode = nRet;
		pTeam->SendTeamMessage(&send, send.dwSize);
		pDestRole->SendMessage(&send, send.dwSize);

		// ���������Ҫɾ������ɾ������
		if( pTeam->IsNeedDelete() )
		{
			DeleteTeam(pTeam);
		}
	}
	else
	{
		// ������ɹ�������������
		tagNS_KickMember send;
		send.dwDestRoleID = dwDestRoleID;
		send.dwErrorCode = nRet;
		pSrcRole->SendMessage(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ����뿪С��
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnMemberLeaveTeam(DWORD dwSenderID, LPVOID pEventMessage)
{
	Role* pMember = g_roleMgr.GetRolePtrByID(dwSenderID);
	if( !P_VALID(pMember) ) return;

	// �ҵ�����
	Team* pTeam = m_mapTeam.Peek(pMember->GetTeamID());
	if( !P_VALID(pTeam) ) return;

	INT nRet =E_Success; 

	nRet = pTeam->LeaveTeam(pMember);

	if( E_Success == nRet )
	{
		// �ɹ����������ж����Ա��Ϣ
		tagNS_LeaveTeam send;
		send.dwRoleID = pMember->GetID();
		send.dwLeaderID = pTeam->GetMemID(0);
		send.dwErrorCode = nRet;
		pTeam->SendTeamMessage(&send, send.dwSize);
		pMember->SendMessage(&send, send.dwSize);

		// �������Ҫɾ������ɾ������
		if( pTeam->IsNeedDelete() )
		{
			DeleteTeam(pTeam);
		}
	}
	else
	{
		// ������ɹ��������뿪��
		tagNS_LeaveTeam send;
		send.dwRoleID = pMember->GetID();
		send.dwLeaderID = pTeam->GetMemID(0);
		send.dwErrorCode = nRet;
		pMember->SendMessage(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ����С��ʰȡģʽ
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnSetPickMode(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNC_SetPickMode* pRecv = (tagNC_SetPickMode*)pEventMessage;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwSenderID);
	if( !P_VALID(pRole) ) return;

	// �ҵ�����
	Team* pTeam = m_mapTeam.Peek(pRole->GetTeamID());
	if( !P_VALID(pTeam) ) return;

	INT nRet = pTeam->SetPickMode(pRole, pRecv->ePickMode);

	if( E_Success == nRet )
	{
		// ���óɹ����������ж�Ա��Ϣ
		tagNS_SetPickMode send;
		send.ePickMode = pRecv->ePickMode;
		send.dwErrorCode = nRet;
		pTeam->SendTeamMessage(&send, send.dwSize);
	}
	else
	{
		// ���ò��ɹ���������������Ϣ
		tagNS_SetPickMode send;
		send.ePickMode = pRecv->ePickMode;
		send.dwErrorCode = nRet;
		pRole->SendMessage(&send, send.dwSize);

	}
}

//-------------------------------------------------------------------------------------------------------
// �ı�С�Ӷӳ�
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnChangeLeader(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNC_ChangeLeader* pRecv = (tagNC_ChangeLeader*)pEventMessage;

	Role* pSrc = g_roleMgr.GetRolePtrByID(dwSenderID);
	Role* pDest = g_roleMgr.GetRolePtrByID(pRecv->dwNewLeader);

	if( !P_VALID(pSrc) || !P_VALID(pDest) ) return;

	// �ҵ�����
	Team* pTeam = m_mapTeam.Peek(pSrc->GetTeamID());
	if( !P_VALID(pTeam) ) return;

	INT nRet = E_Success;
	
	nRet = pTeam->ChangeLeader(pSrc, pDest);

	if( E_Success == nRet )
	{
		// �ɹ������Ͷ�����Ϣ
		tagNS_ChangeLeader send;
		send.dwLeaderID = pSrc->GetID();
		send.dwNewLeaderID = pDest->GetID();
		send.dwErrorCode = nRet;
		pTeam->SendTeamMessage(&send, send.dwSize);
	}
	else
	{
		// ���ɹ������ؽ�����
		tagNS_ChangeLeader send;
		send.dwLeaderID = pSrc->GetID();
		send.dwNewLeaderID = pDest->GetID();
		send.dwErrorCode = nRet;
		pSrc->SendMessage(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ������Ϣ������С��
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::SendTeamMessage(DWORD dwTeamID, LPVOID pMsg, DWORD dwSize)
{
	Team* pTeam = m_mapTeam.Peek(dwTeamID);
	if( !P_VALID(pTeam) ) return;

	pTeam->SendTeamMessage(pMsg, dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ������Ϣ������
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::SendTeamateMessage(DWORD dwTeamID, DWORD dwRole, LPVOID pMsg, DWORD dwSize)
{
	Team* pTeam = m_mapTeam.Peek(dwTeamID);
	if( !P_VALID(pTeam) ) return;

	pTeam->SendTeamateMessage(dwRole, pMsg, dwSize);
}

//-------------------------------------------------------------------------------------------------------
// �������������С�ӳ�Ա����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::SendTeamMesOutBigVis(Role* pRole, DWORD dwTeamID, LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pRole) ) return;

	Team* pTeam = m_mapTeam.Peek(dwTeamID);
	if( !P_VALID(pTeam) ) return;

	pTeam->SendTeamMesOutBigVis(pRole, pMsg, dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ֪ͨ���ѽ��븱��
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::SendTeamInstanceNotify(Role* pRole, DWORD dwTeamID, LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pRole) ) return;

	Team* pTeam = m_mapTeam.Peek(dwTeamID);
	if( !P_VALID(pTeam) ) return;

	pTeam->SendTeamInstanceNotigy(pRole, pMsg, dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ��Ա����
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::RoleOutline(Role* pRole)
{
	if( !P_VALID(pRole) ) return;

	Team* pTeam = m_mapTeam.Peek(pRole->GetTeamID());
	if( !P_VALID(pTeam) ) return;

	INT nRet = pTeam->LeaveTeam(pRole);

	if( E_Success == nRet )
	{
		// �ɹ����������ж����Ա��Ϣ
		tagNS_LeaveTeam send;
		send.dwRoleID = pRole->GetID();
		send.dwLeaderID = pTeam->GetMemID(0);
		send.dwErrorCode = nRet;
		pTeam->SendTeamMessage(&send, send.dwSize);

		// �������Ҫɾ������ɾ������
		if( pTeam->IsNeedDelete() )
		{
			DeleteTeam(pTeam);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// С�ӳ�Ա�ȼ��ı�
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnChangeLevel(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNS_RoleLevelChange* pRecv = (tagNS_RoleLevelChange*)pEventMessage;
	
	Role* pRole = g_roleMgr.GetRolePtrByID(dwSenderID);
	if( !P_VALID(pRole) ) return;

	// �ҵ�����
	Team* pTeam = m_mapTeam.Peek(pRole->GetTeamID());
	if( !P_VALID(pTeam) ) return;

	// ͬ������ȼ�
	pTeam->OnMemLevelChange(pRole);

	// ͬ����С�����
	pTeam->SendTeamMesOutBigVis(pRole, pRecv, pRecv->dwSize);	
}

//-------------------------------------------------------------------------------------------------------
// С�Ӷӳ�ͳ�����ı�
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnChangeLeaderRein(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagLeaderRein* pLeaderRein = (tagLeaderRein*)pEventMessage;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwSenderID);
	if( !P_VALID(pRole) ) return;

	Team* pTeam = m_mapTeam.Peek(pRole->GetTeamID());
	if( !P_VALID(pTeam) ) return;

	if( pTeam->IsLeader(pRole->GetID()) ) return;

	pTeam->CalExpFactor();
}

//-------------------------------------------------------------------------------------------------------
// ������������������Ҽ�һ��buff
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnAddAllRoleBuff(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagAllRoleBuff* pAllRoleBuff = (tagAllRoleBuff*)pEventMessage;

	g_roleMgr.AddAllRoleBuff(pAllRoleBuff->dwBuffTypeID);
}

//-------------------------------------------------------------------------------------------------------
// ����һ��С��
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnCreateTeam(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagCreateTeam* pCreateTeam = (tagCreateTeam*)pEventMessage;
	DWORD	dwSrcRoleID = pCreateTeam->dwSrcRoleID;
	DWORD	dwDesRoleID = pCreateTeam->dwDesRoleID;

	Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwSrcRoleID);
	if( !P_VALID(pSrcRole) ) return;

	Role* pDesRole = g_roleMgr.GetRolePtrByID(dwDesRoleID);
	if( !P_VALID(pDesRole) ) return;

	// ����Ƿ��ж�
	if( GT_INVALID != pSrcRole->GetTeamID() || GT_INVALID != pDesRole->GetTeamID() )
	{
		return;
	}

	Team* pTeam = CreateTeam(pSrcRole, pDesRole);

	// ����Է�ͬ�⣬�����жϳɹ�������ȫ�������Ϣ
	if( P_VALID(pTeam) )
	{
		// ���͸������������ɹ�
		tagNS_InviteReply send;
		send.bAgree = TRUE;
		send.dwErrorCode = E_Success;
		pTeam->ExportAllMemID(send.dwTeamMemID);

		pTeam->SendTeamMessage(&send, send.dwSize);

		// ���Ͷ�Ա��ʼ��Ϣ
		pTeam->SendRoleInitStateToTeam(pDesRole);

		// ���͸��¼�������
		pTeam->SendTeamState(pDesRole);

		// ����´������飬�����öӳ��Ķ�����Ϣ
		pTeam->SendTeamState(pSrcRole);
	}
}

//-------------------------------------------------------------------------------------------------------
// ����һ��С�Ӷ�Ա
//-------------------------------------------------------------------------------------------------------
VOID GroupMgr::OnAddMember(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagAddTeamMember* pCreateTeam = (tagAddTeamMember*)pEventMessage;
	DWORD	dwTeamID = pCreateTeam->dwTeamID;
	DWORD	dwDesRoleID = pCreateTeam->dwDesRoleID;

	Role* pDesRole = g_roleMgr.GetRolePtrByID(dwDesRoleID);
	if( !P_VALID(pDesRole) ) return;

	Team* pTeam = m_mapTeam.Peek(dwTeamID);
	if( !P_VALID(pTeam) ) return;

	Role* pSrcRole =  pTeam->GetMemPtr(0);
	if( !P_VALID(pSrcRole) ) return;

	pTeam->AddMember(pSrcRole, pDesRole);

	// ���͸������������ɹ�
	tagNS_InviteReply send;
	send.bAgree = TRUE;
	send.dwErrorCode = E_Success;
	pTeam->ExportAllMemID(send.dwTeamMemID);

	pTeam->SendTeamMessage(&send, send.dwSize);

	// ���Ͷ�Ա��ʼ��Ϣ
	pTeam->SendRoleInitStateToTeam(pDesRole);

	// ���͸��¼�������
	pTeam->SendTeamState(pDesRole);
}


// Jason
VOID GroupMgr::NotifyApplyToJoinTeam(DWORD RoleID, DWORD errcode,DWORD dwTeamLeaderID,DWORD dwApplicantID,BOOL bTeamLeaderAgree)
{
	Role* pSender	=	g_roleMgr.GetRolePtrByID(RoleID);
	if( !P_VALID(pSender) )
		return;
	tagNS_ApplyReplyRefuse msg;
	//msg.dwErrorCode		= errcode;
	//if( E_Team_Target_No_Team == errcode )
	//	msg.dwTeamLeaderID	= RoleID;
	//else
		msg.dwTeamLeaderID	= dwTeamLeaderID;
	//msg.dwApplicantID	= dwApplicantID;
	//msg.bTeamLeaderAgree= bTeamLeaderAgree;
	if( GT_VALID(dwTeamLeaderID) )
		g_roleMgr.GetRoleNameByID(dwTeamLeaderID, msg.szName);
	else
		memset(msg.szName,0,sizeof(TCHAR) * X_SHORT_NAME);
	msg.dwErrCode = errcode;
	pSender->SendMessage(&msg, msg.dwSize);
}

// Jason modified from method " OnInviteJoinTeam "
VOID GroupMgr::OnApplyJoinTeam(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNC_ApplyToJoinTeam* pRecv = (tagNC_ApplyToJoinTeam*)pEventMessage;
	//Э�����⣬��Ҫ����
	if( !P_VALID(pRecv) )
		return;

	DWORD	dwApplicantID	= dwSenderID;//pRecv->dwApplicantID;
	DWORD	dwTeamMemberID	= pRecv->dwTeamMemberID;

	if( dwApplicantID == dwTeamMemberID )
	{
		NotifyApplyToJoinTeam(dwApplicantID,E_Team_Hack_Tools_Prohibited);
		return;	// �����Լ�����ң�
	}
	//������
	Role* pApplicant	=	g_roleMgr.GetRolePtrByID(dwApplicantID);
	//�ӳ�Ա
	Role* pTeamMember	=	g_roleMgr.GetRolePtrByID(dwTeamMemberID);

	if( !P_VALID(pApplicant) )
	{
		NotifyApplyToJoinTeam(dwApplicantID,E_Team_Apply_Invalid_Param);
		return;
	}
	if( !P_VALID(pTeamMember) )
	{
		NotifyApplyToJoinTeam  (dwApplicantID,E_Team_Peer_Not_Online,dwTeamMemberID);
		return;
	}

	Map* pMap = pApplicant->GetMap();
	if( !P_VALID(pMap) )
	{
		NotifyApplyToJoinTeam(dwApplicantID,E_Team_Cannot_Find_Map);
		return ;
	}
	// �ж��Ƿ���ĳ��ͼ�������������
	if( E_Success != pMap->CanInviteJoinTeam() )
	{
		NotifyApplyToJoinTeam(dwApplicantID,E_Team_Map_CanNot_Invite);
		return ;
	}

	// ��������С�ӣ����ֹ
	DWORD dwTeamIDOfApplicant = 0, dwTeamIDOfDest = 0;
	dwTeamIDOfApplicant = pApplicant->GetTeamID();
	dwTeamIDOfDest		= pTeamMember->GetTeamID();
	if( GT_INVALID != dwTeamIDOfApplicant )
	{
		if( dwTeamIDOfApplicant == dwTeamIDOfDest )
			NotifyApplyToJoinTeam( dwApplicantID, E_Team_Already_In_Team );
		else
			NotifyApplyToJoinTeam( dwApplicantID, E_Team_Already_Have_Team );
		return ;
	}
	if( GT_INVALID == dwTeamIDOfDest )
	{
		//NotifyApplyToJoinTeam( dwApplicantID, E_Team_Target_No_Team, dwTeamMemberID );
		// �������뷽û�жӡ�
		tagNS_ApplyToJoinTeam msg;
		msg.dwApplicantID = dwApplicantID;
		Role* pTeamLeader = NULL;
		if( GT_VALID(dwApplicantID) )
			g_roleMgr.GetRoleNameByID(dwApplicantID, msg.szName);
		else
			memset(msg.szName,0,sizeof(TCHAR) * X_SHORT_NAME);
		msg.nLevel = pApplicant->GetLevel  ();

		pTeamMember->SendMessage  (&msg,msg.dwSize);

		return ;
	}

	Team* pTeam = m_mapTeam.Peek( dwTeamIDOfDest );

	if( !P_VALID(pTeam) )
	{
		NotifyApplyToJoinTeam( dwApplicantID, E_Team_Target_No_Team );
		return ;
	}
	//������
	if( pTeam->GetMemNum() >= MAX_TEAM_NUM )
	{
		NotifyApplyToJoinTeam( dwApplicantID, E_Team_Member_Full );
		return ;
	}

	//�õ��ӳ�id
	DWORD dwLeaderID = pTeam->GetMemID(0);

	tagNS_ApplyToJoinTeam msg;
	msg.dwApplicantID = dwApplicantID;
	Role* pTeamLeader = NULL;
	if( GT_VALID(dwApplicantID) )
		g_roleMgr.GetRoleNameByID(dwApplicantID, msg.szName);
	else
		memset(msg.szName,0,sizeof(TCHAR) * X_SHORT_NAME);

	//if( GT_VALID(dwApplicantID) )
	//{
	//	msg.nLevel = g_roleMgr.GetRolePtrByID(dwApplicantID)->GetLevel();
	//}

	if( dwLeaderID == dwTeamMemberID )//���˼�Ϊ�ӳ�
	{
		pTeamLeader = pTeamMember ;
	}
	else//��ͨ��Ա
	{
		//�ҵ��ӳ�
		pTeamLeader = g_roleMgr.GetRolePtrByID( dwLeaderID );
	}

	//ת����Ϣ
	if( P_VALID(pTeamLeader) )
	{
		msg.nLevel = pApplicant->GetLevel  ();
		pTeamLeader->SendMessage(&msg,msg.dwSize);
	}

}
// Jason modified from method " OnInviteJoinTeamReply "
VOID GroupMgr::OnReplyForApplingToJoinTeam(DWORD dwSenderID, LPVOID pEventMessage)
{
	tagNC_ApplyReply * pRecv = (tagNC_ApplyReply*)pEventMessage;
	if( !P_VALID(pRecv) )
		return ;

	DWORD	dwApplicantID	= pRecv->dwApplicantID;
	DWORD dwLeaderRoleID = /*pRecv->dwTeamLeaderID*/ dwSenderID ;
	//if( dwReplyRoleID != dwSenderID )
	//	return;

	Role* pLeaderRole = g_roleMgr.GetRolePtrByID(dwLeaderRoleID);
	Role* pApplicant	=	g_roleMgr.GetRolePtrByID(dwApplicantID);

	if( !P_VALID(pLeaderRole) || !P_VALID(pApplicant) ) 
		return;

	//DWORD dwInviteRoleID = pReplyRole->GetTeamInvite();
	//if( GT_INVALID == dwInviteRoleID ) return;

	//pReplyRole->SetTeamInvite(GT_INVALID);	// ���ñ������ߵ�������ID

	//Role* pSrcRole = g_roleMgr.GetRolePtrByID(dwInviteRoleID);
	//if( !P_VALID(pSrcRole) ) return;

	// ������Ӧ��û�жӣ���ֹ��Ұɣ������ǲ����������Ҫ�����
	// �ӳ�Ӧ���ж�
	DWORD dwLeaderTeamID = pLeaderRole->GetTeamID();
	if( GT_INVALID != pApplicant->GetTeamID() /*|| GT_INVALID == dwLeaderTeamID*/ )
		return;

	// �ж��Ƿ���ĳ��ͼ�������������
	Map* pMap = pLeaderRole->GetMap();
	if( !P_VALID(pMap) )
		return ;
	if( E_Success != pMap->CanInviteJoinTeam() )
		return;

	// ��ʼ���ж�
	INT nRet = -1;
	BOOL bNewGroup = FALSE;
	Team* pTeam = NULL;


	// �ӳ�ͬ����Ҽ���
	if( pRecv->bTeamLeaderAgree )
	{
		if(GT_INVALID == dwLeaderTeamID)
		{
			//������
			pTeam = CreateTeam(pLeaderRole, pApplicant);
			nRet = E_Success;
			bNewGroup = TRUE;
		}
		else
		{
			pTeam = m_mapTeam.Peek( dwLeaderTeamID );
			// addmember����һЩ�����жϵġ�������������
			nRet = pTeam->AddMember( pLeaderRole, pApplicant );
		}
	}

	// ����ӳ�ͬ�⣬�����жϳɹ�������ȫ�������Ϣ
	if( E_Success == nRet )
	{
		if( !P_VALID(pTeam) ) 
			return;

		// ���͸������������ɹ�
		tagNS_InviteReply send;
		send.bAgree = TRUE;
		send.dwErrorCode = nRet;
		pTeam->ExportAllMemID(send.dwTeamMemID);

		pTeam->SendTeamMessage(&send, send.dwSize);

		// �����¶�Ա��ʼ��Ϣ
		pTeam->SendRoleInitStateToTeam( pApplicant );

		// ���͸��¼�������
		pTeam->SendTeamState( pApplicant );

		// ����´������飬�����öӳ��Ķ�����Ϣ
		if( bNewGroup ) pTeam->SendTeamState(pLeaderRole);
	}
	// �����ͬ�⣬�����ж�ʧ�ܣ���ֻ���͸����뷽
	else
	{
		//tagNS_ApplyReplyRefuse msg;
		//msg.dwTeamLeaderID = dwLeaderRoleID;
		//msg.dwApplicantID = dwApplicantID;
		//msg.bTeamLeaderAgree = FALSE;
		//if (GT_VALID(dwLeaderRoleID))
		//	g_roleMgr.GetRoleNameByNameID(dwLeaderRoleID, msg.szName);
		//else
		//	memset(msg.szName,0,sizeof(TCHAR) * X_SHORT_NAME);
		//pApplicant->SendMessage(&msg, msg.dwSize);
		NotifyApplyToJoinTeam(dwApplicantID,E_Team_Refuse,dwLeaderRoleID);
	}
}


GroupMgr g_groupMgr;