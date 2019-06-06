#include "StdAfx.h"
#include "UpperStateUseItem_LP.h"
#include "RoleMgr.h"
#include "FSM_LP.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "..\WorldDefine\msg_combat.h"
#include "SkillMgr.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "CombatActionChecker.h"
#include "FishMgr.h"
#include "Item.h"

UpperStateUseItem_LP::UpperStateUseItem_LP(void)
{}

UpperStateUseItem_LP::~UpperStateUseItem_LP(void)
{}

void UpperStateUseItem_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole )
{
	ASSERT(pEvent->strEventName==_T("tagUseItemEvent"));
	tagUseItemEvent* pAttackEvent=(tagUseItemEvent*)pEvent;

	m_targetID=pAttackEvent->dwTargetRoleID;
	m_itemID=pAttackEvent->n64ItemID;
	m_itemTypeID=pAttackEvent->dwItemTypeID;
	m_dwSerial=CombatSysUtil::MakeSkillMsgSerial();
	m_eventCache.eventType=tagEventCache::EET_None;

	m_step=EStep_None;

	//--��ü����������
	const tagItemDisplayInfo* pItemDisplay=ItemProtoData::Inst()->FindItemDisplay(m_itemTypeID);
	THROW_FALSE(P_VALID(pItemDisplay),_T("cannot find item display info"));

	const tagItemProto* pItemProto=ItemProtoData::Inst()->FindItemProto(m_itemTypeID);
	THROW_FALSE(P_VALID(pItemProto),_T("cannot find item proto"));

	m_bAttackItem	= false;
	m_bSpellItem	= pItemProto->nPrepareTime>0;
	m_bMoveable		= pItemProto->bMoveable==TRUE;
	m_bAniBlend		= pItemDisplay->bActionBlend==TRUE;
	m_bMoveSpell	= pItemProto->bInterruptMove==FALSE;

	//--������Ϊ���ĵı�ը��Ʒ
	if(m_targetID==GT_INVALID)
	{
		if(m_bSpellItem)
		{
			//ֹͣ�ƶ�
			if(!m_bMoveSpell)
				m_pFSM->Change2LowerState_AutoStand();

			m_actSpell.Active(pRole,NULL,m_itemID,m_itemTypeID,m_dwSerial);
			m_step=EStep_Spell;
		}
		else
		{
			///ֹͣ�ƶ�
			if(!m_bMoveable)
				m_pFSM->Change2LowerState_AutoStand();

			m_actAttack.Active(pRole,NULL,m_itemID,m_itemTypeID,m_dwSerial,true);
			m_step=EStep_Attack;
		}
	}
	//--������ʹ��
	else if(m_targetID==RoleMgr::Inst()->GetLocalPlayerID())
	{
		if(m_bSpellItem)
		{
			//ֹͣ�ƶ�
			if(!m_bMoveSpell)
				m_pFSM->Change2LowerState_AutoStand();

			m_actSpell.Active(pRole,pRole,m_itemID,m_itemTypeID,m_dwSerial);
			m_step=EStep_Spell;
		}
		else
		{
			//ֹͣ�ƶ�
			if(!m_bMoveable)
				m_pFSM->Change2LowerState_AutoStand();

			m_actAttack.Active(pRole,pRole,m_itemID,m_itemTypeID,m_dwSerial,true);
			m_step=EStep_Attack;
		}
	}
	//--�Ա���ʹ��
	else
	{
		//��֧��
		m_pFSM->Change2UpperState_Idle();
		return;
	}
}

void UpperStateUseItem_LP::Update( NavMap* pNav,LocalPlayer* pRole )
{
	//--����-----------------------------------------------------------
	if(m_step==EStep_Spell)
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
			m_actAttack.Active(pRole,pTarget,m_itemID,m_itemTypeID,m_dwSerial,false);
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
				else
					m_pFSM->Change2UpperState_Idle();
			}
			//--�л�������״̬
			else
				m_pFSM->Change2UpperState_Idle();
		}
	}
}

void UpperStateUseItem_LP::OnGameEvent( tagGameEvent* pEvent )
{
	//--����-----------------------------------------------------------
	if(m_step==EStep_Spell)
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

			//��������
			m_eventCache.eventType=tagEventCache::EET_UseSkill;
			m_eventCache.useSkill=*pSkillEvent;
		}
		else if(pEvent->strEventName==_T("tagUseItemEvent"))
		{
			tagUseItemEvent* pItemEvent=(tagUseItemEvent*)pEvent;

			//��������
			m_eventCache.eventType=tagEventCache::EET_UseItem;
			m_eventCache.useItem=*pItemEvent;
		}
	}
}

void UpperStateUseItem_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==TObjRef<Util>()->Crc32("NS_UseItem"))
	{
		tagNS_UseItem* pItemCmd=(tagNS_UseItem*)pNetCmd;
		if(pItemCmd->dwSerial==m_dwSerial)
		{
			if(pItemCmd->dwErrorCode==E_Success)
			{
				if(m_step==EStep_Spell)
				{
					m_actSpell.OnRecvSpellTime(pItemCmd->nSpellTime);
				}
			}
			else//ʹ����Ʒʧ��
			{
				IMSG(_T("ʹ����Ʒʱ���������ش�����룺%d"),pItemCmd->dwErrorCode);
				m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_UseItemInterrupt"))//���
	{
		tagNS_UseItemInterrupt* pCmd = (tagNS_UseItemInterrupt*)pNetCmd;
		if( pCmd->n64ItemID == m_itemID )
		{
			m_pFSM->Change2UpperState_Idle();
		}
	}
}

void UpperStateUseItem_LP::OnExit()
{
	if(m_step==EStep_Attack)
		m_actAttack.Stop(m_pFSM->GetLocalPlayer());
	else if(m_step==EStep_Spell)
		m_actSpell.Stop(m_pFSM->GetLocalPlayer(),false);
}

bool UpperStateUseItem_LP::IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent)
{
	return true;
}

bool UpperStateUseItem_LP::IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent)
{
	//--����---------------------------------------
	if(m_step==EStep_Spell)
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
			case FSM_LP::ELowerState_KeyboardSwim:
			case FSM_LP::ELowerState_MouseSwim:
			case FSM_LP::ELowerState_Float:
			case FSM_LP::ELowerState_KeyboardWaterWalk:
			case FSM_LP::ELowerState_MouseWaterWalk:
			case FSM_LP::ELowerState_WaterStand:
				return true;
			}
		}
		else//�������ƶ�����
		{}
	}

	return false;
}

void UpperStateUseItem_LP::OnChangeLowerState(FSM_LP::ELowerState newState,tagGameEvent* pEvent)
{
	//--����---------------------------------------
	if(m_step==EStep_Spell)
	{
		if(m_bMoveSpell)//�����ƶ�����
		{
			switch(newState)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
			case FSM_LP::ELowerState_Stand:
			case FSM_LP::ELowerState_KeyboardSwim:
			case FSM_LP::ELowerState_MouseSwim:
			case FSM_LP::ELowerState_Float:
			case FSM_LP::ELowerState_KeyboardWaterWalk:
			case FSM_LP::ELowerState_MouseWaterWalk:
			case FSM_LP::ELowerState_WaterStand:
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
			case FSM_LP::ELowerState_Float:
			case FSM_LP::ELowerState_WaterStand:
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
			case FSM_LP::ELowerState_KeyboardSwim:
			case FSM_LP::ELowerState_MouseSwim:
			case FSM_LP::ELowerState_Float:
			case FSM_LP::ELowerState_KeyboardWaterWalk:
			case FSM_LP::ELowerState_MouseWaterWalk:
			case FSM_LP::ELowerState_WaterStand:
				break;
			default:
				{
					m_actAttack.Stop(m_pFSM->GetLocalPlayer());
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
		else//�������ƶ�����
		{}
	}

}

bool UpperStateUseItem_LP::IfCanPlayLowerAni()
{
	if(m_step==EStep_None)
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

bool UpperStateUseItem_LP::IfLowerCanSetYaw()
{
	return true;
}
