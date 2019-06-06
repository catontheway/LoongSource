#pragma once
#include "upperstate_rp.h"
#include "ActionSkillAttack_RP.h"
#include "ActionSkillSpell_RP.h"

/** \class UpperStateSkill_RP
	\brief Զ�����ʹ�ü���״̬
*/
class UpperStateSkill_RP :
	public UpperState_RP
{
	enum EStep
	{
		EStep_Spell,
		EStep_Attack,
	};

public:
	UpperStateSkill_RP(void);
	virtual ~UpperStateSkill_RP(void);

	//--UpperState_RP
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole);	
	virtual void OnExit();
	virtual void Update(NavMap* pNav,Player* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual bool IfCanPlayLowerAni();
	virtual bool IfLowerCanSetYaw();

private:
	ActionSkillSpell_RP			m_actSpell;
	ActionSkillAttack_RP		m_actAttack;
	EStep						m_step;
	DWORD						m_targetID;
	DWORD						m_skillID;
	DWORD						m_serialID;
	bool						m_bMoveable;	//�Ƿ�����ƶ�����
	bool						m_bAniBlend;	//�Ƿ�������ֿ�

	DWORD						m_startSpellTime;
	DWORD						m_spellRealTime;        //��ʵ������ʱ�䣬actSpell�е�����ʱ�������̺�ģ�Ϊ��Ԥ������������
};	
