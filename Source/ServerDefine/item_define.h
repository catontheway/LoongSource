//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_define.h
// author: 
// actor:
// data: 2008-10-17
// last:
// brief: ����������Ʒ��ؽṹ
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ����updateʱ��(��λ��tick)
//-----------------------------------------------------------------------------
const INT ITEM_UPDATE_TIME		= 60 * TICK_PER_SECOND;		// 60s

//-----------------------------------------------------------------------------
// װ��B���������ɸ���[0��10000]
//-----------------------------------------------------------------------------
const INT g_nEquipSpecAttPct[X_EQUIP_SPECATT_NUM] = 
{
	800,	400,	200,	800,	400,
	200,	800,	400,	200,	100,
	800,	400,	200,	100,	500,
	500,	500,	500,	500,	500,
	100,	500,	500,	100
};

//-----------------------------------------------------------------------------
// ͬһ��������Ʒ�ƶ����ɷ��������Ĳ���
//-----------------------------------------------------------------------------
struct tagItemMove
{
	tagItem	*pItem2;		// Ŀ��λ���ϵ���Ʒ
	INT16	n16NumRes1;		// ���ƶ���Ʒ�����ո���
	INT16	n16NumRes2;		// Ŀ��λ������Ʒ�����ո���
	bool	bCreateItem;	// Ŀ��λ���ϵ���Ʒ�Ƿ�Ϊ�´�����(���ʱΪtrue)
	bool	bOverlap;		// �Ƿ�ѵ�
	bool	bChangePos;		// λ���Ƿ��ƶ�

	tagItemMove()
	{
		Init();
	}
	
	VOID Init()
	{
		pItem2		= NULL;
		n16NumRes1	= 0;
		n16NumRes2	= 0;
		bCreateItem = FALSE;
		bOverlap	= FALSE;
		bChangePos	= TRUE;
	}
};

//-----------------------------------------------------------------------------
// �����ɫ����ʱ����µ���Ʒ����
//-----------------------------------------------------------------------------
struct tagItemUpdate
{
	INT64		n64Serial;
	DWORD		dwOwnerID;
	DWORD		dwAccountID;
	INT32		nUseTimes;		// ��Ʒ�ϼ���ʹ�ô���
	INT16		n16Num;			// ��Ʒ����
	INT16		n16Index;		// ������λ��
	BYTE		byConType;		// ��������
	BYTE		byBindType;		// ������
};

struct tagEquipSpecUpdate
{
	INT64			n64Serial;
	tagEquipSpec	equipSpec;
};

//-----------------------------------------------------------------------------
// IM ��ƷӰ������ö��
//-----------------------------------------------------------------------------
enum EIMEffect
{
	EIME_Null			= 0,

	EIME_Color			= 1,	// ��ɫ
	EIME_ComAdvance		= 2,	// ��߳ɹ�����
	EIME_ProtectSign	= 3,	// ���׷���ǿ��ʧ��ʱ������װ��Ǳ��ֵ��
};

//-----------------------------------------------------------------------------
// IM ��ƷӰ��
//-----------------------------------------------------------------------------
struct tagIMEffect
{
	EIMEffect	eEffect;	// Ӱ�����
	DWORD		dwParam1;	// Ӱ��ֵ
	DWORD		dwParam2;
	tagIMEffect()
	{
		eEffect = EIME_Null;
		dwParam1 = 0;
		dwParam2 = 0;
	}
};

//-----------------------------------------------------------------------------
// ��Ʒ��ȴ��Ϣ
//-----------------------------------------------------------------------------
struct tagCDInfo
{
	DWORD		dwElapseTime;
	DWORD		dwRemainTime;
};


// Jason 2009-12-8 ���ʯ���ýṹ
struct tagPhilosophersStoneConfig
{
	union unionKey
	{
		struct tagPair
		{
			INT nStoneType;
			DWORD dwSrcItemTypeID;
		} m_Pair;
		INT64 m_n64Key;
	} uniKey;
	DWORD dwDestItemTypeID;
};

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ���ݵĽṹ
//-----------------------------------------------------------------------------
struct tagItemData
{
	INT64	n64Serial;
	DWORD	dwTypeID;
};

#pragma pack(pop)
