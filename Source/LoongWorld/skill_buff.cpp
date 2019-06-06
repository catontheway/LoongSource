//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: skill_buff.h
// author: Aslan
// actor:
// data: 2008-9-23
// last:
// brief: ����
//-------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/buff_define.h"

#include "att_res.h"
#include "skill_buff.h"
#include "world.h"


//------------------------------------------------------------------------------------
// ���üӳ�
//------------------------------------------------------------------------------------
VOID tagBuffMod::SetMod(const tagSkillProto* pProto)
{
	if( FALSE == P_VALID(pProto) ) return;
	if( ESTT_Buff != pProto->eTargetType ) return;

	// ����ʱ��
	nPersistTickMod += pProto->nBuffPersistTimeAdd / TICK_TIME;

	// ���Ӵ���
	nWarpTimesMod += pProto->nBuffWarpTimesAdd;

	// ������������
	nAttackInterruptRateMod += pProto->nBuffInterruptResistAdd;

	// Ч���ӳ�
	if( EBEM_Null != pProto->eModBuffMode )
	{
		// ֮ǰû�й�����Ч���ӳ�
		if( EBEM_Null == eModBuffEffectMode )
		{
			eModBuffEffectMode = pProto->eModBuffMode;
			nEffectMisc1Mod = pProto->nBuffMisc1Add;
			nEffectMisc2Mod = pProto->nBuffMisc2Add;

			if( EBEM_Persist != pProto->eModBuffMode )
			{
				IFASTCODE->MemCpy(nEffectAttMod, pProto->nBuffEffectAttMod, sizeof(nEffectAttMod));
			}
		}
		// ֮ǰ������Ч���ӳɣ����¼��ܵ�����Ч���ӳɵĽ׶α������һ��
		else if( eModBuffEffectMode == pProto->eModBuffMode )
		{
			nEffectMisc1Mod += pProto->nBuffMisc1Add;
			nEffectMisc2Mod += pProto->nBuffMisc2Add;

			if( EBEM_Persist != pProto->eModBuffMode )
			{
				for(INT n = 0; n < EBEA_End; ++n)
				{
					nEffectAttMod[n] += pProto->nBuffEffectAttMod[n];
				}
			}
		}
		// �¼��ܵ�����Ч���ӳ���֮ǰ�ļӳɲ�һ������������������
		else
		{
			ILOG->Write(_T("skill mod buff failed, skill type id is %u\r\n"), pProto->dwID);
		}
	}

	// �������Լӳ�
	ERoleAttribute eAtt = ERA_Null;
	INT nMod = 0;

	TMap<ERoleAttribute, INT>& mapMod = pProto->mapRoleAttMod;
	TMap<ERoleAttribute, INT>::TMapIterator itMod = mapMod.Begin();
	while( mapMod.PeekNext(itMod, eAtt, nMod) )
	{
		mapRoleAttMod.ModifyValue(eAtt, nMod);
	}

	TMap<ERoleAttribute, INT>& mapModPct = pProto->mapRoleAttModPct;
	TMap<ERoleAttribute, INT>::TMapIterator itModPct = mapModPct.Begin();
	while( mapModPct.PeekNext(itModPct, eAtt, nMod) )
	{
		mapRoleAttModPct.ModifyValue(eAtt, nMod);
	}

	// ���ü���TypeID���뱾��list���Ա㱣��
	listModifier.PushBack(pProto->dwID);

	// ���ø�Mod������Ч
	bValid = TRUE;
}

//------------------------------------------------------------------------------------
// ȡ���ӳ�
//------------------------------------------------------------------------------------
VOID tagBuffMod::UnSetMod(const tagSkillProto* pProto)
{
	if( FALSE == P_VALID(pProto) ) return;
	if( ESTT_Buff != pProto->eTargetType ) return;

	// ����ʱ��
	nPersistTickMod -= pProto->nBuffPersistTimeAdd / TICK_TIME;

	// ���Ӵ���
	nWarpTimesMod -= pProto->nBuffWarpTimesAdd;

	// ������������
	nAttackInterruptRateMod -= pProto->nBuffInterruptResistAdd;

	// Ч���ӳ�
	if( EBEM_Null != pProto->eModBuffMode )
	{
		// ������Ч���ӳɣ����ܵ�����Ч���ӳɵĽ׶α������һ��
		if( eModBuffEffectMode == pProto->eModBuffMode )
		{
			nEffectMisc1Mod -= pProto->nBuffMisc1Add;
			nEffectMisc2Mod -= pProto->nBuffMisc2Add;

			if( EBEM_Persist != pProto->eModBuffMode )
			{
				for(INT n = 0; n < EBEA_End; ++n)
				{
					nEffectAttMod[n] -= pProto->nBuffEffectAttMod[n];
				}
			}
		}
	}

	// �������Լӳ�
	ERoleAttribute eAtt = ERA_Null;
	INT nMod = 0;

	TMap<ERoleAttribute, INT>& mapMod = pProto->mapRoleAttMod;
	TMap<ERoleAttribute, INT>::TMapIterator itMod = mapMod.Begin();
	while( mapMod.PeekNext(itMod, eAtt, nMod) )
	{
		mapRoleAttMod.ModifyValue(eAtt, -nMod);
	}

	TMap<ERoleAttribute, INT>& mapModPct = pProto->mapRoleAttModPct;
	TMap<ERoleAttribute, INT>::TMapIterator itModPct = mapModPct.Begin();
	while( mapModPct.PeekNext(itModPct, eAtt, nMod) )
	{
		mapRoleAttModPct.ModifyValue(eAtt, -nMod);
	}

	listModifier.Erase(const_cast<tagSkillProto*>(pProto)->dwID);
}
