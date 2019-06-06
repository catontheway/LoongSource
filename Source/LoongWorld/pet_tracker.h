//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_tracker.h
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ���������
//-------------------------------------------------------------------------------------------------------
#pragma once


class Unit;
class Role;
class Creature;
class Pet;
class MoveData;
class PetSoul;

//----------------------------------------------------------------------------------------------------
// ���������
//----------------------------------------------------------------------------------------------------
class PetTracker
{
	//----------------------------------------------------------------------------------------------------
	// һЩ����
	//----------------------------------------------------------------------------------------------------
	static const INT		MAX_NEED_MOVE_LEN	= 50 * 3;
	static const INT		MIN_NEED_MOVE_LEN	= 50 * 2;
	static const INT		NEED_TRANS_LEN		= 50 * 40;
	static const INT		MAX_TRY_TIME		= 100;
	static const INT		MAX_TRANS_TICKS		= TICK_PER_SECOND * 5;

public:
	//----------------------------------------------------------------------------------------------------
	// ����������
	//----------------------------------------------------------------------------------------------------
	PetTracker(PetSoul* pSoul, Pet* pPet);
	~PetTracker(){}

	//----------------------------------------------------------------------------------------------------
	// һЩget
	//----------------------------------------------------------------------------------------------------
	Role*		GetMaster()	const;
	Pet*		GetPet() const			{	return m_pPet;		}
	Unit*		GetTarget() const		{	return GetUnit(m_dwTargetID);		}

	//----------------------------------------------------------------------------------------------------
	// һЩPet�õ��ķ���
	//----------------------------------------------------------------------------------------------------
	VOID		SetTarget(DWORD dwTargetID);
	BOOL		NeedPutdown(const Vector3 &vMasterPos, const Vector3 &vPet);
	BOOL		PutDownBack(Pet* pPet, const Vector3 &vMasterPos, const Vector3 &vMasterFace);
	
	//----------------------------------------------------------------------------------------------------
	// ���£����Ƴ�����棩
	//----------------------------------------------------------------------------------------------------
	VOID		Update();

private:
	//----------------------------------------------------------------------------------------------------
	// һЩ�õ��ķ���
	//----------------------------------------------------------------------------------------------------
	Vector3		FindTargetDest( Unit* pTarget );
	BOOL		NeedMove(const Vector3 &vMasterPos, const Vector3 &vPet);
	Vector3		GetNearPos(const Vector3 &vMasterPos, const Vector3 &vMasterFace, FLOAT fMaxRange, FLOAT fMinRange);
	BOOL		TryGoto(Pet* pPet, const Vector3 &vDstPos);
	VOID		UpdatePetSpeed(Unit* pTarget);
	Unit*		GetUnit(DWORD dwID) const;

private:
	DWORD		m_dwTargetID;	// Ŀ��id

	Pet*		m_pPet;			// ����ʵ��
	PetSoul*	m_pSoul;		
	INT			m_nPutBackTicks;// ˲�Ƽ�ʱ
};

