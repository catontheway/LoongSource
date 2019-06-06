#pragma once

class FSM_RP;
class Player;
/** \class GlobalState_RP
	\brief Զ�����ȫ��״̬����
*/
class GlobalState_RP
{
public:
	GlobalState_RP(void);
	virtual ~GlobalState_RP(void);
	void SetFSM(FSM_RP* pFSM){ m_pFSM=pFSM;}			

	//--
	/** ����һ��״̬
	*/
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole,DWORD dwParam=0){};		
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,Player* pRole,DWORD dwParam=0){};		
	/** ����״̬�߼�
	*/
	virtual void Update(NavMap* pNav,Player* pRole) =0;							
	/** ��Ϸ�¼�����
	*/
	virtual void OnGameEvent(tagGameEvent* pEvent) =0;	
	/** ������Ϣ����
	*/
	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;
	/** �Ƿ��ڹر�״̬
	*/
	bool IsClosed(){ return m_bClosed;}

protected:
	FSM_RP* m_pFSM; //����״̬��
	bool m_bClosed;
};
