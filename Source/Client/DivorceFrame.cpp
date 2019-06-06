#include "StdAfx.h"
#include "DivorceFrame.h"
#include "CombatSysUtil.h"
#include "CurrencyMgr.h"
#include "..\WorldDefine\currency_define.h"
#include "RoleMgr.h"
#include "PlayerNameTab.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\msg_marriage.h"
#include "CurrencyMgr.h"
#include "MsgBox.h"
#include "QuestMgr.h"
#include "MarriageFrame.h"
#include "MarriageMgr.h"

DivorceFrame::DivorceFrame(void)
:m_Trunk(this),
m_dwReqTime(0),
m_dwTargetID(GT_INVALID)
{
}

DivorceFrame::~DivorceFrame(void)
{
}

BOOL DivorceFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    // ע�������¼�
    m_pCmdMgr->Register("NS_BreakMarriageReq",      (NETMSGPROC)m_Trunk.sfp2(&DivorceFrame::OnNS_BreakMarriageReq),    _T("NS_BreakMarriageReq"));
    m_pCmdMgr->Register("NS_BreakMarriageReqRes",   (NETMSGPROC)m_Trunk.sfp2(&DivorceFrame::OnNS_BreakMarriageReqRes), _T("NS_BreakMarriageReqRes"));

    // ע����Ϸ�¼�
    m_pFrameMgr->RegisterEventHandle(_T("OpenDivorceApply"),            (FRAMEEVENTPROC)m_Trunk.sfp1(&DivorceFrame::OnOpenDivorceApply));
    m_pFrameMgr->RegisterEventHandle(_T("DivorceConfirmCheckEvent"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&DivorceFrame::OnDivorceConfirmCheckEvent));
    m_pFrameMgr->RegisterEventHandle(_T("DivorceReqCheckEvent"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&DivorceFrame::OnDivorceReqCheckEvent));

    return bRet;
}

BOOL DivorceFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    // ���ٶԻ���
    if(P_VALID(GetObj("DivorceConfirmCheck")))
        KillObj("DivorceConfirmCheck");

    if(P_VALID(GetObj("DivorceReq")))
        KillObj("DivorceReq");

    if (P_VALID(GetObj("BoxDivorceSuccess")))
        KillObj("BoxDivorceSuccess");

    // ע�������¼�
    m_pCmdMgr->UnRegister("NS_BreakMarriageReq",      (NETMSGPROC)m_Trunk.sfp2(&DivorceFrame::OnNS_BreakMarriageReq));
    m_pCmdMgr->UnRegister("NS_BreakMarriageReqRes",   (NETMSGPROC)m_Trunk.sfp2(&DivorceFrame::OnNS_BreakMarriageReqRes));

    // ע����Ϸ�¼�
    m_pFrameMgr->UnRegisterEventHandler(_T("OpenDivorceApply"),             (FRAMEEVENTPROC)m_Trunk.sfp1(&DivorceFrame::OnOpenDivorceApply));
    m_pFrameMgr->UnRegisterEventHandler(_T("DivorceConfirmCheckEvent"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&DivorceFrame::OnDivorceConfirmCheckEvent));
    m_pFrameMgr->UnRegisterEventHandler(_T("DivorceReqCheckEvent"),         (FRAMEEVENTPROC)m_Trunk.sfp1(&DivorceFrame::OnDivorceReqCheckEvent));

    return bRet;
}

VOID DivorceFrame::Update()
{
    GameFrame::Update();
}

DWORD DivorceFrame::OnNS_BreakMarriageReq( tagNS_BreakMarriageReq* pMsg, DWORD )
{
    // ������ת����Ŀ�����

    // ��������δ��ʼ������ֱ�Ӿܾ�
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLp) || !pLp->IsAttributeInited())
    {
        tagNC_BreakMarriageReqRes m;
        m.dwDstID       = pMsg->dwSrcID;
        m.dwErrorCode   = E_Marriage_Refuse;
        if (m_pSession->IsConnect())
            m_pSession->Send(&m);

        return 0;
    }

    if (pMsg->dwSrcID != RoleMgr::Inst()->GetLocalPlayerID())
    {
        // ����Ŀ��id
        m_dwTargetID = pMsg->dwSrcID;
        // ����ʱ��
        m_dwReqTime	 = MAX_MARRIAGE_REQTIME;

        // ����"ͬ��/�ܾ�"�Ի���
        if(P_VALID(GetObj("DivorceReq")))
            KillObj("DivorceReq");

        TCHAR szTemp[X_LONG_NAME] = {0};
        _sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("DivorceReqCheck")], PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcID));
        CreateObj("DivorceReq", "MsgBox");
        TObjRef<MsgBox>("DivorceReq")->Init(_T(""), szTemp, _T("DivorceReqCheckEvent"));
    }

    return 0;
}

DWORD DivorceFrame::OnNS_BreakMarriageReqRes( tagNS_BreakMarriageReqRes* pMsg, DWORD )
{
    if (E_Marriage_Accept != pMsg->dwErrorCode)
    {
        // ��ʾ����
        // ��ʾ����
        if (pMsg->dwErrorCode == E_Marriage_Refuse
            && RoleMgr::Inst()->GetLocalPlayerID() == pMsg->dwSrcID)
            return 0;

        MarriageMgr::Inst().ShowErrMsg(pMsg->dwErrorCode);

        return 0;
    }

    // ���汾�������ż��Ϣ
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (P_VALID(pLp))
    {
        if (pLp->GetID() == pMsg->dwSrcID)
            m_dwTargetID = pMsg->dwDstID;
        else
            m_dwTargetID = pMsg->dwSrcID;
    }
    pLp->SetPlayerLoverID(GT_INVALID);

    // ��ʾ�ɹ�
    MsgBox* pMsgBox = static_cast<MsgBox*>(GetObj("BoxDivorceSuccess"));
    if (P_VALID(pMsgBox))
        KillObj("BoxDivorceSuccess");

    CreateObj("BoxDivorceSuccess", "MsgBox");
    TObjRef<MsgBox>("BoxDivorceSuccess")->Init(_T(""), g_StrTable[_T("DivorceSuccess")], _T(""), MBF_OK, TRUE);

    return 0;
}

DWORD DivorceFrame::OnOpenDivorceApply( tagGameEvent* pEvent )
{
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLp))
        return 0;

    // ���жϱ�������Ƿ��ѻ�
    if (GT_INVALID == pLp->GetPlayerLoverID())
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DivorceHasNotGetMarried")]);
        return 0;
    }

    // ����ȷ����ʾ��
    if(P_VALID(GetObj("DivorceConfirmCheck")))
        KillObj("DivorceConfirmCheck");

    TCHAR szTemp[X_LONG_NAME] = {0};
    _sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("DivorceConfirmCheck")], PlayerNameTab::Inst()->FindNameByID(pLp->GetPlayerLoverID()));
    CreateObj("DivorceConfirmCheck", "MsgBox");
    TObjRef<MsgBox>("DivorceConfirmCheck")->Init(_T(""), szTemp, _T("DivorceConfirmCheckEvent"));


    return 0;
}

DWORD DivorceFrame::OnDivorceConfirmCheckEvent( tagMsgBoxEvent* pEvent )
{
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLp))
        return 0;

    if (pEvent->eResult != MBF_OK)
        return 0;

    // �������Ƿ�ӵ��MIN_DIVORCE_GOLD���ֽ�
    if (MSilver2DBGold(CurrencyMgr::Inst()->GetBagSilver()) < MIN_DIVORCE_GOLD)
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DivorceMoneyLimit")]);
        return 0;
    }

    // ��Ŀ����ҷ����������
    tagNC_BreakMarriageReq msg;
    msg.dwDstID = pLp->GetPlayerLoverID();
    m_pSession->Send(&msg);

    return 0;
}

DWORD DivorceFrame::OnDivorceReqCheckEvent( tagMsgBoxEvent* pEvent )
{
    // �ܾ�
    if (pEvent->eResult != MBF_OK)
    {
        tagNC_BreakMarriageReqRes m;
        m.dwDstID       = m_dwTargetID;
        m.dwErrorCode   = E_Marriage_Refuse;
        if (m_pSession->IsConnect())
            m_pSession->Send(&m);

        return 0;
    }

    // ͬ��
    tagNC_BreakMarriageReqRes m;
    m.dwDstID       = m_dwTargetID;
    m.dwErrorCode   = E_Marriage_Accept;
    if (m_pSession->IsConnect())
        m_pSession->Send(&m);

    return 0;
}