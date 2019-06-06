//-------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: move_data.cpp
// author: Aslan
// actor:
// data: 2009-01-15
// last:
// brief: �ƶ��ṹ
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "map.h"
#include "unit.h"
#include "role.h"
#include "creature.h"
#include "move_data.h"
#include "pet_pocket.h"

//---------------------------------------------------------------------------------------------
// ��̬����
//---------------------------------------------------------------------------------------------
Timer MoveData::m_Timer;

//---------------------------------------------------------------------------------------------
// �����ƶ�������
//---------------------------------------------------------------------------------------------
NavCollider_TileMove::NavCollider_TileMove() : m_vStart(), m_vDest(), m_fTotalTime(0.0f), m_fCurTime(0.0f), m_nMoveTicks(0), m_fXZSpeed(0.0f)
{

}

NavCollider_TileMove::~NavCollider_TileMove() {}

VOID NavCollider_TileMove::Init(const Vector3& vStart, const Vector3& vDest, FLOAT fXZSpeed)
{
	ASSERT( fXZSpeed > 0.0f );

	m_vStart	=	vStart;
	m_vDest		=	vDest;
	m_fXZSpeed	=	fXZSpeed;

	// �����������
	FLOAT fDistanceX = m_vDest.x - m_vStart.x;
	FLOAT fDistanceZ = m_vDest.z - m_vStart.z;
	FLOAT fDistanceSquare = fDistanceX * fDistanceX + fDistanceZ * fDistanceZ;

	// ������ʱ�����������
	m_fTotalTime = IFASTCODE->FastSqrt(fDistanceSquare) / m_fXZSpeed;
	m_nMoveTicks = (INT)(m_fTotalTime * 1000.0f) / TICK_TIME;
	m_nMoveTicks += ( (INT(m_fTotalTime * 1000.0f) - TICK_TIME * m_nMoveTicks) > 0 ? 1 : 0 );

	// ��ǰ�ƶ�ʱ��
	m_fCurTime = 0.0f;
}

ENavResult NavCollider_TileMove::Update(Map* pMap, Vector3& vOutPos)
{
	// �������Ͳ����ƶ�
	if( m_nMoveTicks <= 0 )
	{
		vOutPos = m_vStart;
		return ENR_Arrive;
	}

	// ��ȥtick
	if( --m_nMoveTicks > 0 )
	{
		m_fCurTime += (FLOAT)TICK_TIME / 1000.0f;
	}
	else
	{
		m_fCurTime = m_fTotalTime;
	}

	vOutPos.x = m_vStart.x + (m_vDest.x - m_vStart.x) * (m_fCurTime / m_fTotalTime);
	vOutPos.z = m_vStart.z + (m_vDest.z - m_vStart.z) * (m_fCurTime / m_fTotalTime);
	vOutPos.y = pMap->GetGroundHeight(vOutPos.x, vOutPos.z);

	if( m_nMoveTicks > 0 )
	{
		return ENR_ToBeContinued;
	}
	else
	{
		return ENR_Arrive;
	}
}

//---------------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------------
BOOL MoveData::IsPosInvalid(const Vector3& vPos)
{
	// ����ǲ���ʵ��
	if( _isnan(vPos.x) || _isnan(vPos.y) || _isnan(vPos.z) )
		return TRUE;

	// ����ǲ�������
	if( 0 == _finite(vPos.x) || 0 == _finite(vPos.y) || 0 == _finite(vPos.z) )
	{
		return TRUE;
	}

	return FALSE;	
}

//---------------------------------------------------------------------------------------------
// ��鳯��
//---------------------------------------------------------------------------------------------
BOOL MoveData::IsFaceInvalid(const Vector3& vFace)
{
	// ����ǲ���ʵ��
	if( _isnan(vFace.x) || _isnan(vFace.y) || _isnan(vFace.z) )
		return TRUE;

	// ����ǲ�������
	if( 0 == _finite(vFace.x) || 0 == _finite(vFace.y) || 0 == _finite(vFace.z) )
	{
		return TRUE;
	}

	// �������һ����鿴������
	Vector3 vNewFace = vFace;
	Vec3Normalize(vNewFace);

	if( _isnan(vNewFace.x) || _isnan(vNewFace.y) || _isnan(vNewFace.z) )
		return TRUE;

	if( 0 == _finite(vNewFace.x) || 0 == _finite(vNewFace.y) || 0 == _finite(vNewFace.z) )
	{
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------------------------
// ��ʼ���ƶ��ṹ����Ҫ������Һ������ʼ��ʱ���ƶ�״̬
//---------------------------------------------------------------------------------------------
VOID MoveData::Init(Unit* pOwner, const FLOAT fX, const FLOAT fY, const FLOAT fZ, const FLOAT fFaceX, const FLOAT fFaceY, const FLOAT fFaceZ)
{
	m_pOwner			=		pOwner;
	m_eCurMove			=		EMS_Stand;
	m_eNextPreMove		=		EMS_Stand;
	m_vPos.x			=		fX;
	m_vPos.y			=		fY;
	m_vPos.z			=		fZ;
	m_vFace.x			=		fFaceX;
	m_vFace.y			=		fFaceY;
	m_vFace.z			=		fFaceZ;
	m_vPosStart			=		m_vPos;
	m_vDest				=		m_vPos;
	m_vDir				=		m_vFace;
	m_bWaitClientMsg	=		FALSE;
	m_nVisTileIndex		=		GT_INVALID;
	m_bIsStopped		=		FALSE;
}

//----------------------------------------------------------------------------------------------
// �����ƶ��ṹ������Ҫ��������л���ͼ�͹�������ʱ��һ�������
//----------------------------------------------------------------------------------------------
VOID MoveData::Reset(const FLOAT fX, const FLOAT fY, const FLOAT fZ, const FLOAT fFaceX, const FLOAT fFaceY, const FLOAT fFaceZ)
{
	m_eCurMove			=		EMS_Stand;
	m_eNextPreMove		=		EMS_Stand;
	m_vPos.x			=		fX;
	m_vPos.y			=		fY;
	m_vPos.z			=		fZ;
	m_vFace.x			=		fFaceX;
	m_vFace.y			=		fFaceY;
	m_vFace.z			=		fFaceZ;
	m_vPosStart			=		m_vPos;
	m_vDest				=		m_vPos;
	m_vDir				=		m_vFace;
	m_bWaitClientMsg	=		FALSE;
	m_nVisTileIndex		=		GT_INVALID;
	m_bIsStopped		=		FALSE;
}

//-----------------------------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartRoleWalk(const Vector3& vStart, const Vector3& vEnd)
{
	EMoveRet eRet = IsCanRoleWalk(vStart, vEnd);

	if( EMR_Success == eRet )
	{
		GetColliderWalk()->Init(vStart, vEnd, m_pOwner->GetSize(), m_pOwner->GetXZSpeed());

		m_eCurMove			=	EMS_Walk;
		m_eNextPreMove		=	EMS_Stand;
		m_vPosStart			=	vStart;
		m_vDest				=	vEnd;
		m_vDir				=	vEnd - vStart;
		m_bWaitClientMsg	=	FALSE;
		m_fStartTime		=	m_Timer.GetElapse();
		m_bIsStopped		=	FALSE;

		SetFaceTo(m_vDir);
		OnStartMove();
		OnPosChange(vStart);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//-----------------------------------------------------------------------------------------------
// ������Ծ
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartRoleJump(const Vector3& vStart, const Vector3& vDir)
{
	EMoveRet eRet = IsCanRoleJump(vStart, vDir);

	if( EMR_Success == eRet )
	{
		m_pOwner->m_fJumpXZSpeed	=	m_pOwner->GetXZSpeed();
		m_pOwner->m_fJumpYSpeed		=	m_pOwner->m_fYSpeed;
		GetColliderJump()->Init(vStart, vDir, m_pOwner->GetSize(), m_pOwner->m_fJumpXZSpeed, m_pOwner->m_fJumpYSpeed);

		m_eCurMove			=	EMS_Jump;
		m_eNextPreMove		=	EMS_Stand;
		m_vPosStart			=	vStart;
		m_vDest				=	vStart;
		m_vDir				=	vDir;
		m_bWaitClientMsg	=	FALSE;
		m_fStartTime		=	m_Timer.GetElapse();
		m_bIsStopped		=	FALSE;

		SetFaceTo(m_vDir);
		OnStartMove();
		OnPosChange(vStart);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//------------------------------------------------------------------------------------------------
// �������
//------------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartRoleDrop(const Vector3& vStart, const Vector3& vDir)
{
	EMoveRet eRet = IsCanRoleDrop(vStart, vDir);

	if( EMR_Success == eRet )
	{
		GetColliderJump()->Init(vStart, vDir, m_pOwner->GetSize(), m_pOwner->m_fDropXZSpeed, 0.0f);

		m_eCurMove			=	EMS_Drop;
		m_eNextPreMove		=	EMS_Stand;
		m_vPosStart			=	vStart;
		m_vDest				=	vStart;
		m_vDir				=	vDir;
		m_bWaitClientMsg	=	FALSE;
		m_fStartTime		=	m_Timer.GetElapse();
		m_bIsStopped		=	FALSE;

		SetFaceTo(m_vDir);
		OnStartMove();
		OnPosChange(vStart);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//------------------------------------------------------------------------------------------------
// ���ﴹֱ����
//------------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartRoleVDrop(const Vector3& vStart)
{
	EMoveRet eRet = IsCanRoleVDrop(vStart);

	if( EMR_Success == eRet )
	{
		GetColliderJump()->Init(vStart, MathConst::Zero3, m_pOwner->GetSize(), 0.0f, 0.0f);

		m_eCurMove			=	EMS_VDrop;
		m_eNextPreMove		=	EMS_Stand;
		m_vPosStart			=	vStart;
		m_vDest				=	vStart;
		m_bWaitClientMsg	=	FALSE;
		m_fStartTime		=	m_Timer.GetElapse();
		m_bIsStopped		=	FALSE;

		OnStartMove();
		OnPosChange(vStart);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//------------------------------------------------------------------------------------------------
// ���ﻬ��
//------------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartRoleSlide(const Vector3& vStart)
{
	EMoveRet eRet = IsCanRoleSlide(vStart);

	if( EMR_Success == eRet )
	{
		GetColliderSlide()->Init(vStart, m_pOwner->GetSize());

		m_eCurMove			=	EMS_Slide;
		m_eNextPreMove		=	EMS_Stand;
		m_vPosStart			=	vStart;
		m_vDest				=	vStart;
		m_bWaitClientMsg	=	FALSE;
		m_fStartTime		=	m_Timer.GetElapse();
		m_bIsStopped		=	FALSE;

		OnStartMove();
		OnPosChange(vStart);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//-----------------------------------------------------------------------------------------------
// ������Ӿ
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartRoleSwim(const Vector3& vStart, const Vector3& vEnd)
{
	EMoveRet eRet = IsCanRoleSwim(vStart, vEnd);

	if( EMR_Success == eRet )
	{
		GetColliderSwim()->Init(vStart, vEnd, m_pOwner->GetSize(), m_pOwner->m_fSwimXZSpeed);

		m_eCurMove			=	EMS_Swim;
		m_eNextPreMove		=	EMS_SwimStand;
		m_vPosStart			=	vStart;
		m_vDest				=	vEnd;
		m_vDir				=	vEnd - vStart;
		m_bWaitClientMsg	=	FALSE;
		m_fStartTime		=	m_Timer.GetElapse();
		m_bIsStopped		=	FALSE;

		SetFaceTo(m_vDir);
		OnStartMove();
		OnPosChange(vStart);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//-----------------------------------------------------------------------------------------------
// �������ߣ�bPatrol�����Ƿ�Ѳ�ߣ�bCheck�����Ƿ���·����
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartCreatureWalk(const Vector3& vEnd, EMoveState eState, BOOL bCheck)
{
	EMoveRet eRet = IsCanCreatureWalk(vEnd, eState, bCheck);

	if( EMR_Success == eRet )
	{
		FLOAT fRealSpeed = GetCreatureMoveStateSpeed(eState);

		Creature* pCreature = static_cast<Creature*>(m_pOwner);

		if( !pCreature->NeedCollide() )		// ����ײ��
		{
			GetColliderTileWalk()->Init(m_vPos, vEnd, fRealSpeed);
		}
		else
		{
			GetColliderNPCWalk()->Init(m_vPos, vEnd, m_pOwner->GetSize(), fRealSpeed, (bool)bCheck);
		}

		m_eCurMove			=	eState;
		m_eNextPreMove		=	EMS_Stand;
		m_vPosStart			=	m_vPos;
		m_vDest				=	vEnd;
		m_vDir				=	vEnd - m_vPos;
		m_bWaitClientMsg	=	FALSE;
		m_fStartTime		=	m_Timer.GetElapse();
		m_bIsStopped		=	FALSE;

		SetFaceTo(m_vDir);
		OnStartMove();
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StartHitFly(const Vector3& vDir)
{
	return MoveData::EMR_Success;
}

//-----------------------------------------------------------------------------------------------
// ����ֹͣ�ƶ�
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StopRoleMove(const Vector3& vPos)
{
	EMoveRet eRet = IsCanRoleStop(vPos);

	if( EMR_Success == eRet )
	{
		m_eCurMove			=		EMS_Stand;
		m_eNextPreMove		=		EMS_Stand;
		m_vPosStart			=		vPos;
		m_vDest				=		vPos;
		m_bWaitClientMsg	=		FALSE;
		m_bIsStopped		=		FALSE;

		OnPosChange(vPos);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//-----------------------------------------------------------------------------------------------
// ֹͣ��Ӿ
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StopRoleSwim(const Vector3& vPos)
{
	EMoveRet eRet = IsCanRoleStop(vPos);

	if( EMR_Success == eRet )
	{
		m_eCurMove			=		EMS_SwimStand;
		m_eNextPreMove		=		EMS_SwimStand;
		m_vPosStart			=		vPos;
		m_vDest				=		vPos;
		m_bWaitClientMsg	=		FALSE;
		m_bIsStopped		=		FALSE;

		OnPosChange(vPos);
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return eRet;
}

//-----------------------------------------------------------------------------------------------
// ������ֹͣ�ƶ���һ�����ڼ��ٵ������������ڲ������ֹͣ�ƶ���״̬����ֹͣ
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StopMove(BOOL bSend)
{
	// ����ֹͣ���ƶ�
	if( EMS_Drop	==	m_eCurMove	||
		EMS_VDrop	==	m_eCurMove	||
		EMS_Slide	==	m_eCurMove	||
		EMS_Jump	==	m_eCurMove )
		return EMR_Conflict;

	// ��������վ����Ư��״̬
	if( EMS_Stand == m_eCurMove || EMS_SwimStand == m_eCurMove )
		return EMR_Success;

	// �������ƶ�������Ӿ�ֱ��л���վ����Ư��
	if( EMS_Swim == m_eCurMove )
	{
		m_eCurMove			=		EMS_SwimStand;
		m_eNextPreMove		=		EMS_SwimStand;
	}
	else if( EMS_Walk			==	m_eCurMove ||
		EMS_CreaturePatrol	==	m_eCurMove ||
		EMS_CreatureWalk	==	m_eCurMove ||
		EMS_CreatureFlee	==	m_eCurMove )
	{
		m_eCurMove			=		EMS_Stand;
		m_eCurMove			=		EMS_Stand;
	}

	m_vPosStart			=		m_vPos;
	m_vDest				=		m_vPos;
	m_bWaitClientMsg	=		FALSE;
	m_bIsStopped		=		TRUE;

	// ͬ������Χ���
	if( bSend )
	{
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return EMR_Success;
}

//-----------------------------------------------------------------------------------------------
// ֹͣ�ƶ���������һ���µķ���
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StopMove(const Vector3& vNewFace, BOOL bSend)
{
	// ���һ�³���
	if( IsFaceInvalid(vNewFace) )
	{
		ILOG->Write(_T("face invalid when try to rotate, face=<%f, %f, %f>\r\n"),
					vNewFace.x, vNewFace.y, vNewFace.z);
		return EMR_Invalid;
	}

	// ����ֹͣ���ƶ�
	if( EMS_Drop	==	m_eCurMove	||
		EMS_VDrop	==	m_eCurMove	||
		EMS_Slide	==	m_eCurMove	||
		EMS_Jump	==	m_eCurMove )
		return EMR_Conflict;

	// ��������վ����Ư��״̬
	if( EMS_Stand == m_eCurMove || EMS_SwimStand == m_eCurMove )
		return EMR_Success;

	// �������ƶ�������Ӿ�ֱ��л���վ����Ư��
	if( EMS_Swim == m_eCurMove )
	{
		m_eCurMove			=		EMS_SwimStand;
		m_eNextPreMove		=		EMS_SwimStand;
	}
	else if( EMS_Walk			==	m_eCurMove ||
		EMS_CreaturePatrol	==	m_eCurMove ||
		EMS_CreatureWalk	==	m_eCurMove ||
		EMS_CreatureFlee	==	m_eCurMove )
	{
		m_eCurMove			=		EMS_Stand;
		m_eCurMove			=		EMS_Stand;
	}

	m_vPosStart			=		m_vPos;
	m_vDest				=		m_vPos;
	m_bWaitClientMsg	=		FALSE;
	m_bIsStopped		=		TRUE;

	SetFaceTo(vNewFace);

	// ͬ������Χ���
	if( bSend )
	{
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}

	return EMR_Success;
}

//-----------------------------------------------------------------------------------------------
// ������ǿ��ֹͣ�ƶ�
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::StopMoveForce(BOOL bSend)
{
	// ��������վ����Ư��״̬
	if( EMS_Stand == m_eCurMove || EMS_SwimStand == m_eCurMove )
		return EMR_Success;

	// ��������ƶ�����Ӿ���䣬�������л�����ǰ״̬
	if( EMS_Walk			==	m_eCurMove ||
		EMS_CreaturePatrol	==	m_eCurMove ||
		EMS_CreatureWalk	==	m_eCurMove ||
		EMS_Swim			==	m_eCurMove ||
		EMS_Slide			==	m_eCurMove ||
		EMS_CreatureFlee	==	m_eCurMove )
	{
		if( EMS_Swim == m_eCurMove )
		{
			m_eCurMove			=		EMS_SwimStand;
			m_eNextPreMove		=		EMS_SwimStand;
		}
		else
		{
			m_eCurMove			=		EMS_Stand;
			m_eCurMove			=		EMS_Stand;
		}

		m_vPosStart			=		m_vPos;
		m_vDest				=		m_vPos;
		m_bWaitClientMsg	=		FALSE;
		m_bIsStopped		=		TRUE;

		// ͬ������Χ���
		if( bSend )
		{
			m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
		}

		return EMR_Success;
	}

	// ���������Ծ���������
	if( EMS_Jump		==	m_eCurMove ||
		EMS_Drop		==	m_eCurMove ||
		EMS_VDrop		==	m_eCurMove )
	{
		DropDownStandPoint(bSend);
		m_bIsStopped = TRUE;
		return EMR_Success;
	}

	return EMR_Success;
}

//-----------------------------------------------------------------------------------------------
// ˲�Ƶ�ĳ��λ��վ��
//-----------------------------------------------------------------------------------------------
VOID MoveData::ForceTeleport(const Vector3& vPos, BOOL bSend)
{
	DropDownStandPoint(vPos, bSend);
}

//-----------------------------------------------------------------------------------------------
// �����ƶ�
//-----------------------------------------------------------------------------------------------
VOID MoveData::Update()
{
	if( EMS_Stand == m_eCurMove || EMS_SwimStand == m_eCurMove )
		return;

	if( m_bWaitClientMsg )
		return;

	switch( m_eCurMove )
	{
	case EMS_Walk:
		UpdateRoleWalk();
		break;

	case EMS_Jump:
		UpdateRoleJump();
		break;

	case EMS_Drop:
		UpdateRoleDrop();
		break;

	case EMS_VDrop:
		UpdateRoleVDrop();
		break;

	case EMS_Slide:
		UpdateRoleSlide();
		break;

	case EMS_Swim:
		UpdateRoleSwim();
		break;

	case EMS_CreaturePatrol:
	case EMS_CreatureWalk:
	case EMS_CreatureFlee:
		UpdateCreatureWalk();
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// ������������
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateRoleWalk()
{
	NavCollider* pCollider = m_pOwner->GetMap()->GetNavMap()->GetCollider();
	Role* pOwnerRole = static_cast<Role*>(m_pOwner);

	// ��������
	Vector3 vOutPos;
	float fEndTime = 0.0f;
	DWORD dwCarrierID = GT_INVALID;

	ENavResult eResult = GetColliderWalk()->Update(pCollider, m_Timer.GetElapse() - m_Timer.GetDelta(), 
						m_Timer.GetDelta(), vOutPos, fEndTime, dwCarrierID, (bool)pOwnerRole->IsInRoleState(ERS_WaterWalk));
	OnPosChange(vOutPos);


	if( ENR_ToBeContinued != eResult )
	{
		// ��ǰ�ƶ�ֹͣ
		if( ENR_Arrive == eResult || ENR_Blocking == eResult || ENR_WillCarry )
		{
			m_eCurMove			=	EMS_Stand;
			m_eNextPreMove		=	EMS_Stand;
		}
		else if( ENR_WillOnWater == eResult )
		{
			m_eCurMove			=	EMS_SwimStand;
			m_eNextPreMove		=	EMS_SwimStand;
		}
		else if( ENR_WillSwim == eResult )
		{
			m_eCurMove			=	EMS_SwimStand;
			m_eNextPreMove		=	EMS_SwimStand;
		}
		else if( ENR_WillDrop == eResult )
		{
			m_eCurMove			=	EMS_Drop;
			m_eNextPreMove		=	EMS_Stand;
			m_bWaitClientMsg	=	TRUE;
		}
		else if( ENR_WillSlide == eResult )
		{
			m_eCurMove			=	EMS_Slide;
			m_eNextPreMove		=	EMS_Stand;
			m_bWaitClientMsg	=	TRUE;
		}
		else
		{
			m_eCurMove			=	EMS_Stand;
			m_eNextPreMove		=	EMS_Stand;
		}

		// ����Ƿ���blocking�Ļ������͸�Զ�����
		if( ENR_Blocking == eResult )
		{
			m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
		}
	}
}

//-----------------------------------------------------------------------------------------------
// ����������Ծ
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateRoleJump()
{
	NavCollider* pCollider = m_pOwner->GetMap()->GetNavMap()->GetCollider();
	Role* pOwnerRole = static_cast<Role*>(m_pOwner);

	// ��������
	Vector3 vOutPos;
	FLOAT fEndTime = 0.0f;
	DWORD dwCarrierID = GT_INVALID;
	FLOAT fCDCarrierTime = 0.0f;

	ENavResult eResult = GetColliderJump()->Update(pCollider, m_Timer.GetElapse() - m_Timer.GetDelta(), 
						m_Timer.GetDelta(), NULL, vOutPos, fEndTime, dwCarrierID, fCDCarrierTime, (bool)pOwnerRole->IsInRoleState(ERS_WaterWalk));
	OnPosChange(vOutPos);

	if( ENR_ToBeContinued != eResult )
	{
		if( ENR_Landed == eResult || ENR_WillCarry == eResult )
		{
// 			Role* pOwnerRole = static_cast<Role*>(m_pOwner);
// 			pOwnerRole->GetPetPocket()->CalledPetLeavePocket();

			m_eCurMove			=	EMS_Stand;
			m_eNextPreMove		=	EMS_Stand;
		}
		else if( ENR_WillSwim == eResult )
		{
			m_eCurMove			=	EMS_SwimStand;
			m_eNextPreMove		=	EMS_SwimStand;
		}
		else if( ENR_WillOnWater == eResult  )
		{
			m_eCurMove			=	EMS_SwimStand;
			m_eNextPreMove		=	EMS_SwimStand;
		}
		else if( ENR_WillVDrop == eResult )
		{
			m_eCurMove			=	EMS_VDrop;
			m_eNextPreMove		=	EMS_Stand;
			m_bWaitClientMsg	=	TRUE;
		}
		else if( ENR_WillSlide == eResult )
		{
			m_eCurMove			=	EMS_Slide;
			m_eNextPreMove		=	EMS_Stand;
			m_bWaitClientMsg	=	TRUE;
		}
		else
		{
			m_eCurMove			=	EMS_Stand;
			m_eNextPreMove		=	EMS_Stand;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// �����������
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateRoleDrop()
{
	UpdateRoleJump();
}

//-----------------------------------------------------------------------------------------------
// �������ﴹֱ����
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateRoleVDrop()
{
	UpdateRoleJump();
}

//-----------------------------------------------------------------------------------------------
// �������ﻬ��
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateRoleSlide()
{
	NavCollider* pCollider = m_pOwner->GetMap()->GetNavMap()->GetCollider();
	Role* pOwnerRole = static_cast<Role*>(m_pOwner);

	// ��������
	Vector3 vOutPos;
	FLOAT fEndTime = 0.0f;

	ENavResult eResult = GetColliderSlide()->Update(pCollider, m_Timer.GetElapse() - m_Timer.GetDelta(), 
						m_Timer.GetDelta(), vOutPos, fEndTime, (bool)pOwnerRole->IsInRoleState(ERS_WaterWalk));
	OnPosChange(vOutPos);

	// ��鷵��ֵ
	if( ENR_ToBeContinued != eResult )
	{
		if( ENR_Blocking == eResult )
		{
			m_eCurMove		=	EMS_Stand;
			m_eNextPreMove	=	EMS_Stand;
		}
		else if( ENR_WillSwim == eResult )
		{
			m_eCurMove		=	EMS_SwimStand;
			m_eNextPreMove	=	EMS_SwimStand;
		}
		else if( ENR_WillOnWater == eResult )
		{
			m_eCurMove		=	EMS_SwimStand;
			m_eNextPreMove	=	EMS_SwimStand;
		}
		else
		{
			m_eCurMove		=	EMS_Stand;
			m_eNextPreMove	=	EMS_Stand;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// ����������Ӿ
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateRoleSwim()
{
	NavCollider* pCollider = m_pOwner->GetMap()->GetNavMap()->GetCollider();
	Role* pOwnerRole = static_cast<Role*>(m_pOwner);

	// ��������
	Vector3 vOutPos;
	FLOAT fEndTime = 0.0f;
	DWORD dwCarrierID = GT_INVALID;

	ENavResult eResult = GetColliderSwim()->Update(pCollider, m_Timer.GetElapse() - m_Timer.GetDelta(), 
							m_Timer.GetDelta(), vOutPos, fEndTime, (bool)pOwnerRole->IsInRoleState(ERS_WaterWalk));
	OnPosChange(vOutPos);

	// ��ⷵ��ֵ
	if( ENR_ToBeContinued != eResult )
	{
		if( ENR_Arrive == eResult || ENR_Blocking == eResult )
		{
			m_eCurMove		=	EMS_SwimStand;
			m_eNextPreMove	=	EMS_SwimStand;
		}
		else if( ENR_SwimLanded == eResult )
		{
// 			Role* pOwnerRole = static_cast<Role*>(m_pOwner);
// 			pOwnerRole->GetPetPocket()->CalledPetLeavePocket();

			m_eCurMove		=	EMS_Stand;
			m_eNextPreMove	=	EMS_Stand;
		}
		else
		{
			m_eCurMove		=	EMS_Stand;
			m_eNextPreMove	=	EMS_Stand;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// ���¹�������
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateCreatureWalk()
{
	Creature* pCreature = static_cast<Creature*>(m_pOwner);

	// ��������
	Vector3 vOutPos;
	FLOAT fEndTime = 0.0f;
	DWORD dwCarrierID = GT_INVALID;

	ENavResult eResult = ENR_ToBeContinued;

	// ����ײ��
	if( !pCreature->NeedCollide() )
	{
		eResult = GetColliderTileWalk()->Update(pCreature->GetMap(), vOutPos);
	}
	// ��ײ��
	else
	{
		NavCollider* pCollider = m_pOwner->GetMap()->GetNavMap()->GetCollider();
		eResult = GetColliderNPCWalk()->Update(pCollider, m_Timer.GetElapse() - m_Timer.GetDelta(), 
					m_Timer.GetDelta(), vOutPos, fEndTime, dwCarrierID);
	}
	OnPosChange(vOutPos);

	// ��鷵��ֵ
	if( ENR_ToBeContinued != eResult )
	{
		if( ENR_Arrive == eResult || ENR_Blocking == eResult || ENR_WillCarry == eResult )
		{
			m_eCurMove		=	EMS_Stand;
			m_eNextPreMove	=	EMS_Stand;
		}
		else
		{
			m_eCurMove		=	EMS_Stand;
			m_eNextPreMove	=	EMS_Stand;
		}
	}

	//m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
}

//-----------------------------------------------------------------------------------------------
// ���±�����
//-----------------------------------------------------------------------------------------------
VOID MoveData::UpdateHitFly()
{

}


//-----------------------------------------------------------------------------------------------
// ���ݵ�ǰ�������£��ҵ�һ����ײλ��վ�������ܻᵽ��Ӿ
//------------------------------------------------------------------------------------------------
VOID MoveData::DropDownStandPoint(BOOL bSend)
{
	AABBox box = m_pOwner->GetAABBox();
	m_vPos.y = m_pOwner->GetMap()->GetGroundAndModelHeight(box);


	BOOL bWaterWalk = FALSE;	// �Ƿ�ˮ������

	if( m_pOwner->IsRole() )
	{
		Role* pRole = static_cast<Role*>(m_pOwner);
		bWaterWalk = pRole->IsInRoleState(ERS_WaterWalk);
	}

	BOOL bOnWater = FALSE;
	FLOAT fY = 0.0f;
	if( bWaterWalk )
	{
		bOnWater = m_pOwner->GetMap()->IfWillOnWater(m_vPos, fY);
	}
	else
	{
		bOnWater = m_pOwner->GetMap()->IfWillSwim(m_vPos, m_pOwner->GetSize().y, fY);
	}

	if( bOnWater )
	{
		m_vPos.y = fY;	// ����һ������
		// Ҫ��Ӿ
		m_eCurMove		=	EMS_SwimStand;
		m_eNextPreMove	=	EMS_SwimStand;
	}
	else
	{
		// վ��
		m_eCurMove		=	EMS_Stand;
		m_eNextPreMove	=	EMS_Stand;
	}

	m_vPosStart			=		m_vPos;
	m_vDest				=		m_vPos;
	m_bWaitClientMsg	=		FALSE;

	// ͬ������Χ���
	if( bSend )
	{
		m_pOwner->GetMap()->SyncMovementToBigVisTile(m_pOwner);
	}
}

//-------------------------------------------------------------------------------------
// ����һ�����������£��ҵ�һ����ײλ��վ�������ܻᵽ��Ӿ
//-------------------------------------------------------------------------------------
VOID MoveData::DropDownStandPoint(const Vector3& vPos, BOOL bSend)
{
	// ���һ������
	if( IsPosInvalid(vPos) )
	{
		ILOG->Write(_T("pos invalid when try to drop stand point, pos=<%f, %f, %f>\r\n"),
			vPos.x, vPos.y, vPos.z);
		return;
	}

	StopMoveForce(FALSE);
	OnPosChange(vPos);

	DropDownStandPoint(bSend);
}

//-------------------------------------------------------------------------------------
// λ�øı�
//-------------------------------------------------------------------------------------
VOID MoveData::OnPosChange(const Vector3& vNewPos)
{
	// ���������⺯��
	if( IsPosInvalid(vNewPos) )
	{
		ILOG->Write(_T("Invalid Pos, Current movestate: %d\r\n"), m_eCurMove);
	}

	// ��������
	Vector3 vPos = vNewPos;
	m_pOwner->GetMap()->FixPos(vPos);

	m_vPos = vPos;	// ����Ϊ������

	INT nNewVisIndex = m_pOwner->GetMap()->WorldPosToVisIndex(m_vPos);
	if( m_nVisTileIndex != nNewVisIndex )
	{
		m_pOwner->GetMap()->SyncChangeVisTile(m_pOwner, m_nVisTileIndex, nNewVisIndex);
	}

	// ���������λ����ͬ������Χ���
	if( m_pOwner->IsInStateInvisible() )
	{
		m_pOwner->GetMap()->UpdateLurk2BigVisTileRole(m_pOwner);
	}

	// �������ң������ͼ���򣬼�����Χ�������ͬ������
	if( m_pOwner->IsRole() )
	{
		Role* pRole = static_cast<Role*>(m_pOwner);
		pRole->CheckMapArea();

		pRole->GetMap()->UpdateBigVisTileLurkUnit2Role(pRole);
	}
}

//-------------------------------------------------------------------------------------
// ��ʼ�ƶ�
//-------------------------------------------------------------------------------------
VOID MoveData::OnStartMove()
{
	if( EMS_Stand == m_eCurMove || EMS_SwimStand == m_eCurMove )
		return;

	// ��⼼�����ִ��
	m_pOwner->GetCombatHandler().InterruptPrepare(CombatHandler::EIT_Move, FALSE);

	// ��⼼��ʩ�Ŵ��
	m_pOwner->GetCombatHandler().InterruptOperate(CombatHandler::EIT_Move, m_eCurMove);

	// ���buff���
	m_pOwner->OnInterruptBuffEvent(EBIF_Move);
}

//-------------------------------------------------------------------------------------
// ��ͼ�Ƿ�Ϸ�
//-------------------------------------------------------------------------------------
BOOL MoveData::IsMapValid()
{
	// �����ǰû�е�ͼ���ߵ�ͼû�е���ͼ����ֱ�ӷ���
	Map* pMap = m_pOwner->GetMap();
	if( FALSE == P_VALID(pMap) ) return FALSE;

	NavMap* pNav = pMap->GetNavMap();
	if( FALSE == P_VALID(pNav) ) return FALSE;

	return TRUE;
}

//-------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------
BOOL MoveData::IsInValidDistance(const Vector3& vStart)
{
	FLOAT fDistanceSQ =  Vec3DistSq(m_vPos, vStart);
	if( fDistanceSQ > g_world.GetLaxCheatDistanceSQ() )
	{
		return FALSE;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------
// ����Ƿ��������
//-------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanRoleWalk(const Vector3& vStart, const Vector3& vEnd)
{
	// ���������ң�����
	if( !m_pOwner->IsRole() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	// ���һ������
	if( IsPosInvalid(vStart) || IsPosInvalid(vEnd) )
	{
		ILOG->Write(_T("client pos invalid when try to walk, start=<%f, %f, %f>, end=<%f, %f, %f>\r\n"),
			vStart.x, vStart.y, vStart.z, vEnd.x, vEnd.y, vEnd.z);
		return EMR_Invalid;
	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vStart) || !m_pOwner->GetMap()->IsPosValid(vEnd) )
		return EMR_Invalid;


	// ���һ�¾���
	if( !IsInValidDistance(vStart) )
	{
		return EMR_Invalid;
	}

	// ���һ���ٶ�
	if( m_pOwner->GetXZSpeed() <= 0.0f )
	{
		ILOG->Write(_T("find a role which have a xz speed less than 0, ID: %u, Speed: %f!!!!!!!!!\r\n"), 
			m_pOwner->GetID(), m_pOwner->GetXZSpeed());
		return EMR_SelfLimit;
	}

	// ��⵱ǰ�Ƿ��ڲ����ƶ���״̬
	if( m_pOwner->IsInStateCantMove() )
	{
		return EMR_SelfLimit;
	}

	return EMR_Success;
}

//----------------------------------------------------------------------------------------
// ����Ƿ������Ծ
//----------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanRoleJump(const Vector3& vStart, const Vector3& vDir)
{
	// ���������ң�����
	if( !m_pOwner->IsRole() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	// ���һ������ͳ���
	if( IsPosInvalid(vStart) || IsFaceInvalid(vDir) )
	{
		ILOG->Write(_T("client pos invalid when try to jump, start=<%f, %f, %f>, dir=<%f, %f, %f>\r\n"),
			vStart.x, vStart.y, vStart.z, vDir.x, vDir.y, vDir.z);
		return EMR_Invalid;
	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vStart) )
		return EMR_Invalid;

	// ���һ�¾���
	if( !IsInValidDistance(vStart) )
	{
		return EMR_Invalid;
	}

	// ���һ���ٶ�
	if( m_pOwner->GetXZSpeed() <= 0.0f || m_pOwner->m_fYSpeed <= 0.0f )
	{
		IMSG(_T("find a role whose xz speed or y speed less than 0, ID: %u, xzspeed: %f, yspeed: %f\r\n"), 
			m_pOwner->GetID(), m_pOwner->GetXZSpeed(), m_pOwner->m_fYSpeed);
		return EMR_SelfLimit;
	}

	// ��⵱ǰ�Ƿ��ڲ����ƶ���״̬
	if( m_pOwner->IsInStateCantMove() )
	{
		return EMR_SelfLimit;
	}
// 	Role* pRole = dynamic_cast<Role*>(m_pOwner);
// 	if (P_VALID(pRole) && pRole->IsInRoleState(ERS_Mount))
// 	{
// 		return EMR_SelfLimit;
// 	}

	return EMR_Success;
}

//----------------------------------------------------------------------------------------
// ����Ƿ���Ե���
//----------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanRoleDrop(const Vector3& vStart, const Vector3& vDir)
{
	// ���������ң�����
	if( !m_pOwner->IsRole() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	// ���һ������ͳ���
	if( IsPosInvalid(vStart) || IsFaceInvalid(vDir) )
	{
		ILOG->Write(_T("client pos invalid when try to drop, start=<%f, %f, %f>, dir=<%f, %f, %f>\r\n"),
			vStart.x, vStart.y, vStart.z, vDir.x, vDir.y, vDir.z);
		return EMR_Invalid;
	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vStart) )
		return EMR_Invalid;

	// ���һ�¾���
	if( !IsInValidDistance(vStart) )
	{
		return EMR_Invalid;
	}

	return EMR_Success;
}

//-----------------------------------------------------------------------------------------
// �Ƿ���Դ�ֱ����
//-----------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanRoleVDrop(const Vector3& vStart)
{
	// ���������ң�����
	if( !m_pOwner->IsRole() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	// ���һ������
	if( IsPosInvalid(vStart) )
	{
		ILOG->Write(_T("client pos invalid when try to vDrop, start=<%f, %f, %f>\r\n"),
			vStart.x, vStart.y, vStart.z);
		return EMR_Invalid;
	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vStart) )
		return EMR_Invalid;

	// ���һ�¾���
	if( !IsInValidDistance(vStart) )
	{
		return EMR_Invalid;
	}

	return EMR_Success;
}

//------------------------------------------------------------------------------------------
// �Ƿ���Ի���
//------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanRoleSlide(const Vector3& vStart)
{
	// ���������ң�����
	if( !m_pOwner->IsRole() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	// ���һ������
	if( IsPosInvalid(vStart) )
	{
		ILOG->Write(_T("client pos invalid when try to vdrop, start=<%f, %f, %f>\r\n"),
			vStart.x, vStart.y, vStart.z);
		return EMR_Invalid;
	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vStart) )
		return EMR_Invalid;

	// ���һ�¾���
	if( !IsInValidDistance(vStart) )
	{
		return EMR_Invalid;
	}

	return EMR_Success;
}

//---------------------------------------------------------------------------------------------
// �Ƿ������Ӿ
//---------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanRoleSwim(const Vector3& vStart, const Vector3& vEnd)
{
	// ���������ң�����
	if( !m_pOwner->IsRole() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	if( IsPosInvalid(vStart) || IsPosInvalid(vEnd) )
	{
		ILOG->Write(_T("client pos invalid when try to swim, start=<%f, %f, %f>, end=<%f, %f, %f>\r\n"),
			vStart.x, vStart.y, vStart.z, vEnd.x, vEnd.y, vEnd.z);
		return EMR_Invalid;

	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vStart) || !m_pOwner->GetMap()->IsPosValid(vEnd) )
		return EMR_Invalid;

	// ���һ�¾���
	if( !IsInValidDistance(vStart) )
	{
		return EMR_Invalid;
	}

	// ���һ����Ӿ���ٶ�
	if( m_pOwner->m_fSwimXZSpeed <= 0.0f )
	{
		return EMR_SelfLimit;
	}

	// ��⵱ǰ�Ƿ��ڲ����ƶ���״̬
	if( m_pOwner->IsInStateCantMove() )
	{
		return EMR_SelfLimit;
	}

	return EMR_Success;
}

//-----------------------------------------------------------------------------------------------
// ����Ƿ����ͣ��
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanRoleStop(const Vector3& vPos)
{
	// ���������ң�����
	if( !m_pOwner->IsRole() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	// ���һ������
	if( IsPosInvalid(vPos) )
	{
		ILOG->Write(_T("client pos invalid when try to stop, pos=<%f, %f, %f>\r\n"),
			vPos.x, vPos.y, vPos.z);
		return EMR_Invalid;
	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vPos) )
		return EMR_Invalid;

	// ���һ�¾���
	if( !IsInValidDistance(vPos) )
	{
		return EMR_Invalid;
	}

	return EMR_Success;
}

//-----------------------------------------------------------------------------------------------
// �����Ƿ��������
//-----------------------------------------------------------------------------------------------
MoveData::EMoveRet MoveData::IsCanCreatureWalk(const Vector3& vEnd, EMoveState eState, BOOL bCheck, Vector3* vNearPos)
{
	// ������ǹ������
	if( !m_pOwner->IsCreature() ) return EMR_Invalid;

	// ��ͼ�Ƿ�
	if( !IsMapValid() ) return EMR_Invalid;

	// ���һ������
	if( IsPosInvalid(vEnd) )
	{
		ILOG->Write(_T("creature pos invalid when try to walk, dest=<%f, %f, %f>\r\n"),
			vEnd.x, vEnd.y, vEnd.z);
		return EMR_Invalid;
	}

	// ��ͼ�ж�����
	if( !m_pOwner->GetMap()->IsPosValid(vEnd) )
		return EMR_Invalid;

	// ���һ���ٶ�
	FLOAT fRealSpeed = GetCreatureMoveStateSpeed(eState);

	if( fRealSpeed < 0.0001f )
	{
		return EMR_SelfLimit;
	}

	// �����Ҫ���·��
	if( bCheck )
	{
		// ��⵱ǰ�Ƿ��ڲ����ƶ���״̬
		if( m_pOwner->IsInStateCantMove() )
		{
			return EMR_SelfLimit;
		}

		Creature* pCreature = static_cast<Creature*>(m_pOwner);

		// ����ײ����
		if( !pCreature->NeedCollide() )
		{
			POINT nearPos;
			if( !pCreature->GetMap()->IfCanDirectGo(m_vPos.x, m_vPos.z, vEnd.x, vEnd.z, &nearPos) )
			{
				if( P_VALID(vNearPos) )
				{
					vNearPos->x = FLOAT(nearPos.x * TILE_SCALE);
					vNearPos->z = FLOAT(nearPos.y * TILE_SCALE);
					vNearPos->y = pCreature->GetMap()->GetGroundHeight(vNearPos->x, vNearPos->z);
				}
				return EMR_NoArrive;
			}
			else
			{

			}
		}
		// ��ײ����
		else
		{
			NavCollider_NPCMove NPCMove; 
			NPCMove.Init(m_vPos, vEnd, m_pOwner->GetSize(), fRealSpeed, (bool)bCheck);

			if(ENR_Arrive != NPCMove.IfCanGo(m_pOwner->GetMap()->GetNavCollider(), m_vPos, vEnd, m_pOwner->GetSize(), vNearPos))
				return EMR_NoArrive;
		}
	}

	return EMR_Success;
}

//-------------------------------------------------------------------------------------------
// ���ݹ�����ƶ���ʽ�ĵ���ǰ�ٶ�
//-------------------------------------------------------------------------------------------
FLOAT MoveData::GetCreatureMoveStateSpeed(EMoveState eState)
{
	switch (eState)
	{
	case EMS_CreatureWalk:
		return m_pOwner->GetXZSpeed();

	case EMS_CreaturePatrol:
		return m_pOwner->GetXZSpeed() * 0.4;

	case EMS_CreatureFlee:
		return m_pOwner->GetXZSpeed() * 0.7;

	default:
		return m_pOwner->GetXZSpeed();
	}
}
