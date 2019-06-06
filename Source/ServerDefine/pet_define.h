//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_define.h
// author: 
// actor:
// data: 2009-05-07
// last:
// brief: �������ó�����ؽṹ
//-----------------------------------------------------------------------------
#pragma once
#include "../WorldDefine/pet_define.h"
#include "../WorldDefine/ItemDefine.h"
#pragma pack(push, 1)

const INT MAX_PETSOUL_NUM		= 5;	// ���ӵ�г�������
const INT MAX_CALLED_PET_NUM	= 1;	// ����ٻ���������

const INT SPIRIT_RESUME_COUNT	= 2 * TICK_PER_SECOND * 60;// �����ظ�����


//! �������ó�������
// enum EPetAtt
// {
// 	//��������
// 	// 	EPA_CurExp
// 	// 	EPA_Level
// 	// 	EPA_MaxSkillNum
// 
// 	EPA_Null				= -1,
// 	EPA_Begin				= 0,
// 
// 	// 	һ������
// 	// 	Ӱ��������Ե�����
// 
// 	EPA_1_Begin				= 0,
// 
// 	EPA_Speed				= 0,
// 	EPA_Endurance			= 1,
// 	EPA_Spirit				= 2,
// 
// 	EPA_1_End				= 3,
// 
// 	// 	��������
// 	// 	��һ������Ӱ�������
// 	EPA_2_Begin				= 3,
// 
// 	EPA_MountSpeedAdd		= 3,
// 	EPA_SavetimeAdd			= 4,
// 	EPA_GatherNumAdd		= 5,
// 	EPA_FindTreasurePct		= 6,
// 	EPA_Energy				= 7,	// SaveDB
// 	EPA_MaxEnergy			= 8,
// 	EPA_RoleAttAddPct		= 9,
// 
// 	EPA_2_End				= 10,
// 
// 	// 	��������
// 	// 	ֻ���Լӳɵ�����
// 	EPA_ExpTimes			= 10,
// 	EPA_IQ					= 11,	// SaveDB
// 	EPA_Quality				= 12,	// SaveDB
// 	EPA_BagGrid				= 13,
// 	EPA_Hungry				= 14,	// SaveDB
// 	EPA_RemainPoints		= 15,	// SaveDB
// 	EPA_MountSpeed			= 16,
// 
// 	EPA_End					,
// };

// ϴ�㣺		����EPA_Quality��������
// Ʒ��Ӱ�죺	EPA_Aptitude��EPA_Potential��EPA_SpiritMax
// �ȼ�Ӱ�죺	EPA_MountSpeed��EPA_SpiritMax��EPA_TalentCount



enum EPetAtt
{
	EPA_Null				=-1,
	EPA_Begin				= 0,

	EPA_Quality				= 0,	//Ʒ��	1~5Ʒ���������޵����ʡ��츳�����������츳����	�츳���ܹ������֣����ڶ���������棬�ٽ���������
	EPA_Aptitude			= 1,	//����	�������޵������ɳ���������ʼǱ��	ȡֵ1~100������ȡ����Ʒ��
	EPA_Spirit				= 2,	//����	���ŵȼ��ĳɳ����ɳ���ʹ�ü���ʱ��Ҫ���ĸ�ֵ����Ϣʱ���Զ��ָ�	�ֵ�ǰ����ֵ���������ֵ��
	EPA_TalentCount			= 3,	//�츳����	
	EPA_Potential			= 4,	//Ǳ��	ѧϰ����ʱ���ĸ�ֵ������װ��Ǳ��ֵ	��ʼǱ��=���ʡ�10
	EPA_WuXingEnergy		= 5,	//������	������ʹ�ü���ʱ�����Դ����ֵ����ʩչ�츳����ʱ���ĸ�ֵ	���ܶ�Ӧ������ֵ�ڳ��＼�ܱ�����д
	EPA_MountNum			= 6,	//�������
	EPA_AddOn_Begin			= 7,

	EPA_SpiritMax			= 7,	//�������ֵ=500�����ʡ����ס�10���ȣ�
	EPA_TalentCountMax		= 8,	//�츳�������ֵ	���ڵ�����츳����ʹ�ô���������ÿ�����ã��������ﵽ���ֵʱ������ʹ���츳����	Ʒ�ʰ�=1��Ʒ�ʻ�=2��Ʒ����=3��Ʒ����=4��Ʒ�ʳ�=5 �츳����=�ף�Ʒ�ʡ�3
	EPA_BagGrid				= 9,	//���Ҹ���	ʹ����ؼ���ʱ��������������Ʒ������	Ĭ��4��װ���ӳ�2~16
	EPA_DeliveryConsume		=10,	//��ݺ�ʱ	ʹ�ÿ�ݼ���ʱ�����޹�����ʱ������	Ĭ��0��װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱������5�룬��-5000��
	EPA_SellConsume			=11,	//������ʱ	ʹ�÷�������ʱ�����޹�����ʱ������	Ĭ��0��װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱
	EPA_StorageConsume		=12,	//��ź�ʱ	ʹ�ô�ż���ʱ�����޹�����ʱ������	Ĭ��0��װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱
	EPA_GatherConsume		=13,	//�ɼ���ʱ	ʹ�òɼ�����ʱ�����޹���ʱ������	Ĭ��0��װ���ӳ�-��~0��ms����Ӧ���ܱ�����ʱ�䡱
	EPA_TrainResume			=14,	//ѵ���ָ�	���޵��Զ�ѵ�����ܵĻָ�ʱ������	Ĭ��0��װ���ӳ�-��~0��ms����Ӧ���ܱ��ָ�ʱ�䡱
	EPA_SpiritRate			=15,	//��������	(0-1 0-10000)ʹ�ü���ʱ�������������ű�	Ĭ��100%��װ���ӳ�-100%~0%����Ӧ���ܱ�energy_cost
	EPA_ExpRate				=16,	//����ӳ�	(0-1 0-10000)��þ���ʱ�����ű�	Ĭ��100%��װ���ӳ�0%~400%
	EPA_PickUpResume		=17,	//ʰȡ�ָ�	��������ʰȡ�Ļָ�ʱ��	Ĭ��0��װ���ӳ�-20~-1
	EPA_MedicineSaving		=18,	//��ҩ��ʡ	(0-1 0-10000)����ÿ��ιҩʱ���ܲ����ĸ�ҩƷ	Ĭ��0%��װ���ӳ�1%~5%
	EPA_StrengthEffect		=19,	//ǿ��Ч��	(0-1 0-10000)����ǿ���༼�ܵ�Ч��	Ĭ��100%��װ���ӳ�0%~400%
	EPA_WuXingConsume		=20,	//����������	(0-1 0-10000)��ʹ���츳����ʱ�����͸ü��ܶ�������������	Ĭ��100%��װ���ӳ�-50%~0%����Ӧ���ܱ����������ġ�
	EPA_MountSpeed			=21,	//����ٶ�	(0-1 0-10000)���ޱ�����ʱ���ƶ��ٶ�	Ĭ�ϰ�30%����40%����50%����60%����70% װ���ӳ�0%~100%	�ܽ�Ӱ��
	EPA_MountNumMax			=22,	//�������
	
	EPA_AddOn_End			=23,

	EPA_End					=23,
};

const INT EPA_NUM		= EPA_End - EPA_Begin;


#define IS_EPA(n)		( (n) >= EPA_Begin && (n) < EPA_End )
#define IS_EPA_ADDON(n)	( (n) >= EPA_AddOn_Begin && (n) < EPA_AddOn_End )

#define EPATOEPAAO(n)	( INT((n) - EPA_AddOn_Begin) )

// ��������Ĭ��ֵ
struct tagPetDefMinMax
{
	INT		nDef;
	INT		nMin;
	INT		nMax;
};

//-------------------------------------------------------------------------------------------------------
// ������괴������
//-------------------------------------------------------------------------------------------------------
struct tagDBPetSoulCreate
{
	TCHAR	tszName[X_SHORT_NAME];	// ��������
	INT		nQuality;				// Ʒ��
	INT		nAptitude;				// ����
	INT		nPotential;				// ��ǰǱ�� 
	INT		nSpirte;				// ��ǰ����
//	BOOL	bMountable;				// �ܷ����

	DWORD	dwProtoID;				// ԭ��ID
	DWORD	dwMasterID;				// ����ID
	tagDBPetSoulCreate()	{ZeroMemory(this, sizeof(*this));}
};

//-------------------------------------------------------------------------------------------------------
// �����������
//-------------------------------------------------------------------------------------------------------
struct tagDBPetAtt
{
	DWORD	dwPetID;				// ����ID
	TCHAR	tszName[X_SHORT_NAME];	// ��������
	DWORD	dwProtoID;				// ԭ��ID
	DWORD	dwMasterID;				// ����ID

	INT		nQuality;				// Ʒ��
	INT		nAptitude;				// ����
	INT		nPotential;				// ��ǰǱ�� 
	INT		nSpirit;				// ��ǰ����
	INT		nWuXingEnergy;			// ������
	INT		nTalentCount;			// �츳����
	BYTE	byPetState;				// ����״̬

	INT		nExpCur;				// ��ǰ����
	INT		nStep;					// ��
	INT		nGrade;					// ��
	
	BOOL	bLocked;				// �Ƿ�����

	tagDBPetAtt()	{ZeroMemory(this, sizeof(*this));}
};

//-------------------------------------------------------------------------------------------------------
// ���＼������
//-------------------------------------------------------------------------------------------------------
struct tagDBPetSkill
{
	DWORD	dwTypeID;				// ԭ��id
	INT		nPara1;					// ��ȴ��ʱ
	INT		nPara2;					// ʹ�ü�ʱ
	tagDBPetSkill()	{ZeroMemory(this, sizeof(*this));}
};

//-------------------------------------------------------------------------------------------------------
// �����������
//-------------------------------------------------------------------------------------------------------
struct tagDBPet
{
	tagDBPetAtt		petAtt;			// ��������
	INT				nPetSkillNum;	// ���＼��
	INT				nPetEquipNum;	// ����װ��
	BYTE			byData[1];		// tagDBPetSkill and tagItem
	tagDBPet()	{ZeroMemory(this, sizeof(*this));}
	INT	GetSize()
	{
		if (0 == nPetSkillNum && 0 == nPetEquipNum)
			return sizeof(*this);
		else
			return sizeof(*this) - 1 + sizeof(tagDBPetSkill) * nPetSkillNum + sizeof(tagItem) * nPetEquipNum;
	}
};


#pragma pack(pop)