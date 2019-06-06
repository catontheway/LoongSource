/********************************************************************
	created:	2008/09/14
	created:	14:9:2008   14:20
	filename: 	d:\loong_client\Client\QuestEvent.h
	file path:	d:\loong_client\Client
	file base:	QuestEvent
	file ext:	h
	author:		leili
	
	purpose:	����ϵͳ��Ϸ�¼�����
*********************************************************************/
#pragma once

#pragma pack(push, 1)

struct tagStartNPCTalkEvent : public tagGameEvent
{
	DWORD	globalID;	// NPCȫ��ID
	DWORD	typeID;		// NPC����ID

	tagStartNPCTalkEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, globalID(GT_INVALID)
		, typeID(GT_INVALID)
	{ } 
};

struct tagUseQuestEvent : public tagGameEvent
{
	DWORD	itemID;		// ��ƷTypeID
	UINT16	questID;	// ������

	tagUseQuestEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, itemID(GT_INVALID)
		, questID(GT_INVALID)
	{ }
};

#pragma pack(pop)