#include "StdAfx.h"
#include "LowerStateMouseSwim_LP.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "WorldPickerFrame.h"
#include "PreState.h"
#include "CombatSysUtil.h"


LowerStateMouseSwim_LP::LowerStateMouseSwim_LP(void) :
m_finishedOnExit(true)
{}

LowerStateMouseSwim_LP::~LowerStateMouseSwim_LP(void)
{}

void LowerStateMouseSwim_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState )
{
	ASSERT(pEvent->strEventName==_T("tagMouseMoveEvent"));
	tagMouseMoveEvent* pMoveEvent=(tagMouseMoveEvent*)pEvent;

	//--����event
	m_event=*pMoveEvent;
	m_finishedOnExit=true;
	//--
	m_actMouseSwim.Active(pMoveEvent->start,pMoveEvent->end,pMoveEvent->validDist,pNav,pRole);
}

void LowerStateMouseSwim_LP::Update(NavMap* pNav,LocalPlayer* pRole)
{
	m_actMouseSwim.Update(pNav,pRole);
	if(m_actMouseSwim.IsClosed())
	{
		if(m_actMouseSwim.GetResult()==ActionMouseSwim_LP::ER_Land)
		{
			tagMouseMoveEvent event;
			event.start=pRole->GetPos();
			event.end=m_event.end;
			event.bUserCtrl=false;

			m_finishedOnExit=false;
			m_pFSM->Change2LowerState(FSM_LP::ELowerState_MouseMove,&event);
		}
		else
		{
			m_pFSM->Change2LowerState_Float();
		}
	}
}

void LowerStateMouseSwim_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))//�����ƶ���Ϸ�¼�
	{
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardSwim,pEvent);
	}
	else if(pEvent->strEventName==_T("tagMouseMoveEvent"))//����ƶ��¼�
	{
		tagMouseMoveEvent* pMouseMoveEvent=(tagMouseMoveEvent*)pEvent;
		m_finishedOnExit=pMouseMoveEvent->bUserCtrl;
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseSwim,pEvent);
	}
	else if(pEvent->strEventName==_T("tagJumpEvent"))//��Ծ�¼�
	{
		LocalPlayer* pSelf=m_pFSM->GetLocalPlayer();

		tagJumpEvent* pJumpEvent=(tagJumpEvent*)pEvent;
		pJumpEvent->start=pSelf->GetPos();
		Yaw2Dir(pSelf->GetYaw(),pJumpEvent->dir);

		tagPreState prestate;
		prestate.state=tagPreState::EState_MouseMove;
		prestate.mouseMove=m_event;

		m_finishedOnExit=false;
		m_pFSM->Change2LowerState(FSM_LP::ELowerState_Jump,pEvent,&prestate);
	}
	else if(pEvent->strEventName==_T("tagLPRoleStateChangeEvent"))
	{
		LocalPlayer* pSelf=m_pFSM->GetLocalPlayer();
		if(pSelf->CanWaterWalk())//ˮ������
		{
			tagMouseMoveEvent evt = m_event;
			evt.start=pSelf->GetPos();

			m_finishedOnExit=false;
			m_pFSM->Change2LowerState(FSM_LP::ELowerState_MouseWaterWalk,&evt);
		}
	}
}

void LowerStateMouseSwim_LP::ShowDecal( bool bShow )
{
	tagShowScenePickDecalEvent event(_T("tagShowScenePickDecalEvent"),NULL);
	event.bShow=bShow;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

void LowerStateMouseSwim_LP::SendFinishEvent( bool bSuccessed )
{
	tagMouseMoveFinishEvent event;
	event.bSuccessed=bSuccessed;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

void LowerStateMouseSwim_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_MoveSpeedChange"))//�ٶȱ仯��Ϣ
	{
		tagMouseMoveEvent event=m_event;
		event.start=m_pFSM->GetLocalPlayer()->GetPos();
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseSwim,&event);
	}
	//--˲����Ϣ
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_SpecialMove"))
	{
		tagNS_SpecialMove* pCmd=(tagNS_SpecialMove*)pNetCmd;
		if( ESMT_Teleport == pCmd->eType )
		{
			tagMouseMoveEvent event=m_event;
			event.start=pCmd->vDestPos;
			event.bUserCtrl=false;
			OnGameEvent(&event);
		}
	}
}

void LowerStateMouseSwim_LP::OnExit()
{
	m_actMouseSwim.StopSound();

	if(m_finishedOnExit)
	{
		ShowDecal(false);
		SendFinishEvent(m_actMouseSwim.GetResult()==ActionMouseSwim_LP::ER_Arrive);
	}
}

bool LowerStateMouseSwim_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
	switch(state)
	{
	case FSM_LP::EUpperState_Idle:		//����
	case FSM_LP::EUpperState_Dead:		//����
	case FSM_LP::EUpperState_Dazzy:		//����
	case FSM_LP::EUpperState_Tie:		//����
	case FSM_LP::EUpperState_Sopor:		//��˯
	case FSM_LP::EUpperState_Follow:	//����
	case FSM_LP::EUpperState_PickupItem://ʰȡ
	case FSM_LP::EUpperState_TalkNPC:	//��NPC�Ի�
	case FSM_LP::EUpperState_UseItem:	//ʹ����Ʒ
		{
			return true;
		}
		break;
	}

	return false;
}

bool LowerStateMouseSwim_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	return true;
}