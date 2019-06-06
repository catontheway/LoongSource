//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet.cpp
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ����ʵ��
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "pet.h"

#include "pet_soul.h"
#include "pet_tracker.h"
#include "role.h"
#include "pet_define.h"
//----------------------------------------------------------------------------------------------------
// �����dumyԭ��
//----------------------------------------------------------------------------------------------------
const DWORD TYPE_ID_PET = 9999997;

//----------------------------------------------------------------------------------------------------
// ��������ʵ��
//----------------------------------------------------------------------------------------------------
Pet* Pet::Create( DWORD dwPetID, PetSoul* pSoul )
{
	MTRANS_ELSE_RET(pMaster, pSoul->GetMaster(), Role, NULL);
	Pet* pNew = new Pet(dwPetID, pMaster->GetMapID(), pMaster->GetID(), pMaster->GetMoveData().m_vPos, pMaster->GetMoveData().m_vFace, pSoul);
	pNew->Init(pMaster);
	return pNew;
}

//----------------------------------------------------------------------------------------------------
// ɾ������ʵ��
//----------------------------------------------------------------------------------------------------
VOID Pet::Delete(Pet* pToDel)
{
	SAFE_DEL(pToDel);
}

//----------------------------------------------------------------------------------------------------
// ���캯��
//----------------------------------------------------------------------------------------------------
Pet::Pet( DWORD dwID, DWORD dwMapID, DWORD dwMasterID, Vector3& vPos, Vector3& vFace, PetSoul* pSoul ) 
:Creature(dwID, dwMapID, vPos, vFace, ECST_ByRole, dwMasterID, FALSE),m_pSoul(pSoul)
{
	m_pTracker = new PetTracker(pSoul, this);
}

//----------------------------------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------------------------------
Pet::~Pet()
{
	SAFE_DEL(m_pTracker);
}

//----------------------------------------------------------------------------------------------------
// ���������
//----------------------------------------------------------------------------------------------------
BOOL Pet::IntegrateSoul(PetSoul* pSoul)
{
	// precondition
	if (!P_VALID(pSoul) ||
		GetID() != pSoul->GetID())
	{
		return FALSE;
	}

	//Init PetSoul of this Object
	m_pSoul = pSoul;

	//IntegrateInPet PetSoul with this Pet
	if (!m_pSoul->IntegrateInPet(this))
	{
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// ��ʵ�����
//----------------------------------------------------------------------------------------------------
VOID Pet::DetachSoul()
{
	//DetachFromPet PetSoul
	m_pSoul->DetachFromPet();

	//Reset PetSoul to NULL
	m_pSoul = NULL;

}

//----------------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------------
VOID Pet::Update()
{
	m_pTracker->Update();

}

//----------------------------------------------------------------------------------------------------
// ���Ըı�
//----------------------------------------------------------------------------------------------------
VOID Pet::OnAttChange(INT nIndex)
{
	switch(nIndex)
	{
	case ERA_Speed_XZ:
		m_fXZSpeed = X_DEF_XZ_SPEED * (m_nAtt[ERA_Speed_XZ] / 10000.0f);
		m_MoveData.StopMove();
		break;
	}

}

//----------------------------------------------------------------------------------------------------
// ��ʼ��
//----------------------------------------------------------------------------------------------------
BOOL Pet::Init(Role* pMaster)
{
	const tagCreatureProto* pProto = g_attRes.GetCreatureProto(TYPE_ID_PET);
	ASSERT(P_VALID(pProto));

	Creature::Init(pProto, NULL);

	ASSERT(P_VALID(GetSoul()));
	SetSize(GetSoul()->GetProto()->vSize * GetSoul()->GetProto()->fScale);
	m_fXZSpeed = pMaster->GetXZSpeed();
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// ��ȡtypeid
//----------------------------------------------------------------------------------------------------
DWORD Pet::GetTypeID()
{
	return m_pSoul->GetProtoID();
}

//----------------------------------------------------------------------------------------------------
// ��ø���ģ��
//----------------------------------------------------------------------------------------------------
BYTE Pet::GetPetState()
{
	return GetSoul()->GetPetAtt().GetState();
}

//----------------------------------------------------------------------------------------------------
// �������
//----------------------------------------------------------------------------------------------------
Role* Pet::GetMaster()
{
	return GetSoul()->GetMaster();
}

VOID Pet::Teleport( Unit* pTarget )
{
	Follow(pTarget);

	if (m_pTracker->NeedPutdown(pTarget->GetCurPos(), GetCurPos()))
	{
		m_pTracker->PutDownBack(this, pTarget->GetCurPos(), pTarget->GetFaceTo());
	}
}

VOID Pet::Follow( Unit* pTarget )
{
	m_pTracker->SetTarget(pTarget->GetID());
}