//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: container_define.h
// author: 
// actor:
// data: 2008-8-4
// last:
// brief: ����װ���������ҡ��ֿ����ض���
//--------------------------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
const INT32 MAX_BAG_NUM				= 7;							// �������ҳ��
const INT32 SPACE_ONE_BAG			= 20;							// ������������
const INT32 SPACE_ALL_BAG			= SPACE_ONE_BAG * MAX_BAG_NUM;	// ��󱳰�����

const INT32 SPACE_QUEST_BAG			= 20;							// ������Ʒ��������	
const INT32 SPACE_BAIBAO_BAG		= 20;							// �ٱ���������

const INT32 MAX_BAIBAO_LOG_NUM		= 40;							// �ٱ������Log��

const INT32 MAX_WARE_NUM			= 10;							// �ֿ����ҳ��
const INT32 SPACE_ONE_WARE			= 24;							// �����ֿ����
const INT32 SPACE_ALL_WARE			= SPACE_ONE_WARE * MAX_WARE_NUM;// ���ֿ����

const INT32 MIN_PASSWORD_NUM		= 6;							// ������С����
const INT32 MAX_PASSWORD_NUM		= 14;							// ������󳤶�

const INT32 SPACE_WARE_PER_EXTEND	= 6;							// ��ɫ�ֿ�һ�����������
const INT32 SPACE_BAG_PER_EXTEND	= 5;							// ����һ�����������

const INT32 MAX_WARE_EX_TIMES_SILVER= 4;							// ʹ����Ϸ�����������Ĳֿ����
const INT32 MAX_BAG_EX_TIMES_SILVER	= 4;							// ʹ����Ϸ�����������ı�������

const INT32 MAX_GUILDWARE_NUM       =   5;                          // ���ɲֿ����ҳ��
const INT32 SPACE_ONE_GUILDWARE     =   35;                         // �������ɲֿ����
const INT32 SPACE_ALL_GUILDWARE     =   SPACE_ONE_GUILDWARE * MAX_GUILDWARE_NUM;


//-----------------------------------------------------------------------------
// ö�ٶ���
//-----------------------------------------------------------------------------
// ��������
enum EItemConType
{
	EICT_Start		= 0,
	EICT_Null		= 0,

	EICT_Bag		= 1, // ����
	EICT_Quest		= 2, // ������Ʒ��
	EICT_Baibao		= 3, // �ٱ���
	EICT_RoleWare	= 4, // ��ɫ�ֿ�
	EICT_Equip		= 5, // װ����

	EICT_Shop		= 6, // �̵�
	EICT_Ground		= 7, // ����

    EICT_PetEquip   = 8, // ����װ����

	EICT_GuildWare	= 9, // ���ɲֿ�

	EICT_End		= 10,
};

// װ����
enum EEquipPos	// װ����λ
{
	EEP_Null			= -1,
	EEP_Start			= -1, 

	EEP_Equip_Start		= 0,
	EEP_Head			= 0, // ͷ��
	EEP_Face			= 1, // �沿
	EEP_Body			= 2, // ����
	EEP_Legs			= 3, // ����
	EEP_Back			= 4, // ����
	EEP_Wrist			= 5, // ��
	EEP_Feet			= 6, // �㲿

	EEP_Finger1			= 7, // ��ָ
	EEP_Finger2			= 8, // ��ָ
	EEP_Waist			= 9, // ����
	EEP_Neck			= 10, // ����

	EEP_RightHand		= 11, // ��������
	EEP_LeftHand		= 12, // ��������
	EEP_Equip_End		= 12,

	EEP_MaxEquip		= 13, // װ������λ��

	EEP_FashionHead		= 13, // ͷ��
	EEP_FashionFace		= 14, // �沿
	EEP_FashionBody		= 15, // ����
	EEP_FashionLegs		= 16, // ����
	EEP_FashionBack		= 17, // ����
	EEP_FashionWrist	= 18, // ��
	EEP_FashionFeet		= 19, // �㲿

	EEP_End				= 20,

};

const INT32 X_EQUIP_BAR_SIZE = EEP_End - EEP_Start - 1;
#define MIsInEquipPos(n16Pos)	(n16Pos > EEP_Start && n16Pos < EEP_End)

// ����װ����λ
enum EWeaponPos
{
	EWP_NULL			= 0,	// ��
	EWP_Waist			= 1,	// ����
	EWP_WaistBack		= 2,	// ����
	EWP_Back			= 3,	// ����
	EWP_Hand			= 4,	// �ֲ�
};

const INT32 X_WEAPONPOS_NUM = EWP_Hand + 1;

// �ٱ�����¼����
enum EBaiBaoRecordType
{
	EBBRT_System,			// ϵͳ����
	EBBRT_Mall,				// �̳���Ʒ
	EBBRT_Friend,			// ��������
	EBBRT_Myself,			// �Լ���ȡ
	EBBRT_GroupPurchase,	// �����Ź�
	EBBRT_VipNetBarGift,	// ����������Ʒ
};

//-----------------------------------------------------------------------------
// �ṹ
//-----------------------------------------------------------------------------
// �Զ�����
struct tagItemOrder
{
	INT16	n16OldIndex;
	INT16	n16NewIndex;
};

// �ٱ�����¼
struct tagBaiBaoRecord
{
	INT16	n16Size;		// ������¼����
	INT16	n16Type;		// EBaiBaoRecordType
	DWORD	dwTypeID;		// ��Ʒ��TypeID
	DWORD	dwRoleID;		// ������ҵ�ID��GT_INVALIDΪϵͳ����
	DWORD	dwTime;			// ����ʱ�� tagDWORDTime
	TCHAR	szWords[1];		// ����
};

//-----------------------------------------------------------------------------
// ��
//-----------------------------------------------------------------------------

// �ֿ����ݴ���=����ǰ�ֿ��Կ�����λ��-24��/6
#define MCalWareExTimes(n16CurWareSpace)	((n16CurWareSpace - 24) / 6)

// ��ʹ����Ϸ�����䣬�շ�=���ֿ����ݴ���+1��^2��3000[��λ��Ϸ��]
#define MCalWareExSilver(nExTimes)	((nExTimes + 1) * (nExTimes + 1) * 3000)

// ��ʹ��Ԫ�����䣬���շ�=���ֿ����ݴ���+1����5[��λԪ��]
#define MCalWareExYuanBao(nExTimes)	((nExTimes + 1) * 5)


// �������ݴ���=����ǰ�����Կ�����λ��-20��/5
#define MCalBagExTimes(n16CurBagSpace)	((n16CurBagSpace - 20) / 5)

// ��ʹ����Ϸ�����䣬���շ�=���������ݴ���+1��^2��8000[��λ��Ϸ��]
#define MCalBagExSilver(nExTimes)	((nExTimes + 1) * (nExTimes + 1) * 8000)

// ��ʹ��Ԫ�����䣬���շ�=���������ݴ���+1����10[��λԪ��]
#define MCalBagExYuanBao(nExTimes)	((nExTimes + 1) * 10)

