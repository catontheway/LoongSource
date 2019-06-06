#include "StdAfx.h"
#include "ActionUseItemSpell_LP.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "CombatEvent.h"
#include "ItemProtoData.h"
#include "SkillProEvent.h"
#include "Item.h"
#include "ItemMgr.h"
#include "EffectMgr.h"

ActionUseItemSpell_LP::ActionUseItemSpell_LP(void)
: m_bClosed(true),
m_dwEffectID(GT_INVALID)
{}

ActionUseItemSpell_LP::~ActionUseItemSpell_LP(void)
{}

void ActionUseItemSpell_LP::Active( LocalPlayer* pRole,Role* pTarget,INT64 itemID,DWORD itemTypeID,DWORD dwSerial )
{	
	m_bClosed=false;
	m_bRecvSpellTime=false;
	m_n64ItemID = itemID;
	m_itemID=itemTypeID;
	m_dwSerial=dwSerial;
	m_targetID=pTarget ? pTarget->GetID() : GT_INVALID;

	//--����Ʒ�����в��Ҷ�����
	const tagItemDisplayInfo* pItemProto=ItemProtoData::Inst()->FindItemDisplay(itemTypeID);
	THROW_FALSE(P_VALID(pItemProto),_T("cannot find itemdisplayinfo"));
	
	//--������������
	m_szTrackName=pItemProto->szPrepareMod;
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,m_szTrackName.c_str(),true,true);
	
	//--������Ϣ
	tagNC_UseItem cmd;
	cmd.n64ItemID		= itemID;
	cmd.dwSerial		= m_dwSerial;
	cmd.dwTargetRoleID	= m_targetID;
	cmd.vSrcPos			= pRole->GetPos();
	TObjRef<NetSession>()->Send(&cmd);
}

void ActionUseItemSpell_LP::Update(LocalPlayer* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	if(m_bRecvSpellTime)
	{
		if(Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime>=m_spellTime)
		{
			Stop(pRole,false);
		}
	}
}

void ActionUseItemSpell_LP::OnRecvSpellTime( DWORD spellTime )
{
	if(m_bClosed)
		return;

	m_bRecvSpellTime=true;
	m_spellTime=spellTime;
	m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto( m_itemID );
	if( P_VALID(pProto) && pProto->eSpecFunc == EISF_Fishing )
	{
		m_dwEffectID = EffectMgr::Inst()->PlayRoleEffect( RoleMgr::Inst()->GetLocalPlayerID(), _T("Txqt11"), _T("tag_RHd") );
	}

	//--������Ϸ�¼�����ʾ����������
	tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
	event.bShow=true;
	event.bSkill=false;
	event.dwTypeID=m_itemID;
	event.dwTime=spellTime;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

void ActionUseItemSpell_LP::Stop( LocalPlayer* pRole,bool bSendStopCmd )
{
	if(m_bClosed)
		return;

	m_bClosed=true;

	//--����ֹͣ��������
	if(bSendStopCmd)
	{
		tagNC_UseItemInterrupt cmd;
		cmd.n64ItemID=m_n64ItemID;
		TObjRef<NetSession>()->Send(&cmd);
	}

	//--ֹͣ��������
	pRole->GetAniPlayer()->Sync2Lower();
	
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto( m_itemID );
	if( P_VALID(pProto) && pProto->eSpecFunc == EISF_Fishing )
	{
		EffectMgr::Inst()->StopEffect( m_dwEffectID );
		m_dwEffectID = GT_INVALID;
	}

	//--������Ϸ�¼�����������������
	tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
	event.bShow=false;
	event.dwTypeID=m_itemID;
	event.dwTime=0;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}