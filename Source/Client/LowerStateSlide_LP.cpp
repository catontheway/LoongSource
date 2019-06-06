#include "StdAfx.h"
#include "LowerStateSlide_LP.h"
#include "CombatEvent.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "PreState.h"
#include "CombatSysUtil.h"

LowerStateSlide_LP::LowerStateSlide_LP(void)
{}

LowerStateSlide_LP::~LowerStateSlide_LP(void)
{}

void LowerStateSlide_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState )
{
	ASSERT(pEvent->strEventName==_T("tagSlideEvent"));
	tagSlideEvent* pSlideEvent=(tagSlideEvent*)pEvent;

	if(pPreState!=NULL)
		m_prestate=*pPreState;
	else
		m_prestate.state=tagPreState::EState_None;

	m_actSlide.Active(pSlideEvent->start,pNav,pRole,m_pFSM->IfCanPlayLowerAni());
}

void LowerStateSlide_LP::Update( NavMap* pNav,LocalPlayer* pRole )
{
	m_actSlide.Update(pNav,pRole,m_pFSM->IfCanPlayLowerAni());
	if(m_actSlide.IsClosed())
	{
		if(m_prestate.state!=tagPreState::EState_None)
		{
			m_prestate.Change2LowerState(m_pFSM,pRole,
				m_actSlide.GetResult()==ActionSlide_LP::ER_Swim,
				m_actSlide.GetResult()==ActionSlide_LP::ER_OnWater);
		}
		else if(m_actSlide.GetResult()==ActionSlide_LP::ER_Swim)
		{
			m_pFSM->Change2LowerState_Float();
		}
		else if(m_actSlide.GetResult()==ActionSlide_LP::ER_OnWater)
		{
			m_pFSM->Change2LowerState_WaterStand();
		}
		else
		{
			m_pFSM->Change2LowerState_Stand();
		}
	}
}

void LowerStateSlide_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))//�����ƶ���Ϸ�¼�
	{
		tagKeyboardMoveEvent* pMoveEvent=(tagKeyboardMoveEvent*)pEvent;

		m_prestate.state=tagPreState::EState_KeyboardMove;
		m_prestate.keyboardMove=*pMoveEvent;
	}
	else if(pEvent->strEventName==_T("tagStopKeyboardMoveEvent"))//ֹͣ�����ƶ���Ϸ�¼�
	{
		if(m_prestate.state==tagPreState::EState_KeyboardMove)
			m_prestate.state=tagPreState::EState_None;
	}
	else if(pEvent->strEventName==_T("tagStopPursueEvent"))//ֹͣ׷��
	{
		if(m_prestate.state==tagPreState::EState_Pursue)
			m_prestate.state=tagPreState::EState_None;
	}
}

void LowerStateSlide_LP::ReplayAni( LocalPlayer* pRole )
{
	m_actSlide.ReplayAni(pRole);
}

bool LowerStateSlide_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
	switch(state)
	{
	case FSM_LP::EUpperState_Idle:		//����
	case FSM_LP::EUpperState_Dead:		//����
	case FSM_LP::EUpperState_Dazzy:		//����
	case FSM_LP::EUpperState_Tie:		//����
	case FSM_LP::EUpperState_Sopor:		//��˯
		{
			return true;
		}
		break;
	}

	return false;
}

bool LowerStateSlide_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	return true;
}
