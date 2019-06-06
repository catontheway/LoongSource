
#include "stdafx.h"
#include "vocation.h"
#include "role.h"
#include "creature.h"
#include "../ServerDefine/log_cmdid_define.h"

//-------------------------------------------------------------
// ��ɫ���ר��ְҵ
//-------------------------------------------------------------
DWORD Role::RoleSetVocation( DWORD dwNPCID, EClassType eVocation)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Class) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return ECEC_NPCError;
	}

	// �жϵȼ��Ƿ����80��
	if(this->GetLevel() < 80) return ECEC_NotUpToLevel;
	
	// �ý�ɫ�Ƿ��Ѿ�ѡ����ר��ְҵ
	if( !(this->GetClass() == EV_Base) ) return ECEC_IsNotBaseClass;
	ASSERT(this->GetClassEx() == EHV_Base);

	// �Ƿ��о�ְƾ֤
	TList<tagItem*> ItemList;
	GetItemMgr().GetBagSameItemList(ItemList, ITEM_SetClass);
	if(ItemList.Empty()) return ECEC_LackOfNewclassItem;

	// ��ְ
	this->SetClass(eVocation);

	// ϴ����
	ClearTalent((INT64)0, FALSE);

	// �۳���ְƾ֤
	TList<tagItem*>::TListIterator it = ItemList.Begin();
	GetItemMgr().DelFromBag((*it)->n64Serial, ELCLD_Class_Get, 1);

	tagNS_RoleClassChangeBroadcast send;
	send.dwRoleID	= GetID();
	send.eClass = m_eClass;
	GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);

	return ECEC_Success;
}

//-------------------------------------------------------------
// ��ɫ���Ӣ��ְҵ
//-------------------------------------------------------------
DWORD Role::RoleSetHeroVocation( DWORD dwNPCID, EClassTypeEx eHVocation)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Class) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return ECEC_NPCError;
	}

	// �жϵȼ��Ƿ����120��
	if(this->GetLevel() < 120) return ECEC_NotUpToLevel;

	// �ý�ɫ�Ƿ��Ѿ�ѡ����Ӣ��ְҵ
	if( !(this->GetClassEx() == EHV_Base) ) return ECEC_IsNotBaseClassEx;
	ASSERT(this->GetClass()>EHV_Base && this->GetClassEx()<EHV_End);

	// ѡ���Ӣ��ְҵ�Ƿ���ר��ְҵ��ƥ��
	Vocation::tagHeroVocation* pTemp = g_VocationMap.GetVocationMatchMap().Peek(this->GetClass());
	if ( pTemp->eHVocation_1 != eHVocation && pTemp->eHVocation_2 != eHVocation) return ECEC_ClassNotMatch;

	this->SetClassEx(eHVocation);

	return ECEC_Success;
}

//-------------------------------------------------------------
// ��ɫ����ְҵ
//-------------------------------------------------------------
DWORD Role::ChangeVocation( DWORD dwNPCID, EClassType eVocation)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC)
		|| !pNPC->IsFunctionNPC(EFNPCT_Class) 
		|| !pNPC->CheckNPCTalkDistance(this))
	{
		return ECEC_NPCError;
	}

	// �жϽ�ɫĿǰ�Ƿ��Ѿ���ְ��δ��ְҲ�Ͳ���Ҫ����
	if (this->GetClass() == EV_Base) return ECEC_ClassNotSet;

	// �ж��Ƿ�����ص�IM����
	TList<tagItem*> ItemList;
	GetItemMgr().GetBagSameItemList(ItemList, IM_ChangeClass);
	if(ItemList.Empty()) return ECEC_LackOfReclassItem;

	// תְ
	this->SetClassEx(EHV_Base);
	this->SetClass(eVocation);

	// ϴ����
	ClearTalent((INT64)0, FALSE);

	// �۳�IM����
	TList<tagItem*>::TListIterator it = ItemList.Begin();
	GetItemMgr().DelFromBag((*it)->n64Serial, ELCLD_Class_Get, 1);

	tagNS_RoleClassChangeBroadcast send;
	send.dwRoleID	= GetID();
	send.eClass = GetClass();
	GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);

	return ECEC_Success;
}
