#pragma once

class FSM_PET;

class NetCmdHandler_PET
{
public:
    NetCmdHandler_PET(void);
    virtual ~NetCmdHandler_PET(void);
    void SetFSM(FSM_PET* pFSM){ m_pFSM = pFSM; }

    virtual void OnNetCmd(tagNetCmd* pNetCmd) = 0;
    virtual void OnGameEvent(tagGameEvent* pEvent) = 0;
    virtual void Update() = 0;

protected:
    FSM_PET*				m_pFSM;             //!< ����״̬��
    TObjRef<Util>			m_pUtil;
    TObjRef<GameFrameMgr>	m_pGameFrameMgr;
};


/** \class NS_SyncStandHandler_PET
    \brief վ��״̬ͬ���������
*/
class NS_SyncStandHandler_PET : public NetCmdHandler_PET
{
public:
    NS_SyncStandHandler_PET() {};
    virtual ~NS_SyncStandHandler_PET() {};

    virtual void OnNetCmd(tagNetCmd* pNetCmd);
    virtual void OnGameEvent(tagGameEvent* pEvent) {};
    virtual void Update() {};
};

/** \class NS_StopActionHandler_PET
    \brief ֹͣ������Ϊ�������
*/
class NS_StopActionHandler_PET : public NetCmdHandler_PET
{
public:
    NS_StopActionHandler_PET() {};
    virtual ~NS_StopActionHandler_PET() {};

    virtual void OnNetCmd(tagNetCmd* pNetCmd);
    virtual void OnGameEvent(tagGameEvent* pEvent) {};
    virtual void Update() {};
};