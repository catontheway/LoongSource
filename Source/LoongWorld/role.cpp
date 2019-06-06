//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݽṹ
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "db_session.h"

#include "../WorldDefine/chat.h"
#include "../WorldDefine/chat_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/quest.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/pk_define.h"
#include "../WorldDefine/msg_talent.h"
#include "../WorldDefine/msg_pk.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/msg_stall.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/msg_social.h"
#include "../WorldDefine/msg_prison.h"
#include "../WorldDefine/msg_map.h"
#include "../WorldDefine/exchange_define.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/msg_script.h"
#include "../WorldDefine/msg_loot.h"
#include "../WorldDefine/msg_motion.h"

#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_quest.h"
#include "../ServerDefine/msg_skill.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/login_issue.h"
#include "../WorldDefine/motion_define.h"
#include "pet_define.h"
#include "db_session.h"
#include "login_session.h"
#include "map.h"
#include "map_instance.h"
#include "world.h"
#include "map_creator.h"
#include "map_mgr.h"
#include "att_res.h"
#include "skill.h"
#include "buff.h"
#include "quest.h"
#include "quest_mgr.h"
#include "role.h"
#include "creature.h"
#include "creature_ai.h"
#include "item_creator.h"
#include "role_mgr.h"
#include "stall.h"
#include "group_mgr.h"
#include "social_mgr.h"
#include "script_mgr.h"
#include "title_mgr.h"
#include "pet_pocket.h"
#include "guild_mgr.h"
#include "pet_soul.h"
#include "guild_commodity.h"
#include "guild.h"
//#include "container_restrict.h"

// Jason 2009-12-7 �����͸�
#include "../WorldDefine/msg_player_preventlost.h"



Role* Role::Create( DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession )
{
	return new Role(dwRoleID, pData, pSession);
}

VOID Role::Delete( Role* &pRole )
{
	SAFE_DEL(pRole);
}

Role::SaveRole::SaveRole()
{
	BYTE *byData = new BYTE[X_ROLE_BUFF_SIZE];
	ZeroMemory(byData, X_ROLE_BUFF_SIZE);
	MMSGINIT(byData, tagNDBC_SaveRole);
	m_pSaveRole = (tagNDBC_SaveRole*)(byData) ;
	m_pSaveRole->dwSize = X_ROLE_BUFF_SIZE;
}

Role::SaveRole::~SaveRole()
{
	delete [](BYTE*)m_pSaveRole;
}

VOID Role::SaveRole::Init()
{
	m_pSaveRole->dwRoleID = GT_INVALID;
	ZeroMemory(&m_pSaveRole->RoleData, X_ROLE_BUFF_SIZE - FIELD_OFFSET(tagNDBC_SaveRole, RoleData));
}

Role::SaveRole	Role::m_SaveRole;
Mutex			Role::m_SaveRoleLock;

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
Role::Role(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession)
		: Unit(dwRoleID, pData->RoleDataSave.dwMapID, 
				pData->RoleDataSave.fCoordinate[0], pData->RoleDataSave.fCoordinate[1], pData->RoleDataSave.fCoordinate[2],
				pData->RoleDataSave.fFace[0], pData->RoleDataSave.fFace[1], pData->RoleDataSave.fFace[2]),
		  m_ePKState(ERPKS_Peace), m_pStall(new Stall(this, &(pData->RoleDataSave))),
		  m_ItemMgr(this, pSession->GetSessionID(), dwRoleID, pData->RoleDataSave.n16BagSize, pSession->GetWareSize()),
		  m_CurMgr(this, pData->RoleDataSave.nBagGold, pData->RoleDataSave.nBagSilver, pData->RoleDataSave.nBagYuanBao,
					pSession->GetWareSilver(), pSession->GetBaiBaoYB(), pData->RoleDataSave.nExVolume),
		  m_pSession(pSession), m_LongHun(this), m_nNeedSave2DBCountDown(MIN_ROLE_SAVE2DB_INTERVAL),
		  m_Suit(this),m_dwTeamID(GT_INVALID),m_dwGroupID(GT_INVALID),m_dwTeamInvite(GT_INVALID), m_dwOwnInstanceID(GT_INVALID), m_dwOwnInstanceMapID(GT_INVALID),
		  m_dwExportMapID(GT_INVALID), m_pScript(NULL), m_pPetPocket(NULL),	m_nMotionCounter(GT_INVALID), m_dwPartnerID(GT_INVALID),
		  ScriptData<ESD_Role>(),m_nWorldTalkCounter(GT_INVALID),m_bObjectCoolOff(FALSE), m_bSpeakOff(FALSE), m_bLevelUpClosedSafeGuard(FALSE),
		  m_dwItemTransportMapID(pData->RoleDataSave.dwItemTransportMapID),m_fItemTransportX(pData->RoleDataSave.fItemTransportX),m_fItemTransportZ(pData->RoleDataSave.fItemTransportZ),
		  m_fItemTransportY(pData->RoleDataSave.fItemTransportY),m_dwLoverID(pData->RoleDataSave.dwLoverID),m_bHaveWedding(pData->RoleDataSave.bHaveWedding),
		  m_timeLastLessing(pData->RoleDataSave.timeLastLessing),m_nLessingLevel(pData->RoleDataSave.nLessingLevel),m_bFiredOnceOfLessingLoong(FALSE)
{
	Init(pData);
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
Role::~Role()
{
	// �����������
	tagQuestDoneSave* pDoneQuest = NULL;
	QuestDoneMap::TMapIterator it = m_mapCompleteQuests.Begin();
	while( m_mapCompleteQuests.PeekNext(it, pDoneQuest) )
	{
		SAFE_DEL(pDoneQuest);
	}
	m_mapCompleteQuests.Clear();

	SAFE_DEL(m_pStall);
	m_pScript = NULL;

	SAFE_DEL(m_pTitleMgr);

	SAFE_DEL(m_pPetPocket);

};

//--------------------------------------------------------------------------------
// ��ҵĸ��º���
//--------------------------------------------------------------------------------
VOID Role::Update()
{
	GetMoveData().Update();
	GetCombatHandler().Update();
	UpdateSkill();
	UpdateBuff();
	UpdatePK();
	UpdatePVP();
	UpdateHostilitySafeGuard();
	UpdateTimedIssue();
	GetItemMgr().Update();
	m_pStall->Update();
	m_CurMgr.Update();
	GetPetPocket()->Update();

	--m_nNeedSave2DBCountDown;
	UpdateTalkToWorld();
	UpdateMotionInviteState();

	// Jason �����͸� 2009-12-7 
	// Jason ȡ��������ʱ��������ģʽ��
	// UpdateBlessingOfLoong();
}

//-------------------------------------------------------------------------------------------
// ����Ҽ��뵽��Ϸ������
//-------------------------------------------------------------------------------------------
BOOL Role::AddToWorld(BOOL bFirst)
{
	BOOL bRet = g_mapCreator.TakeOverRoleWhenOnline(this);

	if( bRet && P_VALID(m_pScript) )
	{
		if( bFirst )		// ��һ�ν�����Ϸ����
		{
			m_pScript->OnRoleFirstIntoWorld(this);
		}
		m_pScript->OnRoleIntoWorld(this);	// ������Ϸ����
	}

	return bRet;
}

//-----------------------------------------------------------------------
// ����Ƿ��ڶ�Ӧְ��NPC����
//-----------------------------------------------------------------------
DWORD Role::CheckFuncNPC(DWORD dwNPCID, EFunctionNPCType eNPCType,
						 OUT Creature **ppNPC/* = NULL*/, OUT Map **ppMap/* = NULL*/)
{
	// ���map
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC))
	{
		return E_NPC_NotFound;
	}

	if(!pNPC->IsFunctionNPC(eNPCType))
	{
		return E_NPC_NotValid;
	}

	if(!pNPC->CheckNPCTalkDistance(this))
	{
		return E_NPC_TooFar;
	}

	if(ppNPC != NULL)
	{
		*ppNPC = pNPC;
	}

	if(ppMap != NULL)
	{
		*ppMap = pMap;
	}

	return E_Success;
}

//----------------------------------------------------------------------------------------
// ��ʼ����Ʒװ�� -- �����ͻ���
//----------------------------------------------------------------------------------------
VOID Role::SendInitStateItem()
{
	GetItemMgr().SendInitStateItem();
	GetItemMgr().SendInitStateItemCDTime();
}

//----------------------------------------------------------------------------------------
// ��ʼ��װ
//----------------------------------------------------------------------------------------
VOID Role::SendInitStateSuit()
{
	INT nSuitNum = m_Suit.GetSuitNum();
	if(0 == nSuitNum)
	{
		return;
	}

	INT nSzMsg = sizeof(tagNS_GetRoleInitState_Suit) - 1 + sizeof(tagSuitInit) * nSuitNum;

	MCREATE_MSG(pSend, nSzMsg, NS_GetRoleInitState_Suit);
	if(!P_VALID(pSend))
	{
		ASSERT(P_VALID(pSend));
		return;
	}

	m_Suit.InitSendInitState(pSend->byData);
	pSend->nSuitNum = nSuitNum;
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//---------------------------------------------------------------------------
// ��ʼ��������
//---------------------------------------------------------------------------
VOID Role::SendInitStateLongHun()
{
	INT8 n8Num = m_LongHun.GetActiveNum() + m_LongHun.GetPassiveNum();
	if(n8Num <=0)
	{
		return;
	}

	INT nSzMsg = sizeof(tagNS_GetRoleInitState_Longhun) + (n8Num - 1) * sizeof(INT16);
	
	MCREATE_MSG(pSend, nSzMsg, NS_GetRoleInitState_Longhun);
	if(!P_VALID(pSend))
	{
		ASSERT(P_VALID(pSend));
		return;
	}

	m_LongHun.InitSendInitState(pSend->n16EquipPos, n8Num);
	pSend->n8Num = n8Num;
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//---------------------------------------------------------------------------
// ���������ʼ�������Ը��ͻ���
//---------------------------------------------------------------------------
VOID Role::SendInitStateAtt()
{
	INT nBuffNum = GetBuffNum();
	DWORD dwSize = sizeof(tagNS_GetRoleInitState_Att) + ((nBuffNum > 0) ? (nBuffNum - 1)*sizeof(tagBuffMsgInfo) : 0);

	MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_Att);

	pSend->Avatar = m_Avatar;
	pSend->AvatarEquip = GetAvatarEquip();
	pSend->DisplaySet = GetDisplaySet();
	pSend->nLevel = m_nLevel;
	pSend->nCurLevelExp = m_nCurLevelExp;
	pSend->nCredit = m_nCredit;
	pSend->nIdentity = m_nIdentity;
	pSend->nVIPPoint = m_nVIPPoint;
	pSend->dwState = GetState();
	pSend->dwRoleState = m_RoleState.GetState();
	pSend->dwRoleStateEx = m_RoleStateEx.GetState();
	pSend->ePKState = m_ePKState;
	pSend->dwRebornMapID = m_dwRebornMapID;
	pSend->vRebornPoint = g_mapCreator.GetReBornPoint(m_dwRebornMapID);
	pSend->dwTransportID = m_dwItemTransportMapID;
	pSend->vTransportPoint.x = m_fItemTransportX;
	pSend->vTransportPoint.z = m_fItemTransportZ;
	pSend->dwGuildID = GetGuildID();
	pSend->u16ActTitleID = GetTitleMgr()->GetActiviteTitle();
	pSend->dwLoverID = m_dwLoverID;
	pSend->bHaveWedding = m_bHaveWedding;
	pSend->bHasLeftMsg = m_bHasLeftMsg;
	pSend->eClassType = m_eClass;
	pSend->eClassTypeEx = m_eClassEx;
	pSend->byStallLevel = this->GetStallModeLevel();
	IFASTCODE->MemCpy(pSend->nAtt, m_nAtt, sizeof(m_nAtt));
	IFASTCODE->MemCpy(pSend->nAttPointAdd, m_nAttPointAdd, sizeof(m_nAttPointAdd));

	pSend->nBuffNum = nBuffNum;

	if( nBuffNum > 0 )
	{
		GetAllBuffMsgInfo(pSend->Buff, nBuffNum);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//----------------------------------------------------------------------------
// ���������ʼ���ܸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateSkill()
{
	INT nNum = m_mapSkill.Size();

	TMap<DWORD, Skill*>::TMapIterator it = m_mapSkill.Begin();
	Skill* pSkill = NULL;

	if( nNum <= 0 )
	{
		tagNS_GetRoleInitState_Skill send;
		IFASTCODE->MemCpy(send.Talent, m_Talent, sizeof(m_Talent));
		send.nNum = 0;

		SendMessage(&send, send.dwSize);
	}

	else
	{
		DWORD dwSize = sizeof(tagNS_GetRoleInitState_Skill) + sizeof(tagSkillMsgInfo) * (nNum-1);
		MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_Skill);

		pSend->dwSize = dwSize;
		IFASTCODE->MemCpy(pSend->Talent, m_Talent, sizeof(m_Talent));
		pSend->nNum = nNum;

		INT nIndex = 0;
		while( m_mapSkill.PeekNext(it, pSkill) )
		{
			pSkill->GenerateMsgInfo(&pSend->Skill[nIndex]);
			nIndex++;
		}

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}


}

//----------------------------------------------------------------------------
// ����������������б���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateCompleteQuest()
{
	// �õ��������ĸ���
	INT nCompleteQuestNum = GetCompleteQuestCount();
	
	DWORD dwSize = sizeof(tagNS_GetRoleInitState_CompleteQuest);
	if( nCompleteQuestNum > 0 )
	{
		dwSize += (nCompleteQuestNum-1) * sizeof(tagCompleteQuestMsgInfo);
	}
	
	MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_CompleteQuest);

	pSend->nNum = nCompleteQuestNum;

	INT nIndex = 0;
	tagQuestDoneSave* pDoneQuest = NULL;
	QuestDoneMap::TMapIterator it = m_mapCompleteQuests.Begin();

	while( m_mapCompleteQuests.PeekNext(it, pDoneQuest) )
	{
		pSend->completeQuest[nIndex].u16QuestID = pDoneQuest->u16QuestID;
		pSend->completeQuest[nIndex].dwStartTime = pDoneQuest->dwStartTime;
		pSend->completeQuest[nIndex].nTimes = pDoneQuest->nTimes;
		++nIndex;

		if( nIndex >= nCompleteQuestNum ) break;
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

}

//----------------------------------------------------------------------------
// �������ﵱǰ�����б���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateIncompleteQuest()
{
	//�õ���ǰ����ĸ���
	INT nCurrentQuestNum = GetCurrentQuestCount();

	DWORD dwSize = sizeof(tagNS_GetRoleInitState_IncompleteQuest);
	if( nCurrentQuestNum > 0 )
	{
		dwSize += (nCurrentQuestNum-1) * sizeof(tagIncompleteQuestMsgInfo);
	}

	MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_IncompleteQuest);

	pSend->nNum = nCurrentQuestNum;

	INT		nIndex = 0;
	BYTE*	pTemp = (BYTE*)pSend->incompleteQuest;	

	for(INT n = 0; n < QUEST_MAX_COUNT; ++n)
	{
		if( FALSE == m_Quests[n].IsValid() ) continue;

		DWORD dwQuestSize = 0;

		m_Quests[n].GenerateMsgInfo((tagIncompleteQuestMsgInfo*)pTemp, dwSize, dwQuestSize);
		++nIndex;
		pTemp += dwQuestSize;
		

		if( nIndex >= nCurrentQuestNum ) break;
	}
	pSend->dwSize = dwSize;
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//----------------------------------------------------------------------------
//  ���������Ǯ���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateMoney()
{
	tagNS_GetRoleInitState_Money send;

	CurrencyMgr &CurMgr		= GetCurMgr();
	send.nBagYuanBao		= CurMgr.GetBagYuanBao();
	send.n64BagSilver		= CurMgr.GetBagSilver();
	send.n64WareSilver		= CurMgr.GetWareSilver();
	send.nBaiBaoYuanBao		= CurMgr.GetBaiBaoYuanBao();
	send.nExchangeVolume	= CurMgr.GetExchangeVolume();

	for (INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		send.nCurClanCon[nClanType] = CurMgr.GetClanCon(static_cast<ECLanType>(nClanType));
	}

	SendMessage(&send, send.dwSize);
}

//----------------------------------------------------------------------------
//  �������ﵱǰ�������ݸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateReputation()
{
	tagNS_GetRoleInitState_Reputation send;

	ClanData& clanData = GetClanData();

	for (INT32 clantype = ECLT_BEGIN; clantype < ECLT_END; ++clantype)
	{
		send.nReputation[clantype]	= clanData.RepGetVal((ECLanType)clantype);
		send.nActiveCount[clantype]	= clanData.ActCountGetVal((ECLanType)clantype);
		send.bisFame[clantype]		= clanData.IsFame((ECLanType)clantype);
	}

	SendMessage(&send, send.dwSize);
}

//----------------------------------------------------------------------------
//  �����������ڰ����������ݸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateGuild()
{
	tagNS_GetRoleInitState_Guild send;

	g_guildMgr.InitGuildInitState(this, send.sGuildBase, send.sGuildMember);

	SendMessage(&send, send.dwSize);
}

//----------------------------------------------------------------------------
//  ���ͷ��������ݸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendFatigueGuardInfo( BYTE byCode )
{
	tagNS_FatigueLimit send;
	send.byLimitCode = byCode;
	SendMessage(&send, send.dwSize);
}


//----------------------------------------------------------------------------
//  ����������Ѻͺ�����
//----------------------------------------------------------------------------
VOID Role::SendFriendAndEnemy()
{
	//�õ����Ѹ���
	INT nFriendNum = GetFriendCount();
	INT nIndex = 0;
	tagRoleInfo* pRoleInfo = (tagRoleInfo*)GT_INVALID;

	DWORD dwSize = sizeof(tagNS_SendFriendBlackList);
	if( nFriendNum > 0 )
	{
		dwSize += (nFriendNum-1) * sizeof(tagFriendInfo);
	}

	MCREATE_MSG(pSend, dwSize, NS_SendFriendBlackList);
	IFASTCODE->MemCpy(pSend->dwRolesID, m_dwBlackList, sizeof(DWORD) * MAX_BLACKLIST);

	pSend->nNum = nFriendNum;

	for(INT n = 0; n < MAX_FRIENDNUM; ++n)
	{
		if( GT_INVALID == m_Friend[n].dwFriendID ) continue;

		pSend->FriendInfo[nIndex].dwFriendID = m_Friend[n].dwFriendID;
		pSend->FriendInfo[nIndex].dwFriVal = m_Friend[n].dwFriVal;
		pSend->FriendInfo[nIndex].byGroup = m_Friend[n].byGroup;
		
		pRoleInfo = g_roleMgr.GetRoleInfo(m_Friend[n].dwFriendID);
		if(P_VALID(pRoleInfo))
		{
			pSend->FriendInfo[nIndex].bOnline = pRoleInfo->bOnline;
			pSend->FriendInfo[nIndex].nLevel = pRoleInfo->byLevel;
		}

		++nIndex;
	}
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);	
}

//----------------------------------------------------------------------------------------
// ��NPC����ȡ����
//----------------------------------------------------------------------------------------
INT Role::AcceptQuestFromNPC(UINT16 u16QuestID, DWORD dwNPCID)
{
	// ��������Ƿ����
	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if( !P_VALID(pProto) )	return E_QUEST_NOT_EXIST;

	Creature* pNPC = NULL;

	// ������Ǵ�NPC��ȡ
	if( !P_VALID(dwNPCID) )
	{
		if( g_questMgr.IsNeedNPCAcceptQuest(u16QuestID) )
			return E_QUEST_NPC_NOT_EXIST;
	}
	// �Ǵ�NPC��ȡ
	else
	{
		pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) )
		{
			return E_QUEST_NPC_NOT_EXIST;
		}
		else if( FALSE == pNPC->IsInDistance(*this, 500.0f) )
		{
			return E_QUEST_NPC_TOO_FAR;
		}
		else if( FALSE == g_questMgr.IsNPCAcceptQuest(pNPC, u16QuestID) )
		{
			return E_QUEST_NOT_EXIST;
		}
	}

	// �Ƿ���������ȡ����
	INT nIndex = GT_INVALID;
	INT nRet = CanAcceptQuest(u16QuestID, nIndex, pNPC);

	if( E_Success == nRet )
	{
		// �����������������
		AddQuest(pProto, nIndex);
		m_Quests[nIndex].OnAccept(pNPC);
	}

	return nRet;
}

//----------------------------------------------------------------------------------------
// �Ӵ����������һ������
//----------------------------------------------------------------------------------------
INT Role::AcceptQuestFromMapTrigger(UINT16 u16QuestID, DWORD dwMapTriggerID)
{
	// ��������Ƿ����
	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if( !P_VALID(pProto) )	return E_QUEST_NOT_EXIST;

	// �ҵ��Ƿ��ڴ���������
	if( FALSE == GetMap()->IsInTrigger(this, dwMapTriggerID) )
		return E_CanTakeQuest_FAILED_WRONG_TRIGGER;

	// ��ô������������к�
	DWORD dwQuestSerial = GetMap()->GetTriggerQuestSerial(dwMapTriggerID);
	if( !P_VALID(dwQuestSerial) )
		return E_CanTakeQuest_FAILED_WRONG_TRIGGER;

	// �õ������Ƿ���Ҫ���������
	INT n = 0;
	for(; n < QUEST_TRIGGERS_COUNT; ++n)
	{
		if( 0 == pProto->accept_req_trriger[n] ) continue;

		if( dwQuestSerial == pProto->accept_req_trriger[n] )
			break;
	}
	if( n >= QUEST_TRIGGERS_COUNT )
	{
		return E_CanTakeQuest_FAILED_WRONG_TRIGGER;
	}


	// �Ƿ���������ȡ����
	INT nIndex = GT_INVALID;
	INT nRet = CanAcceptQuest(u16QuestID, nIndex);

	if( E_Success == nRet )
	{
		// �����������������
		AddQuest(pProto, nIndex);
		m_Quests[n].OnAccept(NULL);
	}

	return nRet;
}

//----------------------------------------------------------------------------------------
// ĳ�������Ƿ���Խ�ȡ
//----------------------------------------------------------------------------------------
INT Role::CanAcceptQuest(UINT16 u16QuestID, INT& nIndex, Creature* pNPC)
{
// 	// ������
// 	if (GetEarnRate() < 10000)
// 	{
// 		return E_QUEST_FATIGUEGUARD;
// 	}

//	g_questMgr.IsNeedNPCAcceptQuest(u16QuestID);

	// ��������Ƿ����
	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if( !P_VALID(pProto) )	return E_QUEST_NOT_EXIST;
	
	// ���ְҵ�Ƿ����
	if ( E_Success != VocationCheck( pProto->class_type) )
		return E_CanTakeQuest_Class;

	//INT nTmpClass =  1 << ( m_eClass - 1 );
	////INT nTmpClassEx = 1 << ( m_eClassEx + 8 );
	//INT nTmpClassEx = 0;

	//if ( (INT)m_eClassEx != (INT)EHV_Base )
	//{
	//	nTmpClassEx = 1 << ( m_eClassEx + 8 );
	//}

	//DWORD dwTmpLimitStr = nTmpClass + nTmpClassEx;
	//if( !( pProto->class_type & dwTmpLimitStr ) )
	//	return E_CanTakeQuest_Class;

	// ����Ƿ��Ѿ��ӹ�������
	if( IsHaveQuest(u16QuestID) ) return E_CanTakeQuest_ALREADY_TAKE;

	// ����Ƿ�����������
	if( IsHaveDoneQuest(u16QuestID) )
	{
		// ��������ǲ����ظ��ģ��򷵻��Ѿ�����
		if( !pProto->repeatable ) return E_CanTakeQuest_ALREADY_DONE;

		// ����ǿ��ظ��ģ������ǲ����д������Ƶ�
		if( 0 != pProto->accept_times )
		{
			INT nTimes = GetQuestCompleteTimes(u16QuestID);
			if( nTimes >= pProto->accept_times )
				return E_CanTakeQuest_FAILED_WRONG_TIMES;
		}
	}

	// ���ǰ������
	BYTE byPreQuestOK = 0x01;
	if( pProto->prev_quest_id != 0 && FALSE == IsHaveDoneQuest(pProto->prev_quest_id) )
	{
		byPreQuestOK = 0x00;
	}
	BYTE byPreQuest2OK = 0x01;
	if( pProto->prev_quest2_id != 0 && FALSE == IsHaveDoneQuest(pProto->prev_quest2_id) )
	{
		byPreQuest2OK = 0x00;
	}
	if( pProto->prev_quest_relation == 0 && (byPreQuestOK&byPreQuest2OK) == 0 )//ǰ��������붼���
	{
		return E_CanTakeQuest_FAILED_PREV;
	}
	else if( pProto->prev_quest_relation == 1 && (byPreQuestOK|byPreQuest2OK) == 0 )//���һ��ǰ�����񼴿�
	{
		return E_CanTakeQuest_FAILED_PREV;
	}

	// ��������������ʱ��
	if(pProto->period)
	{
		DWORD dwStartTime = GetQuestCompleteAcceptTime(u16QuestID);
		DWORD dwStartWeekDay = WhichWeekday(dwStartTime);
		DWORD dwCurrentTime = GetCurrentDWORDTime();
		DWORD dwCurrentWeekDay = WhichWeekday(dwCurrentTime);

		// ÿ������
		if(GT_INVALID != dwStartTime && EQuestPeriodic_DAY == pProto->period_type)
		{
			if(dwStartWeekDay == dwCurrentWeekDay)
				return E_CanTakeQuest_Tomorrow;
		}

		// ÿ������
		if (GT_INVALID != dwStartTime && EQuestPeriodic_WEEK == pProto->period_type)
		{
			if(dwCurrentWeekDay != (EWeek)pProto->week)
				return E_CanTakeQuest_Week;
		}
	}

	// ���ȼ�����
	if( 0 != pProto->accept_req_min_level )
	{
		if( GetLevel() < pProto->accept_req_min_level )
			return E_CanTakeQuest_FAILED_LOW_LEVEL;
	}
	if( 0 != pProto->accept_req_max_level )
	{
		if( GetLevel() > pProto->accept_req_max_level )
			return E_CanTakeQuest_FAILED_LOW_LEVEL;
	}

	// ����Ա�����
	if( 0 != pProto->sex )
	{
		BYTE bySex = GetSex();
		if( 0 == bySex && 1 == pProto->sex )
		{
			return E_CanTakeQuest_FAILED_WRONG_SEX;
		}
		if( 1== bySex && 2 == pProto->sex )
		{
			return E_CanTakeQuest_FAILED_WRONG_SEX;
		}
	}

	//// ���ְҵ����
	//if( ECT_Null != pProto->class_type )
	//{
	//	if( GetClass() != pProto->class_type )
	//	{
	//		return E_CanTakeQuest_FAILED_WRONG_CLASS;
	//	}
	//}

	// todo������
	

	// ��⼼������
	for(INT n = 0; n < QUEST_SKILLS_COUNT; n++)
	{
		if( !P_VALID(pProto->accept_req_skill[n]) ) continue;

		Skill* pSkill = GetSkill(pProto->accept_req_skill[n]);
		if( !P_VALID(pSkill) || pSkill->GetLevel() < pProto->accept_req_skill_val[n] )
		{
			return E_CanTakeQuest_FAILED_MISSING_SKILLS;
		}
	}

	// �����������
	for(INT n = 0; n < QUEST_ATTS_COUNT; n++)
	{
		if( ERA_Null >= pProto->accept_req_att[n] ||
			ERA_End <= pProto->accept_req_att[n] )
			continue;
		
		if( GetAttValue(pProto->accept_req_att[n]) < pProto->accept_req_att_val[n] )
			return E_CanTakeQuest_FAILED_MISSING_Att;
	}

	// �����Ʒ����
	for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
	{
		if( !P_VALID(pProto->accept_req_item[n]) ) continue;

		INT nTotal = GetItemMgr().GetBagSameItemCount(pProto->accept_req_item[n]) + GetItemMgr().GetQuestBagSameItemCount(pProto->accept_req_item[n]);
		if( nTotal < pProto->accept_req_item_num[n] )
		{
			return E_CanTakeQuest_FAILED_MISSING_ITEMS;
		}
	}

	// todo������Ƿ��Ѿ���飬�Ƿ�������

	// ��ⱳ���ܷ���½�������������Ʒ
	if( (0 != pProto->src_item) && pProto->src_item_num > 0)
	{
		if( GetItemMgr().GetBagFreeSize() < pProto->src_item_num ) 
			return E_CanTakeQuest_FAILED_BAG_FULL;
	}

	// �ű��ж�
	const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16QuestID);
	if( P_VALID(pScript) && FALSE == pScript->CheckAccept(u16QuestID, this, pNPC) )
	{
		return GT_INVALID;
	}

	// ��������Ƿ�����
	for(nIndex = 0; nIndex < QUEST_MAX_COUNT; ++nIndex)
	{
		if( FALSE == m_Quests[nIndex].IsValid() )
		{
			break;
		}
	}
	if( nIndex >= QUEST_MAX_COUNT )
	{
		return E_CanTakeQuest_FAILED_QUEST_NUM_FULL;
	}

	// ���ͨ��
	return E_Success;
}

//----------------------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------------------
BOOL Role::AddQuest(const tagQuestProto* pProto, INT nIndex)
{
	if( !P_VALID(pProto) ) return FALSE;
	if( nIndex < 0 || nIndex >= QUEST_MAX_COUNT ) return FALSE;
	if( m_Quests[nIndex].IsValid() ) return FALSE;

	// �������
	m_mapCurrentQuests.Add(pProto->id, &m_Quests[nIndex]);
	m_Quests[nIndex].Init(pProto, this, nIndex);

	// ���͸��ͻ�������һ������
	tagNS_AddQuest send1;
	send1.u16QuestID = m_Quests[nIndex].GetID();
	m_Quests[nIndex].GetInitQuestItem(send1.n16ItemCount);
	m_Quests[nIndex].GetInitDynamicTarget(&send1.DynamicTarget, send1.dwSize);
	SendMessage(&send1, send1.dwSize);

	// ���͸����ݿⱣ��
	tagNDBC_AcceptQuest send;
	send.dwRoleID = GetID();
	send.acceptQuest.u16QuestID = m_Quests[nIndex].GetID();
	send.acceptQuest.dwAcceptTime = m_Quests[nIndex].GetAcceptTime();
	g_dbSession.Send(&send, send.dwSize);

	// ���������ʱ��Ҫɾ��ĳЩ������Ʒ����������ɾ��
	if( pProto->del_req_item )
	{
		for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
		{
			if( !P_VALID(pProto->accept_req_item[n]) ) break;

			GetItemMgr().RemoveFromRole(pProto->accept_req_item[n], (INT32)pProto->accept_req_item_num[n], (DWORD)ELCID_Quest_Accept);
		}
	}

	// ���������ʱ���һ��������Ʒ�������������
	if( P_VALID(pProto->src_item) )
	{
		GetItemMgr().Add2Role(EICM_AccQuest, pProto->id, pProto->src_item, pProto->src_item_num, EIQ_Null, ELCID_Quest_Accept);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------
// �������
//----------------------------------------------------------------------------------------
INT Role::CompleteQuest(UINT16 u16QuestID, DWORD dwNPCID, INT nChoiceItemIndex, UINT16& u16NexitQuestID)
{
	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if( !P_VALID(pProto) ) return E_QUEST_NOT_EXIST;

	// ��������Ƿ����
	Quest* pQuest = m_mapCurrentQuests.Peek(u16QuestID);
	if( !P_VALID(pQuest) ) return E_QUEST_NOT_EXIST;

	// ��������Ƿ��Ѿ����
	INT nRet = CanCompleteQuest(pQuest, dwNPCID, nChoiceItemIndex);

	// ������Խ������������������ɾ������
	if( E_Success == nRet )
	{
		pQuest->SetCompleteMark(TRUE);

		// ���ýű������ʱ����
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		pQuest->OnComplete(pNPC);

		// ��������
		RewardQuest(pQuest, nChoiceItemIndex);
		
		// tbc:���ͳƺ���Ϣ
		GetTitleMgr()->SigEvent(ETE_QUEST_COMPLETE, pQuest->GetID(), GT_INVALID);

		// �۳���Ǯ
		if(pProto->complete_del_money)
		{
			INT32 n32Money = pProto->complete_req_money;
			GetCurMgr().DecBagSilver(n32Money, ELCID_Quest_Complete);
		}

		// ɾ������
		RemoveQuest(pQuest->GetID(), TRUE);

		// ����Ƿ��к����Զ���ȡ����
		if(pProto->auto_add_quest)
			u16NexitQuestID = pProto->next_quest_id;
	}

	return nRet;
}

//----------------------------------------------------------------------------------------
// �Ƿ���Խ�����
//----------------------------------------------------------------------------------------
INT Role::CanCompleteQuest(Quest* pQuest, DWORD dwNPCID, INT nChoiceItemIndex)
{
// 	// ������
// 	if (GetEarnRate() < 10000)
// 	{
// 		return E_QUEST_FATIGUEGUARD;
// 	}

	if( !P_VALID(pQuest) ) return GT_INVALID;

	Creature* pNPC = NULL;

	// ���dwNPCID��ΪGT_INVALID�������NPC�Ƿ�Ϸ�
	if( P_VALID(dwNPCID) )
	{
		pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) )
		{
			return E_QUEST_NPC_NOT_EXIST;
		}
		else if( FALSE == pNPC->IsInDistance(*this, 500.0f) )
		{
			return E_QUEST_NPC_TOO_FAR;
		}
		else if( FALSE == g_questMgr.IsNPCCompleteQuest(pNPC, pQuest->GetID()) )
		{
			return E_QUEST_NOT_EXIST;
		}
	}
	// ���dwNPCIDΪGT_INVALID������������Ƿ�ͨ��NPC�Ϳ����ύ
	else
	{
		if( g_questMgr.IsNeedNPCCompleteQuest(pQuest->GetID()) )
		{
			return E_QUEST_NPC_NOT_EXIST;
		}
	}

	// �������û�д������״̬
	if( FALSE == pQuest->IsCanComplete(nChoiceItemIndex, pNPC) )
	{
		// todo: �Ȱѷ���������һ��
		return GT_INVALID;
	}

	return E_Success;
}


//----------------------------------------------------------------------------------------
// �������ɾ������
//----------------------------------------------------------------------------------------
INT Role::DeleteQuest(UINT16 u16QuestID)
{
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return E_QUEST_NOT_EXIST;

	// ���ýű�������ɾ���ӿ�
	pQuest->OnCancel();

	// �ҵ���������ɾ��������
	RemoveQuest(u16QuestID, FALSE);

	return E_Success;
}

//----------------------------------------------------------------------------------------
// ɾ������
//----------------------------------------------------------------------------------------
VOID Role::RemoveQuest(UINT16 u16QuestID, BOOL bComplete)
{
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return;

	const tagQuestProto* pProto = pQuest->GetProto();
	DWORD dwStartTime = pQuest->GetAcceptTime();
	tagQuestDynamicTarget*	pQuestDynamicTarget = pQuest->GetDynamicTarget();

	// ���Ƚ���������
	m_mapCurrentQuests.Erase(u16QuestID);
	pQuest->Destroy();

	// ������������
	if( bComplete )
	{
		// ���������Ҫɾ���ռ�����Ʒ����ɾ����Ʒ
		if( P_VALID(pProto) && pProto->delete_item )
		{
			for(INT i = 0; i < QUEST_ITEMS_COUNT; i++)
			{
				if( !P_VALID(pProto->complete_req_item[i]) )
					break;

				GetItemMgr().RemoveFromRole(pProto->complete_req_item[i], (INT32)pProto->complete_req_item_num[i], (DWORD)ELCID_Quest_Complete);
			}
		}

		// ɾ����������ռ�����Ʒ
		if( P_VALID(pQuestDynamicTarget) && EQTT_Collect == pQuestDynamicTarget->eTargetType)
		{
			for(INT n = 0; n < DYNAMIC_TARGET_COUNT; n++)
			{
				if( !P_VALID(pQuestDynamicTarget->dwTargetID[n]) )
					break;

				GetItemMgr().RemoveFromRole(pQuestDynamicTarget->dwTargetID[n], (INT32)pQuestDynamicTarget->nTargetNum[n], (DWORD)ELCID_Quest_Complete);
			}
		}

		// ɾ������������Ӧ������������Ʒ
		GetItemMgr().RemoveFromRole(u16QuestID, (DWORD)ELCID_Quest_Complete);

		// ����������뵽��������б���
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) )
		{
			pDoneQuest = new tagQuestDoneSave;
			pDoneQuest->u16QuestID = u16QuestID;
			pDoneQuest->dwStartTime = dwStartTime;
			pDoneQuest->nTimes = 0;
			m_mapCompleteQuests.Add(u16QuestID, pDoneQuest);
		}
		pDoneQuest->nTimes += 1;
		pDoneQuest->dwStartTime = dwStartTime;

		// ���͸����ݿⱣ��
		tagNDBC_CompleteQuest send;
		send.dwRoleID = GetID();
		send.QuestDone.u16QuestID = pDoneQuest->u16QuestID;
		send.QuestDone.dwStartTime = pDoneQuest->dwStartTime;
		send.QuestDone.nTimes = pDoneQuest->nTimes;
		g_dbSession.Send(&send, send.dwSize);
	}
	// ������ɾ������
	else
	{
		// ɾ������������Ӧ������������Ʒ
		GetItemMgr().RemoveFromRole(u16QuestID, (DWORD)ELCID_Quest_Discard);

		// ���͸����ݿ�ɾ��
		tagNDBC_DiscardQuest send;
		send.dwRoleID = GetID();
		send.u16QuestID = u16QuestID;
		g_dbSession.Send(&send, send.dwSize);
	}

	// ɾ�������ȡʱ������Ʒ
	if( P_VALID(pProto->src_item) && pProto->src_item_num > 0 )
	{
		GetItemMgr().RemoveFromRole(pProto->src_item, pProto->src_item_num, (DWORD)ELCID_Quest_Complete);
	}
}


//----------------------------------------------------------------------------------------
// ��������NPC״̬
//----------------------------------------------------------------------------------------
VOID Role::UpdateQuestNPCTalk(UINT16 u16QuestID, DWORD dwNPCID)
{
	// �Ƿ����������
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return;

	// �õ������Ƿ����
	Creature* pCreature = GetMap()->FindCreature(dwNPCID);
	if( !P_VALID(pCreature) ) return;

	// ���Ը�������
	pQuest->OnEvent(EQE_Talk, pCreature->GetID(), pCreature->GetTypeID());
}

//----------------------------------------------------------------------------------------
// ��������Trigger״̬
//----------------------------------------------------------------------------------------
VOID Role::UpdateQuestTrigger(UINT16 u16QuestID, DWORD dwTriggerID)
{
	// �Ƿ����������
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return;

	// ���Ȳ鿴�����ǲ����ڸô�������
	if( FALSE == GetMap()->IsInTrigger(this, dwTriggerID) )
		return;

	// �õ��ô��������������к�
	DWORD dwTriggerSerial = GetMap()->GetTriggerQuestSerial(dwTriggerID);
	if( !P_VALID(dwTriggerSerial) ) return;

	// ���Ը�������
	pQuest->OnEvent(EQE_Trigger, dwTriggerSerial);
}

//----------------------------------------------------------------------------
// ���������¼�
//----------------------------------------------------------------------------
VOID Role::OnQuestEvent(EQuestEvent eQuestType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */,DWORD dwEventMisc3/*=0*/)
{
	for(INT n = 0; n < QUEST_MAX_COUNT; ++n)
	{
		if( FALSE == m_Quests[n].IsValid() ) continue;

		m_Quests[n].OnEvent(eQuestType, dwEventMisc1, dwEventMisc2, dwEventMisc3);
	}
}

//-------------------------------------------------------------------------------------
// �������ɿضԻ���ȱʡ�¼�
//-------------------------------------------------------------------------------------
VOID Role::OnDlgDefaultEvent(EMsgUnitType eDlgTarget, DWORD dwTargetID, EDlgOption eDlgOption)
{
	switch (eDlgTarget)
	{
	case EMUT_DlgTarget_Quest:
		{
			Quest* pQuest = GetQuest((UINT16)dwTargetID); 
			if(!P_VALID(pQuest))	return;

			pQuest->OnEvent(EQE_DlgDefault, eDlgOption);
		}
		break;
	case EMUT_DlgTarget_Item:
		{

		}
		break;
	case EMUT_DlgTarget_Creature:
		{

		}
		break;
	default:
		break;
	}
}

//----------------------------------------------------------------------------------------
// ������
//----------------------------------------------------------------------------------------
VOID Role::RewardQuest(Quest* pQuest, INT32 nChoiceItemIndex)
{
	// ������
	if (GetEarnRate() < 10000)
	{
		return ;
	}

	const tagQuestProto* pProto = pQuest->GetProto();
	if( !P_VALID(pProto) ) return;

	// ������Ʒ
	for(INT n = 0; n < QUEST_REW_ITEM; ++n)
	{
		if( !P_VALID(pProto->rew_item[n]) ) break;

		if( pProto->rew_item_num[n] <= 0 ) continue;

		GetItemMgr().Add2Role(EICM_Quest, pQuest->GetID(), pProto->rew_item[n], (INT16)pProto->rew_item_num[n], (EItemQuality)pProto->rew_item_quality[n], ELCID_Quest_Complete);
	}

	// ��ѡ������Ʒ
	if( nChoiceItemIndex >= 0 && nChoiceItemIndex < QUEST_REW_ITEM )
	{
		if( P_VALID(pProto->rew_choice_item[nChoiceItemIndex]) &&
			pProto->rew_choice_item_num[nChoiceItemIndex] > 0 )
		{
			GetItemMgr().Add2Role(EICM_Quest, pQuest->GetID(), pProto->rew_choice_item[nChoiceItemIndex], 
								(INT16)pProto->rew_choice_item_num[nChoiceItemIndex], 
								(EItemQuality)pProto->rew_choice_quality[nChoiceItemIndex], ELCID_Quest_Complete);
		}
	}

	// ����
	if( pProto->rew_xp != 0 )
	{
		ExpChange(pProto->rew_xp);
	}

	// ��Ǯ
	if( pProto->rew_money > 0 )
	{
		GetCurMgr().IncBagSilver(pProto->rew_money, (DWORD)ELCID_Quest_Complete);
	}

	// ����
	for (INT i = 0; JDG_VALID(ERT, pProto->rew_rep[i]) && i < QUEST_REPUTATIONS_COUNT; ++i)
	{
		GetClanData().RepModVal(MTRANS_ERT2ECLT(pProto->rew_rep[i]), pProto->rew_rep_val[i]);
	}

	// ����
	for (INT i = 0; JDG_VALID(ERCT, pProto->rew_contribution[i]) && i < QUEST_CONTRIBUTION_COUNT; ++i)
	{
		GetCurMgr().IncClanCon(pProto->rew_contribution_val[i], GT_INVALID, MTRANS_ECCT2ECLT(pProto->rew_contribution[i]));
	}

	// ����
	for(INT n = 0; n < QUEST_ATTS_COUNT; ++n)
	{
		if( ERA_Null >= pProto->rew_att[n] ||
			ERA_End <= pProto->rew_att[n] )
			break;

		ModAttValue(pProto->rew_att[n], pProto->rew_att_val[n]);
	}

	// todo��Buff
}

//----------------------------------------------------------------------------------------
// ��ͼ������
//----------------------------------------------------------------------------------------
BOOL Role::MapTrigger(DWORD dwTriggerID, DWORD dwMisc)
{
	Map* pMap = GetMap();

	if( !P_VALID(pMap) ) return FALSE;

	const tagMapInfo* pInfo = pMap->GetMapInfo();

	// ���Ƿ���ڸ�Trigger
	tagMapTriggerInfo* pTrigger = pInfo->mapTrigger.Peek(dwTriggerID);
	if( !P_VALID(pTrigger) ) return FALSE;

	// �����ҵ�ǰλ���ǲ�����trigger��Χ֮��
	if( !pMap->IsInTrigger(this, dwTriggerID) ) return FALSE;

	// �鿴pTrigger������

	switch(pTrigger->eType)
	{
		// �л���ͼ
	case EMT_GotoNewMap:
		{
			// �õ�Ŀ���ͼ�ĵ�����
			const tagMapInfo* pMapInfo = g_mapCreator.GetMapInfo(pTrigger->dwMapID);
			if( !P_VALID(pMapInfo) ) return FALSE;

			const tagMapWayPointInfoList* pList = pMapInfo->mapWayPointList.Peek(pTrigger->dwWayPointID);
			if( !P_VALID(pList) ) return FALSE;

			// ��Ŀ�굼�����б�����ȡһ��������
			tagWayPointInfo wayPoint;
			pList->list.RandPeek(wayPoint);

			// todo: �������һ���㣬Ŀǰѡȡ���ĵ�

			// �ƶ����µ�ͼ
			if( FALSE == GotoNewMap(pTrigger->dwMapID, wayPoint.vPos.x, wayPoint.vPos.y, wayPoint.vPos.z, dwMisc))
				return FALSE;
		}
		break;

		// ���񴥷���
	case EMT_Script:
		{
			// �õ���trigger��Ӧ���������к�
			if( !P_VALID(pTrigger->dwQuestSerial) ) return FALSE;
			OnQuestEvent(EQE_Trigger, pTrigger->dwQuestSerial);
		}
		break;

		// ʲô��û�У�����ýű�
	case EMT_Null:
		{
			pMap->OnEnterTrigger(this, pTrigger);
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------
// ֪ͨ���ѽ��븱��
//--------------------------------------------------------------------------------------
DWORD Role::InstanceNotify(BOOL bNotify)
{
	if(bNotify == FALSE)	return GT_INVALID;

	// �ҵ����С��
	const Team* pTeam = g_groupMgr.GetTeamPtr(GetTeamID());
	if( !P_VALID(pTeam) ) return GT_INVALID;

	// �������Ƿ񴴽��˸���
	Map* pMap = g_mapCreator.GetMap(pTeam->GetOwnInstanceMapID(), pTeam->GetOwnInstanceID());
	if( !P_VALID(pMap) || EMT_Instance != pMap->GetMapType() ) return GT_INVALID;

	MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
	if( !P_VALID(pInstance) ) return GT_INVALID;

	const tagInstance* pInstanceProto = pInstance->GetInstanceProto();
	if( !P_VALID(pInstanceProto) ) return GT_INVALID;	

	// �����Ƿ�����֪ͨ����
	if( !pInstanceProto->bNoticeTeamate ) return GT_INVALID;

	// ֪ͨ����
	tagNS_InstanceAgree	 send;
	send.dwInviterID	=	m_dwID;
	send.dwMapID		=	pInstance->GetMapID();
	g_groupMgr.SendTeamInstanceNotify(this, GetTeamID(), &send, send.dwSize);

	return E_Success;
}

//--------------------------------------------------------------------------------------
// ����Ƿ�ͬ��������ҽ��븱��������
//--------------------------------------------------------------------------------------
DWORD Role::InstanceAgree(BOOL bAgree)
{
	if(bAgree == FALSE)		return GT_INVALID;

	if (IsInRoleStateAny(ERS_Commerce | ERS_PrisonArea))	return GT_INVALID;
			
	// �ҵ����С��
	const Team* pTeam = g_groupMgr.GetTeamPtr(GetTeamID());
	if( !P_VALID(pTeam) ) return GT_INVALID;

	// �������Ƿ񴴽��˸���
	Map* pMap = g_mapCreator.GetMap(pTeam->GetOwnInstanceMapID(), pTeam->GetOwnInstanceID());
	if( !P_VALID(pMap) || EMT_Instance != pMap->GetMapType() ) return GT_INVALID;

	MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
	if( !P_VALID(pInstance) ) return GT_INVALID;

	const tagInstance* pInstanceProto = pInstance->GetInstanceProto();
	if( !P_VALID(pInstanceProto) ) return GT_INVALID;

	GotoNewMap(pInstance->GetMapID(), pInstanceProto->vEnterPos.x, pInstanceProto->vEnterPos.y, pInstanceProto->vEnterPos.z, GT_INVALID, FALSE);

	return E_Success;
}

//---------------------------------------------------------------------------------
// ��������뿪����
//---------------------------------------------------------------------------------
DWORD Role::LeaveInstance()
{
	const tagMapInfo* pInfo = GetMap()->GetMapInfo();
	if( !P_VALID(pInfo) ) return GT_INVALID;

	if( EMT_Normal == pInfo->eType )
		return GT_INVALID;

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(pInfo->dwID);
	if( !P_VALID(pMapMgr) ) return GT_INVALID;

	pMapMgr->RoleInstanceOut(this);

	return E_Success;
}

//--------------------------------------------------------------------------------------
// �õ������ĸ���ID������ж����ȡ����ģ����û�ж����ȡ�Լ��ģ�
//--------------------------------------------------------------------------------------
DWORD Role::GetOwnInstanceID() const
{
	DWORD dwTeamID = GetTeamID();
	
	// �����С��
	if( dwTeamID != GT_INVALID )
	{
		const Team *pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
		if( P_VALID(pTeam) )
		{
			return pTeam->GetOwnInstanceID();
		}
		else
		{
			return GT_INVALID;
		}
	}
	else // ���û��С��
	{
		return m_dwOwnInstanceID;
	}
}

//--------------------------------------------------------------------------------------
// �õ������ĸ�����ͼID������ж����ȡ����ģ�û�ж����ȡ�Լ��ģ�
//--------------------------------------------------------------------------------------
DWORD Role::GetOwnInstanceMapID() const
{
	DWORD dwTeamID = GetTeamID();

	// �����С��
	if( dwTeamID != GT_INVALID )
	{
		const Team *pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
		if( P_VALID(pTeam) )
		{
			return pTeam->GetOwnInstanceMapID();
		}
		else
		{
			return GT_INVALID;
		}
	}
	else // ���û��С��
	{
		return m_dwOwnInstanceMapID;
	}
}

//--------------------------------------------------------------------------------------
// ȥһ���µ�ͼ
//--------------------------------------------------------------------------------------
BOOL Role::GotoNewMap(DWORD dwDestMapID, FLOAT fX, FLOAT fY, FLOAT fZ, DWORD dwMisc, BOOL bSameInstace)
{
	if( fX < 0.0f || fZ < 0.0f || fY < 0.0f )
	{
		return FALSE;
	}

	// ���һ�µ�ͼ�Ĵ�С
	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwDestMapID);
	if( !P_VALID(pInfo) ) return FALSE;

	// ���һ������
	if( fX < 0.0f || fX >= FLOAT(pInfo->nWidth * TILE_SCALE) ||
		fZ < 0.0f || fZ >= FLOAT(pInfo->nHeight * TILE_SCALE) )
		return FALSE;

	// ���������
	PetSoul* pMountSoul = GetPetPocket()->GetMountPetSoul();
	if (P_VALID(pMountSoul))
	{
		Role* pPassenger = pMountSoul->GetPassenger();
		if (P_VALID(pPassenger))
		{
			pPassenger->StopMount();
		}
	}

	// ���ȼ���ǲ��Ǳ���ͼ
	if( GetMapID() == dwDestMapID && bSameInstace )
	{

		// ͬ��ͼ�ڴ���
		Vector3 vNewPos(fX, fY, fZ);
		m_MoveData.ForceTeleport(vNewPos);

		// �õ��µľŹ���
		GetMap()->SendGotoNewMapToPlayer(this);

		return TRUE;
	}
	else
	{
		// ���µ�ͼ�� �����ҵ�MapMgr
		MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwDestMapID);
		if( !P_VALID(pMapMgr) ) return FALSE;

		// û���ҵ�MapMgr����ͼID�Ƿ�
		Map* pMap = GetMap();
		if( !P_VALID(pMap) ) return FALSE;

		// �����л���ͼ
		Vector3 vDestPos;
		vDestPos.x = fX;
		vDestPos.y = fY;
		vDestPos.z = fZ;

		g_mapCreator.RoleChangeMap(m_dwID, dwDestMapID, vDestPos, dwMisc);

		return TRUE;
	}
}

//----------------------------------------------------------------------------------------------------
// �س�
//----------------------------------------------------------------------------------------------------
VOID Role::ReturnCity()
{
	if( !P_VALID(m_dwRebornMapID) ) return;

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(m_dwRebornMapID);
	if( !P_VALID(pInfo) ) return;

	GotoNewMap(m_dwRebornMapID, pInfo->vRebornPos.x, pInfo->vRebornPos.y, pInfo->vRebornPos.z);
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID Role::ReName(LPCTSTR szNewName)
{
	ZeroMemory( g_roleMgr.GetRoleInfo(this->GetID())->szRoleName, X_SHORT_NAME );
	_tcscpy_s( g_roleMgr.GetRoleInfo(this->GetID())->szRoleName, X_SHORT_NAME, szNewName);

	SaveNewName2DB();
}

VOID Role::SaveNewName2DB()
{
	tagNDBC_SaveRenameInfo send;

	send.dwAccountID = (g_roleMgr.GetRoleInfo(GetID()))->dwAccountID;
	send.dwRoleID = (g_roleMgr.GetRoleInfo(GetID()))->dwRoleID;
	_tcscpy_s(send.szName, g_roleMgr.GetRoleInfo(this->GetID())->szRoleName);

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------
// ѧϰһ������
//-------------------------------------------------------------------------------------
INT Role::LearnSkill(DWORD dwSkillID, DWORD dwNPCID, INT64 n64ItemID)
{
	// ���ü����Ƿ��Ѿ�����
	Skill* pSkill = GetSkill(dwSkillID);
	if( P_VALID(pSkill) ) return E_LearnSkill_Existed;

	// �����ҵ��ü��ܶ�Ӧ�ľ�̬���ԣ�Ĭ��һ����
	DWORD dwSkillTypeID = Skill::CreateTypeID(dwSkillID, 1);
	const tagSkillProto* pProto = g_attRes.GetSkillProto(dwSkillTypeID);
	if( !P_VALID(pProto) ) return E_LearnSkill_ProtoNotFound;

	// �������ȼ�
	if( m_nLevel < pProto->nNeedRoleLevel )
		return E_LearnSkill_NeedMoreLevel;

	// ���ְҵ����
	if( E_Success != VocationCheck( pProto->dwVocationLimit ))
		return E_LearnSkill_ClassLimit;

	// ���ǰ�ü���
	if( GT_INVALID != pProto->dwPreLevelSkillID )
	{
		DWORD dwPreSkillID = Skill::GetIDFromTypeID(pProto->dwPreLevelSkillID);
		INT nPreSkillLevel = Skill::GetLevelFromTypeID(pProto->dwPreLevelSkillID);
		Skill* pPreSkill = GetSkill(dwPreSkillID);
		if( !P_VALID(pPreSkill) || pPreSkill->GetLevel() < nPreSkillLevel )
			return E_LearnSkill_NoPreSkill;

	}

	INT nTalentIndex = GT_INVALID;			// ��������ʼ����Ҹ�ϵ������Ͷ�����¼��ϵ���ʵ�λ��
	INT nBlankTalentIndex = GT_INVALID;		// ��������ʼ��ܵ���ϵ����δͶ�����¼��ϵ���ʿɲ����λ��

	// ��������ʼ��ܣ���������������������Ƿ��Ѿ�����
	if( ETT_Null != pProto->eTalentType )
	{
		nTalentIndex = FindTalentIndex(pProto->eTalentType);

		if( GT_INVALID != nTalentIndex )
		{
			// ��������ϵ������Ͷ�����ʵ��ǲ������㼼�ܿ�ѧ����
			if( GetTalentPoint(nTalentIndex) < pProto->nNeedTalentPoint )
				return E_LearnSkill_NeedMoreTalentPoint;
		}
		else
		{
			// ��������û�и�ϵ���ʣ����ж��Ƿ񻹿�����ѧϰ�µ�����
			nBlankTalentIndex = FindBlankTalentIndex(pProto->eTalentType);
			if( GT_INVALID == nBlankTalentIndex )
				return E_LearnSkill_ExceedTalentMaxNum;
		}
	}
	
	// NPC��ʽѧϰ
	if( GT_INVALID != dwNPCID )
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) ) return E_LearnSkill_NPCNotEixst;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_LearnSkill_NPCTooFar;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Teacher) )
			return E_LevelUpSkill_NPCCanNotTeach;

		// todo: ��Ҫ���NPC�ɲ����Խ��ڸ��ּ���
	}
	// ������ѧϰ
	else if( GT_INVALID != n64ItemID )
	{
		tagItem *pItem = GetItemMgr().GetBagItem(n64ItemID);
		if( !P_VALID(pItem) )
			return E_Item_NotFound;

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
			return E_Item_LevelLimit;
		
		// �������Ч������
		if( pItem->pProtoType->eSpecFunc != EISF_LearnSkill 
			|| pItem->pProtoType->nSpecFuncVal1 != dwSkillID)
			return E_Item_Used_NotValid;
	}
	// ���ɼ���
	else if (pProto->nType2 == ESSTE_Guild)
	{
		// �ϲ��Ѿ�ͨ����֤
	}
	// Ͷ��ѧϰ
	else
	{
		// ���ü����Ƿ�����������
		if( ETT_Null == pProto->eTalentType )
			return E_LearnSkill_NoTalentType;

		// �������Ŀǰ�����Ƿ������ʵ�
		if( GetAttValue(ERA_TalentPoint) <= 0 )
			return E_LearnSkill_NoTalentPoint;

		// ��⼼�ܱ����ǹ̶�������ʽ
		if( ESLUT_Fixed != pProto->eLevelUpType )
			return GT_INVALID;
	}

	// ������������ܣ����ж�Ŀǰ���ϵ��������������Ƿ񳬹����������
	if( ESSTE_Produce == pProto->nType2 )
	{
		if( GetProduceSkillNum() >= MAX_ROLE_PRODUCE_NUM )
			return E_LearnSkill_ProduceSkillFull;
	}

	// һ���ж�ͨ�������������Ӽ���
	INT nSelfLevel = 0;
	INT nLearnLevel = 0;

	// ������ܲ������ʼ��ܣ�������nSelfLevel
	if( ETT_Null == pProto->eTalentType )
	{
		nSelfLevel = 1;
	}
	// ��������ʼ���
	else
	{
		// �����ͨ��NPC���߼�����ѧ���ģ�������nSelfLevel
		if( GT_INVALID != dwNPCID || GT_INVALID != n64ItemID )
		{
			nSelfLevel = 1;

			// ����ĳϵ����
			if( nTalentIndex != GT_INVALID )
			{
				;
			}
			else if( nBlankTalentIndex != GT_INVALID )
			{
				AddTalent(nBlankTalentIndex, pProto->eTalentType, 0);
				SaveTalentPoint2DB(nBlankTalentIndex);
			}
			else
			{
				ASSERT(0);
			}
		}
		// ����������nLearnLevel����������Ӧ�����ʵ�
		else
		{
			nLearnLevel = 1;

			// ��ҿ۳�һ�����ʵ�
			ModAttValue(ERA_TalentPoint, -1);

			// �ۼ�ĳЩ���ʵ����ʵ�
			if( nTalentIndex != GT_INVALID )
			{
				AddTalentPoint(nTalentIndex, 1);
				SaveTalentPoint2DB(nTalentIndex);
			}
			else if( nBlankTalentIndex != GT_INVALID )
			{
				AddTalent(nBlankTalentIndex, pProto->eTalentType, 1);
				SaveTalentPoint2DB(nBlankTalentIndex);
			}
			else
			{
				ASSERT(0);
			}
		}
	}

	// ��Ӽ���
	pSkill = new Skill(dwSkillID, nSelfLevel, nLearnLevel, 0, 0);
	AddSkill(pSkill);

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;

}


//---------------------------------------------------------------------------------------
// ����һ������
//---------------------------------------------------------------------------------------
INT Role::LevelUpSkill(DWORD dwSkillID, DWORD dwNPCID, INT64 n64ItemID)
{
	// ���ü����Ƿ��Ѿ�����
	Skill* pSkill = GetSkill(dwSkillID);
	if( !P_VALID(pSkill) ) return E_LevelUpSkill_NotExist;

	// ��⼼���Ƿ��Ѿ�������ߵȼ�
	if( pSkill->GetLevel() >= pSkill->GetMaxLevel() )
		return E_LevelUpSkill_ExceedMaxLevel;

	// �õ�������ľ�̬����
	INT nLevel = pSkill->GetLevel() + 1;

	// �����ҵ��ü��ܶ�Ӧ�ľ�̬����
	DWORD dwSkillTypeID = Skill::CreateTypeID(dwSkillID, nLevel);
	const tagSkillProto* pProto = g_attRes.GetSkillProto(dwSkillTypeID);
	if( !P_VALID(pProto) ) return E_LevelUpSkill_ProtoNotFound;

	// ����������ȷ�ʽ�����ģ�����ֱ����
	if( ESLUT_Exp == pProto->eLevelUpType )
		return E_LevelUpSkill_NotFixed;

	// �������ȼ�
	if( m_nLevel < pProto->nNeedRoleLevel )
		return E_LearnSkill_NeedMoreLevel;

	// todo: ���ְҵ����
	//if( E_Success != VocationCheck( pProto-> ))

	// ���ǰ�ü���
	DWORD dwPreSkillID = Skill::GetIDFromTypeID(pProto->dwPreLevelSkillID);
	INT nPreSkillLevel = Skill::GetLevelFromTypeID(pProto->dwPreLevelSkillID);
	Skill* pPreSkill = GetSkill(dwPreSkillID);
	if( !P_VALID(pPreSkill) || pPreSkill->GetLevel() < nPreSkillLevel )
		return E_LearnSkill_NoPreSkill;

	INT nTalentIndex = GT_INVALID;			// ��������ʼ����Ҹ�ϵ������Ͷ�����¼��ϵ���ʵ�λ��

	// ��������ʼ��ܣ���������������������Ƿ��Ѿ�����
	if( ETT_Null != pProto->eTalentType )
	{
		nTalentIndex = FindTalentIndex(pProto->eTalentType);

		if( GT_INVALID != nTalentIndex )
		{
			// ��������ϵ������Ͷ�����ʵ��ǲ������㼼�ܿ�ѧ����
			if( GetTalentPoint(nTalentIndex) < pProto->nNeedTalentPoint )
				return E_LearnSkill_NeedMoreTalentPoint;
		}
		else
		{
			ASSERT(0);
			return GT_INVALID;
		}
	}

	// NPC��ʽ����
	if( GT_INVALID != dwNPCID )
	{
		// NPC����������������
		if( ETT_Null != pProto->eTalentType )
			return GT_INVALID;

		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) ) return E_LevelUpSkill_NPCNotEixst;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_LevelUpSkill_NPCTooFar;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Teacher) )
			return E_LevelUpSkill_NPCCanNotTeach;

		// todo: ��Ҫ���NPC�ɲ����Խ��ڸ��ּ���
	}
	// ������ѧϰ
	else if( GT_INVALID != n64ItemID )
	{
		// �����鲻��������������
		if( ETT_Null != pProto->eTalentType )
			return GT_INVALID;

		// todo: ����Ʒ�����������Ʒ�����������Ч������

		// todo: ���ü��������ϴ��ļ���ID�ǲ��ǲ����еļ���ID
	}
	// ���ɼ���
	else if (pProto->nType2 == ESSTE_Guild)
	{
		// �ϲ��Ѿ�ͨ����֤
	}
	// Ͷ��ѧϰ
	else
	{
		// ���ü����Ƿ�����������
		if( ETT_Null == pProto->eTalentType )
			return E_LevelUpSkill_NoTalentType;

		// �������Ŀǰ�����Ƿ������ʵ�
		if( GetAttValue(ERA_TalentPoint) <= 0 )
			return E_LevelUpSkill_NoTalentPoint;

		// ��⼼�ܱ����ǹ̶�������ʽ
		if( ESLUT_Fixed != pProto->eLevelUpType )
			return GT_INVALID;
	}

	// һ���ж�ͨ����������＼������
	BOOL bLearn = FALSE;

	// �����ͨ��NPC���߼����������ģ���������ȼ�����
	if( GT_INVALID != dwNPCID || GT_INVALID != n64ItemID )
	{
		bLearn = 1;
	}
	// ���ɼ�������
	else if (pProto->nType2 == ESSTE_Guild)
	{
		bLearn = FALSE;
	}
	// �����Ͷ��������
	else
	{
		bLearn = TRUE;

		// ��ҿ۳�һ�����ʵ�
		ModAttValue(ERA_TalentPoint, -1);

		// �ۼ�ĳϵ���ʵ����ʵ�
		if( nTalentIndex != GT_INVALID )
		{
			AddTalentPoint(nTalentIndex, 1);
			SaveTalentPoint2DB(nTalentIndex);
		}
		else
		{
			ASSERT(0);
		}
	}

	// ����
	if( bLearn )
		ChangeSkillLevel(pSkill, ESLC_Learn);
	else
		ChangeSkillLevel(pSkill, ESLC_Self);


	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;

}

//---------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------
INT Role::ForgetSkill(DWORD dwSkillID, DWORD dwNPCID)
{
	// �����������Ƿ��иü���
	Skill* pSkill = GetSkill(dwSkillID);
	if( !P_VALID(pSkill) ) return E_ForgetSkill_NotExist;

	// ��⼼�ܺϷ���
	if( ETT_Null != pSkill->GetTalentType() )
		return E_ForgetSkill_CanNotBeForgotten;

	// ���NPC�Ϸ���
	Creature* pNPC = GetMap()->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ||
		FALSE == pNPC->IsFunctionNPC(EFNPCT_Teacher) ||
		FALSE == pNPC->CheckNPCTalkDistance(this))
		return E_ForgetSkill_NPCNotValid;

	// todo�����NPC�Ƿ�ӵ�иü���

	// ���ͨ����ɾ������
	RemoveSkill(dwSkillID);

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;
}

//-----------------------------------------------------------------------------------
// ϴ��
//-----------------------------------------------------------------------------------
INT Role::ClearTalent(INT64 n64ItemID, BOOL bCheckItem)
{
	if (bCheckItem)
	{
		// �ж���Ʒ�����ǲ���ϴ�����
		tagItem* pItem = GetItemMgr().GetBagItem(n64ItemID);
		if( !P_VALID(pItem) || (pItem->pProtoType->eSpecFunc != EISF_RemoveTalentPt && pItem->pProtoType->eSpecFunc != EISF_ChangeVocation))
			return E_ClearAttPoint_ItemNotValid;

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
			return E_Item_LevelLimit;

		// �ж�ͨ����ɾ����Ʒ
		GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCID_Clear_Talent);
	}
	
	// ����Ƿ�������
	for(INT n = 0; n < X_MAX_TALENT_PER_ROLE; ++n)
	{
		if( ETT_Null == m_Talent[n].eType )
			continue;

		// ɾ�����и�ϵ�����ʼ��ܣ���map�������������ܻ������⣩
		TMap<DWORD, Skill*>::TMapIterator it = m_mapSkill.Begin();
		Skill* pSkill = NULL;
		while( m_mapSkill.PeekNext(it, pSkill) )
		{
			if( pSkill->GetTalentType() == m_Talent[n].eType )
				RemoveSkill(pSkill->GetID());
		}

		// ���������ظ����
		ModAttValue(ERA_TalentPoint, GetTalentPoint(n));
		// ��ո�ϵ����
		RemoveTalent(n);
		// �������ʵ����ݿ�
		SaveTalentPoint2DB(n);
	}

	// ���¼�����������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;

}

//----------------------------------------------------------------------------------
// ����PK����
//----------------------------------------------------------------------------------
INT Role::OpenSafeGuard(INT64 n64ItemID)
{
	// �鿴�Ƿ��Ѿ����ù���
	if( IsInRoleState(ERS_Safeguard) ) return E_SafeGuard_Already;

	if( P_VALID(GetMap()))
	{
		if( !GetMap()->CanSetSafeguard() )	return E_SafeGuard_PKStateInvalid;
	}

	// ���һ��PK״̬������״̬
	if( IsInRoleState(ERS_PK) || IsInRoleState(ERS_PKEX) || m_ePKState != ERPKS_Peace || IsInRoleState(ERS_Arena))
		return E_SafeGuard_PKStateInvalid;

	// ���һ��ʱ��
	//tagDWORDTime curTime = g_world.GetWorldTime();
	//if( CalcTimeDiff(curTime, m_CloseSafeGuardTime) < OPEN_SAFE_GUARD_TIME )
	//	return E_SafeGuard_TimeLimit;

	// ���һ�µȼ�
	BOOL bUseItem = FALSE;
	if( GetLevel() >= SAFE_GUARD_FREE_LEVEL )
	{
		// �����Ʒ�Ƿ�Ϸ�
		const tagItem* pItem = GetItemMgr().GetBagItem(n64ItemID);
		if( !P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_OpenSafeGuard )
		{
			return E_SafeGuard_ItemLimit;
		}
		else
		{
			bUseItem = TRUE;
		}

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
		{
			return E_Item_LevelLimit;
		}
	}

	// �ж�ͨ�����������ΪPK����״̬
	//SetRoleState(ERS_Safeguard);

	// �����ʹ������Ʒ�򿪣���Ҫ�ӱ�����ɾ����Ʒ
	if( bUseItem )
	{
		GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCID_Open_SafeGuard);
		SetRoleState(ERS_Safeguard);
		return E_Success;
	}

	return E_SafeGuard_OpenLimit;
}

//----------------------------------------------------------------------------------
// �ر�PK����
//----------------------------------------------------------------------------------
INT Role::CloseSafeGuard()
{
	// ����ǲ����Ѿ��Ǹ�״̬
	if( !IsInRoleState(ERS_Safeguard) ) return E_SafeGuard_Already;

	// ���ȼ��ȼ�����
	if( GetLevel() < SAFE_GUARD_FORCE_LEVEL )
		return E_SafeGuard_LevelLimit;

	// �ж�ͨ����������ҵ�PK����״̬
	UnsetRoleState(ERS_Safeguard);

	// ������ҹر�PK������ʱ��
	m_CloseSafeGuardTime = g_world.GetWorldTime();

	return E_Success;
}

//----------------------------------------------------------------------------------
// ��������״̬
//----------------------------------------------------------------------------------
INT Role::SetPK(BOOL bOnlyPK)
{
	if (bOnlyPK)
	{
		if (IsInRoleState(ERS_PKEX))
			return E_PKEX_Already;
	}
	else
	{
		if (IsInRoleState(ERS_PK))
			return E_PK_Already;
	}

	// �������ǲ��ǿ���PK����
	if( IsInRoleState(ERS_Safeguard) ) return E_PK_SafeGuard;

	// �ж�ͨ�����������Ϊ����״̬����������״̬���⣩
	if (bOnlyPK)
	{
		SetRoleState(ERS_PKEX);
		UnsetRoleState(ERS_PK);
	}
	else
	{
		SetRoleState(ERS_PK);
		UnsetRoleState(ERS_PKEX);
	}

	// ���ó�������״̬����ʱ
	m_nUnSetPKTickCountDown = UNSET_PK_TICK;
	
	OpenMacroToDealWithLuckState();

	return E_Success;
}

//----------------------------------------------------------------------------------
// ��������״̬
//----------------------------------------------------------------------------------
VOID Role::ResetPK()
{
	if( !IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) ) return;
	m_nUnSetPKTickCountDown = UNSET_PK_TICK;

	tagNS_PK send;
	send.dwErrorCode = E_Success;
	send.bReset = TRUE;
	SendMessage(&send, send.dwSize);
	
	CloseMacroToDealWithLuckState();
}

//----------------------------------------------------------------------------------
// ��������״̬
//----------------------------------------------------------------------------------
VOID Role::UpdatePK()
{
	if( !IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) ) return;

	if(--m_nUnSetPKTickCountDown <= 0)
	{
		UnsetRoleState(ERS_PK);
		UnsetRoleState(ERS_PKEX);
	}
}

//----------------------------------------------------------------------------------
// ����PVP״̬
//----------------------------------------------------------------------------------
VOID Role::SetPVP()
{
	if( !IsInRoleState(ERS_PVP) )
	{
		SetRoleState(ERS_PVP);
	}
	
	m_nUnSetPVPTickCountDown = UNSET_PVP_TICK;
}

//----------------------------------------------------------------------------------
// ����PVP״̬
//-----------------------------------------------------------------------------------
VOID Role::UpdatePVP()
{
	if( !IsInRoleState(ERS_PVP) ) return;

	if(--m_nUnSetPVPTickCountDown <= 0)
	{
		UnsetRoleState(ERS_PVP);
	}
}

//----------------------------------------------------------------------------------
// ����PK״̬
//----------------------------------------------------------------------------------
VOID Role::CalPKState(BOOL bSendMsg)
{
	ERolePKState ePKState = m_ePKState;
	INT n = 0;
	for(; n < ERPKS_End; n++)
	{
		if( m_nAtt[ERA_Morality] > PK_STATE_MORALITY[n] )
		{
			ePKState = (ERolePKState)n;
			break;
		}
	}
	ASSERT( n != ERPKS_End );

	// PK״̬�ı�
	if( ePKState != m_ePKState )
	{
		m_ePKState = ePKState;

		if( bSendMsg )
		{
			tagNS_RolePKStateChange send;
			send.dwRoleID = GetID();
			send.eState = ePKState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}
}

//----------------------------------------------------------------------------------
// ��������Ϊ0ʱ�سǸ����pk����
//----------------------------------------------------------------------------------
VOID Role::SetHostilitySafeGuard()
{
	// ����Ϊ0�سǸ���Ļ�����10���ӵ�pk����
	if ( GetAttValue(ERA_Hostility) == 0 )
	{
		m_nDeadUnSetSafeGuardCountDown = REVIVE_UNSETSAFEGUARD_TICK;
		SetRoleState(ERS_Safeguard);
	}
}

//----------------------------------------------------------------------------------
// ��������Ϊ0ʱ�سǸ����pk����
//----------------------------------------------------------------------------------
VOID Role::UpdateHostilitySafeGuard()
{
	// ��ҵȼ�����30��ʱ������
	if ( m_nLevel < 30 )	return;

	if( !IsInRoleState(ERS_Safeguard) ) return;

	if (m_nDeadUnSetSafeGuardCountDown < 0)
	{
		return;
	}
	else if (m_nDeadUnSetSafeGuardCountDown > 0)
	{
		--m_nDeadUnSetSafeGuardCountDown;
	}
	else
	{
		UnsetRoleState(ERS_Safeguard);
		m_nDeadUnSetSafeGuardCountDown = -1;
	}
}

//----------------------------------------------------------------------------------
// ����ı�
//----------------------------------------------------------------------------------
VOID Role::ExpChange(INT nValue, BOOL bKill, BOOL bForce)
{
	if( 0 == nValue ) return;

	// ���پ���
	if( nValue < 0 )
	{
		INT nTemp = m_nCurLevelExp;
		m_nCurLevelExp += nValue;
		if( m_nCurLevelExp < 0 ) m_nCurLevelExp = 0;

		tagNS_RoleExpChange send;
		send.nExp = m_nCurLevelExp;
		send.nChange = m_nCurLevelExp - nTemp;
		send.bKill = bKill;
		SendMessage(&send, send.dwSize);

		if (!bKill)
		{
			SaveExp2DB();
		}

		return;
	}

	// ��������Ӿ���
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_nLevel);
	INT nLevelUpExpRemain = pEffect->nExpLevelUp - m_nCurLevelExp;
	ASSERT( nLevelUpExpRemain >= 0 );

	// ��������
	if( nValue < nLevelUpExpRemain )
	{
		// ����Ƿ���10%�ı�����������ˣ������һ���Ѫ������
		INT nPhaseExp = pEffect->nExpLevelUp / 10;
		INT nOldPhase = m_nCurLevelExp / nPhaseExp;
		INT nNewPhase = (m_nCurLevelExp + nValue) / nPhaseExp;
		if( nOldPhase != nNewPhase )
		{
			SetAttValue(ERA_HP, GetAttValue(ERA_MaxHP));
			SetAttValue(ERA_MP, GetAttValue(ERA_MaxMP));
		}

		// �ۼ����µľ���
		m_nCurLevelExp += nValue;

		tagNS_RoleExpChange send;
		send.nExp = m_nCurLevelExp;
		send.nChange = nValue;
		send.bKill = bKill;
		SendMessage(&send, send.dwSize);
	}

	// ��Ҫ����
	else
	{
		// �����Ƿ�ǿ���������������ܵĵȼ�����
		INT nMaxLevel = ROLE_LEVEL_LIMIT;
		if( bForce )
		{
			nMaxLevel = MAX_ROLE_LEVEL;
		}

		m_nCurLevelExp += nLevelUpExpRemain;

		// �Ѿ���������
		if( m_nLevel == nMaxLevel )
		{
			if( nLevelUpExpRemain != 0 )
			{
				tagNS_RoleExpChange send;
				send.nExp = m_nCurLevelExp;
				send.nChange = nLevelUpExpRemain;
				send.bKill = bKill;
				SendMessage(&send, send.dwSize);
			}
		}

		// ��������
		else
		{
			INT nTemExpValue = nValue;
			INT nExpRealAdd = nLevelUpExpRemain;
			nValue -= nLevelUpExpRemain;
			INT nNextLevel = m_nLevel + 1;
			INT nAttPointAdd = 0;
			INT nTalentPointAdd = 0;

			// ��������������
			for(; nNextLevel <= nMaxLevel; nNextLevel++)
			{
				pEffect = g_attRes.GetLevelUpEffect(nNextLevel);

				// �������ټ�������
				if( pEffect->nExpLevelUp <= nValue )
				{
					nValue -= pEffect->nExpLevelUp;
					nExpRealAdd = pEffect->nExpLevelUp;
					nAttPointAdd += pEffect->n16RoleAttAvail;
					nTalentPointAdd += pEffect->n16TalentAvail;
				}
				// ��������������
				else
				{
					nAttPointAdd += pEffect->n16RoleAttAvail;
					nTalentPointAdd += pEffect->n16TalentAvail;
					break;
				}
			}

			// ������ı�ȼ�
			if( nNextLevel > nMaxLevel )
			{
				nNextLevel = nMaxLevel;
				// ��������һ��
				if( nValue > pEffect->nExpLevelUp )
				{
					nValue = pEffect->nExpLevelUp;
				}
			}

			LevelChange(nNextLevel, bKill, bForce);

			m_nCurLevelExp = nValue;
			nExpRealAdd += nValue;

			// ������������Ե�����ʵ�
			ModAttValue(ERA_AttPoint, nAttPointAdd);
			ModAttValue(ERA_TalentPoint, nTalentPointAdd);

			// ���;���ı�
			tagNS_RoleExpChange send;
			send.nExp = m_nCurLevelExp;
			send.nChange = nTemExpValue;//nExpRealAdd;
			send.bKill = bKill;
			SendMessage(&send, send.dwSize);

			SaveAttPoint2DB();
			SaveTalentPoint2DB(0);
			//m_SaveRoleLock.Acquire();
			//SaveToDB();
			//m_SaveRoleLock.Release();
		}
	}
	if (!bKill)
	{
		SaveExp2DB();
	}
}

//----------------------------------------------------------------------------
// �ı�ȼ�
//----------------------------------------------------------------------------
VOID Role::LevelChange(INT nValue, BOOL bKill, BOOL bForce)
{
	// �����Ƿ�ǿ��������������ߵȼ�
	INT nMaxLevel = ROLE_LEVEL_LIMIT;
	if( bForce ) nMaxLevel = MAX_ROLE_LEVEL;

	if( nValue < 0 || nValue > nMaxLevel )
		return;

	if( m_nLevel == nValue ) return;

	m_nLevel = nValue;
	m_nCurLevelExp = 0;

	// ���͵ȼ��ı�
	tagNS_RoleLevelChange send;
	send.dwRoleID = GetID();
	send.nLevel = m_nLevel;
	send.bKill = send.bKill;

	SaveLevel2DB();

	if( P_VALID(GetMap()) )
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}

	// �����С����,���͵ȼ��ı��¼�
	if(GetTeamID() != GT_INVALID)
		g_groupMgr.AddEvent(GetID(), EVT_ChangeLevel, send.dwSize, &send);

	// ͬ��tagRoleInfo
	g_socialMgr.AddEvent(GetID(), EVT_SynRoleLevel, send.dwSize, &send);

	// �ƺ��¼�����
	GetTitleMgr()->SigEvent(ETE_ROLE_LEVEL, GT_INVALID, m_nLevel);

	// ����������Լ�����
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_nLevel);
	SetBaseAttValue(ERA_Physique,	pEffect->n16RoleAtt[0] + m_nAttPointAdd[0]);
	SetBaseAttValue(ERA_Strength,	pEffect->n16RoleAtt[1] + m_nAttPointAdd[1]);
	SetBaseAttValue(ERA_Pneuma,		pEffect->n16RoleAtt[2] + m_nAttPointAdd[2]);
	SetBaseAttValue(ERA_InnerForce,	pEffect->n16RoleAtt[3] + m_nAttPointAdd[3]);
	SetBaseAttValue(ERA_Technique,	pEffect->n16RoleAtt[4] + m_nAttPointAdd[4]);
	SetBaseAttValue(ERA_Agility,	pEffect->n16RoleAtt[5] + m_nAttPointAdd[5]);

	// ����ĳЩ������������
	SetBaseAttValue(ERA_MaxHP,		pEffect->n16HP);
	SetBaseAttValue(ERA_MaxMP,		pEffect->n16MP);
	SetBaseAttValue(ERA_MaxVitality,pEffect->n16Vitality);

	RecalAtt();

	// ����Ѫ��������
	SetAttValue(ERA_HP, GetAttValue(ERA_MaxHP));
	SetAttValue(ERA_MP, GetAttValue(ERA_MaxMP));

	// �������30�����ر�pk����
	INT nRet = E_Success;
	if (m_nLevel >= 30)
	{
		if( !m_bLevelUpClosedSafeGuard )
		{
			nRet = CloseSafeGuard();

			// ������Ϣ���ͻ���
			tagNS_SafeGuard send;
			send.bOpen = FALSE;
			send.dwErrorCode = nRet;
			SendMessage(&send, send.dwSize);

			m_bLevelUpClosedSafeGuard = TRUE;
		}
		
	}

	if (bForce)
	{
		PlayerSession* pSession = GetSession();
		if (P_VALID(pSession) && !pSession->IsPrivilegeEnough(1))
		{
			tagNWL_ForbidAccount send;
			send.dwAccountID = pSession->GetSessionID();
			send.dwForbidCode = EPLM_Level;
			g_loginSession.Send(&send, send.dwSize);
		}
	}

	// ���������Ľű�����
	if( P_VALID(m_pScript))
	{
		m_pScript->OnRoleLevelChange(this);
	}
}

//-------------------------------------------------------------------------------
// Ͷ��
//-------------------------------------------------------------------------------
INT Role::BidAttPoint(const INT nAttPointAdd[X_ERA_ATTA_NUM])
{
	// �������Ƿ�Ϸ�
	INT nSum = 0;
	for(INT n = 0; n < X_ERA_ATTA_NUM; n++)
	{
		if( nAttPointAdd[n] < 0 )
			return E_BidAttPoint_PointInvalid;

		nSum += nAttPointAdd[n];
	}

	// ����ܵ���Ϊ0����ֱ�ӷ���
	if( nSum <= 0 ) return E_BidAttPoint_PointInvalid;

	// �����ҵ�ǰ���ϵĿ�Ͷ�����Ե㹻����
	if( GetAttValue(ERA_AttPoint) < nSum )
		return E_BidAttPoint_NoEnoughAttPoint;

	// �ȿ۳����Ե�
	ModAttValue(ERA_AttPoint, -nSum);

	// �����ζ�Ӧÿ�����Խ��мӳ�
	for(INT n = 0; n < X_ERA_ATTA_NUM; n++)
	{
		if( 0 == nAttPointAdd[n] ) continue;

		m_nAttPointAdd[n] += nAttPointAdd[n];
		ModBaseAttValue(MTransIndex2ERAATTA(n), nAttPointAdd[n]);
	}

	// ������������
	RecalAtt();

	// ����ÿ������Ͷ������ǰֵ���ͻ���
	tagNS_RoleAttPointAddChange send;
	IFASTCODE->MemCpy(send.nAttPointAdd, m_nAttPointAdd, sizeof(m_nAttPointAdd));
	SendMessage(&send, send.dwSize);

	SaveAttPoint2DB();

	return E_Success;
}

//----------------------------------------------------------------------------
// ϴ���Ե�
//----------------------------------------------------------------------------
INT Role::ClearAttPoint(INT64 n64ItemID, BOOL bItemCheck)
{
	// �ж���Ʒ�����ǲ���ϴ�����
	tagItem* pItem = GetItemMgr().GetBagItem(n64ItemID);
	if (bItemCheck)
	{
		if( !P_VALID(pItem) || ( pItem->pProtoType->eSpecFunc != EISF_RemoveAttPt ) )
			return E_ClearAttPoint_ItemNotValid;

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
			return E_Item_LevelLimit;

		// �ж�ͨ����ɾ����Ʒ
		GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCID_Clear_Att);
	}

	// ����������һ������Ͷ��
	INT nSum = 0;

	for(INT n = 0; n < X_ERA_ATTA_NUM; n++)
	{
		if( 0 == m_nAttPointAdd[n] ) continue;

		ModBaseAttValue(MTransIndex2ERAATTA(n), -m_nAttPointAdd[n]);
		nSum += m_nAttPointAdd[n];
		m_nAttPointAdd[n] = 0;
	}

	// ������������
	RecalAtt();

	// ���µ�ǰ��Ͷ�����Ե�
	ModAttValue(ERA_AttPoint, nSum);

	// ����ÿ������Ͷ������ǰֵ���ͻ���
	tagNS_RoleAttPointAddChange send;
	IFASTCODE->MemCpy(send.nAttPointAdd, m_nAttPointAdd, sizeof(m_nAttPointAdd));
	SendMessage(&send, send.dwSize);

	// ���Ե�����ݿ�
	SaveAttPoint2DB();

	return E_Success;
}

//-------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------
VOID Role::OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited)
{
	ASSERT( P_VALID(pSkill) && P_VALID(pSrc) );

	// �жϼ��ܵĵ���Ŀ������
	if( !pSkill->IsHostile() && !pSkill->IsFriendly() ) return;

	DWORD dwFriendEnemyFlag = pSrc->FriendEnemy(this);

	// ����ü��ܿ��ԶԵз�ʹ�ã�����˫�������з���ϵ
	if( pSkill->IsHostile() && this != pSrc && (ETFE_Hostile & dwFriendEnemyFlag) )
	{
		// ���������Ҳ����ң�������˫����PVP״̬������״̬
		if( pSrc->IsRole() )
		{
			Role* pSrcRole = static_cast<Role*>(pSrc);

			// ����˫����PVP״̬
			SetPVP();
			pSrcRole->SetPVP();

			// ����������������״̬
			ResetPK();

			// ���������������״̬������������ʱ��
			if( pSrcRole->IsInRoleState(ERS_PK) || pSrcRole->IsInRoleState(ERS_PKEX) )
			{
				pSrcRole->ResetPK();	
			}

			// �����Ҵ��ڷ�����״̬������ͻ��˷��;���
			if ( !IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) )
			{
				tagNS_RoleBeAttacked send;
				SendMessage(&send, send.dwSize);
			}
		}
		else if( pSrc->IsCreature() )
		{
			// ���﹥����ң�����һ����
			Creature* pSrcCreature = static_cast<Creature*>(pSrc);
			if( P_VALID(pSrcCreature->GetAI()) )
				pSrcCreature->GetAI()->AddEnmity(this, 1);
		}

		// ������ϵı������ܺ�װ����Buff����
		if( bHited )
		{
			OnPassiveSkillBuffTrigger(pSrc, ETEE_Hited, 0, 0);
			OnEquipmentBuffTrigger(NULL, ETEE_Hited);
			OnBuffTrigger(pSrc, ETEE_Hited, 0, 0);
			if( bBlock )
			{
				OnPassiveSkillBuffTrigger(pSrc, ETEE_Block, 0, 0);
				OnEquipmentBuffTrigger(NULL, ETEE_Block);
				OnBuffTrigger(pSrc, ETEE_Block, 0, 0);
			}
			if( bCrited )
			{
				OnPassiveSkillBuffTrigger(pSrc, ETEE_Crited, 0, 0);
				OnEquipmentBuffTrigger(NULL, ETEE_Crited);
				OnBuffTrigger(pSrc, ETEE_Crited, 0, 0);
			}

			// ��⵱ǰ���ܵĴ��
			GetCombatHandler().InterruptPrepare(CombatHandler::EIT_Skill, ESSTE_Default == pSkill->GetTypeEx());

			// ���ĳЩ��������ϵ�buff
			OnInterruptBuffEvent(EBIF_BeAttacked);
		}
		else
		{
			OnPassiveSkillBuffTrigger(pSrc, ETEE_Dodge, 0, 0);
			OnEquipmentBuffTrigger(NULL, ETEE_Dodge);
			OnBuffTrigger(pSrc, ETEE_Dodge, 0, 0);
		}
		OnPassiveSkillBuffTrigger(pSrc, ETEE_BeAttacked, 0, 0);
		OnEquipmentBuffTrigger(NULL, ETEE_BeAttacked);
		OnBuffTrigger(pSrc, ETEE_BeAttacked, 0, 0);
	}

	// ����ü��ܿ��Զ��ѷ�ʹ�ã�����˫�������ѷ���ϵ
	if( pSkill->IsFriendly() && (ETFE_Friendly & dwFriendEnemyFlag) )
	{
		// ���ֵ
		INT	nValue = pSkill->GetEnmity() + pSrc->GetAttValue(ERA_Enmity_Degree);

		// ������ϱ�����������Ĺ���ȫ�����ӳ��
		DWORD dwCreatureID = GT_INVALID;
		Creature* pCreature = (Creature*)GT_INVALID;
		m_mapEnmityCreature.ResetIterator();
		while( m_mapEnmityCreature.PeekNext(dwCreatureID) )
		{
			pCreature = GetMap()->FindCreature(dwCreatureID);
			if( !P_VALID(pCreature) ) continue;

			if( P_VALID(pCreature->GetAI()) )
				pCreature->GetAI()->AddEnmity(pSrc, nValue);
		}
	}
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID Role::OnDead(Unit* pSrc, Skill* pSkill/*=NULL*/, const tagBuffProto* pBuff/*=NULL*/, DWORD dwSerial, DWORD dwMisc)
{
	// ��������Ѿ���������ֱ�ӷ���
	if( IsInState(ES_Dead) ) return;

	// ����ʱ����
	OnPassiveSkillBuffTrigger(pSrc, ETEE_Die);
	OnEquipmentBuffTrigger(pSrc, ETEE_Die);
	OnBuffTrigger(pSrc, ETEE_Die);

	// ����Ϊ����״̬
	SetState(ES_Dead);

	// ֹͣ�ƶ�
	m_MoveData.StopMoveForce();

	// ֹͣ��ǰ�����ͷŵļ���
	m_CombatHandler.End();

	// �Ƴ�����������������ʧ��buff
	OnInterruptBuffEvent(EBIF_Die);

	// ��ճ���б�
	ClearEnmityCreature();

	// �ű��ж��Ƿ����װ������ͷ�
	BOOL bEquipLootPenalty = IsDeadPenalty(EDPM_EquipLoot);

	// �Ƿ��о���ͷ�
	BOOL bExpPenalty = IsDeadPenalty(EDPM_Exp);

	// �Ƿ������˳ͷ�
	BOOL bInjuryPenalty = IsDeadPenalty(EDPM_Injury);

	// �Ƿ��������ͷ�
	BOOL bHostilityPenalty = IsDeadPenalty(EDPM_Hostility);

	// �Ƿ������������ͷ�
	BOOL bCommercePenalty = IsDeadPenalty(EDPM_Commerce);

	BOOL bEquip = FALSE;
	BOOL bExp = FALSE;
	BOOL bInjury = FALSE;
	BOOL bHostility = FALSE;
	BOOL bCommerce = FALSE;

	// �Ƿ�����������ͷ�
	CanCancelPenalty(pSrc, bEquip, bExp, bInjury, bHostility, bCommerce);
	
	if (bEquip)
		bEquipLootPenalty = FALSE;		// ���װ������ͷ�

	if (bExp)
		bExpPenalty = FALSE;			// �������ͷ�

	if (bInjury)
		bInjuryPenalty = FALSE;			// ������˳ͷ�

	if (bHostility)
		bHostilityPenalty = FALSE;		// ��������ͷ�

	if (bCommerce)
		bCommercePenalty = FALSE;		// ������������ͷ�


	// �����ͷ�
	if (bEquipLootPenalty)
		DeadPenalty(pSrc, EDPM_EquipLoot);

	if (bExpPenalty)
		DeadPenalty(pSrc, EDPM_Exp);

	if (bInjuryPenalty)
		DeadPenalty(pSrc, EDPM_Injury);

	if (bHostilityPenalty)
		DeadPenalty(pSrc, EDPM_Hostility);

	if (bCommercePenalty)
		DeadPenalty(pSrc, EDPM_Commerce);

	// ��ɱ��
	if( P_VALID(pSrc) )
	{
		pSrc->OnKill(this);
	}

	// ���õ�ͼ�¼�
	if( P_VALID(GetMap()) )
	{
		GetMap()->OnRoleDie(this, pSrc);
	}

	// ȡ������
	UnsetRoleState(ERS_PK);
	UnsetRoleState(ERS_PKEX);

	// ���ͽ�ɫ������Ϣ
	tagNS_RoleDead send;
	send.dwRoleID = GetID();
	send.dwSrcRoleID = (P_VALID(pSrc) ? pSrc->GetID() : GT_INVALID);
	send.eSuggestRevive = ERRT_None;
	if( P_VALID(pSkill) )
	{
		send.eCause = ERDC_Skill;
		send.dwMisc = pSkill->GetTypeID();
	}
	else if( P_VALID(pBuff) )
	{
		send.eCause = ERDC_Buff;
		send.dwMisc = pBuff->dwID;
	}
	else
	{
		send.eCause = ERDC_Other;
		send.dwMisc = GT_INVALID;
	}

	if (P_VALID(pSrc) && pSrc->IsRole() && ERPKS_Wanted == GetPKState())
	{
		// ��Ҫ��������
		m_bNeedPrisonRevive = TRUE;
		// ��������
		send.eSuggestRevive = ERRT_Prison;

		tstringstream tszMsg;
		TCHAR tszRoleName[X_SHORT_NAME] = {0};
		g_roleMgr.GetRoleNameByID(send.dwSrcRoleID, tszRoleName);
		tszMsg << tszRoleName << _T("Ϊ���������");
		g_roleMgr.GetRoleNameByID(send.dwRoleID, tszRoleName);
		tszMsg << tszRoleName << _T("�������");

		DWORD dwLen = sizeof(tagNS_AutoNotice) - sizeof(TCHAR) + 2*(tszMsg.str().length() + 1);

		MCREATE_MSG(pSendMsg, dwLen, NS_AutoNotice);

		pSendMsg->nCirInterval = 60;
		pSendMsg->nType = 0;
		_tcscpy(pSendMsg->szContent, tszMsg.str().c_str());

		g_roleMgr.SendWorldMsg(pSendMsg, dwLen);

		MDEL_MSG(pSendMsg);

	}
	send.dwMisc2 = dwMisc;
	send.dwSerial = dwSerial;

	if( P_VALID(GetMap()) )
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}
}

//---------------------------------------------------------------------------------
// ��ɱĿ��
//---------------------------------------------------------------------------------
VOID Role::OnKill(Unit* pTarget)
{
	ASSERT( P_VALID(pTarget) );

	// ��ɱ�������
	if( pTarget->IsRole() )
	{
		// �����������ܺ�װ��
		OnPassiveSkillBuffTrigger(pTarget, ETEE_Kill);
		OnEquipmentBuffTrigger(pTarget, ETEE_Kill);
		OnBuffTrigger(pTarget, ETEE_Kill);

		Role* pRole = static_cast<Role*>(pTarget);
		ERolePKState ePKState = pRole->GetPKState();

		// �Ƿ��е���ֵ�ͷ�
		if (IsDeadPenalty(EDPM_Morality))
		{
			INT nMoralityDec = 0;

			// ��ɱ�κ���pvp��̨�����ս������������״̬����ң����ı����ֵ
			if(!pRole->IsInRoleState(ERS_Arena) && !pRole->IsInRoleState(ERS_Hostility) )
			{
				// ��ɱ������״̬����ң��ı����ֵ-10, -5, -1
				if (!pRole->IsInRoleState(ERS_PK) && !pRole->IsInRoleState(ERS_PKEX))
				{
					if( ERPKS_Peace == ePKState )
						nMoralityDec =  -10;
					else if( ERPKS_Warnning == ePKState )
						nMoralityDec = -5;
					else if( ERPKS_Wicked == ePKState )
						nMoralityDec = -1;
				}
				
				// ��ɱ����״̬����ң��ı����ֵ-3, -1, 0
				else
				{
					if( ERPKS_Peace == ePKState )
						nMoralityDec =  -3;
					else if( ERPKS_Warnning == ePKState )
						nMoralityDec = -1;
					else if( ERPKS_Wicked == ePKState )
						nMoralityDec = 0;
				}
			}

			if( nMoralityDec != 0 )
				ModAttValue(ERA_Morality, nMoralityDec);
		}
		
		// �Ƿ�������ֵ�ͷ�
		if (IsDeadPenalty(EDPM_Hostility))
		{
			// ��ɱ�κβ���pvp��̨�����ս�����ȷ�����״̬Ҳ������״̬����ң��ı�����
			if( !pRole->IsInRoleState(ERS_PK) && !pRole->IsInRoleState(ERS_PKEX) && !pRole->IsInRoleState(ERS_Arena) && 
				pRole->GetAttValue(ERA_Hostility)==0 )
			{
				INT nAdd = 0;

				if (pRole->GetPKState() == ERPKS_Peace)
				{
					nAdd = 3;		// �����ɱ������ң���������3
				}
				else
				{
					nAdd = 1;		// �����ɱ������������������ң���������1
				}

				// �޸�����ֵǰ�Ƚ�������ϵ�����ֵ��Ӧbuffȥ���������һ���µ�buff
				INT nSrcHostility = GetAttValue(ERA_Hostility);
				if (nSrcHostility > 0)
				{
					RemoveBuff(Buff::GetIDFromTypeID( HOSTILITY_DEADPENALTY_BUFF_ID[nSrcHostility - 1]), TRUE );
				}

				// �ı��ɱ������ֵ
				SetAttValue( ERA_Hostility, min( GetAttValue(ERA_Hostility) + nAdd, MAX_HOSTILITY ) );

				// ��ӳͷ�buff
				nSrcHostility = GetAttValue(ERA_Hostility);
				if (nSrcHostility > 0)
				{
					const tagBuffProto* pProto = g_attRes.GetBuffProto( HOSTILITY_DEADPENALTY_BUFF_ID[nSrcHostility - 1] );
					if( P_VALID(pProto) )
					{
						TryAddBuff(this, pProto, NULL, NULL, NULL);
					}
				}
				
				
				if ( !this->IsInRoleState(ERS_Hostility) )
				{
					// ��ҳ�Ϊ����״̬
					SetRoleState(ERS_Hostility);
				}
			}

			// ����ɱ�����ȥ����Ӧ����ֵ��buff
			INT nTargetHostility = pRole->GetAttValue(ERA_Hostility);
			if (nTargetHostility > 0)
			{
				pRole->RemoveBuff(Buff::GetIDFromTypeID( HOSTILITY_DEADPENALTY_BUFF_ID[nTargetHostility - 1]), TRUE );
			}

			// ����ɱ��ң�����ֵ��1
			pRole->SetAttValue( ERA_Hostility, max( 0, pRole->GetAttValue(ERA_Hostility) - 1 ) );

			// ��ӳͷ�buff
			nTargetHostility = pRole->GetAttValue(ERA_Hostility);
			if (nTargetHostility > 0)
			{
				const tagBuffProto* pProto = g_attRes.GetBuffProto( HOSTILITY_DEADPENALTY_BUFF_ID[nTargetHostility - 1] );
				if( P_VALID(pProto) )
				{
					pRole->TryAddBuff(pRole, pProto, NULL, NULL, NULL);
				}
			}
			

			// �������ɱ�������ֵΪ0���Ҵ�������״̬����ȡ������ɱ�������״̬
			if ( pRole->GetAttValue(ERA_Hostility) == 0 && pRole->IsInRoleState(ERS_Hostility) )
			{
				pRole->UnsetRoleState(ERS_Hostility);
			}
		}

		// �㲥��ɱͨ�������Ϣ
		if (ERPKS_Wanted == ePKState)
		{
			tagNS_SendToPrison send;
			send.dwCaptorRoleID = GetID();
			send.dwCatchedRoleID = pRole->GetID();
			
			g_roleMgr.SendWorldMsg(&send, send.dwSize);
		}

		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_KILL_ROLE, GT_INVALID, GT_INVALID);
	}

	// �����ɱ���ǹ���
	else if( pTarget->IsCreature() )
	{
		Creature* pCreature = dynamic_cast<Creature*>(pTarget);
		if (!P_VALID(pCreature))
			return;

		// �����������ܺ�װ��
		if( !pCreature->IsGameObject() )
		{
			OnPassiveSkillBuffTrigger(pTarget, ETEE_Kill);
			OnEquipmentBuffTrigger(pTarget, ETEE_Kill);
			OnBuffTrigger(pTarget, ETEE_Kill);
		}

		// �ƺ����
		if (pCreature->IsNPC())
		{
			GetTitleMgr()->SigEvent(ETE_KILL_NPC, pCreature->GetTypeID(), GT_INVALID);
		}
		else if (pCreature->IsMonster())
		{
			GetTitleMgr()->SigEvent(ETE_KILL_MONSTER, pCreature->GetTypeID(), GT_INVALID);
		}
		else if (pCreature->IsBoss())
		{
			GetTitleMgr()->SigEvent(ETE_KILL_BOSS, pCreature->GetTypeID(), GT_INVALID);
		}
	}
}

//---------------------------------------------------------------------------------
// �Ƿ���������ͷ�
//---------------------------------------------------------------------------------
BOOL Role::IsDeadPenalty(EDeadPenaltyMode eMode)
{
	if(P_VALID(m_pScript))
		return m_pScript->IsDeadPenalty(this, eMode);

	return TRUE;
}

//---------------------------------------------------------------------------------
// �����ͷ�
//---------------------------------------------------------------------------------
VOID Role::DeadPenalty(Unit* pSrc, EDeadPenaltyMode eMode)
{
	// �������Ǽ���С�������ȼ��ͷ��ĵȼ����ޣ������κ������ͷ�
	if( GetLevel() < DEAD_PENALTY_LEVEL ) return;

	// ���������PVP���������򲻻��ܵ��κ������ͷ�
	if( IsInRoleState(ERS_PVPArea) ) return;

	switch(eMode)
	{
	case EDPM_EquipLoot:
		{
			// װ������ͷ���PK������Ҳ��ܳͷ���������һ�ɱҲ����װ������ĳͷ�
			if( !IsInRoleState(ERS_Safeguard) && P_VALID(pSrc) && pSrc->IsRole() )
			{
				INT nLootProp = LOOT_RATE_PER_PK_STATE[m_ePKState];
				if( nLootProp > 0 &&  IUTIL->Rand() % 10000 < nLootProp )
				{
					// 80%�ļ��ʵ���1��װ�������
					// 15%�ļ��ʵ���2��װ�������
					// 4%�ļ��ʵ���3��װ�������
					// 0.8%�ļ��ʵ���4��װ�������
					// 0.2%�ļ��ʵ���5��װ�������
					INT nNum = 0;
					INT nProp = IUTIL->Rand() % 10000;

					if( nProp < 8000 )			nNum = 1;
					else if( nProp < 9500 )		nNum = 2;
					else if( nProp < 9900 )		nNum = 3;
					else if( nProp < 9980 )		nNum = 4;
					else						nNum = 5;

					// ���ݸ����ӱ�������װ�����е�����Ʒ
					TList<tagItem*> listLootItems;
					TList<tagEquip*> listLootEquips;

					// �漴ȷ��ɾ����װ������Ʒ����
					INT nEquip = IUTIL->Rand() % (nNum + 1);
					INT nItem = nNum - nEquip;

					// ����װ��
					GetItemMgr().LootFromEquipBar(nEquip, listLootEquips, ELCID_Dead_Penalty);

					// ���ɵ���װ��
					tagEquip* pEquip = NULL;
					TList<tagEquip*>::TListIterator iterEquip = listLootEquips.Begin();
					while (listLootEquips.PeekNext(iterEquip, pEquip))
					{
						if (!P_VALID(pEquip))
						{
							continue;
						}

						if( GetMap() )
						{
							tagGroundItem* pGroundItem = new tagGroundItem(GetMap()->GenGroundItemID(), pEquip->dwTypeID, 
								pEquip->n16Num, pEquip, GetCurPos(), GT_INVALID, 
								GT_INVALID, 0, GetID());

							ASSERT(pGroundItem->IsValid());

							GetMap()->AddGroundItem(pGroundItem);
						}
						else
						{
							Destroy(pEquip);
						}
					}

					// ������Ʒ
					GetItemMgr().LootFromBag(nItem, listLootItems, ELCID_Dead_Penalty);

					// ���ɵ�����Ʒ
					tagItem* pItem = NULL;
					TList<tagItem*>::TListIterator iterItem = listLootItems.Begin();
					while (listLootItems.PeekNext(iterItem, pItem))
					{
						if (!P_VALID(pItem))
						{
							continue;
						}

						if( GetMap() )
						{
							tagGroundItem* pGroundItem = new tagGroundItem(GetMap()->GenGroundItemID(), pItem->dwTypeID, 
								pItem->n16Num, pItem, GetCurPos(), GT_INVALID, 
								GT_INVALID, 0, GetID());

							ASSERT(pGroundItem->IsValid());

							GetMap()->AddGroundItem(pGroundItem);
						}
						else
						{
							Destroy(pItem);
						}
					}
				}
			}
		}
		break;

	case EDPM_Exp:
		{
			// ����ͷ���ֻ������״̬����ҲŻ���ʧ���飩
			if( IsInRoleState(ERS_PK) || IsInRoleState(ERS_PKEX) )
			{
				// ֻ�б���һ�ɱ������²Ż���ʧ����
				if( P_VALID(pSrc) && pSrc->IsRole() )
				{
					// ��ʧ�ľ��飺[150/(��ɫ�ȼ�^2)] %����������
					const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(GetLevel());
					ASSERT( P_VALID(pEffect) );
					INT level = GetLevel();
					INT nExp = FLOAT(pEffect->nExpLevelUp / level) / level * 150 ;
					ExpChange(-nExp);
				}
			}
		}

		break;

	case EDPM_Injury:
		{
			// ���˳ͷ�
			// 29%���ʲ�������
			// 30%��������+1
			// 20%��������+2
			// 15%��������+3
			// 5%��������+5
			// 1%��������+10
			INT nInjury = 0;
			INT nProp = IUTIL->Rand() % 100;

			if( nProp < 29 )			nInjury = 0;
			else if( nProp < 59 )		nInjury = 1;
			else if( nProp < 79 )		nInjury = 2;
			else if( nProp < 94 )		nInjury = 3;
			else if( nProp < 99 )		nInjury = 5;
			else						nInjury = 10;

			if( nInjury != 0 )
				ModAttValue(ERA_Injury, nInjury);

		}
		break;

	case EDPM_Hostility:
		{
			// ����״̬���������õ����Գͷ�
			/*if (IsInRoleState(ERS_Hostility))
			{
				const tagBuffProto* pProto = g_attRes.GetBuffProto(HOSTILITY_DEADPENALTY_BUFF_ID);
				if( P_VALID(pProto) )
				{
					TryAddBuff(this, pProto, NULL, NULL, NULL);
				}
			}*/
		}
		break;

	case EDPM_Commerce:
		{
			// ���������ͷ�
			Guild* pGuild = g_guildMgr.GetGuild(GetGuildID());
			if (!P_VALID(pGuild))				return;
			if (!IsInRoleState(ERS_Commerce))	return;
			GuildCommodity* pCommodity = pGuild->GetGuildCommerce().GetCommodity(GetID());
			if (!P_VALID(pCommodity))			return;
			pCommodity->DeadPenalty();
		}
		break;

	default:
		break;
	}

	
	
}

//---------------------------------------------------------------------------------
// �Ƿ�����������ͷ�
//---------------------------------------------------------------------------------
VOID Role::CanCancelPenalty(Unit* pSrc, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommercePenalty)
{
	if( !P_VALID(pSrc) ) return;
	if (pSrc->IsRole())
	{
		if(P_VALID(m_pScript))
		{
			m_pScript->CanCancelPenalty(this, pSrc, bEquip, bExp, bInjury, bHostility, bCommercePenalty);
		}
	}
}

//---------------------------------------------------------------------------------
// Ŀ������ͱ�־
//---------------------------------------------------------------------------------
DWORD Role::TargetTypeFlag(Unit* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;

	if( pTarget->IsRole() )
		return TargetTypeFlag(static_cast<Role*>(pTarget));
	else if( pTarget->IsCreature() )
		return TargetTypeFlag(static_cast<Creature*>(pTarget));
	else
		return 0;
}

DWORD Role::TargetTypeFlag(Role* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;

	// ���Ŀ������Լ����򷵻�����
	if( this == pTarget ) return ETF_Self;

	DWORD dwFlag = 0;

	// todo�������Ҹ��Լ��ǲ���ĳЩ����ϵ������ǣ�����������ص�ĳЩ�ֶ�

	// С�Ӷ���
	if( IsTeamMate(pTarget) )
	{
		dwFlag |= ETF_Teammate;
	}

	// ��Ȼ����ĳЩ����ϵ����Ҳ�������
	dwFlag |= ETF_Player;

	return dwFlag;
}

DWORD Role::TargetTypeFlag(Creature* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;

	DWORD dwFlag = 0;

	// ����
	if( pTarget->IsMonster() )
	{
		// ��ͨ����
		if( pTarget->IsNormalMonster() )
		{
			if( !pTarget->IsBoss() )
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
		if ( pTarget->IsManRes() )
			dwFlag |= ETF_ManRes;
		else if ( pTarget->IsNatuRes() )
			dwFlag |= ETF_NatuRes;
		else
			ASSERT(0);
	}

	// �ɵ������
	else if( pTarget->IsInves() )
	{
		dwFlag |= ETF_InvesGameObject;
	}
	// ��
	else if (pTarget->IsDoor())
	{
		dwFlag |= ETF_Door;
	}

	return dwFlag;
}

//---------------------------------------------------------------------------------
// ��Ŀ��ĵ��������ж�
//---------------------------------------------------------------------------------
DWORD Role::FriendEnemy(Unit* pTarget)
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


DWORD Role::FriendEnemy(Role* pTarget)
{
	DWORD dwFlag = 0;

	// ��ͨ��Դ����Ŀ�귽֮��Ĺ�ϵȷ�����ҹ�ϵ
	if( IsFriendlySocial(pTarget) )
	{
		if( !pTarget->IsInRoleState(ERS_PK) && !this->IsInRoleState(ERS_PK) )
		{
			// ˫������ĳЩ����ϵ������˫��������������״̬�����ѷ�
			dwFlag |= ETFE_Friendly;
		}
		else if( pTarget->IsInRoleState(ERS_PK) && this->IsInRoleState(ERS_PK) )
		{
			// ˫������ĳЩ����ϵ������˫������������״̬�����ѷ�
			dwFlag |= ETFE_Friendly;
		}
		else
		{
			// ˫������ĳЩ����ϵ������ֻ����һ����������״̬�����ѷ�
			dwFlag |= ETFE_Friendly;
		}
	}
	else
	{
		// ����Լ����ó�Ϊ����״̬����Ϊ�з�
		if( IsInRoleState(ERS_PK) )
		{
			dwFlag |= ETFE_Hostile;
		}

		// ����Լ�Ϊ����״̬��ֻ��������ң������ҶԷ�ҲΪ����״̬����Ϊ�з�
		else if (IsInRoleState(ERS_PKEX) && (pTarget->IsInRoleState(ERS_PK) || pTarget->IsInRoleState(ERS_PKEX)) )
		{
			dwFlag |= ETFE_Hostile;
		}

		else
		{
			// ����Ϊ����
			dwFlag |= ETFE_Independent;
		}

		// ����Լ����ڷ�����״̬����Ϊ�з�
		if (!IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX))
		{
			if ( dwFlag & ETFE_Hostile )
			{
				dwFlag ^= ETFE_Hostile;
			}
		}
		
	}

	// �ٿ���һ��PK������ص�����
	if( pTarget->IsInRoleState(ERS_Safeguard) )
	{
		// �Կ���PK��������Ҳ��ܽ���PK
		if( dwFlag & ETFE_Hostile )
		{
			dwFlag ^= ETFE_Hostile;
		}
	}
	if( IsInRoleState(ERS_Safeguard) )
	{
		// �Լ�����PK���������ܶ����˽���PK
		if( dwFlag & ETFE_Hostile )
		{
			dwFlag ^= ETFE_Hostile;
		}

		// ���ܶԵ�ǰ����PVP״̬�µ��ѷ���ҽ���������ʽ
		if( (dwFlag & ETFE_Friendly) && pTarget->IsInRoleState(ERS_PVP) )
		{
			dwFlag ^= ETFE_Friendly;
		}
	}

	// ��ȫ��
	//if( IsInRoleState(ERS_SafeArea) || pTarget->IsInRoleState(ERS_SafeArea) )
	//{
	//	if( dwFlag & ETFE_Hostile )
	//	{
	//		dwFlag ^= ETFE_Hostile;
	//	}
	//}

	// PVP����
	if( IsInRoleState(ERS_PVPArea) && pTarget->IsInRoleState(ERS_PVPArea) )
	{
		dwFlag |= ETFE_Hostile;
	}

	return dwFlag;
}

DWORD Role::FriendEnemy(Creature* pCreature)
{
	DWORD dwFlag = 0;

	// ����
	if( pCreature->IsMonster() )
	{
		dwFlag |= ETFE_Hostile;
	}

	// NPC
	else if( pCreature->IsNPC() )
	{
		if( IsInRoleState(ERS_PK) || IsInRoleState(ERS_PKEX) )
		{
			dwFlag |= ETFE_Hostile;
		}
		else
		{
			dwFlag |= ETFE_Friendly;
		}
	}

	// ����
	else if( pCreature->IsGameObject() )
	{
		if( pCreature->IsRes() 
			|| pCreature->IsInves())
		{
			dwFlag |= ETFE_Independent;
		}
		else if (pCreature->IsDoor())
		{
			dwFlag |= ETFE_Hostile;
		}
		else
		{
			// todo:ʣ�µ�һЩ����Ŀǰ�Ȳ����ǣ������޷������͵�
		}
	}

	// ����
	else if( pCreature->IsPet() )
	{

	}

	return dwFlag;
}

//------------------------------------------------------------------------------
// ע�ᴥ�������ܹ�����
//------------------------------------------------------------------------------
VOID Role::RegisterTriggerSkillSet(ETriggerEventType eEvent, DWORD dwSkillID)
{
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return;

	std::set<DWORD>& skillSet = m_setPassiveSkillTrigger[eTriggerType];

	skillSet.insert(dwSkillID);
}

//----------------------------------------------------------------------------
// ��ע�ᴥ�������ܹ�����
//----------------------------------------------------------------------------
VOID Role::UnRegisterTriggerSkillSet(ETriggerEventType eEvent, DWORD dwSkillID)
{
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return;

	std::set<DWORD>& skillSet = m_setPassiveSkillTrigger[eTriggerType];

	skillSet.erase(dwSkillID);
}

//----------------------------------------------------------------------------------
// װ�����buffԤ��������ȡtigger����
//----------------------------------------------------------------------------------
EPassiveSkillAndEquipTrigger Role::PreRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, BOOL bEquip)
{
	const tagTriggerProto* pProto = g_attRes.GetTriggerProto(dwTriggerID);
	if( !P_VALID(pProto) )
	{
		MAttResCaution(_T("trigger proto"), _T("TriggerID"), dwTriggerID);
		return EPSAET_Null;
	}

	// �Ƿ���װ����trigger
	if( ETEE_Equip == pProto->eEventType )
	{
		ProcEquipBuffTrigger(dwBuffID, bEquip);
		return EPSAET_Null;
	}

	return TriggerTypeToPassiveSkillAndEquipTriggerType(pProto->eEventType);
}

//----------------------------------------------------------------------------
// �������ܴ���������
//----------------------------------------------------------------------------
BOOL Role::OnPassiveSkillBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, 
		DWORD dwEventMisc1/* =GT_INVALID */, DWORD dwEventMisc2/* =GT_INVALID */)
{
	// ���ȸ����¼������жϳ��������ܶ�Ӧ�Ĵ���������
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return FALSE;

	// �ҵ��Ƿ��б������ܶԸ�trigger���ͽ�����ע��
	if( m_setPassiveSkillTrigger[eTriggerType].empty() ) return FALSE;

	// һ������һ�����ܵ���ѯ
	std::set<DWORD>& skillSet = m_setPassiveSkillTrigger[eTriggerType];
	for(std::set<DWORD>::iterator it = skillSet.begin(); it != skillSet.end(); it++)
	{
		// �ҵ�����
		DWORD dwSkillID = *it;
		Skill* pSkill = GetSkill(dwSkillID);
		if( !P_VALID(pSkill) ) continue;
		if( pSkill->GetCoolDownCountDown() > 0 )	continue;

		// todo: ����ط���Ҫ�жϱ������ܵ����ƣ�combathandler�����ټ�һ���жϱ��������ܷ�ʹ�õ��ж�

		BOOL bSelfUseble = FALSE;			// �����ܷ�����
		BOOL bTargetUseble = FALSE;			// Ŀ���ܷ�����

		if( E_Success == m_CombatHandler.CheckTargetLogicLimit(pSkill, pTarget) )
		{
			bTargetUseble = TRUE;
		}
		if( E_Success == m_CombatHandler.CheckSkillLimit(pSkill) )
		{
			bSelfUseble = TRUE;
		}

		// �����Ŀ�궼�������ã���ֱ�ӷ���
		if( FALSE == bSelfUseble && bTargetUseble == FALSE )
			continue;

		// ��鼼������skillbuff���������Ա��е�buffid
		for(INT n = 0; n < MAX_BUFF_PER_SKILL; n++)
		{
			DWORD dwBuffTypeID = pSkill->GetBuffTypeID(n);
			DWORD dwTriggerID = pSkill->GetTriggerID(n);
			if( !P_VALID(dwBuffTypeID) || !P_VALID(dwTriggerID) ) continue;

			const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
			const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
			if( !P_VALID(pBuffProto) || !P_VALID(pTriggerProto) ) continue;

			// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
			if( ETEE_Null != pTriggerProto->eEventType && eEvent != pTriggerProto->eEventType )
				continue;

			// ���Ŀ��������ã���������Ŀ��
			if( bTargetUseble )
				pTarget->TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);

			// �������������ã�������������
			if( bSelfUseble )
				TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);

			StartSkillCoolDown(pSkill);
		}
	}

	return TRUE;
}

//------------------------------------------------------------------------------
// ʹ����Ʒ״̬����
//------------------------------------------------------------------------------
BOOL Role::OnActiveItemBuffTrigger(tagItem* pItem, ETriggerEventType eEvent, 
					DWORD dwEventMisc1/*=GT_INVALID*/, DWORD dwEventMisc2/*=GT_INVALID*/)
{
	// ��Ʒ�Ƿ����
	if( !P_VALID(pItem)) return FALSE;

	for(INT n = 0; n < MAX_BUFF_PER_ITEM; ++n)
	{
		DWORD dwBuffTypeID = pItem->GetBuffID(n);
		DWORD dwTriggerID = pItem->GetTriggerID(n);
		if( !P_VALID(dwBuffTypeID) || !P_VALID(dwTriggerID) ) continue;

		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
		const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
		if( !P_VALID(pBuffProto) || !P_VALID(pTriggerProto) ) continue;

		// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
		if( ETEE_Null != pTriggerProto->eEventType && eEvent != pTriggerProto->eEventType )
			continue;

		// ���Լ��Ƿ�����
		TryAddBuff(this, pBuffProto, pTriggerProto, NULL, pItem);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------
// װ����ر���trigger��������
//----------------------------------------------------------------------------------
VOID Role::OnEquipmentBuffTriggerCommon(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType)
{
	// װ������ͨbuff���� -- ��ѭװ����
	BitSetEquipPos& equipSet = m_bitsetEquipTrigger[eTriggerType];
	for(INT16 i=0; i<equipSet.size(); ++i)
	{
		if(!equipSet.test(i))
		{
			continue;
		}

		// �ҵ�װ��
		tagEquip *pEquip = GetItemMgr().GetEquipBarEquip(i);
		if(!P_VALID(pEquip))
		{
			// ִ�е��ô���˵��װ��buff��trigger����������
			ASSERT(0);
			IMSG(_T("\n\n\nCaution:\n"));
			IMSG(_T("\tTrigger&Buff of equip process maybe have problem! Please tell programmer to check!"));
			continue;
		}

		// �ҵ�װ����buff���������
		for(INT32 i=0; i<MAX_EQUIP_BUFF_NUM; ++i)
		{
			DWORD	dwBuffTypeID	=	GT_INVALID;
			DWORD	dwTriggerID		=	GT_INVALID;
			pEquip->GetTriggerIDBuffID(dwTriggerID, dwBuffTypeID, i);
			if(GT_INVALID == dwTriggerID || GT_INVALID == dwBuffTypeID)
			{
				continue;
			}

			const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
			if(!P_VALID(pTriggerProto))
			{
				MAttResCaution(_T("equip trigger or buff"), _T("equip typeid"), pEquip->dwTypeID);
				continue;
			}

			// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
			if(ETEE_Null == pTriggerProto->eEventType || eEvent != pTriggerProto->eEventType)
			{
				continue;
			}

			const tagBuffProto*	pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
			if(!P_VALID(pBuffProto))
			{
				MAttResCaution(_T("equip buff"), _T("equip typeid"), pEquip->dwTypeID);
				continue;
			}

			// ���Լ��Ƿ�����
			TryAddBuff(this, pBuffProto, pTriggerProto, NULL, pEquip);
		}
	}
}

//----------------------------------------------------------------------------------
// װ����ر���trigger��������
//----------------------------------------------------------------------------------
VOID Role::OnEquipmentBuffTriggerLongHun(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType)
{
	BitSetEquipPos& bitsetLongHun = m_bitsetLongHunTrigger[eTriggerType];
	for(INT16 i=0; i<bitsetLongHun.size(); ++i)
	{
		if(!bitsetLongHun.test(i))
		{
			continue;
		}

		// �ҵ���������ID
		DWORD dwLongHunID = m_LongHun.GetLongHunID(i);
		if(GT_INVALID == dwLongHunID)
		{
			ASSERT(0);
			IMSG(_T("\n\n\tLongHun reg/unreg maybe have problems! Please check!!!\n\n"));
		}

#ifdef	_DEBUG

		// �ҵ�װ��
		tagEquip *pEquip = GetItemMgr().GetEquipBarEquip(i);
		if(!P_VALID(pEquip))
		{
			// ִ�е��ô���˵��װ��buff��trigger����������
			ASSERT(0);
			IMSG(_T("\n\n\nCaution:\n"));
			IMSG(_T("\tTrigger&Buff of Longhun process maybe have problem! Please tell programmer to check!"));
		}
		else
		{
			DWORD	dwLongHunIDCheck	= GT_INVALID;
			BOOL	bOuter				= TRUE;

			pEquip->GetLongHunID(dwLongHunIDCheck, bOuter);
			if(dwLongHunIDCheck != dwLongHunID)
			{
				// ���������������м�¼��ʵ��װ���������
				ASSERT(0);
			}
		}

#endif

		// ��ȡ��������ԭ��
		const tagLongHunProto *pLongHunProto = g_attRes.GetLongHunProto(dwLongHunID);
		if(!P_VALID(pLongHunProto))
		{
			ASSERT(0);
			MAttResCaution(_T("LongHun"), _T("dwLongHunID"), dwLongHunID);
			continue;
		}

		if(GT_INVALID == pLongHunProto->dwTriggerID
			|| (GT_INVALID == pLongHunProto->dwBuffID1 && GT_INVALID == pLongHunProto->dwBuffID2))
		{
			ASSERT(0);
			MAttResCaution(_T("LongHun"), _T("dwLongHunID"), dwLongHunID);
			continue;
		}

		// trigger proto
		const tagTriggerProto* pTriggerProto	= g_attRes.GetTriggerProto(pLongHunProto->dwTriggerID);
		if(!P_VALID(pTriggerProto))
		{
			MAttResCaution(_T("longhun trigger"), _T("LongHunID"), dwLongHunID);
			continue;
		}

		// �鿴trigger�����Ƿ���ͬ�������
		if(ETEE_Null == pTriggerProto->eEventType || eEvent != pTriggerProto->eEventType)
		{
			ASSERT(0);
			IMSG(_T("\n\n\tLongHun reg/unreg maybe have problems! Please check!!!\n\n"));
			continue;
		}

		// buff proto
		if(pLongHunProto->dwBuffID1 != GT_INVALID)
		{
			const tagBuffProto*	pBuffProto	= g_attRes.GetBuffProto(pLongHunProto->dwBuffID1);
			if(!P_VALID(pBuffProto))
			{
				ASSERT(0);
				MAttResCaution(_T("LongHun Buff"), _T("LongHunID"), dwLongHunID);
			}
			else
			{
				// ���Լ��Ƿ�����
				TryAddBuff(this, pBuffProto, pTriggerProto, NULL, NULL);
			}
		}

		if(pLongHunProto->dwBuffID2 != GT_INVALID)
		{
			const tagBuffProto*	pBuffProto	= g_attRes.GetBuffProto(pLongHunProto->dwBuffID2);
			if(!P_VALID(pBuffProto))
			{
				ASSERT(0);
				MAttResCaution(_T("LongHun Buff"), _T("LongHunID"), dwLongHunID);
			}
			else
			{
				// ���Լ��Ƿ�����
				TryAddBuff(this, pBuffProto, pTriggerProto, NULL, NULL);
			}
		}
	}
}

//----------------------------------------------------------------------------------
// װ����ر���trigger��������
//----------------------------------------------------------------------------------
VOID Role::OnEquipmentBuffTriggerSuit(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType)
{
	SetSuitTrigger& suitSet = m_setSuitTrigger[eTriggerType];

	for(SetSuitTrigger::iterator it = suitSet.begin(); it != suitSet.end(); ++it)
	{
		// ��ȡ��װ�����������Ѽ���buff
		INT32 nSuitEquipNum = m_Suit.GetSuitEquipNum(*it);
		ASSERT(nSuitEquipNum >= MIN_SUIT_EQUIP_NUM);

		// ��װԭ��
		const tagSuitProto *pSuitProto = g_attRes.GetSuitProto(*it);
		if(!P_VALID(pSuitProto))
		{
			ASSERT(0);
			MAttResCaution(_T("suit"), _T("SuitID"), *it);
			continue;
		}

		if(nSuitEquipNum < pSuitProto->n8ActiveNum[0])
		{
			ASSERT(0);
			IMSG(_T("\n\n\tSuit trigger reg/unreg maybe have problems! Please check!!!\n\n"));
			continue;
		}

		for(INT32 i=0; i<MAX_SUIT_ATT_NUM; ++i)
		{
			if(nSuitEquipNum < pSuitProto->n8ActiveNum[i])
			{
				break;
			}

			if(GT_INVALID == pSuitProto->dwTriggerID[i] || GT_INVALID == pSuitProto->dwBuffID[i])
			{
				break;
			}

			const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(pSuitProto->dwTriggerID[i]);
			if(!P_VALID(pTriggerProto))
			{
				MAttResCaution(_T("suit trigger"), _T("suitid"), *it);
				continue;
			}

			// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
			if(ETEE_Null == pTriggerProto->eEventType || eEvent != pTriggerProto->eEventType)
			{
				continue;
			}

			const tagBuffProto*	pBuffProto = g_attRes.GetBuffProto(pSuitProto->dwBuffID[i]);
			if(!P_VALID(pBuffProto))
			{
				MAttResCaution(_T("suit buff"), _T("suitid"), *it);
				continue;
			}

			// ���Լ��Ƿ�����
			TryAddBuff(this, pBuffProto, pTriggerProto, NULL, NULL);
		}
	}
}

//----------------------------------------------------------------------------
// װ��״̬����
//----------------------------------------------------------------------------
BOOL Role::OnEquipmentBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, 
								  DWORD dwEventMisc1/* =GT_INVALID */, DWORD dwEventMisc2/* =GT_INVALID */)
{
	// ���ȸ����¼������жϳ��������ܶ�Ӧ�Ĵ���������
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return FALSE;

	// װ������ͨbuff����
	if( m_bitsetEquipTrigger[eTriggerType].any() )
	{
		OnEquipmentBuffTriggerCommon(eEvent, eTriggerType);
	}

	// ��������
	if(m_bitsetLongHunTrigger[eTriggerType].any())
	{
		OnEquipmentBuffTriggerLongHun(eEvent, eTriggerType);
	}

	// ��װ
	if(m_setSuitTrigger[eTriggerType].size() > 0)
	{
		OnEquipmentBuffTriggerSuit(eEvent, eTriggerType);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����ͼ����
//-----------------------------------------------------------------------------
VOID Role::CheckMapArea()
{
	DWORD dwRet = 0;
	if( GetMap() )
		dwRet = GetMap()->CheckArea(this);

	// ��ȫ��
	if( dwRet & ERS_SafeArea )
	{
		// ���밲ȫ����
		if( !IsInRoleState(ERS_SafeArea) ) 
		{
			// ������״̬��������ֵΪ0����ҽ��밲ȫ����Ϊ����ģʽ
			if (!IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) && 0 == GetAttValue(ERA_Hostility) && !IsInRoleState(ERS_Safeguard))
			{
				SetRoleState(ERS_Safeguard);
			}

			SetRoleState(ERS_SafeArea);
		}
	}
	else
	{
		// �뿪��ȫ����
		if( IsInRoleState(ERS_SafeArea) ) 
		{
			// 30�����ϲ���δ����ʮ����pk���������ڵ�����뿪��ȫ����ʱ��Ϊ����ģʽ
			if (m_nLevel >= SAFE_GUARD_FORCE_LEVEL && m_nDeadUnSetSafeGuardCountDown <= 0 && IsInRoleState(ERS_Safeguard))
			{
				UnsetRoleState(ERS_Safeguard);
			}

			UnsetRoleState(ERS_SafeArea);
		}
	}

	// PVP��
	if( dwRet & ERS_PVPArea )
	{
		if( !IsInRoleState(ERS_PVPArea) ) SetRoleState(ERS_PVPArea);
	}
	else
	{
		if( IsInRoleState(ERS_PVPArea) ) UnsetRoleState(ERS_PVPArea);
	}

	// ��̯��
	if( dwRet & ERS_StallArea )
	{
		if( !IsInRoleState(ERS_StallArea) ) SetRoleState(ERS_StallArea);
	}
	else
	{
		if( IsInRoleState(ERS_StallArea) ) UnsetRoleState(ERS_StallArea);
	}

	// ������
	if( dwRet & ERS_PrisonArea )
	{
		if( !IsInRoleState(ERS_PrisonArea) ) SetRoleState(ERS_PrisonArea);
	}
	else
	{
		if( IsInRoleState(ERS_PrisonArea) ) UnsetRoleState(ERS_PrisonArea);
	}


}

//------------------------------------------------------------------------------
// �������ø����
//------------------------------------------------------------------------------
DWORD Role::QuestSetRebornMap(UINT16 u16QuestID, DWORD &dwBornMapID, Vector3 &vBornPos)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	if(pMap->GetMapInfo()->eType != EMT_Normal)
	{
		ASSERT(pMap->GetMapInfo()->eType == EMT_Normal);
		return E_BindRebornMap_MapInvalid;
	}

	if(GetRebornMapID() == pMap->GetMapID())
	{
		return E_BindRebornMap_Already;
	}

	// �ж��Ƿ�������
	if (IsHaveQuest(u16QuestID) || IsHaveDoneQuest(u16QuestID))
	{
		// ����
		m_dwRebornMapID = pMap->GetMapID();
		dwBornMapID = m_dwRebornMapID;
		vBornPos = g_mapCreator.GetReBornPoint(m_dwRebornMapID);
		return E_Success;
	}

	return E_BindRebornMap_MapInvalid;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
DWORD Role::Revive(ERoleReviveType eType, INT64 n64ItemID, BOOL bNeedItem)
{
	if( !IsInState(ES_Dead) )
		return E_Revive_NotDead;	// �Ѿ���������״̬��

	if (m_bNeedPrisonRevive)		// �Ƿ���Ҫ��������
	{
		eType = ERRT_Prison;
		m_bNeedPrisonRevive = FALSE;
	}

	DWORD dwErrorCode = E_Success;
	
	// �жϸ�������
	switch(eType)
	{
	case ERRT_ReturnCity:
		dwErrorCode = CityRevive();
		break;
	case ERRT_Locus:
		dwErrorCode = LocusRevive(n64ItemID, bNeedItem);
		break;
	case ERRT_Accept:
		dwErrorCode = AcceptRevive();
		break;
	case ERRT_Prison:
		dwErrorCode = PrisonRevive();
		break;
	default:
		// ����ִ�е�����(�ϲ����������ж�)
		ASSERT(0);
		dwErrorCode = E_Revive_Unknown;
	}

	GetMap()->OnRevive(this, eType, m_Revive.nHP, m_Revive.nMP, m_Revive.fX, m_Revive.fY, m_Revive.fZ, m_Revive.dwMapID);

	// ����
	if(E_Success == dwErrorCode)
	{
		SetAttValue(ERA_HP, m_Revive.nHP);
		SetAttValue(ERA_MP, m_Revive.nMP);
		UnsetState(ES_Dead);
	}

	// ��ո�����ؼ�¼��Ϣ
	m_Revive.nHP = 0;
	m_Revive.nMP = 0;

	return dwErrorCode;
}

DWORD Role::CityRevive()
{
	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(m_dwRebornMapID);
	if(!P_VALID(pMapInfo))
	{
		ASSERT(P_VALID(pMapInfo));
		return E_Revive_MapNotFound;
	}
	
	// ���ø���㼰Ѫ����������
	m_Revive.nHP = GetAttValue(ERA_MaxHP);
	m_Revive.nMP = GetAttValue(ERA_MaxMP);
	m_Revive.fX = pMapInfo->vRebornPos.x;
	m_Revive.fY = pMapInfo->vRebornPos.y;
	m_Revive.fZ = pMapInfo->vRebornPos.z;
	m_Revive.dwMapID = m_dwRebornMapID;

	SetHostilitySafeGuard();

	return E_Success;
}

DWORD Role::LocusRevive(INT64 n64ItemID, BOOL bNeedItem)
{
	// �����Ʒ
	if (bNeedItem)
	{
		tagItem *pItem = GetItemMgr().GetBagItem(n64ItemID);
		if(!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_RevieveItem)
		{
			return E_Revive_ItemLimit;
		}
	
		// ɾ����Ʒ
		DWORD dwErrorCode = GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCLD_Revive_Locus);
		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}

		// ���ø���㼰Ѫ����������
		m_Revive.nHP = GetAttValue(ERA_MaxHP) / 4;
		m_Revive.nMP = GetAttValue(ERA_MaxMP) / 4;
	}
	else
	{
		// ���ø���㼰Ѫ����������
		m_Revive.nHP = GetAttValue(ERA_MaxHP);
		m_Revive.nMP = GetAttValue(ERA_MaxMP);
	}

	m_Revive.fX = m_MoveData.m_vPos.x;
	m_Revive.fY = m_MoveData.m_vPos.y;
	m_Revive.fZ = m_MoveData.m_vPos.z;
	m_Revive.dwMapID = GetMapID();

	return E_Success;
}

DWORD Role::AcceptRevive()
{
	// ���HP
	if(m_Revive.nHP <= 0)
	{
		return GT_INVALID;
	}
	
	return E_Success;
}

DWORD Role::PrisonRevive()
{
	DWORD	dwPrisonMapID	= g_mapCreator.GetPrisonMapID();
	Vector3	PutInPoint		= g_mapCreator.GetPutInPrisonPoint();

	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(dwPrisonMapID);
	if(!P_VALID(pMapInfo))
	{
		ASSERT(P_VALID(pMapInfo));
		return E_Revive_MapNotFound;
	}

	// ���ø���㼰Ѫ����������
	m_Revive.nHP = GetAttValue(ERA_MaxHP);
	m_Revive.nMP = GetAttValue(ERA_MaxMP);
	m_Revive.fX = PutInPoint.x;
	m_Revive.fY = PutInPoint.y;
	m_Revive.fZ = PutInPoint.z;
	m_Revive.dwMapID = dwPrisonMapID;

	return E_Success;
}

//-----------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------
VOID Role::BeRevived(INT nHP, INT nMP, Unit* pSrc)
{
	if( nHP <= 0 ) return;

	m_Revive.nHP = nHP;
	m_Revive.nMP = nMP;

	if( P_VALID(pSrc) && pSrc->GetMapID() == GetMapID() )
	{
		m_Revive.dwMapID = GetMapID();
		m_Revive.fX = pSrc->GetCurPos().x;
		m_Revive.fY = pSrc->GetCurPos().y;
		m_Revive.fZ = pSrc->GetCurPos().z;
	}
	else
	{
		m_Revive.dwMapID = GetMapID();
		m_Revive.fX = GetCurPos().x;
		m_Revive.fY = GetCurPos().y;
		m_Revive.fZ = GetCurPos().z;
	}


	// ���͸��ͻ���
	tagNS_RoleReviveNotify send;
	SendMessage(&send, send.dwSize);
}

DWORD Role::SendCloseStall()
{
	if(!IsInRoleStateAny(ERS_Stall | ERS_StallSet))
	{
		return E_Success;
	}
	
	tagNS_StallClose send;
	send.dwErrorCode	= CloseStall();
	send.dwStallRoleID	= GetID();
	if(P_VALID(GetMap()))
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}

	return send.dwErrorCode;
}

DWORD Role::UpdateFriOnline()
{
	INT				nFriendNum = GetFriendCount();
	DWORD			dwFriendID = GT_INVALID;
	tagRoleInfo*	pRoleInfo = (tagRoleInfo*)GT_INVALID;
	tagFriend*		pFriend = (tagFriend*)GT_INVALID;
	INT				nIndex = 0;
	DWORD			dwSize = sizeof(tagNS_UpdateFriState);
		
	if( nFriendNum > 0 )
	{
		dwSize += (nFriendNum-1) * sizeof(tagFriUpdate);
	}
	
	MCREATE_MSG(pSend, dwSize, NS_UpdateFriState);
		
	m_mapFriend.ResetIterator();
	while(m_mapFriend.PeekNext(dwFriendID, pFriend))
	{
		pRoleInfo = g_roleMgr.GetRoleInfo(dwFriendID);
		
		if(P_VALID(pRoleInfo))
		{
			pSend->FriUpdate[nIndex].dwRoleID = dwFriendID;
			pSend->FriUpdate[nIndex].bOnline = pRoleInfo->bOnline;
			pSend->FriUpdate[nIndex].nLevel = pRoleInfo->byLevel;
			++nIndex;
		}
		else
		{	
			tagNS_RoleCancelFriend send;
			send.dwDestRoleID = dwFriendID;
			send.byGroup = pFriend->byGroup;
			send.dwErrorCode = E_Success;
			SendMessage(&send, send.dwSize);

			pFriend->dwFriendID = GT_INVALID;
			pFriend->dwFriVal = 0;
			pFriend->byGroup = 1;
			m_mapFriend.Erase(dwFriendID);
		}
	}

	pSend->nNum = nIndex;
	if( nIndex > 0) 
	{
		pSend->dwSize = (nIndex - 1) * sizeof(tagFriUpdate) + sizeof(tagNS_UpdateFriState);
	}
	else
		pSend->dwSize = sizeof(tagNS_UpdateFriState);

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}

//--------------------------------------------------------------------------------
// ���Ըı�������������ݸı�
//--------------------------------------------------------------------------------
VOID Role::OnAttChange(INT nIndex)
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

	case ERA_Endurance:
		m_nAtt[ERA_Endurance] = min(m_nAtt[ERA_Endurance], m_nAtt[ERA_MaxEndurance]);
		break;

		// ����
	case ERA_Morality:
		{
			CalPKState();
		}
		break;

	default:
		break;
	}

	// �ƺ���������
	GetTitleMgr()->SigEvent(ETE_ROLE_ATT, nIndex, m_nAtt[nIndex]);
}

VOID Role::OnAttChange(bool bRecalFlag[ERA_End])
{
	// XZ�����ٶ�
	if( bRecalFlag[ERA_Speed_XZ] )
	{
		m_fXZSpeed = X_DEF_XZ_SPEED * (FLOAT(m_nAtt[ERA_Speed_XZ]) / 10000.0f);
		if( E_Success == m_MoveData.StopMove() )
		{
			// ���͸��Լ��ٶȸı�
			tagNS_MoveSpeedChange send;
			send.dwRoleID = m_dwID;
			send.curPos = m_MoveData.m_vPos;
			send.faceTo = m_MoveData.m_vFace;
			SendMessage(&send, send.dwSize);
		}
	}

	// Y�����ٶ�
	if( bRecalFlag[ERA_Speed_Y] )
	{
		m_fYSpeed = X_DEF_Y_SPEED * (FLOAT(m_nAtt[ERA_Speed_Y]) / 10000.0f);
	}

	// ��Ӿ�ٶ�
	if( bRecalFlag[ERA_Speed_Swim] )
	{
		m_fSwimXZSpeed = X_DEF_XZ_SPEED * (FLOAT(m_nAtt[ERA_Speed_Swim]) / 10000.0f);
		if( E_Success == m_MoveData.StopMove() )
		{
			// ���͸��Լ��ٶȸı�
			tagNS_MoveSpeedChange send;
			send.dwRoleID = m_dwID;
			send.curPos = m_MoveData.m_vPos;
			send.faceTo = m_MoveData.m_vFace;
			SendMessage(&send, send.dwSize);
		}
	}

	// ����ٶ�
	if( bRecalFlag[ERA_Speed_Mount] )
	{
		m_fMountXZSpeed = X_DEF_XZ_SPEED * (FLOAT(m_nAtt[ERA_Speed_Mount]) / 10000.0f);
		if( E_Success == m_MoveData.StopMove() )
		{
			// ���͸��Լ��ٶȸı�
			tagNS_MoveSpeedChange send;
			send.dwRoleID = m_dwID;
			send.curPos = m_MoveData.m_vPos;
			send.faceTo = m_MoveData.m_vFace;
			SendMessage(&send, send.dwSize);
		}
	}

	// ����
	if( bRecalFlag[ERA_Shape] )
	{
		FLOAT fCosf = FLOAT(m_nAtt[ERA_Shape]) / 10000.0f;
		m_Size.x = X_DEF_ROLE_SIZE_X * fCosf;
		m_Size.y = X_DEF_ROLE_SIZE_Y * fCosf;
		m_Size.z = X_DEF_ROLE_SIZE_Z * fCosf;
	}

	// �ӳ�ͳ�����ı�
	if(GetTeamID() != GT_INVALID && bRecalFlag[ERA_Rein])
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(GetTeamID());
		if( P_VALID(pTeam) && pTeam->IsLeader(GetID()) )
		{
			tagLeaderRein	LeaderRein;
			LeaderRein.nLeaderRein = GetAttValue(ERA_Rein);
			g_groupMgr.AddEvent(GetID(), EVT_ChangeRein, sizeof(tagLeaderRein), &LeaderRein);
		}
	}
}

VOID Role::OnLeaveMap( BOOL bChgMap /*= TRUE*/ )
{
	// �л���ͼ��ʧ��buff
	if (bChgMap)
	{
		// buff ���
		OnInterruptBuffEvent(EBIF_ChangeMap);

		// ���ִ��
		GetCombatHandler().InterruptPrepare(CombatHandler::EIT_Move, FALSE);

		// �ͷŴ��
		GetCombatHandler().InterruptOperate(CombatHandler::EIT_Move, EMS_Walk);
	}

	// ��ճ���б�
	ClearEnmityCreature();

	// �����̯
	SendCloseStall();

	// ������
	BreakMount();
	// �����ٻ�����
	GetPetPocket()->CalledPetEnterPocket();

}

VOID Role::OnEnterMap()
{
	m_pScript->OnRoleEnterMap(this);

	// �ͷ��ٻ�����
	GetPetPocket()->CalledPetLeavePocket();
	ContinueMount();

}

//--------------------------------------------------------------------------------
// ���ð�������
//--------------------------------------------------------------------------------
VOID Role::SetGuildID(DWORD dwGuildID)
{
	m_dwGuildID = dwGuildID;

	tagNDBC_ChangeRoleGuild send;
	send.dwRoleID	= GetID();
	send.dwGuildID	= dwGuildID;
	g_dbSession.Send(&send, send.dwSize);
}

//--------------------------------------------------------------------------------
// ���纰��
//--------------------------------------------------------------------------------
BOOL Role::TalkToWorld()
{
	if (m_nWorldTalkCounter < 0)
	{
		m_nWorldTalkCounter = WORLD_CHANNEL_INTERVAL;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//--------------------------------------------------------------------------------
// �������纰����ʱ
//--------------------------------------------------------------------------------
VOID Role::UpdateTalkToWorld()
{
	if (m_nWorldTalkCounter >= 0)
	{
		--m_nWorldTalkCounter;
	}
}

//--------------------------------------------------------------------------------
// ʰȡ��Ʒ
//--------------------------------------------------------------------------------
DWORD Role::PickUpItem( INT64 n64GroundID, BOOL bRole )
{
	if (bRole)
		StopMount();

	MTRANS_ELSE_RET(pMap, GetMap(), Map, E_Loot_Map_Not_Found);
	MTRANS_ELSE_RET(pGroundItem, pMap->GetGroundItem(n64GroundID), tagGroundItem, E_Loot_Item_Not_Found);
	
	DWORD dwRtv = E_Success;

	//ע��ѵ�����Ʒʰȡ��������,����tagGroundItem�е�pItemָ��ᱻ����,���ԷŽ�����Ӧ�������
	if (pGroundItem->dwTypeID == TYPE_ID_MONEY && 
		FALSE == GetCurMgr().IncBagSilver(pGroundItem->nNum, ELCLD_PickUp_Money)
		)
		return E_Loot_Add_Bag_Not_Success;
	//ע��ѵ�����Ʒʰȡ��������,����tagGroundItem�е�pItemָ��ᱻ����,���ԷŽ�����Ӧ�������
	else if (pGroundItem->dwTypeID != TYPE_ID_MONEY && 
		E_Success != (dwRtv = GetItemMgr().Add2Bag(pGroundItem->pItem, ELCLD_PickUp_Item, TRUE))
		)
		return dwRtv;
	else
	{
		// Jason 2009-12-4������䷽ʽ��©���ġ�
		Team::BroadcastLootItemMsgInTeamExceptPicker(this,pGroundItem->pItem);
		//������Ʒ��ʧ��Ϣ
		tagNS_RoleGroundItemDisappear disappear;
		disappear.n64Serial[0] = n64GroundID;
		pMap->SendBigVisTileMsg(this, &disappear, disappear.dwSize);

		//��Map��ɾ��������Ʒ
		//��������Ʒ�ӵ���ɾ��
		pMap->RemoveGroundItem(pGroundItem);

		//��ָ��ָ�ΪNULL
		pGroundItem->pItem=NULL;
		SAFE_DEL(pGroundItem);
	}

	return E_Success;
}

//--------------------------------------------------------------------------------
// ������Ʒ
//--------------------------------------------------------------------------------
DWORD Role::PutDownItem( INT64 n64Serial )
{

	MTRANS_ELSE_RET(pMap, GetMap(), Map, GT_INVALID);

	DWORD dwRet = E_Success;
	tagItem *pItem = NULL;

	;
	if( E_Success != (dwRet = GetItemMgr().DiscardFromBag(n64Serial, ELCLD_Role_Discard_Item, pItem)))
	{
		return dwRet;
	}

	// todo������ط�����һ�¸߶ȣ��������һ��İ취
	if(P_VALID(pItem))
	{
		tagGroundItem* pGroundItem = new tagGroundItem(pMap->GenGroundItemID(), 
													pItem->dwTypeID, pItem->n16Num, pItem, 
													GetCurPos(), GT_INVALID, GT_INVALID, 0, GetID());

		ASSERT(pGroundItem->IsValid());

		pMap->AddGroundItem(pGroundItem);
	}
	return E_Success;
}

VOID Role::SetMount( BOOL bSet, INT nMountSpeed, const tagPetProto* pPetProto )
{
	if (bSet)
	{
		// ��������״̬
		SetRoleState(ERS_Mount);

		// ������ײ��
		SetSize(pPetProto->vSize);

		// ���������ٶ�
		ModAttModValue(ERA_Speed_Mount, nMountSpeed);
	}
	else
	{
		// ��������״̬
		UnsetRoleState(ERS_Mount);

		// ������ײ��
		SetAttRecalFlag(ERA_Shape);

		// ���������ٶ�
		ModAttModValue(ERA_Speed_Mount, -nMountSpeed);
	}

	RecalAtt();	
}

FLOAT Role::GetVNBExpRate()
{
	return GetSession()->GetVNBExpRate() / 10000.0f;
}

FLOAT Role::GetVNBLootRate()
{
	return GetSession()->GetVNBLootRate() / 10000.0f;
}

//--------------------------------------------------------------------------------
// ���ӽ�ɫ����������
//--------------------------------------------------------------------------------
VOID Role::IncTreasureSum()
{
	++m_nTreasureSum;
	if (m_nTreasureSum > ROLE_CHSET_RATE)
		m_nTreasureSum = 1;

	tagNDBC_UpdateTreasureSum send;
	send.dwRoleID	= GetID();
	send.nSum	= m_nTreasureSum;
	g_dbSession.Send(&send, send.dwSize);
	
}

//--------------------------------------------------------------------------------
// ���ý�ɫ����������
//--------------------------------------------------------------------------------
VOID Role::SetTreasureSum(INT nSum)
{
	if (nSum > ROLE_CHSET_RATE)
		nSum = 1;

	m_nTreasureSum = nSum;
	tagNDBC_UpdateTreasureSum send;
	send.dwRoleID	= GetID();
	send.nSum	= m_nTreasureSum;
	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::StopMount()
{
	RemoveBuff(Buff::GetIDFromTypeID(MOUNT2_BUFF_ID), TRUE);
	RemoveBuff(Buff::GetIDFromTypeID(MOUNT_BUFF_ID), TRUE);
}

VOID Role::SetPassenger( Role* pHost )
{
	if (P_VALID(pHost))
	{
		SetBaseAttValue(ERA_Speed_Mount, pHost->GetAttValue(ERA_Speed_Mount));
	}
	else
	{
		SetBaseAttValue(ERA_Speed_Mount, 10000);
	}
	
	RecalAtt(TRUE);

	return ;
}


VOID Role::ContinueMount()
{
	PetSoul* pSoul = GetPetPocket()->GetPreparingPetSoul();
	if (P_VALID(pSoul) && GetPetPocket()->GetKeepMount() && pSoul->CanSetMount(TRUE))
	{
		TryAddBuff(this, g_attRes.GetBuffProto(MOUNT_BUFF_ID), NULL, NULL, NULL);
		GetPetPocket()->SetKeepMount(FALSE);
	}
	
}

void Role::BreakMount()
{
	if (IsInRoleStateAny(ERS_Mount | ERS_Mount2))
	{
		GetPetPocket()->SetKeepMount(TRUE);
		StopMount();
	}
}

//----------------------------------------------------------------------------------------
// ���ָ����װ������װ��������(GM�������)
//----------------------------------------------------------------------------------------
DWORD Role::AddSuit(DWORD dwSuitID, INT nQlty)
{
	TMap<DWORD, tagEquipProto*> mapEquip = g_attRes.GetEquipMap();
	mapEquip.ResetIterator();
	
	DWORD dwEquipID = 0;
	tagEquipProto* pEquipProto = NULL;

	while ( mapEquip.PeekNext(dwEquipID, pEquipProto) )
	{
		for(INT i=0; i<MAX_PEREQUIP_SUIT_NUM; ++i)
		{
			if(dwSuitID == pEquipProto->dwSuitID[i])
			{
				tagItem *pItem = ItemCreator::Create(EICM_GM, NULL, dwEquipID, 1);
				if(!P_VALID(pItem)) return GT_INVALID;
				ASSERT( MIsEquipment(pItem->dwTypeID) );

				ItemCreator::IdentifyEquip((tagEquip*)pItem, (EItemQuality)nQlty);
				GetItemMgr().Add2Bag(pItem, ELCID_GM_CreateItem, TRUE);

				//m_Suit.Add((tagEquip*)pItem, pItem->n16Index);
			}
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------
// ���ָ�����������������(GM�������) nType 0:���� 1:����
//----------------------------------------------------------------------------------------
DWORD Role::AddEquip(INT nType, INT nLevel, INT nQlty)
{
	TMap<DWORD, tagEquipProto*> mapEquip = g_attRes.GetEquipMap();
	mapEquip.ResetIterator();

	DWORD dwEquipID = 0;
	tagEquipProto* pEquipProto = NULL;

	while ( mapEquip.PeekNext(dwEquipID, pEquipProto) )
	{
		if(0 == nType)
		{
			if (pEquipProto->eType != 7) continue;
		}
		else
		{
			if (!(pEquipProto->eType == 8 || pEquipProto->eType == 9 || pEquipProto->eType == 10 || pEquipProto->eType == 11)) continue;
		}

		if(pEquipProto->byMinUseLevel == nLevel || pEquipProto->byMinUseLevel+5 == nLevel)
		{
			tagItem *pItem = ItemCreator::Create(EICM_GM, NULL, dwEquipID, 1);
			if(!P_VALID(pItem)) return GT_INVALID;

			ASSERT( MIsEquipment(pItem->dwTypeID) );

			ItemCreator::IdentifyEquip((tagEquip*)pItem, (EItemQuality)nQlty);
			GetItemMgr().Add2Bag(pItem, ELCID_GM_CreateItem, TRUE);
		}
	}

	return TRUE;
}

MoveData::EMoveRet Role::MoveAction( PFMoveAction2P pAction, Vector3& v1, Vector3& v2 )
{
	if(GetPetPocket()->IsRideAfter())
	{
		return MoveData::EMR_Invalid;
	}

	MoveData::EMoveRet emr = (GetMoveData().*pAction)(v1, v2);
	if (MoveData::EMR_Success == emr)
	{
		PetSoul* pSoul = GetPetPocket()->GetMountPetSoul();
		if (P_VALID(pSoul))
		{
			Role* pPassenger = pSoul->GetPassenger();
			if (P_VALID(pPassenger))
			{
				MoveData::EMoveRet emrp = ((pPassenger->GetMoveData()).*pAction)(v1, v2);
				ASSERT(MoveData::EMR_Success == emrp);
			}
		}
	}

	return emr;
}

MoveData::EMoveRet Role::MoveAction( PFMoveAction1P pAction, Vector3& v )
{
	if(GetPetPocket()->IsRideAfter())
	{
		return MoveData::EMR_Invalid;
	}

	MoveData::EMoveRet emr = (GetMoveData().*pAction)(v);
	if (MoveData::EMR_Success == emr)
	{
		PetSoul* pSoul = GetPetPocket()->GetMountPetSoul();
		if (P_VALID(pSoul))
		{
			Role* pPassenger = pSoul->GetPassenger();
			if (P_VALID(pPassenger))
			{
				MoveData::EMoveRet emrp = ((pPassenger->GetMoveData()).*pAction)(v);
				ASSERT(MoveData::EMR_Success == emrp);
			}
		}
	}

	return emr;
}

DWORD Role::CanCastMotion( Unit* pDest, DWORD dwActionID )
{
	if (!P_VALID(pDest))
	{
		return E_Motion_CanotActive;
	}

	if(!GetMotionInviteStatus(pDest->GetID()))
		return E_Motion_CanotActive;

	const tagMotionProto* pMotion = g_attRes.GetMotionProto(dwActionID);
	if (!P_VALID(pMotion))
	{
		return E_Motion_NotValid;
	}

	// ϵͳ�ж�Ŀ���Ƿ�Ϊ��ɫ�����ǹ��NPC���κηǽ�ɫ�Ŀ�ѡ��Ŀ�꣩���������ϣ�����ʾ�����޷����н��������������������һ����
	if (!pDest->IsRole())
	{
		return E_Motion_CanotActive;
	}
	Role* pDestRole = static_cast<Role*>(pDest);
	// ϵͳ�ж�Ŀ���Ƿ�Ϊ���ԣ��������ϣ�����ʾ�����޷����н��������������������һ����
	if(pDestRole->GetSex() && GetSex() || !pDestRole->GetSex() && !GetSex())
	{
		return E_Motion_CanotActive;
	}
	// ϵͳ�ж�Ŀ���Ƿ��ڷǳ�е״̬���������ϣ�����ʾ�����޷����н��������������������һ����
	if (pDestRole->IsInRoleStateAny(ERS_Combat | ERS_PK | ERS_PKEX | ERS_Arena) )
	{
		return E_Motion_CanotActive;
	}
	// ϵͳ�ж�˫�����Ѻö��Ƿ�����
	tagFriend* pFriend = GetFriendPtr(pDestRole->GetID());
	if (!P_VALID(pFriend) || pFriend->dwFriVal < pMotion->dwFriendVal)
	{
		return E_Motion_FriendValNotEnough;
	}

	// ������ֱ�߾������X�����ӣ�������Զ����������ʧ�ܣ���ȡ����
	if (!IsInCombatDistance(*pDest, MAX_MOTION_DIST))
	{
		return E_Motion_DistanceNotEnough;
	}

	return E_Motion_Success;
	// Ŀ���ɫ����Ļ�ڵ���ȷ����ʾ�򣺡�####�������ɫ��������������###�����Զ������ƣ�����������/�ܾ���
	// ������ܾ�������������ʾ�����Է��ܾ����������󡣡�
	// ��30����δ����Ӧ�����Զ��ж�Ϊ�ܾ���
	// ��������ܡ�������뽻�������������̣�
	// ������������߿�ʼ�Զ��ƶ���
	// �����Զ��ƶ������н������ƶ����򽻻�����ʧ�ܣ���ȡ����
	// �����Զ��ƶ������з����������ƶ����򽻻�����ʧ�ܣ���ȡ����
	// ������ֱ�߾������X�����ӣ�������Զ����������ʧ�ܣ���ȡ����
	// ������������������Զ��ƶ������У����赲����߲�����������򣩣��򽻻�����ʧ�ܣ���ȡ����
	// ������һ���жϡ����̶�ͨ���󣬷������ƶ���������ͬһ����ͬһ�߶�λ�ã�Ȼ��ϵͳ�Զ���������Ϊͬһ����ͬʱ��ʼ���Ž������Զ�����
	// ���ڽ��������������̹����У�����������ɫ��������ɫ��������һ���ѷ���Ľ����������룬��֮ǰ�Ķ����������̱���ϡ�

}

DWORD Role::CastMotion( Unit* pDest, DWORD dwActionID )
{
	Role* pDestRole = static_cast<Role*>(pDest);
	
	tagNS_RoleStyleAction send;
	send.dwActionID = dwActionID;

	send.dwRoleID = GetID();
	send.dwDstRoleID = pDestRole->GetID();
	GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);

	return E_Motion_Success;
}

BOOL Role::GetMotionInviteStatus(DWORD dwRoleID)
{
	// �ǻ���������ͬ��
	if (dwRoleID == m_dwPartnerID)
		return TRUE;
	// û�л�飬���ҳ�ʱ
	else if (!GT_VALID(m_dwPartnerID) && !GT_VALID(m_nMotionCounter))
		return TRUE;
	// ���ǻ�飬����ʱ
	else
		return FALSE;
}

VOID Role::SetMotionInviteStatus(BOOL bSet, DWORD dwRoleID)
{
	if (GT_VALID(m_dwPartnerID) && dwRoleID != m_dwPartnerID)
		return ;

	if (bSet)
	{
		m_nMotionCounter = TICK_PER_SECOND * 60;
		m_dwPartnerID = dwRoleID;
	}
	else
	{
		m_nMotionCounter = GT_INVALID;
		m_dwPartnerID = GT_INVALID;
	}
}

VOID Role::UpdateMotionInviteState()
{
	if (m_nMotionCounter >= 0)
	{
		--m_nMotionCounter;
	}
	// time out, reset the partner
	else if (!GT_VALID(m_nMotionCounter))
	{
		m_dwPartnerID = GT_INVALID;
	}
}

INT Role::CanGather(Creature* pRes)
{
	// �����Դ�ͽ�ɫ
	Creature* pCreature	= pRes;	
	Role* pRole	= this;	

	if ( !P_VALID(pCreature) || !P_VALID(pRole) )
		return GT_INVALID;

	// �ж���Դ����ü���
	Skill* pGatherSkill = NULL;
	if ( pCreature->IsNatuRes() )
		pGatherSkill = pRole->GetSkill(GATHER_SKILL_MINING);
	else if ( pCreature->IsManRes() )
		pGatherSkill = pRole->GetSkill(GATHER_SKILL_HARVEST);
	if ( !P_VALID(pGatherSkill) )
		return GT_INVALID;

	// ����ɫ��������				
	ItemMgr& itemMgr = pRole->GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_UseItem_NotEnoughSpace;

	// ����Դ���빻�� 
	if (!pRole->IsInCombatDistance(*pCreature, pGatherSkill->GetOPDist()))
		return E_UseSkill_DistLimit;	

	// ��Դ�ѱ�ռ��
	if( pCreature->IsTagged() )
		return E_UseSkill_TargetLimit;

	return E_Success;
}

VOID Role::ModMountSpeed( INT nMod )
{
	ModAttModValue(ERA_Speed_Mount, nMod);
	RecalAtt();
}

VOID Role::SaveExp2DB()
{
	tagNDBC_UpdateRoleAtt send;

	send.dwRoleID = GetID();
	send.nType = ERTSA_Exp;
	send.nValue = m_nCurLevelExp;

	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::SaveAttPoint2DB()
{
	tagNDBC_UpdateRoleAttPoint send;

	send.dwRoleID = GetID();
	send.nAttPointLeft = GetAttValue(ERA_AttPoint);
	IFASTCODE->MemCpy(send.nAttPointAdd, m_nAttPointAdd, sizeof(send.nAttPointAdd));

	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::SaveTalentPoint2DB(INT nIndex)
{
	tagNDBC_UpdateRoleTalentPoint send;

	send.dwRoleID = GetID();
	send.nTalentPoint = GetAttValue(ERA_TalentPoint);
	send.Talent = m_Talent[nIndex];
	send.nIndex = nIndex;
	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::SaveLevel2DB()
{
	tagNDBC_UpdateRoleAtt send;

	send.dwRoleID = GetID();
	send.nType = ERTSA_Level;
	send.nValue = m_nLevel;
	g_dbSession.Send(&send, send.dwSize);
}
// ʹ�ü�¼ʽ���ͷ�
VOID Role::UseNoteTakingItemTransport()
{
	GotoNewMap(m_dwItemTransportMapID, m_fItemTransportX, m_fItemTransportY, m_fItemTransportZ);
	
}
// ʹ�ü�¼ʽ���ͷ���¼��ǰ����
VOID Role::SetItemTransportPoint(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetTransportMap);

	E_SET_TRANSPORT_ERROR_CODE dwErrorCode = EST_SuccessSet;
	
	Map *pMap = GetMap();
	//����Ǹ��� �� ������
	if(pMap->GetMapInfo()->eType != EMT_Normal || 
		(pMap->GetMapInfo()->eType == EMT_Normal && pMap->GetMapInfo()->eNoarmalMapType == ENMT_TrainingField))
	{
		dwErrorCode = EST_InInstance;
	}

	// �����Ʒ�Ƿ��ڱ�����
	tagItem* pItem = this->GetItemMgr().GetBagItem(pRecv->n64ItemID); 
	if( !P_VALID(pItem) ) 
	{
		dwErrorCode = EST_LackNeedItem;
	}
	else
	{	
		if( this->IsInRoleState(ERS_PrisonArea) )
		{
			dwErrorCode = EST_SelfStateLimit;
		}
		else if(pItem->pProtoType->eSpecFunc != EISF_TransportTool)
		{
			dwErrorCode = EST_SpecFunError;
		}
	}

	//�ظ��ͻ���
	tagNS_SetTransportMap	sendclient;
	sendclient.dwErrorCode = dwErrorCode;
	sendclient.dwTransportMapID = pMap->GetMapID();
	sendclient.vTransportPoint = GetCurPos();
	SendMessage(&sendclient, sendclient.dwSize);

	if(dwErrorCode == EST_SuccessSet)
	{
		//
		m_dwItemTransportMapID = pMap->GetMapID();
		m_fItemTransportX = sendclient.vTransportPoint.x;
		m_fItemTransportZ = sendclient.vTransportPoint.z;
		m_fItemTransportY = sendclient.vTransportPoint.y;

		tagNDBC_UpdateRoleItemTransport senddb;
		senddb.dwRoleID	= GetID();
		senddb.dwMapID	= pMap->GetMapID();
		senddb.fX		= sendclient.vTransportPoint.x;
		senddb.fZ		= sendclient.vTransportPoint.z;
		senddb.fY		= sendclient.vTransportPoint.y;
		g_dbSession.Send(&senddb, senddb.dwSize);
	}
}


// Jason
BOOL Role::IsValidOfLessingLoong(VOID)
{
	return ROLE_MAX_LESSLEVEL_OF_LOONG > m_nLessingLevel;
}

// Jason 2009-12-7 �����͸�
VOID	Role::UpdateBlessingOfLoong(VOID)
{
	if( IsValidOfLessingLoong() ) //�ﵽ�͸���߼��𣬲���͸�
	{
		tagConfigLessingOfLoong * pCur = g_attRes.GetConfigInfoOfLessingLoong(m_nLessingLevel + 1); // ���ݿ�����Ĭ����0
		if( !P_VALID( pCur ) )
			return;
		DWORD _now = GetCurrentOnlineTime();

		if( (DWORD)m_timeLastLessing == (DWORD)0 )
			m_timeLastLessing = _now;

		if( /*CalcTimeDiff( */_now - m_timeLastLessing /*)*/ >= pCur->nExpireTime && !m_bFiredOnceOfLessingLoong )
		{
			m_bFiredOnceOfLessingLoong = TRUE;
			//m_timeLastLessing = _now;
			NotifyClientToReceiveLessingOfLoong();
			//++m_nLessingLevel;
		}
	}
}

// Jason
VOID Role::NotifyClientToReceiveLessingOfLoong(VOID)
{
	tagNS_SetNextBenediction msg;
	tagConfigLessingOfLoong * pCur/*, * pNext*/;
	msg.isEnd			= !IsValidOfLessingLoong();


	if( !msg.isEnd )
	{
		pCur = g_attRes.GetConfigInfoOfLessingLoong(m_nLessingLevel + 1);//Ĭ�����ݿ�������0

		if( !P_VALID( pCur ) )
			return;

		DWORD _now = GetCurrentOnlineTime();
		DWORD	lastLessingTime = m_timeLastLessing;

		DWORD val = pCur->nExpireTime + (DWORD)m_timeLastLessing ;
		if( val <= _now ) //�����ˣ����߳���
			msg.nextServerTime = 0;
		else
			msg.nextServerTime	= val - _now;/*CalcTimeDiff( tagDWORDTime(val)  , _now )*/;

		msg.nItemTypeID		= pCur->dwRewardItemTypeID;
		msg.nItemCount		= pCur->nItemCount;
		msg.n32Exprence		= pCur->nExperience;
	}
	else
	{
		msg.nItemTypeID		= GT_INVALID;
		msg.nItemCount		= 0;
		msg.n32Exprence		= 0;
	}

	SendMessage  (&msg,msg.dwSize);
}

// Jason
VOID Role::NotifyClientIniLessingOfLoong(VOID)
{
	if( IsValidOfLessingLoong() )
	{
		tagNS_LoongBenedictionInit msg;
		SendMessage( &msg, msg.dwSize );
		NotifyClientToReceiveLessingOfLoong();
	}
}
// Jason
DWORD Role::ReceiveLessingOfLoong(VOID)
{
	tagNS_GetLoongBenediction msg;
	DWORD re = 0;

	// �ﵽ���8�Σ�ֹͣ�û
	if( IsValidOfLessingLoong() )
	{
		do 
		{
			tagConfigLessingOfLoong *pCur = g_attRes.GetConfigInfoOfLessingLoong(m_nLessingLevel + 1);
			if( !P_VALID( pCur ) )
			{
				re = ELBD_No_LessingProto;
				IMSG(_T("Caution: Lessing of loong , no lessing prototype.\n"));
				return re;
				//break;
			}

			DWORD _now = GetCurrentOnlineTime();
			DWORD	lastLessingTime = m_timeLastLessing;

			// �����ж�
			DWORD val = pCur->nExpireTime + (DWORD)m_timeLastLessing ;
			if( val > _now )
			{
				re = ELBD_Time_NotArrive;
				break;
			}// �����ڻ��߳��ڣ�Ӧ�ô���

			if( pCur->nItemCount > 0 && P_VALID(pCur->dwRewardItemTypeID) )
			{
				//do{
				if( ( MIsEquipment(pCur->dwRewardItemTypeID) && ( GetItemMgr().GetBagFreeSize() < pCur->nItemCount ) ) || 
					( !MIsEquipment(pCur->dwRewardItemTypeID) && ( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE || GetItemMgr().GetBagFreeSize() < 1 )/*( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE  || 
					)*/
				)
				)
				{
					re = ELBD_Bag_NotEnoughSpace;

					msg.dwErrorCode = re;
					msg.nExprence	= 0;
					msg.nItemCount	= 0;
					msg.nItemTypeID	= GT_INVALID;

					SendMessage  (&msg,msg.dwSize);

					return re;
				}

					tagItem *pItem = NULL;
					pItem = ItemCreator::CreateByProduce(EICM_LessingLoong,  GetID(), 
						pCur->dwRewardItemTypeID,(EItemQuality)pCur->nQlty,0,0,pCur->nItemCount/*,GetID()*/ );

					if( !P_VALID(pItem) )
					{
						re = ELBD_Item_Create_Failed;
						IMSG(_T("Caution: Lessing of loong , create item failed.\n"));
						return re;
					}


					// �ж��Ƿ�Ϊʱװ���������
					if(P_VALID(pItem) && MIsFashion(pItem))
					{
						ItemCreator::IdentifyEquip((tagEquip*)pItem);
					}


					if( P_VALID(pItem) )
					{
						re = GetItemMgr().Add2Bag(pItem, ELCID_LessingLoong, TRUE);
					}
					else
					{
						re = ELBD_Item_Create_Failed;
						IMSG(_T("Caution: Lessing of loong , bag not enough space.\n"));
						return re;
						//break;
					}
				//}while(0);
			}

			m_timeLastLessing = /*g_world.GetWorldTime()*/GetCurrentOnlineTime();
			++m_nLessingLevel;
			m_bFiredOnceOfLessingLoong = FALSE;

			if( E_Item_MaxHold == re )
			{
				re = ELBD_Bag_NotEnoughSpace;
				//break;
				IMSG(_T("Caution: Lessing of loong , bag not enough space.\n"));
			}

			// ���Ӿ��飬������پ���ġ�
			if( pCur->nExperience > 0 )
				ExpChange(pCur->nExperience);

			msg.dwErrorCode = re;
			msg.nExprence	= pCur->nExperience;
			msg.nItemCount	= pCur->nItemCount;
			msg.nItemTypeID	= pCur->dwRewardItemTypeID;

			SendMessage  (&msg,msg.dwSize);

			NotifyClientToReceiveLessingOfLoong();

			return re;
		} while (0);
	}
	else
		re = ELBD_No_Benediction;

	msg.dwErrorCode = re;
	msg.nExprence	= 0;
	msg.nItemCount	= 0;
	msg.nItemTypeID	= GT_INVALID;

	SendMessage  (&msg,msg.dwSize);
	return re;
}


DWORD Role::GetCurrentOnlineTime(VOID)
{
	DWORD re = 0;
//������ role_save.cpp line : 35	
	if(m_LogoutTime < m_LoginTime)
	{
		re += CalcTimeDiff(g_world.GetWorldTime(), m_LoginTime);
	}
	else
	{
		re += CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime);
	}
	if( m_nOnlineTime == GT_INVALID )
		return re;
	re += m_nOnlineTime;

	return re;
}

DWORD Role::ProcessGoldStoneMsg( INT64 GoldStoneItemID, INT64 destItemID )
{
	tagItem *pItem = GetItemMgr().GetBagItem( GoldStoneItemID ); 
	tagItem *pDestItem = GetItemMgr().GetBagItem( destItemID ); 
	if( !P_VALID( pItem ) || !P_VALID( pDestItem ) )
	{
		IMSG(_T("Caution: Gold Stone , no such item."));
		return E_GoldStone_SrcItemNotExist;
	}

	if( EISF_BeGoldStone == pItem->pProtoType->eSpecFunc )
	{
		tagPhilosophersStoneConfig * pConfig = g_attRes.GetConfigInfoOfPhilosophersStone(pItem->pProtoType->nSpecFuncVal1,pDestItem->pProtoType->dwTypeID);
		if( /*!*/P_VALID( pConfig )  )//û�������ļ���Ҫ��һ��errlog
		//{
		//	IMSG(_T("Caution: Gold Stone , cannot find config file of philosopher's stone.\n"));
		//	return E_GoldStone_NoConfigFile;
		//}

		//if( pConfig->nStoneType == pItem->pProtoType->nSpecFuncVal1 )
		{
			DWORD re = 0;
			tagItem *pNewItem = NULL;
			pNewItem = ItemCreator::Create(EICM_GoldStone,  GetID(), 
				pConfig->dwDestItemTypeID,1,GetID() );


			// �ж��Ƿ�Ϊʱװ���������
			if(P_VALID(pNewItem) && MIsFashion(pNewItem))
			{
				ItemCreator::IdentifyEquip((tagEquip*)pNewItem);
			}

			GetItemMgr  ().DelFromBag(GoldStoneItemID,ELCID_PhilosophersStone,1);

			if( P_VALID(pNewItem) )
			{
				GetItemMgr().DelFromBag  (destItemID,ELCID_PhilosophersStone/*,1*/);
				re = GetItemMgr().Add2Bag(pNewItem, ELCID_PhilosophersStone, TRUE);
			}
			else
			{
				re = E_GoldStone_CreateDestItemFail;
				IMSG(_T("Caution: Gold Stone , create new item false.\n"));
				return re;
				//break;
			}
			return E_GoldStone_Success;
		}
		else
		{
			IMSG(_T("Caution: Gold Stone , philosopher's stone missing match.\n"));
			return E_GoldStone_MissingMatch;
		}
	}
	// else ���ǵ��ʯ

	IMSG(_T("Caution: Gold Stone , it's not philosopher's stone.\n"));

	return E_GoldStone_OtherError;
}

DWORD Role::VocationCheck( DWORD dwVocationLimit )
{
	INT nTmpClass =  1 << ( m_eClass - 1 );
	INT nTmpClassEx = 0;
	INT nTmp = 0;

	if ( (INT)m_eClassEx != (INT)EHV_Base )
	{
		nTmpClassEx = 1 << ( m_eClassEx + 8 );
	}

	nTmp = nTmpClass + nTmpClassEx;

	if ( !( nTmp & dwVocationLimit ) )
		return E_SystemError;

	return E_Success;
}

DWORD Role::CanPickUpItem( INT64 n64GroundID, BOOL bRole /*= TRUE*/ )
{
	MTRANS_ELSE_RET(pMap, GetMap(), Map, E_Loot_Map_Not_Found);
	MTRANS_ELSE_RET(pGroundItem, pMap->GetGroundItem(n64GroundID), tagGroundItem, E_Loot_Item_Not_Found);

	if(pGroundItem->dwTeamID == GT_INVALID && pGroundItem->dwOwnerID != GT_INVALID && GetID() != pGroundItem->dwOwnerID)
		return E_Loot_Belong_To_You;

	if(pGroundItem->dwTeamID != GT_INVALID && GetTeamID() != pGroundItem->dwTeamID)
		return E_Loot_Belong_To_Team;

	if(pGroundItem->dwTeamID != GT_INVALID && pGroundItem->dwOwnerID != GT_INVALID && GetID() != pGroundItem->dwOwnerID)
		return E_Loot_Belong_To_Teammate;

	if(bRole)
	{
		//�����Һ���Ʒ�ľ��� ʰȡ��Χ���Զ�ʰȡ��Χ�ǲ�һ����
		if ( abs(pGroundItem->vPos.x - GetCurPos().x) > X_DEF_AUTO_PICKUP_DIST 
			|| abs(pGroundItem->vPos.z - GetCurPos().z) > X_DEF_AUTO_PICKUP_DIST )
			return E_Loot_Pick_Up_Too_Far;
	}

	//�����Ƿ��пռ�
	if ( TYPE_ID_MONEY != pGroundItem->dwTypeID && GetItemMgr().GetBagFreeSize() <= 0)
		return E_Loot_BAG_NOT_ENOUGH_SPACE;

	// ���ѵ���Ŀ
	BOOL bCan = GetItemMgr().CanAddMaxHoldItem(pGroundItem->dwTypeID, pGroundItem->nNum);

	return bCan ? E_Success : E_Item_MaxHold;
}
