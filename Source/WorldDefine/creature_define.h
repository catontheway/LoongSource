//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: creature_define.h
// author: Aslan
// actor:
// data: 2008-08-25
// last:
// brief: �������Զ���
//-----------------------------------------------------------------------------
#pragma once

#include "reputation.h"
#include "RoleDefine.h"

enum ENPCTeamOrder;

//----------------------------------------------------------------
// ��Ϸ����������
//----------------------------------------------------------------
enum ECreatureType
{
	ECT_NULL		=	0,
	ECT_Monster		=	1,		// ����
	ECT_GameObject	=	2,		// �ɽ����ĵ���
	ECT_NPC			=	3,		// ��ͨNPC
	ECT_Pet			=	4,		// ����
};

//----------------------------------------------------------------
// ��������
//----------------------------------------------------------------
enum EAniType
{
	EAT_None		=	0,		// ��̬
	EAT_KeyFrame	=	1,		// �ؼ�֡
	EAT_Skeleton				// ��������
};

//----------------------------------------------------------------
// ��������
//----------------------------------------------------------------
enum EMonsterType
{
	EMTT_Normal		=	1,		// ��ͨ
	EMTT_Nest		=	2,		// ��Ѩ
	EMTT_Team		=	10,		// ����С�Ӷӳ�
};

//----------------------------------------------------------------
// ��ͨNPC����
//----------------------------------------------------------------
enum ENPCType
{
	ENPCT_Normal	=	0,		// ��ͨNPC
};

//-----------------------------------------------------------------
// �ɽ�����������
//-----------------------------------------------------------------
enum EGameObjectType
{
	EGOT_Normal		=	0,		// ��ͨ����
	EGOT_Gather		=	3,		// �ɲɼ�����
	EGOT_Chest		=	4,		// ����
	EGOT_Flag		=	5,		// ����
	EGOT_Door		=	6,		// ��
	EGOT_QuestInves =   7,      // ����ɵ������
	EGOT_QuestTalk  =   8,      // ����Ի�����
	EGOT_CommonInves=	9,		// ������������
};

//------------------------------------------------------------------
// ��������
//------------------------------------------------------------------
enum EPetType
{
	EPT_Normal		=	0,		// ��ͨ����
};

//------------------------------------------------------------------
// ְ������
//------------------------------------------------------------------
enum EFunctionNPCType
{
	EFNPCT_Null		=	-1,		// ��
	EFNPCT_Shop		=	0,		// �̵�
	EFNPCT_Dak		=	1,		// ��վ
	EFNPCT_Ware		=	2,		// �ֿ�
	EFNPCT_Compose	=	3,		// �ϳ�
	EFNPCT_Guild	=	4,		// ����
	EFNPCT_Guard	=	5,		// ����
	EFNPCT_Yamun	=	6,		// ����
	EFNPCT_Quench	=	7,		// ���
	EFNPCT_Engrave	=	8,		// �Կ�
	EFNPCT_Posy		=	9,		// ����
	EFNPCT_Teacher	=	10,		// ����������
	EFNPCT_Revive	=	11,		// �����
	EFNPCT_GuildMgr	=	12,		// ���ɹ���
	EFNPCT_GuildWare=	13,		// ���ɲֿ�
	EFNPCT_CofC		=	14,		// �̻�
	EFNPCT_Class	=	15,		// ְҵ

	// Jason v1.3.1 2009-12-21 ��ʯ���npc����
	EFNPCT_GemRemoval	=	16,	// ��ʯ���npc
};

enum EAIACTION
{
	AIAT_Attack		=	0,									// ������
	AIAT_Guard		=	1,									// ������
	AIAT_Barbette	=	2,									// ������
	AIAT_Lamster	=	3,									// ������
	AIAT_CallHelp	=	4,									// ������
	AIAT_SpaceOut	=	5,									// ������
};

//------------------------------------------------------------------
// ���ܹ���AI���͵��м�״̬
//------------------------------------------------------------------
enum EFLEETIME
{
	EFT_NotEnter	=	0,									// û�н��������״̬
	EFT_FirstEnter	=	1,									// ��һ�ν�������״̬
	EFT_Enter		=	2,									// ��������״̬
	EFT_CallHelp	=	3,									// ����״̬
};

//------------------------------------------------------------------
// ְ��ID
//------------------------------------------------------------------
typedef union
{
	DWORD	dwCommonID;					// ��ȡ��̬���Ա�ʱ��ʹ�ø��ֶ�
	DWORD	dwShopID;					// �̵�NPC���ҵ��̵�ID
	INT		nProduceType;				// �ϳ�NPC����Ӧ�ĺϳ�����
	DWORD	dwDakID;					// ��վNPC���ҵ�ID
} FunctionID;


//------------------------------------------------------------------
// �ɲɼ���������
//------------------------------------------------------------------
enum EGatherType
{
	EGT_Mine		=	1,		// ����
	EGT_Herb		=	2,		// ��ҩ

	EGT_Food		=	3,		// ʳ��
	EGT_Cloth		=	4,		// ����
};

//------------------------------------------------------------------
// ���Ｖ��
//------------------------------------------------------------------
enum ERank
{
	ER_Null,			// ��
	ER_Normal,			// ��ͨ
	ER_Elite,			// ��Ӣ
	ER_Rare,			// ϡ��
	ER_Quest,			// ����
};

//-------------------------------------------------------------------
// Ѳ������
//-------------------------------------------------------------------
enum ECreaturePatrolType
{
	ECPT_Null = 0,
	ECPT_Stand,			// վ��
	ECPT_Range,			// ����
	ECPT_Path,			// ·��
	ECPT_Wood,			// ľ׮
};

//-------------------------------------------------------------------
// ׷������
//-------------------------------------------------------------------
enum ECreaturePursueType
{
	ECPST_Null = 0,
	ECPST_Normal,		// ��ͨ׷��
	ECPST_Guard,		// ��̨
};

//-------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------
enum ECreatureRespawnType
{
	ECRT_Normal,			// ��ͨ������ʽ
};

//-------------------------------------------------------------------
// ���﹥���������ͣ����飬������
//-------------------------------------------------------------------
enum ECreatureTaggedType
{
	ECTT_NULL = 0,
	ECTT_Hit,					// ����
	ECTT_Kill,					// ��ɱ
	ECTT_All,					// ȫ��	
};

//-------------------------------------------------------------------
// ����AI��������
//-------------------------------------------------------------------
enum EAICreateType
{
	EACT_Null			=		-1,			// ��AI
	EACT_NoAI			=		0,			// ��AI����
	EACT_Indicate		=		1,			// ָ��
	EACT_Random			=		2,			// ��ȫ���
	EACT_GroupRandom	=		3,			// ����ID��Χ�����
};

//---------------------------------------------------------------------
// AI������Ϊ
//---------------------------------------------------------------------
enum EAIFlag
{
	EAIF_Flee			=		1,			// ����
	EAIF_Call			=		2,			// �ٻ�����
};


// Jason 2009-11-26 ������Ʒ����˥������
enum ECreatureItemLootAttenuationType
{
	ECILAT_None		= 0,	// ��˥��
	ECILAT_Normal	= 1,	// ˥��
	ECILAT_Other	= 20
};

//---------------------------------------------------------------------
// ��Ѩ���Խṹ
//---------------------------------------------------------------------
const INT MAX_SPAWN_CREATURE_NUM = 10;		// ��Ѩ����ˢ�µĹ�������

struct tagNestProto
{
	ENPCTeamOrder		eOrderType;								// С�Ӷ���
	FLOAT				fSpace;									// ����С�ӳ�Ա��ļ��
	INT					nSpawnRadius;							// ˢ�¹���ķ�Χ
	INT					nCreatureNum;							// ˢ�µĹ�������
	DWORD				dwSpawnID[MAX_SPAWN_CREATURE_NUM];		// ��Ѩˢ�¹����ID
	INT					nSpawnMax[MAX_SPAWN_CREATURE_NUM];		// ��Ѩˢ��ÿ�ֹ�����������
};

//-------------------------------------------------------------------
// ���NPC������������Խṹ
//-------------------------------------------------------------------
struct tagCreatureProto
{
	// ����
	DWORD					dwTypeID;					// ����ID
	ECreatureType			eType;						// �������
	INT						nType2;						// ���������
	INT						nType3;						// �������3
	INT						nFamily;					// ��������
	BYTE					bySex;						// �Ա�
	BYTE					byReserved[3];				// ������3�ֽ�
	ERank					eLite;						// ����
	INT						nLevel;						// �ȼ�
	Vector3					vSize;						// �����д�С

	// �ƶ���ս��
	ECreaturePatrolType		ePatrolType;				// �ƶ�����
	INT						nPatrolRadius;				// ���ƶ��뾶
	ECreaturePursueType		ePursueType;				// ׷������
	DWORD					dwNormalSkillIDMelee;		// ��ͨ��������ID
	DWORD					dwNormalSkillIDRanged;		// Զ�̹�������ID
	INT						nAttackInterTick;			// ���﹥�����
	INT						nMaxPursueTick;				// ���﹥�����
	BOOL					bCanAttack;					// �Ƿ�ɹ���
	BOOL					bCanBeAttack;				// �Ƿ�ɱ�����
	BOOL					bVisble;					// �Ƿ�ɼ�
	BOOL					bCanHitFly;					// �Ƿ���Ա�����

	// ˢ��
	ECreatureRespawnType	eRespawnType;				// ����ģʽ
	INT						nRespawnTick;				// ����ʱ�䣨��λ��tick��
	INT						nLiveTick;					// ����ʱ��

	// ����
	ECreatureTaggedType		eTaggedType;				// ����Ȩ����
	INT						nExpGive;					// ���辭��
	EReputationType			eRepType1;					// ��������1����
	INT						nRepNum1;					// ��������1ֵ
	EReputationType			eRepType2;					// ��������2����
	INT						nRepNum2;					// ��������2ֵ
	DWORD					dwLootID;					// ��Ʒ������

	// AI
	EAICreateType			eAICreateType;				// AI����ģʽ
	DWORD					dwAIID;						// AIID
	EAIACTION				eAIAction;					// AI��Ϊ��ʽ

	// ְ��
	EFunctionNPCType		eFunctionType;				// ְ������
	FunctionID				uFunctionID;				// ְ��ID

	// ����
	INT						nBaseAtt[ERA_End];			// ��������

	// ����
	UINT16					u16QuestID;					// �ɵ����������������ID
	BYTE					byReserved2[2];				// ������

	// ��Ѩָ��
	tagNestProto*			pNest;						// ��Ѩָ��

	// ����
	EAniType				eAnimationType;
	TCHAR					szModelName[X_LONG_NAME];	// ģ������
	TCHAR					szName[X_SHORT_NAME];		// ��������
	TCHAR					szTitle[X_SHORT_NAME];		// �ƺ�
	TCHAR					szTitleIcon[X_SHORT_NAME];	// �ƺ���ͼ
	TCHAR					szEffect[X_SHORT_NAME];		// ��Ч���
	TCHAR					szIdleSpeech[X_LONG_NAME];	// ���к���
	TCHAR					szCombatSpeech[X_LONG_NAME];// ս������
	TCHAR					szSkillSpeech[X_LONG_NAME];	// ���ܺ���
	TCHAR					szDeathSpeech[X_LONG_NAME];	// ��������
	TCHAR					szMonsterAI[X_SHORT_NAME];	// ����AI

	// Jason 2009-11-26 ����������Ʒ����˥��
	ECreatureItemLootAttenuationType eItemLootAttenuType;// ��Ʒ����˥������

	tagCreatureProto()
	{
		pNest = NULL;
	}

	~tagCreatureProto()
	{
		if( pNest )
			delete pNest;
	}

};

//---------------------------------------------------------------------
// �����AI�ṹ
//---------------------------------------------------------------------
const INT X_MAX_CREATURE_AI_BUFF	=	3;	// ����AI���������Buff����
const INT X_MAX_CREATURE_AI_SKILL	=	5;	// ����AI��������༼������

struct tagCreatureAI
{
	DWORD		dwID;										// AI ID;
	EAIACTION	eAIAction;									// AI��Ϊ����
	DWORD		dwGroupID;									// AI��ID
	DWORD		dwBuffTypeID[X_MAX_CREATURE_AI_BUFF];		// ��ʼ��3��buff ID
	DWORD		dwTriggerID[X_MAX_CREATURE_AI_SKILL];		// 5��������
	DWORD		dwSkillTypeID[X_MAX_CREATURE_AI_SKILL];		// 5������ID
	INT			nTargetType[X_MAX_CREATURE_AI_SKILL];		// ���ܶ�Ӧ��Ŀ������
};

const INT	X_MAX_FLEE_RADIUS			=	2000;				// �����������뾶
const INT	X_MIN_FLEE_RADIUS			=	1500;				// ����������С�뾶
const INT	X_RANGE_FLEE_RADIUS			=	X_MAX_FLEE_RADIUS - X_MIN_FLEE_RADIUS;			// ���ܰ뾶��ֵ
const FLOAT X_MAX_SOS_RADIUS			=	2500;				// ����״̬��������ܾ���
const FLOAT	X_MAX_SOS_RADIUS_SQ			=	X_MAX_SOS_RADIUS * X_MAX_SOS_RADIUS;
	

