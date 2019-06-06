//--------------------------------------------------------------------------------
//!\file lua_functions.cpp
//!\author Aslan
//!
//!\date 2009-03-13
//! last 2009-03-13
//!
//!\brief Lua�ű���������
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//--------------------------------------------------------------------------------
#include "StdAfx.h"
#include "script_mgr.h"

#include "map.h"
#include "map_mgr.h"
#include "map_creator.h"
#include "map_instance.h"
#include "unit.h"
#include "role.h"
#include "creature.h"
#include "creature_ai.h"
#include "skill.h"
#include "gm_policy.h"
#include "buff.h"
#include "activity_mgr.h"
#include "role_mgr.h"
#include "group_mgr.h"
#include "item_creator.h"
#include "MsgInfoMgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "pet_pocket.h"
#include "title_mgr.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/quest.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_digbao.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/msg_virgin_online.h"
#include "../WorldDefine/msg_fishing.h"
#include "../WorldDefine/msg_social.h"
#include "att_res.h"
#include "TreasureChest_mgr.h"
#include "world_session.h"
#include "../ServerDefine/role_data.h"
//--------------------------------------------------------------------------------
// �ű�����ע��
//--------------------------------------------------------------------------------
static int SC_RegisterCreatureEvent(lua_State* L);
static int SC_RegisterQuestEvent(lua_State* L);
static int SC_RegisterRoleEvent(lua_State* L);
static int SC_RegisterMapEvent(lua_State* L);
static int SC_RegisterActEvent(lua_State* L);
static int SC_RegisterWorldEvent(lua_State* L);
static int SC_RegisterItemEvent(lua_State* L);
static int SC_RegisterSkillEvent(lua_State* L);

//---------------------------------------------------------------------------------
// �ű�������
//---------------------------------------------------------------------------------
//static int SC_CreateScriptMutex(lua_State* L);
//static int SC_LockScriptMutex(lua_State* L);
//static int SC_UnLockScriptMutex(lua_State* L);

//---------------------------------------------------------------------------------
// ��Һ����ﹲ�еĽӿں���
//---------------------------------------------------------------------------------
static int SC_ChangeHP(lua_State* L);
static int SC_ChangeMP(lua_State* L);
static int SC_AddBuff(lua_State* L);
static int SC_CancelBuff(lua_State* L); 
static int SC_GetPosition(lua_State* L);
static int SC_IsDead(lua_State* L);
static int SC_IsPlayer(lua_State* L);
static int SC_IsCreature(lua_State* L);
static int SC_StopAttack(lua_State* L);
static int SC_StopMove(lua_State* L);
static int SC_IsAttack(lua_State* L);
static int SC_FriendEnemy(lua_State* L);
static int SC_GetUnitAttValue(lua_State* L);

//---------------------------------------------------------------------------------
// ��Ҷ��к���
//---------------------------------------------------------------------------------
static int SC_RoleAddQuest(lua_State* L);
static int SC_DeleteQuest(lua_State* L);
static int SC_RoleCompleteQuest(lua_State* L);
static int SC_QuestInit(lua_State* L);
static int SC_IsRoleHaveQuest(lua_State* L);
static int SC_IsRoleCompleteQuest(lua_State* L);
static int SC_RoleGotoNewMap(lua_State* L);
static int SC_GetRoleLevel(lua_State *L);
static int SC_GetRoleAttValue(lua_State *L);
static int SC_ModRoleAttValue(lua_State *L);
static int SC_QuestAddRoleItem(lua_State* L);
static int SC_RoleAddSkill(lua_State* L);
static int SC_IsRoleLearnedSkill(lua_State* L);
static int SC_GetRoleName(lua_State* L);
static int SC_IsRoleHaveTeam(lua_State* L);
static int SC_GetRoleTeamMemID(lua_State* L);
static int SC_SetQuestScriptData(lua_State* L);
static int SC_ModQuestScriptData(lua_State* L);
static int SC_GetQuestScriptData(lua_State* L);
static int SC_AddRoleItem(lua_State* L);
static int SC_RoleEquip(lua_State* L);
static int SC_RemoveFromRole(lua_State* L);
static int SC_GetBagFreeSize(lua_State* L);
static int SC_GetRoleItemNum(lua_State* L);
static int SC_AddRoleExp(lua_State* L);
static int SC_AddRoleSilver(lua_State* L);
static int SC_DecRoleSilver(lua_State* L);
static int SC_GetRoleSilver(lua_State* L);
static int SC_GetRoleSex(lua_State* L);
static int SC_GetRoldDressid(lua_State* L);
static int SC_SetRoleScriptData(lua_State* L);
static int SC_ModRoleScriptData(lua_State* L);
static int SC_GetRoleScriptData(lua_State* L);
static int SC_AddRoleExVolume(lua_State* L);
static int SC_IsRoleInStatus(lua_State* L);
static int SC_CanMount(lua_State* L);
static int SC_CanGather(lua_State* L);
static int SC_GetRoleLevelUpExp(lua_State* L);
static int SC_IsRoleOnLine(lua_State* L);
static int SC_GetActiveLongHunNum(lua_State* L);
static int SC_CastActiveLongHun(lua_State* L);
static int SC_ProlifeLongHun(lua_State* L);
static int SC_IsRoleOnStall(lua_State* L);
static int SC_SetRoleState(lua_State* L);
static int SC_UnSetRoleState(lua_State* L);
static int SC_CanOpenChest(lua_State* L);
static int SC_SendChestMsg(lua_State* L);
static int SC_IncTreasureSum(lua_State* L);
static int SC_GetTreasureSum(lua_State* L);
static int SC_GetChestItem(lua_State* L);
static int SC_ItemNeedBroadcast(lua_State* L);
static int SC_StopMount(lua_State* L);
static int SC_InstanceNotify(lua_State* L);
static int SC_IsRoleMarried(lua_State* L);
static int SC_IsRoleWedded(lua_State* L);
static int SC_GetFriendShipValue(lua_State* L);
static int SC_SigTitleEvent(lua_State* L);
static int SC_SigRemoveTitleEvent(lua_State* L);
static int SC_ChangeSkillExp(lua_State* L);
static int SC_IsRoleHaveBuff(lua_State* L);
static int SC_QuestSetRebornMap(lua_State* L);
static int SC_VirginOnlineSendClient(lua_State* L);
static int SC_FishingSendClient(lua_State* L);

static int SC_AddFriendShipValue(lua_State* L);
static int SC_IsCouple(lua_State* L);
static int SC_GetRoleYuanbao(lua_State* L);
//---------------------------------------------------------------------------------
// ������ؽӿ�
//---------------------------------------------------------------------------------
static int SC_GetRoleGuildID(lua_State* L);						// �õ���Ұ���ID�����û�а��ɷ���GT_INVALID)
static int SC_GetGuildPos(lua_State* L);
static int SC_GetGuildFund(lua_State* L);
static int SC_GetGuildMaterial(lua_State* L);
static int SC_GetGuildPeace(lua_State* L);
static int SC_GetGuildReputation(lua_State* L);

static int SC_IsGuildInStatus(lua_State* L);

static int SC_ModifyGuildFund(lua_State* L);
static int SC_ModifyGuildMaterial(lua_State* L);
static int SC_ModifyGuildPeace(lua_State* L);
static int SC_ModifyGuildReputation(lua_State* L);
static int SC_ModifyContribution(lua_State* L);

//---------------------------------------------------------------------------------
// ������к���
//---------------------------------------------------------------------------------
static int SC_GetAroundCreature(lua_State* L);
static int SC_GetAroundRole(lua_State* L);
static int SC_SetCreatureScriptData(lua_State* L);
static int SC_ModCreatureScriptData(lua_State* L);
static int SC_GetCreatureScriptData(lua_State* L);
static int SC_SetCreatureUpdateAITimer(lua_State* L);
static int SC_CreatureUseSkill(lua_State* L);
static int SC_MonsterSay(lua_State* L);
static int SC_MonsterPlayAction(lua_State* L);
static int SC_CreatureChangeAIState(lua_State* L);
static int SC_GetEnterCombatTick(lua_State* L);
static int SC_GetCreatureTypeID(lua_State* L);
static int SC_AddEnmity(lua_State* L);
static int SC_GetCreatureCurTargetID(lua_State* L);

//---------------------------------------------------------------------------------
// �̶���ӿ�
//---------------------------------------------------------------------------------
static int SC_InitEventTime(lua_State* L);
static int SC_GetActIsStart(lua_State* L);
static int SC_AddAllRoleBuff(lua_State* L);
static int SC_SetActScriptData(lua_State* L);
static int SC_GetActScriptData(lua_State* L);
static int SC_SaveActScriptData(lua_State* L);

//---------------------------------------------------------------------------------
// ��ͼ�ӿ�
//---------------------------------------------------------------------------------
static int SC_SetMapScriptData(lua_State* L);
static int SC_GetMapScriptData(lua_State* L);
static int SC_MapCreateCreature(lua_State* L);
static int SC_MapCreateColCreature(lua_State* L);
static int SC_MapDeleteCreature(lua_State* L);
static int SC_MapPlaySceneEffectByObjID(lua_State* L);
static int SC_MapPlaySceneEffectByID(lua_State* L);
static int SC_MapPlaySceneMusic(lua_State* L);
static int SC_AddMapRoleBuff(lua_State* L);
static int SC_CreateInstance(lua_State* L);
static int SC_DeleteInstance(lua_State* L);
static int SC_IsInArea(lua_State* L);
static int SC_PlaySceneEffect(lua_State* L);
static int SC_StopSceneEffect(lua_State* L);
static int SC_OpenCloseDoor(lua_State* L);
static int SC_OpenCloseAllDoors(lua_State* L);

//---------------------------------------------------------------------------------
// �ű���Ϣ�ӿ�
//---------------------------------------------------------------------------------
static int SC_BeginMsgEvent(lua_State* L);
static int SC_AddMsgEvent(lua_State* L);
static int SC_DispatchRoleMsgEvent(lua_State* L);
static int SC_DispatchWorldMsgEvent(lua_State* L);
static int SC_DispatchMapMsgEvent(lua_State* L); 

//---------------------------------------------------------------------------------
// ְҵϵͳ��ؽӿ�
//---------------------------------------------------------------------------------
static int SC_IsRoleInaugural(lua_State* L);
static int SC_ChangeClass(lua_State* L); 

//---------------------------------------------------------------------------------
// ϵͳ��ؽӿ�
//---------------------------------------------------------------------------------
static int SC_TimeGetTime(lua_State* L);
static int SC_DataTime2DWORDTime(lua_State* L);
static int SC_GetCurrentDWORDTime(lua_State* L);
static int SC_CalcTimeDiff(lua_State* L);
static int SC_IncreaseTime(lua_State* L);
static int SC_DecreaseTime(lua_State* L);
static int SC_GetGMExpRate(lua_State* L);
static int SC_GetWorldTick(lua_State* L);

//---------------------------------------------------------------------------------
// �ű�log��¼
//---------------------------------------------------------------------------------
static int SC_WriteLog(lua_State* L);

//--------------------------------------------------------------------------------
// 64λ���ݴ���
//--------------------------------------------------------------------------------
static void		push_64bit_data(lua_State* L, INT64 n64Data);
static INT64	pop_64bit_data(lua_State* L, int nHigh, int nLow);

//---------------------------------------------------------------------------------
// ���С�ӽӿ�
//---------------------------------------------------------------------------------
static int SC_GetTeamLeaderID(lua_State* L);
static int SC_CreateTeam(lua_State* L);
static int SC_AddTeamMember(lua_State* L);

//---------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------
static const luaL_Reg auxLib[] =
{
	{"RegisterCreatureEvent",			SC_RegisterCreatureEvent},
	{"RegisterQuestEvent",				SC_RegisterQuestEvent},
	{"RegisterRoleEvent",				SC_RegisterRoleEvent},
	{"RegisterMapEvent",				SC_RegisterMapEvent},
	{"RegisterActEvent",				SC_RegisterActEvent},
	{"RegisterWorldEvent",				SC_RegisterWorldEvent},
	{"RegisterItemEvent",				SC_RegisterItemEvent},
	{"RegisterSkillEvent",				SC_RegisterSkillEvent},
	//{"CreateScriptMutex",				SC_CreateScriptMutex},
	//{"LockScriptMutex",					SC_LockScriptMutex},
	//{"UnLockScriptMutex",				SC_UnLockScriptMutex},
	{"TimeGetTime",						SC_TimeGetTime},
	{"GetWorldTick",					SC_GetWorldTick},
	{"WriteLog",						SC_WriteLog},
	{"DataTime2DWORDTime",				SC_DataTime2DWORDTime},
	{"GetCurrentDWORDTime",				SC_GetCurrentDWORDTime},
	{"CalcTimeDiff",					SC_CalcTimeDiff},
	{"IncreaseTime",					SC_IncreaseTime},
	{"DecreaseTime",					SC_DecreaseTime},
	{"GetGMExpRate",					SC_GetGMExpRate},
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// ��Һ����ﹲ�нӿڰ�
//----------------------------------------------------------------------------------
static const luaL_Reg unitLib[] = 
{
	{"ChangeHP",						SC_ChangeHP},
	{"ChangeMP",						SC_ChangeMP},
	{"AddBuff",							SC_AddBuff},
	{"CancelBuff",						SC_CancelBuff},
	{"GetPosition",						SC_GetPosition},
	{"IsDead",							SC_IsDead},
	{"IsPlayer",						SC_IsPlayer},
	{"IsCreature",						SC_IsCreature},
	{"StopMove",						SC_StopMove},
	{"StopAttack",						SC_StopAttack},
	{"IsAttack",						SC_IsAttack},
	{"FriendEnemy",						SC_FriendEnemy},
	{"GetUnitAttValue",					SC_GetUnitAttValue},
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// �̶���ӿ�
//----------------------------------------------------------------------------------
static const luaL_Reg actLib[] =
{
	{"InitEventTime",					SC_InitEventTime},
	{"GetActIsStart",					SC_GetActIsStart},
	{"AddAllRoleBuff",					SC_AddAllRoleBuff},
	{"SetActScriptData",				SC_SetActScriptData},
	{"GetActScriptData",				SC_GetActScriptData},
	{"SaveActScriptData",				SC_SaveActScriptData},
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// ��ͼ�¼��ӿ�
//----------------------------------------------------------------------------------
static const luaL_Reg mapLib[] = 
{
	{"SetMapScriptData",				SC_SetMapScriptData},
	{"GetMapScriptData",				SC_GetMapScriptData},
	{"MapCreateCreature",				SC_MapCreateCreature},
	{"MapCreateColCreature",			SC_MapCreateColCreature},
	{"MapDeleteCreature",				SC_MapDeleteCreature},
	{"CreateInstance",					SC_CreateInstance},
	{"DeleteInstance",					SC_DeleteInstance},
	{"AddMapRoleBuff",					SC_AddMapRoleBuff},
	{"IsInArea",						SC_IsInArea},
//	{"MapPlaySceneEffectByObjID",		SC_MapPlaySceneEffectByObjID},
//	{"MapPlaySceneEffectByID",			SC_MapPlaySceneEffectByID},
//	{"MapPlaySceneMusic",				SC_MapPlaySceneMusic},
	{"PlaySceneEffect",					SC_PlaySceneEffect},
	{"StopSceneEffect",					SC_StopSceneEffect},
	{"OpenCloseDoor",					SC_OpenCloseDoor},
	{"OpenCloseAllDoors",				SC_OpenCloseAllDoors},
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// ����¼��ӿ�
//----------------------------------------------------------------------------------
static const luaL_Reg roleLib[] = 
{
	{"RoleGotoNewMap",					SC_RoleGotoNewMap},
	{"GetRoleLevel",					SC_GetRoleLevel},
	{"GetRoleAttValue",					SC_GetRoleAttValue},
	{"ModRoleAttValue",					SC_ModRoleAttValue},
	{"AddQuest",						SC_RoleAddQuest},
	{"DeleteQuest",						SC_DeleteQuest},
	{"RoleCompleteQuest",				SC_RoleCompleteQuest},
	{"QuestInit",						SC_QuestInit},
	{"QuestAddRoleItem",				SC_QuestAddRoleItem},
	{"AddSkill",						SC_RoleAddSkill},
	{"IsLearnedSkill",					SC_IsRoleLearnedSkill},
	{"GetRoleName",						SC_GetRoleName},
	{"IsRoleHaveTeam",					SC_IsRoleHaveTeam},
	{"GetRoleTeamMemID",				SC_GetRoleTeamMemID},
	{"SetQuestScriptData",				SC_SetQuestScriptData},
	{"GetQuestScriptData",				SC_GetQuestScriptData},
	{"ModQuestScriptData",				SC_ModQuestScriptData},
	{"IsRoleHaveQuest",					SC_IsRoleHaveQuest},
	{"IsRoleCompleteQuest",				SC_IsRoleCompleteQuest},
	{"AddRoleItem",						SC_AddRoleItem},
	{"RoleEquip",						SC_RoleEquip},
	{"RemoveFromRole",					SC_RemoveFromRole},
	{"GetBagFreeSize",					SC_GetBagFreeSize},
	{"GetRoleItemNum",					SC_GetRoleItemNum},
	{"AddRoleExp",						SC_AddRoleExp},
	{"AddRoleSilver",					SC_AddRoleSilver},
	{"DecRoleSilver",					SC_DecRoleSilver},
	{"GetRoleSilver",					SC_GetRoleSilver},
	{"GetRoleSex",						SC_GetRoleSex},
	{"GetRoleDressid",					SC_GetRoldDressid},
	{"SetRoleScriptData",				SC_SetRoleScriptData},
	{"ModRoleScriptData",				SC_ModRoleScriptData},
	{"GetRoleScriptData",				SC_GetRoleScriptData},
	{"AddRoleExVolume",					SC_AddRoleExVolume},
	{"IsRoleInStatus",					SC_IsRoleInStatus},
	{"CanMount",						SC_CanMount},
	{"CanGather",						SC_CanGather},
	{"GetRoleLevelUpExp",				SC_GetRoleLevelUpExp},
	{"IsRoleOnLine",					SC_IsRoleOnLine},
	{"GetLongHunID",					SC_GetActiveLongHunNum},
	{"CastLongHun",						SC_CastActiveLongHun},
	{"ProlifeLongHun",					SC_ProlifeLongHun},
	{"IsRoleOnStall",					SC_IsRoleOnStall},	
	{"CanOpenChest",					SC_CanOpenChest},		
	{"SendChestMsg",					SC_SendChestMsg},		
	{"IncTreasureSum",					SC_IncTreasureSum},		
	{"GetTreasureSum",					SC_GetTreasureSum},		
	{"GetChestItem",					SC_GetChestItem},
	{"ItemNeedBroadcast",				SC_ItemNeedBroadcast},
	{"SetRoleState",					SC_SetRoleState},
	{"UnSetRoleState",					SC_UnSetRoleState},
	{"StopMount",						SC_StopMount},
	{"InstanceNotify",					SC_InstanceNotify},
	{"IsRoleMarried",					SC_IsRoleMarried},
	{"IsRoleWedded",					SC_IsRoleWedded},
	{"GetFriendShipValue",				SC_GetFriendShipValue},
	{"SigTitleEvent",					SC_SigTitleEvent},
	{"SigRemoveTitleEvent",				SC_SigRemoveTitleEvent},
	{"ChangeSkillExp",					SC_ChangeSkillExp},
	{"IsRoleHaveBuff",					SC_IsRoleHaveBuff},
	{"QuestSetRebornMap",				SC_QuestSetRebornMap},
	{"VirginOnlineSendClient",			SC_VirginOnlineSendClient},
	{"FishingSendClient",				SC_FishingSendClient},
	{"IsCouple",						SC_IsCouple},
	{"GetRoleYuanbao",					SC_GetRoleYuanbao},
	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// ��Ϣ�¼��ӿ�
//--------------------------------------------------------------------------------------
static const luaL_Reg msgLib[] = 
{
	{"BeginMsgEvent",					SC_BeginMsgEvent},
	{"AddMsgEvent",						SC_AddMsgEvent},
	{"DispatchRoleMsgEvent",			SC_DispatchRoleMsgEvent},
	{"DispatchWorldMsgEvent",			SC_DispatchWorldMsgEvent},
	{"DispatchMapMsgEvent",				SC_DispatchMapMsgEvent},
	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// �����¼��ӿ�
//--------------------------------------------------------------------------------------
static const luaL_Reg creLib[] = 
{
	{"GetAroundCreature",				SC_GetAroundCreature},
	{"GetAroundRole",					SC_GetAroundRole},
	{"GetCreatureScriptData",			SC_GetCreatureScriptData},
	{"SetCreatureScriptData",			SC_SetCreatureScriptData},
	{"ModCreatureScriptData",			SC_ModCreatureScriptData},
	{"SetCreatureAITimer",				SC_SetCreatureUpdateAITimer},
	{"CreatureUseSkill",				SC_CreatureUseSkill},
	{"MonsterSay",						SC_MonsterSay},
	{"MonsterPlayerAction",				SC_MonsterPlayAction},
	{"CreatureChangeAIState",			SC_CreatureChangeAIState},
	{"GetEnterCombatTick",				SC_GetEnterCombatTick},
	{"GetCreatureTypeID",				SC_GetCreatureTypeID},
	{"AddEnmity",						SC_AddEnmity},
	{"GetCreatureCurTargetID",			SC_GetCreatureCurTargetID},
	{NULL,	NULL},
};

static const luaL_Reg teamLib[] = 
{
	{"CreateTeam",						SC_CreateTeam},
	{"AddTeamMember",					SC_AddTeamMember},
	{"GetTeamLeaderID",					SC_GetTeamLeaderID},
	{NULL, NULL},
};

static const luaL_Reg guildLib[] = 
{
	{"GetRoleGuildID",					SC_GetRoleGuildID},
	{"GetRoleGuildPos",					SC_GetGuildPos},
	{"GetGuildFund",					SC_GetGuildFund},
	{"GetGuildMaterial",				SC_GetGuildMaterial},
	{"GetGuildPeace",					SC_GetGuildPeace},
	{"GetGuildReputation",				SC_GetGuildReputation},
	{"ModifyRoleContribute",			SC_ModifyContribution},
	{"ModifyGuildFund",					SC_ModifyGuildFund},
	{"ModifyGuildMaterial",				SC_ModifyGuildMaterial},
	{"ModifyGuildPeace",				SC_ModifyGuildPeace},
	{"ModifyGuildReputation",			SC_ModifyGuildReputation},
	{"IsGuildInStatus",					SC_IsGuildInStatus},
	{NULL,	NULL},
};

//----------------------------------------------------------------------------------
// ע��C�����⺯��
//----------------------------------------------------------------------------------
VOID LuaOpenCommLibs(lua_State*  L)
{
	luaL_openlib(L, "aux",  auxLib,  0);
	luaL_openlib(L, "unit", unitLib, 0);
	luaL_openlib(L, "role", roleLib, 0);
	luaL_openlib(L, "act",  actLib,  0);
	luaL_openlib(L,	"map",  mapLib,  0);
	luaL_openlib(L, "msg",  msgLib,  0);
	luaL_openlib(L, "cre",  creLib,  0);
	luaL_openlib(L, "team", teamLib, 0);
	luaL_openlib(L, "guild", guildLib, 0);
}

//--------------------------------------------------------------------------------
// 64λ���ݴ���
//--------------------------------------------------------------------------------
void push_64bit_data(lua_State* L, INT64 n64Data)
{
	const INT32 nMask	= 0xFFFFFFFF;

	INT32 n32High	= (INT32)((n64Data >> 32) & nMask);
	INT32 n32Low	= (INT32)(n64Data & nMask);

	lua_pushnumber(L, n32High);
	lua_pushnumber(L, n32Low);
}

INT64 pop_64bit_data(lua_State* L, int nHigh, int nLow)
{
	const INT64 n64Mask	= 0x00000000FFFFFFFF;

	INT32 n32High	= lua_tonumber(L, nHigh);
	INT32 n32Low	= lua_tonumber(L, nLow);

	INT64 n64Data	= n32High;
	n64Data			= (n64Data << 32) | (n32Low & n64Mask);

	return n64Data;
}

//-----------------------------------------------------------------------------------
// �ⲿ��ע�ắ��
//-----------------------------------------------------------------------------------
int SC_RegisterCreatureEvent(lua_State* L)
{
	DWORD dwTypeID = luaL_checknumber(L, 1);			// �����ID
	INT nEventType = luaL_checkint(L, 2);				// ������¼�����
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !GT_VALID(dwTypeID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterCreatureEvent(dwTypeID, (EScriptCreatureEvent)nEventType, szFunction);
	return 0;
}

int SC_RegisterQuestEvent(lua_State* L)
{
	UINT16 u16QuestID = (UINT16)luaL_checknumber(L, 1);	// ����ID
	INT nEventType = luaL_checkint(L, 2);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( (INT16)u16QuestID < 0 || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterQuestEvent(u16QuestID, (EScriptQuestEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterRoleEvent(lua_State* L)
{
	INT nEventType = luaL_checkint(L, 1);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 2);	// �ű�����

	if( !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterRoleEvent((EScriptRoleEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterMapEvent(lua_State* L)
{
	const CHAR* szMapName = luaL_checkstring(L, 1);		// ��ͼ����
	INT nEventType = luaL_checkint(L, 2);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !P_VALID(szMapName) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterMapEvent(szMapName, (EScriptMapEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterActEvent(lua_State* L)
{
	DWORD dwActID = luaL_checkint(L, 1);				// �ID
	INT nEventType = luaL_checkint(L, 2);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !GT_VALID(dwActID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterActEvent(dwActID, (EScriptActEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterWorldEvent(lua_State* L)
{
	INT		nEventType = luaL_checkint(L, 1);			// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterWorldEvent((EScriptWorldEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterItemEvent(lua_State* L)
{
	DWORD dwTypeID	=	luaL_checknumber(L, 1);			// ��ƷID
	INT	nEventType	=	luaL_checkint(L, 2);			// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if(!GT_VALID(dwTypeID) || !GT_VALID(nEventType) || !P_VALID(szFunction))
		return 0;

	g_ScriptMgr.RegisterItemEvent(dwTypeID, (EScriptItemEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterSkillEvent(lua_State* L)
{
	DWORD dwSkillID	=	luaL_checknumber(L, 1);			// ����ID
	INT	nEventType	=	luaL_checkint(L, 2);			// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if(!GT_VALID(dwSkillID) || !GT_VALID(nEventType) || !P_VALID(szFunction))
		return 0;

	g_ScriptMgr.RegisterSkillEvent(dwSkillID, (EScriptSkillEvent)nEventType, szFunction);

	return 0;
}


////-------------------------------------------------------------------------------------
//// ����
////-------------------------------------------------------------------------------------
//int SC_CreateScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = g_ScriptMgr.CreateScriptMutex();
//
//	lua_pushnumber(L, 0);
//
//	return 1;
//}
//
//int SC_LockScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = lua_tonumber(L, 1);
//
//	g_ScriptMgr.LockScriptMutex(dwMutexID);
//
//	return 0;
//}
//
//int SC_UnLockScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = lua_tonumber(L, 1);
//
//	g_ScriptMgr.UnLockScriptMutex(dwMutexID);
//
//	return 0;
//}

//-------------------------------------------------------------------------------------
// ���ϵͳʱ��
//-------------------------------------------------------------------------------------
int SC_TimeGetTime(lua_State* L)
{
	DWORD dwTime = timeGetTime();

	lua_pushnumber(L, dwTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���������ǰ��������
//-------------------------------------------------------------------------------------
int SC_GetWorldTick(lua_State* L)
{
	DWORD dwTick = g_world.GetWorldTick();

	lua_pushnumber(L, dwTick);

	return 1;
}

//-------------------------------------------------------------------------------------
//��¼�ű�log
//-------------------------------------------------------------------------------------
int SC_WriteLog(lua_State* L)
{
	ILOG->Write(IUTIL->AnsiToUnicode(lua_tostring(L, 1)));
	return 0;
}

//-------------------------------------------------------------------------------------
// ת��ʱ��ΪDWORD
//-------------------------------------------------------------------------------------
int SC_DataTime2DWORDTime(lua_State* L)
{
	BYTE	byYear		=	lua_tonumber(L, 1);
	BYTE	byMonth		=	lua_tonumber(L, 2);
	BYTE	byDay		=	lua_tonumber(L, 3);
	BYTE	byHour		=	lua_tonumber(L, 4);
	BYTE	byMin		=	lua_tonumber(L, 5);
	BYTE	bySec		=	lua_tonumber(L, 6);

	if( bySec < 0 || bySec > 59 || byMin < 0 || byMin > 59 || byHour < 0 || byHour > 23 ||
		byDay < 0 || byDay > 31 || byMonth <  0 || byMonth > 12 || byYear < 0 || byYear > 63 )
	{
		ASSERT(0);
		return 0;
	}

	DWORD	dwDWORDTime = tagDWORDTime(bySec, byMin, byHour, byDay, byMonth, byYear);

	lua_pushnumber(L, dwDWORDTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���ǰDWORDʱ��
//-------------------------------------------------------------------------------------
int SC_GetCurrentDWORDTime(lua_State* L)
{
	DWORD	dwCurTime = GetCurrentDWORDTime();
	lua_pushnumber(L, dwCurTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// �����������ڵ�ʱ��������
//-------------------------------------------------------------------------------------
int SC_CalcTimeDiff(lua_State* L)
{
	DWORD	dwDestTime		=	lua_tonumber(L, 1);
	DWORD	dwSrcTime		=	lua_tonumber(L, 2);
	
	DWORD dwSecond = CalcTimeDiff(dwDestTime, dwSrcTime);
	lua_pushnumber(L, dwSecond);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ĳ��ʱ�����ĳ��ֵ
//-------------------------------------------------------------------------------------
int SC_IncreaseTime(lua_State* L)
{
	DWORD	dwSrcTime		=	lua_tonumber(L, 1);
	INT		nIncSecond		=	lua_tonumber(L, 2);

	DWORD	dwDestTime = IncreaseTime(dwSrcTime, nIncSecond);	
	lua_tonumber(L, dwDestTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// ʱ���ϵݼ�ָ������
//-------------------------------------------------------------------------------------
int SC_DecreaseTime(lua_State* L)
{
	DWORD	dwSrcTime		=	lua_tonumber(L, 1);
	INT		nDecSecond		=	lua_tonumber(L, 2);

	DWORD	dwDestTime = DecreaseTime(dwSrcTime, nDecSecond);
	lua_tonumber(L, dwDestTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// GM������
//-------------------------------------------------------------------------------------
int SC_GetGMExpRate(lua_State* L)
{
	FLOAT	fGMExpRate = g_GMPolicy.GetExpRate();

	lua_pushnumber(L, fGMExpRate);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��Һ͹��ﹲ�еĽӿں���
//-------------------------------------------------------------------------------------
int SC_ChangeHP(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);
	INT		nHP				=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->ChangeHP(nHP);

	return 0;
}

int SC_ChangeMP(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);
	INT		nMP				=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->ChangeMP(nMP);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ�Unit���ڵ�ͼMapCrc������
//-------------------------------------------------------------------------------------
int SC_GetPosition(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	INT nX = pUnit->GetCurPos().x / TILE_SCALE;
	INT nY = pUnit->GetCurPos().y;
	INT nZ = pUnit->GetCurPos().z / TILE_SCALE;

	lua_pushinteger(L, nX);
	lua_pushinteger(L, nY);
	lua_pushinteger(L, nZ);

	return 3;
}

int SC_IsDead(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	BOOL bDead = pUnit->IsDead();
	lua_pushboolean(L, bDead);

	return 1;
}

int SC_IsPlayer(lua_State* L)
{
	DWORD	dwUnitID		=	lua_tonumber(L, 1);
	BOOL	bPlayer			=	FALSE;

	if(IS_PLAYER(dwUnitID))
		bPlayer = TRUE;

	lua_pushboolean(L, bPlayer);

	return 1;
}

int SC_IsCreature(lua_State* L)
{
	DWORD	dwUnitID		=	lua_tonumber(L, 1);
	BOOL	bCreature		=	FALSE;

	if(IS_CREATURE(dwUnitID) || IS_PET(dwUnitID))
		bCreature = TRUE;

	lua_pushboolean(L, bCreature);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��Unitֹͣ�ƶ�
//-------------------------------------------------------------------------------------
int SC_StopMove(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);
	BOOL	bSendMsg		=	lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->GetMoveData().StopMove(bSendMsg);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Unitֹͣ����
//-------------------------------------------------------------------------------------
int SC_StopAttack(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->GetCombatHandler().End();

	return 0;
}

//-------------------------------------------------------------------------------------
// ��λ�Ƿ���ս���У�ֻ������creature
//-------------------------------------------------------------------------------------
int SC_IsAttack(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	// �õ�creature��ǰ״̬
	INT nFight = 0;
	if (pCreature->GetAI()->GetCurrentStateType() == AIST_Idle)
	{
		nFight = 0;
	}
	else
	{
		nFight = 1;
	}

	lua_pushinteger(L, nFight);
	return 1;

}

//-------------------------------------------------------------------------------------
// �����ж�
//-------------------------------------------------------------------------------------
int SC_FriendEnemy(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwSrcID			=	lua_tonumber(L, 3);
	DWORD	dwDstID			=	lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pSrc = pMap->FindUnit(dwSrcID);
	if( !P_VALID(pSrc) ) return 0;

	Unit* pDest = pMap->FindUnit(dwDstID);
	if( !P_VALID(pDest) ) return 0;

	DWORD dwFriendEnemy = pSrc->FriendEnemy(pDest);

	lua_pushnumber(L, dwFriendEnemy);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ȡUnit Att
//-------------------------------------------------------------------------------------
static int SC_GetUnitAttValue(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	lua_pushinteger(L, pUnit->GetAttValue(nIndex));

	return 1;
}

//-------------------------------------------------------------------------------------
// ��Ӽ���
//-------------------------------------------------------------------------------------
static int SC_RoleAddSkill(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	DWORD	dwSkillTypeID	=	lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	const tagSkillProto* pProto = g_attRes.GetSkillProto(dwSkillTypeID);
	if( !P_VALID(pProto) ) return 0;

	DWORD	dwSkillID	= Skill::GetIDFromTypeID(dwSkillTypeID);
	INT		nLevel		= Skill::GetLevelFromTypeID(dwSkillTypeID);

	Skill*  pSkill = new Skill(dwSkillID, nLevel, 0, 0, 0);
	pRole->AddSkill(pSkill);

	return 0;
}

//-------------------------------------------------------------------------------------
// �Ƿ��Ѿ�ѧ��ָ������
//-------------------------------------------------------------------------------------
static int SC_IsRoleLearnedSkill(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);
	DWORD	dwSkillID	=	lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	Skill* pSkill = pRole->GetSkill(dwSkillID);	
	if (!P_VALID(pSkill))	return 0;

	lua_pushinteger(L, pSkill->GetLevel());

	return 1;
}

int SC_AddBuff(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);
	DWORD	dwBuffTypeID	=	lua_tonumber(L, 4);
	DWORD	dwSrcUnitID		=	lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	Unit* pSrcUnit = pMap->FindUnit(dwSrcUnitID);

	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return 0;

	pUnit->TryAddBuff(pSrcUnit, pProto, NULL, NULL, NULL);

	return 0;
}

int SC_CancelBuff(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwUnitID		=	lua_tonumber(L, 3);
	DWORD	dwBuffTypeID	=	lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	//pUnit->CancelBuff(dwBuffTypeID);
	pUnit->RemoveBuff(Buff::GetIDFromTypeID(dwBuffTypeID), TRUE);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Ҷ��к���
//-------------------------------------------------------------------------------------
int SC_RoleAddQuest(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// �������
	pRole->AcceptQuestFromNPC(u16QuestID, GT_INVALID);

	return 0;
}

int SC_DeleteQuest(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// ɾ������
	pRole->RemoveQuest(u16QuestID, FALSE);

	// ���ͷ�����Ϣ���ͻ���
	tagNS_DeleteQuest send;
	send.u16QuestID = u16QuestID;
	send.dwErrorCode = E_Success;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

int SC_RoleCompleteQuest(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	DWORD	dwNPCID		=	lua_tonumber(L, 3);
	UINT16	u16NextQuestID = 0;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	INT nRet = pRole->CompleteQuest(u16QuestID, dwNPCID, -1, u16NextQuestID);

	// ���ͷ���
	tagNS_CompleteQuest send;
	send.u16QuestID = u16QuestID;
	send.dwErrorCode = nRet;
	pRole->SendMessage(&send, send.dwSize);

	// ����Ƿ��к����Զ���ȡ������
	if(E_Success == nRet && u16NextQuestID != 0)
		pRole->AcceptQuestFromNPC(u16NextQuestID, GT_INVALID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Ҷ��к���
//-------------------------------------------------------------------------------------
int SC_QuestInit(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	
	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	tagQuestDynamicTarget* pTarget = pQuest->GetDynamicTarget();
	if(!P_VALID(pTarget)) return 0;

	pTarget->eTargetType = (EQuestTargetType)lua_tointeger(L, 3);	// ����̬Ŀ������
	pTarget->dwQuestTipsID = lua_tonumber(L, 4);

	if(EQTT_NPCTalk	== pTarget->eTargetType || EQTT_Invest == pTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			pTarget->dwTargetID[i] = lua_tonumber(L ,i + 5);
		}
	}
	else
	{
		for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
		{
			pTarget->dwTargetID[n] = lua_tonumber(L, (2*n)+5);
			pTarget->nTargetNum[n] = lua_tonumber(L, (2*n)+6);
		}
	}

	return 0;
}

int SC_RoleGotoNewMap(lua_State* L)		// ȥһ���µ�ͼ
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	DWORD	dwDestMapID		=	lua_tonumber(L, 4);
	FLOAT	fx				=	lua_tonumber(L, 5);
	FLOAT	fy				=	lua_tonumber(L, 6);
	FLOAT	fz				=	lua_tonumber(L, 7);

	fx = fx * TILE_SCALE;
	fy = fy * TILE_SCALE;
	fz = fz * TILE_SCALE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->GotoNewMap(dwDestMapID, fx, fy, fz);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ���ҵȼ�
//-------------------------------------------------------------------------------------
int SC_GetRoleLevel(lua_State *L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushinteger(L, pRole->GetLevel());
	return 1;
}

//-------------------------------------------------------------------------------------
// ��ȡ��ҵ�ǰ����
//-------------------------------------------------------------------------------------
int SC_GetRoleAttValue(lua_State *L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	lua_pushinteger(L, pRole->GetAttValue(nIndex));
	return 1;
}

//-------------------------------------------------------------------------------------
// �޸���ҵ�ǰ����
//-------------------------------------------------------------------------------------
int SC_ModRoleAttValue(lua_State *L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);
	INT		nValue			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	pRole->ModAttValue(nIndex, nValue);

	return 0;
};

//-------------------------------------------------------------------------------------
// ��һ��������Ʒװ��
//-------------------------------------------------------------------------------------
int SC_QuestAddRoleItem(lua_State* L)
{
	DWORD			dwMapID			=	lua_tonumber(L, 1);
	DWORD			dwInstanceID	=	lua_tonumber(L, 2);
	DWORD			dwRoleID		=	lua_tonumber(L, 3);
	UINT16			u16QuestID		=   (UINT16)lua_tointeger(L, 4);		// ����ID
	DWORD			dwTypeID		=	lua_tonumber(L, 5);					// ��ƷTypeID
	INT				nNum			=	lua_tointeger(L, 6);				// ��Ʒ����
	EItemQuality	eQuality		=	(EItemQuality)lua_tointeger(L, 7);	// ��ƷƷ��
	ELogCmdID		eLogCmdID		=	(ELogCmdID)lua_tointeger(L, 8);		// ��¼log����

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;
	
	pRole->GetItemMgr().Add2Role(EICM_Quest, u16QuestID, dwTypeID, (INT16)nNum, eQuality, eLogCmdID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��һ����Ʒװ��
//-------------------------------------------------------------------------------------
static int SC_AddRoleItem(lua_State* L)
{
	DWORD dwMapID		=	lua_tonumber(L, 1);
	DWORD dwInstanceID	=	lua_tonumber(L, 2);
	DWORD dwRoleID		=	lua_tonumber(L, 3);
	DWORD dwTypeID		=	lua_tonumber(L, 4);
	INT16 n16Num		=	lua_tointeger(L, 5);
	INT32 nQlty			=	lua_tointeger(L, 6);
	INT   nCreateMode	=	lua_tointeger(L, 7);
	INT	  nLogCmdID		=	lua_tointeger(L, 8);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if(n16Num <= 0)
	{
		n16Num = 1;
	}

	tagItem *pNewItem = ItemCreator::Create((EItemCreateMode)nCreateMode, dwRoleID, dwTypeID, n16Num);
	if(!P_VALID(pNewItem))
	{
		return 0;
	}

	if(MIsEquipment(pNewItem->dwTypeID))
	{
		if(nQlty == GT_INVALID)
		{
			// ������	
		}
		else
		{
			ItemCreator::IdentifyEquip((tagEquip*)pNewItem, (EItemQuality)nQlty);
		}
	}

	pRole->GetItemMgr().Add2Bag(pNewItem, (ELogCmdID)nLogCmdID, TRUE);

	//lua_pushnumber(L, pNewItem->n64Serial);
	push_64bit_data(L, pNewItem->n64Serial);
	
	if(!MIsEquipment(dwTypeID))
	{
		return 2;
	}

	lua_pushinteger(L, pNewItem->pEquipProto->eEquipPos);

	return 3;
}

//-------------------------------------------------------------------------------------
// ��Ҵ���װ��
//-------------------------------------------------------------------------------------
static int SC_RoleEquip(lua_State* L)
{
	DWORD dwMapID		=	lua_tonumber(L, 1);
	DWORD dwInstanceID	=	lua_tonumber(L, 2);
	DWORD dwRoleID		=	lua_tonumber(L, 3);
	//INT64 n64Serial		=	lua_tonumber(L, 4);
	INT64 n64Serial		=	pop_64bit_data(L, 4, 5);
	INT16 n16EquipPos	=	lua_tonumber(L, 6);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->Equip(n64Serial, (EEquipPos)n16EquipPos);

	return 0;
}

//-------------------------------------------------------------------------------------
// �ӱ�����������ɾ����Ʒ
//-------------------------------------------------------------------------------------
int SC_RemoveFromRole(lua_State* L)
{
	DWORD dwMapID		=	lua_tonumber(L, 1);
	DWORD dwInstanceID	=	lua_tonumber(L, 2);
	DWORD dwRoleID		=	lua_tonumber(L, 3);
	DWORD dwTypeID		=	lua_tonumber(L, 4);
	INT16 n16Num		=	lua_tointeger(L, 5);
	INT	  nLogCmdID		=	lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT nRet = pRole->GetItemMgr().RemoveFromRole(dwTypeID, n16Num, nLogCmdID);

	lua_pushinteger(L, nRet);

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ұ������пռ�����
//-------------------------------------------------------------------------------------
int SC_GetBagFreeSize(lua_State* L)
{
	DWORD dwRoleID		=	lua_tonumber(L, 1);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	ItemMgr& itemMgr = pRole->GetItemMgr();	

	INT nFreeSize = itemMgr.GetBagFreeSize();

	lua_pushinteger(L, nFreeSize);

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ұ��������񱳰�����Ʒ������
//-------------------------------------------------------------------------------------
int SC_GetRoleItemNum(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	DWORD	dwTypeID		=	lua_tonumber(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) 
	{
		lua_pushinteger(L, 0);
		return 1;
	}

	ItemMgr& itemMgr = pRole->GetItemMgr();

	INT nTotal = itemMgr.GetBagSameItemCount(dwTypeID) + itemMgr.GetQuestBagSameItemCount(dwTypeID);

	lua_pushinteger(L, nTotal);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------------
int SC_GetRoleName(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);

	TCHAR szName[X_SHORT_NAME];
	g_roleMgr.GetRoleNameByID(dwRoleID, szName);

	lua_pushstring(L, IUTIL->UnicodeToUnicode8(szName));

	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ƿ���С��
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveTeam(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetTeamID());

	return 1;
}

//-------------------------------------------------------------------------------------
// ������С�ӳ�ԱID
//-------------------------------------------------------------------------------------
int SC_GetRoleTeamMemID(lua_State* L)
{
	DWORD	dwTeamID		=	lua_tonumber(L, 1);
	
	const Team *pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
	if(!P_VALID(pTeam))	return 0;

	DWORD dwMemID[MAX_TEAM_NUM];
	memset(dwMemID, 0xFF, sizeof(dwMemID));

	pTeam->ExportAllMemID(dwMemID);

	for (INT i = 0; i < MAX_TEAM_NUM; ++i)
	{
		lua_pushnumber(L, dwMemID[i]);
	}

	return MAX_TEAM_NUM;
}

//-------------------------------------------------------------------------------------
// ��������ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// ����ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	lua_tonumber(L, i*2+5);

		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		pQuest->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �޸�����ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_ModQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// ����ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	lua_tonumber(L, i*2+5);

		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		pQuest->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��������ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// ����ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+4);		
		
		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		dwValue = pQuest->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ����Ƿ��ȡ�˸�����
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveQuest(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// ����ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	BOOL bHaveQuest = pRole->IsHaveQuest(u16QuestID);
	lua_pushboolean(L, bHaveQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ƿ�����ĳ����
//-------------------------------------------------------------------------------------
int SC_IsRoleCompleteQuest(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// ����ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	BOOL bCompleteQuest = pRole->IsHaveDoneQuest(u16QuestID);
	lua_pushboolean(L, bCompleteQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// ������Ҿ���
//-------------------------------------------------------------------------------------
int SC_AddRoleExp(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->ExpChange(nValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������ҽ�Ǯ
//-------------------------------------------------------------------------------------
int SC_AddRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	lua_tonumber(L, 2);
	DWORD   dwRoleID		=	lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nValue <= 0 ) return 0;

	pRole->GetCurMgr().IncBagSilver(nValue, (ELogCmdID)nCmdID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������ҽ�Ǯ
//-------------------------------------------------------------------------------------
int SC_DecRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	lua_tonumber(L, 2);
	DWORD   dwRoleID		=	lua_tonumber(L, 3);
	INT64	n64Value		=	lua_tonumber(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( n64Value <= 0 ) return 0;

	pRole->GetCurMgr().DecBagSilver(n64Value, (ELogCmdID)nCmdID);

	return 0;
}


//-------------------------------------------------------------------------------------
// �����ҽ�Ǯ
//-------------------------------------------------------------------------------------
int SC_GetRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	lua_tonumber(L, 2);
	DWORD   dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT64 n64Silver = pRole->GetCurMgr().GetBagSilver();
	lua_pushnumber(L, n64Silver);
	//lua_pushinteger(L, nSilver);
	return 1;
}

//-------------------------------------------------------------------------------------
// ������Ԫ����
//-------------------------------------------------------------------------------------
int SC_GetRoleYuanbao(lua_State* L)
{
	DWORD   dwMapID			=	lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	lua_tonumber(L, 2);
	DWORD   dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT32  iYuanbao = pRole->GetCurMgr().GetBagYuanBao();
	lua_pushnumber(L, iYuanbao);

	return 1;
}
//-------------------------------------------------------------------------------------
// �õ�����Ա�
//-------------------------------------------------------------------------------------
int SC_GetRoleSex(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetSex());
	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���ҷ�װ���
//-------------------------------------------------------------------------------------
int SC_GetRoldDressid(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetAvatar()->wDressMdlID);
	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���Ұ���ID
//-------------------------------------------------------------------------------------
int SC_GetRoleGuildID(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushnumber(L, pRole->GetGuildID());

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ҿɼ�����������
//-------------------------------------------------------------------------------------
int SC_GetActiveLongHunNum(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	// ȡ�ÿɼ�����������
	INT nActiveNum = pRole->GetLongHun().GetActiveNum();

	lua_pushinteger(L, nActiveNum);

	return 1;
}

//-------------------------------------------------------------------------------------
// ������������
//-------------------------------------------------------------------------------------
int SC_CastActiveLongHun(lua_State* L)
{
	DWORD dwRoleID	= lua_tonumber(L, 1);
	DWORD dwSkillID	= lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	// ȡ����Ҽ���
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (!P_VALID(pSkill))
	{
		return 0;
	}

	// ���ȡ���ɱ�������ID
	DWORD dwLongHunID = pRole->GetLongHun().GetRandomLongHunID();
	if (!GT_VALID(dwLongHunID))
	{
		return 0;
	}

	// ȡ����������
	const tagLongHunProto* pProto = g_attRes.GetLongHunProto(dwLongHunID);
	if (!P_VALID(pProto))
	{
		return 0;
	}

	if (pProto->eTypeEx == ELHTE_PowerGather)
	{
		return 0;
	}

	if (GT_VALID(pProto->dwBuffID1))
	{
		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(pProto->dwBuffID1);
		if (P_VALID(pBuffProto))
		{
			pRole->TryAddBuff(pRole, pBuffProto, NULL, pSkill, NULL);
		}
	}
	if (GT_VALID(pProto->dwBuffID2))
	{
		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(pProto->dwBuffID2);
		if (P_VALID(pBuffProto))
		{
			pRole->TryAddBuff(pRole, pBuffProto, NULL, pSkill, NULL);
		}	
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ���꼤��
//-------------------------------------------------------------------------------------
int SC_ProlifeLongHun(lua_State* L)
{
	DWORD dwRoleID	= lua_tonumber(L, 1);
	DWORD dwSkillID	= lua_tonumber(L, 2);
	BOOL bProlife	= lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	// ȡ����Ҽ���
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (!P_VALID(pSkill))
	{
		return 0;
	}

	// ȡ�õ�ǰ���������Ѿ�����������buff
	Buff* pBuff = pRole->GetRelativeSkillBuff(dwSkillID);
	if (!P_VALID(pBuff))
	{
		return 0;
	}

	// ȡ����һ��Buff����
	const tagBuffProto* pProto = g_attRes.GetBuffProto(pBuff->GetTypeID()+1);
	if (!P_VALID(pProto))
	{
		return 0;
	}
	
	if (bProlife)
	{
		pRole->TryAddBuff(pRole, pProto, NULL, pSkill, NULL);
	}
	lua_pushboolean(L, true);

	return 1;
}

//-------------------------------------------------------------------------------------
// �ж�����Ƿ��ڰ�̯
//-------------------------------------------------------------------------------------
int SC_IsRoleOnStall(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	int nOnStall = FALSE;

	// �ж��Ƿ��Ѿ����ڰ�̯���̯����״̬
	if(pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
		nOnStall = TRUE;
	else
		nOnStall = FALSE;

	lua_pushboolean(L, nOnStall);

	return 1;
}

//-------------------------------------------------------------------------------------
// �������״̬
//-------------------------------------------------------------------------------------
int SC_SetRoleState(lua_State* L)
{
	DWORD		dwRoleID	=	lua_tonumber(L, 1);
	INT			nState		=	lua_tointeger(L, 2);
	BOOL		bSendMsg	=	lua_toboolean(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	if( nState <= 0 || nState >= ERS_End)	return 0;

	pRole->SetRoleState((ERoleState)nState, bSendMsg);

	if(ERS_Safeguard == nState)
	{
		// ���ظ��ͻ���
		tagNS_SafeGuard send;
		send.bOpen = TRUE;
		send.dwErrorCode = E_Success;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}

int SC_UnSetRoleState(lua_State* L)
{
	DWORD		dwRoleID	=	lua_tonumber(L, 1);
	INT			nState		=	lua_tointeger(L, 2);
	BOOL		bSendMsg	=	lua_toboolean(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	if( nState <= 0 || nState >= ERS_End)	return 0;

	pRole->UnsetRoleState((ERoleState)nState, bSendMsg);

	if(ERS_Safeguard == nState)
	{
		// ���ظ��ͻ���
		tagNS_SafeGuard send;
		send.bOpen = FALSE;
		send.dwErrorCode = E_Success;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �ܷ�������
//-------------------------------------------------------------------------------------
int SC_CanOpenChest(lua_State* L)
{
	DWORD dwMapID		=	lua_tonumber(L, 1);
	DWORD dwInstanceID	=	lua_tonumber(L, 2);
	DWORD dwRoleID		=	lua_tonumber(L, 3);
	DWORD dwChestID		=	lua_tonumber(L, 4);
	DWORD dwKeyID		=	lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagItemProto* pItemProto = g_attRes.GetItemProto(dwChestID);
	if (!P_VALID(pItemProto))
	{
		return 0;
	}
	lua_pushboolean(L, pItemProto->nSpecFuncVal2 == dwKeyID);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ͻ��˷�����Ϣ
//-------------------------------------------------------------------------------------
int SC_SendChestMsg(lua_State* L)
{
	DWORD dwMapID			=	lua_tonumber(L, 1);
	DWORD dwInstanceID		=	lua_tonumber(L, 2);
	DWORD dwRoleID			=	lua_tonumber(L, 3);
	DWORD dwChestID			=	lua_tonumber(L, 4);
	const string strMsgName =	lua_tostring(L, 5);
	BOOL bOpened			=	lua_toboolean(L, 6);
	BOOL bDestroy			=	lua_toboolean(L, 7);
	DWORD dwItemID			=	lua_tonumber(L, 8);
	INT nItemNum			=	lua_tointeger(L, 9);
	DWORD dwErrorCode		=	lua_tonumber(L, 10);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ������Ϣ���ͻ���
	g_TreasureChestMgr.SendMsg2Client(pRole, dwChestID, strMsgName, bOpened, bDestroy, dwItemID, nItemNum, dwErrorCode);

	return 0;
}

//-------------------------------------------------------------------------------------
// ʹ���籦�俪�������ͽ�ɫ���俪��������1
//-------------------------------------------------------------------------------------
int SC_IncTreasureSum(lua_State* L)
{
	DWORD dwMapID		=	lua_tonumber(L, 1);
	DWORD dwInstanceID	=	lua_tonumber(L, 2);
	DWORD dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	g_worldSession.IncTreasureSum();
	pRole->IncTreasureSum();
	
	return 0;
}

//-------------------------------------------------------------------------------------
// ������籦�俪�������ͽ�ɫ���俪������
//-------------------------------------------------------------------------------------
int SC_GetTreasureSum(lua_State* L)
{
	DWORD dwMapID		=	lua_tonumber(L, 1);
	DWORD dwInstanceID	=	lua_tonumber(L, 2);
	DWORD dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushinteger(L, pRole->GetTreasureSum());
	lua_pushinteger(L, g_worldSession.GetTreasureSum());

	return 2;
}

//-------------------------------------------------------------------------------------
// �õ���������Ʒ
//-------------------------------------------------------------------------------------
int SC_GetChestItem(lua_State* L)
{
	DWORD dwMapID		=	lua_tonumber(L, 1);
	DWORD dwInstanceID	=	lua_tonumber(L, 2);
	DWORD dwRoleID		=	lua_tonumber(L, 3);
	DWORD dwChestID		=	lua_tonumber(L, 4);
	ERateType eRate		=	(ERateType)lua_tointeger(L, 5);
	FLOAT fRand			=	(FLOAT)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagChestItem* pItem = g_TreasureChestMgr.GetRandomItem(dwChestID, eRate, fRand);
	if (!P_VALID(pItem))	return 0;

	DWORD dwItemTypeID = pItem->dwTypeID;	// ������Ʒtypeid
	INT nNum = pItem->nNum;					// ������Ʒ����
	lua_pushnumber(L, dwItemTypeID);
	lua_pushinteger(L, nNum);

	return 2;
}

//-------------------------------------------------------------------------------------
// ��Ʒ�Ƿ���Ҫ�㲥
//-------------------------------------------------------------------------------------
int SC_ItemNeedBroadcast(lua_State* L)
{
	DWORD dwTypeID	=	lua_tonumber(L, 1);

	// �����װ��
	if (MIsEquipment(dwTypeID))
	{
		// �ҵ�װ��
		tagEquipProto* pProto = g_attRes.GetEquipProto(dwTypeID);
		if (!P_VALID(pProto))	return 0;

		lua_pushnumber(L, pProto->bNeedBroadcast);
		return 1;
	}
	
	// ����Ʒ
	else
	{
		// �ҵ���Ʒ
		tagItemProto* pProto = g_attRes.GetItemProto(dwTypeID);
		if (!P_VALID(pProto))	return 0;

		lua_pushnumber(L, pProto->bNeedBroadcast);
		return 1;
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// �õ���������Ʒ
//-------------------------------------------------------------------------------------
int SC_StopMount(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->StopMount();

	return 0;
}

//-------------------------------------------------------------------------------------
// ֪ͨ���ѽ��븱��
//-------------------------------------------------------------------------------------
int SC_InstanceNotify(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);
	BOOL	bNotify		=	lua_toboolean(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	if(bNotify == FALSE)	return 0;

	// �ҵ����С��
	const Team* pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
	if( !P_VALID(pTeam) ) return 0;

	// �������Ƿ񴴽��˸���
	Map* pMap = g_mapCreator.GetMap(pTeam->GetOwnInstanceMapID(), pTeam->GetOwnInstanceID());
	if( !P_VALID(pMap) || EMT_Instance != pMap->GetMapType() ) return 0;

	MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
	if( !P_VALID(pInstance) ) return 0;

	const tagInstance* pInstanceProto = pInstance->GetInstanceProto();
	if( !P_VALID(pInstanceProto) ) return 0;	

	// �����Ƿ�����֪ͨ����
	if( !pInstanceProto->bNoticeTeamate ) return 0;

	// ֪ͨ����
	tagNS_InstanceAgree	 send;
	send.dwInviterID	=	pRole->GetID();
	send.dwMapID		=	pInstance->GetMapID();
	g_groupMgr.SendTeamInstanceNotify(pRole, pRole->GetTeamID(), &send, send.dwSize);

	return 0;
}
//-------------------------------------------------------------------------------------
// �Ƿ��Ѿ����
//-------------------------------------------------------------------------------------
static int SC_IsRoleMarried(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);


	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	lua_pushboolean(L, pRole->IsRoleMarried());

	return 1;
}
//-------------------------------------------------------------------------------------
// �Ƿ�Ϊ����
//-------------------------------------------------------------------------------------
static int SC_IsCouple(lua_State* L)
{
	DWORD	dwRoleID1	=	lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	lua_tonumber(L, 2);

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole1) )	return 0;

	if(pRole1->GetLoverID() == dwRoleID2)
	{
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}

	return 1;
}
//-------------------------------------------------------------------------------------
// �Ƿ��Ѿ����й�����
//-------------------------------------------------------------------------------------
static int SC_IsRoleWedded(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	lua_pushboolean(L, pRole->IsRoleWedded());

	return 1;
}
//-------------------------------------------------------------------------------------
// �õ��Ѻö�(���ܶ�)
//-------------------------------------------------------------------------------------
static int SC_GetFriendShipValue(lua_State* L)
{
	DWORD	dwRoleID1	=	lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole) )	return 0;

	tagFriend* pFriRole = pRole->GetFriendPtr(dwRoleID2);
	if( !P_VALID(pFriRole) )
	{
		lua_pushinteger(L, GT_INVALID);
	}
	else
	{
		lua_pushinteger(L, pFriRole->dwFriVal);
	}
	return 1;
}
//-------------------------------------------------------------------------------------
// �����Ѻö�
//-------------------------------------------------------------------------------------
static int SC_AddFriendShipValue(lua_State* L)
{
	DWORD	dwRoleID1	=	lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	lua_tonumber(L, 2);
	DWORD	dwAddFriVal	=	lua_tonumber(L, 3);

	if(dwAddFriVal <= 0)
		return 0;

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole1) )	return 0;

	Role* pRole2 = g_roleMgr.GetRolePtrByID(dwRoleID2);
	if( !P_VALID(pRole2) )	return 0;

	//�õ�role1������
	tagFriend* pRole1Fri = pRole1->GetFriendPtr(dwRoleID2);

	//�õ�role2������
	tagFriend* pRole2Fri = pRole2->GetFriendPtr(dwRoleID1);


	BOOL bInsert = TRUE;
	DWORD dwFriVal = 0;
	if(P_VALID(pRole1Fri))
	{
		if(pRole1Fri->dwFriVal > 0)
			bInsert = FALSE;

		dwFriVal = pRole1Fri->dwFriVal += dwAddFriVal;

		tagNS_UpdateFriValue send;
		send.dwRoleID = pRole1Fri->dwFriendID;
		send.nFriVal = dwFriVal;
		pRole1->SendMessage(&send, send.dwSize);

		//���³ƺ�	
		if(pRole1->GetLoverID() == pRole1Fri->dwFriendID)
			pRole1->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);

	}

	if(P_VALID(pRole2Fri))
	{
		if(pRole2Fri->dwFriVal > 0)
			bInsert = FALSE;

		dwFriVal = pRole2Fri->dwFriVal += dwAddFriVal;

		tagNS_UpdateFriValue send;
		send.dwRoleID = pRole2Fri->dwFriendID;
		send.nFriVal = dwFriVal;
		pRole2->SendMessage(&send, send.dwSize);

		
		//���³ƺ�	
		if(pRole2->GetLoverID() == pRole2Fri->dwFriendID)
			pRole2->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);
	}

	if(bInsert)
	{
		tagNDBC_InsertFriVal sendDB;
		sendDB.dwRoleID = (dwRoleID1 < dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.dwFriendID = (dwRoleID1 > dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.nFriVal = dwFriVal;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}
	else{
		tagNDBC_UpdateFriVal sendDB;
		sendDB.dwRoleID = (dwRoleID1 < dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.dwFriendID = (dwRoleID1 > dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.nFriVal = dwFriVal;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}

	return 1;
}
//-------------------------------------------------------------------------------------
// ���ӳƺ�
//-------------------------------------------------------------------------------------
int SC_SigTitleEvent(lua_State* L)
{
	DWORD			dwRoleID	= lua_tonumber(L, 1);
	DWORD			dwArg1		= lua_tonumber(L, 2);


	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	BOOL bHas = pRole->GetTitleMgr()->SigEvent(ETE_SCRIPT, dwArg1, GT_INVALID);

	lua_pushboolean(L, bHas);

	return 1;
}
//-------------------------------------------------------------------------------------
// ɾ���ƺ�
//-------------------------------------------------------------------------------------
int SC_SigRemoveTitleEvent(lua_State* L)
{
	DWORD			dwRoleID	= lua_tonumber(L, 1);
	DWORD			titleEvent	= lua_tonumber(L, 2);
	DWORD			dwArg1		= lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	pRole->GetTitleMgr()->SigRemoveEvent((EnumTitleEvent)titleEvent, dwArg1);

	return 1;
}
//-------------------------------------------------------------------------------------
// �ı似��������
//-------------------------------------------------------------------------------------
int SC_ChangeSkillExp(lua_State* L)
{
	DWORD			dwRoleID	=	lua_tonumber(L, 1);
	DWORD			dwSkillID	=	lua_tonumber(L, 2);
	DWORD			dwSkillExp	=	lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if( !P_VALID(pSkill) )
	{
		return 0;
	}

	pRole->ChangeSkillExp(pSkill, dwSkillExp);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����Ƿ���ĳbuff
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveBuff(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	DWORD	dwBuffID		=	lua_tonumber(L, 4);
	BOOL	bRet			=	FALSE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	bRet = pRole->IsHaveBuff(dwBuffID);
	lua_pushboolean(L, bRet);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����������Ҹ����
//-------------------------------------------------------------------------------------
int SC_QuestSetRebornMap(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	UINT16	u16QuestID		=	lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_BindRebornMap send;
	send.dwErrorCode = pRole->QuestSetRebornMap(u16QuestID, send.dwBornMapID, send.vBornPos);
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// �״����߽���Ʒͬ�����ͻ���
//-------------------------------------------------------------------------------------
int SC_VirginOnlineSendClient(lua_State* L)
{
	DWORD	dwRoleID	=	lua_tonumber(L, 1);
	INT64	n64Serial1	=	pop_64bit_data(L, 2, 3);
	INT64	n64Serial2	=	pop_64bit_data(L, 4, 5);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// ���ͻ��˷��ͳ���������Ϣ
	tagNS_VirginOnline send;
	send.n64Serial1 = n64Serial1;
	send.n64Serial2 = n64Serial2;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����ɹ���Ϣ���͸��ͻ���
//-------------------------------------------------------------------------------------
int SC_FishingSendClient(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_Fishing send;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	lua_tonumber(L, i*2+1+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		pRole->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �޸���ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_ModRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	lua_tonumber(L, i*2+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		pRole->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �����ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		dwValue = pRole->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// �����ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_CanMount(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;
	
	INT nRt = pRole->GetPetPocket()->CanRidePet();

	lua_pushinteger(L, nRt);

	return 1;
}

//-------------------------------------------------------------------------------------
// ���������������
//-------------------------------------------------------------------------------------
int SC_AddRoleExVolume(lua_State* L)
{
	DWORD   dwMapID			=	(INT64)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(INT64)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(INT64)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);



	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->GetCurMgr().IncExchangeVolume(nValue, (ELogCmdID)nCmdID);

	return 0;
}


//-------------------------------------------------------------------------------------
// �ж�����״̬
//-------------------------------------------------------------------------------------
int SC_IsRoleInStatus(lua_State* L)
{
	DWORD dwRoleID	= lua_tonumber(L, 1);
	DWORD dwStatus	= lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	
	lua_pushboolean(L, pRole->IsInRoleStateAll(dwStatus));

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ű�ͨ����Ϣ
//-------------------------------------------------------------------------------------
int SC_BeginMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	g_MsgInfoMgr.BeginMsgEvent();
	lua_pushnumber(L, dwMsgInfoID);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ϣ���������¼�����
//-------------------------------------------------------------------------------------
int SC_AddMsgEvent(lua_State* L)
{
	DWORD			dwMsgInfoID		=	lua_tonumber(L, 1);
	EMsgUnitType	eMsgUnitType	=	(EMsgUnitType)lua_tointeger(L, 2);
	LPVOID			pData			=	NULL;

	if(EMUT_RoleName == eMsgUnitType)
	{
		LPCTSTR pData = IUTIL->AnsiToUnicode(lua_tostring(L, 3));
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, eMsgUnitType, (LPVOID)pData);
	}
	else
	{
		DWORD dwData = lua_tonumber(L, 3);
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, eMsgUnitType, &dwData);
	}

	return 0;
}


//-------------------------------------------------------------------------------------
// ���ͽű�ͨ����Ϣ�����
//-------------------------------------------------------------------------------------
int SC_DispatchRoleMsgEvent(lua_State* L)
{	
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	DWORD	dwMsgInfoID		=	lua_tonumber(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, pRole);
	return 0;
}

//-------------------------------------------------------------------------------------
// �����������е�ͼ�ڵ���ҷ��ͽű�ͨ����Ϣ
//-------------------------------------------------------------------------------------
int SC_DispatchWorldMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	lua_tonumber(L, 1);

	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);

	return 0;
}

int SC_DispatchMapMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	lua_tonumber(L, 1);
	DWORD	dwMapID			=	lua_tonumber(L, 2);
	DWORD	dwInstanceID	=	lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	g_MsgInfoMgr.DispatchMapMsgEvent(dwMsgInfoID, pMap);

	return 0;
}

//-------------------------------------------------------------------------------------
// ְҵϵͳ�ӿ�
//-------------------------------------------------------------------------------------
int SC_IsRoleInaugural(lua_State* L)
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	DWORD	dwVocation		=	lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	INT nVocation = (INT)(pRole->GetClass());
	DWORD dwTmpStr = 1 << (nVocation-1);
	if( dwTmpStr & dwVocation )
		lua_pushnumber(L, 1);
	else
		lua_pushnumber(L, 0);

	return 0;
}

int SC_ChangeClass(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);
	DWORD dwNPCID = lua_tonumber(L, 2);
	INT nVocation = lua_tonumber(L, 3);
	
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->ChangeVocation(dwNPCID, (EClassType)nVocation);
	
	return 0;
}

//-------------------------------------------------------------------------------------
// �̶���ӿ�
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// ��ʼ���ʱ���¼��Ĵ���ʱ��
//-------------------------------------------------------------------------------------
int SC_InitEventTime(lua_State* L)
{
	const CHAR* szTableName		=   lua_tostring(L, 1);			// ���ʱ���table������
	INT			nNum			=   lua_tointeger(L, 2);		// ʱ���ĸ���
	DWORD		dwID			=   lua_tonumber(L, 3);			// �ID

	if(nNum <= 0)	return 0;

	CActivity *pActivity = g_activityMgr.GetActivity(dwID);
	if(!P_VALID(pActivity))	return 0;

	for(INT i = 1; i <= nNum; ++i)
	{
		lua_getglobal(L, szTableName);
		lua_pushnumber(L, i);
		lua_gettable(L, -2);
		DWORD	dwTime = lua_tointeger(L, -1);

		pActivity->AddEventTime(dwTime);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Ƿ�ʼ
//-------------------------------------------------------------------------------------
int SC_GetActIsStart(lua_State* L)
{
	BOOL		bStart		=	FALSE;
	DWORD		dwID		=	lua_tonumber(L, 1);			//�ID

	CActivity* pActivity = g_activityMgr.GetActivity(dwID);
	if( !P_VALID(pActivity) )	return 0;

	if( pActivity->IsStart() ) bStart = TRUE;

	lua_pushboolean(L, bStart);
	return 1;
}

//-------------------------------------------------------------------------------------
// ��Ƿ�ʼ
//-------------------------------------------------------------------------------------
int SC_AddAllRoleBuff(lua_State* L)
{
	DWORD	dwBuffTypeID		=	lua_tonumber(L, 1);
	tagAllRoleBuff 	AllRoleBuff;
	AllRoleBuff.dwBuffTypeID = dwBuffTypeID;
	g_groupMgr.AddEvent(GT_INVALID, EVT_AddAllRoleBuff, sizeof(tagAllRoleBuff), &AllRoleBuff);

	return 0;
}

//-------------------------------------------------------------------------------------
// ���ù̶���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetActScriptData(lua_State* L)
{
	DWORD	dwActID			=	lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ��
	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	lua_tonumber(L, i*2+1+3);

		pActivity->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ù̶���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetActScriptData(lua_State* L)
{
	DWORD	dwActID			=	lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ��
	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) ) return 0;

	if( nParamNum <= 0 ) return 0;

	std::vector<INT> vec;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+3);
		vec.push_back(nIndex);
	}

	std::vector<INT>::iterator it = vec.begin();
	while (it != vec.end())
	{
		dwValue = pActivity->GetScriptData(*it);
		lua_pushnumber(L, dwValue);
		++it;
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ����̶���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SaveActScriptData(lua_State* L)
{
	DWORD	dwActID			=	lua_tonumber(L, 1);

	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) )	return 0;
		
	pActivity->SaveActivityData2DB();

	return 0;
}

//-------------------------------------------------------------------------------------
// �������С��
//-------------------------------------------------------------------------------------
int SC_CreateTeam(lua_State* L)
{
	DWORD	dwSrcRoleID			=	lua_tonumber(L, 1);
	DWORD	dwDesRoleID			=	lua_tonumber(L, 2);

	tagCreateTeam	CreateTeam;
	CreateTeam.dwSrcRoleID = dwSrcRoleID;
	CreateTeam.dwDesRoleID = dwDesRoleID;
	g_groupMgr.AddEvent(GT_INVALID, EVT_CreateTeam, sizeof(tagCreateTeam), &CreateTeam);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����һ����ҵ����С��
//-------------------------------------------------------------------------------------
int SC_AddTeamMember(lua_State* L)
{
	DWORD	dwTeamID		=	lua_tonumber(L, 1);
	DWORD	dwDesRoleID		=	lua_tonumber(L, 2);

	tagAddTeamMember	AddTeamMem;
	AddTeamMem.dwTeamID = dwTeamID;
	AddTeamMem.dwDesRoleID = dwDesRoleID;

	g_groupMgr.AddEvent(GT_INVALID, EVT_AddMember, sizeof(tagAddTeamMember), &AddTeamMem);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ�С�Ӷӳ���ID
//-------------------------------------------------------------------------------------
int SC_GetTeamLeaderID(lua_State* L)
{
	DWORD	dwTeamID		=	lua_tonumber(L, 1);

	const Team*	pTeam		=	g_groupMgr.GetTeamPtr(dwTeamID);
	if ( !P_VALID(pTeam) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	lua_pushnumber(L, pTeam->GetMemID(0));

	return 1;
}


//-------------------------------------------------------------------------------------
// ���õ�ͼ�ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetMapScriptData(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	INT		dwInstanceID	=	lua_tointeger(L, 2);
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	lua_tonumber(L, i*2+1+4);

		pMap->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��õ�ͼ�ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetMapScriptData(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	if( nParamNum <= 0 ) return 0;

	std::vector<INT> vec;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+4);
		vec.push_back(nIndex);
	}

	std::vector<INT>::iterator it = vec.begin();
	while (it != vec.end())
	{
		dwValue = pMap->GetScriptData(*it);
		lua_pushnumber(L, dwValue);
		++it;
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ��ͼ��̬ˢ������ײ����
//-------------------------------------------------------------------------------------
int SC_MapCreateCreature(lua_State* L)
{
 	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwTypeID		=	lua_tonumber(L, 3);
	FLOAT	fx				=	lua_tonumber(L, 4);
	FLOAT	fy				=	lua_tonumber(L, 5);
	FLOAT	fz				=	lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Vector3 vPos;
	vPos.x = fx * TILE_SCALE;
	vPos.y = fy;
	vPos.z = fz * TILE_SCALE;

	// ������ﳯ��
	Vector3 vFaceTo;
	FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
	vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.y = 0.0f;

	Creature *pCreature = pMap->CreateCreature(dwTypeID, vPos, vFaceTo);

	if (P_VALID(pCreature))
	{
		lua_pushnumber(L, pCreature->GetID());
		return 1;
	}

	lua_pushnumber(L, GT_INVALID);
	return 1;

}

//-------------------------------------------------------------------------------------
// ��̬ˢ����ײ����
//-------------------------------------------------------------------------------------
int SC_MapCreateColCreature(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwTypeID		=	lua_tonumber(L, 3);
	FLOAT	fx				=	lua_tonumber(L, 4);
	FLOAT	fy				=	lua_tonumber(L, 5);
	FLOAT	fz				=	lua_tonumber(L, 6);
	BOOL	bCollide		=	lua_toboolean(L, 7);
	CHAR*	pPatrolListName	=   const_cast<CHAR*>(lua_tostring(L, 8));


	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Vector3 vPos;
	vPos.x = fx * TILE_SCALE;
	vPos.y = fy;
	vPos.z = fz * TILE_SCALE;

	// ������ﳯ��
	Vector3 vFaceTo;
	FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
	vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.y = 0.0f;

	Creature *pCreature = pMap->CreateCreature(dwTypeID, vPos, vFaceTo, GT_INVALID, bCollide, pPatrolListName);

	if (P_VALID(pCreature))
	{
		lua_pushnumber(L, pCreature->GetID());
		return 1;
	}

	lua_pushnumber(L, GT_INVALID);
	return 1;
}

//-------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------
int SC_MapDeleteCreature(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwID			=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	pMap->DeleteCreature(dwID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------
int SC_CreateInstance(lua_State* L)
{
	DWORD	dwMapID 		=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr))		return 0;

	BOOL	bRet = pMapMgr->CreateScriptInstance(dwInstanceID);

	lua_pushboolean(L, bRet);

	return 1;	
}

//-------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------
int SC_DeleteInstance(lua_State* L)
{
	DWORD   dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr))		return 0;

	MapInstance* pInstance = pMapMgr->GetInstance(dwInstanceID);
	if( !P_VALID(pInstance))			return 0;

	pInstance->SetDelete();
	
	return 0;
}

//-------------------------------------------------------------------------------------
// ���ų�����Ч
//-------------------------------------------------------------------------------------
int SC_PlaySceneEffect(lua_State* L)
{
	DWORD dwMapID			= lua_tonumber(L, 1);
	DWORD dwInstanceID		= lua_tonumber(L, 2);
	ESceneEffectType eType	= (ESceneEffectType)lua_tointeger(L, 3);
	DWORD dwObjID			= lua_tonumber(L, 4);
	DWORD dwEffectID		= lua_tonumber(L, 5);
	FLOAT fPosX				= lua_tonumber(L, 6);
	FLOAT fPosY				= lua_tonumber(L, 7);
	FLOAT fPosZ				= lua_tonumber(L, 8);

	Vector3 vPos(fPosX, fPosY, fPosZ);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->PlaySceneEffect(eType, dwObjID, vPos, dwEffectID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ֹͣ������Ч
//-------------------------------------------------------------------------------------
int SC_StopSceneEffect(lua_State* L)
{
	DWORD dwMapID		= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwObjID		= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->StopSceneEffect(dwObjID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��/�رյ�ͼ��ָ������
//-------------------------------------------------------------------------------------
int SC_OpenCloseDoor(lua_State* L)
{
	DWORD dwMapID		= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwObjID		= lua_tonumber(L, 3);
	BOOL bOpen			= lua_toboolean(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->OpenCloseDoor(dwObjID, bOpen);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��/�رյ�ͼ�����е���
//-------------------------------------------------------------------------------------
int SC_OpenCloseAllDoors(lua_State* L)
{
	DWORD dwMapID		= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	BOOL bOpen			= lua_toboolean(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->OpenCloseAllDoors(bOpen);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����ͼ���е��������һ��buff
//-------------------------------------------------------------------------------------
int SC_AddMapRoleBuff(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwBuffTypeID	=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return 0;

	Map::RoleMap::TMapIterator itRole = pMap->GetRoleMap().Begin();
	Role* pRole = NULL;
	while (pMap->GetRoleMap().PeekNext(itRole, pRole))
	{
		pRole->TryAddBuff(pRole, pProto, NULL, NULL, NULL);
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// ����Ƿ���ĳһ�ű����͵�����
//-------------------------------------------------------------------------------------
int SC_IsInArea(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwRoleID		=	lua_tonumber(L, 3);
	DWORD	dwObjID			=	lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	const tagMapInfo* pMapInfo = pMap->GetMapInfo();
	if( !P_VALID(pMapInfo) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	Role* pRole = pMap->FindRole(dwRoleID);
	if( !P_VALID(pRole) )	
	{
		lua_pushboolean(L, false);
		return 1;
	}

	// �õ���ҵİ����к͸�������
	AABBox roleBox = pRole->GetAABBox();
	INT nTileX = INT(pRole->GetCurPos().x / (FLOAT)TILE_SCALE);
	INT nTileZ = INT(pRole->GetCurPos().z / (FLOAT)TILE_SCALE);

	tagMapAreaInfo* pArea = NULL;

	// ���ű���
	pArea = pMap->IsInArea(pMapInfo->mapScriptArea, roleBox, nTileX, nTileZ);
	if( !P_VALID(pArea) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	if (pArea->dwObjID == dwObjID)
	{
		lua_pushboolean(L, true);
	}
	else
	{
		lua_pushboolean(L, false);
	}

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ù�����Χ�Ĺ���
//-------------------------------------------------------------------------------------
int SC_GetAroundCreature(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	FLOAT	fOPRadius		=   lua_tonumber(L, 4);
	FLOAT	fHigh			=	lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	std::vector<DWORD> vecCreature;
	INT nCreatureNum = 0;
	INT Index = 1;
	fOPRadius = fOPRadius * TILE_SCALE;
	nCreatureNum = pCreature->GetAroundCreature(vecCreature, fOPRadius, fHigh);

	lua_createtable(L,nCreatureNum,0);

	std::vector<DWORD>::iterator it = vecCreature.begin();
	while(it != vecCreature.end())
	{
		lua_pushnumber(L, Index);
		lua_pushnumber(L, *it);
		lua_settable(L,-3);  
		++it;
		++Index;
	}

	return 1;
};

//-------------------------------------------------------------------------------------
// ��ù�����Χ�����
//-------------------------------------------------------------------------------------
int SC_GetAroundRole(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	FLOAT	fOPRadius		=   lua_tonumber(L, 4);
	FLOAT	fHigh			=	lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	std::vector<DWORD> vecRole;
	INT nRoleNum = 0;
	INT Index = 1;
	fOPRadius = fOPRadius * TILE_SCALE;
	nRoleNum = pCreature->GetAroundRole(vecRole, fOPRadius, fHigh);

	lua_createtable(L,nRoleNum,0);

	std::vector<DWORD>::iterator it = vecRole.begin();
	while(it != vecRole.end())
	{
		lua_pushnumber(L, Index);
		lua_pushnumber(L, *it);
		lua_settable(L,-3);  
		++it;
		++Index;
	}

	return 1;
}

//-------------------------------------------------------------------------------------------
// ��ù���Ľű�����
//-------------------------------------------------------------------------------------------
int SC_GetCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// �õ���������
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+5);
		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		dwValue = pCreature->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ���ù���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ��������
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+5);
		dwValue		=	lua_tonumber(L, i*2+6);

		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		pCreature->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �ı����ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_ModCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ��������
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+5);
		dwValue		=	lua_tonumber(L, i*2+6);

		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		pCreature->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//------------------------------------------------------------------------------------------
// ���ù�����½ű�AI��ʱ��������tickΪ��λ
//------------------------------------------------------------------------------------------
int SC_SetCreatureUpdateAITimer(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	INT		nTimer			=	lua_tointeger(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	if( nTimer <= 0 && nTimer != GT_INVALID ) return 0;

	pCreature->GetAI()->SetScriptUpdateTimer(nTimer);

	return 0;
}

//------------------------------------------------------------------------------------------
// ����ʹ�ü���
//------------------------------------------------------------------------------------------
int SC_CreatureUseSkill(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	DWORD	dwSkillID		=	lua_tonumber(L, 4);
	INT		nTargetType		=	lua_tointeger(L, 5);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// �õ�Ŀ��ID
	DWORD dwTargetID = pCreature->GetAI()->GetTargetIDByType((ECreatureTargetFriendEnemy)nTargetType);

	// ʹ�ü���
	pCreature->GetAI()->AIUseSkill(dwSkillID, dwTargetID);

	return 0;
}

//---------------------------------------------------------------------------------------------
// ���ﺰ��
//----------------------------------------------------------------------------------------------
int SC_MonsterSay(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	DWORD	dwSayID			=	lua_tonumber(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ����
	pCreature->Say(dwSayID);

	return 0;
}

//----------------------------------------------------------------------------------------------
// ���ﲥ�Ŷ���
//----------------------------------------------------------------------------------------------
int SC_MonsterPlayAction(lua_State* L)
{
	DWORD		dwMapID			=	lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	lua_tonumber(L, 3);
	const CHAR*	szFourCC		=	lua_tostring(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ����Ƿ�Ϸ���fourCCֻ�����ĸ���С���ַ�
	if( !P_VALID(szFourCC) || strlen(szFourCC) != 4 ) return 0;

	// ת����ID
	DWORD dwActionID = Cool3D::_MakeFourCCA(szFourCC);

	pCreature->PlayerAction(dwActionID);

	return 0;
}

//--------------------------------------------------------------------------------------------------
// �����л�AI״̬
//--------------------------------------------------------------------------------------------------
int SC_CreatureChangeAIState(lua_State* L)
{
	DWORD		dwMapID			=	lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	lua_tonumber(L, 3);
	INT			nState			=	lua_tointeger(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// �л�״̬
	pCreature->GetAI()->ChangeState((AIStateType)nState);

	return 0;
}

//--------------------------------------------------------------------------------------------------
// �������ս�����ʱ��
//--------------------------------------------------------------------------------------------------
int SC_GetEnterCombatTick(lua_State* L)
{
	DWORD		dwMapID			=	lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	lua_tonumber(L, 3);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// �õ�����ս��ʱ��
	DWORD dwTick = pCreature->GetAI()->GetEnterCombatTick();

	lua_pushnumber(L, dwTick);

	return 1;
}

//--------------------------------------------------------------------------------------------------
// ���ݹ���ID�õ�TypeID
//--------------------------------------------------------------------------------------------------
int SC_GetCreatureTypeID(lua_State* L)
{
	DWORD		dwMapID			=	lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	lua_tonumber(L, 3);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	DWORD dwTypeID = pCreature->GetTypeID();

	lua_pushnumber(L, dwTypeID);

	return 1;
}

//-------------------------------------------------------------------------------------
// ���ӹ�����
//-------------------------------------------------------------------------------------
int SC_AddEnmity(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);
	DWORD	dwTargetID		=	lua_tonumber(L, 4);
	DWORD	dwValue			=	lua_tonumber(L, 5);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	Unit* pTarget = pMap->FindUnit(dwTargetID);
	if( !P_VALID(pTarget) ) return 0;

	pCreature->GetAI()->AddEnmity(pTarget, dwValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡ���ﵱǰ�Ĺ���Ŀ��
//-------------------------------------------------------------------------------------
int SC_GetCreatureCurTargetID(lua_State* L)
{
	DWORD	dwMapID			=	lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	lua_tonumber(L, 3);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) 
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	DWORD	dwTargetID = pCreature->GetAI()->GetTargetUnitID();

	lua_pushnumber(L, dwTargetID);

	return 1;
}

//-------------------------------------------------------------------------------------
// �������������辭��
//-------------------------------------------------------------------------------------
int SC_GetRoleLevelUpExp(lua_State* L)
{
	DWORD   dwMapID			=	lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	lua_tonumber(L, 2);
	DWORD   dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ��õ�ǰ�������辭��
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(pRole->GetLevel());
	lua_pushinteger(L, pEffect->nExpLevelUp);
	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ƿ�����
//-------------------------------------------------------------------------------------
int SC_IsRoleOnLine(lua_State* L)
{
	DWORD   dwMapID			=	lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	lua_tonumber(L, 2);
	DWORD   dwRoleID		=	lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
	{
		lua_pushinteger(L, 0);
	}
	else
	{
		lua_pushinteger(L, 1);
	}

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ���ɳ�Աְλ
//---------------------------------------------------------------------------------
static int SC_GetGuildPos(lua_State* L)
{
	DWORD dwGuildID	= lua_tonumber(L, 1);
	DWORD dwRoleID	= lua_tonumber(L, 2);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// ������
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// ���ڰ���
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return 0;
	}

	tagGuildMember* pMember = pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		// ���ڰ�����
		return 0;
	}

	lua_pushinteger(L, pMember->eGuildPos);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ�����ʽ�
//---------------------------------------------------------------------------------
static int SC_GetGuildFund(lua_State* L)
{
	DWORD dwGuildID	= lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nFund);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ�����ʲ�
//---------------------------------------------------------------------------------
static int SC_GetGuildMaterial(lua_State* L)
{
	DWORD dwGuildID	= lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nMaterial);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ���ɰ�����
//---------------------------------------------------------------------------------
static int SC_GetGuildPeace(lua_State* L)
{
	DWORD dwGuildID	= lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().n16Peace);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ��������
//---------------------------------------------------------------------------------
static int SC_GetGuildReputation(lua_State* L)
{
	DWORD dwGuildID	= lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nReputation);

	return 1;
}

//---------------------------------------------------------------------------------
// ������״̬
//---------------------------------------------------------------------------------
static int SC_IsGuildInStatus(lua_State* L)
{
	DWORD dwGuildID	= lua_tonumber(L, 1);
	DWORD dwStatus	= lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushboolean(L, pGuild->IsInGuildStateAll(dwStatus));

	return 1;
}

//---------------------------------------------------------------------------------
// �����ʽ���
//---------------------------------------------------------------------------------
static int SC_ModifyGuildFund(lua_State* L)
{
	DWORD dwGuildID		= lua_tonumber(L, 1);
	DWORD dwRoleID		= lua_tonumber(L, 2);
	INT32 nFund			= lua_tonumber(L, 3);
	DWORD dwLogCmdID	= lua_tointeger(L, 4);

	if (nFund == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nFund > 0)
	{
		pGuild->IncGuildFund(dwRoleID, nFund, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildFund(dwRoleID, -nFund, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// �����ʲı��
//---------------------------------------------------------------------------------
static int SC_ModifyGuildMaterial(lua_State* L)
{
	DWORD dwGuildID		= lua_tonumber(L, 1);
	DWORD dwRoleID		= lua_tonumber(L, 2);
	INT32 nMaterial		= lua_tonumber(L, 3);
	DWORD dwLogCmdID	= lua_tointeger(L, 4);

	if (nMaterial == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nMaterial > 0)
	{
		pGuild->IncGuildMaterial(dwRoleID, nMaterial, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildMaterial(dwRoleID, -nMaterial, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ���ɰ����ȱ��
//---------------------------------------------------------------------------------
static int SC_ModifyGuildPeace(lua_State* L)
{
	DWORD dwGuildID		= lua_tonumber(L, 1);
	DWORD dwRoleID		= lua_tonumber(L, 2);
	INT16 n16Peace		= lua_tonumber(L, 3);
	DWORD dwLogCmdID	= lua_tointeger(L, 4);

	if (n16Peace == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (n16Peace > 0)
	{
		pGuild->IncGuildPeace(dwRoleID, n16Peace, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildPeace(dwRoleID, -n16Peace, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ���ɳ�Ա���ױ��
//---------------------------------------------------------------------------------
static int SC_ModifyGuildReputation(lua_State* L)
{
	DWORD dwGuildID		= lua_tonumber(L, 1);
	DWORD dwRoleID		= lua_tonumber(L, 2);
	INT32 nReputation	= lua_tonumber(L, 3);
	DWORD dwLogCmdID	= lua_tointeger(L, 4);

	if (nReputation == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nReputation > 0)
	{
		pGuild->IncGuildReputation(dwRoleID, nReputation, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildReputation(dwRoleID, -nReputation, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ���ɳ�Ա���ױ��
//---------------------------------------------------------------------------------
static int SC_ModifyContribution(lua_State* L)
{
	DWORD dwGuildID		= lua_tonumber(L, 1);
	DWORD dwRoleID		= lua_tonumber(L, 2);
	INT32 nContribute	= lua_tonumber(L, 3);
	DWORD dwLogCmdID	= lua_tointeger(L, 4);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// ������
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// ���ڰ���
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return 0;
	}

	if (nContribute > 0)
	{
		pGuild->ChangeMemberContribution(dwRoleID, nContribute, TRUE);
	}
	else
	{
		pGuild->ChangeMemberContribution(dwRoleID, -nContribute, FALSE);
	}

	return 0;
}

int SC_CanGather( lua_State* L )
{
	DWORD	dwRoleID		=	lua_tonumber(L, 1);
	DWORD	dwCreatureID	=	lua_tonumber(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) || !P_VALID(pRole->GetMap())) return 0;

	Creature* pRes = pRole->GetMap()->FindCreature(dwCreatureID);
	if (!P_VALID(pRes)) return 0;

	INT nRt = pRole->CanGather(pRes);

	lua_pushinteger(L, nRt);

	return 1;
}