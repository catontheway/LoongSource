//------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loot_mgr.cpp
// author: 
// actor:
// data: 2008-10-23
// last: 2008-10-27
// brief: ���������
//------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/loot.h"
#include "../WorldDefine/msg_loot.h"
#include "../WorldDefine/chat.h"
#include "../WorldDefine/chat_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "loot_mgr.h"
#include "unit.h"
#include "role.h"
#include "item_creator.h"
#include "creature.h"

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
LootMgr::LootMgr()
{
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
LootMgr::~LootMgr()
{
	Destroy();
}

//-------------------------------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::Init()
{
	m_pUtil = "Util";

	// ���ع������
	if( !LoadCreatureLoot() )
	{
		return FALSE;
	}

	// ���ص��伯��
	if( !LoadItemSetLoot() )
	{
		return FALSE;
	}

	// ����������Ʒ����
	if( !LoadQuestItemLoot() )
	{
		return FALSE;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���ع�����Դ
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::LoadCreatureLoot()
{
	// ���ɵ�������
	CreateObj("CreatureLoot", "VarContainer");
	m_pVar = "CreatureLoot";

	// �������ļ� world.ini �еõ������ļ�·��
	TObjRef<VarContainer> pVar = "VarContainer";

	// �õ������ļ��й��������·��
	tstring strCreatureLoot	= pVar->GetString(_T("creature_loot"),	_T("loot"));

	// ���ع�������ļ�
	std::list<tstring> listField;
	m_pVar->Load(NULL, strCreatureLoot.c_str(), "id", &listField);

	// һ��һ���ļ��ع�������ļ�
	for(std::list<tstring>::iterator it = listField.begin(); it != listField.end(); ++it)
	{
		tagCreatureLoot* pCreatureLoot = new tagCreatureLoot;
		ZeroMemory(pCreatureLoot, sizeof(tagCreatureLoot));

		pCreatureLoot->dwLootID		=	_tcstol(it->c_str(), NULL, 10);
		pCreatureLoot->eOpType		=	(ELootOpType)m_pVar->GetDword(_T("optype"), it->c_str(), 0);
		pCreatureLoot->eLootMode	=	(ELootMode)m_pVar->GetDword(_T("mode"), it->c_str(), 0);
		pCreatureLoot->nMinMoney	=	m_pVar->GetDword(_T("min_money"), it->c_str(), 0);
		pCreatureLoot->nMaxMoney	=	m_pVar->GetDword(_T("max_money"), it->c_str(), 0);

		// �������Ʒ
		for(INT i = 0; i < MAX_CREATURE_LOOT_NUM; ++i)
		{
			tstringstream ss;
			ss << _T("item_") << i+1 << _T("_id");
			pCreatureLoot->Loot[i].dwItemID = m_pVar->GetDword(ss.str().c_str(), it->c_str(), 0);
			ss.str(_T(""));
			ss << _T("item_") << i+1 << _T("_type");
			pCreatureLoot->Loot[i].eLootType = (ELootType)m_pVar->GetInt(ss.str().c_str(), it->c_str(), 0);
			ss.str(_T(""));
			ss << _T("item_") << i+1 << _T("_max");
			pCreatureLoot->Loot[i].nMax = m_pVar->GetInt(ss.str().c_str(), it->c_str(), 0);
			ss.str(_T(""));
			ss << _T("item_") << i+1 << _T("_min");
			pCreatureLoot->Loot[i].nMin = m_pVar->GetInt(ss.str().c_str(), it->c_str(), 0);
			ss.str(_T(""));
			ss << _T("item_") << i+1 << _T("_chance");
			pCreatureLoot->Loot[i].fChance = m_pVar->GetFloat(ss.str().c_str(), it->c_str(), 0.0f);
		}

		m_mapCreatureLoot.Add(pCreatureLoot->dwLootID, pCreatureLoot);
	}

	m_pVar->Clear();
	KillObj("CreatureLoot");

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���ص��伯��
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::LoadItemSetLoot()
{
	// ���ɵ�������
	CreateObj("ItemSetLoot", "VarContainer");
	m_pVar = "ItemSetLoot";

	// �������ļ� world.ini �еõ������ļ�·��
	TObjRef<VarContainer> pVar = "VarContainer";
	tstring strLootItemSet = pVar->GetString(_T("loot_item_set"),	_T("loot"));

	// ���ع�������ļ�
	std::list<tstring> listField;
	// ���ص�����Ʒ�����ļ�
	m_pVar->Load(NULL, strLootItemSet.c_str(), "id", &listField);

	for(std::list<tstring>::iterator it = listField.begin(); it != listField.end(); ++it)
	{
		tagLootItemSet* pLootItemSet = new tagLootItemSet;
		ZeroMemory(pLootItemSet, sizeof(tagLootItemSet));

		pLootItemSet->dwSetID = _tcstol(it->c_str(), NULL, 10);

		// ������Ʒ
		for(INT i = 0; i < MAX_ITEM_SET_NUM; ++i)
		{
			tstringstream ss;
			ss << _T("item_") << i+1 << _T("_id");
			pLootItemSet->ItemSet[i].dwItemID = m_pVar->GetDword(ss.str().c_str(), it->c_str(), 0);
			ss.str(_T(""));
			ss << _T("item_") << i+1 << _T("_num");
			pLootItemSet->ItemSet[i].nItemNum = m_pVar->GetDword(ss.str().c_str(), it->c_str(), 0);
		}

		// ����map
		m_mapLootItemSet.Add(pLootItemSet->dwSetID, pLootItemSet);
	}

	m_pVar->Clear();
	KillObj("ItemSetLoot");

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// �����������
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::LoadQuestItemLoot()
{
	// ���ɵ�������
	CreateObj("QuestItemLoot", "VarContainer");
	m_pVar = "QuestItemLoot";

	// �������ļ� world.ini �еõ������ļ�·��
	TObjRef<VarContainer> pVar = "VarContainer";
	tstring strLootQuestItem = pVar->GetString(_T("loot_quest_item"),	_T("loot"));

	// ���ع�������ļ�
	std::list<tstring> listField;
	// �������������Ʒ�ļ�
	m_pVar->Load(NULL, strLootQuestItem.c_str(), "id", &listField);

	for(std::list<tstring>::iterator it = listField.begin(); it != listField.end(); ++it)
	{
		tagLootQuestItem* pLootQuestItem = new tagLootQuestItem;
		ZeroMemory(pLootQuestItem, sizeof(tagLootQuestItem));

		pLootQuestItem->dwCreatureID = _tcstol(it->c_str(), NULL, 10);

		for(INT i = 0; i < MAX_LOOT_QUEST_ITEM_NUM; ++i)
		{
			tstringstream ss;
			ss << _T("item_") << i+1 << _T("_id");
			pLootQuestItem->QuestItem[i].dwQuestItemID  = m_pVar->GetDword(ss.str().c_str(), it->c_str(), 0);
			ss.str(_T(""));
			ss << _T("item_") << i+1 << _T("_chance");
			pLootQuestItem->QuestItem[i].fChance = m_pVar->GetFloat(ss.str().c_str(), it->c_str(), 0);
			ss.str(_T(""));
			ss << _T("item_") << i+1 << _T("_team_chance");
			pLootQuestItem->QuestItem[i].fTeamChance = m_pVar->GetFloat(ss.str().c_str(), it->c_str(), 0);
		}

		m_mapLootQuestItem.Add(pLootQuestItem->dwCreatureID, pLootQuestItem);
	}

	m_pVar->Clear();
	KillObj("QuestItemLoot");

	return TRUE;
}


//-------------------------------------------------------------------------------------------------------
// �ͷ���Դ
//-------------------------------------------------------------------------------------------------------
VOID LootMgr::Destroy()
{
	// �ͷ� �������
	m_mapCreatureLoot.ResetIterator();
	tagCreatureLoot* pCreatureLoot = NULL;
	while (m_mapCreatureLoot.PeekNext(pCreatureLoot))
	{
		SAFE_DEL(pCreatureLoot);
	}
	m_mapCreatureLoot.Clear();

	// �ͷ� ������Ʒ����
	m_mapLootItemSet.ResetIterator();
	tagLootItemSet* pLootItemSet = NULL;
	while (m_mapLootItemSet.PeekNext(pLootItemSet))
	{
		SAFE_DEL(pLootItemSet);
	}
	m_mapLootItemSet.Clear();

	// �ͷ� �������
	m_mapLootQuestItem.ResetIterator();
	tagLootQuestItem* pLootQuestItem = NULL;
	while (m_mapLootQuestItem.PeekNext(pLootQuestItem))
	{
		SAFE_DEL(pLootQuestItem);
	}
	m_mapLootQuestItem.Clear();
}

//---------------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------------
VOID LootMgr::MonsterLoot(Creature* pCreature, Role* pPicker)
{
	if( !P_VALID(pCreature) || !P_VALID(pPicker) )
		return;

	// ����С���������
	QuestLoot(pCreature, pPicker, TRUE);

	// �����ڵ��棬�ǵ�һ���䣬 С�ӵ���
	NormalLoot(pCreature, pPicker, ELT_Ground, FALSE, TRUE);
}

//----------------------------------------------------------------------------------------------
// ��Դ����
//----------------------------------------------------------------------------------------------
VOID LootMgr::ResourceLoot(Creature* pCreature, Role* pPicker)
{
	if( !P_VALID(pCreature) || !P_VALID(pPicker) )
		return;

	// �����ڰ������������ڵ��棬��һ���䣬��С�ӵ���
	NormalLoot(pCreature, pPicker, ELT_BagFirst, TRUE, FALSE);
}

//----------------------------------------------------------------------------------------------
// ����������
//----------------------------------------------------------------------------------------------
VOID LootMgr::InvestigateLoot(Creature* pCreature, Role* pPicker)
{
	if( !P_VALID(pCreature) || !P_VALID(pPicker) )
		return;

	// ��С���������
	QuestLoot(pCreature, pPicker, FALSE);

	// ǿ�����ڱ��������ɾ�����ǵ�һ���䣬��С�ӵ���
	NormalLoot(pCreature, pPicker, ELT_Ground, FALSE, TRUE);
	//NormalLoot(pCreature, pPicker, ELT_ForceBag, FALSE, FALSE);
}


//-------------------------------------------------------------------------------------------------------
// ��ͨ����
//-------------------------------------------------------------------------------------------------------
VOID LootMgr::NormalLoot(Creature* pCreature, Role* pRole, ELootTo eLootTo, BOOL bSingelLoot, BOOL bTeam)
{
	if( !P_VALID(pCreature) ) return;
	if( !P_VALID(pRole) ) return;

	// �õ��������
	const tagCreatureLoot* pLootProto = GetCreatureLoot(pCreature->GetLootID());
	if( !P_VALID(pLootProto) ) return;

	// ����λ������
	INT	nPosIndex =	0;

	// �õ���Ʒ����ļӳ���
	FLOAT fLootChanceAdd = GetCreatureLootChanceAdd(pCreature, pRole,ERA_Money_Add_Rate);

	// ��Ǯ����
	DropMoneyTo(pLootProto, pCreature, eLootTo, pRole, fLootChanceAdd, nPosIndex);

	fLootChanceAdd = GetCreatureLootChanceAdd(pCreature, pRole,ERA_Loot_Add_Rate);
	
	// ��Ʒ����
	for(INT nLootIndex = 0; nLootIndex < MAX_CREATURE_LOOT_NUM; ++nLootIndex)
	{
		const tagLoot* pLoot = &(pLootProto->Loot[nLootIndex]);
		if( !P_VALID(pLoot->dwItemID) ) break;

		DWORD bRtv = FALSE;

		// �������Ʒ����
		if( ELT_Item == pLoot->eLootType )
		{
			bRtv = DropItemTo(pLoot, pCreature, eLootTo, pRole, bTeam, fLootChanceAdd, nPosIndex);
		}

		// ����Ǽ��ϵ���
		else if( ELT_Set == pLoot->eLootType )
		{
			bRtv = DropSetTo(pLoot, pCreature, eLootTo, pRole, bTeam, fLootChanceAdd, nPosIndex);
		}
		else
		{
			ASSERT(0);
			bRtv = FALSE;
		}

		// �����ֻ����һ�Σ���ֱ�ӷ���
		if( bRtv && bSingelLoot )
			break;
	}
}

//-------------------------------------------------------------------------------------------------------
// �������
//-------------------------------------------------------------------------------------------------------
VOID LootMgr::QuestLoot(Creature* pCreature, Role* pRole, BOOL bTeam)
{
	if( !P_VALID(pCreature) ) return;
	if( !P_VALID(pRole) ) return;

	const Team* pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());

	// �õ��������
	const tagLootQuestItem* pQuestLoot	= GetLootQuestItem(pCreature->GetTypeID());
	if( !P_VALID(pQuestLoot) ) return;

	for(INT i = 0; i < MAX_LOOT_QUEST_ITEM_NUM; ++i)
	{
		DWORD dwItemID = pQuestLoot->QuestItem[i].dwQuestItemID;

		// �����С�ӣ�ȡС�Ӽ��ʣ�����ȡ��Ҹ��˼���
		FLOAT fChance = 0.0f;
		if( P_VALID(pTeam) )
		{
			fChance = pQuestLoot->QuestItem[i].fTeamChance;
		}
		else
		{
			fChance = pQuestLoot->QuestItem[i].fChance;
		}

		// �����ƷID�Ƿ�Ϊ��
		if( !P_VALID(dwItemID) ) break;

		// ���㼸��
		if( IUTIL->Rand() % 100000 > INT(fChance * 100000.0f) )
			continue;

		// �ҵ���Ʒ
		tagItemProto* pProto = NULL;
		if( MIsEquipment(dwItemID) )
		{
			pProto = g_attRes.GetEquipProto(dwItemID);
		}
		else
		{
			pProto = g_attRes.GetItemProto(dwItemID);
		}

		if( !P_VALID(pProto) ) continue;

		// ��������ʰȡ
		if(bTeam && P_VALID(pTeam))
		{
			INT		nTeamNum	=	pTeam->GetMemNum();
			Role*	pPicker		=	NULL;

			for(INT i = 0; i < nTeamNum; ++i)
			{
				pPicker = pTeam->GetMemPtr(i);

				if( !P_VALID(pPicker) || !pCreature->IsLootShareDistance(pPicker) )
					continue;

				DropQuestItem(pPicker, pProto, pCreature->GetID());
			}
		}
		// ��������ʰȡ
		else
		{
			DropQuestItem(pRole, pProto, pCreature->GetID());
		}				
	}
}

//-------------------------------------------------------------------------------------------------------
// ȡ����Ʒ����λ��
//-------------------------------------------------------------------------------------------------------
INT LootMgr::GetPutDownPos(Creature* pCreature, Vector3 &vPos, INT &nIndex)
{
	ASSERT(P_VALID(pCreature));

	MTRANS_ELSE_RET(pMap, pCreature->GetMap(), Map, 0);

	for( ; nIndex < MAX_LOOT_POS; ++nIndex)
		if (pMap->CanPutDown(pCreature, nIndex, vPos))
			return TRUE;

	return FALSE;
}

//-------------------------------------------------------------------------------------------------------
// �õ�������ʼӳ�
//-------------------------------------------------------------------------------------------------------
FLOAT LootMgr::GetCreatureLootChanceAdd(Creature* pCreature, Role* pReward,INT addType)
{
	if( !P_VALID(pCreature) || !P_VALID(pReward) ) return 1.0f;

	if( ECTT_All == pCreature->GetTaggedType() ) return 1.0f;

	// �������Դ
	if( pCreature->IsRes() )
	{
		return GetResLootChanceAdd(pCreature, pReward);
	}
	else if( pCreature->IsInves() )
	{
		return GetInvesLootChanceAdd(pReward);
	}
	else
	{
		return GetMosterLootChanceAddEx(pCreature,pReward,addType);//GetMosterLootChanceAdd(pReward);
	}
}

//-------------------------------------------------------------------------------------------------------
// ������ʼӳ�
//-------------------------------------------------------------------------------------------------------
FLOAT LootMgr::GetMosterLootChanceAdd(Role* pReward)
{
	if( !P_VALID(pReward) ) return 1.0f;

	FLOAT	fGMLootRate		=	pReward->GetMap()->GetLootRate();				// ȫ��˫����0-5 ����
	INT		nFatigueRate	=	pReward->GetEarnRate();							// �����ԣ� 0-10000ӳ��Ϊ0-1�ķ��� ����
	INT		nAddOn			=	pReward->GetAttValue(ERA_Loot_Add_Rate);		// ������ʼӳ�
	FLOAT	fVNBLootAdd		=	pReward->GetVNBLootRate();

	return fGMLootRate * (FLOAT(nFatigueRate) / 10000.0f) * (1.0f + FLOAT(nAddOn) / 10000.0f + fVNBLootAdd);
}
FLOAT LootMgr::GetMosterLootChanceAddEx(Creature* pCreature,Role* pReward,INT addType)
{
	if( !P_VALID(pReward) || !P_VALID(pCreature) ) return 1.0f;

	FLOAT decreaseFactor = 1.0;

	const tagCreatureProto *pProto = pCreature->GetProto();
	if( !P_VALID(pProto) ) return 1.0f;

	if( ECILAT_None != pProto->eItemLootAttenuType && ECTT_All != pCreature->GetTaggedType() )
	{
		INT nRoleLevel = pReward->GetLevel  ();
		DWORD dwTeamID = pReward->GetTeamID  ();
		if( GT_VALID( dwTeamID ) )
		{
			const Team * pTeam = g_groupMgr.GetTeamPtr  (dwTeamID);
			if( P_VALID(pTeam) )
			{
				int count = pTeam->GetMemNum();
				Role * p = NULL;
				for(int i = 0; i < count ; ++i)
				{
					p = pTeam->GetMemPtr  (i);
					if( P_VALID(p) && p != pReward )
						nRoleLevel += p->GetLevel  ();
				}
				if( count > 0 )
					nRoleLevel /= count;
			}
		}
		INT nCreatureLevel = pCreature->GetLevel  ();
		INT delta = nRoleLevel - nCreatureLevel;

		if( delta <= 9 )
			decreaseFactor = 1.0;
		else if( delta <= 14 )
			decreaseFactor = 0.8;
		else if( delta <= 19 )
			decreaseFactor = 0.5;
		else if( delta <= 24 )
			decreaseFactor = 0.2;
		else if( delta <= 29 )
			decreaseFactor = 0.1;
		else
			decreaseFactor = 0;
	}

	FLOAT	fGMLootRate		=	pReward->GetMap()->GetLootRate();				// ȫ��˫����0-5 ����
	INT		nFatigueRate	=	pReward->GetEarnRate();							// �����ԣ� 0-10000ӳ��Ϊ0-1�ķ��� ����
	INT		nAddOn			=	pReward->GetAttValue(addType);		// ������ʼӳ�
	FLOAT	fVNBLootAdd		=	pReward->GetVNBLootRate();

	return fGMLootRate * (FLOAT(nFatigueRate) / 10000.0f) * (1.0f + FLOAT(nAddOn) / 10000.0f + fVNBLootAdd) * decreaseFactor;
}

//-------------------------------------------------------------------------------------------------------
// ��Դ���ʼӳ�
//-------------------------------------------------------------------------------------------------------
FLOAT LootMgr::GetResLootChanceAdd(Creature* pCreature, Role* pReward)
{
	if( !P_VALID(pCreature) || !P_VALID(pReward) ) return 1.0f;

	INT		nFatigueRate	=	pReward->GetEarnRate();							// �����ԣ� 0-10000ӳ��Ϊ0-1�ķ��� ����
	INT		nAddOn			=	pReward->CalGatherRate(pCreature);				// �ɼ��ӳ�

	return (FLOAT(nFatigueRate) / 10000.0f) * (1.0f + FLOAT(nAddOn) / 10000.0f);
}

//-------------------------------------------------------------------------------------------------------
// �ɵ��������ʼӳ�
//-------------------------------------------------------------------------------------------------------
FLOAT LootMgr::GetInvesLootChanceAdd(Role* pReward)
{
	return 1.0f;
}

//----------------------------------------------------------------------------------------
// �õ�ĳ�����������
//----------------------------------------------------------------------------------------
VOID LootMgr::GetLootOwner(Creature* pCreature, Role* pPicker, BOOL bTeam, DWORD& dwOwnerID, Role* &pOwnerRole, DWORD& dwTeamID)
{
	// ȫ������
	if( ECTT_All == pCreature->GetTaggedType() )
	{
		dwTeamID	=	GT_INVALID;
		dwOwnerID	=	GT_INVALID;
		pOwnerRole	=	NULL;
	}
	// ��ȫ������
	else
	{
		if( bTeam )
		{
			dwTeamID = pPicker->GetTeamID();

			if( P_VALID(dwTeamID) )
			{
				const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
				if( P_VALID(pTeam) )
				{
					pOwnerRole = pTeam->GetTeamPickRole(pCreature);
				}
				else
				{
					pOwnerRole = NULL;
				}

				if( P_VALID(pOwnerRole) )
				{
					dwOwnerID = pOwnerRole->GetID();
				}
				else
				{
					dwOwnerID = GT_INVALID;
				}
			}
			else
			{
				pOwnerRole = pPicker;
				dwOwnerID = pPicker->GetID();
			}
		}
		else
		{
			dwTeamID	=	GT_INVALID;
			dwOwnerID	=	pPicker->GetID();
			pOwnerRole	=	pPicker;
		}
	}
}

//-----------------------------------------------------------------------------------------
// ���ֵ��䷽ʽ
//-----------------------------------------------------------------------------------------
BOOL LootMgr::LootItem(Creature* pCreature, tagItem* &pItem, ELootTo eLootTo, Role* pOwnerRole, DWORD dwOwnerID, DWORD dwTeamID, INT& nIndex)
{
	if( !P_VALID(pCreature) || !P_VALID(pItem) ) return FALSE;

	// �õ���ͼ
	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return FALSE;

	// ���ݵ��䷽ʽ
	switch(eLootTo)
	{
		// ǿ�Ƶ��䱳��
	case ELT_ForceBag:
		{
			if( !P_VALID(pOwnerRole) || E_Success != pOwnerRole->GetItemMgr().Add2Bag(pItem, ELCID_Loot, TRUE) )
			{
				::Destroy(pItem);
				return FALSE;
			}
			// Jason 2009-11-28 �ж���Ʒ�ȼ������㲥��ս��Ƶ����
			//BOOL canbroadcast = FALSE;
			//if( MIsEquipment(pItem->dwTypeID) )
			//{
			//	if( pItem->pEquipProto->byQuality != EIQ_White )
			//		canbroadcast = TRUE;
			//}
			//else if( pItem->pProtoType->byQuality != EIQ_White )
			//{
			//	canbroadcast = TRUE;
			//}
			//if(canbroadcast)
				Team::BroadcastLootItemMsgInTeamExceptPicker(pOwnerRole,pItem);
			return TRUE;
		}
		break;

		// �ȱ���
	case ELT_BagFirst:
		{
			if( P_VALID(pOwnerRole) )
			{
				if( E_Success == pOwnerRole->GetItemMgr().Add2Bag(pItem, ELCID_Loot, TRUE) )
				{
					// Jason 2009-11-28 �ж���Ʒ�ȼ������㲥��ս��Ƶ����
					//BOOL canbroadcast = FALSE;
					//if( MIsEquipment(pItem->dwTypeID) )
					//{
					//	if( pItem->pEquipProto->byQuality != EIQ_White )
					//		canbroadcast = TRUE;
					//}
					//else if( pItem->pProtoType->byQuality != EIQ_White )
					//{
					//	canbroadcast = TRUE;
					//}
					//if(canbroadcast)
						Team::BroadcastLootItemMsgInTeamExceptPicker(pOwnerRole,pItem);

					return TRUE;
				}
				else
				{
					Vector3 vPos;
					if( !GetPutDownPos(pCreature, vPos, nIndex) )
					{
						::Destroy(pItem);
						return FALSE;
					}
					pMap->PutDownItem(pCreature, pItem, dwOwnerID, dwTeamID, vPos);

					return TRUE;
				}
			}
			else
			{
				Vector3 vPos;
				if( !GetPutDownPos(pCreature, vPos, nIndex) )
				{
					::Destroy(pItem);
					return FALSE;
				}
				pMap->PutDownItem(pCreature, pItem, dwOwnerID, dwTeamID, vPos);

				return TRUE;
			}
		}
		break;

		// �������
	case ELT_Ground:
		{
			Vector3 vPos;
			if( !GetPutDownPos(pCreature, vPos, nIndex) )
			{
				::Destroy(pItem);
				return FALSE;
			}
			pMap->PutDownItem(pCreature, pItem, dwOwnerID, dwTeamID, vPos);
			return TRUE;
		}
		break;

		// Ĭ��
	default:
		{
			ASSERT(0);
			return FALSE;
		}
		break;
	}
}

//-------------------------------------------------------------------------------------------------------
// ��Ǯ����
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::DropMoneyTo( const tagCreatureLoot* pLootProto, Creature* pCreature, ELootTo eLootTo, Role* pPicker, FLOAT fChanceAdd, INT &nIndex )
{
	if( !P_VALID(pLootProto) ) return FALSE;
	if( !P_VALID(pCreature) ) return FALSE;

	// �õ��������ڵ�ͼ
	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return FALSE;

	// �õ�ʵ�ʵ����Ǯ��
	INT nMoney = INT(IUTIL->RandomInRange(pLootProto->nMinMoney, pLootProto->nMaxMoney) * fChanceAdd);
	if( nMoney <= 0 ) return FALSE;

	// �õ�����λ��
	Vector3 vPos(0.0f, 0.0f, 0.0f);
	BOOL bHasPositon = GetPutDownPos(pCreature, vPos, nIndex);
	if( !bHasPositon ) return FALSE;

	pMap->PutDownMoney(pCreature, nMoney, pPicker, vPos);
	++nIndex;
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���ϵ���
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::DropSetTo(const tagLoot* pLoot, Creature* pCreature, ELootTo eLootTo, Role* pPicker, BOOL bTeam, FLOAT fChanceAdd, INT &nIndex)
{
	if( !P_VALID(pLoot) ) return FALSE;
	if( !P_VALID(pCreature) ) return FALSE;
	if( !P_VALID(pPicker) ) return FALSE;

	if( ELT_Set != pLoot->eLootType ) return FALSE;

	// �õ������ͼ
	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return FALSE;

	// ����Ѿ������ˣ��򲻵�����
	if( nIndex >= MAX_LOOT_POS ) return FALSE;

	// �õ����伯��
	const tagLootItemSet* pItemSet = GetLootItemSet(pLoot->dwItemID);
	if( !P_VALID(pItemSet) ) return FALSE;

	// �ж�һ�µ���
	FLOAT fRealChance = pLoot->fChance * fChanceAdd;
	if( IUTIL->Rand() % 100000 > INT(fRealChance * 100000.0f) )
		return FALSE;

	// ���伸��
	INT nDropNum = IUTIL->RandomInRange(pLoot->nMin, pLoot->nMax);

	// ��������Ч��Ʒ
	TList<INT> listValid;
	for(INT i = 0; i < MAX_ITEM_SET_NUM; ++i)
	{
		if( 0 == pItemSet->ItemSet[i].dwItemID || pItemSet->ItemSet[i].nItemNum <= 0)
			continue;

		listValid.PushBack(i);
	}
	if( listValid.Size() <= 0 )
		return FALSE;

	// ����ÿһ��
	for(INT i = 0; i < nDropNum && nIndex < MAX_LOOT_POS; ++i)
	{
		INT nSetIndex = GT_INVALID;	// �������
		listValid.RandPeek(nSetIndex);

		// �õ���Ʒ����
		tagItemProto* pProto = NULL;
		if( MIsEquipment(pItemSet->ItemSet[nSetIndex].dwItemID) )
		{
			pProto = g_attRes.GetEquipProto(pItemSet->ItemSet[nSetIndex].dwItemID);
		}
		else
		{
			pProto = g_attRes.GetItemProto(pItemSet->ItemSet[nSetIndex].dwItemID);
		}
		if( !P_VALID(pProto) ) continue;

		// ������Ʒ
		tagItem* pItem = ItemCreator::Create(EICM_Loot, pCreature->GetID(), pProto->dwTypeID, pItemSet->ItemSet[nSetIndex].nItemNum);
		if( !P_VALID(pItem) ) continue;

		// �ҵ����ε��������
		DWORD dwTeamID	= GT_INVALID;
		Role* pOwnerRole = NULL;
		DWORD dwOwnerID = GT_INVALID;
		GetLootOwner(pCreature, pPicker, bTeam, dwOwnerID, pOwnerRole, dwTeamID);

		// ������Ʒ
		if( LootItem(pCreature, pItem, eLootTo, pOwnerRole, dwOwnerID, dwTeamID, nIndex) )
		{
			++nIndex;
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��Ʒ����
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::DropItemTo(const tagLoot* pLoot, Creature* pCreature, ELootTo eLootTo, Role* pPicker, BOOL bTeam, FLOAT fChanceAdd, INT &nIndex)
{
	if( !P_VALID(pLoot) || !P_VALID(pCreature) || !P_VALID(pPicker) )
		return FALSE;

	if( ELT_Item != pLoot->eLootType ) return FALSE;

	// �õ������ͼ
	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return FALSE;

	// ����Ѿ������ˣ��򲻵�����
	if( nIndex >= MAX_LOOT_POS ) return FALSE;

	// �����ж�һ�µ��ʣ����Ƿ�����
	FLOAT fRealChance = pLoot->fChance * fChanceAdd;
	if( IUTIL->Rand() % 100000 > INT(fRealChance * 100000.0f) )
		return FALSE;

	// �õ���Ʒ����
	tagItemProto* pProto = NULL;
	if( MIsEquipment(pLoot->dwItemID) )
	{
		pProto = g_attRes.GetEquipProto(pLoot->dwItemID);
	}
	else
	{
		pProto = g_attRes.GetItemProto(pLoot->dwItemID);
	}
	if( !P_VALID(pProto) ) return FALSE;
	
	// �õ���Ʒ��Ŀ
	INT16 n16RandNum = IUTIL->RandomInRange(pLoot->nMin, pLoot->nMax);
	if( n16RandNum <= 0 ) return FALSE;

	// ������Ʒ
	tagItem* pItem = ItemCreator::Create(EICM_Loot, pCreature->GetID(), pProto->dwTypeID, n16RandNum);
	if( !P_VALID(pItem) ) return FALSE;

	// �ҵ����ε��������
	DWORD dwTeamID	= GT_INVALID;
	Role* pOwnerRole = NULL;
	DWORD dwOwnerID = GT_INVALID;

	GetLootOwner(pCreature, pPicker, bTeam, dwOwnerID, pOwnerRole, dwTeamID);

	// ������Ʒ
	BOOL bRet = LootItem(pCreature, pItem, eLootTo, pOwnerRole, dwOwnerID, dwTeamID, nIndex);

	if( bRet ) ++nIndex;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ����
//-------------------------------------------------------------------------------------------------------
BOOL LootMgr::DropQuestItem(Role* pPicker, const tagItemProto* pItemProto, DWORD dwCreatureID)
{
	if( !P_VALID(pPicker) ) return FALSE;
	if( !P_VALID(pItemProto) ) return FALSE;

	// �ҵ�����Ʒ��Ӧ������
	UINT16	u16QuestID = (UINT16)pItemProto->dwQuestID;

	// ��ҵ�ǰ�����Ƿ��и�����
	Quest* pQuest = pPicker->GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return FALSE;

	// ���������������Ʒ�Ƿ��Ѿ�����
	if( pQuest->IsQuestItemFull(pItemProto->dwTypeID) ) return FALSE;

	// ����������Ʒ
	tagItem* pItem = ItemCreator::Create(EICM_Loot, dwCreatureID, pItemProto->dwTypeID, 1);
	if( !P_VALID(pItem) ) return FALSE;

	// �����񱳰��������
	if( pPicker->GetItemMgr().Add2QuestBag(pItem, ELCID_Loot) )
	{
		::Destroy(pItem);
		return FALSE;
	}

	return TRUE;
}