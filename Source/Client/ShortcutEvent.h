#pragma once

#pragma pack(push, 1)

enum EShortcutUseSkillState 
{
	ESUSS_NULL = 0,
	ESUSS_Cache,				//���漼��
	ESUSS_CacheUsed,			//���漼���Ѿ�ʹ��
};

struct tagShortcutCacheEvent : public tagGameEvent
{
	DWORD					dwSkillID;		//����ID
	EShortcutUseSkillState	eType;			//״̬
	tagShortcutCacheEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName, pSenderFrame), dwSkillID( GT_INVALID ){}
};

#pragma pack(pop)