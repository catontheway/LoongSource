#pragma once
#include "upperstate_lp.h"
#include "CombatEvent.h"
#include "ActionSkillAttack_LP.h"
#include "ActionSkillSpell_LP.h"

/** \class UpperStateSkill_LP
	\brief �������ʹ�ü���״̬
*/
class UpperStateSkill_LP : public UpperState_LP
{
	enum EStep
	{
		EStep_None,
		EStep_Pursue,				// ׷��Ŀ��
		EStep_Spell,				// ����
		EStep_Attack,				// ����
	};

	struct tagEventCache
	{
		enum EEventType
		{
			EET_None,
			EET_UseSkill,
			EET_UseItem,
			EET_MouseMove,
			EET_KeyboardMove,
		};
		EEventType			 eventType;
		tagUseSkillEvent	 useSkill;
		tagUseItemEvent		 useItem;
		tagMouseMoveEvent	 mouseMove;
		tagKeyboardMoveEvent keyboardMove;
	};

public:
	UpperStateSkill_LP(void);
	virtual ~UpperStateSkill_LP(void);

	//--State_LP
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole);
	virtual void Update(NavMap* pNav,LocalPlayer* pRole);							
	virtual void OnGameEvent(tagGameEvent* pEvent);	
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnExit();
	virtual bool IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent);
	virtual bool IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent);
	virtual void OnChangeLowerState(FSM_LP::ELowerState newState,tagGameEvent* pEvent);
	virtual bool IfCanPlayLowerAni();
	virtual bool IfLowerCanSetYaw();

private:
	bool IsInAttackRange(LocalPlayer* pRole,Role* pTarget);
	void SendStopPursueEvent();

private:
	ActionSkillAttack_LP	m_actAttack;
	ActionSkillSpell_LP		m_actSpell;

	EStep					m_step;

	DWORD					m_targetID;
	DWORD					m_skillID;
	DWORD					m_dwSerial;
	float					m_attackDist;			//��������
	bool					m_bAttackSkill;			//�Ƿ��˺�����
	bool					m_bSpellSkill;			//�Ƿ����ּ���
	bool					m_bMoveable;			//�Ƿ�����ƶ�����
	bool					m_bMoveSpell;			//�Ƿ�����ƶ�����
	bool					m_bAniBlend;			//�Ƿ�������ֿ�
	bool					m_bAutoNormalAttack;	//���Զ�����ͨ����

	DWORD					m_startSpellTime;
	DWORD					m_spellRealTime;        //��ʵ������ʱ�䣬actSpell�е�����ʱ�������̺�ģ�Ϊ��Ԥ������������
	DWORD                   m_dwAttackAdvanceTime;  //����������ǰ��ʱ��

	bool                    m_bWaitForWeaponSwap;	// �ȴ������л�����ʹ�ø����������ܵ�ʱ��
	tagEventCache			m_eventCache;
	DWORD                   m_dwAttackEndTime;      // ��������ʱ��
	DWORD                   m_dwAttackEndDelayTime; // ��ʱ���������ӳٵ����⶯����ɺ���������ܻ�û���˳������ԼӸ���ʱ
};	
