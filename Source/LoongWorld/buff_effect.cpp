//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: buff_effect.cpp
// author: Aslan
// actor:
// data: 2008-11-26
// last:
// brief: ״̬����Ч��
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_loot.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "unit.h"
#include "buff_effect.h"
#include "role.h"
#include "creature.h"
#include "loot_mgr.h"
#include "script_mgr.h"
#include "pet_pocket.h"
#include "pet_soul.h"
#include "creature_ai.h"

BUFFEFFECTRUTINE BuffEffect::m_Effect[EBET_End];

VOID BuffEffect::Init()
{
	RegisterBuffEffectRutine();
}

//--------------------------------------------------------------------------------
// ��ʼ��
//--------------------------------------------------------------------------------
VOID BuffEffect::RegisterBuffEffectRutine()
{
	m_Effect[EBET_Null]					=	&BuffEffect::BuffEffectNull;
	m_Effect[EBET_Teleport]				=	&BuffEffect::BuffEffectTeleport;
	m_Effect[EBET_InterruptSpell]		=	&BuffEffect::BuffEffectInterruptSpell;
	m_Effect[EBET_Dizzy]				=	&BuffEffect::BuffEffectDizzy;
	m_Effect[EBET_Repel]				=	&BuffEffect::BuffEffectRepel;
	m_Effect[EBET_Assault]				=	&BuffEffect::BuffEffectAssault;
	m_Effect[EBET_NoSkill]				=	&BuffEffect::BuffEffectNoSkill;
	m_Effect[EBET_Spor]					=	&BuffEffect::BuffEffectSpor;
	m_Effect[EBET_Tie]					=	&BuffEffect::BuffEffectTie;
	m_Effect[EBET_Dispel]				=	&BuffEffect::BuffEffectDispel;
	m_Effect[EBET_Cancel]				=	&BuffEffect::BuffEffectCancel;
	m_Effect[EBET_Invincible]			=	&BuffEffect::BuffEffectInvincible;
	m_Effect[EBET_InstantDie]			=	&BuffEffect::BuffEffectInstantDie;
	m_Effect[EBET_Pull]					=	&BuffEffect::BuffEffectPull;
	m_Effect[EBET_ReboundDmg]			=	&BuffEffect::BuffEffectNull;
	m_Effect[EBET_AbsorbDmg]			=	&BuffEffect::BuffEffectNull;
	m_Effect[EBET_TransmitDmg]			=	&BuffEffect::BuffEffectNull;
	m_Effect[EBET_HPDrain]				=	&BuffEffect::BuffEffectNull;
	m_Effect[EBET_MPDrain]				=	&BuffEffect::BuffEffectNull;
	m_Effect[EBET_HPTransfer]			=	&BuffEffect::BuffEffectHPTransfer;
	m_Effect[EBET_MPTransfer]			=	&BuffEffect::BuffEffectMPTransfer;
	m_Effect[EBET_Revive]				=	&BuffEffect::BuffEffectRevive;
	m_Effect[EBET_InstantCD]			=	&BuffEffect::BuffEffectInstantCD;
	m_Effect[EBET_Lurk]					=	&BuffEffect::BuffEffectLurk;
	m_Effect[EBET_Fly]					=	&BuffEffect::BuffEffectFly;
	m_Effect[EBET_ChangeEnmity]			=	&BuffEffect::BuffEffectChangeEnmity;
	m_Effect[EBET_Transmit]				=	&BuffEffect::BuffEffectTransmit;
	m_Effect[EBET_Gather]				=	&BuffEffect::BuffEffectGather;
	m_Effect[EBET_DisArm]				=	&BuffEffect::BuffEffectDisArm;
	m_Effect[EBET_ExchangePos]			=	&BuffEffect::BuffEffectExchangePos;
	m_Effect[EBET_Explode]				=	&BuffEffect::BuffEffectExplode;
	m_Effect[EBET_Funk]					=	&BuffEffect::BuffEffectFunk;
	m_Effect[EBET_Pursue]				=	&BuffEffect::BuffEffectPursue;
	m_Effect[EBET_NoPrepare]			=	&BuffEffect::BuffEffectNoPrepare;
	m_Effect[EBET_OnWater]				=	&BuffEffect::BuffEffectOnWater;
	m_Effect[EBET_MoveHPDmg]			=	&BuffEffect::BuffEffectMoveHPDmg;
	m_Effect[EBET_IgnoreArmor]			=	&BuffEffect::BuffEffectIgnoreArmor;
	m_Effect[EBET_Sneer]				=	&BuffEffect::BuffEffectSneer;
	m_Effect[EBET_Mount]				=	&BuffEffect::BuffEffectMount;
	m_Effect[EBET_MountInvite]			=	&BuffEffect::BuffEffectMountInvite;
}

//----------------------------------------------------------------------------------
// ����Buff����Ч��
//----------------------------------------------------------------------------------
VOID BuffEffect::CalBuffEffect(Unit* pTarget, Unit* pSrc, EBuffEffectType eEffect, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet, const tagBuffProto* pProto)
{
	if( eEffect < EBET_Null || eEffect >= EBET_End )
		return;

	BUFFEFFECTRUTINE handler = m_Effect[eEffect];
	(*handler)(pTarget, pSrc, dwEffectMisc1, dwEffectMisc2, bSet, pProto);
}

//----------------------------------------------------------------------------------
// ����Ч������
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectNull(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{

}
//----------------------------------------------------------------------------------
// ����Ч����˲��
// dwEffectMisc1���ƶ��ľ��룬����0ʱΪ��ǰ�ƶ���С��0ʱΪ����ƶ�
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectTeleport(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	INT nTileDist = (INT)dwEffectMisc1;
	if( 0 == nTileDist ) return;

	BOOL bForward = (nTileDist > 0);	// ��ǰ��������ƶ�

	FLOAT fDistAbs = FLOAT(abs(nTileDist) * TILE_SCALE);	// ���Ծ���

	Vector3 vFaceTo = pTarget->GetFaceTo();	// �õ���ҳ���

	// �Ըó�����й�һ��
	Vec3Normalize(vFaceTo);

	// �õ��յ�����
	Vector3 vDest;

	if( bForward )
	{
		vDest = pTarget->GetCurPos() + vFaceTo * fDistAbs;
	}
	else
	{
		vDest = pTarget->GetCurPos() - vFaceTo * fDistAbs;
	}

	// �õ�һ����������յ�
	Vector3 vRealDest;
	NavCollider* pCollider = pTarget->GetMap()->GetNavMap()->GetCollider();

	if( pTarget->IsRole() )
	{
		NavCollider_Move collider;
		collider.IfCanGo(pCollider, pTarget->GetCurPos(), vDest, pTarget->GetSize(), &vRealDest);
	}
	else
	{
		Creature* pTargetCreature = static_cast<Creature*>(pTarget);

		// ����ײ��
		if( !pTargetCreature->NeedCollide() )
		{
			POINT nearPos;
			if( !pTargetCreature->GetMap()->IfCanDirectGo(pTargetCreature->GetCurPos().x, pTargetCreature->GetCurPos().z, vDest.x, vDest.z, &nearPos) )
			{
				vRealDest.x = FLOAT(nearPos.x * TILE_SCALE);
				vRealDest.z = FLOAT(nearPos.y * TILE_SCALE);
				vRealDest.y = pTargetCreature->GetMap()->GetGroundHeight(vRealDest.x, vRealDest.z);
			}
			else
			{
				vRealDest = vDest;
			}
		}
		// ��ײ��
		else
		{
			NavCollider_NPCMove collider;
			collider.IfCanGo(pCollider, pTarget->GetCurPos(), vDest, pTarget->GetSize(), &vRealDest);
		}
	}

	// ��������㲻��ȣ���˲��
	if( pTarget->GetCurPos() != vRealDest )
	{
		// ���ͻ��˷�����Ϣ
		tagNS_SpecialMove send;
		send.dwRoleID = pTarget->GetID();
		send.eType = ESMT_Teleport;
		send.vDestPos = vRealDest;
		pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
	
		// ˲�ƣ�����������Ϣ
		pTarget->GetMoveData().ForceTeleport(vRealDest, FALSE);
	}

	


}

//----------------------------------------------------------------------------------
// ����Ч�����������
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectInterruptSpell(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	// ��ϼ���
	pTarget->GetCombatHandler().InterruptPrepare(CombatHandler::EIT_Null, FALSE, TRUE);
}
//----------------------------------------------------------------------------------
// ����Ч����ѣ��
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectDizzy(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	bSet ? pTarget->SetState(ES_Dizzy) : pTarget->UnsetState(ES_Dizzy);
	
	// buffer����,�����ɵ�����ң����������
	EndbuffContinueIsAttack(pTarget, pSrc, bSet);
}

//----------------------------------------------------------------------------------
// ����Ч��������
// dwEffectMisc1�����˵ľ���
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectRepel(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !P_VALID(pSrc) ) return;

	INT nTileDist = (INT)dwEffectMisc1;
	if( 0 >= nTileDist ) return;
	FLOAT fDistAbs = FLOAT(nTileDist * TILE_SCALE);	// ���Ծ���

	// �õ�����Ŀ��֮�������
	Vector3 vVec = pTarget->GetCurPos() - pSrc->GetCurPos();

	// �Ը��������й�һ��
	Vec3Normalize(vVec);

    //-------------------------------------------------------------------------------
	//���˵��  ������ �� �������  �� ���ؽ�ɫ��λ�������غ�ʱ
	//����������������������ҵ�λ��������Ϊ�������ƽ�ɫ����������  
	//������ �õ�������Ŀ��֮�����������΢С����  
	//-------------------------------------------------------------------------------
    Vector3 vZero(0.0f,0.0f,0.0f);
	Vector3 vI(0.0f,-1.0f,0.0f);
	Vector3 vE(0.0f,1.0f,0.0f);
	
    if( vVec == vZero || vVec.y > 0.91f || vVec.y < -0.95f)
	{
		Vector3 vTemp(-0.73671913f,0.29935482f,0.60632634f);
        vVec = vTemp;
	} 
	//��ӽ���


	// �õ��յ�����
	Vector3 vDest = pTarget->GetCurPos() + vVec * fDistAbs;

	// �õ�һ����������յ�
	Vector3 vRealDest;
	NavCollider* pCollider = pTarget->GetMap()->GetNavMap()->GetCollider();

	if( pTarget->IsRole() )
	{
		NavCollider_Move collider;
		collider.IfCanGo(pCollider, pTarget->GetCurPos(), vDest, pTarget->GetSize(), &vRealDest);
	}
	else
	{
		Creature* pTargetCreature = static_cast<Creature*>(pTarget);

		// ����ײ��
		if( !pTargetCreature->NeedCollide() )
		{
			POINT nearPos;
			if( !pTargetCreature->GetMap()->IfCanDirectGo(pTargetCreature->GetCurPos().x, pTargetCreature->GetCurPos().z, vDest.x, vDest.z, &nearPos) )
			{
				vRealDest.x = FLOAT(nearPos.x * TILE_SCALE);
				vRealDest.z = FLOAT(nearPos.y * TILE_SCALE);
				vRealDest.y = pTargetCreature->GetMap()->GetGroundHeight(vRealDest.x, vRealDest.z);
			}
			else
			{
				vRealDest = vDest;
			}
		}
		// ��ײ��
		else
		{
			NavCollider_NPCMove collider;
			collider.IfCanGo(pCollider, pTarget->GetCurPos(), vDest, pTarget->GetSize(), &vRealDest);
		}
	}

	// ��������㲻��ȣ���˲��
	if( pTarget->GetCurPos() != vRealDest )
	{
		// ���ͻ��˷�����Ϣ
		tagNS_SpecialMove send;
		send.dwRoleID = pTarget->GetID();
		send.eType = ESMT_Repel;
		send.vDestPos = vRealDest;
		pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);

		// ˲�ƣ�����������Ϣ
		pTarget->GetMoveData().ForceTeleport(vRealDest, FALSE);

		// ������ǰ�ͷŵļ���
		pTarget->GetCombatHandler().End();
	}

}
//----------------------------------------------------------------------------------
// ����Ч�������
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectAssault(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !P_VALID(pSrc) || pSrc != pTarget ) return;	// ��漼�ܱ����Լ��ͷ�

	// �õ���һĿ����
	DWORD dwTargetUnitID = pTarget->GetCombatHandler().GetTargetUnitID();

	// �ڵ�ͼ�ڲ��Ҹ�Ŀ��
	Unit* pUnit = pTarget->GetMap()->FindUnit(dwTargetUnitID);
	if( !P_VALID(pUnit) ) return;

	// todo��Ҫȡ��Ŀ���ǰ�����꣬�����ȿ���Ч����ȡĿ������
	Vector3 vPosUnit	=	pUnit->GetCurPos();
	Vector3 vPosTarget	=	pTarget->GetCurPos();

	// �õ�����֮��ľ���
	FLOAT fDist = Vec3Dist(vPosUnit, vPosTarget);
	if( fDist <= 0.001f ) return;

	Vector3 vVec = vPosUnit - vPosTarget;	// �õ�����֮������

	// ���þ����ȥ��ҵ����ߵİ�����֮��
	FLOAT fPullDist = fDist - pSrc->GetSize().z - pTarget->GetSize().z;
	vVec *= ( fPullDist / fDist );

	// ��Ŀ��ĵ�ǰ������ϸ�����������ק��Ŀ���
	vPosTarget += vVec;

	// �鿴�ܷ�ͨ��
	NavCollider* pCollider = pTarget->GetMap()->GetNavMap()->GetCollider();
	ENavResult eResult = ENR_Arrive;

	if( pTarget->IsRole() )
	{
		NavCollider_Move collider;
		eResult = collider.IfCanGo(pCollider, pTarget->GetCurPos(), vPosTarget, pTarget->GetSize(), NULL);
	}
	else
	{
		NavCollider_NPCMove collider;
		eResult = collider.IfCanGo(pCollider, pTarget->GetCurPos(), vPosTarget, pTarget->GetSize(), NULL);
	}

	// ��治��ȥ
	if( ENR_Arrive != eResult ) return;

	// ���Գ���ȥ��������Ϣ
	tagNS_SpecialMove send;
	send.dwRoleID = pTarget->GetID();
	send.eType = ESMT_Assault;
	send.vDestPos = vPosTarget;
	pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);

	// ˲�ƹ�ȥ
	pTarget->GetMoveData().ForceTeleport(vPosTarget, FALSE);

}
//----------------------------------------------------------------------------------
// ����Ч�����⼼
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectNoSkill(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	bSet ? pTarget->SetState(ES_NoSkill) : pTarget->UnsetState(ES_NoSkill);

	// buffer����,�����ɵ�����ң����������
	EndbuffContinueIsAttack(pTarget, pSrc, bSet);
}
//----------------------------------------------------------------------------------
// ����Ч������˯
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectSpor(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	bSet ? pTarget->SetState(ES_Spor) : pTarget->UnsetState(ES_Spor);
	
	// buffer����,�����ɵ�����ң����������
	EndbuffContinueIsAttack(pTarget, pSrc, bSet);
}
//----------------------------------------------------------------------------------
// ����Ч��������
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectTie(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	bSet ? pTarget->SetState(ES_Tie) : pTarget->UnsetState(ES_Tie);
	if (P_VALID(pSrc) && pSrc->IsRole())
	{
		((Role*)pSrc)->StopMount();
	}

	// buffer����,�����ɵ�����ң����������
	EndbuffContinueIsAttack(pTarget, pSrc, bSet);
}
//----------------------------------------------------------------------------------
// ����Ч������ɢ
// dwEffectMisc1��
//	1��������ɫ�������һ������״̬
//	2��������ɫ�������һ���к�״̬
//	3��������ɫ�������һ��ĳ�ֿ������͵�״̬
//  4��������ɫ����ָ��ID��״̬
//	5��������ɫ����ָ��Ч�����͵�buff
// dwEffectMisc2������dwEffectMisc1Ϊ3ʱ����Ϊ��Ӧ��״̬��������
// dwEffectMisc2������dwEffectMisc1Ϊ5ʱ����Ϊ��Ӧ��Ч�����ͣ�bSet��ȥ�����һ������������
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectDispel(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	switch(dwEffectMisc1)
	{
	case 1:
		pTarget->DispelBuff(TRUE);
		break;

	case 2:
		pTarget->DispelBuff(FALSE);
		break;

	case 3:
		pTarget->DispelBuff((EBuffResistType)dwEffectMisc2);
		break;

	case 4:
		pTarget->DispelBuff(dwEffectMisc2);
		break;

	default:
		break;
	}

}

//----------------------------------------------------------------------------------
// ����Ч����������ɫ����ָ��Ч�����͵�buff
// dwEffectMisc2��ȥ�����һ��������ȥ������  dwEffectMisc1����Ӧ��Ч������
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectCancel(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	ASSERT(P_VALID(pTarget));

	pTarget->DispelBuff(dwEffectMisc1, (BOOL)dwEffectMisc2);
}

//----------------------------------------------------------------------------------
// ����Ч�����޵�
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectInvincible(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	bSet ? pTarget->SetState(ES_Invincible) : pTarget->UnsetState(ES_Invincible);
}
//----------------------------------------------------------------------------------
// ����Ч��������
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectInstantDie(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	ASSERT(P_VALID(pTarget));

	pTarget->ChangeHP(-pTarget->GetAttValue(ERA_HP), pSrc, NULL, pProto);
}
//----------------------------------------------------------------------------------
// ����Ч������ק
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectPull(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !P_VALID(pSrc) ) return;

	Vector3 vPosSrc		=	pSrc->GetCurPos();
	Vector3 vPosTarget	=	pTarget->GetCurPos();

	// �õ�����֮��ľ���
	FLOAT fDist = Vec3Dist(vPosSrc, vPosTarget);
	if( fDist <= 0.001f ) return;
	
	// �����ظ���һ��һ�£���������
	if (vPosTarget.x == vPosSrc.x && vPosTarget.z == vPosSrc.z && vPosTarget.y != vPosSrc.y) return;

	Vector3 vVec = vPosSrc - vPosTarget;	// �õ�����֮������

	// ���þ����ȥ��ҵ����ߵİ�����֮��
	FLOAT fPullDist = fDist - pSrc->GetSize().z - pTarget->GetSize().z;
	vVec *= ( fPullDist / fDist );

	// ��Ŀ��ĵ�ǰ������ϸ�����������ק��Ŀ���
	vPosTarget += vVec;

	// ֪ͨ�ͻ���˲��
	tagNS_SpecialMove send;
	send.dwRoleID = pTarget->GetID();
	send.eType = ESMT_Teleport;
	send.vDestPos = vPosTarget;
	pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);

	// ˲��Ŀ��
	pTarget->GetMoveData().ForceTeleport(vPosTarget, FALSE);

}
//----------------------------------------------------------------------------------
// ����Ч����ת��
// dwEffectMisc1���������յ���
// dwEffectMisc2��ת���İٷֱ�
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectHPTransfer(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( FALSE == P_VALID(pSrc) )
		return;

	if( pTarget->IsDead() || pSrc->IsDead() )
		return;

	INT nHP = (INT)dwEffectMisc1;
	INT nPct = (INT)dwEffectMisc2;

	nHP = min(pTarget->GetAttValue(ERA_HP), nHP);

	if( nHP <= 0 || nPct <= 0 ) return;

	// �����ȥѪ��
	pTarget->ChangeHP(-nHP, pSrc, NULL, pProto);

	// �Է�����Ѫ��
	INT nHPAdd = INT((FLOAT)nHP * ((FLOAT)nPct / 10000.0f));
	pSrc->ChangeHP(nHPAdd, pTarget, NULL, pProto);
}
//----------------------------------------------------------------------------------
// ����Ч����ת��
// dwEffectMisc1���������յ���
// dwEffectMic2��ת���İٷֱ�
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectMPTransfer(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( FALSE == P_VALID(pSrc) )
		return;

	if( pTarget->IsDead() || pSrc->IsDead() )
		return;

	INT nMP = (INT)dwEffectMisc1;
	INT nPct = (INT)dwEffectMisc2;

	nMP = min(pTarget->GetAttValue(ERA_MP), nMP);

	if( nMP <= 0 || nPct <= 0 ) return;

	// �����ȥ����
	pTarget->ChangeMP(-nMP);

	// �Է���������
	INT nMPAdd = INT((FLOAT)nMP * ((FLOAT)nPct / 10000.0f));
	pSrc->ChangeMP(nMPAdd);
}
//----------------------------------------------------------------------------------
// ����Ч��������
// dwEffectMisc1��<100000���ָ�Ѫ�������̶�ֵ��>100000���ָ�Ѫ�������ٷֱ�
// dwEffectMisc2�����Ϊ1���򲻻����µ�����
//----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectRevive(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( FALSE == pTarget->IsRole() ) return;
	if( FALSE == pTarget->IsDead() ) return;

	Role* pRole = static_cast<Role*>(pTarget);

	INT nHPMPRegain = INT(dwEffectMisc1);
	
	if( nHPMPRegain <= 0 ) return;

	INT nHP = 0;
	INT nMP = 0;
	if( nHPMPRegain < 100000 )
	{
		nHP = nMP = nHPMPRegain;
	}
	else
	{
		nHP = INT((FLOAT)pRole->GetAttValue(ERA_MaxHP) * (FLOAT(nHPMPRegain  - 100000) / 10000.0f));
		nMP = INT((FLOAT)pRole->GetAttValue(ERA_MaxMP) * (FLOAT(nHPMPRegain  - 100000) / 10000.0f));
	}

	if( nHP <= 0 ) return;

	// ��ұ�����
	pRole->BeRevived(nHP, nMP, pSrc);
}
//-----------------------------------------------------------------------------------
// ����Ч����˲����ȴ
// dwEffectMisc1����Ӧ��Ӧ���������ͣ����Ϊ-1����Ϊ���м���
// dwEffectMisc2������Ĳ�ɾ���ļ���
//-----------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectInstantCD(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	pTarget->ClearAllSkillCoodDown((ETalentType)dwEffectMisc1, dwEffectMisc2);
}
//------------------------------------------------------------------------------------
// ����Ч��������
//------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectLurk(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	ASSERT( P_VALID(pTarget) );

	if( bSet )
	{
		pTarget->Lurk((INT)dwEffectMisc1);
	}
	else
	{
		pTarget->UnLurk();
	}
}
//-------------------------------------------------------------------------------------
// ����Ч��������
//-------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectFly(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{

}
//-------------------------------------------------------------------------------------
// ����Ч�����ı���
// dwEffectMisc1���ı��޵İٷֱ�
//-------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectChangeEnmity(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	pSrc->ChangeEnmityCreatureValue(dwEffectMisc1);
}
//-------------------------------------------------------------------------------------
// ����Ч��������
// dwEffectMisc1��
//	0���ص���ҵĳ����¼��
//	1��������ҵ�����ĳ��У��ݲ�ʵ�֣�
//	2��������ҵ������Ǭ��ʯ���ݲ�ʵ�֣�
//	3��������Ϭ
//-------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectTransmit(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !P_VALID(pTarget) || FALSE == pTarget->IsRole() )
		return;

	Role* pTargetRole = static_cast<Role*>(pTarget);
	Role* pSrcRole = static_cast<Role*>(pSrc);

	if( 0 == dwEffectMisc1 )
	{
		pTargetRole->ReturnCity();
	}
	else if( 1 == dwEffectMisc1 )
	{

	}
	else if( 2 == dwEffectMisc1 )
	{

	}
	//������Ϭ
	else if( 3 == dwEffectMisc1 )
	{	
		//�����Žй���
		pTargetRole->GotoNewMap(pSrcRole->GetMapID(),pSrcRole->GetCurPos().x,pSrcRole->GetCurPos().y,pSrcRole->GetCurPos().z);
		//��Ԫ��
		pSrcRole->GetCurMgr().DecBagYuanBao(1, ELCLD_Skill_Use);
		
	}
}
//-------------------------------------------------------------------------------------
// ����Ч�����ɼ�����
//-------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectGather(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if ( 0 == dwEffectMisc1 )
	{
		GatherEffect(pTarget, pSrc, dwEffectMisc1, dwEffectMisc2, bSet);
	}
	else if ( 1 == dwEffectMisc1)
	{
		InvesEffect(pTarget, pSrc, dwEffectMisc1, dwEffectMisc2, bSet);
	}
}

//-------------------------------------------------------------------------------------
// ����Ч������е
//-------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectDisArm(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	bSet ? pTarget->SetState(ES_DisArm) : pTarget->UnsetState(ES_DisArm);
}

//---------------------------------------------------------------------------------------
// ����λ��
//---------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectExchangePos(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !P_VALID(pSrc) ) return;

	// �ֱ�õ����Եĵ�ǰ����
	Vector3 vPosSrc = pSrc->GetCurPos();
	Vector3 vPosTarget = pTarget->GetCurPos();

	// ���͸�Դ��
	tagNS_SpecialMove send1;
	send1.dwRoleID = pSrc->GetID();
	send1.eType = ESMT_Teleport;
	send1.vDestPos = vPosTarget;
	pSrc->GetMap()->SendBigVisTileMsg(pSrc, &send1, send1.dwSize);

	// ���͸�Ŀ�귽
	tagNS_SpecialMove send2;
	send2.dwRoleID = pTarget->GetID();
	send2.eType = ESMT_Teleport;
	send2.vDestPos = vPosSrc;
	pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send2, send2.dwSize);

	// ˲��Դ���
	pSrc->GetMoveData().ForceTeleport(vPosTarget, FALSE);

	// ˲��Ŀ�����
	pTarget->GetMoveData().ForceTeleport(vPosSrc, FALSE);
}

//----------------------------------------------------------------------------------------
// ����
// dwEffectMisc1�� Ҫɾ����BuffID
// dwEffectMisc2�� Ҫ��ӵ�BuffTypeID
//----------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectExplode(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	DWORD dwSrcBuffID		=	dwEffectMisc1;		// Ҫȥ����BuffID
	DWORD dwDestBuffTypeID	=	dwEffectMisc2;		// Ҫ��ӵ�Buff��TypeID

	// ���Ҫ����ԴBuff�������Ŀ������û�У��򷵻�
	if( P_VALID(dwSrcBuffID) )
	{
		if( !pTarget->IsHaveBuff(dwSrcBuffID) )
			return;
	}

	// ��ɾ��
	if( P_VALID(dwSrcBuffID) )
	{
		pTarget->RemoveBuff(dwSrcBuffID, TRUE);
	}

	// �����
	if( P_VALID(dwEffectMisc2) )
	{
		const tagBuffProto* pProto = g_attRes.GetBuffProto(dwDestBuffTypeID);
		if( P_VALID(pProto) )
		{
			pTarget->TryAddBuff(pSrc, pProto, NULL, NULL, NULL);
		}
	}
}

//----------------------------------------------------------------------------------------
// �־�
//----------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectFunk(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{

}

//-----------------------------------------------------------------------------------------
// ׷��
// dwEffectMisc1: 0������� 1��Ŀ��ǰ�� 2��Ŀ���
//-----------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectPursue(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !P_VALID(pSrc) || pTarget != pSrc ) return;		// ׷��Ҳһ�����ͷ���������ӵ�buff

	// ȡ��target�ĵ�һĿ��
	DWORD dwTargetUnitID = pTarget->GetCombatHandler().GetTargetUnitID();
	Unit* pUnit = pTarget->GetMap()->FindUnit(dwTargetUnitID);

	if( !P_VALID(pUnit) ) return;

	// ȡ�����Ŀ���λ��
	Vector3 vUnitPos = pUnit->GetCurPos();

	// ����dwEffectMisc1��ȡĿ���
	Vector3 vDest;

	if( 0 == dwEffectMisc1 )	// ֱ�������
	{
		Vector3 vTargetPos	=	pUnit->GetCurPos();

		// ��������֮��ľ���
		FLOAT fDist = Vec3Dist(vUnitPos, vTargetPos);
		if( fDist <= 0.001f ) return;

		Vector3 vVec = vUnitPos - vTargetPos;	// �õ�����֮������

		// ���þ����ȥ��ҵ����ߵİ�����֮��
		FLOAT fPursueDist = fDist - pSrc->GetSize().z - pTarget->GetSize().z;
		vVec *= ( fPursueDist / fDist );

		// ��Ŀ��ĵ�ǰ������ϸ�����������ק��Ŀ���
		vDest = pTarget->GetCurPos() + vVec;
	}
	else if( 1 == dwEffectMisc1 )	// Ŀ����ǰ
	{
		Vector3 vFace = pUnit->GetFaceTo();
		Vec3Normalize(vFace);		// ��һ��

		vFace *= pUnit->GetSize().z + pTarget->GetSize().z;

		vDest = vUnitPos + vFace;
	}
	else if( 2 == dwEffectMisc1 )
	{
		Vector3 vFace = pUnit->GetFaceTo();
		Vec3Normalize(vFace);		// ��һ��

		vFace *= -(pUnit->GetSize().z + pTarget->GetSize().z);

		vDest = vUnitPos + vFace;
	}
	else
	{
		return;
	}

	// ���͸��ͻ���˲����Ϣ
	tagNS_SpecialMove send;
	send.dwRoleID = pTarget->GetID();
	send.eType = ESMT_Teleport;
	send.vDestPos = vDest;
	pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);

	// ˲��һ��
	pTarget->GetMoveData().ForceTeleport(vDest, FALSE);

	// �ı�һ�³���
	pTarget->GetMoveData().SetFaceTo(pUnit->GetCurPos() - pTarget->GetCurPos());

}

//------------------------------------------------------------------------------------------
// ������ʱ��
//------------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectNoPrepare(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	ASSERT(P_VALID(pTarget));

	// ��ȡ����ʱ��Ӱ��ٷֱ�(+����������ʱ�䣻-����������ʱ��)
	INT nSkillPrepareModPct = (INT)dwEffectMisc1;

	if( bSet )
	{
		pTarget->GetCombatHandler().ModSkillPrepareModPct(nSkillPrepareModPct);
	}
	else
	{
		pTarget->GetCombatHandler().ModSkillPrepareModPct(-nSkillPrepareModPct);
	}
}

//-------------------------------------------------------------------------------------------
// ˮ������
//-------------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectOnWater(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !pTarget->IsRole() ) return;

	Role* pRole = static_cast<Role*>(pTarget);

	if( bSet )
	{
		if( !pRole->IsInRoleState(ERS_WaterWalk) )
		{
			pRole->SetRoleState(ERS_WaterWalk);
		}
	}
	else
	{
		if( pRole->IsInRoleState(ERS_WaterWalk) )
		{
			pRole->UnsetRoleState(ERS_WaterWalk);
		}
	}
}

//--------------------------------------------------------------------------------------------
// ��·��Ѫ
//--------------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectMoveHPDmg(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	ASSERT(P_VALID(pTarget));

	INT nVal = (INT)dwEffectMisc2;		// ��ֵΪ�ӣ���ֵΪ��
	
	switch(dwEffectMisc1)
	{
	case 1:		// ��Ѫ
		pTarget->ChangeHP(nVal, pSrc, NULL, pProto);
		break;
	case 2:		// ����
		pTarget->ChangeMP(nVal);
		break;
	}
}

//---------------------------------------------------------------------------------------------
// ���ӻ���
//---------------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectIgnoreArmor(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	ASSERT(P_VALID(pTarget));

	// ��ȡ����Ӱ��ٷֱ�(ֻ��Ϊ��ֵ)
	INT nArmorDecPct = (INT)dwEffectMisc1;

	if( bSet )
	{
		pTarget->GetCombatHandler().ModTargetArmorLeftPct(-nArmorDecPct);
	}
	else
	{
		pTarget->GetCombatHandler().ModTargetArmorLeftPct(nArmorDecPct);
	}
}

//----------------------------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectSneer(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if(!P_VALID(pTarget) || !P_VALID(pSrc) )	return;

	if( !pTarget->IsCreature() ) return;
	Creature* pCreature	= static_cast<Creature *>(pTarget);
	if( !P_VALID(pCreature->GetAI()) ) return; 

	if( bSet )
	{
		DWORD dwMaxEnmityUnitID = pCreature->GetAI()->GetMaxEnmityUnitID();
		if( dwMaxEnmityUnitID == pSrc->GetID() )
			return;

		INT nEnmityMod = pCreature->GetAI()->GetEnmityValue(dwMaxEnmityUnitID) - pCreature->GetAI()->GetEnmityValue(pSrc->GetID())+1;

		// ���ӳ�޼ӳ�ֵ
		pCreature->GetAI()->AddEnmityMod(pSrc, nEnmityMod);
		pCreature->GetAI()->SetTargetUnitID(pSrc->GetID());
	}
	else
	{
		pCreature->GetAI()->ClearEnmityModValue(pSrc->GetID());
		DWORD dwMaxEnmityID = pCreature->GetAI()->GetMaxEnmityUnitID();
		pCreature->GetAI()->SetTargetUnitID(dwMaxEnmityID);
	}
}

//----------------------------------------------------------------------------------------------
// �ɼ�Ч��
//----------------------------------------------------------------------------------------------
VOID BuffEffect::GatherEffect(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !pTarget->IsCreature() ) return;
	if( !pSrc->IsRole() ) return;

	// �����Դ�ͽ�ɫ
	Creature* pCreature	= static_cast<Creature *>(pTarget);	
	Role* pRole	= static_cast<Role *>(pSrc);	
	if ( !P_VALID(pCreature) || !P_VALID(pRole) )
		return ;

	// �ж���Դ����ü���
	Skill* pGatherSkill = NULL;
	if ( pCreature->IsNatuRes() )
		pGatherSkill = pRole->GetSkill(GATHER_SKILL_MINING);
	else if ( pCreature->IsManRes() )
		pGatherSkill = pRole->GetSkill(GATHER_SKILL_HARVEST);
	if ( !P_VALID(pGatherSkill) )
		return;

// 	// ����ɫ��������				
// 	ItemMgr& itemMgr = pRole->GetItemMgr();	
// 	if (itemMgr.GetBagFreeSize() <= 0)
// 	{
// 		tagNS_RolePickUpItem send;
// 		send.dwErrorCode	= E_Loot_BAG_NOT_ENOUGH_SPACE;
// 		send.dwRoleID		= pRole->GetID();
// 		pRole->SendMessage(&send, send.dwSize);
// 		return ;
// 	}
// 
// 	// ����Դ���빻�� 
// 	if (!pRole->IsInCombatDistance(*pCreature, pGatherSkill->GetOPDist()))
// 		return ;	
// 
// 	// ��Դ�ѱ�ռ��
// 	if( pCreature->IsTagged() )
// 		return;

	// �Բ����ʵ�����Դ 	
	sLootMgr.ResourceLoot(pCreature, pRole);

	// �������������ӣ������������ܣ�
	const tagCreatureProto* pCreatureProto = g_attRes.GetCreatureProto(pCreature->GetTypeID());

	// ��������� = ��Դ���ṩ�������ȡ�(1+�������/100)
	INT nExpAdd =  INT((FLOAT)pCreatureProto->nExpGive * (1.0f + (FLOAT)pRole->GetAttValue(ERA_Savvy) / 100.0f));
	pRole->ChangeSkillExp(pGatherSkill, nExpAdd);

	// ��Դ��Ѫ
	pCreature->ChangeHP(-1, pSrc);
}

//----------------------------------------------------------------------------------------------
// �������
//----------------------------------------------------------------------------------------------
VOID BuffEffect::InvesEffect(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet,  const tagBuffProto* pProto)
{
	if( !pTarget->IsCreature() ) return;
	if( !pSrc->IsRole() ) return;

	// ��õ���ͽ�ɫ
	Creature* pCreature	= static_cast<Creature *>(pTarget);	
	Role* pRole	= static_cast<Role *>(pSrc);	
	if ( !P_VALID(pCreature) || !P_VALID(pRole) )
		return ;

	// �жϵ��ﲢ��ü���
	Skill* pInvesSkill = NULL;
	if ( pCreature->IsInves() )
		pInvesSkill = pRole->GetSkill(INVES_SKILL);
	if ( !P_VALID(pInvesSkill) )
		return;

// 	// ����Դ���빻�� 
// 	if (!pRole->IsInCombatDistance(*pCreature, pInvesSkill->GetOPDist()))
// 		return ;	
// 
// 	// �����ѱ�ռ��
// 	if( pCreature->IsTagged() )
// 		return;

	// �Բ����ʵ�����Ʒ	
	sLootMgr.InvestigateLoot(pCreature, pRole);

	// ���񴥷�
	pRole->OnQuestEvent(EQE_Invest, pTarget->GetID(), pCreature->GetTypeID());

	// ����ű�����
	pCreature->OnInvest(pRole);

	// ���ｵѪ
	pCreature->ChangeHP(-1, pSrc);
}

//-----------------------------------------------------------------------------------------------------------------
// ���
//-----------------------------------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectMount( Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet/*=TRUE*/, const tagBuffProto* pProto/*=NULL*/ )
{
	if (!P_VALID(pTarget) || !P_VALID(pSrc)) return;
	if (!pTarget->IsRole()) return;

	Role* pRole = dynamic_cast<Role* >(pSrc);
	if (!P_VALID(pRole)) return;

	if (bSet)
	{
		pRole->GetPetPocket()->RidePet();
	}
	else
	{
		PetSoul* pSoul = pRole->GetPetPocket()->GetMountPetSoul();
		if (P_VALID(pSoul))
		{
			Role* pPassenger = pSoul->GetPassenger();
			if (P_VALID(pPassenger))
			{
				pPassenger->GetPetPocket()->StopRideAfter(pRole);
			}
		}
		
		pRole->GetPetPocket()->UnRidePet();
	}
}

//-----------------------------------------------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------------------------------------------
VOID BuffEffect::BuffEffectMountInvite( Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet/*=TRUE*/, const tagBuffProto* pProto/*=NULL*/ )
{
	if (!P_VALID(pTarget) || !P_VALID(pSrc)) return;
	if (!pTarget->IsRole() || !pSrc->IsRole()) return;
	Role* pHost = static_cast<Role* >(pSrc);
	Role* pInvited = static_cast<Role* >(pTarget);

	DWORD dwRtv = E_Success;

	if (bSet)
	{
		dwRtv = pInvited->GetPetPocket()->SetRideAfter(pHost, TRUE);
		ASSERT(E_Success == dwRtv);
		dwRtv = pHost->GetPetPocket()->AddPassenger(pInvited);
		ASSERT(E_Success == dwRtv);
	}
	else
	{
		dwRtv = pInvited->GetPetPocket()->SetRideAfter(pHost, FALSE);
		ASSERT(E_Success == dwRtv);
		dwRtv = pHost->GetPetPocket()->RemovePassenger(pInvited);
		ASSERT(E_Success == dwRtv);
	}
}

//-----------------------------------------------------------------------------------------------------------------
// ��������: �ͷ���buffer�����Ƿ�����������
//-----------------------------------------------------------------------------------------------------------------
BOOL BuffEffect::EndbuffContinueIsAttack(Unit* pTarget, Unit* pSrc, BOOL bSet)
{
	if (!P_VALID(pTarget) || !P_VALID(pSrc))
		return FALSE;

	// ��ʼ��˯�����ӹ���׷��ʱ��
	if ( FALSE ==bSet && P_VALID(pTarget) && pTarget->IsCreature() )
	{
		Creature * pCreature = static_cast<Creature *>(pTarget);
		
		// �鿴�Ƿ�ɴ�
		if( MoveData::EMR_Success == pTarget->GetMoveData().IsCanCreatureWalk(pSrc->GetCurPos(), EMS_CreatureWalk, TRUE) )
		{
			
			if (!P_VALID( pCreature->GetAI()) )
				return FALSE;

			pCreature->GetAI()->AddEnmity(pSrc, 1);

			// ����Դ���Ϊ��������޶����
			pCreature->GetAI()->SetMaxEnmityUnitID(pSrc->GetID());
		}

	}
	return TRUE;
}