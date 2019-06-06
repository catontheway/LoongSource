#pragma once
#include "CombatEvent.h"


class FSM_LP;
/** \class GameEventHandler_LP
	\brief ���������Ϸ�¼�������
*/
class GameEventHandler_LP
{
public:
	GameEventHandler_LP(void);
	virtual ~GameEventHandler_LP(void);
	void SetFSM(FSM_LP* pFSM){m_pFSM=pFSM;}

	/** ��Ϸ�¼�����
		\remark ����false��ʾ��Ϸ�¼�����Ҫ�ٽ���״̬ȥ������
	*/
	virtual bool OnGameEvent(tagGameEvent* pEvent) =0;
	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;
protected:
	FSM_LP*					m_pFSM;
};



/** \class HitTargetEventHandler_LP
	\brief �����������Ŀ����Ϸ�¼�������
*/
class HitTargetEventHandler_LP : public GameEventHandler_LP
{
public:
	HitTargetEventHandler_LP(void);
	virtual ~HitTargetEventHandler_LP(void);

	//--GameEventHandler_LP
	virtual bool OnGameEvent(tagGameEvent* pEvent);
	virtual void OnNetCmd(tagNetCmd* pNetCmd);

	//--
	tagHitTargetEvent* GetLastHitTargetEvent(DWORD roleID);
protected:
	map<DWORD,tagHitTargetEvent> m_eventMap;
};





/** \class UseItemEventHandler_LP
	\brief ����ʹ����Ʒ��Ϸ�¼�������
*/
class UseItemEventHandler_LP : public GameEventHandler_LP
{
public:
	UseItemEventHandler_LP(void);
	virtual ~UseItemEventHandler_LP(void);

	//--GameEventHandler_LP
	virtual bool OnGameEvent(tagGameEvent* pEvent);
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
};
