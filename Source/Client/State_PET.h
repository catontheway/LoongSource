#pragma once

class Pet;
class FSM_PET;

class State_PET
{
public:
    State_PET(void);
    virtual ~State_PET(void);
    void SetFSM(FSM_PET* pFSM){ m_pFSM = pFSM;}

    //! ����һ��״̬
    virtual void Active(tagNetCmd* pNetCmd, NavMap* pNav, Pet* pRole) = 0;

    //! �˳�һ��״̬
    virtual void OnExit(){}
    
    //! ����״̬�߼�
    virtual void Update(NavMap* pNav, Pet* pRole) = 0;

    //! ������Ϣ����
    virtual void OnNetCmd(tagNetCmd* pNetCmd) = 0;

    //! ��Ϸ�¼�����
    virtual void OnGameEvent(tagGameEvent* pEvent) = 0;

protected:
    FSM_PET*            m_pFSM;     //!< ����״̬��
    TObjRef<Util>       m_pUtil;
};
