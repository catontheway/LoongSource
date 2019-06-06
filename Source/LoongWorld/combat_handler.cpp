//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: combat_handler.cpp
// author: Aslan
// actor:
// data: 2008-09-25
// last:
// brief: ս��ϵͳ������
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/msg_combat.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "unit.h"
#include "map.h"
#include "creature.h"
#include "role.h"
#include "combat_handler.h"
#include "script_mgr.h"
#include "title_mgr.h"

//-----------------------------------------------------------------------------
// ʹ�ü���
//-----------------------------------------------------------------------------
INT CombatHandler::UseSkill(DWORD dwSkillID, DWORD dwTargetUnitID, DWORD dwSerial)
{
	Skill* pSkill = m_pOwner->GetSkill(dwSkillID);
	if( !P_VALID(pSkill) ) return E_UseSkill_SkillNotExist;

	Unit* pTargetUnit = m_pOwner->GetMap()->FindUnit(dwTargetUnitID);

	// ����ܷ�Ը�Ŀ�귢������
	INT nRet = CanCastSkill(pSkill, dwTargetUnitID);
	if( E_Success != nRet )	return nRet;

	// �û������������ƣ��������Ӧ����ո�¶�
	if( EITE_Null != pSkill->GetProto()->nWeaponLimit && m_pOwner->IsRole() )
		((Role*)m_pOwner)->GetItemMgr().ProcEquipNewness();

	// ���ü����Ƿ��ܹ��ƶ�ʩ��
	if( !pSkill->IsMoveable() )
	{
		m_pOwner->GetMoveData().StopMoveForce();
	}

	// ���Ŀ������Ҳ����Լ�����ı�����
	if( P_VALID(pTargetUnit) && m_pOwner->GetID() != pTargetUnit->GetID() )
	{
		m_pOwner->GetMoveData().SetFaceTo(pTargetUnit->GetCurPos() - m_pOwner->GetCurPos());
	}


	// ���ʹ�ü��ܴ�ϵ�buff
	m_pOwner->OnInterruptBuffEvent(EBIF_InterCombat);

	// ���ò�����׼������
	m_dwSkillID			=	dwSkillID;
	m_dwTargetUnitID	=	dwTargetUnitID;
	m_dwSkillSerial		=	dwSerial;

	// ����ü�����Ҫ���֣����������ֵ���ʱ��������뼼�����ý׶�
	m_nSkillPrepareCountDown = (INT)(pSkill->GetPrepareTime() * m_fSkillPrepareModPct);

	if( m_nSkillPrepareCountDown > 0 )
	{
		m_bSkillPreparing	=	TRUE;
		m_bSkillOperating	=	FALSE;
	}
	else
	{
		// ���Է��������ü�����ȴ
		m_pOwner->StartSkillCoolDown(pSkill);

		m_bSkillPreparing	=	FALSE;
		m_bSkillOperating	=	TRUE;
		m_nSkillOperateTime	=	0;
		m_nSkillCurDmgIndex	=	0;

		// ����Ŀ��
		CalSkillTargetList();
	}

	return nRet;
}

//-------------------------------------------------------------------------------
// ʹ����Ʒ
//-------------------------------------------------------------------------------
INT CombatHandler::UseItem(INT64 n64ItemID, DWORD dwTargetUnitID, DWORD dwSerial, DWORD &dwTypeID, bool& bImmediate)
{
	if( GT_INVALID == dwTargetUnitID )
		dwTargetUnitID = m_pOwner->GetID();

	// ����ǲ������
	if( !m_pOwner->IsRole() ) return E_UseItem_TargetInvalid;
	Role* pOwnerRole = static_cast<Role*>(m_pOwner);

	// �����Ʒ�Ƿ��ڱ�����
	tagItem* pItem = pOwnerRole->GetItemMgr().GetBagItem(n64ItemID); 
	if( !P_VALID(pItem) ) return E_UseItem_ItemNotExist;

	// ����ܷ�ʹ����Ʒ
	INT nRet = E_Success;
	BOOL bIgnore = FALSE;		// �Ƿ�����ܷ�ʹ����Ʒ��ͨ���ж�

	if(P_VALID(pItem->pScript) && P_VALID(pOwnerRole->GetMap()))
	{
		// ���ű�����Ʒʹ������
		nRet = pItem->pScript->CanUseItem(pOwnerRole->GetMap(), pItem->dwTypeID, dwTargetUnitID, bIgnore);

		// ���»�ȡ��Ʒָ��
		pItem = pOwnerRole->GetItemMgr().GetBagItem(n64ItemID); 
		if( !P_VALID(pItem) ) return E_UseItem_ItemNotExist;
	}

	// ���ʹ����Ʒ��ͨ���ж�
	if(!bIgnore && E_Success == nRet)
		nRet = CanUseItem(pItem);

	if( E_Success != nRet ) return nRet;

	// �������Ʒ�����ƶ�ʹ�ã���ͣ��
	if( !pItem->pProtoType->bMoveable )
	{
		m_pOwner->GetMoveData().StopMoveForce();
	}

	// ���ʹ����Ʒ��ϵ�buff
	m_pOwner->OnInterruptBuffEvent(EBIF_InterCombat);

	// ������ͨ��������������Ӧ�Ĳ�����׼������
	m_n64ItemID			=	n64ItemID;
	m_dwItemSerial		=	dwSerial;
	dwTypeID			=	pItem->dwTypeID;
	m_dwTargetUnitIDItem = dwTargetUnitID;

	// ���ʹ����Ʒ��Ҫ���֣����������ֵ���ʱ������������ý׶�
	m_nItemPrepareCountDown = pItem->pProtoType->nPrepareTime;
	if( m_nItemPrepareCountDown > 0 )
	{
		m_bItemPreparing	=	TRUE;
		m_bItemOperating	=	FALSE;
		bImmediate			=	false;
	}
	else
	{
		m_bItemPreparing	=	FALSE;
		m_bItemOperating	=	TRUE;
		bImmediate			=	true;
	}

	return nRet;
}

//-----------------------------------------------------------------------------
// ���¼������֣�������ֽ����ˣ����л�������״̬
//-----------------------------------------------------------------------------
VOID CombatHandler::UpdateSkillPrepare()
{
	if( !IsUseSkill() ) return;
	if( !IsSkillPreparing() ) return;

	// ��ȥTickʱ��
	m_nSkillPrepareCountDown -= TICK_TIME;

	// ����ʱ��������л�������״̬
	if( m_nSkillPrepareCountDown <= 0 )
	{
		Skill* pSkill = m_pOwner->GetSkill(m_dwSkillID);

		if( P_VALID(pSkill) )
		{
			m_pOwner->StartSkillCoolDown(pSkill);
		}

		m_bSkillPreparing = FALSE;
		m_bSkillOperating = TRUE;
		m_nSkillOperateTime = 0;
		m_nSkillCurDmgIndex = 0;

		// ����Ŀ��
		CalSkillTargetList();
	}
}

//-----------------------------------------------------------------------------
// �������֣�������ֽ����ˣ����л�������״̬
//-----------------------------------------------------------------------------
VOID CombatHandler::UpdateItemPrepare()
{
	if( !IsUseItem() ) return;
	if( !IsItemPreparing() ) return;

	// ��ȥTickʱ��
	m_nItemPrepareCountDown -= TICK_TIME;

	// ����ʱ��������л�������״̬
	if( m_nItemPrepareCountDown <= 0 )
	{
		m_bItemPreparing = FALSE;
		m_bItemOperating = TRUE;
	}
}


//-------------------------------------------------------------------------------
// ���¼��ܲ�����������˼����˺���ʱ��������˺�������˺��������ˣ������buff
//-------------------------------------------------------------------------------
VOID CombatHandler::UpdateSkillOperate()
{
	if( !IsUseSkill() ) return;
	if( !IsSkillOperating() ) return;

	// �����ҵ��������
	Skill* pSkill = m_pOwner->GetSkill(m_dwSkillID);
	if( !P_VALID(pSkill) )
	{
		EndUseSkill();
		return;
	}

	Map* pMap = m_pOwner->GetMap();
	if( !P_VALID(pMap) ) return;

	// �õ��������˺�����
	INT nDmgTimes = pSkill->GetDmgTimes();

	if (m_pOwner->IsRole())
	{
		((Role*)m_pOwner)->GetTitleMgr()->SigEvent(ETE_USE_SKILL, pSkill->GetTypeID(), GT_INVALID);
	}

	// ����˺�����Ϊ0��˵���ü������˺�����ֱ�ӽ��뵽����buff�׶�
	if( nDmgTimes <= 0 )
	{
		// ����buff
		m_pOwner->OnActiveSkillBuffTrigger(pSkill, m_listTargetID, ETEE_Use);
		// ��������
		CalculateCost(pSkill);
		// ����
		EndUseSkill();
		return;
	}

	// �˺�������Ϊ0�����⵱ǰʱ�䵽���Ĵ��˺�
	m_nSkillOperateTime += TICK_TIME;

	for(; m_nSkillCurDmgIndex < nDmgTimes; m_nSkillCurDmgIndex++)
	{
		// ��tick��ɲ�����˶���˺����㣬�ȵ��¸�tick
		if( pSkill->GetDmgTime(m_nSkillCurDmgIndex) > m_nSkillOperateTime )
			break;

		// ʱ�䵽�ˣ���ʼ�����˺�
		TList<DWORD>::TListIterator it = m_listTargetID.Begin();
		DWORD dwTargetID = GT_INVALID;

		while( m_listTargetID.PeekNext(it, dwTargetID) )
		{
			// �ҵ����Ŀ��
			Unit* pTarget = pMap->FindUnit(dwTargetID);

			if( !P_VALID(pTarget) ) continue;

			// �����˺�
			CalculateDmg(pSkill, pTarget);
		}
	}

	// ��������˺��Ƿ��Ѿ��������
	if( m_nSkillCurDmgIndex >= nDmgTimes )
	{
		// ������������Buff
		m_pOwner->OnActiveSkillBuffTrigger(pSkill, m_listHitedTarget, ETEE_Hit);
		m_pOwner->OnActiveSkillBuffTrigger(pSkill, m_listDodgedTarget, ETEE_Dodged);
		m_pOwner->OnActiveSkillBuffTrigger(pSkill, m_listBlockedTarget, ETEE_Blocked);
		m_pOwner->OnActiveSkillBuffTrigger(pSkill, m_listCritedTarget, ETEE_Crit);
		m_pOwner->OnActiveSkillBuffTrigger(pSkill, m_listHitedTarget, ETEE_Use);

		// �ҵ�Ŀ��
		Unit* pTarget = pMap->FindUnit(m_dwTargetUnitID);
		if( P_VALID(pTarget) )
		{
			// Buff����
			if( m_dwTargetEffectFlag & ETEF_Hited )
			{
				// ����
				m_pOwner->OnBuffTrigger(pTarget, ETEE_Hit);
			}
			else
			{
				// ������
				m_pOwner->OnBuffTrigger(pTarget, ETEE_Dodged);
			}

			if( m_dwTargetEffectFlag & ETEF_Block )
			{
				// ����
				m_pOwner->OnBuffTrigger(pTarget, ETEE_Blocked);
			}

			if( m_dwTargetEffectFlag & ETEF_Crited )
			{
				// ����
				m_pOwner->OnBuffTrigger(pTarget, ETEE_Crit);
			}

			// ���㱻�����ܺ�װ��Buff
			if( m_pOwner->IsRole() )
			{
				// ��Ե�һĿ����м���
				Role* pOwnerRole = static_cast<Role*>(m_pOwner);

				if( m_dwTargetEffectFlag & ETEF_Hited )
				{
					// ����
					pOwnerRole->OnPassiveSkillBuffTrigger(pTarget, ETEE_Hit);
					pOwnerRole->OnEquipmentBuffTrigger(NULL, ETEE_Hit);
				}
				else
				{
					// ������
					pOwnerRole->OnPassiveSkillBuffTrigger(pTarget, ETEE_Dodged);
					pOwnerRole->OnEquipmentBuffTrigger(NULL, ETEE_Dodged);
				}

				if( m_dwTargetEffectFlag & ETEF_Block )
				{
					// ����
					pOwnerRole->OnPassiveSkillBuffTrigger(pTarget, ETEE_Blocked);
					pOwnerRole->OnEquipmentBuffTrigger(NULL, ETEE_Blocked);
				}

				if( m_dwTargetEffectFlag & ETEF_Crited )
				{
					// ����
					pOwnerRole->OnPassiveSkillBuffTrigger(pTarget, ETEE_Crit);
					pOwnerRole->OnEquipmentBuffTrigger(NULL, ETEE_Crit);
				}
			}
		}

		// ��������
		CalculateCost(pSkill);

		if (m_pOwner->IsRole())
		{
			Role *pRole = dynamic_cast<Role *>(m_pOwner);
			if (NULL != pRole)
			{
				pRole->GetTitleMgr()->SigEvent(ETE_USE_SKILL, pSkill->GetID(), GT_INVALID);
			}
		}

		// ���ܽ���
		EndUseSkill();
	}
}

//-----------------------------------------------------------------------------
// ����ʹ����ƷЧ��
//-----------------------------------------------------------------------------
VOID CombatHandler::UpdateItemOperate()
{
	if( !IsUseItem() ) return;
	if( !IsItemOperating() ) return;
	if( !m_pOwner->IsRole() ) return;

	Role* pOwnerRole = static_cast<Role*>(m_pOwner);

	// �����ҵ������Ʒ
	tagItem* pItem = pOwnerRole->GetItemMgr().GetBagItem(m_n64ItemID); 
	if( !P_VALID(pItem) )
	{
		EndUseItem();
		return;
	}

	DWORD	dwTypeID = pItem->dwTypeID;
	Map* pMap = pOwnerRole->GetMap();
	if( !P_VALID(pMap) ) return;

	// ��������Ŀ����ͻ���
	tagNS_HitTarget send;
	send.dwRoleID = m_pOwner->GetID();
	send.dwSrcRoleID = m_pOwner->GetID();
	send.eCause = EHTC_Item;
	send.dwMisc = pItem->dwTypeID;
	send.dwSerial = m_dwItemSerial;
	pMap->SendBigVisTileMsg(m_pOwner, &send, send.dwSize);

	// ����buff
	pOwnerRole->OnActiveItemBuffTrigger(pItem, ETEE_Use);

	// ������Ʒ�Ľű�ʹ��Ч��
	if(P_VALID(pItem->pScript) && P_VALID(pOwnerRole->GetMap()))
	{
		pItem->pScript->UseItem(pOwnerRole->GetMap(), pItem->dwTypeID, m_dwTargetUnitIDItem);
	}

	// �ƺ���Ϣ
	pOwnerRole->GetTitleMgr()->SigEvent(ETE_USE_ITEM, dwTypeID, GT_INVALID);

	// ������Ʒ������ȴʱ��
	pOwnerRole->GetItemMgr().Add2CDTimeMap(dwTypeID);

	// ������Ʒ��ʧ
	pOwnerRole->GetItemMgr().ItemUsedFromBag(m_n64ItemID, 1, (DWORD)ELCLD_Item_Use);

	EndUseItem();
}

//-----------------------------------------------------------------------------------
// ȡ������ʹ��
//-----------------------------------------------------------------------------------
VOID CombatHandler::CancelSkillUse(DWORD dwSkillTypeID)
{
	if( !IsValid() || !IsUseSkill() ) return;

	Skill* pSkill = m_pOwner->GetSkill(m_dwSkillID);
	if( !P_VALID(pSkill) || pSkill->GetTypeID() != dwSkillTypeID )
		return;

	BOOL bCanCancel = FALSE;

	// ��������������֣�һ������
	if( IsSkillPreparing() )
	{
		bCanCancel = TRUE;
	}
	// ��������ͷţ���ֻ����ͨ�����ſ���
	else
	{
		if( ESSTE_Default == pSkill->GetTypeEx() )
		{
			bCanCancel = TRUE;
		}
	}

	// �������ȡ��
	if( bCanCancel )
	{
		tagNS_SkillInterrupt send;
		send.dwRoleID = m_pOwner->GetID();
		send.dwSkillID = dwSkillTypeID;

		if( P_VALID(m_pOwner->GetMap()) )
		{
			m_pOwner->GetMap()->SendBigVisTileMsg(m_pOwner, &send, send.dwSize);
		}
		EndUseSkill();
	}
}

//-----------------------------------------------------------------------------------
// ȡ����Ʒ�ͷ�
//-----------------------------------------------------------------------------------
VOID CombatHandler::CancelItemUse(INT64 n64ItemSerial)
{
	if( !IsValid() || !IsUseItem() ) return;

	if( m_n64ItemID != n64ItemSerial ) return;

	BOOL bCanCancel = FALSE;

	// ��Ʒֻ��������ʱ����ȡ��
	if( IsItemPreparing() )
	{
		bCanCancel = TRUE;
	}

	if( bCanCancel )
	{
		tagNS_UseItemInterrupt send;
		send.dwRoleID = m_pOwner->GetID();
		send.n64ItemID = m_n64ItemID;
		send.dwTypeID = GT_INVALID;

		if( P_VALID(m_pOwner->GetMap()) )
		{
			m_pOwner->GetMap()->SendBigVisTileMsg(m_pOwner, &send, send.dwSize);
		}
		EndUseItem();
	}
}

//-----------------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------------
BOOL CombatHandler::InterruptPrepare(EInterruptType eType, BOOL bOrdinary, BOOL bForce)
{
	if( FALSE == IsValid() || FALSE == IsPreparing() )
		return TRUE;

	BOOL bSkill = FALSE;		// �Ǽ��������ֻ�����Ʒ������
	DWORD dwSkillTypeID = GT_INVALID;
	if( IsSkillPreparing() )	bSkill = TRUE;
	else						bSkill = FALSE;

	// ͨ����ʹ����Ʒ����ʹ�ü������жϴ��ֵ
	BOOL bMoveInterrupt = FALSE;
	INT nInterruptSkillRate = 0;

	if( bSkill )
	{
		Skill* pSkill = m_pOwner->GetSkill(m_dwSkillID);
		if( P_VALID(pSkill) )
		{
			const tagSkillProto* pProto = pSkill->GetProto();
			if( P_VALID(pProto) )
			{
				bMoveInterrupt = pProto->bInterruptMove;
				nInterruptSkillRate = (bOrdinary ? pProto->nInterruptSkillOrdRate : pProto->nInterruptSkillSpecRate);
			}
			dwSkillTypeID = pSkill->GetTypeID();
		}
	}
	else
	{
		Role* pRole = static_cast<Role*>(m_pOwner);
		tagItem* pItem = pRole->GetItemMgr().GetBagItem(m_n64ItemID);
		if( P_VALID(pItem) )
		{
			bMoveInterrupt = pItem->pProtoType->bInterruptMove;
			nInterruptSkillRate = pItem->pProtoType->nInterruptSkillOrdRate;
		}
	}

	BOOL bCanInterrupt = FALSE;	// �Ƿ��ܹ����

	if( bForce )
	{
		bCanInterrupt = TRUE;
	}
	else
	{
		// ���Դ��
		switch(eType)
		{
		case EIT_Move:
			{
				if( bMoveInterrupt )
				{
					bCanInterrupt = TRUE;
				}
			}
			break;

		case EIT_Skill:
			{
				// ��ͨ������ϼ���
				if( IUTIL->Rand() % 10000 < nInterruptSkillRate )
				{
					bCanInterrupt = TRUE;
				}
			}
			break;

		default:
			break;
		}
	}

	if( bCanInterrupt )
	{
		// ���ʹ�ϸ���Χ���
		if( bSkill )
		{
			tagNS_SkillInterrupt send;
			send.dwRoleID = m_pOwner->GetID();
			send.dwSkillID = dwSkillTypeID;

			if( P_VALID(m_pOwner->GetMap()) )
			{
				m_pOwner->GetMap()->SendBigVisTileMsg(m_pOwner, &send, send.dwSize);
			}
			EndUseSkill();
		}
		else
		{
			tagNS_UseItemInterrupt send;
			send.dwRoleID = m_pOwner->GetID();
			send.n64ItemID = m_n64ItemID;
			send.dwTypeID = GT_INVALID;

			if( P_VALID(m_pOwner->GetMap()) )
			{
				m_pOwner->GetMap()->SendBigVisTileMsg(m_pOwner, &send, send.dwSize);
			}
			EndUseItem();
		}

		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------------------------
// ����ͷ�
//-------------------------------------------------------------------------------
BOOL CombatHandler::InterruptOperate(EInterruptType eType, DWORD dwMisc, BOOL bForce/* =FALSE */)
{
	if( FALSE == IsValid() || FALSE == IsSkillOperating() )
		return TRUE;

	if( EIT_Move == eType )
	{
		EMoveState eState = (EMoveState)dwMisc;

		// �ߺ���Ӿ��ص��ƶ�����ֻ���ƶ���ϵ���ͨ�����Ŵ��
		if( EMS_Walk			== eState ||
			EMS_Swim			== eState ||
			EMS_CreaturePatrol	== eState ||
			EMS_CreatureWalk	== eState ||
			EMS_CreatureFlee	== eState )
		{
			Skill* pSkill = m_pOwner->GetSkill(m_dwSkillID);
			if( P_VALID(pSkill) && ESSTE_Default == pSkill->GetTypeEx() && !pSkill->IsMoveable() )
			{
				EndUseSkill();
				return TRUE;
			}
		}
		// �����ƶ���ʽ����ֻҪ����ͨ�����ʹ��
		else
		{
			Skill* pSkill = m_pOwner->GetSkill(m_dwSkillID);
			if( P_VALID(pSkill) && ESSTE_Default == pSkill->GetTypeEx() )
			{
				EndUseSkill();
				return TRUE;
			}
		}
	}

	return FALSE;
}


//-------------------------------------------------------------------------------
// �Ƿ����ʹ�ü���
//-------------------------------------------------------------------------------
INT CombatHandler::CanCastSkill(Skill* pSkill, DWORD dwTargetUnitID)
{
	if( !P_VALID(pSkill) )
		return E_SystemError;

	if( CheckSkillConflict(pSkill) ) return E_UseSkill_Operating;

	INT nRet = E_Success;

	nRet = CheckSkillAbility(pSkill);
	if( E_Success != nRet ) return nRet;

	nRet = CheckOwnerLimitSkill();
	if( E_Success != nRet ) return nRet;

	nRet = CheckSkillLimit(pSkill);
	if( E_Success != nRet ) return nRet;

	nRet = CheckTargetLimit(pSkill, dwTargetUnitID);
	if( E_Success != nRet ) return nRet;

	nRet = CheckCostLimit(pSkill);
	if( E_Success != nRet ) return nRet;

	nRet = CheckVocationLimit(pSkill);
	if( E_Success != nRet ) return nRet;

	nRet = CheckMapLimit(pSkill);
	if( E_Success != nRet ) return nRet;

	const SkillScript* pScript = pSkill->GetSkillScript();
	if (P_VALID(pScript))
	{
		nRet = pScript->CanCastSkill(m_pOwner->GetMap(), pSkill->GetID(), m_pOwner->GetID(), dwTargetUnitID);
		if( E_Success != nRet ) return nRet;
	}

	return nRet;
}

//-------------------------------------------------------------------------------
// ���Լ��ܱ����Ƿ��ܹ�ʹ��
//-------------------------------------------------------------------------------
INT CombatHandler::CheckSkillAbility(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return E_UseSkill_SkillNotExist;

	// �������ܲ�����ʹ��
	if( pSkill->IsPassive() )
		return E_UseSkill_PassiveSkill;

	// ������ܵ�Ŀ�����Ͳ���ս��Ŀ����ս��Ŀ�꣬�򲻿���ʹ��
	ESkillTargetType eTargetType = pSkill->GetTargetType();
	if( ESTT_Combat != eTargetType && ESTT_NoCombat != eTargetType )
		return E_UseSkill_SkillTargetInvalid;

	// ���ܵ���ȴʱ�仹û�����򲻿���ʹ��
	if( pSkill->GetCoolDownCountDown() > 0 )
		return E_UseSkill_CoolDowning;

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���Լ���ʹ�����Ƿ��ܹ�ʹ�ü���
//-------------------------------------------------------------------------------
INT CombatHandler::CheckOwnerLimitSkill()
{
	// �Ƿ��ڲ���ʹ�ü��ܵ�״̬
	if( m_pOwner->IsInStateCantCastSkill() )
		return E_UseSkill_UseLimit;

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���Լ��ܱ���ʹ������
//-------------------------------------------------------------------------------
INT CombatHandler::CheckSkillLimit(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return E_UseSkill_SkillNotExist;

	const tagSkillProto* pProto = pSkill->GetProto();
	if( !P_VALID(pProto) ) return E_UseSkill_SkillNotExist;

	// �������ڻ�����������
	if( pProto->nUseHPPctLimit > 0 )
	{
		if( m_pOwner->GetAttValue(ERA_MaxHP) <= 0 )
			return E_UseSkill_UseLimit;

		if( (FLOAT)m_pOwner->GetAttValue(ERA_HP) / (FLOAT)m_pOwner->GetAttValue(ERA_MaxHP) * 10000.0f < pProto->nUseHPPctLimit )
			return E_UseSkill_UseLimit;

	}
	if( pProto->nUseMPPctLimit > 0 )
	{
		if( m_pOwner->GetAttValue(ERA_MaxMP) <= 0 )
			return E_UseSkill_UseLimit;

		if( (FLOAT)m_pOwner->GetAttValue(ERA_MP) / (FLOAT)m_pOwner->GetAttValue(ERA_MaxMP) * 10000.0f < pProto->nUseHPPctLimit )
			return E_UseSkill_UseLimit;

	}

	// �Ա�����
	if( pProto->eSexLimit != ESSL_Null )
	{
		if( ESSL_Man == pProto->eSexLimit )
		{
			if( 1 != m_pOwner->GetSex() )
				return E_UseSkill_SexLimit;
		}
		else if( ESSL_Woman == pProto->eSexLimit )
		{
			if( 0 != m_pOwner->GetSex() )
				return E_UseSkill_SexLimit;
		}
		else
		{

		}
	}

	// ְҵ����

	// ��������
	if( EITE_Null != pProto->nWeaponLimit && m_pOwner->IsRole() )
	{
		Role* pRole = static_cast<Role*>(m_pOwner);
		tagEquip* pWeapon = pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_RightHand);
		if( !P_VALID(pWeapon) || pWeapon->pProtoType->eTypeEx != pProto->nWeaponLimit )
		{
			return E_UseSkill_WeaponLimit;
		}
	}

	// ����Buff����
	if( P_VALID(pProto->dwBuffLimitID) )
	{
		if( !m_pOwner->IsHaveBuff(pProto->dwBuffLimitID) )
		{
			return E_UseSkill_SelfBuffLimit;
		}
	}

	// �������״̬����
	DWORD dwSelfStateFlag = m_pOwner->GetStateFlag();
	if( (dwSelfStateFlag & pProto->dwSelfStateLimit) != dwSelfStateFlag )
	{
		return E_UseSkill_SelfStateLimit;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------
// ����Ŀ������
//-------------------------------------------------------------------------------
INT CombatHandler::CheckTargetLimit(Skill* pSkill, DWORD dwTargetUnitID)
{
	if( !P_VALID(pSkill) )
		return E_UseSkill_SkillNotExist;

	const tagSkillProto* pProto = pSkill->GetProto();
	if( !P_VALID(pProto) ) return E_UseSkill_SkillNotExist;

	// ���TargetUnitID��GT_INVALID������Ҫ�����ж�һ��
	if( GT_INVALID == dwTargetUnitID )
	{
		if( ESOPT_Explode == pSkill->GetOPType() && 0.0f == pSkill->GetOPDist() )
		{
			return E_Success;
		}
		else
		{
			return E_UseSkill_SkillTargetInvalid;
		}
	}

	Unit* pTarget = m_pOwner->GetMap()->FindUnit(dwTargetUnitID);
	if( !P_VALID(pTarget) ) return E_UseSkill_SkillTargetInvalid;

	// Ŀ������߼�����
	INT nRet = CheckTargetLogicLimit(pSkill, pTarget);
	if( nRet != E_Success )	return nRet;

	// λ�����ƣ��������ƺͷ�Χ�ж�
	if( m_pOwner != pTarget )
	{
		// λ������
		if( ESPT_NUll != pProto->ePosType )
		{
			if( ESPT_Front == pProto->ePosType )
			{
				if( FALSE == m_pOwner->IsInFrontOfTarget(*pTarget) )
					return E_UseSkill_PosLimitFront;
			}
			else if( ESPT_Back == pProto->ePosType )
			{
				if( TRUE == m_pOwner->IsInFrontOfTarget(*pTarget) )
					return E_UseSkill_PosLimitBack;
			}
		}

		// Ŀ������ж�
		if( FALSE == m_pOwner->IsInCombatDistance(*pTarget, pSkill->GetOPDist()) )
			return E_UseSkill_DistLimit;

		// ���߼��
		if( m_pOwner->IsRayCollide(*pTarget) )
			return E_UseSkill_RayLimit;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------
// ����Ŀ���߼�����
//-------------------------------------------------------------------------------
INT CombatHandler::CheckTargetLogicLimit(Skill* pSkill, Unit* pTarget)
{
	if( !P_VALID(pSkill) || !P_VALID(pTarget) )
		return E_UseSkill_SkillNotExist;

	const tagSkillProto* pProto = pSkill->GetProto();
	if( !P_VALID(pProto) ) return E_UseSkill_SkillNotExist;

	// ���Ŀ���Ƿ��ܱ�ʹ�ü���
	if( pTarget->IsInStateCantBeSkill() )
	{
		return E_UseSkill_TargetLimit;
	}

	// ���ȼ����Ŀ������ͱ�־tbc:inves
	DWORD dwTargetFlag = m_pOwner->TargetTypeFlag(pTarget);
	if( !(dwTargetFlag & pProto->dwTargetLimit) )
		return E_UseSkill_TargetLimit;

	// �ټ��Ŀ���״̬����
	DWORD dwTargetStatFlag = pTarget->GetStateFlag();
	if( (dwTargetStatFlag & pProto->dwTargetStateLimit) != dwTargetStatFlag )
	{
		return E_UseSkill_TargetStateLimit;
	}

	// ���Ŀ��Buff����
	if( P_VALID(pProto->dwTargetBuffLimitID) )
	{
		if( !pTarget->IsHaveBuff(pProto->dwTargetBuffLimitID) )
		{
			return E_UseSkill_TargetBuffLimit;
		}
	}	

	// �ټ������ж�
	if( m_pOwner != pTarget )
	{
		DWORD dwFriendEnemyFlag = m_pOwner->FriendEnemy(pTarget);

		DWORD dwFriendEnemyLimit = 0;

		if( pProto->bFriendly )		dwFriendEnemyLimit |= ETFE_Friendly;
		if( pProto->bHostile )		dwFriendEnemyLimit |= ETFE_Hostile;
		if( pProto->bIndependent )	dwFriendEnemyLimit |= ETFE_Independent;

		if( !(dwFriendEnemyLimit & dwFriendEnemyFlag) )
		{
			return E_UseSkill_TargetLimit;
		}
	}

	// �жϳɹ�
	return E_Success;
}

//----------------------------------------------------------------------------------------
// ����ͼ�м�������
//----------------------------------------------------------------------------------------
INT CombatHandler::CheckMapLimit(Skill* pSkill)
{
	// �жϵ�ͼ����
	if(P_VALID(m_pOwner->GetMap()))
	{
		BOOL bUesAble = m_pOwner->GetMap()->CanUseSkill(pSkill->GetID());
		if( !bUesAble )	return E_UseSkill_MapLimit;
	}

	return E_Success;
}

//----------------------------------------------------------------------------------------
// ���Լ���ʹ�ó�ͻ������TRUEΪ��ͻ��FALSEΪ�ǳ�ͻ
//----------------------------------------------------------------------------------------
BOOL CombatHandler::CheckSkillConflict(Skill* pSkill)
{
	ASSERT( P_VALID(pSkill) );

	if( !IsValid() ) return FALSE;		// ��ǰû��ʹ���κμ��ܺ��κ���Ʒ

	if( IsPreparing() ) return TRUE;	// ��ǰ�������֣�����ʹ���κμ���

	if( IsUseSkill() )
	{
		// ��ǰ����ʹ�ü��ܣ���鿴�ü����Ƿ�����ͨ����
		Skill* pCurSkill = m_pOwner->GetSkill(m_dwSkillID);
		if( P_VALID(pSkill) && ESSTE_Default != pCurSkill->GetTypeEx() )
		{
			return TRUE;
		}
		else
		{
			EndUseSkill();
			return FALSE;
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------------
// ���Լ�������
//-------------------------------------------------------------------------------
INT CombatHandler::CheckCostLimit(Skill* pSkill)
{
	// �����������
	INT nHPCost = pSkill->GetCost(ESCT_HP);
	if( nHPCost > 0 && m_pOwner->GetAttValue(ERA_HP) < nHPCost )
		return E_UseSkill_CostLimit;


	// �����������
	INT nMPCost = pSkill->GetCost(ESCT_MP);
	if( nMPCost > 0 && m_pOwner->GetAttValue(ERA_MP) < nMPCost )
		return E_UseSkill_CostLimit;


	// ���ŭ������
	INT nRageCost = pSkill->GetCost(ESCT_Rage);
	if( nRageCost > 0 && m_pOwner->GetAttValue(ERA_Rage) < nRageCost )
		return E_UseSkill_CostLimit;


	// ���־�����
	INT nEnduranceCost = pSkill->GetCost(ESCT_Endurance);
	if( nEnduranceCost > 0 && m_pOwner->GetAttValue(ERA_Endurance) < nEnduranceCost )
		return E_UseSkill_CostLimit;


	// ����������
	INT nValicityCost = pSkill->GetCost(ESCT_Valicity);
	if( nValicityCost > 0 && m_pOwner->GetAttValue(ERA_Vitality) < nValicityCost )
		return E_UseSkill_CostLimit;

	return E_Success;
}

//-------------------------------------------------------------------------------
// ����ְҵ����
//-------------------------------------------------------------------------------
INT CombatHandler::CheckVocationLimit(Skill* pSkill)
{
	//ASSERT(P_VALID(pSkill));
	if (!P_VALID(pSkill)) return E_UseSkill_SkillNotExist;

	if (!m_pOwner->IsRole()) return E_Success;

	const tagSkillProto* pProto = pSkill->GetProto();
	if( !P_VALID(pProto) ) return E_UseSkill_SkillNotExist;

	//INT nClass = (INT)((Role*)m_pOwner)->GetClass();
	//INT nClassEx = (INT)((Role*)m_pOwner)->GetClassEx();
	INT nClass = (INT)(static_cast<Role*> (m_pOwner)->GetClass());
	INT nClassEx = (INT)(static_cast<Role*> (m_pOwner)->GetClassEx());

	INT nTmpClass =  1 << ( nClass - 1 );
	INT nTmpClassEx = 0;
	INT nTmp = 0;

	if ( (INT)nClassEx != (INT)EHV_Base )
	{
		nTmpClassEx = 1 << ( nClassEx + 8 );
	}

	nTmp = nTmpClass + nTmpClassEx;

	if ( !( nTmp & pProto->dwVocationLimit ) )
		return E_UseSkill_VocationLimit;

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���㹥��Ŀ�꣬���뵽list�� 
//-------------------------------------------------------------------------------
VOID CombatHandler::CalSkillTargetList()
{
	m_listTargetID.Clear();
	m_listHitedTarget.Clear();
	m_listDodgedTarget.Clear();
	m_listBlockedTarget.Clear();
	m_listCritedTarget.Clear();
	m_dwTargetEffectFlag = 0;

	// ���ݸü��ܵĹ�������͹�����Χ���ж�
	Skill* pSkill = m_pOwner->GetSkill(m_dwSkillID);
	if( !P_VALID(pSkill) ) return;

	// �õ�Ŀ�����
	Unit* pTarget = NULL;
	if( GT_INVALID == m_dwTargetUnitID )	// ���û��ѡĿ�꣬��Ŀ������Լ�
	{
		pTarget = m_pOwner;
	}
	else									// ���ѡ��Ŀ�꣬���ҵ�Ŀ��
	{
		pTarget = m_pOwner->GetMap()->FindUnit(m_dwTargetUnitID);
	}
	if( !P_VALID(pTarget) ) return;

	// �����������ͣ����þ�������ð뾶��ʹ�ü���
	ESkillOPType eOPType = pSkill->GetOPType();
	FLOAT fOPDist = pSkill->GetOPDist();
	FLOAT fOPRadius = pSkill->GetOPRadius();

	// �Ƚ�Ŀ��ӽ�ȥ
	if( m_pOwner != pTarget )
	{
		m_listTargetID.PushBack(pTarget->GetID());
		m_dwTargetEffectFlag = CalculateSkillEffect(pSkill, pTarget);
	}

	// ��ըЧ��
	if( ESOPT_Explode == eOPType )
	{
		// ���������ΧΪ0����ֱ�ӷ���
		if( 0.0f == fOPRadius )
			return;

		// ���������Χ��Ϊ0������Ŀ��Ϊ���ļ��
		FLOAT fOPRadiusSQ = fOPRadius * fOPRadius;

		tagVisTile* pVisTile[ED_End] = {0};

		// �õ�������Χ�ڵ�vistile�б�
		pTarget->GetMap()->GetVisTile(pTarget->GetCurPos(), fOPRadius, pVisTile);
		Role*		pRole		= NULL;
		Creature*	pCreature	= NULL;

		for(INT n = ED_Center; n < ED_End; n++)
		{
			if( !P_VALID(pVisTile[n]) ) continue;

			// ���ȼ������
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			TMap<DWORD, Role*>::TMapIterator it = mapRole.Begin();

			while( mapRole.PeekNext(it, pRole) )
			{
				// ��Ŀ��һ������������
				if( pRole == pTarget || pRole == m_pOwner ) continue;

				// Ŀ����������ж�
				if( E_Success != CheckTargetLogicLimit(pSkill, pRole) )
					continue;

				// ���ܾ����ж�
				FLOAT fDistSQ = Vec3DistSq(pTarget->GetCurPos(), pRole->GetCurPos());
				if( fDistSQ > fOPRadiusSQ  ) continue;

				// ���߼��

				// �ж�ͨ��������Ҽ��뵽�б���
				m_listTargetID.PushBack(pRole->GetID());

				// ���㼼�����ý��
				CalculateSkillEffect(pSkill, pRole);
			}

			// �ټ������
			TMap<DWORD, Creature*>& mapCreature = pVisTile[n]->mapCreature;
			TMap<DWORD, Creature*>::TMapIterator it2 = mapCreature.Begin();

			while( mapCreature.PeekNext(it2, pCreature) )
			{
				// ��Ŀ��һ������������
				if( pCreature == pTarget || pCreature == m_pOwner ) continue;

				// Ŀ����������ж�
				if( E_Success != CheckTargetLogicLimit(pSkill, pCreature) )
					continue;

				// ���ܾ����ж�
				FLOAT fDistSQ = Vec3DistSq(pTarget->GetCurPos(), pCreature->GetCurPos());
				if( fDistSQ > fOPRadiusSQ  ) continue;

				// ���߼��

				// �ж�ͨ������������뵽�б���
				m_listTargetID.PushBack(pCreature->GetID());

				// ���㼼�����ý��
				CalculateSkillEffect(pSkill, pCreature);
			}
		}
	}

	// ����
	else if( ESOPT_Rect == eOPType )
	{
		// ���������Χ���߹�������Ϊ0����ֱ�ӷ���
		if( 0.0f == fOPRadius || 0.0f == fOPDist )	return;

		// ���������Χ��Ϊ0������Ŀ��Ϊ���ļ��
		FLOAT fOPRadiusSQ = fOPRadius * fOPRadius;
		FLOAT fOPDistSQ = fOPDist * fOPDist;

		// ���������Χ�͹����������Ϊ0����������Ϊ��׼���
		FLOAT fTargetX = pTarget->GetCurPos().x;
		FLOAT fTargetY = pTarget->GetCurPos().y;
		FLOAT fTargetZ = pTarget->GetCurPos().z;
		FLOAT fSrcX = m_pOwner->GetCurPos().x;
		FLOAT fSrcY = m_pOwner->GetCurPos().y;
		FLOAT fSrcZ = m_pOwner->GetCurPos().z;

		// ������һĿ��������
		FLOAT fX2 = fTargetX - fSrcX;
		FLOAT fY2 = fTargetY - fSrcY;
		FLOAT fZ2 = fTargetZ - fSrcZ;

		// ���Ŀ�����������ôֱ��ȡ����ĳ�������
		if( m_pOwner == pTarget )
		{
			fX2 = m_pOwner->GetFaceTo().x;
			fZ2 = m_pOwner->GetFaceTo().z;
		}

		if( abs(fX2) < 0.001f && abs(fZ2) < 0.001f )
			return;

		// ������һĿ���ľ����ƽ��
		FLOAT fDistSQ2 = fX2*fX2 + fY2*fY2 + fZ2*fZ2;

		tagVisTile* pVisTile[ED_End] = {0};

		// �õ�vistile�б�
		pTarget->GetMap()->GetVisTile(m_pOwner->GetVisTileIndex(), pVisTile);
		Role*		pRole		= NULL;
		Creature*	pCreature	= NULL;

		for(INT n = ED_Center; n < ED_End; n++)
		{
			if( !P_VALID(pVisTile[n]) ) continue;

			// ���ȼ������
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			TMap<DWORD, Role*>::TMapIterator it = mapRole.Begin();

			while( mapRole.PeekNext(it, pRole) )
			{
				// ��Ŀ��һ������������
				if( pRole == pTarget || pRole == m_pOwner ) continue;

				// Ŀ����������ж�
				if( E_Success != CheckTargetLogicLimit(pSkill, pRole) )
					continue;

				// ������ǰ�������
				FLOAT fX1 = pRole->GetCurPos().x - fSrcX;
				FLOAT fY1 = pRole->GetCurPos().y - fSrcY;
				FLOAT fZ1 = pRole->GetCurPos().z - fSrcZ;

				// �ȼ�鷽λ cos(a) > 0 
				if( fX1*fX2	+ fZ1*fZ2 < 0.0f )
					continue;

				FLOAT fDist1 = fX1*fX2 + fY1*fY2 + fZ1*fZ2;
				FLOAT fDistSQ1 = fDist1 * fDist1;

				// ���ͶӰ����
				FLOAT fProjDistSQ = fDistSQ1 / fDistSQ2;
				if( fProjDistSQ > fOPDistSQ )
					continue;

				// ���㵽ֱ�߾���
				if( fX1*fX1 + fY1*fY1 + fZ1*fZ1 - fProjDistSQ > fOPRadiusSQ)
					continue;

				// ���߼��

				// �ж�ͨ��������Ҽ��뵽�б���
				m_listTargetID.PushBack(pRole->GetID());

				// ���㼼�����ý��
				CalculateSkillEffect(pSkill, pRole);
			}

			// �ټ������
			TMap<DWORD, Creature*>& mapCreature = pVisTile[n]->mapCreature;
			TMap<DWORD, Creature*>::TMapIterator it2 = mapCreature.Begin();

			while( mapCreature.PeekNext(it2, pCreature) )
			{
				// ��Ŀ��һ������������
				if( pCreature == pTarget || pCreature == m_pOwner ) continue;

				// Ŀ����������ж�
				if( E_Success != CheckTargetLogicLimit(pSkill, pCreature) )
					continue;

				// ������ǰ�������
				FLOAT fX1 = pCreature->GetCurPos().x - fSrcX;
				FLOAT fY1 = pCreature->GetCurPos().y - fSrcY;
				FLOAT fZ1 = pCreature->GetCurPos().z - fSrcZ;

				// �ȼ�鷽λ cos(a) > 0 
				if( fX1*fX2	+ fZ1*fZ2 < 0.0f )
					continue;

				// ���ͶӰ����
				FLOAT fProjDistSQ = (fX1*fX2 + fY1*fY2 + fZ1*fZ2) / fDistSQ2;
				if( fProjDistSQ > fOPDistSQ )
					continue;

				// ���㵽ֱ�߾���
				if( fX1*fX1 + fY1*fY1 + fZ1*fZ1 - fProjDistSQ > fOPRadiusSQ)
					continue;

				// ���߼��

				// �ж�ͨ��������Ҽ��뵽�б���
				m_listTargetID.PushBack(pCreature->GetID());

				// ���㼼�����ý��
				CalculateSkillEffect(pSkill, pCreature);
			}
		}
	}
}

//-------------------------------------------------------------------------------
// ���㼼��Ч��
//-------------------------------------------------------------------------------
DWORD CombatHandler::CalculateSkillEffect(Skill* pSkill, Unit* pTarget)
{
	DWORD dwTargetEffectFlag = 0;

	DWORD dwTargetID = pTarget->GetID();

	INT nDmgTimes = pSkill->GetDmgTimes();

	// ���˺����ܣ�������
	if( nDmgTimes <= 0 )
	{
		m_listHitedTarget.PushBack(dwTargetID);
		dwTargetEffectFlag |= ETEF_Hited;

		tagNS_HitTarget send;
		send.dwRoleID = pTarget->GetID();
		send.dwSrcRoleID = m_pOwner->GetID();
		send.eCause = EHTC_Skill;
		send.dwMisc = pSkill->GetTypeID();
		send.dwSerial = m_dwSkillSerial;

		if( P_VALID(pTarget->GetMap()) )
		{
			pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
		}

		pTarget->OnBeAttacked(m_pOwner, pSkill, TRUE, FALSE, FALSE);
		return dwTargetEffectFlag;
	}

	// ��������
	BOOL bHit = CalculateHit(pSkill, pTarget);
	if( FALSE == bHit )
	{
		// δ����
		m_listDodgedTarget.PushBack(dwTargetID);
	}
	else
	{
		// ����
		m_listHitedTarget.PushBack(dwTargetID);
		dwTargetEffectFlag |= ETEF_Hited;

		tagNS_HitTarget send;
		send.dwRoleID = pTarget->GetID();
		send.dwSrcRoleID = m_pOwner->GetID();
		send.eCause = EHTC_Skill;
		send.dwMisc = pSkill->GetTypeID();
		send.dwSerial = m_dwSkillSerial;

		if( P_VALID(pTarget->GetMap()) )
		{
			pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
		}

		// �����
		BOOL bBlocked = CalculateBlock(pSkill, pTarget);
		if( TRUE == bBlocked )
		{
			// ����
			m_listBlockedTarget.PushBack(dwTargetID);
			dwTargetEffectFlag |= ETEF_Block;
		}
		else
		{
			// ���㱩��
			BOOL bCrit = CalculateCritRate(pSkill, pTarget);
			if( TRUE == bCrit )
			{
				m_listCritedTarget.PushBack(dwTargetID);
				dwTargetEffectFlag |= ETEF_Crited;
			}
		}
	}

	// ���������ı���������
	pTarget->OnBeAttacked(m_pOwner, pSkill,
		dwTargetEffectFlag & ETEF_Hited, dwTargetEffectFlag & ETEF_Block, dwTargetEffectFlag & ETEF_Crited);

	return dwTargetEffectFlag;
}


//--------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------
BOOL CombatHandler::CalculateHit(Skill* pSkill, Unit* pTarget)
{
	FLOAT fHit = 0.0f;

	// �⹦����
	if( pSkill->IsExAttackSkill() )
	{
		// ������=0.9��[1-����������ǰ����-��������ǰ���У�/10000]����1-����������ʧ���ʣ��£�1+����������Զ�̶���ʣ�+����������
		fHit = 0.9f * (1.0f - (FLOAT)(pTarget->GetAttValue(ERA_Dodge) - m_pOwner->GetAttValue(ERA_HitRate)) / 10000.0f);
		if( fHit < 0.0f ) fHit = 0.0f;
		if( fHit > 1.0f ) fHit = 1.0f;

		fHit = fHit	* (1.0f - FLOAT(m_pOwner->GetAttValue(ERA_Attack_MissRate)) / 10000.0f);

		// �����ǽ�����Զ�̾���ѡ���ĸ������
		if( pSkill->IsMelee() )
		{
			fHit = fHit / (1.0f + FLOAT(pTarget->GetAttValue(ERA_CloseAttack_DodgeRate)) / 10000.0f);
		}
		else if( pSkill->IsRanged() )
		{
			fHit = fHit / (1.0f + FLOAT(pTarget->GetAttValue(ERA_RemoteAttack_DodgeRate)) / 10000.0f);
		}
	}

	// �ڹ�����
	else if( pSkill->IsInAttackSkill() )
	{
		// ������=1��[1-����������ǰ����-��������ǰ���У�/8500]����1-����������ʧ���ʣ�+����������
		fHit = 1.0f - (FLOAT)(pTarget->GetAttValue(ERA_Dodge) - m_pOwner->GetAttValue(ERA_HitRate)) / 8500.0f;
		if( fHit < 0.0f ) fHit = 0.0f;
		if( fHit > 1.0f ) fHit = 1.0f;

		fHit = fHit * (1.0f - FLOAT(m_pOwner->GetAttValue(ERA_Attack_MissRate)) / 10000.0f);
	}

	// ��������
	else if( pSkill->IsStuntSkill() )
	{
		// ������=100%����1-����������ʧ���ʣ�+����������
		fHit = 1.0f - FLOAT(m_pOwner->GetAttValue(ERA_Attack_MissRate)) / 10000.0f;
	}

	// else
	else
	{

	}

	// ���Ӽ���������
	fHit += pSkill->GetHit();

	// ��Χ��30����100%
	if( fHit < 0.3f ) fHit = 0.3f;
	if( fHit > 1.0f ) fHit = 1.0f;

	// ������Ƿ�������
	return IUTIL->Probability(INT(fHit*100.0f));
}

//----------------------------------------------------------------------------
// �����
//----------------------------------------------------------------------------
BOOL CombatHandler::CalculateBlock(Skill* pSkill, Unit* pTarget)
{
	// ֻ�й����Ŵ��ڷ�����ǰ��ʱ���������ſ��Ը�
	if( FALSE == m_pOwner->IsInFrontOfTarget(*pTarget) )
		return FALSE;

	// ����
	FLOAT fBlock = 0.0f;

	// �⹦����
	if( pSkill->IsExAttackSkill() )
	{
		// Զ�̹���
		if( pSkill->IsMelee() )
		{
			// ��������=����������ǰ�⹦����-���������⹦����+�������ڹ�������/4��/30000
			// ����=[1+����������ǰ��������-��������ǰ�������ɣ�/6000] ����������+�������񵵼��ʼӳ�
			FLOAT fBaseBlock = (FLOAT(pTarget->GetAttValue(ERA_ExDefense)) - FLOAT(m_pOwner->GetAttValue(ERA_ExAttack) + m_pOwner->GetAttValue(ERA_InAttack)) / 4.0f) / 30000.0f;
			fBlock = (1.0f + FLOAT(pTarget->GetAttValue(ERA_DefenseTec) - m_pOwner->GetAttValue(ERA_AttackTec)) / 6000.0f) * fBaseBlock + (FLOAT)pTarget->GetAttValue(ERA_Block_Rate) / 10000.0f;
		}

		// Զ�̹���
		else if( pSkill->IsRanged() )
		{
			// ����=0
			fBlock = 0.0f;
		}		
	}

	// �ڹ�����
	else if( pSkill->IsInAttackSkill() )
	{
		// ��������=����������ǰ�ڹ�����-���������⹦����+�������ڹ�������/4��/30000
		// ����=[1+����������ǰ��������-��������ǰ�������ɣ�/6600] ����������+�������񵵼��ʼӳ�
		FLOAT fBaseBlock = (FLOAT(pTarget->GetAttValue(ERA_InDefense)) - FLOAT(m_pOwner->GetAttValue(ERA_ExAttack) + m_pOwner->GetAttValue(ERA_InAttack)) / 4.0f) / 30000.0f;
		fBlock = (1.0f + FLOAT(pTarget->GetAttValue(ERA_DefenseTec) - m_pOwner->GetAttValue(ERA_AttackTec)) / 6600.0f) * fBaseBlock + (FLOAT)pTarget->GetAttValue(ERA_Block_Rate) / 10000.0f;
	}

	// ��������
	else if( pSkill->IsStuntSkill() )
	{
		fBlock = 0.0f;
	}

	// else
	else
	{

	}

	// ��Χ��0����100%
	if( fBlock < 0.0f ) fBlock = 0.0f;
	if( fBlock > 1.0f ) fBlock = 1.0f;

	// ������Ƿ�������
	return IUTIL->Probability(INT(fBlock*100.0f));
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
BOOL CombatHandler::CalculateCritRate(Skill* pSkill, Unit* pTarget)
{
	// ����������=����������ǰ��������/150000��^0.5
	// ������=���������ʡ�(��������ǰ��������/����������ǰ��������)^0.5+�����ӳ�/1000+���ܸ���������
	FLOAT fCrit = m_pOwner->GetAttValue(ERA_AttackTec) / 150000.0f;
	fCrit = IFASTCODE->FastSqrt(fCrit * ((FLOAT)m_pOwner->GetAttValue(ERA_AttackTec) / (FLOAT)pTarget->GetAttValue(ERA_DefenseTec)));
	fCrit = fCrit + (FLOAT)m_pOwner->GetAttValue(ERA_Crit_Rate) / 1000.0f + pSkill->GetCrit();

	// ��Χ��0����100%
	if( fCrit < 0.0f ) fCrit = 0.0f;
	if( fCrit > 1.0f ) fCrit = 1.0f;

	// ������Ƿ�������
	return IUTIL->Probability(INT(fCrit*100.0f));
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
FLOAT CombatHandler::CalculateCritAmount(Skill* pSkill, Unit* pTarget)
{
	// �⹦����
	if( pSkill->IsExAttackSkill() )
	{
		// �����˺����ӳ�=����������ǰ�⹦����/2000��^0.25+�������������ӳɣ�װ�������ܡ���Ʒ��Ӱ�죩
		return pow(FLOAT(m_pOwner->GetAttValue(ERA_ExAttack) / 2000.0f), 0.25f) + (FLOAT)m_pOwner->GetAttValue(ERA_Crit_Amount) / 10000.0f;
	}

	// �ڹ�����
	else if( pSkill->IsInAttackSkill() )
	{
		// �����˺����ӳ�=����������ǰ�ڹ�����/8000��^0.25+�������������ӳɣ�װ�������ܡ���Ʒ��Ӱ�죩
		return pow(FLOAT(m_pOwner->GetAttValue(ERA_InAttack) / 8000.0f), 0.25f) + (FLOAT)m_pOwner->GetAttValue(ERA_Crit_Amount) / 10000.0f;
	}

	// ��������
	else if( pSkill->IsStuntSkill() )
	{
		// �����˺����ӳ�=������������ǰ�ڹ�����+��������ǰ�⹦������/1000��^0.25+�������������ӳɣ�װ�������ܡ���Ʒ��Ӱ�죩
		return pow(FLOAT((m_pOwner->GetAttValue(ERA_InAttack) + m_pOwner->GetAttValue(ERA_ExAttack)) / 1000.0f), 0.25f) + (FLOAT)m_pOwner->GetAttValue(ERA_Crit_Amount) / 10000.0f;
	}

	// else
	else
	{

	}

	return 0.0f;
}

//-----------------------------------------------------------------------------
// ���㼼���˺�
//-----------------------------------------------------------------------------
VOID CombatHandler::CalculateDmg(Skill* pSkill, Unit* pTarget)
{
	// Ŀ���Ѿ�������ֱ�ӷ���
	if( pTarget->IsDead() ) return;

	DWORD dwTargetID = pTarget->GetID();

	// ��������
	bool bCrit = false;
	FLOAT fCrit = 0.0f;

	// �����жϸ�Ŀ���Ƿ�������
	if( m_listDodgedTarget.IsExist(dwTargetID) )
	{
		// ����δ������Ϣ
		tagNS_RoleHPChange send;
		send.dwRoleID = pTarget->GetID();
		send.dwSrcRoleID = m_pOwner->GetID();
		send.eCause = ERHPCC_SkillDamage;
		send.bMiss = true;
		send.dwMisc = pSkill->GetTypeID();
		send.dwMisc2 = m_nSkillCurDmgIndex;
		send.dwSerial = m_dwSkillSerial;

		if( P_VALID(pTarget->GetMap()) )
		{
			pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
		}
		return;
	}

	// ���ж��Ƿ񱻸���
	else if( m_listBlockedTarget.IsExist(dwTargetID) )
	{
		// ���͸���Ϣ
		tagNS_RoleHPChange send;
		send.dwRoleID = pTarget->GetID();
		send.dwSrcRoleID = m_pOwner->GetID();
		send.eCause = ERHPCC_SkillDamage;
		send.bBlocked = true;
		send.dwMisc = pSkill->GetTypeID();
		send.dwMisc2 = m_nSkillCurDmgIndex;
		send.dwSerial = m_dwSkillSerial;

		if( P_VALID(pTarget->GetMap()) )
		{
			pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
		}
		return;
	}

	// ���ж��Ƿ񱩻�
	else if( m_listCritedTarget.IsExist(dwTargetID) )
	{
		// ���㱩������
		bCrit = true;
		fCrit = CalculateCritAmount(pSkill, pTarget);
	}

	FLOAT fBaseDmg		=	CalBaseDmg(pSkill);						// �����˺�
	FLOAT fAttDefCoef	=	CalAttackDefenceCoef(pSkill, pTarget);	// ����Ӱ��
	//FLOAT fMoraleCoef	=	CalMoraleCoef(pTarget);					// ʿ��Ӱ��
	FLOAT fDerateCoef	=	CalDerateCoef(pSkill, pTarget);			// ����Ӱ��
	FLOAT fInjuryCoef	=	CalInjuryCoef();						// ����Ӱ��
	FLOAT fLevelCoef	=	CalLevelCoef(pSkill, pTarget);			// �ȼ�Ӱ��

	// �����˺�
	FLOAT fDmg = fBaseDmg * fAttDefCoef /** fMoraleCoef*/ * fDerateCoef * fInjuryCoef * fLevelCoef;
	fDmg = fDmg + (FLOAT)m_pOwner->GetAttValue(ERA_ExDamage) - (FLOAT)pTarget->GetAttValue(ERA_ExDamage_Absorb);

	// ���㱩������
	INT nDmg = INT(fDmg * (1.0f + fCrit));
	if( nDmg < 1 ) nDmg = 1;
	
	// ���� = �ɻ��� + ����+20%
	// ���ɵľ�����ȷ��Ϊ15����
	// ��Ҳ��ɻ���,������ɻ���
	if ( pTarget->IsCreature() && bCrit && fDmg >= pTarget->GetAttValue(ERA_HP)*0.2)
	{
		// �Ƿ�ɱ�����
		Creature *pCreature = static_cast<Creature*>(pTarget);
		if ( 1 == pCreature->GetProto()->bCanHitFly )
			m_pOwner->HitFly(pTarget);
	}

	// ��Ѫ
	pTarget->ChangeHP(-nDmg, m_pOwner, pSkill, NULL, bCrit, m_dwSkillSerial, m_nSkillCurDmgIndex);
}

//------------------------------------------------------------------------------
// ��������˺�
//------------------------------------------------------------------------------
inline FLOAT CombatHandler::CalBaseDmg(Skill* pSkill)
{
	FLOAT	fBaseDmg	=	1.0f;

	FLOAT	fWeaponDmg	=	FLOAT(IUTIL->RandomInRange(m_pOwner->GetAttValue(ERA_WeaponDmgMin), m_pOwner->GetAttValue(ERA_WeaponDmgMax))) / (FLOAT)pSkill->GetDmgTimes();
	FLOAT	fWeaponSoul	=	(FLOAT)m_pOwner->GetAttValue(ERA_WeaponSoul) / (FLOAT)pSkill->GetDmgTimes();
	INT		nSkillDmg	=	pSkill->GetDmg(m_nSkillCurDmgIndex);

	// �⹦���������˺�
	// �������˺�/�����˺�����+��������������1+��������ǰ�⹦����/10000��+��������ǰ�⹦������0.02/���ܹ�������
	if( pSkill->IsExAttackSkill() )
	{
		if( nSkillDmg > 100000 )
		{
			// ȡ���������˺��ı���
			fBaseDmg = fWeaponDmg * (FLOAT(nSkillDmg - 100000) / 10000.0f);
		}
		else
		{
			// ȡ���Ǽ����˺�
			fBaseDmg = fWeaponDmg + (FLOAT)nSkillDmg;
		}

		fBaseDmg = fBaseDmg * (1.0f + (FLOAT)m_pOwner->GetAttValue(ERA_ExAttack) / 10000.0f);
		fBaseDmg = fBaseDmg + (FLOAT)m_pOwner->GetAttValue(ERA_ExAttack) * 0.02f / (FLOAT)pSkill->GetDmgTimes();
	}

	// �ڹ����������˺�
	// �����/���ܹ�������+��������������1+��������ǰ�ڹ�����/10000��+��������ǰ�ڹ�������0.02/���ܹ�������
	else if( pSkill->IsInAttackSkill() )
	{
		if( nSkillDmg > 100000 )
		{
			// ȡ���������˺��ı���
			fBaseDmg = fWeaponSoul * ((FLOAT)(nSkillDmg - 100000) / 10000.0f);
		}

		else
		{
			// ȡ���Ǽ����˺�
			fBaseDmg = fWeaponSoul + (FLOAT)nSkillDmg;
		}

		fBaseDmg = fBaseDmg * (1.0f + (FLOAT)m_pOwner->GetAttValue(ERA_InAttack) / 10000.0f);
		fBaseDmg = fBaseDmg + (FLOAT)m_pOwner->GetAttValue(ERA_InAttack) * 0.02f / (FLOAT)pSkill->GetDmgTimes();

	}

	// �������������˺�
	// ��������������+�����˺�/���ܹ�������+���/���ܹ�������������1+����ǰ�⹦������+��ǰ�ڹ�������/10000��+����������ǰ�ڹ�����+��������ǰ�⹦��������0.2
	else if( pSkill->IsStuntSkill() )
	{
		if( nSkillDmg > 100000 )
		{
			// ȡ���������˺��ı���
			fBaseDmg = (fWeaponDmg + fWeaponSoul) * (FLOAT(nSkillDmg - 100000) / 10000.0f);
		}
		else
		{
			// ȡ���Ǽ����˺�
			fBaseDmg = fWeaponDmg + fWeaponSoul + (FLOAT)nSkillDmg;
		}

		fBaseDmg = fBaseDmg * (1.0f + FLOAT(m_pOwner->GetAttValue(ERA_InAttack) + m_pOwner->GetAttValue(ERA_ExAttack)) / 10000.0f);
		fBaseDmg = fBaseDmg + FLOAT(m_pOwner->GetAttValue(ERA_InAttack) + m_pOwner->GetAttValue(ERA_ExAttack)) * 0.2f;
	}

	return fBaseDmg;
}

//-----------------------------------------------------------------------------
// ���㹥��Ӱ��
//-----------------------------------------------------------------------------
inline FLOAT CombatHandler::CalAttackDefenceCoef(Skill* pSkill, Unit* pTarget)
{
	// �⹦��������Ӱ��
	// [1+���������⹦����-�������⹦������/��1000+�����ȼ�*25��]��[1+����������������-�������������ɣ�/��1000+�����ȼ�*25��]
	// �˷����������ӷֱ���0.5����2.0֮��
	if( pSkill->IsExAttackSkill() )
	{
		FLOAT fCoef1 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_ExAttack) - pTarget->GetAttValue(ERA_ExDefense)) / (1000.0f + FLOAT(m_pOwner->GetLevel())*25.0f);
		FLOAT fCoef2 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_AttackTec) - pTarget->GetAttValue(ERA_DefenseTec)) / (1000.0f + FLOAT(m_pOwner->GetLevel())*25.0f);
		if( fCoef1 < 0.5f ) fCoef1 = 0.5f;
		if( fCoef1 > 2.0f ) fCoef1 = 2.0f;
		if( fCoef2 < 0.5f ) fCoef2 = 0.5f;
		if( fCoef2 > 2.0f ) fCoef2 = 2.0f;

		return fCoef1 * fCoef2;
	}

	// �ڹ���������Ӱ��
	// [1+���������ڹ�����-�������ڹ�������/��880+�����ȼ�*22��]��[1+����������������-�������������ɣ�/��1400+�����ȼ�*35��]
	// �˷����������ӷֱ���0.5����2.0֮��
	else if( pSkill->IsInAttackSkill() )
	{
		FLOAT fCoef1 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_InAttack) - pTarget->GetAttValue(ERA_InDefense)) / (880.0f + FLOAT(m_pOwner->GetLevel())*22.0f);
		FLOAT fCoef2 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_AttackTec) - pTarget->GetAttValue(ERA_DefenseTec)) / (1400.0f + FLOAT(m_pOwner->GetLevel())*35.0f);
		if( fCoef1 < 0.5f ) fCoef1 = 0.5f;
		if( fCoef1 > 2.0f ) fCoef1 = 2.0f;
		if( fCoef2 < 0.5f ) fCoef2 = 0.5f;
		if( fCoef2 > 2.0f ) fCoef2 = 2.0f;

		return fCoef1 * fCoef2;
	}

	// ������������Ӱ��
	// �⹦����Ӱ��+�ڹ�����Ӱ��
	else if( pSkill->IsStuntSkill() )
	{
		FLOAT fCoef1 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_ExAttack) - pTarget->GetAttValue(ERA_ExDefense)) / 1000.0f;
		FLOAT fCoef2 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_AttackTec) - pTarget->GetAttValue(ERA_DefenseTec)) / 1000.0f;
		FLOAT fCoef3 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_InAttack) - pTarget->GetAttValue(ERA_InDefense)) / 800.0f;
		FLOAT fCoef4 = 1.0f + FLOAT(m_pOwner->GetAttValue(ERA_AttackTec) - pTarget->GetAttValue(ERA_DefenseTec)) / 2000.0f;
		if( fCoef1 < 0.5f ) fCoef1 = 0.5f;
		if( fCoef1 > 2.0f ) fCoef1 = 2.0f;
		if( fCoef2 < 0.5f ) fCoef2 = 0.5f;
		if( fCoef2 > 2.0f ) fCoef2 = 2.0f;
		if( fCoef3 < 0.5f ) fCoef3 = 0.5f;
		if( fCoef3 > 2.0f ) fCoef3 = 2.0f;
		if( fCoef4 < 0.5f ) fCoef4 = 0.5f;
		if( fCoef4 > 2.0f ) fCoef4 = 2.0f;

		return fCoef1 * fCoef2 * fCoef3 * fCoef4;
	}
	else
	{
		return 0.0f;
	}
}

//-----------------------------------------------------------------------
// ����ʿ��Ӱ��
//-----------------------------------------------------------------------
inline FLOAT CombatHandler::CalMoraleCoef(Unit* pTarget)
{
	// ʿ��Ӱ��=[1+��������ʿ��-������ʿ����/100]
	return FLOAT(m_pOwner->GetAttValue(ERA_Morale) - pTarget->GetAttValue(ERA_Morale)) / 100.0f;
}

//------------------------------------------------------------------------
// �������Ӱ��
//------------------------------------------------------------------------
inline FLOAT CombatHandler::CalDerateCoef(Skill* pSkill, Unit* pTarget)
{
	FLOAT fDerateCoef = 1.0f;

	ERoleAttribute eAtt = pTarget->SkillDmgType2ERA(pSkill->GetDmgType());
	if( ERA_Null == eAtt ) return fDerateCoef;

	// �⹦��������
	// 1 - �������˺����� - ���������׼���
	if( pSkill->IsExAttackSkill() )
	{
		// �ȼ��㻤��
		FLOAT fArmor = 1.0f;
		if( ERA_Derate_Ordinary == eAtt )
		{
			// ��ͨ�˺����׼��� = ����ֵ/��31���������ȼ�+267��
			fArmor = GetTargetArmor(*pTarget) / (31.0f * (FLOAT)pTarget->GetLevel() + 267.0f);
		}
		else
		{
			// �⹦�˺����׼��� = ����ֵ/��46.5���������ȼ�+400��
			fArmor = GetTargetArmor(*pTarget) / (46.5f * (FLOAT)pTarget->GetLevel() + 400.0f);
		}

		// �⹦�˺�����=��##�˺����� + �⹦�˺����� + ȫ���˺�����C ����ȼ���/ 1000
		fDerateCoef = fArmor + FLOAT(pTarget->GetAttValue(eAtt) + pTarget->GetAttValue(ERA_Derate_ExAttack) + pTarget->GetAttValue(ERA_Derate_ALL) - pTarget->GetLevel()) / 1000.0f;
	}

	// �ڹ���������
	// 1 - �������˺����� - ���������׼���
	else if( pSkill->IsInAttackSkill() )
	{
		// �ڹ��˺����׼��� = ����ֵ/��93���������ȼ�+800��
		FLOAT fArmor = GetTargetArmor(*pTarget) / (93.0f * (FLOAT)pTarget->GetLevel() + 800.0f);
		// �ڹ��˺�����=��##�˺����� + �ڹ��˺����� + ȫ���˺����� �C ����ȼ���/ 1000
		fDerateCoef = fArmor + FLOAT(pTarget->GetAttValue(eAtt) + pTarget->GetAttValue(ERA_Derate_InAttack) + pTarget->GetAttValue(ERA_Derate_ALL) - pTarget->GetLevel()) / 1000.0f;
	}

	// ������������
	// ����Ӱ��=1-�������˺�����
	else if( pSkill->IsStuntSkill() )
	{

		// �����˺����� = �������˺����� + ȫ���˺����� �C ����ȼ���/ 1000
		fDerateCoef = (FLOAT)(pTarget->GetAttValue(eAtt) + pTarget->GetAttValue(ERA_Derate_ALL) - pTarget->GetLevel()) / 1000.0f;
	}

	// ��������ֵ
	fDerateCoef = 1.0f - fDerateCoef;
	if( fDerateCoef < 0.2f ) fDerateCoef = 0.2f;
	if( fDerateCoef > 2.0f ) fDerateCoef = 2.0f;

	return fDerateCoef;
}

//------------------------------------------------------------------------
// ��������Ӱ��
//------------------------------------------------------------------------
inline FLOAT CombatHandler::CalInjuryCoef()
{
	// ����Ӱ��=1�����ˡ�0.1�£�4�����ˡ�0.06��
	return (1.0f -(FLOAT)m_pOwner->GetAttValue(ERA_Injury) * 0.1f / (4.0f + (FLOAT)m_pOwner->GetAttValue(ERA_Injury) * 0.06));
}

//-------------------------------------------------------------------------
// ����ȼ�Ӱ��
//-------------------------------------------------------------------------
inline FLOAT CombatHandler::CalLevelCoef(Skill* pSkill, Unit* pTarget)
{
	// �⹦�������ڹ�����
	// 1-���������ȼ�-�������ȼ���/75     ȡֵ��0.2~1.8��
	if( pSkill->IsExAttackSkill() || pSkill->IsInAttackSkill() )
	{
		FLOAT fCoef = 1.0f - FLOAT(pTarget->GetLevel() - m_pOwner->GetLevel()) / 75.0f;
		if( fCoef < 0.2f ) fCoef = 0.2f;
		if( fCoef > 1.8f ) fCoef = 1.8f;

		return fCoef;
	}
	else
	{
		return 1.0f;
	}
}

//----------------------------------------------------------------------------
// ���㼼������
//----------------------------------------------------------------------------
VOID CombatHandler::CalculateCost(Skill* pSkill)
{
	// ��������
	INT nHPCost = pSkill->GetCost(ESCT_HP);
	if( nHPCost > 0  )
	{
		m_pOwner->ChangeHP(-nHPCost, m_pOwner);
	}

	// ��������
	INT nMPCost = pSkill->GetCost(ESCT_MP);
	if( nMPCost > 0  )
	{
		m_pOwner->ChangeMP(-nMPCost);
	}

	// ŭ������
	INT nRageCost = pSkill->GetCost(ESCT_Rage);
	if( nRageCost > 0 )
	{
		m_pOwner->ChangeRage(-nRageCost);
	}


	// �־�����
	INT nEnduranceCost = pSkill->GetCost(ESCT_Endurance);
	if( nEnduranceCost > 0 )
	{
		m_pOwner->ChangeEndurance(-nEnduranceCost);
	}

	// ��������
	INT nValicityCost = pSkill->GetCost(ESCT_Valicity);
	if( nValicityCost > 0  )
	{
		m_pOwner->ChangeVitality(-nValicityCost);
	}
}

//----------------------------------------------------------------------------
// ��Ʒʹ���ж�
//----------------------------------------------------------------------------
INT	CombatHandler::CanUseItem(tagItem *pItem)
{
	if( !P_VALID(pItem)  )
		return E_SystemError;

	if( CheckItemConflict(pItem) ) return E_UseItem_Operating;

	INT nRet = E_Success;

	nRet = CheckItemAbility(pItem);
	if( E_Success != nRet ) return nRet;

	nRet = CheckOwnerLimitItem();
	if( E_Success != nRet ) return nRet;

	nRet = CheckRoleProtoLimit(pItem);
	if( E_Success != nRet ) return nRet;

	nRet = CheckRoleStateLimit(pItem);
	if( E_Success != nRet ) return nRet;

	nRet = CheckRoleVocationLimit(pItem);
	if( E_Success != nRet ) return nRet;

	nRet = CheckMapLimit(pItem);
	if( E_Success != nRet ) return nRet;

	return nRet;
}


//----------------------------------------------------------------------------
// �����Ʒ����
//----------------------------------------------------------------------------
INT	CombatHandler::CheckItemAbility(tagItem *pItem)
{
	if( !P_VALID(pItem) ) return E_UseItem_ItemNotExist;

	// ��Ʒ�Ƿ��ǿ�ʹ����Ʒ
	if(MIsEquipment(pItem->dwTypeID) || pItem->pProtoType->dwBuffID0 == GT_INVALID)
		return E_UseItem_ItemCanNotUse;

	// ��Ʒ����ȴʱ�仹û�����򲻿���ʹ��
	if(((Role*)m_pOwner)->GetItemMgr().IsItemCDTime(pItem->dwTypeID))
		return E_UseItem_CoolDowning;

	return E_Success;
}

//----------------------------------------------------------------------------
// ���ʹ���߱���
//----------------------------------------------------------------------------
INT CombatHandler::CheckOwnerLimitItem()
{
	// �Ƿ��ڲ���ʹ�ü��ܵ�״̬
	if( m_pOwner->IsInStateCantCastSkill() )
		return E_UseItem_UseLimit;

	return E_Success;
}

//----------------------------------------------------------------------------
// ���������������
//----------------------------------------------------------------------------
INT CombatHandler::CheckRoleProtoLimit(tagItem *pItem)
{
	if( !P_VALID(pItem) ) return E_UseItem_ItemNotExist;

	// �Ա�����
	if( pItem->pProtoType->eSexLimit != ESL_Null )
	{
		if( ESL_Man == pItem->pProtoType->eSexLimit )
		{
			if( 1 != m_pOwner->GetSex() )
				return E_UseItem_SexLimit;
		}
		else if( ESL_Woman == pItem->pProtoType->eSexLimit )
		{
			if( 0 != m_pOwner->GetSex() )
				return E_UseItem_SexLimit;
		}
		else
		{

		}
	}

	// �ȼ�����
	if(pItem->pProtoType->byMinUseLevel > m_pOwner->GetLevel() 
		|| pItem->pProtoType->byMaxUseLevel < m_pOwner->GetLevel())
		return E_UseItem_LevelLimit;

	// ְҵ����

	return E_Success;
}


//----------------------------------------------------------------------------
// �������״̬����
//----------------------------------------------------------------------------
INT CombatHandler::CheckRoleStateLimit(tagItem *pItem)
{
	// ����״̬���ƣ����� ��ѣ�Σ�
	DWORD dwSelfStateFlag = m_pOwner->GetStateFlag();

	if( (dwSelfStateFlag & pItem->pProtoType->dwStateLimit) != dwSelfStateFlag )
	{
		return E_UseItem_SelfStateLimit;
	}

	Role *pRole = dynamic_cast<Role *>(m_pOwner);
	if(pRole != NULL)
	{
		DWORD dwRoleState = pRole->GetState();
		if(pRole->IsInRoleState(ERS_Wedding))
		{
			return E_UseItem_SelfStateLimit;
		}
	}
	// ����ڻ�е�����

	// ��������
	return E_Success;
}

//----------------------------------------------------------------------------
// �������ְҵ����
//----------------------------------------------------------------------------
INT CombatHandler::CheckRoleVocationLimit(tagItem *pItem)
{
	if(!P_VALID(pItem)) return E_UseItem_ItemNotExist;

	if(!m_pOwner->IsRole()) return E_Success;

	INT nClass = (INT)(static_cast<Role*> (m_pOwner)->GetClass());
	INT nClassEx = (INT)(static_cast<Role*> (m_pOwner)->GetClassEx());

	INT nTmpClass =  1 << ( nClass - 1 );
	INT nTmpClassEx = 0;

	if ( (INT)nClassEx != (INT)EHV_Base )
	{
		nTmpClassEx = 1 << ( nClassEx + 8 );
	}

	if ( ( nTmpClass + nTmpClassEx ) & pItem->pProtoType->dwVocationLimit )
		return E_Success;
	else
		return E_UseItem_VocationLimit;
}

//----------------------------------------------------------------------------
// ����ͼ����
//----------------------------------------------------------------------------
INT CombatHandler::CheckMapLimit(tagItem* pItem)
{
	// �жϵ�ͼ����
	if(P_VALID(m_pOwner->GetMap()))
	{
		BOOL bUesAble = m_pOwner->GetMap()->CanUseItem(pItem->dwTypeID);
		if( !bUesAble )	return E_UseItem_MapLimit;
	}
	
	return E_Success;
}

//-------------------------------------------------------------------------------
// ������Ʒʹ�ó�ͻ�������ͻ��ΪTRUE������ͻΪFALSE
//-------------------------------------------------------------------------------
BOOL CombatHandler::CheckItemConflict(tagItem* pItem)
{
	if( IsUseItem() ) return TRUE;	// ��ǰ����ʹ����Ʒ������ʹ��

	if( IsUseSkill() )
	{
		// �����Ʒ��������Ʒ���򲻿���ʹ��
		if( pItem->pProtoType->nPrepareTime > 0 ) return TRUE;
		else return FALSE;
	}

	return FALSE;
}

//-------------------------------------------------------------------------------
// ��ȡbuffӰ����Ŀ�껤��ֵ
//-------------------------------------------------------------------------------
FLOAT CombatHandler::GetTargetArmor(Unit &target)
{
	return (FLOAT)target.GetAttValue(ERA_Armor) * m_fTargetArmorLeftPct;
}