//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: famehall_define.h
// author: xlguo
// actor:
// data: 2009-02-26
// last:
// brief: ������ͨ���������û����ṹ
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)
#include "../WorldDefine/famehall_define.h"
enum ETReasureState
{
	ETRS_NULL		= -1,
	ETRS_BEGIN		= 0,

	ETRS_UNACT		= 0,		// δ����
	ETRS_ACT		= 1,		// ����

	ETRS_END		,
	ETRS_NUM		= ETRS_END,
};

// ���������ÿ��������Ŀ
const UINT16 MAX_ENTER_FAMEHALL_SNAP_NUM	= 50;

// ���������ý�ɫ�����Ŀ
const UINT16 MAX_ENTER_FAMEHALL_ROLE_NUM	= 5000;

// �����䱦�����Ŀ
const UINT16 MAX_ACT_TREASURE_NUM			= CLAN_TREASURE_NUM;


//----------------------------------------------------------------------------
// ���ݿ�洢�ṹ
//----------------------------------------------------------------------------
struct tagFameHallEnterSnap
{
	DWORD			dwEnterNameID;
	DWORD			dwRoleID;
	tagDWORDTime	dwtEnterTime;

	BOOL operator ==(const tagFameHallEnterSnap& rhs)
	{
		// �������RoleID����
		return dwRoleID == rhs.dwRoleID /*&& dwEnterNameID == rhs.dwEnterNameID*/;
	}
};



#pragma pack(pop)