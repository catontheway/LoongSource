//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: strength.h
// author: zhangrong
// actor:
// data: 2009-9-7
// last:
// brief: ʵ��ϵͳ
//-------------------------------------------------------------------------------

#pragma once
#include "mutex.h"
#include "../ServerDefine/strength_define.h"

class StrengthRankings
{
	TMap<INT, tagStrengthRoleNode*>		m_mapRankings;	// ���µ����а�ǰ500����ʵ��ֵ����100�ģ�
	Mutex								m_Lock;
public:
	// �����ʼ��ʵ�����а�
	BOOL Init();
	// ��ʼ��ʵ����
	VOID InitRankings(tagNetCmd* pMsg);
	// ����ʵ����ÿ��24�����
	//VOID UpdateWorldRankings();

	TMap<INT, tagStrengthRoleNode*>		GetGlobalRankings()	{ return m_mapRankings; }
};

extern StrengthRankings g_StrengthRankings;