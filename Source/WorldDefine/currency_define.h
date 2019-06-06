//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: currency_define.h
// author: Sxg
// actor:
// data: 2008-08-14
// last:
// brief: ����ϵͳ�г�������
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT32 GOLD2SILVER					= 10000;

const INT32 MAX_BAG_YUANBAO_NUM			= 999999;
const INT32 MAX_BAG_GOLD_NUM			= 999999;
const INT64 MAX_BAG_SILVER_NUM			= (INT64)MAX_BAG_GOLD_NUM * GOLD2SILVER + (GOLD2SILVER - 1);

const INT32 MAX_BAIBAO_YUANBAO_NUM		= 999999;
const INT32 MAX_WARE_GOLD_NUM			= 999999;
const INT64 MAX_WARE_SILVER_NUM			= (INT64)MAX_WARE_GOLD_NUM * GOLD2SILVER + (GOLD2SILVER - 1);

const INT32 MAX_EXCHANGE_VOLUME_NUM		= 99999999;


//-----------------------------------------------------------------------------
// ��
//-----------------------------------------------------------------------------
#define MGold2Silver(nGold)				((INT64)(nGold) * GOLD2SILVER)

#define MSilver2DBGold(n64Silver)		(INT32)((n64Silver) / GOLD2SILVER)
#define MSilver2DBSilver(n64Silver)		(INT32)((n64Silver) % GOLD2SILVER)

//-----------------------------------------------------------------------------
// �ṹ
//-----------------------------------------------------------------------------

// ��Ǯ
struct tagMoney 
{
	INT		nGold;
	INT		nSilver;
};