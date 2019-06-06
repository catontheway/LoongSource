//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: func_npc_define.h
// author: 
// actor:
// data: 2008-11-10
// last:
// brief: ְ��NPC���ԭ�Ͷ���
//-----------------------------------------------------------------------------
#pragma once

#include "cost_type_define.h"

/****************************** ��վ&Ǭ��ʯ��� *****************************/

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT32 MAX_DAK_SITE_NUM		= 20;	// ��վ�ܹ����͵���Ŀ�ĵظ���

//-----------------------------------------------------------------------------
// �ṹ
//-----------------------------------------------------------------------------
struct tagDakSite
{
	DWORD			dwMapID;	// map���
	FLOAT			fX;			// (x,y,z)
	FLOAT			fY;
	FLOAT			fZ;
	ECurCostType	eCostType;	// ��������
	INT32			nCostVal;	// ����ֵ

	tagDakSite() { ZeroMemory(this, sizeof(*this)); dwMapID = GT_INVALID; }
};

struct tagDakProto
{
	DWORD		dwID;		// ��վ���
	tagDakSite	dakSite[MAX_DAK_SITE_NUM];
};