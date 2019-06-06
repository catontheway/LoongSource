#pragma once

#include "base_define.h"
#include "AvatarAtt.h"
#include "QuestDef.h"

#pragma pack(push, 1)

const INT	MAX_ROLENUM_ONEACCOUNT		= 5;				// �ʺŵ�����ɫ����
const INT	MAX_ROLE_LEVEL				= 150;				// ��ҿ���������߼���
const DWORD MAX_SAFECODE_RESET_TIME		= (72 * 60 * 60);	// ��ȫ���������ʱ��(72Сʱ)

const INT	ROLE_LEVEL_LIMIT			= 100;				// �ȼ�����

//---------------------------------------------------------------------------
// ���ݱ仯״̬,���ݿⱣ�����
//---------------------------------------------------------------------------
enum EUpdateDBStatus
{
	EUDBS_Null			= 0, // ������(Ĭ��״̬)
	EUDBS_Insert		= 1, // ������¼
	EUDBS_Remove		= 2, // ɾ��
	EUDBS_Update		= 3, // ���ݸ���
};

//---------------------------------------------------------------------------
// �Ա�
//---------------------------------------------------------------------------
enum ESex
{
	ES_Null			= -1,
	ES_Woman		= 0,	// Ů���ƣ���Ů����
	ES_Man			= 1,	// �����ƣ����п���
};

//----------------------------------------------------------------------------
// ְҵ����
//----------------------------------------------------------------------------
//enum EClassType
//{
//	ECT_Start		= -1,
//
//	ECT_Null		= -1,
//	ECT_Hobo		= 0,	// ����
//	ECT_Sword		= 1,	// ����
//	ECT_Blade		= 2,	// ����
//	ECT_Wand		= 3,	// �ȿ�
//	ECT_Bow			= 4,	// ����
//	ECT_Valor		= 5,	// ��ʦ
//	ECT_Supple		= 6,	// ��ʦ
//	ECT_Apothecary	= 7,	// ҩʦ
//	ECT_Musician	= 8,	// ��ʦ
//	ECT_Stabber		= 9,	// �̿�
//
//	ECT_End			= 10,
//};
//
//const INT32 X_ClASS_TYPE_NUM = ECT_End - ECT_Start - 1;

//---------------------------------------------------------------------------
// ְҵ������չ
//---------------------------------------------------------------------------
//enum EClassTypeEx
//{
//	ECTE_Base		= 0x1,	// ����ְҵ
//
//	ECTE_JianXian	= 0x2,	// ����
//	ECTE_XieWu		= 0x4,	// а��
//
//	ECTE_BaDao		= 0x2,	// �Ե�
//	ECTE_GangRen	= 0x4,	// ���
//
//	ECTE_ZongZhang	= 0x2,	// ����
//	ECTE_DiMo		= 0x4,	// ��ī
//
//	ECTE_TianJian	= 0x2,	// ���
//	ECTE_MoLing		= 0x4,	// ħ��
//
//	ECTE_XuXing		= 0x2,	// ����
//	ECTE_GuiQi		= 0x4,	// ����
//
//	ECTE_XuangWang	= 0x2,	// ����
//	ECTE_MiZong		= 0x4,	// ����
//
//	ECTE_XianYi		= 0x2,	// ��ҽ
//	ECTE_DuYin		= 0x4,	// ����
//n 
//	ECTE_YuSheng	= 0x2,	// ��ʥ
//	ECTE_HuanYin	= 0x4,	// ����
//
//	ECTE_YingWu		= 0x2,	// Ӱ��
//	ECTE_GuiWu		= 0x4,	// ����
//};

//---------------------------------------------------------------
// ר��ְҵ
//---------------------------------------------------------------
enum EClassType				 
{
	EV_Begin = -1,
    EV_Null  = 0,
	
	EV_Base	= 1,	// ����ְҵ������

	EV_Sword,		// ����
	EV_Blade,		// ����
	EV_Wand,		// ���
	EV_Bow,			// ����
	EV_Strength,	// ��ʦ
	EV_Toughness,	// ڤʦ
	EV_Apothecary,	// ҩʦ
	EV_Musician,	// ��ʦ

	EV_End,
};

const INT32 X_ClASS_TYPE_NUM = EV_End - EV_Begin - 2;
//--------------------------------------------------------------
// Ӣ��ְҵ
//--------------------------------------------------------------
enum EClassTypeEx
{
	EHV_Begin = -1,

	EHV_Base = 0,		// ����ְҵ��δѡ��Ӣ��ְҵ

	EHV_JianXian,		// �ɽ�
	EHV_XieWu,			// а��

	EHV_BaDao,			// �Ե�
	EHV_YingWu,			// Ӱ��
	
	EHV_JinGang,		// ���
	EHV_KuangZhan,		// ��ս
	
	EHV_TianJian,		// ���
	EHV_MoLing,			// ħ��
	
	EHV_XuanHuang,		// ����
	EHV_MiaoFa,			// �
	
	EHV_MingWang,		// ڤ��
	EHV_XuKong,			// ���
	
	EHV_XianYi,			// ��ҽ
	EHV_DuYin,			// ����
	
	EHV_YueSheng,		// ��ʥ
	EHV_HuanYin,		// ����

	EHV_End,
};

//--------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------
enum ETalentType
{
	ETT_Start		= -1,
	ETT_Null		= -1,

	ETT_Sword		= 0, // ��
	ETT_Blade		= 1, // ��
	ETT_Wand		= 2, // ����
	ETT_Bow			= 3, // ��
	ETT_Action		= 4, // ս��
	ETT_Valor		= 5, // ����
	ETT_Supple		= 6, // ����
	ETT_Poison		= 7, // ҽ��
	ETT_Swing		= 8, // ����
	ETT_Jugglery	= 9, // ����

	ETT_End			= 10,
};

const INT32 X_TALENT_TYPE_NUM = ETT_End - ETT_Start - 1;

//--------------------------------------------------------------------------
// �������ͣ����ܻ����������͵�������
//--------------------------------------------------------------------------
// enum EReputationType
// {
// 	ERT_Start		= -1,
// 	ERT_Null		= -1,
// 
// 	ERT_End			= 0,
// };

//--------------------------------------------------------------------------
// ��ɫ�����﹫��״̬��ʾ
//--------------------------------------------------------------------------
enum EState
{
	ES_Dead			=	0,	// ����
	ES_Dizzy		=	1,	// ѣ��
	ES_Tie			=	2,	// ����
	ES_Spor			=	3,	// ��˯
	ES_Invincible	=	4,	// �޵�
	ES_Lurk			=	5,	// ����
	ES_DisArm		=	6,	// ��е
	ES_NoSkill		=	7,	// �⼼

	ES_End			=	8,
};

//--------------------------------------------------------------------------
// ��ɫ״̬��ʶ -- ��״̬�ı䣬��Ҫ����Χ��ҹ㲥
//--------------------------------------------------------------------------
enum ERoleState
{
	ERS_Safeguard	= 0x0001,	// ������
	ERS_Combat		= 0x0002,	// ս����̬
	ERS_PK			= 0x0004,	// ����״̬1������������ң�
	ERS_Arena		= 0x0008,	// ��̨(����PVP���ս������״̬�ɽű����ã�

	ERS_SafeArea	= 0x0010,	// ��ȫ��
	ERS_PVPArea		= 0x0020,	// PVP��
	ERS_StallArea	= 0x0040,	// ��̯��
	ERS_PVP			= 0x0080,	// PVP״̬

	ERS_Shop		= 0x0100,	// �̵꽻��
	ERS_Exchange	= 0x0200,	// ��Ҽ�ֱ�ӽ���
	ERS_Stall		= 0x0400,	// ��̯����
	ERS_StallSet	= 0x0800,	// ���ð�̯��Ʒ

	ERS_PrisonArea	= 0x1000,	// ����״̬

	ERS_Mount		= 0x2000,	// ���
	ERS_WaterWalk   = 0x4000,   // ˮ������
	ERS_Mount2		= 0x8000,	// ���2

	ERS_Commerce	= 0x10000,	// ������	
	ERS_PetExchange	= 0x20000,	// ���ｻ��

	ERS_Hostility	= 0x40000,	// ����״̬

	ERS_Wedding		= 0x80000,	// ����״̬

	ERS_PKEX		= 0x100000,	//	����״̬2��ֻ����������ң�

	ERS_End			= 0x40000000,
};

//--------------------------------------------------------------------------
// ��ɫ״̬��ʶ -- ��״̬�ı䣬ֻ��Ҫ֪ͨ��ұ���
//--------------------------------------------------------------------------
enum ERoleStateEx
{
	ERSE_BagPsdPass		= 0x1,	// ����������֤��ͨ��
	ERSE_BagPsdExist	= 0x2,	// �����������
};

//--------------------------------------------------------------------------
// ��ɫPK״̬
//--------------------------------------------------------------------------
enum ERolePKState
{
	ERPKS_Peace = 0,			// ��ƽ
	ERPKS_Warnning,				// ����
	ERPKS_Wicked,				// а��
	ERPKS_Wanted,				// ͨ��
	ERPKS_End,
};


//-------------------------------------------------------------------------------
// �����������ö��(�ô�ö��˳�򡢸�������,���ܻ�Ӱ�쵽���ݿ�װ����)
//-------------------------------------------------------------------------------
enum ERoleAttribute
{
	ERA_Null					=	-1,		//
	
	// һ������
	ERA_AttA_Start				=	0,
	ERA_Physique				=	0,		// ��ǰ���	
	ERA_Strength				=	1,		// ��ǰ����	
	ERA_Pneuma					=	2,		// ��ǰԪ��
	ERA_InnerForce				=	3,		// ��ǰ����	
	ERA_Technique				=	4,		// ��ǰ����	
	ERA_Agility					=	5,		// ��ǰ��
	ERA_AttA_End				=	5,

	// ��ҵĵ���
	ERA_AttPoint				=	6,		// ��ǰ���Ե���
	ERA_TalentPoint				=	7,		// ��ǰ���ʵ���

	// ��������
	ERA_AttB_Start				=	8,
	ERA_MaxHP					=	8,		// �������
	ERA_HP						=	9,		// ��ǰ����
	ERA_MaxMP					=	10,		// �������
	ERA_MP						=	11,		// ��ǰ����
	ERA_ExAttack				=	12,		// �⹦����
	ERA_ExDefense				=	13,		// �⹦����
	ERA_InAttack				=	14,		// �ڹ�����
	ERA_InDefense				=	15,		// �ڹ�����
	ERA_AttackTec				=	16,		// ��������
	ERA_DefenseTec				=	17,		// ��������
	ERA_HitRate					=	18,		// ������
	ERA_Dodge					=	19,		// ������
	ERA_Endurance				=	20,		// �־���
	ERA_MaxEndurance			=	21,		// �־���
	ERA_AttB_End				=	22,

	// ��������
	ERA_HPRegainRate			=	22,		// �����ָ��ٶ�
	ERA_MPRegainRate			=	23,		// �����ָ��ٶ�
	ERA_Rage					=	24,		// ŭ��
	ERA_Vitality				=	25,		// ����
	ERA_MaxVitality				=	26,		// ������
	ERA_VitalityRegainRate		=	27,		// �����ָ��ٶ�
	ERA_Speed_XZ				=	28,		// XZƽ���ٶ��ٶ�
	ERA_Speed_Y					=	29,		// Y�����ٶ�
	ERA_Speed_Swim				=	30,		// ��Ӿ��XZ�ٶ�
	ERA_Speed_Mount				=	31,		// ��˵�XZ�ٶ�

	ERA_EngraveAtt_Start		=	32,		// �Կ�Ӱ������
	ERA_WeaponDmgMin			=	32,		// ��С��������
	ERA_WeaponDmgMax			=	33,		// �����������
	ERA_WeaponSoul				=	34,		// ���
	ERA_Armor					=	35,		// ���߻���
	ERA_EngraveAtt_End			=	35,		// �Կ�Ӱ������

	ERA_Derate_Start			=	36,
	ERA_Derate_Ordinary			=	36,		// ��ͨ�˺�����
	ERA_Derate_Bleeding			=	37,		// ��Ѫ�˺�����
	ERA_Derate_Brunt			=	38,		// ����˺�����
	ERA_Derate_Bang				=	39,		// �ػ��˺�����
	ERA_Derate_Poison			=	40,		// �����˺�����
	ERA_Derate_Thinker			=	41,		// �����˺�����
	ERA_Derate_Injury			=	42,		// �����˺�����
	ERA_Derate_Stunt			=	43,		// �����˺�����
	ERA_Derate_ExAttack			=	44,		// �⹦�˺�����
	ERA_Derate_InAttack			=	45,		// �ڹ��˺�����
	ERA_Derate_ALL				=	46,		// �����˺�����
	ERA_Derate_End				=	46,

	ERA_ExDamage				=	47,		// �����˺�
	ERA_ExDamage_Absorb			=	48,		// �����˺�����

	ERA_Resist_Bleeding			=	49,		// ��Ѫ���ԡ������м���
	ERA_Resist_Weak				=	50,		// �������ԡ������м���
	ERA_Resist_Choas			=	51,		// ���ҿ��ԡ������м���
	ERA_Resist_Special			=	52,		// ���⿹�ԡ������м���
	ERA_Regain_Addtion			=	53,		// �ָ����ӳ�

	ERA_Attack_MissRate			=	54,		// ����ʧ����
	ERA_CloseAttack_DodgeRate	=	55,		// ��ս����ʼӳ�
	ERA_RemoteAttack_DodgeRate	=	56,		// Զ�̶���ʼӳ�

	ERA_Crit_Rate				=	57,		// �����ʼӳ�
	ERA_Crit_Amount				=	58,		// �������ӳ�
	ERA_Block_Rate				=	59,		// ���ʼӳ�

	ERA_Inspiration				=	60,		// ���
	ERA_Lurk					=	61,		// ����
	ERA_Savvy					=	62,		// ����
	ERA_Morale					=	63,		// ʿ��
	ERA_Injury					=	64,		// ����
	ERA_Enmity_Degree			=	65,		// ��޶�
	ERA_Shape					=	66,		// ����
	ERA_Exp_Add_Rate			=	67,		// �����üӳ�		ԭ����/��Ǯ/���� * (1+�ӳ�/10000)
	ERA_Money_Add_Rate			=	68,		// ��Ǯ��üӳ�
	ERA_Loot_Add_Rate			=	69,		// ��Ʒ���伸�ʼӳ�

	// �������
	ERA_Fortune					=	70,		// ��Ե
	ERA_Appearance				=	71,		// ����
	ERA_Rein					=	72,		// ͳ��
	ERA_Knowledge				=	73,		// ����
	ERA_Morality				=	74,		// ����
	ERA_Culture					=	75,		// ��Ϊ
	ERA_Hostility				=	76,		// ����

	ERA_End						=	77,
	// ��ҵ�������������Ͷ�����Ե㣬�Ѿ�Ͷ�ŵ����ʵ�û�зŵ�����������
};

const INT32	X_ERA_ATTA_NUM		= ERA_AttA_End - ERA_AttA_Start + 1;
#define MTransERAAttA2Index(ERAAttA)		((ERAAttA) - ERA_AttA_Start)
#define MTransIndex2ERAATTA(Index)			((Index) + ERA_AttA_Start)

const INT32 X_ERA_AttB_NUM		= ERA_AttB_End - ERA_AttB_Start;
#define MTransERAAttB2Index(ERAAttB)		((ERAAttB) - ERA_AttB_Start)
#define MTransIndex2ERAATTB(Index)			((Index) + ERA_AttB_Start)

const INT32	X_ERA_DERATE_NUM	= ERA_Derate_End - ERA_Derate_Start + 1;
#define MTransERADerate2Index(ERADerate)	((ERADerate) - ERA_Derate_Start)
#define MTransIndex2ERADerate(Index)		((Index) + ERA_Derate_Start)

//--------------------------------------------------------------------------------------
// Զ����ҵ�����ö��
//--------------------------------------------------------------------------------------
enum ERemoteRoleAtt
{
	ERRA_Null		=	-1,		//

	ERRA_MaxHP		=	0,		// �������
	ERRA_HP			=	1,		// ��ǰ����
	ERRA_MaxMP		=	2,		// �������
	ERRA_MP			=	3,		// ��ǰ����
	ERRA_Rage		=	4,		// ŭ��
	ERRA_Speed_XZ	=	5,		// XZƽ���ٶ�
	ERRA_Speed_Y	=	6,		// Yƽ���ٶ�
	ERRA_Speed_Swim	=	7,		// ��Ӿ�ٶ�
	ERRA_Speed_Mount=	8,		// ����ٶ�
	ERRA_Shape		=	9,		// ����
	ERRA_Hostility	=	10,		// ����״̬

	ERRA_End		=	11,
};

//-----------------------------------------------------------------------------------------
// Ŀ�����ͱ�ʾλ
//-----------------------------------------------------------------------------------------
enum ETargetFlag
{
	ETF_Self				=	0x00000001,	// ����
	ETF_Teammate			=	0x00000002,	// ����
	ETF_Guildmate			=	0x00000004,	// ͬ��
	ETF_Lover				=	0x00000008,	// ����
	ETF_Couple				=	0x00000010,	// ��ż
	ETF_Brother				=	0x00000020,	// ���
	ETF_Teacher				=	0x00000040,	// ʦͽ
	ETF_Player				=	0x00000080,	// ���

	ETF_Pet					=	0x00000100,	// ����
	ETF_NPC					=	0x00000200,	// NPC
	ETF_NormalMonster		=	0x00000400,	// ��ͨ����
	ETF_Boss				=	0x00000800,	// Boss
	ETF_Nest				=	0x00001000,	// ��Ѩ
	ETF_NatuRes				=	0x00002000,	// ��Ȼ��Դ
	ETF_ManRes				=	0x00004000,	// ������Դ
	ETF_Door				=	0x00008000,	// ����
	ETF_Building			=	0x00010000,	// ����
	ETF_NormalGameObject	=	0x00020000,	// ��ͨ�ɽ�������
	ETF_InvesGameObject		=	0x00040000,	// �ɵ������
	ETF_QuestTalkGameObject	=	0x00080000,	// ����Ի�����
};

//-----------------------------------------------------------------------------------------
// ״̬��ʾλ
//-----------------------------------------------------------------------------------------
enum EStateFlag
{
	ESF_Dead			=	0x00000001,	// ����
	ESF_NoDead			=	0x00000002,	// ������
	ESF_Dizzy			=	0x00000004,	// ѣ��
	ESF_NoDizzy			=	0x00000008,	// ��ѣ��
	ESF_Spor			=	0x00000010,	// ��˯
	ESF_NoSpor			=	0x00000020,	// �ǻ�˯
	ESF_Tie				=	0x00000040,	// ����
	ESF_NoTie			=	0x00000080,	// �Ƕ���
	ESF_Invincible		=	0x00000100,	// �޵�
	ESF_NoInvincible	=	0x00000200,	// ���޵�
	ESF_NoSkill			=	0x00000400,	// �⼼
	ESF_NoNoSkill		=	0x00000800, // �Ƿ⼼
	ESF_DisArm			=	0x00001000,	// ��е
	ESF_NoDisArm		=	0x00002000,	// �ǽ�е
	ESF_Lurk			=	0x00004000,	// ����
	ESF_NoLurk			=	0x00008000,	// ������
	ESF_Mount			=	0x00010000,	// ���
	ESF_NoMount			=	0x00020000, // �����
	ESF_Prison			=	0x00040000,	// ����
	ESF_NoPrison		=	0x00080000,	// ������
	ESF_Mount2			=	0x00100000,	// ���2
	ESF_NoMount2		=	0x00200000, // �����2
	ESF_Commerce		=	0x00400000,	// ������
	ESF_NoCommerce		=	0x00800000,	// ��������

};

//-----------------------------------------------------------------------------------------
// ��������������Ե�Ĭ��ֵ�������Сֵ
//-----------------------------------------------------------------------------------------
struct tagAttDefMinMax
{
	INT		nDefRole;			// �����ʼ��Ĭ��ֵ
	INT		nDefCreature;		// �����ʼ��Ĭ��ֵ
	INT		nMin;				// ������Сֵ
	INT		nMax;				// �������ֵ
};

////---------------------------------------------------------------------------------------
//// �ͻ��˱������ͬ��������Ϣ�ṹ��δ��ɣ�
////---------------------------------------------------------------------------------------
//struct tagRoleData
//{
//	DWORD			dwID;						// ���ID
//	DWORD			dwRoleNameCrc;				// ����CRC
//	TCHAR			szRoleName[X_SHORT_NAME];	// ����
//
//	DWORD			dwMapID;					// ���ڵĵ�ͼ��ID
//	FLOAT			fPos[3];					// λ��
//	FLOAT			fFaceTo[3];					// ����
//
//	tagAvatarAtt	Avatar;						// ���
//	tagAvatarEquip	AvatarEquip;				// װ�����
//	tagDisplaySet	DisplaySet;					// �����ʾ����
//	INT				nAtt[ERA_End];				// ��ǰ��������
//
//	INT				nAttPointAdd[X_ERA_ATTA_NUM];// ���Ͷ�ŵ�����һ�������е�ֵ
//
//	EClassType		eClass;						// ְҵ
//	EClassTypeEx	eClassEx;					// ��չְҵ
//
//	INT16			n16MaxBagSize;				// ������С
//	INT16			n16MaxWareSize;				// �˺Ųֿ��С
//
//	INT				nSkillNum;					// ���ܸ���
//	INT				nBuffNum;					// ״̬����
//	INT				nTitleNum;					// �ƺŸ���
//	INT				nItemNum;					// ��Ʒ����
//	INT				nEquipNum;					// װ������
//	INT				nFriendNum;					// ���Ѹ���
//	INT				nEnemyNum;					// ��и���
//	INT				nQuestNum;					// �����б����
//	INT				nQuestDoneNum;				// ����ɵ��������
//
//	/*********************************************************************************************************
//	 *�����б�״̬�б��ƺ��б���Ʒ�б�װ���б������б�����б������б�����������б�
//	 *���밴˳�򿽱�
//	 *********************************************************************************************************/
//	//�����б�
//	DWORD			dwIncompleteQuestID[QUEST_MAX_COUNT];			// �ڵ�ǰ�������δ��ɵ����� 30����
//	DWORD			dwIncompleteQuestTakeTime[QUEST_MAX_COUNT];		// �ڵ�ǰ������ϵ�ǰ����Ľ�ȡʱ�� 30����
//	//�����б�
//	
//
//	BYTE			byData[1];
//};

//------------------------------------------------------------------------------------------
// ��ѡ�˽���Ҫ������������Ϣ
//------------------------------------------------------------------------------------------
struct tagSimRole								// size = 64
{
	tagAvatarAtt	Avatar;						// size = 16
	tagAvatarEquip	AvatarEquip;				// size = (2 * 9 + 1) * 4
	TCHAR			szRoleName[X_SHORT_NAME];

	DWORD			dwRoleID;
	DWORD			dwMapID;
	BYTE			byLevel;
	tagDisplaySet	sDisplaySet;
	BYTE			byReserved[2];
};

//------------------------------------------------------------------------------------------
// �Ƿ��Զ����ҹ��������Ϣ -- ע�����ݿ���¼��32λ
//------------------------------------------------------------------------------------------
struct tagRemoteOpenSet
{
	DWORD	bTitle		: 1,	// �ƺ�
			bVCard		: 1,	// ����
			bRoleAtt	: 1,	// ��ɫ����
			bDummy		: 29;	// ռλ��

	tagRemoteOpenSet() { ZeroMemory(this, sizeof(*this)); }
	
	tagRemoteOpenSet& operator = (DWORD dw)
	{
		return *this = *(tagRemoteOpenSet*)&dw;
	}

	operator DWORD()
	{
		return *(DWORD*)this;
	}
};


//------------------------------------------------------------------------------------------
// �����Ϣ�Ĺ�������
//------------------------------------------------------------------------------------------
enum EPlayerPersonlInfoOpenType
{
	EPIS_Private,  // ��ȫ����
	EPIS_Friend,   // ���Ի�Ϊ���ѵ���ҹ���
	EPIS_Public,   // ��ȫ����
	EPIS_End,
};

//------------------------------------------------------------------------------------------
// ��ҵĸ�������
//------------------------------------------------------------------------------------------
struct tagDWPlayerPersonalSet
{
	DWORD	bAcceptTeamInvite		: 1,	// �Ƿ�����������
			bAcceptFactionInvite	: 1,	// �Ƿ���ܰ�������
			bAllowFriendAdd			: 1,	// �Ƿ���������������Լ�Ϊ����
			bAcceptTradeRequest		: 1,	// �Ƿ���ܽ�������
			eInfoOpenType           : 4,    // ��Ϣ�Ĺ�������
			bDummy					: 24;	// ռλ��

	tagDWPlayerPersonalSet() { ZeroMemory(this, sizeof(*this)); }

	tagDWPlayerPersonalSet& operator = (DWORD dw)
	{
		return *this = *(tagDWPlayerPersonalSet*)&dw;
	}

	operator DWORD()
	{
		return *(DWORD*)this;
	}
};

//-------------------------------------------------------------------------------------
// ʵ��ֵ
//-------------------------------------------------------------------------------------
enum EStrengthType
{
	EStrengthT_Begin = 0,

	EStrengthT_Entire = 1,	// ʵ��ֵ
	EStrengthT_Heroism,		// Ӣ��ֵ
	EStrengthT_Wit,			// ı��ֵ
	EStrengthT_Errantry,	// ����ֵ
	EStrengthT_Valor,		// ��սֵ

	EStrengthT_End,
};

#pragma pack(pop)
