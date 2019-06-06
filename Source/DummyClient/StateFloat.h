#pragma once
#include "state.h"

/** Ư��״̬
*/
class StateFloat :
	public State
{
	enum EStep
	{
		EStep_GoDown,	//�³�
		EStep_FloatUp,	//�ϸ�
		EStep_Floating,	//������ˮ��
	};
public:
	StateFloat(void);
	virtual ~StateFloat(void);

	//--State
	virtual void Active(tagGameEvent* pEvent);
	virtual void Update();
	virtual void OnGameEvent(tagGameEvent* pEvent);

private:
	float m_startH;
	float m_endH;
	float m_startTime;
	float m_totalTime;
	EStep m_step;
};
