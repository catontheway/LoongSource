//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: skill_buff.h
// author: Aslan
// actor:
// data: 2008-9-23
// last:
// brief: ����״̬
//-------------------------------------------------------------------------------
#pragma once

class Unit;
struct tagBuffProto;

#include "att_res.h"

//-------------------------------------------------------------------------------
// ���ܶ�״̬��Ӱ��
//-------------------------------------------------------------------------------
struct tagBuffMod
{
	BOOL				bValid;											// �Ƿ���Ч
	INT					nPersistTickMod;								// ����ʱ��
	INT					nWarpTimesMod;									// ��������

	INT					nAttackInterruptRateMod;						// ������������
	EBuffEffectMode		eModBuffEffectMode;								// Ӱ������Ľ׶�BuffЧ��
	INT					nEffectMisc1Mod;								// Ч������1�ӳ�
	INT					nEffectMisc2Mod;								// Ч������2�ӳ�
	INT					nEffectAttMod[EBEA_End];						// ĳ���׶�Buff���������Ըı�Ӱ��

	mutable TMap<ERoleAttribute, INT>	mapRoleAttMod;					// ���ܶԸ�buff���������Ӱ��ļӳ�
	mutable TMap<ERoleAttribute, INT>	mapRoleAttModPct;				// ���ܶԸ�buff���������Ӱ��ļӳ�

	mutable TList<DWORD>				listModifier;					// ����Ӱ���buff�ļ����б�

	tagBuffMod()
	{
		bValid = FALSE;
		nPersistTickMod = 0;
		nWarpTimesMod = 0;

		nAttackInterruptRateMod = 0;
		eModBuffEffectMode = EBEM_Null;
		nEffectMisc1Mod = 0;
		nEffectMisc2Mod = 0;

		ZeroMemory(nEffectAttMod, sizeof(nEffectAttMod));
	}

	VOID Clear()
	{
		bValid = FALSE;
		nPersistTickMod = 0;
		nWarpTimesMod = 0;

		nAttackInterruptRateMod = 0;
		eModBuffEffectMode = EBEM_Null;
		nEffectMisc1Mod = 0;
		nEffectMisc2Mod = 0;

		ZeroMemory(nEffectAttMod, sizeof(nEffectAttMod));

		mapRoleAttMod.Clear();
		mapRoleAttModPct.Clear();
		listModifier.Clear();
	}

	BOOL IsValid() const { return bValid; }

	VOID SetMod(const tagSkillProto* pProto);
	VOID UnSetMod(const tagSkillProto* pProto);
};
