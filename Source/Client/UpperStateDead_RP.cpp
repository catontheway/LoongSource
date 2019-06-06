#include "StdAfx.h"
#include "UpperStateDead_RP.h"
#include "FSM_RP.h"
#include "Player.h"
#include "..\WorldDefine\msg_combat.h"
#include "CombatSysUtil.h"
#include "EffectMgr.h"

UpperStateDead_RP::UpperStateDead_RP(void)
{}

UpperStateDead_RP::~UpperStateDead_RP(void)
{}

void UpperStateDead_RP::Active( bool bPlayAni,Player* pRole )
{
	if(bPlayAni)
	{
		//--������Ч
		CombatSysUtil::Inst()->PlayRoleDeadSound(pRole);

		//--���Ŷ���
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All,_T("dead"),false);
		m_step=EStep_FallDown;
	}
	else
	{
		//���������������һ֡
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All,_T("dead"),false,true,-1);
		m_step=EStep_Lie;
	}
}

void UpperStateDead_RP::Update(NavMap* pNav,Player* pRole)
{
	if(m_step==EStep_FallDown)
	{
		//--����ˤ����Ч
		const DWORD dwFallMsg = _MakeFourCC(_T("fall"));
		const vector<DWORD>& msgCodes = pRole->GetMsgCodes();
		for( size_t nMsgCode = 0; nMsgCode < msgCodes.size(); nMsgCode++ )
		{
			if( dwFallMsg == msgCodes[nMsgCode] )
			{
				CombatSysUtil::Inst()->PlayFallSound(pRole->GetPos());
			}
		}
		if(pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,_T("dead")))
		{
			m_step=EStep_Lie;
		}
	}
}

void UpperStateDead_RP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_RoleRevive"))
	{
		tagNS_RoleRevive* pCmd=(tagNS_RoleRevive*)pNetCmd;
		if(pCmd->dwErrorCode==E_Success)
		{
			m_pFSM->ReplayLowerAni();
			m_pFSM->Change2UpperState_Idle();

			// ���Ÿ�����Ч
			EffectMgr::Inst()->PlayRoleEffect( pCmd->dwRoleID, _T("Txxy03") );
		}
	}
}

bool UpperStateDead_RP::IfCanPlayLowerAni()
{
	return false;
}

bool UpperStateDead_RP::IfLowerCanSetYaw()
{
	return false;
}