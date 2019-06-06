#pragma once

class FSM_LP;
class LocalPlayer;
/** \class GlobalState_LP
	\brief �������ȫ��״̬����
*/
class GlobalState_LP
{
public:
	GlobalState_LP(void);
	virtual ~GlobalState_LP(void);
	void SetFSM(FSM_LP* pFSM){ m_pFSM=pFSM;}			

	//--
	/** ����һ��״̬
	*/
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole) =0;		
	/** ����״̬�߼�
	*/
	virtual void Update(NavMap* pNav,LocalPlayer* pRole) =0;							
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
	FSM_LP* m_pFSM; //����״̬��
	bool m_bClosed;
};
