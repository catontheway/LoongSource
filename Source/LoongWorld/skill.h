//-----------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: skill.h
// author: Aslan
// actor:
// data: 2008-9-10
// last:
// brief: ����
//-----------------------------------------------------------------------------------------
#pragma once

#include "skill_buff.h"
#include "skill_trigger.h"

//-----------------------------------------------------------------------------------------
// ���ܵȼ��ı䷽ʽ
//-----------------------------------------------------------------------------------------
enum ESkillLevelChange
{
	ESLC_Learn = 0,		// Ͷ��
	ESLC_Self = 1,		// �̶�
	ESLC_Temp = 2,		// ��ʱ
};

//-----------------------------------------------------------------------------------------
// �������ܶԸü��ܵ�Ӱ��
//-----------------------------------------------------------------------------------------
struct tagSkillMod
{
	// ��������Ӱ��
	FLOAT			fOPDistMod;								// ���þ���ӳ�
	FLOAT			fOPRadiusMod;							// ���÷�Χ�ӳ�
	INT				nPrepareTimeMod;						// ����ʱ��ӳɣ����룩
	INT				nCoolDownMod;							// ��ȴʱ��ӳ�
	INT				nEnmityMod;								// ���ܳ�޼ӳ�
	INT				nEnmityModPct;							// ���ܳ�ްٷֱȼӳ�
	INT				nHitMod;								// ���������ʼӳ�
	INT				nHitModPct;								// ���������ʰٷֱȼӳ�
	INT				nCritMod;								// ���������ʼӳ�
	INT				nCritModPct;							// ���������ʰٷֱȼӳ�

	// �˺�
	INT				nChannelDmgMod[MAX_CHANNEL_TIMES];		// �ܵ��˺�ÿһ�ε��˺����ӳ�
	INT				nChannelDmgModPct[MAX_CHANNEL_TIMES];	// �ܵ��˺�ÿһ�ε��˺����İٷֱȼӳ�

	// ״̬
	DWORD			dwBuffTypeID[MAX_BUFF_PER_SKILL];		// ��������buff
	DWORD			dwTriggerID[MAX_BUFF_PER_SKILL];		// ����������buff������

	// ����
	INT				nSkillCostMod[ESCT_End];				// �������ļӳ�
	INT				nSkillCostModPct[ESCT_End];				// �������İٷֱȼӳ�

	// ��������Ӱ��
	mutable TMap<ERoleAttribute, INT>	mapRoleAttMod;		// �������ܶԸü�����ɵ���������Ӱ��ļӳ�
	mutable TMap<ERoleAttribute, INT>	mapRoleAttModPct;	// �������ܶԸü�����ɵ���������Ӱ��İٷֱȼӳ�

	tagSkillMod()
	{
		fOPDistMod = 0.0f;
		fOPRadiusMod = 0.0f;
		nPrepareTimeMod = 0;
		nCoolDownMod = 0;
		nEnmityMod = 0;
		nEnmityModPct = 0;
		nHitMod = 0;
		nHitModPct = 0;
		nCritMod = 0;
		nCritModPct = 0;

		memset(dwBuffTypeID, 0xFF, sizeof(dwBuffTypeID));
		memset(dwTriggerID, 0xFF, sizeof(dwTriggerID));
		ZeroMemory(nChannelDmgMod, sizeof(nChannelDmgMod));
		ZeroMemory(nChannelDmgModPct, sizeof(nChannelDmgModPct));
		ZeroMemory(nSkillCostMod, sizeof(nSkillCostMod));
		ZeroMemory(nSkillCostModPct, sizeof(nSkillCostModPct));
	}
};

class SkillScript;

//--------------------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------------------
class Skill
{
public:
	//----------------------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------------------
	Skill();
	Skill(DWORD dwID, INT nSelfLevel, INT nLearnLevel, INT nCoolDown, INT nProficiency);
	~Skill()	{ SAFE_DEL(m_pMod); }

	//----------------------------------------------------------------------------------------
	// һЩͨ�ú���
	//----------------------------------------------------------------------------------------
	static DWORD			GetIDFromTypeID(DWORD dwTypeID)		{ return dwTypeID / 100; }
	static INT				GetLevelFromTypeID(DWORD dwTypeID)	{ return dwTypeID % 100; }
	static DWORD			CreateTypeID(DWORD dwID, INT nLevel){ return dwID * 100 + nLevel; }

	//----------------------------------------------------------------------------------------
	// ��ʼ�������£��������Ϣ
	//----------------------------------------------------------------------------------------
	BOOL					Init(DWORD dwID, INT nSelfLevel, INT nLearnLevel, INT nCoolDown, INT nProficiency);
	VOID					Update();
	VOID					InitSkillSave(OUT tagSkillSave* pSkillSave);
	VOID					GenerateMsgInfo(tagSkillMsgInfo* const pMsgInfo);
	BOOL					IsNeedSave()			{ return m_bNeedSave;}
	VOID					SetSaved()				{ m_bNeedSave = FALSE;}

	//----------------------------------------------------------------------------------------
	// ����Get
	//----------------------------------------------------------------------------------------
	DWORD					GetID()					{ return m_dwID; }
	DWORD					GetTypeID()				{ return m_pProto->dwID; }
	INT						GetLevel()				{ return m_nLevel; }
	INT						GetLearnLevel()			{ return m_nLearnLevel; }
	INT						GetSelfLevel()			{ return m_nSelfLevel; }
	INT						GetTempAddLevel()		{ return m_nTempAddLevel; }
	INT						GetProficiency()		{ return m_nProficiency; }
	INT						GetMaxLevel()			{ return m_pProto->nMaxLevel; }
	INT						GetMaxLearnLevel()		{ return m_pProto->nMaxLearnLevel; }
	INT						GetCoolDownCountDown()	{ return m_nCoolDownCountDown; }
	ESkillType				GetType()				{ return m_pProto->eType; }
	ESkillTypeEx			GetTypeEx()				{ return (ESkillTypeEx)m_pProto->nType2; }
	ESkillTypeEx2			GetTypeEx2()			{ return (ESkillTypeEx2)m_pProto->nType3; }
	ESkillUseType			GetUseType()			{ return m_pProto->eUseType; }
	ESkillDmgType			GetDmgType()			{ return m_pProto->eDmgType; }
	ESkillDistType			GetDistType()			{ return m_pProto->eDistType; }
	ESkillTargetType		GetTargetType()			{ return m_pProto->eTargetType; }
	DWORD					GetTargetSkillID()		{ return m_pProto->dwTargetSkillID; }
	DWORD					GetTargetBuffID()		{ return m_pProto->dwTargetBuffID; }
	DWORD					GetTargetTriggerID()	{ return m_pProto->dwTargetTriggerID; }
	ESkillOPType			GetOPType()				{ return m_pProto->eOPType; }
	INT						GetDmgTimes()			{ return m_pProto->nDmgTimes; }
	ETalentType				GetTalentType()			{ return m_pProto->eTalentType; }
	INT						GetLevelUpExp()			{ return m_pProto->nLevelUpExp; }
	INT						GetLevelUpType()		{ return m_pProto->eLevelUpType; }

	INT						GetDmg(INT nIndex);
	INT						GetDmgTime(INT nIndex);
	FLOAT					GetOPDist();
	FLOAT					GetOPRadius();
	INT						GetPrepareTime();
	INT						GetCoolDown();
	FLOAT					GetHit();
	FLOAT					GetCrit();
	INT						GetCost(ESkillCostType eCostType);
	VOID					GetCostItem(DWORD& dwItemID, INT& nNum);
	DWORD					GetBuffTypeID(INT nIndex);
	DWORD					GetTriggerID(INT nIndex);
	INT						GetEnmity();
	const SkillScript*		GetSkillScript()		{ return m_pScript; }

	const tagSkillProto*	GetProto()				{ return m_pProto; }
	const tagSkillMod*		GetMod()				{ return m_pMod; }
	const Unit*				GetOwner()				{ return m_pOwner; }

	BOOL					IsAffected()			{ return P_VALID(m_pMod); }
	BOOL					IsActive()				{ return ESUT_Active == GetUseType(); }
	BOOL					IsPassive()				{ return ESUT_Passive == GetUseType(); }
	BOOL					IsOrdAttackSkill()		{ return ESDGT_Ordinary == GetDmgType(); }
	BOOL					IsExAttackSkill()		{ ESkillDmgType eType = GetDmgType(); return ( ESDGT_Bleeding == eType || ESDGT_Brunt == eType || ESDGT_Bang == eType || ESDGT_Ordinary == eType ); }
	BOOL					IsInAttackSkill()		{ ESkillDmgType eType = GetDmgType(); return ( ESDGT_Poison == eType || ESDGT_Thinker == eType || ESDGT_Injury == eType ); }
	BOOL					IsStuntSkill()			{ return ESDGT_Stunt == GetDmgType(); }
	BOOL					IsMelee()				{ return ESDT_Melee == GetDistType(); }
	BOOL					IsRanged()				{ return ESDT_Ranged == GetDistType(); }
	BOOL					IsInner()				{ return ESDT_Inner == GetDistType(); }
	BOOL					IsFriendly()			{ return m_pProto->bFriendly; }
	BOOL					IsHostile()				{ return m_pProto->bHostile; }
	BOOL					IsInDependent()			{ return m_pProto->bIndependent; }
	BOOL					IsMoveable()			{ return m_pProto->bMoveable; }
	BOOL					IsCanModAtt();

	//--------------------------------------------------------------------------------
	// ����Set
	//--------------------------------------------------------------------------------
	VOID					SetOwner(Unit* pOwner)		{ m_pOwner = pOwner; }
	VOID					AddProficiency(INT nValue)	{ m_nProficiency += nValue; m_bNeedSave = TRUE;}
	VOID					SetProficiency(INT nValue)	{ m_nProficiency = nValue; m_bNeedSave = TRUE;}

	//--------------------------------------------------------------------------------
	// ��������ӳ�
	//--------------------------------------------------------------------------------
	BOOL					SetMod(const tagSkillProto* pProto);
	BOOL					UnSetMod(const tagSkillProto* pProto);

	//--------------------------------------------------------------------------------
	// �������Զ���������Ӱ��
	//--------------------------------------------------------------------------------
	VOID					SetOwnerMod();
	VOID					UnsetOwnerMod();

	//---------------------------------------------------------------------------------
	// ��������������
	//---------------------------------------------------------------------------------
	VOID					IncLevel(ESkillLevelChange eType, INT nInc=1);
	VOID					DecLevel(ESkillLevelChange eType, INT nDec=1);

	//--------------------------------------------------------------------------------
	// ��ȴ
	//--------------------------------------------------------------------------------
	VOID					StartCoolDown()		{ m_nCoolDownCountDown = GetCoolDown(); m_bNeedSave = TRUE;}
	VOID					ClearCoolDown()		{ m_nCoolDownCountDown = 0; m_bNeedSave = TRUE;}
	BOOL					CountDownCoolDown();	

private:
	//--------------------------------------------------------------------------
	// ����ĳЩ����buff
	//--------------------------------------------------------------------------
	VOID SetSkillBuff(const tagSkillProto* pProto);

	//--------------------------------------------------------------------------
	// ����ĳЩ����buff
	//--------------------------------------------------------------------------
	VOID UnSetSkillBuff(const tagSkillProto* pProto);

	//--------------------------------------------------------------------------
	// ע�Ἴ�ܱ���Ĵ���������
	//--------------------------------------------------------------------------
	VOID RegisterTriggerEvent();

	//--------------------------------------------------------------------------
	// ��ע�Ἴ�ܱ���Ĵ���������
	//--------------------------------------------------------------------------
	VOID UnRegisterTriggerEvent();


private:
	DWORD					m_dwID;						// ����ID����ID��
	INT						m_nLevel;					// ���ܵ�ǰ�ȼ������õȼ�+Ͷ�ŵȼ�+��ʱ�ȼ���

	INT						m_nLearnLevel;				// ���ܵ�ǰͶ�ŵȼ�
	INT						m_nSelfLevel;				// �������õȼ�
	INT						m_nTempAddLevel;			// ��ʱ���ϵĵȼ�						
	INT						m_nProficiency;				// ����������

	INT						m_nCoolDownCountDown;		// ������ȴ

	const SkillScript*		m_pScript;					// ���ܽű�
	const tagSkillProto*	m_pProto;					// ���ܾ�̬����
	mutable tagSkillMod*	m_pMod;						// ��������Ӱ�죨���û���κμ���Ӱ��������ֵΪNULL��
	Unit*					m_pOwner;					// ����������

	BOOL					m_bNeedSave;				// ��Ҫ����
};

//---------------------------------------------------------------------------------
// ���캯��
//---------------------------------------------------------------------------------
inline Skill::Skill() 
:	m_dwID(GT_INVALID), m_nLevel(GT_INVALID), m_nLearnLevel(GT_INVALID), 
	m_nSelfLevel(GT_INVALID), m_nTempAddLevel(GT_INVALID), m_nCoolDownCountDown(GT_INVALID),
	m_pProto(NULL), m_pMod(NULL), m_pOwner(NULL),m_nProficiency(GT_INVALID), m_pScript(NULL), m_bNeedSave(FALSE)
{
}

inline Skill::Skill(DWORD dwID, INT nSelfLevel, INT nLearnLevel, INT nCoolDown, INT nProficiency)
:	m_dwID(GT_INVALID), m_nLevel(GT_INVALID), m_nLearnLevel(GT_INVALID), 
	m_nSelfLevel(GT_INVALID), m_nTempAddLevel(GT_INVALID), m_nCoolDownCountDown(GT_INVALID),
	m_pProto(NULL), m_pMod(NULL), m_pOwner(NULL),m_nProficiency(GT_INVALID), m_pScript(NULL)
{
	Init(dwID, nSelfLevel, nLearnLevel, nCoolDown, nProficiency);
}

//---------------------------------------------------------------------------------
// ����������mod����Ҫ�ı䣬���ü���ԭ��buffҪ���⴦��һ�£�
//---------------------------------------------------------------------------------
inline VOID Skill::IncLevel(ESkillLevelChange eType, INT nInc)
{
	ASSERT( nInc > 0 );

	if( ESLC_Learn == eType )		m_nLearnLevel += nInc;
	else if( ESLC_Self == eType )	m_nSelfLevel += nInc;
	else if( ESLC_Temp == eType )	m_nTempAddLevel += nInc;

	INT nNewLevel = min(m_pProto->nMaxLevel, m_nLearnLevel + m_nSelfLevel + m_nTempAddLevel);
	if( nNewLevel == m_nLevel ) return;

	const tagSkillProto* pNewProto = g_attRes.GetSkillProto(CreateTypeID(m_dwID, nNewLevel));
	ASSERT( P_VALID(pNewProto) );

	// ȥ���õȼ���ԭ��buff
	UnSetSkillBuff(m_pProto);

	m_nLevel = nNewLevel;
	m_pProto = pNewProto;

	// �����µȼ���buff
	SetSkillBuff(m_pProto);
}

//-----------------------------------------------------------------------------------------------
// ���ܽ�����mod����Ҫ�ı䣬���ü���ԭ��buffҪ���⴦��һ�£�
//-----------------------------------------------------------------------------------------------
inline VOID Skill::DecLevel(ESkillLevelChange eType, INT nDec)
{
	ASSERT( nDec > 0 );

	if( ESLC_Learn == eType )		{ m_nLearnLevel -= nDec;	ASSERT(m_nLearnLevel >= 0); }
	else if( ESLC_Self == eType )	{ m_nSelfLevel -= nDec;		ASSERT(m_nSelfLevel >= 0); }
	else if( ESLC_Temp == eType )	{ m_nTempAddLevel -= nDec;	ASSERT( m_nTempAddLevel >= 0 ); }

	INT nNewLevel = min(m_pProto->nMaxLevel, m_nLearnLevel + m_nSelfLevel + m_nTempAddLevel);
	if( nNewLevel == m_nLevel ) return;

	const tagSkillProto* pNewProto = g_attRes.GetSkillProto(CreateTypeID(m_dwID, nNewLevel));
	ASSERT( P_VALID(pNewProto) );

	// ȥ��ԭ���ȼ���buff
	UnSetSkillBuff(m_pProto);

	m_nLevel = nNewLevel;
	m_pProto = pNewProto;

	// �����µȼ���buff
	SetSkillBuff(m_pProto);
}

//---------------------------------------------------------------------------------------------
// ��ȴ����ʱ
//---------------------------------------------------------------------------------------------
inline BOOL Skill::CountDownCoolDown()
{
	m_nCoolDownCountDown -= TICK_TIME;

	if( m_nCoolDownCountDown <= 0 )
	{
		m_nCoolDownCountDown = 0;
		m_bNeedSave = TRUE;
		return TRUE;
	}
	// ����5������Ҫ����
	else if(m_nCoolDownCountDown % (TICK_TIME*5) == 0)
		m_bNeedSave = TRUE;

	return FALSE;
}

//---------------------------------------------------------------------------------------------
// �Ƿ��ǿ���Ӱ���������Եļ���
//---------------------------------------------------------------------------------------------
inline BOOL Skill::IsCanModAtt()
{
	ESkillTargetType eTargetType = m_pProto->eTargetType;
	return IsPassive() && ESTT_Skill != eTargetType && ESTT_Buff != eTargetType && ESTT_Trigger != eTargetType;
}

//---------------------------------------------------------------------------------
// �õ�ĳһ���������˺�ֵ
//---------------------------------------------------------------------------------
inline INT Skill::GetDmg(INT nIndex)
{
	if( nIndex < 0 && nIndex >= MAX_CHANNEL_TIMES ) return 0;

	INT nDmg = m_pProto->nChannelDmg[nIndex];

	if( P_VALID(m_pMod) )
	{
		nDmg = nDmg + m_pMod->nChannelDmgMod[nIndex] + 
			INT((FLOAT)nDmg * ((FLOAT)m_pMod->nChannelDmgModPct[nIndex] / 10000.0f));
	}

	return nDmg;
}

//---------------------------------------------------------------------------------
// �õ�ĳһ��������Ӧ���˺�ʱ�䣨���룩
//---------------------------------------------------------------------------------
inline INT Skill::GetDmgTime(INT nIndex)
{
	ASSERT( nIndex >=0 && nIndex < MAX_CHANNEL_TIMES );
	return m_pProto->nChannelTime[nIndex];
}

//---------------------------------------------------------------------------------
// �õ���������
//---------------------------------------------------------------------------------
inline FLOAT Skill::GetOPDist()
{
	return m_pProto->fOPDist + ( P_VALID(m_pMod) ? m_pMod->fOPDistMod : 0 );
}

//---------------------------------------------------------------------------------
// �õ������뾶
//---------------------------------------------------------------------------------
inline FLOAT Skill::GetOPRadius()
{
	return m_pProto->fOPRadius + ( P_VALID(m_pMod) ? m_pMod->fOPRadiusMod : 0 );
}

//----------------------------------------------------------------------------------
// �õ�����ʱ��
//----------------------------------------------------------------------------------
inline INT Skill::GetPrepareTime()
{
	return m_pProto->nPrepareTime + ( P_VALID(m_pMod) ? m_pMod->nPrepareTimeMod : 0 );
}

//----------------------------------------------------------------------------------
// �õ���ȴʱ��
//----------------------------------------------------------------------------------
inline INT Skill::GetCoolDown()
{
	return m_pProto->nCoolDown + ( P_VALID(m_pMod) ? m_pMod->nCoolDownMod : 0 );
}

//----------------------------------------------------------------------------------
// �õ�����������
//----------------------------------------------------------------------------------
inline FLOAT Skill::GetHit()
{
	INT nHit = m_pProto->nHit;

	if( P_VALID(m_pMod) )
	{
		nHit = nHit + m_pMod->nHitMod + INT((FLOAT)nHit * ((FLOAT)m_pMod->nHitModPct / 10000.0f));
	}
	return (FLOAT)nHit / 10000.0f;
}

//----------------------------------------------------------------------------------
// �õ�����������
//----------------------------------------------------------------------------------
inline FLOAT Skill::GetCrit()
{
	INT nCrit = m_pProto->nCrit;

	if( P_VALID(m_pMod) )
	{
		nCrit = nCrit + m_pMod->nCritMod + INT((FLOAT)nCrit * ((FLOAT)m_pMod->nCritModPct / 10000.0f));
	}
	return (FLOAT)nCrit / 10000.0f;
}

//----------------------------------------------------------------------------------
// �õ����ܳ��
//----------------------------------------------------------------------------------
inline INT Skill::GetEnmity()
{
	INT nEnmity = m_pProto->nEnmity;

	if(P_VALID(m_pMod))
	{
		nEnmity = nEnmity + m_pMod->nEnmityMod + INT((FLOAT)nEnmity * ((FLOAT)m_pMod->nEnmityModPct / 10000.0f));
	}
	return nEnmity;
}


//---------------------------------------------------------------------------------------
// �õ���Ʒ����
//---------------------------------------------------------------------------------------
inline VOID Skill::GetCostItem(DWORD& dwItemID, INT& nNum)
{
	dwItemID = m_pProto->dwCostItemID;
	nNum = m_pProto->nCostItemNum;
}

//---------------------------------------------------------------------------------------
// �õ�ĳ��������Ӧ��BuffID
//---------------------------------------------------------------------------------------
inline DWORD Skill::GetBuffTypeID(INT nIndex)
{
	if( nIndex < 0 || nIndex >= MAX_BUFF_PER_SKILL )
		return GT_INVALID;

	if( P_VALID(m_pMod) )	return m_pMod->dwBuffTypeID[nIndex];
	else					return m_pProto->dwBuffID[nIndex];
}

//---------------------------------------------------------------------------------------
// �õ�ĳ��������Ӧ��TriggerID
//---------------------------------------------------------------------------------------
inline DWORD Skill::GetTriggerID(INT nIndex)
{
	if( nIndex < 0 || nIndex >= MAX_BUFF_PER_SKILL )
		return GT_INVALID;

	if( P_VALID(m_pMod) )	return m_pMod->dwTriggerID[nIndex];
	else					return m_pProto->dwTriggerID[nIndex];
}

//---------------------------------------------------------------------------------------
// �������������Buff
//---------------------------------------------------------------------------------------
inline VOID Skill::SetSkillBuff(const tagSkillProto* pProto)
{
	if( !P_VALID(m_pMod) ) return;

	UnRegisterTriggerEvent();

	// ���Ը������һ��buff
	for(INT n = 0; n < MAX_BUFF_PER_SKILL; n++)
	{
		if( FALSE == GT_VALID(pProto->dwBuffID[n]) )
			continue;

		for(INT m = 0; m < MAX_BUFF_PER_SKILL; m++)
		{
			if( P_VALID(m_pMod->dwBuffTypeID[m]) ) continue;
			m_pMod->dwBuffTypeID[m] = pProto->dwBuffID[n];
			m_pMod->dwTriggerID[m] = pProto->dwTriggerID[n];
			break;
		}
	}

	RegisterTriggerEvent();
}

//---------------------------------------------------------------------------------------
// �������������ĳЩBuff
//---------------------------------------------------------------------------------------
inline VOID Skill::UnSetSkillBuff(const tagSkillProto* pProto)
{
	if( !P_VALID(m_pMod) ) return;

	UnRegisterTriggerEvent();

	for(INT n = 0; n < MAX_BUFF_PER_SKILL; n++)
	{
		if( FALSE == GT_VALID(pProto->dwBuffID[n]) ) continue;

		for(INT m = 0; m < MAX_BUFF_PER_SKILL; m++)
		{
			if( m_pMod->dwBuffTypeID[m] != pProto->dwBuffID[n] )
				continue;

			m_pMod->dwBuffTypeID[m] = GT_INVALID;
			m_pMod->dwTriggerID[m] = GT_INVALID;
			break;
		}
	}

	RegisterTriggerEvent();
}
