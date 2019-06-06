//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: stall_define.h
// author: 
// actor:
// data: 2008-12-17
// last:
// brief: ��̯
//-----------------------------------------------------------------------------
#pragma once

#include "currency_define.h"

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT STALL_MIN_ROLE_LEVEL			= 1;				// ��Ͱ�̯�ȼ�
const INT STALL_MAX_DISPLAY				= 20;				// ̯λ������

const INT64 STALL_MIN_SOLD_SLIVER		= 1;
const INT64 STALL_MAX_SOLD_SLIVER		= MGold2Silver(999999) + 9999;

const INT STALL_MAX_TITLE_NUM			= 10 + 1;			// ̯λ��������(�ַ���+1���ַ���������)
const INT STALL_MAX_ADTEXT_NUM			= 20 + 1;			// ̯λ�������

const INT STALL_MAX_TAX					= MGold2Silver(8000);// һ��̯λ��˰�����ֵ

const INT VIP_STALL_MAX_NUM				= 40;				// VIP̯λ�������

//-------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------
enum
{
	E_Stall_ItemNotFound_InBag		= 1,	// û�дӱ������ҵ���Ʒ
	E_Stall_ItemCanNot_Sold			= 2,	// ָ����Ʒ���ɰ�̯����

	E_Stall_Role_InStall			= 5,	// ��ɫ�Ѵ��ڰ�̯װ��
	E_Stall_Role_Level_2Low			= 6,	// ��ɫ�ȼ�δ�ﵽ30��
	E_Stall_Area_NotAllowed			= 7,	// ���ڳ����������̯
	E_Stall_Space_NotEnough			= 8,	// ��ɫ4������֮����̯λ
	E_Stall_RoleState_NotPermit		= 9,	// ����ս�ˡ����׺�PVP״̬���ɰ�̯

	E_Stall_Role_Pull				= 20,	// ����̯
	E_Stall_Goods_BeSold			= 21,	// ������
	E_Stall_Goods_Refresh			= 22,	// ������Ʒ�Ѹ���
	E_Stall_Goods_Refresh_Price		= 23,	// �ۼ��Ѹ���
	E_Stall_GoodsNum_NotEnough		= 24,	// ������Ʒ��������
	E_Stall_Goods_Empty				= 25,	// û�д�����Ʒ
	E_Stall_Goods_InUsed			= 26,	// ��Ʒ�Ѵ���ʹ��״̬
	E_Stall_Goods_NotInBag			= 27,	// û���ڱ������ҵ�ָ����Ʒ(����Ʒ��ʹ�ã��������ƶ���)

	E_Stall_CustomerMoney_NotEnough	= 30,	// �����߽�Ǯ����
	E_Stall_CustomerBug_NoFreeSpace	= 31,	// �����߱����ռ䲻��

	E_Stall_VIP_Not_Init			= 40,	// ̯λû�г�ʼ��
	E_Stall_VIP_Own_Another			= 41,	// �Ѿ�����������̯λ
	E_Stall_VIP_Info_NotExist		= 42,	// ̯λ��Ϣ������
	E_Stall_VIP_Already_Rent		= 43,	// ��̯λ�Ѿ�������
	E_Stall_VIP_Rent_Error			= 44,	// ������
	E_Stall_VIP_For_Hire			= 45,	// ̯λ�ڴ���״̬
	E_Stall_VIP_GetGoods_Needless	= 46,	// ��Ʒ��Ϣ����Ҫ����
	E_Stall_VIP_Self_Stall			= 47,	// ���ɲ����Լ���̯λ

//	E_Stall_Cannot_Find_StallRole	= 50,	// �Ҳ���̯��
};

//-----------------------------------------------------------------------------
// ̯λ��Ʒ�ṹ
//-----------------------------------------------------------------------------
struct tagMsgStallGoods
{
	INT64	n64UnitPrice;		// ����
	BYTE	byIndex;			// ���ڸ�������(��0��ʼ)
	BYTE	byItem;				// 1��ʾ��Ʒ��0��ʾװ��
	BYTE	byData[1];			// ��Ʒ��װ������
};

//-----------------------------------------------------------------------------
// ̯λģ��ö��
//-----------------------------------------------------------------------------
enum EStallMode
{
	ESM_Wood		= 0,	// ľ����
	ESM_Copper		= 1,	// ͭ����
	ESM_Silver		= 2,	// ������
	ESM_Gold		= 3,	// �ƽ�����
	ESM_Platinum	= 4,	// �׽�����
	ESM_Diamond		= 5,	// ��ʯ����
};

//-----------------------------------------------------------------------------
// VIP̯λ״̬
//-----------------------------------------------------------------------------
enum EVIPStallStatus
{
	EVSS_Open,				// Ӫҵ��
	EVSS_Close,				// ��̯
	EVSS_ForHire,			// ����

	EVSS_End
};

//-----------------------------------------------------------------------------
// VIP̯λ�ṹ
//-----------------------------------------------------------------------------
struct tagVIPStall 
{
	EVIPStallStatus	eStallStatus;						// ̯λ״̬
	DWORD			dwLastGoodsTime;					// ��Ʒ������ʱ��
	DWORD			dwLastUpdateTime;					// ̯λ��Ϣ����ʱ��
	DWORD			dwOwnerID;							// ̯��ID
	INT32			nRemainTime;						// ̯λ����ʣ��ʱ��
	INT32			nRent;								// ���
	BYTE			byStallIndex;						// ̯λ���
	TCHAR			szStallTitle[STALL_MAX_TITLE_NUM];	// ̯λ����

	VOID Reset()
	{
		eStallStatus		= EVSS_ForHire;
		dwLastGoodsTime		= GT_INVALID;
		dwLastUpdateTime	= GT_INVALID;
		dwOwnerID			= GT_INVALID;
		nRemainTime			= GT_INVALID;
		ZeroMemory(szStallTitle, sizeof(TCHAR)*STALL_MAX_TITLE_NUM);
	}
};

//-----------------------------------------------------------------------------
// ��
//-----------------------------------------------------------------------------

// ���ݽ�˰�ܶ����̯λ����ģ�͵ȼ���˰�ռ���
#define MCalStallMode(byLevel, fDecTax, nTotalTax)		\
	if(nTotalTax >= STALL_MAX_TAX)				\
	{											\
		byLevel = ESM_Diamond;					\
		fDecTax = 0.0f;							\
	}											\
	else if(nTotalTax >= MGold2Silver(2200))	\
	{											\
		byLevel = ESM_Platinum;					\
		fDecTax = 0.01f;						\
	}											\
	else if(nTotalTax >= MGold2Silver(700))		\
	{											\
		byLevel = ESM_Gold;						\
		fDecTax = 0.02f;						\
	}											\
	else if(nTotalTax >= MGold2Silver(210))		\
	{											\
		byLevel = ESM_Silver;					\
		fDecTax = 0.03f;						\
	}											\
	else if(nTotalTax >= MGold2Silver(30))		\
	{											\
		byLevel = ESM_Copper;					\
		fDecTax = 0.04f;						\
	}											\
	else										\
	{											\
		byLevel = ESM_Wood;						\
		fDecTax = 0.05f;						\
	}

// ��ǰ�ȼ��������辭��
#define MCalStallLevelExp(byLevel)	((byLevel) * (byLevel) * 2000)

//-----------------------------------------------------------------------------
#pragma pack(pop)