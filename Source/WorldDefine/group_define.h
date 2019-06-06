//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: group_define.h
// actor:
// data: 2008-11-20
// brief: �������Զ���
//-----------------------------------------------------------------------------
#pragma once

//------------------------------------------------------------------------
// ����ʰȡ����
//------------------------------------------------------------------------

const INT      MAX_TEAM_NUM					= 6;		// С���������
const DWORD    TEAM_SYN_TICK				= TICK_PER_SECOND * 5;
const DWORD    TEAM_DISPLAY_NUM				= 3;

enum EPickMode
{
	EPUM_Free,										// ����ʰȡ
	EPUM_Order,										// ˳��ʰȡ
};

enum ERefuseApplyMode
{
	ERAM_Accept,								    // ��������
	ERAM_Refuse,									// �ܾ�����
};

struct tagLeaderRein
{
	INT				nLeaderRein;					// �ӳ�ͳ����
};

struct tagAllRoleBuff
{
	DWORD			dwBuffTypeID;							
};

struct tagCreateTeam
{
	DWORD			dwSrcRoleID;
	DWORD			dwDesRoleID;
};

struct tagAddTeamMember
{
	DWORD			dwTeamID;
	DWORD			dwDesRoleID;
};