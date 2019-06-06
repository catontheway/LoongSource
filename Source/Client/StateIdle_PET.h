#pragma once
#include "State_PET.h"

class StateIdle_PET :
    public State_PET
{
private:
    //! ����״̬����������
    enum EStep
    {
        EStep_Stand,    //!< ��ͨվ��
        EStep_Ease      //!< ץ������
    };
public:
    StateIdle_PET(void);
    virtual ~StateIdle_PET(void);

    virtual void Active(tagNetCmd* pNetCmd, NavMap* pNav, Pet* pRole);		
    virtual void Update(NavMap* pNav, Pet* pRole);							
    virtual void OnNetCmd(tagNetCmd* pNetCmd);
    virtual void OnGameEvent(tagGameEvent* pEvent);

private:
    DWORD m_lastPlayEaseTime;       //!< �ϴβ������ж���ʱ��
    EStep m_step;
};
