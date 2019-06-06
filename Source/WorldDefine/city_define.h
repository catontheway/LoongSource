//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_define.h
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: ����ϵͳ����
//-----------------------------------------------------------------------------
#pragma once
#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ��
//-----------------------------------------------------------------------------
#define MCityOutput(base, prolificacy, level)		((base) * (1.0f + ((prolificacy) - 500) / 1000.0f) * (1 + (level) / 5))

#define CITY_DEVELOP_GOLD_CARD				3300003
#define CITY_DEVELOP_SILVER_CARD			3300002
#define CITY_DEVELOP_COPPER_CARD			3300001

//����������
#define CITY_DEVELOP_GOLD_ADD				15
#define CITY_DEVELOP_SILVER_ADD				5
#define CITY_DEVELOP_COPPER_ADD				2

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT32 MAX_CITY_BUFF_SIZE				= 40 * 1000;		// ����ϵͳ��󻺴��С

const INT32 MAX_CITY_PROLIFICACY			= 1000;				// �������������
const INT32 CITY_PRODUCE_COST				= 50;				// ���в�������������

const INT32 MAX_CITY_DEFENCE				= 10000;			// ������������

const INT32 MAX_CITY_ITEM_OUTPUT			= 5;				// �����Ʒ��������

const INT32 MAX_CITY_ATT_VALUE				= 99999999;			// ���������������
const INT32 MAX_CITY_TAXRATE				= 10;				// �������˰��
const INT32 MODIFY_CITY_TAXRATE_CYCLE		= 2 * 24 * 60 * 60;	// ˰�ʱ������(2��)

const INT32 MAX_CITY_EUDEMONTALLY			= 1000;				// ����ػ������϶�

const INT32 MAX_CITY_ATTACK_FORCE			= 5;				// ��ս�������˰�����������
const INT32 MAX_CITY_DEFENCE_FORCE			= 4;				// ��ս�������˰�����������

const INT32 EXAMINE_CITY_ATT_COST			= 100000;			// �鿴�������Խ�Ǯ����

//-----------------------------------------------------------------------------
// ö��
//-----------------------------------------------------------------------------
enum ECityBroadCastType
{
	ECBC_ChangeHolder		= 0,		//ȫ����Ϣ�㲥����###���������ƣ����###���������ƣ���ͳ��Ȩ����

	ECBC_ChangeTaxRate		= 1,		//���óɹ���ϵͳȫ����㲥����###���������ƣ���˰�ʱ��Ϊ###����

	ECBC_ConfirmAttack		= 2,		//ϵͳȫ����Ϣ�㲥�������γ�ս�ĸ����뷽��ȷ�������λ���ڳ�ս��Ϣ�����в�ѯ����



	ECBC_ReadyForAttack		= 3,		//��ȷ��ӵ�г�ս����Ȩ�ĸ������ɣ�ȫ���ɳ�Ա��ʾ�������ɻ���˶�###���������ƣ��Ĺ���Ȩ�����λ���ڻ�����ս����

	ECBC_ReadyForDefence	= 4,		//��ȷ�ϵİ���ȫ���ɳ�Ա��ʾ���������ѱ�ͬ�����###���������ƣ��ĳ�ս������Ϊ������Э�����أ����λ��Ա������ս��

	ECBC_AttackSignup		= 5,		//ȫ�������ʾ���������Ѷ�###���������ƣ����й�����������

	ECBC_DefenceSignup		= 6,		//ȫ�������ʾ���������Ѷ�###���������ƣ�����Э��������������

	ECBC_UpgradeProlificacy	= 7,		//ͬʱȫ���ɳ�Ա��Ϣ����###����ɫ���ƣ���������####���������ƣ���###���������ƣ�����������###�㡣��

	ECBC_RequestSignup		= 8,		//###���������ƣ����������������###���������ƣ���ͳ��Ȩ��ȫ����ɳ�Ա��Ӧ�ó����������͵���������֮�;����������������0����м��㣡��
};


// ������
enum
{
	E_City_Success						= E_Success,		// �ɹ�

	E_City_Not_BeOccupied				= 1,		// ����û�б�ռ��
	E_City_Guild_NotMatch				= 2,		// ��Ұ�����Ϣ����
	E_City_Item_NotEnough				= 3,		// �����Ʒ������Ҫ��
	E_City_Power_NotEnough				= 4,		// ���Ȩ�޲���
	E_City_Time_Limit					= 5,		// ʱ������
	E_City_Taxation_NotEnough			= 6,		// ˰����
	E_City_Prolificacy_LackItem			= 7,		// ȱ����Ʒ

	E_City_SignUp_ForceError			= 11,		// ������Ӫ����
	E_City_SignUp_LevelLimit			= 12,		// �����ȼ�����
	E_City_SignUp_HolderConflict		= 13,		// ͳ�ΰ��ɲ��ܱ���
	E_City_SignUp_InsideCityConflict	= 14,		// �����ڳ�ͻ
	E_City_SignUp_InterCityConflict		= 15,		// ���м��ͻ

	E_City_SignUp_Fund_NotEnough		= 16,		// �����ʽ���
	E_City_SignUp_Hold_CityMax			= 17,		// ռ����������ﵽ����
	E_City_SignUp_Force_Full			= 18,		// ��ս�����а�����������
	E_City_SignUp_Already_Confirm		= 19,		// �����Ѿ��õ�����
	E_City_SignUp_Info_Error			= 20,		// ������Ϣ����
	E_City_SignUp_Minfare_Limite		= 21,		// ���������ֵ����
};

// ������������
enum ECityAttType
{
	ECAT_CityID,			// ����ID
	ECAT_ClanType,			// ��������
	ECAT_Holder,			// ͳ�ΰ���ID
	ECAT_Defence,			// ����
	ECAT_EudemonTally,		// �ػ������϶�
	ECAT_TaxRate,			// ˰��
	ECAT_TaxRateTime,		// ˰�ʸ���ʱ��
	ECAT_Taxation,			// ˰��
	ECAT_Prolificacy,		// ������
	ECAT_FundOutput,		// �ʽ����
	ECAT_MaterialOutput,	// �ʲĲ���
	ECAT_ItemOutput,		// ��Ʒ����

	ECAT_RequestSignUp,		// ���������
	ECAT_AttackSignUp,		// ��������
	ECAT_DefenceSignUp,		// �ط�����

	ECAT_AttackSignUpNum,		// ����������
	ECAT_DefenceSignUpNum,		// �ط�������
	ECAT_DefenceConfirmNum,		// �ط��Ѿ�ȷ�ϵ���

	ECAT_AttackSignUpGuildID,	// ��������������id
	ECAT_DefenceSignUpGuildID,	// ���ط���������id

	ECAT_ConfirmDefenceGuildID,	// ���ط���ȷ�ϵİ���id


	ECAT_End
};

// ��������
enum ECityForceType
{
	ECFT_Attack,		// ��������
	ECFT_Defence,		// ��������
	ECFT_Request,		// �״�����

	ECFT_End
};

// ��������
enum ECityGateType
{
	ECGT_Broken,		// ����ľ��
	ECGT_RoundLog,		// Բľ����
	ECGT_Massive,		// ����ľ��
	ECGT_Copper,		// ͭƤľ��
	ECGT_Megalith,		// ��ʯ����
	ECGT_BlackIron,		// ��������
	ECGT_DarkIron,		// ��������
	ECGT_Eternity,		// �������

	ECGT_End
};

const INT32 CITY_GATE_DEFENCE[ECGT_End] = 
{
	0,		// ����ľ��
	1000,	// Բľ����
	3000,	// ����ľ��
	4000,	// ͭƤľ��
	6000,	// ��ʯ����
	7000,	// ��������
	8000,	// ��������
	9000,	// �������
};

// ��������������
enum EProlificacyUpgrade
{
	EPU_Copper,			// ���з�չͭ��
	EPU_Silver,			// ���з�չ����
	EPU_Gold,			// ���з�չ����

	EPU_End
};

// ˰��ת������
enum ETaxTransformType
{
	ETTT_GuildFund,		// �����ʽ�
	ETTT_PersonalSilver,// ���˽�Ǯ

	ETTT_End
};

// ��ս״̬
enum ECityStruggleState
{
	ECSGS_Start				=	0,

	ECSGS_UnStruggle		=	0,			// �ǳ�ս
	ECSGS_Struggle			=	1,			// ��ս��

	ECSGS_End				=	ECSGS_Struggle,
};

//-----------------------------------------------------------------------------
// �ṹ
//-----------------------------------------------------------------------------

// ����˰��
struct tagCityTaxRate
{
	BYTE byCityID;
	BYTE byTaxRate;
};

// ��ս�����ṹ
struct tagCitySignUp
{
	DWORD				dwGuildID;		// ��������ID
	ECityForceType		eCityForceType;	// ������������
	INT32				nFare;			// ������(������Ϊ����ʱ�����Ƿ񾭹�����)
};

// ������Ʒ�����ṹ
struct tagCityItemOutput 
{
	DWORD				dwTypeID;		// ��ƷTypeID
	INT					nItemNum;		// ��������
	INT32				nLimit;			// ����������
};
struct tagWarTime
{
	DWORD	WarWeekDay		: 3;	// 0 - 6    0Ϊ����
	DWORD	BegHour			: 5;	// 0 - 23
	DWORD	BegMinutemin	: 6;	// 0 - 59
	DWORD	EndHour			: 5;	// 0 - 23
	DWORD	EndMinutemin	: 6;	// 0 - 59

	//--------------------------------------------------------------------------------
	// operator =
	//--------------------------------------------------------------------------------
	tagWarTime& operator = (const tagWarTime& time)
	{ 
		WarWeekDay=time.WarWeekDay;
		BegHour=time.BegHour; 
		BegMinutemin=time.BegMinutemin;
		EndHour=time.EndHour;
		EndMinutemin=time.EndMinutemin;
		return *this;
	}

	//--------------------------------------------------------------------------------
	// operator =
	//--------------------------------------------------------------------------------
	tagWarTime& operator = (DWORD dwTime)
	{
		return (*this = *(tagWarTime*)&dwTime);
	}
	//--------------------------------------------------------------------------------
	// copy constructor
	//--------------------------------------------------------------------------------
	tagWarTime(const tagWarTime& time)
	{
		*this = time;
	}

	//--------------------------------------------------------------------------------
	// constructor
	//--------------------------------------------------------------------------------
	tagWarTime()
	{
		memset(this,0,sizeof(*this));
	};
};

// �������Խṹ -- Proto
struct tagCityProto 
{
	DWORD				dwCityID;						// ����ID
	ECLanType			eClanType;						// ��������
	DWORD				dwShellID;						// ����ID
	DWORD				dwKernelID;						// ����ID
	INT32				nFundOutput;					// �ʽ����
	INT32				nMaterialOutput;				// �ʲĲ���
	DWORD				dwMinSigupFare;					// ��ͱ�������
	DWORD				dwDefaultDefence;				// Ĭ�Ϸ���ֵ
	DWORD				dwActivityID;					// ��Ӧ�Ļid
	tagCityItemOutput	sItemOutput[MAX_CITY_ITEM_OUTPUT];	// ��Ʒ����
};

// �������Խṹ -- DB
struct tagCityAtt2DB
{
	DWORD				dwCityID;		// ����ID
	DWORD				dwGuildID;		// ͳ�ΰ���ID
	INT32				nDefence;		// ������
	INT32				nEudemonTally;	// �ػ������϶�
	INT32				nTaxRate;		// ˰��
	DWORD				dwTaxRateTime;	// ���һ�θ���˰�ʵ�ʱ��
	INT32				nTaxation;		// ˰��
	INT32				nProlificacy;	// ������
};

// �������Խṹ -- Client
struct tagCityAtt2Client : public tagCityAtt2DB
{
	ECityGateType		eCityGateType;		// ��������
	INT32				nFundOutputBase;	// �����ʽ����
	INT32				nFundOutputCur;		// ��ǰ�ʽ����
	INT32				nMaterialOutputBase;// �����ʲĲ���
	INT32				nMaterialOutputCur;	// ��ǰ�ʲĲ���
	INT					nItemOutputType;	// ��Ʒ��������
	tagCityItemOutput	sItemData[1];		// ��Ʒ����
};

// ���е�һ���Խṹ
struct tagCityAppointedAtt 
{
	ECityAttType		eType;
	INT					nDataNum;
	BYTE				byData[1];
};

//---------------------------------------------------------------------------------
// ��ɫ��ս�������Խṹ
//---------------------------------------------------------------------------------
struct tagStruggleRevive
{
	ECityForceType		eForceType;		// �������������
	INT					nDeadTick;		// ��ɫ����ʱ��ս��tick
};

//-----------------------------------------------------------------------------
#pragma pack(pop)