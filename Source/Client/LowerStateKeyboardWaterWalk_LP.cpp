#include "StdAfx.h"
#include "LowerStateKeyboardWaterWalk_LP.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "CombatSysUtil.h"
#include "WorldFrame.h"
#include "CombatEvent.h"
#include "..\WorldDefine\action.h"
#include "PreState.h"
#include "MapMgr.h"

LowerStateKeyboardWaterWalk_LP::LowerStateKeyboardWaterWalk_LP(void)
{}

LowerStateKeyboardWaterWalk_LP::~LowerStateKeyboardWaterWalk_LP(void)
{}

void LowerStateKeyboardWaterWalk_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState )
{
	ASSERT(pEvent->strEventName==_T("tagKeyboardMoveEvent"));
	tagKeyboardMoveEvent* pMoveEvent=(tagKeyboardMoveEvent*)pEvent;

	//--����event
	m_event=*pMoveEvent;

	m_actKeyboardWaterWalk.Active(pMoveEvent->start,pMoveEvent->dir,pNav,pRole);
}

void LowerStateKeyboardWaterWalk_LP::Update( NavMap* pNav,LocalPlayer* pRole )
{
	m_actKeyboardWaterWalk.Update(pNav,pRole);
	if(m_actKeyboardWaterWalk.IsClosed())
	{
		if(m_actKeyboardWaterWalk.GetResult()==ActionKeyboardWaterWalk_LP::ER_Ground)
		{
			//�л����µļ����ƶ�״̬
			tagKeyboardMoveEvent event(_T("tagKeyboardMoveEvent"),NULL);
			event.start=pRole->GetPos();
			event.dir=m_event.dir;

			m_pFSM->Change2LowerState(FSM_LP::ELowerState_KeyboardMove,&event);
		}
		else
		{
			m_pFSM->Change2LowerState_WaterStand();
		}
	}
}

void LowerStateKeyboardWaterWalk_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))//�µļ����ƶ��¼�
	{
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardWaterWalk,pEvent);
	}
	else if(pEvent->strEventName==_T("tagStopKeyboardMoveEvent"))//ֹͣ�����ƶ���Ϸ�¼�
	{
		m_actKeyboardWaterWalk.Stop(MapMgr::Inst()->GetNavMap(),m_pFSM->GetLocalPlayer());
		m_pFSM->Change2LowerState_WaterStand();
	}
	else if(pEvent->strEventName==_T("tagRotateCameraEvent"))//��ת��ɫ��Ϸ�¼�
	{
		Role* pRole=m_pFSM->GetLocalPlayer();

		//���¼�������ƶ���Ϊ
		m_actKeyboardWaterWalk.Active(pRole->GetPos(),m_actKeyboardWaterWalk.GetMoveDir(),MapMgr::Inst()->GetNavMap(),m_pFSM->GetLocalPlayer());
	}
	else if(pEvent->strEventName==_T("tagJumpEvent"))//��Ծ��Ϸ�¼�
	{
		tagJumpEvent* pJumpEvent=(tagJumpEvent*)pEvent;
		pJumpEvent->dir=m_actKeyboardWaterWalk.GetMoveVec();

		tagPreState prestate;
		prestate.state=tagPreState::EState_KeyboardMove;
		prestate.keyboardMove=m_event;

		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_Jump,pEvent,&prestate);
	}
	else if(pEvent->strEventName==_T("tagLPRoleStateChangeEvent"))
	{
		LocalPlayer* pSelf=m_pFSM->GetLocalPlayer();
		if(!pSelf->CanWaterWalk())//ˮ������Ч������
		{
			CombatSysUtil::Inst()->PullLocalPlayer();

			tagKeyboardMoveEvent evt(_T("tagKeyboardMoveEvent"),NULL);
			evt.start=pSelf->GetPos();
			evt.dir=m_event.dir;

			if(MapMgr::Inst()->GetNavMap()->GetCollider()->IfWillSwim(pSelf->GetPos(),pSelf->GetRoleSize().y))
				m_pFSM->Change2LowerState(FSM_LP::ELowerState_KeyboardSwim,&evt);
			else
				m_pFSM->Change2LowerState(FSM_LP::ELowerState_KeyboardMove,&evt);
		}
	}
}

void LowerStateKeyboardWaterWalk_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_MoveSpeedChange"))//�ٶȱ仯��Ϣ
	{
		tagKeyboardMoveEvent event=m_event;
		event.start=m_pFSM->GetLocalPlayer()->GetPos();
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardWaterWalk,&event);
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

void LowerStateKeyboardWaterWalk_LP::OnExit()
{
}

bool LowerStateKeyboardWaterWalk_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
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

bool LowerStateKeyboardWaterWalk_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	return true;
}