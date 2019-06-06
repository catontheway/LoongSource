#include "StdAfx.h"
#include "LowerStateDrop_RP.h"
#include "..\WorldDefine\action.h"
#include "EffectMgr.h"
#include "Player.h"
#include "FSM_RP.h"

LowerStateDrop_RP::LowerStateDrop_RP(void)
{}

LowerStateDrop_RP::~LowerStateDrop_RP(void)
{}

void LowerStateDrop_RP::Active( tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole )
{
	ASSERT(pNetCmd->dwID==pNetCmd->Crc32("NS_SyncDrop"));
	tagNS_SyncDrop* pCmd=(tagNS_SyncDrop*)pNetCmd;

	//--��ʼ����ײ�����
	m_collider.Init(pCmd->srcPos,pCmd->dir,pRole->GetRoleSize(),pCmd->fXZSpeed,0.0f);
	m_lastCDTime=Kernel::Inst()->GetAccumTime()-pCmd->curTime;

	//--���Ŷ���
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Drop,AvatarAniMixer::EAP_Lower,_T("jmp2"),true);

	//--���ó���
	float yaw=CalcYaw(pCmd->dir);
	pRole->SetMoveYaw(yaw);
	pRole->SetYaw(yaw);

	//--��ˮ��Ч���
	m_bPlayInEffect=false;

	m_step=EStep_Dropping;
}

void LowerStateDrop_RP::Update( NavMap* pNav,Player* pRole )
{
	//--��ײ���
	Vector3 nowPos;
	float endTime;
	DWORD dwCarrierObjID;
	float cdCarrierTime;

	ENavResult result=m_collider.Update(pNav->GetCollider(),m_lastCDTime,Kernel::Inst()->GetAccumTime()-m_lastCDTime,NULL,nowPos,endTime,dwCarrierObjID,cdCarrierTime,pRole->CanWaterWalk());
	m_lastCDTime=Kernel::Inst()->GetAccumTime();

	pRole->SetPos(nowPos);

	//--��Ծ��------------------------------------------------------------------------------
	if(m_step==EStep_Dropping)
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
			m_pFSM->Change2LowerState_Stand();
		}
	}
}

void LowerStateDrop_RP::OnNetCmd( tagNetCmd* pNetCmd )
{
	m_pFSM->Change2LowerState(pNetCmd);
}

void LowerStateDrop_RP::ReplayAni( Player* pRole )
{
	if(m_step==EStep_Dropping)
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Drop,AvatarAniMixer::EAP_Lower,_T("jmp2"),true);
	else if(m_step==EStep_Land)
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Land,AvatarAniMixer::EAP_Lower,_T("jmp3"),false);
}

bool LowerStateDrop_RP::IfCanChange2UpperState( FSM_RP::EUpperState state )
{
	switch(state)
	{
	case FSM_RP::EUpperState_BeAttack:
		return false;
	}

	return true;
}