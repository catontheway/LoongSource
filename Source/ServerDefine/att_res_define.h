//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: att_res_define.h
// author: Sxg
// actor:
// data: 2009-02-19
// last:
// brief: ��Դ���������ݽṹ����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/longhun_define.h"
//-----------------------------------------------------------------------------
// Ʒ��Ӱ���װ��������ز���
//-----------------------------------------------------------------------------
struct tagEquipQltyEffect
{
	// ��������Ӱ��ϵ��
	FLOAT fWeaponFactor;
	FLOAT fArmorFactor;

	// һ������
	INT32 nAttAFactor;
	FLOAT fAttAFactor;
	INT32 nAttANumEffect;

	// Ǳ��ֵ
	FLOAT fPotFactor;

	// ��Ƕ������ -- ��¼���ֵļ���
	INT32 nHoleNumPct[MAX_EQUIPHOLE_NUM + 1];

	// �������� -- ����
	INT32 nLonghunPct[X_LONGHUN_TYPE_NUM];

	// ��������
	INT32 nSpecAttPct;
};

//-----------------------------------------------------------------------------
// ��������֮���Ӱ��ṹ
//-----------------------------------------------------------------------------
struct tagSkillModify
{
	TList<DWORD>	listModify;		// ֱ��Ӱ��������ܵĴӼ���
};

//-----------------------------------------------------------------------------
// ʱװ���ɹ�����ض���
//-----------------------------------------------------------------------------
struct tagFashionGen
{
	FLOAT		fAppearanceFactor;	// ��������(AppearancePct)
	INT16		n16ReinPct;			// ͳ�����Լӳ�(ReinPct)
	INT16		n16SavvyPct;		// �������Լӳ�(SavvyPct)
	INT16		n16FortunePct;		// ��Ե���Լӳ�(FortunePct)
	INT8		n8ReinVal;			// ֵ=��Ʒ�ȼ���ReinVal[ȡ��](ReinVal)
	INT8		n8SavvyVal;			// ֵ=��Ʒ�ȼ���SavvyVal[ȡ��](SavvyVal)
	INT8		n8FortuneVal1;		// ֵ=FortuneVal1�����¸���20%��+װ��Ʒ��/FortuneVal2
	INT8		n8FortuneVal2;		// (FortuneVal1, FortuneVal2)
	INT8		n8Dummy[2];
};

struct tagFashionColorPct	// ʱװ����ʱ��ɫ����
{
	INT16	n16ColorPct[X_COLOR_NUM];	// ��ɫ����
};