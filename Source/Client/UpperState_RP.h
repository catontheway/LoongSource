#pragma once
#include "FSM_RP.h"

class Player;

/** \class UpperState_RP
	\brief Զ���������״̬����
*/
class UpperState_RP
{
public:
	UpperState_RP(void);
	virtual ~UpperState_RP(void);
	void SetFSM(FSM_RP* pFSM){ m_pFSM=pFSM;}			

	//--
	/** ����һ��״̬
	*/
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole) =0;	
	/** ����һ��״̬
	*/
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,Player* pRole){}	
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
	/** �Ƿ����������Ŷ�����
	*/
	virtual bool IfCanPlayLowerAni() =0;
	/** �Ƿ������������ó���
	*/
	virtual bool IfLowerCanSetYaw() =0;
	/** ����״̬�ı�
	*/
	virtual void OnChangeLowerState(FSM_RP::ELowerState state){}

protected:
	FSM_RP* m_pFSM; //����״̬��
};
