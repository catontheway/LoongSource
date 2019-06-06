#include "StdAfx.h"
#include "PetTradeFrame.h"
#include "PetTradeUIFrame.h"
#include "CombatSysUtil.h"
#include "CombatEvent.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "PlayerNameTab.h"
#include "AudioSys.h"
#include "..\WorldDefine\msg_pet_exchange.h"

const DWORD MAX_PET_EXCHANGE_REQTIME	=	60 * 1000;	// ���ȴ�����ʱ�䣬��λ������

PetTradeFrame::PetTradeFrame(void) :
    m_Trunk(this),
    m_dwTargetID(GT_INVALID),
    m_dwReqTime(GT_INVALID)
{
}

PetTradeFrame::~PetTradeFrame(void)
{
}

BOOL PetTradeFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    // ע��������Ϣ
    m_pCmdMgr->Register("NS_PetExchangeReq",    (NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeReq),	_T("NS_PetExchangeReq"));
    m_pCmdMgr->Register("NS_PetExchangeReqRes",	(NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeReqRes),	_T("NS_PetExchangeReqRes"));
    m_pCmdMgr->Register("NS_PetExchangeCancel",	(NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeCancel),	_T("NS_PetExchangeCancel"));
    m_pCmdMgr->Register("NS_PetExchangeFinish",	(NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeFinish),	_T("NS_PetExchangeFinish"));

    // ע����Ϸ�¼�
    m_pMgr->RegisterEventHandle(_T("TargetEvent_ReqPetTrade"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PetTradeFrame::OnEventReqTrade));
    m_pMgr->RegisterEventHandle(_T("MsgBox_PetExchangeReqCheck"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PetTradeFrame::OnEventReqReply));

    return bRet;
}

BOOL PetTradeFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    // ע��������Ϣ
    m_pCmdMgr->UnRegister("NS_PetExchangeReq",      (NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeReq));
    m_pCmdMgr->UnRegister("NS_PetExchangeReqRes",	(NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeReqRes));
    m_pCmdMgr->UnRegister("NS_PetExchangeCancel",	(NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeCancel));
    m_pCmdMgr->UnRegister("NS_PetExchangeFinish",	(NETMSGPROC)m_Trunk.sfp2(&PetTradeFrame::OnNetPetExchangeFinish));

    // ע����Ϸ�¼�
    m_pMgr->UnRegisterEventHandler(_T("TargetEvent_ReqPetTrade"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PetTradeFrame::OnEventReqTrade));
    m_pMgr->UnRegisterEventHandler(_T("MsgBox_PetExchangeReqCheck"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PetTradeFrame::OnEventReqReply));

    return bRet;
}

VOID PetTradeFrame::Update()
{
    GameFrame::Update();
    if(GT_VALID(m_dwReqTime))
    {
        m_dwReqTime -= Kernel::Inst()->GetDeltaTimeDW();
        if(m_dwReqTime > 0x7FFFFFFF)// ���m_dwReqTimeΪ��ֵ��ر������״���
        {
            if(P_VALID(GetObj("PetExchangeReq")))
            {
                KillObj("PetExchangeReq");
                tagNC_PetExchangeReqRes cmd;
                cmd.dwDstID		= m_dwTargetID;
                cmd.dwErrorCode = E_Pet_Exchange_NotAnswer;
                m_pSession->Send(&cmd);
                m_dwReqTime = GT_INVALID;
            }
        }
    }
}

VOID PetTradeFrame::ShowExchangeErrorMsg( DWORD dwErrorCode, BOOL bNotMine /*= FALSE*/ )
{
    if (dwErrorCode > E_Pet_Exchange_Accept && dwErrorCode < E_Pet_Exchange_End )
    {
        TCHAR szTmp[X_SHORT_NAME] = {0};
        _sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("PetExchangeErrorMsg%d"), dwErrorCode);
        if(bNotMine) //��������Լ�����Ҫ���϶Է���ң���ʾ����
            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetExchange_TargetError")], g_StrTable[szTmp]);
        else
            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);
    }
}

DWORD PetTradeFrame::OnNetPetExchangeReq( tagNS_PetExchangeReq* pNetCmd, DWORD )
{
    m_dwTargetID = pNetCmd->dwSrcID;

    // ��������δ��ʼ������ܾ�
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLp) || !pLp->IsAttributeInited())
    {
        m_dwTargetID = pNetCmd->dwSrcID;

        tagNC_PetExchangeReqRes m;
        m.dwDstID       = m_dwTargetID;
        m.dwErrorCode   = E_Pet_Exchange_NotAnswer;
        m_pSession->Send(&m);

        return 0;
    }

    if (pNetCmd->dwSrcID != RoleMgr::Inst()->GetLocalPlayerID())
    {
        m_dwTargetID = pNetCmd->dwSrcID;
        m_dwReqTime	 = MAX_PET_EXCHANGE_REQTIME;

        if(P_VALID(GetObj("PetExchangeReq")))
        {
            KillObj("PetExchangeReq");
        }
        TCHAR szTemp[X_LONG_NAME] = {0};
        _sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("PetExchange_ReqCheck")], PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwSrcID));
        CreateObj("PetExchangeReq", "MsgBox");
        TObjRef<MsgBox>("PetExchangeReq")->Init(_T(""), szTemp, _T("MsgBox_PetExchangeReqCheck"));
    }
    return 0;
}

DWORD PetTradeFrame::OnNetPetExchangeReqRes( tagNS_PetExchangeReqRes* pNetCmd, DWORD )
{
    DWORD dwLPID = RoleMgr::Inst()->GetLocalPlayerID();
    if (pNetCmd->dwSrcID == dwLPID)
    {
        if (pNetCmd->dwErrorCode == E_Pet_Exchange_Accept)
        {
            OpenPetTrade(pNetCmd->dwDstID);
        }
        else
        {
            ShowExchangeErrorMsg(pNetCmd->dwErrorCode);
        }
    }
    else if (pNetCmd->dwDstID == dwLPID)
    {
        if (pNetCmd->dwErrorCode == E_Pet_Exchange_Accept)
        {
            OpenPetTrade(pNetCmd->dwSrcID);
        }
        else
        {
            ShowExchangeErrorMsg(pNetCmd->dwErrorCode);
        }
    }
    return 0;
}

DWORD PetTradeFrame::OnNetPetExchangeCancel( tagNS_PetExchangeCancel* pNetCmd, DWORD )
{
    // ��ʾ��ʾ��Ϣ
    CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetExchange_Cancel")]);

    // �رս��״���
    ClosePetTrade();
    return 0;
}

DWORD PetTradeFrame::OnNetPetExchangeFinish( tagNS_PetExchangeFinish* pNetCmd, DWORD )
{
    if(E_Success == pNetCmd->dwErrorCode)
    {
        // ��ʾ��ʾ��Ϣ
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetExchange_Finish")]);

        // ���Ž��������Ч
        TObjRef<AudioSys>()->Play2DSound(_T("trade"), SOUND_NORMAL);
    }
    else// ��ʾ������ʾ
    {
        // ��ʾ����ʧ��
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PetExchange_Failed")]);
        ShowExchangeErrorMsg(pNetCmd->dwErrorCode, pNetCmd->dwFailedRoleID != RoleMgr::Inst()->GetLocalPlayerID());
    }

    // ����ǿռ䲻�㣬�򲻹رս��״��ڣ����������Ҫ�ر�
    if(E_Pet_Exchange_NotEnough_Space == pNetCmd->dwErrorCode)
    {
        UnlockPetTrade();
    }
    else
    {
        ClosePetTrade();
    }
    return 0;
}

DWORD PetTradeFrame::OnEventReqTrade( tagFollowRoleEvent* pGameEvent )
{
    if(pGameEvent->dwTargetRoleID != RoleMgr::Inst()->GetLocalPlayerID())
    {
        if(RoleMgr::Inst()->IsOutValidDist(pGameEvent->dwTargetRoleID))
        {
            ShowExchangeErrorMsg(E_Pet_Exchange_OutOfRange);
            return 0;
        }

        tagNC_PetExchangeReq m;
        m.dwDstID = pGameEvent->dwTargetRoleID;
        m_pSession->Send(&m);
    }
    return 0;
}

DWORD PetTradeFrame::OnEventReqReply( tagMsgBoxEvent* pGameEvent )
{
    tagNC_PetExchangeReqRes m;
    m.dwDstID = m_dwTargetID;
    if(MBF_OK == pGameEvent->eResult)
    {
        m.dwErrorCode = E_Pet_Exchange_Accept;
        if(RoleMgr::Inst()->IsOutValidDist(m_dwTargetID))
        {
            m.dwErrorCode = E_Pet_Exchange_OutOfRange;
            ShowExchangeErrorMsg(E_Pet_Exchange_OutOfRange);
        }
    }
    else if(MBF_Cancel == pGameEvent->eResult)
    {
        m.dwErrorCode = E_Pet_Exchange_Refuse;
    }
    else
    {
        m.dwErrorCode = E_Pet_Exchange_NotAnswer;
    }
    m_pSession->Send(&m);
    m_dwReqTime = GT_INVALID;
    return 0;
}

VOID PetTradeFrame::OpenPetTrade( DWORD dwTargetID )
{
    // �������׽���
    PetTradeUIFrame* pFrame = (PetTradeUIFrame*)m_pMgr->GetFrame(_T("PetTradeUIFrame"));
    if (!P_VALID(pFrame))
    {
        pFrame = (PetTradeUIFrame*)m_pMgr->CreateFrame(m_strName.c_str(), _T("PetTradeUIFrame"), _T("PetTradeUIFrame"), 0);
    }
    
    if (P_VALID(pFrame))
    {
        pFrame->SetTargetPlayerID(dwTargetID);
    }
}

VOID PetTradeFrame::ClosePetTrade()
{
    // ɾ�����׽���
    GameFrame* pFrame = m_pMgr->GetFrame(_T("PetTradeUIFrame"));
    if (P_VALID(pFrame))
        m_pMgr->AddToDestroyList(pFrame);

    // ɾ��������ʾ��
    if(P_VALID(GetObj("PetExchangeReq")))
    {
        KillObj("PetExchangeReq");
    }
}

VOID PetTradeFrame::UnlockPetTrade()
{
    PetTradeUIFrame* pFrame = (PetTradeUIFrame*)m_pMgr->GetFrame(_T("PetTradeUIFrame"));
    if (P_VALID(pFrame))
    {
        pFrame->LockDstControl(false);
        pFrame->LockSrcControl(false);
    }
}