#include "StdAfx.h"
#include "ActionJump_LP.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\action.h"
#include "EffectMgr.h"
#include "CombatSysUtil.h"
#include "Pet.h"

ActionJump_LP::ActionJump_LP(void)
: m_step(EStep_Closed)
, m_result(ER_Unknow)
{}

ActionJump_LP::~ActionJump_LP(void)
{}

void ActionJump_LP::Active( const Vector3& start,const Vector3& dir,bool bOrginJump,NavMap* pNav,LocalPlayer* pRole )
{
	m_result=ER_Unknow;
	m_step=EStep_StartJump;

	//--��ʼ����ײ�����
	Vector3 xzDir = bOrginJump ?  MathConst::Zero3 : dir;
	float xzSpeed;
	//if(pNav->GetCollider()->IfWillSwim(start,pRole->GetRoleSize().y))
	//	xzSpeed = pRole->GetSwimSpeed();
	//else
	xzSpeed = pRole->GetMoveSpeed();

	m_collider.Init(start,xzDir,pRole->GetRoleSize(),xzSpeed,pRole->GetJumpYSpeed());

	//--����������Ϣ
	tagNC_Jump cmd;
	cmd.srcPos=start;
	cmd.dir=xzDir;
	TObjRef<NetSession>()->Send(&cmd);

	//--���ó���
	if(!bOrginJump)
		pRole->SetYaw(CalcYaw(xzDir));

	//--���Ŷ���
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_StartJump,AvatarAniMixer::EAP_Lower,_T("jmp1"),false);

	//--���ų�ˮ��Ч 
	float waterH;
	if(pNav->GetCollider()->GetWaterHeight(pRole->GetPos(),waterH)
		&& pRole->GetPos().y<=waterH)
	{
		Vector3 effectPos=pRole->GetPos();
		effectPos.y=waterH;
		EffectMgr::Inst()->PlayEffect(effectPos,pRole->GetDestYaw(),_T("txqt04"));
	}

	//--��ʼ����ˮ��Ч��־
	m_bPlayInEffect=false;
}

void ActionJump_LP::Update( bool bPlayLandAni, NavMap* pNav,LocalPlayer* pRole )
{
	if(m_step==EStep_Closed)
		return;

	//--��ײ���
	float deltaTime=Kernel::Inst()->GetDeltaTime();
	float startTime=Kernel::Inst()->GetAccumTime()-deltaTime;

	Vector3 nowPos;
	float endTime;
	DWORD dwCarrierObjID;
	float cdCarrierTime;
	ENavResult result=m_collider.Update(pNav->GetCollider(),startTime,deltaTime,NULL,nowPos,endTime,dwCarrierObjID,cdCarrierTime,pRole->CanWaterWalk());

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
			if(result==ENR_Landed
				&&bPlayLandAni)//�ж��Ƿ񲥷���ض���
			{
				//--���ŽŲ���Ч
				Pet* pMount = pRole->GetMount();
				if( NULL == pMount )
					CombatSysUtil::Inst()->PlayFootSound(nowPos);
				else
					CombatSysUtil::Inst()->PlayNPCFootSound(pMount);

				//--������ض���
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
	//--���---------------------------------------------------------------------------------
	if(m_step==EStep_Land)
	{
		if( pRole->GetAniPlayer()->IsTrackEnd( AvatarAniPlayer::EAT_Land, _T("jmp3") ) )
		{
			m_step=EStep_Closed;			
			m_result=ER_Land;
		}
	}
}