#pragma once
#include "upperstate_rp.h"

/**	\class UpperStateDead_RP
	\brief Զ���������״̬
*/
class UpperStateDead_RP : public UpperState_RP
{
	enum EStep
	{
		EStep_FallDown,	//����
		EStep_Lie,		//����
	};

public:
	UpperStateDead_RP(void);
	virtual ~UpperStateDead_RP(void);

	//--UpperState_RP
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole){}
	virtual void Update(NavMap* pNav,Player* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent){}
	virtual bool IfCanPlayLowerAni();
	virtual bool IfLowerCanSetYaw();

	//--
	void Active(bool bPlayAni,Player* pRole);	

private:
	EStep	m_step;
};
