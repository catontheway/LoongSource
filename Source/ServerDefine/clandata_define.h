//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: clandata_define.h
// author: 
// actor:
// data: 2009-02-19
// last:
// brief: ������Ϣ���ݿ�洢�ṹ
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push,1)
#include "../WorldDefine/reputation.h"
struct tagDBReputeData 
{
	INT8			m_n8ActCount[ECLT_NUM];					// ʣ�༤�����
	INT32			m_nReputation[ECLT_NUM];				// ����ֵ
	INT32			m_nContribution[ECLT_NUM];				// ����ֵ
	tagDWORDTime	m_dwtLastResetTime;						// �ϴ���������ʱ��
	UINT16			m_u16FameMask;							// �����ó�Ա
	tagDBReputeData(){	ZeroMemory(this, sizeof(*this));}
};
#pragma pack(pop)