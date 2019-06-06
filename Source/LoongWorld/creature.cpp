//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: creature.cpp
// author: Aslan
// actor:
// data: 2008-8-11
// last:
// brief: ��Ϸ�������࣬�������еĹ����ͨNPC���ɽ���ʽ�������ȵ�
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/move_define.h"
#include "../WorldDefine/creature_define.h"
#include "../WorldDefine/msg_combat.h"
#include "../ServerDefine/NPCTeam_define.h"

#include "unit.h"
#include "buff.h"
#include "creature.h"
#include "creature_ai.h"
#include "role.h"
#include "map.h"
#include "loot_mgr.h"
#include "team.h"
#include "group_mgr.h"
#include "script_mgr.h"
#include "path_finder.h"
#include "NPCTeam.h"
#include "NPCTeam_mgr.h"
#include "ai_trigger.h"

Creature* Creature::Create( DWORD dwID, DWORD dwMapID, const tagCreatureProto* pProto, Vector3& vPos, Vector3& vFaceTo, ECreatureSpawnedType eSpawnedType, DWORD dwSpawnerID, BOOL bCollide, const tagMapWayPointInfoList* patrolList, 
						   DWORD dwTeamID/*=GT_INVALID*/, DWORD dwMapObjID /*= GT_INVALID*/ )
{
	Creature* pNew = new Creature(dwID, dwMapID, vPos, vFaceTo, eSpawnedType, dwSpawnerID, bCollide, dwTeamID, dwMapObjID);
	pNew->Init(pProto, patrolList);

	return pNew;
}

VOID Creature::Delete( Creature* &pCreature )
{
	SAFE_DEL(pCreature);
}
//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
Creature::Creature(DWORD dwID, DWORD dwMapID, Vector3& vPos, Vector3& vFaceTo, 
				   ECreatureSpawnedType eSpawnedType, DWORD dwSpawnerID, BOOL bCollide, DWORD dwTeamID, DWORD dwMapObjID)
				   :Unit(dwID, dwMapID, vPos.x, vPos.y, vPos.z, vFaceTo.x, vFaceTo.y, vFaceTo.z), ScriptData(),
				   m_pProto(NULL), m_pAIController(NULL), m_eSpawnedType(eSpawnedType), m_dwSpawnerID(dwSpawnerID),m_nLiveTick(0),
				   m_vBornPos(vPos), m_vBornFace(vFaceTo), m_bCollide(bCollide), m_bTagged(FALSE), m_dwTaggedOwner(GT_INVALID),
				   m_nRespawnTickCountDown(0), m_pMeleeSkill(NULL), m_pRangedSkill(NULL), m_dwTeamID(dwTeamID), m_pScript(NULL),
				   m_bDoorOpen(FALSE),m_dwMapObjID(dwMapObjID)

{
	
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
Creature::~Creature()
{
	Destroy();
}

//--------------------------------------------------------------------------------
// ��ʼ��
//--------------------------------------------------------------------------------
BOOL Creature::Init(const tagCreatureProto* pProto, const tagMapWayPointInfoList* patrolList)
{
	m_pProto = pProto;

	// ��ʼ���ű�
	m_pScript = g_ScriptMgr.GetCreatureScript(pProto->dwTypeID);

	// ��ʼ������
	InitAtt(pProto);

	// ��ʼ��AI
	InitAI(pProto, patrolList);

	// ���������ʼ����
	CalInitAtt();

	// ���ýű�
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnLoad(this);
	}

	return TRUE;
}

//---------------------------------------------------------------------------------
// ��ʼ����������
//---------------------------------------------------------------------------------
BOOL Creature::InitAtt(const tagCreatureProto* pProto)
{
	// ���ð�����
	m_Size = pProto->vSize;

	// �Ӿ�̬�����п����������������
	IFASTCODE->MemCpy(m_nBaseAtt, pProto->nBaseAtt, sizeof(m_nBaseAtt));

	m_nLevel = pProto->nLevel;
	m_nCurLevelExp = 0;

	// ���ݹ������ͨ����id�����ɼ���
	const tagSkillProto* pMeleeSkillProto = g_attRes.GetSkillProto(pProto->dwNormalSkillIDMelee);
	if( P_VALID(pMeleeSkillProto) )
	{
		DWORD dwSkillID = Skill::GetIDFromTypeID(pMeleeSkillProto->dwID);
		DWORD nLevel = Skill::GetLevelFromTypeID(pMeleeSkillProto->dwID);

		m_pMeleeSkill = new Skill(dwSkillID, nLevel, 0, 0, 0);
		ASSERT(P_VALID(m_pMeleeSkill));
		m_mapSkill.Add(m_pMeleeSkill->GetID(), m_pMeleeSkill);
	}
	else
	{
		m_pMeleeSkill = NULL;
	}

	const tagSkillProto* pRangedSkillProto = g_attRes.GetSkillProto(pProto->dwNormalSkillIDRanged);
	if( P_VALID(pRangedSkillProto) )
	{
		DWORD dwSkillID = Skill::GetIDFromTypeID(pRangedSkillProto->dwID);
		DWORD nLevel = Skill::GetLevelFromTypeID(pRangedSkillProto->dwID);

		m_pRangedSkill = new Skill(dwSkillID, nLevel, 0, 0, 0);

		m_mapSkill.Add(m_pRangedSkill->GetID(), m_pRangedSkill);
	}
	else
	{
		m_pRangedSkill = NULL;
	}

	return TRUE;
}

//---------------------------------------------------------------------------------
// ��ʼ������AI
//---------------------------------------------------------------------------------
BOOL Creature::InitAI(const tagCreatureProto* pProto, const tagMapWayPointInfoList*	patrolList)
{
	// ����״̬��
	if( pProto->eAICreateType != EACT_Null )
	{
		m_pAIController = new AIController(this, patrolList);
	}

	return TRUE;
}

//---------------------------------------------------------------------------------
// �����ʱ��ˢ������
//---------------------------------------------------------------------------------
VOID Creature::RefreshAtt()
{
	// �ƶ����
	m_MoveData.Reset(m_vBornPos.x, m_vBornPos.y, m_vBornPos.z, m_vBornFace.x, m_vBornFace.y, m_vBornFace.z);

	// �ٶ����
	m_fXZSpeed = X_DEF_XZ_SPEED;
	m_fYSpeed = X_DEF_Y_SPEED;

	// ���������
	m_Size = m_pProto->vSize;

	// ���֮ǰ��״̬
	ClearState();

	// ĳЩ��������
	m_bTagged = FALSE;
	m_dwTaggedOwner = GT_INVALID;
	m_nRespawnTickCountDown = 0;
	m_nLiveTick = 0;

	// ���Ե���
	ZeroMemory(m_nAttMod, sizeof(m_nAttMod));
	ZeroMemory(m_nAttModPct, sizeof(m_nAttModPct));
	ZeroMemory(m_nAtt, sizeof(m_nAtt));
	IFASTCODE->MemCpy(m_nBaseAtt, m_pProto->nBaseAtt, sizeof(m_nBaseAtt));

	// �������
	TMap<DWORD, Skill*>::TMapIterator it = m_mapSkill.Begin();
	Skill* pSkill = NULL;
	while( m_mapSkill.PeekNext(it, pSkill) )
	{
		// ��ʼ���ܲ����
		if( pSkill != m_pMeleeSkill && pSkill != m_pRangedSkill )
		{
			RemoveSkill(pSkill->GetID());
		}
	}
}

//---------------------------------------------------------------------------------
// �����ʱ��ˢ��AI
//---------------------------------------------------------------------------------
VOID Creature::RefreshAI()
{
	// ����״̬��
	if( P_VALID(m_pAIController) )
	{
		m_pAIController->Refresh();
	}
}


//---------------------------------------------------------------------------------
// ������ﵱǰ��������
//---------------------------------------------------------------------------------
VOID Creature::CalInitAtt()
{
	for(INT n = 0; n < ERA_End; n++)
	{
		m_nAtt[n] = m_nBaseAtt[n] + m_nAttMod[n] + (INT)(m_nBaseAtt[n] * (FLOAT)m_nAttModPct[n] / 10000.0f);
		// todo��ȡ������
	}

	// ��ĳЩ����ǰ���ԡ����ó����ֵ
	m_nAtt[ERA_HP] = m_nAtt[ERA_MaxHP];
	m_nAtt[ERA_MP] = m_nAtt[ERA_MaxMP];
	m_nAtt[ERA_Vitality] = m_nAtt[ERA_MaxVitality];
	m_nAtt[ERA_Endurance] = m_nAtt[ERA_MaxEndurance];

	// ����ĳЩ�ɻ�������Ӱ���ֵ
	m_fXZSpeed *= FLOAT(m_nAtt[ERA_Speed_XZ]) / 10000.0f;
	m_fYSpeed *= FLOAT(m_nAtt[ERA_Speed_Y]) / 10000.0f;
	m_Size *= FLOAT(m_nAtt[ERA_Shape]) / 10000.0f;
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID Creature::Destroy()
{
	SAFE_DEL(m_pAIController);
}

//---------------------------------------------------------------------------------
// ���º���
//---------------------------------------------------------------------------------
VOID Creature::Update()
{
	GetMoveData().Update();
	UpdateCombat();
	UpdateAI();
	UpdateSkill();
	UpdateBuff();
	UpdateTimedIssue();
	UpdateLiveTime();
}

//---------------------------------------------------------------------------------
// ���¼���
//---------------------------------------------------------------------------------
VOID Creature::UpdateCombat()
{
	// ��ǰ���ܻ�������
	if( m_CombatHandler.IsValid() )
	{
		m_CombatHandler.Update();

		if( !m_CombatHandler.IsValid() )
		{
			// �������ͷ���ɣ������´ι����ĵȴ�ʱ��
			GetAI()->SetNextAttackWaitTick(m_pProto->nAttackInterTick);
		}
	}
}

//---------------------------------------------------------------------------------
// ����AI
//---------------------------------------------------------------------------------
VOID Creature::UpdateAI()
{
	// ���ݹ���Ŀǰ������һ��AI״̬������ν��и���
	if( P_VALID(m_pAIController) )
		m_pAIController->Update();
}

//---------------------------------------------------------------------------------
// ��������ʱ��
//---------------------------------------------------------------------------------
VOID Creature::UpdateLiveTime()
{
	if(0 != m_pProto->nLiveTick)
	{
		++m_nLiveTick;
		if(m_nLiveTick >= m_pProto->nLiveTick)
		{
			OnDisappear();
		}
	}
}

//---------------------------------------------------------------------------------
// �����Һ�NPC̸���ľ���
//---------------------------------------------------------------------------------
BOOL Creature::CheckNPCTalkDistance(Role* pRole)
{
	if( !P_VALID(pRole) ) return FALSE;
	return IsInDistance(*pRole, MAX_NPC_TALK_DISTANCE);
}

//---------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------
VOID Creature::OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited)
{
	ASSERT( P_VALID(pSkill) && P_VALID(pSrc) );
	
	// �жϼ��ܵĵ���Ŀ������
	if( !pSkill->IsHostile() && !pSkill->IsFriendly() ) return;
	DWORD dwFriendEnemyFlag = pSrc->FriendEnemy(this);

	if( pSkill->IsHostile() && (ETFE_Hostile & dwFriendEnemyFlag) )
	{
		if( pSrc->IsRole() )
		{
			// �������֮ǰ�����������Ϊ����
			if( ECTT_Hit == GetTaggedType() && !IsTagged() )
			{
				SetTagged(pSrc->GetID());
			}
		}
		
		// �����ޣ������AI����
		if( P_VALID(m_pAIController) )
		{
			INT	nValue = pSkill->GetEnmity() + pSrc->GetAttValue(ERA_Enmity_Degree);
			m_pAIController->AddEnmity(pSrc, nValue);
			m_pAIController->OnEvent(pSrc, ETEE_BeAttacked);
		}

		if( bHited )
		{
			OnBuffTrigger(pSrc, ETEE_Hited, 0, 0);
			if( bBlock )
			{
				OnBuffTrigger(pSrc, ETEE_Block, 0, 0);
			}
			if( bCrited )
			{
				OnBuffTrigger(pSrc, ETEE_Crited, 0, 0);
			}

			// ��⵱ǰ���ܵĴ��
			GetCombatHandler().InterruptPrepare(CombatHandler::EIT_Skill, ESSTE_Default == pSkill->GetTypeEx());
			// ���ĳЩ��������ϵ�buff
			OnInterruptBuffEvent(EBIF_BeAttacked);
		}
		else
		{
			OnBuffTrigger(pSrc, ETEE_Dodge, 0, 0);
		}
		OnBuffTrigger(pSrc, ETEE_BeAttacked, 0, 0);
	}
}

VOID Creature::OnBuffInjury(Unit* pSrc)
{
	if( !P_VALID(pSrc) )	return;

	if( pSrc->IsRole() )
	{
		// �������֮ǰ�����������Ϊ����
		if( ECTT_Hit == GetTaggedType() && !IsTagged() )
		{
			SetTagged(pSrc->GetID());
		}
	}

	// �����ޣ������AI����
	if( P_VALID(m_pAIController) )
	{
		m_pAIController->AddEnmity(pSrc, 1);
		m_pAIController->OnEvent(pSrc, ETEE_BeAttacked);
	}
}

VOID Creature::OnAIAttack()
{
	DWORD dwSkillID = m_CombatHandler.GetSkillID();

	// �������ͨ�������������ﴥ���������¼�
	if( P_VALID(m_pMeleeSkill) && dwSkillID  == m_pMeleeSkill->GetID() )
	{
		if(P_VALID(m_pAIController))
		{
			AITriggerMgr* pTrggerMgr = GetAI()->GetAITriggerMgr();
			if(P_VALID(pTrggerMgr))
			{
				pTrggerMgr->OnEvent(ETEE_Attack); 
				return;
			}
		}
	}

	if(P_VALID(m_pRangedSkill) && dwSkillID == m_pRangedSkill->GetID())
	{
		if(P_VALID(GetAI()))
		{
			AITriggerMgr* pTrggerMgr = GetAI()->GetAITriggerMgr();
			if(P_VALID(pTrggerMgr))
			{
				pTrggerMgr->OnEvent(ETEE_Attack); 
				return;
			}
		}
	}

}

//---------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------
VOID Creature::OnDead(Unit* pSrc, Skill* pSkill, const tagBuffProto* pBuff, DWORD dwSerial, DWORD dwMisc)
{
	// ��������Ѿ���������ֱ�ӷ���
	if( IsInState(ES_Dead) ) return;

	// ������
	OnBuffTrigger(pSrc, ETEE_Die);

	//�����ű� Ŀǰ��ʹ��bRemove����ű�����ֵ
	BOOL bRemove = TRUE;
	if (P_VALID(m_pScript))
	{
		m_pScript->OnDie(this, pSrc, bRemove);
	}

	// ����Ϊ����״̬
	SetState(ES_Dead);

	// ֹͣ�ƶ�
	m_MoveData.StopMoveForce();

	// ֹͣ��ǰ�ļ���
	m_CombatHandler.End();

	// ��������ʱAI����

	// �Ƴ��������е�buff
	RemoveAllBuff();

	// ��ճ���б�
	if( P_VALID(GetAI()) )
		GetAI()->ClearAllEnmity();

	// �ҵ�����������
	Role* pReward = FindRewardRole(pSrc);
	if( P_VALID(pReward) )
	{
		// ���㾭�齱��
		BOOL bKill = FALSE;
		if( P_VALID(pSkill) )
			bKill = TRUE;

		if(pReward->GetTeamID() ==GT_INVALID)
		{
			ExpReward(pReward, bKill);

			// ��������ɱ�ּ���
			pReward->OnQuestEvent(EQE_Kill, GetTypeID(), GetLevel());
		}
		else
		{
			const Team *pTeam = g_groupMgr.GetTeamPtr(pReward->GetTeamID());
			std::vector<Role*>	vecRole;
			INT   nShareRole = CalExpShareRole(pTeam, vecRole);

			std::vector<Role*>::iterator it = vecRole.begin();
			while (it != vecRole.end())
			{
				Role* pMember = *it;
				++it;

				if( !P_VALID(pMember) )
					continue;

				ExpReward(pMember, bKill, pTeam, nShareRole);

				// ��������ɱ�ּ���
				pMember->OnQuestEvent(EQE_Kill, GetTypeID(), GetLevel());
			}			
		}
		
		// ������佱��
		sLootMgr.MonsterLoot(this, pReward);
	}

	// ��ɱ��
	if( P_VALID(pSrc) )
	{
		pSrc->OnKill(this);
	}

	// ��ͼ�¼�
	GetMap()->OnCreatureDie(this, pSrc);

	// ���͹���������Ϣ
	tagNS_RoleDead send;
	send.dwRoleID = GetID();
	send.dwSrcRoleID = (P_VALID(pSrc) ? pSrc->GetID() : GT_INVALID);
	send.dwSerial = dwSerial;
	if( P_VALID(pSkill) )
	{
		send.eCause = ERDC_Skill;
		send.dwMisc = pSkill->GetTypeID();
		send.dwMisc2 = dwMisc;
	}
	else if( P_VALID(pBuff) )
	{
		send.eCause = ERDC_Buff;
		send.dwMisc = pBuff->dwID;
		send.dwMisc2 = dwMisc;
	}
	else
	{
		send.eCause = ERDC_Other;
		send.dwMisc = GT_INVALID;
		send.dwMisc2 = dwMisc;
	}

	GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);

	// ������������ʱ��
	SetRespawnCountDown(GetRespawnTick());

	// �ӵ�ͼ���õ�������
//	if (bRemove)
//	{
		GetMap()->RemoveCreature(this);
//	}
}

//---------------------------------------------------------------------------------
// ������ʧ
//---------------------------------------------------------------------------------
VOID Creature::OnDisappear()
{
	// ��������Ѿ���������ֱ�ӷ���
	if( IsInState(ES_Dead) ) return;

	// ����Ϊ����״̬
	SetState(ES_Dead);

	// ֹͣ�ƶ�
	m_MoveData.StopMoveForce();

	// ֹͣ��ǰ�ļ���
	m_CombatHandler.End();

	// ��������ʱAI����

	// �Ƴ��������е�buff
	RemoveAllBuff();

	// ��ճ���б�
	if( P_VALID(GetAI()) )
		GetAI()->ClearAllEnmity();

	// ��ͼ�¼�
	GetMap()->OnCreatureDisappear(this);

	// ������������ʱ��
	SetRespawnCountDown(GetRespawnTick());

	// �ӵ�ͼ���õ�������
	GetMap()->RemoveCreature(this);

	// ������ʧ�ű�����
	if (P_VALID(m_pScript))
		m_pScript->OnDisappear(this);

	// ͬ�����ͻ���
	GetMap()->SyncRemoveUnitToBigVisTile(this);
}

//----------------------------------------------------------------------------------
// �����ɱ
//----------------------------------------------------------------------------------
VOID Creature::OnKill(Unit* pTarget)
{
	OnBuffTrigger(pTarget, ETEE_Kill);
}

//----------------------------------------------------------------------------------
// ���Ը���
//----------------------------------------------------------------------------------
ECreatureReviveResult Creature::TryRevive()
{
	// ��������Ƕ�̬���ɵģ�����Ҫɾ���ڴ���
	if( ECST_Dynamic == m_eSpawnedType ) return ECRR_NeedDestroy;

	// �����ʱ����һ��Tick
	CountDownRespawnTick();

	// �������ʱ��û�����򷵻�ʧ��
	if( GetRespawnCountDown() > 0 ) return ECRR_Failed;

	// �����ˢ�µ�֣�����Ҫ�������Ʒ
	if ( ECST_SpawnPoint == m_eSpawnedType ) return ECRR_NeedReplace;

	// ����ǳ�Ѩ�Ĺ���
	if( ECST_Nest == m_eSpawnedType )
	{
		// �ҵ�ˢ���ĳ�Ѩ�Ƿ񻹴��ڣ�����������ˣ�˵����Ѩ��ûˢ�£���ô�Լ�Ҳ��ˢ��
		Creature* pNestCreature = GetMap()->FindCreature(m_dwSpawnerID);
		if( !P_VALID(pNestCreature) ) return ECRR_Failed;

		// ����ҵ������ҵ��ó�Ѩ�ĵĳ�Ѩ����
		const tagCreatureProto* pProto = pNestCreature->GetProto();
		if( !P_VALID(pProto) || !P_VALID(pProto->pNest) ) return ECRR_Failed;

		const tagNestProto* pNest = pProto->pNest;

		// �ҵ�һ�������ߵ������
		Vector3 vNewBornPos;

		vNewBornPos.x = FLOAT(IUTIL->Rand() % (pNest->nSpawnRadius * 2) - pNest->nSpawnRadius) + pNestCreature->GetCurPos().x;
		vNewBornPos.z = FLOAT(IUTIL->Rand() % (pNest->nSpawnRadius * 2) - pNest->nSpawnRadius) + pNestCreature->GetCurPos().z;

		if( GetMap()->IfCanGo(vNewBornPos.x, vNewBornPos.z) )
		{
			vNewBornPos.y = GetMap()->GetGroundHeight(vNewBornPos.x, vNewBornPos.z);
		}
		else
		{
			// û���ҵ�һ������㣬��Tick������
			return ECRR_Failed;
		}

		// ���һ������
		Vector3 vNewBornFaceTo;
		FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
		vNewBornFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
		vNewBornFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
		vNewBornFaceTo.y = 0.0f;


		// ������ĳ�������ͳ���������������ֵ
		m_vBornPos = vNewBornPos;
		m_vBornFace = vNewBornFaceTo;
	}

	// ���ͨ����������Ը�����
	OnRevive();

	return ECRR_Success;
}

//---------------------------------------------------------------------------------
// ���︴��
//---------------------------------------------------------------------------------
VOID Creature::OnRevive()
{
	// ˢ������
	RefreshAtt();
	// ˢ��AI
	RefreshAI();
	// ���¼�������
	CalInitAtt();

	if(P_VALID(m_pScript))
	{
		m_pScript->OnRespawn(this);
	}
}

//---------------------------------------------------------------------------------
// ���ҵ����������ߣ�������Ϊ��ɱ�ߣ�
//---------------------------------------------------------------------------------
Role* Creature::FindRewardRole(Unit* pSrc)
{
	Role* pReward = NULL;
	if( ECTT_Hit == m_pProto->eTaggedType )
	{
		pReward = GetMap()->FindRole(m_dwTaggedOwner);
	}
	else
	{
		if( P_VALID(pSrc) && pSrc->IsRole() )
			pReward = static_cast<Role*>(pSrc);
	}

	return pReward;
}

//---------------------------------------------------------------------------------
// ���齱��
//---------------------------------------------------------------------------------
VOID Creature::ExpReward(Role* pReward, BOOL bKill, const Team *pTeam, INT nShareNum)
{
	if( !P_VALID(pReward) ) return;

	INT nExp  = 0;

	// ��������
	FLOAT fBaseExp = (FLOAT)m_pProto->nExpGive;
	
	// ��-5<=Z-A<=5ʱ��De=100%��
	// ��Z-A<=-6��6<=Z-Aʱ��De=1-��|Z-A|-3��*10%
	FLOAT fCoef = 1.0f;
	FLOAT fLevelDist = FLOAT(abs(GetLevel() - pReward->GetLevel()));
	if(fLevelDist >= 6.0f)
	{
		fCoef = 1.0f - (fLevelDist - 3.0f)  * 0.1f;
	}


	// ȡ�����ޣ�0����1.2
	if( fCoef < 0.0f ) fCoef = 0.0f;
	if( fCoef > 1.0f ) fCoef = 1.0f;

	// �����صľ��齱��
	if(P_VALID(pTeam))
	{
		// D1=1+n*30%, ��ֵ���ڵ��ڶ�������þ�����
		FLOAT fD1 = 1.0f + ((FLOAT)nShareNum - 1.0f) * 0.3f;
		// D2=1-n*(1-�ӳ�ͳ����/999)*5%, ��ֵ���ڵ���ͳ����Ӱ��̶�
		FLOAT fD2 = 1.0f - ((FLOAT)nShareNum - 1.0f) * (1.0f - pTeam->GetMemPtr(0)->GetAttValue(ERA_Rein)/ 999.0f) * 0.05f;

		// Exp=(Xm*D1*D2/n) *De1*De2
		nExp = INT(fBaseExp * fCoef * fD1 * fD2 * pTeam->GetExpFactor() / nShareNum);

	}
	else
	{
		nExp = INT(fBaseExp * fCoef);
	}
	nExp = pReward->CalRealGainExp(nExp);
	pReward->ExpChange(nExp);
}

//--------------------------------------------------------------------------
// ���ʱ,����Ƿ��ھ���������Ч��Χ
//--------------------------------------------------------------------------
BOOL Creature::IsExpShareDistance(Role* pReward)
{
	const Vector3& vSrc = GetCurPos();
	const Vector3& vDest = pReward->GetCurPos();

	FLOAT fDistSq = Vec3DistSq(vSrc, vDest);

	if(GetMap() == pReward->GetMap() && pReward->IsDead() == FALSE &&fDistSq <= X_DEF_PICKUP_DIST_SQ)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------
// ���ʱ,����Ƿ��ڵ���������Ч��Χ
//--------------------------------------------------------------------------
BOOL Creature::IsLootShareDistance(Role* pReward)
{
	const Vector3& vSrc = GetCurPos();
	const Vector3& vDest = pReward->GetCurPos();

	FLOAT fDistSq = Vec3DistSq(vSrc, vDest);

	if(GetMapID() == pReward->GetMapID() && fDistSq <= X_DEF_PICKUP_DIST_SQ)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------
// ������ӷ������С�����(������Ҹ�����
//--------------------------------------------------------------------------
INT Creature::CalExpShareRole(const Team *pTeam, std::vector<Role*>  &vecRole)
{
	INT		nShareNum = 0;
	Role	*pReward = (Role*)GT_INVALID;
	if(P_VALID(pTeam))
	{
		INT nTeamNum = pTeam->GetMemNum();

		for(INT i = 0; i < nTeamNum; ++i)
		{
			pReward = pTeam->GetMemPtr(i);

			if(!P_VALID(pReward) || !IsExpShareDistance(pReward))
				continue;
			
			++nShareNum;
			vecRole.push_back(pReward);
		}
	}

	return nShareNum;
}

//--------------------------------------------------------------------------
// Ŀ���ʾ�����ж�
//--------------------------------------------------------------------------
DWORD Creature::TargetTypeFlag(Unit* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;

	if( pTarget->IsRole() )
		return TargetTypeFlag(static_cast<Role*>(pTarget));
	else if( pTarget->IsCreature() )
		return TargetTypeFlag(static_cast<Creature*>(pTarget));
	else
		return 0;
}

DWORD Creature::TargetTypeFlag(Role* pTarget)
{
	ASSERT( P_VALID(pTarget) );

	return ETF_Player;
}
DWORD Creature::TargetTypeFlag(Creature* pTarget)
{
	if( !P_VALID(pTarget) )
		return 0;

	if( this == pTarget )
		return ETF_Self;

	DWORD dwFlag = 0;

	// ����
	if( pTarget->IsMonster() )
	{
		// ��ͨ����
		if( pTarget->IsNormalMonster() )
		{
			if( !pTarget->IsBoss())
				dwFlag |= ETF_NormalMonster;
			else
				dwFlag |= ETF_Boss;
		}

		// ��Ѩ
		else if( pTarget->IsNest())
		{
			dwFlag |= ETF_Nest;
		}
	}

	// NPC
	else if( pTarget->IsNPC() )
	{
		dwFlag |= ETF_NPC;
	}

	// ����
	else if( pTarget->IsPet() )
	{
		dwFlag |= ETF_Pet;
	}

	// ��Դ
	else if( pTarget->IsRes() )
	{
		if ( pTarget->IsNatuRes() )
			dwFlag |= ETF_NatuRes;
		else if ( pTarget->IsManRes() )
			dwFlag |= ETF_ManRes;
		else
			ASSERT(0);
	}

	// �ɵ������
	else if( pTarget->IsInves() )
	{
		dwFlag |= ETF_InvesGameObject;
	}

	// �źͽ����ٿ����ŵ��ĸ�������ȥ

	return dwFlag;
}

//---------------------------------------------------------------------------
// ��Ŀ��ĵ��������ж�
//---------------------------------------------------------------------------
DWORD Creature::FriendEnemy(Unit* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;
	if( !P_VALID(GetMap()) || GetMap() != pTarget->GetMap() ) return 0;

	// ����Ļ����������
	if( this == pTarget )
	{
		return ETFE_Friendly | ETFE_Hostile | ETFE_Independent;
	}

	// ȡ���ڵ�ͼ�ĵ��ҹ�ϵ�ж�
	BOOL bIgnore = FALSE;
	DWORD dwMapFlag = GetMap()->GetFriendEnemyFlag(this, pTarget, bIgnore);

	// �������Ҫ�жϵ�������ģ����ж�����
	if( !bIgnore )
	{
		DWORD dwSelfFlag = 0;
		if( pTarget->IsRole() )
			dwSelfFlag = FriendEnemy(static_cast<Role*>(pTarget));
		else if( pTarget->IsCreature() )
			dwSelfFlag = FriendEnemy(static_cast<Creature*>(pTarget));
		else
			dwSelfFlag = 0;

		return (dwMapFlag | dwSelfFlag);
	}
	else
	{
		return dwMapFlag;
	}
}

DWORD Creature::FriendEnemy(Role* pTarget)
{
	DWORD dwFlag = 0;

	// ��������ǹ���
	if( IsMonster() )
	{
		dwFlag |= ETFE_Hostile;
	}

	// ���������NPC
	else if( IsNPC() )
	{
		// ������������״̬����Ϊ�з�
		if( pTarget->IsInRoleState(ERS_PK) || pTarget->IsInRoleState(ERS_PKEX) )
		{
			dwFlag |= ETFE_Hostile;
		}
		// ����Ϊ�ѷ�
		else
		{
			dwFlag |= ETFE_Friendly;
		}
	}

	// ��������ǵ����ʲô������
	else if( IsGameObject() )
	{

	}

	// ����ǳ�����Ȳ���
	else if( IsPet() )
	{

	}

	return dwFlag;
}

DWORD Creature::FriendEnemy(Creature* pTarget)
{
	DWORD dwFlag = 0;

	// ��������ǹ���
	if( IsMonster() )
	{
		if( pTarget->IsMonster() )
		{
			dwFlag |= ETFE_Friendly;
		}
		else if( pTarget->IsNPC() )
		{
			dwFlag |= ETFE_Hostile;
		}
	}

	// ���������NPC
	else if( IsNPC() )
	{
		if( pTarget->IsMonster() )
		{
			dwFlag |= ETFE_Hostile;
		}
		else if( pTarget->IsNPC() )
		{
			dwFlag |= ETFE_Friendly;
		}
	}

	// ��������ǵ���Ȳ�������
	else if( IsGameObject() )
	{

	}

	// ��������ǳ���Ȳ�������
	else if( IsPet() )
	{

	}

	return dwFlag;
}

//----------------------------------------------------------------------------------
// ������������¼�
//----------------------------------------------------------------------------------
VOID Creature::OnInvest(Role* pSrc)
{
	if(!P_VALID(m_pScript))	return;

	m_pScript->OnInvest(this, pSrc);
}

//----------------------------------------------------------------------------------
// �õ�����С��ָ��
//----------------------------------------------------------------------------------
NPCTeam* Creature::GetTeamPtr()
{
	NPCTeam* pTeam = (NPCTeam*)GT_INVALID;
	if(GT_INVALID == m_dwTeamID)
		return pTeam;

	Map* pMap = GetMap();
	if(P_VALID(pMap))
	{
		NPCTeamMgr* pTeamMgr = pMap->GetTeamMgr();
		if(P_VALID(pTeamMgr))
		{
			pTeam = pTeamMgr->GetNPCTeam(m_dwTeamID);
		}
	}
	return pTeam;
}

//----------------------------------------------------------------------------------
// �õ���Χ�����ID������������
//----------------------------------------------------------------------------------
INT Creature::GetAroundCreature(std::vector<DWORD> &vecCreature, FLOAT fOPRadius, FLOAT fHigh)
{
	// �����ΧΪ0����ֱ�ӷ���
	if( 0.0f == fOPRadius )
		return 0;

	INT nCreatureNum = 0;
	tagVisTile* pVisTile[ED_End] = {0};

	// �õ���Χ�ڵ�vistile�б�
	GetMap()->GetVisTile(GetCurPos(), fOPRadius, pVisTile);
	Creature*	pCreature	= NULL;

	for(INT n = ED_Center; n < ED_End; n++)
	{
		if( !P_VALID(pVisTile[n]) ) continue;

		// �������
		TMap<DWORD, Creature*>& mapCreature = pVisTile[n]->mapCreature;
		TMap<DWORD, Creature*>::TMapIterator it2 = mapCreature.Begin();

		while( mapCreature.PeekNext(it2, pCreature) )
		{
			if(pCreature->IsDead()) continue;

			// ������
			FLOAT fDistSQ = Vec3Dist(GetCurPos(), pCreature->GetCurPos());
			if(fDistSQ > fOPRadius)	continue;

			// ��������뵽�б���
			if(abs(pCreature->GetCurPos().y - GetCurPos().y) <= fHigh)
			{
				vecCreature.push_back(pCreature->GetID());
				nCreatureNum++;
			}
		}
	}

	return nCreatureNum;
}

//----------------------------------------------------------------------------------
// �õ���Χ��ҵ�ID������������
//----------------------------------------------------------------------------------
INT	Creature::GetAroundRole(std::vector<DWORD> &vecRole, FLOAT fOPRadius, FLOAT fHigh)
{
	// ���������ΧΪ0����ֱ�ӷ���
	if( 0.0f == fOPRadius )
		return 0;

	INT nRoleNum = 0;
	tagVisTile* pVisTile[ED_End] = {0};

	// �õ���Χ�ڵ�vistile�б�
	GetMap()->GetVisTile(GetCurPos(), fOPRadius, pVisTile);
	Role*		pRole		= NULL;

	for(INT n = ED_Center; n < ED_End; n++)
	{
		if( !P_VALID(pVisTile[n]) ) continue;

		// �������
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD, Role*>::TMapIterator it = mapRole.Begin();

		while( mapRole.PeekNext(it, pRole) )
		{
			if(pRole->IsDead()) continue;

			// ������
			FLOAT fDistSQ = Vec3DistSq(GetCurPos(), pRole->GetCurPos());
			if(fDistSQ > fOPRadius)	continue;

			// ����Ҽ��뵽�б���
			if(abs(pRole->GetCurPos().y - GetCurPos().y) <= fHigh)
			{
				vecRole.push_back(pRole->GetID());
				nRoleNum++;
			}
		}
	}

	return nRoleNum;
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID Creature::Say(DWORD dwSayID)
{
	tagNS_MonsterSay send;
	send.dwRoleID = GetID();
	send.dwSayID = dwSayID;

	if( P_VALID(GetMap()) )
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}
}

//--------------------------------------------------------------------------------
// ���Ŷ���
//--------------------------------------------------------------------------------
VOID Creature::PlayerAction(DWORD dwActionID)
{
	tagNS_MonsterPlayAction send;
	send.dwRoleID = GetID();
	send.dwActionFourCC = dwActionID;

	if( P_VALID(GetMap()) )
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}
}

//----------------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------------
VOID Creature::SetTagged(DWORD dwOwner)
{
	m_dwTaggedOwner = dwOwner;
	m_bTagged = GT_VALID(m_dwTaggedOwner);

	// ���͸��ͻ���
	tagNS_CreatureTaggedOwnerChange send;
	send.dwCreatureID = GetID();
	send.dwTaggedOwner = m_dwTaggedOwner;
	
	if( P_VALID(GetMap()) )
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}
}

//----------------------------------------------------------------------------------
// ������״̬
//----------------------------------------------------------------------------------
VOID Creature::SetDoorStatus( BOOL bOpen, BOOL bSync /*= TRUE*/ )
{
	if (!IsDoor())	return;
	if (bOpen == m_bDoorOpen)	return;
	
	// ������״̬
	m_bDoorOpen = bOpen;

	if (!bSync)		return;

	// ͬ������Χ���
	if (m_bDoorOpen)
	{
		tagNS_OpenDoor send;
		send.dwRoleID = GetID();
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}
	else
	{
		tagNS_CloseDoor send;
		send.dwRoleID = GetID();
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}
}
