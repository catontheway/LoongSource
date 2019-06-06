#pragma once

#pragma pack(push, 1)

/** ����ĳ����
*/
struct tagEnterMapAreaEvent : public tagGameEvent
{
	EMapArea	eAreaType;
	DWORD		mapAreaID;

	tagEnterMapAreaEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, eAreaType(EMA_Null),mapAreaID(GT_INVALID)
	{ } 
};

/** �뿪ĳ����
*/
struct tagLeaveMapAreaEvent : public tagGameEvent
{
	EMapArea	eAreaType;
	DWORD		mapAreaID;

	tagLeaveMapAreaEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, eAreaType(EMA_Null),mapAreaID(GT_INVALID)
	{ } 
};

/**	����ĳ������
*/
struct tagEnterTriggerEvent : public tagGameEvent
{
	DWORD	triggerID;

	tagEnterTriggerEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, triggerID(GT_INVALID)
	{ }
};

#pragma pack(pop)