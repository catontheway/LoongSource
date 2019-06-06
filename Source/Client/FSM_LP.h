#pragma once
#include "..\WorldDefine\msg_combat.h"
#include "ActionState.h"

class LocalPlayer;
class LowerState_LP;
class UpperState_LP;
class GlobalState_LP;
class ActionState_LP;
class NetCmdHandler_LP;
class GameEventHandler_LP;
class UpperStateUseItem_LP;

struct tagPreState;
struct tagHitTargetEvent;

/** \class FSM_LP
	\brief �������״̬��
*/
class FSM_LP
{
public:
	enum ELowerState					//����״̬
	{
		ELowerState_None=-1,
		ELowerState_MouseMove=0,		//����ƶ�
		ELowerState_MouseSwim,			//�����Ӿ
		ELowerState_MouseWaterWalk,		//���ˮ������
		ELowerState_KeyboardMove,		//�����ƶ�
		ELowerState_KeyboardSwim,		//������Ӿ
		ELowerState_KeyboardWaterWalk,	//����ˮ������
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
		ELowerState_Pursue,				//׷��
		ELowerState_Num
	};

	enum EUpperState					//����״̬
	{
		EUpperState_None=-1,
		EUpperState_Idle=0,				//����
		EUpperState_Skill,				//ʹ�ü���
		EUpperState_UseItem,			//ʹ����Ʒ
		EUpperState_Dead,				//����
		EUpperState_TalkNPC,			//��NPC�Ի�
		EUpperState_Follow,				//����
		EUpperState_Dazzy,				//����
		EUpperState_Tie,				//����
		EUpperState_Sopor,				//��˯
		EUpperState_StyleAction,		//���Զ���
		EUpperState_PickupItem,			//ʰȡ
		EUpperState_BeAttack,			//������
		EUpperState_Num
	};

private:
	enum EGlobalState					//ȫ��״̬
	{
		EGlobalState_Combat,			//ս��״̬
		EGlobalState_Num
	};

	enum EGameEventHandler				//��Ϸ�¼�������
	{
		EGameEventHandler_HitTarget,	//����Ŀ��
		EGameEventHandler_UseItem,		//ʹ����Ʒ
		EGameEventHandler_Num,
	};

public:
	FSM_LP(LocalPlayer* pLocalPlayer);
	virtual ~FSM_LP(void);

	//--��������------------------------------------------------------------------------------
	void Init();
	void Destroy();
	void Update();
	void OnGameEvent(tagGameEvent* pEvent);
	void OnNetCmd(tagNetCmd* pNetCmd);

	//--����״̬�л�---------------------------------------------------------------------------
	void Change2LowerState(ELowerState state,tagGameEvent* pEvent,tagPreState* pPreState=NULL);
	void Change2LowerState(ELowerState state,tagNetCmd* pNetCmd);
	void Change2LowerState_Stand();
	void Change2LowerState_Float();
	void Change2LowerState_WaterStand();
	void Change2LowerState_AutoStand();
	bool TryChange2LowerState(ELowerState state,tagGameEvent* pEvent,tagPreState* pPreState=NULL);

	//--����״̬�л�--------------------------------------------------------------------------
	void Change2UpperState(EUpperState state,tagGameEvent* pEvent);
	void Change2UpperState(EUpperState state,tagNetCmd* pNetCmd);
	void Change2UpperState_Idle();
	void Change2UpperState_Dead(bool bPlayAni,ERoleReviveType reviveType=ERRT_None);
	bool TryChange2UpperState(EUpperState state,tagGameEvent* pEvent);

	//--����״̬�л�---------------------------------------------------------------------------
	void Change2ActionState(EActionState state);
	EActionState GetActionState(){ return m_activeActionState;	}

	//--���������---------------------------------------------------------------------------
	bool IfCanPlayLowerAni();
	void ReplayLowerAni();
	bool IfLowerCanSetYaw();

	//--��õ�ǰ״̬---------------------------------------------------------------------------
	bool IsReadyState();
	bool IsCombatState();
	bool IsSkillState()		{ return m_activeUpperState==EUpperState_Skill;	}
	bool IsUpperState_UseItem(){ return m_activeUpperState == EUpperState_UseItem; }
	bool IsDeadState()		{ return m_activeUpperState==EUpperState_Dead;	}
	bool IsSwimState();
	bool IsOnWaterState();	
	bool IsStandState()		{ return m_activeLowerState==ELowerState_Stand;	}
	bool IsFloatState()		{ return m_activeLowerState==ELowerState_Float;	}
	bool IsWaterStandState(){ return m_activeLowerState==ELowerState_WaterStand;}
	bool IsUpperIdleState()	{ return m_activeUpperState==EUpperState_Idle;	}
	bool IsMoveState()      { return m_activeLowerState==ELowerState_MouseMove || 
		                             m_activeLowerState==ELowerState_KeyboardMove || 
									 m_activeLowerState==ELowerState_KeyboardWaterWalk || 
									 m_activeLowerState==ELowerState_MouseWaterWalk || 
									 m_activeLowerState==ELowerState_KeyboardSwim || 
									 m_activeLowerState==ELowerState_MouseSwim; }
	bool IsSpecailMoveState() { return   m_activeLowerState==ELowerState_Assault ||
										 m_activeLowerState==ELowerState_Repel ||
									     m_activeLowerState==ELowerState_Pull; }
	bool IsStyleActionState() { return m_activeUpperState==EUpperState_StyleAction; }
	//--
	tagHitTargetEvent* GetLastHitTargetEvent(DWORD roleID);
	LocalPlayer* GetLocalPlayer(){ return m_pLocalPlayer;}
	
	/**���������ж���
	*/
	void DontPlayEaseSkeleton();

	/** �����ǰ����ʹ����Ʒ״̬�򷵻�ʹ����Ʒ��type id
		���򷵻�0��ע)����ר�ã���������Ҫ����
	*/
	DWORD GetUsingItemTypeID();

private:
	ELowerState					m_activeLowerState;							//��ǰ����״̬
	EUpperState					m_activeUpperState;							//��ǰ����״̬
	EActionState				m_activeActionState;						//��ǰ����״̬

	LowerState_LP*				m_lowerStates[ELowerState_Num];				//��������״̬
	UpperState_LP*				m_upperStates[EUpperState_Num];				//��������״̬
	ActionState_LP*				m_actionStates[EActionState_Num];			//���ж���״̬
	GlobalState_LP*				m_globalStates[EGlobalState_Num];			//����ȫ��״̬

	vector<NetCmdHandler_LP*>	m_netCmdHandlers;							//������Ϣ������
	GameEventHandler_LP*		m_gameEventHandlers[EGameEventHandler_Num];	//��Ϸ�¼�������

	LocalPlayer*				m_pLocalPlayer;								//״̬��������ɫ
	TObjRef<Util>				m_pUtil;
};
