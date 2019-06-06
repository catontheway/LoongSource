#include "StdAfx.h"
#include "ActionMouseSwim_LP.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "..\WorldDefine\action.h"
#include "EffectMgr.h"
#include "AudioSys.h"

ActionMouseSwim_LP::ActionMouseSwim_LP(void)
: m_bClosed(true)
, m_result(ER_Unknow)
, m_dwSoundID(GT_INVALID)
{
	m_lastPlayeEffectTime=Kernel::Inst()->GetAccumTime();
}

ActionMouseSwim_LP::~ActionMouseSwim_LP(void)
{}

void ActionMouseSwim_LP::Active( const Vector3& start,const Vector3& end,float validDist,NavMap* pNav,LocalPlayer* pRole,bool bNeedSendStopCmd )
{
	m_bClosed=false;
	m_result=ER_Unknow;
	m_validDistSq=validDist*validDist;
	m_moveEnd=end;
	m_bNeedSendStopCmd=bNeedSendStopCmd&&validDist>0.0f;

	//--�ж��Ƿ������Ч����
	if(Vec3DistSq(pRole->GetPos(),m_moveEnd)<=m_validDistSq)
	{
		m_bClosed=true;
		m_result=ER_Arrive;
		return;
	}

	//--���Կ��Ƿ����ֱ�ߵ���
	if(m_collider.IfCanGo(pNav->GetCollider(),start,end,pRole->GetRoleSize(),NULL,100)==ENR_Arrive)
	{
		//--
		m_moveMode=EMM_DirectGo;
		//--
		m_collider.Init(start,end,pRole->GetRoleSize(),pRole->GetSwimSpeed());
		//--
		pRole->SetMoveYaw(CalcYaw(end-start));
		//--
		SendCmd(start,end);
	}
	//--����ʹ��Ѱ·
	else if(m_pathFinder.Init(start,end,pNav,pRole,false))
	{
		//--
		m_moveMode=EMM_FindPath;

		Vector3 point;
		if(m_pathFinder.GetPathPoint(pNav,pRole,point))
		{
			//--
			m_collider.Init(start,point,pRole->GetRoleSize(),pRole->GetSwimSpeed());
			//--
			pRole->SetMoveYaw(CalcYaw(point-start));
			//--
			SendCmd(start,point);
		}
		else
		{
			m_bClosed=true;
			m_result=ER_CannotArrive;
			return;
		}
	}
	//--ֱ������
	else
	{
		//--
		m_moveMode=EMM_DirectGo;
		//--
		m_collider.Init(start,end,pRole->GetRoleSize(),pRole->GetSwimSpeed());
		//--
		pRole->SetMoveYaw(CalcYaw(end-start));
		//--
		SendCmd(start,end);
	}

	//--���Ŷ���
	pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Lower,_T("swm2"),true);

	//--���ó���
	pRole->SetYaw(pRole->GetMoveYaw());

	//--����ˮ����Ч
	PlaySprayEffect(pNav,pRole);

	//--������Ч
	PlaySound(pRole);
}

void ActionMouseSwim_LP::Update( NavMap* pNav,LocalPlayer* pRole )
{
	if(m_bClosed)
		return;

	//--��ײ���
	Vector3 nowPos;
	float endTime;
	float deltaTime=Kernel::Inst()->GetDeltaTime();
	float startTime=Kernel::Inst()->GetAccumTime()-deltaTime;
	ENavResult result=m_collider.Update(pNav->GetCollider(),startTime,deltaTime,nowPos,endTime,false);

	pRole->SetPos(nowPos);

	//--����ˮ����Ч
	PlaySprayEffect(pNav,pRole);

	//--������Чλ��
	UpdateSound(pRole->GetPos());

	//--�ж��Ƿ������Ч����
	if(Vec3DistSq(pRole->GetPos(),m_moveEnd)<=m_validDistSq)
	{
		if(m_bNeedSendStopCmd)
		{
			tagNC_StopWalk cmd;
			cmd.curPos=pRole->GetPos();
			cmd.bSwim=true;
			TObjRef<NetSession>()->Send(&cmd);
		}
		m_bClosed=true;
		m_result=ER_Arrive;
		return;
	}

	if(m_moveMode==EMM_DirectGo)//direct go mode
	{
		if(result!=ENR_ToBeContinued)
		{
			m_bClosed=true;
			if(result==ENR_SwimLanded)
				m_result=ER_Land;
			else
				m_result=ER_CannotArrive;
		}
	}
	else//find path mode
	{
		if(result==ENR_Arrive)
		{
			Vector3 point;
			if(m_pathFinder.GetPathPoint(pNav,pRole,point))
			{
				//--
				m_collider.Init(pRole->GetPos(),point,pRole->GetRoleSize(),pRole->GetMoveSpeed());

				//--
				SendCmd(pRole->GetPos(),point);

				//--���Ŷ���
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Lower,_T("swm2"),true);

				//--���ó���
				pRole->SetMoveYaw(CalcYaw(point-pRole->GetPos()));
				pRole->SetYaw(pRole->GetMoveYaw());
			}
			else
			{
				m_bClosed=true;
				m_result=ER_CannotArrive;
				return;
			}
		}
		else if(result!=ENR_ToBeContinued)
		{
			m_bClosed=true;
			if(result==ENR_SwimLanded)
				m_result=ER_Land;
			else
				m_result=ER_CannotArrive;
		}
	}
}

void ActionMouseSwim_LP::Stop(LocalPlayer* pRole)
{
	if(m_bClosed)
		return;

	m_bClosed=true;

	tagNC_StopWalk cmd;
	cmd.curPos = pRole->GetPos();
	cmd.bSwim=true;
	TObjRef<NetSession>()->Send(&cmd);
}

void ActionMouseSwim_LP::PlaySprayEffect( NavMap* pNav,LocalPlayer* pRole )
{
	if(Kernel::Inst()->GetAccumTime()-m_lastPlayeEffectTime>0.1f)
	{
		m_lastPlayeEffectTime=Kernel::Inst()->GetAccumTime();

		float waterH;
		if(pNav->GetCollider()->GetWaterHeight(pRole->GetPos(),waterH)
			&& pRole->GetPos().y<waterH)
		{
			Vector3 effectPos=pRole->GetPos();
			effectPos.y=waterH;
			EffectMgr::Inst()->PlayEffect(effectPos,pRole->GetDestYaw(),_T("txqt06"));
		}
	}
}

void ActionMouseSwim_LP::SendCmd( const Vector3& start,const Vector3& end )
{
	tagNC_MouseWalk cmd;
	cmd.srcPos=start;
	cmd.dstPos=end;
	cmd.bSwim=true;
	TObjRef<NetSession>()->Send(&cmd);
}

void ActionMouseSwim_LP::StopSound()
{
	if(GT_INVALID!=m_dwSoundID)
	{
		TObjRef<AudioSys>()->Stop3DSound(m_dwSoundID);
		TObjRef<AudioSys>()->Destroy3DSound(m_dwSoundID);
		m_dwSoundID=GT_INVALID;
	}
}

void ActionMouseSwim_LP::PlaySound(LocalPlayer* pRole)
{
	m_dwSoundID=TObjRef<AudioSys>()->Create3DSound(_T("swim"),true,100.0f,100.0f*50.0f,SOUND_WISPER);
	if(GT_INVALID!=m_dwSoundID)
	{
		TObjRef<AudioSys>()->Set3DSoundAtt(m_dwSoundID,pRole->GetPos(),MathConst::Zero3);
		TObjRef<AudioSys>()->Play3DSound(m_dwSoundID);
	}
}

void ActionMouseSwim_LP::UpdateSound( const Vector3& rolePos )
{
	if(GT_INVALID!=m_dwSoundID)
	{
		TObjRef<AudioSys>()->Set3DSoundAtt(m_dwSoundID,rolePos,MathConst::Zero3);
	}
}