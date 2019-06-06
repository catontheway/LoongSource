#pragma once
#include "gameframe.h"

struct tagNS_BreakMarriageReq;
struct tagNS_BreakMarriageReqRes;

const int MIN_DIVORCE_GOLD  = 100;

//! \class   DivorceFrame
//! \breif   ������
//! \author  hyu
//! \date    2009-9-23
//! \last    2009-10-23
class DivorceFrame :
    public GameFrame
{
public:
    DivorceFrame(void);
    ~DivorceFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual VOID Update();

private:

    // ����������Ϣ
    DWORD OnNS_BreakMarriageReq     (tagNS_BreakMarriageReq*    pMsg, DWORD);
    DWORD OnNS_BreakMarriageReqRes  (tagNS_BreakMarriageReqRes* pMsg, DWORD);

    // ������Ϸ�¼�
    DWORD OnOpenDivorceApply        (tagGameEvent*      pEvent);
    DWORD OnDivorceConfirmCheckEvent(tagMsgBoxEvent*    pEvent);
    DWORD OnDivorceReqCheckEvent    (tagMsgBoxEvent*    pEvent);

    // ����
    TSFPTrunk<DivorceFrame>     m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    // ����
    DWORD					    m_dwReqTime;        //!< ���������Ӧʱ��
    DWORD					    m_dwTargetID;       //!< ������ID
};
