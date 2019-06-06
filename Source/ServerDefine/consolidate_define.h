//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: consolidate_define.h
// author: 
// actor:
// data: 2008-06-04
// last:
// brief: ��������װ��ǿ����ؽṹ
//-----------------------------------------------------------------------------
#include "../ServerDefine/base_define.h"
#include "../WorldDefine/compose_define.h"

#pragma once

#pragma pack(push, 1)

// ǿ������
enum EConsolidateTypeSer
{
	ECTS_Posy		= 0,	// ����
	ECTS_Engrave	= 1,	// �Կ�
	ECTS_Inlay		= 2,	// ��Ƕ
	ECTS_Brand		= 3,	// ��ӡ
	ECTS_LoongSoul	= 4,	// ����
	ECTS_Quench		= 5,	// ���
	ECTS_Produce	= 6,	// ����
	ECTS_Decompose  = 8,	// �㻯�ֽ�
	ECTS_RaisePotVal = 9,	// ����Ǳ��ֵ Jason 2009-12-6
};

// ��Ҫ����װ�����ε�ǿ������
//enum EConsolidateFlare
//{
//	ECF_Posy = 0,			// ����
//	ECF_Engrave = 1,		// �Կ�
//	ECF_Inlay = 2,			// ��Ƕ
//	ECF_Brand = 3,			// ��ӡ
//	ECF_LoongSoul = 4,		// ����
//};

// ��������ת��
#define ConsolidatePosyAttConvert(eRoleState, ePosyState)\
		{												 \
		eRoleState = ERA_Null;							 \
		switch(ePosyState)							     \
			{										     \
		case EPosyAtt_MaxHP:							 \
			eRoleState = ERA_MaxHP;					     \
		break;										     \
		case EPosyAtt_MaxMP:							 \
			eRoleState = ERA_MaxMP;					     \
		break;										     \
		case EPosyAtt_ExAttack:							 \
			eRoleState = ERA_ExAttack;					 \
		break;										     \
		case EPosyAtt_ExDefense:						 \
			eRoleState = ERA_ExDefense;					 \
		break;										     \
		case EPosyAtt_InAttack:							 \
			eRoleState = ERA_InAttack;					 \
		break;										     \
		case EPosyAtt_InDefense:						 \
			eRoleState = ERA_InDefense;					 \
		break;										     \
			}											 \
		}

//���ƽṹ��
struct tagPosyProtoSer
{
	DWORD				dwID;					// �䷽ID
	TCHAR				szName[X_SHORT_NAME];	// �䷽����
	EPosyAtt			ePosyAtt;				// ��������
	BYTE				byPosyTimes;			// ���Ƶȼ�
	INT					nSuccessRate;			// �ɹ���
	INT					nPotValConsume;			// Ǳ��ֵ����
	DWORD				dwMoneyConsume;			// ��Ǯ����
	FLOAT 				fcoefficientA; 			// ���Լ�ֵ��ʽ����A
	FLOAT 				fcoefficientB; 			// ���Լ�ֵ��ʽ����B
	FLOAT 				fcoefficientC; 			// ���Լ�ֵ��ʽ����C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY];   // ǿ������
	INT					nTotleStuff;										// ���ϵ�������
	tagPosyProtoSer(){ZeroMemory(this,sizeof(*this));}
};


//�Կ̽ṹ��
struct tagEngraveProtoSer
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EEngraveAtt			eEngraveAtt;			//�Կ�����
	BYTE				byEngraveTimes;			//�Կ̴���
	INT					nSuccessRate;			//�ɹ���
	INT					nPotValConsume;			//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	FLOAT 				fcoefficientA; 			//���Լ�ֵ��ʽ����A
	FLOAT 				fcoefficientB; 			//���Լ�ֵ��ʽ����B
	FLOAT 				fcoefficientC; 			//���Լ�ֵ��ʽ����C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
	INT					nTotleStuff;										// ���ϵ�������
	tagEngraveProtoSer(){ZeroMemory(this,sizeof(*this));}
};


//�����ϳ�
struct tagProduceProtoSer
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EProduceType		eProType;				//��������
	EComposeType		eComType;				//�ϳ�����
	EFormulaFrom		eFormFrom;				//�䷽��Դ
	INT			  		nFormLvl;				//�䷽�ȼ�
	DWORD				dwProItemTypeID;		//�ϳɵ���Ʒ
	DWORD				dwProQuantity;			//�ϳɵ�����
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwVitalityConsume;		//��������
	DWORD				dwMasterIncre;			//����������
	DWORD				dwMoneyConsume;			//��Ǯ����
	tagProduceSutff		ProduceStuff[MAX_PRODUCE_STUFF_QUANTITY];//��������
};

struct tagDeComposeProtoSer
{
	DWORD				dwID;					//�䷽ID
	EComposeType		eComType;				//�ϳ�����
	BYTE				byLevel;				//װ���ȼ�
	BYTE				byQuality;				//װ��Ʒ��
	EItemType			eType;					//װ������
	EItemTypeEx			eTypeEx;				//��������
	EEquipPos			ePos;					//װ��λ��
	INT					nFormLvl;				//�䷽�ȼ�
	EFormulaFrom		eFormFrom;				//�䷽��Դ
	DWORD				dwVitalityConsume;		//��������
	DWORD				dwMasterIncre;			//����������
	DWORD				dwMoneyConsume;			//��Ǯ����
	tagOutputStuff		OutputStuff[MAX_OUTPUT_QUANTITY];			//�������	
};


//���ṹ��
struct tagQuenchProtoSer
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	tagQuenchAtt		srcQuenchAtt;			//ԭ���Լ�����ֵ
	tagQuenchAtt		dstQuenchAtt;			//Ŀ�����Լ�����ֵ
	DWORD				dwPotValConsume;		//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	INT					nSuccessRate;			//�ɹ���
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
};


#pragma pack(pop)

