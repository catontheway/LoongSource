#pragma once
#include "state.h"

/** ����״̬
*/
class StateIdle :
	public State
{
public:
	StateIdle(void);
	virtual ~StateIdle(void);

	//--State
	virtual void Active(tagGameEvent* pEvent);
	virtual void Update();
	virtual void OnGameEvent(tagGameEvent* pEvent);

};
