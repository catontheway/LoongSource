//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: log_cmdid_define.h
// author: 
// actor:
// data: 2008-11-11
// last:
// brief: ��������¼log�õ������������ö�� -- �������
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// log��������Ϣö�� -- һ����������ò�Ҫ�޸�(1.���ݿ�������Ӧ��¼��2.�ű�������д��)
//-----------------------------------------------------------------------------
enum ELogCmdID
{
	ELCID_Null					= 0,	// ��

	// �̵����
	ELCID_Shop_BuyItem			= 1,	// ����Ʒ(��װ��)
	ELCID_Shop_BuyEquip			= 2,	// ��װ��
	ELCID_Shop_Sell				= 3,	// ����Ʒ&װ��

	// PK�������
	ELCID_Open_SafeGuard		= 11,	// ����PK����

	// �����ͷ�
	ELCID_Dead_Penalty			= 12,	// PK�ͷ�

	// �ٱ������
	ELCID_BaiBao_LoadFromDB		= 20,	// ��item_baibao���ж�ȡ����Ʒ
	ELCID_BaiBao_Bill_YuanBao	= 21,	// Ԫ��ֱ��

	// �������
	ELCID_Bag_PosOverlap		= 30,	// ��ʼ����ɫ����ʱ��������Ʒλ���ص�

	// ϴ��
	ELCID_Clear_Att				= 40,	// ϴ���Ե�
	ELCID_Clear_Talent			= 41,	// ϴ���ʵ�


	// ��ɫ�ֿ����
	ELCID_RoleWare_PosOverlap	= 50,	// ��ʼ����ɫ�ֿ�ʱ��������Ʒλ���ص�
	ELCID_RoleWare_SaveSilver	= 51,	// ��Ǯ
	//ELCID_RoleWare_SaveYuanBao	= 52,	// ��Ԫ��
	ELCID_RoleWare_GetSilver	= 53,	// ȡǮ
	ELCID_RoleWare_GetYuanBao	= 54,	// ȡԪ��

	
	// �̳�
	ELCID_Mall_BuyItem			= 60,	// �����̳���Ʒ
	ELCID_Mall_BuyPack			= 61,	// �����̳���Ʒ��
	ELCID_Mall_FreeItem			= 62,	// ��ȡ�̳������Ʒ
	ELCID_Mall_BuyItem_Add		= 63,	// �����̳���Ʒʱ����
	ELCID_Mall_BuyPack_Add		= 64,	// �����̳���Ʒ��ʱ����
	ELCID_Mall_PresentItem		= 65,	// �����̳���Ʒ
	ELCID_Mall_PresentPack		= 66,	// �����̳���Ʒ��
	ELCID_Mall_PresentItem_Add	= 67,	// �����̳���Ʒʱ����
	ELCID_Mall_PresentPack_Add	= 68,	// �����̳���Ʒ��ʱ����

	// Ԫ������
	ELCID_Trade_SaveYuanBao		= 70,	// �����˻���Ԫ��
	ELCID_Trade_SaveSilver		= 71,	// �����˻����Ǯ
	ELCID_Trade_DepositYuanBao  = 72,	// �ӽ����˻�ȡ��Ԫ��
	ELCID_Trade_DepositSilver	= 73,	// �ӽ����˻�ȡ����Ǯ
	ELCID_Trade_Tax				= 74,	// �ύ����ʱ�۳�������

	// �̳� - ��
	ELCLD_Mall_ExchangeItem		= 80,	// �һ��̳���Ʒ
	ELCLD_Mall_ExchangePack		= 81,	// �һ��̳���Ʒ��
	ELCLD_Mall_ExchangeItem_Add	= 82,	// �һ��̳���Ʒʱ����
	ELCLD_Mall_ExchangePack_Add	= 83,	// �һ��̳���Ʒ��ʱ����

	// �Ź�
	ELCID_GroupPurchase_Faild		= 90,	// �Ź�ʧ�ܷ�������
	ELCLD_GroupPurchase_BuyItem		= 91,	// �Ź������̳���Ʒ
	ELCLD_GroupPurchase_BuyItem_Add	= 92,	// �Ź��̳���Ʒ����

	// �������
	ELCID_Quest_Loot			= 100,	// �������
	ELCID_Quest_Complete		= 101,	// �������
	ELCID_Quest_Accept			= 102,	// ��ȡ����
	ELCID_Quest_Discard			= 103,	// ��������
	ELCID_Quest_rewards			= 104,  // ������

	// װ��ǿ�����
	ELCID_Compose_Posy			= 150,	// ����
	ELCID_Compose_Engrave		= 151,	// �Կ�
	ELCID_Compose_Enchase		= 152,	// ��Ƕ
	ELCID_Compose_Brand			= 153,	// ��ӡ
	ELCID_Compose_LongFu		= 154,	// ����
	ELCID_Compose_Produce		= 155,	// �ϳ�
	ELCID_Compose_Quench		= 156,	// ���
	ELCID_Compose_Decompose		= 157,	// �㻯,װ���ֽ�
	ELCID_Compose_Chisel		= 158,	// ����
	ELCID_Compose_Dye			= 159,	// Ⱦɫ

	// ��Ҽ佻��
	ELCID_Exchange_Verify		= 160,	// ����ȷ��
	// Jason û��λ���ˣ���ʱ�����2009-12-6
	ELCID_Raise_PotVal			= 165,	// û�п�϶�ˣ�������ô����������
	ELCID_LessingLoong			= 166,	// �����͸�
	ELCID_PhilosophersStone		= 167,	// ���ʯ

	// ԭ�ظ���
	ELCLD_Revive_Locus			= 180,

	// ��̯
	ELCLD_Stall_BeSold			= 190,	// ��̯����
	ELCLD_Stall_Buy				= 191,	// ��̯λ�Ϲ���

	// ������
	ELCLD_Dak					= 200,

	// ��Ʒ
	ELCLD_Item_Use				= 300,	// ��Ʒʹ��
	ELCLD_Item_Move				= 301,	// ͬһ�������ƶ�
	ELCLD_Item_Move2Other		= 302,	// ��ͬ�������ƶ�

	ELCLD_Item_AtTerm			= 310,	// ��ʱ����Ʒ������ɾ��

	// װ��
	ELCLD_Equip_Identify		= 350,	// װ������
	ELCLD_Equip_ReGet			= 351,	// װ���ָ�
	
	// ����
	ELCD_1stOnline_Item			= 380,	// ���ַ���

	// ��ҳԴӱ����ӵ���Ʒ
	ELCLD_Role_Discard_Item		= 401,

	// ���ʰȡ��Ʒ
	ELCLD_PickUp_Item			= 402,
	ELCLD_PickUp_Money			= 403,

	// ������������
	ELCLD_Send_Gift				= 410,

	// ��Ʒ��
	ELCLD_Gift_Bag				= 420,

	// ְ��NPC���
	ELCLD_Bag_Extend			= 450,	// ���䱳��
	ELCLD_RoleWare_Extend		= 451,	// ��ɫ�ֿ�����
	ELCLD_CureInjure			= 452,	// npc��������
	
	// ��ͨ����
	ELCID_Loot					= 500,	// ��ͨ����

	// �������
	ELCLD_Guild_Create			= 600,	// ��������
	ELCLD_Guild_Upgrade			= 601,	// ������ʩ����
	ELCLD_Guild_SpreadAffair	= 602,	// ��������
	ELCLD_Guild_DailyCost		= 603,	// �ճ�����
	ELCLD_Guild_SkillResearch	= 604,	// ���ɼ�������
	ELCLD_Guild_SkillLearn		= 605,	// ѧϰ���ɼ���
	ELCLD_Guild_Commerce		= 606,	// ����

	// �������
	ELCLD_CHAT_WORLD_CHANNEL		= 700,	//������Ƶ������
	ELCLD_SHOW_EQUIP_WORLD_CHANNEL	= 701,	//������Ƶ��չʾװ��
	ELCLD_SHOW_ITEM_WORLD_CHANNEL	= 702,	//������Ƶ��չʾ��Ʒ
	ELCLD_CAST_TALK_IM				= 703,	//����ʹ��IM����

	// �������
	ELCLD_PET_ITEM_CREATE_PET		= 800,	// ���ɳ���
	ELCLD_PET_ITEM_FEED_PET			= 801,	// ����ιʳ
	ELCLD_PET_ITEM_PET_LEARNSKILL	= 802,	// ����ѧϰ����
	ELCLD_PET_ITEM_PET_REVIVE		= 803,	// ���︴��
	ELCLD_PET_ITEM_PET_EQUIP		= 804,	// ����װ��
	ELCLD_RolePourExpPet			= 805,	// ��ע����
	ELCLD_PET_ITEM_PET_Enhance		= 806,	// ����װ��
	ELCLD_PET_ITEM_PET_Lock			= 807,	// ��������
	ELCLD_PET_ITEM_PET_UnLock		= 808,	// �������
	ELCLD_PET_ITEM_PET_Food			= 809,	// ����ʳ��
	ELCLD_PET_SKILL_SALE			= 810,	// ����С������
	
	ELCLD_PET_CREATE_PET			= 811,	// ��������
	ELCLD_PET_GAIN_PET				= 812,	// ��ó���
	ELCLD_PET_DEL_PET_CREATE		= 813,	// ɾ���������ʧ�ܣ�
	ELCLD_PET_DEL_PET_INIT			= 814,	// ɾ�������ʼ��ʧ�ܣ�
	ELCLD_PET_DEL_PET_CLIENT		= 815,	// ɾ������ͻ������
	ELCLD_PET_LOSE_PET				= 816,	// ʧȥ����

	// �������
	ELCLD_ACT_TREASURE				= 900,	// ���������䱦���

	//�̶�����
	ELCLD_WU_SE_SHI					= 1000, // �̶��ɾ����ɫʯ
	ELCLD_PVP						= 1001, // pvp�̶��

	// VIP̯λ���
	ELCLD_VIP_Stall_Rent			= 1100,	// VIP̯λ����

	// ��������
	ELCLD_VIP_NetBar				= 1200,	// ��������

	// ����
	ELCLD_Skill_Use					= 1300, // ����
		
	// ְҵ���
	ELCLD_Class_Change				= 1400, // תְ
	ELCLD_Class_Get					= 1401, // ��ְ

	// ��������ڴ˴�֮�ϼ�
	ELCID_GemRemoval				= 1500,	// ��ʯ���
	
	// GM����
	ELCID_GM_ClearBag				= 1000000,	// ������
	ELCID_GM_CreateItem				= 1000001,	// ������Ʒ/װ��/ʱװ
	ELCID_GM_GetMoney				= 1000002,	// ��ý�Ǯ
	ELCID_GM_GetYuanBao				= 1000003,	// ���Ԫ��
	ELCID_GM_GetExVolume			= 1000004,	// �������
	ELCID_GM_GetFund				= 1000005,	// ��ð����ʽ�
	ELCID_GM_GetMaterial			= 1000006,	// ��ð����ʲ�
	ELCID_GM_GetContribute			= 1000007,	// ��ð��ɸ��˹���
};


//-----------------------------------------------------------------------------
// log�����ݽṹ
//-----------------------------------------------------------------------------
struct tagLogSilver				// ��Ǯ�仯
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	DWORD	dwRoleIDRel;
	INT64	n64Silver;			// +����ã�-��ʧȥ
	INT64	n64TotalSilver;		// ��Ӧ����������
	INT8	n8LogConType;		// �������ֿ�
};

struct tagLogYuanBao			// Ԫ���仯
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nYuanBao;			// +����ã�-��ʧȥ
	INT		nTotalYuanBao;		// ��Ӧ����������
	INT8	n8LogConType;		// �������ٱ���
};

struct tagLogExVolume			// ����仯
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nExVolume;			// +����ã�-��ʧȥ
	INT		nTotalExVolume;		// ��Ӧ����������
};

struct tagLogTimeStat			// ������Ҷ�ʱͳ��
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	INT64	n64BagSilver;
	INT64	n64WareSilver;
	INT		nBagYuanBao;
	INT		nBaiBaoYuanBao;
	INT		nExVolume;
};

struct tagLogItem				// ��Ʒlog�ṹ
{
	DWORD		dwRoleID;
	DWORD		dwTypeID;

	INT64		n64Serial1;
	INT64		n64Serial2;
	INT8		n8ConType1;
	INT8		n8ConType2;
	INT16		n16ResNum1;
	INT16		n16ResNum2;

	INT16		n16OptNum;		// �仯����(+����ʾ��ã�-����ʾʧȥ)
	DWORD		dwCmdID;
	DWORD		dwRoleIDRel;
};

struct tagLogMallSell			// �̳ǳ�����Ʒlog�ṹ
{
	INT64		n64Serial;
	DWORD		dwTypeID;
	DWORD		dwRoleID;		// ������
	DWORD		dwToRoleID;		// �����Ʒ��
	DWORD		dwFstGainTime;
	DWORD		dwExistTime;
	INT			nMaxUseTimes;
	INT			nCostYuanBao;
	INT			nCostExVolume;
	DWORD		dwCmdID;
	INT16		n16NumSell;
};

struct tagLogMallSellPack		// �̳ǳ�����Ʒ��log�ṹ
{
	DWORD		dwPackID;
	DWORD		dwBuyRoleID;
	DWORD		dwToRoleID;
	INT			nCostYuanBao;
};

struct tagLogItemTims			// ��Ʒʹ�ô���log�ṹ
{
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT64		n64Serial;
	DWORD		dwTypeID;
	INT			nUsedTimes;
	INT			nMaxUseTimes;
	INT8		n8ConType;
};

struct tagLogFund
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nFund;
	INT			nTotalFund;
};

struct tagLogMaterial
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nMaterial;
	INT			nTotalMaterial;
};

struct tagLogReputation
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nReputation;
	INT			nTotalReputation;
};

struct tagLogGMCmd
{
	DWORD		dwRoleID;
	TCHAR		szGMCmd[X_LONG_STRING];
	DWORD		dwErrorCode;
};

struct tagLogPet
{
	DWORD		dwMasterID;
	DWORD		dwPetID;
	DWORD		dwCmdID;
};


//-----------------------------------------------------------------------------
#pragma pack(pop)