#pragma once

class State_NPC;
class NetCmdHandler_NPC;
class GameEventHandler_NPC;
class NPC;
struct tagHitTargetEvent;

/** \class FSM_NPC
	\brief NPC״̬��
*/
class FSM_NPC
{
public:
	enum EState					//״̬����
	{
		EState_Idle,			//����
		EState_MoveByCollide,	//������ײ���ƶ�
		EState_MoveByTile,		//���ڸ��ӵ��ƶ�
		EState_Skill,			//ʹ�ü���
		EState_Dead,			//����
		EState_BeAttack,		//������
		EState_HitFly,			//����
		EState_Repel,			//����
		EState_Assault,			//���
		EState_Pull,			//��ק
		EState_Dazzy,			//����
		EState_Tie,				//����
		EState_Sopor,			//��˯
		EState_Num		
	};

private:
	enum EGameEventHandler		//��Ϸ�¼�������
	{
		EGameEventHandler_HitTarget,//����Ŀ��
		EGameEventHandler_Num,
	};

public:
	FSM_NPC(NPC* pNPC);
	virtual ~FSM_NPC(void);

	void Init();
	void Destroy();
	void Update();
	void OnNetCmd(tagNetCmd* pNetCmd);
	void OnGameEvent(tagGameEvent* pEvent);

	void Change2State(tagNetCmd* pNetCmd);
	void Change2State(tagGameEvent* pEvent);
	void Change2IdleState();
	void Change2DeadState();
	void Change2State(EState state,tagNetCmd* pNetCmd);

	bool IsDeadState(){ return m_activeState==EState_Dead;}
    bool IsHitFlyState(){ return m_activeState==EState_HitFly;}
	bool IsSpecailMoveState() { return  m_activeState==EState_Assault ||
										m_activeState==EState_Repel ||
										m_activeState==EState_Pull; }

	tagHitTargetEvent* GetLastHitTargetEvent(DWORD roleID);

	NPC* GetNPC(){ return m_pNPC;}

    void SetNSDead(bool bDead) { m_bNSDead = bDead; }
    bool bNSDead() { return m_bNSDead; }

private:
	EState							m_activeState;			//��ǰ״̬
	State_NPC*						m_states[EState_Num];	//����״̬
	vector<NetCmdHandler_NPC*>		m_netCmdHandlers;		//������Ϣ������
	GameEventHandler_NPC*			m_gameEventHandlers[EGameEventHandler_Num];	//��Ϸ�¼�������
	NPC*							m_pNPC;					//״̬��������ɫ
	TObjRef<Util>					m_pUtil;

    bool                            m_bNSDead;              //�յ�������������Ϣ
};
