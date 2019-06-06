#include "StdAfx.h"
#include "MarriageFrame.h"
#include "MarriageMgr.h"
#include "..\WorldDefine\msg_marriage.h"
#include "RoleMgr.h"
#include "PlayerNameTab.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\currency_define.h"
#include "CurrencyMgr.h"
#include "MsgBox.h"
#include "QuestMgr.h"

MarriageFrame::MarriageFrame(void) :
    m_Trunk(this),
    m_dwReqTime(0),
    m_dwTargetID(GT_INVALID),
    m_dwNpcId(GT_INVALID)
{
}

MarriageFrame::~MarriageFrame(void)
{
}

BOOL MarriageFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    // ע�������¼�
    m_pCmdMgr->Register("NS_GetMarriageReq",        (NETMSGPROC)m_Trunk.sfp2(&MarriageFrame::OnNS_GetMarriageReq),      _T("NS_GetMarriageReq"));
    m_pCmdMgr->Register("NS_GetMarriageReqRes",     (NETMSGPROC)m_Trunk.sfp2(&MarriageFrame::OnNS_GetMarriageReqRes),   _T("NS_GetMarriageReqRes"));

    // ע����Ϸ�¼�
    m_pFrameMgr->RegisterEventHandle(_T("OpenMarriageApply"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&MarriageFrame::OnOpenMarriageApply));
    m_pFrameMgr->RegisterEventHandle(_T("MarriageInputNameCommit"), (FRAMEEVENTPROC)m_Trunk.sfp1(&MarriageFrame::OnMarriageInputNameCommit));
    m_pFrameMgr->RegisterEventHandle(_T("MarriageReqCheckEvent"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&MarriageFrame::OnMarriageReqCheckEvent));

    return bRet;
}

BOOL MarriageFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    // ���ٶԻ���
    MsgBox* pMsgBox = static_cast<MsgBox*>(GetObj("BoxCanNotMarry"));
    if (P_VALID(pMsgBox))
        KillObj("BoxCanNotMarry");

    MsgInputBox* pInputBox = static_cast<MsgInputBox*>(GetObj("BoxMarriageInputName"));
    if (P_VALID(pInputBox))
        KillObj("BoxMarriageInputName");

    MsgBox* pObjMsgBox = static_cast<MsgBox*>(GetObj("BoxObjCanNotMarry"));
    if (P_VALID(pObjMsgBox))
        KillObj("BoxObjCanNotMarry");

    if(P_VALID(GetObj("MarriageReq")))
        KillObj("MarriageReq");

    if (P_VALID(GetObj("BoxMarrySuccess")))
        KillObj("BoxMarrySuccess");

    // ע�������¼�
    m_pCmdMgr->UnRegister("NS_GetMarriageReq",        (NETMSGPROC)m_Trunk.sfp2(&MarriageFrame::OnNS_GetMarriageReq));
    m_pCmdMgr->UnRegister("NS_GetMarriageReqRes",     (NETMSGPROC)m_Trunk.sfp2(&MarriageFrame::OnNS_GetMarriageReqRes));

    // ע����Ϸ�¼�
    m_pFrameMgr->UnRegisterEventHandler(_T("OpenMarriageApply"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&MarriageFrame::OnOpenMarriageApply));
    m_pFrameMgr->UnRegisterEventHandler(_T("MarriageInputNameCommit"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&MarriageFrame::OnMarriageInputNameCommit));
    m_pFrameMgr->UnRegisterEventHandler(_T("MarriageReqCheckEvent"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&MarriageFrame::OnMarriageReqCheckEvent));

    return bRet;
}

VOID MarriageFrame::Update()
{
    GameFrame::Update();
}

DWORD MarriageFrame::OnNS_GetMarriageReq( tagNS_GetMarriageReq* pMsg, DWORD )
{
    // ������ת����Ŀ�����

    // ��������δ��ʼ������ֱ�Ӿܾ�
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLp) || !pLp->IsAttributeInited())
    {
        tagNC_GetMarriageReqRes m;
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
        if(P_VALID(GetObj("MarriageReq")))
            KillObj("MarriageReq");

        TCHAR szTemp[X_LONG_NAME] = {0};
        _sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("MarriageReqCheck")], PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcID));
        CreateObj("MarriageReq", "MsgBox");
        TObjRef<MsgBox>("MarriageReq")->Init(_T(""), szTemp, _T("MarriageReqCheckEvent"));
    }

    return 0;
}

DWORD MarriageFrame::OnNS_GetMarriageReqRes( tagNS_GetMarriageReqRes* pMsg, DWORD )
{
    if (E_Marriage_Accept != pMsg->dwErrorCode)
    {
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
    pLp->SetPlayerLoverID(m_dwTargetID);

    // ��ʾ�ɹ�
    TCHAR szTemp[100] = {0};
    _sntprintf(szTemp, sizeof(szTemp) / sizeof(TCHAR),
        g_StrTable[_T("MarriageObjLimit")],
        PlayerNameTab::Inst()->FindNameByID(m_dwTargetID));

    MsgBox* pMsgBox = static_cast<MsgBox*>(GetObj("BoxMarrySuccess"));
    if (P_VALID(pMsgBox))
        KillObj("BoxMarrySuccess");

    _sntprintf(szTemp, sizeof(szTemp) / sizeof(TCHAR),
        g_StrTable[_T("MarriageSuccess")],
        PlayerNameTab::Inst()->FindNameByID(m_dwTargetID));
    CreateObj("BoxMarrySuccess", "MsgBox");
    TObjRef<MsgBox>("BoxMarrySuccess")->Init(_T(""), szTemp, _T(""), MBF_OK, TRUE);


    return 0;
}

DWORD MarriageFrame::OnOpenMarriageApply( tagGameEvent* pEvent )
{
    m_dwNpcId = QuestMgr::Inst()->GetTalkNPCGlobalID();

    // ���ж��Ƿ�߱��������
    if (!IfCanGetToMarry())
    {
        // ��ʾ����
        MsgBox* pMsgBox = static_cast<MsgBox*>(GetObj("BoxCanNotMarry"));
        if (P_VALID(pMsgBox))
            KillObj("BoxCanNotMarry");

        CreateObj("BoxCanNotMarry", "MsgBox");
        TObjRef<MsgBox>("BoxCanNotMarry")->Init(_T(""), g_StrTable[_T("MarriageLimit")], _T("eventCanNotMarry"), MBF_OK, TRUE);


        return 0;
    }

    // ���������������Ҫ����������������������
    MsgInputBox* pMsgBox = static_cast<MsgInputBox*>(GetObj("BoxMarriageInputName"));
    if (P_VALID(pMsgBox))
        KillObj("BoxMarriageInputName");

    CreateObj("BoxMarriageInputName", "MsgInputBox");
    TObjRef<MsgInputBox>("BoxMarriageInputName")->Init(g_StrTable[_T("MarriageInputName")], g_StrTable[_T("MarriageInputNameTip")], _T("MarriageInputNameCommit"), GT_INVALID, MBIT_Name, NULL, TRUE);

    return 0;
}

bool MarriageFrame::IfCanGetToMarry()
{
    // �������Ƿ�δ���

    // �������Ƿ�ӵ��MIN_MARRIAGE_GOLD���ֽ�
    if (MSilver2DBGold(CurrencyMgr::Inst()->GetBagSilver()) < MIN_MARRIAGE_GOLD)
        return false;

    return true;
}

bool MarriageFrame::ifObjCanGetToMarry( LPCTSTR szPlayerName )
{
    DWORD playerId      = GT_INVALID;   //!< Ŀ�����id
    Player* pPlayer     = NULL;         //!< Ŀ�����obj

    // ͨ��player name ���player id
    playerId = PlayerNameTab::Inst()->FindIDByName(szPlayerName);

    if (GT_INVALID == playerId)
        return false;

    // ���浱ǰĿ����Ϣ
    m_dwTargetID    = playerId;
    m_strTargetName = szPlayerName;

    // �ھŹ���ȡ��player
    Role* pRole = RoleMgr::Inst()->FindRole(playerId);

    if (!P_VALID(pRole))
        return false;

    if (!pRole->IS_KIND_OF(Player))
        return false;

    pPlayer = static_cast<Player*>(pRole);

    // ȡ�ñ������
    LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLocalPlayer))
        return false;

    // �жϽ������Ƿ���������Ϊ����
    if (pLocalPlayer->GetAvatarAtt().bySex == pPlayer->GetAvatarAtt().bySex)
        return false;

    // ���������������Ƿ�Ϊ���ѣ����Ѻöȡ�720

    // �������Ƿ�δ���

    // �������Ƿ������ϸ���10��������

    return true;
}

DWORD MarriageFrame::OnMarriageInputNameCommit( tagMsgInputBoxEvent* pEvent )
{
    if (pEvent->eResult != MBF_OK)
        return 0;

    // �ж���Է��Ƿ��н������
    if (!ifObjCanGetToMarry(pEvent->strInputName.c_str()))
    {
        // ��ʾ����
        MsgBox* pMsgBox = static_cast<MsgBox*>(GetObj("BoxObjCanNotMarry"));
        if (P_VALID(pMsgBox))
            KillObj("BoxObjCanNotMarry");

        CreateObj("BoxObjCanNotMarry", "MsgBox");
        TObjRef<MsgBox>("BoxObjCanNotMarry")->Init(_T(""), g_StrTable[_T("MarriageObjLimit")], _T("eventObjCanNotMarry"), MBF_OK, TRUE);

        return 0;
    }

    // ��Ŀ����ҷ��ͽ������
    tagNC_GetMarriageReq msg;
    msg.dwDstID = m_dwTargetID;
    msg.dwNpcID = m_dwNpcId;

    if (m_pSession->IsConnect())
        m_pSession->Send(&msg);

    return 0;
}

DWORD MarriageFrame::OnMarriageReqCheckEvent( tagMsgBoxEvent* pEvent )
{
    // �ܾ�
    if (pEvent->eResult != MBF_OK)
    {
        tagNC_GetMarriageReqRes m;
        m.dwDstID       = m_dwTargetID;
        m.dwErrorCode   = E_Marriage_Refuse;
        if (m_pSession->IsConnect())
            m_pSession->Send(&m);

        return 0;
    }

    // ͬ��
    tagNC_GetMarriageReqRes m;
    m.dwDstID       = m_dwTargetID;
    m.dwErrorCode   = E_Marriage_Accept;
    if (m_pSession->IsConnect())
        m_pSession->Send(&m);

    return 0;
}