#include "StdAfx.h"
#include "GlobalStateCombat_LP.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "FSM_LP.h"
#include "CombatSys.h"
#include "CombatSysUtil.h"
#include "NetSession.h"

GlobalStateCombat_LP::GlobalStateCombat_LP(void)
{}

GlobalStateCombat_LP::~GlobalStateCombat_LP(void)
{}

void GlobalStateCombat_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole )
{
	ASSERT(pEvent->strEventName==_T("tagEnterCombatEvent"));

	GameFrameMgr* pGameFrmMgr=TObjRef<GameFrameMgr>();
	m_pCombatSys=(CombatSys*)pGameFrmMgr->GetFrame(_T("CombatSys"));
	ASSERT(P_VALID(m_pCombatSys));

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

	//--������Ϣ
	tagNC_EnterCombat cmd;
	TObjRef<NetSession>()->Send(&cmd);

	m_bClosed=false;
	m_bStartTimer=false;
	m_step=EStep_HandUpWeapon;
}

void GlobalStateCombat_LP::Update( NavMap* pNav,LocalPlayer* pRole )
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
			if(m_bStartTimer)
			{
				if(m_pFSM->IsSkillState()
					||m_pCombatSys->IsSelectEnemy())
				{
					m_bStartTimer=false;
				}
				else
				{
					if(Kernel::Inst()->GetAccumTimeDW()-m_dwStartTime>=5000)
					{
						pRole->OpenAllEquip(false);
						m_step=EStep_CloseEquip;
					}
				}
			}
			else
			{
				if(!m_pFSM->IsSkillState()
					&&!m_pCombatSys->IsSelectEnemy())
				{
					m_bStartTimer=true;
					m_dwStartTime=Kernel::Inst()->GetAccumTimeDW();
				}
			}
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

				m_bClosed=true;

				//--������Ϸ�¼�
				tagLeaveCombatEvent event(_T("tagLeaveCombatEvent"),NULL);
				m_pFSM->OnGameEvent(&event);
				TObjRef<GameFrameMgr>()->SendEvent(&event);

				//--����ͬ����������
				pRole->GetAniPlayer()->Sync2Lower();

				//--������Ϣ
				tagNC_LeaveCombat cmd;
				TObjRef<NetSession>()->Send(&cmd);
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

void GlobalStateCombat_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagEnterCombatEvent"))
	{
		if(m_step==EStep_HandDownWeapon)
		{
			LocalPlayer* pSelf=m_pFSM->GetLocalPlayer();

			//--�����ó���������
			m_szEnterTrackName=CombatSysUtil::Inst()->GetEnterCombatTrackName(pSelf->GetRWeaponType());
			pSelf->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,
				AvatarAniMixer::EAP_Upper,
				m_szEnterTrackName.c_str(),
				false,
				false,
				0.0f,
				0.2f,
				AvatarAniMixer::EPri_Lower);

			//--�������ص�����
			pSelf->HandUpWeapon(true);

			m_step=EStep_HandUpWeapon;
			m_bStartTimer=false;
		}
	}
}

void GlobalStateCombat_LP::OnNetCmd( tagNetCmd* pNetCmd )
{}