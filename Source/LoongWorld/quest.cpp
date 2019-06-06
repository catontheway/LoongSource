//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: quest.h
// actor:
// data: 2008-9-1
// last:
// brief: ������
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/quest.h"

#include "role.h"
#include "quest.h"
#include "quest_mgr.h"
#include "script_mgr.h"

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
Quest::Quest()
: ScriptData<ESD_Quest>(),m_pProto(NULL), m_pOwner(NULL),m_dwStartTime(GT_INVALID), m_nIndex(GT_INVALID), m_pScript(NULL)
{
	memset(m_n16CreatureCount, 0, sizeof(m_n16CreatureCount));
	memset(m_bSpeakNPC, false, sizeof(m_bSpeakNPC));
	memset(m_bTrigger, false, sizeof(m_bTrigger));
	memset(m_n16ItemCount, 0, sizeof(m_n16ItemCount));
	memset(m_bInvest, false, sizeof(m_bInvest));
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
Quest::~Quest()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
VOID Quest::Init(const tagQuestProto* pQuestProto, Role* pRole, INT nIndex)
{
	ASSERT( P_VALID(pQuestProto) && P_VALID(pRole) );
	ASSERT( nIndex >= 0 && nIndex < QUEST_MAX_COUNT );

	m_pProto = pQuestProto;
	m_pOwner = pRole;
	m_nIndex = nIndex;
	m_dwStartTime = g_world.GetWorldTime();
	m_pDynamicTarget = NULL;
	m_bComplete = FALSE;

	// ��ʼ����������״̬
	memset(m_n16CreatureCount, 0, sizeof(m_n16CreatureCount));
	memset(m_bSpeakNPC, false, sizeof(m_bSpeakNPC));
	memset(m_bTrigger, false, sizeof(m_bTrigger));
	memset(m_n16ItemCount, 0, sizeof(m_n16ItemCount));
	memset(m_bInvest, false, sizeof(m_bInvest));

	// ��ʼ���ű�
	m_pScript = g_ScriptMgr.GetQuestScript(m_pProto->id);

	// ��ʼ������̬Ŀ��
	if(EQTM_DYNAMIC == pQuestProto->target_mode)
	{
		m_pDynamicTarget = new tagQuestDynamicTarget;

		// ��ʼ���ű���̬Ŀ��
		if(P_VALID(m_pDynamicTarget) && P_VALID(m_pScript))
		{
			m_pScript->OnInit(pQuestProto->id, m_pOwner);
		}
	}

	// ��װ������ϵ���Ʒ����
	InitQuestItem();
}

VOID Quest::Init(const tagQuestSave* pQuestSave, Role* pRole, INT nIndex)
{
	ASSERT( P_VALID(pQuestSave) && P_VALID(pRole) );
	ASSERT( nIndex >= 0 && nIndex < QUEST_MAX_COUNT );

	m_pProto = g_questMgr.GetQuestProto(pQuestSave->u16QuestID);
	if( !P_VALID(m_pProto) ) return;

	m_pOwner = pRole;
	m_nIndex = nIndex;
	m_dwStartTime = pQuestSave->dwAcceptTime;
	m_pDynamicTarget = NULL;
	m_bComplete = FALSE;

	// ��ʼ����������״̬
	IFASTCODE->MemCpy(m_n16CreatureCount, pQuestSave->n16MonsterNum, sizeof(m_n16CreatureCount));
	IFASTCODE->MemCpy(m_ScriptData.dwData, pQuestSave->Data.dwData, sizeof(DWORD)*ESD_Quest);
	memset(m_bSpeakNPC, false, sizeof(m_bSpeakNPC));
	memset(m_bTrigger, false, sizeof(m_bTrigger));
	memset(m_n16ItemCount, 0, sizeof(m_n16ItemCount));
	memset(m_bInvest, false, sizeof(m_bInvest));

	// ��ʼ���ű�
	m_pScript = g_ScriptMgr.GetQuestScript(m_pProto->id);

	// ��ʼ������̬Ŀ��
	if(EQTM_DYNAMIC == m_pProto->target_mode)
	{
		m_pDynamicTarget = new tagQuestDynamicTarget;

		if(!P_VALID(m_pDynamicTarget))
			return;

		m_pDynamicTarget->eTargetType = pQuestSave->DynamicTarget.eTargetType;
		m_pDynamicTarget->dwQuestTipsID = pQuestSave->DynamicTarget.dwQuestTipsID;
		IFASTCODE->MemCpy(m_pDynamicTarget->dwTargetID, pQuestSave->DynamicTarget.dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(m_pDynamicTarget->nTargetNum, pQuestSave->DynamicTarget.nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
	}

	// ��װ������ϵ���Ʒ����
	InitQuestItem();
}

//--------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------
VOID Quest::Destroy()
{
	m_pOwner = NULL;
	m_pProto = NULL;
	m_dwStartTime = GT_INVALID;
	m_nIndex = GT_INVALID;

	m_pScript = NULL;
}

//--------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------
VOID Quest::InitQuestSave(OUT tagQuestSave* pQuestSave)
{
	pQuestSave->u16QuestID = GetID();
	pQuestSave->dwAcceptTime = GetAcceptTime();
	IFASTCODE->MemCpy(pQuestSave->n16MonsterNum, m_n16CreatureCount, sizeof(pQuestSave->n16MonsterNum));
	IFASTCODE->MemCpy(pQuestSave->Data.dwData, m_ScriptData.dwData, sizeof(DWORD)*ESD_Quest);

	if(P_VALID(m_pDynamicTarget))
	{
		pQuestSave->DynamicTarget.eTargetType = m_pDynamicTarget->eTargetType;
		pQuestSave->DynamicTarget.dwQuestTipsID = m_pDynamicTarget->dwQuestTipsID;
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.dwTargetID, m_pDynamicTarget->dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.nTargetNum, m_pDynamicTarget->nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);		
	}
}

//--------------------------------------------------------------------------------
// ������Ϣ�ṹ
//--------------------------------------------------------------------------------
VOID Quest::GenerateMsgInfo(tagIncompleteQuestMsgInfo* pInfo, DWORD &dwMsgSize, DWORD &dwQuestSize)
{
	ASSERT( IsValid() );
	dwQuestSize = sizeof(tagIncompleteQuestMsgInfo);

	pInfo->u16QuestID = m_pProto->id;
	pInfo->dwAcceptTime = m_dwStartTime;
	IFASTCODE->MemCpy(pInfo->n16CreatureNum, m_n16CreatureCount, sizeof(pInfo->n16CreatureNum));
	IFASTCODE->MemCpy(pInfo->n16ItemNum, m_n16ItemCount, sizeof(pInfo->n16ItemNum));

	if(EQTM_DYNAMIC == m_pProto->target_mode)
	{
		pInfo->DynamicTarget.eTargetType = m_pDynamicTarget->eTargetType;
		pInfo->DynamicTarget.dwQuestTipsID = m_pDynamicTarget->dwQuestTipsID;
		IFASTCODE->MemCpy(pInfo->DynamicTarget.dwTargetID, m_pDynamicTarget->dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pInfo->DynamicTarget.nTargetNum, m_pDynamicTarget->nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
	}
	else
	{
		// ������Ϣ��С
		dwQuestSize-= sizeof(tagQuestDynamicTarget); 
		dwMsgSize -= sizeof(tagQuestDynamicTarget);
	}
}

//---------------------------------------------------------------------------------
// �õ���ʼ��Ʒ�б�
//---------------------------------------------------------------------------------
VOID Quest::GetInitQuestItem(INT16* const n16ItemCount)
{
	ASSERT( IsValid() );

	IFASTCODE->MemCpy(n16ItemCount, m_n16ItemCount, sizeof(m_n16ItemCount));
}

//---------------------------------------------------------------------------------
// �õ���̬Ŀ��
//---------------------------------------------------------------------------------
VOID Quest::GetInitDynamicTarget(tagQuestDynamicTarget* pInfo, DWORD &dwSize)
{
	if(EQTM_DYNAMIC == m_pProto->target_mode)
	{
		pInfo->eTargetType = m_pDynamicTarget->eTargetType;
		pInfo->dwQuestTipsID = m_pDynamicTarget->dwQuestTipsID;
		IFASTCODE->MemCpy(pInfo->dwTargetID, m_pDynamicTarget->dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pInfo->nTargetNum, m_pDynamicTarget->nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
	}
	else
	{
		dwSize -= sizeof(tagQuestDynamicTarget);
	}
}

//---------------------------------------------------------------------------------
// ����Ҫ��������Ʒ�Ƿ��Ѿ��㹻
//---------------------------------------------------------------------------------
BOOL Quest::IsQuestItemFull(DWORD dwItemTypeID)
{
	if( FALSE == IsValid() ) return FALSE;

	// ���������û�л����Ʒ����ֱ�ӷ���
	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) ) return FALSE;

	// ���������Ʒ�Ƿ�����
	for(INT n = 0; n < QUEST_ITEMS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
			//break;

		if( m_pProto->complete_req_item[n] != dwItemTypeID )
			continue;

		return m_n16ItemCount[n] >= m_pProto->complete_req_item_num[n];
	}

	return FALSE;
}

//---------------------------------------------------------------------------------
// ���������Ƿ�������
//---------------------------------------------------------------------------------
BOOL Quest::IsCanComplete(INT nChoiceItemIndex, Creature* pNPC)
{
	BOOL bComplete = FALSE;
	if( m_pProto->only_one_condition )
	{
		bComplete = IsCanCompleteCreatureKill(TRUE)	||
					IsCanCompleteItem(TRUE)			||
					IsCanCompleteNPCTalk(TRUE)		||
					IsCanCompleteTrigger(TRUE)		||
					IsCanCompleteMoney(TRUE)		||
					IsCanCompleteLevel(TRUE)		||
//					IsCanCompleteInvest(TRUE)		||
					IsCanCompleteMap(TRUE);
	}
	else
	{
		bComplete = IsCanCompleteCreatureKill()	&&
					IsCanCompleteItem()			&&
					IsCanCompleteNPCTalk()		&&
					IsCanCompleteTrigger()		&&
					IsCanCompleteMoney()		&&
					IsCanCompleteLevel()		&&
//					IsCanCompleteInvest()		&&
					IsCanCompleteMap();
	}

	if( P_VALID(m_pScript) )
	{
		bComplete = bComplete && m_pScript->CheckComplete(m_pProto->id, m_pOwner, pNPC);
	}

	return bComplete && IsCanCompleteReward(nChoiceItemIndex);
}

//----------------------------------------------------------------------------------
// �¼�����
//----------------------------------------------------------------------------------
VOID Quest::OnEvent(EQuestEvent eQuestType, DWORD dwEventMisc1/*=0*/, DWORD dwEventMisc2/*=0*/, DWORD dwEventMisc3/*=0*/)
{
	switch(eQuestType)
	{
	case EQE_Kill:
		OnEventCreatureKill(dwEventMisc1, dwEventMisc2);
		break;

	case EQE_Item:
		OnEventItem(dwEventMisc1, dwEventMisc2, dwEventMisc3);
		break;

	case EQE_Talk:
		OnEventNPCTalk(dwEventMisc1, dwEventMisc2);
		break;

	case EQE_Trigger:
		OnEventTrigger(dwEventMisc1);
		break;

	case EQE_Invest:
		OnEventInvest(dwEventMisc1,dwEventMisc2);
		break;

	case EQE_DlgDefault:
		OnEventDlgDefault(dwEventMisc1);
		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------------
// ɱ���¼�
//------------------------------------------------------------------------------------
VOID Quest::OnEventCreatureKill(DWORD dwCreatureTypeID, INT nCreatureLevel)
{
	if( !P_VALID(m_pProto) ) return;

	// ���ýű�
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnCreatureKill(m_pProto->id, m_pOwner, dwCreatureTypeID);
	}

	// ��̬��ɱĿ�����
	if(P_VALID(m_pDynamicTarget) && EQTT_Kill == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			BOOL bFind = FALSE;
			if(m_pDynamicTarget->dwTargetID[i] == dwCreatureTypeID	  ||
				m_pDynamicTarget->dwTargetID[i+1] == dwCreatureTypeID ||
				m_pDynamicTarget->dwTargetID[i+2] == dwCreatureTypeID ||
				m_pDynamicTarget->dwTargetID[i+3] == dwCreatureTypeID)
			{
				m_n16CreatureCount[i] += 1;
				bFind = TRUE;
			}

			if( bFind )
			{
				// ���͸���
				tagNS_QuestUpdateKillCreature send;
				send.u16QuestID = GetID();
				send.nCreatureIndex = i;
				send.n16NewKillNum = m_n16CreatureCount[i];
				m_pOwner->SendMessage(&send, send.dwSize);

				// �ҵ��ˣ��Ͳ���Ҫ��������
				break;
			}
		}
			
		return;
	}

	// ���������û��ɱ�֣���ֱ�ӷ���
	if( !(m_pProto->quest_flags & Quest_Flag_KILL) ) return;
	
	for(INT n = 0; n < QUEST_CREATURES_COUNT; n++)
	{
		if( !P_VALID(m_pProto->complete_req_creature[n*3]) )
			break;

		BOOL bFind = FALSE;
		if( m_pProto->creature_level )
		{
			if( m_pProto->complete_req_creature[n*3]	== nCreatureLevel ||
				m_pProto->complete_req_creature[n*3+1]	== nCreatureLevel ||
				m_pProto->complete_req_creature[n*3+2]	== nCreatureLevel
			)
			{
				if( m_n16CreatureCount[n] < m_pProto->complete_req_creature_num[n] )
				{
					m_n16CreatureCount[n] += 1;
					bFind = TRUE;
				}
			}
		}
		else
		{
			if( m_pProto->complete_req_creature[n*3]	== dwCreatureTypeID ||
				m_pProto->complete_req_creature[n*3+1]	== dwCreatureTypeID ||
				m_pProto->complete_req_creature[n*3+2]	== dwCreatureTypeID
				)
			{
				if( m_n16CreatureCount[n] < m_pProto->complete_req_creature_num[n] )
				{
					m_n16CreatureCount[n] += 1;
					bFind = TRUE;
				}
			}
		}

		if( bFind )
		{
			// ���͸���
			tagNS_QuestUpdateKillCreature send;
			send.u16QuestID = GetID();
			send.nCreatureIndex = n;
			send.n16NewKillNum = m_n16CreatureCount[n];
			m_pOwner->SendMessage(&send, send.dwSize);

			// �ҵ��ˣ��Ͳ���Ҫ��������
			break;
		}
		
	}
}

//-------------------------------------------------------------------------------
// ��Ʒ�¼�
//-------------------------------------------------------------------------------
VOID Quest::OnEventItem(DWORD dwItemTypeID, INT nNum, BOOL bAdd)
{
	if( !P_VALID(m_pProto) ) return;
	if( nNum <= 0 ) return;
	if( m_bComplete ) return;

	// ��̬�ռ���Ʒ����
	if(P_VALID(m_pDynamicTarget) && EQTT_Collect == m_pDynamicTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			BOOL bFind = FALSE;
			if(m_pDynamicTarget->dwTargetID[i] == dwItemTypeID)
			{
				if( bAdd) m_n16ItemCount[i] += nNum;
				else	  m_n16ItemCount[i] -= nNum;

				bFind = TRUE;
			}

			if( bFind )
			{
				// ���͸���
				tagNS_QuestUpdateItem send;
				send.u16QuestID = GetID();
				send.nItemIndex = i;
				send.n16NewItemNum = m_n16ItemCount[i];
				m_pOwner->SendMessage(&send, send.dwSize);

				// �ҵ��ˣ��Ͳ���Ҫ��������
				break;
			}
		}

		return;
	}

	// ���������û�л����Ʒ����ֱ�ӷ���
	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) ) return;

	// �����Ҷ�Ӧ����Ʒ
	for(INT n = 0; n < QUEST_ITEMS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
			//break;

		BOOL bFind = FALSE;

		if( m_pProto->complete_req_item[n] == dwItemTypeID )
		{
			if( bAdd )	m_n16ItemCount[n] += nNum;
			else		m_n16ItemCount[n] -= nNum;

			bFind = TRUE;
		}

		if( bFind )
		{
			// ���͸���
			tagNS_QuestUpdateItem send;
			send.u16QuestID = GetID();
			send.nItemIndex = n;
			send.n16NewItemNum = m_n16ItemCount[n];
			m_pOwner->SendMessage(&send, send.dwSize);

			// �ҵ��ˣ��Ͳ���Ҫ��������
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// NPC�Ի��¼�
//-----------------------------------------------------------------------------
VOID Quest::OnEventNPCTalk(DWORD dwNPCID, DWORD dwNPCTypeID)
{
	if( !P_VALID(m_pProto) ) return;

	// ��̬NPC�Ի�ͳ��
	if(P_VALID(m_pDynamicTarget) && EQTT_NPCTalk == m_pDynamicTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			BOOL bFind = FALSE;

			if( m_pDynamicTarget->dwTargetID[i] == dwNPCTypeID )
			{
				if( !m_bSpeakNPC[i] )
				{
					m_bSpeakNPC[i] = true;
					bFind = TRUE;
				}
			}
			
			if( bFind )
			{
				// ���͸���
				tagNS_QuestUpdateNPCTalk send;
				send.u16QuestID = GetID();
				send.nIndex = i;
				send.dwErrorCode = E_Success;
				m_pOwner->SendMessage(&send, send.dwSize);

				// ����NPC�Ի��ű�
				if( P_VALID(m_pScript) )
				{
					m_pScript->OnNPCTalk(m_pProto->id, m_pOwner, dwNPCID, dwNPCTypeID);
				}

				// �ҵ��ˣ��Ͳ���Ҫ��������
				break;
			}
		}

		return;
	}

	// ���������û�л����Ʒ����ֱ�ӷ���
	if( !(m_pProto->quest_flags & Quest_Flag_NPC_TALK) ) return;

	// �����Ҷ�Ӧ����Ʒ
	for(INT n = 0; n < QUEST_NPC_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_npc[n]) )
			break;

		BOOL bFind = FALSE;

		if( m_pProto->complete_req_npc[n] == dwNPCTypeID )
		{
			if( !m_bSpeakNPC[n] )
			{
				m_bSpeakNPC[n] = true;
				bFind = TRUE;
			}
		}
		else
		{
			// ��NPC��ID��һ���������NPC֮ǰû�жԹ����������������
			if( m_pProto->only_in_order )
			{
				if( false == m_bSpeakNPC[n] )
				{
					tagNS_QuestUpdateNPCTalk send;
					send.u16QuestID = GetID();
					send.nIndex = n;
					send.dwErrorCode = E_CanUpdateQuest_Fail;
					m_pOwner->SendMessage(&send, send.dwSize);

					break;
				}
			}
		}

		if( bFind )
		{
			// ���͸���
			tagNS_QuestUpdateNPCTalk send;
			send.u16QuestID = GetID();
			send.nIndex = n;
			send.dwErrorCode = E_Success;
			m_pOwner->SendMessage(&send, send.dwSize);

			// ����NPC�Ի��ű�
			if( P_VALID(m_pScript) )
			{
				m_pScript->OnNPCTalk(m_pProto->id, m_pOwner, dwNPCID, dwNPCTypeID);
			}


			// �ҵ��ˣ��Ͳ���Ҫ��������
			break;
		}
	}

	// û���ҵ���Ҳ���ؿͻ���
	tagNS_QuestUpdateNPCTalk send;
	send.u16QuestID = GetID();
	send.nIndex = GT_INVALID;
	send.dwErrorCode = E_CanUpdateQuest_Fail;
	m_pOwner->SendMessage(&send, send.dwSize);
}

//--------------------------------------------------------------------------------
// �������¼�
//--------------------------------------------------------------------------------
VOID Quest::OnEventTrigger(DWORD dwTriggerSerial)
{
	if( !P_VALID(m_pProto) ) return;

	// ���������û�л����Ʒ����ֱ�ӷ���
	if( !(m_pProto->quest_flags & Quest_Flag_Trigger) ) return;

	// �����Ҷ�Ӧ����Ʒ
	for(INT n = 0; n < QUEST_TRIGGERS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_trigger[n]) )
			break;

		BOOL bFind = FALSE;

		if( m_pProto->complete_req_trigger[n] == dwTriggerSerial )
		{
			m_bTrigger[n] = true;
			bFind = TRUE;
		}

		if( bFind )
		{
			// ���͸���
			tagNS_QuestUpdateTrigger send;
			send.u16QuestID = GetID();
			send.nIndex = n;
			m_pOwner->SendMessage(&send, send.dwSize);

			// �ҵ��ˣ��Ͳ���Ҫ��������
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// ��������¼�
//--------------------------------------------------------------------------------
VOID Quest::OnEventInvest(DWORD dwNPCID, DWORD dwNPCTypeID)
{
	if( !P_VALID(m_pProto) ) return;

	// ��̬NPC�Ի�ͳ��
	if(P_VALID(m_pDynamicTarget) && EQTT_Invest == m_pDynamicTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			BOOL bFind = FALSE;

			if( m_pDynamicTarget->dwTargetID[i] == dwNPCTypeID )
			{
				if( !m_bInvest[i] )
				{
					m_bInvest[i] = true;
					bFind = TRUE;
				}
			}

			if( bFind )
			{
				// ���͸���
				tagNS_QuestUpdateInveset send;
				send.u16QuestID = GetID();
				send.nIndex = i;
				m_pOwner->SendMessage(&send, send.dwSize);

				if(P_VALID(m_pScript))
				{
					m_pScript->OnInvest(m_pProto->id, m_pOwner, dwNPCTypeID);
				}

				// �ҵ��ˣ��Ͳ���Ҫ��������
				break;
			}
		}

		return;
	}

	// ���������û�л����Ʒ����ֱ�ӷ���
	if( !(m_pProto->quest_flags & Quest_Flag_Invest) ) return;

	// �����Ҷ�Ӧ����Ʒ
	for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->investigate_objects[n]) && !P_VALID(m_pProto->investigate_objects[n+4]) )
			break;

		BOOL bFind = FALSE;

		if( m_pProto->investigate_objects[n] == dwNPCTypeID || m_pProto->investigate_objects[n+4] == dwNPCTypeID)
		{
			m_bInvest[n] = true;
			bFind = TRUE;
		}

		if( bFind )
		{
			// ���͸���
			tagNS_QuestUpdateInveset send;
			send.u16QuestID = GetID();
			send.nIndex = n;
			m_pOwner->SendMessage(&send, send.dwSize);

			if(P_VALID(m_pScript))
			{
				m_pScript->OnInvest(m_pProto->id, m_pOwner, dwNPCTypeID);
			}

			// �ҵ��ˣ��Ͳ���Ҫ��������
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// �������ɿضԻ���ȱʡ�¼�
//--------------------------------------------------------------------------------
VOID Quest::OnEventDlgDefault(DWORD dwDlgOption)
{
	if(!P_VALID(m_pScript))		return;

	m_pScript->OnDlgDefault(m_pProto->id, m_pOwner, dwDlgOption);
}

//--------------------------------------------------------------------------------
// ��ʼ����������Ҫ����Ʒ
//--------------------------------------------------------------------------------
VOID Quest::InitQuestItem()
{
	if( !P_VALID(m_pProto) ) return;

	// �ж϶�̬�ռ�����Ʒ
	if (P_VALID(m_pDynamicTarget) && EQTT_Collect == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->nTargetNum[i] == 0)
				break;

			m_n16ItemCount[i]  = m_pOwner->GetItemMgr().GetBagSameItemCount(m_pDynamicTarget->dwTargetID[i])
				+ m_pOwner->GetItemMgr().GetQuestBagSameItemCount(m_pDynamicTarget->dwTargetID[i]);
		}

		return;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) )
		return;

	for(INT n = 0; n < QUEST_ITEMS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_item[n]) )
			break;

		m_n16ItemCount[n] = m_pOwner->GetItemMgr().GetBagSameItemCount(m_pProto->complete_req_item[n])
						+ m_pOwner->GetItemMgr().GetQuestBagSameItemCount(m_pProto->complete_req_item[n]);
	}
}

//----------------------------------------------------------------------------------
// ɱ���Ƿ�����
//----------------------------------------------------------------------------------
BOOL Quest::IsCanCompleteCreatureKill(BOOL bOnlyOne)
{
	// �ж϶�̬��ɱĿ��
	if (P_VALID(m_pDynamicTarget) && EQTT_Kill == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->nTargetNum[i] == 0)
				continue;

			if(m_n16CreatureCount[i] < m_pDynamicTarget->nTargetNum[i])
				return FALSE;
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_KILL) )
		return (bOnlyOne ? FALSE : TRUE);

	// ����һ���Ϳ���
	if( m_pProto->only_one_creature )
	{
		for(INT n = 0; n < QUEST_CREATURES_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_creature[n*3]) )
				break;

			if( m_n16CreatureCount[n] >= m_pProto->complete_req_creature_num[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// ȫ��
	else
	{
		for(INT n = 0; n < QUEST_CREATURES_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_creature[n*3]) )
				break;

			if( m_n16CreatureCount[n] < m_pProto->complete_req_creature_num[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// ��Ʒ�Ƿ�����
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteItem(BOOL bOnlyOne)
{
	// �ж϶�̬�ռ�����Ʒ
	if (P_VALID(m_pDynamicTarget) && EQTT_Collect == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->nTargetNum[i] == 0)
				continue;

			if(m_n16ItemCount[i] < m_pDynamicTarget->nTargetNum[i])
				return FALSE;
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) )
		return (bOnlyOne ? FALSE : TRUE);

	// ����һ���Ϳ���
	if( m_pProto->only_one_item )
	{
		for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
				//break;

			if( m_n16ItemCount[n] >= m_pProto->complete_req_item_num[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// ȫ��
	else
	{
		for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
				//break;

			if( m_n16ItemCount[n] < m_pProto->complete_req_item_num[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// NPC�Ƿ�����
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteNPCTalk(BOOL bOnlyOne)
{
	if (P_VALID(m_pDynamicTarget) && EQTT_NPCTalk == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->dwTargetID[i] == 0)
				continue;

			if( !m_bSpeakNPC[i])
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_NPC_TALK) )
		return (bOnlyOne ? FALSE : TRUE);

	// ����һ���Ϳ���
	if( m_pProto->only_one_npc )
	{
		for(INT n = 0; n < QUEST_NPC_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_npc[n]) )
				break;

			if( m_bSpeakNPC[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// ȫ��
	else
	{
		for(INT n = 0; n < QUEST_NPC_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_npc[n]) )
				break;

			if( !m_bSpeakNPC[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// �������Ƿ�����
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteTrigger(BOOL bOnlyOne)
{
	if( !(m_pProto->quest_flags & Quest_Flag_Trigger) )
		return (bOnlyOne ? FALSE : TRUE);

	// ����һ���Ϳ���
	if( m_pProto->only_one_trigger )
	{
		for(INT n = 0; n < QUEST_TRIGGERS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_trigger[n]) )
				break;

			if( m_bTrigger[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// ȫ��
	else
	{
		for(INT n = 0; n < QUEST_TRIGGERS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_trigger[n]) )
				break;

			if( !m_bTrigger[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// ��������Ƿ�����
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteInvest(BOOL bOnlyOne)
{
	if (P_VALID(m_pDynamicTarget) && EQTT_Invest == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->dwTargetID[i] == GT_INVALID)
				continue;

			if( !m_bInvest[i])
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_Invest) )
		return (bOnlyOne ? FALSE : TRUE);

	for(INT n = 0; n < DYNAMIC_TARGET_COUNT; n++)
	{
		if( !P_VALID(m_pProto->investigate_objects[n]) && !P_VALID(m_pProto->investigate_objects[n+4]) )
			break;

		if( !m_bInvest[n] )
		{
			return FALSE;
		}	
	}
	return TRUE;
}

//----------------------------------------------------------------------------
// ��Ǯ
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteMoney(BOOL bOnlyOne)
{
	if( 0 == m_pProto->complete_req_money )
		return (bOnlyOne ? FALSE : TRUE);

	if( m_pOwner->GetCurMgr().GetBagSilver() < m_pProto->complete_req_money )
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------------
// �ȼ�
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteLevel(BOOL bOnlyOne)
{
	if( 0 == m_pProto->complete_req_level )
		return (bOnlyOne ? FALSE : TRUE);

	if( m_pOwner->GetLevel() < m_pProto->complete_req_level )
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------------
// ��ͼ
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteMap(BOOL bOnlyOne)
{
	// todo:
	return TRUE;
}

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteReward(INT nChoiceItemIndex)
{
	INT16 n16BagSpace = 0;
	INT16 n16QuestSpace = 0;
	INT16 n16TmpBagSpace = 0;
	INT16 n16TmpQuestSpace = 0;
	INT16 n16TmpMaxDummy;
	
	// �鿴��������Ʒ�����ܷ�ŵ���
	for(INT n = 0; n < QUEST_REW_ITEM; ++n)
	{
		if( !P_VALID(m_pProto->rew_item[n]) )
			break;

		// �õ������Ʒ���ڱ�����ռ�õĸ���
		if(!ItemMgr::CalSpaceUsed(m_pProto->rew_item[n], 
			m_pProto->rew_item_num[n], n16TmpBagSpace, n16TmpQuestSpace, n16TmpMaxDummy))
		{
			return FALSE;
		}

		n16BagSpace		+= n16TmpBagSpace;
		n16QuestSpace	+= n16TmpQuestSpace;
	}

	// ����п�ѡ��������鿴������Ʒ�е�������
	INT nTmp = nChoiceItemIndex;
	if(nTmp >= 0 && nTmp < QUEST_REW_ITEM
		&& P_VALID(m_pProto->rew_choice_item[nTmp]))
	{
		// �õ������Ʒ���ڱ�����ռ�õĸ���
		if(!ItemMgr::CalSpaceUsed(m_pProto->rew_choice_item[nTmp], 
			m_pProto->rew_choice_item_num[nTmp], n16TmpBagSpace, n16TmpQuestSpace, n16TmpMaxDummy))
		{
			return FALSE;
		}

		n16BagSpace		+= n16TmpBagSpace;
		n16QuestSpace	+= n16TmpQuestSpace;
	}

	// �õ���������е���λ�ܷ������Щ��Ʒ
	if( m_pOwner->GetItemMgr().GetBagFreeSize() < n16BagSpace
		|| m_pOwner->GetItemMgr().GetQuestItemBagFreeSize() < n16QuestSpace)
	{
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------------
// �����ȡʱ
//---------------------------------------------------------------------------------
VOID Quest::OnAccept(Creature* pNPC)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnAccept(m_pProto->id, m_pOwner, pNPC);
	}
}

//----------------------------------------------------------------------------------
// �������ʱ
//----------------------------------------------------------------------------------
VOID Quest::OnComplete(Creature* pNPC)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnComplete(m_pProto->id, m_pOwner, pNPC);
	}
}

//-----------------------------------------------------------------------------------
// ����ȡ��ʱ
//-----------------------------------------------------------------------------------
VOID Quest::OnCancel()
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnCancel(m_pProto->id, m_pOwner);
	}
}

