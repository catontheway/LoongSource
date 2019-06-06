#include "StdAfx.h"
#include "ActionSlide_LP.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\action.h"
#include "FSM_LP.h"

ActionSlide_LP::ActionSlide_LP(void)
: m_bClosed(false)
, m_result(ER_Unknow)
{}

ActionSlide_LP::~ActionSlide_LP(void)
{}

void ActionSlide_LP::Active( const Vector3& start,NavMap* pNav,LocalPlayer* pRole,bool bCanPlayAni )
{
	m_bClosed=false;
	m_result=ER_Unknow;

	//--��ʼ����ײ�����
	m_collider.Init(start,pRole->GetRoleSize());

	//--����������Ϣ
	tagNC_Slide cmd;
	cmd.srcPos=start;
	TObjRef<NetSession>()->Send(&cmd);

	//--��¼��ʼ����ʱ��
	m_startSlideTime=Kernel::Inst()->GetAccumTime();
	m_bPlayAni=false;
}

void ActionSlide_LP::Update( NavMap* pNav,LocalPlayer* pRole,bool bCanPlayAni )
{
	if(m_bClosed)
		return;

	//--��ײ���
	float deltaTime=Kernel::Inst()->GetDeltaTime();
	float startTime=Kernel::Inst()->GetAccumTime()-deltaTime;

	Vector3 nowPos;
	float endTime;
	ENavResult result=m_collider.Update(pNav->GetCollider(),startTime,deltaTime,nowPos,endTime,pRole->CanWaterWalk());

	pRole->SetPos(nowPos);
	
	if(result!=ENR_ToBeContinued)
	{
		m_bClosed=true;

		if(result==ENR_WillSwim)
			m_result=ER_Swim;
		else if(result==ENR_WillOnWater)
			m_result=ER_OnWater;
		else
			m_result=ER_Stand;
	}

	//--����һ��ʱ���Ժ�ſ�ʼ���Ż�����������������������ʵ
	if(!m_bPlayAni
		&&Kernel::Inst()->GetAccumTime()-m_startSlideTime>=1.0f)
	{
		m_bPlayAni=true;

		if(bCanPlayAni)
			pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Slide,AvatarAniMixer::EAP_Lower,_T("jmp2"),true);
	}
}

void ActionSlide_LP::ReplayAni( LocalPlayer* pRole )
{
	if( m_bClosed )
		return;

	if(Kernel::Inst()->GetAccumTime()-m_startSlideTime>=1.0f)
	{
		m_bPlayAni=true;

		pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Slide,AvatarAniMixer::EAP_Lower,_T("jmp2"),true);
	}
}
