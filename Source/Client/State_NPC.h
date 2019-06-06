#pragma once

class FSM_NPC;
class NPC;
/** \class State_NPC
	\brief NPC״̬����
*/
class State_NPC
{
public:
	State_NPC(void);
	virtual ~State_NPC(void);
	void SetFSM(FSM_NPC* pFSM){ m_pFSM=pFSM;}			

	//--
	/** ����һ��״̬
	*/
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,NPC* pRole) =0;		
	/** �˳�һ��״̬
	*/
	virtual void OnExit(){};
	/** ����״̬�߼�
	*/
	virtual void Update(NavMap* pNav,NPC* pRole) =0;							
	/** ������Ϣ����
	*/
	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;
	/** ��Ϸ�¼�����
	*/
	virtual void OnGameEvent(tagGameEvent* pEvent) =0;	

protected:
	FSM_NPC* m_pFSM; //����״̬��
};
