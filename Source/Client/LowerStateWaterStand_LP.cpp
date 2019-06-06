#include "StdAfx.h"
#include "LowerStateWaterStand_LP.h"
#include "FSM_LP.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "..\WorldDefine\action.h"
#include "CombatSysUtil.h"
#include "RoleEvent.h"
#include "EffectMgr.h"
#include "MapMgr.h"

LowerStateWaterStand_LP::LowerStateWaterStand_LP(void)
{}

LowerStateWaterStand_LP::~LowerStateWaterStand_LP(void)
{}

void LowerStateWaterStand_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState )
{
	//--���Ŷ���
	if(pRole->IsReadyState())
	{
		const TCHAR* szTrackName=CombatSysUtil::Inst()->GetReadyTrackName(pRole->GetRWeaponType());
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Ready,AvatarAniMixer::EAP_Lower,szTrackName,true);
	}
	else
	{
		const TCHAR* szTrackName=CombatSysUtil::Inst()->GetStandTrackName(pRole->GetRWeaponType());
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_Lower,szTrackName,true);
	}

	//--������Ч
	Vector3 effectPos=pRole->GetPos();
	effectPos.y=pNav->GetCollider()->GetWaterHeightEx(effectPos,effectPos.y);
	m_effectID=EffectMgr::Inst()->PlayEffect(effectPos,pRole->GetDestYaw(),_T("txqt05"));
}

void LowerStateWaterStand_LP::Update(NavMap* pNav,LocalPlayer* pRole)
{}

void LowerStateWaterStand_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagMouseMoveEvent"))
	{
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseWaterWalk,pEvent);
	}
	else if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))
	{
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardWaterWalk,pEvent);
	}
	else if(pEvent->strEventName==_T("tagJumpEvent"))
	{
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_Jump,pEvent);
	}
	else if(pEvent->strEventName==_T("tagLPRoleStateChangeEvent"))
	{
		if(!m_pFSM->GetLocalPlayer()->CanWaterWalk())//ˮ������Ч������
		{
			LocalPlayer* pRole = m_pFSM->GetLocalPlayer();
			CombatSysUtil::Inst()->PullLocalPlayer();
			if(MapMgr::Inst()->GetNavMap()->GetCollider()->IfWillSwim(pRole->GetPos(),pRole->GetRoleSize().y))
				m_pFSM->Change2LowerState_Float();
			else
				m_pFSM->Change2LowerState_Stand();		
		}
	}
}

void LowerStateWaterStand_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	//--˲����Ϣ
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_SpecialMove"))
	{
		tagNS_SpecialMove* pCmd=(tagNS_SpecialMove*)pNetCmd;
		if( ESMT_Teleport == pCmd->eType )
		{
			m_pFSM->Change2LowerState_AutoStand();
		}
	}
}

void LowerStateWaterStand_LP::OnExit()
{
	if(m_effectID!=GT_INVALID)
	{
		EffectMgr::Inst()->StopEffect(m_effectID);
		m_effectID=GT_INVALID;
	}
}

bool LowerStateWaterStand_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
	switch(state)
	{
	case FSM_LP::EUpperState_Idle:		//����
	case FSM_LP::EUpperState_Dead:		//����
	case FSM_LP::EUpperState_Dazzy:		//����
	case FSM_LP::EUpperState_Tie:		//����
	case FSM_LP::EUpperState_Sopor:		//��˯
	case FSM_LP::EUpperState_Follow:	//����
	case FSM_LP::EUpperState_TalkNPC:	//��NPC�Ի�
	case FSM_LP::EUpperState_PickupItem://ʰȡ
	case FSM_LP::EUpperState_UseItem:	//ʹ����Ʒ
		{
			return true;
		}
		break;
	}

	return false;
}

bool LowerStateWaterStand_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{	
	return true;
}