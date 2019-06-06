#pragma once

#include "ItemDefine.h"

#pragma pack(push, 1)

#define MAX_CONSOLIDATE_POS_QUANTITY 11
#define MAX_CONSOLIDATE_STUFF_QUANTITY 6
#define MAX_PRODUCE_STUFF_QUANTITY 6
#define MAX_CONSOLIDATE_ROLEATT 3
#define MAX_OUTPUT_QUANTITY	6

//��������
enum EProduceType
{
	EPCT_NULL	= 0,
	EPCT_Artisan,			//����1
	EPCT_DanTraining,		//����2
	EPCT_Smith,				//����3
	EPCT_Casting,			//����4
	EPCT_Dressmaker,		//�÷�5
	EPCT_Aechnics,			//����6
	EPCT_Smilt,				//ұ��7
	EPCT_PointUp,			//�㻯8
	EPCT_GodArtisan,		//�콳9	add xtian 2009-4-13
	EPCT_Pet,				//����10 add xtian 2009-8-21
	EPCT_EquipMaster,		// װ����ʦ
};
//�ϳ�����(�������͵�����)
enum EComposeType
{
	ECOMT_NULL = 0,
	ECOMT_StuffRefine = 10,			//(ʳ��,��)����	10
	ECOMT_StuffProcess,				//(ʳ��,��)�ӹ�	11
	ECOMT_StuffDecompose1,			//����			13(add xtian 2009-10-13)
	ECOMT_RefineDan = 20,			//����			20
	ECOMT_Cooking,					//���			21
	ECOMT_WeaponMake = 30,			//��������		30
	ECOMT_MillstoneMake,			//����Ʒ			31
	ECIMT_WeaponAlbum,				// ����ͼ��		32
	ECOMT_LoricaeMake = 40,			//��������		40
	ECOMT_RollMake,					//����������		41
	ECOMT_ArmorAlbum,				// ����ͼ��		42
	ECOMT_ClothMake = 50,			//��������		50
	ECOMT_FashionMake,				//ʱװ����		51
	ECOMT_ClothArmorAlbum,			// ����ͼ��		52
	ECOMT_DecorateMake = 60,		//��Ʒ����		60
	ECOMT_ChiselMake,				//�������		61(add xtian 12-3)
	ECOMT_GemProcess,				//��ʯ�ӹ�		62(add xtian 2009-2-12)
	ECOMT_DecorationAlbum,			// ��Ʒͼ��		63
	ECOMT_Refine = 70,				//(��ʯ,��ҩ)���� 70
	ECOMT_Process,					//(��ʯ,��ҩ)�ӹ� 71
	ECOMT_StuffDecompose2,			//����2			72(add xtian 2009-10-13)
	ECOMT_EquipdDecompose = 80,		//װ���ֽ�		80
	ECOMT_PointUp,					//�㻯			81
	ECOMT_Rebuild = 90,				//ӡ�Ǹ���		90	add xtian 2009-4-13
	ECOMT_LoongSoulCom,				//����ϳ�		91
	ECOMT_LoongSoulReb,				//�������		92
	ECOMT_GemCompose,				//��ʯ�ϳ�		93
	ECOMT_GemReBuild,				//��ʯ����		94
	ECOMT_GemCarve,					//��ʯ����		95
	ECOMT_PetMake = 100,			//���Ǻϳ�		100 add xtian 2009-8-21
	ECOMT_WeaponProduce = 110,		// ��������		110
	ECOMT_ArmorProdeuce = 111,		// ��������		111
	ECOMT_ClothArmorProduce = 112,	// ��������		112
	ECOMT_DecorationProduce = 113,	// ��Ʒ����		113

};	

//ÿ�����������ĺϳ���������
#define ArtisanSubTypeQuan		ECOMT_StuffDecompose1 - ECOMT_StuffRefine + 1
#define DanTrainingSubTypeQuan	ECOMT_Cooking -  ECOMT_RefineDan + 1
#define SmithSubTypeQuan		ECIMT_WeaponAlbum - ECOMT_WeaponMake + 1
#define CastingSubTypeQuan		ECOMT_ArmorAlbum - ECOMT_LoricaeMake + 1
#define DressmakerSubTypeQuan	ECOMT_ClothArmorAlbum - ECOMT_ClothMake + 1
#define AechnicsSubTypeQuan		ECOMT_DecorationAlbum - ECOMT_DecorateMake + 1
#define SmiltSubTypeQuan		ECOMT_StuffDecompose2 - ECOMT_Refine + 1
#define PointUpSubTypeQuan		ECOMT_PointUp - ECOMT_EquipdDecompose + 1
#define GodArtisanSubTypeQuan	ECOMT_GemCarve - ECOMT_Rebuild + 1
#define PetSubTypeQuan			ECOMT_PetMake - ECOMT_PetMake + 1
#define EquipSubTypeQuan		ECOMT_DecorationProduce - ECOMT_WeaponProduce + 1

//�ϳ��䷽����
enum EFormulaType
{
	EFORMT_NULL	= 0,
	EFORMT_OreRefine,		//��ʯ�ᴿ	1
	EFORMT_HerbalRefine,	//��ҩ�ᴿ	2
	EFORMT_ClothRefine,		//���Ͼ���	3
	EFORMT_FoodRefine,		//ʳ�ľ���	4
	EFORMT_OreProcess,		//��ʯ�ӹ�	5
	EFORMT_HerbalProcess,	//��ҩ�ӹ�	6
	EFORMT_ClothProcess,	//���ϼӹ�	7
	EFORMT_FoodProcess,		//ʳ�ļӹ�	8
	EFORMT_ThewDrug,		//����ҩ		9
	EFORMT_EnergyDrug,		//����ҩ		10
	EFORMT_RestoreDrug,		//ȫ��ҩ		11
	EFORMT_GodDrug,			//��ҩ		12
	EFORMT_SecondReDrug,	//���ҩ		13
	EFORMT_InterinjuryDrug,	//����ҩ		14
	EFORMT_NormalDrug,		//��ͨҩ		15
	EFORMT_Wine,			//��			16
	EFORMT_Cate,			//��ʳ		17
	EFORMT_Glave,			//��			18
	EFORMT_Sword,			//��			19
	EFORMT_Halberd,			//����		20
	EFORMT_Bow,				//��			21
	EFORMT_SmallOrgan,		//С�ͷ���	22
	EFORMT_BigOrgan,		//���ͷ���	23
	EFORMT_Instrument,		//��			24
	EFORMT_Millstone,		//ħʯ		25
	EFORMT_Armet,			//ͷ��		26
	EFORMT_UpperLoricae,	//�������	27
	EFORMT_LowerLoricae,	//�������	28
	EFORMT_Armguard,		//����		29
	EFORMT_BattleShoe,		//սѥ		30
	EFORMT_Roll,			//������		31
	EFORMT_Cap,				//ñ��		32
	EFORMT_Jupe,			//����		33	
	EFORMT_Pants,			//����		34
	EFORMT_Glove,			//����		35
	EFORMT_Shoe,			//Ь��		36
	EFORMT_Mantle,			//����		37
	EFORMT_FashionCap,		//ʱװñ��	38
	EFORMT_FashionJupe,		//ʱװ����	39
	EFORMT_FashionPants,	//ʱװ����	40
	EFORMT_FashionGloave,	//ʱװ����	41
	EFORMT_FashionShoe,		//ʱװЬ��	42
	EFORMT_FashionMantle,	//ʱװ����	43
	EFORMT_FingerRing,		//��ָ		44
	EFORMT_Caestus,			//����		45
	EFORMT_Necklace,		//����		46
	EFORMT_Chisel,			//���		47
	EFORMT_YuanRockIncise,	//ԭʯ�и�	48
	EFORMT_GeBlade,			//�		49
	EFORMT_GangRebuild,		//�ֶ�ӡ���� 50 add xtian 2009-4-13
	EFORMT_YuRebuild,		//��ĥӡ����
	EFORMT_YinRebuild,		//����ӡ����
	EFORMT_JinRebuild,		//����ӡ����
	EFORMT_JinLoongSoul,	//������
	EFORMT_MuLoongSoul,		//ľ����
	EFORMT_ShuLoongSoul,	//ˮ����
	EFORMT_HuoLoongSoul,	//������
	EFORMT_TuLoongSoul,		//������
	EFORMT_JinLoongSoulReb,	//���������
	EFORMT_MuLoongSoulReb,	//ľ�������
	EFORMT_ShuLoongSoulReb,	//ˮ�������
	EFORMT_HuoLoongSoulReb,	//���������
	EFORMT_TuLoongSoulReb,	//���������
	EFORMT_2Gem,			//������ʯ
	EFORMT_3Gem,			//������ʯ
	EFORMT_4Gem,			//�ļ���ʯ
	EFORMT_2GemRebuild,		//������ʯ����
	EFORMT_3GemRebuild,		//������ʯ����
	EFORMT_4GemRebuild,		//�ļ���ʯ����
	EFORMT_RefineDanOther,	//��������
	EFORMT_WeaponMakeOther, //������������
	EFORMT_LoricaeMakeOther,//������������
	EFORMT_ClothMakeOther,	//������������
	EFORMT_DecorateMakeOther,//��Ʒ��������
	EFORMT_ShangSoul,			//��Ʒ����
	EFORMT_LingSoul,			//��Ʒ����
	EFORMT_XianSoul,			//��Ʒ����
	EFORMT_ShengSoul,			//ʥƷ����
	EFORMT_MakeJade,			//����
	EFORMT_PetFood,				//���޿���
	EFORMT_GemBlade,			//��ʯ��
	EFORMT_GemCarve,			//��ʯ����
	EFORMT_ClothDecompose,		//���Ͻ���
	EFORMT_FoodDecompose,		//ʳ�Ľ���
	EFORMT_OreDecompose,		//��ʯ����
	EFORMT_HerbalDecompose,		//��ҩ����
	EFORMT_Album,				// ͼ��
	EFORMT_5Gem,			// �弶��ʯ
	EFORMT_6Gem,			// ������ʯ
	EFORMT_7Gem,			// �߼���ʯ
	EFORMT_5GemRebuild,		// �弶��ʯ����
	EFORMT_6GemRebuild,		// ������ʯ����
	EFORMT_7GemRebuild,		// �߼���ʯ����
};

//�䷽��Դ
enum EFormulaFrom
{
	EFormulaFrom_NULL = 0,
	EFormulaFrom_Skill,					//����
	EFormulaFrom_NPC,					//NPC
	EFormulaFrom_Item,					//��Ʒ
	EFormulaFrom_SkillorNPC,			//���ܼ�NPC
	EFormulaFrom_SkillorItem,			//���ܼ���Ʒ
	EFormulaFrom_NPCorItem,				//NPC����Ʒ
};

//װ��ǿ������
enum EConsolidateType
{
	ECliDateT_NULL 		= 0,		//��
	ECliDateT_Posy		= 1,		//����
	ECliDateT_Engrave	= 2,		//�Կ�
	ECliDateT_Quench,				//���
};

//ǿ����������
enum EPosyAtt
{
	EPosyAtt_NULL		= 0,	//��	
	EPosyAtt_MaxHP		= 1,    //�������
	EPosyAtt_MaxMP		= 2,	//�������
	EPosyAtt_ExAttack	= 3,	//�⹦����
	EPosyAtt_InAttack	= 4,	//�ڹ�����
	EPosyAtt_ExDefense	= 5,	//�⹦����
	EPosyAtt_InDefense	= 6,	//�ڹ�����

	EPosyAtt_End,
};
//ǿ���Կ�����
enum EEngraveAtt
{
	EEngraveAtt_NULL		= 0,	//��
	EEngraveAtt_WeaponDmg	= 1,	//�����˺�
	EEngraveAtt_WeaponSoul  = 2,	//���
	EEngraveAtt_Armor		= 3,	//���߻���(��)
	EEngraveAtt_Deration	= 4,	//���߻���(��)
	EEngraveAtt_End,
};

//�������
enum EQuenchType
{
	EEQuenchType_Metal		= 0,	//���
	EEQuenchType_Wood,				//ľ��
	EEQuenchType_Water,				//ˮ��
	EEQuenchType_Fire,				//���
	EEQuenchType_Soil,				//����
};


//��ǿ��װ����λ
struct tagConsolidatePos
{
	EEquipPos ePos;
	BYTE	  byConsolidate;
};

//ǿ������
struct tagConsolidateStuff
{
	DWORD				dwStuffID;			//����ID
	EStuffType			eStuffType;			//��������
	DWORD				dwStuffNum;			//��������
};

//�����ϳɲ���
struct tagProduceSutff
{
	DWORD				dwStuffID;			//����ID
	EStuffType			eStuffType;			//��������
	DWORD				dwStuffNum;			//��������	
};

//���ƽṹ��
struct tagPosyProto
{

	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EPosyAtt			ePosyAtt;				//��������
	BYTE				byPosyTimes;			//���ƴ���
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwPotValConsume;		//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	FLOAT 				fcoefficientA; 			//���Լ�ֵ��ʽ����A
	FLOAT 				fcoefficientB; 			//���Լ�ֵ��ʽ����B
	FLOAT 				fcoefficientC; 			//���Լ�ֵ��ʽ����C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY];	//��ǿ��װ����λ
};

//�Կ̽ṹ��
struct tagEngraveProto
{

	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EEngraveAtt			eEngraveAtt;			//�Կ�����
	BYTE				byEngraveTimes;			//�Կ̴���
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwPotValConsume;		//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	FLOAT 				fcoefficientA; 			//���Լ�ֵ��ʽ����A
	FLOAT 				fcoefficientB; 			//���Լ�ֵ��ʽ����B
	FLOAT 				fcoefficientC; 			//���Լ�ֵ��ʽ����C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY];	//��ǿ��װ����λ
};

//�������
struct tagQuenchAtt
{
	EWuXing				eWuXing;
	INT					nWuXingValue;
};

//���ṹ��
struct tagQuenchProto
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EQuenchType			eQuenchType;			//�������
	tagQuenchAtt		srcQuenchAtt;			//ԭ���Լ�����ֵ
	tagQuenchAtt		dstQuenchAtt;			//Ŀ�����Լ�����ֵ
	DWORD				dwPotValConsume;		//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
};

//�����ϳ�
struct tagProduceProto
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EProduceType		eProType;				//��������
	EComposeType		eComType;				//�ϳ�����
	EFormulaType		eFormType;				//�䷽����
	EFormulaFrom		eFormFrom;				//�䷽��Դ
	DWORD				dwFormLvl;				//�䷽�ȼ�
	DWORD				dwProItemTypeID;		//�ϳɵ���Ʒ
	DWORD				dwProQuantity;			//�ϳɵ�����
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwVitalityConsume;		//��������
	DWORD				dwMasterIncre;			//����������
	DWORD				dwMoneyConsume;			//��Ǯ����
	tagProduceSutff		ProduceStuff[MAX_PRODUCE_STUFF_QUANTITY];//��������
};


//ǿ������ǿ��������
struct tagConsolidateRoleAtt
{
	ERoleAttribute		eRoleAtt;
	INT					nAttVal;
};

//��Ƕ,��ӡ,����(ǿ������)
struct tagConsolidateItem
{
	DWORD					dwID;						//���ߵ�ID
	DWORD					dwPotValConsume;			//Ǳ��ֵ����
	INT						nSuccessRate;				//�ɹ���
	EWuXing					eWuXing;					//��������
	tagConsolidateRoleAtt	tagRoleAtt[MAX_CONSOLIDATE_ROLEATT];		//ǿ������	
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY];	//��ǿ��װ����λ
};

//  �ֽ�㻯�����Ʒ
struct tagOutputStuff
{
	DWORD				dwStuffTypeID;			//�������TypeID
	INT					nPerMinVal;				//������С����
	INT					nPerMaxVal;				//�����������
	INT					nSucMinVal;				//�ɹ���С����
	INT					nSucMaxVal;				//�ɹ��������
	INT					nRate;					//����
};

// �ֽ�㻯
struct tagDeComposeProto
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EComposeType		eComType;				//�ϳ�����
	BYTE				byLevel;				//װ���ȼ�
	BYTE				byQuality;				//װ��Ʒ��
	EItemType			eType;					//װ������
	EItemTypeEx			eTypeEx;				//��������
	EEquipPos			ePos;					//װ��λ��
	DWORD				dwFormLvl;				//�䷽�ȼ�
	EFormulaFrom		eFormFrom;				//�䷽��Դ
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwVitalityConsume;		//��������
	DWORD				dwMasterIncre;			//����������
	DWORD				dwMoneyConsume;			//��Ǯ����
	tagOutputStuff		OutputStuff[MAX_OUTPUT_QUANTITY];			//�������		
};

#pragma pack(pop)
