//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: ai_transition.h
// author: wjiang
// actor:
// data: 2009-5-31
// last:
// brief: AI״̬ת����
//-------------------------------------------------------------------------------------------------------


#include "stdafx.h"
#include "ai_transition.h"
#include "creature_ai.h"
#include "creature.h"

AITransition* AggressiveTransition::Instance()
{
	static AggressiveTransition instance;
	return &instance;
}

//------------------------------------------------------------------------------------------
// �����͹���״̬ת��
//------------------------------------------------------------------------------------------
AIStateType	AggressiveTransition::Transition(AIController* pAI, AIStateType eCurState)
{
	switch(eCurState)
	{
	case AIST_Idle:
		return IdleTransition(pAI);

	case AIST_Pursue:
		return PursueTransition(pAI);

	case AIST_Attack:
		return AttackTransition(pAI);

	case AIST_Return:
		return ReturnTransition(pAI);

	default:
		return eCurState;
	}
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType AggressiveTransition::IdleTransition(AIController* pAI)
{
	// ����
	if( pAI->GetMaxEnmityUnitID() != GT_INVALID )
	{
		return AIST_Pursue;
	}

	return AIST_Idle;
}

//------------------------------------------------------------------------------------------
// ׷��״̬ת��
//------------------------------------------------------------------------------------------
AIStateType AggressiveTransition::PursueTransition(AIController* pAI)
{
	// �������target�Ƿ������л��ɷ���״̬
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�껹���ڣ����⼼��
	if( pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Attack;
	}

	return AIST_Pursue;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType AggressiveTransition::AttackTransition(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	
	// ������ܻ�û�����
	if( pOwner->GetCombatHandler().IsValid() )
	{
		return AIST_Attack;
	}

	// ���û��Ŀ����
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�겻�ڹ�����Χ֮����
	if( !pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Pursue;
	}

	return AIST_Attack;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType AggressiveTransition::ReturnTransition(AIController* pAI)
{
	if( pAI->IsArrivedReturnPos() )
	{
		return AIST_Idle;
	}

	return AIST_Return;
}

AITransition* GuardTransition::Instance()
{
	static GuardTransition instance;
	return &instance;
}

//------------------------------------------------------------------------------------------
// �����͹���״̬ת��
//------------------------------------------------------------------------------------------
AIStateType GuardTransition::Transition(AIController* pAI, AIStateType eCurState)
{
	switch(eCurState)
	{
	case AIST_Idle:
		return IdleTransition(pAI);

	case AIST_Attack:
		return AttackTransition(pAI);

	case AIST_Return:
		return ReturnTransition(pAI);

	default:
		return eCurState;
	}
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType GuardTransition::IdleTransition(AIController* pAI)
{
	// ����
	if( pAI->GetMaxEnmityUnitID() != GT_INVALID )
	{
		return AIST_Attack;
	}

	return AIST_Idle;

}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType GuardTransition::AttackTransition(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();

	// ������ܻ�û�����
	if( pOwner->GetCombatHandler().IsValid() )
	{
		return AIST_Attack;
	}

	// ���û��Ŀ����
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// ���Ŀ�겻�ڹ�����Χ���ˣ���������ĳ��
	if( !pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		pAI->SetEnmityActive(pAI->GetTargetUnitID(), FALSE);
		return AIST_Attack;
	}

	return AIST_Attack;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType GuardTransition::ReturnTransition(AIController* pAI)
{
	if( pAI->IsArrivedReturnPos() )
	{
		return AIST_Idle;
	}

	return AIST_Return;
}

AITransition* BarbetteTransition::Instance()
{
	static BarbetteTransition instance;
	return &instance;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType BarbetteTransition::Transition(AIController* pAI, AIStateType eCurState)
{
	switch(eCurState)
	{
	case AIST_Idle:
		return IdleTransition(pAI);

	case AIST_Attack:
		return AttackTransition(pAI);

	default:
		return eCurState;
	}
}

//------------------------------------------------------------------------------------------
// �����Ϳ���״̬ת��
//------------------------------------------------------------------------------------------
AIStateType BarbetteTransition::IdleTransition(AIController* pAI)
{
	// ����
	if( pAI->GetMaxEnmityUnitID() != GT_INVALID )
	{
		pAI->SetTargetUnitID(pAI->GetMaxEnmityUnitID());
		return AIST_Attack;
	}

	return AIST_Idle;
}

//------------------------------------------------------------------------------------------
// �����͹���״̬ת��
//------------------------------------------------------------------------------------------
AIStateType BarbetteTransition::AttackTransition(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();

	// ������ܻ�û�����
	if( pOwner->GetCombatHandler().IsValid() )
	{
		return AIST_Attack;
	}

	// ���û��Ŀ����
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Idle;
	}

	// �����ǰĿ�겻�ڷ�Χ�ڣ����������
	if( !pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		pAI->SetEnmityActive(pAI->GetTargetUnitID(), FALSE);
		return AIST_Attack;
	}

	return AIST_Attack;
}


AITransition* LamsterTransition::Instance()
{
	static LamsterTransition instance;
	return &instance;
}

//------------------------------------------------------------------------------------------
// �����͹���״̬ת��
//------------------------------------------------------------------------------------------
AIStateType	LamsterTransition::Transition(AIController* pAI, AIStateType eCurState)
{
	switch(eCurState)
	{
	case AIST_Idle:
		return IdleTransition(pAI);

	case AIST_Pursue:
		return PursueTransition(pAI);

	case AIST_Attack:
		return AttackTransition(pAI);

	case AIST_Return:
		return ReturnTransition(pAI);

	case AIST_Flee:
		return FleeTransition(pAI);

	default:
		return eCurState;
	}
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType LamsterTransition::IdleTransition(AIController* pAI)
{
	// ����
	if( pAI->GetMaxEnmityUnitID() != GT_INVALID )
	{
		return AIST_Pursue;
	}

	return AIST_Idle;
}

//------------------------------------------------------------------------------------------
// ׷��״̬ת��
//------------------------------------------------------------------------------------------
AIStateType LamsterTransition::PursueTransition(AIController* pAI)
{
	// �������target�Ƿ������л��ɷ���״̬
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�껹���ڣ����⼼��
	if( pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Attack;
	}

	return AIST_Pursue;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType LamsterTransition::AttackTransition(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();

	// ������ܻ�û�����
	if( pOwner->GetCombatHandler().IsValid() )
	{
		return AIST_Attack;
	}

	// ���û��Ŀ����
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�겻�ڹ�����Χ֮����
	if( !pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Pursue;
	}

	// ��ǰ����С�ڻ�������20��
	if(EFT_NotEnter == pAI->GetFleeTime() && (pOwner->GetBaseAttValue(ERA_MaxHP) * 0.2f) > pOwner->GetAttValue(ERA_HP))
	{
		DWORD	dwFleeTick = IUTIL->Rand() % 50 + 100;
		pAI->SetFleeTick(dwFleeTick);
		return AIST_Flee;
	}

	return AIST_Attack;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType LamsterTransition::FleeTransition(AIController* pAI)
{
	// ����״̬����ʱ�����
	if( pAI->GetFleeTick() <= 0 )
	{
		return AIST_Pursue;
	}

	return AIST_Flee;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType LamsterTransition::ReturnTransition(AIController* pAI)
{
	if( pAI->IsArrivedReturnPos() )
	{
		return AIST_Idle;
	}

	return AIST_Return;
}


AITransition* CallHelpTransition::Instance()
{
	static CallHelpTransition instance;
	return &instance;
}

//------------------------------------------------------------------------------------------
// �����͹���״̬ת��
//------------------------------------------------------------------------------------------
AIStateType	CallHelpTransition::Transition(AIController* pAI, AIStateType eCurState)
{
	switch(eCurState)
	{
	case AIST_Idle:
		return IdleTransition(pAI);

	case AIST_Pursue:
		return PursueTransition(pAI);

	case AIST_Attack:
		return AttackTransition(pAI);

	case AIST_Return:
		return ReturnTransition(pAI);

	case AIST_SOS:
		return CallForHelpTransition(pAI);

	default:
		return eCurState;
	}
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType CallHelpTransition::IdleTransition(AIController* pAI)
{
	// ����
	if( pAI->GetMaxEnmityUnitID() != GT_INVALID )
	{
		return AIST_Pursue;
	}

	return AIST_Idle;
}

//------------------------------------------------------------------------------------------
// ׷��״̬ת��
//------------------------------------------------------------------------------------------
AIStateType CallHelpTransition::PursueTransition(AIController* pAI)
{
	// �������target�Ƿ������л��ɷ���״̬
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�껹���ڣ����⼼��
	if( pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Attack;
	}

	return AIST_Pursue;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType CallHelpTransition::AttackTransition(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();

	// ������ܻ�û�����
	if( pOwner->GetCombatHandler().IsValid() )
	{
		return AIST_Attack;
	}

	// ���û��Ŀ����
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�겻�ڹ�����Χ֮����
	if( !pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Pursue;
	}

	// ��ǰ����С�ڻ�������20��
	if(EFT_NotEnter == pAI->GetFleeTime() && (pOwner->GetBaseAttValue(ERA_MaxHP) * 0.2f) > pOwner->GetAttValue(ERA_HP))
	{
		return AIST_SOS;
	}

	return AIST_Attack;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType CallHelpTransition::CallForHelpTransition(AIController* pAI)
{
	// ����״̬����ʱ�����
	if( EFT_CallHelp == pAI->GetFleeTime() )
	{
		return AIST_Pursue;
	}

	return AIST_SOS;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType CallHelpTransition::ReturnTransition(AIController* pAI)
{
	if( pAI->IsArrivedReturnPos() )
	{
		return AIST_Idle;
	}

	return AIST_Return;
}

//------------------------------------------------------------------------------------------
// ������״̬ת��
//------------------------------------------------------------------------------------------
AITransition* SpaceOutTransition::Instance()
{
	static SpaceOutTransition instance;
	return &instance;
}

AIStateType	SpaceOutTransition::Transition(AIController* pAI, AIStateType eCurState)
{
	switch(eCurState)
	{
	case AIST_Idle:
		return IdleTransition(pAI);

	case AIST_Pursue:
		return PursueTransition(pAI);

	case AIST_Attack:
		return AttackTransition(pAI);

	case AIST_Return:
		return ReturnTransition(pAI);

	case AIST_SpaceOut:
		return SpaceTransition(pAI);

	default:
		return eCurState;
	}
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType SpaceOutTransition::IdleTransition(AIController* pAI)
{
	// ����
	if( pAI->GetMaxEnmityUnitID() != GT_INVALID )
	{
		return AIST_Pursue;
	}

	return AIST_Idle;
}

//------------------------------------------------------------------------------------------
// ׷��״̬ת��
//------------------------------------------------------------------------------------------
AIStateType SpaceOutTransition::PursueTransition(AIController* pAI)
{
	// �������target�Ƿ������л��ɷ���״̬
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�껹���ڣ����⼼��
	if( pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Attack;
	}

	return AIST_Pursue;
}

//------------------------------------------------------------------------------------------
// ����״̬ת��
//------------------------------------------------------------------------------------------
AIStateType SpaceOutTransition::AttackTransition(AIController* pAI)
{
	Creature* pOwner = pAI->GetOwner();
	Unit* pTarget = pAI->GetPursueTarget();

	// ������ܻ�û�����
	if( pOwner->GetCombatHandler().IsValid() )
	{
		return AIST_Attack;
	}

	// ���û��Ŀ����
	if( !P_VALID(pAI->GetTargetUnitID()) )
	{
		return AIST_Return;
	}

	// �����ǰĿ�겻�ڹ�����Χ֮����
	if( !pAI->IsInAttackDist(pAI->GetTargetUnitID()) )
	{
		return AIST_Pursue;
	}

	// ���㱣�־��봥�����
	DWORD dwCombatTick = g_world.GetWorldTick() - pAI->GetEnterCombatTick();
	if(0 == dwCombatTick % pAI->GetSpaceOutTick() )
	{
		if( pOwner->IsInDistance(*pTarget, CREATURE_SPACE_OUT_CHANGE_DIST) )
		{
			if( pAI->CalSpaceOutPos() )
				return AIST_SpaceOut;
		}
	}

	return AIST_Attack;
}

AIStateType SpaceOutTransition::SpaceTransition(AIController* pAI)
{
	if( pAI->IsArrivedPos())
		return AIST_Attack;

	return AIST_SpaceOut;
}

AIStateType SpaceOutTransition::ReturnTransition(AIController* pAI)
{
	if( pAI->IsArrivedReturnPos() )
	{
		return AIST_Idle;
	}

	return AIST_Return;
}