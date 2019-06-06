#pragma once
#include "FSM_LP.h"

class LocalPlayer;
struct tagPreState;

/** \class UpperState_LP
	\brief �����������״̬����
*/
class UpperState_LP
{
public:
	UpperState_LP(void);
	virtual ~UpperState_LP(void);
	void SetFSM(FSM_LP* pFSM){ m_pFSM=pFSM;}			

	//--
	/** ����һ��״̬
	*/
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole) =0;	
	/** ����һ��״̬
	*/
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,LocalPlayer* pRole){}
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
	/** ����״̬�л�����
	*/
	virtual void OnChangeLowerState(FSM_LP::ELowerState newState,tagGameEvent* pEvent){}
	/** �Ƿ����������Ŷ�����
	*/
	virtual bool IfCanPlayLowerAni() =0;
	/** �Ƿ������������ó���
	*/
	virtual bool IfLowerCanSetYaw() =0;

protected:
	FSM_LP* m_pFSM; //����״̬��
};
