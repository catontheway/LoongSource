//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_quest.h
// author: 
// actor:
// data: 2008-09-11
// last:
// brief: �������
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------


#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/QuestDef.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ��ȡ����
//-----------------------------------------------------------------------------
CMD_START(NDBC_AcceptQuest)						// ��ȡ���� World ---> DB
	DWORD			dwRoleID;					// ��ɫID
	tagAcceptQuest  acceptQuest;	
CMD_END;

CMD_START(NDBS_AcceptQuest)						// ��ȡ���� World <--- DB ��ʱû�з���ֵ
CMD_END;

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
CMD_START(NDBC_CompleteQuest)					// ������� World ---> DB
	DWORD				dwRoleID;				// ��ɫID
	tagQuestDoneSave	QuestDone;
CMD_END;

CMD_START(NDBS_CompleteQuest)					// ������� World <--- DB ��ʱû�з���ֵ
CMD_END;

//------------------------------------------------------------------------------
// ɾ������
//------------------------------------------------------------------------------
CMD_START(NDBC_DiscardQuest)					// �������� World ---> DB
	DWORD	dwRoleID;							// ��ɫID
	UINT16	u16QuestID;							// ����ID
CMD_END;

CMD_START(NDBS_DiscardQuest)					// �������� World <--- DB
CMD_END;

#pragma pack(pop)
