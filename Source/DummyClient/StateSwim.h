#pragma once
#include "state.h"
#include "GameEvent.h"

/** ��Ӿ״̬
*/
class StateSwim :
	public State
{
public:
	StateSwim(void);
	virtual ~StateSwim(void);

	//--State
	virtual void Active(tagGameEvent* pEvent);
	virtual void Update();
	virtual void OnGameEvent(tagGameEvent* pEvent);

private:
	tagSwimEvent	m_event;
	Vector3			m_moveStart;
	Vector3			m_moveDest;
	Vector3			m_moveDir;
	float			m_moveStartTime;
	float			m_moveTime;//�ƶ���Ŀ�����Ҫ����ʱ��
	NavCollider::EReturn m_ret;

	list<NavCollider::tagMovePoint>	m_movePath;
};
