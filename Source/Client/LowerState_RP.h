#pragma once
#include "FSM_RP.h"

class Player;
/** \class LowerState_RP
	\brief Զ���������״̬����
*/
class LowerState_RP
{
public:
	LowerState_RP(void);
	virtual ~LowerState_RP(void);
	void SetFSM(FSM_RP* pFSM){ m_pFSM=pFSM;}			

	//--
	/** ����һ��״̬
	*/
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole) =0;	
	/** �˳�һ��״̬
	*/
	virtual void OnExit(){};
	/** ���²��Ŷ���
	*/
	virtual void ReplayAni(Player* pRole){}
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
