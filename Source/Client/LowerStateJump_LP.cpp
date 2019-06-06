#include "StdAfx.h"
#include "LowerStateJump_LP.h"
#include "CombatEvent.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "CombatSysUtil.h"
#include "WorldFrame.h"
#include "PreState.h"


LowerStateJump_LP::LowerStateJump_LP(void)
{}

LowerStateJump_LP::~LowerStateJump_LP(void)
{}

void LowerStateJump_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState )
{
    // �ر����޸���
    pRole->SwitchMountPitch(false);

	ASSERT(pEvent->strEventName==_T("tagJumpEvent"));
	tagJumpEvent* pJumpEvent=(tagJumpEvent*)pEvent;

	//--
	if(pPreState!=NULL)
		m_prestate=*pPreState;
	else
		m_prestate.state=tagPreState::EState_None;
	m_bOriginJump=!m_prestate.IsMoving();

	m_actJump.Active(pJumpEvent->start,pJumpEvent->dir,m_bOriginJump,pNav,pRole);
}

void LowerStateJump_LP::Update(NavMap* pNav,LocalPlayer* pRole)
{
	m_actJump.Update(!m_prestate.IsMoving(),pNav,pRole);

	if(m_actJump.IsClosed())
	{
        // �����޸���
        pRole->SwitchMountPitch(true);

		if(m_actJump.GetResult()==ActionJump_LP::ER_VDrop)
		{
			tagVDropEvent event(_T("tagVDropEvent"),NULL);
			event.start=pRole->GetPos();
			m_pFSM->Change2LowerState(FSM_LP::ELowerState_VDrop,&event,&m_prestate);
		}
		else if(m_actJump.GetResult()==ActionJump_LP::ER_Slide)
		{
			tagSlideEvent event(_T("tagSlideEvent"),NULL);
			event.start=pRole->GetPos();
			m_pFSM->Change2LowerState(FSM_LP::ELowerState_Slide,&event,&m_prestate);
		}
		else if(m_prestate.state!=tagPreState::EState_None)
		{
			m_prestate.Change2LowerState(m_pFSM,pRole,
				m_actJump.GetResult()==ActionJump_LP::ER_Swim,
				m_actJump.GetResult()==ActionJump_LP::ER_OnWater);
		}
		else if(m_actJump.GetResult()==ActionJump_LP::ER_Swim)
		{
			m_pFSM->Change2LowerState_Float();
		}
		else if(m_actJump.GetResult()==ActionJump_LP::ER_OnWater)
		{
			m_pFSM->Change2LowerState_WaterStand();
		}
		else
		{
			m_pFSM->Change2LowerState_Stand();
		}
	}
}

void LowerStateJump_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))//�����ƶ�
	{
		tagKeyboardMoveEvent* pMoveEvent=(tagKeyboardMoveEvent*)pEvent;
		
		if(m_actJump.IsLanding())//���ڲ�����ض���
		{
			m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardMove,pEvent);
		}
		else
		{
			if(m_bOriginJump)//ԭ����
			{
				Role* pRole=m_pFSM->GetLocalPlayer();
				ClientCamera* pCamera=GetWorldFrm()->GetCamera();

				tagDropEvent dropEvent(_T("tagDropEvent"),NULL);
				dropEvent.start=pRole->GetPos();
				dropEvent.dir=CombatSysUtil::Inst()->CalcKeyMoveDir(pMoveEvent->dir,pCamera->GetYaw());

				tagPreState prestate;
				prestate.state=tagPreState::EState_KeyboardMove;
				prestate.keyboardMove=*pMoveEvent;

				m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_Drop,&dropEvent,&prestate);
			}
			else
			{
				m_prestate.state=tagPreState::EState_KeyboardMove;
				m_prestate.keyboardMove=*pMoveEvent;
			}
		}
	}
	else if(pEvent->strEventName==_T("tagStopKeyboardMoveEvent"))//ֹͣ�����ƶ�
	{
		if(m_prestate.state==tagPreState::EState_KeyboardMove)
			m_prestate.state=tagPreState::EState_None;
	}
	else if(pEvent->strEventName==_T("tagJumpEvent"))//��Ծ
	{
		if(m_actJump.IsLanding())//���ڲ�����ض���
		{
			m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_Jump,pEvent,&m_prestate);
		}
	}
	else if(pEvent->strEventName==_T("tagMouseMoveEvent"))//����ƶ�
	{
		if(m_actJump.IsLanding())//���ڲ�����ض���
		{
			m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseMove,pEvent);
		}
	}
	else if(pEvent->strEventName==_T("tagStopPursueEvent"))//ֹͣ׷��
	{
		if(m_prestate.state==tagPreState::EState_Pursue)
			m_prestate.state=tagPreState::EState_None;
	}
}

bool LowerStateJump_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
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

bool LowerStateJump_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	return true;
}