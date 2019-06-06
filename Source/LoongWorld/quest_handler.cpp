//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: character_handler.cpp
// author: lh
// actor:
// data: 2008-08-31
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- �������
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/quest.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/msg_script.h"
#include "../WorldDefine/ScriptMsgInfo.h"

#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "quest.h"
#include "quest_mgr.h"
#include "script_mgr.h"
#include "creature.h"
#include "activity_mgr.h"

//------------------------------------------------------------------------------------
// ��ȡ����
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleNPCAcceptQuest(tagNetCmd* pCmd)
{
	if (P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_NPCAcceptQuest* pRecv = (tagNC_NPCAcceptQuest*)pCmd;
	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT nRet = pRole->AcceptQuestFromNPC(pRecv->u16QuestID, pRecv->dwNPCID);

	tagNS_AcceptQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------------------
// ͨ��������������
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleTriggerAcceptQuest(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_TriggerAcceptQuest* pRecv = (tagNC_TriggerAcceptQuest*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT nRet = pRole->AcceptQuestFromMapTrigger(pRecv->u16QuestID, pRecv->dwTriggerID);

	tagNS_AcceptQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------------
// �Ƿ������ɴ�����
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCompleteQuest(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_CompleteQuest* pRecv = (tagNC_CompleteQuest*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	UINT16 u16NexitQuestID = 0;

	INT nRet = pRole->CompleteQuest(pRecv->u16QuestID, pRecv->dwNPCID, pRecv->nRewChoicesItemIndex, u16NexitQuestID);

	// ���ͷ���
	tagNS_CompleteQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	// ����Ƿ��к����Զ���ȡ������
	if(E_Success == nRet && u16NexitQuestID != 0)
		pRole->AcceptQuestFromNPC(u16NexitQuestID, GT_INVALID);

	return 0;
}

//------------------------------------------------------------------------------------
// �Ƿ����ɾ��������
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDeleteQuest(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_DeleteQuest* pRecv = (tagNC_DeleteQuest*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT nRet = pRole->DeleteQuest(pRecv->u16QuestID);

	// ���ͷ�����Ϣ
	tagNS_DeleteQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------------
// ����NPC�Ի�����״̬
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUpdateQuestNPCTalk(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_UpdateQuestNPCTalk* pRecv = (tagNC_UpdateQuestNPCTalk*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	pRole->UpdateQuestNPCTalk(pRecv->u16QuestID, pRecv->dwNPCID);

	return 0;
}

//-----------------------------------------------------------------------------
// �ͻ��˷�������ĶԻ���ȱʡ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDlgDefaultMsg(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DlgDefaultMsg);

	Role* pRole = GetRole();

	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	switch (pRecv->eDlgTarget)
	{
	case EMUT_DlgTarget_Quest:
		break;
	case EMUT_DlgTarget_Item:
		break;
	case EMUT_DlgTarget_Creature:
		{
			DWORD			dwNpcID		= pRecv->dwTargetID;
			EDlgOption		eOption		= pRecv->eDlgOption;
			MTRANS_ELSE_RET(pMap,	pRole->GetMap(),				Map,			GT_INVALID);
			MTRANS_ELSE_RET(pNpc,	pMap->FindCreature(dwNpcID),	Creature,		GT_INVALID);
			MTRANS_ELSE_RET(pScript,pNpc->GetScript(),				CreatureScript,	GT_INVALID);
			pScript->OnTalk(pNpc, pRole, (INT)eOption);
		}
		break;
	default:
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// �ͻ��˴����������ű���ȱʡ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDefaultRequest(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DefaultRequest);

	Role* pRole = GetRole();

	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	switch (pRecv->eDlgTarget)
	{
		case EMUT_Request_Activity:
			{
				CActivity* pActivity = g_activityMgr.GetActivity(pRecv->dwTargetID);
				if( !P_VALID(pActivity) ) return GT_INVALID;

				const ActScript* pScript = pActivity->GetScript();
				if( !P_VALID(pActivity) ) return GT_INVALID;

				pScript->OnDefaultRequest(pRecv->dwTargetID, pRole, pRecv->dwEventType);
			}
			break;

		default:
			break;
	}

	return 0;
}

