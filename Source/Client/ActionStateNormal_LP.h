#pragma once
#include "ActionState_LP.h"
#include "CombatEvent.h"

/** \class ActionStateNormal_LP
	\brief ���������������״̬
*/
class ActionStateNormal_LP : public ActionState_LP
{
public:
	ActionStateNormal_LP(void);
	virtual ~ActionStateNormal_LP(void);

	//--ActionState_LP
	virtual void Active(NavMap* pNav,LocalPlayer* pRole);
	virtual void Update(NavMap* pNav,LocalPlayer* pRole);							
	virtual void OnGameEvent(tagGameEvent* pEvent);	
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual bool IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent);
	virtual bool IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent);
};