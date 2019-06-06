//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_tracker.cpp
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ���������
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "pet_tracker.h"

#include "pet_soul.h"
#include "unit.h"
#include "role.h"
#include "pet.h"
#include "move_data.h"

//----------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------
VOID PetTracker::Update()
{
	Unit* pTarget = GetTarget();
	Role* pMaster = GetMaster();
	// ��û��Ŀ�꣬���������
	if (!P_VALID(pTarget) && P_VALID(pMaster))
	{
		SetTarget(pMaster->GetID());
		pTarget = GetTarget();
	}
	else if (!P_VALID(pMaster))
		return ;

	// ��Ŀ����߳�����Ч
	if (!P_VALID(pTarget) ||	!P_VALID(m_pPet) ) 
		return;

	// ���³����ٶ�
	UpdatePetSpeed(pTarget);

	// �����ƶ�����
	m_pPet->GetMoveData().Update();

	// ����Ŀ��״̬�ҵ�Ŀ�ĵص�
	Vector3 vTargetPos		= pTarget->GetMoveData().m_vPos;
	Vector3 vTargetFace		= pTarget->GetMoveData().m_vFace;
	Vector3 vPetDest		= m_pPet->GetMoveData().m_vDest;
	Vector3 vPetCur			= m_pPet->GetCurPos();

	// �Ƿ���Ҫ˲��
	if (NeedPutdown(vTargetPos, m_pPet->GetCurPos()))
	{
		PutDownBack(m_pPet, pTarget->GetMoveData().m_vPos, pTarget->GetMoveData().m_vFace);
		return ;
	}

	// �Ƿ���Ҫ�ƶ�
	if (!NeedMove(vTargetPos, m_pPet->GetCurPos()))
		return;

	// �����ߵ�Ŀ�����
	Vector3 vTmp = GetNearPos(vTargetPos, vTargetFace, (FLOAT)MAX_NEED_MOVE_LEN, (FLOAT)MIN_NEED_MOVE_LEN);
	if (TryGoto(m_pPet, vTmp) )
		m_pPet->GetMoveData().StartCreatureWalk(vTmp, EMS_CreatureWalk, FALSE);
}

//----------------------------------------------------------------------------------------------------
// ����Ŀ���ҵ�Ŀ�ĵ�
//----------------------------------------------------------------------------------------------------
Vector3 PetTracker::FindTargetDest( Unit* pTarget )
{
	Vector3	vTargetPos;
	EMoveState eTarMove	= pTarget->GetMoveData().m_eCurMove;
	Vector3 vTargetCurPos	= pTarget->GetMoveData().m_vPos;
	Vector3	vTargetDestPos	= pTarget->GetMoveData().m_vDest;
	if (EMS_Stand == eTarMove)
	{
		vTargetPos = vTargetCurPos;
	}
	else
	{
		vTargetPos = vTargetDestPos;
	}
	return vTargetPos;
}

//----------------------------------------------------------------------------------------------------
// �Ƿ���Ҫ�ƶ�
//----------------------------------------------------------------------------------------------------
BOOL PetTracker::NeedMove(const Vector3 &vMasterPos, const Vector3 &vPet)
{
	BOOL bNeedMove	= FALSE;
	FLOAT fDist = Vec3Dist(vMasterPos, vPet);
	if (fDist >= (FLOAT)MAX_NEED_MOVE_LEN)
	{
		bNeedMove	= TRUE;
	}
	else if (fDist <= (FLOAT)MIN_NEED_MOVE_LEN)
	{
		bNeedMove	= TRUE;
	}
	return bNeedMove;
}

//----------------------------------------------------------------------------------------------------
// ��һ������Χ�ҵ������
//----------------------------------------------------------------------------------------------------
Vector3	PetTracker::GetNearPos(const Vector3 &vMasterPos, const Vector3 &vMasterFace, FLOAT fMaxRange, FLOAT fMinRange)
{
	//���ѡ��һ���Ƕ�
	FLOAT fAngle	= (IUTIL->Rand() % 360) / 360.0f * 3.14f;
	//���ѡ��һ������ķ�Χ
	FLOAT fDist		= IUTIL->Rand() % INT(fMaxRange - fMinRange) + fMinRange;
	//����Ŀ���
	Vector3 vRt		= vMasterPos;
	vRt.x +=	sin(fAngle) * fDist;
	vRt.z +=	cos(fAngle) * fDist;
	return vRt;
}

//----------------------------------------------------------------------------------------------------
// �ɷ��ƶ���ĳ����
//----------------------------------------------------------------------------------------------------
BOOL PetTracker::TryGoto( Pet* pPet, const Vector3 &vDstPos )
{
	BOOL	bRtv = TRUE;
	if (!P_VALID(pPet))
	{
		bRtv = FALSE;
	}
	else if( MoveData::EMR_Success != pPet->GetMoveData().IsCanCreatureWalk(vDstPos, EMS_CreatureWalk, TRUE))
	{
		bRtv = FALSE;
	}

	return bRtv;		
}

//----------------------------------------------------------------------------------------------------
// �ѳ���ŵ����
//----------------------------------------------------------------------------------------------------
BOOL PetTracker::PutDownBack( Pet* pPet, const Vector3 &vMasterPos, const Vector3 &vMasterFace )
{
	Vector3 vFaceNormal = -vMasterFace;
	Vec3Normalize(vFaceNormal);
	Vector3 vNewPos		= vMasterPos + vFaceNormal * MAX_NEED_MOVE_LEN;

	if (!P_VALID(pPet))
	{
		return FALSE;
	}
	//tbc:
	pPet->GetMoveData().ForceTeleport(vNewPos);
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// ���ó�����ٶ�
//----------------------------------------------------------------------------------------------------
VOID PetTracker::UpdatePetSpeed(Unit* pTarget)
{
	if(P_VALID(m_pPet) && P_VALID(pTarget) && m_pPet->GetXZSpeed() != pTarget->GetXZSpeed() && pTarget->GetXZSpeed() != 0)
	{
		Role* pRole = static_cast<Role*>(pTarget);
		if(pTarget->IsRole() && pRole->IsInRoleStateAny(ERS_Mount | ERS_Mount2))
		{
			m_pPet->SetAttValue(ERA_Speed_XZ, pTarget->GetAttValue(ERA_Speed_Mount), TRUE);	
		}
		else
		{
			m_pPet->SetAttValue(ERA_Speed_XZ, pTarget->GetAttValue(ERA_Speed_XZ), TRUE);	
		}
		
		m_pPet->GetMoveData().StopMove();
	}
}

//----------------------------------------------------------------------------------------------------
// ���캯��Z
//----------------------------------------------------------------------------------------------------
PetTracker::PetTracker(PetSoul* pSoul, Pet* pPet)
{
	ASSERT(P_VALID(pSoul));
	ASSERT(P_VALID(pPet));
	m_pPet		= pPet;
	m_pSoul		= pSoul;

	m_dwTargetID = GT_INVALID;

}

Role* PetTracker::GetMaster() const
{
	return m_pSoul->GetMaster();
}

BOOL PetTracker::NeedPutdown( const Vector3 &vMasterPos, const Vector3 &vPet )
{
	BOOL bNeedMove	= FALSE;
	FLOAT fDist = Vec3Dist(vMasterPos, vPet);
	if (fDist >= NEED_TRANS_LEN)
	{
		bNeedMove	= TRUE;
	}
	if (m_nPutBackTicks-- > 0)
	{
		return FALSE;
	}
	m_nPutBackTicks = MAX_TRANS_TICKS;

	return bNeedMove;
}

VOID PetTracker::SetTarget( DWORD dwTargetID )
{
	Unit* pTarget = GetUnit(dwTargetID);
	if (P_VALID(pTarget))
	{
		m_dwTargetID = dwTargetID;	
		UpdatePetSpeed(pTarget);
	}
	else
	{
		m_dwTargetID = GetMaster()->GetID();
		UpdatePetSpeed(GetMaster());
	}
}

Unit* PetTracker::GetUnit( DWORD dwID ) const
{
	Unit* pUnit = NULL;
	if (P_VALID(m_pPet) && m_pPet->GetMap())
	{
		pUnit = m_pPet->GetMap()->FindUnit(dwID);
	}
	return pUnit;
}