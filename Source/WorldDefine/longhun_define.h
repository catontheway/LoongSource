//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: longhun_define.h
// author: 
// actor:
// data: 2009-03-13
// last: 
// brief: ������ض���
//-----------------------------------------------------------------------------
#pragma once



#include "container_define.h"
//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT X_LONGHUN_EFFECT_POS_NUM		= 2;		// ָ�������ܹ�����Ĳ������������Ը���

//-----------------------------------------------------------------------------
// ָ��������������������Щ��������
//-----------------------------------------------------------------------------
struct tagLongHunActivePos
{
	EEquipPos		eOuter;
	EEquipPos		eInner[2];
};

const tagLongHunActivePos g_sLongHunActivePos[EEP_MaxEquip] = 
{
	{EEP_Wrist,		EEP_Wrist,		EEP_Feet},		// 0, ͷ��
	{EEP_Null,		EEP_Null,		EEP_Null},		// 1, �沿
	{EEP_Head,		EEP_Head,		EEP_Wrist},		// 2, ����
	{EEP_Body,		EEP_Body,		EEP_Head},		// 3, ����
	{EEP_Finger2,	EEP_Finger2,	EEP_Waist},		// 4, ����
	{EEP_Feet,		EEP_Feet,		EEP_Legs},		// 5, ��
	{EEP_Legs,		EEP_Legs,		EEP_Body},		// 6, �㲿

	{EEP_Neck,		EEP_Neck,		EEP_Back},		// 7, ��ָ
	{EEP_Waist,		EEP_Waist,		EEP_Finger1},	// 8, ��ָ
	{EEP_Finger1,	EEP_Finger1,	EEP_Neck},		// 9, ����
	{EEP_Back,		EEP_Back,		EEP_Finger2},	// 10, ����

	{EEP_LeftHand,	EEP_LeftHand,	EEP_RightHand},	// 11, ��������
	{EEP_RightHand,	EEP_RightHand,	EEP_LeftHand}	// 12, ��������
};

//-----------------------------------------------------------------------------
// ָ�������ܹ�������Щ��������������
//-----------------------------------------------------------------------------
struct tagLongHunCanActivePos
{
	EEquipPos		eEquipPos[X_LONGHUN_EFFECT_POS_NUM];

	tagLongHunCanActivePos()
	{
		this->eEquipPos[0] = EEP_Null;
		this->eEquipPos[1] = EEP_Null;
	}
	
	VOID Set(EEquipPos eEquipPos)
	{
		if(EEP_Null == this->eEquipPos[0])
		{
			this->eEquipPos[0] = eEquipPos;
			return;
		}

		this->eEquipPos[1] = eEquipPos;
	}
};

extern tagLongHunCanActivePos g_sLongHunCanActivePos[EEP_MaxEquip];


//-----------------------------------------------------------------------------
// ������������
//-----------------------------------------------------------------------------
enum ELongHunType
{
	ELHT_Start		= 0,

	ELHT_Outer		= 0,	// ������
	ELHT_Inner		= 1,	// ������

	ELHT_End		= 1,
};

const INT32 X_LONGHUN_TYPE_NUM = ELHT_End - ELHT_Start + 1;


enum ELongHunTypeEx
{
	ELHTE_Null			= -1,

	ELHTE_Start			= 0,

	ELHTE_PowerGather	= 0,	// ����
	ELHTE_PowerBurst	= 1,	// ����
	ELHTE_CriticalHit	= 2,	// ��ɱ

	ELHTE_End			= 2,
};

const INT X_LONGHUN_TYPEEX_NUM = ELHTE_End - ELHTE_Start + 1;

//-----------------------------------------------------------------------------
// ����������������ʱ����ͬ��λ��Ӧ�������ɵ�������������
//-----------------------------------------------------------------------------
const ELongHunTypeEx g_eEquipGenLongHunTypeEx[EEP_MaxEquip] = 
{
	ELHTE_PowerGather,		// 0, ͷ��
	ELHTE_Null,				// 1, �沿
	ELHTE_PowerGather,		// 2, ����
	ELHTE_PowerGather,		// 3, ����
	ELHTE_PowerBurst,		// 4, ����
	ELHTE_PowerGather,		// 5, ��
	ELHTE_PowerGather,		// 6, �㲿

	ELHTE_PowerBurst,		// 7, ��ָ
	ELHTE_PowerBurst,		// 8, ��ָ
	ELHTE_PowerBurst,		// 9, ����
	ELHTE_PowerBurst,		// 10, ����

	ELHTE_CriticalHit,		// 11, ��������
	ELHTE_CriticalHit		// 12, ��������
};

//-----------------------------------------------------------------------------
// ��������ԭ��(��̬����)
//-----------------------------------------------------------------------------
struct tagLongHunProto
{
	DWORD			dwID;								// id
	ELongHunType	eType;								// ����
	ELongHunTypeEx	eTypeEx;							// ��������������ɱ
	DWORD			dwTriggerID;						// trigger id
	DWORD			dwBuffID1;							// buff id
	DWORD			dwBuffID2;							// buff id
	bool			bQltyCanGen[X_EQUIP_QUALITY_NUM];	// ָ��Ʒ���Ƿ��������ף��ƣ��̣�������
	BYTE			byWuXingActive[X_WUXING_TYPE_NUM];	// ������������ľ��ˮ������
};

//------------------------------------------------------------------------------
// ��������ԭ�ͣ���̬���ԣ��ͻ��˰棩
//------------------------------------------------------------------------------
struct tagLongHunProtoClient 
	: public tagLongHunProto
{
	TCHAR			szName[X_SHORT_NAME];				// ����
	TCHAR			szDesc[X_LONG_NAME];				// ����
};