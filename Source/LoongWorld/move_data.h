//-------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: move_data.h
// author: Aslan
// actor:
// data: 2009-01-13
// last:
// brief: �ƶ��ṹ
//-------------------------------------------------------------------------------------------------
#pragma once

#include "../Cool3D/Util/Timer.h"

class Unit;
class Role;
class Creature;
class Map;

//------------------------------------------------------------------------------------------------------
// ���ӹ����ƶ�������
//------------------------------------------------------------------------------------------------------
class NavCollider_TileMove
{
public:
	NavCollider_TileMove();
	~NavCollider_TileMove();

public:
	VOID		Init(const Vector3& vStart, const Vector3& vDest, FLOAT fXZSpeed);
	ENavResult	Update(Map* pMap, Vector3& vOutPos);

private:
	Vector3			m_vStart;		// ��ʼ����
	Vector3			m_vDest;		// �յ�����
	FLOAT			m_fTotalTime;	// �ܹ���Ҫ�ƶ��೤ʱ��
	FLOAT			m_fCurTime;		// ��ǰ���˶೤ʱ��
	INT				m_nMoveTicks;	// ��Ҫ���ܵ�������
	FLOAT			m_fXZSpeed;		// xz������ƶ��ٶ�
};

class MoveData
{
public:
	//---------------------------------------------------------------------------------------------
	// �ƶ��Ľ��
	//---------------------------------------------------------------------------------------------
	enum EMoveRet
	{
		EMR_Success		=	0,		// �����ƶ�
		EMR_SelfLimit	=	1,		// �������ƣ���������ѣ�Σ�����ȵ�
		EMR_NoArrive	=	2,		// ���ܵ���
		EMR_Conflict	=	3,		// �ƶ�״̬֮���ͻ
		EMR_Invalid		=	4,		// �Ƿ�����
	};

public:
	//---------------------------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------------------------
	static BOOL		IsPosInvalid(const Vector3& vPos);
	static BOOL		IsFaceInvalid(const Vector3& vFace);

	//---------------------------------------------------------------------------------------------
	// CONSTRUCT
	//---------------------------------------------------------------------------------------------
	MoveData() : m_pOwner(NULL), m_eCurMove(EMS_Stand), m_eNextPreMove(EMS_Stand), 
				 m_nVisTileIndex(GT_INVALID), m_bWaitClientMsg(FALSE), m_bIsStopped(FALSE),
				 m_pColliderWalk(NULL), m_pColliderJump(NULL), m_pColliderSwim(NULL),
				 m_pColliderSlide(NULL), m_pColliderHitFly(NULL), m_pColliderNPCWalk(NULL), m_pColliderTileWalk(NULL) {}

	~MoveData()
	{
		SAFE_DEL(m_pColliderWalk);
		SAFE_DEL(m_pColliderJump);
		SAFE_DEL(m_pColliderSwim);
		SAFE_DEL(m_pColliderSlide);
		SAFE_DEL(m_pColliderHitFly);
		SAFE_DEL(m_pColliderNPCWalk);
		SAFE_DEL(m_pColliderTileWalk);
	}

	//-------------------------------------------------------------------------------------
	// �õ���ǰ�ƶ�״̬
	//-------------------------------------------------------------------------------------
	EMoveState	GetCurMoveState()		{ return m_eCurMove; }
	EMoveState	GetNextMoveState()		{ return m_eNextPreMove; }
	BOOL		IsStopped()				{ return m_bIsStopped; }

	//---------------------------------------------------------------------------------------------
	// ��ʼ���ƶ��ṹ����Ҫ������Һ������ʼ��ʱ���ƶ�״̬
	//---------------------------------------------------------------------------------------------
	VOID Init(Unit* pOwner, const FLOAT fX, const FLOAT fY, const FLOAT fZ, const FLOAT fFaceX, const FLOAT fFaceY, const FLOAT fFaceZ);

	//----------------------------------------------------------------------------------------------
	// �����ƶ��ṹ������Ҫ��������л���ͼ�͹�������ʱ��һ�������
	//----------------------------------------------------------------------------------------------
	VOID Reset(FLOAT fX, const FLOAT fY, const FLOAT fZ, const FLOAT fFaceX, const FLOAT fFaceY, const FLOAT fFaceZ);

	//-----------------------------------------------------------------------------------------------
	// ��ʼ�ƶ�
	//-----------------------------------------------------------------------------------------------
	EMoveRet	StartRoleWalk(const Vector3& vStart, const Vector3& vEnd);
	EMoveRet	StartRoleJump(const Vector3& vStart, const Vector3& vDir);
	EMoveRet	StartRoleDrop(const Vector3& vStart, const Vector3& vDir);
	EMoveRet	StartRoleVDrop(const Vector3& vStart);
	EMoveRet	StartRoleSlide(const Vector3& vStart);
	EMoveRet	StartRoleSwim(const Vector3& vStart, const Vector3& vEnd);
	EMoveRet	StartCreatureWalk(const Vector3& vDest, EMoveState eState, BOOL bCheck=TRUE);
	EMoveRet	StartHitFly(const Vector3& vDir);
	EMoveRet	StopRoleMove(const Vector3& vPos);
	EMoveRet	StopRoleSwim(const Vector3& vPos);

	//-----------------------------------------------------------------------------------------------
	// ֹͣ�ƶ�
	//-----------------------------------------------------------------------------------------------
	EMoveRet	StopMove(BOOL bSend=TRUE);
	EMoveRet	StopMove(const Vector3& vNewFace, BOOL bSend=TRUE);
	EMoveRet	StopMoveForce(BOOL bSend=TRUE);

	//-----------------------------------------------------------------------------------------------
	// ˲��
	//-----------------------------------------------------------------------------------------------
	VOID		ForceTeleport(const Vector3& vPos, BOOL bSend=TRUE);

	//-----------------------------------------------------------------------------------------------
	// �ı䳯��
	//-----------------------------------------------------------------------------------------------
	VOID		SetFaceTo(const Vector3& vFace);

	//-----------------------------------------------------------------------------------------------
	// �����ƶ�
	//-----------------------------------------------------------------------------------------------
	VOID		Update();
	VOID		UpdateRoleWalk();
	VOID		UpdateRoleJump();
	VOID		UpdateRoleDrop();
	VOID		UpdateRoleVDrop();
	VOID		UpdateRoleSlide();
	VOID		UpdateRoleSwim();
	VOID		UpdateCreatureWalk();
	VOID		UpdateHitFly();

	//-------------------------------------------------------------------------------------
	// ȡվ�������
	//-------------------------------------------------------------------------------------
	VOID		DropDownStandPoint(BOOL bSend=TRUE);
	VOID		DropDownStandPoint(const Vector3& vPos, BOOL bSend=TRUE);

private:
	//-------------------------------------------------------------------------------------
	// ���ּ��
	//-------------------------------------------------------------------------------------
	EMoveRet	IsCanRoleWalk(const Vector3& vStart, const Vector3& vEnd);
	EMoveRet	IsCanRoleJump(const Vector3& vStart, const Vector3& vDir);
	EMoveRet	IsCanRoleDrop(const Vector3& vStart, const Vector3& vDir);
	EMoveRet	IsCanRoleVDrop(const Vector3& vStart);
	EMoveRet	IsCanRoleSlide(const Vector3& vStart);
	EMoveRet	IsCanRoleSwim(const Vector3& vStart, const Vector3& vEnd);
	EMoveRet	IsCanRoleStop(const Vector3& vPos);
	EMoveRet	IsCanHitFly(const Vector3& vEnd);

public:
	EMoveRet	IsCanCreatureWalk(const Vector3& vEnd, EMoveState eState, BOOL bCheck, Vector3* vNearPos=NULL);
	BOOL		IsInValidDistance(const Vector3& vStart);

private:

	BOOL		IsMapValid();
	FLOAT		GetCreatureMoveStateSpeed(EMoveState eState);

private:

	//-------------------------------------------------------------------------------------
	// λ�øı�
	//-------------------------------------------------------------------------------------
	VOID		OnPosChange(const Vector3& vNewPos);

	//-------------------------------------------------------------------------------------
	// ��ʼ�ƶ�
	//-------------------------------------------------------------------------------------
	VOID		OnStartMove();


	//-------------------------------------------------------------------------------------
	// �õ����߿�����
	//-------------------------------------------------------------------------------------
	NavCollider_Move*		GetColliderWalk()		{ if( !P_VALID(m_pColliderWalk) )		m_pColliderWalk = new NavCollider_Move;			return m_pColliderWalk; }
	NavCollider_Jump*		GetColliderJump()		{ if( !P_VALID(m_pColliderJump) )		m_pColliderJump = new NavCollider_Jump;			return m_pColliderJump; }
	NavCollider_Swim*		GetColliderSwim()		{ if( !P_VALID(m_pColliderSwim) )		m_pColliderSwim = new NavCollider_Swim;			return m_pColliderSwim; }
	NavCollider_Slide*		GetColliderSlide()		{ if( !P_VALID(m_pColliderSlide) )		m_pColliderSlide = new NavCollider_Slide;		return m_pColliderSlide; }
	NavCollider_HitFly*		GetColliderHitFly()		{ if( !P_VALID(m_pColliderHitFly) )		m_pColliderHitFly = new NavCollider_HitFly;		return m_pColliderHitFly; }
	NavCollider_NPCMove*	GetColliderNPCWalk()	{ if( !P_VALID(m_pColliderNPCWalk) )	m_pColliderNPCWalk = new NavCollider_NPCMove;	return m_pColliderNPCWalk; }
	NavCollider_TileMove*	GetColliderTileWalk()	{ if( !P_VALID(m_pColliderTileWalk) )	m_pColliderTileWalk = new NavCollider_TileMove;	return m_pColliderTileWalk; }

public:
	static Timer			m_Timer;							// �ƶ���ʱ��

	Unit*					m_pOwner;							// ������
	EMoveState				m_eCurMove;							// ��ǰ�ƶ�״̬
	EMoveState				m_eNextPreMove;						// �ڵ�ǰ״̬���С����֮�󡱣�Ԥ��Ҫ�л�����״̬
	Vector3					m_vPos;								// ��ǰλ��
	Vector3					m_vPosStart;						// ��ǰ״̬��ʼʱ�ĳ�ʼλ��
	Vector3					m_vDir;								// ��ǰ�ƶ�����
	Vector3					m_vDest;							// Ŀ���
	Vector3					m_vFace;							// ����

	INT						m_nVisTileIndex;					// ��ǰ�����Ŀ��ӵ�ש����
	BOOL					m_bWaitClientMsg;					// �����ǰ״̬�������һ��״̬����վ�������ֶ����������Ƿ��ڵȴ��ͻ�����Ϣ

	FLOAT					m_fStartTime;						// �ƶ���ʼ��ʱ��

	BOOL					m_bIsStopped;						// �����ͣ��

	NavCollider_Move*		m_pColliderWalk;					// ���߿�����
	NavCollider_Jump*		m_pColliderJump;					// ��Ծ������ʹ�ֱ���������
	NavCollider_Swim*		m_pColliderSwim;					// ��Ӿ������
	NavCollider_Slide*		m_pColliderSlide;					// ���������
	NavCollider_HitFly*		m_pColliderHitFly;					// ���ɿ�����
	NavCollider_NPCMove*	m_pColliderNPCWalk;					// NPC�ƶ�������
	NavCollider_TileMove*	m_pColliderTileWalk;				// �����ƶ�������
};

//-------------------------------------------------------------------------------------------
// �ı�����
//-------------------------------------------------------------------------------------------
inline VOID MoveData::SetFaceTo(const Vector3& vFace)
{
	if( IsFaceInvalid(vFace) ) return;
	if( abs(vFace.x) < 0.001f && abs(vFace.z) < 0.001f ) return;

	m_vFace = vFace;
	m_vFace.y = 0;
}

