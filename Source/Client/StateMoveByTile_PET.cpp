#include "StdAfx.h"
#include "StateMoveByTile_PET.h"
#include "Pet.h"
#include "FSM_PET.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\action.h"

StateMoveByTile_PET::StateMoveByTile_PET(void)
{
}

StateMoveByTile_PET::~StateMoveByTile_PET(void)
{
}

void StateMoveByTile_PET::Active( tagNetCmd* pNetCmd,NavMap* pNav,Pet* pRole )
{
    ASSERT(pNetCmd->dwID==m_pUtil->Crc32("NS_SyncWalk"));
    tagNS_SyncWalk* pCmd=(tagNS_SyncWalk*)pNetCmd;

    //--��¼��ʼ�ƶ�ʱ���
    m_moveStartTime	= Kernel::Inst()->GetAccumTime();
    m_moveStart		= Vector2(pCmd->srcPos.x,pCmd->srcPos.z);
    m_moveDest		= Vector2(pCmd->dstPos.x,pCmd->dstPos.z);
    m_moveDir		= m_moveDest-m_moveStart;	
    m_moveTime		= D3DXVec2Length(&m_moveDir)/pCmd->fXZSpeed;
    m_moveDeltaTime = pCmd->curTime;
    D3DXVec2Normalize(&m_moveDir,&m_moveDir);

    //--���ó���
    pRole->SetYaw(CalcYaw(m_moveDir));

    //--���Ŷ���
    const TCHAR* szTrackName=CombatSysUtil::Inst()->GetNPCMoveTrackName(pCmd->fXZSpeed);
    pRole->PlayTrack(szTrackName,true);
}

void StateMoveByTile_PET::Update( NavMap* pNav,Pet* pRole )
{
	float runTime=Kernel::Inst()->GetAccumTime()-m_moveStartTime+m_moveDeltaTime;
	if(runTime>=m_moveTime)//finish
	{
		m_pFSM->Change2IdleState();
		return;
	}

	float r=runTime/m_moveTime;
	if(r>1)r=1;

	Vector3 nowPos;
	nowPos.x=LinearLerp(m_moveStart.x,m_moveDest.x,r);
	nowPos.z=LinearLerp(m_moveStart.y,m_moveDest.y,r);
	nowPos.y=pNav->GetNPCNavMap()->SafeGetHeightSlerp(nowPos.x,nowPos.z);

	pRole->SetPos(nowPos);
}

void StateMoveByTile_PET::OnNetCmd( tagNetCmd* pNetCmd )
{
    if(pNetCmd->dwID==pNetCmd->Crc32("NS_MoveSpeedChange"))//�ٶȱ仯��Ϣ
    {
        m_pFSM->Change2IdleState();
    }
    else
    {
        m_pFSM->Change2State(pNetCmd);
    }
}