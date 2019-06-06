#include "StdAfx.h"
#include "GlobalStateCombat_RP.h"
#include "Player.h"
#include "FSM_RP.h"
#include "..\WorldDefine\role_att.h"
#include "CombatSysUtil.h"

GlobalStateCombat_RP::GlobalStateCombat_RP(void)
{}

GlobalStateCombat_RP::~GlobalStateCombat_RP(void)
{}

void GlobalStateCombat_RP::Active( tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole,DWORD dwParam )
{
	ASSERT(pNetCmd->dwID==m_pUtil->Crc32("NS_SetRoleState"));

	if(dwParam==0)
	{
		//--�����ó���������
		m_szEnterTrackName=CombatSysUtil::Inst()->GetEnterCombatTrackName(pRole->GetRWeaponType());
		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,
			AvatarAniMixer::EAP_Upper,
			m_szEnterTrackName.c_str(),
			false,
			false,
			0.0f,
			0.2f,
			AvatarAniMixer::EPri_Lower);

		//--�������ص�����
		pRole->HandUpWeapon(true);

		m_step=EStep_HandUpWeapon;
	}
	else
	{
		//--�������ص�����
		pRole->HandUpWeapon(true);

		//--չ��װ��
		pRole->OpenAllEquip(true);
		m_step=EStep_OpenEquip;
	}

	m_bClosed=false;
}

void GlobalStateCombat_RP::Update( NavMap* pNav,Player* pRole )
{
	switch(m_step)
	{
	case EStep_HandUpWeapon:   //������
		{
			if(pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,m_szEnterTrackName.c_str()))
			{
				//--����ͬ����������
				pRole->GetAniPlayer()->Sync2Lower();

				pRole->OpenAllEquip(true);
				m_step=EStep_OpenEquip;
			}
		}
		break;
	case EStep_OpenEquip:      //չ��װ�������Σ�
		{
			if(pRole->IsAllEquipOpened())
			{
				m_step=EStep_Fighting;
			}
		}
		break;
	case EStep_Fighting:	   //ս��״̬
		{
		}
		break;
	case EStep_CloseEquip:     //�ر�װ�������Σ�
		{
			if( pRole->IsAllEquipClosed() )
			{
				//--�����ջ���������
				m_szLeaveTrackName=CombatSysUtil::Inst()->GetLeaveCombatTrackName(pRole->GetRWeaponType());
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,
					AvatarAniMixer::EAP_Upper,
					m_szLeaveTrackName.c_str(),
					false,
					false,
					0.0f,
					0.2f,
					AvatarAniMixer::EPri_Lower);
				m_step=EStep_HandDownWeapon;
			}
		}
		break;
	case EStep_HandDownWeapon: //������
		{
			if( pRole->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Other,m_szLeaveTrackName.c_str()) )
			{
				pRole->HandUpWeapon(false);

				//--����ͬ����������
				pRole->GetAniPlayer()->Sync2Lower();

				m_bClosed=true;
			}
			else
			{
				//--�������ص�����
				const DWORD dwPutMsg = _MakeFourCC(_T("put"));
				const vector<DWORD>& msgCodes = pRole->GetMsgCodes();
				for( size_t nMsgCode = 0; nMsgCode < msgCodes.size(); nMsgCode++ )
				{
					if( dwPutMsg == msgCodes[nMsgCode] )
					{
						pRole->HandUpWeapon(false);
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void GlobalStateCombat_RP::OnGameEvent( tagGameEvent* pEvent )
{}

void GlobalStateCombat_RP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==m_pUtil->Crc32("NS_SetRoleState"))
	{
		if(m_step==EStep_HandDownWeapon)
		{
			tagNS_SetRoleState* pCmd=(tagNS_SetRoleState*)pNetCmd;
			if(pCmd->eState==ERS_Combat)
			{
				Player* pRole=m_pFSM->GetPlayer();

				//--�����ó���������
				m_szEnterTrackName=CombatSysUtil::Inst()->GetEnterCombatTrackName(pRole->GetRWeaponType());
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,
					AvatarAniMixer::EAP_Upper,
					m_szEnterTrackName.c_str(),
					false,
					false,
					0.0f,
					0.2f,
					AvatarAniMixer::EPri_Lower);

				m_step=EStep_HandUpWeapon;
			}
		}
	}
	else if(pNetCmd->dwID==m_pUtil->Crc32("NS_UnSetRoleState"))
	{
		tagNS_UnSetRoleState* pCmd=(tagNS_UnSetRoleState*)pNetCmd;
		if(pCmd->eState==ERS_Combat)
		{
			Player* pRole=m_pFSM->GetPlayer();

			pRole->OpenAllEquip(false);
			m_step=EStep_CloseEquip;
		}
	}
}