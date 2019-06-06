#pragma once

#include "RoleDefine.h"
#include "container_define.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/reputation.h"
#include "specfunc_define.h"

#pragma pack(push, 1)

class ItemScript;

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
const INT MAX_ITEM_TYPE_NUM				= 100000;	// ��̬���У���Ʒװ�����������

const INT MAX_ROLEATT_BASE_EFFECT		= 6;	// װ��ԭ���п���д��Ӱ�����Ը���

const INT MAX_POSY_TIMES				= 20;	// һ��װ�������������Ĵ���
const INT MAX_ROLEATT_POSY_EFFECT		= 4;	// һ��װ�����������Ӱ������Ը���

const INT MAX_ENGRAVE_TIMES				= 20;	// һ��װ�����������Կ̴���
const INT MAX_ROLEATT_ENGRAVE_EFFECT	= 4;	// һ��װ�����Կ����Ӱ������Ը���

const INT MAX_ROLEATT_ENHANCE_EFFECT	= 3;	// ǿ��װ���Ĳ���Ӱ������Ը���
const INT MAX_EQUIPHOLE_NUM				= 5;	// һ��װ������������Ƕ������

const INT MAX_BRAND_LEVEL				= 10;	// װ������ӡ�ȼ�����

const INT MAX_LONGHUN_LEVEL				= 10;	// ����ȼ�����


const INT ABRASION2USETIMES				= 25;	// ����ʹ�ô�����ո�¶Ƚ���.1��ո�¶� = ʹ��50��

const INT MAX_ITEM_DCTIME				= 7 * 24 * 60 * 60;	// ��Ʒ���ȴʱ��(��λ����)

const INT MAX_EQUIP_ATTACH_NUM          = 3;    // װ�����ر���ģ���������

const INT MAX_PEREQUIP_SUIT_NUM			= 3;	// һ��װ����������3����װ

const INT MAX_EQUIP_BUFF_NUM			= 5;	// װ���ϴ���buff������
//-----------------------------------------------------------------------------
// ö�ٶ���
//-----------------------------------------------------------------------------
enum EItemType
{
	EIT_Null		= 0,
	EIT_Expend		= 1,	// ����Ʒ
	EIT_Unexpend	= 2,	// ������Ʒ
	EIT_Spec		= 3,	// ������Ʒ
	EIT_Pet			= 4,	// ����
	EIT_Bag			= 5,	// ��Ʒ��
	EIT_Mall		= 6,	// �̳ǵ���

	EIT_Weapon		= 7,	// ����
	EIT_ClothArmor	= 8,	// ����
	EIT_Decoration	= 9,	// ��Ʒ
	EIT_Fashion		= 10,	// ʱװ
	EIT_Armor		= 11,	// ����
};

enum EItemTypeEx
{
	EITE_Null		= 0,
	EITE_Resume		= 1,	// �ָ���
	EITE_Advance	= 2,	// ǿ����
	EITE_Other		= 3,	// ����
	EITE_Material	= 4,	// ԭ����
	EITE_Capture	= 5,	// ս��Ʒ
	EITE_Quest		= 6,	// ������Ʒ
	EITE_Book		= 7,	// �ؼ�
	EITE_Pet		= 8,	// ����
	EITE_Ride		= 9,	// ����
	EITE_Expend		= 10,	// ����
	EITE_Time		= 11,	// ʱ��

	EITE_Sword		= 12,	// ��
	EITE_Blade		= 13,	// ��
	EITE_Wand		= 14,	// ����
	EITE_Bow		= 15,	// �� 
	EITE_Faqi		= 16,	// ����
	EITE_Qin		= 17,	// ��
};

enum EItemTypeReserved
{
	EITR_Null		= 0,
	EITR_Yaopin		= 1,	// ҩƷ
	EITR_Shipin		= 2,	// ʳƷ
	EITR_Xiandan	= 3,	// �ɵ�
	EITR_Longhun	= 4,	// ����
	EITR_Yinji		= 5,	// ӡ��
	EITR_Baoshi		= 6,	// ��ʯ
	EITR_Qizhen		= 7,	// ����
	EITR_Ranliao	= 8,	// Ⱦ��
	EITR_Siliao		= 9,	// ����
	EITR_Fuzhou		= 10,	// ����
	EITR_Kuangshi	= 11,	// ��ʯ
	EITR_Caoyao		= 12,	// ��ҩ
	EITR_ShiCai		= 13,	// ʳ��
	EITR_Buliao		= 14,	// ����
	EITR_Mucai		= 15,	// ľ��
	EITR_Longyuan	= 16,	// ����ԭ��
	EITR_Yinyuan	= 17,	// ӡ��ԭ��

};

enum EItemQuality
{
	EIQ_Start		= -1,

	EIQ_Null		= -1,	// δ����
	EIQ_White		= 0,
	EIQ_Yellow		= 1,
	EIQ_Green		= 2,
	EIQ_Blue		= 3,
	EIQ_Orange		= 4,
	// Jason 2009-11-30 ������ɫƷ��
	EIQ_Purple		= 5,

	EIQ_End			= 6,
};

const INT32 X_EQUIP_QUALITY_NUM = EIQ_End - EIQ_Start - 1;

enum EBindMode	// ������
{
	EBM_Null		= 0,	// ��

	EBM_Gain		= 1,	// ��ð�
};

enum EBindStatus	// ��״̬
{
	EBS_Unknown		= 0,	// δ֪�ģ����̵��е���Ʒ
	EBS_Unbind		= 1,	// δ��
	EBS_Bind		= 2,	// ��
};

//enum ETimeMode		// ��ʱ��ʽ
//{
//	ETM_Null		= 0,
//	TEM_Create		= 1, // ���ɼ�ʱ
//	TEM_Online		= 2, // ���ϼ�ʱ
//	TEM_Battle		= 3, // ս����ʱ
//	TEM_Quest		= 4, // ���񴥷���ʱ
//	TEM_Used		= 5, // ʹ�ú��ʱ
//	TEM_Get			= 6, // ��ü�ʱ
//};

enum ESexLimit		// ��Ʒ��ʹ�õĽ�ɫ�Ա�,Ҫ��AvatarAtt���Ա��Ӧ
{
	ESL_Null		= -1,	// �����ƣ�����Ů������
	ESL_Woman		= 0,	// Ů���ƣ���Ů����
	ESL_Man			= 1,	// �����ƣ����п���
};

enum EClassLimit
{
	ECL_Sword		= 0x1,	// ����
	ECL_JianXian	= 0x2,	// ����
	ECL_XieWu		= 0x4,	// а��
					
	ECL_Blade		= 0x1,	// ����
	ECL_BaDao		= 0x2,	// �Ե�
	ECL_GangRen		= 0x4,	// ���
					
	ECL_Wand		= 0x1,	// �ȿ�
	ECL_ZongZhang	= 0x2,	// ����
	ECL_DiMo		= 0x4,	// ��ī
					
	ECL_Bow			= 0x1,	// ����
	ECL_TianJian	= 0x2,	// ���
	ECL_MoLing		= 0x4,	// ħ��
					
	ECL_Valor		= 0x1,	// ��ʦ
	ECL_XuXing		= 0x2,	// ����
	ECL_GuiQi		= 0x4,	// ����
					
	ECL_Supple		= 0x1,	// ��ʦ
	ECL_XuangWang	= 0x2,	// ����
	ECL_MiZong		= 0x4,	// ����

	ECL_Apothecary	= 0x1,	// ҩʦ
	ECL_XianYi		= 0x2,	// ��ҽ
	ECL_DuYin		= 0x4,	// ����

	ECL_Musician	= 0x1,	// ��ʦ
	ECL_YueSheng	= 0x2,	// ��ʥ
	ECL_HuanYin		= 0x4,	// ����

	ECL_Stabber		= 0x1,	// �̿�
	ECL_YingWu		= 0x2,	// Ӱ��
	ECL_GuiWu		= 0x4,	// ����
};

enum EWuXing	// ��������
{
	EWX_Start		= -1,
	EWX_Null		= -1,

	EWX_Metal		= 0,
	EWX_Wood		= 1,
	EWX_Water		= 2,
	EWX_Fire		= 3,
	EWX_Soil		= 4,

	EWX_End			= 5,
};

const INT32 X_WUXING_TYPE_NUM = EWX_End - EWX_Start - 1;


enum EStuffType		// ��������
{
	EST_Null		= 0,	//��
	EST_Stone		= 1,	//ʯ
	EST_Jade		= 2,	//��
	EST_Iron		= 3,	//��
	EST_Silver		= 4,	//��
	EST_Gold		= 5,	//��
	EST_ChunXinCao	= 6,	//������
	EST_XiaKuCao	= 7,	//�Ŀݲ�
	EST_QiuBaiCao	= 8,	//��ײ�
	EST_DongKuiCao	= 9,	//������
	EST_DongYangCao = 10,	//������
	EST_Flax		= 11,	//�鲼
	EST_Leather		= 12,	//Ƥ��
	EST_Cotton		= 13,	//�޲�
	EST_SilkOrgandy	= 14,	//��ɴ
	EST_Silk		= 15,	//˿��
	EST_Chop		= 16,	//���
	EST_Dew			= 17,	//¶ˮ
	EST_Grain		= 18,	//����
	EST_Bone		= 19,	//��ͷ
	EST_Potherb		= 20,	//Ұ��
	EST_IcePith		= 21,	// ����
	EST_FirePith	= 22,	// ��
	EST_WoodPith	= 23,	// ľ��
};

enum EItemCreateMode	// ��Ʒ&װ��������ʽ
{
	EICM_Null			= 0,	// û�м�¼
	EICM_GM				= 1,	// GM��������
	EICM_Loot			= 2,	// �������
	EICM_Quest			= 3,	// ������
	EICM_Product		= 4,	// �������
	EICM_ShopNPC		= 5,	// �̵�NPC
	EICM_AccQuest		= 6,	// ������
	EICM_Mall			= 7,	// �̳�
	EICM_Gift			= 8,	// ��Ʒ��
	EICM_ActTreasure	= 9,	// ���������䱦
	EICM_GMTool			= 10,	// GM��������
	EICM_PetSkill		= 11,	// ���＼��
	EICM_VipNetBar		= 12,	// ��������
	EICM_ReGet			= 13,	// ���߻ָ�
	EICM_Activity		= 14,	// �����
	EICM_LessingLoong	= 15,	// �����͸�
	EICM_GoldStone		= 16,	// ���ʯ
	EICM_GemRemoval		= 17,	// ��ʯ���
	EICM_ServerMerge	= 18,	// �Ϸ�����

};

enum EEnhanceEquipPos	// ��ǿ����װ������
{
	EEEP_Hand		= 0x00000001,	// ����
	EEEP_Head		= 0x00000002,	// ͷ��
	EEEP_Face		= 0x00000004,	// �沿
	EEEP_Body		= 0x00000008,	// ����
	EEEP_Legs		= 0x00000010,	// ����
	EEEP_Back		= 0x00000020,	// ����
	EEEP_Wrist		= 0x00000040,	// ��
	EEEP_Feet		= 0x00000080,	// �㲿
	EEEP_Finger		= 0x00000100,	// ��ָ
	EEEP_Waist		= 0x00000200,	// ����
	EEEP_Neck		= 0x00000400,	// ����
};

enum EEquipMdlType     // װ����ģ������
{
	EEMT_NULL	= 0,
	EEMT_FSKIN  = 1,   // ��Ƥ
	EEMT_FAK    = 2,   // �ؼ�֡
	EEMT_FSM    = 3,   // ��̬ģ��
};

enum EEquipAttachPos    // װ���ɱ��β��ֵĹ���λ��
{
	EEAP_NULL   = 0,
	EEAP_Hat    = 1,	// ͷ��
	EEAP_Back   = 2,	// ����
	EEAP_RSld   = 3,	// �Ҽ�
	EEAP_LSld   = 4,	// ���
	EEAP_Wep6   = 5,	// ������
	EEAP_Num    = 6,
};

enum EEquipSpecAtt		// װ��B������
{
	EESA_Null				= -1,
	EESA_Start				= 0,

	EESA_LevelLim_Simple	= 0,	// ����		��װ���ȼ�����-5����Ϳɼ�����0
	EESA_LevelLim_Fine		= 1,	// ����		��װ���ȼ�����-10����Ϳɼ�����0
	EESA_LevelLim_None		= 2,	// �޼���	��װ���޵ȼ�����

	EESA_AttALim_Simple		= 3,	// ���		��װ���������Ƽ���10%��ȡ��
	EESA_AttALim_Comfort	= 4,	// ����		��װ���������Ƽ���25%��ȡ��
	EESA_AttALim_Light		= 5,	// ��ӯ		��װ���������Ƽ���50%��ȡ��

	EESA_Potential_YinFeng	= 6,	// ����		��װ���ĳ�ʼǱ��ֵ+200
	EESA_Potential_YinHuang	= 7,	// ����		��װ���ĳ�ʼǱ��ֵ+400
	EESA_Potential_FeiFeng	= 8,	// �ɷ�		��װ���ĳ�ʼǱ��ֵ+800
	EESA_Potential_MingHuang= 9,	// ����		��װ���ĳ�ʼǱ��ֵ+1200
	EESA_Potential_WoLong	= 10,	// ����		װ���ĳ�ʼǱ��ֵ���5%
	EESA_Potential_CangLong	= 11,	// ����		װ���ĳ�ʼǱ��ֵ���10%
	EESA_Potential_FuLong	= 12,	// ����		װ���ĳ�ʼǱ��ֵ���20%
	EESA_Potential_ShengLong= 13,	// ����		װ���ĳ�ʼǱ��ֵ���30%

	EESA_EquipSpec_RelEnd	= 13,	// 

	EESA_Guard_Posy			= 14,	// �����ػ�	װ���ڽ�������ǿ��ʱ����ʧ��
	EESA_Guard_Brand		= 15,	// ��ӡ�ػ�	װ���ڽ�����ӡǿ��ʱ����ʧ��
	EESA_Guard_Engrave		= 16,	// �Կ��ػ�	װ���ڽ����Կ�ǿ��ʱ����ʧ��
	EESA_Guard_LongFu		= 17,	// �����ػ�	װ���ڽ�������ǿ��ʱ����ʧ��
	EESA_Guard_Quench		= 18,	// ����ػ�	װ���ڽ��д��ǿ��ʱ����ʧ��
	EESA_Guard_Cut			= 19,	// �����ػ�	װ���ڽ��п���ǿ��ʱ����ʧ��
	EESA_Guard_AllProduce	= 20,	// ����		װ���ڽ����κ�ǿ��ʱ����ʧ��

	EESA_Guard_Talisman		= 21,	// ��а		����װ��ǿ��ʱ�ɹ�������[����5%]
	EESA_Guard_Fortune		= 22,	// ����		����װ��ǿ��ʱ����������[����5%]

	EESA_Guard_DeadPunish	= 23,	// �����ػ�	��װ������������ʱ����

	EESA_End				= 23,
};

const INT X_EQUIP_SPECATT_NUM	= EESA_End - EESA_Start + 1;

enum ECorlor		// ��ɫ��ͼö��
{
	EC_Null		= -1,

	EC_Start	= 0,
	EC_Val0		= 0,
	EC_Val1		= 1,
	EC_Val2		= 2,
	EC_Val3		= 3,
	EC_Val4		= 4,
	EC_Val5		= 5,
	EC_Val6		= 6,
	EC_Val7		= 7,
	EC_Val8		= 8,
	EC_Val9		= 9,
	EC_Val10	= 10,
	EC_Val11	= 11,
	EC_End		= 11,
};

const INT X_COLOR_NUM	= EC_End - EC_Start + 1;

//-----------------------------------------------------------------------------
// �ṹ����
//-----------------------------------------------------------------------------
struct tagRoleAttEffect
{
	ERoleAttribute	eRoleAtt;
	INT32			nValue;

	tagRoleAttEffect() { ZeroMemory(this, sizeof(*this)); eRoleAtt = ERA_Null; }
};

struct tagEquipQltyPct
{
	DWORD				dwTypeID;
	INT32				nEquipQltyPct[X_EQUIP_QUALITY_NUM];	// Ʒ�����ɼ���
};

struct tagCDTime
{
	DWORD		dwTypeID;
	DWORD		dwTime;		// ��ȴʱ��(��λ������)
};

/** struct	tagItemDisplayInfo
brief	��Ʒ��ʾ�ṹ
remarks	��Ҫ���ڴ洢��Ʒ�����ơ����͡�������ģ�͵ȵ�	
*/
struct tagItemDisplayInfo
{
	TCHAR				szType[X_SHORT_NAME];				// ��ʾ����
	TCHAR				szName[X_SHORT_NAME];				// ��Ʒ����
	TCHAR				szDesc[X_HUGE_NAME];				// ��Ʒ����
	TCHAR				szDescEx[X_LONG_NAME];				// ��Ʒ��������
	TCHAR				szMaleIcon[X_SHORT_NAME];			// ��Ʒͼ���а�
	TCHAR				szFemaleIcon[X_SHORT_NAME];			// ��Ʒͼ��Ů��
	TCHAR				szActionMod[X_SHORT_NAME];			// ��Ʒʹ�ö������
	TCHAR				szEffectMod[X_SHORT_NAME];			// ��Ʒʹ����Ч���
	TCHAR				szSourceEffect[X_SHORT_NAME];		// ��Ʒʹ�÷�������Ч���
	TCHAR				szDestEffect[X_SHORT_NAME];			// ��Ʒʹ��Ŀ����Ч���
	TCHAR				szPrepareMod[X_SHORT_NAME];			// ��Ʒʹ�����ֶ������
	TCHAR				szMaleRightMod[X_SHORT_NAME];		// ��ģ�ͱ��1
	TCHAR				szFemaleRightMod[X_SHORT_NAME];		// Ůģ�ͱ��1
	TCHAR				szMaleLeftMod[X_SHORT_NAME];		// ��ģ�ͱ��2
	TCHAR				szFemaleLeftMod[X_SHORT_NAME];		// Ůģ�ͱ��2
	TCHAR				szDropMod[X_SHORT_NAME];			// ����ģ�ͱ��
	TCHAR				szPertainMod[X_SHORT_NAME];			// ����ģ�ͱ��
	TCHAR				szBladeFlareSFX[X_SHORT_NAME];		// ������Ч���
	TCHAR				szBladeGlowSFX[X_SHORT_NAME];		// ������Ч���
	TCHAR				szAttachSFX[X_SHORT_NAME];			// ���������Ч���
	EWeaponPos			ePertainModPos;						// ����ģ�͹���λ��
	EEquipMdlType       eModelType;							// ģ������
	EEquipAttachPos     eAttachModelPos[MAX_EQUIP_ATTACH_NUM];  // ���ر���ģ�͵�λ��
	BOOL				bActionBlend;						// �Ƿ�������ֿ�
	
	tagItemDisplayInfo(){ZeroMemory(this, sizeof(*this));}
};

struct tagItemProto
{
	DWORD				dwTypeID;		// ������Ʒ������(ͬ����Ʒ��ͬ)

	EItemType			eType;			// ��������
	EItemTypeEx			eTypeEx;		// ��չ����
	EItemTypeReserved	eTypeReserved;	// ��������

	EStuffType			eStuffType;		// �������ͣ��ϳ���

 	DWORD				dwQuestID;		// ������
	INT32				nBasePrice;		// �����۸�

	INT32				nMaxUseTimes;	// ���ʹ�ô���
	INT16				n16MaxLapNum;	// ����������
	INT16				n16MaxHoldNum;	// ���ӵ������

	DWORD				dwTimeLimit;	// ��ʱ����Ʒ(��)

	INT16				n16Enmity;		// ���

	BYTE				byMinUseLevel;	// �ȼ�����
	BYTE				byMaxUseLevel;	// �ȼ�����
	ESexLimit			eSexLimit;		// �Ա�����
	DWORD				dwVocationLimit;// ְҵ����

	EReputationType		eClanRepute;	// ������������ֵ
	INT32				nClanReputeVal;	// ��������ֵ
	EReputationType		eOtherClanRepute;	// ������������ֵ
	INT32				nOtherClanReputeVal;// ��������ֵ

	EItemSpecFunc		eSpecFunc;		// ��Ʒ���⹦������
	INT32				nSpecFuncVal1;
	INT32				nSpecFuncVal2;

	ESkillOPType		eOPType;							// ��������
	FLOAT				fOPDist;							// ���þ��루���絥λ��
	FLOAT				fOPRadius;							// ���÷�Χ�����絥λ��
	INT32				nPrepareTime;						// ����ʱ�䣨���룩
	DWORD				dwCooldownTime;						// ������ȴʱ��(��λ��ms)
	INT32				nInterruptSkillOrdRate;				// ��ͨ���ܴ�ϼ���[0,10000]
	DWORD				dwSpecBuffLimitID;					// ����BUFF����ID
	DWORD				dwStateLimit;						// ����״̬��־λ����
	DWORD				dwTargetLimit;						// ��������
	BOOL				bMoveable;							// �Ƿ�����ƶ�ʹ��			
	bool				bInterruptMove;						// �Ƿ��ƶ����

	BYTE				byLevel;			// ��Ʒ�ȼ�
	BYTE				byQuality;			// ��ƷƷ��

	BYTE				byBindType;			// ������

	DWORD				dwTriggerID0;		// ��������
	DWORD				dwBuffID0;			// Buff
	
	DWORD				dwTriggerID1;		// ������ID1
	DWORD				dwBuffID1;			// ״̬ID1

	DWORD				dwTransTriggerID;	// ���δ�������ID
	DWORD				dwTransID;			// ���κ�ID

	bool				bNeedBroadcast;		// �Ƿ�㲥(0�����㲥��1���㲥)
	bool				bCanSell;			// �Ƿ񲻿ɳ���(0���ɳ��ۣ�1�����ɳ���)

	bool				bNeedLog;			// �Ƿ���Ҫ��log(����������Ӫͨ�����ݱ�item_needlog�༭)
	BYTE				byLogMinQlty;		// ��¼log��Ʒ�����Ʒ��

	bool				bDeadLoot;			// ��������

	tagItemDisplayInfo*	pDisplayInfo;		// ��Ʒ��ʾ�ṹ���ͻ�����
	
	tagItemProto(){ZeroMemory(this,sizeof(*this));}
	tagItemProto(DWORD dwID){ZeroMemory(this,sizeof(*this)); dwTypeID = dwID;}
};

struct tagEquipProto: public tagItemProto
{
	DWORD				dwSuitID[MAX_PEREQUIP_SUIT_NUM];		// ��װ���
	BYTE				bySuitMinQlty[MAX_PEREQUIP_SUIT_NUM];	// ���Ʒ��Ҫ��
	bool				bCanDye;								// ʱװ�Ƿ��Ⱦɫ

	INT16				n16AttALimit[X_ERA_ATTA_NUM];			// ��������
	
	DWORD				dwVocationLimitWear;					// ����ְҵ����

	EEquipPos			eEquipPos;		// װ��λ��
	EWeaponPos			eWeaponPos;		// ����λ��

	INT16				n16WuHun;		// �ڹ��˺�������
	INT16				n16MinDmg;		// ������С�˺�
	INT16				n16MaxDmg;		// ��������˺�
	INT16				n16Armor;		// ���߻���

	INT32				nMaxPotVal;		// װ�����Ǳ��ֵ
	INT32				nPotVal;		// װ��Ǳ��ֵ
	// Jason 2009-12-1 װ��Ǳ��ֵ��������
	INT32				nPotIncTimes;	// Ǳ��ֵ����ʣ�������ע�����ֵ�ǵݼ��ġ�

	tagRoleAttEffect	BaseEffect[MAX_ROLEATT_BASE_EFFECT];// �ӹ�ǰ��Ӱ�������(��Ʒ���޹�)
	BYTE				byLongfuAtt[X_WUXING_TYPE_NUM];		// ��������,������

	//BYTE				byClassLimit[X_ClASS_TYPE_NUM];		// ְҵ����(ָ��ְҵ����)//??

	DWORD				dwTriggerID2;		// ������ID1
	DWORD				dwBuffID2;			// ״̬ID1
	
	DWORD				dwColor;			// ��ɫ

	INT16				n16Newness;			// ո�¶� -- ������

	bool				bIdentifyProduct;	// ��������״̬
	bool				bIdentifyLoot;		// �������״̬

	tagEquipProto(){ZeroMemory(this,sizeof(*this));}
	tagEquipProto(DWORD dwID){ZeroMemory(this, sizeof(*this)); dwTypeID = dwID;}
};

#define MEquipMaxUseTimes(pEquipProto)	((pEquipProto)->n16Newness * ABRASION2USETIMES)

struct tagGemProto		// ǿ����Ʒԭ��
{
	DWORD				dwTypeID;		// ��Ʒԭ�ͱ��

	INT16				n16PotValNeed;	// ��Ҫ���ĵ�װ��Ǳ��ֵ
	INT16				n16SuccessPct;	// �ɹ���

	EWuXing				eWuXing;		// ���꣬����������(-1��ʾ����Ʒ��������)
	tagRoleAttEffect	BaseEffect[MAX_ROLEATT_ENHANCE_EFFECT];	// װ��ǿ��Ӱ��

	INT32				nEnhanceFlg;	// ��λ��¼ -- EEnhanceEquipPos

	tagGemProto() { ZeroMemory(this, sizeof(*this)); }
};

struct tagItem
{
	INT64			n64Serial;		// ��ƷΨһ���к�(��ͬ�ѵ���Ʒ��ͬ)
	DWORD			dwTypeID;		// ������Ʒ������(ͬ����Ʒ��ͬ)

	INT16			n16Num;			// ��Ʒ����
	bool			bLock	: 1;	// ������־
	bool			bUsing	: 1;	// �Ƿ��ڶԸ���Ʒ������������
	BYTE			byBind;			// ��״̬
	INT32			nUseTimes;		// ��Ʒ�ϼ���ʹ�ô���

	DWORD			dwAccountID;	// ����˺�ID, �ٱ����ͽ�ɫ�ֿ���(ͬһ�ʺ��½�ɫ����)
	DWORD			dwOwnerID;		// ӵ����, GT_INVALID��ʾû��ӵ����

	EItemConType	eConType;		// ��Ʒ������������
	INT16			n16Index;		// ��Ʒ�������е�λ��
	INT16			n16Dummy;		// �ڴ������

	EItemCreateMode	eCreateMode;	// ��Ʒ&װ�����ɷ�ʽ
	DWORD			dwCreateID;		// �ⲿ���ɸ���Ʒ��ID���磺GMID��QuestID��, �����ϲ���ʾ
	
	DWORD			dwCreatorID;	// ��Ʒ�����ߣ�GT_INVALID��ʾϵͳ����, ��������ʾ
	tagDWORDTime	dwCreateTime;	// ������Ʒ��ʱ��

	DWORD			dwNameID;		// �䱦����ID

	tagDWORDTime	dw1stGainTime;	// ��һ����һ��ʱ��

	EUpdateDBStatus	eStatus;		// ��¼��Ʒ�Ƿ���Ҫ�������ݿ����

	union
	{
		const tagItemProto	*pProtoType;	// ָ����Ʒ��������
		const tagEquipProto	*pEquipProto;	// ִ��װ����������
	};

	const ItemScript		*pScript;		// �ű�ָ��
	
	tagItem() { ZeroMemory(this, sizeof(*this)); }
	VOID SetOwner(DWORD dwNewOwnerID, DWORD dwNewAccountID) { dwOwnerID = dwNewOwnerID; dwAccountID = dwNewAccountID; }
	VOID SetPos(EItemConType eNewCon, INT16 n16NewIndex) { eConType = eNewCon; n16Index = n16NewIndex; }
	VOID SetIndex(INT16 n16NewIndex) { n16Index = n16NewIndex; }
	VOID SetUsing(bool bUsingState) { bUsing = bUsingState; }
	VOID SetUpdate(EUpdateDBStatus eNewUpdateStatus) { eStatus = eNewUpdateStatus; }
	INT64 GetKey() const { return n64Serial; }
	INT16 GetNum() const { return n16Num; }
	BOOL CanOverlap() const { return (P_VALID(pProtoType) ? pProtoType->n16MaxLapNum > 1 : FALSE); }
	VOID IncUseTimes(INT32 nUse) { nUseTimes += nUse; SetUpdate(EUDBS_Update); }

	// �õ�ĳ��������Ӧ��BuffID
	DWORD GetBuffID(INT nIndex)
	{
		switch(nIndex)
		{
		case 0:
			return pProtoType->dwBuffID0;
		case 1:
			return pProtoType->dwBuffID1;
		case 2:
			return pProtoType->dwTransID;
		default:
			return GT_INVALID;
		}
	}

	// �õ�ĳ��������Ӧ��TriggerID
	DWORD GetTriggerID(INT nIndex)
	{
		switch(nIndex)
		{
		case 0:
			return pProtoType->dwTriggerID0;
		case 1:
			return pProtoType->dwTriggerID1;
		case 2:
			return pProtoType->dwTransTriggerID;
		default:
			return GT_INVALID;
		}
	}

	VOID Bind()
	{
		ASSERT(P_VALID(pProtoType));
		switch(pProtoType->byBindType)
		{
		case EBM_Null:
			byBind = EBS_Unbind;
			break;
		case EBM_Gain:
			byBind = EBS_Bind;
			break;
		}
	}

	BOOL IsBind() const { return EBS_Bind == byBind; }

	BOOL IsLevelLimit(BYTE byRoleLevel) const
	{
		return pProtoType->byMinUseLevel > byRoleLevel || pProtoType->byMaxUseLevel < byRoleLevel;
	}

	BOOL IsTimeLimit() const { return pProtoType->dwTimeLimit != GT_INVALID; }

};

struct tagEquipSpec
{
	/******װ������ʱȷ��******/
	INT16				n16WuHun;		// �ڹ��˺�������
	INT16				n16MinDmg;		// ������С�˺�
	INT16				n16MaxDmg;		// ��������˺�
	INT16				n16Armor;		// ���߻���

	INT32				nPotVal;		// װ��Ǳ��ֵ(ԭǱ��ֵ+��������ֵ)
	INT32				nPotValUsed;	// �Ѿ����ĵ�Ǳ��ֵ
	// Jason 2009-12-1 װ��Ǳ��ֵ��������
	INT32				nPotIncTimes;	// Ǳ��ֵ����ʣ�������ע�����ֵ�ǵݼ��ġ�
	// Jason 2009-12-2 ������ɫװ����������
	DWORD				dwPurpleQltyIdfPct;

	// һ������
	INT32				nRoleAttEffect[X_ERA_ATTA_NUM];

	// ��,����������(GT_INVALID��ʾû��)
	DWORD				dwLongHunInnerID;
	DWORD				dwLongHunOuterID;

	// ��������
	BYTE				bySpecAtt;

	// ʹ�õȼ�����(�����ⲿ��������)
	BYTE				byMinUseLevel;
	BYTE				byMaxUseLevel;

	// Ʒ��
	BYTE				byQuality;

	/******ʱװ����ʱȷ��******/
	INT16				n16Appearance;	// ����
	BYTE				byRein;			// ͳ��
	BYTE				bySavvy;		// ����
	BYTE				byFortune;		// ��Ե
	INT8				n8ColorID;		// ��ɫ
	INT8				n8Dummy[2];

	/******װ��ǿ��ʱ�޸�******/
	bool				bCanCut;		// �Ƿ�ɿ���
	BYTE				byFlareVal;		// ����

	// ԭʼһ������(�����Զ��ӵ���ֶ��ӵ㲿��)����
	INT16				n16AttALimMod;
	INT16				n16AttALimModPct;	/*10000Ϊ����1*/

	// ���������ģ��Կ̣���Ƕ����ӡ(�˺������������)
	BYTE				byLongfuLevel;
	BYTE				byPosyTimes;
	BYTE				byEngraveTimes;
	BYTE				byHoleNum;
	BYTE				byBrandLevel;

	BYTE				byLongfuAtt[X_WUXING_TYPE_NUM];
	tagRoleAttEffect	PosyEffect[MAX_ROLEATT_POSY_EFFECT];
	INT32				nEngraveAtt[MAX_ROLEATT_ENGRAVE_EFFECT];
	DWORD				dwHoleGemID[MAX_EQUIPHOLE_NUM];
	INT32				nBrandAtt[X_ERA_DERATE_NUM];

	/******����װ�� -- Ʒ������������******/
	INT16				n16QltyModPct;		// һ��������(-10000,10000)
	INT16				n16QltyModPctEx;	// ����������[0,10000)

	/******����װ�� -- Ǳ��ֵӰ��ϵ��******/
	INT16				n16PotValModPct;

	/******����******/
	tagEquipSpec() { ZeroMemory(this, sizeof(*this)); }
};

struct tagEquip: public tagItem
{
	tagEquipSpec	equipSpec;

	VOID SetFlareVal(BYTE byFlareVal)
	{
		equipSpec.byFlareVal = byFlareVal;
	}

	VOID ChangeFlareVal(BYTE byFlareVal)
	{
		equipSpec.byFlareVal += byFlareVal;
		equipSpec.byFlareVal = (equipSpec.byFlareVal > 100) ? 100 : equipSpec.byFlareVal;
		equipSpec.byFlareVal = (equipSpec.byFlareVal < 0 ) ? 0 : equipSpec.byFlareVal;
	}

	VOID ChangePotVal(INT nValue)
	{
		INT nOrgPotVal = equipSpec.nPotVal;
		equipSpec.nPotVal += nValue;

		if(equipSpec.nPotVal < 0)		equipSpec.nPotVal = 0;

		if(equipSpec.nPotVal > pEquipProto->nMaxPotVal)
			equipSpec.nPotVal = pEquipProto->nMaxPotVal;

		if(nValue < 0)
			equipSpec.nPotValUsed += (nOrgPotVal - equipSpec.nPotVal);
	}

	VOID IncAttackTimes()
	{		
		++nUseTimes;

		if(eStatus != EUDBS_Update)
		{
			SetUpdate(EUDBS_Update);
		}
	}

	VOID DecAttackTimes(INT nTimes)
	{
		nUseTimes = min(MEquipMaxUseTimes(pEquipProto), nUseTimes);
		nUseTimes = max(0, nUseTimes - nTimes);

		if(eStatus != EUDBS_Update)
		{
			SetUpdate(EUDBS_Update);
		}
	}

	BOOL IsNewessChange(INT &nCurAttackTimes)
	{
		nCurAttackTimes = MEquipMaxUseTimes(pEquipProto);
		if(nUseTimes > nCurAttackTimes)
		{
			nUseTimes = nCurAttackTimes;
			return FALSE;
		}

		nCurAttackTimes = nUseTimes;
		if(nUseTimes % ABRASION2USETIMES == 0)
		{
			return TRUE;
		}

		return FALSE;
	}

	VOID GetTriggerIDBuffID(OUT DWORD &dwTriggerID, OUT DWORD &dwBuffID, IN INT nIndex)
	{
		dwTriggerID = dwBuffID = GT_INVALID;

		if(!P_VALID(pEquipProto))
		{
			ASSERT(0);
			return;
		}

		switch(nIndex)
		{
		case 0:
			dwTriggerID = pEquipProto->dwTriggerID0;
			dwBuffID	= pEquipProto->dwBuffID0;
			break;
		case 1:
			dwTriggerID = pEquipProto->dwTriggerID1;
			dwBuffID	= pEquipProto->dwBuffID1;
			break;
		case 2:
			dwTriggerID = pEquipProto->dwTriggerID2;
			dwBuffID	= pEquipProto->dwBuffID2;
			break;
		default:
			break;
		}
	}

	VOID GetLongHunID(DWORD &dwLongHunID, BOOL &bOuterLongHun) const
	{
		if(equipSpec.dwLongHunOuterID != GT_INVALID)
		{
			bOuterLongHun	= TRUE;
			dwLongHunID		= equipSpec.dwLongHunOuterID;
			return;
		}

		if(equipSpec.dwLongHunInnerID != GT_INVALID)
		{
			bOuterLongHun	= FALSE;
			dwLongHunID		= equipSpec.dwLongHunInnerID;
			return;
		}
	}

	BYTE GetLongfuAtt(INT nWuXingType) const
	{
		return equipSpec.byLongfuAtt[nWuXingType] + pEquipProto->byLongfuAtt[nWuXingType];
	}
};

// ������Ʒ&װ��(������tagItem��ʹ������������)
static VOID Destroy(tagItem *&pItem)
{
	if(!P_VALID(pItem))
	{
		return;
	}
	
	if(MIsEquipment(pItem->dwTypeID))
	{
		delete ((tagEquip*)pItem);
	}
	else
	{
		delete pItem;
	}

	pItem = NULL;
}

// ������Ʒ&װ��(������tagItem��ʹ������������)
static VOID Destroy(tagEquip *&pEquip)
{
	if(!P_VALID(pEquip))
	{
		return;
	}

	SAFE_DEL(pEquip);
}

//-----------------------------------------------------------------------------
// �������� -- �ͽṹ���
//-----------------------------------------------------------------------------
const INT SIZE_ITEM						= sizeof(tagItem);
const INT SIZE_EQUIP					= sizeof(tagEquip);
const INT SIZE_EQUIPSPEC				= sizeof(tagEquipSpec);


//-----------------------------------------------------------------------------
#pragma pack(pop)
