#pragma once
#include "FSM_LP.h"

class LocalPlayer;
/** \class ActionState_LP
	\brief ������Ҷ���״̬����
*/
class ActionState_LP
{
public:
	ActionState_LP(void);
	virtual ~ActionState_LP(void);
	void SetFSM(FSM_LP* pFSM){ m_pFSM=pFSM;}			

	/** ����һ��״̬
	*/
	virtual void Active(NavMap* pNav,LocalPlayer* pRole) =0;	
	/** �˳�һ��״̬
	*/
	virtual void OnExit(){};
	/** ����״̬�߼�
	*/
	virtual void Update(NavMap* pNav,LocalPlayer* pRole) =0;							
	/** ��Ϸ�¼�����
	*/
	virtual void OnGameEvent(tagGameEvent* pEvent) =0;	
	/** ������Ϣ����
	*/
	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;
	/** �Ƿ�����л�����״̬
	*/
	virtual bool IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent) =0;
	/** �Ƿ�����л�����״̬
	*/
	virtual bool IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent) =0;

protected:
	FSM_LP* m_pFSM; //����״̬��
};
