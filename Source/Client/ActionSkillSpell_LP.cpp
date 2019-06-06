#include "StdAfx.h"
#include "ActionSkillSpell_LP.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "CombatEvent.h"
#include "SkillProtoData.h"
#include "EffectMgr.h"
#include "NPC.h"
#include "AudioSys.h"

ActionSkillSpell_LP::ActionSkillSpell_LP(void)
: m_bClosed(true)
, m_dwChopSoundID(GT_INVALID)
{}

ActionSkillSpell_LP::~ActionSkillSpell_LP(void)
{}

void ActionSkillSpell_LP::Active( LocalPlayer* pRole,Role* pTarget,DWORD skillID,DWORD dwSerial )
{	
	m_bClosed=false;
	m_bRecvSpellTime=false;
	m_skillID=skillID;
	m_dwSerial=dwSerial;
	m_targetID=pTarget ? pTarget->GetID() : GT_INVALID;

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

	//--������Ϣ
	tagNC_Skill cmd;
	cmd.dwSkillID		= skillID;
	cmd.dwSerial		= m_dwSerial;
	cmd.dwTargetRoleID	= m_targetID;
	cmd.vSrcPos			= pRole->GetPos();
	TObjRef<NetSession>()->Send(&cmd);

	m_step=EStep_Action1;

	//--��¼��Ч����
	m_strPreSfx=pSkillProto->szPreSfx;
	m_vecPreSfxTagNode=pSkillProto->vecTagPreSfx;
	m_vecPreSfxID.resize(m_vecPreSfxTagNode.size(),GT_INVALID);

	//--���Ųɼ���Ч
	if(pTarget!=NULL
		&&pTarget->IS_KIND_OF(NPC)
		&&((NPC*)pTarget)->IsGather())
	{
		m_dwChopSoundID=TObjRef<AudioSys>()->Create3DSound(_T("chop"),true,100.0f,100.0f*50.0f,SOUND_NORMAL);
		if(GT_INVALID!=m_dwChopSoundID)
		{
			TObjRef<AudioSys>()->Set3DSoundAtt(m_dwChopSoundID,pRole->GetPos(),MathConst::Zero3);
			TObjRef<AudioSys>()->Play3DSound(m_dwChopSoundID);
		}
	}
	else
	{
		m_dwChopSoundID=GT_INVALID;
	}
}

void ActionSkillSpell_LP::Update(LocalPlayer* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	if(m_bRecvSpellTime)
	{
		if(Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime>=m_spellTime)
		{
			Stop(pRole,false);
			return;
		}
	}

	if(m_step==EStep_Action1)
	{
		if(pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,m_szTrackName1.c_str()))
		{
			pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,m_szTrackName2.c_str(),true,true);
			m_step=EStep_Action2;
		}
	}

	//--����Ŀ��
	if(pTarget!=NULL && pTarget!=pRole)
	{
		FacetoTarget(pRole,pTarget);
	}
}

void ActionSkillSpell_LP::OnRecvSpellTime( DWORD spellTime )
{
	ASSERT(!m_bClosed);

	m_bRecvSpellTime=true;
	m_spellTime=spellTime;
	m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

	//--����������Ч
	StopEffect();
	PlayEffect(RoleMgr::Inst()->GetLocalPlayerID());
}

void ActionSkillSpell_LP::Stop( LocalPlayer* pRole,bool bSendStopCmd )
{
	if(m_bClosed)
		return;
	m_bClosed=true;

	//--������������
	if(bSendStopCmd)
	{
		tagNC_SkillInterrupt cmd;
		cmd.dwSkillID=m_skillID;
		TObjRef<NetSession>()->Send(&cmd);
	}

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

	//--ֹͣ�ɼ���Ч
	if(GT_INVALID!=m_dwChopSoundID)
	{
		TObjRef<AudioSys>()->Stop3DSound(m_dwChopSoundID);
		TObjRef<AudioSys>()->Destroy3DSound(m_dwChopSoundID);
		m_dwChopSoundID=GT_INVALID;
	}

	//--ֹͣ��Ч
	StopEffect();
}

void ActionSkillSpell_LP::PlayEffect(DWORD dwRoleID)
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

void ActionSkillSpell_LP::StopEffect()
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

void ActionSkillSpell_LP::FacetoTarget(LocalPlayer* pSelf,Role* pTarget)
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