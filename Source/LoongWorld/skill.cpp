//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: skill.cpp
// author: Aslan
// actor:
// data: 2008-9-10
// last:
// brief: ����
//-------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/skill_define.h"

#include "att_res.h"
#include "unit.h"
#include "role.h"
#include "skill_buff.h"
#include "skill_trigger.h"
#include "skill.h"
#include "role.h"

//------------------------------------------------------------------------------
// ���ܳ�ʼ��
//------------------------------------------------------------------------------
BOOL Skill::Init(DWORD dwID, INT nSelfLevel, INT nLearnLevel, INT nCoolDown, INT nProficiency)
{
	ASSERT( P_VALID(dwID) && nCoolDown >= 0 && nSelfLevel >= 0 && nLearnLevel >= 0 );
	ASSERT( nSelfLevel > 0 || nLearnLevel > 0 );

	INT nLevel = nSelfLevel + nLearnLevel;

	m_dwID = dwID;
	m_nLevel = nSelfLevel + nLearnLevel;
	m_nSelfLevel = nSelfLevel;
	m_nLearnLevel = nLearnLevel;
	m_nTempAddLevel = 0;
	m_nCoolDownCountDown = nCoolDown;
	m_nProficiency = nProficiency;

	m_pProto = g_attRes.GetSkillProto(CreateTypeID(m_dwID, m_nLevel));
	ASSERT( P_VALID(m_pProto) );

	// ������ܱ���������Ӱ�죬������Mod�ṹ
	if( g_attRes.CanBeModified(m_dwID) )
	{
		m_pMod = new tagSkillMod;

		// ���������Ա��е�buff�����õ�mod����
		SetSkillBuff(m_pProto);
	}
	// ����Ͳ�����
	else
	{
		m_pMod = NULL;
	}

	m_pScript = g_ScriptMgr.GetSkillScript(CreateTypeID(m_dwID, m_nLevel));
	m_pOwner = NULL;

	return TRUE;
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID Skill::Update()
{

}

//---------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------
VOID Skill::InitSkillSave(OUT tagSkillSave *pSkillSave)
{
	pSkillSave->dwID			=	m_dwID;
	pSkillSave->nLearnLevel		=	m_nLearnLevel;
	pSkillSave->nSelfLevel		=	m_nSelfLevel;
	pSkillSave->nProficiency	=	m_nProficiency;
	pSkillSave->nCoolDown		=	m_nCoolDownCountDown;
}

//----------------------------------------------------------------------------
// ���ɼ�����Ϣ�ṹ
//----------------------------------------------------------------------------
VOID Skill::GenerateMsgInfo(tagSkillMsgInfo* const pMsgInfo)
{
	pMsgInfo->dwID			=	GetID();
	pMsgInfo->nLevel		=	GetLevel();
	pMsgInfo->nLearnLevel	=	GetLearnLevel();
	pMsgInfo->nProficiency	=	GetProficiency();
	pMsgInfo->nMaxCoolDown	=	GetCoolDown();
	pMsgInfo->nCoolDown		=	GetCoolDownCountDown();
	pMsgInfo->fOpDist		=	GetOPDist();
	pMsgInfo->nPrepareTime	=	GetPrepareTime();

	// �˺�
	for(INT n = 0; n < MAX_CHANNEL_TIMES; n++)
	{
		pMsgInfo->nChannelDmg[n] = GetDmg(n);
	}

	// ����
	for(INT n = 0; n < ESCT_End; n++)
	{
		pMsgInfo->nCost[n] = GetCost((ESkillCostType)n);
	}
}

//---------------------------------------------------------------------------------
// ���ü������Լӳɣ������Ƿ������Ա�����¹�
//---------------------------------------------------------------------------------
BOOL Skill::SetMod(const tagSkillProto* pProto)
{
	if( !P_VALID(pProto) ) return FALSE;
	if( ESTT_Skill != pProto->eTargetType ) return FALSE;

	// ��������
	m_pMod->fOPDistMod += pProto->fOPDist;

	// �����뾶
	m_pMod->fOPRadiusMod += pProto->fOPRadius;

	// ����ʱ��	
	m_pMod->nPrepareTimeMod += pProto->nPrepareTime;

	// ��ȴʱ��
	m_pMod->nCoolDownMod += pProto->nCoolDown;

	// ���
	if( abs(pProto->nEnmity) < 100000 )
	{
		m_pMod->nEnmityMod += pProto->nEnmity;
	}
	else
	{
		m_pMod->nEnmityModPct += (pProto->nEnmity > 0 ? 
			pProto->nEnmity - 100000 : pProto->nEnmity + 100000);
	}

	// ����
	if( abs(pProto->nHit) < 100000 )
	{
		m_pMod->nHitMod += pProto->nHit;
	}
	else
	{
		m_pMod->nHitModPct += (pProto->nHit > 0 ? 
			pProto->nHit - 100000 : pProto->nHit + 100000);
	}

	// ����
	if( abs(pProto->nCrit) < 100000 )
	{
		m_pMod->nCritMod += pProto->nCrit;
	}
	else
	{
		m_pMod->nCritModPct += (pProto->nCrit > 0 ? 
			pProto->nCrit - 100000 : pProto->nCrit + 100000);
	}

	// �˺�
	for(INT n = 0; n < MAX_CHANNEL_TIMES; n++)
	{
		if( abs(pProto->nChannelDmg[n]) < 100000 )
		{
			m_pMod->nChannelDmgMod[n] += pProto->nChannelDmg[n];
		}
		else
		{
			m_pMod->nChannelDmgModPct[n] += (pProto->nChannelDmg[n] > 0 ? 
				pProto->nChannelDmg[n] - 100000 : pProto->nChannelDmg[n] + 1000000);
		}
	}

	// ����
	for(INT n = 0; n < ESCT_End; n++)
	{
		if( abs(pProto->nSkillCost[n]) < 100000 )
		{
			m_pMod->nSkillCostMod[n] += pProto->nSkillCost[n];
		}
		else
		{
			m_pMod->nSkillCostModPct[n] += (pProto->nSkillCost[n] > 0 ? 
				pProto->nSkillCost[n] - 100000 : pProto->nSkillCost[n] + 1000000);
		}
	}

	// Buff��Trigger
	SetSkillBuff(pProto);

	// �������Լӳ�
	ERoleAttribute eAtt = ERA_Null;
	INT nMod = 0;

	TMap<ERoleAttribute, INT>& mapMod = pProto->mapRoleAttMod;
	TMap<ERoleAttribute, INT>::TMapIterator itMod = mapMod.Begin();
	while( mapMod.PeekNext(itMod, eAtt, nMod) )
	{
		m_pMod->mapRoleAttMod.ModifyValue(eAtt, nMod);
	}

	TMap<ERoleAttribute, INT>& mapModPct = pProto->mapRoleAttModPct;
	TMap<ERoleAttribute, INT>::TMapIterator itModPct = mapModPct.Begin();
	while( mapModPct.PeekNext(itModPct, eAtt, nMod) )
	{
		m_pMod->mapRoleAttModPct.ModifyValue(eAtt, nMod);
	}

	// ����ñ�Ӱ��ļ��ܱ�������������ϣ���Ҫ������������Ӱ��
	if( P_VALID(m_pOwner) && IsCanModAtt() )
	{
		TMap<ERoleAttribute, INT>& mapMod = pProto->mapRoleAttMod;
		TMap<ERoleAttribute, INT>::TMapIterator itMod = mapMod.Begin();
		while( mapMod.PeekNext(itMod, eAtt, nMod) )
		{
			m_pOwner->ModAttModValue(eAtt, nMod);
		}

		TMap<ERoleAttribute, INT>& mapModPct = pProto->mapRoleAttModPct;
		TMap<ERoleAttribute, INT>::TMapIterator itModPct = mapModPct.Begin();
		while( mapModPct.PeekNext(itModPct, eAtt, nMod) )
		{
			m_pOwner->ModAttModValuePct(eAtt, nMod);
		}
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------
// �����������Լӳɣ����ؼ������Ա����Ƿ���¹�
//----------------------------------------------------------------------------------
BOOL Skill::UnSetMod(const tagSkillProto* pProto)
{
	if( !P_VALID(m_pMod) ) return FALSE;
	if( ESTT_Skill != pProto->eTargetType ) return FALSE;

	// ��������
	m_pMod->fOPDistMod -= pProto->fOPDist;

	// �����뾶
	m_pMod->fOPRadiusMod -= pProto->fOPRadius;

	// ����ʱ��	
	m_pMod->nPrepareTimeMod -= pProto->nPrepareTime;

	// ��ȴʱ��
	m_pMod->nCoolDownMod -= pProto->nCoolDown;

	// ���
	if( abs(pProto->nEnmity) < 100000 )
	{
		m_pMod->nEnmityMod -= pProto->nEnmity;
	}
	else
	{
		m_pMod->nEnmityModPct -= (pProto->nEnmity > 0 ? 
			pProto->nEnmity - 100000 : pProto->nEnmity + 100000);
	}

	// ����
	if( abs(pProto->nHit) < 100000 )
	{
		m_pMod->nHitMod -= pProto->nHit;
	}
	else
	{
		m_pMod->nHitModPct -= (pProto->nHit > 0 ? 
			pProto->nHit - 100000 : pProto->nHit + 100000);
	}

	// ����
	if( abs(pProto->nCrit) < 100000 )
	{
		m_pMod->nCritMod -= pProto->nCrit;
	}
	else
	{
		m_pMod->nCritModPct -= (pProto->nCrit > 0 ? 
			pProto->nCrit - 100000 : pProto->nCrit + 100000);
	}

	// �˺�
	for(INT n = 0; n < MAX_CHANNEL_TIMES; n++)
	{
		if( abs(pProto->nChannelDmg[n]) < 100000 )
		{
			m_pMod->nChannelDmgMod[n] -= pProto->nChannelDmg[n];
		}
		else
		{
			m_pMod->nChannelDmgModPct[n] -= (pProto->nChannelDmg[n] > 0 ? 
				pProto->nChannelDmg[n] - 100000 : pProto->nChannelDmg[n] + 1000000);
		}
	}

	// ����
	for(INT n = 0; n < ESCT_End; n++)
	{
		if( abs(pProto->nSkillCost[n]) < 100000 )
		{
			m_pMod->nSkillCostMod[n] -= pProto->nSkillCost[n];
		}
		else
		{
			m_pMod->nSkillCostModPct[n] -= (pProto->nSkillCost[n] > 0 ? 
				pProto->nSkillCost[n] - 100000 : pProto->nSkillCost[n] + 1000000);
		}
	}

	// Buff��Trigger
	UnSetSkillBuff(pProto);

	// �������Լӳ�
	ERoleAttribute eAtt = ERA_Null;
	INT nMod = 0;

	TMap<ERoleAttribute, INT>& mapMod = pProto->mapRoleAttMod;
	TMap<ERoleAttribute, INT>::TMapIterator itMod = mapMod.Begin();
	while( mapMod.PeekNext(itMod, eAtt, nMod) )
	{
		m_pMod->mapRoleAttMod.ModifyValue(eAtt, -nMod);
	}

	TMap<ERoleAttribute, INT>& mapModPct = pProto->mapRoleAttModPct;
	TMap<ERoleAttribute, INT>::TMapIterator itModPct = mapModPct.Begin();
	while( mapModPct.PeekNext(itModPct, eAtt, nMod) )
	{
		m_pMod->mapRoleAttModPct.ModifyValue(eAtt, -nMod);
	}

	// ����ñ�Ӱ��ļ��ܱ�������������ϣ���Ҫ������������Ӱ��
	if( P_VALID(m_pOwner) && IsCanModAtt() )
	{
		TMap<ERoleAttribute, INT>& mapMod = pProto->mapRoleAttMod;
		TMap<ERoleAttribute, INT>::TMapIterator itMod = mapMod.Begin();
		while( mapMod.PeekNext(itMod, eAtt, nMod) )
		{
			m_pOwner->ModAttModValue(eAtt, -nMod);
		}

		TMap<ERoleAttribute, INT>& mapModPct = pProto->mapRoleAttModPct;
		TMap<ERoleAttribute, INT>::TMapIterator itModPct = mapModPct.Begin();
		while( mapModPct.PeekNext(itModPct, eAtt, nMod) )
		{
			m_pOwner->ModAttModValuePct(eAtt, -nMod);
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------
// �������߼��ϼ������Լӳ�
//--------------------------------------------------------------------------------------
VOID Skill::SetOwnerMod()
{
	// ���Ƕ���������Ӱ��ļ��ܲ�����
	if( !P_VALID(m_pProto) || FALSE == IsCanModAtt() )
		return;

	// ��������߲����ڣ�Ҳ������
	if( !P_VALID(m_pOwner) ) return;

	// �ȼ��㾲̬���Լӳ�
	ERoleAttribute eType = ERA_Null;
	INT nValue = 0;

	// �ȼ���ƽֵ�ӳ�
	TMap<ERoleAttribute, INT>::TMapIterator it = m_pProto->mapRoleAttMod.Begin();
	while( m_pProto->mapRoleAttMod.PeekNext(it, eType, nValue) )
	{
		m_pOwner->ModAttModValue(eType, nValue);
	}

	// �ټ���ٷֱȼӳ�
	it = m_pProto->mapRoleAttModPct.Begin();
	while( m_pProto->mapRoleAttModPct.PeekNext(it, eType, nValue) )
	{
		m_pOwner->ModAttModValuePct(eType, nValue);
	}

	// �ټ��㼼�ܱ�Ӱ��ӳ�
	if( P_VALID(m_pMod) )
	{
		ERoleAttribute eType = ERA_Null;
		INT nValue = 0;

		// �ȼ���ƽֵ�ӳ�
		TMap<ERoleAttribute, INT>::TMapIterator it = m_pMod->mapRoleAttMod.Begin();
		while( m_pMod->mapRoleAttMod.PeekNext(it, eType, nValue) )
		{
			m_pOwner->ModAttModValue(eType, nValue);
		}

		// �ټ���ٷֱȼӳ�
		it = m_pMod->mapRoleAttModPct.Begin();
		while( m_pMod->mapRoleAttModPct.PeekNext(it, eType, nValue) )
		{
			m_pOwner->ModAttModValuePct(eType, nValue);
		}
	}

	// Buffע��
	RegisterTriggerEvent();
}

//-------------------------------------------------------------------------------------
// �������������ϵļ������Լӳ�
//-------------------------------------------------------------------------------------
VOID Skill::UnsetOwnerMod()
{
	// ���Ƕ���������Ӱ��ļ��ܲ�����
	if( !P_VALID(m_pProto) || FALSE == IsCanModAtt() )
		return;

	// ��������߲����ڣ�Ҳ������
	if( !P_VALID(m_pOwner) ) return;

	// �ȼ��㾲̬���Լӳ�
	ERoleAttribute eType = ERA_Null;
	INT nValue = 0;

	// �ȳ���ƽֵ�ӳ�
	TMap<ERoleAttribute, INT>::TMapIterator it = m_pProto->mapRoleAttMod.Begin();
	while( m_pProto->mapRoleAttMod.PeekNext(it, eType, nValue) )
	{
		m_pOwner->ModAttModValue(eType, -nValue);
	}

	// �ٳ����ٷֱȼӳ�
	it = m_pProto->mapRoleAttModPct.Begin();
	while( m_pProto->mapRoleAttModPct.PeekNext(it, eType, nValue) )
	{
		m_pOwner->ModAttModValuePct(eType, -nValue);
	}

	// �ٳ������ܱ�Ӱ��ӳ�
	if( P_VALID(m_pMod) )
	{
		ERoleAttribute eType = ERA_Null;
		INT nValue = 0;

		// �ȼ���ƽֵ�ӳ�
		TMap<ERoleAttribute, INT>::TMapIterator it = m_pMod->mapRoleAttMod.Begin();
		while( m_pMod->mapRoleAttMod.PeekNext(it, eType, nValue) )
		{
			m_pOwner->ModAttModValue(eType, -nValue);
		}

		// �ټ���ٷֱȼӳ�
		it = m_pMod->mapRoleAttModPct.Begin();
		while( m_pMod->mapRoleAttModPct.PeekNext(it, eType, nValue) )
		{
			m_pOwner->ModAttModValuePct(eType, -nValue);
		}
	}

	// Buff��ע��
	UnRegisterTriggerEvent();
}

//-----------------------------------------------------------------------------------------
// ע�Ἴ������Ĵ���������
//-----------------------------------------------------------------------------------------
VOID Skill::RegisterTriggerEvent()
{
	if( !IsCanModAtt() ) return;

	if( !P_VALID(m_pOwner) ) return;
	if( !m_pOwner->IsRole() ) return;

	Role* pRole = static_cast<Role*>(m_pOwner);

	for(INT n = 0; n < MAX_BUFF_PER_SKILL; ++n)
	{
		DWORD dwTriggerID = GetTriggerID(n);
		if( !P_VALID(dwTriggerID) ) continue;

		const tagTriggerProto* pProto = g_attRes.GetTriggerProto(dwTriggerID);
		if( !P_VALID(pProto) ) continue;

		pRole->RegisterTriggerSkillSet(pProto->eEventType, m_dwID);	
	}
}

//------------------------------------------------------------------------------------------
// ��ע�Ἴ������Ĵ���������
//------------------------------------------------------------------------------------------
VOID Skill::UnRegisterTriggerEvent()
{
	if( !IsCanModAtt() ) return;

	if( !P_VALID(m_pOwner) ) return;
	if( !m_pOwner->IsRole() ) return;

	Role* pRole = static_cast<Role*>(m_pOwner);

	for(INT n = 0; n < MAX_BUFF_PER_SKILL; ++n)
	{
		DWORD dwTriggerID = GetTriggerID(n);
		if( !P_VALID(dwTriggerID) ) continue;

		const tagTriggerProto* pProto = g_attRes.GetTriggerProto(dwTriggerID);
		if( !P_VALID(pProto) ) continue;

		pRole->UnRegisterTriggerSkillSet(pProto->eEventType, m_dwID);	
	}
}

//-------------------------------------------------------------------------------------
// �õ���������
//-------------------------------------------------------------------------------------
inline INT Skill::GetCost(ESkillCostType eCostType)
{
	ASSERT( eCostType >= 0 && eCostType < ESCT_End );

	INT nCost = m_pProto->nSkillCost[eCostType];

	if (nCost > 100000 && ESCT_Rage != eCostType)
	{
		INT  nIndex(-1);
		float fTmpCost(0);
		fTmpCost = (float)nCost;
		
		if ( ESCT_HP ==eCostType)
		{
			nIndex = ERA_MaxHP;
		}
		else if (ESCT_MP ==eCostType)
		{
			nIndex = ERA_MaxMP;
		}
		else if (ESCT_Endurance ==eCostType)
		{
			nIndex = ERA_MaxEndurance;
		}
		else if (ESCT_Valicity ==eCostType)
		{
			nIndex = ERA_MaxVitality;
		}

		float fChangeRate = ((fTmpCost/100)-1000)/100;

		if( !P_VALID(m_pOwner) )
			return 0;
		nCost = float(( m_pOwner->GetAttValue(nIndex))*fChangeRate); 
	}

	if( P_VALID(m_pMod) )
	{
		nCost = nCost + m_pMod->nSkillCostMod[eCostType] + INT((FLOAT)nCost * ((FLOAT)m_pMod->nSkillCostModPct[eCostType] / 10000.0f));
	}

	return nCost;
}
