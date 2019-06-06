//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: exit_game_handle.cpp
// author: twan
// actor:
// data: 2009-12-7
// last:
// brief: ������ʾ
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "script_mgr.h"

#include "../WorldDefine/msg_player_preventlost.h"

//------------------------------------------------------------------------------------
// ������ʾ
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLeaveNotify(tagNetCmd *pCmd)
{
	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��ȡ�ȼ���Ӧ�ĵ��ε��㾭��
	INT nExp = g_ScriptMgr.GetRoleScript()->OnGetRoleFishingExp(pRole);

	BOOL bDoubleQuest = pRole->IsHaveDoneQuest(DOUBLE_EXP_QUEST_ID);
	BOOL bFishingQuest = pRole->IsHaveDoneQuest(FISHING_QUEST_ID);

	// ��ҵȼ�С��10��
	if (pRole->GetLevel() < 10)
	{
		bDoubleQuest = TRUE;					// С��10��û��˫���͵�������
		bFishingQuest = TRUE;
	}

	if (pRole->GetLevel() < 20)
	{
		bFishingQuest = TRUE;					// С��20��û�е�������
	}

	tagNS_ExitGamePrompt send;
	send.bDualExpQuestDo = bDoubleQuest;		// ˫����������
	send.bFishQuestDo = bFishingQuest;			// ��������
	send.nExprence = 120 * nExp;				// 1Сʱ���㾭��

	SendMessage(&send, send.dwSize);
	
	return E_Success;
}