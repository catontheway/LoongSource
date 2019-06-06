#pragma once
#include "state_npc.h"

/**	\class StateIdle_NPC
	\brief Զ������������״̬
*/
class StateIdle_NPC : public State_NPC
{
	enum EStep
	{
		EStep_Stand,//վ��
		EStep_Ease,//����
		EStep_Buff,//Buff����
	};
public:
	StateIdle_NPC(void);
	virtual ~StateIdle_NPC(void);

	//--State_NPC
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,NPC* pRole);		
	virtual void Update(NavMap* pNav,NPC* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);

private:
	bool PlayBuffAni(NPC* pRole);

private:
	DWORD m_lastPlayEaseTime;//�ϴβ������ж���ʱ��
	EStep m_step;
};
