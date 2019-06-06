#pragma once
#include "FSM_LP.h"

class LocalPlayer;
struct tagPreState;
/** \class LowerState_LP
	\brief �����������״̬����
*/
class LowerState_LP
{
public:
	LowerState_LP(void);
	virtual ~LowerState_LP(void);
	void SetFSM(FSM_LP* pFSM){ m_pFSM=pFSM;}			

	//--
	/** ����һ��״̬
	*/
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState=NULL) =0;	
	/** ����һ��״̬
	*/
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState=NULL){}	
	/** �˳�һ��״̬
	*/
	virtual void OnExit(){};
	/** ���²��Ŷ���
	*/
	virtual void ReplayAni(LocalPlayer* pRole){}
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
	virtual void OnChangeUpperState(FSM_LP::EUpperState newState){}

protected:
	FSM_LP* m_pFSM; //����״̬��
};
