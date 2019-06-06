//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_init.cpp
// author: 
// actor:
// data: 2008-8-11
// last:
// brief: ��ʼ���������ݽṹ
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_virgin_online.h"
#include "../WorldDefine/msg_activity.h"

#include "role.h"
#include "att_res.h"
#include "skill.h"
#include "quest.h"
#include "quest_mgr.h"
#include "map_creator.h"
#include "script_mgr.h"
#include "title_mgr.h"
#include "pet_pocket.h"
#include "vip_netbar.h"
#include "guild.h"
#include "guild_mgr.h"

//-------------------------------------------------------------------------------
// ��ʼ�����
//-------------------------------------------------------------------------------
VOID Role::Init(const tagRoleDataLoad* pData)
{
	ASSERT( P_VALID(pData) );

	const tagRoleDataConst* pDataConst = &pData->RoleDataConst;
	const tagRoleDataSave* pDataSave = &pData->RoleDataSave;

	// ��ʼ����ҽű�
	m_pScript = g_ScriptMgr.GetRoleScript();

	// ��ҽű�����
	IFASTCODE->MemCpy(m_ScriptData.dwData, pDataSave->Data.dwData, sizeof(DWORD)*ESD_Role);

	// �ƺŹ�����
	m_pTitleMgr		= new TitleMgr;

	// �½������
	m_pPetPocket	= new PetPocket;
	
	// ��ʼ����������
	IFASTCODE->MemCpy(&m_Avatar, &pDataConst->Avatar, sizeof(m_Avatar));
	m_DisplaySet = pDataSave->DisplaySet;
	m_dwRebornMapID = pDataSave->dwRebornMapID;
	m_CreatTime = pDataConst->CreateTime;
	m_LoginTime = g_world.GetWorldTime();		// ��������ʱ��
	m_LogoutTime = pDataSave->LogoutTime;
	m_nOnlineTime = pDataSave->nOnlineTime;
	m_nCurOnlineTime = pDataSave->nCurOnlineTime;
	m_eClass = pDataSave->eClass;
	m_eClassEx = pDataSave->eClassEx;
	
	// ��ʼ����ɫ��ʵ��ֵ�������ֵ
	m_Strength.dwValue = pData->RoleDataSave.Strength.dwValue;
	m_nStrengthNum = pData->RoleDataSave.nStrengthNum;
	m_Strength.m_dwHeroism = pData->RoleDataSave.Strength.m_dwHeroism;
	m_Strength.m_dwWit = pData->RoleDataSave.Strength.m_dwWit;
	m_Strength.m_dwErrantry = pData->RoleDataSave.Strength.m_dwErrantry;
	m_Strength.m_dwValor = pData->RoleDataSave.Strength.m_dwValor;
		
	m_bHasLeftMsg = pDataSave->bHasLeftMsg;

	// ��ʼ���ϴλ�ȡ�̳������Ʒʱ��
	SetLastGetMallFreeTime(pDataSave->dwTimeGetMallFree);

	// ���������Ƿ����������������
	m_RoleStateEx.SetState(ERSE_BagPsdExist);
	if(GetSession() && !GetSession()->IsHaveBagPsd())
	{
		m_RoleStateEx.UnsetState(ERSE_BagPsdExist);
		m_RoleStateEx.SetState(ERSE_BagPsdPass);
	}

	// ��ʼ���ƺ�ѡ��
	m_pTitleMgr->InitOpts(this, pDataSave->u16ActiveTitleID, pDataSave->sRemoteOpenSet.bTitle);

	// PK�������
	m_CloseSafeGuardTime = pDataSave->CloseSafeGuardTime;
	m_nDeadUnSetSafeGuardCountDown = pDataSave->nDeadUnSetSafeGuardCountDown;

	m_bNeedPrisonRevive = pDataSave->bNeedPrisonRevive;		//��Ҫ��������

	IFASTCODE->MemCpy(m_Talent, pDataSave->talent, sizeof(m_Talent));

	if(pDataSave->bSafeGuard)
	{
		SetRoleState(ERS_Safeguard, FALSE);
	}

	// ����
	m_dwGuildID = pDataSave->dwGuildID;
	if (GT_VALID(m_dwGuildID))
	{
		// �������Ƿ��ڰ�����
		Guild* pGuild	= g_guildMgr.GetGuild(m_dwGuildID);
		if (!P_VALID(pGuild) || !P_VALID(pGuild->GetMember(GetID())))
		{
			SetGuildID(GT_INVALID);
		}
		else
		{
			// ����״̬
			GuildCommodity* pCommodity = pGuild->GetGuildCommerce().GetCommodity(m_dwID);
			if (P_VALID(pCommodity))
			{
				SetRoleState(ERS_Commerce, FALSE);
			}
		}
	}
	
	// ��ȡ�������������ݿ�����
	InitAtt(pDataSave);

	// todo����ȡ�����б�װ���б�buff�б���������Զ��������Ե�Ӱ��
	/*********************************************************************************************************
	*�����б�״̬�б��ƺ��б��ƺ������б���Ʒ�б�װ���б������б�����б������б�����������б���ɫ����
	*���밴˳���ȡ(��˳���tagRoleDataSave�ж�Ӧ)
	*********************************************************************************************************/

	const BYTE *pDBData = &pDataSave->byData[0];	// �б�����ָ��,��ָ�����ڶ�ȡ�������ƶ�,��pDBData��ֵ�Ǳ仯��

	// ��ʼ�������б�
	InitSkill(pDBData, pDataSave->nSkillNum);

	// ��ʼ��״̬�б�
	InitBuff(pDBData, pDataSave->nBuffNum);

	// ��ʼ���ƺ��б�
	m_pTitleMgr->InitTitles(pDBData, pDataSave->nTitleNum);

	// ��ʼ����Ʒװ���б�
	InitItem(pDBData, pDataSave->nItemNum);

	// ��ʼ�����ѳ���б�
	InitFriend(pDBData, pDataSave->nFriendNum);
	
	// ��ʼ����ҵ�ǰ�����б�
	InitCurrentQuest(pDBData, pDataSave->nQuestNum);

	// ��ʼ�������������б�
	InitCompleteQuest(pDBData, pDataSave->nQuestDoneNum);

	// ��ʼ����Ʒ��ȴʱ��
	InitItemCDTime(pDBData, pDataSave->nItemCDTimeNum);

	// ��ʼ�����Ѷ�
	InitFriendValue(pDBData, pDataSave->nFriendshipNum);

	// ��ʼ��������
	InitBlackList(pDBData, pDataSave->nBlackNum);

	// ��ʼ����ɫ����
	m_VCard.Init(pDBData, this);

	// ��ʼ����������
	m_ClanData.Init(pDBData, this);

	// ��ʼ�������
	m_pPetPocket->Init(pDBData, pDataSave->nPetsNum, this);

	// ���������Ѿ�������ϣ����¼��������ʼ��ǰ����
	CalInitAtt();

	// ��������״̬
	CalInitState();

	g_VipNetBarMgr.PlayerNotify(GetSession()->GetSessionID());

	// ��ʼ����ɫ���������״̬
	InitChestState();

	// �������30��������ʱ����Ҫ���ر�pk����
	if ( m_nLevel >= 30 )
	{
		m_bLevelUpClosedSafeGuard = TRUE;
	}

	tagNS_RoleInitOk send;
	GetSession()->SendMessage(&send, send.dwSize);

	//UpdateBlessingOfLoong();// Jason
	// Jason 12-8-2007������ģʽ
	NotifyClientIniLessingOfLoong();

	// Jason v1.3.1�ⲿ����
	g_dbSession.SendMsgToPlayerForExtLinks  (this);
}

//----------------------------------------------------------------------------------------------
// ��ʼ�����ݿ����е���������
//----------------------------------------------------------------------------------------------
VOID Role::InitAtt(const tagRoleDataSave* pDataSave)
{
	// ����һ�����Ե�Ͷ��
	IFASTCODE->MemCpy(m_nAttPointAdd, pDataSave->nAttPointAdd, sizeof(m_nAttPointAdd));

	// �������Ը���Ĭ��ֵ
	for(INT n = 0; n < ERA_End; n++)
	{
		m_nBaseAtt[n] = g_attRes.GetAttDefRole(n);
	}

	// ����һЩ���������ݿ��е����ԣ�һЩ�������ֵ�ı�����ԣ���ȱ��һ��ʿ����
	m_nAtt[ERA_HP]			=	pDataSave->nHP;
	m_nAtt[ERA_MP]			=	pDataSave->nMP;
	m_nAtt[ERA_Rage]		=	pDataSave->nRage;
	m_nAtt[ERA_Vitality]	=	pDataSave->nVitality;
	m_nAtt[ERA_Endurance]	=	pDataSave->nEndurance;
	m_nAtt[ERA_Knowledge]	=	pDataSave->nKnowledge;
	m_nAtt[ERA_Injury]		=	pDataSave->nInjury;
	m_nAtt[ERA_Morale]		=	pDataSave->nMorale;
	m_nAtt[ERA_Morality]	=	pDataSave->nMorality;
	m_nAtt[ERA_Culture]		=	pDataSave->nCulture;
	m_nAtt[ERA_AttPoint]	=	pDataSave->nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	pDataSave->nTalentPoint;
	m_nAtt[ERA_Hostility]	=	pDataSave->nHostility;

	m_nLevel		= pDataSave->nLevel;					// �ȼ�
	m_nCurLevelExp	= pDataSave->nCurExp;					// ��ǰ��������

	m_eClass	= pDataSave->eClass;						// ְҵ
	m_eClassEx	= pDataSave->eClassEx;						// ְҵ��չ
	m_nCredit	= pDataSave->nCredit;						// ���ö�
	m_nIdentity = pDataSave->nIdentity;						// ���
	m_nVIPPoint = pDataSave->nVIPPoint;						// ��Ա����
	m_nTreasureSum = pDataSave->nTreasureSum;				//�����������

	// ͨ������ȼ������Ӧ��һ������
	const tagLevelUpEffect* pLevelUp = g_attRes.GetLevelUpEffect(pDataSave->nLevel);

	SetBaseAttValue(ERA_Physique, pLevelUp->n16RoleAtt[ERA_Physique]);
	SetBaseAttValue(ERA_Strength, pLevelUp->n16RoleAtt[ERA_Strength]);
	SetBaseAttValue(ERA_Pneuma, pLevelUp->n16RoleAtt[ERA_Pneuma]);
	SetBaseAttValue(ERA_InnerForce, pLevelUp->n16RoleAtt[ERA_InnerForce]);
	SetBaseAttValue(ERA_Technique, pLevelUp->n16RoleAtt[ERA_Technique]);
	SetBaseAttValue(ERA_Agility, pLevelUp->n16RoleAtt[ERA_Agility]);

	// ����ĳЩ��������
	SetBaseAttValue(ERA_MaxHP,			pLevelUp->n16HP);
	SetBaseAttValue(ERA_MaxMP,			pLevelUp->n16MP);
	SetBaseAttValue(ERA_MaxVitality,	pLevelUp->n16Vitality);

	// ����������Ե�Ͷ��ֵ������һ������
	ModBaseAttValue(ERA_Physique, m_nAttPointAdd[ERA_Physique]);
	ModBaseAttValue(ERA_Strength, m_nAttPointAdd[ERA_Strength]);
	ModBaseAttValue(ERA_Pneuma, m_nAttPointAdd[ERA_Pneuma]);
	ModBaseAttValue(ERA_InnerForce, m_nAttPointAdd[ERA_InnerForce]);
	ModBaseAttValue(ERA_Technique, m_nAttPointAdd[ERA_Technique]);
	ModBaseAttValue(ERA_Agility, m_nAttPointAdd[ERA_Agility]);
}

//---------------------------------------------------------------------------------
// ��ʼ������
//---------------------------------------------------------------------------------
VOID Role::InitSkill(const BYTE* &pData, INT32 nNum)
{
	// ���ȼ�����ͨ��������
	for(INT n = 0; n < X_NORMAL_ATTACK_SKILL_NUM; n++)
	{
		DWORD dwSkillID = NORMAL_ATTACK_SKILL_ID[n];
		Skill* pSkill = new Skill(dwSkillID, 1, 0, 0, 0);
		AddSkill(pSkill, FALSE);
	}

	// ���ر���ļ���
	if( nNum <= 0 ) return;

	const tagSkillSave* pSkillSave = (const tagSkillSave*)pData;

	for(INT n = 0; n < nNum; n++)
	{
		Skill* pSkill = new Skill(pSkillSave->dwID, pSkillSave->nSelfLevel, pSkillSave->nLearnLevel, pSkillSave->nCoolDown, pSkillSave->nProficiency);

		// �����ܼ��뵽�б���
		AddSkill(pSkill, FALSE);

		pSkillSave++;
	}

	// ����ָ��
	pData = (const BYTE*)((tagSkillSave*)pData + nNum);
	//pData = (const BYTE*)pSkillSave;
}

//-------------------------------------------------------------------------------
// ��ʼ��״̬
//-------------------------------------------------------------------------------
VOID Role::InitBuff(const BYTE* &pData, INT32 nNum)
{
	if( nNum <= 0 ) return;

	INT nBeBuffIndex = 0;
	INT nDeBuffIndex = MAX_BENEFIT_BUFF_NUM;

/*	const tagBuffSave* pEnd = (tagBuffSave*)pData + nNum;*/

	for(INT n = 0; n < nNum; n++)
	{
		tagBuffSave* pBuffSave = (tagBuffSave*)pData;

		const tagBuffProto* pProto = g_attRes.GetBuffProto(Buff::GetTypeIDFromIDAndLevel(pBuffSave->dwBuffID, pBuffSave->n8Level));
		if( !P_VALID(pProto) ) goto next_buff;

		// �������߼�ʱBuff
		if (pProto->bOfflineConsume)
		{
			// ��ȡ��ǰʱ��͸�����ϴ����ߵ�ʱ���(��λ����)
			DWORD dwOfflineTick = CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime) * TICK_PER_SECOND;
			
			// ����Buffʣ�����ʱ��(��ʹDWORD=>INT32Ҳû����)
			if (dwOfflineTick > MAX_BUFF_PERSIST_TICK)
			{
				goto next_buff;
			}
			else if ( pProto->nPersistTick > (INT32)dwOfflineTick + pBuffSave->nPersistTick )
			{
				pBuffSave->nPersistTick += (INT32)dwOfflineTick;
			}
			else
			{
				goto next_buff;
			}
		}

		// ���������Buff
		INT nIndex = GT_INVALID;
		if( pProto->bBenifit )
		{
			nIndex = nBeBuffIndex;
			// ����Buff����������
			if( nIndex >= MAX_BENEFIT_BUFF_NUM ) continue;
			++nBeBuffIndex;
		}
		// �������к�Buff
		else
		{
			nIndex = nDeBuffIndex;
			// �к�Buff����������
			if( nIndex >= MAX_BUFF_NUM ) continue;
			++nDeBuffIndex;
		}

		m_Buff[nIndex].Init(this, pBuffSave, nIndex);
		m_mapBuff.Add(m_Buff[nIndex].GetID(), &m_Buff[nIndex]);

next_buff:
		// �ۼ�ָ��
		pData += sizeof(tagBuffSave) - 1 + pBuffSave->n8ModifierNum * sizeof(DWORD);
	}

/*	pData = (const BYTE*)pEnd;*/
}

//-------------------------------------------------------------------------------
// ��ʼ�������Ʒ(װ��)
//-------------------------------------------------------------------------------
VOID Role::InitItem(const BYTE* &pData, INT32 nNum)
{
	INT32 nItemSize		= sizeof(tagItem);
	INT32 nEquipSize	= sizeof(tagEquip);
	
	// ��������Ϣ
	ZeroMemory(&m_AvatarEquipEquip, SIZE_AVATAR_EQUIP);
	ZeroMemory(&m_AvatarEquipFashion, SIZE_AVATAR_EQUIP);
	
	DWORD dwErrorCode;
	TList<tagItem *> listItem;
	const tagItem	*pTmpItem	= NULL;
	tagItem			*pNewItem	= NULL;

	pTmpItem = (const tagItem *)pData;
	for(INT32 i=0; i<nNum; ++i)
	{
		if(!MIsEquipment(pTmpItem->dwTypeID))
		{
			pNewItem = new tagItem;
			IFASTCODE->MemCpy(pNewItem, pTmpItem, nItemSize);
			pNewItem->pProtoType = g_attRes.GetItemProto(pTmpItem->dwTypeID);

			pTmpItem = (const tagItem*)((BYTE*)pTmpItem + nItemSize);
		}
		else
		{
			pNewItem = new tagEquip;
			IFASTCODE->MemCpy(pNewItem, pTmpItem, nEquipSize);
			pNewItem->pProtoType = g_attRes.GetEquipProto(pTmpItem->dwTypeID);

			pTmpItem = (const tagItem*)((BYTE*)pTmpItem + nEquipSize);
		}

		if(!P_VALID(pNewItem->pProtoType))
		{
			ASSERT(P_VALID(pNewItem->pProtoType));
			MAttResCaution(_T("item/equip"), _T("typeid"), pNewItem->dwTypeID);
			IMSG(_T("The item(SerialNum: %lld) hasn't found proto type!\n"), pNewItem->n64Serial);
			Destroy(pNewItem);
			continue;
		}

		pNewItem->eStatus = EUDBS_Null;
		pNewItem->pScript = g_ScriptMgr.GetItemScript( pNewItem->dwTypeID);
		
		dwErrorCode = Put2Container(pNewItem);
		if(dwErrorCode != E_Success)
		{
			if(dwErrorCode != E_Item_Place_NotFree)
			{
				ASSERT(0);
				Destroy(pNewItem);
				continue;
			}

			listItem.PushBack(pNewItem);
		}
	}

	// ����Ƿ�����λ���ظ����µĲ�����ӵ�������
	while(listItem.Size() != 0)
	{
		pNewItem = listItem.PopFront();

		// ����Ǳ�����ֿ��е���Ʒ�������λ
		switch(pNewItem->eConType)
		{
		case EICT_Bag:
			IMSG(_T("Ignor bag overlap for role<roleid:%u> item<serial: %lld> Container<id:%u> Position<index:%u>\n"), m_dwID, pNewItem->n64Serial, pNewItem->eConType, pNewItem->n16Index);
			ILOG->Write(_T("Ignor bag overlap for role<roleid:%u> item<serial: %lld> Container<id:%u> Position<index:%u>\n"), m_dwID, pNewItem->n64Serial, pNewItem->eConType, pNewItem->n16Index);
			Destroy(pNewItem);
			break;
		case EICT_Equip:
			dwErrorCode = GetItemMgr().Add2Bag(pNewItem, (DWORD)ELCID_Bag_PosOverlap, FALSE, FALSE);
			if(E_Con_NotEnoughSpace == dwErrorCode)
			{
				IMSG(_T("Because container<eType: %d> is full, item<serial: %lld\n> load failed!\n"), 
					EICT_Bag, pNewItem->n64Serial);
				IMSG(_T("Make some places and login again can solve this problem!\n"));				
			}
			if(dwErrorCode != E_Success)
				Destroy(pNewItem);
			break;
		case EICT_RoleWare:
			dwErrorCode = GetItemMgr().Add2RoleWare(pNewItem, (DWORD)ELCID_RoleWare_PosOverlap, FALSE, FALSE);
			if(E_Con_NotEnoughSpace == dwErrorCode)
			{
				IMSG(_T("Because container<eType: %d> is full, item<serial: %lld> load failed!\n"), 
					EICT_RoleWare, pNewItem->n64Serial);
				IMSG(_T("Make some places and login again can solve this problem!\n"));
			}
			if(dwErrorCode != E_Success)
				Destroy(pNewItem);
			break;
		default:
			ASSERT(0);
			Destroy(pNewItem);
			continue;
		}
		
		ASSERT(E_Success == dwErrorCode);
	}

	// ����ָ��
	pData = (const BYTE*)pTmpItem;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж������Ʒ(װ��),���뵽��Ӧ��������
//-------------------------------------------------------------------------------
DWORD Role::Put2Container(tagItem *pItem)
{
	DWORD dwErrorCode = GetItemMgr().Put2Container(pItem);

	if(EICT_Equip == pItem->eConType && E_Success == dwErrorCode)
	{
		MTRANS_POINTER(p, pItem, tagEquip);
		ProcEquipEffectAtt(p, TRUE, p->n16Index);
		ResetOneEquipDisplay(p, p->n16Index);
		m_Suit.Add(p, p->n16Index, FALSE);
		m_LongHun.Add(p, p->n16Index, FALSE);
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж��������Ʒ��ȴʱ��
//-------------------------------------------------------------------------------
VOID Role::InitItemCDTime(const BYTE* &pData, INT32 nNum)
{
	if(nNum <= 0)
	{
		return;
	}

	// ��ȡ��ǰʱ��͸�����ϴ����ߵ�ʱ���(��λ����)
	DWORD dwInterval = CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime);
	if(dwInterval > (DWORD)MAX_ITEM_DCTIME)
	{
		// ����ָ��
		pData = pData + nNum * sizeof(tagCDTime);
		return;
	}

	// ת���ɺ���
	dwInterval *= 1000;
	
	ItemMgr &itemMgr = GetItemMgr();
	const tagCDTime *p = (const tagCDTime*)pData;

	for(INT32 i=0; i<nNum; ++i)
	{
		if(p->dwTime > dwInterval)
		{
			itemMgr.Add2CDTimeMap(p->dwTypeID, p->dwTime - dwInterval);
		}

		++p;
	}

	// ����ָ��
	pData = (const BYTE*)((tagCDTime *)pData + nNum);
	//pData = (const BYTE*)p;
}

//-------------------------------------------------------------------------------
// ��ʼ�������б�
//-------------------------------------------------------------------------------
VOID Role::InitFriend(const BYTE* &pData, INT32 nNum)
{
	for(INT i = 0; i < MAX_FRIENDNUM; ++i)
	{
		m_Friend[i].dwFriendID = GT_INVALID;
		m_Friend[i].dwFriVal = 0;
		m_Friend[i].byGroup = 1;
	}

	// ��������ʱ��
	memset(m_dwBlackList, 0XFF, sizeof(m_dwBlackList));
	if(nNum <= 0)
		return;

	const tagFriendSave	*pFriend = NULL;
	DWORD dwSizeFriend = sizeof(tagFriendSave);
	pFriend = (const tagFriendSave*)pData;

	for(INT m = 0; m < nNum; ++m)
	{
		SetFriend(m, pFriend->dwFriendID, 0, pFriend->nGroupID);
		pFriend = (const tagFriendSave*)((BYTE*)pFriend + dwSizeFriend);
	}

	// ����ָ��
	pData = (const BYTE*)((tagFriendSave*)pData + nNum);
	//pData = (const BYTE*)pFriend;
}

//-------------------------------------------------------------------------------
// ��ʼ�����Ѷ�
//-------------------------------------------------------------------------------
VOID Role::InitFriendValue(const BYTE* &pData, INT32 nNum)
{
	const tagFriendshipSave *pFriendSave = NULL;
	tagFriend *pFriend = NULL;
	DWORD dwSizeFriend = sizeof(tagFriendshipSave);
	pFriendSave = (const tagFriendshipSave*)pData;

	for(INT i = 0; i < nNum; ++i)
	{
		pFriend = m_mapFriend.Peek(pFriendSave->dwFriendID);

		if(P_VALID(pFriend))
			pFriend->dwFriVal = pFriendSave->nFriVal;

		pFriendSave = (const tagFriendshipSave*)((BYTE*)pFriendSave + dwSizeFriend);
	}

	// ����ָ��
	pData = (const BYTE*)((tagFriendshipSave*)pData + nNum);
	//pData = (const BYTE*)pFriendSave;
}

//-------------------------------------------------------------------------------
// ��ʼ��������
//-------------------------------------------------------------------------------
VOID Role::InitBlackList(const BYTE* &pData, INT32 nNum)
{
	const DWORD *pBlackList = NULL;
	DWORD dwSize = sizeof(DWORD);
	pBlackList = (const DWORD*)pData;

	for(INT i = 0; i < nNum; ++i)
	{
		SetBlackList(i, *pBlackList);
		pBlackList = (const DWORD*)((BYTE*)pBlackList + dwSize);
	}

	// ����ָ��
	pData = (const BYTE*)((DWORD*)pData + nNum);
	//pData = (const BYTE*)pBlackList;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж�����ҵ�ǰ�����񼯺�
//-------------------------------------------------------------------------------
VOID Role::InitCurrentQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestSave* pQuestSave = (const tagQuestSave*)pData;
	const tagQuestSave* pEndQuestSave = pQuestSave + nNum;
	const tagQuestProto* pProto = NULL;
	INT nIndex = 0;

	for ( ; pQuestSave < pEndQuestSave; ++pQuestSave)
	{
		pProto = g_questMgr.GetQuestProto(pQuestSave->u16QuestID);
		if( !P_VALID(pProto) ) 
		{
			MAttResCaution(_T("QuestFile"), _T("questID"), pQuestSave->u16QuestID);
			continue;
		}

		m_Quests[nIndex].Init(pQuestSave, this, nIndex);
		m_mapCurrentQuests.Add(m_Quests[nIndex].GetID(), &m_Quests[nIndex]);
		++nIndex;
	}
	
	pData = (const BYTE*)((tagQuestSave*)pData + nNum);
	//pData = (const BYTE *)pEndQuestSave;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж�������Ѿ���ɵ����񼯺�
//-------------------------------------------------------------------------------
VOID Role::InitCompleteQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestDoneSave* pQuestDoneSave = (const tagQuestDoneSave*)pData;

	for(INT32 i = 0; i < nNum; ++i)
	{
		tagQuestDoneSave* pDoneQuest = new tagQuestDoneSave;
		IFASTCODE->MemCpy(pDoneQuest, pQuestDoneSave, sizeof(tagQuestDoneSave));
		m_mapCompleteQuests.Add(pDoneQuest->u16QuestID, pDoneQuest);
		
		++pQuestDoneSave;
	}

	// ����ָ��
	pData = (const BYTE*)((tagQuestDoneSave*)pData + nNum);
	//pData = (const BYTE*)pQuestDoneSave;
}

//-------------------------------------------------------------------------------
// ���������ʼ��ǰ����
//-------------------------------------------------------------------------------
VOID Role::CalInitAtt()
{
	// �ȱ���һЩ�������ֵ�ı������
	INT nHP				=	m_nAtt[ERA_HP];
	INT nMP				=	m_nAtt[ERA_MP];
	INT nVitality		=	m_nAtt[ERA_Vitality];
	INT nEndurance		=	m_nAtt[ERA_Endurance];
	INT nKnowledge		=	m_nAtt[ERA_Knowledge];
	INT nInjury			=	m_nAtt[ERA_Injury];
	INT nMorale			=	m_nAtt[ERA_Morale];
	INT nMorality		=	m_nAtt[ERA_Morality];
	INT nCulture		=	m_nAtt[ERA_Culture];
	INT nAttPoint		=	m_nAtt[ERA_AttPoint];
	INT nTalentPoint	=	m_nAtt[ERA_TalentPoint];
	INT nHostility		=	m_nAtt[ERA_Hostility];
	INT nRage			=	m_nAtt[ERA_Rage];

	for(INT n = 0; n < ERA_End; n++)
	{
		m_nAtt[n] = CalAttMod(m_nBaseAtt[n], n);
	}

	// ͨ��һ�����Եĵ�ǰֵ�����������
	m_nAtt[ERA_MaxHP]		=	m_nAtt[ERA_Physique] * 10 + m_nBaseAtt[ERA_MaxHP];
	m_nAtt[ERA_MaxMP]		=	m_nAtt[ERA_Pneuma] * 10 + m_nBaseAtt[ERA_MaxMP];
	m_nAtt[ERA_ExAttack]	=	m_nAtt[ERA_Strength] * 5;
	m_nAtt[ERA_ExDefense]	=	m_nAtt[ERA_Physique] * 5 + m_nAtt[ERA_Strength];
	m_nAtt[ERA_InAttack]	=	m_nAtt[ERA_InnerForce] * 5;
	m_nAtt[ERA_InDefense]	=	m_nAtt[ERA_Pneuma] * 5 + m_nAtt[ERA_InnerForce];
	m_nAtt[ERA_AttackTec]	=	m_nAtt[ERA_Technique] * 6;
	m_nAtt[ERA_DefenseTec]	=	m_nAtt[ERA_Agility] * 4;
	m_nAtt[ERA_HitRate]		=	m_nAtt[ERA_Technique] * 10;
	m_nAtt[ERA_Dodge]		=	m_nAtt[ERA_Agility] * 10;
	m_nAtt[ERA_MaxEndurance]=	(m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;

	// ����������Լӳ�
	for(INT n = ERA_AttB_Start; n < ERA_AttB_End; n++)
	{
		m_nAtt[n] = CalAttMod(m_nAtt[n], n);
	}

	// ����ĳЩ����ǰ���ԡ������ֵ֮�����Сֵ
	if( nHP < 0 )
	{
		m_nAtt[ERA_HP] = m_nAtt[ERA_MaxHP];
	}
	else
	{
		m_nAtt[ERA_HP] = min(nHP, m_nAtt[ERA_MaxHP]);
	}

	if( nMP < 0 )
	{
		m_nAtt[ERA_MP] = m_nAtt[ERA_MaxMP];
	}
	else
	{
		m_nAtt[ERA_MP] = min(nMP, m_nAtt[ERA_MaxMP]);
	}

	if( nVitality < 0 )
	{
		m_nAtt[ERA_Vitality] = m_nAtt[ERA_MaxVitality];
	}
	else
	{
		m_nAtt[ERA_Vitality] = min(nVitality, m_nAtt[ERA_MaxVitality]);
	}

	if( nEndurance < 0 )
	{
		m_nAtt[ERA_Endurance] = m_nAtt[ERA_MaxEndurance];
	}
	else
	{
		m_nAtt[ERA_Endurance] = min(nEndurance, m_nAtt[ERA_MaxEndurance]);
	}

	// �����ϲ���������Ըı�ĵ�ǰ����
	m_nAtt[ERA_Knowledge]	=	nKnowledge;
	m_nAtt[ERA_Injury]		=	nInjury;
	m_nAtt[ERA_Morale]		=	nMorale;
	m_nAtt[ERA_Morality]	=	nMorality;
	m_nAtt[ERA_Culture]		=	nCulture;
	m_nAtt[ERA_AttPoint]	=	nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	nTalentPoint;
	m_nAtt[ERA_Hostility]	=	nHostility;
	m_nAtt[ERA_Rage]		=	nRage;

	// ����ĳЩ��������Ա仯������
	m_fMountXZSpeed *= FLOAT(m_nAtt[ERA_Speed_Mount]) / 10000.0f;
	m_fXZSpeed *= FLOAT(m_nAtt[ERA_Speed_XZ]) / 10000.0f;
	m_fYSpeed *= FLOAT(m_nAtt[ERA_Speed_Y]) / 10000.0f;
	m_fSwimXZSpeed *= FLOAT(m_nAtt[ERA_Speed_Swim]) / 10000.0f;
	m_Size *= FLOAT(m_nAtt[ERA_Shape]) / 10000.0f;
	// todo: pk״̬

	// ��������ֶ�
	ClearAttRecalFlag();
}

//-----------------------------------------------------------------------
// �����ʼ״̬
//-----------------------------------------------------------------------
VOID Role::CalInitState()
{
	// ����PK״̬
	CalPKState(FALSE);

	// ��������״̬
	if( GetAttValue(ERA_HP) <= 0 )
	{
		SetState(ES_Dead, FALSE);
	}
}

//------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------
VOID Role::Online(BOOL bFirst/* =FALSE */)
{
	if( bFirst )
		VirginOnline();

	// �������ߵĽű�����
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnRoleOnline(this);
	}
}

//------------------------------------------------------------------------
// ��һ������
//------------------------------------------------------------------------
VOID Role::VirginOnline()
{
	// ��������ʱ��
	m_nCurOnlineTime = 0;
	m_nOnlineTime = 0;

	// ���ó�����ͼ�͸����ͼ
	m_dwRebornMapID = m_dwMapID = g_mapCreator.GetBornMapID();

	// �õ�һ������ĳ�����
	Vector3 vBornPos = g_mapCreator.RandGetOneBornPos();

	// ���һ������
	FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);

	// ������
	m_MoveData.Reset(vBornPos.x, vBornPos.y, vBornPos.z, cosf(fYaw * 3.1415927f / 180.0f), 0.0f, sinf(fYaw * 3.1415927f / 180.0f));

	// ���ó������޵Ľű�����
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnRoleFirstOnline(this);
	}
}

//------------------------------------------------------------------------
// ��ʼ�������������
//------------------------------------------------------------------------
VOID Role::InitChestState()
{
	m_TreasureState.nChestSerial = 0;
	m_TreasureState.nKeySerial = 0;
	m_TreasureState.dwChestTypeID = 0;
	m_TreasureState.dwKeyTypeID = 0;
	m_TreasureState.ChestItem.dwTypeID = 0;
	m_TreasureState.ChestItem.nNum = 0;
	m_TreasureState.ChestItem.nTableID = 0;
	m_TreasureState.ChestItem.fBeginRate = 0.0;
	m_TreasureState.ChestItem.fNormalRate = 0.0;
	m_TreasureState.ChestItem.fRoleRate = 0.0;
	m_TreasureState.ChestItem.fServerRate = 0.0;
	
}