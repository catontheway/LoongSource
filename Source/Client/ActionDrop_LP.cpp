#include "StdAfx.h"
#include "ActionDrop_LP.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\action.h"
#include "EffectMgr.h"
#include "CombatSysUtil.h"
#include "FSM_LP.h"
#include "Pet.h"

ActionDrop_LP::ActionDrop_LP(void)
: m_step(EStep_Closed)
, m_result(ER_Unknow)
{}

ActionDrop_LP::~ActionDrop_LP(void)
{}

void ActionDrop_LP::Active( const Vector3& start,const Vector3& dir,NavMap* pNav,LocalPlayer* pRole )
{
	m_step=EStep_Drop;
	m_result=ER_Unknow;

	m_collider.Init(start,dir,pRole->GetRoleSize(),pRole->GetDropXzSpeed(),0.0f);

	//--����������Ϣ
	tagNC_Drop cmd;
	cmd.srcPos=start;
	cmd.dir=dir;
	TObjRef<NetSession>()->Send(&cmd);

	//--���ó���
	pRole->SetYaw(CalcYaw(dir));

	//--���Ŷ���
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Drop,AvatarAniMixer::EAP_Lower,_T("jmp2"),true);

	//--��Ч���
	m_bPlayInEffect=false;
}

void ActionDrop_LP::Update( bool bPlayLandAni,NavMap* pNav,LocalPlayer* pRole )
{
	if(m_step==EStep_Closed)
		return;

	//--����״̬--------------------------------------------------------------------------------
	if(m_step==EStep_Drop)
	{
		//--��ײ���
		float deltaTime=Kernel::Inst()->GetDeltaTime();
		float startTime=Kernel::Inst()->GetAccumTime()-deltaTime;

		Vector3 nowPos;
		float endTime;
		DWORD dwCarrierObjID;
		float cdCarrierTime;
		ENavResult result=m_collider.Update(pNav->GetCollider(),startTime,deltaTime,NULL,nowPos,endTime,dwCarrierObjID,cdCarrierTime,pRole->CanWaterWalk());

		pRole->SetPos(nowPos);

		//--������ˮ��Ч
		if(!m_bPlayInEffect)
		{
			float waterH;
			if(pNav->GetCollider()->GetWaterHeight(nowPos,waterH)
				&& nowPos.y<=waterH)
			{
				m_bPlayInEffect=true;

				Vector3 effectPos=nowPos;
				effectPos.y=waterH;
				EffectMgr::Inst()->PlayEffect(effectPos,pRole->GetDestYaw(),_T("txqt04"));
			}
		}

		if(result!=ENR_ToBeContinued)
		{
			if(result==ENR_Landed
				&&bPlayLandAni)//�ж��Ƿ񲥷���ض���
			{
				//--���ŽŲ���Ч
				Pet* pMount = pRole->GetMount();
				if( NULL == pMount )
					CombatSysUtil::Inst()->PlayFootSound(nowPos);
				else
					CombatSysUtil::Inst()->PlayNPCFootSound(pMount);

				//������ض���
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Land,AvatarAniMixer::EAP_Lower,_T("jmp3"),false);

				m_step=EStep_Land;
			}
			else
			{
				//--���ŽŲ���Ч
				Pet* pMount = pRole->GetMount();
				if( NULL == pMount )
					CombatSysUtil::Inst()->PlayFootSound(nowPos);
				else
					CombatSysUtil::Inst()->PlayNPCFootSound(pMount);

				m_step=EStep_Closed;

				if(result==ENR_WillVDrop)
					m_result=ER_VDrop;
				else if(result==ENR_WillSlide)
					m_result=ER_Slide;
				else if(result==ENR_WillSwim)
					m_result=ER_Swim;
				else if(result==ENR_WillOnWater)
					m_result=ER_OnWater;
				else
					m_result=ER_Land;
				return;
			}
		}

	}

	//--���״̬----------------------------------------------------------------------------------
	if(m_step==EStep_Land)
	{
		if( pRole->GetAniPlayer()->IsTrackEnd( AvatarAniPlayer::EAT_Land, _T("jmp3") ) )
		{
			m_step=EStep_Closed;			
			m_result=ER_Land;
			return;
		}
	}
}

void ActionDrop_LP::ReplayAni( LocalPlayer* pRole )
{
	if( m_step==EStep_Closed )
		return;

	if(m_step==EStep_Drop)
	{
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Drop,AvatarAniMixer::EAP_Lower,_T("jmp2"),true);
	}
	else if(m_step==EStep_Land)
	{
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Land,AvatarAniMixer::EAP_Lower,_T("jmp3"),false);
	}
}