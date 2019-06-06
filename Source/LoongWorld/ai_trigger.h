//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: ai_trigger.h
// author: wjiang
// actor:
// data: 2009-6-25
// last:
// brief: ����AI������
//-----------------------------------------------------------------------------
#pragma once

class Unit;
class Creature;
class AITriggerScript;
struct tagTriggerProto;
enum ECreatureTargetFriendEnemy;


class AITrigger
{
public:
	AITrigger();
	~AITrigger() {}

public:
	BOOL		Init(DWORD dwCreatureTypeID, DWORD dwTypeID, INT dwTargetType, DWORD dwSkillID=GT_INVALID);
	VOID		Refresh();
	VOID		SetTriggerActive(INT eEventType);
	VOID		OnEvent(INT eEventType, Creature* pOwner);
	VOID		Update(Creature* pOwner);

private:
	VOID		SetActive(BOOL bActive) { m_bActive = bActive; }
	BOOL		TestTimeIntervalTrigger();
	BOOL		TestEventTrigger(Creature* pOwner);
	BOOL		TestStateTrigger(Creature* pOwner, DWORD& dwTargetID);
	VOID		OnTrigger(Creature* pOwner, DWORD dwTargetID);

	BOOL						m_bActive;			// �������Ƿ񱻼���
	INT							m_nActiveTick;		// ����ʱ��
	DWORD						m_dwTriggeredTimes;	// �Ѵ����Ĵ���
	DWORD						m_dwSkillID;		// ����ID
	ECreatureTargetFriendEnemy	m_eTargeType;		// Ŀ������
	const tagTriggerProto*		m_pProto;
};

class AITriggerMgr
{
public:
	AITriggerMgr():m_pOwner(NULL), m_bPaused(FALSE) {}
	~AITriggerMgr();

public:
	BOOL		Init(Creature* pOwner, const tagCreatureAI* m_pAIProto);
	VOID		SetTriggerActive(INT eEventType);
	VOID		Refresh();
	VOID		Update();
	VOID		OnEvent(INT eEventType);

	VOID		Pause()			{ m_bPaused = TRUE; }
	VOID		UnPause()		{ m_bPaused = FALSE; }
	BOOL		IsPaused()		{ return m_bPaused; }

public:
	Creature*					m_pOwner;			 
	BOOL						m_bPaused;
	std::list<AITrigger*>		m_listTrigger;
};