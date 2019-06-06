#include "StdAfx.h"
#include "ActionSkillSpell_NPC.h"
#include "NPC.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "SkillProtoData.h"
#include "CombatEvent.h"
#include "EffectMgr.h"
#include "CreatureData.h"

ActionSkillSpell_NPC::ActionSkillSpell_NPC(void)
: m_bClosed(true)
{
}

ActionSkillSpell_NPC::~ActionSkillSpell_NPC(void)
{}

void ActionSkillSpell_NPC::Active( NPC* pRole,Role* pTarget,DWORD skillID,DWORD spellTime )
{	
	m_bClosed=false;

	//--�Ӽ��������в��Ҷ�����
	const tagSkillProtoClient* pSkillProto=SkillProtoData::Inst()->FindSkillProto(skillID);
	ASSERT(pSkillProto!=NULL);
	
	//--������������
	pRole->PlayTrack(pSkillProto->szPrepareMod,true,true);

	//--�Զ�����Ŀ��
	if(pTarget)
	{
		Vector3 dir=pTarget->GetPos()-pRole->GetPos();
		pRole->SetYaw(CalcYaw(dir));
	}

	//--��¼����ID
	m_skillID=skillID;
	m_spellTime=spellTime;
	m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

	//--����������Ч
	StopEffect();
	m_vecPreSfxID.resize(pSkillProto->vecTagPreSfx.size(),GT_INVALID);
	if( _T('\0') != pSkillProto->szPreSfx[0] )
	{
		for( size_t nEff = 0; nEff < pSkillProto->vecTagPreSfx.size(); nEff++ )
		{
			ASSERT( GT_INVALID == m_vecPreSfxID[nEff] );
			tstring strBoneName = CreatureData::Inst()->FindNpcBoneName( pRole->GetID(), pSkillProto->vecTagPreSfx[nEff].c_str() );
			m_vecPreSfxID[nEff]=EffectMgr::Inst()->PlayRoleEffect(pRole->GetID(),pSkillProto->szPreSfx,strBoneName.c_str());
		}
	}

	//--���ͽ�ɫ��ʼ�����¼�
	tagRoleSpellStartEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bSkill=true;
	evt.dwTypeID=skillID;
	evt.dwCurTime=0;
	evt.dwTimeCount=spellTime;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}

void ActionSkillSpell_NPC::Update(NPC* pRole,Role* pTarget)
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

		StopEffect();
	}
}

void ActionSkillSpell_NPC::Stop(NPC* pRole)
{
	//--���ͽ�ɫ���������¼�
	tagRoleSpellStopEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bInterrupt=true;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);

	StopEffect();
}

void ActionSkillSpell_NPC::StopEffect()
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

void ActionSkillSpell_NPC::OnTargetSelectEvent(tagTargetSelectEvent* pEvent,Role* pRole)
{
	if(m_bClosed)
		return;

	//--���ͽ�ɫ��ʼ�����¼�
	tagRoleSpellStartEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bSkill=true;
	evt.dwTypeID=m_skillID;
	evt.dwCurTime=Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime;
	evt.dwTimeCount=m_spellTime;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}