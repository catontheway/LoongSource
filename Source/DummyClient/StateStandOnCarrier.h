#pragma once
#include "state.h"

/** �ؾ���վ��״̬
*/
class StateStandOnCarrier :
	public State
{
public:
	StateStandOnCarrier(void);
	virtual ~StateStandOnCarrier(void);

	//--State
	virtual void Active(tagGameEvent* pEvent);
	virtual void Update();
	virtual void OnGameEvent(tagGameEvent* pEvent);

private:
	DWORD		m_dwCarrierObjID;
	Vector3		m_localPos;
};
