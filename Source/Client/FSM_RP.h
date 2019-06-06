#pragma once
#include "ActionState.h"

class LowerState_RP;
class UpperState_RP;
class ActionState_RP;
class NetCmdHandler_RP;
class GameEventHandler_RP;
class GlobalState_RP;
class Player;
struct tagHitTargetEvent;

/** \class FSM_RP
	\brief Զ�����״̬��
*/
class FSM_RP
{
public:
	enum ELowerState					//����״̬
	{
		ELowerState_None=-1,
		ELowerState_Move=0,				//�ƶ�
		ELowerState_Swim,				//��Ӿ
		ELowerState_WaterWalk,			//ˮ������
		ELowerState_Stand,				//½��վ��
		ELowerState_Float,				//ˮ��Ư��
		ELowerState_WaterStand,			//ˮ��վ��
		ELowerState_Jump,				//��Ծ 
		ELowerState_Drop,				//����
		ELowerState_VDrop,				//��ֱ����	
		ELowerState_Slide,				//����
		ELowerState_HitFly,				//����
		ELowerState_Repel,				//����
		ELowerState_Assault,			//���
		ELowerState_Pull,				//��ק
		ELowerState_Num
	};

	enum EUpperState					//����״̬
	{
		EUpperState_None=-1,
		EUpperState_Idle=0,				//����
		EUpperState_Skill,				//ʹ�ü���
		EUpperState_UseItem,			//ʹ����Ʒ
		EUpperState_Dead,				//����
		EUpperState_Dazzy,				//����
		EUpperState_Tie,				//����
		EUpperState_Sopor,				//��˯
		EUpperState_StyleAction,		//���Զ���
		EUpperState_BeAttack,			//������
		EUpperState_Num
	};

	enum EGlobalState					//ȫ��״̬
	{
		EGlobalState_Combat,			//ս��״̬
		EGlobalState_Num
	};

private:
	enum EGameEventHandler				//��Ϸ�¼�������
	{
		EGameEventHandler_HitTarget,	//����Ŀ��
		EGameEventHandler_Num,
	};

public:
	FSM_RP(Player* pPlayer);
	virtual ~FSM_RP(void);

	//--��������------------------------------------------------------------------------------
	void Init();
	void Destroy();
	void Update();
	void OnNetCmd(tagNetCmd* pNetCmd);
	void OnGameEvent(tagGameEvent* pEvent);

	//--����״̬�л�---------------------------------------------------------------------------
	void Change2LowerState(ELowerState state,tagNetCmd* pNetCmd);
	void Change2LowerState(tagNetCmd* pNetCmd);
	void Change2LowerState_Stand();
	void Change2LowerState_Float();
	void Change2LowerState_WaterStand();
	void Change2LowerState_AutoStand();

	//--����״̬�л�--------------------------------------------------------------------------
	void Change2UpperState(EUpperState state,tagNetCmd* pNetCmd);
	void Change2UpperState(tagNetCmd* pNetCmd);
	void Change2UpperState_Idle();
	void Change2UpperState_Dead(bool bPlayAni);
	bool TryChange2UpperState(EUpperState state,tagGameEvent* pEvent);

	//--����״̬�л�---------------------------------------------------------------------------
	void Change2ActionState(EActionState state);
	EActionState GetActionState(){ return m_activeActionState;	}

	//--���������---------------------------------------------------------------------------
	bool IfCanPlayLowerAni();
	void ReplayLowerAni();
	bool IfLowerCanSetYaw();

	//--ȫ��״̬�л�---------------------------------------------------------------------------
	void ActiveGlobalState(EGlobalState state,tagNetCmd* pNetCmd,DWORD dwParam);

	//--���״̬-------------------------------------------------------------------------------
	bool IsReadyState();
	bool IsStandState()		{ return m_activeLowerState==ELowerState_Stand;	}
	bool IsFloatState()		{ return m_activeLowerState==ELowerState_Float;	}
	bool IsWaterStandState(){ return m_activeLowerState==ELowerState_WaterStand;}
	bool IsDeadState()		{ return m_activeUpperState==EUpperState_Dead;	}
	bool IsUpperIdleState()	{ return m_activeUpperState==EUpperState_Idle;	}
	bool IsSpecailMoveState() { return  m_activeLowerState==ELowerState_Assault ||
										m_activeLowerState==ELowerState_Repel ||
										m_activeLowerState==ELowerState_Pull; }
	bool IsStyleActionState() { return m_activeUpperState==EUpperState_StyleAction; }
	//--
	tagHitTargetEvent* GetLastHitTargetEvent(DWORD roleID);
	Player* GetPlayer(){ return m_pPlayer;}

private:
	ELowerState					m_activeLowerState;							//��ǰ����״̬
	EUpperState					m_activeUpperState;							//��ǰ����״̬
	EActionState				m_activeActionState;						//��ǰ����״̬

	LowerState_RP*				m_lowerStates[ELowerState_Num];				//��������״̬
	UpperState_RP*				m_upperStates[EUpperState_Num];				//��������״̬
	ActionState_RP*				m_actionStates[EActionState_Num];			//���ж���״̬
	GlobalState_RP*				m_globalStates[EGlobalState_Num];			//����ȫ��״̬

	vector<NetCmdHandler_RP*>	m_netCmdHandlers;							//������Ϣ������
	GameEventHandler_RP*		m_gameEventHandlers[EGameEventHandler_Num];	//��Ϸ�¼�������

	Player*						m_pPlayer;									//״̬��������ɫ
	TObjRef<Util>				m_pUtil;
};
