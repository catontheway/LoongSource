#pragma once
#include "upperstate_rp.h"
#include "PersonalityActData.h"

/**	\class UpperStateStyleAction_RP
	\brief Զ����ҿ���״̬
*/
class UpperStateStyleAction_RP : public UpperState_RP
{
	enum E3ActStep
	{
		E3ActStep_None,
		E3ActStep_In,				// ����
		E3ActStep_Holding,			// ����
		E3ActStep_Out,				// �뿪
	};
public:
	UpperStateStyleAction_RP(void);
	virtual ~UpperStateStyleAction_RP(void);

	//--UpperState_RP
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole);		
	virtual void Update(NavMap* pNav,Player* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void OnExit();
	virtual bool IfCanPlayLowerAni();
	virtual bool IfLowerCanSetYaw();
	virtual void OnChangeLowerState(FSM_RP::ELowerState state);

private:
	tagPersonAct m_sPersonActData;
	int          m_step;
	DWORD        m_targetID;
	bool         m_bSentEventOnExit;
	bool         m_bDestRole;
};
