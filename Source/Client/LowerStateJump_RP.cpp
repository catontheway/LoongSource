#include "StdAfx.h"
#include "LowerStateJump_RP.h"
#include "Player.h"
#include "FSM_RP.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\action.h"
#include "EffectMgr.h"


LowerStateJump_RP::LowerStateJump_RP(void)
{}

LowerStateJump_RP::~LowerStateJump_RP(void)
{}

void LowerStateJump_RP::Active( tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole )
{
    // �ص����޸���
    pRole->SwitchMountPitch(false);

	ASSERT(pNetCmd->dwID==pNetCmd->Crc32("NS_SyncJump"));
	tagNS_SyncJump* pCmd=(tagNS_SyncJump*)pNetCmd;

	//--��ʼ����ײ�����
	m_collider.Init(pCmd->srcPos,pCmd->dir,pRole->GetRoleSize(),pCmd->fXZSpeed,pCmd->fYSpeed);
	m_lastCDTime=Kernel::Inst()->GetAccumTime()-pCmd->curTime;

	//--���Ŷ���
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_StartJump,AvatarAniMixer::EAP_Lower,_T("jmp1"),false);

	//--���ó���
	if(pCmd->dir.x!=0.0f && pCmd->dir.z!=0.0f)
	{
		float yaw=CalcYaw(pCmd->dir);
		pRole->SetMoveYaw(yaw);
		pRole->SetYaw(yaw);
	}

	//--���ų�ˮ��Ч 
	float waterH;
	if(pNav->GetCollider()->GetWaterHeight(pRole->GetPos(),waterH)
		&& pRole->GetPos().y<=waterH)
	{

		Vector3 effectPos=pRole->GetPos();
		effectPos.y=waterH;
		EffectMgr::Inst()->PlayEffect(effectPos,pRole->GetDestYaw(),_T("txqt04"));
	}

	//--��ˮ��Ч���
	m_bPlayInEffect=false;

	m_step=EStep_StartJump;
}

void LowerStateJump_RP::Update(NavMap* pNav,Player* pRole)
{
	//--��ײ���
	Vector3 nowPos;
	float endTime;
	DWORD dwCarrierObjID;
	float cdCarrierTime;

	ENavResult result=m_collider.Update(pNav->GetCollider(),m_lastCDTime,Kernel::Inst()->GetAccumTime()-m_lastCDTime,NULL,nowPos,endTime,dwCarrierObjID,cdCarrierTime,pRole->CanWaterWalk());
	m_lastCDTime=Kernel::Inst()->GetAccumTime();
	
	pRole->SetPos(nowPos);

	//--����------------------------------------------------------------------------------
	if(m_step==EStep_StartJump)
	{
		//--
		if(pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_StartJump,_T("jmp1"))
			||result!=ENR_ToBeContinued)
		{
			pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Jumpping,AvatarAniMixer::EAP_Lower,_T("jmp2"),true);

			m_step=EStep_Jumpping;
		}
	}
	//--��Ծ��------------------------------------------------------------------------------
	if(m_step==EStep_Jumpping)
	{
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
			if(result==ENR_Landed)//�ж��Ƿ񲥷���ض���
			{
				//--������ض���
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Land,AvatarAniMixer::EAP_Lower,_T("jmp3"),false);

				m_step=EStep_Land;
			}
			else
			{
                // �����޸���
                pRole->SwitchMountPitch(true);

				m_pFSM->Change2LowerState_Stand();

				return;
			}
		}
	}
	//--���---------------------------------------------------------------------------------
	if(m_step==EStep_Land)
	{
		if( pRole->GetAniPlayer()->IsTrackEnd( AvatarAniPlayer::EAT_Land, _T("jmp3") ) )
		{
            // �����޸���
            pRole->SwitchMountPitch(true);

			m_pFSM->Change2LowerState_Stand();
		}
	}
}

void LowerStateJump_RP::OnNetCmd( tagNetCmd* pNetCmd )
{
	m_pFSM->Change2LowerState(pNetCmd);
}

bool LowerStateJump_RP::IfCanChange2UpperState( FSM_RP::EUpperState state )
{
	switch(state)
	{
	case FSM_RP::EUpperState_BeAttack:
		return false;
	}

	return true;
}