#include "StdAfx.h"
#include "ActionUseItemAttack_LP.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "EffectMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "CombatEvent.h"
#include "ItemProtoData.h"

ActionUseItemAttack_LP::ActionUseItemAttack_LP(void)
: m_bClosed(true)
{}

ActionUseItemAttack_LP::~ActionUseItemAttack_LP(void)
{}

void ActionUseItemAttack_LP::Active( LocalPlayer* pRole,Role* pTarget,INT64 itemID,DWORD itemTypeID,DWORD dwSerial,bool bSendNetMsg )
{	
	m_bClosed=false;

	//--��ȡ��ʾ�����Ʒ����
	const tagItemDisplayInfo* pItemDisplay=ItemProtoData::Inst()->FindItemDisplay(itemTypeID);
	THROW_FALSE(P_VALID(pItemDisplay),_T("cannot find itemdisplayinfo"));
	m_trackName=pItemDisplay->szActionMod;
	m_targetID=pTarget ? pTarget->GetID() : GT_INVALID;
	m_bAniBlend=pItemDisplay->bActionBlend==TRUE;

	//--��ȡ��Ʒ����
	const tagItemProto* pItemProto=ItemProtoData::Inst()->FindItemProto(itemTypeID);
	THROW_FALSE(P_VALID(pItemProto),_T("cannot find itemproto"));
	m_bSingle=pItemProto->fOPRadius==0;
	m_bMoveable=pItemProto->bMoveable==TRUE;

	if(m_trackName.size()>0)
	{
		//--���Ź�������
		if(m_bMoveable)
		{
			if(m_bAniBlend)
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,m_trackName.c_str(),false,true);
			else
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All,m_trackName.c_str(),false,true);
		}
		else
			pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All,m_trackName.c_str(),false,true);

		//--������Ч
		if(pItemDisplay->szSourceEffect[0]!=_T('\0'))
		{
			EffectMgr::Inst()->PlayRoleEffect(pRole->GetID(),pItemDisplay->szSourceEffect);
		}
	}
	
	if(bSendNetMsg)
	{
		//--������Ϣ
		tagNC_UseItem cmd;
		cmd.n64ItemID		= itemID;
		cmd.dwTargetRoleID	= m_targetID;
		cmd.dwSerial		= m_dwSerial;
		cmd.vSrcPos			= pRole->GetPos();
		TObjRef<NetSession>()->Send(&cmd);
	}

	//--��¼��ƷID
	m_itemID=itemTypeID;
	m_dwSerial=dwSerial;

	//--���û�м��ܶ��������Ϸ�������Ŀ���¼�
	if(m_trackName.empty())
	{
		m_bClosed=true;

		tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
		event.dwSrcRoleID=pRole->GetID();
		event.dwTarRoleID=m_targetID;
		event.bSingle=m_bSingle;
		event.bLast=true;
		event.eCause=EHTC_Item;
		event.dwMisc=m_itemID;
		event.dwMisc2=0;
		event.dwSerial=m_dwSerial;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
}

void ActionUseItemAttack_LP::Update(LocalPlayer* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	if(pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,m_trackName.c_str()))
	{
		//--վ���򻬶���������Ժ󣬻ָ�������
		if(!m_bMoveable
			||!m_bAniBlend)
		{
			pRole->ReplayLowerAni();
		}

		//--
		m_bClosed=true;
		return;
	}
}

void ActionUseItemAttack_LP::Stop(LocalPlayer* pRole)
{
	if(m_bClosed)
		return;

	m_bClosed=true;

	//--վ���򻬶���������Ժ󣬻ָ�������
	if(!m_bMoveable
		||!m_bAniBlend)
	{
		pRole->ReplayLowerAni();
	}

	//--��������Ŀ���¼�
	tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
	event.dwSrcRoleID=pRole->GetID();
	event.dwTarRoleID=m_targetID;
	event.bSingle=m_bSingle;
	event.bLast=true;
	event.eCause=EHTC_Item;
	event.dwMisc=m_itemID;
	event.dwMisc2=0;
	event.dwSerial=m_dwSerial;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}