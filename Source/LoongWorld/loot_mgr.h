//------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loot_mgr.h
// author: 
// actor:
// data: 2008-10-27
// last:
// brief: ������Ʒ������
// remark: ��ȡ�������ѯ�����
//------------------------------------------------------------------------------------------------------
#pragma once

#include "singleton.h"
#include "..\WorldDefine\loot.h"

// ������λ����
const INT MAX_LOOT_POS		= 24;

struct tagLoot;
struct tagCreatureLoot;
struct tagLootItemSet;
struct tagLootQuestItem;
class Creature;
class Role;
class Team;

//-------------------------------------------------------------------------------------------------------
// ���䷽ʽ
//-------------------------------------------------------------------------------------------------------
enum ELootTo
{
	ELT_BagFirst	= 0,	// �����뱳���������������
	ELT_ForceBag	= 1,	// ���뱳��
	ELT_Ground		= 2,	// ����
};

//-----------------------------------------------------------------------------------------------------
// ���ֵ�������
//-----------------------------------------------------------------------------------------------------
typedef TMap<DWORD, tagCreatureLoot*>	CreatureLootMap;	// <����ID,����ṹָ��>
typedef TMap<DWORD, tagLootItemSet*>	LootItemSetMap;		// <������Ʒ����ID,������Ʒ���Ͻṹָ��>
typedef TMap<DWORD, tagLootQuestItem*>	LootQuestItemMap;	// <����ID,����������Ʒ�ṹָ��>

//-----------------------------------------------------------------------------------------------------
// ���������
//-----------------------------------------------------------------------------------------------------
class LootMgr : public Singleton<LootMgr>
{	
public:
	LootMgr();
	~LootMgr();
	
public:
    //-----------------------------------------------------------------------------------------
	// ��ʼ��������
	//-----------------------------------------------------------------------------------------
	BOOL					Init();
	VOID					Destroy();

public:
	//----------------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------------
	VOID					MonsterLoot(Creature* pCreature, Role* pPicker);

	//----------------------------------------------------------------------------------------
	// ��Դ����
	//----------------------------------------------------------------------------------------
	VOID					ResourceLoot(Creature* pCreature, Role* pPicker);

	//----------------------------------------------------------------------------------------
	// ����������
	//----------------------------------------------------------------------------------------
	VOID					InvestigateLoot(Creature* pCreature, Role* pPicker);

private:
	//----------------------------------------------------------------------------------------
	// ���ֵ������Լ���
	//----------------------------------------------------------------------------------------
	BOOL					LoadCreatureLoot();
	BOOL					LoadItemSetLoot();
	BOOL					LoadQuestItemLoot();

	//----------------------------------------------------------------------------------------
	// ��ȡ����ָ��
	//----------------------------------------------------------------------------------------
	const tagCreatureLoot*	GetCreatureLoot(DWORD dwCreatureID)		{ return m_mapCreatureLoot.Peek(dwCreatureID); }
	const tagLootItemSet*	GetLootItemSet(DWORD dwSetID)			{ return m_mapLootItemSet.Peek(dwSetID); }
	const tagLootQuestItem*	GetLootQuestItem(DWORD dwCreatureID)	{ return m_mapLootQuestItem.Peek(dwCreatureID); }

	//----------------------------------------------------------------------------------------
	// ��Ҫ���䷽��
	//----------------------------------------------------------------------------------------
	VOID					NormalLoot(Creature* pCreature, Role* pRole, ELootTo eLootTo, BOOL bSingelLoot, BOOL bTeam);
	VOID					QuestLoot(Creature* pCreature, Role* pRole, BOOL bTeam);

	//----------------------------------------------------------------------------------------
	// ��ȡ��һ������λ��
	//----------------------------------------------------------------------------------------
	INT						GetPutDownPos(Creature* pCreature, Vector3 &vPos, INT &nIndex);

	//----------------------------------------------------------------------------------------
	// �����ж�
	//----------------------------------------------------------------------------------------
	FLOAT					GetCreatureLootChanceAdd(Creature* pCreature, Role* pReward,INT addType = ERA_Loot_Add_Rate);
	FLOAT					GetMosterLootChanceAdd(Role* pReward);
	FLOAT					GetResLootChanceAdd(Creature* pCreature, Role* pReward);
	FLOAT					GetInvesLootChanceAdd(Role* pReward);

	// Jason 2009-11-28 ����˥��
	FLOAT					GetMosterLootChanceAddEx(Creature* pCreature,Role* pReward,INT addType = ERA_Loot_Add_Rate);

	//----------------------------------------------------------------------------------------
	// �õ�ĳ�����������
	//----------------------------------------------------------------------------------------
	VOID					GetLootOwner(Creature* pCreature, Role* pPicker, BOOL bTeam, DWORD& dwOwnerID, Role* &pOwnerRole, DWORD& dwTeamID);

	//-----------------------------------------------------------------------------------------
	// ���ֵ��䷽ʽ
	//-----------------------------------------------------------------------------------------
	BOOL					LootItem(Creature* pCreature, tagItem* &pItem, ELootTo eLootTo, Role* pOwnerRole, DWORD dwOwnerID, DWORD dwTeamID, INT& nIndex);

	//----------------------------------------------------------------------------------------
	// ���ֵ��䷽��
	//----------------------------------------------------------------------------------------
	BOOL					DropItemTo(const tagLoot* pLoot, Creature* pCreature, ELootTo eLootTo, Role* pPicker, BOOL bTeam, FLOAT fChanceAdd, INT &nIndex);
	BOOL					DropSetTo(const tagLoot* pLoot, Creature* pCreature, ELootTo eLootTo, Role* pPicker, BOOL bTeam, FLOAT fChanceAdd, INT &nIndex);
	BOOL					DropMoneyTo(const tagCreatureLoot* pLootProto, Creature* pCreature, ELootTo eLootTo, Role* pPicker, FLOAT fChanceAdd, INT &nIndex);
	BOOL					DropQuestItem(Role* pPicker, const tagItemProto* pItemProto, DWORD dwCreatureID);

private:
	TObjRef<VarContainer>	m_pVar;
	TObjRef<Util>			m_pUtil;

	//-----------------------------------------------------------------------------------------
	// ���ֵ���
	//-----------------------------------------------------------------------------------------
	CreatureLootMap			m_mapCreatureLoot;		// �������
	LootItemSetMap			m_mapLootItemSet;		// ������Ʒ����
	LootQuestItemMap		m_mapLootQuestItem;		// ����������Ʒ
};

#define sLootMgr LootMgr::getSingleton()

