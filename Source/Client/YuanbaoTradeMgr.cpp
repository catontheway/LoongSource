#include "StdAfx.h"
#include "YuanbaoTradeMgr.h"
#include "RoleMgr.h"

YuanbaoTradeMgr::YuanbaoTradeMgr(void) : m_Trunk(this)
{
    m_mapSellListByPrice.clear();
    m_mapBuyListByPrice.clear();
    m_mapOrderByID.clear();
}

YuanbaoTradeMgr::~YuanbaoTradeMgr(void)
{
    ClearSellPriceList();
    ClearBuyPriceList();
    ClearOrderList();
}

VOID YuanbaoTradeMgr::Init()
{
    // ��ȡ����
    m_pCmdMgr       =   TObjRef<NetCmdMgr>();
    m_pUtil         =   TObjRef<Util>();
    m_pFrameMgr     =   TObjRef<GameFrameMgr>();
    m_pSession      =   TObjRef<NetSession>();
    m_pCurrency     =   CurrencyMgr::Inst();

    ClearSellPriceList();
    ClearBuyPriceList();
    ClearOrderList();
    ClearYBAccount();

    // ע��������Ϣ�ص�����
    m_pCmdMgr->Register("NS_SynAccoutYB",         (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynAccoutYB),           _T("OnNS_SynAccoutYB"));
    m_pCmdMgr->Register("NS_SynAccoutSilver",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynAccoutSilver),       _T("OnNS_SynAccoutSilver"));
    m_pCmdMgr->Register("NS_SynAccout",           (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynAccout),             _T("OnNS_SynAccout"));
    m_pCmdMgr->Register("NS_SaveYB2Account",      (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SaveYB2Account),        _T("OnNS_SaveYB2Account"));
    m_pCmdMgr->Register("NS_DepositAccountYB",    (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_DepositAccountYB),      _T("OnNS_DepositAccountYB"));
    m_pCmdMgr->Register("NS_SaveSilver2Account",  (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SaveSilver2Account),    _T("OnNS_SaveSilver2Account"));
    m_pCmdMgr->Register("NS_DepositAccountSilver",(NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_DepositAccountSilver),  _T("OnNS_DepositAccountSilver"));
    m_pCmdMgr->Register("NS_GetSellPriceList",    (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetSellPriceList),      _T("OnNS_GetSellPriceList"));
    m_pCmdMgr->Register("NS_GetBuyPriceList",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetBuyPriceList),       _T("OnNS_GetBuyPriceList"));
    m_pCmdMgr->Register("NS_GetYBAccount",        (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetYBAccount),          _T("OnNS_GetYBAccount"));
    m_pCmdMgr->Register("NS_SynSellPriceList",    (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynSellPriceList),      _T("OnNS_SynSellPriceList"));
    m_pCmdMgr->Register("NS_SynBuyPriceList",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynBuyPriceList),       _T("OnNS_SynBuyPriceList"));
    m_pCmdMgr->Register("NS_SubmitSellOrder",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SubmitSellOrder),       _T("OnNS_SubmitSellOrder"));
    m_pCmdMgr->Register("NS_SubmitBuyOrder",      (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SubmitBuyOrder),        _T("OnNS_SubmitBuyOrder"));
    m_pCmdMgr->Register("NS_GetYuanbaoOrder",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetYuanbaoOrder),       _T("OnNS_GetYuanbaoOrder"));
    m_pCmdMgr->Register("NS_DeleteOrder",         (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_DeleteOrder),           _T("OnNS_DeleteOrder"));
}

VOID YuanbaoTradeMgr::Destroy()
{
    // ע��������Ϣ�ص�����
    m_pCmdMgr->UnRegister("NS_SynAccoutYB",         (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynAccoutYB));
    m_pCmdMgr->UnRegister("NS_SynAccoutSilver",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynAccoutSilver));
    m_pCmdMgr->UnRegister("NS_SynAccout",           (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynAccout));
    m_pCmdMgr->UnRegister("NS_SaveYB2Account",      (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SaveYB2Account));
    m_pCmdMgr->UnRegister("NS_DepositAccountYB",    (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_DepositAccountYB));
    m_pCmdMgr->UnRegister("NS_SaveSilver2Account",  (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SaveSilver2Account));
    m_pCmdMgr->UnRegister("NS_DepositAccountSilver",(NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_DepositAccountSilver));
    m_pCmdMgr->UnRegister("NS_GetSellPriceList",    (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetSellPriceList));
    m_pCmdMgr->UnRegister("NS_GetBuyPriceList",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetBuyPriceList));
    m_pCmdMgr->UnRegister("NS_GetYBAccount",        (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetYBAccount));
    m_pCmdMgr->UnRegister("NS_SynSellPriceList",    (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynSellPriceList));
    m_pCmdMgr->UnRegister("NS_SynBuyPriceList",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SynBuyPriceList));
    m_pCmdMgr->UnRegister("NS_SubmitSellOrder",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SubmitSellOrder));
    m_pCmdMgr->UnRegister("NS_SubmitBuyOrder",      (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_SubmitBuyOrder));
    m_pCmdMgr->UnRegister("NS_GetYuanbaoOrder",     (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_GetYuanbaoOrder));
    m_pCmdMgr->UnRegister("NS_DeleteOrder",         (NETMSGPROC)m_Trunk.sfp2(&YuanbaoTradeMgr::OnNS_DeleteOrder));

    ClearSellPriceList();
    ClearBuyPriceList();
    ClearOrderList();
    ClearYBAccount();
}

YuanbaoTradeMgr g_YuanbaoTradeMgr;
YuanbaoTradeMgr* YuanbaoTradeMgr::Inst()
{
    return &g_YuanbaoTradeMgr;
}

VOID YuanbaoTradeMgr::UpdateSellPriceList( tagYBPriceList* pList )
{
    if (P_VALID(pList))
    {
        _MapPrice::iterator it = m_mapSellListByPrice.find(pList->nPrice);
        if (it != m_mapSellListByPrice.end())
        {
            // ����У���ɾ��ԭ����
            SAFE_DELETE(it->second);
            m_mapSellListByPrice.erase(it);
        }
        
        // ��������Ϊ0�Ĳ�����
        if (pList->nNum != 0)
            m_mapSellListByPrice.insert(make_pair(pList->nPrice, pList));
    }
}

VOID YuanbaoTradeMgr::ClearSellPriceList()
{
    // �ͷſռ�
    for (_MapPrice::iterator it = m_mapSellListByPrice.begin();
        it != m_mapSellListByPrice.end(); ++it)
        SAFE_DELETE(it->second);

    // ��ջ���
    m_mapSellListByPrice.clear();
}

VOID YuanbaoTradeMgr::UpdateBuyPriceList( tagYBPriceList* pList )
{
    if (P_VALID(pList))
    {
        _MapPrice::iterator it = m_mapBuyListByPrice.find(pList->nPrice);
        if (it != m_mapBuyListByPrice.end())
        {
            // ����У���ɾ��ԭ����
            SAFE_DELETE(it->second);
            m_mapBuyListByPrice.erase(it);
        }

        // ��������Ϊ0�Ĳ�����
        if (pList->nNum != 0)
            m_mapBuyListByPrice.insert(make_pair(pList->nPrice, pList));
    }
}

VOID YuanbaoTradeMgr::ClearBuyPriceList()
{
    // �ͷſռ�
    for (_MapPrice::iterator it = m_mapBuyListByPrice.begin();
        it != m_mapBuyListByPrice.end(); ++it)
        SAFE_DELETE(it->second);

    // ��ջ���
    m_mapBuyListByPrice.clear();
}

VOID YuanbaoTradeMgr::InsertOrderList( tagYuanBaoOrder* pOrder )
{
    if (P_VALID(pOrder))
    {
        _MapOrder::_Pairib ret = m_mapOrderByID.insert(make_pair(pOrder->dwID, pOrder));
        if (!ret.second)
        {
            // ����������ص��²���ʧ�ܣ�����ԭ���Ķ���
            SAFE_DELETE(pOrder);
        }
    }
}

VOID YuanbaoTradeMgr::ClearOrderList()
{
    // �ͷſռ�
    for (_MapOrder::iterator it = m_mapOrderByID.begin(); it != m_mapOrderByID.end(); ++it)
        SAFE_DELETE(it->second);

    // ��ջ���
    m_mapOrderByID.clear();
}

VOID YuanbaoTradeMgr::BeginNC_SaveYB2Account( INT nNum )
{
    tagNC_SaveYB2Account e;
    e.nNum      =   nNum;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_DepositAccountYB( INT nNum )
{
    tagNC_DepositAccountYB e;
    e.nNum      =   nNum;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_SaveSilver2Account( INT64 nNum )
{
    tagNC_SaveSilver2Account e;
    e.nNum      =   nNum;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_DepositAccountSilver( INT64 nNum )
{
    tagNC_DepositAccountSilver e;
    e.nNum      =   nNum;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_SynYBTradeInfo()
{
     tagNC_SynYBTradeInfo e;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_SubmitSellOrder( INT nNum, INT64 nPrice )
{
    tagNC_SubmitSellOrder e;
    e.nNum      =   nNum;
    e.nPrice    =   (INT)nPrice;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_SubmitBuyOrder( INT nNum, INT64 nPrice )
{
    tagNC_SubmitBuyOrder e;
    e.nNum      =   nNum;
    e.nPrice    =   (INT)nPrice;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_GetYuanbaoOrder()
{
    tagNC_GetYuanbaoOrder e;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

VOID YuanbaoTradeMgr::BeginNC_DeleteOrder( DWORD dwOrderID, EYBOTYPE eYBOType )
{
    tagNC_DeleteOrder e;
    e.dwOrderID     =   dwOrderID;
    e.eYBOType      =   eYBOType;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

DWORD YuanbaoTradeMgr::OnNS_SaveYB2Account( tagNS_SaveYB2Account* msg, DWORD )
{
    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SaveYB2Account;
    e.bRet      =   E_Success == msg->dwErrorCode;
    e.dwErrCode =   msg->dwErrorCode;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_DepositAccountYB( tagNS_DepositAccountYB* msg, DWORD )
{
    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_DepositAccountYB;
    e.bRet      =   E_Success == msg->dwErrorCode;
    e.dwErrCode =   msg->dwErrorCode;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SaveSilver2Account( tagNS_SaveSilver2Account* msg, DWORD )
{
    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SaveSilver2Account;
    e.bRet      =   E_Success == msg->dwErrorCode;
    e.dwErrCode =   msg->dwErrorCode;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_DepositAccountSilver( tagNS_DepositAccountSilver* msg, DWORD )
{
    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_DepositAccountSilver;
    e.bRet      =   E_Success == msg->dwErrorCode;
    e.dwErrCode =   msg->dwErrorCode;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_GetSellPriceList( tagNS_GetSellPriceList* msg, DWORD )
{
    // ���ԭ�л���
    ClearSellPriceList();

    for (INT i = 0; i != msg->nListNum; ++i)
    {
        // ����䳤�б�
        tagYBPriceList* pPriceList = new tagYBPriceList();
        *pPriceList = msg->YBPriceList[i];

        // ���뻺��
        UpdateSellPriceList(pPriceList);
    }

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_GetSellPriceList;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_GetBuyPriceList( tagNS_GetBuyPriceList* msg, DWORD )
{
    // ���ԭ�л���
    ClearBuyPriceList();

    for (INT i = 0; i != msg->nListNum; ++i)
    {
        // ����䳤�б�
        tagYBPriceList* pPriceList = new tagYBPriceList();
        *pPriceList = msg->YBPriceList[i];

        // ���뻺��
        UpdateBuyPriceList(pPriceList);
    }

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_GetBuyPriceList;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_GetYBAccount( tagNS_GetYBAccount* msg, DWORD )
{
    ASSERT(msg->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID());
    m_YBAccount.dwRoleID        =   msg->dwRoleID;
    m_YBAccount.nYuanBao        =   msg->nYuanBao;
    m_YBAccount.nMoney          =   msg->nSilver;
    m_YBAccount.bHasBuyOrder    =   msg->bBuyOrder;
    m_YBAccount.bHasSellOrder   =   msg->bSellOrder;

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_GetYBAccount;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SynSellPriceList( tagNS_SynSellPriceList* msg,DWORD )
{
    tagYBPriceList* pPriceList = new tagYBPriceList();
    pPriceList->nNum    =   msg->nNum;
    pPriceList->nPrice  =   msg->nPrice;
    UpdateSellPriceList(pPriceList);

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SynSellPriceList;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SynBuyPriceList( tagNS_SynBuyPriceList* msg, DWORD )
{
    tagYBPriceList* pPriceList = new tagYBPriceList();
    pPriceList->nNum    =   msg->nNum;
    pPriceList->nPrice  =   msg->nPrice;
    UpdateBuyPriceList(pPriceList);

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SynBuyPriceList;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SubmitSellOrder( tagNS_SubmitSellOrder* msg, DWORD )
{
    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SubmitSellOrder;
    e.bRet      =   E_Success == msg->dwErrorCode;
    e.dwErrCode =   msg->dwErrorCode;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SubmitBuyOrder( tagNS_SubmitBuyOrder* msg, DWORD )
{
    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SubmitBuyOrder;
    e.bRet      =   E_Success == msg->dwErrorCode;
    e.dwErrCode =   msg->dwErrorCode;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_GetYuanbaoOrder( tagNS_GetYuanbaoOrder* msg, DWORD )
{
    // ����б�
    ClearOrderList();

    for (INT i = 0; i != msg->nNum; ++i)
    {
        // ����䳤�б�
        tagYuanBaoOrder* pOrder = new tagYuanBaoOrder();// *(msg->Orders + i * sizeof(tagYuanBaoOrder) ) );
        *pOrder     =   msg->Orders[i];
        //memcpy(pOrder, msg->Orders[0] + i * sizeof(tagYuanBaoOrder), sizeof(tagYuanBaoOrder));

        // ���뻺��
        InsertOrderList(pOrder);
    }

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_GetYuanbaoOrder;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_DeleteOrder( tagNS_DeleteOrder* msg, DWORD )
{
    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_DeleteOrder;
    e.bRet      =   E_Success == msg->dwErrorCode;
    e.dwErrCode =   msg->dwErrorCode;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SynAccoutYB( tagNS_SynAccoutYB* msg, DWORD )
{
    m_YBAccount.nYuanBao        =   msg->nYuanBao;

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SynAccoutYB;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SynAccoutSilver( tagNS_SynAccoutSilver* msg, DWORD )
{
    m_YBAccount.nMoney          =   msg->nSilver;

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SynAccoutSilver;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

DWORD YuanbaoTradeMgr::OnNS_SynAccout( tagNS_SynAccout* msg, DWORD )
{
    m_YBAccount.nMoney          =   msg->nSilver;
    m_YBAccount.nYuanBao        =   msg->nYuanBao;

    tagYBReturnEvnt e(_T("tagYBReturnEvnt"), NULL);
    e.eType     =   EYBR_SynAccout;
    m_pFrameMgr->SendEvent(&e);

    return 0;
}

void YuanbaoTradeMgr::ClearYBAccount()
{
    memset(&m_YBAccount, 0, sizeof(m_YBAccount));
}