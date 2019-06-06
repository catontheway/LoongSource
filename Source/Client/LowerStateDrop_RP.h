#pragma once
#include "lowerstate_rp.h"

/** \class LowerStateDrop_RP
	\brief Զ����ҵ���״̬
*/
class LowerStateDrop_RP : public LowerState_RP
{
	enum EStep
	{
		EStep_Dropping,
		EStep_Land,
	};

public:
	LowerStateDrop_RP(void);
	virtual ~LowerStateDrop_RP(void);

	//--LowerState_RP
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole);		
	virtual void Update(NavMap* pNav,Player* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent){}
	virtual void ReplayAni(Player* pRole);
	virtual bool IfCanChange2UpperState(FSM_RP::EUpperState state);

private:
	NavCollider_Jump		m_collider;
	EStep					m_step;
	float					m_lastCDTime;
	bool					m_bPlayInEffect;//�Ƿ��Ѳ�����ˮ��Ч
};
