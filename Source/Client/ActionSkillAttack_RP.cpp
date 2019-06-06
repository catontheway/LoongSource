#include "StdAfx.h"
#include "ActionSkillAttack_RP.h"
#include "Player.h"
#include "RoleMgr.h"
#include "CombatEvent.h"
#include "SkillProtoData.h"
#include "EffectMgr.h"
#include "AudioSys.h"
#include "CombatSysUtil.h"

ActionSkillAttack_RP::ActionSkillAttack_RP(void)
: m_bClosed(true)
, m_vPlayerPos( 0.0f, 0.0f, 0.0f )
{
	m_dwMoveEffectID[0] = GT_INVALID;
	m_dwMoveEffectID[1] = GT_INVALID;
}

ActionSkillAttack_RP::~ActionSkillAttack_RP(void)
{}

void ActionSkillAttack_RP::Active( Player* pRole,Role* pTarget,DWORD skillID,DWORD dwSerial )
{	
	m_bClosed=false;

	//--��¼����ID
	m_skillID=skillID;
	m_dwSerial=dwSerial;
	m_targetID=pTarget ? pTarget->GetID() : GT_INVALID;

	//--��ȡ��������
	const tagSkillProtoClient* pSkillProto=SkillProtoData::Inst()->FindSkillProto(skillID);
	THROW_FALSE(P_VALID(pSkillProto),_T("cannot find skillproto"));
	m_trackName=pSkillProto->szActionMod;
	m_bSingle=pSkillProto->fOPRadius==0;
	m_dmgTimes=pSkillProto->nDmgTimes;
	m_bMoveable=pSkillProto->bMoveable==TRUE;
	m_bAniBlend=pSkillProto->bActionBlend==TRUE;
	m_bHostile=pSkillProto->bHostile==TRUE;

	//--������Ч/��Ч���ݣ������ظ����
	m_strAtkSfx=pSkillProto->szAtkSfx;
	m_vecAtkSfxTagNode=pSkillProto->vecTagAtkSfx;
	m_strLchSfx=pSkillProto->szLchSfx;
	m_strLchSfxTagNode=pSkillProto->szTagLchSfx;
	m_vecAttackEffectID.resize(m_vecAtkSfxTagNode.size(),GT_INVALID);
	m_strAtkSound=pSkillProto->szSound;

	if(m_trackName.size()>0)
	{
		//--���Ź�������
		if(m_bMoveable)
		{
			if(m_bAniBlend)
			{
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,m_trackName.c_str(),false,true);
			}
			else
			{
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All,m_trackName.c_str(),false,true);

				//--���Ż���������Ч
				ASSERT( GT_INVALID == m_dwMoveEffectID[0] && GT_INVALID == m_dwMoveEffectID[1] );
				m_dwMoveEffectID[0] = EffectMgr::Inst()->PlayRoleEffect( pRole->GetID(), _T("Txxy08"), _T("Bip01 R Foot") );
				m_dwMoveEffectID[1] = EffectMgr::Inst()->PlayRoleEffect( pRole->GetID(), _T("Txxy08"), _T("Bip01 L Foot") );
				//--�����أ��ƶ�ʱ����ʾ
				EffectMgr::Inst()->ShowEffect( m_dwMoveEffectID[0], false );
				EffectMgr::Inst()->ShowEffect( m_dwMoveEffectID[0], false );
				m_vPlayerPos = pRole->GetPos();
			}
		}
		else
			pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All,m_trackName.c_str(),false,true);

		//--����Ŀ��
		if(pTarget!=NULL && pTarget!=pRole)
		{
			FaceToTarget(pRole,pTarget);
		}
	}

	//--���û�м��ܶ��������Ϸ��������¼�
	if(m_trackName.empty())
	{
		tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
		event.dwSrcRoleID=pRole->GetID();
		event.dwTarRoleID=m_targetID;
		event.bSingle=m_bSingle;
		event.bLast=true;
		event.eCause=EHTC_Skill;
		event.dwMisc=m_skillID;
		event.dwMisc2=max(m_dmgTimes,0);
		event.dwSerial=m_dwSerial;
		TObjRef<GameFrameMgr>()->SendEvent(&event);

		m_bClosed=true;
	}
}

void ActionSkillAttack_RP::Update(Player* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	const vector<DWORD>& msgCodes = pRole->GetMsgCodes();
	for( size_t nMsgCode = 0; nMsgCode < msgCodes.size(); nMsgCode++ )
	{
		OnMsgCode(pRole,msgCodes[nMsgCode]);
	}

	const bool bTrackEnd = pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,m_trackName.c_str());
	if( bTrackEnd || (m_dmgTimes>1 && NULL!=pTarget && pTarget->IsDead()) )
	{
		m_bClosed=true;

		//--�ָ��������ͳ���
		RestoreYawAndLowerAni(pRole);

		//--ֹͣ��Ч
		StopEffect(!bTrackEnd);
		return;
	}

	//--����Ŀ��
	if(pTarget!=NULL && pTarget!=pRole)
	{
		FaceToTarget(pRole,pTarget);
	}

	//--��ʾ/���ػ�����Ч
	const Vector3 vCurPos = pRole->GetPos();
	const bool bShowMoveEffect = TRUE == ( vCurPos != m_vPlayerPos );
	EffectMgr::Inst()->ShowEffect( m_dwMoveEffectID[0], bShowMoveEffect );
	EffectMgr::Inst()->ShowEffect( m_dwMoveEffectID[0], bShowMoveEffect );
}

void ActionSkillAttack_RP::StopEffect(bool bAtkEffect)
{
	//--ֹͣ������Ч(��������ʱ����Ҫֹͣ������Ч)
	if( bAtkEffect )
		StopAtkSfx();

	for( size_t nEff = 0; nEff < m_vecAttackEffectID.size(); nEff++ )
		m_vecAttackEffectID[nEff] = GT_INVALID;

	//--ֹͣ����������Ч
	if( GT_INVALID != m_dwMoveEffectID[0] )
	{
		EffectMgr::Inst()->StopEffect( m_dwMoveEffectID[0] );
		m_dwMoveEffectID[0] = GT_INVALID;
	}
	if( GT_INVALID != m_dwMoveEffectID[1] )
	{
		EffectMgr::Inst()->StopEffect( m_dwMoveEffectID[1] );
		m_dwMoveEffectID[1] = GT_INVALID;
	}
}

void ActionSkillAttack_RP::Stop(Player* pRole)
{
	if(m_bClosed)
		return;

	m_bClosed=true;

	//--�ָ��������ͳ���
	RestoreYawAndLowerAni(pRole);

	//--��������Ŀ���¼�
	tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
	event.dwSrcRoleID=pRole->GetID();
	event.dwTarRoleID=m_targetID;
	event.eCause=EHTC_Skill;
	event.bSingle=m_bSingle;
	event.bLast=true;
	event.dwMisc=m_skillID;
	event.dwMisc2=max(m_dmgTimes-1,0);
	event.dwSerial=m_dwSerial;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
	
	//--ֹͣ��Ч
	StopEffect(true);
}

void ActionSkillAttack_RP::FaceToTarget( Player* pSelf,Role* pTarget )
{
	Vector3 dir=pTarget->GetPos()-pSelf->GetPos();

	if(m_bMoveable)
	{
		if(m_bAniBlend)//������ֿ�
		{
			//--�����վ��״̬������Ŀ��
			if(pSelf->IsStandState())
				pSelf->SetYaw(CalcYaw(dir));

			//--��ת����
			Vector3 targetPos;
			pTarget->GetHeadPos(targetPos);
			pSelf->FaceTo(targetPos,FALSE);
		}
		else//����ս��
		{
			pSelf->SetYaw(CalcYaw(dir));
		}	
	}
	else
	{
		pSelf->SetYaw(CalcYaw(dir));
	}
}

void ActionSkillAttack_RP::RestoreYawAndLowerAni( Player* pSelf )
{
	if(m_bMoveable)
	{
		if(m_bAniBlend)//������ֿ�
			pSelf->FaceBack();
		else//����ս��
			pSelf->ReplayLowerAni();	
	}
	else
		pSelf->ReplayLowerAni();

	//--վ�������ָ�, �ƶ����ָ�����
	if( pSelf->IsStandState() )
	{
		pSelf->SetMoveYaw( pSelf->GetYaw() );
	}
	else
	{
		pSelf->SetYaw( pSelf->GetMoveYaw() );
	}
}

void ActionSkillAttack_RP::OnMsgCode(Player* pRole,const DWORD dwMsgCode)
{
	TCHAR szAniMsg[5];
	ZeroMemory(szAniMsg,sizeof(szAniMsg));
	_FourCC2Str(dwMsgCode,szAniMsg);

	//--��������Ŀ���¼�
	if(_tcsncmp(szAniMsg,_T("bak"),3)==0)
	{
		int channel=_tstoi(&szAniMsg[3]);

		tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
		event.dwSrcRoleID=pRole->GetID();
		event.dwTarRoleID=m_targetID;
		event.eCause=EHTC_Skill;
		event.bSingle=m_bSingle;
		event.bLast=(channel+1>=m_dmgTimes);
		event.dwMisc=m_skillID;
		event.dwMisc2=channel;
		event.dwSerial=m_dwSerial;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}

	//--����/ֹͣ������Ч
	if(_tcsncmp(szAniMsg,_T("atk"),3)==0 )
	{
		if( _T('e') != szAniMsg[3] )
			PlayAtkSfx(pRole->GetID());
		else
			StopAtkSfx();
	}

	//--���ŷ�����Ч
	if(_tcsncmp(szAniMsg,_T("lch"),3)==0)
	{
		int channel=_tstoi(&szAniMsg[3]);

		tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
		event.dwSrcRoleID=pRole->GetID();
		event.dwTarRoleID=m_targetID;
		event.eCause=EHTC_Skill;
		event.bSingle=m_bSingle;
		event.bLast=(channel+1>=m_dmgTimes);
		event.dwMisc=m_skillID;
		event.dwMisc2=channel;
		event.dwSerial=m_dwSerial;
		if( m_strLchSfx.empty() )
			TObjRef<GameFrameMgr>()->SendEvent(&event);
		else
			EffectMgr::Inst()->PlayLaunchEffect(event,m_strLchSfx.c_str(),m_strLchSfxTagNode.c_str());

		//--���Ź�����Ч
		if(m_strAtkSound != _T(""))
			TObjRef<AudioSys>()->Play3DSound(m_strAtkSound.c_str(),100.0f,100.0f*10.0f,pRole->GetPos(),SOUND_WISPER);

	}

	//--���Ź���������
	if(m_bHostile&&_tcsncmp(szAniMsg,_T("bf"),2)==0)
	{
		// 20%����
		if( 0 == rand() % 5 )
			CombatSysUtil::Inst()->PlayPlayerRoarSound(pRole);
	}
}

void ActionSkillAttack_RP::PlayAtkSfx(DWORD dwRoleID)
{
	if( !m_strAtkSfx.empty() )
	{
		for( size_t nEff = 0; nEff < m_vecAtkSfxTagNode.size(); nEff++ )
		{
			//ASSERT( GT_INVALID == m_vecAttackEffectID[nEff] );
			m_vecAttackEffectID[nEff]=EffectMgr::Inst()->PlayRoleEffect(dwRoleID,m_strAtkSfx.c_str(),m_vecAtkSfxTagNode[nEff].c_str());
		}
	}
}
void ActionSkillAttack_RP::StopAtkSfx()
{
	for( size_t nEff = 0; nEff < m_vecAttackEffectID.size(); nEff++ )
	{
		if( GT_INVALID != m_vecAttackEffectID[nEff] )
		{
			EffectMgr::Inst()->StopEffect(m_vecAttackEffectID[nEff]);
			m_vecAttackEffectID[nEff] = GT_INVALID;
		}
	}
}
