#include "StdAfx.h"
#include "LowerStateKeyboardMove_LP.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "CombatSysUtil.h"
#include "WorldFrame.h"
#include "CombatEvent.h"
#include "..\WorldDefine\action.h"
#include "PreState.h"
#include "MapMgr.h"
#include "RoleEvent.h"
#include "PersonalityActData.h"

LowerStateKeyboardMove_LP::LowerStateKeyboardMove_LP(void)
{}

LowerStateKeyboardMove_LP::~LowerStateKeyboardMove_LP(void)
{}

void LowerStateKeyboardMove_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState )
{
	ASSERT(pEvent->strEventName==_T("tagKeyboardMoveEvent"));
	tagKeyboardMoveEvent* pMoveEvent=(tagKeyboardMoveEvent*)pEvent;

	//--����event
	m_event=*pMoveEvent;

	m_actKeyboardMove.Active(pMoveEvent->start,pMoveEvent->dir,pNav,pRole,
		m_pFSM->IfCanPlayLowerAni(),m_pFSM->IfLowerCanSetYaw());
}

void LowerStateKeyboardMove_LP::Update( NavMap* pNav,LocalPlayer* pRole )
{
	m_actKeyboardMove.Update(pNav,pRole,m_pFSM->IfCanPlayLowerAni(),m_pFSM->IfLowerCanSetYaw());
	if(m_actKeyboardMove.IsClosed())
	{
		//--����
		if(m_actKeyboardMove.GetResult()==ActionKeyboardMove_LP::ER_Drop)
		{
			tagDropEvent event(_T("tagDropEvent"),NULL);
			event.start=pRole->GetPos();
			event.dir=m_actKeyboardMove.GetMoveVec();

			tagPreState prestate;
			prestate.state=tagPreState::EState_KeyboardMove;
			prestate.keyboardMove=m_event;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_Drop,&event,&prestate);
		}
		//--����
		else if(m_actKeyboardMove.GetResult()==ActionKeyboardMove_LP::ER_Slide)
		{
			tagSlideEvent event(_T("tagSlideEvent"),NULL);
			event.start=pRole->GetPos();

			tagPreState prestate;
			prestate.state=tagPreState::EState_KeyboardMove;
			prestate.keyboardMove=m_event;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_Slide,&event,&prestate);
		}
		//--��Ӿ
		else if(m_actKeyboardMove.GetResult()==ActionKeyboardMove_LP::ER_Swim)
		{
			tagKeyboardMoveEvent event(_T("tagKeyboardMoveEvent"),NULL);
			event.start=pRole->GetPos();
			event.dir=m_event.dir;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_KeyboardSwim,&event);
		}
		//--ˮ������
		else if(m_actKeyboardMove.GetResult()==ActionKeyboardMove_LP::ER_OnWater)
		{
			tagKeyboardMoveEvent event(_T("tagKeyboardMoveEvent"),NULL);
			event.start=pRole->GetPos();
			event.dir=m_event.dir;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_KeyboardWaterWalk,&event);
		}
		else
		{
			m_pFSM->Change2LowerState_Stand();
		}
	}
}

void LowerStateKeyboardMove_LP::OnGameEvent( tagGameEvent* pEvent )
{
	//--�����ƶ��¼�-------------------------------------------------------------------
	if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))
	{
		tagKeyboardMoveEvent* pMoveEvent=(tagKeyboardMoveEvent*)pEvent;

		Role* pRole=m_pFSM->GetLocalPlayer();
		NavMap* pNav=MapMgr::Inst()->GetNavMap();

		//--����
		if(pNav->GetCollider()->IfWillSlide(pRole->GetPos(),pRole->GetRoleSize()))
		{
			tagSlideEvent event(_T("tagSlideEvent"),NULL);
			event.start=pRole->GetPos();

			tagPreState prestate;
			prestate.state=tagPreState::EState_KeyboardMove;
			prestate.keyboardMove=*pMoveEvent;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_Slide,&event,&prestate);
		}
		//--��ֱ��
		else if(pNav->GetCollider()->IfWillVDrop(pRole->GetPos(),pRole->GetRoleSize()))
		{
			tagVDropEvent event(_T("tagVDropEvent"),NULL);
			event.start=pRole->GetPos();

			tagPreState prestate;
			prestate.state=tagPreState::EState_KeyboardMove;
			prestate.keyboardMove=*pMoveEvent;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_VDrop,&event,&prestate);
		}
		//--
		else
		{
			m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardMove,pEvent);
		}
	}
	//--ֹͣ�����ƶ�-------------------------------------------------------------------
	else if(pEvent->strEventName==_T("tagStopKeyboardMoveEvent"))
	{
		Role* pRole=m_pFSM->GetLocalPlayer();
		NavMap* pNav=MapMgr::Inst()->GetNavMap();

		m_actKeyboardMove.Stop(MapMgr::Inst()->GetNavMap(),m_pFSM->GetLocalPlayer());

		//--����
		if(pNav->GetCollider()->IfWillSlide(pRole->GetPos(),pRole->GetRoleSize()))
		{
			tagSlideEvent event(_T("tagSlideEvent"),NULL);
			event.start=pRole->GetPos();

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_Slide,&event);
		}
		//--��ֱ��
		else if(pNav->GetCollider()->IfWillVDrop(pRole->GetPos(),pRole->GetRoleSize()))
		{
			tagVDropEvent event(_T("tagVDropEvent"),NULL);
			event.start=pRole->GetPos();

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_VDrop,&event);
		}
		//--
		else
		{
			m_pFSM->Change2LowerState_Stand();
		}
	}
	//--��ת��ͷ-------------------------------------------------------------------
	else if(pEvent->strEventName==_T("tagRotateCameraEvent"))
	{
		Role* pRole=m_pFSM->GetLocalPlayer();
		NavMap* pNav=MapMgr::Inst()->GetNavMap();

		//--����
		if(pNav->GetCollider()->IfWillSlide(pRole->GetPos(),pRole->GetRoleSize()))
		{
			tagSlideEvent event(_T("tagSlideEvent"),NULL);
			event.start=pRole->GetPos();

			tagPreState prestate;
			prestate.state=tagPreState::EState_KeyboardMove;
			prestate.keyboardMove=m_event;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_Slide,&event,&prestate);
		}
		//--��ֱ��
		else if(pNav->GetCollider()->IfWillVDrop(pRole->GetPos(),pRole->GetRoleSize()))
		{
			tagVDropEvent event(_T("tagVDropEvent"),NULL);
			event.start=pRole->GetPos();

			tagPreState prestate;
			prestate.state=tagPreState::EState_KeyboardMove;
			prestate.keyboardMove=m_event;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_VDrop,&event,&prestate);
		}
		//--
		else
		{
			tagKeyboardMoveEvent newEvent;
			newEvent.start=pRole->GetPos();
			newEvent.dir=m_event.dir;

			m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardMove,&newEvent);
		}
	}
	//--��Ծ-------------------------------------------------------------------
	else if(pEvent->strEventName==_T("tagJumpEvent"))
	{
		tagJumpEvent* pJumpEvent=(tagJumpEvent*)pEvent;
		pJumpEvent->dir=m_actKeyboardMove.GetMoveVec();

		tagPreState prestate;
		prestate.state=tagPreState::EState_KeyboardMove;
		prestate.keyboardMove=m_event;

		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_Jump,pEvent,&prestate);
	}
	//--����ս��-------------------------------------------------------------------
	if(pEvent->strEventName==_T("tagLeaveCombatEvent"))
	{
		m_actKeyboardMove.ReplayAni(m_pFSM->GetLocalPlayer());
	}
	//--�л�����-------------------------------------------------------------------
	else if(pEvent->strEventName==_T("tagWeaponChangeEvent"))
	{
		tagWeaponChangeEvent* pWeaponChangeEvent = (tagWeaponChangeEvent*)pEvent;
		if( pWeaponChangeEvent->bRightWeapon )
			m_actKeyboardMove.ReplayAni(m_pFSM->GetLocalPlayer());
	}
}

void LowerStateKeyboardMove_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	//--�ٶȱ仯��Ϣ-------------------------------------------------------------------
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_MoveSpeedChange"))
	{
		tagKeyboardMoveEvent event=m_event;
		event.start=m_pFSM->GetLocalPlayer()->GetPos();
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardMove,&event);
	}
	//--˲����Ϣ
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_SpecialMove"))
	{
		tagNS_SpecialMove* pCmd=(tagNS_SpecialMove*)pNetCmd;
		if( ESMT_Teleport == pCmd->eType )
		{
			tagKeyboardMoveEvent event = m_event;
			event.start = pCmd->vDestPos;
			OnGameEvent( &event );
		}
	}
}

void LowerStateKeyboardMove_LP::ReplayAni( LocalPlayer* pRole )
{
	m_actKeyboardMove.ReplayAni(pRole);
}

bool LowerStateKeyboardMove_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
	switch(state)
	{
	case FSM_LP::EUpperState_BeAttack:		//������
		{
			return false;
		}
		break;
	case FSM_LP::EUpperState_StyleAction:
		{
			tagPlayPersonalityActEvent* pStyleEvent = (tagPlayPersonalityActEvent*)pEvent;
			if( tagPersonAct::T_Upper != pStyleEvent->pData->eType )
				return false;
		}
		break;
	}

	return true;
}

bool LowerStateKeyboardMove_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	switch(state)
	{
	//Ϊ�˲�Ӱ����������ԣ������ƶ�״̬��ʹ�ü���ʱ�����Զ��ӽ�Ŀ��
	case FSM_LP::ELowerState_Pursue:
		return false;
	}
	return true;
}
