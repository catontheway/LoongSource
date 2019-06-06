//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: creature.h
// author: Aslan
// actor:
// data: 2008-8-11
// last:
// brief: ��Ϸ�������࣬�������еĹ����ͨNPC���ɽ���ʽ�������ȵ�
//-----------------------------------------------------------------------------
#pragma once

#include "unit.h"

class Unit;
class Role;
class AIController;
class Skill;
class Team;
class CreatureScript;
class PathFinder;
class NPCTeam;
struct tagCreatureProto;
struct tagCreatureAI;
struct tagMapCreatureInfo;

//-----------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------
const INT	CREATURE_PATROL_REST_TICK_MIN				=	5 * TICK_PER_SECOND;		// ����Ѳ�߹�������Ϣ����Сʱ�䡪��5��
const INT	CREATURE_PATROL_REST_TICK_INTERVAL			=	10 * TICK_PER_SECOND;		// ����Ѳ�߹�������Ϣ�������10��
const INT	CREATURE_LOOK_FOR_ENEMY_REST_TICK_MIN		=	1 * TICK_PER_SECOND;		// �������еļ������Сʱ�䡪��1��
const INT	CREATURE_LOOK_FOR_ENEMY_REST_TICK_INTERVAL	=	2 * TICK_PER_SECOND;		// �������еļ�������ʱ����2��
const INT	CREATURE_CHECK_AI_EVENT_TICK				=	5;							// ������AI�¼���ʱ��������5������
const INT	CREATURE_RETURN_TICK						=	10 * TICK_PER_SECOND;		// �����ڶ�������û�иı������·��ء���10��
const INT	CREATURE_LOCK_TARGET_MIN_TICK				=	6 * TICK_PER_SECOND;		// ���﹥��ʱ����Ŀ�����Сʱ�䡪��6��
const INT	CREATURE_LOCK_TARGET_TICK_INTERVAL			=	4 * TICK_PER_SECOND;		// ���﹥��ʱ����Ŀ������ʱ�䡪��4��

const FLOAT	MAX_NPC_TALK_DISTANCE						=	20 * 50.0f;					// ��NPC˵������С����Ϊ1000�����絥λ(20������)
const FLOAT	CREATURE_PURSUE_NEAR_DIST					=	70.0f;						// ����׷����Ŀ�긽���ľ���
const INT	CREATURE_SPACE_OUT_MIN						=	15 * TICK_PER_SECOND;		// �����������������������Ϊ�����ʱ����
const INT	CREATURE_SPACE_OUT_MAX						=	30 * TICK_PER_SECOND;		// �����������������������Ϊ���ʱ����
const FLOAT CREATURE_SPACE_OUT_DIST						=	16 * 50.0f;					// �����ľ���
const FLOAT CREATURE_SPACE_OUT_CHANGE_DIST				=	4 * 50.0f;					// С�ڸþ���ʱ��������������Ϊ

//-----------------------------------------------------------------------------------
// ��������ɷ�ʽ
//-----------------------------------------------------------------------------------
enum ECreatureSpawnedType
{
	ECST_Load		=	0,			// ��ͼ�ڰڷż�������
	ECST_Nest		=	1,			// ��Ѩ����
	ECST_Dynamic	=	2,			// ��̬����
	ECST_SpawnPoint	=	3,			// ˢ�ֵ�����
	ECST_ByRole		=	4,			// �ɽ�ɫ����
	ECST_Team		=	5,			// С�Ӷӳ�����
};

//----------------------------------------------------------------------------------
// ���︴����
//----------------------------------------------------------------------------------
enum ECreatureReviveResult
{
	ECRR_Success		=	0,			// �����ɹ�
	ECRR_Failed			=	1,			// ����ʧ��
	ECRR_NeedDestroy	=	2,			// ��̬���ɣ���Ҫɾ���ڴ�
	ECRR_NeedReplace	=	3,			// ˢ�µ�֣���Ҫ�滻
};

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
class Creature : public Unit, public ScriptData<ESD_Creature>
{
	friend class AIController;

public:
	//-------------------------------------------------------------------------
	// ��̬���������ɹ���
	//-------------------------------------------------------------------------
	static Creature* Create(DWORD dwID, DWORD dwMapID, const tagCreatureProto* pProto, 
									Vector3& vPos, Vector3& vFaceTo, ECreatureSpawnedType eSpawnedType, 
									DWORD dwSpawnerID, BOOL bCollide, const tagMapWayPointInfoList* patrolList, 
									DWORD dwTeamID=GT_INVALID, DWORD dwMapObjID = GT_INVALID);

	static VOID		Delete(Creature* &pCreature);

	//--------------------------------------------------------------------------
	// Constructor&Destructor
	//--------------------------------------------------------------------------
protected:
	Creature(DWORD dwID, DWORD dwMapID, Vector3& vPos, Vector3& vFaceTo, ECreatureSpawnedType eSpawnedType, 
			 DWORD dwSpawnerID, BOOL bCollide, DWORD dwTeamID=GT_INVALID, DWORD dwMapObjID = GT_INVALID);
	virtual ~Creature();
public:
	//--------------------------------------------------------------------------
	// ��ʼ�������ٺ���
	//--------------------------------------------------------------------------
	BOOL					Init(const tagCreatureProto* pProto, const tagMapWayPointInfoList* patrolList);
	VOID					Destroy();

	//--------------------------------------------------------------------------
	// �������
	//--------------------------------------------------------------------------
	BOOL					IsTagged() const			{ return m_bTagged; }
	VOID					SetTagged(DWORD dwOwner);

	//--------------------------------------------------------------------------
	// С�����
	//--------------------------------------------------------------------------
	DWORD					GetTeamID()					{ return m_dwTeamID; }
	VOID					SetTeamID(DWORD dwID)		{ m_dwTeamID = dwID; }
	NPCTeam*				GetTeamPtr();

	//--------------------------------------------------------------------------
	// �õ�����
	//--------------------------------------------------------------------------
	const tagCreatureProto*	GetProto() const		{ return m_pProto; }
	AIController*			GetAI()	const			{ return m_pAIController; }
	ECreatureType			GetType() const			{ return m_pProto->eType; }
	DWORD					GetSpawnPtID() const	{ return m_dwSpawnerID;}
	ECreatureTaggedType		GetTaggedType()	const	{ return m_pProto->eTaggedType; }
	DWORD					GetTaggedOwner() const	{ return m_dwTaggedOwner; }
	DWORD					GetLootID()	const		{ return m_pProto->dwLootID; } 
	DWORD					GetShopID() const		{ return m_pProto->uFunctionID.dwShopID; }
	DWORD					GetDakID()	const		{ return m_pProto->uFunctionID.dwDakID; }

	const Vector3&			GetBornPos() const		{ return m_vBornPos; }
	const Vector3&			GetBornFace() const		{ return m_vBornFace; }

	//-----------------------------------------------------------------------------
	// �����ж�
	//-----------------------------------------------------------------------------
	BOOL					IsMonster()	const		{ return GetType() == ECT_Monster; }
	BOOL					IsNPC()	const			{ return GetType() == ECT_NPC; }
	BOOL					IsGameObject() const	{ return GetType() == ECT_GameObject; }
	BOOL					IsPet()	const			{ return GetType() == ECT_Pet; }

	BOOL					IsNormalMonster() const	{ return IsMonster() && (EMTT_Normal == m_pProto->nType2 || EMTT_Team == m_pProto->nType2); }
	BOOL					IsNest() const			{ return IsMonster() && EMTT_Nest == m_pProto->nType2; }
	BOOL					IsTeam() const			{ return IsMonster() && EMTT_Team == m_pProto->nType2; }
	BOOL					IsBoss() const			{ return IsNormalMonster() && ER_Rare == m_pProto->eLite; }

	BOOL					IsRes()	const			{ return IsGameObject() && EGOT_Gather == m_pProto->nType2; }
	BOOL					IsInves() const			{ return IsGameObject() && (EGOT_QuestInves == m_pProto->nType2 || EGOT_CommonInves == m_pProto->nType2); }
	BOOL					IsDoor() const			{ return IsGameObject() && (EGOT_Door == m_pProto->nType2); }

	BOOL					IsNatuRes()	const		{ return IsRes() && (EGT_Mine == m_pProto->nType3 || EGT_Herb == m_pProto->nType3); }
	BOOL					IsManRes()	const		{ return IsRes() && (EGT_Food == m_pProto->nType3 || EGT_Cloth == m_pProto->nType3); }

	//-----------------------------------------------------------------------------
	// NPCְ��
	//-----------------------------------------------------------------------------
	BOOL					IsFunctionNPC(EFunctionNPCType eFuncType) const	{ return eFuncType == m_pProto->eFunctionType; }

	//-----------------------------------------------------------------------------
	// ��ײ
	//-----------------------------------------------------------------------------
	BOOL					NeedCollide() const			{ return m_bCollide; }
	VOID					SetCollide(BOOL bCollide)	{ m_bCollide = bCollide; }

	//-----------------------------------------------------------------------------
	// �ű�
	//-----------------------------------------------------------------------------
	const CreatureScript*	GetScript() const		{ return m_pScript; }

	//--------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------
	virtual VOID			Update();

	//--------------------------------------------------------------------------
	// �õ�����ID��������Ҫ��̬ʵ��
	//--------------------------------------------------------------------------
	virtual DWORD			GetTypeID()						{ return m_pProto->dwTypeID; }

	//--------------------------------------------------------------------------
	// �õ���ͼ����ID(��)
	//--------------------------------------------------------------------------
	const DWORD				GetMapObjID()					{ return m_dwMapObjID; }

	//--------------------------------------------------------------------------
	// �õ��Ա���Һ������ߵĲ���ͬһ��
	//--------------------------------------------------------------------------
	virtual BYTE			GetSex() const					{ return m_pProto->bySex; }

	//--------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------
	Skill*					GetMeleeSkill()					{ return m_pMeleeSkill; }
	Skill*					GetRangedSkill()				{ return m_pRangedSkill; }

	//--------------------------------------------------------------------------
	// ״̬���
	//--------------------------------------------------------------------------
	virtual BOOL			IsInStateCantMove()				{ return Unit::IsInStateCantMove() || GetCombatHandler().IsValid(); }
	virtual BOOL			IsInStateCantCastSkill()		{ return Unit::IsInStateCantCastSkill() || (FALSE == m_pProto->bCanAttack); }
	virtual BOOL			IsInStateCantBeSkill()			{ return !m_pProto->bCanBeAttack; }
	virtual BOOL			IsInStateInvisible()			{ return Unit::IsInStateInvisible() || (FALSE == m_pProto->bVisble); }

	//--------------------------------------------------------------------------
	// �������
	//--------------------------------------------------------------------------
	BOOL					IsExpShareDistance(Role* pReward);
	BOOL					IsLootShareDistance(Role* pReward);
	INT						CalExpShareRole(const Team* pTeam, std::vector<Role*>  &vecRole);
	BOOL					CheckNPCTalkDistance(Role* pRole);

	//--------------------------------------------------------------------------
	// ս��״̬���
	//--------------------------------------------------------------------------
	virtual VOID			OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited);
	virtual VOID			OnDead(Unit* pSrc, Skill* pSkill=NULL, const tagBuffProto* pBuff=NULL, DWORD dwSerial=GT_INVALID, DWORD dwMisc=0);
	virtual VOID			OnKill(Unit* pTarget);
	VOID					OnInvest(Role* pSrc);
	VOID					OnDisappear();
	VOID					OnAIAttack();
	VOID					OnBuffInjury(Unit* pSrc);

	//--------------------------------------------------------------------------
	// ս���ж�
	//--------------------------------------------------------------------------
	virtual DWORD			TargetTypeFlag(Unit* pTarget);
	virtual DWORD			FriendEnemy(Unit* pTarget);

	//--------------------------------------------------------------------------
	// ս������
	//--------------------------------------------------------------------------
	INT						GetAroundCreature(std::vector<DWORD> &vecCreature, FLOAT fOPRadius, FLOAT fHigh);
	INT						GetAroundRole(std::vector<DWORD> &vecRole, FLOAT fOPRadius, FLOAT fHigh);	

	//--------------------------------------------------------------------------
	// �������
	//--------------------------------------------------------------------------
	ECreatureReviveResult	TryRevive();
	VOID					OnRevive();
	ECreatureRespawnType	GetRespawnType()					{ return m_pProto->eRespawnType; }
	INT						GetRespawnTick()					{ return m_pProto->nRespawnTick; }
	INT						GetRespawnCountDown()				{ return m_nRespawnTickCountDown; }
	VOID					SetRespawnCountDown(INT nTick)		{ m_nRespawnTickCountDown = nTick; }
	VOID					CountDownRespawnTick()				{ m_nRespawnTickCountDown -= 1; }

	//--------------------------------------------------------------------------
	// �������
	//--------------------------------------------------------------------------
	VOID					Say(DWORD dwSayID);
	VOID					PlayerAction(DWORD dwActionID);

	//--------------------------------------------------------------------------
	// ��
	//--------------------------------------------------------------------------
	BOOL					GetDoorStatus()		{ return m_bDoorOpen; }
	VOID					SetDoorStatus(BOOL bOpen, BOOL bSync = TRUE);
private:
	//--------------------------------------------------------------------------
	// ��ʼ��
	//--------------------------------------------------------------------------
	BOOL					InitAtt(const tagCreatureProto* pProto);
	BOOL					InitAI(const tagCreatureProto* pProto, const tagMapWayPointInfoList* patrolList);

	//-------------------------------------------------------------------------
	// �����ʼ���ﵱǰ����
	//--------------------------------------------------------------------------
	VOID					CalInitAtt();

	//--------------------------------------------------------------------------
	// ����ʱ��ˢ�º���
	//--------------------------------------------------------------------------
	VOID					RefreshAtt();
	VOID					RefreshAI();

	//--------------------------------------------------------------------------
	// ���ָ���
	//--------------------------------------------------------------------------
	VOID					UpdateAI();
	VOID					UpdateLiveTime();
	VOID					UpdateCombat();

	//--------------------------------------------------------------------------
	// ���Ըı�
	//--------------------------------------------------------------------------
	virtual VOID			OnAttChange(INT nIndex);
	virtual VOID			OnAttChange(bool bRecalFlag[ERA_End]);

	//--------------------------------------------------------------------------
	// ս���ж�
	//--------------------------------------------------------------------------
	DWORD					TargetTypeFlag(Role* pTarget);
	DWORD					TargetTypeFlag(Creature* pTarget);
	DWORD					FriendEnemy(Role* pTarget);
	DWORD					FriendEnemy(Creature* pTarget);

	//--------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------
	Role*					FindRewardRole(Unit* pSrc);
	VOID					ExpReward(Role* pReward, BOOL bKill=FALSE, const Team *pTeam=(Team*)GT_INVALID, INT nShareNum=1);

private:
	//---------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------
	const tagCreatureProto*					m_pProto;					// ��̬����
	AIController*							m_pAIController;			// AI������
	BOOL									m_bTagged;					// �����Ƿ��ѱ�����
	DWORD									m_dwTaggedOwner;			// ������

	Vector3									m_vBornPos;					// ������
	Vector3									m_vBornFace;				// ��������
	BOOL									m_bCollide;					// �Ƿ������ײ

	INT										m_nLiveTick;				// ����ʱ��
	INT										m_nRespawnTickCountDown;	// ���������ʱ

	ECreatureSpawnedType					m_eSpawnedType;				// ���ɷ�ʽ�����أ���Ѩ����̬��
	DWORD									m_dwSpawnerID;				// ����ǳ�Ѩ���߶�̬�����ģ����¼������ID������ID���������ˢ�µ㴴���ģ����¼ˢ�µ�id

	BOOL									m_bDoorOpen;				// �ŵ�״̬
	DWORD									m_dwMapObjID;				// �����ڵ�ͼ�е�ObjID

	//----------------------------------------------------------------------------
	// �ű�
	//----------------------------------------------------------------------------
	const CreatureScript*					m_pScript;

	//----------------------------------------------------------------------------
	// ����С��
	//----------------------------------------------------------------------------
	DWORD									m_dwTeamID;				// ����С��ID

	//----------------------------------------------------------------------------
	// ���＼��
	//----------------------------------------------------------------------------
	Skill*									m_pMeleeSkill;			// ��������ͨ����
	Skill*									m_pRangedSkill;			// Զ�̹�����ͨ����
};

//---------------------------------------------------------------------------------
// ���Ըı�
//---------------------------------------------------------------------------------
inline VOID Creature::OnAttChange(INT nIndex)
{
	switch(nIndex)
	{
		// Ѫ��
	case ERA_HP:
		m_nAtt[ERA_HP] = min(m_nAtt[ERA_HP], m_nAtt[ERA_MaxHP]);
		break;

		// ����
	case ERA_MP:
		m_nAtt[ERA_MP] = min(m_nAtt[ERA_MP], m_nAtt[ERA_MaxMP]);
		break;

		// ����
	case ERA_Vitality:
		m_nAtt[ERA_Vitality] = min(m_nAtt[ERA_Vitality], m_nAtt[ERA_MaxVitality]);
		break;

		// �־���
	case ERA_Endurance:
		m_nAtt[ERA_Endurance] = min(m_nAtt[ERA_Endurance], m_nAtt[ERA_MaxEndurance]);
		break;

	default:
		break;
	}
}

inline VOID Creature::OnAttChange(bool bRecalFlag[ERA_End])
{
	if( bRecalFlag[ERA_Speed_XZ] )
	{
		m_fXZSpeed = X_DEF_XZ_SPEED * (FLOAT(m_nAtt[ERA_Speed_XZ]) / 10000.0f);
		m_MoveData.StopMove();
	}

	if( bRecalFlag[ERA_Shape] )
	{
		m_Size = m_pProto->vSize * (FLOAT(m_nAtt[ERA_Shape]) / 10000.0f);
	}
}

