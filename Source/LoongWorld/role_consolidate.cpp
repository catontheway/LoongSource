//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_consolidate.cpp
// author: Wjiang
// actor:
// data: 
// last:
// brief: װ��ǿ��
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/specfunc_define.h"
#include "../WorldDefine/compose_define.h"
#include "../ServerDefine/consolidate_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/item_define.h"

#include "role.h"
#include "map.h"
#include "creature.h"
#include "item_creator.h"
#include "title_mgr.h"
//-----------------------------------------------------------------------------
// ����
// Jason 2009-12-1 ������ɫƷ�ʲ��ϵļӳ�����
// Jason 2009-12-4 �޸ģ�ֻ��ָ�����͵Ĳ��ϣ��ż���Ӱ��ϵ��ͳ��
//-----------------------------------------------------------------------------
DWORD Role::PosyEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Posy) )
		return E_Consolidate_NPCCanNotPosy;

	// �ҵ�ǿ������
	const tagPosyProtoSer *pPosyProto = g_attRes.GetPosyProto(dwFormulaID);
	if(!P_VALID(pPosyProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ������ƴ���
	if(pEquip->equipSpec.byPosyTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondPosyTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byPosyTimes + 1 ) != pPosyProto->byPosyTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsPosyPos(pPosyProto->ePosyAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotPosy;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pPosyProto->nPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pPosyProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// �����Ҳ����Ƿ��㹻(˳������ܲ���������
	INT nItemQualityNum[EIQ_End];
	ZeroMemory(nItemQualityNum, sizeof(nItemQualityNum));

	INT nTotleStuff = 0;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pPosyProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pPosyProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pPosyProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pPosyProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->n16Num < (INT)pPosyProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;

		// Jason ������ָ��ĳ�����͵Ĳ��ϣ����򣬲�ͳ��
		if( EST_Null != pPosyProto->ConsolidateStuff[n].eStuffType )
		{
			nItemQualityNum[pItemStuff->pProtoType->byQuality] += pPosyProto->ConsolidateStuff[n].dwStuffNum;
			nTotleStuff += pPosyProto->ConsolidateStuff[n].dwStuffNum;
		}
	}

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagPosyProtoSer *pProto = const_cast<tagPosyProtoSer*>(pPosyProto);		
	CalIMEffect(ECTS_Posy, IMEffect, n64IMID, pProto);

	// �����������
	for(INT n = 0; n < nArraySz; ++n)
	{
		 GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Posy, (INT16)pPosyProto->ConsolidateStuff[n].dwStuffNum);
	}

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pPosyProto->dwMoneyConsume, ELCID_Compose_Posy);

	// ����ɹ���
	// ���ճɹ���=�����ɹ���+��-10%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+5%����Ʒ��������
	// /�ܲ�������+10%����Ʒ��������/�ܲ�������+20%����Ʒ��������/�ܲ�����������[1+��װ���ȼ�-75��/150]
	// +����ɫ��Ե/1000��+IM���߼ӳ�                                      ���ճɹ��ʵ�ȡֵΪ0%-100%
	FLOAT fProp = 0;
	if( nTotleStuff )
		fProp = (FLOAT)pPosyProto->nSuccessRate + (((-0.1f * (FLOAT)nItemQualityNum[EIQ_White] 
				+ 0.05f * (FLOAT)nItemQualityNum[EIQ_Green] + 0.1f * (FLOAT)nItemQualityNum[EIQ_Blue] 
				+ 0.2f * (FLOAT)nItemQualityNum[EIQ_Orange] + 0.5 * (FLOAT)nItemQualityNum[EIQ_Purple] ) / (FLOAT)nTotleStuff) 
				* (1.0f + ((FLOAT)pEquip->pProtoType->byLevel - 75.0f) / 150.0f) + ((FLOAT) GetAttValue(ERA_Fortune) / 1000.0f)) * 10000;

	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Posy);

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)			// �ɹ�
	{
		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);
		
		// װ�����ƴ�����һ
		pEquip->equipSpec.byPosyTimes++;

		// ����װ����Ӧǿ��ֵ
		// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�
		/* ����Ʒ�ʼӳ�=��-20%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+20%
		   ����Ʒ��������/�ܲ�������+50%����Ʒ��������/�ܲ�������+100%����Ʒ��������/�ܲ���������
		   ����Ʒ�ʼӳɵ�ȡֵΪ0%-100%  */

		// �����ӳ�
		FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pPosyProto->fcoefficientA 
							+ pPosyProto->fcoefficientB) / pPosyProto->fcoefficientC;

		// ����Ʒ�ʼӳ�
		FLOAT fStuffAttInc = 0;
		if( nTotleStuff )
			fStuffAttInc = (-0.3f * (FLOAT)nItemQualityNum[EIQ_White] + 0.5f * (FLOAT)nItemQualityNum[EIQ_Green] 
							  + (FLOAT)nItemQualityNum[EIQ_Blue] + 2.0f * (FLOAT)nItemQualityNum[EIQ_Orange] 
							  + 3.0 * (FLOAT)nItemQualityNum[EIQ_Purple] )
							  / (FLOAT)nTotleStuff; 

		//if(fStuffAttInc < 0.0f)
		//	fStuffAttInc = 0.0f;
		//if(fStuffAttInc > 1.0f)
		//	fStuffAttInc = 1.0f;

		// ����ת�� 
		ERoleAttribute eRoleAtt = ERA_Null;
		ConsolidatePosyAttConvert(eRoleAtt, pPosyProto->ePosyAtt);

		for(INT n = 0; n < MAX_ROLEATT_POSY_EFFECT; ++n)
		{
			if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == eRoleAtt)
			{
				pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + fStuffAttInc));
				break;
			}
			else if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == ERA_Null)
			{
				pEquip->equipSpec.PosyEffect[n].eRoleAtt = eRoleAtt;
				pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + fStuffAttInc));
				break;
			}
		}

		// Ǳ��ֵ����
		// ����������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000
		FLOAT fPefectProp = (fProp * 0.05f) / 10000.0f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ���Ƴɹ���75%
			pEquip->ChangePotVal(-pPosyProto->nPotValConsume * 75 / 100);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-pPosyProto->nPotValConsume);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else				// ʧ��
	{
		// װ��Ǳ��ֵ����5��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-20/*5*/); // Jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM����
//-----------------------------------------------------------------------------
DWORD Role::GMPosyEquip(DWORD dwFormulaID, INT16 n16ItemIndex)
{
	// �ҵ�ǿ������
	const tagPosyProtoSer *pPosyProto = g_attRes.GetPosyProto(dwFormulaID);
	if(!P_VALID(pPosyProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16ItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ������ƴ���
	if(pEquip->equipSpec.byPosyTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondPosyTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byPosyTimes + 1 ) != pPosyProto->byPosyTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsPosyPos(pPosyProto->ePosyAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotPosy;

	// װ�����ƴ�����һ
	pEquip->equipSpec.byPosyTimes++;

	// ����װ����Ӧǿ��ֵ
	// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�

	// �����ӳ�
	FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pPosyProto->fcoefficientA 
		+ pPosyProto->fcoefficientB) / pPosyProto->fcoefficientC;

	// ����ת�� 
	ERoleAttribute eRoleAtt = ERA_Null;
	ConsolidatePosyAttConvert(eRoleAtt, pPosyProto->ePosyAtt);

	for(INT n = 0; n < MAX_ROLEATT_POSY_EFFECT; ++n)
	{
		if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == eRoleAtt)
		{
			pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + 0));
			break;
		}
		else if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == ERA_Null)
		{
			pEquip->equipSpec.PosyEffect[n].eRoleAtt = eRoleAtt;
			pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + 0));
			break;
		}
	}

	// ����װ������
	CalEquipFlare(pEquip);
		
	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//-----------------------------------------------------------------------------
// �Կ�
// Jason 2009-12-1 ������ɫƷ������Ӱ��
// Jason 2009-12-4 ֻͳ�Ʋ���������ȷ�Ĳ���
//-----------------------------------------------------------------------------
DWORD Role::EngraveEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Engrave) )
		return E_Compose_NPCCanNotEngrave;

	// �ҵ�ǿ������
	const tagEngraveProtoSer *pEngraveProto = g_attRes.GetEngraveProto(dwFormulaID);
	if(!P_VALID(pEngraveProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ����Կ̴���
	if(pEquip->equipSpec.byEngraveTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondEngraveTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byEngraveTimes + 1 ) != pEngraveProto->byEngraveTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsEngravePos(pEngraveProto->eEngraveAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotEngrave;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pEngraveProto->nPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pEngraveProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// �����Ҳ����Ƿ��㹻(˳������ܲ���������
	INT nItemQualityNum[EIQ_End];
	ZeroMemory(nItemQualityNum, sizeof(nItemQualityNum));

	INT nTotleStuff = 0;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pEngraveProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pEngraveProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pEngraveProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pEngraveProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->n16Num < (INT)pEngraveProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;

		if( EST_Null != pEngraveProto->ConsolidateStuff[n].eStuffType )
		{
			nItemQualityNum[pItemStuff->pProtoType->byQuality] += pEngraveProto->ConsolidateStuff[n].dwStuffNum;
			nTotleStuff += pEngraveProto->ConsolidateStuff[n].dwStuffNum;
		}
	}

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagEngraveProtoSer  *pProto = const_cast<tagEngraveProtoSer*>(pEngraveProto);
	CalIMEffect(ECTS_Engrave, IMEffect, n64IMID, pProto);

	// �����������
	for(INT n = 0; n < nArraySz; ++n)
	{
		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Engrave, (INT16)pEngraveProto->ConsolidateStuff[n].dwStuffNum);
	}

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pEngraveProto->dwMoneyConsume, ELCID_Compose_Engrave);

	// ����ɹ���
	// ���ճɹ���=�����ɹ���+��-10%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+5%����Ʒ��������
	// /�ܲ�������+10%����Ʒ��������/�ܲ�������+20%����Ʒ��������/�ܲ�����������[1+��װ���ȼ�-75��/150]
	// +����ɫ��Ե/1000��+IM���߼ӳ�                                      ���ճɹ��ʵ�ȡֵΪ0%-100%
	FLOAT fProp = 0;
	if( nTotleStuff )
		fProp = (FLOAT)pEngraveProto->nSuccessRate + (((-0.1f * (FLOAT)nItemQualityNum[EIQ_White] 
				+ 0.05f * (FLOAT)nItemQualityNum[EIQ_Green] + 0.1f * (FLOAT)nItemQualityNum[EIQ_Blue] 
				+ 0.2f * (FLOAT)nItemQualityNum[EIQ_Orange] + 0.5 * (FLOAT)nItemQualityNum[EIQ_Purple] ) / (FLOAT)nTotleStuff) 
				* (1.0f + ((FLOAT)pEquip->pProtoType->byLevel - 75.0f) / 150.0f) 
				+ ((FLOAT) GetAttValue(ERA_Fortune) / 1000.0f)) * 10000;

	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Engrave);

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)			// �ɹ�
	{
		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

		// װ�����ƴ�����һ
		pEquip->equipSpec.byEngraveTimes++;

		// ����װ����Ӧǿ��ֵ
		// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�
		/* ����Ʒ�ʼӳ�=��-20%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+20%
		����Ʒ��������/�ܲ�������+50%����Ʒ��������/�ܲ�������+100%����Ʒ��������/�ܲ���������
		����Ʒ�ʼӳɵ�ȡֵΪ0%-50%  */

		// �����ӳ�
		FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pEngraveProto->fcoefficientA 
			+ pEngraveProto->fcoefficientB) / pEngraveProto->fcoefficientC;

		// ����Ʒ�ʼӳ�
		FLOAT fStuffAttInc = 0;
		if( nTotleStuff )
			fStuffAttInc = (-0.3f * (FLOAT)nItemQualityNum[EIQ_White] + 0.5f * (FLOAT)nItemQualityNum[EIQ_Green] 
							  + (FLOAT)nItemQualityNum[EIQ_Blue] + 2.0f * (FLOAT)nItemQualityNum[EIQ_Orange] 
							  + 3.0f * (FLOAT)nItemQualityNum[EIQ_Purple] )
							  / (FLOAT)nTotleStuff; 

		//if(fStuffAttInc < 0.0f)
		//	fStuffAttInc = 0.0f;
		//if(fStuffAttInc > 1.0f)
		//	fStuffAttInc = 1.0f;

		// ���ռӳ�
		INT nEngraveAttInc = (INT)(fBaseAttInc * (1.0f + fStuffAttInc));

		switch(pEngraveProto->eEngraveAtt)
		{
		case EEngraveAtt_WeaponDmg:
			pEquip->equipSpec.nEngraveAtt[0] += nEngraveAttInc;
			pEquip->equipSpec.nEngraveAtt[1] += nEngraveAttInc;
			break;
		case EEngraveAtt_WeaponSoul:
			pEquip->equipSpec.nEngraveAtt[2] += nEngraveAttInc;
			break;
		case EEngraveAtt_Armor:
			pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
			break;
		case EEngraveAtt_Deration:
			pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
			break;
		default:
			break;
		}

		// Ǳ��ֵ����
		// ����������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000
		FLOAT fPefectProp = (fProp * 0.05f) / 10000.0f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ���Ƴɹ���75%
			pEquip->ChangePotVal(-pEngraveProto->nPotValConsume * 75 / 100);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-pEngraveProto->nPotValConsume);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else				// ʧ��
	{
		// װ��Ǳ��ֵ����5��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-20/*5*/); //Jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM �Կ�
//-----------------------------------------------------------------------------
DWORD Role::GMEngraveEquip(DWORD dwFormulaID, INT16 n16ItemIndex)
{
	// �ҵ�ǿ������
	const tagEngraveProtoSer *pEngraveProto = g_attRes.GetEngraveProto(dwFormulaID);
	if(!P_VALID(pEngraveProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16ItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ����Կ̴���
	if(pEquip->equipSpec.byEngraveTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondEngraveTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byEngraveTimes + 1 ) != pEngraveProto->byEngraveTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsEngravePos(pEngraveProto->eEngraveAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotEngrave;

	// װ�����ƴ�����һ
	pEquip->equipSpec.byEngraveTimes++;

	// ����װ����Ӧǿ��ֵ
	// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�

	// �����ӳ�
	FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pEngraveProto->fcoefficientA 
		+ pEngraveProto->fcoefficientB) / pEngraveProto->fcoefficientC;

	// ���ռӳ�
	INT nEngraveAttInc = (INT)(fBaseAttInc * (1.0f + 0));

	switch(pEngraveProto->eEngraveAtt)
	{
	case EEngraveAtt_WeaponDmg:
		pEquip->equipSpec.nEngraveAtt[0] += nEngraveAttInc;
		pEquip->equipSpec.nEngraveAtt[1] += nEngraveAttInc;
		break;
	case EEngraveAtt_WeaponSoul:
		pEquip->equipSpec.nEngraveAtt[2] += nEngraveAttInc;
		break;
	case EEngraveAtt_Armor:
		pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
		break;
	case EEngraveAtt_Deration:
		pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
		break;
	default:
		break;
	}
	
	// ����װ������
	CalEquipFlare(pEquip);

	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//-----------------------------------------------------------------------------
// ��Ƕ
//-----------------------------------------------------------------------------
DWORD Role::InlayEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// �ҵ���ʯ
	tagItem *pItemGem = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemGem) || pItemGem->pProtoType->eSpecFunc != EISF_HoleGem )
	{
		return E_Consolidate_Gem_Not_Exit;
	}

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemGem->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	if((INT)pEquip->equipSpec.byHoleNum == 0)
		return E_Consolidate_Gem_Not_Hole;

	// ����Ƿ�ʯ�Ѿ�����
	if(pEquip->equipSpec.dwHoleGemID[pEquip->equipSpec.byHoleNum - 1] != GT_INVALID 
		&& pEquip->equipSpec.dwHoleGemID[pEquip->equipSpec.byHoleNum - 1] != 0 )
		return E_Consolidate_Gem_Full;

	// ��ⱦʯ�Ƿ�����Ƕ��װ����
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotInlay;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// ��Ƕ��ʯ
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
		{
			pEquip->equipSpec.dwHoleGemID[i] = pItemGem->dwTypeID;
			break;
		}
	}

	// ����װ������
	CalEquipFlare(pEquip);

	// ����װ��Ǳ��ֵ
	pEquip->ChangePotVal(-(INT)pConsolidateProto->dwPotValConsume);
	GetItemMgr().UpdateEquipSpec(*pEquip);

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_Enchase, 1);

	// �ƺ��¼�����
	GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

	return E_Compose_Consolidate_Success;	
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
DWORD Role::ChiselEquip(INT64 n64SrcItemID, INT64 n64SuffID, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if(!MIsIdentified(pEquip))
		return E_Consolidate_NotIdentified;

	if(pEquip->equipSpec.bCanCut == false)
		return E_Consolidate_Equip_CanNot_Chisel;

	// �ҵ���ʯ
	tagItem *pItemChisel = GetItemMgr().GetBagItem(n64SuffID);
	if(!P_VALID(pItemChisel) || pItemChisel->pProtoType->eSpecFunc != EISF_Chisel )
	{
		return E_Consolidate_Chisel_Not_Exit;
	}

	// ���װ����Ƕ����
	if(pEquip->equipSpec.byHoleNum >= MAX_EQUIPHOLE_NUM)
		return E_Consolidate_Equip_Hole_Full;

	// �ȼ�С���������װ���ȼ����򲻿ɽ��п���
	if(pEquip->pProtoType->byLevel > (pItemChisel->pProtoType->byLevel * 10))
		return E_Consolidate_Chisel_Not_Match;

	// ����ɹ���(�������ճɹ���=��ʯ�����ɹ��ʡ���1-װ����ǰ��Ƕ������/5��+����ɫ��Ե/1000��)
	FLOAT fProp = (((FLOAT)pItemChisel->pProtoType->nSpecFuncVal1 * ( 1.0f - (FLOAT)pEquip->equipSpec.byHoleNum / 5.0f)) / 10000.0f 
						+ (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f) *10000.0f;

	// ��������
	GetItemMgr().DelFromBag(n64SuffID, (DWORD)ELCID_Compose_Chisel, 1);

	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Cut);

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)		// �ɹ�
	{
		// ����������
		// ������=1%+����ɫ��Ե/1000����ȡֵ��0-100%��
		FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			pEquip->equipSpec.byHoleNum = ((pEquip->equipSpec.byHoleNum + 2 > MAX_EQUIPHOLE_NUM) ? MAX_EQUIPHOLE_NUM : (pEquip->equipSpec.byHoleNum + 2));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return  E_Compose_Consolidate_Perfect;
		}
		else
		{
			++(pEquip->equipSpec.byHoleNum);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}

	return E_Compose_Consolidate_Lose;
}

//-----------------------------------------------------------------------------
// ʱװȾɫ
//-----------------------------------------------------------------------------
DWORD Role::DyeFashion(INT64 n64DyeSN, INT64 n64EquipSerial, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���Ⱦɫʱװ
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64EquipSerial);
	if(!P_VALID(pEquip))
		return E_Dye_NotExist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Dye_OtherItem;

	if(!MIsFashion(pEquip))
		return E_Dye_OtherItem;

	if (pEquip->pEquipProto->bCanDye == false)
		return E_Dye_OtherItem;

	// �ҵ�Ⱦ��
	tagItem *pItemDye = GetItemMgr().GetBagItem(n64DyeSN);
	if(!P_VALID(pItemDye) || pItemDye->pProtoType->eSpecFunc != EISF_Dye )
	{
		return E_Dye_NotExist;
	}

	// �����ɫ�Ƿ���ͬ
	if(pEquip->equipSpec.n8ColorID == pItemDye->pProtoType->nSpecFuncVal1)
		return E_Dye_Exist;

	// �ȼ�С����Ⱦɫ��װ���ȼ����򲻿ɽ���Ⱦɫ
	if(pEquip->pProtoType->byLevel > (pItemDye->pProtoType->byLevel * 10))
		return E_Dye_LvLimit;

	// �ı�ʱװ��ɫ
	pEquip->equipSpec.n8ColorID = pItemDye->pProtoType->nSpecFuncVal1;
	GetItemMgr().UpdateEquipSpec(*pEquip);

	// ��������
	GetItemMgr().DelFromBag(n64DyeSN, (DWORD)ELCID_Compose_Dye, 1);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ����B�����Զ�ǿ���ɹ��ʵ�Ӱ��
//-----------------------------------------------------------------------------
FLOAT Role::CalSpecAttEffectSuc(EEquipSpecAtt eSpecAtt, FLOAT fProp, EEquipSpecAtt eSpecType)
{
	switch (eSpecAtt)
	{
	case EESA_Guard_Posy:
	case EESA_Guard_Brand:
	case EESA_Guard_Engrave:
	case EESA_Guard_LongFu:
	case EESA_Guard_Quench:
	case EESA_Guard_Cut:
		{
			if(eSpecAtt != eSpecType)
				return fProp;
			else
				return 10000.0f;
		}
	case EESA_Guard_AllProduce:
		return 10000.0f;
		break;
	case EESA_Guard_Talisman:
		return fProp * 1.05f;
		break;
	default:
		return fProp;
		break;
	}
}

// ����B�����Զ�ǿ�������ʵ�Ӱ��
FLOAT Role::CalSpecAttEffectPef(EEquipSpecAtt eSpecAtt, FLOAT fPerfect)
{
	switch(eSpecAtt)
	{
	case EESA_Guard_Fortune:
		return fPerfect * 1.05f;
		break;
	default:
		return fPerfect;
		break;
	}
}

//-----------------------------------------------------------------------------
// ��ӡ
//-----------------------------------------------------------------------------
DWORD Role::BrandEquip(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// �ҵ�ӡ��
	tagItem *pItemBrand = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemBrand) || pItemBrand->pProtoType->eSpecFunc != EISF_Brand )
		return E_Consolidate_Brand_Not_Exit;

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemBrand->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// ���ӡ�ǵȼ��Ƿ�ﵽ����
	if((pEquip->equipSpec.byBrandLevel + pItemBrand->pProtoType->byLevel) > MAX_BRAND_LEVEL)
		return E_Consolidate_Brand_TopLevel;

	// ���װ���Ƿ��ܱ�ӡ��
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotBrand;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// ��¼ӡ�ǵȼ�
	BYTE byLevel = pItemBrand->pProtoType->byLevel;

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_Brand, 1);

	// ����B�����ԶԳɹ���Ӱ��
	FLOAT fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), (FLOAT)(pConsolidateProto->nSuccessRate), EESA_Guard_Brand);

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagConsolidateItem *pProto = const_cast<tagConsolidateItem*>(pConsolidateProto);		
	CalIMEffect(ECTS_Brand, IMEffect, n64IMID, pProto);

	// ����ɹ���
	BOOL bResult = IUTIL->Rand() % 10000 <= fProp;

	if(bResult)		// �ɹ�
	{
		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

		// װ����ӡ�ȼ�(������Ϊӡ�ǵȼ�)
		pEquip->equipSpec.byBrandLevel += byLevel;

		// װ�����Ӷ�Ӧǿ������
		for(INT n = 0; n < MAX_CONSOLIDATE_ROLEATT; ++n)
		{
			pEquip->equipSpec.nBrandAtt[MTransERADerate2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt)] 
															+= pConsolidateProto->tagRoleAtt[n].nAttVal;
		}

		// �����Ƿ�����������������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000)
		FLOAT fPefectProp = (pConsolidateProto->nSuccessRate * 0.05f) / 10000.0f+ (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ��ӡ�ɹ���75%
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume * 75 / 100));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// װ��Ǳ��ֵ����10��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-40/*10*/);// jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
DWORD Role::LoongSoulEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// �ҵ�����ʯ
	tagItem *pItemLoongSoul = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemLoongSoul) || pItemLoongSoul->pProtoType->eSpecFunc != EISF_LongHunShi )
		return E_Consolidate_LoongSoul_Not_Exit;

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemLoongSoul->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// ���ӡ�ǵȼ��Ƿ�ﵽ����
	if((pEquip->equipSpec.byLongfuLevel + pItemLoongSoul->pProtoType->byLevel) > MAX_LONGHUN_LEVEL)
		return E_Consolidate_LoongSoul_TopLevel;

	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotLoongSoul;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// ��¼����ʯ�ȼ�
	BYTE byLevel = pItemLoongSoul->pProtoType->byLevel;

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_LongFu, 1);

	// ����ɹ���(�ɹ���=�����ɹ��ʡ�[1+��װ���ȼ�-75��/300]+����ɫ��Ե/1000))
	FLOAT fProp = pConsolidateProto->nSuccessRate * (1.0f + (FLOAT)(pEquip->pProtoType->byLevel - 75.0f) / 300.0f)
					+ ((FLOAT)GetAttValue(ERA_Fortune) / 1000.0f) * 10000;

	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_LongFu);

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)		// �ɹ�
	{
		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

		// װ������ȼ�(������Ϊ����ʯ�ȼ�)
		pEquip->equipSpec.byLongfuLevel += byLevel;

		// װ�����Ӷ�Ӧǿ������
		pEquip->equipSpec.byLongfuAtt[pConsolidateProto->eWuXing] += byLevel;
		
		// �����Ƿ�������������=1%+����ɫ��Ե/1000))
		FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp =  CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ����ɹ���75%
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume * 75 / 100));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// װ����Ӧ�������ȼ���0���������꣬������װ��Ǳ��ֵ��װ���������������
		pEquip->equipSpec.byLongfuLevel = 0;
		ZeroMemory(pEquip->equipSpec.byLongfuAtt, sizeof(pEquip->equipSpec.byLongfuAtt));
		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM����
//-----------------------------------------------------------------------------
DWORD Role::GMLoongSoulEquip(INT64 n64SrcItemID, INT16 n16DstItemIndex)
{
	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16DstItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// �ҵ�����ʯ
	tagItem *pItemLoongSoul = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemLoongSoul) || pItemLoongSoul->pProtoType->eSpecFunc != EISF_LongHunShi )
		return E_Consolidate_LoongSoul_Not_Exit;

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemLoongSoul->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// ���ӡ�ǵȼ��Ƿ�ﵽ����
	if((pEquip->equipSpec.byLongfuLevel + pItemLoongSoul->pProtoType->byLevel) > MAX_LONGHUN_LEVEL)
		return E_Consolidate_LoongSoul_TopLevel;

	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotLoongSoul;

	// ��¼����ʯ�ȼ�
	BYTE byLevel = pItemLoongSoul->pProtoType->byLevel;

	// װ������ȼ�(������Ϊ����ʯ�ȼ�)
	pEquip->equipSpec.byLongfuLevel += byLevel;

	// װ�����Ӷ�Ӧǿ������
	pEquip->equipSpec.byLongfuAtt[pConsolidateProto->eWuXing] += byLevel;

	// ����װ������
	CalEquipFlare(pEquip);

	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//---------------------------------------------------------------------------------
// ���
//---------------------------------------------------------------------------------
DWORD Role::QuenchEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, 
											INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Quench) )
		return E_Consolidate_NPCCanNotQuench;

	// �ҵ�ǿ������
	const tagQuenchProtoSer *pQuenchProto = g_attRes.GetQuenchProto(dwFormulaID);
	if(!P_VALID(pQuenchProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pQuenchProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pQuenchProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// ԭ���Ե�����ֵ�Ƿ��㹻
	if(pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] < pQuenchProto->srcQuenchAtt.nWuXingValue)
		return E_Consolidate_WXValueInadequate;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pQuenchProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pQuenchProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pQuenchProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pQuenchProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->n16Num < (INT)pQuenchProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;
	}

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagQuenchProtoSer *pProto = const_cast<tagQuenchProtoSer*>(pQuenchProto);
	CalIMEffect(ECTS_Quench, IMEffect, n64IMID, pProto);

	// �����������
	for(INT n = 0; n < nArraySz; ++n)
	{
		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Quench, (INT16)pQuenchProto->ConsolidateStuff[n].dwStuffNum);
	}

	// װ��Ǳ��ֵ����
	pEquip->ChangePotVal(-(INT)(pQuenchProto->dwPotValConsume));

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pQuenchProto->dwMoneyConsume, ELCID_Compose_Quench);

	// ����B�����ԶԳɹ���Ӱ��
	FLOAT fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), (FLOAT)pQuenchProto->nSuccessRate, EESA_Guard_Quench);

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= fProp;
	if(bResult)
	{
		// ����Ŀ������
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] -= pQuenchProto->srcQuenchAtt.nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->dstQuenchAtt.eWuXing] += pQuenchProto->dstQuenchAtt.nWuXingValue;
		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Success;
	}
	else		// ʧ��
	{
		// ���ӵ����������ڳ���ԭ���Ժ�Ŀ��������������������������
		EWuXing		eType = EWX_Null; 
		INT			nWuXingValue = pQuenchProto->dstQuenchAtt.nWuXingValue;
		BYTE		byTotalValue = 0;
		while (TRUE)
		{
			eType =(EWuXing)(IUTIL->Rand() % 5);
			if(pQuenchProto->srcQuenchAtt.eWuXing != eType && pQuenchProto->dstQuenchAtt.eWuXing != eType)
				break;
		}

		// �䷽���������Ŀ����������ֵΪn����ʧ��ʱʵ������ֵ��[n-2��n+1]�������
		// ȡֵ��ΧΪ[1��10]���Ҵ����������������ܺϲ��ܳ���10
		nWuXingValue = IUTIL->Rand() % 3 + (nWuXingValue - 2);

		for(INT i = 0; i < EWX_End; ++i)
		{
			byTotalValue += pEquip->equipSpec.byLongfuAtt[i];
		}

		nWuXingValue = (nWuXingValue > (10 - byTotalValue)) ? (10 - byTotalValue) : nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[eType] += nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] -= pQuenchProto->srcQuenchAtt.nWuXingValue;

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Consolidate_False;

	}
}
//---------------------------------------------------------------------------------
// �����ϳ���Ʒ
//---------------------------------------------------------------------------------
DWORD Role::ProduceItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID,
								INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	// �ҵ��ϳɱ�����
	const tagProduceProtoSer* pProduceProto = g_attRes.GetProduceProto(dwFormulaID);

	if(!P_VALID(pProduceProto))
		return E_Compose_Formula_Not_Exist;

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckProduceLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pProduceProto, n64StuffID, nArraySz);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagProduceProtoSer *pProto = const_cast<tagProduceProtoSer*>(pProduceProto);
	CalIMEffect(ECTS_Produce, IMEffect, n64IMID, pProto);

	// �����������
	for(INT n = 0; n < nArraySz; ++n)
	{
		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
	}

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);

	// ���Ӻϳɼ���������(ֻ�м��ܺϳɲŻ����Ӽ��������ȣ�
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pProduceProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// �۳���ҵĻ���ֵ
	ChangeVitality(-(INT)pProduceProto->dwVitalityConsume);

	// ����ɹ���
	FLOAT	fProp = (FLOAT)pProduceProto->nSuccessRate;

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	// �ƺ��¼�����
	if (bResult && GT_VALID(dwNPCID))
		GetTitleMgr()->SigEvent(ETE_COMPOSITE_SKILL_SUCCESS	, GT_INVALID, GT_INVALID);

	if(bResult)			// �ɹ�
	{
		// ��úϳɵ���
		if(pProduceProto->eComType == ECOMT_RefineDan)
		{
			// ����������
			FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;
			BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

			// ���������������Ʒ��������
			if(bPefect)
			{
				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
								pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity * 2), m_dwID);
				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}

				return E_Compose_Consolidate_Perfect;
			}
			else
			{
				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
								pProduceProto->dwProItemTypeID, (INT16)pProduceProto->dwProQuantity, m_dwID);
				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}

				return E_Compose_Consolidate_Success;
			}
		}
		else
		{
			tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
								pProduceProto->dwProItemTypeID, (INT16)pProduceProto->dwProQuantity, m_dwID);
			if( P_VALID(pItem) )
			{
				GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
			}
			
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		return E_Compose_Consolidate_Lose;
	}

}

//---------------------------------------------------------------------------------
// �����ϳ�װ��
// Jason 2009-12-4 ���������ͼ���ӳ�
//--------------------------------------------------------------------------------
DWORD Role::ProduceEquip(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
										INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	// �ҵ��ϳɱ�����
	const tagProduceProtoSer* pProduceProto = g_attRes.GetProduceProto(dwFormulaID);

	if(!P_VALID(pProduceProto))
		return E_Compose_Formula_Not_Exist;

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckProduceLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pProduceProto, n64StuffID, nArraySz);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	// �����������(˳��������Ʒ�ʣ�
	INT nStuffNum[EIQ_End];
	ZeroMemory(nStuffNum, sizeof(nStuffNum));
	INT nTotalNum = 0;

	for(INT n = 0; n < nArraySz; ++n)
	{	
		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if( EST_Null != pProduceProto->ProduceStuff[n].eStuffType ) // Jason��ֻͳ��������ȷ�Ĳ�������
		{
			nStuffNum[pItemStuff->pProtoType->byQuality] += pProduceProto->ProduceStuff[n].dwStuffNum;
			nTotalNum += pProduceProto->ProduceStuff[n].dwStuffNum;
		}

		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
	}

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);

	// ���Ӻϳɼ���������(ֻ�м��ܺϳɲŻ����Ӽ��������ȣ�
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pProduceProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// �۳���ҵĻ���ֵ
	ChangeVitality(-(INT)pProduceProto->dwVitalityConsume);

	// ��úϳɵ���
	/* 1��������=-50%����Ʒ��������/����������+10%����Ʒ��������/����������+15%����Ʒ��������
				 /����������+50%����Ʒ��������/����������+100%����Ʒ��������/����������

	   2��������=5%����Ʒ��������/����������+20%����Ʒ��������/����������+30%����Ʒ��������/����������						*/
	FLOAT fModify1 = 0;
	FLOAT fModify2 = 0;
	FLOAT fModify3 = 0;
	FLOAT fPurplePct = 0;
	INT nPotAdding = 0;

	if( nTotalNum )
	{
		fModify1 = ((-0.5f * (FLOAT)nStuffNum[EIQ_White] + 0.1f * (FLOAT)nStuffNum[EIQ_Yellow]
		+ 0.15f * (FLOAT)nStuffNum[EIQ_Green] + 0.5f * (FLOAT)nStuffNum[EIQ_Blue] 
		+ (FLOAT)nStuffNum[EIQ_Orange]
		+ (FLOAT)nStuffNum[EIQ_Purple]  ) / (FLOAT)nTotalNum) * 10000.0f;

		fModify2 = (( 0.05f * (FLOAT)nStuffNum[EIQ_Green] + 0.2f * (FLOAT)nStuffNum[EIQ_Blue] + 0.3f * (FLOAT)nStuffNum[EIQ_Orange] + 0.5 * nStuffNum[EIQ_Purple] ) / (FLOAT)nTotalNum) * 10000.0f;

		// Ǳ��ֵ������
		// Ǳ��ֵӰ��ϵ�� = 0.7����Ʒ��������/����������+0.8����Ʒ��������/����������+0.9����Ʒ��������/����������+1.0����Ʒ��������/����������+1.1����Ʒ��������/����������
		fModify3 = ((0.7f * (FLOAT)nStuffNum[EIQ_White] + 0.8f * (FLOAT)nStuffNum[EIQ_Yellow]
		+ 0.9f * (FLOAT)nStuffNum[EIQ_Green] + 1.0f * (FLOAT)nStuffNum[EIQ_Blue]
		+ 1.1f * (FLOAT)nStuffNum[EIQ_Orange] 
		+ 1.2  * nStuffNum[EIQ_Purple] ) / (FLOAT)nTotalNum) * 10000.0f;

		fPurplePct = ( 0.05 * (FLOAT)nStuffNum[EIQ_Blue] + 0.22 * (FLOAT)nStuffNum[EIQ_Orange] + nStuffNum[EIQ_Purple] ) / nTotalNum;

		// Jason 2009-12-1
		//����װ��������Ǳ��ֵ���� = 
		//��̬����ֵ + ��N��/N��*0+N��/N��*0+N��/N��*0+N��/N��*3+N��/N��*6+N��/N��*10��
		//��ֵ����ȡ��
		nPotAdding = ( nStuffNum[EIQ_Blue] * 3 + nStuffNum[EIQ_Orange] * 6 + nStuffNum[EIQ_Purple] * 10 ) / nTotalNum;
	}


	FLOAT nRnd = ( (FLOAT) IUTIL->RandomInRange(0,100) ) / 100;
	


	tagItem *pItem = NULL;
	if( nRnd > fPurplePct || fPurplePct == 0 )//�����ɫƷ����������Ϊ0��Ҳ�����ϵļ���������Jason 2009-12-23 fix
		pItem = ItemCreator::Create(EICM_Product,  m_dwID, 
			pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3,nPotAdding);
	else
		pItem = ItemCreator::CreateByProduce(EICM_Product,  m_dwID, 
		pProduceProto->dwProItemTypeID,EIQ_Purple, nPotAdding ,(DWORD)(fPurplePct * 100),(INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3);

	// �ж��Ƿ�Ϊʱװ���������
	if(P_VALID(pItem) && MIsFashion(pItem))
	{
		// ����IM���ߵ�Ӱ��
		tagIMEffect			IMEffect;
		tagProduceProtoSer *pProto = const_cast<tagProduceProtoSer*>(pProduceProto);
		CalIMEffect(ECTS_Produce, IMEffect, n64IMID, pProto);

		ItemCreator::IdentifyEquip((tagEquip*)pItem, EIQ_Null, &IMEffect);
	}
	
	if( P_VALID(pItem) )
	{
		GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
	}

	// �ƺ��¼�����
	if (GT_VALID(dwNPCID))
		GetTitleMgr()->SigEvent(ETE_COMPOSITE_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

	return E_Compose_Consolidate_Success;
}

//---------------------------------------------------------------------------------
// �㻯,װ���ֽ�
//---------------------------------------------------------------------------------
DWORD Role::DeComposeItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
								INT64 n64IMID, INT64 n64Item, DWORD dwCmdID)
{
	// �ҵ��ϳɱ�����
	const tagDeComposeProtoSer* pDeComposeProto = g_attRes.GetDeComposeProto(dwFormulaID);

	if(!P_VALID(pDeComposeProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ��������
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64Item); 

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckDeComposeLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pDeComposeProto, n64Item, pEquip);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	// �㻯ֻ���ڻ�ɫ������װ�����ֽ�����Ʒ��
	if(pDeComposeProto->eComType == ECOMT_PointUp && pEquip->equipSpec.byQuality == (BYTE)EIQ_White)
		return E_Compose_Quality_Not_Match;

	// ��߼�ʱװ���ɵ㻯��ֽ�
	if(pEquip->pProtoType->eType == EIT_Fashion || pEquip->pEquipProto->eEquipPos == EEP_Face)
		return E_Compose_Not_Fashion;

	// ��������װ�����ɵ㻯��ֽ�
	if(pEquip->bLock == true)
		return E_Compose_Equip_Lock;

	// ��ʱ�����Ƶ�װ�����ɵ㻯��ֽ�
	if(pEquip->pProtoType->dwTimeLimit != GT_INVALID)
		return E_Compose_Equip_Time_Limit;

	// δ������װ�����ɵ㻯��ֽ�
	if(pEquip->equipSpec.byQuality == (BYTE)EIQ_Null)
		return E_Compose_Equip_Not_identify;

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pDeComposeProto->dwMoneyConsume, ELCID_Compose_Decompose);

	// ���Ӻϳɼ���������(ֻ�м��ܺϳɲŻ����Ӽ��������ȣ�
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pDeComposeProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// �۳���ҵĻ���ֵ
	ChangeVitality(-(INT)pDeComposeProto->dwVitalityConsume);

	// �����Ƿ�����
	// ������Ǳ��ֵ > Ǳ��ֵ����/5 
	// ����������������ʱ�����������ʵĹ�ʽΪ��������=1%+��ɫ��Ե/1000)
	BOOL	bPerfect = FALSE;
	BOOL	bSuccess = FALSE;
	if( pEquip->equipSpec.nPotValUsed > pEquip->pEquipProto->nMaxPotVal / 5)
		bPerfect = TRUE;
	else
	{
		FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;
		bPerfect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);
	}

	// �����������
	GetItemMgr().DelFromBag(n64Item, (DWORD)ELCID_Compose_Decompose, (INT16)pEquip->n16Num);

	for(INT i = 0; i < MAX_OUTPUT_QUANTITY; ++i)
	{
		if(pDeComposeProto->OutputStuff[i].dwStuffTypeID == GT_INVALID)
			break;

		// ����ɹ���
		BOOL bResult = IUTIL->Rand() % 10000 <= (pDeComposeProto->OutputStuff[i].nRate);
		
		if(bResult)
		{
			bSuccess = TRUE;

			// ����
			if(bPerfect)
			{
				INT nStuffNum = IUTIL->Rand() % (pDeComposeProto->OutputStuff[i].nPerMaxVal + 1);

				if(nStuffNum < pDeComposeProto->OutputStuff[i].nPerMinVal)
					nStuffNum = pDeComposeProto->OutputStuff[i].nPerMinVal;

				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
					pDeComposeProto->OutputStuff[i].dwStuffTypeID, (INT16)nStuffNum, m_dwID);

				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}
			}
			else	// �ɹ�
			{
				INT nStuffNum = IUTIL->Rand() % (pDeComposeProto->OutputStuff[i].nSucMaxVal + 1);

				if(nStuffNum < pDeComposeProto->OutputStuff[i].nSucMinVal)
					nStuffNum = pDeComposeProto->OutputStuff[i].nSucMinVal;

				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
					pDeComposeProto->OutputStuff[i].dwStuffTypeID, (INT16)nStuffNum, m_dwID);

				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}
			}
		}		
	}

	if(bSuccess)
	{
		if(bPerfect)
			return E_Compose_Consolidate_Perfect;
		else	
			return E_Compose_Consolidate_Success;
	}
	else
		return E_Compose_Consolidate_Lose;
}

//---------------------------------------------------------------------------------
// ��������ϳ�ʱ������
//---------------------------------------------------------------------------------
DWORD Role::CheckProduceLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
								const tagProduceProtoSer* &pProduceProto, INT64 n64StuffID[], INT32 nArraySz)
{
	// ���ܺϳɷ�ʽ
	if(( pProduceProto->eFormFrom == EFormulaFrom_Skill || pProduceProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pProduceProto->eFormFrom == EFormulaFrom_SkillorNPC) && GT_INVALID != dwSkillID)
	{
		Skill* pSkill = GetSkill(dwSkillID);

		if(!P_VALID(pSkill))
			return E_Compose_Skill_Not_Exist;

		// ��⼼�������Ƿ�ƥ��
		if(Skill2ProduceType(pSkill->GetTypeEx2()) != pProduceProto->eProType)
			return E_Compose_FormulaNotMatch;

		// ��⼼�ܵȼ����䷽�ȼ��Ƿ�ƥ��
		if(pSkill->GetLevel() < pProduceProto->nFormLvl)
			return E_Compose_FormulaNotMatch;		
	}
	// ��Ʒ�ϳɷ�ʽ
	else if(( pProduceProto->eFormFrom == EFormulaFrom_Item || pProduceProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pProduceProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != n64ItemID)
	{
		// ��Ʒ�Ƿ��ڱ�����

		// �Ƿ�Ϊ�ϳ�����Ʒ
	}
	// NPC�ϳɷ�ʽ
	else if(( pProduceProto->eFormFrom == EFormulaFrom_NPC || pProduceProto->eFormFrom == EFormulaFrom_SkillorNPC
		|| pProduceProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != dwNPCID)
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC)) 
			return E_Compose_NPC_Not_Exist;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_Compose_NPC_Distance;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Compose) )
			return E_Compose_NPCCanNotCompose;

		// ��Ҫ���NPC�Ƿ���кϳ��䷽
	}

	// ����ɫ��������				
	ItemMgr& itemMgr = GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_Compose_Bag_Full;

	// ��һ���ֵ�Ƿ��㹻
	if(GetAttValue(ERA_Vitality) < (INT)pProduceProto->dwVitalityConsume)
		return E_Compose_Vitality_Inadequate;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pProduceProto->dwMoneyConsume)
		return E_Compose_NotEnough_Money;

	// �����Ҳ����Ƿ��㹻
	for(INT n = 0; n < MAX_PRODUCE_STUFF_QUANTITY; ++n)
	{
		if(pProduceProto->ProduceStuff[n].dwStuffID == GT_INVALID 
			&& pProduceProto->ProduceStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Compose_Stuff_Not_Enough;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Compose_Stuff_Not_Enough;
		}

		if(pItemStuff->dwTypeID != pProduceProto->ProduceStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pProduceProto->ProduceStuff[n].eStuffType)
			return E_Compose_Stuff_Not_Enough;

		if(pItemStuff->n16Num < (INT)pProduceProto->ProduceStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;
	}

	return E_Success;
}

//---------------------------------------------------------------------------------
// ��������ϳ�ʱ������
//---------------------------------------------------------------------------------
DWORD Role::CheckDeComposeLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
						 const tagDeComposeProtoSer* &pDeComposeProto, INT64 n64Item, tagEquip *pEquip)
{
	// ���ܺϳɷ�ʽ
	if(( pDeComposeProto->eFormFrom == EFormulaFrom_Skill || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pDeComposeProto->eFormFrom == EFormulaFrom_SkillorNPC) && GT_INVALID != dwSkillID)
	{
		Skill* pSkill = GetSkill(dwSkillID);

		if(!P_VALID(pSkill))
			return E_Compose_Skill_Not_Exist;

		// ��⼼�������Ƿ�ƥ��
		if(pSkill->GetTypeEx2() != ESSTE2_PointUp)
			return E_Compose_FormulaNotMatch;

		// ��⼼�ܵȼ����䷽�ȼ��Ƿ�ƥ��
		if(pSkill->GetLevel() < pDeComposeProto->nFormLvl)
			return E_Compose_FormulaNotMatch;		
	}
	// ��Ʒ�ϳɷ�ʽ
	else if(( pDeComposeProto->eFormFrom == EFormulaFrom_NPC || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorNPC
			|| pDeComposeProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != n64ItemID )
	{
		// ��Ʒ�Ƿ��ڱ�����

		// �Ƿ�Ϊ�ϳ�����Ʒ
	}
	// NPC�ϳɷ�ʽ
	else if( ( pDeComposeProto->eFormFrom == EFormulaFrom_Item || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorItem
			|| pDeComposeProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != dwNPCID )
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC)) 
			return E_Compose_NPC_Not_Exist;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_Compose_NPC_Distance;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Compose) )
			return E_Compose_NPCCanNotCompose;

		// ��Ҫ���NPC�Ƿ���кϳ��䷽
	}

	// ����ɫ��������				
	ItemMgr& itemMgr = GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_Compose_Bag_Full;

	// ��һ���ֵ�Ƿ��㹻
	if(GetAttValue(ERA_Vitality) < (INT)pDeComposeProto->dwVitalityConsume)
		return E_Compose_Vitality_Inadequate;

	if(!P_VALID(pEquip))
		return E_Compose_Stuff_Not_Enough;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pDeComposeProto->dwMoneyConsume)
		return E_Compose_NotEnough_Money;

	// ���װ���ȼ��Ƿ�����
	if((pEquip->pProtoType->byLevel / 10) < pDeComposeProto->byLevel)
		return E_Compose_Equip_Level_Inadequate;

	// ���װ��Ʒ���Ƿ�����Ҫ��
	if(pEquip->equipSpec.byQuality < pDeComposeProto->byQuality)
		return E_Compose_Equip_Quality_Inadequate;

	// ������������Ƿ������䷽Ҫ��
	if(pEquip->pProtoType->eType == EIT_Weapon && pEquip->pProtoType->eTypeEx != pDeComposeProto->eTypeEx)
		return E_Compose_Equip_Type_Inadequate;

	// ���װ���Ƿ������䷽Ҫ��
	if((pEquip->pProtoType->eType == EIT_ClothArmor 
		|| pEquip->pProtoType->eType == EIT_Decoration 
		|| pEquip->pProtoType->eType == EIT_Armor) 
		&& pEquip->pEquipProto->eEquipPos != pDeComposeProto->ePos)

		return E_Compose_Equip_Type_Inadequate;

	return E_Success;
}

//---------------------------------------------------------------------------------
// ������������ת��Ϊ��������
//---------------------------------------------------------------------------------
EProduceType Role::Skill2ProduceType(ESkillTypeEx2 eSkillType)					 
{													 				 
	switch(eSkillType)								 
	{												 
	case ESSTE2_Smilt:					return EPCT_Smilt;							 
	case ESSTE2_Artisan:				return EPCT_Artisan;						 
	case ESSTE2_DanTraining:			return EPCT_DanTraining;					 
	case ESSTE2_Smith:					return EPCT_Smith;							 
	case ESSTE2_Casting:				return EPCT_Casting;						 
	case ESSTE2_Dressmaker:				return EPCT_Dressmaker;						 
	case ESSTE2_Aechnics:				return EPCT_Aechnics;
	default:							return EPCT_NULL;
	}												 
}

//---------------------------------------------------------------------------------
// ������Ʒ���Ƿ���Ҫ�������ߣ�
//---------------------------------------------------------------------------------
tagItem* Role::CreateItem(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num, DWORD dwCreator)
{
	tagItemProto *pProto = g_attRes.GetItemProto(dwTypeID);
	if(!P_VALID(pProto))
		return (tagItem*)GT_INVALID;

	if(pProto->n16MaxLapNum > 1)
		return ItemCreator::Create(EICM_Product,  m_dwID, dwTypeID, n16Num);
	else
		return ItemCreator::Create(EICM_Product,  m_dwID, dwTypeID, n16Num, m_dwID);		
}

//---------------------------------------------------------------------------------
// ����IM��Ʒ��Ӱ��
//---------------------------------------------------------------------------------
VOID Role::CalIMEffect(EConsolidateTypeSer eConType, tagIMEffect &IMEffect, INT64 n64IMID, const LPVOID pProto)
{

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( GT_INVALID == n64IMID)
		return;

	tagItem *pItemIM = GetItemMgr().GetBagItem(n64IMID); 
	if(!P_VALID(pItemIM))
		return;

	switch (eConType)
	{
	case ECTS_Posy:
		{
			const tagPosyProtoSer*  pPosyProto = (tagPosyProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
			   if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Posy 
				  || pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
				  || pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
			   {
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Posy, 1);
			   }
			   break;
			}

			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Posy, 1);
			}
		}
		break;
	case ECTS_Engrave:
		{
			const tagEngraveProtoSer* pEngraveProto = (tagEngraveProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Engrave 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Engrave, 1);
				}
				break;
			}

			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Engrave, 1);
			}
		}
		break;
	case ECTS_Inlay:
		{

		}
		break;
	case ECTS_Brand:
		{
			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Brand, 1);
			}
		}
		break;
	case ECTS_LoongSoul:
		{

		}
		break;
	case ECTS_Quench:
		{
			const tagQuenchProtoSer *pQuenchProto  = (tagQuenchProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Quench 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Quench, 1);
				}
				break;
			}
		}
		break;
	case ECTS_Produce:
		{
			const tagProduceProtoSer*  pProduceProto = (tagProduceProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)pProduceProto->eComType 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)ESFCA_AllProduce
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Produce, 1);
				}
				break;
			}
			else if (pItemIM->pProtoType->eSpecFunc == EISF_ColorProbability)
			{
				IMEffect.eEffect = EIME_Color;
				IMEffect.dwParam1 = pItemIM->pProtoType->nSpecFuncVal1;
				IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Produce, 1);
			}
		}
		break;
	case ECTS_Decompose:
		{

		}
		break;
	case ECTS_RaisePotVal:
		{
			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign/*EISF_EquipAddPotValue*/)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
			}
			GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Raise_PotVal, 1);
		}
		break;
	}
}


//---------------------------------------------------------------------------------
// ����װ������
//---------------------------------------------------------------------------------
VOID Role::CalEquipFlare(tagEquip* pEquip)
{
	BYTE byFlareVal = 0;
	BYTE byHoleBNum = 0;
	// ������Ƕ����
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		byHoleBNum++;
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
		{
			break;
		}
	}

	// ����ֵ��=  1�����ƴ��� + 1���Կ̴��� + 2����Ƕ���� + 2����ǰ��ӡ�ȼ� + 3��װ����ǰ�����ȼ�
	byFlareVal = pEquip->equipSpec.byPosyTimes + pEquip->equipSpec.byEngraveTimes + byHoleBNum * 2 
				+ pEquip->equipSpec.byBrandLevel * 2 + pEquip->equipSpec.byLongfuLevel * 3;

	pEquip->SetFlareVal(byFlareVal);
}



DWORD Role::RaiseEquipPotVal(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD & dwPotValAdded)
{
	dwPotValAdded = 0;
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�������װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// �ҵ�Ǳ����������
	tagItem *pItemRaiser = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemRaiser) || pItemRaiser->pProtoType->eSpecFunc != EISF_EquipAddPotValue )
		return E_Compose_Stuff_Not_Exist/*E_Consolidate_Brand_Not_Exit*/;

	// �ҵ�ǿ�����ݣ��������Ҫ
	//const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemRaiser->dwTypeID);
	//if(!P_VALID(pConsolidateProto))
	//	return E_Compose_Formula_Not_Exist;

	// ����������������
	if( 0 == pEquip->equipSpec.nPotIncTimes )
		return E_Raise_PotVal_Times_Zero;

	// ���Ǳ���������ޣ���Ҫ��ԭ����������ֵ
	INT32 nMaxPotVal = pEquip->pEquipProto->nMaxPotVal/*pEquip->equipSpec.nPotVal + pEquip->equipSpec.nPotValUsed*/;
	if( pEquip->equipSpec.nPotVal >= nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ )
		return E_Raise_PotVal_Not_Needing;

	INT nPotDelta = pItemRaiser->pProtoType->nSpecFuncVal2;

	// ���տ�����װ������Ǳ��
	if((pEquip->equipSpec.nPotVal + nPotDelta ) > nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ )
		nPotDelta = nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ - pEquip->equipSpec.nPotVal;

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Raise_PotVal, 1);

	// ����ɹ���
	FLOAT fProp = /*(*/ pItemRaiser->pProtoType->nSpecFuncVal1 + (FLOAT)GetAttValue(ERA_Fortune) /*)*/ / 1000.0f;

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	//tagConsolidateItem *pProto = const_cast<tagConsolidateItem*>(pConsolidateProto);		
	CalIMEffect(ECTS_RaisePotVal, IMEffect, n64IMID, NULL);

	// ����ɹ���
	INT rand_n = IUTIL->Rand();
	BOOL bResult = /*IUTIL->RandomInRange(0,100) <= fProp;*/rand_n  % 10000 <= fProp;

	if(bResult)		// �ɹ�
	{
		// �ݼ�����Ǳ��ֵ����
		--pEquip->equipSpec.nPotIncTimes;
		if( pEquip->equipSpec.nPotIncTimes < 0 )
			pEquip->equipSpec.nPotIncTimes = 0;

		// �����Ƿ�����������������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000)
		FLOAT fPefectProp = 0.01 + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		rand_n = IUTIL->Rand();
		BOOL bPefect = rand_n % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			dwPotValAdded = pEquip->equipSpec.nPotVal;
			pEquip->ChangePotVal( pEquip->equipSpec.nPotVal );
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			dwPotValAdded = nPotDelta;
			pEquip->ChangePotVal( nPotDelta );
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// װ��Ǳ��ֵ����100��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal( -100 );

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}

	return 0;
}
// Jason Gem removal info v1.3.1 2009-12-21
DWORD Role::GetGemRemovalInfo(DWORD dwNPCID,INT64 equipID,INT8 & num,DWORD dwGemIDs[MAX_EQUIPHOLE_NUM])
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GemRemoval) )
		return E_Gem_NPCCanNotRemoveGem;

	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(equipID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	num = pEquip->equipSpec.byHoleNum;
	if(num > MAX_EQUIPHOLE_NUM)
		num = MAX_EQUIPHOLE_NUM;
	else if( num < 0 )
		num = 0;

	memset(dwGemIDs,0,sizeof(dwGemIDs));

	for(int i = 0; i < num ; ++i)
		dwGemIDs[i] = pEquip->equipSpec.dwHoleGemID[i];

	return E_Success;
}

DWORD Role::RemoveGemFromEquip(DWORD dwNPCID,INT64 equipID,INT8 num,INT64 SignIDs[MAX_EQUIPHOLE_NUM])
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GemRemoval) )
		return E_Gem_NPCCanNotRemoveGem;

	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(equipID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	INT8 n8Num = pEquip->equipSpec.byHoleNum;


	tagItem* pItems[MAX_EQUIPHOLE_NUM] = {0};
	INT64 signs[MAX_EQUIPHOLE_NUM] = {0};
	DWORD gems[MAX_EQUIPHOLE_NUM] = {0};
	int count = 0;

	for(int i = 0,j = 0; i < n8Num ; ++i)
	{
		tagItemProto * pItemProto = g_attRes.GetItemProto( pEquip->equipSpec.dwHoleGemID[i] );
		tagItem *pItemGemSign = GetItemMgr().GetBagItem(SignIDs[i]);
		if( P_VALID(pItemProto) && P_VALID(pItemGemSign) && pItemGemSign->pProtoType->eSpecFunc == EISF_GemRemovalSign )
		{
			if( pItemProto->byQuality == pItemGemSign->pProtoType->byQuality )
			{
				signs[j] = SignIDs[i];
				gems[j++] = pItemProto->dwTypeID;
				++count;
			}
		}
	}

	if( count > 0 )
	{
		INT64 totalSilver = count * 10 * 10000;
		if( GetCurMgr  ().GetBagSilver() >= totalSilver )
		{
			GetCurMgr().DecBagSilver(totalSilver, ELCID_GemRemoval);
			GetItemMgr  ().DelFromBag  (equipID,ELCID_GemRemoval,1);
			for (int i = 0; i < count ; ++i)
			{
				pItems[i] = ItemCreator::Create  (EICM_GemRemoval,GetID  (),gems[i]);
				if( !P_VALID(pItems[i]) )
				{
				}
				//else
				{
					GetItemMgr  ().DelFromBag  (signs[i],ELCID_GemRemoval,1);
				}
			}
			GetItemMgr().Add2Bag  (pItems,count,ELCID_GemRemoval,TRUE);
		}
		else
			return E_Consolidate_NotEnough_Money;
	}


	return E_Success;
}


