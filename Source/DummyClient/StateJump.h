#pragma once
#include "state.h"
#include "GameEvent.h"

/** ��Ծ״̬
*/
class StateJump :
	public State
{
public:
	StateJump(void);
	virtual ~StateJump(void);

	//--State
	virtual void Active(tagGameEvent* pEvent);
	virtual void Update();
	virtual void OnGameEvent(tagGameEvent* pEvent);

private:
	tagJumpEvent		m_event;
	NavCollider_Jump	m_collider;
};
