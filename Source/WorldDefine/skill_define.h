//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: skill_define.h
// author: Aslan
// actor:
// data: 2008-09-08
// last:
// brief: ���ܾ�̬����
//-----------------------------------------------------------------------------
#pragma once

#include "RoleDefine.h"
#include "buff_define.h"

#pragma pack(push, 1)

const INT MAX_CHANNEL_TIMES = 5;		// �ܵ�������������
const INT MAX_BUFF_PER_SKILL = 3;		// ÿ���������������buff����
const INT MAX_BUFF_PER_ITEM = 3;		// ÿ����Ʒ���������buff����

const INT MAX_ROLE_PRODUCE_NUM = 3;		// ÿ�����������ѧ������������ܵ�����

//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
enum ESkillType
{
	ESST_Null		=	0,
	ESST_Role		=	1,		// ��ɫ����
	ESST_Creature	=	2,		// ���＼��
	ESST_Item		=	3,		// ��Ʒ����
	ESST_Social		=	4,		// ��Ἴ��
	ESST_Pet		=	5,		// ���＼��
};

enum ESkillTypeEx
{
	ESSTE_Null		=	0,
	ESSTE_Default	=	1,		// ��ͨ����
	ESSTE_Sword		=	2,		// ��
	ESSTE_Blade		=	3,		// ��
	ESSTE_Wand		=	4,		// ����
	ESSTE_Bow		=	5,		// ��
	ESSTE_Valor		=	6,		// ����
	ESSTE_Supple	=	7,		// ����
	ESSTE_Poison	=	8,		// ҽ��
	ESSTE_Swing		=	9,		// ����
	ESSTE_Action	=	10,		// ս��
	ESSTE_Jugglery	=	11,		// ����
	ESSTE_Produce	=	12,		// ��������
	ESSTE_Guild		=	13,		// ���ɼ���
	ESSTE_Valentine	=	14,		// ���¼���
	ESSTE_Consort	=	15,		// ���޼���
	ESSTE_Brother	=	16,		// �ֵܼ���
	ESSTE_Teacher	=	17,		// ʦͽ����
	ESSTE_Monster	=	18,		// ������ͨ����
	ESSTE_MonsterAI	=	19,		// ����AI����
	ESSTE_ItemUse	=	20,		// ��Ʒ����
	ESSTE_Equipment	=	21,		// װ������
	ESSTE_Longhun	=	22,		// ���꼼��
	ESSTE_Society	=	23,		// ��Ἴ��
	ESSTE_PetSkill	=	24,		// ���＼��
};

// nType3�����ж�������������
enum ESkillTypeEx2
{
	ESSTE2_NULL		= 0,		//
	ESSTE2_Mining	= 1,		//����
	ESSTE2_Harvestry,			//�ջ�
	ESSTE2_Smilt,				//ұ��
	ESSTE2_Artisan,				//�ɽ�
	ESSTE2_DanTraining,			//����
	ESSTE2_Smith,				//����
	ESSTE2_Casting,				//����
	ESSTE2_Dressmaker,			//�÷�
	ESSTE2_Aechnics,			//����
	ESSTE2_PointUp,				//�㻯
	ESSTE2_Hidden,				//����ʾ�ļ���

    ESSTE_End
};

//------------------------------------------------------------------------------
// ����Ŀ������
//------------------------------------------------------------------------------
enum ESkillTargetType
{
	ESTT_Null		=	0,
	ESTT_Buff		=	1,		// ״̬
	ESTT_Skill		=	2,		// ����
	ESTT_Trigger	=	3,		// ������
	ESTT_Combat		=	4,		// ս��Ŀ��
	ESTT_NoCombat	=	5,		// ��ս��Ŀ��
};

//-------------------------------------------------------------------------------
// ���ܾ�������
//-------------------------------------------------------------------------------
enum ESkillDistType
{
	ESDT_Null		=	0,
	ESDT_Melee		=	1,		// ����
	ESDT_Ranged		=	2,		// Զ��
	ESDT_Inner		=	3,		// �ڹ�
};

//-------------------------------------------------------------------------------
// ����ʩչ����
//-------------------------------------------------------------------------------
enum ESkillUseType
{
	ESUT_Null		=	0,
	ESUT_Active		=	1,		// ����
	ESUT_Passive	=	2,		// ����
};

//-------------------------------------------------------------------------------
// ������������
//-------------------------------------------------------------------------------
enum ESkillOPType
{
	ESOPT_NUll		=	0,
	ESOPT_Rect		=	1,		// ����
	ESOPT_Explode	=	2,		// ��ը
};

//--------------------------------------------------------------------------------
// �����˺�����
//--------------------------------------------------------------------------------
enum ESkillDmgType
{
	ESDGT_Null		=	0,
	ESDGT_Ordinary	=	1,		// ��ͨ�˺�����
	ESDGT_Bleeding	=	2,		// ��Ѫ�˺�����
	ESDGT_Brunt		=	3,		// ����˺�����
	ESDGT_Bang		=	4,		// �ػ��˺�����
	ESDGT_Poison	=	5,		// �����˺�����
	ESDGT_Thinker	=	6,		// �����˺�����
	ESDGT_Injury	=	7,		// �����˺�����
	ESDGT_Stunt		=	8,		// �����˺�����
};

//---------------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------------
enum ESkillCostType
{
	ESCT_HP			=	0,		// ����
	ESCT_MP			=	1,		// ����
	ESCT_Rage		=	2,		// ŭ��
	ESCT_Endurance	=	3,		// �־�
	ESCT_Valicity	=	4,		// ����
	ESCT_End		=	5,
};

//---------------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------------
enum ESkillLevelUpType
{
	ESLUT_Fixed		=	0,		// �̶�
	ESLUT_Exp		=	1,		// ������
	ESLUT_Book		=	2,		// ������
};

//---------------------------------------------------------------------------------
// �����Ա�ʹ������
//---------------------------------------------------------------------------------
enum ESkillSexLimit
{
	ESSL_Null		=	0,
	ESSL_Man		=	1,
	ESSL_Woman		=	2,
};

//---------------------------------------------------------------------------------
// ����ְҵʹ������
//---------------------------------------------------------------------------------
//enum ESkillClassLimit
//{
//
//};

//---------------------------------------------------------------------------------
// ������������
//---------------------------------------------------------------------------------
enum ESkillWeaponLimit
{

};

//---------------------------------------------------------------------------------
// ����λ������
//---------------------------------------------------------------------------------
enum ESkillPosType
{
	ESPT_NUll		=	0,		// ������
	ESPT_Front		=	1,		// ����ǰ
	ESPT_Back		=	2,		// �����
};

//---------------------------------------------------------------------------------
// ���ܾ�̬����
//----------------------------------------------------------------------------------
struct tagSkillProto 
{
	DWORD				dwID;								// ����ID
	ESkillType			eType;								// ��������
	INT					nType2;								// ����������
	INT					nType3;								// ����������
	DWORD				dwMainSkillID;						// ������ID���������Ϊ�����ܣ������Ŀ����д��
	ETalentType			eTalentType;						// ��������

	// Ŀ������
	ESkillTargetType	eTargetType;						// Ŀ���������
	DWORD				dwTargetSkillID;					// ����Ŀ��ID��ֻ�������Ŀ���������Ϊ���ܣ�
	DWORD				dwTargetBuffID;						// ״̬Ŀ��ID��ֻ�������Ŀ���������Ϊ״̬��
	DWORD				dwTargetTriggerID;					// ������Ŀ��ID��ֻ�������Ŀ���������Ϊ���ܵĴ�������

	// ���ܵ����жϱ�־
	BOOL				bFriendly;							// ���ѷ�ʹ��
	BOOL				bHostile;							// �Եз�ʹ��
	BOOL				bIndependent;						// ��������ʹ��

	// �����ж�
	ESkillDistType		eDistType;							// �����������ͣ�����Զ�̣�
	ESkillUseType		eUseType;							// ʩչ���ͣ�������������
	ESkillOPType		eOPType;							// ��������
	FLOAT				fOPDist;							// ���þ��루���絥λ��
	FLOAT				fOPRadius;							// ���÷�Χ�����絥λ��
	BOOL				bInterruptMove;						// �Ƿ��ƶ��������
	INT					nInterruptSkillOrdRate;				// ��ͨ���ܴ�ϼ���
	INT					nInterruptSkillSpecRate;			// ���⼼�ܴ�ϼ���
	INT					nPrepareTime;						// ����ʱ�䣨���룩
	INT					nCoolDown;							// ��ȴʱ�䣨���룩
	ESkillDmgType		eDmgType;							// �˺�����
	INT					nEnmity;							// ���ܳ��
	INT					nHit;								// ����������
	INT					nCrit;								// ����������
	BOOL				bMoveable;							// �Ƿ�����ƶ�ʩ��

	// �ܵ�
	INT					nChannelDmg[MAX_CHANNEL_TIMES];		// �ܵ��˺�ÿһ�ε��˺���
	INT					nChannelTime[MAX_CHANNEL_TIMES];	// �ܵ��˺�ÿһ�ε�ʱ��
	INT					nDmgTimes;							// �ܵ��˺��ܴ���

	// ״̬
	DWORD				dwBuffID[MAX_BUFF_PER_SKILL];		// ��������buff
	DWORD				dwTriggerID[MAX_CHANNEL_TIMES];		// ����������buff������

	// ����
	DWORD				dwCostItemID;						// ������ƷID
	INT					nCostItemNum;						// ������Ʒ����
	INT					nSkillCost[ESCT_End];				// ��������

	// ����
	INT					nLevel;								// ���ܵȼ�
	ESkillLevelUpType	eLevelUpType;						// �������ͣ��̶��������ȣ�
	INT					nLevelUpExp;						// ��Ҫ�����ȣ�������������������ļ��ܣ�
	DWORD				dwPreLevelSkillID;					// ǰ�ü�������ID
	INT					nMaxLevel;							// �ȼ�����
	INT					nMaxLearnLevel;						// ���ܿ�ѧ���ޣ�С�ڵ��ڵȼ����ޣ�
	INT					nNeedRoleLevel;						// ������������ҵȼ�����
	INT					nNeedTalentPoint;					// ����������Ҫ����ҵ�ǰ����Ͷ����
	//EClassType			eNeedClassType;						// ����������Ҫ��ְҵ����

	// ѧϰ����
	DWORD				dwLearnVocationLimit;				// ѧϰ���ܵ�ְҵ����

	// ʹ������
	INT					nUseHPPctLimit;						// �����ٷֱ�����
	INT					nUseMPPctLimit;						// �����ٷֱ�����
	ESkillSexLimit		eSexLimit;							// �Ա�����
	EClassType			eClassLimit;						// ְҵ����
	INT					nWeaponLimit;						// ��������
	ESkillPosType		ePosType;							// λ������
	DWORD				dwTargetLimit;						// Ŀ���������
	DWORD				dwSelfStateLimit;					// ����״̬����
	DWORD				dwTargetStateLimit;					// Ŀ��״̬����
	BOOL				bInterCombat;						// ʹ�ú��Ƿ���뱸ս
	DWORD				dwBuffLimitID;						// ��������״̬����ID
	DWORD				dwTargetBuffLimitID;				// Ŀ������״̬����ID
	DWORD				dwVocationLimit;					// ְҵ����
	
	// �������ӳ�
	INT					nTriggerEventMisc1Add;				// �¼�����ֵ1�ӳ�
	INT					nTriggerEventMisc2Add;				// �¼�����ֵ2�ӳ�
	INT					nTriggerStateMisc1Add;				// ״̬����ֵ1�ӳ�
	INT					nTriggerStateMisc2Add;				// ״̬����ֵ2�ӳ�
	INT					nTriggerPropAdd;					// �������ʼӳ�

	// ״̬�ӳ�
	INT					nBuffPersistTimeAdd;				// ����ʱ��ӳɣ����룩
	INT					nBuffWarpTimesAdd;					// ���Ӵ������޼ӳ�
	INT					nBuffInterruptResistAdd;			// �����������Լӳ�
	EBuffEffectMode		eModBuffMode;						// Ӱ��Buff���ĸ�����Ч���׶�
	INT					nBuffMisc1Add;						// ����Ч��ֵ�ӳ�
	INT					nBuffMisc2Add;						// ����Ч��ֵ�ӳ�
	INT					nBuffEffectAttMod[EBEA_End];		// Ӱ���ĳ���׶ε����ﵱǰ����

	// ��������Ӱ��
	mutable TMap<ERoleAttribute, INT>	mapRoleAttMod;		// ��������Ӱ��
	mutable TMap<ERoleAttribute, INT>	mapRoleAttModPct;	// �������԰ٷֱ�Ӱ��
	// ���е��ַ�������û����������棬��Щ�����У�
	// ���ƣ�������������ţ���Ч��ţ�ͼ���ţ����ֶ������

};

//--------------------------------------------------------------------------------
// ���ܴ��Ϳͻ��˵���Ϣ�ṹ
//--------------------------------------------------------------------------------
struct tagSkillMsgInfo
{
	DWORD	dwID;							// ����ID
	INT		nLevel;							// ��ǰ�ȼ�
	INT		nLearnLevel;					// ��ǰͶ��ȼ�
	INT		nCoolDown;						// ��ȴ
	INT		nMaxCoolDown;					// ���ܵ������ȴʱ��
	INT		nProficiency;					// ������
	FLOAT	fOpDist;						// ���þ���
	INT		nPrepareTime;					// ����ʱ��
	INT		nChannelDmg[MAX_CHANNEL_TIMES];	// �˺�
	INT		nCost[ESCT_End];				// ����
};

//--------------------------------------------------------------------------------
// ����ģ��ṹ�ͻ��˰�
//--------------------------------------------------------------------------------*/
struct tagSkillProtoClient : public tagSkillProto
{
	TCHAR					szName[X_SHORT_NAME];			// ��������
	TCHAR					szDesc[X_HUGE_NAME];			// ��������
	TCHAR					szIcon[X_SHORT_NAME];			// ����ͼ��
	TCHAR					szActionMod[X_SHORT_NAME];		// ����ʹ�ö���ģ��
	TCHAR					szPrepareMod[X_SHORT_NAME];		// ����ʹ�����ֶ���ģ��
	TCHAR					szNextDesc[X_HUGE_NAME];		// ��һ����������
	TCHAR					szSound[X_LONG_NAME];			// ���ܹ�����Ч
	TCHAR					szPreSfx[X_SHORT_NAME];			// ������Ч
	std::vector<tstring>	vecTagPreSfx;					// ������Ч���ص���������
	TCHAR					szAtkSfx[X_SHORT_NAME];			// ������Ч
	std::vector<tstring>	vecTagAtkSfx;					// ������Ч���ص���������
	TCHAR					szLchSfx[X_SHORT_NAME];			// ������Ч
	TCHAR					szTagLchSfx[X_SHORT_NAME];		// ������Ч���ص���������
	TCHAR					szBatkSfx[X_SHORT_NAME];		// ��������Ч�������
	INT						nNumBatkSfx;					// ��������Ч����
	BOOL					bActionBlend;					// ���ܶ����Ƿ�������ֿ�
	BOOL					bAutoNormalAttack;				// �����Ƿ��Զ�����ͨ����
};


//----------------------------------------------------------------------------------
// ��ͨ����
//----------------------------------------------------------------------------------
const INT X_NORMAL_ATTACK_SKILL_NUM = 7;

const DWORD NORMAL_ATTACK_SKILL_ID[X_NORMAL_ATTACK_SKILL_NUM] = // ��ͨ��������ID����ID��
{
	10100,					// ͽ����ͨ����
	10101,					// ����ͨ����
	10102,					// ����ͨ����
	10103,					// ������ͨ����
	10104,					// ����ͨ����
	10105,					// ������ͨ����
	10106					// ����ͨ����
};

//-----------------------------------------------------------------------------------
// ĳ�����ܵ�TypeID�ǲ�����ͨ��������
//-----------------------------------------------------------------------------------
inline BOOL IsNormalAttackSkill(DWORD dwSkillID)
{
	for(INT n = 0; n < X_NORMAL_ATTACK_SKILL_NUM; n++)
	{
		if( NORMAL_ATTACK_SKILL_ID[n] == dwSkillID )
			return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------------
// �ɼ��༼��
//-----------------------------------------------------------------------------------
const DWORD GATHER_SKILL_MINING		= 90000;	// ���ɣ��ɼ���ʯ����ҩ
const DWORD GATHER_SKILL_HARVEST	= 90001;	// �ջ񣬲ɼ�ʳ�ļ�����

//-----------------------------------------------------------------------------------
// ������＼��tbc:inves
//-----------------------------------------------------------------------------------
const DWORD INVES_SKILL				= 90010;

#pragma pack(pop)
