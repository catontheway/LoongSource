#include "StdAfx.h"
#include "ActionUseItemSpell_RP.h"
#include "Player.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "ItemProtoData.h"
#include "CombatEvent.h"
#include "EffectMgr.h"

ActionUseItemSpell_RP::ActionUseItemSpell_RP(void)
: m_bClosed(true),
m_dwEffectID( GT_INVALID)
{}

ActionUseItemSpell_RP::~ActionUseItemSpell_RP(void)
{}

void ActionUseItemSpell_RP::Active( Player* pRole,Role* pTarget,DWORD itemID,DWORD spellTime )
{	
	m_bClosed=false;

	//--����Ʒ�����в��Ҷ�����
	const tagItemDisplayInfo* pItemProto=ItemProtoData::Inst()->FindItemDisplay(itemID);
	THROW_FALSE(P_VALID(pItemProto),_T("cannot find itemdisplayinfo"));
	
	//--������������
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,pItemProto->szPrepareMod,true,true);

	//--��¼����ID
	m_itemID=itemID;
	m_spellTime=spellTime;
	m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto( m_itemID );
	if( P_VALID(pProto) && pProto->eSpecFunc == EISF_Fishing )
	{
		m_dwEffectID = EffectMgr::Inst()->PlayRoleEffect( pRole->GetID(), _T("Txqt11"), _T("tag_RHd") );
	}

	//--���ͽ�ɫ��ʼ�����¼�
	tagRoleSpellStartEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bSkill=false;
	evt.dwTypeID=itemID;
	evt.dwCurTime=0;
	evt.dwTimeCount=spellTime;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}

void ActionUseItemSpell_RP::Update(Player* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	if(Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime>=m_spellTime)
	{
		m_bClosed=true;

		//--���ͽ�ɫ���������¼�
		tagRoleSpellStopEvent evt;
		evt.dwRoleID=pRole->GetID();
		evt.bInterrupt=false;
		TObjRef<GameFrameMgr>()->SendEvent(&evt);
	}
}

void ActionUseItemSpell_RP::Stop(Player* pRole)
{
	if(m_bClosed)
		return;

	m_bClosed=true;

	//--ֹͣ��������
	pRole->GetAniPlayer()->Sync2Lower();

	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto( m_itemID );
	if( P_VALID(pProto) && pProto->eSpecFunc == EISF_Fishing )
	{
		EffectMgr::Inst()->StopEffect( m_dwEffectID );
		m_dwEffectID = GT_INVALID;
	}

	//--���ͽ�ɫ���������¼�
	tagRoleSpellStopEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bInterrupt=true;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}

void ActionUseItemSpell_RP::OnTargetSelectEvent(tagTargetSelectEvent* pEvent,Role* pRole)
{
	if(m_bClosed)
		return;

	//--���ͽ�ɫ��ʼ�����¼�
	tagRoleSpellStartEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bSkill=false;
	evt.dwTypeID=m_itemID;
	evt.dwCurTime=Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime;
	evt.dwTimeCount=m_spellTime;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}