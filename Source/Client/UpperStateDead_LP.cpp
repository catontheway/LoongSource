#include "StdAfx.h"
#include "UpperStateDead_LP.h"
#include "FSM_LP.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "CombatSysUtil.h"
#include "EffectMgr.h"

UpperStateDead_LP::UpperStateDead_LP(void)
{}

UpperStateDead_LP::~UpperStateDead_LP(void)
{}

void UpperStateDead_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole )
{
	ASSERT(0 && _T("�ѷ���"));
}

void UpperStateDead_LP::Active( bool bPlayAni,LocalPlayer* pRole,ERoleReviveType reviveType )
{
	m_reviveType = reviveType;

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
		Revive();
	}
}

void UpperStateDead_LP::Update(NavMap* pNav,LocalPlayer* pRole)
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
			Revive();
		}
	}
}

void UpperStateDead_LP::OnGameEvent( tagGameEvent* pEvent )
{}

void UpperStateDead_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_RoleRevive"))
	{
		tagNS_RoleRevive* pCmd=(tagNS_RoleRevive*)pNetCmd;
		if(pCmd->dwErrorCode==E_Success)
		{
			m_pFSM->Change2UpperState_Idle();
			m_pFSM->ReplayLowerAni();

			// ���Ÿ�����Ч
			EffectMgr::Inst()->PlayRoleEffect( pCmd->dwRoleID, _T("Txxy03") );
		}
	}
}

void UpperStateDead_LP::Revive( void )
{
	if( m_reviveType==ERRT_None )
	{
		//��ʾ�����˵�
		tagShowDeadMenuEvent event(_T("tagShowDeadMenuEvent"),NULL);
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
	else
	{
		//���͸�����Ϣ
		tagNC_RoleRevive cmd;
		cmd.eType=m_reviveType;
		cmd.n64ItemID=0;
		TObjRef<NetSession>()->Send(&cmd);
	}
}

bool UpperStateDead_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
	return true;
}

bool UpperStateDead_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	return false;
}

bool UpperStateDead_LP::IfCanPlayLowerAni()
{
	return false;
}

bool UpperStateDead_LP::IfLowerCanSetYaw()
{
	return false;
}