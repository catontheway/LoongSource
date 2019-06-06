#pragma once
#include "FSM_RP.h"

class Player;
/** \class ActionState_RP
	\brief Զ����Ҷ���״̬����
*/
class ActionState_RP
{
public:
	ActionState_RP(void);
	virtual ~ActionState_RP(void);
	void SetFSM(FSM_RP* pFSM){ m_pFSM=pFSM;}			

	/** ����һ��״̬
	*/
	virtual void Active(NavMap* pNav,Player* pRole) =0;	
	/** �˳�һ��״̬
	*/
	virtual void OnExit(){};
	/** ����״̬�߼�
	*/
	virtual void Update(NavMap* pNav,Player* pRole) =0;							
	/** ��Ϸ�¼�����
	*/
	virtual void OnGameEvent(tagGameEvent* pEvent) =0;	
	/** ������Ϣ����
	*/
	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;
	/** �Ƿ�����л�����״̬
	*/
	virtual bool IfCanChange2UpperState(FSM_RP::EUpperState state) =0;

protected:
	FSM_RP* m_pFSM; //����״̬��
};
