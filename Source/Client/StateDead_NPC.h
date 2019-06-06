#pragma once
#include "state_npc.h"

/**	\class StateDead_NPC
	\brief NPC����״̬
*/
class StateDead_NPC : public State_NPC
{
	enum EStep
	{
        EStep_Drop,     //����
		EStep_FallDown,	//����
		EStep_Lie,		//����
		EStep_Fadeout,	//����
		EStep_Closed,	//����
	};
public:
	StateDead_NPC(void);
	virtual ~StateDead_NPC(void);

	//--State_NPC
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,NPC* pRole);		
	virtual void Update(NavMap* pNav,NPC* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent){}

private:
    NavCollider_Jump    m_collider;      // ��npc�ڰ������ʱ�Ƚ��е���
    float               m_lastCDTime;
	EStep	m_step;
	float	m_startLieTime;
	float	m_startFadeTime;
};
