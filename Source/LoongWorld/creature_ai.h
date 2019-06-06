//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: creature_ai.h
// author: Aslan
// actor:
// data: 2008-09-05
// last:
// brief: ��Ϸ������AI�࣬һ������״̬������������AI״̬
//-----------------------------------------------------------------------------
#pragma once

#include "ai_transition.h"
#include "ai_trigger.h"
#include "script_mgr.h"
#include "creature.h"
#include "creature_order.h"

class Unit;
class Creature;
class AIState;
class AITransition;
class AITriggerMgr;
class CVector2D;
struct tagCreatureAI;
struct tagTriggerProto;
enum  EFLEETIME;

const FLOAT CREATURE_RETURN_HEIGH = 100.0;

//-----------------------------------------------------------------------------
// AI״̬����
//-----------------------------------------------------------------------------
enum AIStateType
{
	AIST_Idle,				// ����״̬
	AIST_Pursue,			// ׷��״̬
	AIST_Attack,			// ����״̬
	AIST_Flee,				// ����״̬
	AIST_SOS,				// ����״̬
	AIST_Return,			// ����״̬
	AIST_Follow,			// ����״̬
	AIST_SpaceOut,			// ��������
};

//-----------------------------------------------------------------------------
// AI״̬������
//-----------------------------------------------------------------------------
class AIState
{
public:
	virtual ~AIState() {}

	// ����״̬����
	virtual VOID OnEnter(AIController* pAI) = 0;
	// �뿪״̬����
	virtual VOID OnExit(AIController* pAI) = 0;
	// ���º���
	virtual VOID Update(AIController* pAI) = 0;
	// �¼���Ӧ
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1=0, DWORD dwEventMisc2=0) = 0;
};

//-----------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------
class AIStateIdle : public AIState
{
public:
	virtual ~AIStateIdle() {}

public:
	AIStateIdle() {}
	static AIStateIdle* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);	// �����᲻��̫����

private:
	AIStateIdle(const AIStateIdle&);
	AIStateIdle& operator=(const AIStateIdle&);
};

//-----------------------------------------------------------------------------
// ׷��״̬
//-----------------------------------------------------------------------------
class AIStatePursue: public AIState
{
public:
	AIStatePursue() {}
	virtual ~AIStatePursue() {}

public:
	static AIStatePursue* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);

private:
	AIStatePursue(const AIStatePursue&);
	AIStatePursue& operator=(const AIStatePursue&);
};

//-----------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------
class AIStateAttack: public AIState
{
public:
	AIStateAttack() {}
	virtual ~AIStateAttack() {}

public:
	static AIStateAttack* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);

private:
	AIStateAttack(const AIStateAttack&);
	AIStateAttack& operator=(const AIStateAttack&);
};

//-----------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------
class AIStateFlee: public AIState
{
public:
	AIStateFlee() {}
	virtual ~AIStateFlee() {}

public:
	static AIStateFlee* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);

private:
	AIStateFlee(const AIStateFlee&);
	AIStateFlee& operator=(const AIStateFlee&);
};

//-----------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------
class AIStateCallHelp : public AIState
{
public:
	AIStateCallHelp() {}
	virtual ~AIStateCallHelp() {}

public:
	static AIStateCallHelp* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);

private:
	AIStateCallHelp(const AIStateCallHelp&);
	AIStateCallHelp& operator=(const AIStateCallHelp&);
};

//-----------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------
class AIStateReturn : public AIState
{
public:
	AIStateReturn() {}
	virtual ~AIStateReturn() {}

public:
	static AIStateReturn* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);

private:
	AIStateReturn(const AIStateReturn&);
	AIStateReturn& operator=(const AIStateReturn&);
};

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
class AIStateSpaceOut : public AIState
{
public:
	AIStateSpaceOut() {}
	virtual ~AIStateSpaceOut() {}

public:
	static AIStateSpaceOut* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc/* =0 */, DWORD dwEventMisc2/* = 0 */);

private:
	AIStateSpaceOut(const AIStateSpaceOut&);
	AIStateSpaceOut& operator=(const AIStateSpaceOut&);
};

//-----------------------------------------------------------------------------
// ����״̬��һ����Ϊȫ��״̬
//-----------------------------------------------------------------------------
class AIStateFollow: public AIState
{
public:
	AIStateFollow() {}
	virtual ~AIStateFollow() {}

public:
	static AIStateFollow* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);

private:
	AIStateFollow(const AIStateFollow&);
	AIStateFollow& operator=(const AIStateFollow&);
};

//-----------------------------------------------------------------------------
// �ű�״̬���������нű�
//-----------------------------------------------------------------------------
class AIStateScript: public AIState
{
public:
	AIStateScript() {}
	virtual ~AIStateScript() {}

public:
	static AIStateScript* Instance();

	virtual VOID OnEnter(AIController* pAI);
	virtual VOID OnExit(AIController* pAI);
	virtual VOID Update(AIController* pAI);
	virtual BOOL OnEvent(AIController* pAI, Unit* pSrc, INT nEventType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */);

private:
	AIStateScript(const AIStateFollow&);
	AIStateScript& operator=(const AIStateScript&);
};

//-----------------------------------------------------------------------------
// AI��������Ϊһ����״̬��
//-----------------------------------------------------------------------------
class AIController
{
public:
	explicit AIController(Creature* pCreature, const tagMapWayPointInfoList* patrolList);
	~AIController() { Destroy(); }

	//--------------------------------------------------------------------------
	// ��ʼ���͸���
	//--------------------------------------------------------------------------
	VOID			InitPatrol(const tagMapWayPointInfoList* patrolList);
	VOID			InitAIProto();
	VOID			InitTriggerMgr();
	VOID			InitAITransition();
	VOID			InitPathFinder();

	VOID			Update();
	VOID			Destroy();
	VOID			Refresh();

	//--------------------------------------------------------------------------
	// �¼�����
	//--------------------------------------------------------------------------
	VOID			OnEvent(Unit* pSrc, INT nEventType, DWORD dwEventMisc1=0, DWORD dwEventMisc2=0);

	//--------------------------------------------------------------------------
	// �õ�����
	//--------------------------------------------------------------------------
	Creature*		GetOwner()								{ return m_pOwner; }

	//--------------------------------------------------------------------------
	// ״̬���
	//--------------------------------------------------------------------------
	AIStateType		GetCurrentStateType()					{ return m_eCurAIState; }
	AIState*		GetCurrentState()						{ return m_pCurrentState; }
	AIState*		GetPreviousState()						{ return m_pPreState; }
	AIState*		GetGlobalState()						{ return m_pGlobalState; }

	VOID			SetCurrentStateType(AIStateType eType)	{ m_eCurAIState = eType; }
	VOID			SetCurrentState(AIState* pState)		{ m_pCurrentState = pState; }
	VOID			SetPreviousState(AIState* pState)		{ m_pPreState = pState; }
	VOID			SetGlobalState(AIState* pState)			{ m_pGlobalState = pState; }

	VOID			ChangeState(AIStateType eState);

	//---------------------------------------------------------------------------
	// ս���߼����
	//---------------------------------------------------------------------------

	// ͨ��
	DWORD			GetEnterCombatTick() const			{ return m_dwEnterCombatTick; }
	const Vector3&	GetEnterCombatPos() const			{ return m_vPosEnterCombat; }
	VOID			SetEnterCombatPos(Vector3& vPos)	{ m_vPosEnterCombat = vPos; }
	VOID			SetEnterCombatTick(DWORD dwTick)	{ m_dwEnterCombatTick = dwTick; }
	BOOL			IsTargetValid(DWORD dwID);
	DWORD			GetTargetIDByType(ECreatureTargetFriendEnemy eTargetType);

	// Ѳ��
	BOOL			IsReversePatrol() const				{ return m_bReversePatrol; }
	BOOL			IsPatroling() const					{ return m_bIsPatroling; }
	VOID			SetIsPatroling(BOOL bPatrol)		{ m_bIsPatroling = bPatrol; }
	VOID			SetIsReversePatrol(BOOL bReverse)	{ m_bReversePatrol = bReverse; }
	VOID			ReSetPatrolRestTick()				{ m_nPatrolRestTick = IUTIL->Rand() % CREATURE_PATROL_REST_TICK_INTERVAL + CREATURE_PATROL_REST_TICK_MIN; }
	VOID			UpdatePatrol();
	VOID			StartPatrol();

	// ����
	VOID			ReSetLookForTargetTick()			{ m_nLookForTargetTick = IUTIL->Rand() % CREATURE_LOOK_FOR_ENEMY_REST_TICK_INTERVAL + CREATURE_LOOK_FOR_ENEMY_REST_TICK_MIN; }
	DWORD			UpdateLookForEnemy();
	DWORD			StartLookForEnemy();
	BOOL			UpdateLockTarget();

	// ׷��
	const Vector3&	GetPursueTargetPos() const			{ return m_vPursueTargetPos; }
	DWORD			GetTargetUnitID() const				{ return m_dwTargeUnitID; }
	Unit*			GetPursueTarget();
	BOOL			GetPosNearPursueTarget(Unit* pTarget, Vector3& vPos);
	VOID			StartPursue(Unit* pTarget);
	VOID			UpdatePursue(Unit* pTarget);
	VOID			SetPursueTargetPos(Vector3& vPos)	{ m_vPursueTargetPos = vPos; }
	VOID			SetTargetUnitID(DWORD dwID)			{ m_dwTargeUnitID = dwID; }
	VOID			ReSetLockTargetTime()				{ m_nLockTargetTick = IUTIL->Rand() % CREATURE_LOCK_TARGET_TICK_INTERVAL + CREATURE_LOCK_TARGET_MIN_TICK; }

	// ����
	BOOL			IsInAttackDist(DWORD dwTargetUnitID);
	INT				GetNextAttackWaitTick()	const		{ return m_nNextAttackWaitTick; }
	VOID			SetNextAttackWaitTick(INT nTick)	{ m_nNextAttackWaitTick = nTick; }
	VOID			CountDownNextAttackWaitTick()		{ --m_nNextAttackWaitTick; }
	INT				GetCheckAIEvnetTick() const			{ return m_nCheckAIEventTick; }
	VOID			SetCheckAIEventTick(INT nTick)		{ m_nCheckAIEventTick = nTick; }
	INT				AIUseSkill(DWORD dwSkillID, DWORD dwTargetUnitID);
	DWORD			GetAIUseSkillID()					{ return m_dwUseSkillID; }

	// ����
	const Vector3&	GetFleePos() const					{ return m_vFleePos; }
	VOID			SetFleePos(Vector3 vPos)			{ m_vFleePos = vPos; }
	BOOL			IsArrivedFleePos()					{ return m_bArrivedFleePos; }
	VOID			SetIsArrivedFleePos(BOOL bArrived)	{ m_bArrivedFleePos = bArrived; }
	VOID			SetFleeTime(EFLEETIME eFleeTime)	{ m_eFleeTime = eFleeTime; }
	EFLEETIME		GetFleeTime()						{ return m_eFleeTime; }
	VOID			CountDownNextFleeTick()				{ --m_dwFleeTick; }
	DWORD			GetFleeTick()						{ return m_dwFleeTick; }
	VOID			SetFleeTick(DWORD dwFleeTick)		{ m_dwFleeTick = dwFleeTick; }                                                                     
	VOID			CalFleePos();
	VOID			StartFlee(Vector3 vFleePos);
	VOID			UpdateFlee();

	// ����
	BOOL			CalHelpPos();
	DWORD			GetHelperID()						{ return m_dwHelperID; }
	VOID			SetHelperID(DWORD dwHelperID)		{ m_dwHelperID = dwHelperID; }
	BOOL			NeedReCalHelpPos();

	// ���־���
	BOOL			CalSpaceOutPos();
	VOID			StartSpaceOut();
	VOID			UpdateSpaceOut();
	DWORD			GetSpaceOutTick()					{ return m_dwSpaceOutTick; }
	BOOL			IsArrivedPos()						{ return m_bArrivedPos; }
	VOID			SetIfArrivedPos(BOOL bArrived)		{ m_bArrivedPos = bArrived; }

	// ����
	VOID			StartReturn();
	VOID			UpdateReturn();
	BOOL			IsArrivedReturnPos()				{ return m_bArrivedReturnPos; }

	//---------------------------------------------------------------------------
	// AIת�������
	//---------------------------------------------------------------------------
	AITransition*	GetTransition()						{ return m_pTransition; }

	//----------------------------------------------------------------------------
	// ���������������
	//----------------------------------------------------------------------------
	AITriggerMgr*	GetAITriggerMgr()					{ return m_pAITrigger; }

	//----------------------------------------------------------------------------
	// ������
	//----------------------------------------------------------------------------
	DWORD			RandRoleInEnmityList();
	DWORD			GetMinEnmityInEnmityList();
	VOID			AddEnmity(Unit *pUnit, INT nValue, BOOL bSyncTeam=TRUE);
	VOID			AddEnmityMod(Unit *pUnit, INT nValue, BOOL bSyncTeam=TRUE);
	VOID			ClearAllEnmity();
	VOID			ClearEnmity(DWORD dwRoleID);
	VOID			SetEnmityActive(DWORD dwRoleID, BOOL bActive);
	VOID			CalMaxEnmity();
	DWORD			GetMaxEnmityUnitID()				{ return m_dwMaxEnmityUnitID; }
	INT				GetEnmityValue(DWORD dwID);
	INT				GetBaseEnmityValue(DWORD dwID);
	VOID			ClearEnmityModValue(DWORD dwID);
	VOID			SetMaxEnmityUnitID(DWORD dwID)	{ m_dwMaxEnmityUnitID = dwID; }
	BOOL			IsEnmityListAllInvincible();

	//----------------------------------------------------------------------------
	// �ű����
	//----------------------------------------------------------------------------
	VOID			SetScriptUpdateTimer(INT nTimer)	{ m_nScriptUpdateTimer = nTimer; }

private:
	//----------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------
	VOID			UpdateAIController();
	VOID			UpdateTransition();
	VOID			UpdateCurrentState();
	VOID			UpdateTriggerMgr();

	//----------------------------------------------------------------------------
	// ״̬���
	//----------------------------------------------------------------------------
	AIState*		GetStateByType(AIStateType eStateType);

	//----------------------------------------------------------------------------
	// ״̬ת�������
	//----------------------------------------------------------------------------
	AITransition*	GetTransitionByType();

	//----------------------------------------------------------------------------
	// ��������
	//----------------------------------------------------------------------------
	BOOL			IsInStateCantUpdateAI();

private:
	Creature*				m_pOwner;					// ��Ӧ������

	//----------------------------------------------------------------------------
	// ״̬
	//----------------------------------------------------------------------------
	AIStateType				m_eCurAIState;				// ��ǰAI״̬ö��
	AIState*				m_pCurrentState;			// ��ǰAI״̬
	AIState*				m_pPreState;				// ��һ�ε�״̬
	AIState*				m_pGlobalState;				// ȫ��AI״̬

	//----------------------------------------------------------------------------
	// ״̬ת����
	//----------------------------------------------------------------------------
	AITransition*			m_pTransition;				// AI״̬ת����

	//----------------------------------------------------------------------------
	// ������������
	//----------------------------------------------------------------------------
	AITriggerMgr*			m_pAITrigger;				// AI������������

	//----------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------
	const tagCreatureProto*	m_pProto;					// �������Ӧ�ľ�̬����
	const tagCreatureAI*	m_pAIProto;					// �������Ӧ��AI��̬����

	//----------------------------------------------------------------------------
	// �ű�
	//----------------------------------------------------------------------------
	const CreatureScript*	m_pScript;					// ����ű�
	INT						m_nScriptUpdateTimer;		// ����ű�����AI��ʱ�䣬��ʼ��ʱΪGT_INVALID�������£��ɽű����������ʱ��
	INT						m_nScriptUpdateCountDown;	// ����ű�����AI����ʱ

	//-----------------------------------------------------------------------------
	// AIս���߼�����
	//-----------------------------------------------------------------------------

	// ����״̬
	Vector3*				m_pWPArray;					// ·��Ѳ�߹ֵ�Ѳ�ߵ������б�
	INT						m_nWPNum;					// ���м���������
	INT						m_nWPIndex;					// ��ǰѲ�����ĸ�������
	BOOL					m_bReversePatrol;			// ·��Ѳ�߹��Ƿ����������෴·��Ѳ��
	BOOL					m_bIsPatroling;				// ����Ѳ��
	INT						m_nPatrolRestTick;			// ������һ��·������Ϣ��Tick
	INT						m_nLookForTargetTick;		// ��һ�����е�Tick

	// ׷���͹���
	Vector3					m_vPursueTargetPos;			// ����׷����ǰ��Ŀ���
	BOOL					m_bPursueFailed;			// ׷��ʧ��
	BOOL					m_bCanNotPursue;			// ��ʱ����׷��
	BOOL					m_bPathFinding;				// Ѱ·��
	INT						m_nCheckPursueTick;			// �����ѯ�ƶ���ʱ����

	DWORD					m_dwTargeUnitID;			// Ŀ�����ID
	INT						m_nCheckAIEventTick;		// ��һ�μ��AI�¼���tick
	DWORD					m_dwEnterCombatTick;		// ����ս����tick

	DWORD					m_dwUseSkillID;				// ʹ�õļ���ID
	INT						m_nNextAttackWaitTick;		// ���ι�����ɺ��´ι�����Ҫ�ȴ���ʱ��
	DWORD					m_dwSerial;					// �������к�

	// ����
	Vector3					m_vFleePos;					// ���ܵ�Ŀ���
	CVector2D				m_vFleeDir;					// ���ܵķ���
	DWORD					m_dwFleeTick;				// ����״̬����ʱ��
	BOOL					m_bArrivedFleePos;			// �Ƿ񵽴����ܵ�Ŀ���
	EFLEETIME				m_eFleeTime;				// ��������״̬�Ĵ���
	DWORD					m_dwHelperID;				// ��ȶ���ID

	// ����
	Vector3					m_vPosEnterCombat;			// ����ս��ʱ������
	BOOL					m_bArrivedReturnPos;		// �Ƿ񷵻ص�����ս��ʱ������

	// ���־���
	DWORD					m_dwSpaceOutTick;			// ���־��봥�����
	BOOL					m_bArrivedPos;				// �Ƿ񵽴�Ŀ���

	//-----------------------------------------------------------------------
	// ���ϵͳ
	//-----------------------------------------------------------------------
	TMap<DWORD, tagEnmity*>	m_mapEnmity;				// ����б�
	DWORD					m_dwMaxEnmityUnitID;		// �����ID
	INT						m_nLockTargetTick;			// ����Ŀ�굹��ʱ
	INT						m_nNoAnmityTick;			// û�����ӳ�޵�������

	//-----------------------------------------------------------------------
	// Ѱ·���
	//-----------------------------------------------------------------------
	PathFinder*				m_pPathFinder;
};

//---------------------------------------------------------------------------
// �ı䵱ǰAI״̬
//----------------------------------------------------------------------------
inline VOID AIController::ChangeState(AIStateType eState)
{
	if( P_VALID(m_pCurrentState) )
		m_pCurrentState->OnExit(this);

	m_eCurAIState = eState;
	m_pCurrentState = GetStateByType(eState);

	if( P_VALID(m_pCurrentState) )
		m_pCurrentState->OnEnter(this);
}

//------------------------------------------------------------------------------------------
// ����״̬���͵õ�״̬����
//------------------------------------------------------------------------------------------
inline AIState* AIController::GetStateByType(AIStateType eStateType)
{
	switch (eStateType)
	{
	case AIST_Idle:
		return AIStateIdle::Instance();

	case AIST_Pursue:
		return AIStatePursue::Instance();

	case AIST_Attack:
		return AIStateAttack::Instance();

	case AIST_Return:
		return AIStateReturn::Instance();

	case AIST_Flee:
		return AIStateFlee::Instance();

	case AIST_SOS:
		return AIStateCallHelp::Instance();

	case AIST_SpaceOut:
		return AIStateSpaceOut::Instance();

	default:
		return AIStateScript::Instance();
	}
}

//------------------------------------------------------------------------------------------
// ����״̬��
//------------------------------------------------------------------------------------------
inline VOID AIController::UpdateAIController()
{
	// ����޽ű���������
	if( !P_VALID(m_pScript) ) return;

	// ��������Ƿ��Ѿ���ͣ
	if( GT_INVALID == m_nScriptUpdateTimer ) return;

	// ����ʱ
	if( --m_nScriptUpdateCountDown > 0 ) return;

	// ���ýű�
	m_nScriptUpdateCountDown = m_nScriptUpdateTimer;
	m_pScript->OnUpdateAI(m_pOwner);
}

//------------------------------------------------------------------------------------------
// ����״̬�л�
//------------------------------------------------------------------------------------------
inline VOID AIController::UpdateTransition()
{
	if( !P_VALID(m_pTransition) ) return;

	// ���״̬�Ƿ�ת��
	AIStateType eNewState = m_pTransition->Transition(this, m_eCurAIState);

	if( m_eCurAIState != eNewState )
	{
		ChangeState(eNewState);
	}
}

//------------------------------------------------------------------------------------------
// ���µ�ǰ״̬
//------------------------------------------------------------------------------------------
inline VOID AIController::UpdateCurrentState()
{
	// ���µ�ǰAI״̬
	if( P_VALID(m_pCurrentState) )
		m_pCurrentState->Update(this);

	// ����ȫ��AI״̬
	if( P_VALID(m_pGlobalState) )
		m_pGlobalState->Update(this);
}

//------------------------------------------------------------------------------------------
// ���´�����
//------------------------------------------------------------------------------------------
inline VOID AIController::UpdateTriggerMgr()
{
	// ���´�����
	if( P_VALID(m_pAITrigger) && !m_pAITrigger->IsPaused() )
	{
		m_pAITrigger->Update();
	}
}

//------------------------------------------------------------------------------------------
// ���ڲ��ܸ���AI��״̬
//------------------------------------------------------------------------------------------
inline BOOL AIController::IsInStateCantUpdateAI()
{
	return m_pOwner->IsInState(ES_Dead) || m_pOwner->IsInState(ES_Dizzy); // || ����
}
