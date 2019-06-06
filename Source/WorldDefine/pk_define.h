//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pk_define.h
// author: Aslan
// actor:
// data: 2008-11-11
// last:
// brief: PK��ز����趨
//-----------------------------------------------------------------------------
#pragma once

#include "RoleDefine.h"

#pragma pack(push, 1)

const INT MAX_HOSTILITY							=	9;							// ����������ֵ
const INT DEAD_PENALTY_LEVEL					=	20;							// ��������ͷ��ĵȼ�����
const INT SAFE_GUARD_FORCE_LEVEL				=	30;							// ���봦��PK�����ĵȼ�����
const INT SAFE_GUARD_FREE_LEVEL					=	120;						// �������л�PK�����ĵȼ�����
const INT OPEN_SAFE_GUARD_TIME					=	120*60;						// ���ϴ��л�������ģʽ���ٴο���PK��������Ҫ�ȴ���ʱ�䣨��λ���룩

const INT UNSET_PK_TICK							=	120*TICK_PER_SECOND;		// ��������״̬����Ҫ�೤ʱ��ָ���δ����״̬����λ���룩
const INT UNSET_PVP_TICK						=	30*TICK_PER_SECOND;			// PVP״̬�£���Ҫ�೤ʱ��ָ���δPVP״̬����λ���룩
const INT REVIVE_UNSETSAFEGUARD_TICK			=	10*60*TICK_PER_SECOND;		// ����״̬�������������������0����سǸ���ʱ����pk����״̬��ʱ�䣨��λ���룩

const INT PK_STATE_MORALITY[ERPKS_End]			=	{-30, -100, -1000, -10000};	// �����׶ε�PK״̬��Ӧ�ĵ���ֵ

const INT KILL_MORALITY_DEC[ERPKS_End]			=	{-10, -5, -1, 0};			// ��ɱ����PK״̬��Ҷ�Ӧ�ĵ���ֵ����

const INT LOOT_RATE_PER_PK_STATE[ERPKS_End]		=	{0, 3000, 7000, 10000};		// ��ͬPK״̬���������������Ʒ��װ���ļ���

// ��ͬ����ֵ�¸������ӵĳͷ�buff id
const INT HOSTILITY_DEADPENALTY_BUFF_ID[MAX_HOSTILITY]		=	{2022301, 2022401, 2022501, 2022601, 2022701, 2022801, 2022901, 2023001, 2023101};					

#pragma pack(pop)