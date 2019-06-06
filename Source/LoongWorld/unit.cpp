//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݽṹ
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/base_define.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/buff_define.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/action.h"
#include "../WorldDefine/move_define.h"
#include "../WorldDefine/remote_role.h"
#include "world.h"
#include "role.h"
#include "title_mgr.h"
#include "creature.h"
#include "creature_ai.h"
#include "map.h"
#include "map_creator.h"
#include "map_mgr.h"
#include "att_res.h"
#include "buff.h"
#include "buff_effect.h"
#include "unit.h"
#include "group_mgr.h"

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
Unit::Unit(DWORD dwID, DWORD dwMapID, FLOAT fX, FLOAT fY, FLOAT fZ, FLOAT fFaceX, FLOAT fFaceY, FLOAT fFaceZ)
: m_dwID(dwID), m_Size(X_DEF_ROLE_SIZE_X, X_DEF_ROLE_SIZE_Y, X_DEF_ROLE_SIZE_Z), m_fXZSpeed(X_DEF_XZ_SPEED), 
  m_fYSpeed(X_DEF_Y_SPEED), m_fSwimXZSpeed(X_DEF_XZ_SPEED), m_fDropXZSpeed(X_DEF_DROP_XZ_SPEED), m_fSlideSpeed(X_DEF_SLIDE_SPEED), 
  m_dwMapID(dwMapID), m_pMap(NULL), m_dwInstanceID(GT_INVALID), m_bNeedRecal(FALSE), m_StateMgr(), m_nHPMPRegainTickCountDown(HP_MP_REGAIN_INTER_TICK),
  m_nVitalityRegainTickCountDown(VITALITY_REGAIN_INTER_TICK), m_nEnduranceRegainTickCountDown(ENDURANCE_REGAIN_INTER_TICK), m_nProduceSkillNum(0),
  m_nInjuryRegainTickCountDown(INJURY_REGAIN_INTER_TICK)
{
	ZeroMemory(m_nAtt, sizeof(m_nAtt));
	ZeroMemory(m_nBaseAtt, sizeof(m_nBaseAtt));
	ZeroMemory(m_nAttMod, sizeof(m_nAttMod));
	ZeroMemory(m_nAttModPct, sizeof(m_nAttModPct));
	ZeroMemory(m_bAttRecalFlag, sizeof(m_bAttRecalFlag));

	m_MoveData.Init(this, fX, fY, fZ, fFaceX, fFaceY, fFaceZ);
	m_CombatHandler.Init(this);
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
Unit::~Unit()
{
	// ɾ�����м���
	TMap<DWORD, Skill*>::TMapIterator itSkill = m_mapSkill.Begin();
	Skill* pSkill = NULL;

	while( m_mapSkill.PeekNext(itSkill, pSkill) )
	{
		SAFE_DEL(pSkill);
	}
	m_mapSkill.Clear();

	// ɾ�����е�buff�޸���
	TList<DWORD>* pListModifier = NULL;
	TMap<DWORD, TList<DWORD>*>::TMapIterator itModifier = m_mapBuffModifier.Begin();
	while( m_mapBuffModifier.PeekNext(itModifier, pListModifier) )
	{
		SAFE_DEL(pListModifier);
	}
	m_mapBuffModifier.Clear();

	// ɾ������trigger�޸���
	itModifier = m_mapTriggerModifier.Begin();
	while( m_mapTriggerModifier.PeekNext(itModifier, pListModifier) )
	{
		SAFE_DEL(pListModifier);
	}
	m_mapTriggerModifier.Clear();
}

//-------------------------------------------------------------------------------
// ������ʱ����µĺ���
//-------------------------------------------------------------------------------
VOID Unit::UpdateTimedIssue()
{
	if( IsDead() ) return;

	// ��Ѫ����
	if( --m_nHPMPRegainTickCountDown <= 0 )
	{
		m_nHPMPRegainTickCountDown = HP_MP_REGAIN_INTER_TICK;

		if( GetAttValue(ERA_HPRegainRate) != 0 )
		{
			ChangeHP(GetAttValue(ERA_HPRegainRate), this);
		}
		if( GetAttValue(ERA_MPRegainRate) != 0 )
		{
			ChangeMP(GetAttValue(ERA_MPRegainRate));
		}
	}

	// �ػ���
	if( --m_nVitalityRegainTickCountDown <= 0 )
	{
		m_nVitalityRegainTickCountDown = VITALITY_REGAIN_INTER_TICK;

		if( GetAttValue(ERA_VitalityRegainRate) != 0 )
		{
			ChangeVitality(GetAttValue(ERA_VitalityRegainRate));
		}
	}

	// �س־���
	if( --m_nEnduranceRegainTickCountDown <= 0 )
	{
		m_nEnduranceRegainTickCountDown = ENDURANCE_REGAIN_INTER_TICK;

		ChangeEndurance(5);
	}

	// ������
	if( --m_nInjuryRegainTickCountDown <= 0 )
	{
		m_nInjuryRegainTickCountDown = INJURY_REGAIN_INTER_TICK;
		if (GetAttValue(ERA_Injury) > 0)
		{
			ModAttValue(ERA_Injury, INJURY_INCREASE_VAL);
		}		
	}

	// ������һЩ��ʱ��仯��

	if( IsRole() )
	{
		Role* pRole = static_cast<Role *>(this);
		ASSERT(P_VALID(pRole));

		// ���н�ɫ�ص���
		if( pRole->IsInRoleState(ERS_PrisonArea) && --m_nInPrisonMoraleRegainTickCountDown <= 0 )
		{
			m_nInPrisonMoraleRegainTickCountDown = INPRISON_MORALE_REGAIN_INTER_TICK;

			INT nCurMorale = pRole->GetAttValue(ERA_Morality);
			INT nNeed = 0;
			if (nCurMorale < 0)
			{
				nNeed = 0 - nCurMorale;
				if (nNeed > INPRISON_MORAL_INCREASE_VAL)
				{
					nNeed = INPRISON_MORAL_INCREASE_VAL;
				}
			}
			else
			{
				DWORD	dwPrisonMapID	= g_mapCreator.GetPrisonMapID();
				Vector3	PutOutPoint		= g_mapCreator.GetPutOutPrisonPoint();

				pRole->GotoNewMap(dwPrisonMapID, PutOutPoint.x, PutOutPoint.y, PutOutPoint.z);
			}

			ModAttValue(ERA_Morality, nNeed);
			return;
		}	

		// �����������ڣ�����ֵÿ15����+1
		if( !pRole->IsInRoleState(ERS_PrisonArea) && --m_nOutPrisonMoraleRegainTickCountDown <= 0 )
		{
			m_nOutPrisonMoraleRegainTickCountDown = 15 * INPRISON_MORALE_REGAIN_INTER_TICK;
			INT nCurMorale = pRole->GetAttValue(ERA_Morality);
			if (nCurMorale < 0)
			{
				ModAttValue(ERA_Morality, 1);
			}
		}
	}
}

//--------------------------------------------------------------------------------
// ���������־λ���е�ǰ��������
//--------------------------------------------------------------------------------
VOID Unit::RecalAtt(BOOL bSendMsg)
{
	bool bAttARecal[X_ERA_ATTA_NUM] = {false};		// һ�������Ƿ����������
	bool bAttBRecal[X_ERA_AttB_NUM] = {false};		// ���������Ƿ����������

	// ���Ƚ�ĳЩ����ǰ���ԡ���ȡ����
	INT nHP				=	m_nAtt[ERA_HP];
	INT nMP				=	m_nAtt[ERA_MP];
	INT nVitality		=	m_nAtt[ERA_Vitality];
	INT nEndurance		=	m_nAtt[ERA_Endurance];
	INT nKnowledge		=	m_nAtt[ERA_Knowledge];
	INT nInjury			=	m_nAtt[ERA_Injury];
	INT nMorale			=	m_nAtt[ERA_Morale];
	INT nMorality		=	m_nAtt[ERA_Morality];
	INT nCulture		=	m_nAtt[ERA_Culture];
	INT nAttPoint		=	m_nAtt[ERA_AttPoint];
	INT nTalentPoint	=	m_nAtt[ERA_TalentPoint];

	// ��ʼ����
	for(INT n = 0; n < ERA_End; n++)
	{
		if( false == GetAttRecalFlag(n) )
			continue;

		// ��������Ҫ���¼���
		m_nAtt[n] = CalAttMod(m_nBaseAtt[n], n);

		if( n >= ERA_AttA_Start && n <= ERA_AttA_End )
		{
			bAttARecal[MTransERAAttA2Index(n)] = true;
		}
		else if( n >= ERA_AttB_Start && n < ERA_AttB_End )
		{
			bAttBRecal[MTransERAAttB2Index(n)] = true;
		}
	}


	// �����Ҫ���¼������Ե�Ϊ�������Ҫ�ж�һ�����ԺͶ�������֮��Ĺ���
	if( IsRole() )
	{
		// ���Ȳ鿴�Ƿ���һ�����Խ��������¼���
		for(INT n = 0; n < X_ERA_ATTA_NUM; ++n)
		{
			if( !bAttARecal[n] ) continue;

				// �õ����ĸ�һ������
			ERoleAttribute eType = (ERoleAttribute)MTransIndex2ERAATTA(n);

			switch(eType)
			{
				// ��ǣ���������Ե�����������⹦����Ҫ���¼���
			case ERA_Physique:
				{
					// �����������
					m_nAtt[ERA_MaxHP] = m_nAtt[ERA_Physique] * 10 + m_nBaseAtt[ERA_MaxHP];
					m_nAtt[ERA_MaxHP] = CalAttMod(m_nAtt[ERA_MaxHP], ERA_MaxHP);

					// �����⹦����
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 5 + m_nAtt[ERA_Strength];
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);

					// �Ѿ����¼�����Ķ�����������Ͳ���Ҫ���¼�����
					bAttBRecal[MTransERAAttB2Index(ERA_MaxHP)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_ExDefense)] = false;

					// �����Щ�������Ա�������Ҫ���¼��㣬�������ϣ�����Ϊ����Ϣ�Ĳο�
					if( !m_bAttRecalFlag[ERA_MaxHP] ) m_bAttRecalFlag[ERA_MaxHP] = true;
					if( !m_bAttRecalFlag[ERA_ExDefense] ) m_bAttRecalFlag[ERA_ExDefense] = true;
				}
				break;

				// ��������������Ե��⹦�������⹦�����ͳ־�����Ҫ���¼���
			case ERA_Strength:
				{
					// �����⹦����
					m_nAtt[ERA_ExAttack] = m_nAtt[ERA_Strength] * 5;
					m_nAtt[ERA_ExAttack] = CalAttMod(m_nAtt[ERA_ExAttack], ERA_ExAttack);

					// �����⹦����
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 5 + m_nAtt[ERA_Strength];
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);

					// �Ѿ����¼�����Ķ�����������Ͳ���Ҫ���¼�����
					bAttBRecal[MTransERAAttB2Index(ERA_ExAttack)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_ExDefense)] = false;

					// �����Щ�������Ա�������Ҫ���¼��㣬�������ϣ�����Ϊ����Ϣ�Ĳο�
					if( !m_bAttRecalFlag[ERA_ExAttack] ) m_bAttRecalFlag[ERA_ExAttack] = true;
					if( !m_bAttRecalFlag[ERA_ExDefense] ) m_bAttRecalFlag[ERA_ExDefense] = true;
				}
				break;

				// Ԫ������������Ե�����������ڹ�������Ҫ���¼���
			case ERA_Pneuma:
				{
					// �����������
					m_nAtt[ERA_MaxMP] = m_nAtt[ERA_Pneuma] * 10 + m_nBaseAtt[ERA_MaxMP];
					m_nAtt[ERA_MaxMP] = CalAttMod(m_nAtt[ERA_MaxMP], ERA_MaxMP);

					// �����ڹ�����
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Pneuma] * 5 + m_nAtt[ERA_InnerForce];
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);

					// �Ѿ����¼�����Ķ�����������Ͳ���Ҫ���¼�����
					bAttBRecal[MTransERAAttB2Index(ERA_MaxMP)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_InDefense)] = false;

					// �����Щ�������Ա�������Ҫ���¼��㣬�������ϣ�����Ϊ����Ϣ�Ĳο�
					if( !m_bAttRecalFlag[ERA_MaxMP] ) m_bAttRecalFlag[ERA_MaxMP] = true;
					if( !m_bAttRecalFlag[ERA_InDefense] ) m_bAttRecalFlag[ERA_InDefense] = true;
				}
				break;

				// ��������������Ե��ڹ��������ڹ������ͳ־���Ҫ���¼���
			case ERA_InnerForce:
				{
					// �����ڹ�����
					m_nAtt[ERA_InAttack] = m_nAtt[ERA_InnerForce] * 5;
					m_nAtt[ERA_InAttack] = CalAttMod(m_nAtt[ERA_InAttack], ERA_InAttack);

					// �����ڹ�����
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Pneuma] * 5 + m_nAtt[ERA_InnerForce];
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);

					// �Ѿ����¼�����Ķ�����������Ͳ���Ҫ���¼�����
					bAttBRecal[MTransERAAttB2Index(ERA_InAttack)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_InDefense)] = false;

					// �����Щ�������Ա�������Ҫ���¼��㣬�������ϣ�����Ϊ����Ϣ�Ĳο�
					if( !m_bAttRecalFlag[ERA_InAttack] ) m_bAttRecalFlag[ERA_InAttack] = true;
					if( !m_bAttRecalFlag[ERA_InDefense] ) m_bAttRecalFlag[ERA_InDefense] = true;
				}
				break;

				// ����������������еĹ������ɺ�������Ҫ���¼���
			case ERA_Technique:
				{
					// ���㹥������
					m_nAtt[ERA_AttackTec] = m_nAtt[ERA_Technique] * 6;
					m_nAtt[ERA_AttackTec] = CalAttMod(m_nAtt[ERA_AttackTec], ERA_AttackTec);

					// ��������
					m_nAtt[ERA_HitRate]	= m_nAtt[ERA_Technique] * 10;
					m_nAtt[ERA_HitRate] = CalAttMod(m_nAtt[ERA_HitRate], ERA_HitRate);

					// ����־���
					m_nAtt[ERA_MaxEndurance] = (m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
					m_nAtt[ERA_MaxEndurance] = CalAttMod(m_nAtt[ERA_MaxEndurance], ERA_MaxEndurance);

					// �Ѿ����¼�����Ķ�����������Ͳ���Ҫ���¼�����
					bAttBRecal[MTransERAAttB2Index(ERA_AttackTec)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_HitRate)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_MaxEndurance)] = false;

					// �����Щ�������Ա�������Ҫ���¼��㣬�������ϣ�����Ϊ����Ϣ�Ĳο�
					if( !m_bAttRecalFlag[ERA_AttackTec] ) m_bAttRecalFlag[ERA_AttackTec] = true;
					if( !m_bAttRecalFlag[ERA_HitRate] ) m_bAttRecalFlag[ERA_HitRate] = true;
					if( !m_bAttRecalFlag[ERA_MaxEndurance] ) m_bAttRecalFlag[ERA_MaxEndurance] = true;
				}
				break;

				// ��������������еķ������ɺ�������Ҫ���¼���
			case ERA_Agility:
				{
					// �����������
					m_nAtt[ERA_DefenseTec] = m_nAtt[ERA_Agility] * 4;
					m_nAtt[ERA_DefenseTec] = CalAttMod(m_nAtt[ERA_DefenseTec], ERA_DefenseTec);

					// ��������
					m_nAtt[ERA_Dodge] =	m_nAtt[ERA_Agility] * 10;
					m_nAtt[ERA_Dodge] = CalAttMod(m_nAtt[ERA_Dodge], ERA_Dodge);

					// ����־���
					m_nAtt[ERA_MaxEndurance] = (m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
					m_nAtt[ERA_MaxEndurance] = CalAttMod(m_nAtt[ERA_MaxEndurance], ERA_MaxEndurance);

					// �Ѿ����¼�����Ķ�����������Ͳ���Ҫ���¼�����
					bAttBRecal[MTransERAAttB2Index(ERA_DefenseTec)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_Dodge)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_MaxEndurance)] = false;

					// �����Щ�������Ա�������Ҫ���¼��㣬�������ϣ�����Ϊ����Ϣ�Ĳο�
					if( !m_bAttRecalFlag[ERA_DefenseTec] ) m_bAttRecalFlag[ERA_DefenseTec] = true;
					if( !m_bAttRecalFlag[ERA_Dodge] ) m_bAttRecalFlag[ERA_Dodge] = true;
					if( !m_bAttRecalFlag[ERA_MaxEndurance] ) m_bAttRecalFlag[ERA_MaxEndurance] = true;
				}
				break;

			default:
				break;
			}
		}

		// ���жϼ�������Ƿ��ж���������Ҫ����
		for(INT n = 0; n < X_ERA_AttB_NUM; ++n)
		{
			if( !bAttBRecal[n] ) continue;

			// �õ����ĸ���������
			ERoleAttribute eType = (ERoleAttribute)MTransIndex2ERAATTB(n);

			switch(eType)
			{
				// �������
			case ERA_MaxHP:
				{
					// �����������
					m_nAtt[ERA_MaxHP] = m_nAtt[ERA_Physique] * 10 + m_nBaseAtt[ERA_MaxHP];
					m_nAtt[ERA_MaxHP] = CalAttMod(m_nAtt[ERA_MaxHP], ERA_MaxHP);
				}
				break;

				// �������
			case ERA_MaxMP:
				{
					// �����������
					m_nAtt[ERA_MaxMP] = m_nAtt[ERA_Pneuma] * 10 + m_nBaseAtt[ERA_MaxMP];
					m_nAtt[ERA_MaxMP] = CalAttMod(m_nAtt[ERA_MaxMP], ERA_MaxMP);
				}
				break;

				// �⹦����
			case ERA_ExAttack:
				{
					// �����⹦����
					m_nAtt[ERA_ExAttack] = m_nAtt[ERA_Strength] * 5;
					m_nAtt[ERA_ExAttack] = CalAttMod(m_nAtt[ERA_ExAttack], ERA_ExAttack);
				}
				break;

				// �⹦����
			case ERA_ExDefense:
				{
					// �����⹦����
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 5 + m_nAtt[ERA_Strength];
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);
				}
				break;

				// �ڹ�����
			case ERA_InAttack:
				{
					// �����ڹ�����
					m_nAtt[ERA_InAttack] = m_nAtt[ERA_InnerForce] * 5;
					m_nAtt[ERA_InAttack] = CalAttMod(m_nAtt[ERA_InAttack], ERA_InAttack);
				}
				break;

				// �ڹ�����
			case ERA_InDefense:
				{
					// �����ڹ�����
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Pneuma] * 5 + m_nAtt[ERA_InnerForce];
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);
				}
				break;

				// ��������
			case ERA_AttackTec:
				{
					// ���㹥������
					m_nAtt[ERA_AttackTec] = m_nAtt[ERA_Technique] * 6;
					m_nAtt[ERA_AttackTec] = CalAttMod(m_nAtt[ERA_AttackTec], ERA_AttackTec);
				}
				break;

				// ��������
			case ERA_DefenseTec:
				{
					// �����������
					m_nAtt[ERA_DefenseTec] = m_nAtt[ERA_Agility] * 4;
					m_nAtt[ERA_DefenseTec] = CalAttMod(m_nAtt[ERA_DefenseTec], ERA_DefenseTec);
				}
				break;

				// ����
			case ERA_HitRate:
				{
					// ��������
					m_nAtt[ERA_HitRate]	= m_nAtt[ERA_Technique] * 10;
					m_nAtt[ERA_HitRate] = CalAttMod(m_nAtt[ERA_HitRate], ERA_HitRate);
				}
				break;

				// ����
			case ERA_Dodge:
				{
					// ��������
					m_nAtt[ERA_Dodge] =	m_nAtt[ERA_Agility] * 10;
					m_nAtt[ERA_Dodge] = CalAttMod(m_nAtt[ERA_Dodge], ERA_Dodge);
				}
				break;

				// �־���
			case ERA_MaxEndurance:
				{
					// ����־���
					m_nAtt[ERA_MaxEndurance] = (m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
					m_nAtt[ERA_MaxEndurance] = CalAttMod(m_nAtt[ERA_MaxEndurance], ERA_MaxEndurance);
				}
				break;

			default:
				break;
			}
		}
	}

	// ����ԭ�ȱ��������
	m_nAtt[ERA_Knowledge]	=	nKnowledge;
	m_nAtt[ERA_Injury]		=	nInjury;
	m_nAtt[ERA_Morale]		=	nMorale;
	m_nAtt[ERA_Morality]	=	nMorality;
	m_nAtt[ERA_Culture]		=	nCulture;
	m_nAtt[ERA_AttPoint]	=	nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	nTalentPoint;

	// ��֮ǰ�����ĳЩ����ǰ���ԡ����Ӧ�ġ����ֵ��ȡ��Сֵ
	m_nAtt[ERA_HP] = min(nHP, m_nAtt[ERA_MaxHP]);
	m_nAtt[ERA_MP] = min(nMP, m_nAtt[ERA_MaxMP]);
	m_nAtt[ERA_Vitality] = min(nVitality, m_nAtt[ERA_MaxVitality]);
	m_nAtt[ERA_Endurance] = min(nEndurance, m_nAtt[ERA_MaxEndurance]);

	// �Ƚ�ԭ����HP���µ�HP֮���Ƿ�һ�£������һ�£���Ѫԭ������Ҫ���¼��㣬��������Ҫ���¼��㣬�Ա㷢��Ϣ
	if( m_nAtt[ERA_HP] != nHP && !m_bAttRecalFlag[ERA_HP] ) m_bAttRecalFlag[ERA_HP] = true;
	if( m_nAtt[ERA_MP] != nMP && !m_bAttRecalFlag[ERA_MP] ) m_bAttRecalFlag[ERA_MP] = true;
	if( m_nAtt[ERA_Vitality] != nVitality && !m_bAttRecalFlag[ERA_Vitality] ) m_bAttRecalFlag[ERA_Vitality] = true;
	if( m_nAtt[ERA_Endurance] != nEndurance && !m_bAttRecalFlag[ERA_Endurance] ) m_bAttRecalFlag[ERA_Endurance] = true;

	// �����Ҫ������Ϣ
	if( bSendMsg )
	{
		// ���Ȳ鿴�Ƿ���Ҫ�㲥��Զ�������Ҫ֪����һЩ���ԣ�
		BOOL bNeedBroadcast = FALSE;			// �Ƿ���Ҫ�㲥
		bool bRemoteRoleAtt[ERRA_End] = {false};// Զ���������
		INT nRemoteRoleAtt[ERRA_End] = {0};		// Զ���������
		INT nRoleAttNum = 0;					// ����������Ըı������
		INT nRemoteRoleAttNum = 0;				// Զ��������Ըı������

		// �������ϲ���
		for(INT n = 0; n < ERA_End; ++n)
		{
			if( m_bAttRecalFlag[n] )
			{
				++nRoleAttNum;
				ERemoteRoleAtt eType = ERA2ERRA((ERoleAttribute)n);
				if( eType != ERRA_Null )
				{
					++nRemoteRoleAttNum;
					bRemoteRoleAtt[eType] = true;
					nRemoteRoleAtt[eType] = m_nAtt[n];

					if( !bNeedBroadcast ) bNeedBroadcast = TRUE;
				}
			}
		}

		// ���Ȳ鿴�Ƿ���Ҫ�㲥
		if( bNeedBroadcast && nRemoteRoleAttNum > 0 )
		{
			DWORD dwSize = sizeof(tagNS_RemoteAttChangeMutiple) + sizeof(tagRemoteAttValue) * (nRemoteRoleAttNum - 1);
			MCREATE_MSG(pSend, dwSize, NS_RemoteAttChangeMutiple);

			pSend->dwSize = dwSize;
			pSend->dwRoleID = m_dwID;
			pSend->nNum = nRemoteRoleAttNum;

			INT nIndex = 0;
			for(INT n = 0; n < ERRA_End; ++n)
			{
				if( bRemoteRoleAtt[n] )
				{
					pSend->value[nIndex].eType = (ERemoteRoleAtt)n;
					pSend->value[nIndex].nValue = nRemoteRoleAtt[n];
					++nIndex;
				}
			}

			if( P_VALID(GetMap()) )
				GetMap()->SendBigVisTileMsg(this, pSend, pSend->dwSize);

			MDEL_MSG(pSend);
		}

		// ��������ﲢ����Ҫ���ͣ����͸��������
		if( IsRole() && nRoleAttNum > 0 )
		{
			Role* pRole = reinterpret_cast<Role*>(this);

			DWORD dwSize = sizeof(tagNS_RoleAttChangeMutiple) + sizeof(tagRoleAttValue) * (nRoleAttNum - 1);
			MCREATE_MSG(pSend, dwSize, NS_RoleAttChangeMutiple);

			pSend->dwSize = dwSize;
			pSend->nNum = nRoleAttNum;

			INT nIndex = 0;
			for(INT n = 0; n < ERA_End; ++n)
			{
				if( GetAttRecalFlag(n) )
				{
					pSend->value[nIndex].eType = (ERoleAttribute)n;
					pSend->value[nIndex].nValue = m_nAtt[n];
					++nIndex;
				}
			}

			pRole->SendMessage(pSend, pSend->dwSize);

			MDEL_MSG(pSend);
		}
	}

	// ����ĳЩ��Ϊ���Ըı��Ӱ���ֵ
	OnAttChange(m_bAttRecalFlag);

	// ��������־λ
	ClearAttRecalFlag();
}

//--------------------------------------------------------------------------------
// �������Լӳ�����ֵ����ȡ������
//--------------------------------------------------------------------------------
INT Unit::CalAttMod(INT nBase, INT nIndex)
{
	// ���nIndex�Ƿ�Ϸ�
	ASSERT( nIndex > ERRA_Null && nIndex < ERA_End );

	INT nValue = nBase + m_nAttMod[nIndex] + INT((FLOAT)nBase * (FLOAT(m_nAttModPct[nIndex]) / 10000.0f));

	if( nValue < g_attRes.GetAttMin(nIndex) ) nValue = g_attRes.GetAttMin(nIndex);
	if( nValue > g_attRes.GetAttMax(nIndex) ) nValue = g_attRes.GetAttMax(nIndex);

	return nValue;
}

//--------------------------------------------------------------------------------
// ����ĳ�����Ա仯���ͻ��ˣ���������Ա仯��Ҫ�㲥��㲥
//--------------------------------------------------------------------------------
VOID Unit::SendAttChange(INT nIndex)
{
	ASSERT( nIndex > ERA_Null && nIndex < ERA_End );

	ERemoteRoleAtt eRemote = ERA2ERRA((ERoleAttribute)nIndex);

	// ����
	if( IsCreature() )
	{
		if( ERRA_Null != eRemote )
		{
			// ����Զ��ͬ������
			tagNS_RemoteAttChangeSingle send;
			send.dwRoleID = GetID();
			send.eType = eRemote;
			send.nValue = m_nAtt[nIndex];

			if( GetMap() )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}
	// ���
	else if( IsRole() )
	{
		// ���ȷ��͸��Լ�
		Role* pRole = reinterpret_cast<Role*>(this);

		tagNS_RoleAttChangeSingle send;
		send.eType = (ERoleAttribute)nIndex;
		send.nValue = m_nAtt[nIndex];
		pRole->SendMessage(&send, send.dwSize);

		// �����Ҫͬ����Զ�����
		if( ERRA_Null != eRemote )
		{
			// ����Զ��ͬ������
			tagNS_RemoteAttChangeSingle send;
			send.dwRoleID = GetID();
			send.eType = eRemote;
			send.nValue = m_nAtt[nIndex];

			if( GetMap() )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
			
			// ͬ�����Ը�С�����
			if(pRole->GetTeamID() != GT_INVALID && IsTeamRemoteAtt(eRemote))
				g_groupMgr.SendTeamMesOutBigVis(pRole, pRole->GetTeamID(), &send, send.dwSize);
		}
	}
}

//-----------------------------------------------------------------------------------------
// �õ�����״̬��־λ
//-----------------------------------------------------------------------------------------
DWORD Unit::GetStateFlag()
{
	DWORD dwStateFlag = 0;

	dwStateFlag |=	( IsInState(ES_Dead)		?	ESF_Dead		:	ESF_NoDead );
	dwStateFlag |=	( IsInState(ES_Dizzy)		?	ESF_Dizzy		:	ESF_NoDizzy );
	dwStateFlag |=	( IsInState(ES_Spor)		?	ESF_Spor		:	ESF_NoSpor );
	dwStateFlag |=	( IsInState(ES_Tie)			?	ESF_Tie			:	ESF_NoTie );
	dwStateFlag |=	( IsInState(ES_Invincible)	?	ESF_Invincible	:	ESF_NoInvincible );
	dwStateFlag |=	( IsInState(ES_Lurk)		?	ESF_Lurk		:	ESF_NoLurk );
	dwStateFlag |=	( IsInState(ES_DisArm)		?	ESF_DisArm		:	ESF_NoDisArm );
	dwStateFlag |=	( IsInState(ES_NoSkill)		?	ESF_NoSkill		:	ESF_NoNoSkill );

	return dwStateFlag;
}


//-----------------------------------------------------------------------------
// ����ĳ�����ܵ�Ӱ��
//-----------------------------------------------------------------------------
VOID Unit::AddSkillMod(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;

	ESkillTargetType eTarget = pSkill->GetTargetType();

	switch(eTarget)
	{
		// Ӱ�켼��
	case ESTT_Skill:
		{
			Skill* pTargetSkill = m_mapSkill.Peek(pSkill->GetTargetSkillID());
			if( !P_VALID(pTargetSkill) ) return;

			if( pTargetSkill->SetMod(pSkill->GetProto()) && IsRole() )
			{
				Role* pRole = reinterpret_cast<Role*>(this);

				tagNS_UpdateSkill send;
				pTargetSkill->GenerateMsgInfo(&send.Skill);
				pRole->SendMessage(&send, send.dwSize);
			}
		}
		break;

		// Ӱ��Buff
	case ESTT_Buff:
		{
			RegisterBuffModifier(pSkill);
		}
		break;

		// Ӱ��Trigger
	case ESTT_Trigger:
		{
			RegisterTriggerModifier(pSkill);
		}
		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------
// ���㵱ǰ�����б��е����м��ܶ�ĳ�����ܵ�Ӱ��
//------------------------------------------------------------------------------
VOID Unit::AddSkillBeMod(Skill* pSkill)
{
	// �鿴��ǰ�����б����Ƿ������Ӱ��ü��ܵļ���
	tagSkillModify* pModify = g_attRes.GetSkillModifier(pSkill->GetID());

	if( !P_VALID(pModify) || pModify->listModify.Empty() )
		return;

	// ͨ���б���Ҽ����б�������Ӱ�켼��
	TList<DWORD>& list = pModify->listModify;

	TList<DWORD>::TListIterator it = list.Begin();

	Skill* pModSkill = NULL;
	DWORD dwModSkillID = GT_INVALID;
	while( list.PeekNext(it, dwModSkillID) )
	{
		pModSkill = m_mapSkill.Peek(dwModSkillID);
		if( !P_VALID(pModSkill) ) continue;

		// ����һ�����ܣ�����Ӱ��
		pSkill->SetMod(pModSkill->GetProto());
	}
}

//-----------------------------------------------------------------------------
// ȥ��ĳ�����ܵ�Ӱ��
//-----------------------------------------------------------------------------
VOID Unit::RemoveSkillMod(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;

	ESkillTargetType eTarget = pSkill->GetTargetType();

	switch(eTarget)
	{
		// Ӱ�켼��
	case ESTT_Skill:
		{
			Skill* pTargetSkill = m_mapSkill.Peek(pSkill->GetTargetSkillID());
			if( !P_VALID(pTargetSkill) ) return;

			if( pTargetSkill->UnSetMod(pSkill->GetProto()) && IsRole() )
			{
				Role* pRole = reinterpret_cast<Role*>(this);

				tagNS_UpdateSkill send;
				pTargetSkill->GenerateMsgInfo(&send.Skill);
				pRole->SendMessage(&send, send.dwSize);
			}
		}
		break;

		// Ӱ��Buff
	case ESTT_Buff:
		{
			UnRegisterBuffModifier(pSkill);
		}
		break;

		// Ӱ��Trigger
	case ESTT_Trigger:
		{
			UnRegisterTriggerModifier(pSkill);
		}
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------------
// ����һ������
//----------------------------------------------------------------------------------
VOID Unit::AddSkill(Skill* pSkill, BOOL bSendMsg)
{
	ASSERT( P_VALID(pSkill) );

	AddSkillMod(pSkill);
	AddSkillBeMod(pSkill);

	pSkill->SetOwner(this);
	m_mapSkill.Add(pSkill->GetID(), pSkill);
	if( pSkill->GetCoolDownCountDown() > 0 )
	{
		m_listSkillCoolDown.PushBack(pSkill->GetID());
	}

	pSkill->SetOwnerMod();

	// ������������ܣ����������е��������ܸ�����1
	if( ESSTE_Produce == pSkill->GetTypeEx() )
	{
		++m_nProduceSkillNum;
	}
}

//----------------------------------------------------------------------------------
// ȥ��һ������
//----------------------------------------------------------------------------------
VOID Unit::RemoveSkill(DWORD dwSkillID)
{
	Skill* pSkill = m_mapSkill.Peek(dwSkillID);
	if( !P_VALID(pSkill) ) return;

	// ������������ܣ����������е��������ܸ�����1
	if( ESSTE_Produce == pSkill->GetTypeEx() )
	{
		--m_nProduceSkillNum;
	}

	// ��������ɸü��ܲ���������buff
	RemoveAllBuffBelongSkill(pSkill->GetID(), FALSE);

	// ����Ӱ��
	pSkill->UnsetOwnerMod();
	m_mapSkill.Erase(dwSkillID);
	m_listSkillCoolDown.Erase(dwSkillID);

	pSkill->SetOwner(NULL);

	RemoveSkillMod(pSkill);	

	SAFE_DEL(pSkill);
}

//----------------------------------------------------------------------------------
// �����������򽵼�
//----------------------------------------------------------------------------------
VOID Unit::ChangeSkillLevel(Skill* pSkill, ESkillLevelChange eType, INT nChange)
{
	if( !P_VALID(pSkill) ) return;
	if( 0 == nChange ) return;

	// ������ȥ����������Ӱ�죬��ȥ������������Ӱ��
	pSkill->UnsetOwnerMod();
	RemoveSkillMod(pSkill);

	// �ı似�ܵȼ�
	if( nChange > 0 )
		pSkill->IncLevel(eType, nChange);
	else
		pSkill->DecLevel(eType, nChange);

	// ���¼�������Ӱ��
	AddSkillMod(pSkill);
	pSkill->SetOwnerMod();
}

//----------------------------------------------------------------------------------
// �ı似��������
//----------------------------------------------------------------------------------
VOID Unit::ChangeSkillExp(Skill *pSkill, INT nValue)
{
	if( !P_VALID(pSkill) )	return;
	if( nValue <= 0 )	return;
	if( ESLUT_Exp != pSkill->GetLevelUpType() ) return;
	if( pSkill->GetLevel() == pSkill->GetMaxLevel() ) return;
	
	// ������
	if ( IsRole() )
	{
		Role* pRole = static_cast<Role*>(this);
		FLOAT fEarnRate = pRole->GetEarnRate() / 10000.0f;
		nValue = INT(nValue * fEarnRate);
	}

	if( nValue <= 0 ) return;

	// �Ƿ�������
	BOOL bLevelChanged = FALSE;

	// �õ��ü��ܵ���������
	INT nRemainProficency = pSkill->GetLevelUpExp() - pSkill->GetProficiency();

	// ����þ���С����������
	if( nValue < nRemainProficency )
	{
		pSkill->AddProficiency(nValue);
	}
	// ����þ�����ڵ�����������
	else
	{
		// �鿴��Ҫ������
		while( nValue >= nRemainProficency )
		{
			// �Ѿ�������ߵȼ�
			if( pSkill->GetLevel() >= pSkill->GetMaxLevel() )
				break;

			nValue -= nRemainProficency;
			ChangeSkillLevel(pSkill, ESLC_Self);
			if( !bLevelChanged ) bLevelChanged = TRUE;

			// �õ���һ������������
			nRemainProficency = pSkill->GetLevelUpExp();
		}

		// �鿴������ۼӾ���
		if( nValue >= nRemainProficency )
		{
			pSkill->SetProficiency(nRemainProficency);
		}
		else
		{
			pSkill->SetProficiency(nValue);
		}
	}

	// �ƺ��¼�����
	Role *pRole = dynamic_cast<Role *>(this);
	if (NULL != pRole)
	{
		pRole->GetTitleMgr()->SigEvent(ETE_SKILL_LEVEL, pSkill->GetID(), pSkill->GetLevel());
	}

	// �����������������Ըı�
	if( bLevelChanged && NeedRecalAtt() )
	{
		RecalAtt();
	}
}

//----------------------------------------------------------------------------------
// ������CD
//----------------------------------------------------------------------------------
VOID Unit::StartSkillCoolDown(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;

	pSkill->StartCoolDown();

	m_listSkillCoolDown.PushBack(pSkill->GetID());
}

//----------------------------------------------------------------------------------
// ���ĳ�����ܵ�CD
//----------------------------------------------------------------------------------
VOID Unit::ClearSkillCoolDown(DWORD dwSkillID)
{
	Skill* pSkill = GetSkill(dwSkillID);
	if( !P_VALID(pSkill) ) return;

	pSkill->ClearCoolDown();
	m_listSkillCoolDown.Erase(dwSkillID);
}

//------------------------------------------------------------------------------
// ��������״̬����
//------------------------------------------------------------------------------
BOOL Unit::OnActiveSkillBuffTrigger(Skill* pSkill, TList<DWORD>& listTarget, ETriggerEventType eEvent, 
									DWORD dwEventMisc1/*=GT_INVALID*/, DWORD dwEventMisc2/*=GT_INVALID*/)
{
	// ���ܲ����ڻ��߲�����������
	if( !P_VALID(pSkill) || FALSE == pSkill->IsActive() ) return FALSE;

	// ���ܽű�
	const SkillScript* pScript = pSkill->GetSkillScript();
	if (P_VALID(pScript))
	{
		BOOL bIgnore = FALSE;
		pScript->CastSkill(GetMap(), pSkill->GetID(), GetID(), bIgnore);
		if (bIgnore)
		{
			return TRUE;
		}
	}

	// ��鼼������skillbuff���������Ա��е�buffid
	for(INT n = 0; n < MAX_BUFF_PER_SKILL; ++n)
	{
		DWORD dwBuffTypeID = pSkill->GetBuffTypeID(n);
		DWORD dwTriggerID = pSkill->GetTriggerID(n);
		if( !P_VALID(dwBuffTypeID) || !P_VALID(dwTriggerID) ) continue;

		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
		const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
		if( !P_VALID(pBuffProto) || !P_VALID(pTriggerProto) ) continue;

		// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
		if( ETEE_Null != pTriggerProto->eEventType && eEvent != pTriggerProto->eEventType )
			continue;

		// ͨ��buff����Ӷ���������
		DWORD dwTargetUnitID = GT_INVALID;
		TList<DWORD>::TListIterator it = listTarget.Begin();
		while( listTarget.PeekNext(it, dwTargetUnitID) )
		{
			Unit* pTarget = GetMap()->FindUnit(dwTargetUnitID);
			if( !P_VALID(pTarget) ) continue;

			pTarget->TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);
		}

		// ���Ը��������
		// ������������buff��ӹ���
		// ��1��������ܵ�Ŀ����������buff����Ӷ���������ֻҪ��������ͳ������
		// ��2��������ܵ�Ŀ������������buff����Ӷ���Ϊ����ʱ�ų������
		DWORD dwSkillTargetUnitID = GetCombatHandler().GetTargetUnitID();
		if( GT_INVALID == dwSkillTargetUnitID || GetID() == dwSkillTargetUnitID )
		{
			if( pBuffProto->dwTargetAddLimit & ETF_Self )
				TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);
		}
		else
		{
			if( pBuffProto->dwTargetAddLimit == ETF_Self )
				TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// Buff����
//-----------------------------------------------------------------------------------
BOOL Unit::OnBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1/* =GT_INVALID */, DWORD dwEventMisc2/* =GT_INVALID */)
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		m_Buff[n].OnTrigger(pTarget, eEvent, dwEventMisc1, dwEventMisc2);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ע��Buff�޸���
//-----------------------------------------------------------------------------------
VOID Unit::RegisterBuffModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Buff != pSkill->GetTargetType() ) return;

	DWORD dwBuffID = pSkill->GetTargetBuffID();
	if( FALSE == P_VALID(dwBuffID) ) return;

	TList<DWORD>* pList = m_mapBuffModifier.Peek(dwBuffID);
	if( !P_VALID(pList) )
	{
		pList = new TList<DWORD>;
		m_mapBuffModifier.Add(dwBuffID, pList);
	}

	DWORD dwSkillTypeID = pSkill->GetTypeID();
	pList->PushBack(dwSkillTypeID);				// ����ע����Ǽ��ܵ�����id
	
}

//-----------------------------------------------------------------------------------
// ����Buff�޸���
//-----------------------------------------------------------------------------------
VOID Unit::UnRegisterBuffModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Buff != pSkill->GetTargetType() ) return;

	DWORD dwBuffID = pSkill->GetTargetBuffID();
	if( FALSE == P_VALID(dwBuffID) ) return;

	TList<DWORD>* pList = m_mapBuffModifier.Peek(dwBuffID);
	if( P_VALID(pList) )
	{
		DWORD dwSkillTypeID = pSkill->GetTypeID();
		pList->Erase(dwSkillTypeID);
	}
}

//-----------------------------------------------------------------------------------
// ע��Trigger�޸���
//-----------------------------------------------------------------------------------
VOID Unit::RegisterTriggerModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Trigger != pSkill->GetTargetType() ) return;

	DWORD dwTriggerID = pSkill->GetTargetTriggerID();
	if( FALSE == P_VALID(dwTriggerID) ) return;

	TList<DWORD>* pList = m_mapTriggerModifier.Peek(dwTriggerID);
	if( !P_VALID(pList) )
	{
		pList = new TList<DWORD>;
		m_mapTriggerModifier.Add(dwTriggerID, pList);
	}
	DWORD dwSkillTypeID = pSkill->GetTypeID();
	pList->PushBack(dwSkillTypeID);				// ����ע����Ǽ��ܵ�����id

}

//-----------------------------------------------------------------------------------
// ����Buff�޸���
//-----------------------------------------------------------------------------------
VOID Unit::UnRegisterTriggerModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Trigger != pSkill->GetTargetType() ) return;

	DWORD dwTriggerID = pSkill->GetTargetTriggerID();
	if( FALSE == P_VALID(dwTriggerID) ) return;

	TList<DWORD>* pList = m_mapTriggerModifier.Peek(dwTriggerID);
	if( P_VALID(pList) )
	{
		DWORD dwSkillTypeID = pSkill->GetTypeID();
		pList->Erase(dwSkillTypeID);
	}
}

//-----------------------------------------------------------------------------------
// �������Buff
//-----------------------------------------------------------------------------------
BOOL Unit::TryAddBuff(Unit* pSrc, const tagBuffProto* pBuffProto, const tagTriggerProto* pTriggerProto, 
					  Skill* pSkill, tagItem* pItem, BOOL bRecalAtt, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	if( !P_VALID(pBuffProto) ) return FALSE;

	// ͨ������ID�õ�ID�͵ȼ�
	DWORD dwBuffID = Buff::GetIDFromTypeID(pBuffProto->dwID);
	INT nBuffLevel = Buff::GetLevelFromTypeID(pBuffProto->dwID);

	// �ж�trigger
	if( P_VALID(pSrc) && P_VALID(pTriggerProto) )
	{
		if( !pSrc->TestTrigger(this, pTriggerProto, dwEventMisc1, dwEventMisc2) )
			return FALSE;

		// �ƺ��¼�����
		Role *pRole = dynamic_cast<Role *>(pSrc);
		if (NULL != pRole)
		{
			pRole->GetTitleMgr()->SigEvent(ETE_TRIGGER, pTriggerProto->dwID, GT_INVALID);
		}
	}

	// �������ߴ��ڣ���������ߺ����������
	if( P_VALID(pSrc) )
	{
		// ���ȼ�������������
		DWORD dwTargetFlag = pSrc->TargetTypeFlag(this);
		if( !(pBuffProto->dwTargetAddLimit & dwTargetFlag) )
			return FALSE;

		// �����е����ж�
	}

	// �ټ��buff���״̬����
	DWORD dwStatFlag = GetStateFlag();
	if( (dwStatFlag & pBuffProto->dwTargetAddStateLimit) != dwStatFlag )
	{
		return FALSE;
	}

	// �жϿ�������
	ERoleAttribute eReistType = BuffResistType2ERA(pBuffProto->eResistType);
	if( ERA_Null != eReistType && ERA_Regain_Addtion != eReistType )
	{
		// ����������
		// ��ʽ��������=��1-##����/200��
		INT nProp = 100 - INT((FLOAT)GetAttValue(eReistType) / 2.0f);
		if( nProp < 0 ) nProp = 0;
		if( (IUTIL->Rand() % 100) > nProp )
			return FALSE;
	}

	// �жϵ����͵���
	INT nIndex = GT_INVALID;
	INT nRet = BuffCounteractAndWrap(pSrc, dwBuffID, nBuffLevel, pBuffProto->nLevel, pBuffProto->dwGroupFlag, pBuffProto->bBenifit, nIndex);

	if( EBCAWR_CanNotAdd == nRet ) return FALSE;
	else if( EBCAWR_Wraped == nRet ) return TRUE;

	ASSERT( EBCAWR_CanAdd == nRet );

	// ���buff
	AddBuff(pBuffProto, pSrc, (P_VALID(pSkill) ? pSkill->GetID() : GT_INVALID), pItem, nIndex, bRecalAtt);
	return TRUE;
}

//-----------------------------------------------------------------------------------
// ��GM�������Buff
//-----------------------------------------------------------------------------------
BOOL Unit::GMTryAddBuff(Unit* pSrc, const tagBuffProto* pBuffProto, const tagTriggerProto* pTriggerProto, 
					  Skill* pSkill, tagItem* pItem, BOOL bRecalAtt, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	if( !P_VALID(pBuffProto) ) return FALSE;

	// ͨ������ID�õ�ID�͵ȼ�
	DWORD dwBuffID = Buff::GetIDFromTypeID(pBuffProto->dwID);
	INT nBuffLevel = Buff::GetLevelFromTypeID(pBuffProto->dwID);

	// �ж�trigger
	if( P_VALID(pSrc) && P_VALID(pTriggerProto) )
	{
		if( !pSrc->TestTrigger(this, pTriggerProto, dwEventMisc1, dwEventMisc2) )
			return FALSE;

		// �ƺ��¼�����
		Role *pRole = dynamic_cast<Role *>(pSrc);
		if (NULL != pRole)
		{
			pRole->GetTitleMgr()->SigEvent(ETE_TRIGGER, pTriggerProto->dwID, GT_INVALID);
		}
	}

	// �������ߴ��ڣ���������ߺ����������
	//if( P_VALID(pSrc) )
	//{
	//	// ���ȼ�������������tbc:inves
	//	DWORD dwTargetFlag = pSrc->TargetTypeFlag(this);
	//	if( !(pBuffProto->dwTargetAddLimit & dwTargetFlag) )
	//		return FALSE;

	//	// �����е����ж�
	//}

	// �жϿ�������
	ERoleAttribute eReistType = BuffResistType2ERA(pBuffProto->eResistType);
	if( ERA_Null != eReistType && ERA_Regain_Addtion != eReistType )
	{
		// ����������
		// ��ʽ��������=��1-##����/200��
		INT nProp = 100 - INT((FLOAT)GetAttValue(eReistType) / 2.0f);
		if( nProp < 0 ) nProp = 0;
		if( (IUTIL->Rand() % 100) > nProp )
			return FALSE;
	}

	// �жϵ����͵���
	INT nIndex = GT_INVALID;
	INT nRet = BuffCounteractAndWrap(pSrc, dwBuffID, nBuffLevel, pBuffProto->nLevel, pBuffProto->dwGroupFlag, pBuffProto->bBenifit, nIndex);

	if( EBCAWR_CanNotAdd == nRet ) return FALSE;
	else if( EBCAWR_Wraped == nRet ) return TRUE;

	ASSERT( EBCAWR_CanAdd == nRet );

	// ���buff
	for (int i= ERA_AttA_Start; i< ERA_End; i++)
	{
		SetAttRecalFlag(i);
	}
	AddBuff(pBuffProto, pSrc, (P_VALID(pSkill) ? pSkill->GetID() : GT_INVALID), pItem, nIndex, bRecalAtt);
	return TRUE;
}

//----------------------------------------------------------------------------------
// ����һ��״̬
//----------------------------------------------------------------------------------
VOID Unit::AddBuff(const tagBuffProto* pBuffProto, Unit* pSrc, DWORD dwSrcSkillID, const tagItem* pItem, INT nIndex, BOOL bRecalAtt)
{
	if( !P_VALID(pBuffProto) ) return;
	if( nIndex < 0 && nIndex >= MAX_BUFF_NUM ) return;

	// ͨ��BuffID�ҵ���Buff�Ƿ���Mod
	DWORD dwBuffID = Buff::GetIDFromTypeID(pBuffProto->dwID);
	TList<DWORD>* pListModifier = NULL;
	if( P_VALID(pSrc) ) pListModifier = pSrc->GetBuffModifier(dwBuffID);

	// �����˲ʱbuff
	if( pBuffProto->bInstant )
	{
		// ����˲ʱЧ��
		if( !P_VALID(pListModifier) || pListModifier->Empty() )
		{
			CalBuffInstantEffect(pSrc, EBEM_Instant, pBuffProto, NULL);
		}
		else
		{
			// ��ʱ����һ��mod�ṹ
			tagBuffMod mod;
			TList<DWORD>::TListIterator it = pListModifier->Begin();
			DWORD dwSkillTypeID = GT_INVALID;

			while( pListModifier->PeekNext(it, dwSkillTypeID) )
			{
				const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(dwSkillTypeID);
				if( !P_VALID(pSkillProto) ) continue;

				mod.SetMod(pSkillProto);
			}

			CalBuffInstantEffect(pSrc, EBEM_Instant, pBuffProto, &mod);
		}

		// ������Ϣ
		if( P_VALID(GetMap()) )
		{
			tagNS_AddRoleBuff send;
			send.dwRoleID = GetID();
			send.Buff.dwBuffTypeID = pBuffProto->dwID;
			send.Buff.nWarpTimes = 1;
			send.Buff.nMaxPersistTime = GT_INVALID;
			send.Buff.nPersistTimeLeft = GT_INVALID;

			GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
		}
	}
	else
	{
		m_Buff[nIndex].Init(pBuffProto, this, pSrc, dwSrcSkillID, pItem, nIndex, pListModifier);
		m_mapBuff.Add(m_Buff[nIndex].GetID(), &m_Buff[nIndex]);

		// ������Ϣ
		if( GetMap() )
		{
			tagNS_AddRoleBuff send;
			send.dwRoleID = GetID();
			send.Buff.dwBuffTypeID = m_Buff[nIndex].GetTypeID();
			send.Buff.nWarpTimes = m_Buff[nIndex].GetWrapTimes();
			send.Buff.nMaxPersistTime = m_Buff[nIndex].GetPersistTime();
			send.Buff.nPersistTimeLeft = m_Buff[nIndex].GetPersistTimeLeft();

			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}

	// ���¼����������
	if( bRecalAtt && NeedRecalAtt() ) RecalAtt();
}

//----------------------------------------------------------------------------------
// ȥ��һ��״̬
//----------------------------------------------------------------------------------
INT Unit::RemoveBuff(DWORD dwBuffID, BOOL bRecalAtt, BOOL bSelf)
{
	Buff* pBuff = m_mapBuff.Peek(dwBuffID);
	if( !P_VALID(pBuff) ) return GT_INVALID;

	// �õ�Buff��ǰ��״̬
	Buff::EBuffState eState = pBuff->GetState();

	// ����ɾ����Buff������Ҫ�ٴ���
	if( Buff::EBS_Destroying == eState )
	{
		return GT_INVALID;
	}

	// ��ǰ���ڳ�ʼ������µ�Buff��Ҫ��ӵ�ɾ���б�
	if( Buff::EBS_Initing	== eState ||
		Buff::EBS_Updating	== eState )
	{
		m_listDestroyBuffID.PushBack(pBuff->GetID());
		return GT_INVALID;
	}

	// ��ǰ�ڿ���״̬��Buff����������ɾ��
	if( GetMap() )
	{
		tagNS_RemoveRoleBuff send;
		send.dwRoleID = GetID();
		send.dwBuffTypeID = pBuff->GetTypeID();

		if( P_VALID(GetMap()) )
		{
			GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
		}
	}

	DWORD dwBuffInterruptID = pBuff->GetBuffInterruptID();

	INT nIndex = pBuff->GetIndex();
	m_mapBuff.Erase(dwBuffID);
	pBuff->Destroy(bSelf);

	// ɾ������BuffID
	if( P_VALID(dwBuffInterruptID) )
	{
		RemoveBuff(dwBuffInterruptID, FALSE);
	}

	// ���¼����������
	if( bRecalAtt && NeedRecalAtt() ) RecalAtt();

	return nIndex;
}

//-----------------------------------------------------------------------------------
// ȥ��������ĳ�����ܲ�����buff
//-----------------------------------------------------------------------------------
VOID Unit::RemoveAllBuffBelongSkill(DWORD dwSkillID, BOOL bRecalAtt)
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		// �������Լ�ʩ�ŵĲ����ɸü��ܲ���
		if( m_Buff[n].GetSrcUnitID() != GetID() || m_Buff[n].GetSrcSkillID() != dwSkillID )
			continue;

		RemoveBuff(m_Buff[n].GetID(), FALSE);
	}

	if( bRecalAtt && NeedRecalAtt() )
	{
		RecalAtt();
	}
}

//-----------------------------------------------------------------------------------
// ȥ������Buff
//-----------------------------------------------------------------------------------
VOID Unit::RemoveAllBuff()
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		RemoveBuff(m_Buff[n].GetID(), FALSE);
	}

	if( NeedRecalAtt() )
		RecalAtt();
}

//-----------------------------------------------------------------------------------
// ȥ������������к�Buff
//-----------------------------------------------------------------------------------
VOID Unit::RemoveAllBuff(BOOL bBenefit)
{
	INT nStart = 0, nEnd = MAX_BENEFIT_BUFF_NUM;

	if( !bBenefit )
	{
		nStart = MAX_BENEFIT_BUFF_NUM;
		nEnd = MAX_BUFF_NUM;
	}

	INT nIndex = GT_INVALID;
	for(INT n = nStart; n < nEnd; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;
		RemoveBuff(m_Buff[n].GetID(), FALSE);
	}

	if( NeedRecalAtt() )
		RecalAtt();
}

//-----------------------------------------------------------------------------------
// ����Buff
//-----------------------------------------------------------------------------------
VOID Unit::UpdateBuff()
{
	// ���ȸ������д��ڵ�Buff
	for(INT n = 0; n < MAX_BUFF_NUM; n++)
	{
		BOOL IsValid = m_Buff[n].IsValid();
		if( !IsValid) continue;

		// ���ø�Buff��Update
		if( m_Buff[n].Update() )
		{
			RemoveBuff(m_Buff[n].GetID(), TRUE, TRUE);
		}
	}

	// ��ɾ��ɾ���б����Buff
	if( !m_listDestroyBuffID.Empty() )
	{
		DWORD dwBuffID = m_listDestroyBuffID.PopFront();
		while( P_VALID(dwBuffID) )
		{
			RemoveBuff(dwBuffID, TRUE);

			dwBuffID = m_listDestroyBuffID.PopFront();
		}
	}
}

//-----------------------------------------------------------------------------------
// �ֶ�ȡ��һ��Buff
//-----------------------------------------------------------------------------------
BOOL Unit::CancelBuff(DWORD dwBuffID)
{
	Buff* pBuff = GetBuffPtr(dwBuffID);
	if( !P_VALID(pBuff) ) return FALSE;

	// ���Ƿ����ֶ����
	if( !pBuff->Interrupt(EBIF_Manual) ) return FALSE;

	// ɾ��Buff
	RemoveBuff(dwBuffID, TRUE);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ���Buff�¼�
//-----------------------------------------------------------------------------------
VOID Unit::OnInterruptBuffEvent(EBuffInterruptFlag eFlag, INT nMisc/* =GT_INVALID */)
{
	BOOL bNeedRecal = FALSE;
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;
		if( !m_Buff[n].Interrupt(eFlag, nMisc) ) continue;
		
		RemoveBuff(m_Buff[n].GetID(), FALSE);
		bNeedRecal = TRUE;
	}

	if( bNeedRecal && NeedRecalAtt() )	RecalAtt();
}

//-----------------------------------------------------------------------------------
// ��ɢ���һ��Buff������ָ����Buff����Debuff��
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(BOOL bBenefit)
{
	INT nStart = 0, nEnd = MAX_BENEFIT_BUFF_NUM;

	if( !bBenefit )
	{
		nStart = MAX_BENEFIT_BUFF_NUM;
		nEnd = MAX_BUFF_NUM;
	}

	INT nIndex = GT_INVALID;
	for(INT n = nStart; n < nEnd; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;
		nIndex = n;
	}

	if( GT_INVALID != nIndex )
	{
		RemoveBuff(m_Buff[nIndex].GetID(), TRUE);
	}
}

//-----------------------------------------------------------------------------------
// ��ɢ���һ����������ΪeType��Buff
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(EBuffResistType eType)
{
	INT nIndex = GT_INVALID;
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		if( m_Buff[n].GetResistType() != eType ) continue;

		nIndex = n;
	}

	if( GT_INVALID != nIndex )
	{
		RemoveBuff(m_Buff[nIndex].GetID(), TRUE);
	}
}

//-----------------------------------------------------------------------------------
// ��ɢĳһ��ָ��ID��Buff
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(DWORD dwBuffID)
{
	RemoveBuff(dwBuffID, TRUE);
}

//-----------------------------------------------------------------------------------
// ��ɢĳһ��ָ��Ч�����͵�buff
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(INT nType, BOOL bLastOne)
{
	//if (!(eType == EBET_Dizzy || eType == EBET_NoSkill || eType == EBET_Spor || eType == EBET_Tie || eType == EBET_Invincible
	//	  || eType == EBET_DisArm || eType == EBET_NoPrepare || eType == EBET_IgnoreArmor || eType == EBET_Sneer || eType == EBET_Immunity))
	//	return;
	EBuffEffectType eType = EBET_Null;
	switch(nType)
	{
	case 1:
		eType = EBET_Dizzy;
		break;
	case 2:
		eType = EBET_NoSkill;
		break;
	case 3:
		eType = EBET_Spor;
		break;
	case 4:
		eType = EBET_Tie;
		break;
	case 5:
		eType = EBET_Invincible;
		break;
	case 6:
		eType = EBET_DisArm;
		break;
	case 7:
		eType = EBET_NoPrepare;
		break;
	case 8:
		eType = EBET_IgnoreArmor;
		break;
	case 9:
		eType = EBET_Sneer;
		break;
	case 10:
		eType = EBET_Immunity;
		break;
	default:
		break;
	}

	INT nIndex = GT_INVALID;
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		if( m_Buff[n].GetEffectType(EBEM_Persist) != eType ) continue;

		if(bLastOne)
			nIndex = n;
		else
			RemoveBuff(m_Buff[n].GetID(), TRUE);
	}

	if( GT_INVALID != nIndex && bLastOne)
	{
		RemoveBuff(m_Buff[nIndex].GetID(), TRUE);
	}
}

//-----------------------------------------------------------------------------------
// ����Buff��˲ʱ��Ч����˲ʱ��Ч��������˲ʱЧ�����������Ч���ͽ���ʱЧ��
//-----------------------------------------------------------------------------------
VOID Unit::CalBuffInstantEffect(Unit* pSrc, EBuffEffectMode eMode, const tagBuffProto* pProto, const tagBuffMod* pMod, INT nWrapTimes, Unit* pTriggerTarget)
{
	if( !P_VALID(pProto) ) return;
	if( EBEM_Instant != eMode && EBEM_Inter != eMode && EBEM_Finish != eMode ) return;

	// ˲ʱЧ�������ĵ��Ӵ���
	if( EBEM_Instant == eMode ) nWrapTimes = 1;

	BOOL bHaveMod = FALSE;
	if( P_VALID(pMod) && pMod->IsValid() && pMod->eModBuffEffectMode == eMode )
	{
		bHaveMod = TRUE;
	}

	// ���Լӳ�Ӱ��
	INT nAttMod[EBEA_End] = {0};
	const INT* pnAttMod = NULL;
	switch(eMode)
	{
	case EBEM_Instant:
		pnAttMod = pProto->nInstantAttMod;
		break;

	case EBEM_Inter:
		pnAttMod = pProto->nInterAttMod;
		break;

	case EBEM_Finish:
		pnAttMod = pProto->nFinishAttMod;
		break;

	default:
		break;
	}

	// Ч��
	EBuffEffectType eEffect = pProto->eEffect[eMode];
	DWORD dwEffectMisc1 = pProto->dwEffectMisc1[eMode];
	DWORD dwEffectMisc2 = pProto->dwEffectMisc2[eMode];

	// mod�����Ӱ��
	if( bHaveMod )
	{
		for(INT n = 0; n < EBEA_End; ++n)
		{
			nAttMod[n] = pnAttMod[n] + pMod->nEffectAttMod[n];

			if( abs(nAttMod[n]) > 100000 )	// �ٷֱ�
			{
				INT nAtt = 0;
				switch(n)
				{
				case EBEA_HP:
					nAtt = GetAttValue(ERA_MaxHP);
					break;

				case EBEA_MP:
					nAtt = GetAttValue(ERA_MaxMP);
					break;

				case EBEA_Vitality:
					nAtt = GetAttValue(ERA_MaxVitality);
					break;

				case ERA_Endurance:
					nAtt = GetAttValue(ERA_MaxEndurance);
					break;

				default:
					break;
				}

				nAttMod[n] = (nAttMod[n] > 0 ? 1 : -1) * INT((FLOAT)nAtt * (FLOAT(abs(nAttMod[n]) - 100000) / 10000.0f));
			}

			nAttMod[n] *= nWrapTimes;
		}
		dwEffectMisc1 += pMod->nEffectMisc1Mod;
		dwEffectMisc2 += pMod->nEffectMisc2Mod;
	}
	else
	{
		for(INT n = 0; n < EBEA_End; ++n)
		{
			nAttMod[n] = pnAttMod[n];

			if( abs(nAttMod[n]) > 100000 )	// �ٷֱ�
			{
				INT nAtt = 0;
				switch(n)
				{
				case EBEA_HP:
					nAtt = GetAttValue(ERA_MaxHP);
					break;

				case EBEA_MP:
					nAtt = GetAttValue(ERA_MaxMP);
					break;

				case EBEA_Vitality:
					nAtt = GetAttValue(ERA_MaxVitality);
					break;

				case ERA_Endurance:
					nAtt = GetAttValue(ERA_MaxEndurance);
					break;

				default:
					break;
				}

				nAttMod[n] = (nAttMod[n] > 0 ? 1 : -1) * INT((FLOAT)nAtt * (FLOAT(abs(nAttMod[n]) - 100000) / 10000.0f));
			}

			nAttMod[n] *= nWrapTimes;
		}
	}

	// ���ݹ������ͣ���������͹�����Χ��ȷ����Χ��
	TList<Unit*> listTargetUnit;
	CalBuffTargetList(pSrc, pProto->eOPType, pProto->fOPDistance, pProto->fOPRadius, 
						pProto->eFriendly, pProto->dwTargetLimit, pProto->dwTargetStateLimit, listTargetUnit, pTriggerTarget);

	Unit* pTarget = listTargetUnit.PopFront();
	while( P_VALID(pTarget) )
	{
		// ��������Ӱ��
		for(INT n = 0; n < EBEA_End; ++n)
		{
			if( 0 == nAttMod[n] ) continue;

			switch(n)
			{
			case EBEA_HP:
				{
					// ����ǻָ���buff
					if (pProto->eResistType == EBRT_Regain)
					{
						// Ԫ��Ӱ��ָ��ٷֱȣ�0.4���ȼ���1.8�µ�ǰԪ��ֵ������Сֵȡ0��
						FLOAT fPneuma = (FLOAT)max( 0.0, (0.4 - (FLOAT)(pSrc->GetLevel()) * 1.8 / (FLOAT)(pSrc->GetAttValue(ERA_Pneuma))) );

						// ����Ӱ��ָ��ٷֱȣ������£���ɫ�ȼ���50��50��
						FLOAT fRegain = (FLOAT)(pSrc->GetAttValue(ERA_Regain_Addtion)) / (FLOAT)((pSrc->GetLevel())*50 + 50);

						nAttMod[n] = (INT)((FLOAT)nAttMod[n] * (FLOAT)(1.0 + fPneuma + fRegain));
					}

					// ����ǰ���
					if (pProto->eResistType == EBRT_Bleeding)
					{
						// Ԫ��Ӱ�찵�˰ٷֱȣ�0.2���ȼ���0.9�µ�ǰԪ��ֵ������Сֵȡ0��
						FLOAT fPneuma = (FLOAT)max( 0.0, (0.2 - (FLOAT)(pSrc->GetLevel()) * 0.9 / (FLOAT)(pSrc->GetAttValue(ERA_Pneuma))) );

						// ����Ӱ�찵�˰ٷֱȣ������£���ɫ�ȼ���100��100��
						FLOAT fRegain = (FLOAT)(pSrc->GetAttValue(ERA_Regain_Addtion)) / (FLOAT)(pSrc->GetLevel()*100 + 100);

						// ״̬���ö���İ��˿��԰ٷֱ�=���˿��ԡ�200
						FLOAT fResist = (FLOAT)GetAttValue(ERA_Resist_Bleeding) / 200.0;

						// ���հ���Ч����״̬����ֵ����1��Ԫ��Ӱ�찵�˰ٷֱȣ�����Ӱ�찵�˰ٷֱȣ�
						// ״̬���ö���İ��˿��ԡ�200��
						nAttMod[n] = (INT)((FLOAT)nAttMod[n] * (FLOAT)(1.0 + fPneuma + fRegain - fResist));
					}

					if( pTarget->IsCreature() )
					{
						Creature* pCreature = static_cast<Creature*>(pTarget);
						if( !P_VALID(pCreature) )	break;
						
						// ������߶��ǹ���򲻻ᴥ��BuffInjury
						if (!this->IsCreature())
						{
							pCreature->OnBuffInjury(pSrc);
						}
					}

					pTarget->ChangeHP(nAttMod[n], pSrc, NULL, pProto);
				}
				break;

			case EBEA_MP:
				{
					pTarget->ChangeMP(nAttMod[n]);
				}
				break;

			case EBEA_Rage:
				{
					pTarget->ChangeRage(nAttMod[n]);
				}
				break;

			case EBEA_Vitality:
				{
					pTarget->ChangeVitality(nAttMod[n]);
				}
				break;

			case EBEA_Endurance:
				{
					pTarget->ChangeEndurance(nAttMod[n]);
				}
				break;

			case EBEA_Morale:
				{
					pTarget->ModAttValue(ERA_Morale, nAttMod[n]);
				}
				break;

			case EBEA_Injury:
				{
					pTarget->ModAttValue(ERA_Injury, nAttMod[n]);
				}
				break;

			default:
				break;
			}
		}

		// ��������Ч��
		if( EBET_Null != eEffect )
		{
			BuffEffect::CalBuffEffect(pTarget, pSrc, eEffect, dwEffectMisc1, dwEffectMisc2, TRUE, pProto);
		}

		// ��ȡһ��
		pTarget = listTargetUnit.PopFront();
	}
}

//-----------------------------------------------------------------------------------
// ����Buff�ĳ�����Ч����ֻ�ܶ���������
//-----------------------------------------------------------------------------------
VOID Unit::CalBuffPersistEffect(Unit* pSrc, const tagBuffProto* pProto, const tagBuffMod* pMod, INT nWrapTimes, BOOL bSet/* =TRUE */)
{
	if( !P_VALID(pProto) ) return;

	BOOL bHaveMod = FALSE;
	if( P_VALID(pMod) && pMod->IsValid() && EBEM_Persist == pMod->eModBuffEffectMode )
	{
		bHaveMod = TRUE;
	}

	EBuffEffectType eEffect = pProto->eEffect[EBEM_Persist];
	DWORD dwEffectMisc1 = pProto->dwEffectMisc1[EBEM_Persist];
	DWORD dwEffectMisc2 = pProto->dwEffectMisc2[EBEM_Persist];

	if( bHaveMod )
	{
		dwEffectMisc1 += pMod->nEffectMisc1Mod;
		dwEffectMisc2 += pMod->nEffectMisc2Mod;
	}

	// �ȼ������Լӳ�
	ERoleAttribute eAtt = ERA_Null;
	INT nValue = 0;
	TMap<ERoleAttribute, INT>::TMapIterator it;
	TMap<ERoleAttribute, INT>::TMapIterator itPct;

	// �ȼ��㾲̬���Ե�
	it = pProto->mapRoleAttMod.Begin();
	while( pProto->mapRoleAttMod.PeekNext(it, eAtt, nValue) )
	{
		ModAttModValue(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
	}

	itPct = pProto->mapRoleAttModPct.Begin();
	while( pProto->mapRoleAttModPct.PeekNext(itPct, eAtt, nValue) )
	{
		ModAttModValuePct(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
	}

	// �ڼ���mod��
	if( bHaveMod )
	{
		it = pMod->mapRoleAttMod.Begin();
		while( pMod->mapRoleAttMod.PeekNext(it, eAtt, nValue) )
		{
			ModAttModValue(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
		}

		itPct = pMod->mapRoleAttModPct.Begin();
		while( pMod->mapRoleAttModPct.PeekNext(itPct, eAtt, nValue) )
		{
			ModAttModValuePct(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
		}
	}

	// �ټ���Ч��
	if( EBET_Null != eEffect )
	{
		BuffEffect::CalBuffEffect(this, pSrc, eEffect, dwEffectMisc1, dwEffectMisc2, bSet, pProto);
	}
}

//-----------------------------------------------------------------------------------
// ����Buff�ĳ�����Ч��
//-----------------------------------------------------------------------------------
VOID Unit::WrapBuffPersistEffect(Unit* pSrc, const tagBuffProto* pProto, const tagBuffMod* pMod)
{
	CalBuffPersistEffect(pSrc, pProto, pMod, 1, TRUE);
}


//-----------------------------------------------------------------------------------
// ����Buff�����ö���
//-----------------------------------------------------------------------------------
VOID Unit::CalBuffTargetList(Unit* pSrc, EBuffOPType eOPType, FLOAT fOPDist, FLOAT fOPRadius, 
							 EBuffFriendEnemy eFriendEnemy, DWORD dwTargetLimit, DWORD dwTargetStateLimit, TList<Unit*>& listTarget, Unit* pTarget)
{
	// ���������Ѿ��������ˣ�ֱ�ӷ���
	if( !P_VALID(pSrc) ) return;

	// ���ȼ��һ������
	if( pSrc->IsBuffTargetValid(this, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
	{
		listTarget.PushBack(this);
	}

	// �ټ��һ��Ŀ��
	if( P_VALID(pTarget) && pSrc->IsBuffTargetValid(pTarget, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
	{
		listTarget.PushBack(pTarget);
	}

	// Ԥ�ȼ��һ�¹�����Χ
	if( 0.0f == fOPRadius ) return;

	// ���������Χ��Ϊ0������Ŀ��Ϊ���ļ��
	FLOAT fOPRadiusSQ = fOPRadius * fOPRadius;

	tagVisTile* pVisTile[ED_End] = {0};

	// �õ�������Χ�ڵ�vistile�б�
	GetMap()->GetVisTile(GetCurPos(), fOPRadius, pVisTile);
	Role*		pRole		= NULL;
	Creature*	pCreature	= NULL;

	for(INT n = ED_Center; n < ED_End; n++)
	{
		if( !P_VALID(pVisTile[n]) ) continue;

		// ���ȼ������
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD, Role*>::TMapIterator it = mapRole.Begin();

		while( mapRole.PeekNext(it, pRole) )
		{
			if( pRole == this ) continue;

			if( FALSE == pSrc->IsBuffTargetValid(pRole, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
				continue;

			// �����ж�
			FLOAT fDistSQ = Vec3DistSq(GetCurPos(), pRole->GetCurPos());
			if( fDistSQ > fOPRadiusSQ  ) continue;

			// ���߼��

			// �ж�ͨ��������Ҽ��뵽�б���
			listTarget.PushBack(pRole);
		}

		// �ټ������
		TMap<DWORD, Creature*>& mapCreature = pVisTile[n]->mapCreature;
		TMap<DWORD, Creature*>::TMapIterator it2 = mapCreature.Begin();

		while( mapCreature.PeekNext(it2, pCreature) )
		{
			if( pCreature == this ) continue;

			if( FALSE == pSrc->IsBuffTargetValid(pCreature, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
				continue;

			// �����ж�
			FLOAT fDistSQ = Vec3DistSq(GetCurPos(), pCreature->GetCurPos());
			if( fDistSQ > fOPRadiusSQ  ) continue;

			// ���߼��

			// �ж�ͨ������������뵽�б���
			listTarget.PushBack(pCreature);
		}
	}
}

//-----------------------------------------------------------------------------------
// ȡ��ָ������ʩ�ŵ�buff
//-----------------------------------------------------------------------------------
Buff* Unit::GetRelativeSkillBuff(DWORD dwSkillID, BOOL bBenefit /*= TRUE*/)
{
	INT nStart = 0;
	INT nEnd = MAX_BENEFIT_BUFF_NUM;

	if( !bBenefit )
	{
		nStart = MAX_BENEFIT_BUFF_NUM;
		nEnd = MAX_BUFF_NUM;
	}

	for(INT n = nStart; n < nEnd; n++)
	{
		// �յ�λ��
		if( !m_Buff[n].IsValid() )
		{
			continue;
		}
		
		// ����ID��ͬ
		if (m_Buff[n].GetSrcSkillID() == dwSkillID)
		{
			return &m_Buff[n];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------------
// ���ĳ��Ŀ���ǲ���Buff�ĺϷ�Ŀ��
//-----------------------------------------------------------------------------------
BOOL Unit::IsBuffTargetValid(Unit* pTarget, EBuffFriendEnemy eFriendEnemy, DWORD dwTargetLimit, DWORD dwTargetStateLimit)
{
	if( !P_VALID(pTarget) ) return FALSE;

	// �����ж�
	DWORD dwTargetFlag = TargetTypeFlag(pTarget);
	if( !(dwTargetFlag & dwTargetLimit) )
		return FALSE;

	// ״̬�ж�
	DWORD dwTargetStateFlag = pTarget->GetStateFlag();
	if( (dwTargetStateFlag & dwTargetStateLimit) != dwTargetStateFlag )
		return FALSE;

	// �����ж�
	DWORD dwFriendEnemy = FriendEnemy(pTarget);
	if( EBFE_Friendly == eFriendEnemy )
	{
		if( !(dwFriendEnemy & ETFE_Friendly) )
			return FALSE;
	}
	else if( EBFE_Hostile == eFriendEnemy )
	{
		if( !(dwFriendEnemy & ETFE_Hostile) )
			return FALSE;
	}
	else if( EBFE_Independent == eFriendEnemy )
	{
		if( !(dwFriendEnemy & ETFE_Independent) )
			return FALSE;
	}
	else if( EBFE_All == eFriendEnemy )
	{
		if( !(dwFriendEnemy & (ETFE_Friendly | ETFE_Hostile | ETFE_Independent)) )
			return FALSE;
	}
	else
	{
		return FALSE;
	}

	// �ж�ͨ��
	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����Buff�ĵ����͵���
//-----------------------------------------------------------------------------------
INT Unit::BuffCounteractAndWrap(Unit* pSrc, DWORD dwBuffID, INT nBuffLevel, INT nGroupLevel, DWORD dwGroupFlag, BOOL bBenefit, INT& nIndex)
{
	// ���ȼ��ID
	Buff* pBuff = m_mapBuff.Peek(dwBuffID);

	// �������һ��ͬ����buff
	if( P_VALID(pBuff) && Buff::EBS_Idle == pBuff->GetState() )
	{
		if( pBuff->GetLevel() > nBuffLevel )
		{
			return EBCAWR_CanNotAdd;
		}
		else if( pBuff->GetLevel() == nBuffLevel )
		{
			// ��⿴���ܲ��ܵ���
			
			if( P_VALID(pSrc) && pBuff->GetSrcUnitID() == pSrc->GetID() )
			{
				pBuff->Wrap();
				if( NeedRecalAtt() ) RecalAtt();

				if( P_VALID(GetMap()) )
				{
					tagNS_UpdateRoleBuff send;
					send.dwRoleID = GetID();
					send.Buff.dwBuffTypeID = pBuff->GetTypeID();
					send.Buff.nWarpTimes = pBuff->GetWrapTimes();
					send.Buff.nMaxPersistTime = pBuff->GetPersistTime();
					send.Buff.nPersistTimeLeft = pBuff->GetPersistTimeLeft();

					GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
				}

				return EBCAWR_Wraped;
			}
			else
			{
				// ���������ؿ������
				nIndex = RemoveBuff(pBuff->GetID(), TRUE);
				ASSERT( GT_INVALID != nIndex );
				return EBCAWR_CanAdd;
			}
		}
		else
		{
			// ������buff�����ؿ������
			nIndex = RemoveBuff(pBuff->GetID(), TRUE);
			ASSERT( nIndex != GT_INVALID );
			return EBCAWR_CanAdd;
			
		}
	}
	// ���������һ��ͬ����buff
	else
	{
		INT nStart = 0;
		INT nEnd = MAX_BENEFIT_BUFF_NUM;

		if( !bBenefit )
		{
			nStart = MAX_BENEFIT_BUFF_NUM;
			nEnd = MAX_BUFF_NUM;
		}

		INT nBlankIndex = GT_INVALID;		// ��λ
		INT nFullIndex = GT_INVALID;		// ���Buff��ʱ���������Ա�����������
		for(INT n = nStart; n < nEnd; n++)
		{
			// �յ�λ��
			if( !m_Buff[n].IsValid() )
			{
				if( GT_INVALID == nBlankIndex )
				{
					nBlankIndex = n;
					if( GT_INVALID == dwGroupFlag ) break;
				}
				continue;
			}
			// group_flag��ͬ
			else if( (dwGroupFlag != GT_INVALID) && 
					 (m_Buff[n].GetGroupFlag() == dwGroupFlag) )
			{
				// �����ǰ������Idle״̬�����ܶ���
				if( Buff::EBS_Idle != m_Buff[n].GetState() )
				{
					return EBCAWR_CanNotAdd;
				}

				// �Ƚϵȼ����������ȼ���
				if( m_Buff[n].GetGroupLevel() > nGroupLevel )
				{
					return EBCAWR_CanNotAdd;
				}
				else
				{
					nIndex = RemoveBuff(m_Buff[n].GetID(), TRUE);
					return EBCAWR_CanAdd;
				}
			}
			// buff��ʱ����
			else if( GT_INVALID == nFullIndex && 
					 Buff::EBS_Idle == m_Buff[n].GetState() &&
					 m_Buff[n].Interrupt(EBIF_BuffFull) )
			{
				nFullIndex = n;
			}
		}

		if( GT_INVALID != nBlankIndex )
		{
			nIndex = nBlankIndex;
			return EBCAWR_CanAdd;
		}
		else if( GT_INVALID != nFullIndex )
		{
			RemoveBuff(m_Buff[nFullIndex].GetID(), TRUE);
			nIndex = nFullIndex;
			return EBCAWR_CanAdd;
		}
		else
		{
			return EBCAWR_CanNotAdd;
		}
	}
}

//------------------------------------------------------------------------------------------------------
// ����ĳ����ͨ�������Ƿ���������
//------------------------------------------------------------------------------------------------------
BOOL Unit::TestTrigger(Unit* pTarget, const tagTriggerProto* pProto, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	if( !P_VALID(pProto) ) return TRUE;

	// ���һ�¸ô������Ƿ��б�������Ӱ��
	TList<DWORD>* pListModifier = m_mapTriggerModifier.Peek(pProto->dwID);

	if( !P_VALID(pListModifier) || pListModifier->Empty() )
	{
		// û��Ӱ�죬ֱ�Ӽ���
		return TestEventTrigger(pTarget, pProto, NULL, dwEventMisc1, dwEventMisc2) &&
			   TestStateTrigger(pTarget, pProto, NULL);
	}
	else
	{
		// ���㴥����Ӱ��
		tagTriggerMod mod;

		TList<DWORD>::TListIterator it = pListModifier->Begin();
		DWORD dwSkillTypeID = GT_INVALID;

		while( pListModifier->PeekNext(it, dwSkillTypeID) )
		{
			const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(dwSkillTypeID);
			if( !P_VALID(pSkillProto) ) continue;

			mod.SetMod(pSkillProto);
		}

		// ������Ӱ�������ϣ���ʼ���㴥�������
		return TestEventTrigger(pTarget, pProto, &mod, dwEventMisc1, dwEventMisc2) &&
			   TestStateTrigger(pTarget, pProto, &mod);
	}
}

//----------------------------------------------------------------------------------------------------------
// ����ĳ�����������¼������Ƿ�����
//----------------------------------------------------------------------------------------------------------
BOOL Unit::TestEventTrigger(Unit* pTarget, const tagTriggerProto* pProto, const tagTriggerMod* pMod, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	// û���¼�Ҫ����ֱ�ӷ��سɹ�
	if( ETEE_Null == pProto->eEventType ) return TRUE;

	// ����Ҫ�жϴ����ʣ���ֱ�ӷ��سɹ�
	if (m_bTriggerOff == TRUE)
		return TRUE;

	// ���������Լ���
	INT nProp = pProto->nEventProp + ( P_VALID(pMod) ? pMod->nPropMod : 0 );
	if( IUTIL->Rand() % 10000 > nProp ) return FALSE;

	// ����������ĳЩ������¼������������жϣ�Ŀǰû����Ҫ�����жϵ�

	return TRUE;
}

//------------------------------------------------------------------------------------------------------------
// ����ĳ����������״̬�����Ƿ�����
//------------------------------------------------------------------------------------------------------------
BOOL Unit::TestStateTrigger(Unit* pTarget, const tagTriggerProto* pProto, const tagTriggerMod* pMod)
{
	if( ETEE_Null == pProto->eStateType ) return TRUE;

	DWORD dwStateMisc1 = pProto->dwStateMisc1 + ( P_VALID(pMod) ? pMod->nStateMisc1Mod : 0 );
	DWORD dwStateMisc2 = pProto->dwStateMisc2 + ( P_VALID(pMod) ? pMod->nStateMisc2Mod : 0 );

	// �鿴�����trigger���������ж�
	switch(pProto->eStateType)
	{
		// ��������
	case ETST_HPHigher:
		{
			INT nHPLimit = (INT)dwStateMisc1;

			if( nHPLimit < 100000 )
			{
				return GetAttValue(ERA_HP) > nHPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxHP) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_HP) / (FLOAT)GetAttValue(ERA_MaxHP) * 10000)
									>= (nHPLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// ��������
	case ETST_HPLower:
		{
			INT nHPLimit = (INT)dwStateMisc1;

			if( nHPLimit < 100000 )
			{
				return GetAttValue(ERA_HP) < nHPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxHP) > 0 )
				{
					return (INT)((FLOAT)GetAttValue(ERA_HP) / (FLOAT)GetAttValue(ERA_MaxHP) * 10000) 
										< (nHPLimit - 100000);
				}
				else
					return FALSE;
			}
		}
		break;

		// ��������
	case ETST_MPHigher:
		{
			INT nMPLimit = (INT)dwStateMisc1;

			if( nMPLimit < 100000 )
			{
				return GetAttValue(ERA_MP) > nMPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxMP) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_MP) / (FLOAT)GetAttValue(ERA_MaxMP) * 10000)
									>= (nMPLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// ��������
	case ETST_MPLower:
		{
			INT nMPLimit = (INT)dwStateMisc1;

			if( nMPLimit < 100000 )
			{
				return GetAttValue(ERA_MP) < nMPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxMP) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_MP) / (FLOAT)GetAttValue(ERA_MaxMP) * 10000)
									< (nMPLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// ŭ������
	case ETST_RageHigher:
		{
			INT nRageLimit = (INT)dwStateMisc1;
			return GetAttValue(ERA_Rage) > nRageLimit;
		}
		break;

		// ŭ������
	case ETST_RageLower:
		{
			INT nRageLimit = (INT)dwStateMisc1;
			return GetAttValue(ERA_Rage) < nRageLimit;
		}
		break;

		// �־�������
	case ETST_EnduranceHigher:
		{
			INT nEnduranceLimit = (INT)dwStateMisc1;

			if( nEnduranceLimit < 100000 )
			{
				return GetAttValue(ERA_Endurance) > nEnduranceLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxEndurance) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_Endurance) / (FLOAT)GetAttValue(ERA_MaxEndurance) * 10000)
									>= (nEnduranceLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// �־�������
	case ETST_EnduranceLower:
		{
			INT nEnduranceLimit = (INT)dwStateMisc1;

			if( nEnduranceLimit < 100000 )
			{
				return GetAttValue(ERA_Endurance) < nEnduranceLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxEndurance) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_Endurance) / (FLOAT)GetAttValue(ERA_MaxEndurance) * 10000)
									< (nEnduranceLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// ����ӵ��ĳbuff
	case ETST_SelfHaveBuff:
		{
			DWORD dwBuffID = dwStateMisc1;
			return IsHaveBuff(dwBuffID);
		}
		break;

		// Ŀ��ӵ��ĳbuff
	case ETST_TargetHaveBuff:
		{
			DWORD dwBuffID = dwStateMisc2;
			if( !P_VALID(pTarget) ) return FALSE;

			return pTarget->IsHaveBuff(dwBuffID);
		}
		break;

	default:
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------
// ��ӹ��ﵽ����ҳ�޵Ĺ����б�
//----------------------------------------------------------------------------------
VOID Unit::AddEnmityCreature(Unit *pUnit)
{
	if(FALSE == pUnit->IsCreature())
		return;

	m_mapEnmityCreature.Add(pUnit->GetID(), pUnit->GetID());
}

//----------------------------------------------------------------------------------
// ɾ������Ӹ���ҳ�޵Ĺ����б�
//----------------------------------------------------------------------------------
VOID Unit::DelEnmityCreature(Unit *pUnit)
{
	if(FALSE == pUnit->IsCreature())
		return;

	m_mapEnmityCreature.Erase(pUnit->GetID());
}

//----------------------------------------------------------------------------------
// �����ҳ�޵Ĺ����б�
//----------------------------------------------------------------------------------
VOID Unit::ClearEnmityCreature()
{
	DWORD	dwCreatureID = GT_INVALID;

	Creature* pCreature = (Creature*)GT_INVALID;
	m_mapEnmityCreature.ResetIterator();
	while (m_mapEnmityCreature.PeekNext(dwCreatureID))
	{
		pCreature = GetMap()->FindCreature(dwCreatureID);
		if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()))
			continue;

		pCreature->GetAI()->ClearEnmity(GetID());
	}

	m_mapEnmityCreature.Clear();
}

//----------------------------------------------------------------------------------
// ���հٷֱȸı���
//----------------------------------------------------------------------------------
VOID Unit::ChangeEnmityCreatureValue(DWORD dwRate)
{
	DWORD	dwCreatureID	=	GT_INVALID;
	INT		nEnmityMod		=	0;
	INT		nEnmityTotal	=	0;

	Creature* pCreature = (Creature*)GT_INVALID;
	m_mapEnmityCreature.ResetIterator();
	while (m_mapEnmityCreature.PeekNext(dwCreatureID))
	{
		pCreature = GetMap()->FindCreature(dwCreatureID);
		if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()))
			continue;

		nEnmityTotal = pCreature->GetAI()->GetBaseEnmityValue(GetID());
		nEnmityMod = (FLOAT)nEnmityTotal * (10000.0 - (FLOAT)dwRate) / 10000.0;
		pCreature->GetAI()->AddEnmity(this, -nEnmityMod, FALSE);
	}
}

FLOAT Unit::GetXZSpeed()
{
	if (this->IsRole())
	{
		Role* pRole = static_cast<Role*>(this);
		if (pRole->IsInRoleStateAny(ERS_Mount | ERS_Mount2))
		{
			return m_fMountXZSpeed;
		}
		else
		{
			return m_fXZSpeed;
		}
	}
	else
	{
		return m_fXZSpeed;
	}
}


//----------------------------------------------------------------------------------
// ����:ֻ�Թ���
//----------------------------------------------------------------------------------
BOOL Unit::HitFly(Unit* pTarget)
{
	if(!P_VALID(pTarget))
		return FALSE;

	// Ŀ���Ѿ�������ֱ�ӷ���
	if( pTarget->IsDead() )
		return FALSE;

	// ��8����ΪΪ���ɵ���Զ����
	FLOAT fDistAbs = FLOAT(8 * TILE_SCALE);	// ���Ծ���

	// �õ�����Ŀ��֮�������
	Vector3 vVec = GetCurPos() - pTarget->GetCurPos();

	// ������Һ͹�����ͬһ����
	if ( GetCurPos() == pTarget->GetCurPos())
	{

		//vDest = pTarget->GetCurPos() + fDistAbs;
		vVec = pTarget->GetFaceTo();
	}

	// �Ը��������й�һ��
	Vec3Normalize(vVec);

	// �õ��յ�����
	Vector3 vDest = pTarget->GetCurPos() + (-1) * vVec * fDistAbs;

	// �õ�һ����������յ�
	Vector3 vRealDest;
	NavCollider* pCollider = pTarget->GetMap()->GetNavMap()->GetCollider();

	Creature* pTargetCreature = static_cast<Creature*>(pTarget);

	// ����ײ��
	if( !pTargetCreature->NeedCollide() )
	{
		POINT nearPos;
		if( !pTargetCreature->GetMap()->IfCanDirectGo(pTargetCreature->GetCurPos().x, pTargetCreature->GetCurPos().z, vDest.x, vDest.z, &nearPos) )
		{
			vRealDest.x = FLOAT(nearPos.x * TILE_SCALE);
			vRealDest.z = FLOAT(nearPos.y * TILE_SCALE);
			vRealDest.y = pTargetCreature->GetMap()->GetGroundHeight(vRealDest.x, vRealDest.z);
		}
		else
		{
			vRealDest = vDest;
		}
	}
	// ��ײ��
	else
	{
		NavCollider_NPCMove collider;
		collider.IfCanGo(pCollider, pTarget->GetCurPos(), vDest, pTarget->GetSize(), &vRealDest);
	}

	// ��������㲻���,���ͻ�����Ϣ
	if( pTarget->GetCurPos() != vRealDest )
	{
		// ���ͻ��˷�����Ϣ
		tagNS_HitFly send;
		send.dwRoleID    = pTarget->GetID();				// Ŀ��ID
		send.curPos      = pTarget->GetCurPos();			// Ŀ�굱ǰ����
		send.destPos	 = vRealDest;						// Ŀ����յ�
		send.faceTo	     = vVec;							// ���ɵĳ���
		send.dwSkillID   = GetCombatHandler().GetSkillID();	// ʹ�ü���ID
		send.dwSrcRoleID = GetID();							// ������ID
		send.bCollide    = 1;								// ��Ҫ��ײ���

		// ˲�ƣ�����������Ϣ
		pTarget->GetMoveData().ForceTeleport(vRealDest, FALSE);

		pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
		
		// ������ǰ�ͷŵļ���
		pTarget->GetCombatHandler().End();
	}
	
	return TRUE;
}


//----------------------------------------------------------------------------------
// �����͹رպ�״̬���������ҵ����⴦��
//----------------------------------------------------------------------------------
BOOL Unit::OpenMacroToDealWithLuckState()
{
	// �Ƕ�������״̬
	if ( IsInStateInvisible() )
	{
		Map* pMap = GetMap();
		if( !P_VALID(pMap) )
			return FALSE;
		
		// �õ��Ź����ڵ��������
		tagVisTile* pVisTile[ED_End] = {0};

		// �����Χ�Ź���Ŀ��ӵ�ש
		pMap->GetVisTile( GetVisTileIndex(), pVisTile);
		

		// ͬ���������б������
		tagNS_RemoveRemote sendRemove;
		sendRemove.dwRoleID[0] = GetID();

		for(INT n = 0; n < ED_End; n++)
		{
			if( NULL == pVisTile[n] )
				continue;
			
			// �ҵ�ÿ����ש����
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			mapRole.ResetIterator();
			Role* pRole = NULL;

			while( mapRole.PeekNext(pRole) )
			{
				
				if( !P_VALID(pRole) )
					continue;

				// ���Ѳ�����
				if( pRole->CanSeeTargetEntirely(this) )
				{
					continue;
				}
				
				// ��ȫ͸������
				if( pRole->IsInVisDist(this) || !pRole->IsInVisDistForOpenMacre(this))
				{
					pRole->RemoveFromVisList(GetID());
					pRole->SendMessage(&sendRemove, sendRemove.dwSize);
				}
			}
		}
		

	}

	return  TRUE;
}

BOOL Unit::CloseMacroToDealWithLuckState()
{
	// �Ƕ�������״̬
	if ( IsInStateInvisible() )
	{
		Map* pMap = GetMap();
		if( !P_VALID(pMap) )
			return FALSE;

		BYTE	byMsg[1024] = {0};
		DWORD	dwSize = pMap->CalMovementMsgEx(this, byMsg, 1024);
		if( 0 == dwSize  ) return FALSE;


		// �õ��Ź����ڵ��������
		tagVisTile* pVisTile[ED_End] = {0};

		// �����Χ�Ź���Ŀ��ӵ�ש
		pMap->GetVisTile( GetVisTileIndex(), pVisTile);

		for(INT n = 0; n < ED_End; n++)
		{
			if( NULL == pVisTile[n] )
				continue;

			// �ҵ�ÿ����ש����
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			mapRole.ResetIterator();
			Role* pRole = NULL;

			while( mapRole.PeekNext(pRole) )
			{

				if( !P_VALID(pRole) )
					continue;

				// ���Ѳ�����
				if( pRole->CanSeeTargetEntirely(this) )
				{
					continue;
				}
				
				//  �ظ���͸��
				if(  pRole->IsInVisDist(this) || !pRole->IsInVisDistForOpenMacre(this))
				{
					pRole->Add2VisList(GetID());
					pRole->SendMessage(byMsg, dwSize);
				}
				
			}
		}
	
	}

	return TRUE;
}
