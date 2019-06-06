#include "StdAfx.h"
#include "UpperStateSkill_LP.h"
#include "RoleMgr.h"
#include "FSM_LP.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "..\WorldDefine\msg_combat.h"
#include "SkillMgr.h"
#include "SkillProtoData.h"
#include "CombatSysUtil.h"
#include "CombatActionChecker.h"
#include "SkillProEvent.h"
#include "PlayerActionData.h"
#include "RoleEvent.h"

UpperStateSkill_LP::UpperStateSkill_LP(void)
{}

UpperStateSkill_LP::~UpperStateSkill_LP(void)
{}

void UpperStateSkill_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole )
{
	ASSERT(pEvent->strEventName==_T("tagUseSkillEvent"));
	tagUseSkillEvent* pAttackEvent=(tagUseSkillEvent*)pEvent;

	m_startSpellTime = 0;
	m_spellRealTime = GT_INVALID;
	m_step=EStep_None;
	m_eventCache.eventType=tagEventCache::EET_None;
	m_bWaitForWeaponSwap = false;

	m_targetID=pAttackEvent->dwTargetRoleID;
	m_skillID=pAttackEvent->dwSkillID;

	//--���漼���������
	const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(m_skillID);
	ASSERT(P_VALID(pSkillData));
	const tagSkillProtoClient* pSkillProto=pSkillData->pProto;
	THROW_FALSE(P_VALID(pSkillProto),_T("cannot find skill proto"));
	m_bAttackSkill		= pSkillProto->bHostile==TRUE;
	m_bSpellSkill		= pSkillData->nPrepareTime>0;
	m_bMoveable			= pSkillProto->bMoveable==TRUE;
	m_bAniBlend			= pSkillProto->bActionBlend==TRUE;
	m_bMoveSpell		= pSkillProto->bInterruptMove==FALSE;
	m_bAutoNormalAttack = pSkillProto->bAutoNormalAttack==TRUE;
	m_dwAttackEndTime = 0;
	m_dwAttackAdvanceTime = 0;
	m_dwAttackEndDelayTime = 0;
	if( m_bSpellSkill )
	{
		const tagPlayerActionData* pActData = PlayerActionData::Inst()->FindData( pSkillProto->szActionMod );
		if( NULL != pActData )
			m_dwAttackAdvanceTime = pActData->dwAttackTime;
	}
	if( m_bAutoNormalAttack )
	{
		m_dwAttackEndDelayTime = m_bSpellSkill ? 500 : 100;
	}

	// ���������ļ��ܣ������л�����
	if( pSkillProto->nWeaponLimit != pRole->GetRWeaponType() && pSkillProto->nWeaponLimit == pRole->GetLWeaponType() && EITE_Null != pSkillProto->nWeaponLimit )
	{
		tagNC_SwapWeapon cmd;
		TObjRef<NetSession>()->Send( &cmd );
		m_eventCache.eventType=tagEventCache::EET_UseSkill;
		m_eventCache.useSkill=*pAttackEvent;
		m_bWaitForWeaponSwap=true;
		return;
	}

	m_dwSerial=CombatSysUtil::MakeSkillMsgSerial();

	//--������Ϊ���ĵı�ը����
	if(m_targetID==GT_INVALID)
	{
		if(m_bSpellSkill)
		{
			//ֹͣ�ƶ�
			if(!m_bMoveSpell)
				m_pFSM->Change2LowerState_Stand();

			m_actSpell.Active(pRole,NULL,m_skillID,m_dwSerial);
			m_step=EStep_Spell;
		}
		else
		{
			//ֹͣ�ƶ�
			if(!m_bMoveable)
				m_pFSM->Change2LowerState_Stand();

			m_actAttack.Active(pRole,NULL,m_skillID,m_dwSerial,true);
			m_step=EStep_Attack;
		}
	}
	//--������ʹ��
	else if(m_targetID==RoleMgr::Inst()->GetLocalPlayerID())
	{
		if(m_bSpellSkill)
		{
			//ֹͣ�ƶ�
			if(!m_bMoveSpell)
				m_pFSM->Change2LowerState_Stand();

			m_actSpell.Active(pRole,pRole,m_skillID,m_dwSerial);
			m_step=EStep_Spell;
		}
		else
		{
			//ֹͣ�ƶ�
			if(!m_bMoveable)
				m_pFSM->Change2LowerState_Stand();

			m_actAttack.Active(pRole,pRole,m_skillID,m_dwSerial,true);
			m_step=EStep_Attack;
		}
	}
	//--�Ա���ʹ��
	else
	{
		//--���Ŀ���Ƿ����
		Role* pTarget=RoleMgr::Inst()->FindRole(m_targetID);
		if(pTarget==NULL)
		{
			m_pFSM->Change2UpperState_Idle();
			return;
		}

		//--���㹥������
		LocalPlayer* pSelf=RoleMgr::Inst()->GetLocalPlayer();

		//--fOPDist�Ƕ�̬�ģ�����Ҫ��SkillMgr��ȡ
		m_attackDist=pSkillData->fOPDist+(pSelf->GetRoleSize().z+pTarget->GetRoleSize().z)*0.5f;
		
		//--���Ŀ���Ƿ��ڹ�����Χ��
		if(IsInAttackRange(pSelf,pTarget))
		{
			if(m_bSpellSkill)
			{
				//ֹͣ�ƶ�
				if(!m_bMoveSpell)
					m_pFSM->Change2LowerState_Stand();

				m_actSpell.Active(pRole,pTarget,m_skillID,m_dwSerial);
				m_step=EStep_Spell;
			}
			else
			{
				//ֹͣ�ƶ�
				if(!m_bMoveable)
					m_pFSM->Change2LowerState_Stand();

				m_actAttack.Active(pRole,pTarget,m_skillID,m_dwSerial,true);
				m_step=EStep_Attack;
			}
		}
		else
		{
			m_step=EStep_Pursue;

			tagPursueEvent event;
			event.start=pSelf->GetPos();
			event.targetType=tagPursueEvent::ETargetType_Role;
			event.targetRoleID=m_targetID;
			event.validDist=m_attackDist;
			event.bNeedSendStopCmd=true;

			if(!m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_Pursue,&event))
			{
				CombatSysUtil::Inst()->ShowActionCheckMsg(CombatActionChecker::EEC_TooFar);//��ʾ����̫Զ
				m_pFSM->Change2UpperState_Idle();
				return;
			}
		}
	}

	//--���ͽ���ս��״̬�¼�
	if( pSkillProto->bInterCombat )
	{
		tagEnterCombatEvent event(_T("tagEnterCombatEvent"),NULL);
		m_pFSM->OnGameEvent(&event);
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
}

void UpperStateSkill_LP::Update( NavMap* pNav,LocalPlayer* pRole )
{
	//--����Ŀ��------------------------------------------------------
	if(m_step==EStep_Pursue)
	{
		//--���Ŀ���Ƿ����
		Role* pTarget=RoleMgr::Inst()->FindRole(m_targetID);
		if(pTarget==NULL)
		{
			m_pFSM->Change2UpperState_Idle();
			SendStopPursueEvent();
			return;
		}
	}
	//--����-----------------------------------------------------------
	else if(m_step==EStep_Spell)
	{
		//--���Ŀ���Ƿ����
		Role* pTarget=NULL;
		if(m_targetID!=GT_INVALID)
		{
			pTarget=RoleMgr::Inst()->FindRole(m_targetID,false,true);
			if(pTarget==NULL)
			{
				m_pFSM->Change2UpperState_Idle();
				return;
			}
		}

		//--
		m_actSpell.Update(pRole,pTarget);
		if(m_actSpell.IsClosed())//����ʱ�����
		{
			m_actAttack.Active(pRole,pTarget,m_skillID,m_dwSerial,false);
			m_step=EStep_Attack;
			return;
		}		
	}
	//--����-----------------------------------------------------------
	else if(m_step==EStep_Attack)
	{
		//--���Ŀ���Ƿ����
		Role* pTarget=NULL;
		if(m_targetID!=GT_INVALID)
		{
			pTarget=RoleMgr::Inst()->FindRole(m_targetID,false,true);
			if(pTarget==NULL)
			{
				m_pFSM->Change2UpperState_Idle();
				return;
			}
		}

		//--
		m_actAttack.Update(pRole,pTarget);
		if(m_actAttack.IsClosed())
		{
			if( 0 == m_dwAttackEndTime )
			{
				m_dwAttackEndTime=Kernel::Inst()->GetAccumTimeDW();
				if( m_dwAttackEndDelayTime > 0 )
				{
					const TCHAR* szTrackName=CombatSysUtil::Inst()->GetReadyTrackName(pRole->GetRWeaponType());
					pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Ready,AvatarAniMixer::EAP_All,szTrackName,true);
				}
			}
			else if( Kernel::Inst()->GetAccumTimeDW() - m_dwAttackEndTime > m_dwAttackEndDelayTime )
			{
				//--����Ƿ��л������Ϸ�¼�
				if(m_eventCache.eventType!=tagEventCache::EET_None)
				{
					if(m_eventCache.eventType==tagEventCache::EET_UseSkill)
					{
						if(!m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&m_eventCache.useSkill))
							m_pFSM->Change2UpperState_Idle();
					}
					else if(m_eventCache.eventType==tagEventCache::EET_UseItem)
					{
						if(!m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,&m_eventCache.useItem))
							m_pFSM->Change2UpperState_Idle();
					}
					else if(m_eventCache.eventType==tagEventCache::EET_MouseMove)
					{
						m_pFSM->Change2UpperState_Idle();
						m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseMove,&m_eventCache.mouseMove);
					}
					else if(m_eventCache.eventType==tagEventCache::EET_KeyboardMove)
					{
						m_pFSM->Change2UpperState_Idle();
						m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardMove,&m_eventCache.keyboardMove);
					}
					else
						m_pFSM->Change2UpperState_Idle();
				}
				//--�Զ�����ͨ����
				else if( m_bAutoNormalAttack 
					&& pTarget!=NULL 
					&& !pTarget->IsInState(ES_Dead) 
					&& !(m_pFSM->IsMoveState() && !CombatSysUtil::Inst()->NormalAttackCanMove()) )
				{
					tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL);
					event.dwSkillID=SkillMgr::Inst()->GetNormalAttackTypeID();
					event.dwTargetRoleID=m_targetID;
					if(!m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&event))
						m_pFSM->Change2UpperState_Idle();
				}
				//--�л�������״̬
				else
					m_pFSM->Change2UpperState_Idle();
			}
		}
	}

	if( m_step==EStep_Attack || m_step==EStep_Spell)
	{
		if( GT_INVALID!=m_spellRealTime &&
			Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime>=m_spellRealTime)
		{
			//--������Ϸ�¼�����������������
			tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
			event.bShow=false;
			event.bSkill=true;
			event.dwTypeID=m_skillID;
			event.dwTime=0;
			TObjRef<GameFrameMgr>()->SendEvent(&event);

			m_startSpellTime = 0;
			m_spellRealTime = GT_INVALID;
		}
	}
}

void UpperStateSkill_LP::OnGameEvent( tagGameEvent* pEvent )
{
	//--�ȴ��л�����
	if(m_bWaitForWeaponSwap)
	{
		if(pEvent->strEventName==_T("tagWeaponChangeEvent"))
		{
			tagWeaponChangeEvent* pWeaponChangeEvent = (tagWeaponChangeEvent*)pEvent;
			if( pWeaponChangeEvent->bRightWeapon )
			{
				if( !m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&m_eventCache.useSkill) )
					m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	//--����Ŀ��------------------------------------------------------
	else if(m_step==EStep_Pursue)
	{
		if(pEvent->strEventName==_T("tagUseSkillEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
		}
		else if(pEvent->strEventName==_T("tagUseItemEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,pEvent);
		}
		else if(pEvent->strEventName==_T("tagFollowRoleEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Follow,pEvent);
		}
		else if(pEvent->strEventName==_T("tagTalkNPCEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_TalkNPC,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPickupItemEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_PickupItem,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPlayPersonalityActEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_StyleAction,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPursueFinishEvent"))
		{
			tagPursueFinishEvent* pFinishEvent=(tagPursueFinishEvent*)pEvent;
			if(pFinishEvent->bSuccessed)
			{
				//--���Ŀ���Ƿ����
				Role* pTarget=RoleMgr::Inst()->FindRole(m_targetID);
				if(pTarget==NULL)
				{
					m_pFSM->Change2UpperState_Idle();
					return;
				}

				if(m_bSpellSkill)
				{
					m_actSpell.Active(m_pFSM->GetLocalPlayer(),pTarget,m_skillID,m_dwSerial);
					m_step=EStep_Spell;
				}
				else
				{
					m_actAttack.Active(m_pFSM->GetLocalPlayer(),pTarget,m_skillID,m_dwSerial,true);
					m_step=EStep_Attack;
				}
			}
			else
			{
				m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	//--����-----------------------------------------------------------
	else if(m_step==EStep_Spell)
	{
/*		if(pEvent->strEventName==_T("tagUseSkillEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
		}
		else if(pEvent->strEventName==_T("tagUseItemEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,pEvent);
		}
		else */if(pEvent->strEventName==_T("tagFollowRoleEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Follow,pEvent);
		}
		else if(pEvent->strEventName==_T("tagTalkNPCEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_TalkNPC,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPickupItemEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_PickupItem,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPlayPersonalityActEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_StyleAction,pEvent);
		}
		else if(pEvent->strEventName==_T("tagESCCancelEvent"))
		{
			tagESCCancelEvent* pCancelEvent=(tagESCCancelEvent*)pEvent;

			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->Change2UpperState_Idle();

			pCancelEvent->bCanceled=TRUE;
		}
	}
	//--����-----------------------------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(pEvent->strEventName==_T("tagUseSkillEvent"))
		{
			tagUseSkillEvent* pSkillEvent=(tagUseSkillEvent*)pEvent;

			if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//��ǰʹ����ͨ����
			{
				if(pSkillEvent->dwSkillID==SkillMgr::Inst()->GetNormalAttackTypeID())//��ͨ����
				{
					if(pSkillEvent->dwTargetRoleID==m_targetID)//ͬһĿ��
					{
						//�����κδ���
					}
					else//��ͬĿ��
					{
						//ֱ����
						m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
					}
				}
				else//����
				{
					//ֱ����
					m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
				}
			}
			else//��ǰʹ�ü���
			{
				//��������
				m_eventCache.eventType=tagEventCache::EET_UseSkill;
				m_eventCache.useSkill=*pSkillEvent;
			}
		}
		else if(pEvent->strEventName==_T("tagUseItemEvent"))
		{
			tagUseItemEvent* pItemEvent=(tagUseItemEvent*)pEvent;

			if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//��ǰʹ����ͨ����
			{
				//ֱ����
				m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,pEvent);
			}
			else
			{
				//��������
				m_eventCache.eventType=tagEventCache::EET_UseItem;
				m_eventCache.useItem=*pItemEvent;
			}
		}
		else if(pEvent->strEventName==_T("tagMouseMoveEvent"))
		{
			tagMouseMoveEvent* pMoveEvent=(tagMouseMoveEvent*)pEvent;

			if(!m_bMoveable
				&&m_skillID!=SkillMgr::Inst()->GetNormalAttackTypeID())
			{
				//��������
				m_eventCache.eventType=tagEventCache::EET_MouseMove;
				m_eventCache.mouseMove=*pMoveEvent;
			}
		}
		else if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))
		{
			tagKeyboardMoveEvent* pMoveEvent=(tagKeyboardMoveEvent*)pEvent;

			if(!m_bMoveable
				&&m_skillID!=SkillMgr::Inst()->GetNormalAttackTypeID())
			{
				//��������
				m_eventCache.eventType=tagEventCache::EET_KeyboardMove;
				m_eventCache.keyboardMove=*pMoveEvent;
			}
		}
		else if(pEvent->strEventName==_T("tagStopKeyboardMoveEvent"))
		{
			if(m_eventCache.eventType==tagEventCache::EET_KeyboardMove)
				m_eventCache.eventType=tagEventCache::EET_None;
		}
		else if(pEvent->strEventName==_T("tagESCCancelEvent"))
		{
			tagESCCancelEvent* pCancelEvent=(tagESCCancelEvent*)pEvent;

			if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//��ǰʹ����ͨ����
			{
				m_actAttack.Stop(m_pFSM->GetLocalPlayer(),true);
				m_pFSM->Change2UpperState_Idle();

				pCancelEvent->bCanceled=TRUE;
			}
		}
	}
}

void UpperStateSkill_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==TObjRef<Util>()->Crc32("NS_Skill"))
	{
		tagNS_Skill* pSkillCmd=(tagNS_Skill*)pNetCmd;
		if(pSkillCmd->dwSerial==m_dwSerial)
		{
			//IMSG(_T("[%d]recv ns_skill,skill=%d,errcode=%d\n"),timeGetTime(),pSkillCmd->dwSkillID,pSkillCmd->dwErrorCode);
			if(pSkillCmd->dwErrorCode==E_Success)
			{
				if(m_step==EStep_Spell)
				{
					// ����������������ʱ�䣬Ԥ����������
					DWORD dwSpellActionTime = 0;
					if( pSkillCmd->nSpellTime > 0 && (DWORD)pSkillCmd->nSpellTime > m_dwAttackAdvanceTime )
					    dwSpellActionTime = pSkillCmd->nSpellTime - m_dwAttackAdvanceTime;
					m_actSpell.OnRecvSpellTime(dwSpellActionTime);

					if( pSkillCmd->nSpellTime > 0 )
					{
						m_spellRealTime= pSkillCmd->nSpellTime;
						m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

						//--������Ϸ�¼�����ʾ����������
						tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
						event.bShow=true;
						event.bSkill=true;
						event.dwTypeID=m_skillID;
						event.dwTime=m_spellRealTime;
						TObjRef<GameFrameMgr>()->SendEvent(&event);
					}
				}
			}
			else//ʹ�ü���ʧ��
			{
				IMSG(_T("ʹ�ü���ʱ���������ش�����룺%d"),pSkillCmd->dwErrorCode);
				m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_SkillInterrupt"))
	{
		tagNS_SkillInterrupt* pCmd = (tagNS_SkillInterrupt*)pNetCmd;
		if( pCmd->dwSkillID == m_skillID )
			m_pFSM->Change2UpperState_Idle();
	}
}

bool UpperStateSkill_LP::IsInAttackRange(LocalPlayer* pRole,Role* pTarget)
{
	return Vec3DistSq(pRole->GetPos(),pTarget->GetPos()) < m_attackDist*m_attackDist;
}

void UpperStateSkill_LP::SendStopPursueEvent()
{
	tagStopPursueEvent event;
	m_pFSM->OnGameEvent(&event);
}

void UpperStateSkill_LP::OnExit()
{
	if(m_step==EStep_Attack)
		m_actAttack.Stop(m_pFSM->GetLocalPlayer(),false);
	else if(m_step==EStep_Spell)
		m_actSpell.Stop(m_pFSM->GetLocalPlayer(),false);

	if( GT_INVALID!=m_spellRealTime )
	{
		//--������Ϸ�¼�����������������
		tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
		event.bShow=false;
		event.bSkill=true;
		event.dwTypeID=m_skillID;
		event.dwTime=0;
		TObjRef<GameFrameMgr>()->SendEvent(&event);

		m_startSpellTime = 0;
		m_spellRealTime = GT_INVALID;
	}
}

bool UpperStateSkill_LP::IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent)
{
	return true;
}

bool UpperStateSkill_LP::IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent)
{
	//--׷��---------------------------------------
	if(m_step==EStep_Pursue||m_bWaitForWeaponSwap)
	{
		return true;
	}
	//--����---------------------------------------
	else if(m_step==EStep_Spell)
	{
		return true;
	}
	//--����---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//�����ƶ�����
		{
			switch(state)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
			case FSM_LP::ELowerState_Stand:
				return true;
			}
		}
		if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//��ͨ�������Ա��ƶ����
			return true;
	}

	return false;
}

void UpperStateSkill_LP::OnChangeLowerState(FSM_LP::ELowerState newState,tagGameEvent* pEvent)
{
	//--׷��---------------------------------------
	if(m_step==EStep_Pursue||m_bWaitForWeaponSwap)
	{
		switch(newState)
		{
		case FSM_LP::ELowerState_Pursue:
		case FSM_LP::ELowerState_Jump:
		case FSM_LP::ELowerState_Slide:
		case FSM_LP::ELowerState_VDrop:
		case FSM_LP::ELowerState_Drop:
		case FSM_LP::ELowerState_Stand:
			break;
		case FSM_LP::ELowerState_KeyboardMove:
			{
				if( !m_bWaitForWeaponSwap )
					m_pFSM->Change2UpperState_Idle();
			}
			break;
		default:
			{
				m_pFSM->Change2UpperState_Idle();
			}
			break;
		}
	}
	//--����---------------------------------------
	else if(m_step==EStep_Spell)
	{
		if(m_bMoveSpell)//�����ƶ�����
		{
			switch(newState)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
			case FSM_LP::ELowerState_Stand:
				break;
			default:
				{
					m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
		else//�������ƶ�����
		{
			switch(newState)
			{
			case FSM_LP::ELowerState_Stand:
				break;
			default:
				{
					m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
	}
	//--����---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//�����ƶ�����
		{
			switch(newState)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
			case FSM_LP::ELowerState_Stand:
				break;
			default:
				{
					m_actAttack.Stop(m_pFSM->GetLocalPlayer(),false);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
		else//�������ƶ�����
		{
			//��ͨ�������ܱ�����ƶ��ͼ����ƶ����
			switch(newState)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
				{
					m_actAttack.Stop(m_pFSM->GetLocalPlayer(),false);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
	}

}

bool UpperStateSkill_LP::IfCanPlayLowerAni()
{
	if(m_step==EStep_None)
	{
		return true;
	}
	//--׷��---------------------------------------
	else if(m_step==EStep_Pursue)
	{
		return true;
	}
	//--����---------------------------------------
	else if(m_step==EStep_Spell)
	{
		return true;
	}
	//--����---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//�����ƶ�����
		{
			if(m_bAniBlend)
				return true;
		}
		else//�������ƶ�����
		{}
	}

	return false;
}

bool UpperStateSkill_LP::IfLowerCanSetYaw()
{
	if(m_step==EStep_None)
	{
		return true;
	}
	//--׷��---------------------------------------
	else if(m_step==EStep_Pursue)
	{
		return true;
	}
	//--����---------------------------------------
	else if(m_step==EStep_Spell)
	{
		return true;
	}
	//--����---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//�����ƶ�����
		{
			if(m_bAniBlend)
				return true;
		}
		else//�������ƶ�����
		{}
	}

	return false;
}
