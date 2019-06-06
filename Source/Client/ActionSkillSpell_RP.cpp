#include "StdAfx.h"
#include "ActionSkillSpell_RP.h"
#include "Player.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "SkillProtoData.h"
#include "EffectMgr.h"
#include "CombatEvent.h"

ActionSkillSpell_RP::ActionSkillSpell_RP(void)
: m_bClosed(true)
{}

ActionSkillSpell_RP::~ActionSkillSpell_RP(void)
{}

void ActionSkillSpell_RP::Active( Player* pRole,Role* pTarget,DWORD skillID,DWORD spellTime )
{	
	m_bClosed=false;

	//--�Ӽ��������в��Ҷ�����
	const tagSkillProtoClient* pSkillProto=SkillProtoData::Inst()->FindSkillProto(skillID);
	ASSERT(pSkillProto!=NULL);
	m_bMoveSpell = pSkillProto->bInterruptMove==FALSE;
	
	//--������������
	m_szTrackName1=pSkillProto->szPrepareMod;
	m_szTrackName2=pSkillProto->szPrepareMod;
	m_szTrackName1+=_T('1');
	m_szTrackName2+=_T('2');
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,m_szTrackName1.c_str(),false,true);

	//--����Ŀ��
	if(pTarget!=NULL && pTarget!=pRole)
	{
		FacetoTarget(pRole,pTarget);
	}

	//--��¼����ID
	m_skillID=skillID;
	m_spellTime=spellTime;
	m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

	m_step=EStep_Action1;

	//--��¼��Ч����
	m_strPreSfx=pSkillProto->szPreSfx;
	m_vecPreSfxTagNode=pSkillProto->vecTagPreSfx;
	m_vecPreSfxID.resize(m_vecPreSfxTagNode.size(),GT_INVALID);
}

void ActionSkillSpell_RP::Update(Player* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	if(Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime>=m_spellTime)
	{
		m_bClosed=true;
		StopEffect();
	}

	if(m_step==EStep_Action1)
	{
		if(pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,m_szTrackName1.c_str()))
		{
			pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,m_szTrackName2.c_str(),true,true);
			m_step=EStep_Action2;

			//--����������Ч
			StopEffect();  // ��ʱû�йرգ���
			PlayEffect(pRole->GetID());
		}
	}

	//--����Ŀ��
	if(pTarget!=NULL && pTarget!=pRole)
	{
		FacetoTarget(pRole,pTarget);
	}
}

void ActionSkillSpell_RP::Stop(Player* pRole)
{
	if(m_bClosed)
		return;

	m_bClosed=true;

	//--ֹͣ��������
	pRole->GetAniPlayer()->Sync2Lower();

	//--վ�������ָ�, �ƶ����ָ�����
	if( pRole->IsStandState() )
	{
		pRole->SetMoveYaw( pRole->GetYaw() );
	}
	else
	{
		pRole->SetYaw( pRole->GetMoveYaw() );
	}


	//--�ָ�������
	pRole->FaceBack();

	//--ֹͣ��Ч
	StopEffect();
}

void ActionSkillSpell_RP::PlayEffect(DWORD dwRoleID)
{
	if( !m_strPreSfx.empty() )
	{
		for( size_t nEff = 0; nEff < m_vecPreSfxTagNode.size(); nEff++ )
		{
			ASSERT( GT_INVALID == m_vecPreSfxID[nEff] );
			m_vecPreSfxID[nEff]=EffectMgr::Inst()->PlayRoleEffect(dwRoleID,m_strPreSfx.c_str(),m_vecPreSfxTagNode[nEff].c_str());
		}
	}
}

void ActionSkillSpell_RP::StopEffect()
{
	//--ֹͣ������Ч
	for( size_t nEff = 0; nEff < m_vecPreSfxID.size(); nEff++ )
	{
		if( GT_INVALID != m_vecPreSfxID[nEff] )
		{
			EffectMgr::Inst()->StopEffect(m_vecPreSfxID[nEff]);
			m_vecPreSfxID[nEff] = GT_INVALID;
		}
	}
}

void ActionSkillSpell_RP::FacetoTarget( Player* pSelf,Role* pTarget )
{
	Vector3 dir=pTarget->GetPos()-pSelf->GetPos();

	if(m_bMoveSpell)
	{
		// �����վ��״̬������Ŀ��
		if(pSelf->IsStandState())
			pSelf->SetYaw(CalcYaw(dir));

		// ��ת����
		Vector3 targetPos;
		pTarget->GetHeadPos(targetPos);
		pSelf->FaceTo(targetPos,FALSE);
	}
	else
	{
		// ����Ŀ��
		pSelf->SetYaw(CalcYaw(dir));
	}
}