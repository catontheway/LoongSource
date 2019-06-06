#pragma once

class Pet;
class State_PET;
class NetCmdHandler_PET;
class GameEventHandler_PET;

//! \class  FSM_PET
//! \brief  ����״̬��
//! \author hyu
//! \date   2990-4-7
class FSM_PET
{
public:
    enum EState
    {
        EState_Idle,            //!< ����
        EState_MoveByCollide,	//!< ������ײ���ƶ�
        EState_MoveByTile,		//!< ���ڸ��ӵ��ƶ�
        EState_Shutdown,        //!< ״̬��ֹͣ״̬,����Ȩ������ɫ,�����,����
        EState_Num
    };
public:
    FSM_PET(Pet* pPet);
    virtual ~FSM_PET(void);

    void Init();
    void Destroy();
    void Update();
    void OnNetCmd(tagNetCmd* pNetCmd);
    void OnGameEvent(tagGameEvent* pEvent);

    void Change2State(tagNetCmd* pNetCmd);
    void Change2State(tagGameEvent* pEvent);
    void Change2IdleState();
    void Change2State(EState state,tagNetCmd* pNetCmd);

    Pet* GetPet() { return m_pPet; }

private:
    EState							m_activeState;						        //!< ��ǰ״̬
    State_PET*						m_states[EState_Num];				        //!< ����״̬
    vector<NetCmdHandler_PET*>		m_netCmdHandlers;					        //!< ������Ϣ������
    vector<GameEventHandler_PET*>	m_gameEventHandlers;                        //!< ��Ϸ�¼�������
    Pet*                            m_pPet;                                     //!< ״̬����������
    TObjRef<Util>					m_pUtil;
};
