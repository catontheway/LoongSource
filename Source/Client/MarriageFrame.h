#pragma once
#include "gameframe.h"

struct tagNS_GetMarriageReq;
struct tagNS_GetMarriageReqRes;

const int MIN_MARRIAGE_GOLD         = 66;           //!< �����Ҫ����С����
const DWORD MAX_MARRIAGE_REQTIME	= 60 * 1000;	//!< ���ȴ�����ʱ�䣬��λ������

//! \class   MarriageFrame
//! \breif   ������
//! \author  hyu
//! \date    2009-9-23
//! \last    2009-10-23
class MarriageFrame :
    public GameFrame
{
public:
    MarriageFrame(void);
    ~MarriageFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual VOID Update();

private:
    // �жϱ�������Ƿ�ɽ��
    bool IfCanGetToMarry();
    // �ж���Ŀ������Ƿ�ɽ��
    bool ifObjCanGetToMarry(LPCTSTR szPlayerName);

    // ����������Ϣ
    DWORD OnNS_GetMarriageReq       (tagNS_GetMarriageReq*      pMsg, DWORD);
    DWORD OnNS_GetMarriageReqRes    (tagNS_GetMarriageReqRes*   pMsg, DWORD);

    // ������Ϸ�¼�
    DWORD OnOpenMarriageApply       (tagGameEvent* pEvent);
    DWORD OnMarriageInputNameCommit (tagMsgInputBoxEvent* pEvent);
    DWORD OnMarriageReqCheckEvent   (tagMsgBoxEvent* pEvent);

    // ����
    TSFPTrunk<MarriageFrame>    m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    // ����
    DWORD					    m_dwReqTime;        //!< ���������Ӧʱ��
    DWORD					    m_dwTargetID;       //!< ������ID
    tstring                     m_strTargetName;    //!< ������name
    DWORD                       m_dwNpcId;          //!< NPC��ʶ

};
