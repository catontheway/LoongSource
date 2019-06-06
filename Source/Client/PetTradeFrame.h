#pragma once
#include "gameframe.h"

struct tagNS_PetExchangeReq;
struct tagNS_PetExchangeReqRes;
struct tagNS_PetExchangeCancel;
struct tagNS_PetExchangeFinish;
struct tagFollowRoleEvent;
struct tagMsgBoxEvent;

//! \class   PetTradeFrame
//! \breif   ���ｻ��
//! \author  hyu
//! \date    2009-8-13
class PetTradeFrame :
    public GameFrame
{
public:
    PetTradeFrame(void);
    ~PetTradeFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual VOID Update();

    // ���״�������ʾ
    VOID ShowExchangeErrorMsg(DWORD dwErrorCode, BOOL bNotMine = FALSE);

private:
    // ����������Ϣ
    DWORD OnNetPetExchangeReq(tagNS_PetExchangeReq* pNetCmd, DWORD);
    DWORD OnNetPetExchangeReqRes(tagNS_PetExchangeReqRes* pNetCmd, DWORD);
    DWORD OnNetPetExchangeCancel(tagNS_PetExchangeCancel* pNetCmd, DWORD);
    DWORD OnNetPetExchangeFinish(tagNS_PetExchangeFinish* pNetCmd, DWORD);

    // ������Ϸ�¼�
    DWORD OnEventReqTrade(tagFollowRoleEvent* pGameEvent);
    DWORD OnEventReqReply(tagMsgBoxEvent* pGameEvent);

    // �򿪳��ｻ�׽���
    VOID OpenPetTrade(DWORD dwTargetID);
    // �رճ��ｻ�׽���
    VOID ClosePetTrade();
    // �������ｻ�׽���
    VOID UnlockPetTrade();

private:
    // ����
    TSFPTrunk<PetTradeFrame>    m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    // ����
    DWORD					    m_dwReqTime;  // ����������Ӧʱ��	
    DWORD					    m_dwTargetID; // ���׶���ID
};
