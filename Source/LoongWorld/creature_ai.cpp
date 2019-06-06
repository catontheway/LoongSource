//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: creature_ai.cpp
// author: Aslan
// actor:
// data: 2008-09-09
// last:
// brief: ��Ϸ������AI�࣬һ������״̬������������AI״̬
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "../WorldDefine/creature_define.h"
#include "../ServerDefine/NPCTeam_define.h"

#include "unit.h"
#include "role.h"
#include "creature.h"
#include "creature_ai.h"
#include "ai_transition.h"
#include "ai_trigger.h"
#include "path_finder.h"
#include "NPCTeam.h"

//------------------------------------------------------------------------------
// ����AI״̬
//------------------------------------------------------------------------------
AIStateIdle* AIStateIdle::Instance()
{
	static AIStateIdle instance;
	return &instance;
}

VOID AIStateIdle::OnEnter(AIController* pAI)
{
	pAI->ClearAllEnmity();
	pAI->SetEnterCombatTick(0);

	pAI->SetIsPatroling(FALSE);
	pAI->SetIsReversePatrol(FALSE);
	pAI->ReSetPatrolRestTick();
	pAI->ReSetLookForTargetTick();
}

VOID AIStateIdle::OnExit(AIController* pAI)
{
	AITriggerMgr* pAITrigger = pAI->GetAITriggerMgr();
	if( P_VALID(pAITrigger) )
		pAITrigger->SetTriggerActive(ETEE_DetectTarget);
}

VOID AIStateIdle::Update(AIController* pAI)
{
	// ����Ѳ��
	pAI->UpdatePatrol();

	// ��������
	pAI->UpdateLookForEnemy();
}

BOOL AIStateIdle::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}

//--------------------------------------------------------------------------------
// ׷��״̬
//--------------------------------------------------------------------------------
AIStatePursue* AIStatePursue::Instance()
{
	static AIStatePursue instance;
	return &instance;
}

VOID AIStatePursue::OnEnter(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	pAI->ReSetLockTargetTime();
	pAI->SetTargetUnitID(pAI->GetMaxEnmityUnitID());

	Unit* pUnit = pAI->GetOwner()->GetMap()->FindUnit(pAI->GetTargetUnitID());
	if( P_VALID(pUnit) )
	{
		pAI->StartPursue(pUnit);
	}
	else
	{
		pAI->SetPursueTargetPos(pAI->GetOwner()->GetCurPos());
	}

	if( 0 == pAI->GetEnterCombatTick() )
	{
		pAI->SetEnterCombatTick(g_world.GetWorldTick());
		pAI->SetEnterCombatPos(pAI->GetOwner()->GetCurPos());

		const CreatureScript *pScript = pOwner->GetScript();
		if (P_VALID(pScript))
		{
			pScript->OnEnterCombat(pOwner);
		}
	}
}

VOID AIStatePursue::OnExit(AIController* pAI)
{

}

VOID AIStatePursue::Update(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	// �������е���ʱ
	pAI->CountDownNextAttackWaitTick();
	BOOL bTargetChange = pAI->UpdateLockTarget();

	// ���û�ҵ���ֱ�ӷ���
	if( !P_VALID(pAI->GetTargetUnitID()) ) return;

	// �ҵ�������target
	Unit* pTarget = pAI->GetPursueTarget();
	if( !P_VALID(pTarget) )  return;

	// ���Ŀ��ı��ˣ������·���׷��
	if( bTargetChange )
	{
		pAI->StartPursue(pTarget);
	}
	else
	{
		pAI->UpdatePursue(pTarget);
	}
}

BOOL AIStatePursue::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}

//---------------------------------------------------------------------------------
// ����״̬
//---------------------------------------------------------------------------------
AIStateAttack* AIStateAttack::Instance()
{
	static AIStateAttack instance;
	return &instance;
}

VOID AIStateAttack::OnEnter(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	if( 0 == pAI->GetEnterCombatTick() )
	{
		pAI->SetEnterCombatTick(g_world.GetWorldTick());
		pAI->SetEnterCombatPos(pAI->GetOwner()->GetCurPos());

		const CreatureScript *pScript = pOwner->GetScript();
		if (P_VALID(pScript))
		{
			pScript->OnEnterCombat(pOwner);
		}
	}

	AITriggerMgr* pAITrigger = pAI->GetAITriggerMgr();
	if( P_VALID(pAITrigger) )
	{
		pAITrigger->SetTriggerActive(ETEE_InterCombatTime);
	}
}

VOID AIStateAttack::OnExit(AIController* pAI)
{

}

VOID AIStateAttack::Update(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	BOOL bTargetChange = pAI->UpdateLockTarget();

	CombatHandler& handler = pOwner->GetCombatHandler();

	// �����ǰ���ܲ����ͷ���
	if( !handler.IsValid() )
	{
		pAI->CountDownNextAttackWaitTick();

		if( P_VALID(pAI->GetTargetUnitID()) && pAI->GetNextAttackWaitTick() < 0 )
		{
			// �����ٴη���
			if( pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
			{
				INT nRet = pAI->AIUseSkill(pAI->GetAIUseSkillID(), pAI->GetTargetUnitID());

				if( E_Success == nRet )
					pOwner->OnAIAttack();
			}
		}
	}
}

BOOL AIStateAttack::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------------
AIStateFlee* AIStateFlee::Instance()
{
	static AIStateFlee instance;
	return &instance;
}

VOID AIStateFlee::OnEnter(AIController* pAI)
{
	pAI->SetFleeTime(EFT_FirstEnter);
}

VOID AIStateFlee::OnExit(AIController* pAI)
{
	pAI->SetIsArrivedFleePos(FALSE);
}

VOID AIStateFlee::Update(AIController* pAI)
{
	// �ݼ���������״̬��ʱ��
	pAI->CountDownNextFleeTick();

	if(50 > pAI->GetFleeTick())
		return;

	if( EFT_FirstEnter == pAI->GetFleeTime() )
	{
		pAI->CalFleePos();
		pAI->StartFlee(pAI->GetFleePos());

		pAI->SetFleeTime(EFT_Enter);
	}

	// �Ѿ��������ܵ�Ŀ���
	if( pAI->IsArrivedFleePos() )
	{
		pAI->CalFleePos();
		pAI->StartFlee(pAI->GetFleePos());
	}
	else
	{
		pAI->UpdateFlee();
	}
}

BOOL AIStateFlee::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------------
AIStateCallHelp* AIStateCallHelp::Instance()
{
	static AIStateCallHelp instance;
	return &instance;
}

VOID AIStateCallHelp::OnEnter(AIController* pAI)
{
	pAI->SetFleeTime(EFT_FirstEnter);
}

VOID AIStateCallHelp::OnExit(AIController* pAI)
{
	pAI->SetIsArrivedFleePos(FALSE);
}

VOID AIStateCallHelp::Update(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	// Ѱ�����Ŀ��
	if( EFT_FirstEnter == pAI->GetFleeTime() )
	{
		if( pAI->CalHelpPos() )
		{
			pAI->StartFlee(pAI->GetFleePos());
			pAI->SetFleeTime(EFT_Enter);
		}
		else
		{
			pAI->SetFleeTime(EFT_CallHelp);
		}
	}

	// �Ѿ��������ܵ�Ŀ���
	if( pAI->IsArrivedFleePos() )
	{
		if( EFT_CallHelp != pAI->GetFleeTime() )
		{
			pAI->SetFleeTime(EFT_CallHelp);
			if( P_VALID(pAI->GetHelperID()))
			{
				Creature* pCreature = pOwner->GetMap()->FindCreature(pAI->GetHelperID());
				if( P_VALID(pCreature) && !pCreature->IsDead() )
				{
					if( P_VALID(pCreature->GetAI()) )
					{
						Unit* pTarget = pOwner->GetMap()->FindRole(pAI->GetTargetUnitID());
						pCreature->GetAI()->AddEnmity(pTarget, 1);
					}
				}
			}
		}
	}
	else
	{
		if( pAI->NeedReCalHelpPos() )
		{
			if( pAI->CalHelpPos() )
				pAI->StartFlee(pAI->GetFleePos());
			else
				pAI->SetFleeTime(EFT_CallHelp);
		}
		pAI->UpdateFlee();
	}
}

BOOL AIStateCallHelp::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}


//------------------------------------------------------------------------------------
// ����״̬
//------------------------------------------------------------------------------------
AIStateReturn* AIStateReturn::Instance()
{
	static AIStateReturn instance;
	return &instance;
}

VOID AIStateReturn::OnEnter(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	pAI->StartReturn();
	pOwner->SetState(ES_Invincible);
	pOwner->SetTagged(GT_INVALID);
	pAI->ClearAllEnmity();
	pOwner->RemoveAllBuff(FALSE);
	pAI->SetTargetUnitID(GT_INVALID);
	pAI->SetFleeTime(EFT_NotEnter);

	const CreatureScript *pScript = pOwner->GetScript();
	if (P_VALID(pScript))
	{
		pScript->OnLeaveCombat(pOwner);
	}

	AITriggerMgr* pAITrigger = pAI->GetAITriggerMgr();
	if( P_VALID(pAITrigger) )
	{
		pAITrigger->Refresh();
	}
}

VOID AIStateReturn::OnExit(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	pOwner->SetAttValue(ERA_HP, pOwner->GetAttValue(ERA_MaxHP));
	pOwner->SetAttValue(ERA_MP, pOwner->GetAttValue(ERA_MaxMP));
	pOwner->UnsetState(ES_Invincible);
	pOwner->SetTagged(GT_INVALID);
}

VOID AIStateReturn::Update(AIController* pAI)
{
	pAI->UpdateReturn();
}

BOOL AIStateReturn::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ����״̬
//--------------------------------------------------------------------------------------
AIStateFollow* AIStateFollow::Instance()
{
	static AIStateFollow instance;
	return &instance;
}

VOID AIStateFollow::OnEnter(AIController* pAI)
{

}

VOID AIStateFollow::OnExit(AIController* pAI)
{

}

VOID AIStateFollow::Update(AIController* pAI)
{
	
}


BOOL AIStateFollow::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------
AIStateSpaceOut* AIStateSpaceOut::Instance()
{
	static AIStateSpaceOut instance;
	return &instance;
}

VOID AIStateSpaceOut::OnEnter(AIController* pAI)
{
	pAI->StartSpaceOut();
}

VOID AIStateSpaceOut::OnExit(AIController* pAI)
{
	pAI->SetIfArrivedPos(FALSE);
}

VOID AIStateSpaceOut::Update(AIController* pAI)
{
	pAI->UpdateSpaceOut();
}


BOOL AIStateSpaceOut::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	return TRUE;
}

//---------------------------------------------------------------------------------------
// �ű�״̬
//---------------------------------------------------------------------------------------
AIStateScript* AIStateScript::Instance()
{
	static AIStateScript instance;
	return &instance;
}

VOID AIStateScript::OnEnter(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	AITriggerMgr* pAITrigger = pAI->GetAITriggerMgr();
	if( P_VALID(pAITrigger) )
	{
		pAITrigger->Pause();		// ��ͣ������
	}

	const CreatureScript* pScript = pOwner->GetScript();
	if( P_VALID(pScript) ) pScript->OnEnterCurAI(pOwner);
}

VOID AIStateScript::OnExit(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	AITriggerMgr* pAITrigger = pAI->GetAITriggerMgr();
	if( P_VALID(pAITrigger) )
	{
		pAITrigger->UnPause();		// �ָ�������
	}

	const CreatureScript* pScript = pOwner->GetScript();
	if( P_VALID(pScript) ) pScript->OnLeaveCurAI(pOwner);
}

VOID AIStateScript::Update(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return;

	const CreatureScript* pScript = pOwner->GetScript();
	if( P_VALID(pScript) ) pScript->OnUpdateCurAI(pOwner);
}


BOOL AIStateScript::OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	Creature* pOwner = pAI->GetOwner();
	if( !P_VALID(pOwner) ) return FALSE;

	const CreatureScript* pScript = pOwner->GetScript();
	if( P_VALID(pScript) ) return pScript->OnEventCurAI(pOwner);
	
	return FALSE;
}

//---------------------------------------------------------------------------------------
// AI������
//---------------------------------------------------------------------------------------
AIController::AIController(Creature* pCreature, const tagMapWayPointInfoList* patrolList) : m_pOwner(pCreature), m_pCurrentState(NULL), 
												  m_pPreState(NULL), m_pGlobalState(NULL), m_pProto(NULL), m_pAIProto(NULL), m_pAITrigger(NULL), 
												  m_pScript(NULL), m_pTransition(NULL), m_nScriptUpdateTimer(GT_INVALID), m_nScriptUpdateCountDown(0), 
												  m_pWPArray(NULL), m_nWPNum(0), m_nWPIndex(0), m_bReversePatrol(FALSE), m_bIsPatroling(FALSE), 
												  m_nPatrolRestTick(0), m_nLookForTargetTick(0), m_bPursueFailed(FALSE), m_bCanNotPursue(FALSE), m_vPursueTargetPos(0.0f, 0.0f, 0.0f), 
												  m_dwTargeUnitID(GT_INVALID), m_nCheckAIEventTick(0), m_dwEnterCombatTick(0), m_dwUseSkillID(GT_INVALID),
												  m_nNextAttackWaitTick(0), m_dwSerial(0), m_vFleePos(0.0f, 0.0f, 0.0f), m_vPosEnterCombat(0.0f, 0.0f, 0.0f), m_eFleeTime(EFT_NotEnter),
												  m_dwMaxEnmityUnitID(GT_INVALID), m_nLockTargetTick(0), m_mapEnmity(), m_pPathFinder(NULL), m_dwFleeTick(0), m_bArrivedFleePos(FALSE),
												  m_dwSpaceOutTick(CREATURE_SPACE_OUT_MIN), m_bArrivedPos(FALSE)

{
	// ��ʼ����̬����
	m_pProto = m_pOwner->GetProto();

	// ��ʼ���ű�
	m_pScript = pCreature->GetScript();

	// ��ʼ��Ѳ��
	InitPatrol(patrolList);

	// ��ʼ��AI����
	InitAIProto();

	// ��ʼ��������������
	InitTriggerMgr();

	// ��ʼ��״̬������
	InitAITransition();

	// ��ʼ��Ѱ·
	InitPathFinder();

	// ��ʼ��ΪIdle״̬
	m_eCurAIState = AIST_Idle;
	m_pCurrentState = AIStateIdle::Instance();

	// ��ʼ��ս������
	ReSetPatrolRestTick();
	ReSetLookForTargetTick();
	ReSetLockTargetTime();
}

//-----------------------------------------------------------------------------------------
// ˢ��
//-----------------------------------------------------------------------------------------
VOID AIController::Refresh()
{
	// ��������
	m_eCurAIState				=	AIST_Idle;
	m_pCurrentState				=	AIStateIdle::Instance();
	m_pPreState					=	NULL;
	m_pGlobalState				=	NULL;
	m_nScriptUpdateTimer		=	GT_INVALID;
	m_nScriptUpdateCountDown	=	0;
	m_nWPIndex					=	0;
	m_bReversePatrol			=	FALSE;
	m_bIsPatroling				=	FALSE;
	m_nPatrolRestTick			=	0;
	m_nLookForTargetTick		=	0;
	m_vPursueTargetPos			=	Vector3(0.0f, 0.0f, 0.0f);
	m_bPursueFailed				=	FALSE;
	m_bCanNotPursue				=	FALSE;
	m_dwTargeUnitID				=	GT_INVALID;
	m_nCheckAIEventTick			=	0;
	m_dwEnterCombatTick			=	0;
	m_dwUseSkillID				=	GT_INVALID;
	m_nNextAttackWaitTick		=	0;
	m_dwSerial					=	0;
	m_vFleePos					=	Vector3(0.0f, 0.0f, 0.0f);
	m_vPosEnterCombat			=	Vector3(0.0f, 0.0f, 0.0f);
	m_dwMaxEnmityUnitID			=	GT_INVALID;
	m_nLockTargetTick			=	0;
	m_eFleeTime					=	EFT_NotEnter;
	m_bArrivedFleePos			=	FALSE;


	// ���³�ʼ������
	InitAIProto();

	// ��ʼ��������������
	InitTriggerMgr();

	// ��ʼ��״̬������
	InitAITransition();

	// ����ս������
	ReSetPatrolRestTick();
	ReSetLookForTargetTick();
	ReSetLockTargetTime();
}

//-----------------------------------------------------------------------------------------
// ��ʼ��Ѳ��
//-----------------------------------------------------------------------------------------
VOID AIController::InitPatrol(const tagMapWayPointInfoList* patrolList)
{
	if( !P_VALID(m_pProto) ) return;

	// ������Ѳ��
	if( ECPT_Path == m_pProto->ePatrolType && P_VALID(patrolList) && patrolList->list.Size() >= 1 )
	{
		INT nWPNum = patrolList->list.Size();

		m_pWPArray = new Vector3[nWPNum];
		m_nWPNum = nWPNum;
		m_nWPIndex = 0;

		tagWayPointInfo info;
		INT nIndex = 0;
		TList<tagWayPointInfo>::TListIterator it = patrolList->list.Begin();

		while( patrolList->list.PeekNext(it, info) )
		{
			m_pWPArray[nIndex] = info.vPos;
			++nIndex;
		}

		m_bReversePatrol = FALSE;
	}
	else
	{
		m_pWPArray = NULL;
		m_nWPNum = 0;
		m_nWPIndex = 0;
	}
}

//-----------------------------------------------------------------------------------------
// ��ʼ��AI��̬����
//-----------------------------------------------------------------------------------------
VOID AIController::InitAIProto()
{
	if( !P_VALID(m_pProto) ) return;

	// �õ���̬AI����
	switch(m_pProto->eAICreateType)
	{
	case EACT_Indicate:
		{
			// ָ����ʽ����ֱ�Ӹ��ݹ���AI��ID�õ�AI��̬����
			m_pAIProto = g_attRes.GetCreatureAI(m_pProto->dwAIID);
		}
		break;

	case EACT_Random:
		{
			// �����ʽ��������ĸ���������һ��AI
			m_pAIProto = g_attRes.RandGetCreatureAI();
		}
		break;

	case EACT_GroupRandom:
		{
			// �������ʽ������һ�����������ѡ��һ��AI
			m_pAIProto = g_attRes.RandGetCreatureAIInGroup(m_pProto->dwAIID);
		}
		break;
	}

	// �����AI��̬���ԣ������������Ը���������buff�ͼ���
	if( P_VALID(m_pAIProto) )
	{
		// ���ȸ��������buff
		for(INT n = 0; n < X_MAX_CREATURE_AI_BUFF; n++)
		{
			if( GT_INVALID == m_pAIProto->dwBuffTypeID[n] )
				break;

			// �ҵ�buff����
			const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(m_pAIProto->dwBuffTypeID[n]);
			if( !P_VALID(pBuffProto) ) continue;

			// ���buff
			m_pOwner->TryAddBuff(m_pOwner, pBuffProto, NULL, NULL, NULL);
		}

		// �ٸ�������Ӽ���
		for(INT n = 0; n < X_MAX_CREATURE_AI_SKILL; n++)
		{
			if( GT_INVALID == m_pAIProto->dwSkillTypeID[n] )
				break;

			// �ҵ��������Ժʹ���������
			const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(m_pAIProto->dwSkillTypeID[n]);

			if( !P_VALID(pSkillProto) ) continue;

			// ���ɼ���
			DWORD dwSkillID = Skill::GetIDFromTypeID(pSkillProto->dwID);
			INT nLevel = Skill::GetLevelFromTypeID(pSkillProto->dwID);
			Skill* pSkill = new Skill(dwSkillID, nLevel, 0, 0, 0);
			m_pOwner->AddSkill(pSkill);
		}
	}
}

//------------------------------------------------------------------------------------------
// ��ʼ��������������
//-------------------------------------------------------------------------------------------
VOID AIController::InitTriggerMgr()
{
	if( P_VALID(m_pAITrigger) ) SAFE_DEL(m_pAITrigger);

	if( P_VALID(m_pAIProto) )
	{
		m_pAITrigger = new AITriggerMgr;
		m_pAITrigger->Init(m_pOwner, m_pAIProto);
	}
}

//------------------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------------------
VOID AIController::InitAITransition()
{
	m_pTransition = GetTransitionByType();
}

//------------------------------------------------------------------------------------------
// ��ʼ��Ѱ·
//------------------------------------------------------------------------------------------
VOID AIController::InitPathFinder()
{
	if( m_pOwner->NeedCollide() )
		m_pPathFinder = new NPCPathFinderCol();
	else
		m_pPathFinder = new NPCPathFinder();

	m_pPathFinder->Init(GetOwner());
}

//------------------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------------------
VOID AIController::Destroy()
{
	SAFE_DEL(m_pAITrigger);
	SAFE_DEL(m_pWPArray);
	SAFE_DEL(m_pPathFinder);
}

//------------------------------------------------------------------------------------------
// ����AI״̬
//------------------------------------------------------------------------------------------
VOID AIController::Update()
{
	// ������ڲ��ܸ���AI��״̬���򷵻�
	if( IsInStateCantUpdateAI() ) return;

	// ����״̬��
	UpdateAIController();

	// �����л�
	UpdateTransition();

	// ���µ�ǰ״̬
	UpdateCurrentState();

	// ���´�����
	UpdateTriggerMgr();
}

//------------------------------------------------------------------------------------------
// �����¼�
//------------------------------------------------------------------------------------------
VOID AIController::OnEvent(Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */)
{
	// �����ǰ״̬�����ڣ�����ȫ��״̬����
	if( !P_VALID(m_pCurrentState) )
	{
		if( P_VALID(m_pGlobalState) )
		{
			m_pGlobalState->OnEvent(this, pSrc, nEventType, dwEventMisc1, dwEventMisc2);
		}
	}
	// ����ǰ״̬��������¼�������ȫ��״̬����
	else
	{
		if( FALSE == m_pCurrentState->OnEvent(this, pSrc, nEventType, dwEventMisc1, dwEventMisc2) )
		{
			if( P_VALID(m_pGlobalState) )
			{
				m_pGlobalState->OnEvent(this, pSrc, nEventType, dwEventMisc1, dwEventMisc2);
			}
		}
	}
}

//------------------------------------------------------------------------------------------
// ���ݹ����ƶ����͵õ�״̬ת����
//------------------------------------------------------------------------------------------
AITransition* AIController::GetTransitionByType()
{
	const tagCreatureProto* pProto = m_pOwner->GetProto();
	if( !P_VALID(pProto) ) return NULL;

	switch(pProto->eAIAction)
	{
	case AIAT_Attack:
		return AggressiveTransition::Instance();
		
	case AIAT_Guard:
		return GuardTransition::Instance();

	case AIAT_Barbette:
		return BarbetteTransition::Instance();

	case AIAT_Lamster:
		return LamsterTransition::Instance();

	case AIAT_CallHelp:
		return CallHelpTransition::Instance();

	case AIAT_SpaceOut:
		return SpaceOutTransition::Instance();

	default:
		return NULL;
	}
}

//--------------------------------------------------------------------------------------------
// ����Ѳ��
//--------------------------------------------------------------------------------------------
VOID AIController::UpdatePatrol()
{
	// ������ﱾ����ǲ����ģ�ֱ��return
	if( ECPT_Stand == m_pProto->ePatrolType || 
		ECPT_Null == m_pProto->ePatrolType )
		return;

	// �����ƶ�
	if( m_bIsPatroling )
	{
		if( EMS_Stand == m_pOwner->GetMoveData().m_eCurMove )
		{
			// �Ѿ������յ㣬��ԭ����Ϣһ��ʱ��
			m_bIsPatroling = FALSE;
			ReSetPatrolRestTick();
		}
		return;
	}

	// ������Ϣ
	if( m_nPatrolRestTick > 0 )
	{
		m_nPatrolRestTick--;
		return;
	}

	// ��ʼ��һ�ε�Ѳ��
	StartPatrol();
}

//---------------------------------------------------------------------------------------------
// ��ʼѲ��
//---------------------------------------------------------------------------------------------
VOID AIController::StartPatrol()
{
	switch(m_pProto->ePatrolType)
	{
		// վ����ľ׮
	case ECPT_Null:
	case ECPT_Stand:
	case ECPT_Wood:
		return;
		break;

		// ��Χ������ƶ� 
	case ECPT_Range:
		{
			Vector3 vDest = m_pOwner->GetCurPos();

			INT nRange = m_pProto->nPatrolRadius * 2;
			if( 0 >= nRange ) return;

			vDest.x = FLOAT(IUTIL->Rand() % nRange - m_pProto->nPatrolRadius) + m_pOwner->GetBornPos().x;
			vDest.z = FLOAT(IUTIL->Rand() % nRange - m_pProto->nPatrolRadius) + m_pOwner->GetBornPos().z;

			if( MoveData::EMR_Success == m_pOwner->GetMoveData().StartCreatureWalk(vDest, EMS_CreaturePatrol) )
			{
				m_bIsPatroling = TRUE;
			}
		}
		break;

		// ����·��
	case ECPT_Path:
		{
			if( !P_VALID(m_pWPArray) || m_nWPNum <= 1 ) return;

			// �ȼ��һ�µ�ǰ����������
			if( !m_bReversePatrol && (m_nWPIndex == m_nWPNum - 1) )
			{
				// ����Ѳ���Ѿ��������յ㣬���۷�
				m_bReversePatrol = !m_bReversePatrol;
			}
			else if( m_bReversePatrol && 0 == m_nWPIndex )
			{
				// ����Ѳ���Ѿ��������յ㣬���۷�
				m_bReversePatrol = !m_bReversePatrol;
			}

			// ȡ���������������һ����
			if( m_bReversePatrol )
				--m_nWPIndex;
			else
				++m_nWPIndex;

			// ȡ����һ����
			Vector3 vDest = m_pWPArray[m_nWPIndex];

			if( m_pOwner->IsTeam() )
			{
				NPCTeam* pTeam = m_pOwner->GetTeamPtr();
				if( P_VALID(pTeam) )
					pTeam->OrderTeammateMove(m_pOwner, vDest);

				return;
			}

			if( MoveData::EMR_Success == m_pOwner->GetMoveData().StartCreatureWalk(vDest, EMS_CreaturePatrol) )
			{
				m_bIsPatroling = TRUE;
			}
		}
		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------------
// ������Ϊ���� 0-��Ϣ 1-û���ҵ����� 2-�ҵ�����
//------------------------------------------------------------------------------------
DWORD AIController::UpdateLookForEnemy()
{
	// ���������Ϣ
	if( m_nLookForTargetTick > 0 )
	{
		m_nLookForTargetTick--;
		return 0;
	}

	// �����Ϣʱ�䵽�ˣ���ʼ����
	ReSetLookForTargetTick();

	return StartLookForEnemy();
}

//------------------------------------------------------------------------------------
// ��ʼ���� 0-��Ϣ 1-û���ҵ����� 2-�ҵ�����
//------------------------------------------------------------------------------------
DWORD AIController::StartLookForEnemy()
{
	tagVisTile* pVisTile[ED_End] = {0};
	
	// ����������ֵ
	FLOAT fMaxDist = (FLOAT)m_pOwner->GetAttValue(ERA_Inspiration);
	
	// �����ֵ,��������ֱ�ӷ��� 
	if( fMaxDist <= 0.0f  ) return 0;

	FLOAT fMaxDistSQ = fMaxDist * fMaxDist;

	// �õ���Ұ��Χ�ڵ�vistile�б�
	m_pOwner->GetMap()->GetVisTile(m_pOwner->GetMoveData().m_vPos, fMaxDist, pVisTile);

	Role* pBestRole = NULL;
	FLOAT fNearestDistSq = FLOAT_MAX;

	// ����ط��������Ż�һ�£������vistile���ҵ�����һ���������
	for(INT n = ED_Center; n < ED_End; n++)
	{
		if( !P_VALID(pVisTile[n]) ) continue;

		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;

		TMap<DWORD, Role*>::TMapIterator it = mapRole.Begin();
		Role* pRole = NULL;

		while( mapRole.PeekNext(it, pRole) )
		{
			// �������ﲻ���
			if( pRole->IsDead() ) continue;

			// �Ƿ��޵�
			if( pRole->IsInState(ES_Invincible)) continue;

			// ���ǵ��˲����
			if( m_pOwner->FriendEnemy(pRole) != ETFE_Hostile ) continue;

			// ������
			FLOAT fDistSQ = Vec3DistSq(m_pOwner->GetCurPos(), pRole->GetCurPos());
			

			// ������������Ŀ������з�Χ ���з�Χ <= (������� - �������ֵ)
			FLOAT fLurkDist = fMaxDist - pRole->GetAttValue(ERA_Lurk);
			if( fLurkDist <= 100.0f  )
				fLurkDist = 100.0f;
			FLOAT fLurkDistSQ = fLurkDist * fLurkDist;

			// �������С��֮ǰ����ľ���
			if( fDistSQ > fNearestDistSq || fDistSQ > fLurkDistSQ )
				continue;

			// �鿴�Ƿ�ɴ�
			if( MoveData::EMR_Success == m_pOwner->GetMoveData().IsCanCreatureWalk(pRole->GetCurPos(), EMS_CreatureWalk, TRUE) )
			{
				pBestRole = pRole;
				fNearestDistSq = fDistSQ;
			}
		}
	}

	// �ҵ�
	if( P_VALID(pBestRole) )
	{
		AddEnmity(pBestRole, 1);
		return 2;
	}

	// û���ҵ�
	ReSetLookForTargetTick();
	return 1;

}

//-----------------------------------------------------------------------------
// ����׷����Ŀ��
//-----------------------------------------------------------------------------
Unit* AIController::GetPursueTarget()
{
	if( P_VALID(m_dwTargeUnitID) )
	{
		if( IS_PLAYER(m_dwTargeUnitID) )
		{
			return m_pOwner->GetMap()->FindRole(m_dwTargeUnitID);
		}
		else if( IS_CREATURE(m_dwTargeUnitID) )
		{
			return m_pOwner->GetMap()->FindCreature(m_dwTargeUnitID);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
// �ҵ���Ŀ���ٽ��ĵ㣬����Ҳ���������FALSE
//------------------------------------------------------------------------------
BOOL AIController::GetPosNearPursueTarget(Unit* pTarget, Vector3& vPos)
{
	if( !P_VALID(pTarget) )
		return FALSE;

	// ����Ŀ��λ��
	const Vector3& vTargetPos = pTarget->GetCurPos();

	// �����ά����
	FLOAT fXDist = vTargetPos.x - m_pOwner->GetCurPos().x;
	FLOAT fZDist = vTargetPos.z - m_pOwner->GetCurPos().z;

	FLOAT fXDistABS = abs(fXDist);
	FLOAT fZDistABS = abs(fZDist);

	if( fXDistABS < 0.0001f && fZDistABS < 0.0001f )
	{
		vPos = vTargetPos;
		return TRUE;
	}

	// ����ȡ��x�����z����ľ������
	FLOAT fX = (fXDistABS / (fXDistABS + fZDistABS)) * (CREATURE_PURSUE_NEAR_DIST + pTarget->GetSize().z + m_pOwner->GetSize().z);
	FLOAT fZ = (fZDistABS / (fXDistABS + fZDistABS)) * (CREATURE_PURSUE_NEAR_DIST + pTarget->GetSize().z + m_pOwner->GetSize().z);

	if(fX > fXDistABS && fZ > fZDistABS)
	{
		vPos = vTargetPos;
		return TRUE;
	}

	// ȡ��Ŀ���
	vPos.x = ( (fXDist > 0) ? vTargetPos.x - fX : vTargetPos.x + fX );
	vPos.z = ( (fZDist > 0) ? vTargetPos.z - fZ : vTargetPos.z + fZ );

	// ȡ��y����
	if( !m_pOwner->NeedCollide() )
	{
		vPos.y = m_pOwner->GetMap()->GetGroundHeight(vPos.x, vPos.z);
	}
	else
	{
		vPos.y = vTargetPos.y;
	}

	return TRUE;	
}

//------------------------------------------------------------------------------
// ���Ŀ���ǲ����ڹ�����Χ֮��
//------------------------------------------------------------------------------
BOOL AIController::IsInAttackDist(DWORD dwTargetUnitID)
{
	m_dwUseSkillID = GT_INVALID;

	// ������Ŀ�겻�Ϸ����򷵻ش���
	if( !IsTargetValid(dwTargetUnitID) ) return FALSE;

	// �õ�Ŀ��
	Unit* pTarget = m_pOwner->GetMap()->FindUnit(dwTargetUnitID);
	if( !P_VALID(pTarget) ) return FALSE;

	// �����ͨ������Զ�̹���
	Skill* pMeleeSkill = m_pOwner->GetMeleeSkill();
	Skill* pRangedSkill = m_pOwner->GetRangedSkill();

	// �ȼ����̹���
	if( P_VALID(pMeleeSkill) )
	{
		if( m_pOwner->IsInCombatDistance(*pTarget, pMeleeSkill->GetOPDist()) )
		{
			m_dwUseSkillID = pMeleeSkill->GetID();
			return TRUE;
		}
	}

	// �ټ��Զ�̹���
	if( P_VALID(pRangedSkill) )
	{
		if( m_pOwner->IsInCombatDistance(*pTarget, pRangedSkill->GetOPDist()) )
		{
			m_dwUseSkillID = pRangedSkill->GetID();
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ׷��Ŀ��
//------------------------------------------------------------------------------
VOID AIController::StartPursue(Unit* pTarget)
{
	if( !P_VALID(pTarget) ) return;

	// ֹͣѰ·������һЩ״̬
	m_bPursueFailed		=	FALSE;
	m_bCanNotPursue		=	FALSE;
	m_bPathFinding		=	FALSE;
	m_nCheckPursueTick	=	IUTIL->Rand() % 6 + 5;

	// �ȼ��ֱ��
	Vector3 vNearPos;
	MoveData::EMoveRet eRet = m_pOwner->GetMoveData().IsCanCreatureWalk(pTarget->GetCurPos(), EMS_CreatureWalk, TRUE, &vNearPos);

	// �ɴ�
	if( MoveData::EMR_Success == eRet )
	{
		m_pOwner->GetMoveData().StartCreatureWalk(pTarget->GetCurPos(), EMS_CreatureWalk, FALSE);
	}
	// ���ڲ����ƶ���״̬
	else if( MoveData::EMR_SelfLimit == eRet )
	{
		m_bCanNotPursue = TRUE;
	}
	// ���ɴ�
	else if( MoveData::EMR_NoArrive == eRet )
	{
		// �������ľ����ǿ��Թ����ľ��룬��ֱ�ӹ�ȥ
		Skill* pMeleeSkill	=	m_pOwner->GetMeleeSkill();
		Skill* pRangedSkill	=	m_pOwner->GetRangedSkill();

		FLOAT fMaxDist = 0.0f;

		if( P_VALID(pMeleeSkill) )
		{
			if( pMeleeSkill->GetOPDist() > fMaxDist )
				fMaxDist = pMeleeSkill->GetOPDist();
		}
		if( P_VALID(pRangedSkill) )
		{
			if( pRangedSkill->GetOPDist() > fMaxDist )
				fMaxDist = pRangedSkill->GetOPDist();
		}

		// �õ�·�����������Ŀ��ľ���
		FLOAT fDist = Vec3Dist(vNearPos, pTarget->GetCurPos());
		Vector3 vNearPosTop = vNearPos;
		vNearPosTop.y += m_pOwner->GetSize().y;

		if( fDist < fMaxDist && !m_pOwner->GetMap()->IsRayCollide(vNearPosTop, pTarget->GetCurPosTop()) )
		{
			m_pOwner->GetMoveData().StartCreatureWalk(vNearPos, EMS_CreatureWalk, FALSE);
		}
		else
		{
			// Ѱ·
			Vector3 vPos;
			BOOL bRet = m_pPathFinder->FindingPath(m_pOwner->GetCurPos(), pTarget->GetCurPos());
			if( bRet && m_pPathFinder->GetCurPathPoint(m_pOwner->GetCurPos(), vPos) )
			{
				m_bPathFinding = TRUE;
				m_pOwner->GetMoveData().StartCreatureWalk(vPos, EMS_CreatureWalk, FALSE);
			}
			else
			{
				m_bPursueFailed = TRUE;
			}
		}
	}
	// ����ԭ��
	else
	{
		m_bPursueFailed = TRUE;
	}

	if( !m_bPursueFailed )
	{
		SetPursueTargetPos(pTarget->GetCurPos());
	}
}

//---------------------------------------------------------------------------------
// ����׷��
//---------------------------------------------------------------------------------
VOID AIController::UpdatePursue(Unit* pTarget)
{
	// ����׷������ʱ
	--m_nCheckPursueTick;
	++m_nNoAnmityTick;

	// ���׷��ʧ�ܣ���ȡ����б����������Ŀ��
	if( m_bPursueFailed )
	{
		SetEnmityActive(pTarget->GetID(), FALSE);
		SetTargetUnitID(GetMaxEnmityUnitID());

		if( P_VALID(GetTargetUnitID()) )
		{
			pTarget = m_pOwner->GetMap()->FindUnit(GetTargetUnitID());
			if( P_VALID(pTarget) )
			{
				StartPursue(pTarget);
			}
		}
		return;
	}

	// ���׷��״̬
	if( m_bCanNotPursue )
	{
		if( !m_pOwner->IsInStateCantMove() )
		{
			StartPursue(pTarget);
		}
		return;
	}
	else
	{
		if( m_pOwner->IsInStateCantMove() )
		{
			m_bCanNotPursue = TRUE;
			return;
		}
	}

	// �����ʱ��û�����ӳ�ޣ���׷��ʧ��
	const tagCreatureProto* pProto = pProto = m_pOwner->GetProto();
	if( !P_VALID(pProto) || m_nNoAnmityTick > pProto->nMaxPursueTick )
	{
		m_bPursueFailed = TRUE;
		return;
	}

	// ����Ƿ��Ѿ�ͣ������
	if( EMS_Stand == m_pOwner->GetMoveData().GetCurMoveState() )
	{
		if( m_pOwner->GetMoveData().IsStopped() )
		{
			// ���ƶ��ж�
			StartPursue(pTarget);
		}
		else
		{
			// ����ǿ��ͣ�£�˵��������
			if( m_bPathFinding )
			{
				// Ѱ·��
				Vector3 vNewPos;
				if( m_pPathFinder->GetCurPathPoint(m_pOwner->GetCurPos(), vNewPos) )
				{
					m_pOwner->GetMoveData().StartCreatureWalk(vNewPos, EMS_CreatureWalk, FALSE);
					return;
				}
				else
				{
					StartPursue(pTarget);
					return;
				}
			}
			else
			{
				// ��Ѱ·��
				StartPursue(pTarget);
				return;
			}
		}
	}

	// ������¼�⵹��ʱ�ǲ����Ѿ�����
	if( m_nCheckPursueTick < 0 )
	{
		m_nCheckPursueTick = IUTIL->Rand() % 6 + 5;
		if( abs(pTarget->GetCurPos().x - GetPursueTargetPos().x) > 50.0f || 
			abs(pTarget->GetCurPos().z - GetPursueTargetPos().z) > 50.0f )
		{
			StartPursue(pTarget);
		}
	}
}

//---------------------------------------------------------------------------------
// ʹ��AI����
//---------------------------------------------------------------------------------
INT AIController::AIUseSkill(DWORD dwSkillID, DWORD dwTargetUnitID)
{
	INT nRet = m_pOwner->GetCombatHandler().UseSkill(dwSkillID, dwTargetUnitID, m_dwSerial);

	if( E_Success == nRet )
	{
		Skill* pSkill = m_pOwner->GetSkill(dwSkillID);
		ASSERT( P_VALID(pSkill) );

		m_pOwner->GetMoveData().StopMove();
		m_dwUseSkillID = dwSkillID;

		tagNS_Skill send;
		send.dwSrcRoleID = m_pOwner->GetID();
		send.dwTarRoleID = dwTargetUnitID;
		send.dwSkillID = pSkill->GetTypeID();
		send.nSpellTime = m_pOwner->GetCombatHandler().GetSkillPrepareCountDown();
		send.dwSerial = m_dwSerial;
		send.dwErrorCode = E_Success;

		if( P_VALID(m_pOwner->GetMap()) )
			m_pOwner->GetMap()->SendBigVisTileMsg(m_pOwner, &send, send.dwSize);

		// �ۼ����к�
		++m_dwSerial;
	}
	else
	{
	}

	return nRet;
}

//----------------------------------------------------------------------------------
// ���ӳ��
//----------------------------------------------------------------------------------
VOID AIController::AddEnmity(Unit *pUnit, INT nValue, BOOL bSyncTeam)
{
	if( !P_VALID(pUnit) || pUnit->IsDead())
		return;

	tagEnmity* pEnmity = m_mapEnmity.Peek(pUnit->GetID());
	if( P_VALID(pEnmity) )
	{
		pEnmity->nEnmity += nValue;
		if( !pEnmity->bActive ) pEnmity->bActive = TRUE;
	}
	else
	{
		pEnmity = new tagEnmity;

		pUnit->AddEnmityCreature(m_pOwner);
		pEnmity->dwID = pUnit->GetID();
		pEnmity->nEnmity += nValue;
		m_mapEnmity.Add(pUnit->GetID(), pEnmity);
	}

	// ���������
	CalMaxEnmity();

	// ���ó���޸ı�ʱ��
	m_nNoAnmityTick = 0;

	// ����Ǽ��ٳ�ޣ��������е���ʱ
	if( nValue < 0 )
		m_nLockTargetTick = 0;

	if( bSyncTeam )
	{
		// ͬ��С�ӳ��
		NPCTeam* pTeam = m_pOwner->GetTeamPtr();
		if(P_VALID(pTeam))
			pTeam->OnEvent(NTE_SynEnmity, pUnit, 1);
	}
}

//----------------------------------------------------------------------------------
// ���ӳ��ֵ�ӳ�
//----------------------------------------------------------------------------------
VOID AIController::AddEnmityMod(Unit *pUnit, INT nValue, BOOL bSyncTeam)
{
	if( !P_VALID(pUnit) || pUnit->IsDead())
		return;

	tagEnmity* pEnmity = m_mapEnmity.Peek(pUnit->GetID());
	if( P_VALID(pEnmity) )
	{
		pEnmity->nEnmityMod += nValue;
	}
	else
	{
		pEnmity = new tagEnmity;

		pUnit->AddEnmityCreature(m_pOwner);
		pEnmity->dwID = pUnit->GetID();
		pEnmity->nEnmity += 1;
		pEnmity->nEnmityMod += nValue;
		m_mapEnmity.Add(pUnit->GetID(), pEnmity);
	}

	// ���������
	CalMaxEnmity();

	// ���ó���޸ı�ʱ��
	m_nNoAnmityTick = 0;

	if( bSyncTeam )
	{
		// ͬ��С�ӳ��
		NPCTeam* pTeam = m_pOwner->GetTeamPtr();
		if(P_VALID(pTeam))
			pTeam->OnEvent(NTE_SynEnmity, pUnit, 1);
	}
}

//----------------------------------------------------------------------------------
// ��չ�����
//----------------------------------------------------------------------------------
VOID AIController::ClearAllEnmity()
{
	tagEnmity*	pEnmity		=	(tagEnmity*)GT_INVALID;
	DWORD		dwID		=	GT_INVALID;

	m_mapEnmity.ResetIterator();
	while( m_mapEnmity.PeekNext(dwID, pEnmity) )
	{
		if( P_VALID(pEnmity) )
		{
			Unit *pUnit = m_pOwner->GetMap()->FindUnit(pEnmity->dwID);
			if( P_VALID(pUnit) )
				pUnit->DelEnmityCreature(m_pOwner);
		}
		SAFE_DEL(pEnmity);
	}

	m_mapEnmity.Clear();

	m_dwMaxEnmityUnitID = GT_INVALID;
}

//----------------------------------------------------------------------------------
// ɾ��һ����ҵĳ��
//----------------------------------------------------------------------------------
VOID AIController::ClearEnmity(DWORD dwRoleID)
{
	tagEnmity *pEnmity = m_mapEnmity.Peek(dwRoleID);
	if( P_VALID(pEnmity) )
	{
		m_mapEnmity.Erase(dwRoleID);
		SAFE_DEL(pEnmity);
	}

	CalMaxEnmity();
}

//----------------------------------------------------------------------------------
// ����һ����ҵĳ�޼���
//----------------------------------------------------------------------------------
VOID AIController::SetEnmityActive(DWORD dwRoleID, BOOL bActive)
{
	tagEnmity *pEnmity = m_mapEnmity.Peek(dwRoleID);
	if( P_VALID(pEnmity) )
	{
		pEnmity->bActive = bActive;
	}

	CalMaxEnmity();
}

//----------------------------------------------------------------------------------
// ����������Ŀ��
//----------------------------------------------------------------------------------
VOID AIController::CalMaxEnmity()
{
	INT nMaxEnmity = 0;

	m_dwMaxEnmityUnitID		=	GT_INVALID;
	tagEnmity* pEnmity		=	(tagEnmity*)GT_INVALID;

	TMap<DWORD, tagEnmity*>::TMapIterator it = m_mapEnmity.Begin();
	while( m_mapEnmity.PeekNext(it, pEnmity) )
	{
		if( !P_VALID(pEnmity) || !pEnmity->bActive )
			continue;

		if( !IsTargetValid(pEnmity->dwID) )
		{
			pEnmity->bActive = FALSE;
			continue;
		}

		if( pEnmity->nEnmity + pEnmity->nEnmityMod > nMaxEnmity )
		{
			nMaxEnmity = pEnmity->nEnmity + pEnmity->nEnmityMod;
			m_dwMaxEnmityUnitID = pEnmity->dwID;
		}
	}
}

//----------------------------------------------------------------------------------
// ����Ŀ���Ƿ�Ϸ�
//----------------------------------------------------------------------------------
BOOL AIController::IsTargetValid(DWORD dwID)
{
	Unit *pUnit = m_pOwner->GetMap()->FindUnit(dwID);
	if( !P_VALID(pUnit) )
		return FALSE;

	// �Ƿ�����
	if( pUnit->IsDead() )
		return FALSE;

	// �Ƿ�����
	if( pUnit->IsInState(ES_Lurk) ) 
		return FALSE;

	// �Ƿ��޵�
	if( pUnit->IsInState(ES_Invincible) && !IsEnmityListAllInvincible() )
		return FALSE;

	Map* pMap = pUnit->GetMap();
	if( pMap != m_pOwner->GetMap() )
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------------------
// �������е���ʱ
//----------------------------------------------------------------------------------
BOOL AIController::UpdateLockTarget()
{
	// �Ƿ��л���Ŀ��
	BOOL bTargetChange = FALSE;

	// �ҵ�Ŀ��
	while( P_VALID(GetTargetUnitID()) )
	{
		if( !IsTargetValid(GetTargetUnitID()) )
		{
			SetEnmityActive(GetTargetUnitID(), FALSE);
			SetTargetUnitID(GetMaxEnmityUnitID());
			ReSetLockTargetTime();
			bTargetChange = TRUE;
			continue;
		}
		else
		{
			break;
		}
	}

	// ���û���л�Ŀ��
	if( !bTargetChange )
	{
		// �Ѿ�����Ŀ��
		if( m_nLockTargetTick > 0 )
		{
			m_nLockTargetTick--;
		}
		else
		{
			// �����������ʱ�����������¿�ʼ����
			ReSetLockTargetTime();

			if( GetTargetUnitID() != GetMaxEnmityUnitID() )
			{
				// ��������޵�Ŀ��λ��ǰĿ��
				SetTargetUnitID(GetMaxEnmityUnitID());
				bTargetChange = TRUE;
			}
		}
	}

	return bTargetChange;
}

//----------------------------------------------------------------------------------
// ����Ŀ�����ͻ��Ŀ��ID
//----------------------------------------------------------------------------------
DWORD AIController::GetTargetIDByType(ECreatureTargetFriendEnemy eTargetType)
{
	DWORD dwTargetID = GT_INVALID;
	switch(eTargetType)
	{
	case ECFE_NULL:
		return dwTargetID;

	case ECFE_Self:
		return m_pOwner->GetID();

	case ECFE_Leader:
		{
			NPCTeam* pTeam = m_pOwner->GetTeamPtr();
			if( P_VALID(pTeam) )
			{
				dwTargetID = pTeam->GetLeaderID();
			}
			return dwTargetID;
		}

	case ECFE_RandPlayer:
		{
			dwTargetID = RandRoleInEnmityList();
			return dwTargetID;
		}
	case ECFE_MinEnimty:
		{
			dwTargetID = GetMinEnmityInEnmityList();
			return dwTargetID;
		}
	case ECFE_CurPlayer:
		{
			return m_dwTargeUnitID;
		}
	default:
		return dwTargetID;
	}
}


//----------------------------------------------------------------------------------
// �������б��������ȡһ��Ŀ��
//----------------------------------------------------------------------------------
DWORD AIController::RandRoleInEnmityList()
{
	tagEnmity*	pEnmity	=	NULL;
	DWORD		dwID	=	GT_INVALID;

	if( m_mapEnmity.Empty() ) return GT_INVALID;

	m_mapEnmity.RandPeek(dwID, pEnmity);
	return dwID;
}

//----------------------------------------------------------------------------------
// �������б��г����СĿ��
//----------------------------------------------------------------------------------
DWORD AIController::GetMinEnmityInEnmityList()
{
	tagEnmity*	pEnmity		=	NULL;
	DWORD		dwID		=	GT_INVALID;
	DWORD		dwTargetID	=	GT_INVALID;
	INT			nValue		=	INT_MAX;

	TMap<DWORD, tagEnmity*>::TMapIterator it = m_mapEnmity.Begin();

	while( m_mapEnmity.PeekNext(it, dwID, pEnmity) )
	{
		if( pEnmity->nEnmity + pEnmity->nEnmityMod < nValue )
		{
			nValue = pEnmity->nEnmity + pEnmity->nEnmityMod;
			dwTargetID = dwID;
		}
	}

	return dwTargetID;
}

//----------------------------------------------------------------------------------
// �õ������ֵ
//----------------------------------------------------------------------------------
INT AIController::GetEnmityValue(DWORD dwID)
{
	tagEnmity* pEnmity = m_mapEnmity.Peek(dwID);
	if( P_VALID(pEnmity) )	return pEnmity->nEnmity + pEnmity->nEnmityMod;

	return 0;
}

//----------------------------------------------------------------------------------
// �õ��������ֵ
//----------------------------------------------------------------------------------
INT AIController::GetBaseEnmityValue(DWORD dwID)
{
	tagEnmity* pEnmity = m_mapEnmity.Peek(dwID);
	if( P_VALID(pEnmity) ) return pEnmity->nEnmity;

	return 0;
}

//----------------------------------------------------------------------------------
// ���ó�޼�ֵ
//----------------------------------------------------------------------------------
VOID AIController::ClearEnmityModValue(DWORD dwID)
{
	tagEnmity* pEnmity = m_mapEnmity.Peek(dwID);
	if( P_VALID(pEnmity) ) 
	{
		pEnmity->nEnmityMod = 0;
		CalMaxEnmity();
	}
}

//----------------------------------------------------------------------------------
// ����õ�һ���������ܵ�Ŀ���
//----------------------------------------------------------------------------------
VOID AIController::CalFleePos()
{
	// ��һ��ѡ������Ŀ���
	if(EFT_FirstEnter == m_eFleeTime)
	{
		INT	  nIndex = 0;
		while(TRUE)
		{
			nIndex += 1;

			// ����õ�һ�����ܵľ���
			m_vFleeDir.fx = (FLOAT)(IUTIL->Rand() % X_RANGE_FLEE_RADIUS) + X_MIN_FLEE_RADIUS;
			m_vFleeDir.fy = 0.0f;

			// ����õ����ܵķ�������
			FLOAT fAng = (FLOAT)(IUTIL->Rand() % 360);

			// ��������Ŀ���
			m_vFleeDir.Vec2DRotateAroundOrigin(fAng*3.1415927f / 180.0f);

			m_vFleePos.x = m_vFleeDir.fx + m_pOwner->GetCurPos().x;
			m_vFleePos.z = m_vFleeDir.fy + m_pOwner->GetCurPos().z;

			if(m_pOwner->NeedCollide())
			{
				m_vFleePos.y = m_pOwner->GetCurPosTop().y;
				break;
			}
			else
			{
				if( m_pOwner->GetMap()->IfCanGo(m_vFleePos.x, m_vFleePos.z) )
				{
					m_vFleePos.y = m_pOwner->GetMap()->GetGroundHeight(m_vFleePos.x, m_vFleePos.z);
					break;
				}
			}

			if (nIndex > 10)	break;
		}
	}
	else
	{
		INT	  nIndex = 0;
		while(TRUE)
		{
			nIndex += 1;

			// ����õ����ܵķ�������
			FLOAT fAng = (FLOAT)(IUTIL->Rand() % 160) - 80.0f;

			// ��������Ŀ���
			m_vFleeDir.Vec2DRotateAroundOrigin(fAng*3.1415927f / 180.0f);

			m_vFleePos.x = m_vFleeDir.fx + m_pOwner->GetCurPos().x;
			m_vFleePos.z = m_vFleeDir.fy + m_pOwner->GetCurPos().z;

			if(m_pOwner->NeedCollide())
			{
				m_vFleePos.y = m_pOwner->GetCurPosTop().y;
				break;
			}
			else
			{
				if( m_pOwner->GetMap()->IfCanGo(m_vFleePos.x, m_vFleePos.z) )
				{
					m_vFleePos.y = m_pOwner->GetMap()->GetGroundHeight(m_vFleePos.x, m_vFleePos.z);
					break;
				}
			}

			if (nIndex > 10)	break;
		}
	}
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID AIController::StartFlee(Vector3 vFleePos)
{
	m_bArrivedFleePos	=	FALSE;
	m_bPursueFailed		=	FALSE;
	m_bCanNotPursue		=	FALSE;
	m_bPathFinding		=	FALSE;

	// �ȼ��ֱ��
	Vector3 vNearPos;
	MoveData::EMoveRet eRet = m_pOwner->GetMoveData().IsCanCreatureWalk(vFleePos, EMS_CreatureFlee, TRUE, &vNearPos);

	// �ɴ�
	if( MoveData::EMR_Success == eRet )
	{
		m_pOwner->GetMoveData().StartCreatureWalk(vFleePos, EMS_CreatureFlee, FALSE);
	}
	// ���ڲ����ƶ���״̬
	else if( MoveData::EMR_SelfLimit == eRet )
	{
		m_bCanNotPursue = TRUE;
	}
	// ���ɴ�
	else if( MoveData::EMR_NoArrive == eRet )
	{
		// Ѱ·
		Vector3 vPos;
		BOOL bRet = m_pPathFinder->FindingPath(m_pOwner->GetCurPos(), vFleePos);
		if( bRet && m_pPathFinder->GetCurPathPoint(m_pOwner->GetCurPos(), vPos) )
		{
			m_bPathFinding = TRUE;
			m_pOwner->GetMoveData().StartCreatureWalk(vPos, EMS_CreatureFlee, FALSE);
		}
		else
		{
			m_bPursueFailed = TRUE;
		}
	}
	// ����ԭ��
	else
	{
		m_bPursueFailed = TRUE;
	}

	if( !m_bPursueFailed )
	{
		SetFleePos(vFleePos);
	}
}

//---------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------
VOID AIController::UpdateFlee()
{
	// û���ҵ����ܵ�Ŀ���,�����¼���
	if( m_bPursueFailed )
	{
		CalFleePos();
		StartFlee(m_vFleePos);
		return;
	}

	// �������״̬
	if( m_bCanNotPursue )
	{
		if( !m_pOwner->IsInStateCantMove() )
		{
			StartFlee(m_vFleePos);
		}
		return;
	}
	else
	{
		if( m_pOwner->IsInStateCantMove() )
		{
			m_bCanNotPursue = TRUE;
			return;
		}
	}

	// ����Ƿ��Ѿ�ͣ������
	if( EMS_Stand == m_pOwner->GetMoveData().GetCurMoveState() )
	{
		if( m_pOwner->GetMoveData().IsStopped() )
		{
			// ���ƶ��ж�
			StartFlee(m_vFleePos);
		}
		else
		{
			// ����ǿ��ͣ�£�˵��������
			if( m_bPathFinding )
			{
				// Ѱ·��
				Vector3 vNewPos;
				if( m_pPathFinder->GetCurPathPoint(m_pOwner->GetCurPos(), vNewPos) )
				{
					m_pOwner->GetMoveData().StartCreatureWalk(vNewPos, EMS_CreatureFlee, FALSE);
					return;
				}
				else
				{
					m_bArrivedFleePos = TRUE;
					return;
				}
			}
			else
			{
				// ��Ѱ·��
				m_bArrivedFleePos = TRUE;
				return;
			}
		}
	}
}

//----------------------------------------------------------------------------------
// ����õ�һ��������ȵ�Ŀ���
//----------------------------------------------------------------------------------
BOOL AIController::CalHelpPos()
{
	// ��һ��ѡ������Ŀ���
	if(EFT_FirstEnter == m_eFleeTime)
	{
		tagVisTile* pVisTile[ED_End] = {0};

		// �õ���Ұ��Χ�ڵ�vistile�б�
		m_pOwner->GetMap()->GetVisTile(m_pOwner->GetMoveData().m_vPos, X_MAX_SOS_RADIUS, pVisTile);

		Creature* pCreature = NULL;
		FLOAT fNearestDistSq = FLOAT_MAX;

		for(INT n = ED_Center; n < ED_End; n++)
		{
			if( !P_VALID(pVisTile[n]) ) continue;

			TMap<DWORD, Creature*>& mapCreature = pVisTile[n]->mapCreature;

			TMap<DWORD, Creature*>::TMapIterator it = mapCreature.Begin();
			Creature* pCreature = NULL;

			while( mapCreature.PeekNext(it, pCreature) )
			{
				// �������ﲻ���
				if( pCreature->IsDead() ) continue;

				// �Լ������
				if( pCreature->GetID() == m_pOwner->GetID() )	continue;

				const tagCreatureProto* pProto = pCreature->GetProto();
				if( !P_VALID(pProto) || AIAT_CallHelp != pProto->eAIAction ) continue;

				// �з������
				if( m_pOwner->FriendEnemy(pCreature) != ETFE_Friendly ) continue;

				// ���Ŀ���AIΪ����״̬
				if( !P_VALID(pCreature->GetAI()) ) continue;
				if( AIST_Idle != pCreature->GetAI()->GetCurrentStateType() ) continue;

				// ������
				FLOAT fDistSQ = Vec3DistSq(m_pOwner->GetCurPos(), pCreature->GetCurPos());

				// �������С��֮ǰ����ľ���
				if( fDistSQ > fNearestDistSq || fDistSQ > X_MAX_SOS_RADIUS_SQ )
					continue;

				// �鿴�Ƿ�ɴ�
				if( MoveData::EMR_Success == m_pOwner->GetMoveData().IsCanCreatureWalk(pCreature->GetCurPos(), EMS_CreatureWalk, TRUE) )
				{
					GetPosNearPursueTarget(pCreature, m_vFleePos);
					fNearestDistSq = fDistSQ;
					m_dwHelperID = pCreature->GetID();
				}
			}
		}

		if( fNearestDistSq != FLOAT_MAX)
			return TRUE;
	}
	else
	{
		Creature* pCreature = m_pOwner->GetMap()->FindCreature(m_dwHelperID);
		if( !P_VALID(pCreature) )
			return FALSE;

		// �鿴�Ƿ�ɴ�
		if( MoveData::EMR_Success == m_pOwner->GetMoveData().IsCanCreatureWalk(pCreature->GetCurPos(), EMS_CreatureWalk, TRUE) )
		{
			GetPosNearPursueTarget(pCreature, m_vFleePos);
			return TRUE;
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------------------
// �Ƿ���Ҫ���¼������Ŀ���
//----------------------------------------------------------------------------------
BOOL AIController::NeedReCalHelpPos()
{
	Creature* pCreature = m_pOwner->GetMap()->FindCreature(m_dwHelperID);
	if( P_VALID(pCreature) )
	{
		FLOAT fDistanceX = m_vFleePos.x - pCreature->GetCurPos().x;
		FLOAT fDistanceZ = m_vFleePos.z - pCreature->GetCurPos().z;
		FLOAT fDistanceSquare = fDistanceX * fDistanceX + fDistanceZ * fDistanceZ;
		FLOAT fMaxHelpDist = CREATURE_PURSUE_NEAR_DIST + pCreature->GetSize().z + m_pOwner->GetSize().z;

		if(fDistanceSquare > fMaxHelpDist*fMaxHelpDist)
			return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID AIController::StartReturn()
{
	m_bPathFinding		=	FALSE;
	m_bArrivedReturnPos	=	FALSE;

	// �ȼ��ֱ��
	MoveData::EMoveRet eRet = MoveData::EMR_Success;

	if( m_pOwner->NeedCollide() )
		eRet = m_pOwner->GetMoveData().IsCanCreatureWalk(GetEnterCombatPos(), EMS_CreatureWalk, TRUE);
	else
		eRet = m_pOwner->GetMoveData().IsCanCreatureWalk(GetEnterCombatPos(), EMS_CreatureWalk, FALSE);

	// �ɴ�
	if( MoveData::EMR_Success == eRet )
	{
		m_pOwner->GetMoveData().StartCreatureWalk(GetEnterCombatPos(), EMS_CreatureWalk, FALSE);
	}
	// ���ɴ�
	else if( MoveData::EMR_NoArrive == eRet || MoveData::EMR_SelfLimit == eRet)
	{
		// Ѱ·
		Vector3 vPos;
		BOOL bRet = m_pPathFinder->FindingPath(m_pOwner->GetCurPos(), GetEnterCombatPos());
		if( bRet && m_pPathFinder->GetCurPathPoint(m_pOwner->GetCurPos(), vPos) )
		{
			m_bPathFinding = TRUE;
			m_pOwner->GetMoveData().StartCreatureWalk(vPos, EMS_CreatureWalk, FALSE);
		}
		else
		{
			m_bArrivedReturnPos = TRUE;
			m_pOwner->GetMoveData().ForceTeleport(m_pOwner->GetBornPos(),TRUE);
		}
	}
	// ����ԭ��
	else
	{
		m_bArrivedReturnPos = TRUE;
		m_pOwner->GetMoveData().ForceTeleport(m_pOwner->GetBornPos(),TRUE);
	}
}

VOID AIController::UpdateReturn()
{
	// ����Ƿ��Ѿ�ͣ������
	if( EMS_Stand == m_pOwner->GetMoveData().GetCurMoveState() )
	{
			// ����ǿ��ͣ�£�˵��������
		if( m_bPathFinding )
		{
			// Ѱ·��
			Vector3 vNewPos;
			if( m_pPathFinder->GetCurPathPoint(m_pOwner->GetCurPos(), vNewPos) )
			{
				m_pOwner->GetMoveData().StartCreatureWalk(vNewPos, EMS_CreatureWalk, FALSE);
				return;
			}
			else
			{
				m_bArrivedReturnPos = TRUE;
				return;
			}
		}
		else
		{
			// ��Ѱ·��
			m_bArrivedReturnPos = TRUE;
			return;

		}
	}
}

//------------------------------------------------------------------------------
// ������������ʱ��Ŀ���
//------------------------------------------------------------------------------
BOOL AIController::CalSpaceOutPos()
{
	// �ҵ���ǰĿ��
	Unit* pTarget = GetPursueTarget();
	if( !P_VALID(pTarget) )  return FALSE;

	// ����õ���������
	FLOAT fAng = (FLOAT)(IUTIL->Rand() % 360);

	// ����Ŀ���
	CVector2D vDir(CREATURE_SPACE_OUT_DIST);
	vDir.Vec2DRotateAroundOrigin(fAng*3.1415927f / 180.0f);

	m_vFleePos.x = vDir.fx + pTarget->GetCurPos().x;
	m_vFleePos.z = vDir.fy + pTarget->GetCurPos().z;

	if(m_pOwner->NeedCollide())
	{
		m_vFleePos.y = m_pOwner->GetCurPosTop().y;
		return TRUE;
	}
	else
	{
		if( m_pOwner->GetMap()->IfCanGo(m_vFleePos.x, m_vFleePos.z) )
		{
			m_vFleePos.y = m_pOwner->GetMap()->GetGroundHeight(m_vFleePos.x, m_vFleePos.z);
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// �������������
//------------------------------------------------------------------------------
VOID AIController::StartSpaceOut()
{
	m_bArrivedReturnPos	=	FALSE;

	// ��һ��ʱ�Ĵ������
	m_dwSpaceOutTick	=	IUTIL->RandomInRange(CREATURE_SPACE_OUT_MIN, CREATURE_SPACE_OUT_MAX);

	// ���ֱ��
	MoveData::EMoveRet eRet = m_pOwner->GetMoveData().IsCanCreatureWalk(m_vFleePos, EMS_CreatureWalk, TRUE);

	// �ɴ�
	if( MoveData::EMR_Success == eRet )
	{
		m_pOwner->GetMoveData().StartCreatureWalk(m_vFleePos, EMS_CreatureWalk, FALSE);
	}
	// ���ɴ�
	else if( MoveData::EMR_NoArrive == eRet || MoveData::EMR_SelfLimit == eRet)
	{
		m_bArrivedPos = TRUE;
	}
	// ����ԭ��
	else
	{
		m_bArrivedPos = TRUE;
	}

}

VOID AIController::UpdateSpaceOut()
{
	// ����Ƿ��Ѿ�ͣ������
	if( EMS_Stand == m_pOwner->GetMoveData().GetCurMoveState() )
	{
		if( m_pOwner->GetMoveData().IsStopped() )
		{
			// ���ƶ��ж�
			StartSpaceOut();
		}
		else
		{
			m_bArrivedPos = TRUE;
		}
	}
}

//----------------------------------------------------------------------------------
// ������б��е�����Ƿ񶼴����޵�״̬
//----------------------------------------------------------------------------------
BOOL AIController::IsEnmityListAllInvincible()
{
	tagEnmity*	pEnmity		=	(tagEnmity*)GT_INVALID;
	Unit*		pTarget		=	NULL;	

	m_mapEnmity.ResetIterator();
	while( m_mapEnmity.PeekNext(pEnmity) )
	{
		if( !P_VALID(pEnmity) )
			continue;

		pTarget = m_pOwner->GetMap()->FindUnit(pEnmity->dwID);
		if( !P_VALID(pTarget) )
			continue;

		// �Ƿ�����
		if( pTarget->IsDead() )
			continue;

		// �Ƿ�����
		if( pTarget->IsInState(ES_Lurk) ) 
			continue;

		if( !pTarget->IsInState(ES_Invincible) )
			return FALSE;
	}

	return TRUE;
}

