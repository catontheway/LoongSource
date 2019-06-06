//--------------------------------------------------------------------------------
//!\file script_mgr.h
//!\author Aslan
//!
//!\date 2008-09-01
//! last 2008-09-01
//!
//!\brief Lua�ű�����������Ҫ������ֽű�����֯�����ɼ�����
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//--------------------------------------------------------------------------------
#pragma once

extern "C"
{
#include "../lua/src/lua.h"
#include "../lua/src/lauxlib.h"
#include "../lua/src/lualib.h"
};
#include "mutex.h"
#include "event_mgr.h"
#include "../ServerDefine/activity_define.h"
#include "../WorldDefine/penalty_define.h"

class Unit;
class Creature;
class Role;
class CActivity;
class ScriptMgr;
class Skill;

struct tagMapTriggerInfo;
struct tagMapAreaInfo;

enum ERoleReviveType;

//-------------------------------------------------------------------------------
// ����ű��¼�ö��
//-------------------------------------------------------------------------------
enum EScriptQuestEvent
{
	ESQE_On_Accept			=	0,			// �����ȡ
	ESQE_On_Complete		=	1,			// �������
	ESQE_On_Cancel			=	2,			// ����ȡ��
	ESQE_On_Creature_Kill	=	3,			// ɱ��һֻ����
	ESQE_On_CheckAccept		=	4,			// ����ȡ
	ESQE_On_CheckComplete	=	5,			// ��⽻
	ESQE_On_NPC_Talk		=	6,			// ��NPC�Ի�
	ESQE_On_Init			=	7,			// �����ʼ��
	ESQE_On_Dlg_Default		=	8,			// �������ɿضԻ���ȱʡ�¼�
	ESQE_On_Invest			=	9,			// �������		
	ESQE_End				=	10,
};

//---------------------------------------------------------------------------------
// ����ű��¼�ö��
//---------------------------------------------------------------------------------
enum EScriptCreatureEvent
{
	// �ճ��¼�
	ESCAE_On_Load			=	0,			// ����
	ESCAE_On_Respawn		=	1,			// ����
	ESCAE_On_Enter_Combat	=	2,			// ����ս��
	ESCAE_On_Leave_Combat	=	3,			// �뿪ս��
	ESCAE_On_Die			=	4,			// ����
	ESCAE_On_Timer			=	5,			// ��ʱ����
	ESCAE_On_Invest			=	6,			// ������
	ESCAE_On_Talk			=	7,			// ��˵��

	// AI�¼�
	ESCAE_On_UpdateAI		=	8,			// ����AI״̬��
	ESCAE_On_UpdateCurAI	=	9,			// ���µ�ǰAI״̬
	ESCAE_On_EnterCurAI		=	10,			// ���뵱ǰAI״̬
	ESCAE_On_LeaveCurAI		=	11,			// �뿪��ǰAI״̬
	ESCAE_On_EventCurAI		=	12,			// ��ǰAI״̬�¼�����
	ESCAE_On_Disappear		=	13,			// ������ʧ����

	ESCAE_End				=	14,
};

//---------------------------------------------------------------------------------
// ��ҽű�
//---------------------------------------------------------------------------------
enum EScriptRoleEvent
{
	ESRE_On_Online				=	0,			// ����
	ESRE_On_FirstOnline			=	1,			// ��һ������
	ESRE_On_IntoWorld			=	2,			// ������Ϸ����
	ESRE_On_FirstIntoWorld		=	3,			// ��һ�ν�����Ϸ����
	ESRE_On_EnterMap			=	4,			// �����ͼ
	ESRE_IsDeadPenalty			=   5,			// �Ƿ����ҽ��������ͷ�
	ESRE_On_LevelChange			=	6,			// �ȼ�����
	ESRE_On_OpenChest			=	7,			// ��������
	ESRE_On_StopChest			=	8,			// ֹͣ���������������Ʒ
	ESRE_On_AgainChest			=	9,			// �ٿ�һ��
	ESRE_On_GetItem				=	10,			// �õ�������Ʒ
	ESRE_On_GetMarriage			=	11,			// ���
	ESRE_On_GetMarriageCheck	=	12,			// �����
	ESRE_On_BreakMarriage		=	13,			// ���
	ESRE_On_BreakMarriageCheck	=	14,			// �����
	ESRE_IsEquipLootPenalty		=   15,			// װ������ͷ�
	ESRE_IsExpPenalty			=	16,			// ����ͷ�
	ESRE_IsInjuryPenalty		=	17,			// ���˳ͷ�
	ESRE_IsMoralityPenalty		=	18,			// ���³ͷ�
	ESRE_IsHostilityPenalty		=	19,			// �����ͷ�
	ESRE_IsCommercePenalty		=	20,			// ���������ͷ�
	ESRE_GetRoleFishingExp		=	21,			// ���㾭�飨�����ã�
	ESRE_CanCancelPenalty		=	22,			// �Ƿ�����������ͷ�
	ESRE_On_CompleteCommerce	=	23,			// �������

	ESRE_End					=	24,
};

//---------------------------------------------------------------------------------
// ��ͼ�ű�
//---------------------------------------------------------------------------------
enum EScriptMapEvent
{
	ESME_OnInit					=	0,			// ��ʼ��ʱ
	ESME_OnTimer				=	1,			// ��ʱ����
	ESME_OnPlayerEnter			=	2,			// ��ҽ���
	ESME_OnPlayerLeave			=	3,			// ����뿪
	ESME_OnCreatureDie			=	4,			// ��������
	ESME_OnRoleDie				=	5,			// �������
	ESME_OnEnterTrigger			=	6,			// ���봥����
	ESME_OnEnterArea			=	7,			// ��������
	ESME_CanInviteJoinTeam		=	8,			// �Ƿ������������
	ESME_CanLeaveTeam			=	9,			// �Ƿ������뿪����
	ESME_CanChangeLeader		=	10,			// �Ƿ����ƽ��ӳ�
	ESME_On_Revive				=	11,			// ��Ҹ���	
	ESME_CanEnterWhenOnline		=   12,			// �������ʱ�Ƿ��ܼ����ͼ
	ESME_GetExportMapAndCoord	=	13,			// �õ�����뿪��ǰ��ͼ��ĵ�ͼ�ɣĺ�����
	ESME_GetOnePerfectMap		=   14,			// �ҵ���ѵĸ���ʵ��
	ESME_CanEnter				=	15,			// ����Ƿ��ܽ���õ�ͼ
	ESME_FriendEnemy			=	16,			// ���������ĵ��ҹ�ϵ
	ESME_CanKickMember			=	17,			// �Ƿ������ߵ�����
	ESME_OnCreatureDisappear	=	18,			// ������ʧ
	ESME_Safeguard				=	19,			// �Ƿ�������ҿ�������ģʽ
	ESME_CanUseItem				=	20,			// �Ƿ�����ʹ����Ʒ
	ESME_CanUseSkill			=	21,			// �Ƿ�����ʹ�ü���
	ESME_End					=	22,
};

//---------------------------------------------------------------------------------
// �̶���ű�
//---------------------------------------------------------------------------------
enum EScriptActEvent
{
	ESAE_OnInit				=	0,			// ��ʼ��ʱ
	ESAE_OnTimer			=	1,			// ��ʱ����
	ESAE_OnStart			=	2,			// ���ʼ
	ESAE_OnEnd				=	3,			// �����
	ESAE_OnTimerMin			=	4,			// �ÿ���Ӹ���
	ESAE_OnDefaultRequest	=	5,			// �ͻ��˴����������ű���ȱʡ��Ϣ 
	ESAE_Broad				=	6,			// ��㲥
	ESAE_End				=	7,
};

//---------------------------------------------------------------------------------
// ��Ϸ�����¼��ű�
//---------------------------------------------------------------------------------
enum EScriptWorldEvent
{
	ESWE_Adventure			=	0,			// ��������
	ESWE_End				=	1,
};

//---------------------------------------------------------------------------------
// ��Ʒ�¼��ű�
//---------------------------------------------------------------------------------
enum EScriptItemEvent
{
	ESIE_CanUse				=	0,			// ��Ʒ�Ƿ����
	ESIE_Use				=	1,			// ��Ʒʹ��
	ESIE_End
};

//---------------------------------------------------------------------------------
// �����¼��ű�
//---------------------------------------------------------------------------------
enum EScriptSkillEvent
{
	ESSE_CanCast			=	0,			// �����Ƿ����
	ESSE_Cast				=	1,			// ����ʹ��	
	ESSE_End
};

//---------------------------------------------------------------------------------
// ͨ�ýű���
//---------------------------------------------------------------------------------
template<INT nSize>
class Script
{
public:
	VOID RegisterFunc(INT nIndex, const CHAR* szFunc);
	VOID Destroy();

protected:
	Script();
	~Script();

protected:
	CHAR*	m_szFunc[nSize];		// �ű������ַ���
};

template<INT nSize>
inline Script<nSize>::Script()
{
	ZeroMemory(m_szFunc, sizeof(m_szFunc));
}

template<INT nSize>
inline Script<nSize>::~Script()
{
	Destroy();
}

template<INT nSize>
inline VOID Script<nSize>::RegisterFunc(INT nIndex, const CHAR* szFunc)
{
	if( nIndex < 0  || nIndex >= nSize ) return;
	if( !P_VALID(szFunc) ) return;

	if( P_VALID(m_szFunc[nIndex]) )
	{
		free(m_szFunc[nIndex]);
	}

	m_szFunc[nIndex] = _strdup(szFunc);
}

template<INT nSize>
inline VOID Script<nSize>::Destroy()
{
	for(INT n = 0; n < nSize; ++n)
	{
		if( P_VALID(m_szFunc[n]) )
		{
			free(m_szFunc[n]);
			m_szFunc[n] = NULL;
		}
	}
}

//---------------------------------------------------------------------------------
// ����ű�
//---------------------------------------------------------------------------------
class QuestScript : public Script<ESQE_End>
{
	friend class ScriptMgr;
private:
	~QuestScript() {}
public:
	INT  CheckAccept(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const;
	INT  CheckComplete(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const;

	VOID OnInit(UINT16 u16QuestID, Role* pOwner) const;
	VOID OnAccept(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnComplete(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnCancel(UINT16 u16QuestID, Role* pOwner) const;
	VOID OnCreatureKill(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const;
	VOID OnNPCTalk(UINT16 u16QuestID, Role* pOwner, DWORD dwNPCID, DWORD dwNPCTypeID) const;
	VOID OnDlgDefault(UINT16 u16QuestID, Role* pOwner, DWORD dwDlgOption) const;
	VOID OnInvest(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const;
};

//----------------------------------------------------------------------------------
// ����AI�ű�
//----------------------------------------------------------------------------------
class CreatureScript : public Script<ESCAE_End>
{
	friend class ScriptMgr;
private:
	~CreatureScript() {}
public:
	VOID OnLoad(Creature* pCreature) const;
	VOID OnTalk(Creature* pCreature, Role* pRole, INT nIndex=-1) const;
	VOID OnRespawn(Creature* pCreature) const;
	VOID OnEnterCombat(Creature* pCreature) const;
	VOID OnLeaveCombat(Creature* pCreature) const;
	VOID OnDie(Creature* pCreature, Unit* pKiller, BOOL& bRemove) const;
	VOID OnInvest(Creature* pCreature, Role* pScr) const;

	VOID OnUpdateAI(Creature* pCreature) const;
	VOID OnUpdateCurAI(Creature* pCreature) const;
	VOID OnEnterCurAI(Creature* pCreature) const;
	VOID OnLeaveCurAI(Creature* pCreature) const;
	VOID OnDisappear(Creature* pCreature) const;
	BOOL OnEventCurAI(Creature* pCreature) const;
};

//-------------------------------------------------------------------------------------
// ��ҽű�
//-------------------------------------------------------------------------------------
class RoleScript : public Script<ESRE_End>
{
	friend class ScriptMgr;
private:
	~RoleScript() {}
public:
	VOID	OnRoleOnline(Role* pRole) const;
	VOID	OnRoleFirstOnline(Role* pRole) const;
	VOID	OnRoleIntoWorld(Role* pRole) const;
	VOID	OnRoleFirstIntoWorld(Role* pRole) const;
	VOID	OnRoleEnterMap(Role* pRole) const;
	BOOL	IsDeadPenalty(Role* pRole, EDeadPenaltyMode eMode) const;
	VOID	OnRoleLevelChange(Role* pRole) const;
	VOID	OnOpenChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const;
	VOID	OnStopChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const;
	VOID	OnAgainChest(Role* pRole) const;
	VOID	OnGetItem(Role* pRole, DWORD dwChestID, DWORD dwItemID, INT nNum) const;
	VOID	OnGetMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnGetMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	VOID	OnBreakMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnBreakMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	INT		OnGetRoleFishingExp(Role* pRole) const;
	DWORD	CanCancelPenalty(Role* pRole, Unit* pKiller, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommerce) const;
	VOID	OnCompleteCommerce(Role* pRole,DWORD dwTaelProgress) const;
};

//-------------------------------------------------------------------------------------
// ��ͼ�ű�
//-------------------------------------------------------------------------------------
class MapScript : public Script<ESME_End>
{
	friend class ScriptMgr;
private:
	~MapScript() {}
public:
	VOID	OnInit(Map* pMap) const;
	VOID	OnTimer(Map* pMap, INT nMilliSec) const;
	VOID	OnPlayerEnter(Role* pRole, Map* pMap) const;
	VOID	OnPlayerLeave(Role* pRole, Map* pMap) const;
	VOID	OnCreatureDie(Creature* pCreature, Unit* pKiller, Map* pMap) const;
	VOID	OnRoleDie(Role* pRole, Unit* pKiller, Map* pMap) const;
	VOID	Revive(Role* pRole, ERoleReviveType eType, INT &nReviveHP, INT &nReviveMP, FLOAT &fx, FLOAT &fy, FLOAT &fz, DWORD &dwRebornMapID) const;
	VOID	OnEnterTrigger(Role* pRole, tagMapTriggerInfo* pTrigger, Map* pMap) const;
	VOID	OnEnterArea(Role* pRole, tagMapAreaInfo* pArea, Map* pMap) const;
	INT		CanInviteJoinTeam(Map* pMap) const;
	INT		CanLeaveTeam(Map* pMap) const;
	INT		CanChangeLeader(Map* pMap) const;
	INT		CanKickMember(Map* pMap) const;
	DWORD	FriendEnemy(Map* pMap, Unit* pSrc, Unit* pTarget, BOOL& bIgnore) const;
	VOID	OnCreatureDisappear(Creature* pCreature, Map* pMap) const;
	INT		CanSetSafeguard(Map* pMap) const;
	BOOL	CanUseSkill(Map* pMap, DWORD dwTypeID) const;
	BOOL	CanUseItem(Map* pMap, DWORD dwTypeID) const;

	// �ű������ĸ������нӿ�
	INT		GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const;
	VOID	CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const;
	VOID	GetOnePerfectMap(Role* pRole, DWORD &dwInstanceID) const;
	INT		CanEnter(Role* pRole) const;
};

//-------------------------------------------------------------------------------------
// ��ű�
//-------------------------------------------------------------------------------------
class ActScript : public Script<ESAE_End>
{
	friend class ScriptMgr;
private:
	~ActScript() {}
public:
	VOID OnInit(DWORD dwActID) const;
	VOID OnTimer(DWORD dwActID, INT nSec) const;
	VOID OnTimerMin(DWORD dwActID) const;
	VOID OnActStart(DWORD dwActID) const;
	VOID OnActEnd(DWORD dwActID) const;
	VOID OnDefaultRequest(DWORD dwActID, Role* pRole, DWORD	dwEventType) const;
	VOID BroadActivityState(EActBroadMode eState) const;
};

//-------------------------------------------------------------------------------------
// ��Ʒ�ű�
//-------------------------------------------------------------------------------------
class ItemScript : public Script<ESIE_End>
{
	friend class ScriptMgr;
private:
	~ItemScript() {}
public:
	BOOL CanUseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, BOOL &bIgnore) const;
	VOID UseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID) const;
};

//-------------------------------------------------------------------------------------
// ���ܽű�
//-------------------------------------------------------------------------------------
class SkillScript : public Script<ESSE_End>
{
	friend class ScriptMgr;
private:
	~SkillScript () {}
public:
	DWORD CanCastSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwDstUnitID) const;
	VOID CastSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, BOOL &bIgnore) const;
};


//-------------------------------------------------------------------------------------
// ��Ϸ�����¼��ű�
//-------------------------------------------------------------------------------------
class WorldScript : public Script<ESWE_End>
{
	friend class ScriptMgr;
private:
	~WorldScript() {}
public:
	VOID OnAdventure(Role* pRole) const;
};

//-------------------------------------------------------------------------------------
// �ű�������
//-------------------------------------------------------------------------------------
class ScriptMgr : public EventMgr<ScriptMgr>
{
public:
	//---------------------------------------------------------------------------------
	// ��ʼ��������
	//---------------------------------------------------------------------------------
	BOOL Init();
	VOID Update();
	VOID Destroy();
	
	//---------------------------------------------------------------------------------
	// ע��ű�����
	//---------------------------------------------------------------------------------
	VOID RegisterCreatureEvent(DWORD dwID, EScriptCreatureEvent eEvent, const CHAR* szFunction);
	VOID RegisterQuestEvent(UINT16 u16QuestID, EScriptQuestEvent eEvent, const CHAR* szFunction);
	VOID RegisterRoleEvent(EScriptRoleEvent eEvent, const CHAR* szFunction);
	VOID RegisterMapEvent(const CHAR* szMapName, EScriptMapEvent eEvent, const CHAR* szFunction);
	VOID RegisterActEvent(DWORD dwActID, EScriptActEvent eEvent, const CHAR* szFunction);
	VOID RegisterWorldEvent(EScriptWorldEvent eEvent, const CHAR* szFunction);
	VOID RegisterItemEvent(DWORD dwTypeID, EScriptItemEvent eEvent, const CHAR* szFunction);
	VOID RegisterSkillEvent(DWORD dwTypeID, EScriptSkillEvent eEvent, const CHAR* szFunction);	
	//---------------------------------------------------------------------------------
	// ���ɽű�����
	//---------------------------------------------------------------------------------
	const CreatureScript*	GetCreatureScript(DWORD dwCreatureID)	{ return m_mapCreatureScript.Peek(dwCreatureID); }
	const QuestScript*		GetQuestScript(UINT16 u16QuestID)		{ return m_mapQusetScript.Peek(u16QuestID); }
	const MapScript*		GetMapScript(DWORD dwMapID)				{ return m_mapMapScript.Peek(dwMapID); }
	const RoleScript*		GetRoleScript()							{ return m_pRoleScript; }
	const ActScript*		GetActScript(DWORD dwActID)				{ return m_mapActScript.Peek(dwActID); }
	const WorldScript*		GetWorldScript()						{ return m_pWorldScript; }
	const ItemScript*		GetItemScript(DWORD dwTypeID)			{ return m_mapItemScript.Peek(dwTypeID); }
	const SkillScript*		GetSkillScript(DWORD dwTypeID)			{ return m_mapSkillScript.Peek(dwTypeID); }

	//---------------------------------------------------------------------------------
	// ���ýű�
	//---------------------------------------------------------------------------------
	VOID	CallScriptFunction(CHAR* szFunction, CHAR* szFormat, ...);

	//---------------------------------------------------------------------------------
	// ���ɽű���
	//---------------------------------------------------------------------------------
	DWORD	CreateScriptMutex();

	//---------------------------------------------------------------------------------
	// ��ס���ĳ���ű���
	//---------------------------------------------------------------------------------
	VOID	LockScriptMutex(DWORD dwMutexID);
	VOID	UnLockScriptMutex(DWORD dwMutexID);

	//---------------------------------------------------------------------------------
	// ��ȡȫ�ֱ���
	//---------------------------------------------------------------------------------
	template<typename T>
	VOID	GetGlobal(LPCSTR strVarName, OUT T &res);

private:
	//---------------------------------------------------------------------------------
	// ע���첽�¼�����
	//---------------------------------------------------------------------------------
	static VOID	RegisterScriptEventFunc();

	//---------------------------------------------------------------------------------
	// �첽�¼�������
	//---------------------------------------------------------------------------------
	VOID	OnReload(DWORD dwSenderID, LPVOID pEventMessage);

	//---------------------------------------------------------------------------------
	// ��ʼ��
	//---------------------------------------------------------------------------------
	VOID	LoadScripts();
	VOID	LoadScriptsFromDir(LPCTSTR szDir, std::vector<tstring>& luaFiles, BOOL bFirst=FALSE);
	VOID	RegisterCoreFunctions();

	//-----------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------
	VOID	DestroyScripts();
	VOID	DestroyAllStates();
	VOID	UnloadScripts();

	//-----------------------------------------------------------------------------------
	// ���¼��ؽű�
	//-----------------------------------------------------------------------------------
	VOID	Reload();
	
	//-----------------------------------------------------------------------------------
	// �õ�һ���߳�״̬
	//-----------------------------------------------------------------------------------
	lua_State* GetThreadState();
	lua_State* CreateThreadState(DWORD dwThreadID);

	//-----------------------------------------------------------------------------------
	// ������ʾ
	//-----------------------------------------------------------------------------------
	VOID ErrMsg(lua_State* pState);
	
private:
	typedef TMap<UINT16, QuestScript*>			QuestScriptMap;
	typedef TMap<DWORD, CreatureScript*>		CreatureScriptMap;
	typedef TMap<DWORD, MapScript*>				MapScriptMap;
	typedef TMap<DWORD, ActScript*>				ActScriptMap;
	typedef TMap<DWORD, ItemScript*>			ItemScriptMap;
	typedef TMap<DWORD, SkillScript*>			SkillScriptMap;

private:
	TObjRef<Util>					m_pUtil;
	Mutex							m_Lock;					// ���������߳�״̬����

	lua_State*						m_pMasterState;			// ��״̬
	TSafeMap<DWORD, lua_State*>		m_mapThreadState;		// �����߳�״̬

	TMap<DWORD, Mutex*>				m_mapScriptMutex;		// �ű�����������Ҫ�������еĽű�����
	DWORD							m_dwScriptMutexIDGen;	// �ű���ID������

	QuestScriptMap					m_mapQusetScript;		// ����ű�������ID---�ű���
	CreatureScriptMap				m_mapCreatureScript;	// ����ű�������ID�����ű���
	MapScriptMap					m_mapMapScript;			// ��ͼ�ű�����ͼ����ID�����ű���
	ActScriptMap					m_mapActScript;			// ��ű����ID�����ű���
	ItemScriptMap					m_mapItemScript;		// ��Ʒ�ű�����ƷTypeID�����ű�)
	SkillScriptMap					m_mapSkillScript;		// ���ܽű�

	WorldScript*					m_pWorldScript;			// ��Ϸ����ű�
	RoleScript*						m_pRoleScript;			// ��ҽű�
	DWORD							m_dwMaxPcallTime;		// �ű����õ��ʱ��
};


//-----------------------------------------------------------------------------------
// ���ɽű���
//-----------------------------------------------------------------------------------
inline DWORD ScriptMgr::CreateScriptMutex()
{
	Mutex* pMutex = new Mutex;

	++m_dwScriptMutexIDGen;
	m_mapScriptMutex.Add(m_dwScriptMutexIDGen, pMutex);

	return m_dwScriptMutexIDGen;
}

//------------------------------------------------------------------------------------
// ��סĳ���ű���
//------------------------------------------------------------------------------------
inline VOID ScriptMgr::LockScriptMutex(DWORD dwMutexID)
{
	Mutex* pMutex = m_mapScriptMutex.Peek(dwMutexID);

	if( P_VALID(pMutex) )
	{
		pMutex->Acquire();
	}
}

//-------------------------------------------------------------------------------------
// ���ĳ���ű���
//-------------------------------------------------------------------------------------
inline VOID ScriptMgr::UnLockScriptMutex(DWORD dwMutexID)
{
	Mutex* pMutex = m_mapScriptMutex.Peek(dwMutexID);

	if( P_VALID(pMutex) )
	{
		pMutex->Release();
	}
}

//---------------------------------------------------------------------------------
// ��ȡȫ�ֱ���
//---------------------------------------------------------------------------------
template<typename INT>
VOID ScriptMgr::GetGlobal(LPCSTR strVarName, OUT INT &res)
{
	lua_State* pThreadState = GetThreadState();
	if( !P_VALID(pThreadState) ) return;

	m_Lock.Acquire();

	lua_getglobal(pThreadState, strVarName);
	res = luaL_checkint(pThreadState, 1);

	lua_pop(pThreadState, 1);

	m_Lock.Release();
}

extern ScriptMgr g_ScriptMgr;

