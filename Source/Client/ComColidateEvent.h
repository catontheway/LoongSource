#pragma once

#pragma pack(push, 1)

//�򿪺ϳɽ���ķ�ʽ
enum EStartCompose
{
	EStartCompose_NULL = 0,
	EStartCompose_Skill,
	EStartCompose_NPC,	
	EStartCompose_Item,
};

//
typedef union
{
	DWORD			npcID;				// NPCȫ��id 
	DWORD			skillID;			// ����id
	INT64			itemID;				// ��Ʒ64id
}ComposeID;

struct tagStartComposeUIEvent 
	: public tagGameEvent
{
	EStartCompose	eStartCompose;	
	ComposeID		uComposeID;

	tagStartComposeUIEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
	{ 
	} 
};

#pragma pack(pop)